
#include <stddef.h> // for size_t

struct STDecodeInfo
{
	unsigned char *src_bak;
	unsigned int src_len;
	unsigned char *desc_bak;
	unsigned int desc_len;

	unsigned char *src;
	unsigned int src_remainsize;
	unsigned char *desc;
	unsigned int desc_remainsize;


	unsigned short remain_lens;

	// used for tables
	unsigned char table_1[0x1000];                //ok  // size 不知道， 但肯定不会超过一个字的长度
	unsigned short table_2[0x100];             //ok   // size ok

	unsigned char huffman_table_1[0x1fe];        //ok  // size ok
	unsigned short huffman_table_2[0x1000];     //ok   // size ok

	unsigned short table_x[0x1000];  //426ef8  //ok     //size 不知道，
	unsigned short table_y[0x1000];  //4246dc  //ok     //size 不知道，

	// used for bit stream
	unsigned short b16_curval;              //ok
	unsigned char b16_tmpbyte;              //ok
	unsigned char b16_remainbit;            //ok
};


static unsigned short getbits(STDecodeInfo *info, int bit)
{
	unsigned short ret = 0;
	while(bit)
	{
		if (bit > info->b16_remainbit)
		{
			if (info->b16_remainbit == 0)
			{
				if (info->src_remainsize == 0)
				{
					info->b16_tmpbyte = 0;
				}
				else
				{
					--info->src_remainsize;
					info->b16_tmpbyte = *(info->src++);
				}
				info->b16_remainbit = 8;
			}
			else
			{
				ret <<= info->b16_remainbit;
				ret |= (info->b16_tmpbyte << (8-info->b16_remainbit)) >> (8-info->b16_remainbit);
				bit -= info->b16_remainbit;
				info->b16_remainbit = 0;
			}
		}
		else
		{
			ret <<= bit;
			ret  |= (info->b16_tmpbyte << (8-info->b16_remainbit)) >> (8-bit);
			info->b16_remainbit -= bit;
			bit = 0;
		}
	}
	return ret;
}

static void shl_bits(STDecodeInfo *info, int bit)
{
	int count = bit / 16;
	int remain = bit % 16;
	unsigned short wTmp;

	if (count)
	{
		for (int i=0; i<count; ++i)
		{
			wTmp = getbits(info, 16);
		}
	}
	else
	{
		wTmp = info->b16_curval;
	}
	
	if (remain)
	{
		info->b16_curval = (wTmp << remain) | getbits(info, remain);
	}
	else
	{
		info->b16_curval = wTmp;
	}
}

static void init_table2(STDecodeInfo *info, int input_table_len, unsigned char *input_table, int bytelen, unsigned short *output_table, int output_table_len)
{
	unsigned short tmp_table1[0x10];
	unsigned short tmp_table2[0x11];
	unsigned short tmp;
	int remainbytelen = 0x10 - bytelen;

	int i, shlbit;

	// init tmp_table
	for (i=0; i<0x10; ++i) tmp_table1[i] = 0;

	for (i=0; i<input_table_len; ++i)
	{
		tmp = input_table[i];
		if (tmp-- == 0) continue;
		if (tmp >= sizeof(tmp_table1)) throw 1;
		++tmp_table1[tmp];
	}

	tmp_table2[0] = 0;
	for (i=0, shlbit=0xf; i<0x10; ++i)
	{
		tmp_table2[i+1] =tmp_table2[i] + (tmp_table1[i]<<(shlbit--));
	}
	if (0 != tmp_table2[i]) throw 1;


	for (i=0; i<bytelen; ++i)
	{
		tmp_table2[i] >>= remainbytelen;
		tmp_table1[i] = 1<<(bytelen-i-1);
	}
	for (i=bytelen; i<0x10; ++i)
	{
		tmp_table1[i] = 1<<(0x10-i-1);
	}

	// decode output_table
	tmp = tmp_table2[bytelen] >> remainbytelen;
	if (0 != tmp && (1<<bytelen) != tmp)
	{
		if ((1<<bytelen) > output_table_len) throw 1;
		for (int i=tmp; i<(1<<bytelen); ++i) output_table[i] = 0;
	}



	int offset = input_table_len;
	for (i=0; i<input_table_len; ++i)
	{
		int index = input_table[i];
		if (index-- == 0) continue;

		if (index >= (int)sizeof(tmp_table1) || index>= (int)sizeof(tmp_table2)) 
			throw 1;
		unsigned short tbl1val = tmp_table1[index];
		unsigned short tbl2val = tmp_table2[index];

		if (index >= bytelen)
		{
			unsigned short * addr;
			if ( (tbl2val>>remainbytelen) >= output_table_len) throw 1;
			addr = output_table + ( tbl2val >> remainbytelen );
			unsigned short testbit = 1<<(remainbytelen-1);
			int count = index-bytelen+1;
			while(count--) 
			{
				if (*addr == 0)
				{
					if (offset >= (int)sizeof(info->table_x)) throw 1;
					info->table_y[offset] = 0;
					info->table_x[offset] = 0;
					*addr = offset++;
				}
				if (tbl2val & testbit)
				{
					if (*addr >= sizeof(info->table_x)) 
						throw 1;
					addr = info->table_x+*addr;
				}
				else
				{
					if (*addr >= sizeof(info->table_y)) 
						throw 1;
					addr = info->table_y+*addr;
				}
				testbit >>= 1;
			}
			*addr = i;
		}
		else if (tbl1val>0)
		{
			if (tbl2val+tbl1val > output_table_len) throw 1;
			for (int j=0; j<tbl1val; ++j)
				output_table[tbl2val+j] = i;
		}

		tmp_table2[index] += tmp_table1[index];
	}
}


