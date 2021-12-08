#include "algorithms.h"

bool unknown_2_decode(const unsigned char *key, unsigned char *desc, const unsigned char *table_key_index, const unsigned char *table_bits,  unsigned long len)
{
#define UNKNOW_2_DECODE_GETBIT4TAG(val, pos)  ( ((val) >> ((7-(pos))*4)) & 0xf )
	unsigned char *desc_end = desc+len;
	while(desc+8 <=desc_end) //8字节一次解
	{
		for (int i=0x1f; i>=0; --i)
		{
			unsigned long tmp1, tmp2;
			unsigned char index = *(table_key_index+i);

			if (index > 0x1f) return false; //防止key表内存越界
			tmp1 = GETDWORD(desc) + GETDWORD(key + index*4);
			tmp2 = 0;
			for (int j=0; j<8; ++j)
			{
				tmp2 = (tmp2 << 4) | *(BYTE *)(table_bits + 0x10*(7-j) + UNKNOW_2_DECODE_GETBIT4TAG(tmp1, j));
			}
			tmp2 = (tmp2>>21) | (tmp2<<11);
			tmp2 ^= GETDWORD(desc+4);
			
			SETDWORD(desc+4, GETDWORD(desc));
			SETDWORD(desc, tmp2);
		}

		// swap
		unsigned long tmp = GETDWORD(desc+4);
		SETDWORD(desc+4, GETDWORD(desc));
		SETDWORD(desc, tmp);

		//next
		desc += 8;
	}
	return true;
#undef UNKNOW_2_DECODE_GETBIT4TAG
}
