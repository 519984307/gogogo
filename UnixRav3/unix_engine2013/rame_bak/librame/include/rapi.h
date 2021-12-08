#ifndef __K_API_DEF_
#define __K_API_DEF_

typedef struct _RFILE_FIND_DATA
{
	LARGE_INTEGER	CreationTime;
	LARGE_INTEGER	LastAccessTime;
	LARGE_INTEGER	LastWriteTime;
	LARGE_INTEGER	EndOfFile;
	DWORD			FileAttributes;
	LPCSTR			FullNameA;
	LPCWSTR			FullNameW;
} RFILE_FIND_DATA, *LPRFILE_FIND_DATA;

HRESULT WINAPI RApiInit( HMODULE hModule );
HRESULT WINAPI RFileCreateW( LPHANDLE lpHandle, LPCWSTR wFileName, UINT32 uMode, BOOL bSkipWin = FALSE );
HRESULT WINAPI RFileCreateA( LPHANDLE lpHandle, LPCSTR aFileName, UINT32 uMode );
HRESULT WINAPI RFileRead( HANDLE hFile, LPVOID vpData, DWORD cbToRead, LPDWORD lpEff );
HRESULT WINAPI RFileWrite( HANDLE hFile, LPCVOID vpData, DWORD cbToWrite, LPDWORD lpEff );
HRESULT WINAPI RFileFlush( HANDLE hFile );
HRESULT WINAPI RFileSetPointerEx( HANDLE hFile, LARGE_INTEGER liOff, ULARGE_INTEGER* pulNewOff, DWORD dwMoveMethod );
HRESULT WINAPI RFileSetEnd( HANDLE hFile );
HRESULT WINAPI RFileClose( HANDLE hFile );
HRESULT WINAPI RFileDeleteByHandle( HANDLE hFile );
HRESULT WINAPI RFileGetSizeEx( HANDLE hFile, ULARGE_INTEGER* lpSize64 );
HRESULT WINAPI RFileDeleteByNameA( LPCSTR aFileName );
HRESULT WINAPI RFileDeleteByNameW( LPCWSTR wFileName );
HRESULT WINAPI RFileEnumFirstW( LPHANDLE lpFind, LPCWSTR wDirectory, LPRFILE_FIND_DATA lpFindData );
HRESULT WINAPI RFileEnumFirstA( LPHANDLE lpFind, LPCSTR aDirectory, LPRFILE_FIND_DATA lpFindData );
HRESULT WINAPI RFileEnumNext( HANDLE hFind, LPRFILE_FIND_DATA lpFindData );
HRESULT WINAPI RFileEnumClose( HANDLE hFind );


#endif