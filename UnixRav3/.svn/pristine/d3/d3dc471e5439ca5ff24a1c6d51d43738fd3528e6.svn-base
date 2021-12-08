#ifndef __EXTERN_H__
#define __EXTERN_H__

#include "cfgdef2.h"
#include "cfgmutex.h"

//全局变量声明
extern void*		g_pBasePtr;			//基地址
extern RS_BYTE*		g_pAddrImport;		//导入文件的起始地址
extern RS_DDWORD		g_dwSHMStart;		//已经使用的共享内存的起始地址
extern RS_DDWORD		g_dwSHMEnd;			//已经使用的共享内存的结束地址
extern CCfgMutex	g_cMutexMem;		//写内存全局互斥量
extern CCfgMutex	g_cMutexLoad;		//共享内存装载的互斥量

#ifndef UNIXPLATFORM
extern HANDLE		g_hInstance;		//全局DLL句柄
extern HMODULE		g_hZipModule;		//Zip.dll的句柄
#endif

extern RS_LONG		*g_plRefRead;			//正在读的引用计数
extern RS_LONG		*g_plRefReadRequest;	//正在请求读的引用技术
extern RS_LONG		*g_plRefWrite;			//正在写的引用技术
extern RS_BOOL		g_bIsMutexWriteCreated;	//互斥量是否已创建的标志
extern RS_DWORD		g_dwExpiredFlag;		//是否已过期的标志
extern RS_DWORD		g_dwInstallFlag;		//是否已经安装过的标志
extern fnCompressDef		g_pCompress;	//压缩函数指针
extern fnUncompressDef		g_pUncompress;	//解压缩函数指针

//全局函数声明
void InnerTrace(char *pszInfo, ...);		//记录日志
void InnerTraceEx(char *pszInfo, ...);
BOOL IsTerminalServer();
void GetMutexName(DWORD dwMutexType, char *pName, char *pProductName);
void FormatErrorMessage(DWORD dwErrorCode, char *pFileName, long lLine, char *pFunction);
int ReadREGString(	HKEY hKey,
                                        const char* lpSubKey,
                                        const char* lpValueName,
                                        char* lpszData,
                                        int &nDataLen);
extern RS_BOOL LoadZipDll();
extern RS_BOOL UnloadZipDll();
#endif //__EXTERN_H__
