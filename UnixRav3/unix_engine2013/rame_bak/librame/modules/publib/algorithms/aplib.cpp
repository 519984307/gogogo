/*
 * aPLib compression library  -  the smaller the better :)
 *
 * C depacker
 *
 * Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 */

#include "algorithms.h"




#define CHECK1(length1)  {  \
	if( srcCount+(length1)>srcLen) throw 1; \
	srcCount += length1; }
#define CHECK2(length2)  {  \
	if( descCount+(length2)>descLen) throw 1; \
	descCount += length2; }


/* internal data structure */
typedef struct {
	const unsigned char *source;
	unsigned char *destination;
	unsigned int tag;
	unsigned int bitcount;
} APDEPACKDATA;

namespace {
class CAplib
{
public:
	int aP_getbit1tag(APDEPACKDATA *ud)
	{
		unsigned int bit;

		/* check if tag is empty */
		if (!ud->bitcount--)
		{
			/* load next tag */
			CHECK1(1);
			ud->tag = *ud->source++;
			ud->bitcount = 7;
		}

		/* shift bit out of tag */
		bit = (ud->tag >> 7) & 0x01;
		ud->tag <<= 1;

		return bit;
	}
	int aP_getbit4tag(APDEPACKDATA *ud)
	{
		unsigned int bit;

		/* check if tag is empty */
		if (!ud->bitcount--)
		{
			/* load next tag */
			CHECK1(4);
			ud->tag =  GETDWORD(ud->source);
			ud->source += 4;
			ud->bitcount = 31;
		}

		/* shift bit out of tag */
		bit = (ud->tag >> 31) & 0x01;
		ud->tag <<= 1;

		return bit;
	}

	int aP_getbit(APDEPACKDATA *ud)
	{
		if (1==tagByte)
			return aP_getbit1tag(ud);
		else
			return aP_getbit4tag(ud);
	}


	unsigned int aP_getgamma(APDEPACKDATA *ud)
	{
		unsigned int result = 1;

		/* input gamma2-encoded bits */
		do {
			result = (result << 1) + aP_getbit(ud);
		} while (aP_getbit(ud));

		return (result);
	}
	unsigned int aP_getgamma2(APDEPACKDATA *ud)
	{
		unsigned int result = 1;

		/* input gamma2-encoded bits */
		while (aP_getbit(ud)) {
			result = (result << 1) + aP_getbit(ud);
		}

		return (result << 1) + aP_getbit(ud);
	}

