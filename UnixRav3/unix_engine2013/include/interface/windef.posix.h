#ifndef _PUBPOSIX_H_
#define _PUBPOSIX_H_

#if (TARGET_OS==OS_POSIX)

#include <ctype.h>
#include <string.h>
#include <wchar.h>

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field)    ((long)(long*)&(((type *)0)->field))
#endif

// <basetsd.h> ================================================================
//
//
#if TARGET_BITS == 64
typedef UINT64			ULONG_PTR;
#else
typedef UINT32			ULONG_PTR;
#endif

typedef ULONG_PTR DWORD_PTR, *	PDWORD_PTR;

#define MOVEFILE_REPLACE_EXISTING       0x00000001
#define MOVEFILE_COPY_ALLOWED           0x00000002
#define MOVEFILE_DELAY_UNTIL_REBOOT     0x00000004
#define MOVEFILE_WRITE_THROUGH          0x00000008

#define CP_UTF7							65000       // UTF-7 translation
#define CP_UTF8							65001       // UTF-8 translation

#define	FORCEINLINE						inline

// <winnt.h> ================================================================
// Basics
#ifndef VOID
	#define VOID void
#endif

typedef int						BOOL;
typedef DWORD					*PDWORD;
typedef DWORD					*LPDWORD;
typedef BOOL					*PBOOL;
typedef BOOL					*LPBOOL;
typedef BYTE					*PBYTE;
typedef BYTE					*LPBYTE;
typedef const BYTE				*LPCBYTE;
typedef WORD					*PWORD;
typedef WORD					*LPWORD;
typedef QWORD					*PQWORD;
typedef QWORD					*LPQWORD;
//
typedef int						INT;
typedef unsigned int			UINT;
typedef unsigned int			*PUINT;
typedef long                    NTSTATUS;
typedef char					CHAR;
#if defined(UNICODE)||defined(_UNICODE)
typedef wchar_t					TCHAR;
#else
typedef char					TCHAR;
#endif
typedef unsigned char			UCHAR;
typedef signed short			SHORT;
typedef unsigned short			USHORT;
typedef INT32					LONG;
//typedef DWORD					ULONG;
typedef float					FLOAT;
typedef double					DOUBLE;
typedef LONG					SCODE;
typedef short					VARIANT_BOOL;
typedef int						INT;
typedef unsigned int			UINT;
typedef size_t					SIZE_T;
// Pointer to Basics
typedef void *					PVOID;
typedef void *					LPVOID;
typedef const void *			LPCVOID;
typedef SHORT *					PSHORT;
typedef LONG *					PLONG;
// UNICODE Types
typedef wchar_t					WCHAR;
typedef WCHAR *					PWCHAR;
typedef WCHAR *					PWSTR;
typedef WCHAR *					LPWSTR;
typedef const WCHAR *			PCWSTR;
typedef const WCHAR *			LPCWSTR;
// ANSI Types
typedef CHAR *					PCHAR;
typedef CHAR *					PSTR;
typedef CHAR *					LPSTR;
typedef const CHAR *			PCSTR;
typedef const CHAR *			LPCSTR;
// TCHAR types
typedef const CHAR *			LPCTSTR;
// HANDLE
typedef void *					HANDLE;
typedef HANDLE *				LPHANDLE;
// HMODULE
typedef void *					HMODULE;

