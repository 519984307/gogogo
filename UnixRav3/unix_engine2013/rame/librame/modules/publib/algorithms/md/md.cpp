#include "../algorithms.h"





const BYTE CMD::m_abyPadding[MD_BUFFER_SIZE] = { 0x80 };



void CMD::Init()
{
	m_stContext.count[0] = m_stContext.count[1] = 0;
	// Load magic initialization constants.
	m_stContext.digest[0] = 0x67452301; /* A */
	m_stContext.digest[1] = 0xEFCDAB89; /* B */
	m_stContext.digest[2] = 0x98BADCFE; /* C */
	m_stContext.digest[3] = 0x10325476; /* D */
}


void CMD::Update(const void *pvSrc, DWORD cbSize)
{
	const BYTE *pbySrc = (const BYTE*)pvSrc;
	DWORD mdi, *pdw;
	#ifdef RS_BIG_ENDIAN
		DWORD buffer[MD_BUFFER_SIZE/4], i, j;
	#endif

	// compute number of bytes mod 64
	mdi = (m_stContext.count[0] >>3) & 0x3F;

	// update number of bits
	if((m_stContext.count[0] +(cbSize <<3)) < m_stContext.count[0])
	{
		m_stContext.count[1]++;
	}
	m_stContext.count[0] += cbSize <<3;
	m_stContext.count[1] += cbSize >>29;

	while(cbSize--)
	{
		// add new character to buffer, increment mdi
		m_stContext.buffer[mdi++] = *pbySrc++;

		// transform if necessary
		if(mdi >= MD_BUFFER_SIZE)
		{
			#ifdef RS_BIG_ENDIAN
				for(pdw = buffer, i = j = 0; i < MD_BUFFER_SIZE/4; ++i, j += 4)
				{
					pdw[i] = ((DWORD)m_stContext.buffer[j+3] <<24) |
							 ((DWORD)m_stContext.buffer[j+2] <<16) |
							 ((DWORD)m_stContext.buffer[j+1] <<8)  |
							 ((DWORD)m_stContext.buffer[j]);
				}
			#else
				pdw = (DWORD*)m_stContext.buffer;
			#endif

			Transform(pdw);
			mdi = 0;
		}
	}
}


void CMD::Final()
{
	DWORD mdi, padLen, buffer[MD_BUFFER_SIZE/4], *pdw;
	#ifdef RS_BIG_ENDIAN
		BYTE  by, *pby;
		DWORD i, j;
	#endif

	// save number of bits
	buffer[MD_BUFFER_SIZE/4 -2] = m_stContext.count[0];
	buffer[MD_BUFFER_SIZE/4 -1] = m_stContext.count[1];

	// compute number of bytes mod 64
	mdi = (m_stContext.count[0] >>3) & 0x3F;

	// pad out to 56 mod 64
	padLen = (mdi < MD_BUFFER_SIZE -8) ?
			 (MD_BUFFER_SIZE -8 -mdi) : (MD_BUFFER_SIZE*2 -8 -mdi);
	Update(m_abyPadding, padLen);

	// append length in bits and transform
	#ifdef RS_BIG_ENDIAN
		for(pdw = buffer, i = j = 0; i < MD_BUFFER_SIZE/4 -2; ++i, j += 4)
		{
			pdw[i] = ((DWORD)m_stContext.buffer[j+3] <<24) |
					 ((DWORD)m_stContext.buffer[j+2] <<16) |
					 ((DWORD)m_stContext.buffer[j+1] <<8)  |
					 ((DWORD)m_stContext.buffer[j]);
		}
	#else
		*(DWORD*)&m_stContext.buffer[MD_BUFFER_SIZE -8] = buffer[MD_BUFFER_SIZE/4 -2];
		*(DWORD*)&m_stContext.buffer[MD_BUFFER_SIZE -4] = buffer[MD_BUFFER_SIZE/4 -1];
		pdw = (DWORD*)m_stContext.buffer;
	#endif

	Transform(pdw);

	#ifdef RS_BIG_ENDIAN
		for(pby = (BYTE*)m_stContext.digest, i = 0; i < MD_BUFFER_SIZE/4; i += 4)
		{
			by = pby[i];
			pby[i]   = pby[i+3];
			pby[i+3] = by;
			by = pby[i+1];
			pby[i+1] = pby[i+2];
			pby[i+2] = by;
		}
	#endif
}
