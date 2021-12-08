#include "../../public/rame.h"
#include "algorithms.h"


static voidpf exe32pack_alloc (voidpf opaque, uInt items, uInt size)
{
	return RSNEW char[items*size];
};
static void exe32pack_free (voidpf opaque, voidpf address)
{
	delete [](char*)address;
};

bool  inline_exe32pack_inflate_decode(unsigned char *dest, size_t*destLen, unsigned char *source, size_t sourceLen)
{
	try{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return false;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return false;

    stream.zalloc = exe32pack_alloc;
    stream.zfree = exe32pack_free;

	err = inflateInit2(&stream, -DEF_WBITS);
    if (err != Z_OK) return false;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
		return false;
    }
    *destLen = stream.total_out;

    return Z_OK == inflateEnd(&stream);
	}
	catch (...) {
		throw;
	}
}
bool  exe32pack_inflate_decode(unsigned char *dest, size_t *destLen, unsigned char *source, size_t sourceLen)
{
	try
	{
		return inline_exe32pack_inflate_decode(dest, destLen, source, sourceLen);
	}
	catch (...)
	{
		return false;
	}
}




bool  exe32pack_lz_fast_decode(unsigned char * dest, size_t destlen,  unsigned char * src, size_t srclen, size_t *descprocess)
{
	unsigned char *destbak = dest;
	unsigned char *srcend = src+srclen;
	unsigned char *destend = dest+destlen;
	unsigned char c;

	if (src+1 > srcend) return false;
	if (1 == *src++)
	{
		if (srclen-1>destlen) return false;
		while(srclen-- >0) *dest++ = *src++;
		return true;
	}
	else
	{
		int bitcount = 0;
		unsigned short bittest = 0;
		while(src < srcend)
		{
			if (bitcount-- == 0)
			{
				bitcount = 15;
				if (src+2 > srcend) return false;
				bittest = *src++;
				bittest |= ((unsigned short)(*src++)) << 8;
			}

			if (bittest & 1)
			{
				if (src+2 > srcend) return false;

				unsigned char byt1 = *src++;
				unsigned char byt2 = *src++;
				unsigned char len = (byt1 & 0x0f) + 1;
				unsigned short off = ((byt1 & 0xf0) << 4) | byt2;
				
				if (dest-off<destbak || dest+len>destend) 
					return false;

				while(len-- >0)
				{
					c = *(dest-off);
					*dest++ = c;
				}
			}
			else
			{
				if(dest+1>destend || src+1>srcend) return false;
				*dest++ = *src++;
			}
			bittest >>= 1;
		}
		if (descprocess) *descprocess =(int) (dest - destbak);
		return true;
	}
}

