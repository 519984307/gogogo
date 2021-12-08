#ifndef __RXMPE_BYTE_AREA__
#define __RXMPE_BYTE_AREA__

template < typename T >
struct br
{
	typedef CONST T *	BTPTR;
	
	struct ByteRegionT
	{
		BTPTR	Current;
		BTPTR	Last;

		FORCEINLINE ByteRegionT() : Current(0), Last(0)
		{}

		FORCEINLINE ByteRegionT( CONST VOID * lpElmt, CONST VOID * lpLast )
			: Current( (BTPTR)lpElmt )
			, Last( (BTPTR)lpLast )
		{}
		FORCEINLINE ByteRegionT( CONST VOID * lpElmt, SIZE_T ssCount )
			: Current( (BTPTR)lpElmt )
			, Last( (BTPTR)lpElmt + ssCount )
		{}

		FORCEINLINE BTPTR CurrentPtr(){ return Current; };

		FORCEINLINE BOOL SetBuffer( CONST VOID * lpData, SIZE_T cbBytes )
		{
			Current = (BTPTR)lpData;
			Last = Current + cbBytes;
			return Current <= Last;
		}

		FORCEINLINE BOOL SetLast( CONST VOID * lpData )
		{
			if( lpData < Current ) return FALSE;
			Last = (BTPTR)lpData;
			return TRUE;
		}

		template < class P > 
		FORCEINLINE BOOL FetchPtr( P * & lpOut, SIZE_T sCount )
		{
			BTPTR lpDest = Test( sizeof(P) * sCount );
			if( !lpDest ) return FALSE;
			lpOut = (P*)Current;
			Current = lpDest;
			return TRUE;
		}

		template < class P > 
		FORCEINLINE BOOL PeekPtr( P * & lpOut, SIZE_T sCount )
		{
			BTPTR lpDest = Test( sizeof(P) * sCount );
			if( !lpDest ) return FALSE;
			lpOut = (P*)Current;
			return TRUE;
		}

		FORCEINLINE BOOL FetchVLQ32( UINT32& rValue )
		{
			// If A is 0, then this is the last VLQ octet of the integer. If A is 1, then another VLQ octet follows.
			UINT8 * p = 0;
			UINT8	b = 0;
			if( PeekPtr( p, 5 ) )
			{
				for( size_t i = 0; ; ++ i, rValue <<= 7 )
				{
					b = p[i];
					rValue |= (b&0x7F);
					if( 0 == (b&0x80) ) 
						break;
				}
			}
			else
			{
				for( size_t i = 0; ; ++ i, rValue <<= 7 )
				{
					if( !FetchVal(b) ) return FALSE;
					rValue |= (b&0x7F);
					if( 0 == (b&0x80) ) 
						break;
				}
			}
			return TRUE;
		}

		template < class P > 
		FORCEINLINE BOOL FetchVal( P & rValue )
		{
			BTPTR lpDest = Test(sizeof(rValue));
			if( !lpDest ) return FALSE;
			rValue = *(P*)Current;
			Current = lpDest;
			return TRUE;
		}
		template < class P > 
		FORCEINLINE BOOL PeekVal( P & rValue )
		{
			BTPTR lpDest = Test(sizeof(rValue));
			if( !lpDest ) return FALSE;
			rValue = *(P*)Current;
			return TRUE;
		}
		FORCEINLINE BOOL FetchByteArray( ByteRegionT & rArray, SIZE_T cbLength )
		{
			BTPTR lpDest = Test( cbLength );
			if( !lpDest ) return FALSE;
			rArray.Current = Current;
			rArray.Last = lpDest;
			Current = lpDest;
			return TRUE;
		}
		FORCEINLINE BOOL PeekByteArray( ByteRegionT & rArray, SIZE_T cbLength )
		{
			BTPTR lpDest = Test( cbLength );
			if( !lpDest ) return FALSE;
			rArray.Current = Current;
			rArray.Last = lpDest;
			return TRUE;
		}
		FORCEINLINE BTPTR Test( XLONG lDist )
		{
			BTPTR lpDest = Current + lDist;
			if( lpDest > Last || lpDest < Current )
				return NULL;
			return lpDest;
		}
		FORCEINLINE BTPTR Seek( XLONG lDist )
		{
			BTPTR lpDest = Test( lDist );
			if( !lpDest ) return FALSE;
			BTPTR lpRet = Current;
			Current = lpDest;
			return lpRet;
		}

		FORCEINLINE SIZE_T RestBytes()
		{
			return Last - Current;
		}

		FORCEINLINE SIZE_T Length()
		{
			return Last - Current;
		}
	};
	
	struct ByteRegionLT : ByteRegionT
	{
		using ByteRegionT::Current;
		using ByteRegionT::Last;
		
		BTPTR	First;

		FORCEINLINE ByteRegionLT() : ByteRegionT(), First(0)
		{}

		FORCEINLINE ByteRegionLT( CONST VOID* lpElmt, CONST VOID* lpLast )
			: ByteRegionT(lpElmt, lpLast)
			, First( (BTPTR)lpElmt )
		{}
		FORCEINLINE ByteRegionLT( CONST VOID* lpElmt, SIZE_T ssCount )
			: ByteRegionT(lpElmt, ssCount)
			, First( (BTPTR)lpElmt )
		{}
		FORCEINLINE ByteRegionLT( CONST VOID* lpFirst, CONST VOID* lpLast, CONST VOID * lpCurrent )
			: ByteRegionT(lpCurrent, lpLast)
			, First( (BTPTR)lpFirst )
		{}

		FORCEINLINE VOID Reset()
		{
			Current = First;
		}

		FORCEINLINE SIZE_T Offset()
		{
			return Current - First;
		}

		FORCEINLINE BOOL SetCurrent( BTPTR lpCurrent )
		{
			if( lpCurrent < First || lpCurrent > Last ) 
				return FALSE;
			Current = lpCurrent;
			return TRUE;
		}

		FORCEINLINE BOOL SetOffset( SIZE_T uOffset )
		{
			return SetCurrent( First + uOffset );
		}

		FORCEINLINE BTPTR Test( XLONG lDist )
		{
			BTPTR lpDest = Current + lDist;
			if( lpDest > Last || lpDest < First )
				return NULL;
			return lpDest;
		}
		FORCEINLINE BTPTR Seek( XLONG lDist )
		{
			BTPTR lpDest = Test( lDist );
			if( !lpDest ) return FALSE;
			BTPTR lpRet = Current;
			Current = lpDest;
			return lpRet;
		}
		FORCEINLINE SIZE_T Length()
		{
			return Last - First;
		}
	};
};

typedef br<CHAR>::ByteRegionT	ByteRegion;
typedef br<CHAR>::ByteRegionLT	ByteRegionL;



#endif