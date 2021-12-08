
typedef struct {
	const unsigned char *source;
	unsigned char *destination;
	ULONG tag;
	ULONG bitcount;
	// new for check
	ULONG srclen;
	ULONG destlen;
	unsigned char *srcbak;
	unsigned char *destbak;
} STDecodeInfo;



static int getbit(STDecodeInfo *info)
{
	unsigned int bit;

	/* check if tag is empty */
	if (!info->bitcount--)
	{
		/* load next tag */
		if (info->source >= info->srcbak+info->srclen) throw 1;
		info->tag = *info->source++;
		info->bitcount = 7;
	}

	/* shift bit out of tag */
	bit = (info->tag >> 7) & 0x01;
	info->tag <<= 1;
	return bit;
}


static unsigned int getgamma(STDecodeInfo *info)
{
	unsigned int result = 1;

	/* input gamma2-encoded bits */
	do {
		result = (result << 1) + getbit(info);
	} while (!getbit(info));

	return (result);
}


static bool inline_decode(const unsigned char *src, const ULONG srclen, unsigned char*dest, const ULONG destlen, ULONG *srcprocess = 0, ULONG *destprocess=0)
{
	int off = 1;
	int lens = 0;
	int eax;

	STDecodeInfo info;
	info.source = src;
	info.srcbak = (unsigned char *)src;
	info.destbak = info.destination = dest;
	info.srclen = srclen;
	info.destlen = destlen;
	info.bitcount = 0;
	info.tag = 0;

	try
	{
	for(; ;)
	{
		while(getbit(&info)) 
		{
			if (info.source >= info.srcbak+info.srclen || info.destination>=info.destbak+info.destlen) throw 1;
			*(info.destination++) = *(info.source++);
		}
		
		eax = getgamma(&info) - 3;
		if (eax >= 0)
		{
			eax = (eax<<8) |  *(info.source++);
			if ((int)0xffffffff == eax) 
			{
				if (srcprocess) *srcprocess = (ULONG) (info.source - src);
				if (destprocess) *destprocess = (ULONG) (info.destination - dest);
				return true; //decode over
			}
			off = eax+1;
		}

		lens = (lens<<1) + getbit(&info);
		lens = (lens<<1) + getbit(&info);
		if (0 == lens )	lens = getgamma(&info)+2;
		if (off <= 0xd00) 
			lens ++;
		else
			lens +=2;

		if (info.destination+lens > info.destbak+info.destlen) throw 1; 
		if (info.destination-off < info.destbak) throw 1;
		for (int _lens = lens; _lens>0; --_lens) *(info.destination++) = *(info.destination-off);
		lens = 0;
	}
	}


	catch(...)
	{
		if (srcprocess) *srcprocess = (ULONG) (info.source - src);
		if (destprocess) *destprocess = (ULONG) (info.destination - dest);
		return false;
	}
}


bool lz_pelock_decode(const unsigned char *src,const ULONG srclen, unsigned char*dest,  const ULONG destlen, ULONG *srcprocess = 0, ULONG *destprocess=0)
{
	try
	{
		return inline_decode(src, srclen, dest, destlen, srcprocess, destprocess);
	}
	catch (...)
	{
		return false;
	}
}
