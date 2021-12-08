#ifndef __MATCH_METHOD_TEMPLATE__
#define __MATCH_METHOD_TEMPLATE__

#include "matching.h"


namespace mm
{
	struct matcher
	{
		UTIL::co<IREStream> stm;

		matcher( IREStream * pstm ) : stm( pstm ) {};

		template < size_t LEN >
		bool match( size_t offset, LPCVOID data, bool i = false )
		{
			BYTE m[LEN] = {};
			if( LEN != CFileAccessor(stm).ReadPos( m, (long)offset, LEN ) ) 
				return false;
			BYTE * be = m+LEN;
			CONST BYTE * pb = (CONST BYTE *)data;
			if( i ) 
				return 0 == iStmCmp( m, be, pb, pb + LEN );
			else
				return 0 == StmCmp( m, be, pb, pb + LEN );
		}
		template < size_t LEN >
		bool wmatch( size_t offset, LPCVOID data, bool i = false )
		{
			BYTE m[LEN] = {};
			if( LEN != CFileAccessor(stm).ReadPos( m, (long)offset, LEN ) ) 
				return false;
			BYTE * be = m+LEN;
			CONST BYTE * pb = (CONST BYTE *)data;
			if( i ) 
				return 0 == iwStmCmp( m, be, pb, pb + LEN );
			else
				return 0 == wStmCmp( m, be, pb, pb + LEN );
		}

		template < size_t LEN,  size_t RANGE>
		bool search( size_t offset, LPCVOID data, size_t& loc, bool i = false)
		{
			return xsearch<LEN,RANGE>( offset, data, loc, i, false );
		}
		template < size_t LEN,  size_t RANGE>
		bool wsearch( size_t offset, LPCVOID data, size_t& loc, bool i = false)
		{
			return xsearch<LEN,RANGE>( offset, data, loc, i, true );
		}

		template < size_t LEN,  size_t RANGE>
		bool xsearch( size_t offset, LPCVOID data, size_t& loc, bool i = false, bool w =false )
		{
			if( RANGE > 1024 ) 
				return xsearch_big<LEN,RANGE>( offset, data, loc, i, w );
			BYTE buf[RANGE] = {};
			UINT32 cb = CFileAccessor(stm).ReadPos( buf, (long)offset, RANGE );
			if( cb < LEN ) return false;
			BYTE * be = buf+RANGE;
			CONST BYTE * pb = (CONST BYTE *)data;
			CONST BYTE * pe = pb + LEN;
			CONST BYTE * pp = be;
			if( i ) 
			{
				if( w )
					pp = iwStmStm( buf, be, pb, pe );
				else
					pp = iStmStm( buf, be, pb, pe );
			}
			else
			{
				if( w )
					pp = wStmStm( buf, be, pb, pe );
				else
					pp = StmStm( buf, be, pb, pe );
			}
			if( pp == be ) return false;
			loc = offset + pp - buf;
			return true;
		}
		template < size_t LEN,  size_t RANGE>
		bool xsearch_big( size_t offset, LPCVOID data, size_t& loc, bool i = false, bool w =false )
		{
			std::vector<BYTE> buffer;
			buffer.resize(RANGE);
			BYTE * buf = &buffer[0];

			UINT32 cb = CFileAccessor(stm).ReadPos( buf, (long)offset, RANGE );
			if( cb < LEN ) return false;

			BYTE * be = buf+RANGE;
			CONST BYTE * pb = (CONST BYTE *)data;
			CONST BYTE * pe = pb + LEN;
			CONST BYTE * pp = be;
			if( i ) 
			{
				if( w )
					pp = iwStmStm( buf, be, pb, pe );
				else
					pp = iStmStm( buf, be, pb, pe );
			}
			else
			{
				if( w )
					pp = wStmStm( buf, be, pb, pe );
				else
					pp = StmStm( buf, be, pb, pe );
			}
			if( pp == be ) return false;
			loc = offset + pp - buf;
			return true;
		}

		template < typename TYPEC>
		bool read( size_t offset, TYPEC& t )
		{
			return sizeof(t) == CFileAccessor(stm).ReadPos( &t, (long)offset, sizeof(t) );
		}

	};
	
};


#endif