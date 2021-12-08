#ifndef __ALGORITHM_SHA_1_H__
#define __ALGORITHM_SHA_1_H__

#include <stdio.h>
#include <limits.h>

#include "sha.h"


class SHA1 : public SHA // Message Digest Algorithm SHA1
{
public:
	virtual void  Init();
	virtual void  Update(const void *pvSrc, size_t cbSize);
	virtual void  Final();
	virtual const unsigned char* GetDigestStream() { return m_digest; }
	virtual size_t GetDigestSize() { return SHA1_DIGEST_SIZE; }

private:
	void compile();

private:
	/* define an unsigned 32-bit type */
	#if UINT_MAX == 0xffffffff
		typedef   unsigned int     sha1_32t;
	#elif ULONG_MAX == 0xffffffff
		typedef   unsigned long    sha1_32t;
	#else
		#error Please define sha1_32t as an unsigned 32 bit type in sha1.h
	#endif

	/* type to hold the SHA1 context */
	struct sha1_ctx
	{   sha1_32t count[2];
		sha1_32t hash[5];
		sha1_32t wbuf[16];
	};

	enum
	{
		SHA1_BLOCK_SIZE  = 64,
		SHA1_DIGEST_SIZE = 20,
	};

private:
	sha1_ctx m_ctx;
	unsigned char m_digest[SHA1_DIGEST_SIZE];

	/* SHA1 final padding and digest calculation  */
	static const sha1_32t mask[4];
	static const sha1_32t bits[4];
};


#endif // __ALGORITHM_SHA_1_H__
