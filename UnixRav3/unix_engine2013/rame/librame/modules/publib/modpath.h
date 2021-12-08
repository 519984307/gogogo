#ifndef __MOD_PATH__
#define __MOD_PATH__


#if !defined RISING_SDK

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
#endif //TARGET_OS==OS_WINDOWS

#else //!defined RISING_SDK

template < size_t FIXB >
struct LSBuffer
{
	BYTE	_Fix[FIXB];
	LPVOID	_Ptr;
	LSBuffer( SIZE_T cb )
	{
		if( cb < FIXB ) 
		{
			_Ptr = _Fix;
			memset( _Fix, 0, FIXB );
		}
		else 
		{
			_Ptr = new BYTE[cb];
			memset( _Fix, 0, cb );
		}
	}
	~LSBuffer()
	{
		if( !_Ptr ) return;
		if( _Fix == _Ptr ) return;
		delete [] (PBYTE)_Ptr;
	}
	template < class T >
	operator T*() 
	{
		return (T*)_Ptr;
	}
};

struct ModulePath
{
	std::string& Path;
	std::string license_path;

#if TARGET_OS==OS_WINDOWS
	std::wstring& wPath;
	ModulePath( std::string& rPath, std::wstring &xPath ) : Path(rPath) , wPath(xPath)
	{
	}
	void Init( HMODULE hModule )
	{
		// ansi
		Path.resize(0x1000);
		DWORD len = ::GetModuleFileNameA((HMODULE)hModule, &Path[0], Path.size() );
		while(len-- && '\\' != Path[len]);
		Path.assign( Path.c_str(), Path.c_str() + len + 1 );
		
		std::string cfgfilePath = Path + "rame.cfg";
		std::string cfgPath;
		if( read_path( cfgfilePath.c_str(), cfgPath ) )
			Path = cfgPath;

		// unicode
		wPath.resize(0x1000);
		len = ::GetModuleFileNameW((HMODULE)hModule, &wPath[0], wPath.size() );
		while(len-- && L'\\' != wPath[len]);
		wPath.assign( wPath.c_str(), wPath.c_str() + len + 1 );
	}
#else
	ModulePath( std::string& rPath ) : Path(rPath)
	{
		std::string cur_path;
		cur_path.resize(0x1000);
		cur_path = getcwd( &cur_path[0], cur_path.size() );
		cur_path.push_back('/');
		cur_path.append("rame.cfg");
		read_path( cur_path.c_str(), Path );
	}
#endif //TARGET_OS==OS_WINDOWS

	inline BOOL contain(char c, const char sp_arr[], int sp_cnt)
	{
		for (int i=0; i<sp_cnt; i++)
		{
			if(c == sp_arr[i]) return TRUE;
		}
		return FALSE;
	}
	const char * pop_name(const char * & p, int & len, const char sp_arr[], int sp_cnt)
	{
		const char * ret = 0;
		len = 0;
		while (*p && !len) {
			ret = p;
			for (; *p && !contain(*p, sp_arr, sp_cnt); ++p, ++len);
			if (!len) ++p;
		}
		if (*p) ++p;
		return len ? ret : NULL;
	}
	BOOL read_path( LPCSTR cur_path, std::string& rame_path )
	{
		BOOL ret = FALSE;
		FILE * fp = NULL;
		do
		{
			fp = fopen( cur_path, "rb" );
			if( NULL == fp ) return ret;
			fseek( fp, 0, SEEK_END );
			int file_len = ftell(fp);
			fseek( fp, 0, SEEK_SET );
			LSBuffer<512> buffer( file_len );
			if( file_len != fread( buffer, 1, file_len, fp ) )
				break;
			const char* pstart = (char*)buffer;
			const char* pend = (char*)buffer + file_len;
			//printf("@@ 1 ||%s\n", pstart);
			while( pstart < pend )
			{
				int32_t length;
				const char sp_arr[] = {'\r', '\n'};
				const char* ptr = pop_name( pstart, length, sp_arr, sizeof(sp_arr) );
				if( NULL == ptr )
					break;;
				//printf("@@ 2 ||%s\n", ptr);
				std::string sub, key;
				sub.assign( ptr, 0, length );
				int pos = sub.find( ':', 0 );
				key.assign( sub, 0, pos );
				//printf("@@ 3 ||%s\n", key.c_str());
				if( key == "PATH")
				{
					rame_path.assign( ptr+pos+1, ptr+length );
					rame_path.push_back('/');
					//printf("@@ 4 ||%s\n", rame_path.c_str());
					ret = TRUE;
				}
				if( key == "LICENSE_PATH")
				{
					license_path.assign( ptr+pos+1, ptr+length );
					//printf("@@ 5 ||%s\n", license_path.c_str());
				}
			}
		}
		while( false );
		if( fp ) fclose( fp );
		return ret;
	}
};

#endif //!defined RISING_SDK

#endif