static void init_table(STDecodeInfo *info, int tablelen, int lenbits, int zeropos)
{
	unsigned short len = info->b16_curval >> (16-lenbits);         
	shl_bits(info, lenbits);

	if (len == 0)
	{
		unsigned short setvalue = info->b16_curval >> (16-lenbits);
		shl_bits(info, lenbits);

		for (int i=0; i<tablelen; ++i) 
			info->table_1[i] = 0;
		for (int i=0; i<0x100; ++i)
			info->table_2[i] = setvalue;
		return;
	}


	for (unsigned short i=0; i<len;)
	{
		unsigned char byt =  info->b16_curval >> 13;	//最高3位

		if (byt == 7)
		{
			unsigned short testbit = 0x1000;
			while(info->b16_curval & testbit)
			{
				testbit >>= 1;
				++byt;
			}
			shl_bits(info, byt - 3);
		}
		else
		{
			shl_bits(info, 3);
		}
		info->table_1[i++] = byt;

		if (i==zeropos)
		{
			unsigned short high_bit2 = (info->b16_curval>>14);
			shl_bits(info, 2);
			while(high_bit2-- > 0) info->table_1[i++] = 0;
		}
	}

	while(len < tablelen) info->table_1[len++] = 0;

	init_table2(info, tablelen, info->table_1, 0x8, info->table_2, sizeof(info->table_2));
}



static void init_huffman_table(STDecodeInfo *info)
{
	short table_lens = info->b16_curval >> 7; //最高9位
	shl_bits(info, 9);
	unsigned short si;

	if (0 == table_lens)
	{
		si = info->b16_curval >> 7;
		shl_bits(info, 9);
		for (int i=0; i<0x1fe; ++i)  info->huffman_table_1[i] = 0;
		for (int i=0; i<0x1000; ++i)  info->huffman_table_2[i] = si;  //si 双字节
		return;
	}

	if (table_lens < 0)		// 这里永远不会被执行吧? short>>7 怎么可能为负数?
	{
		for (int i= 0; i<0x1fe; ++i) info->huffman_table_1[i] = 0;
		init_table2(info, 0x1fe, info->huffman_table_1, 0xc, info->huffman_table_2, sizeof(info->huffman_table_2));
		return;
	}

	for (int i=0; i<table_lens; ++i)
	{
		if( (size_t)(info->b16_curval>>8) >= sizeof(info->table_2)) throw 1;
		si = info->table_2[info->b16_curval>>8];
		int testbit = 0x80;
		while (si >= 0x13)
		{
			if (info->b16_curval & testbit)
			{
				if (si >= sizeof(info->table_x)) throw 1;
				si = info->table_x[si];
			}
			else
			{
				if (si >= sizeof(info->table_y)) throw 1;
				si = info->table_y[si];
			}
			testbit >>= 1;
		}
		shl_bits(info, info->table_1[si]);

		if (si>2)
		{
			info->huffman_table_1[i] = si-2;
		}
		else 
		{
			unsigned short zerolen;
			switch(si)
			{
			case 0:
				zerolen = 1;
				break;
			case 1:
				zerolen = (info->b16_curval >> 0xc) + 0x03;
				shl_bits(info, 4);
				break;
			case 2:
				zerolen = (info->b16_curval >> 0x7) + 0x14;
				shl_bits(info, 9);
				break;
			default:
				zerolen = 0; //? 不会被执行， 为了c需要default而写
				break;
			}

			while(zerolen-- > 0) info->huffman_table_1[i++] = 0;
			--i;
		}
	}

	if (table_lens < 0x1fe)
		for (int i= table_lens; i<0x1fe; ++i) info->huffman_table_1[i] = 0;

	init_table2(info, 0x1fe, info->huffman_table_1, 0xc, info->huffman_table_2, sizeof(info->huffman_table_2));
}



