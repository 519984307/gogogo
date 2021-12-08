#ifdef UNIXPLATFORM
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
#else
    #include "stdafx.h"
#endif

#include "config.h"
#include "configimpl.h"
#include "shmem.h"
#include "cfgmutex.h"

//=======================全局变量=======================
void		*g_pBasePtr = (void *)0x1000;		//作为模板类的参数来传递基地址
RS_BYTE		*g_pAddrImport = 0;
#ifndef UNIXPLATFORM
HANDLE		g_hInstance = NULL;					//全局DLL句柄
HMODULE		g_hZipModule = 0;
#endif

RS_DDWORD	g_dwSHMStart = 0;
RS_DDWORD	g_dwSHMEnd = 0;
CCfgMutex	g_cMutexMem;
CCfgMutex	g_cMutexLoad;

RS_LONG		*g_plRefRead = 0;
RS_LONG		*g_plRefReadRequest = 0;
RS_LONG		*g_plRefWrite = 0;
RS_BOOL		g_bIsMutexWriteCreated = RS_FALSE;
RS_DWORD	g_dwExpiredFlag = 0;		//是否已过期的标志
RS_DWORD	g_dwInstallFlag = 0;		//是否已经安装过的标志
fnCompressDef g_pCompress = 0;
fnUncompressDef g_pUncompress = 0;

//=======================导出函数=======================
//创建CConfig对象
IConfig* NewConfigObject()
{
        CConfig* pConfig = new CConfig;
        MyTrace("NewConfigObject1 [--->] Addr = %8x", (RS_DDWORD)pConfig);
        return pConfig;
}

//释放CConfig对象
void ReleaseConfigObject(IConfig** pConfig)
{
        if(pConfig && *pConfig)
        {
                MyTrace("ReleaseConfigObject [<---] Addr = %8x", (RS_DDWORD)*pConfig);
                delete (CConfig *)(*pConfig);
                *pConfig = NULL;
        }
}

#ifndef UNIXPLATFORM
//=======================DLL入口=======================
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                                         )
{
    switch (ul_reason_for_call)
        {
                case DLL_PROCESS_ATTACH:
                        MyTrace("CfgDll.Dll Attach!");
                        g_hInstance = hModule;
// Added by LauranceZhang 2004-11-15 18:22:23 <
                        if(ERR_SUCCESS != g_cMutexLoad.Create("Load_Shared_Memory"))
                        {
                                MyTrace("Create Mutex(Load_Shared_Memory) failed! Failed to load CfgDll.Dll!");
                                return false;
                        }
// Added by LauranceZhang 2004-11-15 18:22:23 >
                        break;
                case DLL_THREAD_ATTACH:
                        break;
                case DLL_THREAD_DETACH:
                        break;
                case DLL_PROCESS_DETACH:
                        CSharedMem::Finalize();			//释放共享内存
                        MyTrace("CfgDll.Dll Detach!");
                        //删除全局互斥量
                        //g_cMutexMem.Destroy();
                        break;
    }
    return true;
}

