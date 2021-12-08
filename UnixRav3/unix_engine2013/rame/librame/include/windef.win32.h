#ifndef _PUBWIN32_H_
#define _PUBWIN32_H_

#if (TARGET_OS==OS_WINDOWS)

#include <windows.h>

// <winnt.h> ================================================================
//
#ifndef INLINE
#ifdef __cplusplus
#define INLINE    inline
#else
#define INLINE
#endif
#endif // INLINE
//
#define DLLEXPORT			__declspec(dllexport)
#define DLLIMPORT			__declspec(dllimport)	

// <windef.h> ================================================================
//
typedef BYTE*				LPBYTE;
typedef const BYTE *		LPCBYTE;
typedef unsigned __int64	QWORD;
typedef QWORD	*PQWORD,	*LPQWORD;

#define MAKELONG64(a, b)	((LONGLONG)(((DWORD)((QWORD)(a) & 0xffffffff)) | ((QWORD)((DWORD)((QWORD)(b) & 0xffffffff))) << 32))
#define LOLONG(q)           ((WORD)((QWORD)(q) & 0xffffffff))
#define HILONG(q)           ((WORD)((QWORD)(q) >> 32))

// <winbase.h> ================================================================
//
#if (_MSC_VER < 1300)
#define INVALID_SET_FILE_POINTER	((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES		((DWORD)-1)
#endif
//
typedef const FILETIME *PCFILETIME, *LPCFILETIME;

// <winbase.h> ================================================================
//
// #define MAKEINTRESOURCEW(i) (LPWSTR)((ULONG_PTR)((WORD)(i)))
#define MAKERESOURCEWINT(s) (DWORD)((ULONG_PTR)((LPWSTR)(s))) 

// <objbase.h> ================================================================
// 
#include <objbase.h>

// <wtypes.h> ================================================================
//

// <ntddk.h> ================================================================
//
#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

#define ALIGN_DOWN(length, type) \
	((ULONG)(length) & ~(sizeof(type) - 1))

#define ALIGN_UP(length, type) \
	(ALIGN_DOWN(((ULONG)(length) + sizeof(type) - 1), type))

#endif // (TARGET_OS==OS_WINDOWS)

#endif // _PUBWIN32_H_