typedef LONGLONG *PLONGLONG;
typedef ULONGLONG *PULONGLONG;
typedef union _LARGE_INTEGER {
    struct __S { // avoid FreeBSD warning: anonymous class type not used to declare any objects
	#if !defined(ARCH_BIG_ENDIAN)
		UINT32 LowPart;
		INT32 HighPart;
	#else
		INT32 HighPart;
		UINT32 LowPart;
	#endif
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
typedef union _ULARGE_INTEGER {
    struct __S { // avoid FreeBSD warning: anonymous class type not used to declare any objects
	#if !defined(ARCH_BIG_ENDIAN)
		UINT32 LowPart;
		UINT32 HighPart;
	#else
		UINT32 HighPart;
		UINT32 LowPart;
	#endif
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;
// COM Defines and Macros
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef LONG HRESULT;
#endif // !_HRESULT_DEFINED
//
#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif
//

#define __forceinline
#define __fastcall

#ifndef INLINE
#ifdef __cplusplus
#define INLINE    inline
#else
#define INLINE
#endif
#endif // INLINE
//
#define DLLEXPORT
#define DLLIMPORT
//
#define __cdecl
#define __stdcall

#define STDMETHODCALLTYPE       __stdcall
#define STDMETHODVCALLTYPE      __cdecl

#define STDAPICALLTYPE          __stdcall
#define STDAPIVCALLTYPE         __cdecl

#define STDAPI                  EXTERN_C HRESULT STDAPICALLTYPE
#define STDAPI_(type)           EXTERN_C type STDAPICALLTYPE

#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE

#define STDAPIV                 EXTERN_C HRESULT STDAPIVCALLTYPE
#define STDAPIV_(type)          EXTERN_C type STDAPIVCALLTYPE

#define STDMETHODIMPV           HRESULT STDMETHODVCALLTYPE
#define STDMETHODIMPV_(type)    type STDMETHODVCALLTYPE

//
#define CALLBACK    __stdcall

typedef int (CALLBACK *FARPROC)();

//
#define GENERIC_READ						(0x80000000L)
#define GENERIC_WRITE						(0x40000000L)
#define GENERIC_EXECUTE						(0x20000000L)
#define GENERIC_ALL							(0x10000000L)
//
#define FILE_SHARE_READ						0x00000001
#define FILE_SHARE_WRITE					0x00000002
#define FILE_SHARE_DELETE					0x00000004
#define FILE_ATTRIBUTE_READONLY				0x00000001
#define FILE_ATTRIBUTE_HIDDEN				0x00000002
#define FILE_ATTRIBUTE_SYSTEM				0x00000004
#define FILE_ATTRIBUTE_DIRECTORY			0x00000010
#define FILE_ATTRIBUTE_ARCHIVE				0x00000020
#define FILE_ATTRIBUTE_DEVICE				0x00000040
#define FILE_ATTRIBUTE_NORMAL				0x00000080
#define FILE_ATTRIBUTE_TEMPORARY			0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE			0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT		0x00000400
#define FILE_ATTRIBUTE_COMPRESSED			0x00000800
#define FILE_ATTRIBUTE_OFFLINE				0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	0x00002000
#define FILE_ATTRIBUTE_ENCRYPTED			0x00004000
//
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16
#define IMAGE_NT_OPTIONAL_HDR32_MAGIC		0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC		0x20b
#define IMAGE_DIRECTORY_ENTRY_RESOURCE		2   // Resource Directory
#define IMAGE_SIZEOF_SHORT_NAME             8
#define IMAGE_RESOURCE_NAME_IS_STRING       0x80000000
#define IMAGE_RESOURCE_DATA_IS_DIRECTORY    0x80000000
//
#define DLL_PROCESS_ATTACH   				1
#define DLL_THREAD_ATTACH    				2
#define DLL_THREAD_DETACH    				3
#define DLL_PROCESS_DETACH   				0

// <windef.h> ================================================================
//
// BASETYPES
typedef ULONG *								PULONG;
typedef USHORT *							PUSHORT;
typedef UCHAR *								PUCHAR;
//
#define MAX_PATH          260
//
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
//
#ifndef FALSE
#define FALSE               0
#endif
//
#ifndef TRUE
#define TRUE                1
#endif
//
#ifndef IN
#define IN
#endif
//
#ifndef OUT
#define OUT
#endif
//
#ifndef OPTIONAL
#define OPTIONAL
#endif
//
#define WINAPI					__stdcall
#define APIENTRY    WINAPI
//
#ifndef CONST
#define CONST               const
#endif
//
//
/*!!!
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
!!!*/
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))


// <winbase.h> ================================================================
//
#define INVALID_HANDLE_VALUE			(HANDLE)-1
#define INVALID_FILE_ATTRIBUTES			(ULONG)-1
#define INVALID_FILE_SIZE				(ULONG)-1
#define INVALID_SET_FILE_POINTER		(ULONG)-1
//
#define FILE_FLAG_WRITE_THROUGH         0x80000000
#define FILE_FLAG_OVERLAPPED            0x40000000
#define FILE_FLAG_NO_BUFFERING          0x20000000
#define FILE_FLAG_RANDOM_ACCESS         0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN       0x08000000
#define FILE_FLAG_DELETE_ON_CLOSE       0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS      0x02000000
#define FILE_FLAG_POSIX_SEMANTICS       0x01000000
#define FILE_FLAG_OPEN_REPARSE_POINT    0x00200000
#define FILE_FLAG_OPEN_NO_RECALL        0x00100000
#define FILE_FLAG_FIRST_PIPE_INSTANCE   0x00080000
//
#define CREATE_NEW				1
#define CREATE_ALWAYS			2
#define OPEN_EXISTING			3
#define OPEN_ALWAYS				4
#define TRUNCATE_EXISTING		5

#define FILE_TYPE_UNKNOWN   0x0000
#define FILE_TYPE_DISK      0x0001
#define FILE_TYPE_CHAR      0x0002
#define FILE_TYPE_PIPE      0x0003
#define FILE_TYPE_REMOTE    0x8000
//
#define FILE_BEGIN				0
#define FILE_CURRENT			1
#define FILE_END				2
//
typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef const FILETIME *PCFILETIME, *LPCFILETIME;
//
typedef enum _GET_FILEEX_INFO_LEVELS {
    GetFileExInfoStandard,
    GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;
//
typedef struct _SECURITY_ATTRIBUTES{
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
//
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union __U { // avoid gcc error: an anonymous union can only have non-static data members
        struct __S { // avoid FreeBSD warning: anonymous class type not used to declare any objects
		#if !defined(ARCH_BIG_ENDIAN)
			DWORD Offset;
			DWORD OffsetHigh;
		#else
			DWORD OffsetHigh;
			DWORD Offset;
		#endif
        } u;
        PVOID Pointer;
    };
    HANDLE  hEvent;
} OVERLAPPED, *POVERLAPPED, *LPOVERLAPPED;
//
typedef struct _BY_HANDLE_FILE_INFORMATION {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD dwVolumeSerialNumber;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD nNumberOfLinks;
    DWORD nFileIndexHigh;
    DWORD nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
//
typedef struct _WIN32_FIND_DATAW {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    WCHAR  cFileName[ MAX_PATH ];
    WCHAR  cAlternateFileName[ 14 ];
} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
//
typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ MAX_PATH ];
    CHAR   cAlternateFileName[ 14 ];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
//
typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;

// <winbase.h> ================================================================
//
#define MAKEINTRESOURCEW(i) (LPWSTR)((ULONG_PTR)((WORD)(i)))
#define MAKERESOURCEWINT(s) (DWORD)((ULONG_PTR)((LPWSTR)(s)))

// <objbase.h> ================================================================
//
#define interface struct
#define STDMETHOD(method)			virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)		virtual type STDMETHODCALLTYPE method
#define STDMETHODV(method)			virtual HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_(type,method)	virtual type STDMETHODVCALLTYPE method
#define PURE = 0

// <wtypes.h> ================================================================
//
typedef enum tagSTATFLAG {
	STATFLAG_DEFAULT	= 0,
	STATFLAG_NONAME	= 1,
	STATFLAG_NOOPEN	= 2
} STATFLAG;
//
typedef USHORT OLECHAR;
typedef OLECHAR *LPOLESTR;
typedef const USHORT *LPCOLESTR;
#define OLECH(ch) OLECHAR(ch)

typedef /* [wire_marshal] */ OLECHAR *BSTR;

// <ntddk.h> ================================================================
//
#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005


// <objidl.h> ================================================================
typedef struct _GUID {
	unsigned int  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;
typedef GUID CLSID;
typedef GUID IID;
#ifdef __cplusplus
	#define REFGUID const GUID &
	#define REFIID const IID &
	#define REFCLSID const IID &
#else
	#define REFGUID const GUID *
	#define REFIID const IID *
	#define REFCLSID const IID *
#endif

typedef CLSID*		LPCLSID;
typedef GUID*		LPCGUID;

//
typedef struct tagSTATSTG
    {
    LPOLESTR pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
    } 	STATSTG;

typedef enum tagSTGTY
    {
	STGTY_STORAGE	= 1,
	STGTY_STREAM	= 2,
	STGTY_LOCKBYTES	= 3,
	STGTY_PROPERTY	= 4
    } 	STGTY;

//winnt.h
typedef DWORD SECURITY_INFORMATION, *PSECURITY_INFORMATION;
typedef PVOID PSECURITY_DESCRIPTOR;
typedef struct _ACL {
    BYTE  AclRevision;
    BYTE  Sbz1;
    WORD   AclSize;
    WORD   AceCount;
    WORD   Sbz2;
} ACL;
typedef ACL *PACL;

#define VARIANT_TRUE 	((VARIANT_BOOL)-1)
#define VARIANT_FALSE 	((VARIANT_BOOL)0)

//////////////////////////////////////////////////////////////////////////

//typedef NTSTATUS (STDAPICALLTYPE *ProcDllInitialize)(IN PUNICODE_STRING);
//typedef NTSTATUS (STDAPICALLTYPE *ProcDllUnload)();

#endif // (TARGET_OS==OS_POSIX)

#endif // _PUBPOSIX_H_
