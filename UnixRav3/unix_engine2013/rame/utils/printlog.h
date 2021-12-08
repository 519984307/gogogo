#ifndef __RS_PRINT_LOG_H__
#define __RS_PRINT_LOG_H__

#if (TARGET_OS != OS_WINDOWS)
#include <syslog.h>
#endif

inline void PRINT_INFO( const char* format, ... )
{
	const static int nCnt = 2048;
	char szBuffer[nCnt] = {};
#if (TARGET_OS == OS_WINDOWS)
	int nLength = snprintf( szBuffer, nCnt, "[%d] ", GetCurrentThreadId());
#else
	int nLength = snprintf( szBuffer, nCnt, "[0x%lx] ", pthread_self());
#endif

	va_list arg_ptr;
	va_start( arg_ptr, format );
	nLength += vsnprintf( szBuffer+nLength, nCnt-nLength, format, arg_ptr );
	va_end( arg_ptr );

#if (TARGET_OS == OS_WINDOWS)
	OutputDebugStringA( szBuffer );
#else
	printf("%s", szBuffer );
	openlog("RSlogview", LOG_CONS | LOG_PID, 0);
	syslog(LOG_USER | LOG_INFO, "%s", szBuffer );
	closelog(); 
#endif
}



/*
#define RFAILED		RFAILED_LOG
#define RASSERT		RASSERT_LOG

#define RFAILED_LOG(x) \
{\
	HRESULT _h_r_ = (x); \
	if(FAILED(_h_r_)) \
	{ \
		PRINT_INFO(__FUNCTION__ " L(%d) %s failed *** hr: 0x%08x \r\n", __LINE__, #x, _h_r_); \
		return _h_r_; \
	} \
}

#define RASSERT_LOG(x, _h_r_) \
{ \
	if(!(x)) \
	{ \
		PRINT_INFO(__FUNCTION__ " L(%d) %s error *** hr: 0x%08x \r\n", __LINE__, #x, _h_r_); \
		return _h_r_; \
	} \
}
*/

#endif //__RS_PRINT_LOG_H__
