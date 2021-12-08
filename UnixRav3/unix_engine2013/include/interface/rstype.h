////////////////////////////////////////////////////////////////////////////////
//	项目名称：瑞星杀毒软件2004版
//	文件名称：DataType.h
//	实现功能：对跨平台的数据类型和通用常量的定义
//	作    者：张林刚
//	编写日期：2003-5-14 13:50:01
//
//	修改历史（自下而上填写 内容包括：日期  修改人  修改说明）
//  2004-7-12
////////////////////////////////////////////////////////////////////////////////
#ifndef	__RSTYPE_H__
#define __RSTYPE_H__

/*******************************************************************************
<平台类型定义>
*******************************************************************************/
// windows
#if defined(PLATFORM_TYPE_WINDOWS) || \
	defined(PLATFORM_TYPE_IBM_RISC) || \
	defined(PLATFORM_TYPE_HP_RISC) || \
	defined(PLATFORM_TYPE_SUN_SPARC) || \
	defined(PLATFORM_TYPE_SUN_INTEL) || \
	defined(PLATFORM_TYPE_FREEBSD) || \
	defined(PLATFORM_TYPE_LINUX_INTEL)
#	define PLATFORM_TYPE_DEFINED
#endif
#if defined(WIN32) || defined(_WIN32)
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_WINDOWS
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
// ibm risc
#if (defined(AIX) || defined(_AIX) || defined(__AIX) || defined(__AIX__)) || \
	(defined(aix) || defined(_aix) || defined(__aix) || defined(__AIX__))
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_IBM_RISC
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
// hp risc
#if (defined(hppa) || defined(_hppa) || defined(__hppa) || defined(__hppa__)) || \
	(defined(hpux) || defined(_hpux) || defined(__hpux) || defined(__hpux__))
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_HP_RISC
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
// sun sparc
#if (defined(sun) || defined(_sun) || defined(__sun) || defined(__sun__)) && \
    (defined(sparc) || defined(_sparc) || defined(__sparc) || defined(__sparc__))
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_SUN_SPARC
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
// sun intel
#if (defined(sun) || defined(_sun) || defined(__sun) || defined(__sun__)) && \
    (defined(i386) || defined(_i386) || defined(__i386) || defined(__i386__))
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_SUN_INTEL
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
// linux
#if (defined(linux) || defined(_linux) || defined(__linux) || defined(__inux__)) && \
    (defined(i386) || defined(_i386) || defined(__i386) || defined(__i386__))
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_LINUX_INTEL
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
// FreeBSD
#if (defined(FreeBSD) || defined(_FreeBSD) || defined(__FreeBSD) || defined(__FreeBSD__)) && \
    (defined(i386) || defined(_i386) || defined(__i386) || defined(__i386__))
#	ifndef PLATFORM_TYPE_DEFINED
#	define PLATFORM_TYPE_FREEBSD
#	define PLATFORM_TYPE_DEFINED
#	endif
#endif
#if !defined(PLATFORM_TYPE_WINDOWS) && \
    !defined(PLATFORM_TYPE_IBM_RISC) && \
    !defined(PLATFORM_TYPE_HP_RISC) && \
    !defined(PLATFORM_TYPE_SUN_SPARC) && \
	!defined(PLATFORM_TYPE_SUN_INTEL) && \
	!defined(PLATFORM_TYPE_LINUX_INTEL) && \
    !defined(PLATFORM_TYPE_FREEBSD)
#	error "Unknown Platform Type !!!!"
#endif

#ifndef far
#	define far
#endif

/*******************************************************************************
<平台相关定义>
*******************************************************************************/
//
//	platform:	Windows
//
#ifdef PLATFORM_TYPE_WINDOWS
#	define	RS_LITTLE_ENDIAN
#ifdef PLATFORM_TYPE_NATIVE
#	include "rsnative.h"
#else
#	include	<windows.h>
#	include <TCHAR.H>
#endif
#	define	RS_PACK_ONEBYTE
#	define	RS_PACK_EIGHTBYTE
#endif

