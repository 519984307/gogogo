#ifndef __RX_FILE_UTIL_H_H__
#define __RX_FILE_UTIL_H_H__


#if (TARGET_OS == OS_WINDOWS)
#include <io.h>
#else
#include <unistd.h>
#endif

namespace FileUtil {;



inline BOOL PathIsExist( LPCSTR path ){
#if (TARGET_OS == OS_WINDOWS)
	return -1 != _access( path, 0 );
#else
	return 0 == access( path, F_OK );
#endif
}


inline BOOL IsFile( LPCSTR path ) {
	RASSERT( path, FALSE );
#if (TARGET_OS == OS_WINDOWS)
	DWORD dwFileAttr = GetFileAttributesA( path );
	if( dwFileAttr == INVALID_FILE_ATTRIBUTES ) return FALSE;
	if( dwFileAttr & FILE_ATTRIBUTE_REPARSE_POINT ) return FALSE;
	return FILE_ATTRIBUTE_DIRECTORY & dwFileAttr ? FALSE : TRUE;
#else
	struct stat statInfo;
	if(stat(path, &statInfo) != 0) return FALSE;
	return S_ISREG(statInfo.st_mode) ? TRUE : FALSE;
#endif
}


#if (TARGET_OS == OS_WINDOWS)
static int SetWindowsPath(tcs::xstr_t & ThisDir, tcs::xstr_t & ThisPath)
{
	std::string tmp; tmp.resize(2048);
	DWORD len = ::GetModuleFileNameA( GetModuleHandleA(0), &tmp[0], tmp.size() );
	ThisPath.set( 0, tmp.c_str() );
	char * p = rs_strrchr( (LPSTR)tmp.c_str(), '\\' );
	if( p ) *p = '\x00';
	ThisDir.set( 0, tmp.c_str() );
	return 0;
}
#else
static int SetLinuxPath(tcs::xstr_t & ThisDir, tcs::xstr_t & ThisPath)
{
	std::string tmp; tmp.resize(2048);
	ssize_t len = ::readlink("/proc/self/exe", &tmp[0], tmp.size() - 1);
	if(len <= 0 || len == tmp.size() - 1) {
		// fail
		return -1;
	}
	tmp[len] = 0;
	ThisPath.set( 0, tmp.c_str() );
	char * p = rs_strrchr( (LPSTR)tmp.c_str(), '/' );
	if( p ) *p = '\x00';
	ThisDir.set( 0, tmp.c_str() );
	return 0;
}
static int SetLinuxPath2(tcs::xstr_t & ThisDir, tcs::xstr_t & ThisPath)
{
	std::string tmp; tmp.resize(2048);
	if(NULL == realpath("./", &tmp[0])) {
		// fail
		return -1;
	}
	ThisDir.set( 0, tmp.c_str() );
	rs_strcat( &tmp[strlen(tmp.c_str())],"/conscan" );
	ThisPath.set( 0, tmp.c_str() );
	return 0;
}
#endif // (TARGET_OS == OS_WINDOWS)

inline int GetCurCmdPath(tcs::xstr_t & ThisDir, tcs::xstr_t & ThisPath)
{
#if (TARGET_OS == OS_WINDOWS)
	return SetWindowsPath(ThisDir, ThisPath);
#else
	int l = SetLinuxPath(ThisDir, ThisPath);
	if(-1 != l) return l;
	return SetLinuxPath2(ThisDir, ThisPath);
#endif
}

//////////////////////////////////////////////////////////////////////////

#if (TARGET_OS == OS_WINDOWS)
static int SetWindowsPath(std::string & work_dir)
{
	std::string tmp; tmp.resize(2048);
	DWORD len = ::GetModuleFileNameA( GetModuleHandleA(0), &tmp[0], tmp.size() );
	char * p = tcsrchr( tmp.c_str(), '\\' );
	if( p ) *p = '\x00';
	work_dir = tmp;
	return 0;
}
#else
static int SetLinuxPath(std::string & work_dir)
{
	std::string tmp; tmp.resize(2048);
	ssize_t len = ::readlink("/proc/self/exe", &tmp[0], tmp.size() - 1);
	if(len <= 0 || len == tmp.size() - 1) {
		// fail
		return -1;
	}
	tmp[len] = 0;
	char * p = tcsrchr( tmp.c_str(), '/' );
	if( p ) *p = '\x00';
	work_dir = tmp;
	return 0;
}
static int SetLinuxPath2(std::string & work_dir)
{
	std::string tmp; tmp.resize(2048);
	if(NULL == realpath("./", &tmp[0])) {
		// fail
		return -1;
	}
	work_dir = tmp;
	return 0;
}
#endif // (TARGET_OS == OS_WINDOWS)

inline int GetCurWorkPath(std::string & work_dir)
{
#if (TARGET_OS == OS_WINDOWS)
	return SetWindowsPath(work_dir);
#else
	int l = SetLinuxPath(work_dir);
	if(-1 != l) return l;
	return SetLinuxPath2(work_dir);
#endif
}


};


#endif //__RX_FILE_UTIL_H_H__