static int get_a_char(STDecodeInfo *info)
{
	if (0 == info->remain_lens)
	{
		info->remain_lens = info->b16_curval;
		shl_bits(info, 16);
		init_table(info, 0x13, 0x5, 0x3);
		init_huffman_table(info);
		init_table(info, 0xe, 0x4, -1);
	}
	--info->remain_lens;

	if ((size_t)(info->b16_curval>>4) >= sizeof(info->huffman_table_2)) throw 1;
	unsigned short si = info->huffman_table_2[info->b16_curval>>4];
	int testbit = 0x8, loopcount = 0;
	while (si >= 0x1fe)
	{
		if (info->b16_curval & testbit)
		{
			if (si >= sizeof(info->table_x)) throw 1;
			si = info->table_x[si];
		}
		else
		{
			if (si >= sizeof(info->table_y)) throw 1;
			si = info->table_y[si];
		}
		testbit >>= 1;
		if (++loopcount > 100) throw 1;
	}
	shl_bits(info, info->huffman_table_1[si]);
	return si;
}


static int getoff(STDecodeInfo *info)
{
	if ((size_t)(info->b16_curval>>8) >= sizeof(info->table_2))  throw 1;
	unsigned short si = info->table_2[info->b16_curval>>8];
	int testbit = 0x80, loopcount = 0;
	while (si >= 0xe)
	{
		if (info->b16_curval & testbit)
		{
			if (si >= sizeof(info->table_x)) throw 1;
			si = info->table_x[si];
		}
		else
		{
			if (si >= sizeof(info->table_y)) throw 1;
			si = info->table_y[si];
		}
		testbit >>= 1;
		if (++loopcount > 100) throw 1;
	}
	shl_bits(info, info->table_1[si]);

	if (0 == si) return 0;

	--si;
	int ret =  (1 << si) + (info->b16_curval >> (0x10-si));
	shl_bits(info, si);
	return ret;
}


static bool decodeinternal(STDecodeInfo *info)
{
	for(; ;)
	{
		int ch = get_a_char(info);
		if (ch <= 0xff)
		{
			if (info->desc_remainsize == 0)  return true;
			--info->desc_remainsize;
			*(info->desc++) = ch;
			continue;
		}
	 
		int copylens = ch-0xfd;
		unsigned char * copypos = info->desc - getoff(info) - 1;
		if (copypos < info->desc_bak)
			return false;
		for(; copylens>0; copylens--)
		{
     			if (info->desc_remainsize == 0)  return true;
			--info->desc_remainsize;
			*(info->desc++) = *(copypos++);
		}
	}
}

bool lz_expressor_decode(unsigned char *desc, unsigned int desclen, const unsigned char *src, unsigned int srclen,  unsigned int * descprocess=0,  unsigned int * srcprocess = 0)
{
	STDecodeInfo info;
	info.desc_bak = info.desc = desc;
	info.desc_len = info.desc_remainsize = desclen;
	info.src_bak = info.src = (unsigned char *)src;
	info.src_len = info.src_remainsize = srclen;
	info.b16_curval = 0;
	info.b16_remainbit = 0;
	info.b16_tmpbyte = 0;
	info.remain_lens = 0;

	try
	{
		shl_bits(&info, 0x10);
		bool ret =  decodeinternal(&info);
		if (descprocess)
			*descprocess = info.desc_len - info.desc_remainsize;
		if (srcprocess)
			*srcprocess = info.src_len - info.src_remainsize;
		return ret;
	}
	catch (...)
	{
		if (descprocess)
			*descprocess = info.desc_len - info.desc_remainsize;
		if (srcprocess)
			*srcprocess = info.src_len - info.src_remainsize;
		return false;
	}
}
