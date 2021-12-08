#ifndef __RX_BLOB__
#define __RX_BLOB__


struct RX_CBLOB
{
	const void *	Data;
	size_t			Length;

	RX_CBLOB() : Data(0), Length(0){};

	RX_CBLOB( CONST RX_CBLOB& right ) : Data(right.Data), Length(right.Length)
	{
	};

	RX_CBLOB& operator = ( CONST RX_CBLOB& right )
	{
		Data = (right.Data), Length = (right.Length);
		return *this;
	}
	RX_CBLOB( const void * b, size_t l ) : Data(b), Length(l)
	{
	};

	void rx_free()
	{
		Data = 0;
		Length = 0;
	}

	virtual ~RX_CBLOB() { ; }

	// safe, size check
	template < class T >
	bool get_t( T & t ) const
	{
		if( Length < sizeof(t) ) return false;
		t = *((T*)Data);
		return true;
	}

	// unsafe, no size check
	template < class T >
	T & ref_t()	const
	{
		return *((T*)Data);
	}

	// unsafe, no size check
	template < class T >
	T * asptr_t( size_t l = sizeof(T) )	const
	{
		if( l > Length ) return 0;
		return ((T*)Data);
	}

};
//
//struct RX_BLOB
//{
//	typedef void ( * free_t )( RX_BLOB * );
//	const void *	Data;
//	size_t			Length;
//	free_t			FreePtr;
//	void			Free() { if( FreePtr ) FreePtr( this ); Data = 0; Length = 0; FreePtr = 0; };
//
//	static void cpp_delete( RX_BLOB * p )
//	{
//		if( !p ) return;
//		if( p->Data ) { delete ((char*)(p->Data)); };
//	}
//
//	RX_BLOB() : Data(0), Length(0), FreePtr(0){};
//
//	RX_BLOB( RX_BLOB& right ) : Data(right.Data), Length(right.Length), FreePtr(right.FreePtr)
//	{
//		right.FreePtr = 0;
//	};
//
//	RX_BLOB&  operator = ( RX_BLOB& right )
//	{
//		Data = (right.Data), Length = (right.Length), FreePtr = (right.FreePtr);
//		right.FreePtr = 0;
//		return *this;
//	}
//
//	template < class T >
//	RX_BLOB( T& t ) : Data(&t), Length(sizeof(t)), FreePtr(0){};
//
//	RX_BLOB( const void * b, size_t l )
//		: Data(b), Length(l), FreePtr(0){};
//
//	RX_BLOB( const void * b, size_t l, free_t pfn )
//		: Data(b), Length(l), FreePtr(pfn){};
//
//	~RX_BLOB() { Free(); }
//
//	// safe, size check
//	template < class T >
//	bool get_t( T & t ) const
//	{
//		if( Length < sizeof(t) ) return false;
//		t = *((T*)Data);
//		return true;
//	}
//
//	// unsafe, no size check
//	template < class T >
//	T & ref_t()	const
//	{
//		return *((T*)Data);
//	}
//
//	// unsafe, no size check
//	template < class T >
//	T * asptr_t( size_t l = sizeof(T) )	const
//	{
//		if( l > Length ) return 0;
//		return ((T*)Data);
//	}
//};


struct RX_BLOB : RX_CBLOB
{
	typedef void ( * free_t )( RX_BLOB * );
	free_t			FreePtr;
	void			rx_free() 
	{ 
		if( FreePtr ) FreePtr( this ); 
		Data = 0; Length = 0; FreePtr = 0; 
	};

	template < class T >
	static void cpp_delete( RX_BLOB * p )
	{
		if( !p ) return;
		if( p->Data ) { delete [] ((T*)(p->Data)); };
	}

	RX_BLOB() : RX_CBLOB(), FreePtr(0){};

	RX_BLOB( RX_CBLOB& right )
	{
		Data = (right.Data), Length = (right.Length);
		FreePtr = NULL;
	}

	RX_BLOB( RX_BLOB& right )
	{
		Data = (right.Data), Length = (right.Length), FreePtr = (right.FreePtr);
		right.FreePtr = 0;
	};

	template< class T >
	RX_BLOB( T& r )
	{
		Data = &r;
		Length = sizeof(T);
		FreePtr = NULL;
	}

	RX_BLOB&  operator = ( RX_BLOB& right )
	{
		rx_free();
		Data = (right.Data), Length = (right.Length), FreePtr = (right.FreePtr);
		right.FreePtr = 0;
		return *this;
	}
	RX_BLOB&  operator = ( const RX_CBLOB& right )
	{
		rx_free();
		Data = (right.Data), Length = (right.Length);
		return *this;
	}

	RX_BLOB( const void * b, size_t l ) : RX_CBLOB(b,l), FreePtr(0)
	{}

	RX_BLOB( const void * b, size_t l, free_t pfn )
		: RX_CBLOB(b,l), FreePtr(pfn)
	{};

	~RX_BLOB() { rx_free(); }
};


#endif
