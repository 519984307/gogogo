#ifndef NULL
#define NULL 0
#endif

bool lzss_decode(int N, int F, unsigned char *text_buf, unsigned char *dest, unsigned int destlen, unsigned char *src, unsigned int srclen, unsigned int *destprocess = NULL, unsigned int *srcprocess = NULL)
{
//#define N         4096    /* size of ring buffer */
//#define F         18    /* upper limit for match_length */   //注意~!!! 4096, 16 是microsoft的compress和extract所使用的常量
#define THRESHOLD    2 /* encode string into position and length if match_length is greater than this */
#define SRCCHECK {	if (src >=srcend) \
					{					\
						if (NULL != destprocess) *destprocess = dprocess;	\
						if (NULL != srcprocess) *srcprocess = sprocess;	\
						return false;	\
					}}

#define DESTCHECK  	{	if (dest >=destend) \
					{					\
						if (NULL != destprocess) *destprocess = dprocess;	\
						if (NULL != srcprocess) *srcprocess = sprocess;	\
						return true;	\
					}}


	//unsigned char  text_buf[N + F - 1];    /* ring buffer of size N, with extra F-1 bytes to facilitate string comparison */
    int i, j, k, r;
    unsigned int flags;
    int dprocess = 0;
	int sprocess = 0;
	const unsigned char * destend = dest+destlen;
	const unsigned char * srcend = src+srclen;




    for (i = 0; i < N - F; ++i) text_buf[i] = ' ';
    r = N - F; flags = 0;
    for ( ; ; ) {
        if (((flags >>= 1) & 256) == 0) {
			SRCCHECK;
            flags = *(src++) | 0xff00;        /* uses higher byte cleverly  to count eight */
			++sprocess;
        }                           
        if (flags & 1) {
			SRCCHECK;
			text_buf[r++] = *(dest++) =  *(src++);
			r &= (N - 1);
			++sprocess;
			++dprocess;
			DESTCHECK;
        } else {
			SRCCHECK;
			i = *(src++);
			++sprocess;
			SRCCHECK;
			j = *(src++);
			++sprocess;
            i |= ((j & 0xf0) << 4); j = (j & 0x0f) + THRESHOLD;
            for (k = 0; k <= j; ++k) {
                text_buf[r++] = *(dest++) = text_buf[(i + k) & (N - 1)];
                 r &= (N - 1);
				 ++dprocess;
				DESTCHECK;
            }
        }
    }

}


bool lzss_szdd_decode(unsigned char * dest, unsigned int destlen,  unsigned char * src, unsigned int srclen, unsigned int *destprocess = NULL, unsigned int *srcprocess = NULL)   
{
	const int N = 4096;
	const int F = 16;
	unsigned char  text_buf[N + F - 1];
	try
	{
		return lzss_decode(N, F, text_buf, dest, destlen, src, srclen, destprocess, srcprocess);
	}
	catch (...)
	{
		return false;
	}
}
bool lzss_molebox_decode(unsigned char * dest, unsigned int destlen,  unsigned char * src, unsigned int srclen, unsigned int *destprocess = NULL, unsigned int *srcprocess = NULL)   
{
	const int N = 4096;
	const int F = 18;
	unsigned char  text_buf[N + F - 1];
	try
	{
		return lzss_decode(N, F, text_buf, dest, destlen, src, srclen, destprocess, srcprocess);
	}
	catch (...)
	{
		return false;
	}
}

