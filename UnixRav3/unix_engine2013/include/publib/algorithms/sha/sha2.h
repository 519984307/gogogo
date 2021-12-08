#ifndef __ALGORITHM_SHA_2_H__
#define __ALGORITHM_SHA_2_H__

#include <stdio.h>
#include <limits.h>

#include "sha.h"



class SHA2 : public SHA // Message Digest Algorithm SHA2
{
protected:
	/* define an unsigned 32-bit type */
	#if UINT_MAX == 0xffffffff
		typedef   unsigned int     sha2_32t;
	#elif ULONG_MAX == 0xffffffff
		typedef   unsigned long    sha2_32t;
	#else
		#error Please define sha2_32t as an unsigned 32 bit type in sha2.h/sha2.cpp
	#endif

	/* define an unsigned 64-bit type */
	#if defined( _MSC_VER )
		typedef unsigned __int64   sha2_64t;
	#elif ULONG_MAX == 0xffffffffffffffff
		typedef unsigned long      sha2_64t;
	#elif ULONG_MAX == 0xffffffff
		typedef unsigned long long sha2_64t;   /* a somewhat dangerous guess */
	#else
		#error Please define sha2_64t as an unsigned 64 bit type in sha2.h/sha2.cpp
	#endif
};


//////////////////////////////////////////////////////////////////////////


class SHA2_256 : public SHA2 // Message Digest Algorithm SHA2_256
{
public:
	virtual void  Init();
	virtual void  Update(const void *pvSrc, size_t cbSize);
	virtual void  Final();
	virtual const unsigned char* GetDigestStream() { return m_digest; }
	virtual size_t GetDigestSize() { return SHA256_DIGEST_SIZE; }

private:
	void compile();

private:
	/* type to hold the SHA256 context */
	struct sha256_ctx
	{   sha2_32t count[2];
		sha2_32t hash[8];
		sha2_32t wbuf[16];
	};

	enum
	{
		SHA256_BLOCK_SIZE  = 64,
		SHA256_DIGEST_SIZE = 32,
	};

private:
	sha256_ctx m_ctx;
	unsigned char m_digest[SHA256_DIGEST_SIZE];

	/* SHA256 mixing data */
	static const sha2_32t k256[64];
	/* SHA256 Final padding and digest calculation */
	static const sha2_32t m1[4];
	static const sha2_32t b1[4];
	/* SHA256 initialisation data */
	static const sha2_32t i256[8];
};


//////////////////////////////////////////////////////////////////////////


class SHA2_512 : public SHA2 // Message Digest Algorithm SHA2_512
{
public:
	virtual void  Init();
	virtual void  Update(const void *pvSrc, size_t cbSize);
	virtual void  Final() { end(SHA512_DIGEST_SIZE); }
	virtual const unsigned char* GetDigestStream() { return m_digest; }
	virtual size_t GetDigestSize() { return SHA512_DIGEST_SIZE; }

protected:
	void compile();
	void end(unsigned int hlen);

protected:
	/* type to hold the SHA384/512 context			*/
	struct sha512_ctx
	{   sha2_64t count[2];
		sha2_64t hash[8];
		sha2_64t wbuf[16];
	};

	enum
	{
		SHA512_BLOCK_SIZE  = 128,
		SHA512_DIGEST_SIZE = 64,
	};

protected:
	sha512_ctx m_ctx;

private:
	unsigned char m_digest[SHA512_DIGEST_SIZE];

	/* SHA384/SHA512 mixing data */
	static const sha2_64t k512[80];
	/* SHA384/512 Final padding and digest calculation */
	static const sha2_64t m2[8];
	static const sha2_64t b2[8];
	/* SHA512 initialisation data */
	static const sha2_64t i512[80];
};


//////////////////////////////////////////////////////////////////////////


class SHA2_384 : public SHA2_512 // Message Digest Algorithm SHA2_384
{
public:
	virtual void  Init();
	virtual void  Final() { end(SHA384_DIGEST_SIZE); }
	virtual size_t GetDigestSize() { return SHA384_DIGEST_SIZE; }

private:
	/* type to hold the SHA384/512 context			*/
	typedef sha512_ctx  sha384_ctx;

	enum
	{
		SHA384_BLOCK_SIZE = SHA512_BLOCK_SIZE,
		SHA384_DIGEST_SIZE = 48,
	};

private:
	/* SHA384 initialisation data */
	static const sha2_64t i384[80];
};


#endif // __ALGORITHM_SHA_2_H__
