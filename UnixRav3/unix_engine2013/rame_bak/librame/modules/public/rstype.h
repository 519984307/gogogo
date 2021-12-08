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

#ifndef far
#	define far
#endif

/*******************************************************************************
<通用数据类型定义>
*******************************************************************************/
typedef			INT					RS_INT;
typedef			UINT				RS_UINT;
typedef			BOOL					RS_BOOL;

typedef			BYTE				RS_BYTE;
typedef			WORD				RS_WORD;
typedef 		DWORD				RS_DWORD;
typedef			LONG				RS_LONG;
typedef 		ULONG				RS_ULONG;
typedef			RS_DWORD			HITEM;
typedef			HITEM				*PHITEM;
typedef			float				RS_FLOAT;
typedef			double				RS_DOUBLE;
typedef			SHORT				RS_SHORT;
typedef			USHORT				RS_USHORT;
typedef			CHAR				RS_CHAR;
#ifdef PLATFORM_TYPE_WINDOWS
typedef			TCHAR				RS_TCHAR;
#else
typedef			char				RS_TCHAR;
#endif
typedef			UINT8				RS_UCHAR;
typedef			wchar_t				RS_WCHAR;
#define			RS_VOID				void
typedef			RS_VOID	far			*RS_LPVOID;

typedef			RS_CHAR	far			*RS_LPSTR;
typedef			RS_WCHAR far		*RS_LPWSTR;
typedef			const RS_CHAR far	*RS_LPCSTR;
typedef			const RS_WCHAR far	*RS_LPCWSTR;

typedef			INT64				RS_LONGLONG;
typedef			UINT64				RS_ULONGLONG;
// 本机类型
typedef 		INT8				RS_INT8;
typedef 		INT16				RS_INT16;
typedef 		UINT16				RS_UINT16;
typedef 		INT32				RS_INT32;
typedef 		UINT32				RS_UINT32;

// The non-Windows branch is added by Dandy Cheung for Linux compiling. 2004-07-05
// BUT I cannot warrant that it is correct on every *nix platform

typedef			INT64				RS_INT64;
typedef			UINT64				RS_UINT64;

typedef			RS_DWORD			RS_RESULT;
typedef			LPCVOID				RS_HANDLE;

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

#define	IN					//输入参数标志宏
#define	OUT					//输出参数标志宏
#define	INOUT				//输入输出参数标志宏

#define	RS_NULL				NULL
#define	RS_FALSE			FALSE
#define	RS_TRUE				TRUE
///
#define GETDWORD			GET_U32
#define GETWORD				GET_U16
#define GETSDWORD			GET_S32
#define GETSWORD			GET_S16
#define SETDWORD			SET_U32
#define SETWORD				SET_U16
#define SETQWORD			SET_U64
#define GETQWORD			GET_U64
#define	RS_GETUSHORT		GETWORD				//别的平台下面用指针强制转换
#define	RS_GETULONG			GETDWORD
#define RS_SWAPWORD			SWAP2LE16
#define RS_SWAPDWORD		SWAP2LE32

/*******************************************************************************
<字节序相关定义>
*******************************************************************************/
//	define some MACRO for byte-sequence
#if TARGET_ENDIAN == BYTE_ENDIAN_B
	#define RS_BIG_ENDIAN
#endif

#ifdef	RS_BIG_ENDIAN
#	define	RS_LOBYTE(usNum)	(UINT8)(usNum & 0xFF)
#	define	RS_HIBYTE(usNum)	(UINT8)((usNum & 0xFF00)>>8)
#	define	RS_LOWORD(ulNum)	(UINT16)(ulNum & 0xFFFF)
#	define	RS_HIWORD(ulNum)	(UINT16)((ulNum & 0xFFFF0000)>>16)
#	define	RS_MAKEWORD(xl,xh)	((xh << 8) | (xl&0x00ff))
#	define	RS_MAKEDWORD(xl,xh)	((xh << 16) | (xl&0x00ffff))
//#	define	RS_GETUSHORT(x)		GET_U16(x)										//(RS_USHORT)RS_MAKEWORD(*(RS_UCHAR*)(x),*((RS_UCHAR*)(x)+1))	// 注意两个强制转换是必须的，为了解决在其他平台上的对齐问题
//#	define	RS_GETULONG(x)		GET_U32(x)										//(RS_ULONG)RS_MAKEDWORD(RS_MAKEWORD(*((RS_UCHAR*)(x)),*((RS_UCHAR*)(x)+1)),RS_MAKEWORD(*((RS_UCHAR*)(x)+2),*((RS_UCHAR*)(x)+3)))
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
//#define GETDWORD(source) (	(ULONG)( (UINT8)(*((UINT8 *)(source))) ) | \
//	(ULONG)( (UINT8)(*((UINT8 *)(source)+1))<<8 ) | \
//	(ULONG)( (UINT8)(*((UINT8 *)(source)+2))<<16 ) | \
//	(ULONG)( (UINT8)(*((UINT8 *)(source)+3))<<24 ) )
//
//#define GETWORD(source) (	(UINT16)( (UINT8)(*((UINT8 *)(source))) ) | \
//	(UINT16)( (UINT8)(*((UINT8 *)(source)+1))<<8 ) )
//
//#define SETDWORD(source, value) {  *((UINT8 *)(source)) = (UINT8)(((ULONG)(value))&0xff); \
//	*((UINT8 *)(source)+1) = (UINT8)((((ULONG)(value))>>8)&0xff); \
//	*((UINT8 *)(source)+2) = (UINT8)((((ULONG)(value))>>16)&0xff); \
//	*((UINT8 *)(source)+3) = (UINT8)((((ULONG)(value))>>24)&0xff); }
//	
//#define SETWORD(source, value) {  *((UINT8 *)(source)) = (UINT8)(((UINT16)(value))&0xff); \
//	*((UINT8 *)(source)+1) = (UINT8)((((UINT16)(value))>>8)&0xff); \
//	}

#else	// not RS_BIG_ENDIAN
#	define	RS_LOBYTE(usNum)	(UINT8)(usNum & 0xFF)
#	define	RS_HIBYTE(usNum)	(UINT8)((usNum & 0xFF00)>>8)	
#	define	RS_LOWORD(ulNum)	(UINT16)(ulNum & 0xFFFF)
#	define	RS_HIWORD(ulNum)	(UINT16)((ulNum & 0xFFFF0000)>>16)

	inline RS_VOID RsSetUShort(RS_UCHAR* pBuffer,RS_USHORT ushData)
	{
		*(RS_USHORT* )pBuffer =ushData;
	}
	inline RS_VOID	RsSetULong(RS_UCHAR* pBuffer,RS_ULONG ulData)
	{
		*(RS_ULONG* )pBuffer =ulData;
	}	

//#define GETDWORD(source) (*(const DWORD *)(BYTE *)(source))
//#define GETWORD(source) (*(const WORD *)(BYTE *)(source))
//#define SETDWORD(source, value) *(DWORD *)(source) = (DWORD)(value)
//#define SETWORD(source, value) *(WORD *)(source) = (WORD)(value)


#endif




#endif // __RSTYPE_H__
