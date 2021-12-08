#ifndef __RSLIB_ALGORITHMS_H__
#define __RSLIB_ALGORITHMS_H__
#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif

#include "../../public/rame.h"
/********************************************************************
zlib
********************************************************************/
#include "../../../../lib3rd/deflate/zlib.h"

#ifndef MAX_WBITS
#  define MAX_WBITS   15 /* 32K LZ77 window */
#endif

#ifndef DEF_WBITS
#  define DEF_WBITS MAX_WBITS
#endif

#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif

/********************************************************************
bz2 (bzlib2)
********************************************************************/
#include "bz2.104/bzlib.h"



/********************************************************************
aplib.cpp
********************************************************************/
//bool aplib_decode(DWORD &retESI, const DWORD dwCompressSize, 
				  //DWORD &retEDI, const DWORD dwUnCompressSize);
bool aplib_decode4tag(	BYTE* &retESI, const DWORD dwCompressSize, 
						BYTE* &retEDI, const DWORD dwUnCompressSize);
bool aplib_decode_010b(	BYTE* &retESI, const DWORD dwCompressSize, 
						BYTE* &retEDI, const DWORD dwUnCompressSize);
bool aplib_decode_017b(	BYTE* &retESI, const DWORD dwCompressSize, 
						BYTE* &retEDI, const DWORD dwUnCompressSize);
bool aplib_decode_022b(	BYTE* &retESI, const DWORD dwCompressSize, 
						BYTE* &retEDI, const DWORD dwUnCompressSize);
bool aplib_decode_036(	BYTE* &retESI, const DWORD dwCompressSize, 
						BYTE* &retEDI, const DWORD dwUnCompressSize);

#define aplib_decode aplib_decode_036


/********************************************************************
pecompact 2.x 专用
********************************************************************/

/********************************************************************
brieflz.cpp
********************************************************************/
bool brieflz_decode(unsigned char * desc, size_t desclen, unsigned char *src, size_t srclen);

/********************************************************************
ffce.cpp
********************************************************************/
bool ffce_decode(unsigned char * desc, size_t desclen, unsigned char *src, size_t srclen);
bool ffce_decode2(unsigned char * desc, size_t desclen, unsigned char *src, size_t srclen);

/********************************************************************
jcalg1.cpp
********************************************************************/
bool jcalg1_decode(unsigned char *pDestination, ULONG desclen, const unsigned char *pSource, ULONG srclen, ULONG * srcProcess=0, ULONG * destProcess=0);


/********************************************************************
exe32pack.cpp 专用
********************************************************************/
bool  exe32pack_inflate_decode(unsigned char *dest, size_t *destLen, unsigned char *source, size_t sourceLen);
bool  exe32pack_lz_fast_decode(unsigned char * desc, size_t desclen,  unsigned char * src, size_t srclen, size_t *descprocess = 0);




/********************************************************************
/lzma
********************************************************************/
#define _LZMA_PROB32


#ifdef _LZMA_PROB32
#define CProb UINT32
#else
#define CProb UINT16
#endif

#ifndef SizeT
//#include <stddef.h>
#define SizeT size_t
#endif

typedef struct _CLzmaProperties
{
	int lc;
	int lp;
	int pb;
}CLzmaProperties;

typedef struct _CLzmaPropertiesOutRead
{
  int lc;
  int lp;
  int pb;

  // for out read
  ULONG  DictionarySize;
}CLzmaPropertiesOutRead;

typedef struct _CLzmaDecoderStateCBOutRead
{
  CLzmaPropertiesOutRead Properties;
  CProb *Probs;

  // for cb;
  const unsigned char *Buffer;
  const unsigned char *BufferLim;

  // for out read
  unsigned char *Dictionary;
  ULONG Range;
  ULONG Code;
  ULONG DictionaryPos;
  ULONG GlobalPos;
  ULONG DistanceLimit;
  ULONG Reps[4];
  int State;
  int RemainLen;
  unsigned char TempDictionary[4];
} CLzmaDecoderStateCBOutRead;