	void aP_depack_010b(int tagbyte, const void *source, const DWORD dwCompressSize,
		void *destination, const DWORD dwUnCompressSize, DWORD *pCountIn, DWORD *pCountOut)
	{
		APDEPACKDATA ud;
		unsigned int offs, len, R0=0, LWM;
		int done;
		int i;
		srcCount = 0; descCount = 0;
		srcLen = dwCompressSize; descLen = dwUnCompressSize;
		tagByte = tagbyte;

		ud.source = (const unsigned char *) source;
		ud.destination = (unsigned char *) destination;
		ud.bitcount = 0;

		LWM = 0;
		done = 0;

		/* first byte verbatim */
		//CHECK1(1) CHECK2(1);

		/* main decompression loop */
		while (!done)
		{
			if (aP_getbit(&ud))
			{
				if (aP_getbit(&ud))
				{
					if (aP_getbit(&ud))
					{
						offs = 0;

						for (i = 4; i; i--) offs = (offs << 1) + aP_getbit(&ud);

						if (offs)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						} else {
							CHECK2(1);
							*ud.destination++ = 0x00;
						}


					} else {
						CHECK1(1);
						offs = *ud.source++;

						len = 2 + (offs & 0x0001);

						offs >>= 1;

						if (offs)
						{
							for (; len; len--)
							{
								CHECK2(1);
								if (descCount<offs) throw 1;
								*ud.destination = *(ud.destination - offs);
								ud.destination++;
							}
						} else done = 1;
					}

				} else {

					offs = aP_getgamma2(&ud);
					--offs;
					if (offs == 1)
					{
						offs = R0;
						len = aP_getgamma2(&ud);
						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}
					}
					else
					{
						--offs;
						--offs;
						offs <<= 8;
						CHECK1(1);
						offs += *ud.source++;
						len = aP_getgamma2(&ud);

						if (offs < 128) len += 2;

						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}
						R0 = offs;
					}
				}

			} else {
				CHECK1(1) CHECK2(1);
				*ud.destination++ = *ud.source++;
			}
		}

		if (pCountIn) *pCountIn = ud.source - (unsigned char *) source;
		if (pCountOut) *pCountOut = ud.destination - (unsigned char *) destination;
		return;
	}
	void aP_depack_017b(int tagbyte, const void *source, const DWORD dwCompressSize,
		void *destination, const DWORD dwUnCompressSize, DWORD *pCountIn, DWORD *pCountOut)
	{
		APDEPACKDATA ud;
		unsigned int offs, len, R0=0, LWM;
		int done;
		int i;
		srcCount = 0; descCount = 0;
		srcLen = dwCompressSize; descLen = dwUnCompressSize;
		tagByte = tagbyte;

		ud.source = (const unsigned char *) source;
		ud.destination = (unsigned char *) destination;
		ud.bitcount = 0;

		LWM = 0;
		done = 0;

		/* first byte verbatim */
		CHECK1(1) CHECK2(1);
		*ud.destination++ = *ud.source++;

		/* main decompression loop */
		while (!done)
		{
			if (aP_getbit(&ud))
			{
				if (aP_getbit(&ud))
				{
					if (aP_getbit(&ud))
					{
						offs = 0;

						for (i = 4; i; i--) offs = (offs << 1) + aP_getbit(&ud);

						if (offs)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						} else {
							CHECK2(1);
							*ud.destination++ = 0x00;
						}


					} else {
						CHECK1(1);
						offs = *ud.source++;

						len = 2 + (offs & 0x0001);

						offs >>= 1;

						if (offs)
						{
							for (; len; len--)
							{
								CHECK2(1);
								if (descCount<offs) throw 1;
								*ud.destination = *(ud.destination - offs);
								ud.destination++;
							}
						} else done = 1;

						R0 = offs;
					}

				} else {

					offs = aP_getgamma(&ud);
					--offs;
					if (offs == 1)
					{
						offs = R0;
						len = aP_getgamma(&ud);
						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}
					}
					else
					{
						--offs;
						--offs;
						offs <<= 8;
						CHECK1(1);
						offs += *ud.source++;
						len = aP_getgamma(&ud);

						if (offs >= 1280) len++;
						if (offs < 128) len += 2;

						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}
						R0 = offs;
					}
				}

			} else {
				CHECK1(1) CHECK2(1);
				*ud.destination++ = *ud.source++;
			}
		}

		if (pCountIn) *pCountIn = ud.source - (unsigned char *) source;
		if (pCountOut) *pCountOut = ud.destination - (unsigned char *) destination;
		return;
	}
	void aP_depack_022b(int tagbyte, const void *source, const DWORD dwCompressSize,
		void *destination, const DWORD dwUnCompressSize, DWORD *pCountIn, DWORD *pCountOut)
	{
		APDEPACKDATA ud;
		unsigned int offs, len, R0=0, LWM;
		int done;
		int i;
		srcCount = 0; descCount = 0;
		srcLen = dwCompressSize; descLen = dwUnCompressSize;
		tagByte = tagbyte;

		ud.source = (const unsigned char *) source;
		ud.destination = (unsigned char *) destination;
		ud.bitcount = 0;

		LWM = 0;
		done = 0;

		/* first byte verbatim */
		CHECK1(1) CHECK2(1);
		*ud.destination++ = *ud.source++;

		/* main decompression loop */
		while (!done)
		{
			if (aP_getbit(&ud))
			{
				if (aP_getbit(&ud))
				{
					if (aP_getbit(&ud))
					{
						offs = 0;

						for (i = 4; i; i--) offs = (offs << 1) + aP_getbit(&ud);

						if (offs)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						} else {
							CHECK2(1);
							*ud.destination++ = 0x00;
						}



					} else {
						CHECK1(1);
						offs = *ud.source++;

						len = 2 + (offs & 0x0001);

						offs >>= 1;

						if (offs)
						{
							for (; len; len--)
							{
								CHECK2(1);
								if (descCount<offs) throw 1;
								*ud.destination = *(ud.destination - offs);
								ud.destination++;
							}
						} else done = 1;

						R0 = offs;
					}

				} else {

					offs = aP_getgamma(&ud);
					--offs;
					if (offs == 1)
					{
						offs = R0;
						len = aP_getgamma(&ud);
						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}
					}
					else
					{
						--offs;
						--offs;
						offs <<= 8;
						CHECK1(1);
						offs += *ud.source++;
						len = aP_getgamma(&ud);

						if (offs >= 32000) len++;
						if (offs >= 1280) len++;
						if (offs < 128) len += 2;

						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}
						R0 = offs;
					}
				}

			} else {
				CHECK1(1) CHECK2(1);
				*ud.destination++ = *ud.source++;
			}
		}

		if (pCountIn) *pCountIn = ud.source - (unsigned char *) source;
		if (pCountOut) *pCountOut = ud.destination - (unsigned char *) destination;
		return;
	}

	void aP_depack(int tagbyte, const void *source, const DWORD dwCompressSize,
		void *destination, const DWORD dwUnCompressSize, DWORD *pCountIn, DWORD *pCountOut)
	{
		APDEPACKDATA ud;
		unsigned int offs, len, R0=0, LWM;
		int done;
		int i;
		srcCount = 0; descCount = 0;
		srcLen = dwCompressSize; descLen = dwUnCompressSize;
		tagByte = tagbyte;

		ud.source = (const unsigned char *) source;
		ud.destination = (unsigned char *) destination;
		ud.bitcount = 0;

		LWM = 0;
		done = 0;

		/* first byte verbatim */
		CHECK1(1) CHECK2(1);
		*ud.destination++ = *ud.source++;

		/* main decompression loop */
		while (!done)
		{
			if (aP_getbit(&ud))
			{
				if (aP_getbit(&ud))
				{
					if (aP_getbit(&ud))
					{
						offs = 0;

						for (i = 4; i; i--) offs = (offs << 1) + aP_getbit(&ud);

						if (offs)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						} else {
							CHECK2(1);
							*ud.destination++ = 0x00;
						}

						LWM = 0;

					} else {
						CHECK1(1);
						offs = *ud.source++;

						len = 2 + (offs & 0x0001);

						offs >>= 1;

						if (offs)
						{
							for (; len; len--)
							{
								CHECK2(1);
								if (descCount<offs) throw 1;
								*ud.destination = *(ud.destination - offs);
								ud.destination++;
							}
						} else done = 1;

						R0 = offs;
						LWM = 1;
					}

				} else {

					offs = aP_getgamma(&ud);

					if ((LWM == 0) && (offs == 2))
					{
						offs = R0;

						len = aP_getgamma(&ud);

						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}

					} else {

						if (LWM == 0) offs -= 3; else offs -= 2;

						offs <<= 8;
						CHECK1(1);
						offs += *ud.source++;

						len = aP_getgamma(&ud);

						if (offs >= 32000) len++;
						if (offs >= 1280) len++;
						if (offs < 128) len += 2;

						for (; len; len--)
						{
							CHECK2(1);
							if (descCount<offs) throw 1;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
						}

						R0 = offs;
					}

					LWM = 1;
				}

			} else {
				CHECK1(1) CHECK2(1);
				*ud.destination++ = *ud.source++;
				LWM = 0;
			}
		}

		if (pCountIn) *pCountIn = ud.source - (unsigned char *) source;
		if (pCountOut) *pCountOut = ud.destination - (unsigned char *) destination;
		return;
	}