//=======================全局函数=======================
void InnerTrace(char *pszInfo, ...)
{
#ifndef LOGTOFILE
#ifndef LOGTODBGVIEW_REAL
        return;
#endif
#endif

        static int nLine = 1;
        FILE* pFile = NULL;
        CHAR szFileName[1024] = {0};
        CHAR szWrite[32*1024] = {0};
        GetModuleFileName(NULL, szFileName, 1024);

#ifdef LOGTOFILE
        RS_CHAR* pPos = strrchr(szFileName, '\\');
        if(pPos != RS_NULL)
        {
                char szExeName[100] = {0};
                strncpy(szExeName, pPos+1, 100);
                *(pPos+1) = 0;
                strcat(szFileName, "CfgDll.dll - ");
                strcat(szFileName, szExeName);
                strcat(szFileName, ".txt");
        }
        else
        {
                strcat(szFileName, "CfgDll.dll.txt");
        }
MYOPEN:
        pFile = fopen(szFileName, "r+t");	//try to open file
        if(!pFile)
        {
                pFile = fopen(szFileName, "w+t");
                if(!pFile)
                        return;
                fclose(pFile);
                pFile = NULL;
                goto MYOPEN;
        }

        fseek(pFile, 0, SEEK_END);
        DWORD dwFileLen = ftell(pFile);
        if(dwFileLen > 0x100000)
        {
                pFile = fopen(szFileName, "w+t");
                if(!pFile)
                        return;
                fclose(pFile);
                pFile = NULL;
                goto MYOPEN;
        }
        sprintf(szWrite, "%04d: ", nLine);
#endif

#ifdef LOGTODBGVIEW_REAL
        DWORD dwTHreadID= GetCurrentThreadId();
//	HANDLE hProcess = GetCurrentProcess();
//	CHAR szProcess[1024] = {0};
//	DWORD dwProcessSize = 1024;
//	GetProcessImageFileName(hProcess, szProcess, dwProcessSize );
        //sprintf(szWrite, "ThreadID=%x, %s => %04d: ", dwTHreadID, szFileName, nLine);
        //sprintf(szWrite, "ThreadID=%08x,=> %04d: ", dwTHreadID, nLine);
        sprintf(szWrite, "Process=%s, ThreadID=%x->", szFileName, dwTHreadID);
#endif

        // 格式化字符串
        TCHAR szInfo[32*1024] = { 0 };
        va_list args;
        va_start(args, pszInfo);
        _vsntprintf(szInfo, 32*1024, pszInfo, args);
        va_end(args);

        strncat(szWrite, szInfo, 32*1024);
        strncat(szWrite,"\n", 32*1024);
#ifdef LOGTOFILE
                fseek(pFile, 0, SEEK_END);
                if(1 != fwrite(szWrite, strlen(szWrite), 1, pFile))
                {
                        fclose(pFile);
                        return;
                }
#endif
#ifdef LOGTODBGVIEW_REAL
        OutputDebugString(szWrite);
#endif

        nLine ++;
#ifdef LOGTOFILE
        fclose(pFile);
#endif
}
void GetMutexName(DWORD dwMutexType, char *pName, char *pProductName)
{
        CHAR szMutexName[128] = {0};
        if(IsTerminalServer())
                strncpy(szMutexName, "Global\\", 128);

        RS_CHAR szExe[1024] = {0};
        GetModuleFileName((HMODULE)g_hInstance, szExe, 1024);
        RS_CHAR* pPos = strrchr(szExe, '\\');
        if(pPos)
                strncat(szMutexName, pPos+1, 128);
        if(dwMutexType==MUTEX_WRITE_MEM)
                strncat(szMutexName, "_MUTEX_WRITE_MEM_", 128);
        else if(dwMutexType==MUTEX_WRITE_FILE)
                strncat(szMutexName, "_MUTEX_WRITE_FILE_", 128);

        if(pProductName)
                strncat(szMutexName, pProductName, 128);
        strncpy(pName, szMutexName, 128);
}
#else
void GetMutexName(DWORD dwMutexType, char *pName, char *pProductName)
{



        CHAR szMutexName[128] = {0};

        RS_CHAR szExe[1024] = {0};
        getcwd( szExe, 1024);
        RS_CHAR* pPos = strrchr(szExe, '\\');
        if(pPos)
                strncat(szMutexName, pPos+1, 128);
        if(dwMutexType==MUTEX_WRITE_MEM)
                strncat(szMutexName, "_MUTEX_WRITE_MEM_", 128);
        else if(dwMutexType==MUTEX_WRITE_FILE)
                strncat(szMutexName, "_MUTEX_WRITE_FILE_", 128);

        if(pProductName)
                strncat(szMutexName, pProductName, 128);
        strncpy(pName, szMutexName, 128);
}
#endif

