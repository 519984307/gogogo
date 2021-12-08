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
	size_t tag;
	size_t bitcount;
} DEPACKDATA;

namespace{
class CFfce
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

	size_t getgamma(DEPACKDATA *ud)
	{
		size_t result = 1;

		/* input gamma2-encoded bits */
		do {
			result = (result << 1) + getbit(ud);
		} while (getbit(ud));

		return (result);
	}
public:
	bool decode(unsigned char *desc, size_t dwUnCompressSize, unsigned char *src, size_t dwCompressSize)
	{
		srcCount = 0;
		descCount = 0;
		descLen = dwUnCompressSize;
		srcLen = dwCompressSize;
		if (0==descLen || 0==srcLen) return true;

		// init depackdata
		DEPACKDATA ud;
		ud.source = src;
		ud.destination = desc;
		if (srcLen<4) throw 1;
		ud.tag = GETDWORD(ud.source);
		ud.bitcount = 0;

		// for lz
		long offs, len=3, lastoffs=0;

		/* first byte verbatim */
		CHECK1(1) CHECK2(1);
		*ud.destination++ = *ud.source++;

		/* main decompression loop */
		for (; ;)
		{
			while (!getbit(&ud))
			{
				CHECK1(1) CHECK2(1);
				*ud.destination++ = *ud.source++;
				len = 3;
			}
			if (ud.destination >= desc+descLen) return true;


			offs = 1;
			long a=0, b=1, fibo;
			do
			{
				do
				{
					fibo = a+b;
					a = b;
					b = fibo;
				}while (!getbit(&ud));

				offs += fibo;

				b = a+fibo;
				a = fibo;

			}while(!getbit(&ud));

			offs -= len;
			if (offs>=0)
			{
				for (int i=0; i<6; ++i)
				{ offs = (offs<<1) + getbit(&ud); }
				len = getgamma(&ud);
				offs ++;
				lastoffs = offs;
				if (offs>0x8000) len++;
				if (offs>0x780) len++;
				if (descCount<(size_t)offs) throw 1;
				CHECK2(len);
				for (; len; --len)
				{
					// bug md5: f3f214d8e296ec0810de1b8d616e547e
					unsigned char c = *(ud.destination-offs);
					*(ud.destination++) = c;
				}
			}		
			else
			{
				len = getgamma(&ud);
				if (descCount<(size_t)lastoffs) throw 1;
				CHECK2(len);
				for (; len; --len)
				{
					unsigned char c = *(ud.destination-lastoffs);
					*(ud.destination++) = c;
				}
			}
			len = 2;
		}
	}

	bool decode2(unsigned char *desc, size_t dwUnCompressSize, unsigned char *src, size_t dwCompressSize)
	{
		srcCount = 0;
		descCount = 0;
		descLen = dwUnCompressSize;
		srcLen = dwCompressSize;
		if (0==descLen || 0==srcLen) return true;

		// init depackdata
		DEPACKDATA ud;
		ud.source = src;
		ud.destination = desc;
		if (srcLen<4) throw 1;
		ud.tag = GETDWORD(ud.source);
		ud.bitcount = 0;

		// for lz
		long offs, len=3, lastoffs=0;

		/* first byte verbatim */
		CHECK1(1) CHECK2(1);
		*ud.destination++ = *ud.source++;

		/* main decompression loop */
		for (; ;)
		{
			while (!getbit(&ud))
			{
				CHECK1(1) CHECK2(1);
				*ud.destination++ = *ud.source++;
				len = 3;
			}
			if (ud.destination >= desc+descLen) return true;


			offs = 1;
			long a=0, b=1, fibo;
			do
			{
				do
				{
					fibo = a+b;
					a = b;
					b = fibo;
				}while (!getbit(&ud));

				offs += fibo;

				b = a+fibo;
				a = fibo;

			}while(!getbit(&ud));

			offs -= len;
			if (offs>=0)
			{
				CHECK1(1)
				offs = (offs << 8) | (*ud.source++);
				offs ++;
				len = getgamma(&ud);
				lastoffs = offs;
				if (offs>0x4000) len++;
				if (offs>0x780) len++;
				if (descCount<(size_t)offs) throw 1;
				CHECK2(len);
				for (; len; --len)
				{
					unsigned char c = *(ud.destination-offs);
					*(ud.destination++) = c;
				}
			}		
			else
			{
				len = getgamma(&ud);
				if (descCount<(size_t)lastoffs) throw 1;
				CHECK2(len);
				for (; len; --len)
				{
					unsigned char c = *(ud.destination-lastoffs);
					*(ud.destination++) = c;
				}
			}
			len = 2;
		}
	}


private:
	size_t srcCount, descCount;
	size_t srcLen, descLen;
};
} // namespace

bool ffce_decode(unsigned char * desc, size_t desclen, unsigned char *src, size_t srclen)
{
	try
	{
		CFfce ffce;
		return  ffce.decode(desc, desclen, src, srclen);
	}
	catch(...)
	{
		return false;
	}
}


bool ffce_decode2(unsigned char * desc, size_t desclen, unsigned char *src, size_t srclen)
{
	try
	{
		CFfce ffce;
		return  ffce.decode2(desc, desclen, src, srclen);
	}
	catch(...)
	{
		return false;
	}
}
