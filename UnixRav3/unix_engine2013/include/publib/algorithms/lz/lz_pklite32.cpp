#include "../algorithms.h"


namespace LZ_PKLITE32 { ;



#ifndef  GETDWORD
#error "where is GETDWORD?"
#endif



#ifdef GETBYTE
#undef GETBYTE
#endif
#define GETBYTE(v)  (*(BYTE *)(v))

#ifdef IN
#undef IN
#endif
#define IN

#ifdef OUT
#undef OUT
#endif
#define OUT

#ifdef GUARD
#undef GUARD
#endif
#ifdef GUARDIDX
#undef GUARDIDX
#endif
#define GUARD(val, size)	{ if ((val)<0 || (val)>size) throw 1; } 
#define GUARDIDX(index, size, type_size) { if ( (index)<0 || ((index)+1)*(type_size)>(size) ) throw 1; }

static const int tbl_bit_mask[] = {0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff};
static const char tbl_idx_19[] = {0x10, 0x11, 0x12, 0x0, 0x8, 0x7, 0x9, 0x6,
				0xa, 0x5, 0xb, 0x4, 0xc, 0x3, 0xd, 0x2, 0xe, 0x1, 0xf,};

enum E_ERROR
{
	ERROR_TBL_BIT_MASK_BOUND = 0x10,

};


struct STDecodeInfo
{

	char * dest;
	char * src;
	int srclen;
	int dest_pos;
	int src_pos;
	int init_0_14;	//??

	int destlen;	//hjj add
};




typedef int (* FNCOPYFROMSRC) (char * dest, int copy_lens, STDecodeInfo *decodeinfo, int new_destlen);
typedef void (* FNCOPYTODEST) (char * src, int copy_lens, STDecodeInfo *decodeinfo, int new_srclen);

struct ST
{

	char * buf_size_fcc;	//count = 0x140		// char * ptr
	char * buf_add_580;		//count = 0x20		// char * ptr

	char * src_tmpbuf;
	char * src_tmpbuf_end;
	char * src_tmpbuf_end_sub_3;
	char * src_tmpbuf_cur;

	char * dest_tmpbuf;
	char * dest_tmpbuf_end;
	char * dest_tmpbuf_cur;

	WORD * buf_add_140;		//count = 0x100		// word * ptr
	WORD * buf_add_340;		//count = 0x120		// word * ptr  // use for transform_3_table_by_340

	WORD * buf_add_5a0;		//count = 0x100		// word * ptr	// use for GetCopyPos		
	WORD * buf_add_820;		//count = 0x100		// word * ptr	// use for GetCopyCount
	WORD * buf_add_7a0;		//count = 0x40  	// word * ptr	// use for GetCopyPos
	WORD * buf_add_a20;		//count = 0x240		// word * ptr	// use for GetCopyCount

	DWORD dwLastError;

	DWORD tmpdword1;
	int copy_lens_from_src;
	int copy_lens_from_dest;

    FNCOPYFROMSRC fnCopyFromSrc;
	FNCOPYTODEST  fnCopyToDest;
	STDecodeInfo * pDecodeInfo;

	int fnCopy1_fail_tickcount;
	int field_5C_equ0;				//allways equ 0
	BYTE shr_count;
//00000061                 db ? ; undefined
//00000062                 db ? ; undefined
//00000063                 db ? ; undefined
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;

	int field_80_ea0;

	int src_len;
	int dest_len;
	char * buf_size_fcc_orig;

