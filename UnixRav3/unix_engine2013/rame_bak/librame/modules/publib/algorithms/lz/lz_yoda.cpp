#include "../algorithms.h"

static inline int inline_lz_yoda_decode(unsigned char * dest, unsigned int destlen, const unsigned char * src, unsigned int srclen)
{
	try
	{
	const unsigned char *destbak = dest;
	const unsigned char *srcend = src+srclen;
	const unsigned char *destend = dest+destlen;

	unsigned char *copy_from;
	LONG copy_offset;
	LONG copy_lens;
	LONG left_lens;      //src to dest left copy lens

	unsigned char c;

	// edx = copy_from;
	// ecx = copy_lens;
	// eax = cpy_offset;

	// esi = src;
	// edi = dest;

	// debug
	int k=0;
	///////



	c = *(src++);


	if (c>=0x10)
	{
		copy_lens = c - 0xe;
	}
	else

	{
_al_jbe_11_start:

		////////debug
		if (dest - destbak > 0xf30)
		{
			k++;
		}
		////////debug

		if (c >= 0x10)
			goto _al_jae_10_start1;
		copy_lens = c;
		if (copy_lens == 0)
		{
			unsigned char tmp;
			while(0 == (tmp=*(src++)))
			{
				copy_lens += 0xff;
				if (src+copy_lens+0x12 > srcend) 
					return -4; //_error_read_ja_srclen
			}
			copy_lens += (tmp+0x15);
		}
		else
		{
			copy_lens += 6;
		}
	}
	goto _fuck_you;




	for(; ;)
	{
_al_jae_10_start1:

		////////debug
		if (dest - destbak > 0xf30)
		{
			k++;
		}
		////////debug

		if (c < 0x40)
		{
			if (c < 0x20 )
			{
				if (c < 0x10)	// 0x0 <= al <= 0xf
				{
					copy_lens = 2;
					copy_offset = ((unsigned char) *(src++)) * 4 + (c>>2);

					copy_from = dest-1-copy_offset;
					if (copy_from < destbak) return -6; 
					if (dest+copy_lens > destend) return -5;

					//memmove(dest, copy_from, 2);
					for(int i=0; i<2; ++i)
					{
						*(dest+i) = *(copy_from+i);
					}
					dest += 2;
					goto _copy_end_proc;
				}
				else		// 0x10 <= al <= 0x1f
				{

					copy_offset = (c&0x8) <<0xd; //  13bit, 3-7  ->  16-20
					copy_lens = c & 7;

					// get copy_lens
					if (copy_lens == 0)
					{
						unsigned char tmp;
						while (0 == (tmp=*(src++)))
						{
							copy_lens += 0xff;
							if (src+3 > srcend)
								return -4; //_error_read_ja_srclen
						};
						copy_lens += (tmp+0xc);
					}
					else
					{
						copy_lens += 5;
					}

					// get copy_from
					copy_offset += GETWORD(src); src+=2;
					copy_offset >>= 2;
					if (copy_offset == 0) // _decode_end
					{
						if (dest > destend) return -5;
						if (src > srcend) 
							return -4;
						if (src < srcend) return -8;
						if (copy_lens != 6) return 1;
						destlen = (int) (dest - destbak);
						return 0; //ok
					}
					copy_offset += 0x3fff; //fix
					copy_from = dest - 1 - copy_offset;
					goto _eax_jae_ebp;
				}

			}
			else			// c >= 0x20
			{
				copy_lens = c - 0x20;

				if (copy_lens == 0)
				{
					unsigned char tmp;
					while (0 == (tmp=*(src++)))
					{
						copy_lens += 0xff;
						if (src+3 >  srcend) 
							return -4;
					}
					copy_lens += (tmp+0x24);   
				}
				else
				{
					copy_lens += 5;
				}

				copy_offset = GETWORD(src); src+= 2;
				copy_offset >>= 2;
				copy_from = dest -1 - copy_offset;
				if (copy_offset < 3) 
					goto _eax_jb_ebp;
				else
					goto _eax_jae_ebp;
			}

		}
		else	// c >= 0x40 
		{
			copy_lens = (c>>5) + 4;
			copy_offset = ((c>>2) & 0x7) + ((unsigned char) *(src++)) * 8;
			copy_from = dest - 1 - copy_offset;
			if (copy_offset < 3)
				goto _eax_jb_ebp;
			else

				goto _eax_jae_ebp;
		}



_eax_jb_ebp:
		unsigned char * eax;
		if (copy_from < destbak) return -6;
		eax = dest + copy_lens -3;
		if (eax > destend) return -5;
		copy_lens -= 3;
		//memmove(dest, copy_from, copy_lens);
		for (int i=0; i<copy_lens; ++i)
		{
			*(dest+i)  = *(copy_from+i);
		}
		dest += copy_lens;
		goto _copy_end_proc;


_eax_jae_ebp:
		if (copy_from < destbak) return -6; 

		eax = dest + copy_lens -3;			// 其实这里的copy_lens就是需要减去3的， 这里， (copy_lens+3)/4, 表示了下对齐。
		copy_lens >>= 2;
		if (eax > destend) return -5; 

		for (int i=0; i<copy_lens; ++i)  //他是这样拷的， 如果直接用memmove， 会不会有问题呢， .
		{
			DWORD tmp = GETDWORD(copy_from);
			SETDWORD(dest, tmp);
			copy_from += 4;
			dest += 4;
		}
		dest = eax;


_copy_end_proc:
		left_lens = GETWORD(src-2) & 3;
		if (0 != left_lens)
		{
			if (dest+left_lens > destend) return -5;
			if (src+left_lens > srcend) 
				return -4;
			//memmove(dest, src, left_lens);				//其实这里他是拷贝了4字节， 然后dest和src都增加个left_lens,不知道这么处理是不是会影响后面？
			for(int i=0; i<left_lens; ++i)
			{
				*(dest+i) = *(src+i);
			}
			dest+=left_lens; src+= left_lens;
			c = *(src++);
			goto _al_jae_10_start1;
		}
		if (src > srcend)  
			return -4;
		c = *(src++);
		goto _al_jbe_11_start;
	}








	//////////////////////////
_fuck_you:

	if (dest+copy_lens-3 > destend) return -5; //_error_copy_end_ja_destend
	if (src+copy_lens-3 > srcend) 
		return -4; //_error_read_ja_srclen


	/////////////////////////////////////   src to dest start
	copy_offset = copy_lens;
	copy_offset ^= 3;
	copy_offset &= 3;

	copy_lens >>= 2;


	for(int i=0; i<copy_lens; ++i)
	{
		DWORD tmp;
		tmp = GETDWORD(src);
		SETDWORD(dest, tmp);
		src += 4;
		dest += 4;
	}

	src -= copy_offset;
	dest -= copy_offset;						// copy_offset is mod value
	////////////////////////////////////  src to dest end

	c = *(src++);

	if (c >= 0x10)  goto _al_jae_10_start1;

	if (dest+3 > destend) return -5; 

	copy_offset = (c>>2) + ((unsigned char) *(src++)) * 4;
	copy_from = dest-0x801-copy_offset;		

	if (copy_from < destbak) return -6; 

	DWORD tmp = GETDWORD(copy_from);
	SETDWORD(dest, tmp);
	dest += 3;
	goto _copy_end_proc;
	}

	catch (...) 
	{
		throw;
	}
}

int lz_yoda_decode(unsigned char * dest, unsigned int destlen, const unsigned char * src, unsigned int srclen)
{
	try
	{
		return inline_lz_yoda_decode(dest, destlen, src, srclen);
	}
	catch(...)
	{
		return -10;
	}
}
