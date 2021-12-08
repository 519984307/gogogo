#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_



//////////////////////////////////////////////////////////////////////////
//宏调整
//posix下，一直维持输出调试信息状态
#if defined(PLATFORM_TYPE_POSIX)
#define _DEBUG_OUTPUT_
#endif
//_debug 下，默认为输出调试信息
#if defined(_DEBUG) && !defined(_DEBUG_OUTPUT_)
#define _DEBUG_OUTPUT_
#endif



//////////////////////////////////////////////////////////////////////////
//定义

#define _DEBUG_DEFAULT_LOGFILE				"c:\\dbg_log.txt"
#define _DEBUG_DEFAULT_LOGFILE_POSIX		"./posix_log.txt"

class debug_recorder
{
public:

//for win32/debug
	template<class T>
	static inline void debug(const T *fmt, ...);

	template<class T>
	static inline void console(const T *fmt, ...);

	template<class T>
	static inline void log(LPSTR pszFileName, const T *fmt, ...);


//for posix/all condition
	static inline void posix_log(LPSTR pszFileName, const CHAR *fmt, ...);

};


//////////////////////////////////////////////////////////////////////////
//[对07的dbg进行扩展]
//
//1. 记录到文件：
//2. 记录到debug:
//3. 记录到console:
//4. 类似sprintf的解析
//5. 支持宽、窄字符
//6. 不依赖其他模块。
//7. 自定义编译开关
//8. 为windows和posix两个平台提供支持



//////////////////////////////////////////////////////////////////////////
//example1:
// #include "dbgprintf.h"
// dbg_log(0,"test");	//for debug

//example2:
// #if defined(_RELEASE) || defined(_XXX)
// #define _DEBUG_OUTPUT_
// #endif
// #include "dbgprintf.h"
// dbg_log(0,"test");	//for debug/release/XXX








#if defined(PLATFORM_TYPE_WIN32) 
#include <atlstr.h>

namespace _dbg_util
{
	inline void dbg_output_debug_string(const char *str) 
	{ 
		OutputDebugStringA(str); 
	}
	inline void dbg_output_debug_string(const wchar_t *str) 
	{ 
		OutputDebugStringW(str);
	}

	inline void dbg_output_console_string(const char *str)
	{
		printf(str);
	}

	inline void dbg_output_console_string(const wchar_t *str)
	{
		wprintf(str);
	}

	inline void dbg_output_log_string(LPSTR pszFileName, const char* str)
	{
		if(FILE* fp=fopen(pszFileName, "a+b"))
		{
			fwrite(str, strlen(str),1, fp);
			fclose(fp);
		}
	}

	inline void dbg_output_log_string(LPSTR pszFileName, const wchar_t* str)
	{
		if(FILE* fp=fopen(pszFileName, "a+b"))
		{
			fwrite(str, wcslen(str)*sizeof(wchar_t),1, fp);
			fclose(fp);
		}
	}

	template<class T>
	struct __dbg_string {};

	template<>
	struct __dbg_string<char>
	{
		typedef CString TYPE;
	};

	template<>
	struct __dbg_string<wchar_t>
	{
		typedef CStringW TYPE;
	};
};

#endif




//////////////////////////////////////////////////////////////////////////
//debug_recorder::debug
//debug_recorder::console
//debug_recorder::log


#if defined(_DEBUG_OUTPUT_) && defined(PLATFORM_TYPE_WIN32) 


template<class T>
inline void debug_recorder::debug(const T *fmt, ...)
{
	typename _dbg_util::__dbg_string<T>::TYPE str;
	va_list ap;
	va_start(ap, fmt);
	str.FormatV(fmt, ap);
	va_end(ap);
	_dbg_util::dbg_output_debug_string(str);
}


template<class T>
inline void debug_recorder::console(const T *fmt, ...)
{
	typename _dbg_util::__dbg_string<T>::TYPE str;
	va_list ap;
	va_start(ap, fmt);
	str.FormatV(fmt, ap);
	va_end(ap);
	_dbg_util::dbg_output_console_string(str);
}


template<class T>
inline void debug_recorder::log(LPSTR pszFileName, const T *fmt, ...)
{
	pszFileName = pszFileName?pszFileName:_DEBUG_DEFAULT_LOGFILE;
	typename _dbg_util::__dbg_string<T>::TYPE str;
	va_list ap;
	va_start(ap, fmt);
	str.FormatV(fmt, ap);
	va_end(ap);
	_dbg_util::dbg_output_log_string(pszFileName, str);
}

#else

template<class T>
inline void debug_recorder::debug(const T *fmt, ...){}

template<class T>
inline void debug_recorder::console(const T *fmt, ...){}

template<class T>
inline void debug_recorder::log(LPSTR pszFileName, const T *fmt, ...){}

#endif



//////////////////////////////////////////////////////////////////////////
//debug_recorder::posix_log

#if defined(_DEBUG_OUTPUT_) && defined(PLATFORM_TYPE_POSIX) 

#include <stdarg.h>
#include <stdio.h>

inline void debug_recorder::posix_log(LPSTR pszFileName, const CHAR *fmt, ...)
{
	if(!fmt) return;
	pszFileName = pszFileName?pszFileName:((CHAR*)_DEBUG_DEFAULT_LOGFILE_POSIX);
	static CHAR g_strPrintBuff[0x1000];
	g_strPrintBuff[0x1000-1]=0;

	va_list vaArgList;
	va_start(vaArgList, fmt);
	vsnprintf(g_strPrintBuff, sizeof(g_strPrintBuff), fmt, vaArgList);
	va_end(vaArgList);

	if(FILE* fp=fopen(pszFileName, "a+b"))
	{
		fwrite(g_strPrintBuff, strlen(g_strPrintBuff),1, fp);
		fclose(fp);
	}
}

#else

inline void debug_recorder::posix_log(LPSTR pszFileName, const CHAR *fmt, ...){}

#endif












#endif // #ifndef _DEBUG_PRINT_H_