void FormatErrorMessage(DWORD dwErrorCode, char *pFileName, long lLine, char *pFunction)
{
#ifndef LOGERRORMSG
        return;
#endif
#ifdef UNIXPLATFORM
        char pszFileName[256],pszFunction[256];
        if(pFileName)
                strcpy(pszFileName,pFileName);
        else
                pszFileName[0]='\0';

        if(pFunction)
                strcpy(pszFunction,pFunction);
        else
                pszFunction[0]='\0';

#else
        char *pszFileName = 0, *pszFunction = 0;
        pszFileName = pFileName ? pFileName: "";
        pszFunction = pFunction ? pFunction: "";
#endif

        char szErrMsg[4096] = {0};
        char szDetail[1024] = {0};
        switch(dwErrorCode)
        {
        case ERR_CFG_MEM_ALLOCFAILED	:
                strncpy(szDetail, "Memory alloc failed.", 1024);
                break;
        case ERR_CFG_SHM_INITFAILED		:
                strncpy(szDetail, "Shared memory initialize failed.", 1024);
                break;
        case ERR_CFG_SHM_ALLOCFAILED	:
                strncpy(szDetail, "Shared memory alloc failed.", 1024);
                break;
        case ERR_CFG_PARAM_INVALID		:
                strncpy(szDetail, "Function's parameter is invalid.", 1024);
                break;
        case ERR_CFG_BUFFER_OVERFLOW	:
                strncpy(szDetail, "Buffer is overflowed.", 1024);
                return;
                break;
        case ERR_CFG_USER_NOTFOUND		:
                strncpy(szDetail, "Not found the user of configration.", 1024);
                break;
        case ERR_CFG_GUID_SETFAILED		:
                strncpy(szDetail, "Set GUID failed.", 1024);
                break;
        case ERR_CFG_REGISTRY_READFAILED	:
                strncpy(szDetail, "Read registry failed.", 1024);
                break;
        case ERR_CFG_PRODUCT_IDNOTMATCH	:
                strncpy(szDetail, "Product ID is not matched the one in configration.", 1024);
                break;
        case ERR_CFG_MUTEX_EXISITED		:
                strncpy(szDetail, "Mutex has been existed.", 1024);
                break;
        case ERR_CFG_MUTEX_CREATEFAILED	:
                strncpy(szDetail, "Mutex create failed.", 1024);
                break;
        case ERR_CFG_MUTEX_LOCKFAILED	:
                strncpy(szDetail, "Mutex lock failed.", 1024);
                break;

        case ERR_CFG_FILE_OPENFAILED	:
                strncpy(szDetail, "Open file failed.", 1024);
                break;
        case ERR_CFG_FILE_READFAILED	:
                strncpy(szDetail, "Read file failed.", 1024);
                break;
        case ERR_CFG_FILE_WRITEFAILED	:
                strncpy(szDetail, "Write file failed.", 1024);
                break;
        case ERR_CFG_FILE_ISEMPTY		:
                strncpy(szDetail, "File is empty.", 1024);
                break;
        case ERR_CFG_FILE_TYPEINVALID	:
                strncpy(szDetail, "File type is invalid.", 1024);
                break;
        case ERR_CFG_FILE_HEADINVALID	:
                strncpy(szDetail, "File head is invalid.", 1024);
                break;
        case ERR_CFG_FILE_CHECKSUMFAILED	:
                strncpy(szDetail, "File checksum failed.", 1024);
                break;
        case ERR_CFG_TREE_ISEMPTY		:
                strncpy(szDetail, "Tree is empty.", 1024);
                break;
        case ERR_CFG_TREE_REFNOTEMPTY	:
                strncpy(szDetail, "Tree reference is not zero.", 1024);
                break;
        case ERR_CFG_TREE_LOADFAILED	:
                strncpy(szDetail, "Tree load failed.", 1024);
                break;
        case ERR_CFG_TREE_CLONEFAILED	:
                strncpy(szDetail, "Tree clone failed.", 1024);
                break;
        case ERR_CFG_NODE_ISNOTOPENED	:
                strncpy(szDetail, "Tree node is not opened.", 1024);
                break;
        case ERR_CFG_NODE_NORIGHT		:
                strncpy(szDetail, "Tree node has no right.", 1024);
                break;
        case ERR_CFG_NODE_LOCKED		:
                strncpy(szDetail, "Tree node has been locked.", 1024);
                break;
        case ERR_CFG_NODE_IMPORTFAILED	:
                strncpy(szDetail, "Tree import failed.", 1024);
                break;
        case ERR_CFG_NODE_HASOPENED		:
                strncpy(szDetail, "Tree node has been opened by other objects.", 1024);
                break;
        case ERR_CFG_ITEM_EXISTED		:
                strncpy(szDetail, "Tree node or item has been existed.", 1024);
                break;
        case ERR_CFG_ITEM_NOTEXISTED	:
                strncpy(szDetail, "Tree node or item is not existed.", 1024);
                break;
        case ERR_CFG_ITEM_SEARCHFAILED	:
                strncpy(szDetail, "Search tree node or item failed.", 1024);
                break;
        case ERR_CFG_ITEM_INSERTFAILED	:
                strncpy(szDetail, "Insert tree node or item failed.", 1024);
                break;
        case ERR_CFG_ITEM_NOTEXIST:
                strncpy(szDetail, "Node or item is not existed, query failed.", 1024);
                break;

        case ERR_SN_VAL_INVALID			:
                break;
        case ERR_SN_VAL_SUCCESS			:
                break;
        case ERR_SN_PARAM_ERR			:
                break;
        case ERR_SN_LENGTH_ERR			:
                break;
        case ERR_SN_STRING_ERR			:
                break;
        case ERR_SN_TYPE_ERR			:
                break;
        case ERR_SN_DECODE0_ERR			:
                break;
        case ERR_SN_DECODE1_ERR			:
                break;
        case ERR_SN_DECODE2_ERR			:
                break;
        case ERR_SN_DECODE3_ERR			:
                break;
        case ERR_SN_DECODE4_ERR			:
                break;
        case ERR_SN_DECODE5_ERR			:
                break;
        case ERR_SN_CHECK_FAILED		:
                break;
        case ERR_SN_GETINFO_FAILED		:
                break;
        case ERR_SN_NOT_MATCH			:
                break;
        case ERR_SN_CHECK_EXPIRED		:
                break;
        default:
                strncpy(szDetail, "Unkown error message; please update your software.", 1024);
                break;
        }
#ifdef UNIXPLATFORM
        printf("\n%s\n",szErrMsg);
#else
        sprintf(szErrMsg, "Config_Error: file=[%s]; line=[%d]; func=[%s]; Detail=[%s]", pszFileName, lLine, pFunction, szDetail);
        InnerTrace(szErrMsg);
#endif
}

