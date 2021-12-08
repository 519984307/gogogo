#ifndef __MUTEX_H__
#define __MUTEX_H__

// xuzilong 2004-12-07 mod <
#ifdef WIN32
#include "defsa.h"
#endif
// xuzilong 2004-12-07 mod <

#ifdef WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif // !_WINDOWS_
#else // WIN32
typedef int BOOL;
#define TRUE	1
#define FALSE	0
#endif // WIN32



///////////////////////// !!! IMPORTANT !!! //////////////////////////////
/* NOTE:
// 1. About MutexCreate()
//    This function creates a new mutex or opens an existing mutex.
// 2. About MutexDestroy()
//    The behavior of this function is different between UNIX and
//    Windows(inter-process mutex only). On Windows, the mutex maintains
//    a reference count automatically, the count increments one when 
//    the MutexCreate is called and decrements one when the MutexDestroy
//    is called. If a process calls the MutexDestroy, the mutex will be
//    removed only the count is zero. But on UNIX, no reference count is
//    maintained, so the mutex will be removed immediately whenever the
//    MutexDestroy is called.
//    This is a restriction of the UNIX operating system.
////////////////////////////////////////////////////////////////////////*/

//#define WIN32_DEBUG
#ifdef WIN32_DEBUG
typedef HANDLE Mutex;
#else
typedef void* Mutex;
#endif // WIN32_DEBUG

// Mutex type
#define MT_INTRAPROCESS	1
#define MT_INTERPROCESS	2
#ifdef WIN32
#define MT_GLOBAL		3
#endif // WIN32

Mutex MutexCreate(unsigned int type, const char* name);
BOOL MutexLock(Mutex mtx, unsigned long millisec_wait);
void MutexUnlock(Mutex mtx);
void MutexDestroy(Mutex mtx);
#if defined( _NOUSESHM_) && defined(UNIXPLATFORM)
#define MutexCreate(d,b) ((void *)0xffffffff)
#define MutexLock(a,b)	(true)
#define MutexUnlock(a)
#define MutexDestroy(a)

#endif // _NOUSESHM_

/*************************************************************************
** Data structure
*************************************************************************/

typedef struct
{
	unsigned int type;
	void* pobj;
} Muteks;

#endif // __MUTEX_H__