	char * src;
	char * dest;
};

void * memcpy (void * dst, const void * src, size_t count, int dst_len, int src_len)
{
	if (0 == count) return NULL;
	if (dst_len <= 0) return NULL;
	if (src_len <= 0) return NULL;
	if (count > (size_t)dst_len || count > (size_t)src_len) throw 1;
	return ::memcpy(dst, src, count);
}


// 从压缩中的数据读到dest的buf里面去
static int CopyFromSrc(char * dest, int copy_lens, STDecodeInfo *info, int new_destlen)
{
	
	if (copy_lens+info->src_pos > info->srclen)
	{
		copy_lens = info->srclen - info->src_pos;
	}
	memcpy(dest, info->src+info->src_pos, copy_lens, new_destlen, info->srclen-info->src_pos);			//dest可能越界!
	info->src_pos +=copy_lens;
	return copy_lens;
}

// 把src的buf写到解压缩后的地方去
static void CopyToDest(char * src, int copy_lens, STDecodeInfo *info, int new_srclen)
{
	memcpy(info->dest+info->dest_pos, src, copy_lens, info->destlen-info->dest_pos, new_srclen);			//dest可能越界! src可能越界!
	info->dest_pos += copy_lens;
}


//从源中读st->copy_lens_from_src的字节到临时的src buf中去， 实际程序只读了一次， copy_lens_from_src大小就是srclen;
static void src_to_st_buf(ST *st)										
{
	if (0 != st->fnCopy1_fail_tickcount)
	{
		++st->fnCopy1_fail_tickcount;
		return;
	}
	int copylens = st->fnCopyFromSrc(st->src_tmpbuf, st->copy_lens_from_src, st->pDecodeInfo, st->src_tmpbuf_end-st->src_tmpbuf);
	if (0 == copylens) 
		++st->fnCopy1_fail_tickcount;
	st->src_tmpbuf_cur = st->src_tmpbuf;
	st->src_tmpbuf_end = st->src_tmpbuf+copylens;
	st->src_tmpbuf_end_sub_3 = st->src_tmpbuf+copylens-3;
}


//从源中读一个字节，  内部的实现是从src buf读， src buf空了的话则调用src_to_st_buf填下src_buf
static unsigned char GetSrcByte(ST *st)
{
   int ret = *(st->src_tmpbuf_cur++);
   if (st->src_tmpbuf_cur >= st->src_tmpbuf_end)
      src_to_st_buf(st);
   return ret;
}

//从源中忽略多少位
static void SkipBits(int count, ST * st)
{
	if (count <= st->shr_count) 
	{
		st->tmpdword1 >>= count;
		st->shr_count -= count;
		return;
	}

	st->tmpdword1 >>= st->shr_count;

	if (st->src_tmpbuf_cur < st->src_tmpbuf_end_sub_3)
	{
		st->tmpdword1 = ( (GETDWORD(st->src_tmpbuf_cur)<<8) | (st->tmpdword1) ) >>
			( count - st->shr_count );

		st->src_tmpbuf_cur += 3;
		st->shr_count =  st->shr_count - count + 0x18;
	}

	else if (st->src_tmpbuf_cur < st->src_tmpbuf_end)
	{
		st->tmpdword1 = ( (GETBYTE(st->src_tmpbuf_cur)<<8) | st->tmpdword1 ) >>  
			( count - st->shr_count );

		st->src_tmpbuf_cur += 1;
		st->shr_count = st->shr_count - count + 0x8;
	}

	else
	{
		src_to_st_buf(st);
		st->tmpdword1 = ( (GETDWORD(st->src_tmpbuf_cur)<<8) | (st->tmpdword1) ) >>
			( count - st->shr_count );

		st->src_tmpbuf_cur += 3;
		st->shr_count =  st->shr_count - count + 0x18;
	}
}

//从源中读1位
static int get_one_bit(ST * st)
{
    int ret = st->tmpdword1 & 1;
    SkipBits(1, st);
    return ret;
}

//从源中读多少位 (1<=count<=8)
static int GetBits(int count, ST * st)
{
	if (count < 1 || count > 8)
	{
		st->dwLastError = ERROR_TBL_BIT_MASK_BOUND;
		return 0;
	}
	int ret = st->tmpdword1 & tbl_bit_mask[count-1];
	SkipBits(count, st);
	return ret;
}

//得到从dest拷贝到dest的时候, 位置
static int GetCopyPos(ST *st)
{
	signed short index = st->buf_add_5a0[st->tmpdword1 & 0xff];
	int count;   

	// get shr_count
	if (index >= 0)
	{
		GUARDIDX(index, 0x20, 1);
		count = st->buf_add_580[index];
	}
	else
	{
		SkipBits(8, st);

		unsigned char testbit = (unsigned char )(st->tmpdword1 & 0xff);
		count = 0;

		do
		{
			GUARDIDX((testbit&1) | (~index), 0x80, 2);
			index = st->buf_add_7a0[(testbit&1) | (~index)];
			testbit >>= 1;       
			++count;          
		}while(index < 0);
	}

	SkipBits(count, st);

	if (index < 4) return index;
	index &= 0xffff;			//index为高位值
	count = (index>>1) - 1;		//count为余下的位数

	if ( count > 8 )
	{
		int left = count-8;

		int tmpbyte = st->tmpdword1 & 0xff;
		SkipBits(8, st);

		int ret =  (((index & 1) + 2) << count) + 
			( (tbl_bit_mask[left-1] & st->tmpdword1) << 8 )  + tmpbyte;
		SkipBits(left, st);        
		return ret;
	}
	else
	{
		int ret  = (((index & 1) + 2) << count) + 
			(tbl_bit_mask[count-1] & st->tmpdword1);
		SkipBits(count, st);
		return ret;
	}
}


//得到从dest拷贝到dest的时候, 数量

static int GetCopyCount(ST *st)
{
	if (st->fnCopy1_fail_tickcount >6)				//因为只进行一次srt_to_st_buf操作， 所以fnCopy1_fail_tickcount为0或1, 有是在原位置copy. 所以就是为0了..
	{
		st->dwLastError = 7;
		return 0x100;
	}

	// get first shr_count
	signed short index = st->buf_add_820[st->tmpdword1 & 0xff];
	int count;
	if (index >= 0)
	{
		GUARDIDX(index, 0x140, 1);
		count = st->buf_size_fcc[index];
		if (0 == count)
		{
			st->dwLastError = 7;
			return 0x100;
		}
	}
	else
	{
		SkipBits(8, st);
		unsigned char testbit = (unsigned char)(st->tmpdword1 & 0xff);
		int i;
		for(i=1; i<=8; ++i) 
		{
			GUARDIDX((testbit & 1) | (~index), 0x480, 2);
			index = st->buf_add_a20[ (testbit & 1) | (~index) ];
			if(index >=0) break;
			testbit >>=1;
		}
		if (i == 9)
		{
			st->dwLastError = 7;
			return 0x100;
		}
		count = i;
	}

	SkipBits(count, st);

	if (index != 0x11d)
	{
		if (index < 0x109) return index;
		index -= 0x101;
		// index低2位+4为 ret的高位,  index的其他位为ret低位的数量
		count = (index>>2) - 1;

		int ret = (tbl_bit_mask[count-1] & st->tmpdword1) + 
			(((index & 3) + 4) << count) + 0x101;    

		SkipBits(count, st);
		return ret;
	}


	if (0 != st->field_5C_equ0) return 0x200;

	unsigned char old_tmpdword = (unsigned char)(st->tmpdword1);
	SkipBits(8, st);
	unsigned char new_tmpdword = (unsigned char)(st->tmpdword1);
	SkipBits(8, st);

	int ret = new_tmpdword << 8 | old_tmpdword;
	if (0 == ret)
	{ 
		st->fnCopyToDest(st->dest_tmpbuf, (int)(st->dest_tmpbuf_cur - st->dest_tmpbuf),  st->pDecodeInfo, st->dest_tmpbuf_end-st->dest_tmpbuf);
		st->dest_tmpbuf_cur = st->dest_tmpbuf;		//empty dest buf
		SkipBits(st->shr_count, st);
	}
	return ret+0x101;
}       



//从原中拷贝字符
static void GetDataBySrc(ST * st)
{
	int copy_lens;
	int copy_lens_check;

	if (7 != st->shr_count)
	{
		SkipBits(st->shr_count & 7, st);
	}

	// get edi
	int shr_count = st->shr_count;
	if ( shr_count >= 16)
	{
		copy_lens = st->tmpdword1 & 0xffff;
		copy_lens_check = (st->tmpdword1 >> 16) | (GetSrcByte(st) << 8);
	}
	else
	{
		copy_lens = st->tmpdword1;
		if (shr_count < 8)
		{
			copy_lens  |= GetSrcByte(st) << 8;
		}
		copy_lens_check = GetSrcByte(st) | GetSrcByte(st)<<8;
	}

	copy_lens_check = (~copy_lens_check) & 0xffff;
	if (copy_lens != copy_lens_check) 
	{
		st->dwLastError = 5;       //error num = 5?
	}

	int lens = copy_lens;

	while (lens)
	{
		int left_lens = (int)(st->src_tmpbuf_end - st->src_tmpbuf_cur);

		int left_lens2 = (int)(st->dest_tmpbuf + st->copy_lens_from_dest  //这里copy_lens_from_dest其实就是destlen, 这个目标buf的大小
			- st->dest_tmpbuf_cur);

		if (copy_lens > left_lens) copy_lens = left_lens;        
		if (copy_lens > left_lens2) copy_lens = left_lens2;       

		memcpy(st->dest_tmpbuf_cur,  st->src_tmpbuf_cur,  copy_lens, st->dest_tmpbuf_end-st->dest_tmpbuf_cur, st->src_tmpbuf_end-st->src_tmpbuf_cur);
		lens -= copy_lens;

		st->src_tmpbuf_cur += copy_lens;
		st->dest_tmpbuf_cur += copy_lens;

		if (st->src_tmpbuf_cur >=  st->src_tmpbuf_end)
		{
			src_to_st_buf(st);
			if (st->src_tmpbuf_cur == st->src_tmpbuf_end)  break;       //src is empty
		}

		if(st->dest_tmpbuf_cur == st->dest_tmpbuf_end)				//dest is full
		{
			st->dest_tmpbuf_cur = st->dest_tmpbuf;
			st->fnCopyToDest(st->dest_tmpbuf, st->copy_lens_from_dest, st->pDecodeInfo, st->dest_tmpbuf_end-st->dest_tmpbuf);
		}
	}

	st->tmpdword1 = GetSrcByte(st);
	st->shr_count = 0;
}





//从目的中拷贝字符
static void GetDataByDest(ST *st)
{
	char * dest_to_read;
	int copy_count;

	for(; ;)
	{
		copy_count = GetCopyCount(st);   

		if (0x100 == copy_count) return;

		if (copy_count >0x100)
		{
			copy_count -= 0xfe;
			dest_to_read = st->dest_tmpbuf_cur -1 - GetCopyPos(st) ;     
			if (dest_to_read < st->dest_tmpbuf)
			{
				dest_to_read += st->copy_lens_from_dest;  //loop pool
			}

			for(; copy_count>0; --copy_count)
			{
				*(st->dest_tmpbuf_cur++) = *(dest_to_read++); //copy a byte from dest

				if (st->dest_tmpbuf_cur == st->dest_tmpbuf_end)    //buf is full
				{
					st->dest_tmpbuf_cur = st->dest_tmpbuf;  //empty buf
					st->fnCopyToDest(st->dest_tmpbuf, st->copy_lens_from_dest, st->pDecodeInfo, st->dest_tmpbuf_end-st->dest_tmpbuf);
				}    

				if (dest_to_read == st->dest_tmpbuf_end)
				{
					dest_to_read = st->dest_tmpbuf;
				}
			};
		} 
		else
		{
			*(st->dest_tmpbuf_cur++) = (char)copy_count;
			if (st->dest_tmpbuf_cur == st->dest_tmpbuf_end)  //buf is full, write to dest
			{
				st->dest_tmpbuf_cur = st->dest_tmpbuf;  //set buf empty 
				st->fnCopyToDest(st->dest_tmpbuf, st->copy_lens_from_dest, st->pDecodeInfo, st->dest_tmpbuf_end-st->dest_tmpbuf);
			}
		}
	}

}



static int transform_3_table_by_340(int count,  IN OUT char * buf1, OUT WORD * buf2, IN WORD * buf3, ST * st, int new_buf1_size, int new_buf2_size, int new_buf3_size)
{
	short var_48[0x11] = {0};
	short var_24[0x11];

	GUARD(count*1, new_buf1_size);

	for (int i=0; i<count; ++i)
	{
		++ var_48[buf1[i]];
	}

	var_24[1] = 0;
	for (int i=0; i<0xf; ++i)		//15次
		var_24[i+2] = (var_24[i+1] + var_48[i+1])<<1;


	if (0 != var_24[0x10])
	{
		int total = 0;
		for(int i=1; i<0x10; ++i)
		{
			total += var_48[i];           
			if (total > 1)      // fail
			{
				st->dwLastError = 5;
				return 1;
			}
		}     
	}

	GUARD(count*2, 0x240);
	for(int i=0; i<count; ++i)			//transform 1 buf_add_340
	{
		if (0 == buf1[i])
		{
			st->buf_add_340[i] = 0;
		}
		else
		{
			st->buf_add_340[i] = var_24[buf1[i]];
			++ var_24[buf1[i]];
		}
	}

	for(int i = 0; i< count; ++i)          //transform 2 buf1
	{
		if (buf1[i] >1)
		{
			int bitcount= buf1[i];

			unsigned short val = st->buf_add_340[i];
			int highbit = 1 << (bitcount-1);

			unsigned short revval = 0;
			do{
				revval  = (revval>>1) | (val & highbit);
				val <<= 1;
			}while(--bitcount> 0);

			st->buf_add_340[i] = revval;
		}
	}


	GUARD(0x200, new_buf2_size);
	memset(buf2, 0, 0x200);

	int var_count = 0;
	while(--count >=0)					//transform 3 buf2 && buf3
	{

		unsigned char c2 = buf1[count];
		if (0 == c2) continue;

		if (c2 <= 8)
		{
			int step = 1<<c2;
			int index = st->buf_add_340[count];
			GUARDIDX(index, new_buf2_size, 2);
			do
			{
				buf2[index] = count;
				index += step;
			}while(index < 0x100);
			continue;
		}

		WORD * pbuf3 = buf2+(st->buf_add_340[count] & 0xff);
		int testbit = 0x100;

		for(int left_c2=c2-8; left_c2>0; --left_c2)
		{
			if (*pbuf3 == 0)
			{
				*pbuf3 =~var_count;
				GUARDIDX(var_count, new_buf3_size, 2);
				buf3[var_count++] = 0;
				GUARDIDX(var_count, new_buf3_size, 2);
				buf3[var_count++] = 0;
			}

			if (st->buf_add_340[count] & testbit)
			{
				GUARDIDX((WORD)~(*pbuf3) + 1, new_buf3_size, 2);
				pbuf3 = buf3 + (WORD)~(*pbuf3) + 1;
			}
			else
			{
				GUARDIDX((WORD)~(*pbuf3), new_buf3_size, 2);
				pbuf3 = buf3 + (WORD)~(*pbuf3);
			}

			testbit <<= 1;
		}
		* pbuf3 = count;
	}
	return 0;
}





static int AdjustTable(ST *st)
{
	int count_1, count_2, totalcount;
	char buf[0x13];
	int ret;

	int count;
	int index;
	int offset;
	char copy_byte;
	int copy_lens;

	ret = GetBits(2, st);
	if (3 == ret)
	{
		st->dwLastError = 6;
		return 1;
	}
	if (1 == ret)
	{
		memset(st->buf_size_fcc, 8, 0x90);
		memset(st->buf_size_fcc+0x90, 9, 0x70);
		memset(st->buf_size_fcc+0x100, 7, 0x18);
		memset(st->buf_size_fcc+0x118, 8, 0x8);
		memset(st->buf_add_580, 5, 0x20);
		if (0 != transform_3_table_by_340(0x120, st->buf_size_fcc,
			st->buf_add_820, st->buf_add_a20, st, 0x140, 0x200, 0x480))
			return 1;
		if (0 != transform_3_table_by_340(0x20,  st->buf_add_580,
			st->buf_add_5a0, st->buf_add_7a0, st, 0x20, 0x200, 0x80))
			return 1;
		return 0;
	}

	count_1 = GetBits(5, st)+0x101;			//101~132
	count_2 = GetBits(5, st) + 1;			//1~32
	totalcount = count_1 + count_2;

	// init buf
	memset(buf, 0, 0x13);
	count = GetBits(4, st)+3;			//0x3~0x12
	for(int i=0; i<=count; ++i)
	{
		buf[tbl_idx_19[i]] = GetBits(3, st);
	}
	if (0 != transform_3_table_by_340(0x13, buf, 
		st->buf_add_140, 0, st, 0x13, 0x200, 0x0))		//第3个表写0？？ 会有问题吧
		return 1;


	// init buf_size_fcc;
	offset = 0;
	if (totalcount > 0)
	{
		do
		{
			index = st->buf_add_140[st->tmpdword1 & 0xff];  //0x10, 0x11, 0x12,..0,1,2,....
			SkipBits(buf[index], st);

			switch(index - 0x10)
		 {
			case 0:
				copy_lens = GetBits(2, st)+ 3;
				GUARDIDX(offset-1, 0x140, 1);
				copy_byte =  st->buf_size_fcc[offset-1];
				break;
			case 1:
				copy_lens = GetBits(3, st) + 3;
				copy_byte = 0;
				break;
			case 2:
				copy_lens = GetBits(7, st) + 0xb;
				copy_byte = 0;
				break;
			default:
				copy_lens = 1;
				copy_byte = (char)(index);
				break;
		 }

		 if (offset+copy_lens > totalcount) 
		 {
			 st->dwLastError = 5;   //error?
			 return 1;
		 }
		 for(int i=0; i<copy_lens; ++i)
		 {
			 GUARDIDX(offset, 0x140, 1);
			 st->buf_size_fcc[offset++] = copy_byte;
		 }
		}while(offset<totalcount);
	}


	memcpy(st->buf_add_580, st->buf_size_fcc+count_1, count_2, 0x20, 0x140-count_1);     
	if (0 != transform_3_table_by_340(count_1, st->buf_size_fcc,
		st->buf_add_820, st->buf_add_a20, st, 0x140, 0x200, 0x480))
		return 1;

	if (0 != transform_3_table_by_340(count_2, st->buf_add_580,
		st->buf_add_5a0, st->buf_add_7a0, st, 0x20, 0x200, 0x80))
		return 1;
	return 0;     
}



static int decodeinternal3(int arg0, ST *st)
{
	st->field_5C_equ0 = arg0;
	src_to_st_buf(st);
	if (st->src_tmpbuf_cur == st->src_tmpbuf_end) return 0;

	// init bit stream
	st->tmpdword1 = (unsigned char)*(st->src_tmpbuf_cur++);
	st->shr_count = 0;

	for(; ;)
	{
		int decode_over = get_one_bit(st);
		if (st->tmpdword1 & 3)
		{
			if (0 == AdjustTable(st)) GetDataByDest(st);
		}   
		else
		{
			SkipBits(2, st);
			GetDataBySrc(st);			
		}

		if ( 1 == decode_over) break;
		if ( 0 != st->dwLastError) break;
	}

	if (0 != st->dwLastError) return st->dwLastError;
	st->fnCopyToDest(st->dest_tmpbuf, (int)(st->dest_tmpbuf_cur - st->dest_tmpbuf),
		st->pDecodeInfo, st->dest_tmpbuf_end-st->dest_tmpbuf);
	return st->dwLastError;
}

static int explode(FNCOPYFROMSRC fnCopyFromSrc, FNCOPYTODEST fnCopyToDest, ST * st, STDecodeInfo * info, int arg_10)
{
	st->fnCopyFromSrc = fnCopyFromSrc;
	st->pDecodeInfo = info;
	st->buf_size_fcc = st->buf_size_fcc_orig;
	st->buf_add_140 = (WORD *)(st->buf_size_fcc_orig + 0x140);
	st->buf_add_340 = (WORD *)(st->buf_size_fcc_orig + 0x340);
	st->buf_add_580 = st->buf_size_fcc_orig + 0x580;
	st->buf_add_5a0 = (WORD *)(st->buf_size_fcc_orig + 0x5a0);
	st->buf_add_7a0 = (WORD *)(st->buf_size_fcc_orig + 0x7a0);
	st->buf_add_820 = (WORD *)(st->buf_size_fcc_orig + 0x820);
	st->fnCopyToDest = fnCopyToDest;
	st->buf_add_a20 = (WORD *)(st->buf_size_fcc_orig + 0xa20);
	st->src_tmpbuf = st->src;
	st->dest_tmpbuf_cur = st->dest;
	st->dest_tmpbuf = st->dest;
	st->copy_lens_from_dest = st->dest_len;
	st->dest_tmpbuf_end = st->dest + st->dest_len;
	st->copy_lens_from_src = st->src_len;
	st->dwLastError = 0;
	st->fnCopy1_fail_tickcount = 0;
	return decodeinternal3(arg_10, st);
}


static STDecodeInfo * fnInitDecodeInfo(STDecodeInfo *info, char * src, int srclen, char * dest, int destlen)
{
	if (NULL == info) return NULL;
	info->dest = dest;
	info->src =  src;
	info->srclen = srclen;
	info->src_pos = 0;
	info->dest_pos = 0;
	info->init_0_14 = 0; 
	info->destlen = destlen;
	//fnDecodeInfoReset(info);
	{
		//		info->src_pos = 0;
		//		info->dest_pos = 0;
		//		info->init_0_14 = 0; 		
	}
	return info;
}


static int decodeinternal(char * src, int srclen, char * dest, int *destlen,  STDecodeInfo *info)
{
	try
	{
	ST st;

	if (NULL == src || NULL == dest || NULL == destlen || *destlen<=0)
	{
		if (NULL != destlen) destlen = 0;  //bug. 汇编代码没这个NULL判断
		return 3;	//param error.
	}
	if (NULL == fnInitDecodeInfo(info, src, srclen, dest, *destlen))
	{
		*destlen = 0;
		return 4;	//init error
	}

	//	if (NULL == info+0x20)
	//	{
	//		//msg "Couldn't allocate memory!"
	//		//msg (info);
	//		*destlen = 0;
	//		return 4;	//memory overflow
	//	}

	char byte_4d5634[0xfcc];
	memset(&st, 0, sizeof(st));
	st.field_80_ea0 = 0xea0;
	st.src_len = srclen;
	st.dest_len = *destlen;
	st.buf_size_fcc_orig = byte_4d5634;
	st.src = src;
	st.dest = dest;

	int ret = explode(CopyFromSrc, CopyToDest, &st, info, 0);
	if (0 != ret)
	{
		// msg explode returned ret
		*destlen = 0;
	}
	// msg (info)
	if (0 != info->init_0_14)
		return -2;
	return ret;
	}
	catch(...)
	{
		throw;
	}
}

}	//namespace




bool lz_pklite32_decode(const unsigned char * src, unsigned int srclen, unsigned char * dest, unsigned int destlen)
{
	try
	{
		LZ_PKLITE32::STDecodeInfo info;
		return 0 == LZ_PKLITE32::decodeinternal((char *)src, srclen, (char *)dest, (int *)&destlen, &info);
	}
	catch(...)
	{
		return false;
	}
}

