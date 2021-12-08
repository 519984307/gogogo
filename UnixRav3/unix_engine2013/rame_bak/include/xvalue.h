#ifndef __XVALUE_DEF__
#define __XVALUE_DEF__

#include "iunknown.h"
#include "refable.h"
#include "blob.h"

namespace xv
{
#pragma pack(1)
	struct type_byte
	{
		UINT8		type:5;
		UINT8		bsiz:3;
	} __attribute__((packed));
#pragma pack()

#define TP(t,s)		((t)|((s)<<5))

	enum value_size
	{
		_8			=	0,
		_16			=	1,
		_32			=	2,
		_64			=	3,
		_128		=	4,
	};

	enum value_type
	{
		dNull		=	0x0,
		dInt		=	0x1,
		dReal		=	0x2,
		dLocalStr	=	0x3,
		dUtf8Str	=	0x4,
		dUniStr		=	0x5,
		dBuffer		=	0x6,
		dArray		=	0x7,
		dObject		=	0x8,
		dUnknown	=	0x9,
		dBoolean	=	0x10,
	};

	typedef value_type	ValueType;

	struct Value;

	struct Output
	{
		virtual bool write( const void * p, size_t cb ) = 0;
	};

	struct vComplex : IRefable
	{
		virtual size_t	length() = 0;
		virtual bool	clear() = 0;
		virtual bool	clone( vComplex ** ppo ) = 0;
		virtual bool	cloneTo( vComplex * po ) = 0;
	};

	struct vArray : vComplex
	{
		virtual xv::Value&	ref( size_t i ) = 0;
		virtual bool 		set( size_t i, const xv::Value & item ) = 0;
		virtual bool 		get( size_t i, xv::Value & item ) = 0;
		virtual bool 		erase( size_t i ) = 0;
		typedef bool 		( * _enum_cb_ptr )( size_t name, xv::Value & item, bool last, void * context );
		virtual bool 		enum_it( _enum_cb_ptr fcb, void * context ) = 0;
	};

	// max key length is 255
	struct vObject : vComplex
	{
		virtual xv::Value&	ref( const char * name ) = 0;
		virtual bool 		set( const char * name, const xv::Value & item ) = 0;
		virtual bool 		get( const char * name, xv::Value & item ) = 0;
		virtual bool 		erase( const char * name ) = 0;
		typedef bool 		( * _enum_cb_ptr )( const char * name, xv::Value & item, bool last, void * context );
		virtual bool 		enum_it( _enum_cb_ptr fcb, void * context ) = 0;
	};

	struct vBuffer : vComplex
	{
		virtual bool			assign( const void * p, size_t cb ) = 0;
		virtual UINT8 *		    base() = 0;
		virtual bool			resize( UINT32 size ) = 0;
	};

	struct vStringA : vComplex
	{
		virtual bool			assign( const char * str ) = 0;
		virtual const char *	c_str() = 0;
	};

	struct vStringW : vComplex
	{
		virtual bool			assign( const wchar_t * str ) = 0;
		virtual const wchar_t *	c_str() = 0;
	};

	typedef RX_BLOB		chunk_t;

	enum { MAX_KEY_LEN = 255 };

#pragma pack(4)
	struct Value
	{
		type_byte		_type;
		union
		{
			INT64		integer;
			double		real;
			vComplex *	complex;
			IUnknown*	punk;
			bool		vbool;
			INT64       anyval;
		};
	} __attribute__((packed)) ;
#pragma pack()
};

//////////////////////////////////////////////////////////////////////////
//
//
//				Basic APIs
//
//
//////////////////////////////////////////////////////////////////////////
typedef xv::Value		XVALUE;
typedef xv::value_type	XVTYPE;
typedef xv::value_size	XVSIZE;

BOOL WINAPI	ValueInit( XVALUE* );
BOOL WINAPI	ValueClear( XVALUE* );
BOOL WINAPI	ValueSetType( XVALUE* , XVTYPE );
BOOL WINAPI	ValueSetType2( XVALUE* , XVTYPE, XVSIZE);
BOOL WINAPI	ValueClone( const XVALUE*, XVALUE* );
BOOL WINAPI	ValueSet( RS_CONST_DEF XVALUE*, XVALUE* );
BOOL WINAPI	ValueCheckType( RS_CONST_DEF XVALUE*, XVTYPE );
BOOL WINAPI	ValueCheckType2( const XVALUE*, XVTYPE, XVSIZE);
BOOL WINAPI	ValueCheckTypes(const XVALUE*, UINT32 );
BOOL WINAPI ValueLoad( XVALUE*, const void *, size_t );
BOOL WINAPI ValueSave( XVALUE*, xv::Output * );
BOOL WINAPI ValueToJson( xv::Value* vp, xv::Output * wo );
BOOL WINAPI ValueFromJson( xv::Value* vp, LPCSTR json_string, SIZE_T );

#endif
