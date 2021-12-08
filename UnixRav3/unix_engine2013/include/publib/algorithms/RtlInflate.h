#ifndef __RTL_INFLATE_H__
#define __RTL_INFLATE_H__


#define RS_STATUS_SUCCESS					((INT32)0x00000000L)
#define RS_STATUS_INVALID_PARAMETER			((INT32)0xC000000DL)
#define RS_STATUS_UNSUPPORTED_COMPRESSION	((INT32)0xC000025FL)
#define RS_STATUS_BAD_COMPRESSION_BUFFER	((INT32)0xC0000242L)
#define RS_STATUS_BUFFER_TOO_SMALL			((INT32)0xC0000023L)

/* decompress a single LZNT1 chunk */
static UCHAR *lznt1_decompress_chunk(UCHAR *dst, ULONG dst_size, UCHAR *src, ULONG src_size)
{
	UCHAR *src_cur = src, *src_end = src + src_size;
	UCHAR *dst_cur = dst, *dst_end = dst + dst_size;
	ULONG displacement_bits, length_bits;
	ULONG code_displacement, code_length;
	WORD flags, code;

	while (src_cur < src_end && dst_cur < dst_end)
	{
		flags = 0x8000 | *src_cur++;
		while ((flags & 0xff00) && src_cur < src_end)
		{
			if (flags & 1)
			{
				/* backwards reference */
				if (src_cur + sizeof(WORD) > src_end)
					return NULL;

				code = *(WORD *)src_cur;
				src_cur += sizeof(WORD);

				/* find length / displacement bits */
				for (displacement_bits = 12; displacement_bits > 4; displacement_bits--)
					if ((1 << (displacement_bits - 1)) < dst_cur - dst) break;

				length_bits       = 16 - displacement_bits;
				code_length       = (code & ((1 << length_bits) - 1)) + 3;
				code_displacement = (code >> length_bits) + 1;

				if (dst_cur < dst + code_displacement)
					return NULL;

				/* copy bytes of chunk - we can't use memcpy() since source and dest can
				* be overlapping, and the same bytes can be repeated over and over again */
				while (code_length--)
				{
					if (dst_cur >= dst_end) return dst_cur;
					*dst_cur = *(dst_cur - code_displacement);
					dst_cur++;
				}
			}
			else
			{
				/* uncompressed data */
				if (dst_cur >= dst_end) return dst_cur;
				*dst_cur++ = *src_cur++;
			}
			flags >>= 1;
		}
	}

	return dst_cur;
}

/* decompress data encoded with LZNT1 */
static NTSTATUS lznt1_decompress(UCHAR *dst, ULONG dst_size, UCHAR *src, ULONG src_size,
	ULONG offset, ULONG *final_size, UCHAR *workspace)
{
	UCHAR *src_cur = src, *src_end = src + src_size;
	UCHAR *dst_cur = dst, *dst_end = dst + dst_size;
	ULONG chunk_size, block_size;
	WORD chunk_header;
	UCHAR *ptr;

	if (src_cur + sizeof(WORD) > src_end)
		return RS_STATUS_BAD_COMPRESSION_BUFFER;

	/* skip over chunks with a distance >= 0x1000 to the destination offset */
	while (offset >= 0x1000 && src_cur + sizeof(WORD) <= src_end)
	{
		chunk_header = *(WORD *)src_cur;
		src_cur += sizeof(WORD);
		if (!chunk_header) goto out;
		chunk_size = (chunk_header & 0xfff) + 1;

		if (src_cur + chunk_size > src_end)
			return RS_STATUS_BAD_COMPRESSION_BUFFER;

		src_cur += chunk_size;
		offset  -= 0x1000;
	}

	/* handle partially included chunk */
	if (offset && src_cur + sizeof(WORD) <= src_end)
	{
		chunk_header = *(WORD *)src_cur;
		src_cur += sizeof(WORD);
		if (!chunk_header) goto out;
		chunk_size = (chunk_header & 0xfff) + 1;

		if (src_cur + chunk_size > src_end)
			return RS_STATUS_BAD_COMPRESSION_BUFFER;

		if (dst_cur >= dst_end)
			goto out;

		if (chunk_header & 0x8000)
		{
			/* compressed chunk */
			if (!workspace) return STATUS_ACCESS_VIOLATION;
			ptr = lznt1_decompress_chunk(workspace, 0x1000, src_cur, chunk_size);
			if (!ptr) return RS_STATUS_BAD_COMPRESSION_BUFFER;
			if (ptr - workspace > offset)
			{
				block_size = min((ptr - workspace) - offset, dst_end - dst_cur);
				memcpy(dst_cur, workspace + offset, block_size);
				dst_cur += block_size;
			}
		}
		else
		{
			/* uncompressed chunk */
			if (chunk_size > offset)
			{
				block_size = min(chunk_size - offset, dst_end - dst_cur);
				memcpy(dst_cur, src_cur + offset, block_size);
				dst_cur += block_size;
			}
		}

		src_cur += chunk_size;
	}

	/* handle remaining chunks */
	while (src_cur + sizeof(WORD) <= src_end)
	{
		chunk_header = *(WORD *)src_cur;
		src_cur += sizeof(WORD);
		if (!chunk_header) goto out;
		chunk_size = (chunk_header & 0xfff) + 1;

		if (src_cur + chunk_size > src_end)
			return RS_STATUS_BAD_COMPRESSION_BUFFER;

		/* fill space with padding when the previous chunk was decompressed
		* to less than 4096 bytes. no padding is needed for the last chunk
		* or when the next chunk is truncated */
		block_size = ((dst_cur - dst) + offset) & 0xfff;
		if (block_size)
		{
			block_size = 0x1000 - block_size;
			if (dst_cur + block_size >= dst_end)
				goto out;
			memset(dst_cur, 0, block_size);
			dst_cur += block_size;
		}

		if (dst_cur >= dst_end)
			goto out;

		if (chunk_header & 0x8000)
		{
			/* compressed chunk */
			dst_cur = lznt1_decompress_chunk(dst_cur, dst_end - dst_cur, src_cur, chunk_size);
			if (!dst_cur) return RS_STATUS_BAD_COMPRESSION_BUFFER;
		}
		else
		{
			/* uncompressed chunk */
			block_size = min(chunk_size, dst_end - dst_cur);
			memcpy(dst_cur, src_cur, block_size);
			dst_cur += block_size;
		}

		src_cur += chunk_size;
	}

out:
	if (final_size)
		*final_size = dst_cur - dst;

	return RS_STATUS_SUCCESS;

}

