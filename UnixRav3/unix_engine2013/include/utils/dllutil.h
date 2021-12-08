#ifndef __RE_DLLUTIL_H__
#define __RE_DLLUTIL_H__

#if defined(PLATFORM_TYPE_POSIX)
//#include <dlfcn.h>
//#include <stdio.h>
#endif

struct CDllUtil
{
#if !defined(PLATFORM_TYPE_POSIX)
	static HMODULE WINAPI LoadLibraryA( LPCSTR lpLibFileName )
	{
		return ::LoadLibraryA(lpLibFileName);
	}
	static HMODULE WINAPI LoadLibraryW( LPCWSTR lpLibFileName )
	{
		return ::LoadLibraryW(lpLibFileName);
	}
	static FARPROC WINAPI GetProcAddress( HMODULE hModule, LPCSTR lpProcName )
	{
		return ::GetProcAddress(hModule, lpProcName);
	}

	static BOOL WINAPI FreeLibrary( HMODULE hLibModule )
	{
		return ::FreeLibrary(hLibModule);
	}

	//////////////////////////////////////////////////////////////////////////
#if 0
	static BOOL SetEngineDllDirectory( HMODULE hModule )
	{
		std::wstring tmp; tmp.resize(2048);

		DWORD len = ::GetModuleFileNameW( hModule, &tmp[0], tmp.size() );
		if( 0 == len )
		{
			//PRINT_INFO( "[RAME] GetModuleFileName failed. errorcode = %d\n", ::GetLastError() );
			return FALSE;
		}
		WCHAR * p = (WCHAR *)tcsrchr( tmp.c_str(), '\\' );
		if( p ) *p = L'\x00';
		if( SetDllDirectoryW(tmp.c_str()) )
		{
			//PRINT_INFO( "[RAME] SetDllDirectory %ws successfully.\n", tmp.c_str() );
			return TRUE;
		}
		else
		{
			//PRINT_INFO( "[RAME] SetDllDirectory %ws failed. errorcode = %d\n", tmp.c_str(), ::GetLastError() );
			return FALSE;
		}
	}

	static BOOL RestoreDllDirectory()
	{
		if( SetDllDirectoryW(NULL) )
		{
			//PRINT_INFO( "[RAME] SetDllDirectory null successfully.\n" );
			return TRUE;
		}
		else
		{
			//PRINT_INFO( "[RAME] SetDllDirectory null failed. errorcode = %d\n", ::GetLastError() );
			return FALSE;
		}
	}
#endif

#else //PLATFORM_TYPE_POSIX
	static HMODULE WINAPI LoadLibraryA( LPCSTR lpLibFileName )
	{
		HMODULE h = dlopen(lpLibFileName, RTLD_LAZY);
		if (!h)
		{
//			printf("%s\n", dlerror());
			//fputs(dlerror(), stderr);
		}
		return h;
	}

	static FARPROC WINAPI GetProcAddress( HMODULE hModule, LPCSTR lpProcName )
	{
		return (FARPROC)dlsym(hModule, lpProcName);
	}

	static BOOL WINAPI FreeLibrary( HMODULE hLibModule )
	{
		return dlclose(hLibModule);
	}
#endif //PLATFORM_TYPE_POSIX
};


#endif //__RE_DLLUTIL_H__