//
//	platform:	AIX(RISC)
//
#ifdef	PLATFORM_TYPE_IBM_RISC
#	define	_LINUX_FAMILY
#	define	RS_BIG_ENDIAN
#	include "strings.h"					//use strcasecmp
#	define	RS_PACK_ONEBYTE		__attribute__((packed))
#	define	RS_PACK_EIGHTBYTE	__attribute__((aligned(8)))
#endif

//
//	platform:	Sun Solaris(sparc)
//
#ifdef	PLATFORM_TYPE_SUN_SPARC
#	define	_LINUX_FAMILY
#	define	RS_BIG_ENDIAN				//Big Byte End
#	include "strings.h"
#	define	strcmpi	strcasecmp
#	define	stricmp strcasecmp
#	define	strnicmp strncasecmp
#	define	lstrcpyn strncpy
#	define	RS_PACK_ONEBYTE		__attribute__((packed))
#	define	RS_PACK_EIGHTBYTE	__attribute__((aligned(8)))
#endif

//
//	platform:	Sun Solaris(intel)
//
#ifdef	PLATFORM_TYPE_SUN_INTEL
#	define	_LINUX_FAMILY
#	define	RS_LITTLE_ENDIAN
#	include "strings.h"
#	define	strcmpi	strcasecmp
#	define	lstrcpyn strncpy
#	define	RS_PACK_ONEBYTE		__attribute__((packed))
#	define	RS_PACK_EIGHTBYTE	__attribute__((aligned(8)))
#endif

#ifdef PLATFORM_TYPE_FREEBSD
#	define PLATFORM_TYPE_LINUX_INTEL
#endif

//
//	platform:	HP(RISC)
//
#ifdef	PLATFORM_TYPE_HP_RISC
#	define	_LINUX_FAMILY
#	define	RS_BIG_ENDIAN
#	include "strings.h"						//use strcasecmp
#	define	RS_PACK_ONEBYTE		__attribute__((packed))
#	define	RS_PACK_EIGHTBYTE	__attribute__((aligned(8)))
#endif

//
//	platform:	linux(intel)
//
#ifdef	PLATFORM_TYPE_LINUX_INTEL
#	define	_LINUX_FAMILY
#	define	RS_LITTLE_ENDIAN
#	define	RS_PACK_ONEBYTE		__attribute__((packed))
#	define	RS_PACK_EIGHTBYTE	__attribute__((aligned(8)))
#	include "string.h"
#endif

//
//	use for all linux-family platform
//
#ifdef	_LINUX_FAMILY
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <errno.h>
#	include <unistd.h>
#	include "stdlib.h"
#	include "string.h"
#	define	strcmpi strcasecmp
#	define	stricmp strcasecmp
#	define	strnicmp strncasecmp
#	define	_stricmp strcasecmp
#	define	_strdup strdup
	char*	_strlwr( char* string );
	char*	_strupr( char* string );
#	define	strlwr  _strlwr
#	define	strupr  _strupr
#endif


#ifndef MIN
	#define	MIN(a,b) (((a) < (b)) ? (a) : (b))		//这两个宏linux下面没有定义
#endif
#ifndef MAX
	#define	MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

/*******************************************************************************
<通用数据类型定义>
*******************************************************************************/
typedef			int					RS_INT;
typedef			unsigned int		RS_UINT;
typedef			int					RS_BOOL;

typedef			unsigned char		RS_BYTE;
typedef			unsigned short		RS_WORD;
typedef			unsigned long		RS_DWORD;
typedef			RS_DWORD			HITEM;
typedef			HITEM				*PHITEM;
typedef			float				RS_FLOAT;
typedef			double				RS_DOUBLE;
typedef			long				RS_LONG;
typedef			unsigned long		RS_ULONG;
typedef			short				RS_SHORT;
typedef			unsigned short		RS_USHORT;
typedef			char				RS_CHAR;
#ifdef PLATFORM_TYPE_WINDOWS
typedef			TCHAR				RS_TCHAR;
#else
typedef			char				RS_TCHAR;
#endif
typedef			unsigned char		RS_UCHAR;
typedef			wchar_t				RS_WCHAR;
#define			RS_VOID				void
typedef			RS_VOID	far			*RS_LPVOID;