/******************************************************************************
*  RtlDecompressFragment	[NTDLL.@]
*/
NTSTATUS WINAPI RtlDecompressFragment(USHORT format, PUCHAR uncompressed, ULONG uncompressed_size,
	PUCHAR compressed, ULONG compressed_size, ULONG offset,
	PULONG final_size, PVOID workspace)
{
	//   TRACE("0x%04x, %p, %u, %p, %u, %u, %p, %p\n", format, uncompressed,
	//       uncompressed_size, compressed, compressed_size, offset, final_size, workspace);

	switch (format & ~COMPRESSION_ENGINE_MAXIMUM)
	{
	case COMPRESSION_FORMAT_LZNT1:
		return lznt1_decompress(uncompressed, uncompressed_size, compressed,
			compressed_size, offset, final_size, (PUCHAR)workspace);

	case COMPRESSION_FORMAT_NONE:
	case COMPRESSION_FORMAT_DEFAULT:
		return RS_STATUS_INVALID_PARAMETER;

	default:
		//      FIXME("format %u not implemented\n", format);
		return RS_STATUS_UNSUPPORTED_COMPRESSION;
	}
}


/******************************************************************************
*  RtlDecompressBuffer		[NTDLL.@]
*/
NTSTATUS WINAPI RtlDecompressBuffer(USHORT format, PUCHAR uncompressed, ULONG uncompressed_size,
	PUCHAR compressed, ULONG compressed_size, PULONG final_size)
{
	//  TRACE("0x%04x, %p, %u, %p, %u, %p\n", format, uncompressed,
	//     uncompressed_size, compressed, compressed_size, final_size);

	return RtlDecompressFragment(format, uncompressed, uncompressed_size,
		compressed, compressed_size, 0, final_size, NULL);
}


/* compress data using LZNT1, currently only a stub */
static NTSTATUS lznt1_compress(UCHAR *src, ULONG src_size, UCHAR *dst, ULONG dst_size,
							   ULONG chunk_size, ULONG *final_size, UCHAR *workspace)
{
	UCHAR *src_cur = src, *src_end = src + src_size;
	UCHAR *dst_cur = dst, *dst_end = dst + dst_size;
	ULONG block_size;

	while (src_cur < src_end)
	{
		/* determine size of current chunk */
		block_size = min(0x1000, src_end - src_cur);
		if (dst_cur + sizeof(WORD) + block_size > dst_end)
			return RS_STATUS_BUFFER_TOO_SMALL;

		/* write (uncompressed) chunk header */
		*(WORD *)dst_cur = 0x3000 | (block_size - 1);
		dst_cur += sizeof(WORD);

		/* write chunk content */
		memcpy(dst_cur, src_cur, block_size);
		dst_cur += block_size;
		src_cur += block_size;
	}

	if (final_size)
		*final_size = dst_cur - dst;

	return RS_STATUS_SUCCESS;
}


/******************************************************************************
*  RtlCompressBuffer		[NTDLL.@]
*/
NTSTATUS WINAPI RtlCompressBuffer(USHORT format, PUCHAR uncompressed, ULONG uncompressed_size,
								  PUCHAR compressed, ULONG compressed_size, ULONG chunk_size,
								  PULONG final_size, PVOID workspace)
{
	//FIXME("0x%04x, %p, %u, %p, %u, %u, %p, %p: semi-stub\n", format, uncompressed,
	//	uncompressed_size, compressed, compressed_size, chunk_size, final_size, workspace);

	switch (format & ~COMPRESSION_ENGINE_MAXIMUM)
	{
	case COMPRESSION_FORMAT_LZNT1:
		return lznt1_compress(uncompressed, uncompressed_size, compressed,
			compressed_size, chunk_size, final_size, (UCHAR*)workspace);

	case COMPRESSION_FORMAT_NONE:
	case COMPRESSION_FORMAT_DEFAULT:
		return RS_STATUS_INVALID_PARAMETER;

	default:
		//FIXME("format %u not implemented\n", format);
		return RS_STATUS_UNSUPPORTED_COMPRESSION;
	}
}

#endif //__RTL_INFLATE_H__
