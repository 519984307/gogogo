#ifndef _PUBNATIVE_H_
#define _PUBNATIVE_H_

#if (TARGET_OS==OS_NATIVE)

// <winnt.h> ================================================================
// Pointer to Basics
typedef void		*PVOID;
typedef void		*LPVOID;
typedef const void	*LPCVOID;
// HANDLE 
typedef HANDLE *	LPHANDLE;
//
#ifndef INLINE
#ifdef __cplusplus
#define INLINE    inline
#else
#define INLINE
#endif
#endif // INLINE
//
#define DLLEXPORT	__declspec(dllexport)
#define DLLIMPORT	__declspec(dllimport)	
//
#define DLL_PROCESS_ATTACH   1    
#define DLL_THREAD_ATTACH    2    
#define DLL_THREAD_DETACH    3    
#define DLL_PROCESS_DETACH   0    

//////////////////////////////////////////////////////////////////////////

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field)    ((long)(long*)&(((type *)0)->field))
#endif

// <windef.h> ================================================================
//
#define MAX_PATH 260
//
#define CALLBACK    __stdcall
#define WINAPI		__stdcall
#define PASCAL      __stdcall
#define APIENTRY    WINAPI
//
typedef unsigned int		DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef UINT64				QWORD;
typedef DWORD	*PDWORD,	*LPDWORD;
typedef BOOL	*PBOOL,		*LPBOOL;
typedef BYTE	*PBYTE,		*LPBYTE;
typedef const	BYTE		*LPCBYTE;
typedef int		*PINT,		*LPINT;
typedef WORD	*PWORD,		*LPWORD;
typedef QWORD	*PQWORD,	*LPQWORD;
//
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
//
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
//
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
//
typedef int (CALLBACK *FARPROC)();
//
// DECLARE_HANDLE(HKEY);
typedef HANDLE HKEY;
typedef HKEY *	PHKEY;

DECLARE_HANDLE(HMODULE); 
DECLARE_HANDLE(HINSTANCE);

// <winbase.h> ================================================================
//
#define INVALID_HANDLE_VALUE	(HANDLE)-1
#define INVALID_FILE_ATTRIBUTES	(ULONG)-1
#define INVALID_FILE_SIZE		(ULONG)-1
#define INVALID_SET_FILE_POINTER	(ULONG)-1
//
#define FILE_BEGIN				0
#define FILE_CURRENT			1
#define FILE_END				2
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
//
#define FILE_TYPE_UNKNOWN   0x0000
#define FILE_TYPE_DISK      0x0001
#define FILE_TYPE_CHAR      0x0002
#define FILE_TYPE_PIPE      0x0003
#define FILE_TYPE_REMOTE    0x8000
//
#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)
//
typedef struct _FILETIME{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef const FILETIME *PCFILETIME, *LPCFILETIME;
//
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;
VOID
WINAPI
InitializeCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection
    );
VOID
WINAPI
DeleteCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection
    );
VOID
WINAPI
EnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection
    );
VOID
WINAPI
LeaveCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection
    );

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
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } U;
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
#ifndef UNICODE
typedef WIN32_FIND_DATAA	WIN32_FIND_DATA;
typedef WIN32_FIND_DATAA	*LPWIN32_FIND_DATA;	
#else
typedef WIN32_FIND_DATAW	WIN32_FIND_DATA;
typedef WIN32_FIND_DATAW	*LPWIN32_FIND_DATA;
#endif
//
typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;
//
DWORD
WINAPI
GetModuleFileNameA(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
    );
HMODULE
WINAPI
GetModuleHandleA(
    LPCSTR lpModuleName
    );
HMODULE
WINAPI
LoadLibraryA(
    LPCSTR lpLibFileName
    );
FARPROC
WINAPI
GetProcAddress (
    HMODULE hModule,
    LPCSTR lpProcName
    );
BOOL
WINAPI
FreeLibrary(
			HMODULE hLibModule
			);
DWORD
WINAPI
GetVersion();


HANDLE
WINAPI
GetStdHandle(
			 IN DWORD nStdHandle
			 );