typedef			RS_CHAR	far			*RS_LPSTR;
typedef			RS_WCHAR far		*RS_LPWSTR;
typedef			const RS_CHAR far	*RS_LPCSTR;
typedef			const RS_WCHAR far	*RS_LPCWSTR;

#ifdef PLATFORM_TYPE_WINDOWS
typedef			__int64				RS_LONGLONG;
typedef			unsigned __int64	RS_ULONGLONG;
#else
typedef			long long			RS_LONGLONG;
typedef			unsigned long long	RS_ULONGLONG;
#endif

// 本机类型
typedef unsigned char		RS_INT8;
typedef short				RS_INT16;
typedef unsigned short		RS_UINT16;
typedef int					RS_INT32;
typedef unsigned int		RS_UINT32;

// The non-Windows branch is added by Dandy Cheung for Linux compiling. 2004-07-05
// BUT I cannot warrant that it is correct on every *nix platform
#ifdef PLATFORM_TYPE_WINDOWS
typedef __int64				RS_INT64;
typedef unsigned __int64	RS_UINT64;
#else 
typedef long long			RS_INT64;
typedef unsigned long long	RS_UINT64;
#endif // PLATFORM_TYPE_WINDOWS

typedef RS_DWORD			RS_RESULT;
typedef		void*			RS_HANDLE;

// The non-Windows branch is added by Dandy Cheung for Linux compiling. 2004-07-05
#ifdef PLATFORM_TYPE_WINDOWS
#define RS_STDCALL __stdcall
#else
#define RS_STDCALL
#endif // PLATFORM_TYPE_WINDOWS

//版本号类型定义（使用一个四字节的数值代表xx.xx.xx.xx）
typedef			RS_DWORD			RS_VERSION;		
		
//结果类型定义（二进制最高位为'0'（正整数），则表示成功，为'1'（负整数）则表示失败）
//typedef			RS_LONG				RS_RESULT;				

#define			IN											//输入参数标志宏
#define			OUT											//输出参数标志宏
#define			INOUT										//输入输出参数标志宏

#define			RS_NULL				0
#define			RS_FALSE			0
#define			RS_TRUE				1

/*******************************************************************************
<字节序相关定义>
*******************************************************************************/
//	define some MACRO for byte-sequence
#ifdef	RS_BIG_ENDIAN
#	define	RS_LOBYTE(usNum)	(unsigned char)(usNum & 0xFF)
#	define	RS_HIBYTE(usNum)	(unsigned char)((usNum & 0xFF00)>>8)

#	define	RS_LOWORD(ulNum)	(unsigned short)(ulNum & 0xFFFF)
#	define	RS_HIWORD(ulNum)	(unsigned short)((ulNum & 0xFFFF0000)>>16)

#	define	RS_LODWORD(llNum)	(unsigned int)(llNum & 0xFFFFFFFFLL)
#	define	RS_HIDWORD(llNum)	(unsigned int)((llNum & 0xFFFFFFFF00000000LL)>>32)

#	define	RS_MAKEWORD(xl,xh)	((xh << 8)  | (xl & 0x00ff))
#	define	RS_MAKEDWORD(xl,xh)	((xh << 16) | (xl & 0x00ffff))
#	define	RS_MAKEQWORD(ql,qh)	((qh << 32) | (ql & 0xffffffffLL))

#	define	RS_SWAPWORD(x)		RS_MAKEWORD(RS_HIBYTE(x),RS_LOBYTE(x))
#	define	RS_SWAPDWORD(x)		RS_MAKEDWORD(RS_SWAPWORD(RS_HIWORD(x)),RS_SWAPWORD(RS_LOWORD(x)))

#	define	RS_SWAPQWORD(x)		RS_MAKEQWORD(RS_SWAPDWORD(RS_HIDWORD(x)),RS_SWAPDWORD(RS_LODWORD(x)))

