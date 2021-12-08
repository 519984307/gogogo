#ifndef _RS_STDLIB_H_
#define _RS_STDLIB_H_


//
#include <stdlib.h>

// memory ================================================================
#if defined( PLATFORM_TYPE_NATIVE)
//
// #include "../../COMMON/RSCOMMON/RSNATIVE.H"
//
#define rs_malloc	bs_malloc
#define rs_calloc	bs_calloc
#define rs_free		bs_free

#else
//
#define rs_malloc	malloc
#define rs_calloc	calloc
#define rs_free		free

#endif 


// string ================================================================
#ifdef PLATFORM_TYPE_WIN32
//
inline size_t rs_wcstombs(char *mbstr, const wchar_t *wcstr, size_t count)
{
	return ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL); 
}

inline size_t rs_mbstowcs(wchar_t *wcstr, const char *mbstr, size_t count)
{
	return ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
}

#else
//
inline size_t rs_wcstombs(char *mbstr, const wchar_t *wcstr, size_t count)
{
	return wcstombs(mbstr, wcstr, count)+1;
}

inline size_t rs_mbstowcs(wchar_t *wcstr, const char *mbstr, size_t count)
{
	return mbstowcs(wcstr, mbstr, count)+1;
}

#endif



#endif

