#ifndef __RXMPE_BITMAP__
#define __RXMPE_BITMAP__


namespace mpe
{
	typedef UTIL::sentry<UINT8*,UTIL::default_array_sentry>	SAFE_BUFFER;

	struct sdiv_t 
	{
		SIZE_T quot;
		SIZE_T rem;
	};

	static FORCEINLINE sdiv_t sdiv( SIZE_T numer, SIZE_T denom )
	{
		sdiv_t result;
		result.quot = numer / denom;
		result.rem = numer % denom;
		return result;
	};

	struct BitMap
	{
	protected:
		SAFE_BUFFER		mBufferOwner;
		SIZE_T			mBits;
		UINT8 *			mBuffer;
	public:
		BitMap() : mBits(0), mBuffer(0)
		{
		}

		SIZE_T	BitCount() { return mBits; };

		UINT8*	BitBuffer() { return mBuffer; };

		VOID Clear()
		{
			mBufferOwner = (UINT8*)0;
			mBuffer = 0;
			mBits = 0;
		}
		BOOL Attach( UINT8 * lpData, SIZE_T cBits )
		{
			if( !lpData || !cBits ) return FALSE;
			Clear();
			mBuffer = lpData;
			mBits = cBits;
			return TRUE;
		}
		BOOL Create( SIZE_T bits )
		{
			Clear();
			if( !bits ) return TRUE;
			sdiv_t t = sdiv( (long)bits, (long)8 );
			if( t.rem ) t.quot ++;
			mBits = t.quot * 8;
			mBufferOwner = new UINT8[t.quot];
			if( !mBufferOwner ) return FALSE;
			mBuffer = mBufferOwner;
			Zero();
			return TRUE;
		}
		FORCEINLINE BOOL Zero()
		{
			if( !mBuffer ) return TRUE;
			memset( mBuffer, 0, mBits/8 );
			return TRUE;
		}
		FORCEINLINE BOOL SetBit( SIZE_T i, BOOL bSet = TRUE )
		{
			if( i >= mBits ) return FALSE;
			sdiv_t t  = sdiv( i, 8 );
			if( bSet ) 
				mBuffer[t.quot] |= (1<<t.rem);
			else
				mBuffer[t.quot] ^= (1<<t.rem);
			return TRUE;
		}
		FORCEINLINE BOOL TestBit( SIZE_T i )
		{
			if( i >= mBits ) return FALSE;
			sdiv_t t = sdiv( i, 8 );
			return ((mBuffer[t.quot] & (1<<t.rem))) != 0;
		}
	};
};


#endif