DWORD
WINAPI
GetTickCount(
			 VOID
			 );
BOOL 
WINAPI
UnmapViewOfFile(
  LPCVOID lpBaseAddress
);


//为了和posttrt不发生冲突，此处用extern C
extern "C"
{
	DWORD 
		WINAPI
		GetFileAttributesA(
		IN LPCSTR lpFileName
		);



	BOOL
		WINAPI
		CloseHandle(
		IN OUT HANDLE hObject
		);

	BOOL
		WINAPI
		FindClose(
		IN OUT HANDLE hFindFile
		);


	BOOL
		WINAPI
		HeapFree(
		IN HANDLE hHeap,
		IN DWORD dwFlags,
		IN LPVOID lpMem
		);
};


//
#ifndef UNICODE
#define lstrcpy	strcpy
#define lstrcpyn strncpy
#define lstrlen	strlen
#define lstrcmp	strcmp
#define lstrcmpi _stricmp
#else
#define lstrcpy wcscpy
#define lstrcpyn wcsncpy
#define lstrlen wcslen
#define lstrcmp wcscmp
#define lstrcmpi _wcsicmp
#endif


// <winreg.h> ================================================================
//
typedef ACCESS_MASK REGSAM;

#define HKEY_CLASSES_ROOT           (( HKEY ) (ULONG_PTR)((LONG)0x80000000) )
#define HKEY_CURRENT_USER           (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
#define HKEY_LOCAL_MACHINE          (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
#define HKEY_USERS                  (( HKEY ) (ULONG_PTR)((LONG)0x80000003) )
#define HKEY_PERFORMANCE_DATA       (( HKEY ) (ULONG_PTR)((LONG)0x80000004) )
#define HKEY_PERFORMANCE_TEXT       (( HKEY ) (ULONG_PTR)((LONG)0x80000050) )
#define HKEY_PERFORMANCE_NLSTEXT    (( HKEY ) (ULONG_PTR)((LONG)0x80000060) )
#define HKEY_CURRENT_CONFIG         (( HKEY ) (ULONG_PTR)((LONG)0x80000005) )
#define HKEY_DYN_DATA               (( HKEY ) (ULONG_PTR)((LONG)0x80000006) )



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
typedef wchar_t OLECHAR;
typedef OLECHAR *LPOLESTR;
typedef const OLECHAR *LPCOLESTR;
#define OLESTR(str) L##str
#define OLECH(ch) L##ch
//
typedef short VARIANT_BOOL;
#define _VARIANT_BOOL /##/
typedef LONG SCODE;
typedef struct tagCY
    {
    LONGLONG int64;
    } 	CY;
typedef double DATE;
typedef struct tagBLOB
    {
    ULONG cbSize;
    /* [size_is] */ BYTE *pBlobData;
    } 	BLOB;
typedef struct tagCLIPDATA
    {
    ULONG cbSize;
    long ulClipFmt;
    /* [size_is] */ BYTE *pClipData;
    } 	CLIPDATA;
typedef /* [wire_marshal] */ OLECHAR *BSTR;
typedef struct tagBSTRBLOB
    {
    ULONG cbSize;
    /* [size_is] */ BYTE *pData;
    } 	BSTRBLOB;


// <objidl.h> ================================================================
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
    {	STGTY_STORAGE	= 1,
	STGTY_STREAM	= 2,
	STGTY_LOCKBYTES	= 3,
	STGTY_PROPERTY	= 4
    } 	STGTY;



// <objidl.h> ================================================================
//




//<locale.h>  ================================================================
#define LC_ALL          0
#define LC_COLLATE      1
#define LC_CTYPE        2
#define LC_MONETARY     3
#define LC_NUMERIC      4
#define LC_TIME         5

extern "C" char * __cdecl setlocale(int,char const *);



//<wincon.h> ==================================================================
//

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.


BOOL
WINAPI
SetConsoleTextAttribute(
						IN HANDLE hConsoleOutput,
						IN WORD wAttributes);






#endif // (TARGET_OS==OS_NATIVE)

#endif // _PUBNATIVE_H_
