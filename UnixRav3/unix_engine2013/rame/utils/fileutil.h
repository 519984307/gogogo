#ifndef __RX_FILE_UTIL_H_H__
#define __RX_FILE_UTIL_H_H__


#if (TARGET_OS == OS_WINDOWS)
#include <io.h>
#else
#include <unistd.h>
#endif

namespace FileUtil {;



inline BOOL PathIsExist( LPCSTR path ){
#ifdef _WIN32
	return -1 != _access( path, 0 );
#else
	return 0 == access( path, F_OK );
#endif
}


};


#endif //__RX_FILE_UTIL_H_H__