#ifndef UNIXPLATFORM
int ReadREGString(	HKEY hKey,
                                        const char* lpSubKey,
                                        const char* lpValueName,
                                        char* lpszData,
                                        int &nDataLen)
{
        if(hKey == NULL || lpSubKey == NULL || lpValueName == NULL || !lpszData)
                return ERR_CFG_PARAM_INVALID;

        memset(lpszData, 0, nDataLen * sizeof(char));

        HKEY hkKey;
        LONG lResult = RegOpenKey(hKey, lpSubKey, &hkKey);
        if(ERROR_SUCCESS != lResult)
                return ERR_CFG_REG_OPENFAILED;

        int nActualLen = 0;
        //先去得到要分配的内存大小。
        DWORD dwType = REG_SZ;
        lResult = RegQueryValueEx(hkKey, lpValueName, 0, &dwType, NULL, (unsigned long *)&nActualLen);
        if(ERROR_SUCCESS != lResult)
        {
                RegCloseKey(hkKey);
                return ERR_CFG_REG_QUERYFAILED;
        }

        //取得所需要的值
        if (nActualLen > nDataLen)
        {
                nDataLen = nActualLen;
                RegCloseKey(hkKey);
                return ERR_CFG_BUFFER_OVERFLOW;
        }

        lResult = RegQueryValueEx(hkKey, lpValueName , 0, &dwType, (LPBYTE)lpszData, (unsigned long *)&nDataLen);
        if(ERROR_SUCCESS != lResult)
        {
                RegCloseKey(hkKey);
                return ERR_CFG_REG_QUERYFAILED;
        }
        RegCloseKey(hkKey);

        return ERROR_SUCCESS;
}

RS_BOOL LoadZipDll()
{
        // 装入zip.dll动态库
        CHAR szModulePath[1024] = {0};
        GetModuleFileName(NULL, szModulePath, 1024);
        CHAR* pPos = strrchr(szModulePath, '\\');
        if(pPos != NULL)
        {
                *(pPos+1) = 0;
                strcat(szModulePath, "zip.dll");
        }
        else
        {
                strcpy(szModulePath, "zip.dll");
        }

        g_hZipModule = LoadLibrary(szModulePath);
        if(g_hZipModule == NULL)
                return RS_FALSE;

        // 取函数指针

        g_pCompress	= (fnCompressDef)GetProcAddress(g_hZipModule, "compress");
        g_pUncompress = (fnUncompressDef)GetProcAddress(g_hZipModule, "uncompress");

        if(	!g_pCompress || !g_pUncompress )
        {
                FreeLibrary(g_hZipModule);
                return RS_FALSE;
        }
        return RS_TRUE;
}

