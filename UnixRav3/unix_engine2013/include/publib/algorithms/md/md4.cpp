#include "../algorithms.h"





// F, G and H are basic MD4 functions.
#define F(x, y, z)	(((x) & (y)) | ((~x) & (z)))
#define G(x, y, z)	(((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z)	((x) ^ (y) ^ (z))

// ROTATE_LEFT rotates x left n bits.
#define ROTATE_LEFT(x, n)	(((x) << (n)) | ((x) >> (32-(n))))

// FF, GG and HH are transformations for rounds 1, 2 and 3
// Rotation is separate from addition to prevent recomputation
#define FF(a, b, c, d, x, s)							\
		{												\
			(a) += F((b), (c), (d)) + (x);				\
			(a)  = ROTATE_LEFT((a), (s));				\
		}
#define GG(a, b, c, d, x, s)							\
		{												\
			(a) += G((b), (c), (d)) + (x) + 0x5A827999;	\
			(a)  = ROTATE_LEFT((a), (s));				\
		}
#define HH(a, b, c, d, x, s)							\
		{												\
			(a) += H((b), (c), (d)) + (x) + 0x6ED9EBA1;	\
			(a)  = ROTATE_LEFT((a), (s));				\
		}

// Round 1
#define S11 3
#define S12 7
#define S13 11
#define S14 19
// Round 2
#define S21 3
#define S22 5
#define S23 9
#define S24 13
// Round 3
#define S31 3
#define S32 9
#define S33 11
#define S34 15


// MD4 basic transformation. Transform state based on in.
void CMD4::Transform(const DWORD *in)
{
	DWORD
	a = m_stContext.digest[0],
	b = m_stContext.digest[1],
	c = m_stContext.digest[2],
	d = m_stContext.digest[3];


	/* Round 1 */
	FF(a, b, c, d, in[ 0], S11); /*  1 */
	FF(d, a, b, c, in[ 1], S12); /*  2 */
	FF(c, d, a, b, in[ 2], S13); /*  3 */
	FF(b, c, d, a, in[ 3], S14); /*  4 */
	FF(a, b, c, d, in[ 4], S11); /*  5 */
	FF(d, a, b, c, in[ 5], S12); /*  6 */
	FF(c, d, a, b, in[ 6], S13); /*  7 */
	FF(b, c, d, a, in[ 7], S14); /*  8 */
	FF(a, b, c, d, in[ 8], S11); /*  9 */
	FF(d, a, b, c, in[ 9], S12); /* 10 */
	FF(c, d, a, b, in[10], S13); /* 11 */
	FF(b, c, d, a, in[11], S14); /* 12 */
	FF(a, b, c, d, in[12], S11); /* 13 */
	FF(d, a, b, c, in[13], S12); /* 14 */
	FF(c, d, a, b, in[14], S13); /* 15 */
	FF(b, c, d, a, in[15], S14); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, in[ 0], S21); /* 17 */
	GG(d, a, b, c, in[ 4], S22); /* 18 */
	GG(c, d, a, b, in[ 8], S23); /* 19 */
	GG(b, c, d, a, in[12], S24); /* 20 */
	GG(a, b, c, d, in[ 1], S21); /* 21 */
	GG(d, a, b, c, in[ 5], S22); /* 22 */
	GG(c, d, a, b, in[ 9], S23); /* 23 */
	GG(b, c, d, a, in[13], S24); /* 24 */
	GG(a, b, c, d, in[ 2], S21); /* 25 */
	GG(d, a, b, c, in[ 6], S22); /* 26 */
	GG(c, d, a, b, in[10], S23); /* 27 */
	GG(b, c, d, a, in[14], S24); /* 28 */
	GG(a, b, c, d, in[ 3], S21); /* 29 */
	GG(d, a, b, c, in[ 7], S22); /* 30 */
	GG(c, d, a, b, in[11], S23); /* 31 */
	GG(b, c, d, a, in[15], S24); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, in[ 0], S31); /* 33 */
	HH(d, a, b, c, in[ 8], S32); /* 34 */
	HH(c, d, a, b, in[ 4], S33); /* 35 */
	HH(b, c, d, a, in[12], S34); /* 36 */
	HH(a, b, c, d, in[ 2], S31); /* 37 */
	HH(d, a, b, c, in[10], S32); /* 38 */
	HH(c, d, a, b, in[ 6], S33); /* 39 */
	HH(b, c, d, a, in[14], S34); /* 40 */
	HH(a, b, c, d, in[ 1], S31); /* 41 */
	HH(d, a, b, c, in[ 9], S32); /* 42 */
	HH(c, d, a, b, in[ 5], S33); /* 43 */
	HH(b, c, d, a, in[13], S34); /* 44 */
	HH(a, b, c, d, in[ 3], S31); /* 45 */
	HH(d, a, b, c, in[11], S32); /* 46 */
	HH(c, d, a, b, in[ 7], S33); /* 47 */
	HH(b, c, d, a, in[15], S34); /* 48 */


	m_stContext.digest[0] += a;
	m_stContext.digest[1] += b;
	m_stContext.digest[2] += c;
	m_stContext.digest[3] += d;
}


#undef F
#undef G
#undef H

#undef ROTATE_LEFT

#undef FF
#undef GG
#undef HH

#undef S11
#undef S12
#undef S13
#undef S14

#undef S21
#undef S22
#undef S23
#undef S24

#undef S31
#undef S32
#undef S33
#undef S34
