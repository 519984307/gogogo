#include "algorithms.h"

#define CHECK1(length1)  {  \
	if( srcCount+(length1)>srcLen) throw 1; \
	srcCount += length1; }
#define CHECK2(length2)  {  \
	if( descCount+(length2)>descLen) throw 1; \
	descCount += length2; }



typedef struct {
	const unsigned char *source;
	unsigned char *destination;
	unsigned long tag;
	unsigned long bitcount;
} DEPACKDATA;

class CBrieflz
{
private:
	int getbit(DEPACKDATA *ud)
	{
		unsigned int bit;

		/* check if tag is empty */
		if (!ud->bitcount--)
		{
			CHECK1(4);
			/* load next tag */
			ud->tag = GETDWORD(ud->source);
			ud->source += 4;
			ud->bitcount = 31;
		}

		/* shift bit out of tag */
		bit = (ud->tag >> 31) & 0x01;
		ud->tag <<= 1;

		return bit;
	}

	unsigned long getgamma(DEPACKDATA *ud)
	{
		unsigned long result = 1;

		/* input gamma2-encoded bits */
		do {
			result = (result << 1) + getbit(ud);
		} while (getbit(ud));

		return (result);
	}
public:
	bool decode(unsigned char *desc, unsigned long dwUnCompressSize, unsigned char *src, unsigned long dwCompressSize)
	{
		srcCount = 0;
		descCount = 0;
		descLen = dwUnCompressSize;
		srcLen = dwCompressSize;
		if (0==descLen || 0==srcLen) return true;

		// init apdepackdata
		DEPACKDATA ud;
		ud.source = src;
		ud.destination = desc;
		if (srcLen<4) throw 1;
		ud.tag = GETDWORD(ud.source);
		ud.bitcount = 0;

		// for lz
		long offs, len;

		/* first byte verbatim */
		CHECK1(1) descCount++;
		*ud.destination++ = *ud.source++;
		if (ud.destination >= desc+descLen) return true;

		/* main decompression loop */
		while (ud.destination < desc+descLen)
		{
			if (getbit(&ud)) 
			{
				len = getgamma(&ud) + 2;

/*				offs =getgamma(&ud) - 2;
				CHECK1(1);
				offs = offs<<8 | *ud.source++ +1;
				*/

				offs = getgamma(&ud);
				CHECK1(1);
				offs = offs<<8;
				offs |= *ud.source++;
				offs -= 0x1ff;






				if (descCount<(unsigned long)offs) throw 1;
				CHECK2(len);
				for (; len; --len)
					*(ud.destination++) = *(ud.destination-offs);			
			}
			else
			{
				CHECK1(1) descCount++;
				*ud.destination++ = *ud.source++;
			}
		}
		return true;
	}

private:
	unsigned long srcCount, descCount;
	unsigned long srcLen, descLen;
};


bool brieflz_decode(unsigned char * desc, unsigned long desclen, unsigned char *src, unsigned long srclen)
{
	try
	{
		CBrieflz brieflz;
		return  brieflz.decode(desc, desclen, src, srclen);
	}
	catch(...)
	{
		return false;
	}
}
