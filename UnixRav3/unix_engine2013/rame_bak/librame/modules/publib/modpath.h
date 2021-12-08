#ifndef __MOD_PATH__
#define __MOD_PATH__

#if TARGET_OS==OS_WINDOWS

struct ModulePath
{
	std::string& Path;
	std::wstring& wPath;
	ModulePath( std::string& rPath, std::wstring &xPath ) : Path(rPath) , wPath(xPath)
	{
	}
	void Init( HMODULE hModule )
	{
		Path.resize(0x1000);
		DWORD len = ::GetModuleFileNameA((HMODULE)hModule, &Path[0], Path.size() );
		while(len-- && '\\' != Path[len]);
		Path.assign( Path.c_str(), Path.c_str() + len + 1 );

		wPath.resize(0x1000);
		len = ::GetModuleFileNameW((HMODULE)hModule, &wPath[0], wPath.size() );
		while(len-- && L'\\' != wPath[len]);
		wPath.assign( wPath.c_str(), wPath.c_str() + len + 1 );
	}
};
#else
struct ModulePath
{
	std::string& Path;
	ModulePath( std::string& rPath ) : Path(rPath)
	{
		Path.resize(0x1000);
		Path = getcwd( &Path[0], Path.size() );
		Path.push_back('/');
	}
};
#endif

#endif