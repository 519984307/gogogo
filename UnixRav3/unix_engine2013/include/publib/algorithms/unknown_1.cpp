#include "algorithms.h"

#include <string.h>

#define GETVALUE1(p, v) ( ( ((*((char *)(p)) << 1) + (*((char *)(p)+5)) + (v)) << 4 ) \
					+ ( *((char *)(p)+1) << 3 ) \
					+ ( *((char *)(p)+2) << 2 ) \
					+ ( *((char *)(p)+3) << 1 ) \
					+ ( *((char *)(p)+4) ) )


static void get_mask(void * newmask, void * raw_data, int length)
{
	int off = 0;
	int bits = 8;
	int tmpchar = 0;
	for (int i=0; i<length; ++i)
	{
		if (8 == bits) 
		{
			tmpchar = *((char *)(raw_data) + (off++));
			bits = 0;
		}
		*((char *)(newmask) + i) = tmpchar & 1;
		tmpchar >>= 1;
		++bits;
	}
}

static void new_mask_by_index(void * newmask, void * oldmask, void *indexs, int length)
{
	char tmp[0x100];
	for (int i=0; i<length; ++i)
	{
		*(tmp + i) =  *((char *)oldmask + *((char *)(indexs)+i) -1);
	}
	memmove(newmask, tmp, length);
}

static void rol_mask(void *mask, int total_len, int len)
{
	char tmp[0x100];
	memmove(tmp, mask, len);
	memmove(mask, (char *)mask+len, total_len - len);
	memmove((char *)mask+(total_len-len), tmp, len);
}

static void xor_mask(void *mask1, void *mask2, int length)
{
	for (int i=0; i<length; ++i)
	{
		*((char *)(mask1) + i) ^= *((char *)(mask2) + i);
	}
}

static void calc_mask_1(void * newmask, void * oldmask, void * indexs)
{
	for (int i=0; i<0x8; ++i)
	{
		int off = GETVALUE1((char *)oldmask + i*6, i*4);
		get_mask((char *)newmask + i*4, (char *)indexs+off, 4);
	}
}

static void calc_mask(void *mask_main, void*mask_decode,  void *indexs_1, void *indexs_2, void *indexs_3)
{
	char temp[0x30];
	new_mask_by_index(temp, mask_main, indexs_1, 0x30);
	xor_mask(temp, mask_decode, 0x30);
	calc_mask_1(mask_main, temp, indexs_3);
	new_mask_by_index(mask_main, mask_main, indexs_2, 0x20);
}

static void decode_data_by_mask(void * desc, void * mask, int length)
{
	int off = 0;
	for (int i=0; i<length; i+=8)
	{
		*((char *)desc+(off++)) = 
		(*((char *)(mask)+i  )     ) | (*((char *)(mask)+i+1) << 1) |
		(*((char *)(mask)+i+2) << 2) | (*((char *)(mask)+i+3) << 3) |
		(*((char *)(mask)+i+4) << 4) | (*((char *)(mask)+i+5) << 5) |
		(*((char *)(mask)+i+6) << 6) | (*((char *)(mask)+i+7) << 7) ;
	}
}

static void init_table(void *mask, void *raw_data, void *rol_table, void *indexs_1, void *indexs_2)
{
	char tmptable[0x40];

	get_mask(tmptable, raw_data,  0x40);
	new_mask_by_index(tmptable, tmptable, indexs_1, 0x38);

	for (int i=0; i<0x10; ++i)
	{
		rol_mask(tmptable, 0x1c, ((char *)rol_table)[i]);
		rol_mask(tmptable+0x1c, 0x1c, ((char *)rol_table)[i]);
		new_mask_by_index((char *)(mask)+i*0x30, tmptable, indexs_2, 0x30);
	}
}



static bool inline_unknown_1_decode(STUnknown_1_param * param, unsigned char * desc, unsigned long desclen, unsigned char *src, unsigned long srclen)
{
	char table_decode[0x300];
	char table_main[0x40];

	char table_tmp[0x20];
	

	if (NULL == desc || NULL == src || desclen != srclen) return false;

	try
	{

		// init table
		init_table(table_decode, param->p9, param->p7, param->p5, param->p6);

		// decode loop
		for (unsigned long i = 0; i<desclen; i+=8)
		{
			get_mask(table_main, src+i, 0x40);
			new_mask_by_index(table_main, table_main, param->p1, 0x40);
			for (int j=0; j<0x10; ++j)
			{
				memcpy(table_tmp, table_main, 0x20);
				calc_mask(table_main, table_decode + ((0xf-j) * 0x30), param->p3, param->p4, param->p8);
				xor_mask(table_main, table_main + 0x20, 0x20);
				memcpy(table_main + 0x20, table_tmp, 0x20);
			}	
			new_mask_by_index(table_main, table_main, param->p2, 0x40);
			decode_data_by_mask(desc+i, table_main, 0x40);
		}

		return true;
	}

	catch(...)
	{
		throw;
	}
}


bool unknown_1_decode(STUnknown_1_param * param, unsigned char * desc, unsigned long desclen, unsigned char *src, unsigned long srclen)
{
	bool ret;
	try
	{
		ret = inline_unknown_1_decode(param, desc, desclen, src, srclen);
	}
	catch (...)
	{
		ret = false;
	}
	return ret;
}
