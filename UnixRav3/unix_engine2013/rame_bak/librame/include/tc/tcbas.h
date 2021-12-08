#ifndef __TCE_INCLUDE__
#define __TCE_INCLUDE__

#define DM(n)				(1<<(n))
#define bzero(p,s)			memset(p,0,s)
#define FASTCALL			__fastcall

using namespace tcr;

static inline UINT16 SizeOfTcBin( LPTCREC pr )
{
	return pr->Length;
}
static inline CONST UINT8* TcBinPtr( LPTCREC pr )
{
	return (CONST UINT8*)(pr + 1);
}

//
//	记录执行上下文
//

enum 
{ 
	REG_COUNT = 16, 
	ARG_COUNT = 16,
};

//
// condition
//

enum 
{
	IF_LT,	IF_LE,	IF_EQ,	
	IF_NE,	IF_GE,	IF_GT,

	IF_TRUE = IF_NE,
	IF_FALSE= IF_EQ,

	IF_Z	= IF_EQ,
	IF_NZ	= IF_NE,

	IF_ANY	= 15,
};

//
//	Region define
//

enum {
	RGN_GTLT,		// (a,b)
	RGN_GELT,		// [a,b)
	RGN_GTLE,		// (a,b]
	RGN_GELE,		// [a,b]
};

enum { COMMON_BUFFER_SIZE = 512*1024 };

enum TCATOM_VALUE_TYPE
{
	VNUM = 0,
};

typedef UINT32	TCV32;
typedef UINT32	TCTP;

typedef struct	_atom
{	
	TCV32		v;
	TCTP		t;
} TCATOM, * LPTCATOM;


typedef LPTCATOM	ARGS16_PTR;

typedef UINT16	TCEPC;

template < class T >
FORCEINLINE BOOL IS_INVALID_PC( T v ) { return (TCEPC)v != v; };

FORCEINLINE TCV32 &	atomv( TCATOM & o ) { return o.v; }

FORCEINLINE TCTP &	atomt( TCATOM & o ) { return o.t; }

typedef struct _TIEXEC_CONTEXT
{
	TCATOM		regs[REG_COUNT];	// register
	INT32		fr;					// result register
	union {
#ifdef RS_BIG_ENDIAN
		struct{
			UINT16	unused;
			TCEPC	pc;					// pc
		};
#else
		TCEPC	pc;					// pc
#endif
		PVOID	npc;				// native pc
	};
	TCEPC		ppc;				// prev-pc
	//////////////////////////////////////////////////////////////////////////
	UINT32		exf;				// extend-flags
} TCECTX, *LPTCECTX;

#define REGIDX(p,r)			( ( &r - &(p->regs[0]) ) )

typedef struct TCE_CTX_AND_REC
{
	LPTCREC		RecPtr;
	LPTCECTX	CtxPtr;
} TCRX, * LPTCRX;

#define REGIDX2(x,r)			( ( &r - &(x->CtxPtr->regs[0]) ) )

struct INVOKE_PARAMS
{
	ARGS16_PTR	Arg16;
	LPTCECTX	TcCtx;
	LPTCREC		Record;
};

struct HIT_CONTEXT
{
	LPTCREC		RecPtr;
	TCECTX		TceCtx;
};

//
// 缓冲区对象
//

template < size_t BufSize >
class TBUFFER										// te used buffer
{
protected:
	enum { MAX_BUFFER_SIZE = 64 * 1024 * 1024 };
	enum { DEFAULT_SIZE = BufSize, };
	UINT8						m_buf1[DEFAULT_SIZE];
	UTIL::sentry<UINT8*,UTIL::_DAS>	m_buf2;
	UINT8 *						m_buf;
public:
	TBUFFER() : m_buf(0) { bzero( m_buf1, DEFAULT_SIZE ); }
	FORCEINLINE UINT8 * alloc( SIZE_T cb )
	{
		if( cb > MAX_BUFFER_SIZE ) return 0;
		if( cb > DEFAULT_SIZE ) 
		{
			m_buf2 = RSNEW UINT8[cb];
			if( !m_buf2 ) return 0;
			bzero( m_buf2.m_p, cb );
			m_buf = m_buf2.m_p;
		}
		else
		{
			m_buf = m_buf1;
		}
		return m_buf;
	}
	FORCEINLINE void free()
	{
		if( m_buf2 ) m_buf2.dispose();
		m_buf = 0;
	}

	FORCEINLINE UINT8 * base()
	{
		return m_buf;
	}

	FORCEINLINE void setptr( const void * p )
	{
		m_buf = (UINT8 *)p;
	}
};

typedef TBUFFER<64>	BUFFER64;

class ITCESite;

typedef ITCESite*	SITEPTR;

typedef struct EMUX : TCRX
{
	SITEPTR		SitePtr;
	TCATOM		Args[ARG_COUNT];
} EMUX, *LPEMUX;


class ITCESite
{
public:
	virtual TCV32 FASTCALL stmRead( TCATOM & loc, SIZE_T cb, BUFFER64& buf, EMUX& emux ) PURE;

	virtual TCV32 FASTCALL stmWrite( TCATOM & loc, LPCVOID buf, SIZE_T cb, EMUX& emux ) PURE;

	virtual void FASTCALL doDone( BOOL is_done ) PURE;

	virtual BOOL FASTCALL doInvokeEx( LPTCECTX ctx, CLSID & clsid, SIZE_T rri, EMUX& emux ) PURE;

	// 2013 new
	virtual BOOL FASTCALL doInvokeEx2( LPTCECTX ctx, LPCSTR rProgId, SIZE_T rri, EMUX& emux ) PURE;

	virtual BOOL FASTCALL doCvtType( TCATOM & pos, TCTP tgtType, EMUX& emux )  PURE;

	virtual BOOL FASTCALL doInvoke( TCV32 dispid, TCATOM & Arg0_Ret, EMUX& emux, BOOL breakIfFail )  PURE;

	virtual BOOL FASTCALL doUpdate( TCV32 ulUpdateMask, EMUX& emux )  PURE;

	virtual BOOL FASTCALL fireDebugEvent( SIZE_T uRecEvent, EMUX& emux, BOOL& bContinueDebug )  PURE;

	virtual TCV32 FASTCALL OutStmRead( BOOL bReadDest, TCATOM & loc, LPVOID lpBuf, SIZE_T cb, EMUX& emux ) PURE;

};

#include "opcode.h"

#endif
