#ifndef __REF_PTR_IMPL__
#define __REF_PTR_IMPL__

#include "interlocked.h"

struct IRefable
{
	virtual LONG _reference() = 0;
	virtual LONG _release() = 0;
};

struct RefalbeImp : IRefable
{
protected:
	LONG	_ref_count;
public:
	RefalbeImp() : _ref_count(0) {};
	virtual ~RefalbeImp() {};
	inline LONG get_ref_count(){return _ref_count;}
	REFABLE_IMP_MT;
};

struct Refalbe
{
protected:
	LONG	_ref_count;
public:
	Refalbe() : _ref_count(0) {};
};

typedef RefalbeImp	refable_impl_t;

template < class T >
struct refp
{
	T * m_p;
	refp() : m_p(0)
	{}

	refp( int p ) : m_p(0)
	{
	}
	template< class P>
	refp( P* p ) : m_p(0)
	{
		bind( (T*)p );
	}
	refp( const refp & r ) : m_p(0)
	{
		bind( r.m_p );
	}
	refp & operator = ( const refp<T> & t )
	{
		return bind( t.m_p );
	}
	refp & operator = ( T* p )
	{
		return bind( p );
	}
	template< class P>
	refp & operator = ( P * p )
	{
		return bind( (T*)p );
	}
	virtual ~refp()
	{
		dispose();
	}
	bool operator < ( const refp & r )
	{
		return m_p < r.m_p;
	}
	refp & bind( T * p, bool rf = true )
	{
		dispose();
		m_p = (T*)p;
		if( rf && m_p ) m_p->_reference();
		return *this;
	}
	T * operator -> ( )
	{
		return m_p;
	}
	bool operator < ( const refp<T> & r ) const
	{
		return m_p < r.m_p;
	}
	operator T * ()
	{
		return m_p;
	}
	void dispose()
	{
		if( m_p ) m_p->_release();
		m_p = 0;
	}
	void attach( T * p )
	{
		dispose();
		m_p = p;
	}
	T * detach()
	{
		T * r = m_p;
		m_p = 0;
		return r;
	}
	template < class U >
	U * cast()
	{
		return (U*)m_p;
	}
	T** pp()
	{
		dispose();
		return &m_p;
	}
};

//////////////////////////////////////////////////////////////////////////
template < class T >
struct tcop
{
	T * m_p;
	tcop() : m_p(0)
	{}

	tcop( int p ) : m_p(0)
	{
	}
	template< class P>
	tcop( P* p ) : m_p(0)
	{
		bind( (T*)p );
	}
	tcop( const tcop & r ) : m_p(0)
	{
		bind( r.m_p );
	}
	tcop & operator = ( const tcop<T> & t )
	{
		return bind( t.m_p );
	}
	tcop & operator = ( T* p )
	{
		return bind( p );
	}
	template< class P>
	tcop & operator = ( P * p )
	{
		return bind( (T*)p );
	}
	~tcop()
	{
		destroy();
	}
	bool operator < ( const tcop & r )
	{
		return m_p < r.m_p;
	}
	tcop & bind( T * p, bool rf = true )
	{
		destroy();
		m_p = (T*)p;
		if( rf && m_p ) m_p->_reference();
		return *this;
	}
	T * operator -> ( )
	{
		return m_p;
	}
	bool operator < ( const tcop<T> & r ) const
	{
		return m_p < r.m_p;
	}
	operator T * ()
	{
		return m_p;
	}
	void destroy()
	{
		if( m_p ) m_p->_release();
		m_p = 0;
	}
	void attach( T * p )
	{
		destroy();
		m_p = p;
	}
	T * detach()
	{
		T * r = m_p;
		m_p = 0;
		return r;
	}
};


#endif