RS_BOOL UnloadZipDll()
{
        if(g_hZipModule && FreeLibrary(g_hZipModule))
        {
                g_pCompress		= RS_NULL;
                g_pUncompress	= RS_NULL;
                g_hZipModule	= RS_NULL;
        }

        return RS_TRUE;
}

void InnerTraceEx(char *pszInfo, ...)
{
#ifdef FOR_INNER_TRACE
        static int nLine = 1;
        FILE* pFile = NULL;
        CHAR szFileName[1024] = {0};
        CHAR szWrite[32*1024] = {0};
        GetModuleFileName(NULL, szFileName, 1024);

        RS_CHAR* pPos = strrchr(szFileName, '\\');
        if(pPos != RS_NULL)
        {
                char szExeName[100] = {0};
                strncpy(szExeName, pPos+1, 100);
                *(pPos+1) = 0;
                strcat(szFileName, "CfgDll.dll - ");
                strcat(szFileName, szExeName);
                strcat(szFileName, ".txt");
        }
        else
        {
                strcat(szFileName, "CfgDll.dll.txt");
        }
MYOPEN:
        pFile = fopen(szFileName, "r+t");	//try to open file
        if(!pFile)
        {
                pFile = fopen(szFileName, "w+t");
                if(!pFile)
                        return;
                fclose(pFile);
                pFile = NULL;
                goto MYOPEN;
        }

        fseek(pFile, 0, SEEK_END);
        DWORD dwFileLen = ftell(pFile);
        if(dwFileLen > 0x100000)
        {
                pFile = fopen(szFileName, "w+t");
                if(!pFile)
                        return;
                fclose(pFile);
                pFile = NULL;
                goto MYOPEN;
        }
        sprintf(szWrite, "%04d: ", nLine);

        DWORD dwTHreadID= GetCurrentThreadId();
//	HANDLE hProcess = GetCurrentProcess();
//	CHAR szProcess[1024] = {0};
//	DWORD dwProcessSize = 1024;
//	GetProcessImageFileName(hProcess, szProcess, dwProcessSize );
        //sprintf(szWrite, "ThreadID=%x, %s => %04d: ", dwTHreadID, szFileName, nLine);
        //sprintf(szWrite, "ThreadID=%08x,=> %04d: ", dwTHreadID, nLine);
        sprintf(szWrite, "Process=%s, ThreadID=%x->", szFileName, dwTHreadID);

        // 格式化字符串
        TCHAR szInfo[32*1024] = { 0 };
        va_list args;
        va_start(args, pszInfo);
        _vsntprintf(szInfo, 32*1024, pszInfo, args);
        va_end(args);

        strncat(szWrite, szInfo, 32*1024);
        strncat(szWrite,"\n", 32*1024);
                fseek(pFile, 0, SEEK_END);
                if(1 != fwrite(szWrite, strlen(szWrite), 1, pFile))
                {
                        fclose(pFile);
                        return;
                }
        OutputDebugString(szWrite);

        nLine ++;
        fclose(pFile);
#endif
}
#endif


#ifdef UNIXPLATFORM
#include "zlib.h"

int UnZipCFG(unsigned char *szOutBuf,unsigned long* nOutLen,unsigned char *szSrcbuf,unsigned long nSrcLen)
{
        return uncompress(szOutBuf,nOutLen,szSrcbuf,nSrcLen);
}


int ZipCFG(unsigned char *szOutBuf,unsigned long* nOutLen,unsigned char *szSrcbuf,unsigned long nSrcLen)
{

        return compress(szOutBuf,nOutLen,szSrcbuf,nSrcLen);

}



RS_BOOL LoadZipDll()
{

        g_pCompress	= (fnCompressDef)ZipCFG;
        g_pUncompress = (fnUncompressDef)UnZipCFG;

        if(	!g_pCompress || !g_pUncompress )
        {
                return RS_FALSE;
        }
        return RS_TRUE;
}

RS_BOOL UnloadZipDll()
{
                g_pCompress		= RS_NULL;
                g_pUncompress	= RS_NULL;
                return RS_TRUE;
}

void InnerTraceEx(char *pszInfo, ...)
{
}
#endif