typedef struct _ILzmaInCallback
{
  int (*Read)(void *object, const unsigned char **buffer, SizeT *bufferSize);
} ILzmaInCallback;

#define LZMA_RESULT_OK 0
#define LZMA_RESULT_DATA_ERROR 1
#define LZMA_PROPERTIES_SIZE 5


#define LZMA_BASE_SIZE 1846
#define LZMA_LIT_SIZE 768
#define LzmaGetNumProbs(Properties) (LZMA_BASE_SIZE + (LZMA_LIT_SIZE << ((Properties)->lc + (Properties)->lp)))
#define kLzmaNeedInitId (-2)
#define LzmaDecoderInit(vs) { (vs)->RemainLen = kLzmaNeedInitId; }

int LzmaDecodeProperties(CLzmaProperties *props,  unsigned char prop);
int LzmaDecodeProperties_OutRead(CLzmaPropertiesOutRead *propsRes, const unsigned char *propsData, int size);


int lzma_decode(CLzmaProperties *probs,
			   const unsigned char *inStream, SizeT inSize, SizeT *inSizeProcessed,
			   unsigned char *outStream, SizeT outSize, SizeT *outSizeProcessed);

int lzma_decode_cb(CLzmaProperties *probs, 
			ILzmaInCallback *InCallback,
			unsigned char *outStream, SizeT outSize, SizeT *outSizeProcessed);

int lzma_decode_cb_outread(CLzmaDecoderStateCBOutRead *vs, 
			ILzmaInCallback *InCallback,
			unsigned char *outStream, SizeT outSize, SizeT *outSizeProcessed);


/********************************************************************
 e8e9.cpp
********************************************************************/
#define e8e9_std e8e9
long e8e9(unsigned char * start, long length);

/********************************************************************
e8e9_1.cpp
********************************************************************/
#define e8e9_exprossor e8e9_1
#define e8e9_himemys1 e8e9_1
void e8e9_1(char *buf, long len);


/********************************************************************
unknown_1.cpp
********************************************************************/
#define mian_nspack_decrypt unknown_1_decode
struct STUnknown_1_param
{
	static const ULONG P1LEN = 0x40;
	static const ULONG P2LEN = 0x40;
	static const ULONG P3LEN = 0x30;
	static const ULONG P4LEN = 0x20;
	static const ULONG P5LEN = 0x38;
	static const ULONG P6LEN = 0x30;
	static const ULONG P7LEN = 0x10;
	static const ULONG P8LEN = 0x200;
	static const ULONG P9LEN = 0x8;
	void * p1;			// mask_index   0x40 byte
	void * p2;			// mask_index	0x40 byte
	void * p3;			// mask_index	0x30 byte
	void * p4;			// mask_index	0x20 byte
	void * p5;			// mask_index	0x38 byte
	void * p6;			// mask_index	0x30 byte
	void * p7;			// rol_table	0x10 byte
	void * p8;			// mask_index	0x200 byte
	void * p9;			// mask			0x8  byte
};
bool unknown_1_decode(STUnknown_1_param * param, unsigned char * desc, ULONG desclen, unsigned char *src, ULONG srclen);

/********************************************************************
unknown_2.cpp
key[0x80], table_key_index[0x20], table_bit[0x80]
********************************************************************/
#define mian_nspack_decrypt2 unknown_2_decode
bool unknown_2_decode(const unsigned char *key, unsigned char *desc, const unsigned char *table_key_index, const unsigned char *table_bits,  ULONG len);