#	define	RS_GETUSHORT(x)		(RS_USHORT)RS_MAKEWORD(*(RS_UCHAR*)(x),*((RS_UCHAR*)(x)+1))	// 注意两个强制转换是必须的，为了解决在其他平台上的对齐问题
#	define	RS_GETULONG(x)		(RS_ULONG)RS_MAKEDWORD(RS_MAKEWORD(*((RS_UCHAR*)(x)),*((RS_UCHAR*)(x)+1)),RS_MAKEWORD(*((RS_UCHAR*)(x)+2),*((RS_UCHAR*)(x)+3)))
	inline RS_VOID	RsSetUShort(RS_UCHAR* pBuffer,RS_USHORT ushData)
	{
		*(pBuffer +1) =RS_LOBYTE(ushData);
		*pBuffer =RS_HIBYTE(ushData);
	}
	inline RS_VOID RsSetULong(RS_UCHAR* pBuffer,RS_ULONG ulData)
	{
		*(pBuffer+3) =RS_LOBYTE(RS_LOWORD(ulData));
		*(pBuffer+2) =RS_HIBYTE(RS_LOWORD(ulData));
		*(pBuffer+1) =RS_LOBYTE(RS_HIWORD(ulData));
		*pBuffer =RS_HIBYTE(RS_HIWORD(ulData));
	}
#define GETDWORD(source) (	(unsigned long)( (unsigned char)(*((unsigned char *)(source))) ) | \
	(unsigned long)( (unsigned char)(*((unsigned char *)(source)+1))<<8 ) | \
	(unsigned long)( (unsigned char)(*((unsigned char *)(source)+2))<<16 ) | \
	(unsigned long)( (unsigned char)(*((unsigned char *)(source)+3))<<24 ) )

#define GETWORD(source) (	(unsigned short)( (unsigned char)(*((unsigned char *)(source))) ) | \
	(unsigned short)( (unsigned char)(*((unsigned char *)(source)+1))<<8 ) )

#define SETDWORD(source, value) {  *((unsigned char *)(source)) = (unsigned char)(((unsigned long)(value))&0xff); \
	*((unsigned char *)(source)+1) = (unsigned char)((((unsigned long)(value))>>8)&0xff); \
	*((unsigned char *)(source)+2) = (unsigned char)((((unsigned long)(value))>>16)&0xff); \
	*((unsigned char *)(source)+3) = (unsigned char)((((unsigned long)(value))>>24)&0xff); }
	
#define SETWORD(source, value) {  *((unsigned char *)(source)) = (unsigned char)(((unsigned short)(value))&0xff); \
	*((unsigned char *)(source)+1) = (unsigned char)((((unsigned short)(value))>>8)&0xff); \
	}

#else	// not RS_BIG_ENDIAN
#	define	RS_LOBYTE(usNum)	(unsigned char)(usNum & 0xFF)
#	define	RS_HIBYTE(usNum)	(unsigned char)((usNum & 0xFF00)>>8)	
#	define	RS_SWAPWORD
#	define	RS_SWAPDWORD
#	define  RS_SWAPQWORD
#	define	RS_LOWORD(ulNum)	(unsigned short)(ulNum & 0xFFFF)
#	define	RS_HIWORD(ulNum)	(unsigned short)((ulNum & 0xFFFF0000)>>16)
#	define	RS_GETUSHORT(x)		*(const RS_USHORT* )(x)							//别的平台下面用指针强制转换
#	define	RS_GETULONG(x)		*(const RS_ULONG* )(x)
	inline RS_VOID RsSetUShort(RS_UCHAR* pBuffer,RS_USHORT ushData)
	{
		*(RS_USHORT* )pBuffer =ushData;
	}
	inline RS_VOID	RsSetULong(RS_UCHAR* pBuffer,RS_ULONG ulData)
	{
		*(RS_ULONG* )pBuffer =ulData;
	}	
#define GETDWORD(source) (*(const DWORD *)(BYTE *)(source))
#define GETWORD(source) (*(const WORD *)(BYTE *)(source))
#define SETDWORD(source, value) *(DWORD *)(source) = (DWORD)(value)
#define SETWORD(source, value) *(WORD *)(source) = (WORD)(value)
#endif

typedef unsigned short	ochar;


#endif // __RSTYPE_H__
