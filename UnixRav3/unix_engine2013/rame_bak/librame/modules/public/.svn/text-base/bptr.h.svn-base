#ifndef __BPRT__
#define __BPRT__

struct bptr_t
{
	const unsigned char * _head;
	const unsigned char * _bptr;
	const unsigned char * _tail;

	inline bptr_t( const unsigned char * h, const unsigned char * t ) 
		: _head(h), _tail(t), _bptr(h)
	{}

	inline unsigned char * ptr()
	{
		return (unsigned char *)_bptr;
	}
	inline size_t offset( )
	{
		return _bptr - _head;
	}

	inline bool seek( long ofs )
	{
		if( _bptr + ofs > _tail ) return false;
		if( _bptr + ofs < _head ) return false;
		_bptr += ofs;
		return true;
	}
	inline bool test( long ofs )
	{
		if( _bptr + ofs > _tail ) return false;
		if( _bptr + ofs < _head ) return false;
		return true;
	}
	template < class T >
	inline T * asptr( long len )
	{
		if( len <= 0 ) return NULL;
		if( !test( len ) ) return NULL;
		return (T*)_bptr;
	}
	inline bool setofs( size_t ofs )
	{
		if( _head + ofs > _tail ) return false;
		if( _head + ofs < _head ) return false;
		_bptr = _head + ofs;
		return true;
	}
	inline bool curr( unsigned char & rc )
	{
		return peek(rc);
	}
	inline bool next( unsigned char & rc )
	{
		return fetch(rc);
	}
	template < class T >
	inline bool fetch( T & rc )
	{
		if( !peek(rc) ) return false;
		_bptr += sizeof(T);
		return true;
	}
	template < class T >
	inline bool peek( T & rc )
	{
		if( _bptr + sizeof(rc) >= _tail ) return false;
#ifdef RS_BIG_ENDIAN
		switch( sizeof(rc) )
		{
		default:
			rc = *(T*)(_bptr); break;
		case 2:
			rc = GETWORD( _bptr ); break;
		case 4:
			rc = GETDWORD( _bptr ); break;
		case 8:
			rc = GETQWORD( _bptr ); break;
		}
#else
		rc = *(T*)(_bptr);
#endif
		return true;
	}
	inline bool fetch( unsigned char * dest, size_t cb )
	{
		if( !peek( dest, cb ) ) return false;
		_bptr += cb;
		return true;
	}
	inline bool peek( unsigned char * dest, size_t cb )
	{
		if( _bptr + cb > _tail ) return false;
		for( size_t b = 0; b < cb; ++ b )
			dest[b] = _bptr[b];
		return true;
	}
	inline int getb()
	{
		uint8_t b;
		if( !fetch(b) ) return -1;
		return b;
	}
	inline bool eof()
	{
		return _bptr >= _tail;
	}
	inline bool bof()
	{
		return _bptr < _head;
	}
};

#endif