private:
	DWORD srcCount, descCount;
	DWORD srcLen, descLen;
	int  tagByte;
};
} //namespace

bool aplib_decode4tag(PBYTE &retESI, const DWORD dwCompressSize, 
 PBYTE &retEDI, const DWORD dwUnCompressSize)
{
	if (dwCompressSize<=0 || dwUnCompressSize<=0 || retESI==0 || retEDI==0) return false;
	CAplib aplib;
	try
	{
		DWORD countIn, countOut;
		aplib.aP_depack(4, (const void *)retESI, dwCompressSize,  (void *)retEDI, dwUnCompressSize, &countIn, &countOut);
		retESI += countIn;
		retEDI += countOut;
	}
	catch(...)
	{
		return false;
	}
	return true;
}


bool aplib_decode_010b(PBYTE &retESI, const DWORD dwCompressSize, 
					   PBYTE &retEDI, const DWORD dwUnCompressSize)
{
	if (dwCompressSize<=0 || dwUnCompressSize<=0 || retESI==0 || retEDI==0) return false;
	CAplib aplib;
	try
	{
		DWORD countIn, countOut;
		aplib.aP_depack_010b(1, (const void *)retESI, dwCompressSize,  (void *)retEDI, dwUnCompressSize, &countIn, &countOut);
		retESI += countIn;
		retEDI += countOut;
	}
	catch(...)
	{
		return false;
	}
	return true;
}


bool aplib_decode_017b(PBYTE &retESI, const DWORD dwCompressSize, 
					   PBYTE &retEDI, const DWORD dwUnCompressSize)
{
	if (dwCompressSize<=0 || dwUnCompressSize<=0 || retESI==0 || retEDI==0) return false;
	CAplib aplib;
	try
	{
		DWORD countIn, countOut;
		aplib.aP_depack_017b(1, (const void *)retESI, dwCompressSize,  (void *)retEDI, dwUnCompressSize, &countIn, &countOut);
		retESI += countIn;
		retEDI += countOut;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool aplib_decode_022b(PBYTE &retESI, const DWORD dwCompressSize, 
			   PBYTE &retEDI, const DWORD dwUnCompressSize)
{
	if (dwCompressSize<=0 || dwUnCompressSize<=0 || retESI==0 || retEDI==0) return false;
	CAplib aplib;
	try
	{
		DWORD countIn, countOut;
		aplib.aP_depack_022b(1, (const void *)retESI, dwCompressSize,  (void *)retEDI, dwUnCompressSize, &countIn, &countOut);
		retESI += countIn;
		retEDI += countOut;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//可能是0.36?以上版本
bool aplib_decode_036(PBYTE &retESI, const DWORD dwCompressSize, 
					  PBYTE &retEDI, const DWORD dwUnCompressSize)
{
	if (dwCompressSize<=0 || dwUnCompressSize<=0 || retESI==0 || retEDI==0) return false;
	CAplib aplib;
	try
	{
		DWORD countIn, countOut;
		aplib.aP_depack(1, (const void *)retESI, dwCompressSize,  (void *)retEDI, dwUnCompressSize, &countIn, &countOut);
		retESI += countIn;
		retEDI += countOut;
	}
	catch(...)
	{
		return false;
	}
	return true;
}
