#ifndef __RS_PRINT_LOG_H__
#define __RS_PRINT_LOG_H__


inline void PRINT_INFO( const char* format, ... )
{
	char szBuffer[MAX_PATH] = {};
	int nLength = sprintf_s( szBuffer, _countof(szBuffer), "RS--- Tid_%d ", GetCurrentThreadId());

	va_list arg_ptr;
	va_start( arg_ptr, format );
	vsprintf_s( szBuffer+nLength, _countof(szBuffer)-nLength, format, arg_ptr );
	va_end( arg_ptr );

#if (TARGET_OS == OS_WINDOWS)
	OutputDebugStringA( szBuffer );
#else
	printf("%s", szBuffer );
#endif

//	FILE * fp = fopen( "c:\\rsengineerr.log", "a" );
//	if( NULL == fp ) return;
//	fwrite( szBuffer, 1, strlen(szBuffer)+1, fp );
//	fclose(fp);
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
