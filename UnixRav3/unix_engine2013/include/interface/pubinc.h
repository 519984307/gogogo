#ifndef __RAME_PUBLIC_INC__
#define __RAME_PUBLIC_INC__

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#pragma warning (disable: 4996)

#include "../../include/platform.h"

#if (TARGET_OS == OS_WINDOWS )

#	include "windef.win32.h"

#else

#	include "win32.error.h"

#	if (TARGET_OS == OS_NATIVE )
#		include "windef.native.h"
#	else
#		include "windef.posix.h"
#	endif

#endif // (TARGET_OS == TARGET_OS_WINDOWS )

#include "../../include/bq.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	rs-memory
//

#define	RSNEW				new

#ifndef IMPL_MEMORY_ALLOC

	#ifdef USE_C_MEMORY_ALLOC

		#if CPP_COMPILER==CC_MSVC
			#pragma message( "MEMORY_ALLOC: STD_C_MALLOC" )
		#else
			#warning "MEMORY_ALLOC: STD_C_MALLOC"
		#endif

		#define	rxmalloc		malloc
		#define	rxfree			free
		#define	rxrealloc		realloc
		#define	rxcalloc		calloc

	#else

		#if CPP_COMPILER==CC_MSVC
			#pragma message( "MEMORY_ALLOC: RS_MEMORY_ALLOC" )
		#else
			#warning "MEMORY_ALLOC: RS_MEMORY_ALLOC"
		#endif

		void * STDAPIVCALLTYPE	rxmalloc( size_t cb );
		void   STDAPIVCALLTYPE	rxfree( void * p );
		void * STDAPIVCALLTYPE	rxrealloc( void * p, size_t cb );
		void * STDAPIVCALLTYPE	rxcalloc( size_t cnt, size_t size );

		#define	malloc	rxmalloc
		#define	free	rxfree
		#define	realloc	rxrealloc
		#define	calloc	rxcalloc

	#endif

#else

	#if CPP_COMPILER==CC_MSVC
		#pragma message( "MEMORY_ALLOC: IMPL_MEMORY_ALLOC" )
	#else
		#warning "MEMORY_ALLOC: IMPL_MEMORY_ALLOC"
	#endif

	void * STDAPIVCALLTYPE	rxmalloc( size_t cb );
	void   STDAPIVCALLTYPE	rxfree( void * p );
	void * STDAPIVCALLTYPE	rxrealloc( void * p, size_t cb );
	void * STDAPIVCALLTYPE	rxcalloc( size_t cnt, size_t size );

#endif


//////////////////////////////////////////////////////////////////////////
//
//	c++ new/delete
//

#if (CPP_COMPILER == CC_MSVC)

#ifndef DISABLE_NEW_OVERRIDE

	FORCEINLINE void* STDMETHODVCALLTYPE operator new (size_t size)
	{
		return malloc(size);
	}
	FORCEINLINE  void STDMETHODVCALLTYPE operator delete (void *p)
	{
		free(p);
	}
	FORCEINLINE  void* STDMETHODVCALLTYPE operator new [] (size_t size)
	{
		return malloc(size);
	}
	FORCEINLINE  void STDMETHODVCALLTYPE operator delete [] (void *p)
	{
		free(p);
	}

#endif //DISABLE_NEW_OVERRIDE

#endif


//////////////////////////////////////////////////////////////////////////
//
//	DynLib exports
//

#define DECLARE_MY_CREATEOBJECT(nsname)			\
	extern "C" {								\
	HRESULT WINAPI REGetClassObject(const CLSID& , const IID& , void **);	\
	}
#define BEGIN_MY_CREATEOBJECT(nsname)	extern "C" {
#define END_MY_CREATEOBJECT(nsname)	}

#define ASSIGNMENT_OPERATOR(Cls) Cls & operator = ( const Cls & );


//////////////////////////////////////////////////////////////////////////
//
//
//
#include "rxruntim.h"


//////////////////////////////////////////////////////////////////////////
//
//
//

//	utils

#include "../utils/utility.h"
#include "../utils/utility_ex.h"
#include "../utils/libstr.hpp"
#include "../utils/dllutil.h"
#include "../utils/txvalue.hpp"
#include "../utils/printlog.h"
#include "../utils/lightalg.h"




//////////////////////////////////////////////////////////////////////////
//
//
//

#define INOUT
#define ASSERT0
#define ASSERT(x)

//////////////////////////////////////////////////////////////////////////
#define	NEED_CARE_HERE(x)			care(__FILE__,__LINE__)

static inline void care( LPCSTR , int )
{
}


#endif	//_PUBINC_H_