/********************************************************************
md md4 md5
md.cpp md4.cpp md5.cpp
********************************************************************/
class CMD // Message Digest Algorithm
{
public:
	void  Init();
	void  Update(const void *pvSrc, DWORD cbSize);
	void  Final();
	const BYTE* GetDigestStream()
	{
	#ifdef RS_BIG_ENDIAN
		for( int i = 0; i < 4; i++)
		{
			m_ret_digest[i] = RS_SWAPDWORD(m_stContext.digest[i]);
		}
		return (BYTE*)m_ret_digest;
	#else
		return (BYTE*)m_stContext.digest;
	#endif
	}
	DWORD GetDigestSize() { return sizeof(m_stContext.digest); }

private:
	virtual void Transform(const DWORD *in) = 0;

protected:
	enum { MD_BUFFER_SIZE = 0x40 };
	struct STCONTEXT
	{
		DWORD count[2];  // number of bits handled mod 2^64
		DWORD digest[4]; // message digest (ABCD)
		BYTE  buffer[MD_BUFFER_SIZE]; // input buffer
	} m_stContext;
	static const BYTE m_abyPadding[MD_BUFFER_SIZE];
#ifdef RS_BIG_ENDIAN
	DWORD m_ret_digest[4]; 
#endif
};

class CMD4 : public CMD
{
	void Transform(const DWORD *in);
};
class CMD5 : public CMD
{
	void Transform(const DWORD *in);
};

/********************************************************************
twofish/twofish.cpp
********************************************************************/
class CTwoFish
{
public:
	void InitKey(unsigned char *key, int bits);
	void SetExpandKey(UINT32 AK[40], UINT32 AQF[4][256]);
	void ECB_Encrypt(UINT8 PT[16]);
	void ECB_Decrypt(UINT8 PT[16]);

private:
	UINT32 K[40];
	UINT32 QF[4][256];
};


/********************************************************************
lz/lz_expressor.cpp
********************************************************************/
bool lz_expressor_decode(unsigned char *desc, UINT32 desclen, const unsigned char *src, UINT32 srclen,  UINT32 * descprocess=0,  UINT32 * srcprocess = 0);


/********************************************************************
lz/lz_pelock.cpp
********************************************************************/
bool lz_pelock_decode(const unsigned char *src,const ULONG srclen, unsigned char*dest,  const ULONG destlen, ULONG *srcprocess = 0, ULONG *destprocess=0);

/********************************************************************
lz/lzss.cpp
********************************************************************/
bool lzss_szdd_decode(unsigned char * dest, UINT32 destlen,  unsigned char * src, UINT32 srclen, UINT32 *destprocess = NULL, UINT32 *srcprocess = NULL);
bool lzss_molebox_decode(unsigned char * dest, UINT32 destlen,  unsigned char * src, UINT32 srclen, UINT32 *destprocess = NULL, UINT32 *srcprocess = NULL);

/********************************************************************
lz/lz_yoda.cpp   0 is success
********************************************************************/
int lz_yoda_decode(unsigned char * dest, UINT32 destlen, const unsigned char * src, UINT32 srclen);

/********************************************************************
lz/lz_pklite32_decode.cpp
********************************************************************/
bool lz_pklite32_decode(const unsigned char * src, UINT32 srclen, unsigned char * dest, UINT32 destlen);

/********************************************************************
lz/lz_pklite32_decode.cpp
********************************************************************/
#include "lzo/minilzo.h"

/********************************************************************
sha/
********************************************************************/
#include "sha/sha.h"
#include "sha/sha1.h"
#include "sha/sha2.h"


/********************************************************************
aes/
********************************************************************/
#include "aes/aescpp.h"


/********************************************************************
rc/
********************************************************************/
struct rc4_state
{
    int x, y, m[256];
};
void rc4_setup( struct rc4_state *s, unsigned char *key,  int length );
void rc4_crypt( struct rc4_state *s, unsigned char *data, int length );


/********************************************************************
crc32/
********************************************************************/
UINT32 update_crc32(UINT32 CurCRC, const char * Buf, size_t BufSize);
UINT32 std_crc32(const char *data, size_t lens);
/********************************************************************
crc64/
********************************************************************/
UINT64 update_crc64(UINT64 CurCRC, const char * Buf, size_t BufSize);
UINT64 std_crc64(const char *data, size_t lens);

/************************************************************************/
/* blowfish                                                             */
/************************************************************************/
#include "blowfish/blowfish.h"
#endif //__RSLIB_ALGORITHMS_H__

