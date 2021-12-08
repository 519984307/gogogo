#ifdef UNIXPLATFORM
    #include <string.h>
    #include <time.h>
#else
    #include "stdafx.h"
    #include "time.h"
#endif

#include "configimpl.h"
#include "tree.h"
#include "cfgmutex.h"
#include "RSTYPE.H"

CTree::CTree()
{
        m_dwReference = 0;
        m_dwSortType = ORDER_ASCENDING;
        m_dwLoadMode = LOAD_MODE_INIT;

        memset(m_szFileName,0,RS_MAX_PATH);
}

CTree::~CTree()
{
        if(g_pAddrImport && m_dwLoadMode == LOAD_MODE_ADD)
        {
                //删除导入数据时申请的共享内存
                RS_DDWORD dwDataAddr = (RS_DDWORD)g_pAddrImport;
                if( (dwDataAddr < g_dwSHMStart) || (dwDataAddr > g_dwSHMEnd) )
                {
                        //MyTrace("MODE_ADD_SHM Deleted Addr==0x%08x", g_pAddrImport);
                        InnerTraceEx("Delete Succe Mem Start=0x%x", g_pAddrImport);
                        MYDELETE(g_pAddrImport);
                        g_pAddrImport = 0;
                }
        }
}

//取产品名
RS_DWORD CTree::GetProductNameFromFile(RS_CHAR *pszFileName, RS_CHAR *pszPrtname)
{
        if(!pszPrtname)
                return ERR_CFG_PARAM_INVALID;

        //get config data file's full name
        RS_CHAR szFileName[RS_MAX_PATH] = {0};
        if(pszFileName != RS_NULL)
                strcpy(szFileName, pszFileName);
        else
        {
#ifdef UNIXPLATFORM
                RS_CHAR* pPos=NULL;
#else
                GetModuleFileName((HMODULE)g_hInstance, szFileName, RS_MAX_PATH);
                RS_CHAR* pPos = strrchr(szFileName, '\\');
#endif
                if(pPos != RS_NULL)
                {
                        *(pPos+1) = 0;
                        strcat(szFileName, "RsConfig.cfg");
                }
                else
                {
                        strcpy(szFileName, "RsConfig.cfg");
                }
        }
        //MyTrace("RealConfigFileName==%s", szFileName);

        FILE *fp = fopen(szFileName, "rb");
        if(!fp)
                return ERR_CFG_FILE_OPENFAILED;

        //////////////////////////////////////////////////////////
        //取数据文件的标志
        fseek(fp, 0, SEEK_SET);
        RS_DWORD dwFlag = 0;
        if(1 != fread(&dwFlag, sizeof(RS_DWORD), 1, fp))
                return ERR_CFG_FILE_READFAILED;

        RS_SWAPDWORDEX(dwFlag);
        //the file is correct or not?
        if(dwFlag != 0x47464352)
                return ERR_CFG_FILE_TYPEINVALID;

        //取数据文件的版本
        fseek(fp, 4, SEEK_SET);
        RS_DWORD dwVer = 0;
        if(1 != fread(&dwVer, sizeof(RS_DWORD), 1, fp))
                return ERR_CFG_FILE_READFAILED;
        RS_SWAPDWORDEX(dwVer);
        if(dwVer <= 0x10001001)	//是新版本文件?
                return ERR_CFG_FILE_TYPEINVALID;

        STCONFIGHEADER *t = 0;
        RS_DDWORD dwOffset = (RS_DDWORD)(&t->szProductName) - (RS_DDWORD)(t);
        fseek(fp, dwOffset, SEEK_SET);
        if(1 != fread(pszPrtname, sizeof(RS_CHAR)*16, 1, fp))
                return ERR_CFG_FILE_READFAILED;
        if(pszPrtname[15]!=0)
                pszPrtname[15] = 0;
        //////////////////////////////////////////////////////////

        fclose(fp);
        return ERR_SUCCESS;
}

//取产品名
RS_DWORD CTree::GetProductName(FILE *fp, RS_CHAR *pszPrtname)
{
        if((!fp) || (!pszPrtname))
                return ERR_CFG_PARAM_INVALID;
        RS_DWORD dwOldOffset = ftell(fp);

        STCONFIGHEADER *t = 0;
        RS_DDWORD dwOffset = (RS_DDWORD)(&t->szProductName) - (RS_DDWORD)(t);
        fseek(fp, dwOffset, SEEK_SET);
        if(1 != fread(pszPrtname, sizeof(RS_CHAR)*16, 1, fp))
                return ERR_CFG_FILE_READFAILED;
        if(pszPrtname[15]!=0)
                pszPrtname[15] = 0;
        fseek(fp, dwOldOffset, SEEK_SET);
        return ERR_SUCCESS;
}

RS_DWORD CTree::LoadTreeFromFileOLD(RS_CHAR* pszFileName)
{
        RS_DWORD dwRet = ERR_SUCCESS;

        //get config data file's full name
        if(pszFileName == RS_NULL)
        {
#ifdef UNIXPLATFORM
                RS_CHAR* pPos=NULL;
#else
                GetModuleFileName((HMODULE)g_hInstance, m_szFileName, 1024);
                RS_CHAR* pPos = strrchr(m_szFileName, '\\');
#endif
                if(pPos)
                {
                        *(pPos+1) = 0;
                        strcat(m_szFileName, "RsConfig.cfg");
                }
                else
                {
                        strcpy(m_szFileName, "RsConfig.cfg");
                }
        }
        else
        {
                strcpy(m_szFileName, pszFileName);
        }

        FILE* pFile = fopen(m_szFileName, "r+b");	//try to open file
        if(!pFile)
        {
                //解决受限用户不能以可写方式打开文件的问题
                pFile = fopen(m_szFileName, "rb");
                if(!pFile)
                        return ERR_CFG_FILE_OPENFAILED;
        }

        MyTrace("真正的文件名＝＝%s", m_szFileName);
        //////////////////////////////////////////////////////////
        //取数据文件的标志
        fseek(pFile, 0, SEEK_SET);
        RS_DWORD dwFlag = 0;
        if(1 != fread(&dwFlag, sizeof(RS_DWORD), 1, pFile))
                return ERR_CFG_FILE_READFAILED;
        //the file is correct or not?
        RS_SWAPDWORDEX(dwFlag);
        if(dwFlag != 0x47464352)
                return ERR_CFG_FILE_TYPEINVALID;

        //取数据文件的版本
        fseek(pFile, 4, SEEK_SET);
        RS_DWORD dwVer = 0;
        if(1 != fread(&dwVer, sizeof(RS_DWORD), 1, pFile))
                return ERR_CFG_FILE_READFAILED;
        RS_SWAPDWORDEX(dwVer);
        if(dwVer <= 0x10001001)	//是新版本文件?
                return ERR_CFG_FILE_TYPEINVALID;

        STCONFIGHEADEROLD stConfig = {0};			//file header
        fseek(pFile, 0, SEEK_END);
        RS_DWORD dwFileLen = ftell(pFile);		//get file size
        if(dwFileLen==0)
                return ERR_CFG_FILE_ISEMPTY;

        //get file header
        fseek(pFile, 0, SEEK_SET);
        if(1 != fread(&stConfig, sizeof(STCONFIGHEADEROLD), 1, pFile))
                return ERR_CFG_FILE_READFAILED;

        //the file is correct or not?
        RS_SWAPDWORDEX(stConfig.dwFileFlag );
        if(stConfig.dwFileFlag != 0x47464352)
                return ERR_CFG_FILE_TYPEINVALID;

        bool bNeedExcludeOpenFlag = RS_FALSE;
        //initialize shared memory using Data length and extra size
        if(!CSharedMem::IsInitialized())
        {
                //取共享内存的名字并初始化共享内存
                RS_CHAR szProductName[16] = {0};
                dwRet = GetProductName(pFile, szProductName);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
                //512k的额外空间
                if(!CSharedMem::Initialize(dwFileLen + 0x80000, szProductName, bNeedExcludeOpenFlag))
                        return ERR_CFG_SHM_INITFAILED;
        }

        //get the shared memory address
        RS_BYTE *pData = RS_NULL;
        if(m_dwLoadMode==LOAD_MODE_INIT)
                pData = (RS_BYTE *)CSharedMem::GetPrivateData();
        if(RS_NULL == pData)
        {
                //alloc a new block memory for data use
                pData = MYNEW(RS_BYTE, dwFileLen);
                if(!pData)
                        return ERR_CFG_SHM_ALLOCFAILED;
                memset(pData, 0, dwFileLen);
                MyTrace("MODE_ADD_SHM New Addr==0x%08x Len==%d", (RS_DDWORD)((RS_BYTE *)pData), dwFileLen);

                //load the config tree into shared memory from data file
                fseek(pFile, 0, SEEK_SET);
                if(1 != fread(pData, dwFileLen, 1, pFile))
                        return ERR_CFG_FILE_READFAILED;

                //close file handler
                fclose(pFile);

                //decode data buffer
                dwRet = DecodeDataOLD(pData);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;

                if(m_dwLoadMode==LOAD_MODE_INIT)
                {
                        //store the base address
                        CSharedMem::SetPrivateData((RS_DDWORD)pData);
                        //保存共享内存的长度
                        ((STCONFIGHEADEROLD *)pData)->dwSHMLength = dwFileLen;
                        //保存文件名
                        if(m_szFileName)
                        {
                                memset(((STCONFIGHEADEROLD *)pData)->szFileName, 0, sizeof(((STCONFIGHEADEROLD *)pData)->szFileName));
                                strcpy(((STCONFIGHEADEROLD *)pData)->szFileName, m_szFileName);
                                //MyTrace("Copy Share FileName ==%s", ((STCONFIGHEADEROLD *)pData)->szFileName);
                        }
                }
        }
        else
        {
                //use mapping gap to adjust data pointer, because mapping memory's address is different in different process
                pData += (RS_DWORD)(CSharedMem::GetMappingGap());
                //modify the memory's length
                dwFileLen = ((STCONFIGHEADEROLD *)(pData))->dwSHMLength;
        }
        if(m_dwLoadMode==LOAD_MODE_INIT)
        {
                //initialize global variables
                g_pBasePtr = (void *)pData;
                g_dwSHMStart = (RS_DDWORD)pData;
                g_dwSHMEnd = (RS_DDWORD)pData + dwFileLen;
        }
        else
        {
                g_pAddrImport = pData;
        }

        m_pRoot = (CNode *)(pData + ((STCONFIGHEADEROLD *)(pData))->dwDataOffset);
        //追加模式则需要调整指针
        if(m_dwLoadMode==LOAD_MODE_ADD)
        {
                RS_DDWORD dwAdjust = (RS_DDWORD)(pData - CSharedMem::GetPrivateData() - (RS_DDWORD)(CSharedMem::GetMappingGap()));
                AdjustOffset(m_pRoot, dwAdjust);		//调整这棵树的偏移
        }
        else
        {
                if(bNeedExcludeOpenFlag)
                {
                        MyTrace("Exclude All Open Status !!!!");
                        ExcludeOpenStatus(m_pRoot, CONFIG_STATE_OPENED);
                }
        }
        return dwRet;
}

RS_DWORD CTree::LoadTreeFromFile(const RS_CHAR* pszFileName)
{
        CMemMutex memMutex(&g_cMutexMem);
        RS_DWORD dwRet = ERR_SUCCESS;

        //get config data file's full name
        if(pszFileName == RS_NULL)
        {
#ifdef UNIXPLATFORM
                RS_CHAR* pPos=NULL;
#else
                GetModuleFileName((HMODULE)g_hInstance, m_szFileName, 1024);
                RS_CHAR* pPos = strrchr(m_szFileName, '\\');
#endif
                if(pPos)
                {
                        *(pPos+1) = 0;
                        strcat(m_szFileName, "RsConfig.cfg");
                }
                else
                {
                        strcpy(m_szFileName, "RsConfig.cfg");
                }
        }
        else
        {
                strcpy(m_szFileName, pszFileName);
        }

        FILE* pFile = fopen(m_szFileName, "r+b");	//try to open file
        if(!pFile)
        {
                //解决受限用户不能以可写方式打开文件的问题
                pFile = fopen(m_szFileName, "rb");
                if(!pFile)
                        return ERR_CFG_FILE_OPENFAILED;
        }

        MyTrace("真正的文件名＝＝%s", m_szFileName);
        //////////////////////////////////////////////////////////
        //取数据文件的标志
        fseek(pFile, 0, SEEK_SET);
        RS_DWORD dwFlag = 0;
        if(1 != fread(&dwFlag, sizeof(RS_DWORD), 1, pFile))
                return ERR_CFG_FILE_READFAILED;
        RS_SWAPDWORDEX(dwFlag);
        //the file is correct or not?
        if(dwFlag != 0x47464352)
                return ERR_CFG_FILE_TYPEINVALID;

        //取数据文件的版本
        fseek(pFile, 4, SEEK_SET);
        RS_DWORD dwVer = 0;
        if(1 != fread(&dwVer, sizeof(RS_DWORD), 1, pFile))
                return ERR_CFG_FILE_READFAILED;
        RS_SWAPDWORDEX(dwVer);
        if(dwVer <= 0x10001001)	//是新版本文件?
        {
                fclose(pFile);
                //装入旧版本文件
#ifdef FOR_CFG_EDITOR
                return LoadTreeFromFileOLD(m_szFileName);
#endif
                return ERR_FAILED;
        }

        STCONFIGHEADER stConfig = {0};			//file header
        fseek(pFile, 0, SEEK_END);
        RS_DWORD dwFileLen = ftell(pFile);		//get file size
        if(dwFileLen==0)
                return ERR_CFG_FILE_ISEMPTY;

        //get file header
        fseek(pFile, 0, SEEK_SET);
        if(1 != fread(&stConfig, sizeof(STCONFIGHEADER), 1, pFile))
                return ERR_CFG_FILE_READFAILED;
        SwapHeader(&stConfig);
        //the file is correct or not?
        if(stConfig.dwFileFlag != 0x47464352)
                return ERR_CFG_FILE_TYPEINVALID;

        bool bNeedExcludeOpenFlag = RS_FALSE;
        //initialize shared memory using Data length and extra size
        if(!CSharedMem::IsInitialized())
        {
                //取共享内存的名字并初始化共享内存
                RS_CHAR szProductName[16] = {0};
                dwRet = GetProductName(pFile, szProductName);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
                //512k的额外空间
                if(!CSharedMem::Initialize(dwFileLen + 0x80000, szProductName, bNeedExcludeOpenFlag))
                        return ERR_CFG_SHM_INITFAILED;
        }

        //get the shared memory address
        RS_BYTE *pData = RS_NULL;
        if(m_dwLoadMode==LOAD_MODE_INIT)
                pData = (RS_BYTE *)CSharedMem::GetPrivateData();
        if(RS_NULL == pData)
        {
                //alloc a new block memory for data use
                pData = MYNEW(RS_BYTE, dwFileLen);
                if(!pData)
                {
                        InnerTraceEx("Alloc Error Mem Length = 0x%x",dwFileLen);
                        return ERR_CFG_SHM_ALLOCFAILED;
                }
                InnerTraceEx("Alloc Succe Mem Length = 0x%x; Start=0x%x", dwFileLen, pData);

                memset(pData, 0, dwFileLen);
                MyTrace("MODE_ADD_SHM New Addr==0x%08x Len==%d", (RS_DDWORD)((RS_BYTE *)pData), dwFileLen);

                //load the config tree into shared memory from data file
                fseek(pFile, 0, SEEK_SET);
                if(1 != fread(pData, dwFileLen, 1, pFile))
                        return ERR_CFG_FILE_READFAILED;

                //close file handler
                fclose(pFile);

                //decode data buffer
                dwRet = DecodeData(pData);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;

                if(m_dwLoadMode==LOAD_MODE_INIT)
                {
                        //store the base address
                        CSharedMem::SetPrivateData((RS_DDWORD)pData);
                        //保存共享内存的长度
                        ((STCONFIGHEADER *)pData)->dwSHMLength = dwFileLen;
                        //保存文件名
                        if(m_szFileName)
                        {
                                memset(((STCONFIGHEADER *)pData)->szFileName, 0, sizeof(((STCONFIGHEADER *)pData)->szFileName));
                                strcpy(((STCONFIGHEADER *)pData)->szFileName, m_szFileName);
                                //MyTrace("Copy Share FileName ==%s", ((STCONFIGHEADER *)pData)->szFileName);
                        }
                }
        }
        else
        {
                //use mapping gap to adjust data pointer, because mapping memory's address is different in different process
                pData += (RS_DWORD)(CSharedMem::GetMappingGap());
                //modify the memory's length
                dwFileLen = ((STCONFIGHEADER *)(pData))->dwSHMLength;
        }
        if(m_dwLoadMode==LOAD_MODE_INIT)
        {
                //initialize global variables
                g_pBasePtr = (void *)pData;
                g_dwSHMStart = (RS_DDWORD)pData;
                g_dwSHMEnd = (RS_DDWORD)pData + dwFileLen;
        }
        else
        {
                g_pAddrImport = pData;
        }

        m_pRoot = (CNode *)(pData + ((STCONFIGHEADER *)(pData))->dwDataOffset);
        //追加模式则需要调整指针
        if(m_dwLoadMode==LOAD_MODE_ADD)
        {
                RS_DDWORD dwAdjust = (RS_DDWORD)(pData - CSharedMem::GetPrivateData() - (RS_DDWORD)(CSharedMem::GetMappingGap()));
                AdjustOffset(m_pRoot, dwAdjust);		//调整这棵树的偏移
        }
        else
        {
                if(bNeedExcludeOpenFlag)
                {
                        MyTrace("Exclude All Open Status !!!!");
                        ExcludeOpenStatus(m_pRoot, CONFIG_STATE_OPENED);
                }
        }
        return dwRet;
}

RS_DWORD CTree::InitTree(const RS_CHAR* pszFileName)
{
        CMemMutex loadMutex(&g_cMutexLoad);
        RS_DWORD dwRet = ERR_SUCCESS;
        m_pRoot = RS_NULL;
        m_pControl = RS_NULL;
        RS_CHAR szProductName[128] = {0};

        MyTrace("Load File == %s", pszFileName?pszFileName:"NULL");
        //if this is the first time to load tree
        if(CSharedMem::IsInitialized())
        {
                if(m_dwLoadMode==LOAD_MODE_INIT)	//get the base pointer and root node pointer directly
                {
                        RS_BYTE *pData = RS_NULL;
                        pData = (RS_BYTE *)CSharedMem::GetPrivateData();
                        pData += (RS_DWORD)CSharedMem::GetMappingGap();
                        if(pData)
                        {
                                if(!g_pBasePtr)
                                        g_pBasePtr = (void *)pData;
                                m_pRoot = (CNode *)(pData + ((STCONFIGHEADER *)(pData))->dwDataOffset);
                                g_dwSHMStart = (RS_DDWORD)pData;
                                g_dwSHMEnd = (RS_DDWORD)pData + ((STCONFIGHEADER *)(pData))->dwSHMLength ;
                        }
                        MyTrace("初始模式装入! 共享内存已存在! FileName== %s", ((STCONFIGHEADER *)g_pBasePtr)->szFileName);

                        //初始化写内存互斥量
                        if(!g_bIsMutexWriteCreated)
                        {
                                //在这里创建写内存的互斥量
                                strncpy(szProductName, ((STCONFIGHEADER *)g_pBasePtr)->szProductName, 128);
                                dwRet = g_cMutexMem.Create(MUTEX_WRITE_MEM, szProductName);
                                if( ( ERR_SUCCESS != dwRet) && (ERR_CFG_MUTEX_EXISITED != dwRet) )
                                        return dwRet;
                                g_bIsMutexWriteCreated = RS_TRUE;
                        }
                }
                else if(m_dwLoadMode==LOAD_MODE_ADD)		//在共享内存中装入另一棵树
                {
                        MyTrace("追加模式装入! FileName==%s", pszFileName);
                        dwRet = LoadTreeFromFile(pszFileName);
                        if(dwRet != ERR_SUCCESS)
                                return dwRet;
                        m_szFileName[0] = 0;		//这种情况需要将文件名置空，以便保存到上一个文件
                }
        }
        else
        {
                //获得写互斥量的名字
                dwRet = GetProductNameFromFile((RS_CHAR *)pszFileName, szProductName);
                if(dwRet != ERR_SUCCESS)
                        return dwRet;
                //初始化写内存互斥量
                if(!g_bIsMutexWriteCreated)
                {
                        //在这里创建写内存的互斥量
                        dwRet = g_cMutexMem.Create(MUTEX_WRITE_MEM, szProductName);
                        if( ( ERR_SUCCESS != dwRet) && (ERR_CFG_MUTEX_EXISITED != dwRet) )
                                return dwRet;
                        g_bIsMutexWriteCreated = RS_TRUE;
                }

                //装入数据文件
                MyTrace("初始模式装入! FileName==%s", pszFileName);
                dwRet = LoadTreeFromFile(pszFileName);
                if(dwRet != ERR_SUCCESS)
                        return dwRet;
        }

// Deleted by LauranceZhang 2004-10-28 19:25:23 <
/*
        //在这里初始化引用计数
        if( !g_plRefRead && !g_plRefReadRequest && !g_plRefWrite)
        {
                if(g_pBasePtr)
                {
                        //以下内容修改需要互斥
                        CMemMutex memMutex(2, RS_NULL, &g_cMutexMem);
                        g_plRefRead = (RS_LONG *) (((STCONFIGHEADER *)(g_pBasePtr))->szPrivateData);
                        g_plRefReadRequest = (RS_LONG *)((RS_BYTE *)g_plRefRead + sizeof(RS_LONG));
                        g_plRefWrite = (RS_LONG *)((RS_BYTE *)g_plRefReadRequest + sizeof(RS_LONG));
                        *g_plRefWrite +=1;
                }
        }
*/
// Deleted by LauranceZhang 2004-10-28 19:25:23 >
        return ERR_SUCCESS;
}
RS_VOID CTree::AdjustOffset(CNode*pRoot, RS_DWORD dwAdjust)
{
        CNode* pCur = pRoot;
        CNode* pChild = RS_NULL;
        CNode* pSibling = RS_NULL;

        while(pCur)
        {
                pCur->Swap();
                RS_BYTE *pData = pCur->GetDataEntry();
                if(pData)
                        pCur->SetDataEntry( pData + dwAdjust);
                RS_CHAR *pName = pCur->GetNameEntry();
                if(pName)
                        pCur->SetNameEntry( pName + dwAdjust);

                RS_CHAR *pMemo = pCur->GetMemoEntry();
                if(pMemo)
                        pCur->SetMemoEntry( pMemo + dwAdjust);

                pChild = pCur->GetChild();
                if(pChild)
                {
                        pChild = (CNode*)((RS_BYTE *)pChild + dwAdjust);
                        pCur->SetChild(pChild);
                        AdjustOffset(pChild, dwAdjust);
                }

                pSibling = pCur->GetSibling();
                if(pSibling)
                {
                        pSibling = (CNode*)((RS_BYTE *)pSibling + dwAdjust);
                        pCur->SetSibling(pSibling);
                }
                pCur = pSibling;
        }
}

RS_VOID CTree::ExcludeOpenStatus(CNode*pRoot, RS_DWORD dwStatus)
{
        CNode* pCur = pRoot;
        CNode* pChild = RS_NULL;

        while(pCur)
        {
                pCur->Swap();
                pCur->ExcludeStatus(dwStatus);
                pChild = pCur->GetChild();
                if(pChild)
                        ExcludeOpenStatus(pChild, dwStatus);

                pCur = pCur->GetSibling();
        }
}

// Modified by LauranceZhang 2004-8-21 10:40:01 <
/*
RS_BOOL CTree::WriteFileHeader(FILE *fp)
{
        if (!fp)
                return RS_FALSE;

        RS_BYTE *pData = RS_NULL;

        if(CSharedMem::IsInitialized())
                pData = (RS_BYTE *)CSharedMem::GetPrivateData();

        if(!pData)		//No shared memory;
        {
                STCONFIGHEADER stConfig = {0};
                stConfig.dwFileFlag = 0x47464352;
                stConfig.dwVersion = 0x10001000;
                stConfig.dwCompressKeyLen = 0x10;
                stConfig.dwEncryptKeyLen = 0x10;

                RS_DWORD dwTmp = 0x10 - (stConfig.dwCompressKeyLen + stConfig.dwEncryptKeyLen) %0x10;
                if(dwTmp==0x10)
                        dwTmp = 0;
                stConfig.dwDataOffset = sizeof(STCONFIGHEADER) + stConfig.dwCompressKeyLen + stConfig.dwEncryptKeyLen + dwTmp;
                stConfig.dwHeaderLen =  sizeof(STCONFIGHEADER) + stConfig.dwCompressKeyLen + stConfig.dwEncryptKeyLen + dwTmp;

                //写创建时间和发布时间
                SYSTEMTIME time;
                GetSystemTime(&time);
                sprintf(stConfig.szCreateTime, "%d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
                sprintf(stConfig.szReleaseTime, "%d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

                fseek(fp, 0, SEEK_SET);
                if(1 != fwrite(&stConfig, sizeof(STCONFIGHEADER), 1, fp))
                        return RS_FALSE;

                RS_CHAR *pchData = RS_NULL;
                if(stConfig.dwEncryptKeyLen != 0)
                {
                        pchData = new RS_CHAR[stConfig.dwEncryptKeyLen];
                        if(!pchData)
                                return FALSE;
                        memset(pchData, 0, stConfig.dwEncryptKeyLen);

                        if(1 != fwrite(pchData, sizeof(RS_CHAR)*stConfig.dwEncryptKeyLen, 1, fp))
                                return RS_FALSE;
                        delete []pchData;
                }
                if(stConfig.dwCompressKeyLen != 0)
                {
                        pchData = new RS_CHAR[stConfig.dwCompressKeyLen];
                        if(!pchData)
                                return FALSE;
                        memset(pchData, 0, stConfig.dwCompressKeyLen);

                        if(1 != fwrite(pchData, sizeof(RS_CHAR)*stConfig.dwCompressKeyLen, 1, fp))
                                return RS_FALSE;
                        delete []pchData;
                }

                if(dwTmp != 0)
                {
                        pchData = new RS_CHAR[dwTmp];
                        if(!pchData)
                                return FALSE;
                        memset(pchData, 'A', stConfig.dwCompressKeyLen);

                        if(dwTmp != fwrite(pchData, 1, dwTmp, fp))
                                return RS_FALSE;
                        delete []pchData;
                }
        }
        else
        {

                RS_DWORD dwGap = CSharedMem::GetMappingGap();
                pData += dwGap;

                STCONFIGHEADER *pHeader = (STCONFIGHEADER *)pData;
                fseek(fp, 0, SEEK_SET);
                if(1 != fwrite(pHeader, sizeof(STCONFIGHEADER), 1, fp))
                        return RS_FALSE;
                if(1 != fwrite(pData + sizeof(STCONFIGHEADER), pHeader->dwEncryptKeyLen, 1, fp))
                        return RS_FALSE;
                if(1 != fwrite(pData + sizeof(STCONFIGHEADER) + pHeader->dwEncryptKeyLen, pHeader->dwCompressKeyLen, 1, fp))
                        return RS_FALSE;
        }
        return RS_TRUE;
}
*/
RS_DWORD CTree::WriteFileHeader(FILE *fp, STCONFIGHEADER *pHeader)
{
        if (!fp)
                return ERR_CFG_PARAM_INVALID;

        if(pHeader)		//有参数传入，这种情况是给编辑器使用
        {
                pHeader->dwFileFlag = 0x47464352;
                pHeader->dwHeaderLen = sizeof(STCONFIGHEADER);
                pHeader->dwDataOffset = sizeof(STCONFIGHEADER);

                SwapHeader(pHeader);
                if(1 != fwrite(pHeader, sizeof(STCONFIGHEADER), 1, fp))
                        return ERR_CFG_FILE_WRITEFAILED;
        }
        else
        {
                RS_BYTE *pData = RS_NULL;

                if(CSharedMem::IsInitialized())
                        pData = (RS_BYTE *)CSharedMem::GetPrivateData();

                if(!pData)		//No shared memory;
                {
                        STCONFIGHEADER stConfig = {0};

                        stConfig.dwFileFlag = 0x47464352;
                        stConfig.dwVersion = 0x10001001;
                        stConfig.dwHeaderLen = sizeof(STCONFIGHEADER);
                        stConfig.dwFileLen;

                        stConfig.dwProductID;
                        stConfig.dwDataOffset = sizeof(STCONFIGHEADER);;
                        stConfig.dwCheckSum1 = 1;
                        stConfig.dwCheckSum2 = 1;

//			stConfig.szProductName[16];

                        //写创建时间和发布时间
#ifdef UNIXPLATFORM
                        time_t lcTime;
                        struct tm *stTime={0};
                        char aszTime[20] = {0};
                        lcTime=time(0);
                        stTime=localtime(&lcTime);
                        sprintf(stConfig.szCreateTime, "%04d-%02d-%02d %02d:%02d:%02d", stTime->tm_year+1900,
                                stTime->tm_mon+1, stTime->tm_mday, stTime->tm_hour, stTime->tm_min,
                                stTime->tm_sec);
                        sprintf(stConfig.szReleaseTime, "%04d-%02d-%02d %02d:%02d:%02d", stTime->tm_year+1900,
                                stTime->tm_mon+1, stTime->tm_mday, stTime->tm_hour, stTime->tm_min,
                                stTime->tm_sec);

#else
                        SYSTEMTIME time;
                        GetSystemTime(&time);
                        sprintf(stConfig.szCreateTime, "%d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
                        sprintf(stConfig.szReleaseTime, "%d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#endif
                        stConfig.dwEncryptMethod = 1;
                        stConfig.dwEncryptKeyLen = strlen(stConfig.szCreateTime);
                        stConfig.dwCompressMethod = 0;
                        stConfig.dwCompressKeyLen = 0;

                        strcpy((RS_CHAR *)stConfig.byEncryptKey, stConfig.szCreateTime);
//			stConfig.dwCompressKey[32];

//			stConfig.szFileName[320];
//			stConfig.szPrivateData[256];

                        fseek(fp, 0, SEEK_SET);
                        SwapHeader(pHeader);
                        if(1 != fwrite(&stConfig, sizeof(STCONFIGHEADER), 1, fp))
                                return ERR_CFG_FILE_WRITEFAILED;
                }
                else
                {
                        pData += (RS_DWORD)CSharedMem::GetMappingGap();

                        STCONFIGHEADER *pHead = (STCONFIGHEADER *)pData;
                        fseek(fp, 0, SEEK_SET);
                        SwapHeader(pHead);
                        if(1 != fwrite(pHead, sizeof(STCONFIGHEADER), 1, fp))
                                return ERR_CFG_FILE_WRITEFAILED;
                        SwapHeader(pHead);
                }
        }
        return ERR_SUCCESS;
}

// Modified by LauranceZhang 2004-8-21 10:40:01 >

RS_DWORD CTree::SaveToFile(RS_VOID *pHeader)
{
        RS_DWORD dwRet = ERR_SUCCESS;
        FILE* pFile = RS_NULL;

        if(g_pBasePtr)
        {
                STCONFIGHEADER *pHeader = (STCONFIGHEADER *)g_pBasePtr;
                if(pHeader->szFileName)
                {
#ifndef FOR_CFG_EDITOR
                        strcpy(m_szFileName, pHeader->szFileName);
#endif
                        //MyTrace("取到的共享文件名==%s", m_szFileName);
                }
        }

        if(0 == strlen(m_szFileName))
        {
#ifdef UNIXPLATFORM
                RS_CHAR* pPos=NULL;
#else
                GetModuleFileName((HMODULE)g_hInstance, m_szFileName, 1024);
                RS_CHAR* pPos = strrchr(m_szFileName, '\\');
#endif
                if(pPos != RS_NULL)
                {
                        *(pPos+1) = 0;
                        strcat(m_szFileName, "RsConfig.cfg");
                }
                else
                {
                        strcpy(m_szFileName, "RsConfig.cfg");
                }
        }

        //MyTrace("最后的文件名==%s", m_szFileName);
#ifdef FOR_CFG_EDITOR
        strcat(m_szFileName,"_new");
#endif

        pFile = fopen(m_szFileName, "wb");
        if(!pFile)
        {
                //RS_DWORD dwErrCode = GetLastError();
                FormatErrorMessage(ERR_CFG_FILE_OPENFAILED, __FILE__, __LINE__, "SaveToFile");
                return ERR_CFG_FILE_OPENFAILED;
        }
        dwRet = WriteFileHeader(pFile, (STCONFIGHEADER *)pHeader);
        fclose(pFile);
        if(dwRet != ERR_SUCCESS)		//write file header using dafault value
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SaveToFile");
                return dwRet;
        }

        pFile = fopen(m_szFileName, "r+b");
        if(!pFile)
        {
                //RS_DWORD dwErrCode = GetLastError();
                FormatErrorMessage(ERR_CFG_FILE_OPENFAILED, __FILE__, __LINE__, "SaveToFile");
                return ERR_CFG_FILE_OPENFAILED;
        }

        fseek(pFile, 0, SEEK_END);

        //dwReserved2的最低位表示是否去掉名字信息
        //dwReserved2的低2位表示编辑器加载时是否需要密码
        RS_BOOL bRemoveInfo = pHeader ? ((STCONFIGHEADER *)pHeader)->dwPrivate & 0x1: 0;
        dwRet = SaveTree(m_pRoot, pFile, RS_TRUE, bRemoveInfo);
        if(dwRet != ERR_SUCCESS)
        {
                fclose(pFile);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SaveToFile");
                return dwRet;
        }

        dwRet = EncodeFile(pFile);
        if(dwRet != ERR_SUCCESS)
        {
                fclose(pFile);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SaveToFile");
                return dwRet;
        }
        fclose(pFile);

        return dwRet;
}

//RS_DWORD CTree::SaveTree(CNode* pRoot, FILE* fp, RS_BOOL bSaveSibling, RS_BOOL bRemoveInfo)
RS_DWORD CTree::SaveTree(CNode* pRoot, FILE* fp, RS_BOOL bSaveSibling, RS_BOOL bRemoveInfo, CNode* pParent)
{
        if(pRoot == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode node = *pRoot;
        RS_DWORD dwHeadOffset = 0;
        RS_DWORD dwChildOffset = 0;
        RS_DWORD dwSiblingOffset = 0;
        RS_DWORD dwCurOffset = 0;
        RS_DWORD dwWrited = 0;

        // 已删除的结点的处理方式（直接处理其兄弟结点）
        if((node.GetStatus() & CONFIG_STATE_DELETED) == CONFIG_STATE_DELETED)
        {
                //return SaveTree(node.GetSibling(), fp, bSaveSibling, bRemoveInfo);
                return SaveTree(node.GetSibling(), fp, bSaveSibling, bRemoveInfo, pParent);
        }

        // 以下是正常结点的处理方式
        dwHeadOffset = ftell(fp);		// 暂时记录此结点的起始偏移

        //
#ifdef FOR_INNER_MODIFY
        if(pParent && pParent->GetID()==0x1001 && node.GetID()==0x15)
        {
                RS_DWORD dwNameLen = 0;
                pParent->GetName(RS_NULL, dwNameLen);
                if(dwNameLen != 0)
                {
                        RS_CHAR *pszName = new RS_CHAR[dwNameLen + 1];
                        if(!pszName)
                                return ERR_CFG_PARAM_INVALID;
                        pParent->GetName(pszName, dwNameLen);
                        pszName[dwNameLen] = 0;
                        if(strcmp(pszName, "RSMID_ENGINEMAIN")==0)
                        {
                                RS_DWORD dwDataLen = 0;
                                node.GetData(RS_NULL, dwDataLen);
                                if(dwDataLen==4)
                                {
                                        RS_DWORD dwVal = 1;
                                        node.SetData((RS_BYTE *)&dwVal, dwDataLen, CONFIG_DATA_DWORD);
                                }
                        }
                        delete []pszName;
                }
        }
#endif
        //去掉所有锁定状态
//#ifdef _DEBUG
//	RS_DWORD dwState = node.GetStatus();
//	if((dwState & CONFIG_STATE_LOCKED) == CONFIG_STATE_LOCKED)
//		dwState &= ~CONFIG_STATE_LOCKED;
//	node.SetStatus(dwState);
//#endif

        // 写基本数据结构(定长的头)
        dwWrited = fwrite(&node, sizeof(CNode), 1, fp);
        if(dwWrited != 1)
                return ERR_CFG_FILE_WRITEFAILED;
        node.SetNodeEntry(dwHeadOffset);	//更新内存结点在文件中的入口

        RS_DWORD dwLeft = 0;
        RS_BYTE abyBlank[4] = {0};
        // 写数据
        RS_BYTE* pBuffer = RS_NULL;
        RS_DWORD dwDataLen = node.GetDataLen();
        if(dwDataLen != 0)
        {
                RS_DWORD dwDataOffset = ftell(fp);			// 暂时记录数据的起始偏移
                dwWrited = fwrite(node.GetDataEntry(), dwDataLen, 1, fp);
                if(dwWrited != 1)
                        return ERR_CFG_FILE_WRITEFAILED;
                node.SetDataEntry((RS_BYTE *)(dwDataOffset + (RS_BYTE *)g_pBasePtr));	//更新内存数据在文件中的入口
                //加上冗余数据进行32位对齐
                dwLeft = (sizeof(RS_DWORD) - dwDataLen % sizeof(RS_DWORD));
                if(dwLeft!=0)
                {
                        dwWrited = fwrite(abyBlank, dwLeft, 1, fp);
                        if(dwWrited != 1)
                                return ERR_CFG_FILE_WRITEFAILED;
                }
        }

        // 写名字
        RS_DWORD dwNameLen = node.GetNameLen();
        if(bRemoveInfo)
                dwNameLen = 0;
        if(dwNameLen != 0)
        {
                RS_DWORD dwNameOffset = ftell(fp);			// 暂时记录数据的起始偏移
                dwWrited = fwrite(node.GetNameEntry(), dwNameLen, 1, fp);
                if(dwWrited != 1)
                        return ERR_CFG_FILE_WRITEFAILED;
                node.SetNameEntry((RS_CHAR *)(dwNameOffset + (RS_CHAR *)g_pBasePtr));	//更新内存数据在文件中的入口
                dwLeft = (sizeof(RS_DWORD) - dwNameLen % sizeof(RS_DWORD));
                if(dwLeft!=0)
                {
                        dwWrited = fwrite(abyBlank, dwLeft, 1, fp);
                        if(dwWrited != 1)
                                return ERR_CFG_FILE_WRITEFAILED;
                }
        }
        else
        {
                node.SetName(RS_NULL);
        }

        // 写备注
        RS_DWORD dwMemoLen = node.GetMemoLen();
        if(bRemoveInfo)
                dwMemoLen = 0;
        if(dwMemoLen != 0)
        {
                RS_DWORD dwMemoOffset = ftell(fp);			// 暂时记录数据的起始偏移
                dwWrited = fwrite(node.GetMemoEntry(), dwMemoLen, 1, fp);
                if(dwWrited != 1)
                        return ERR_CFG_FILE_WRITEFAILED;
                node.SetMemoEntry((RS_CHAR *)(dwMemoOffset + (RS_CHAR *)g_pBasePtr));	//更新内存数据在文件中的入口
                dwLeft = (sizeof(RS_DWORD) - dwMemoLen % sizeof(RS_DWORD));
                if(dwLeft!=0)
                {
                        dwWrited = fwrite(abyBlank, dwLeft, 1, fp);
                        if(dwWrited != 1)
                                return ERR_CFG_FILE_WRITEFAILED;
                }
        }
        else
        {
                node.SetMemo(RS_NULL);
        }


        RS_DWORD dwChildSaved = ERR_SUCCESS;
        RS_DWORD dwSiblingSaved = ERR_SUCCESS;
        // 保存孩子
        if(node.GetChild() != RS_NULL)
        {
                dwChildOffset = ftell(fp);
                //dwChildSaved = SaveTree(node.GetChild(), fp, RS_TRUE, bRemoveInfo);
                dwChildSaved = SaveTree(node.GetChild(), fp, RS_TRUE, bRemoveInfo, &node);
        }

        // 保存兄弟
        if(bSaveSibling)
        {
                if(node.GetSibling() != RS_NULL)
                {
                        dwSiblingOffset = ftell(fp);
                        //dwSiblingSaved = SaveTree(node.GetSibling(), fp, RS_TRUE, bRemoveInfo);
                        dwSiblingSaved = SaveTree(node.GetSibling(), fp, RS_TRUE, bRemoveInfo, pParent);
                }
        }

        // 刷新此结点数据
        RS_DWORD dwTmp = (dwChildSaved==ERR_SUCCESS) ? dwChildOffset : 0;
        node.SetChild((CNode *)(dwTmp + (RS_CHAR *)g_pBasePtr));
        dwTmp = (dwSiblingSaved==ERR_SUCCESS) ? dwSiblingOffset : 0;
        node.SetSibling((CNode *)(dwTmp + (RS_CHAR *)g_pBasePtr));

        node.Swap();

        dwCurOffset = ftell(fp);
        fseek(fp, dwHeadOffset, SEEK_SET);
        fwrite(&node, sizeof(CNode), 1, fp);
        fseek(fp, dwCurOffset, SEEK_SET);

        return ERR_SUCCESS;
}

CNode* CTree::SearchChildByName(RS_CHAR* pszName, CNode* pParent)
{

        CNode* pCur = pParent;
        CNode* pChild = RS_NULL;

        if(pCur == RS_NULL)
                pCur = m_pRoot;

        if(pCur == RS_NULL)
                return RS_NULL;

        pChild = pCur->GetChild();

        while(pChild)
        {
                RS_DWORD dwLen = pChild->GetNameLen();
                if((dwLen == 0) || (strlen(pszName) != dwLen))
                {
                        pChild = pChild->GetSibling();
                        continue;
                }

                RS_CHAR* pszBuffer = MYNEW(RS_CHAR, dwLen + 1);
                if(!pszBuffer)
                        return RS_NULL;

                pChild->GetName(pszBuffer, dwLen);
                pszBuffer[dwLen] = 0;
                if(0 == memcmp(pszBuffer, pszName, dwLen))
                {
                        MYDELETE(pszBuffer);
                        break;
                }

                MYDELETE(pszBuffer);
                pChild = pChild->GetSibling();
        }

        return pChild;
}

CNode* CTree::SearchSiblingByName(RS_CHAR* pszName, CNode* pSibling)
{

        CNode* pCur = pSibling;
        CNode* pFind = RS_NULL;

        if(pCur == RS_NULL)
                pCur = m_pRoot;

        if(pCur == RS_NULL)
                return RS_NULL;

        pFind = pCur;

        while(pFind)
        {
                RS_DWORD dwLen = pFind->GetNameLen();
                if((dwLen == 0) || (strlen(pszName) != dwLen))
                {
                        pFind = pFind->GetSibling();
                        continue;
                }

                RS_CHAR* pszBuffer = MYNEW(RS_CHAR, dwLen + 1);
                if(!pszBuffer)
                        return RS_NULL;

                pFind->GetName(pszBuffer, dwLen);
                pszBuffer[dwLen] = 0;
                if(0 == memcmp(pszBuffer, pszName, dwLen))
                {
                        MYDELETE(pszBuffer);
                        break;
                }

                MYDELETE(pszBuffer);
                pFind = pFind->GetSibling();
        }

        return pFind;
}

CNode* CTree::SearchChildById(RS_DWORD dwId, CNode* pParent)
{
        CNode* pCur = pParent;
        CNode* pChild = RS_NULL;

        if(pCur == RS_NULL)
                pCur = m_pRoot;

        if(pCur == RS_NULL)
                return RS_NULL;

        pChild = pCur->GetChild();

        while(pChild)
        {
                if((pChild->GetStatus() & CONFIG_STATE_DELETED) != CONFIG_STATE_DELETED)
                {
                        if(pChild->GetID() == dwId)
                        break;
                }
                pChild = pChild->GetSibling();
        }

        return pChild;
}

CNode* CTree::SearchSiblingById(RS_DWORD dwId, CNode* pSibling)
{
        CNode* pCur = pSibling;
        CNode* pFind = RS_NULL;

        if(pCur == RS_NULL)
                pCur = m_pRoot;

        if(pCur == RS_NULL)
                return RS_NULL;

        pFind = pCur;

        while(pFind)
        {
                if(!pFind->IsStatusMatchRule(CONFIG_STATE_DELETED))
                {
                        if(pFind->GetID() == dwId)
                                break;
                }

                pFind = pFind->GetSibling();
        }

        pCur = pFind;
        return pCur;
}


CNode* CTree::InsertChildById(RS_DWORD dwId, CNode* pParent)
{
        CNode m_cDataNode;
        memset(&m_cDataNode, 0, sizeof(CNode));

        //初始状态
        m_cDataNode.SetID(dwId);
        m_cDataNode.SetLevel(7);
        m_cDataNode.AppendStatus(CONFIG_STATE_OPENED);

        CNode* pHead = InsertChild(m_cDataNode,pParent);
        return pHead;
}

CNode* CTree::InsertSiblingById(RS_DWORD dwId, CNode* pSubHead)
{
        CNode m_cDataNode;
        memset(&m_cDataNode, 0, sizeof(CNode));

        //初始状态
        m_cDataNode.SetID(dwId);
        m_cDataNode.SetLevel(7);
        m_cDataNode.AppendStatus(CONFIG_STATE_OPENED);

        CNode* pHead = RS_NULL;
        pHead = InsertSibling(m_cDataNode, pSubHead);
        return pHead;
}


RS_DWORD CTree::OpenItem(const RS_DWORD dwItemId, PHITEM phItem, RS_DWORD dwAccess)
{
        HITEM hItem = SearchItemById(dwItemId, (HITEM)(CNode *)m_pRoot);
        if(hItem == RS_NULL)
                return ERR_CFG_ITEM_SEARCHFAILED;

        CNode* pCur = (CNode*)hItem;

        // 判断是否能以此权限打开
        if(!pCur->IsAccessMatchRule(dwAccess))
                return ERR_CFG_NODE_NORIGHT;

        pCur->AppendStatus(CONFIG_STATE_OPENED);
        pCur->SetAccess(dwAccess);

        *phItem = hItem;
        return ERR_SUCCESS;
}

RS_DWORD CTree::CloseItem(PHITEM phItem)
{
        if(phItem == RS_NULL || *phItem == RS_NULL)
                return ERR_SUCCESS;

        MyTrace("CTree::CloseItem == %x", *phItem);

        ((CNode*)(*phItem))->ExcludeStatus(CONFIG_STATE_OPENED);
        *phItem = 0;
        return ERR_SUCCESS;
}

RS_DWORD CTree::SetItemName(HITEM hItem, RS_CHAR* pszName)
{
        if(hItem == 0)
                return ERR_CFG_PARAM_INVALID;
        return ((CNode*)hItem)->ResetName(pszName);
}

RS_DWORD CTree::SetItemMemo(HITEM hItem, RS_CHAR* pszMemo)
{
        if(hItem == 0)
                return ERR_CFG_PARAM_INVALID;
        return ((CNode*)hItem)->ResetMemo(pszMemo);
}

RS_DWORD CTree::SetItemValue(HITEM hItem, RS_DWORD dwDataType, const RS_BYTE* pbyData, RS_DWORD dwSize)
{
        if(hItem == 0)
                return ERR_CFG_PARAM_INVALID;

        if(((CNode*)hItem)->IsStatusMatchRule(CONFIG_STATE_LOCKED))
                return ERR_CFG_NODE_LOCKED;
        return ((CNode*)hItem)->ResetData(pbyData, dwSize, dwDataType);
}

RS_DWORD CTree::SetItemValueBYTE(HITEM hItem, RS_BYTE byData)
{
        return SetItemValue(hItem, CONFIG_DATA_BYTE, &byData, sizeof(RS_BYTE));
}

RS_DWORD CTree::SetItemValueWORD(HITEM hItem, RS_WORD wData)
{
        return SetItemValue(hItem, CONFIG_DATA_WORD, (RS_BYTE*)&wData, sizeof(RS_WORD));
}

RS_DWORD CTree::SetItemValueDWORD(HITEM hItem, RS_DWORD dwData)
{
        return SetItemValue(hItem, CONFIG_DATA_DWORD, (RS_BYTE*)&dwData, sizeof(RS_DWORD));
}

RS_DWORD CTree::SetItemValueBinary(HITEM hItem, RS_BYTE *pbyData, RS_DWORD dwSize)
{
        return SetItemValue(hItem, CONFIG_DATA_BINARY, pbyData, dwSize);
}

RS_DWORD CTree::SetItemValueString(HITEM hItem, RS_UCHAR* pszData, RS_DWORD dwSize)
{
        return SetItemValue(hItem, CONFIG_DATA_STRING, (RS_BYTE*)pszData, dwSize);
}

RS_DWORD CTree::SetItemInfo(HITEM hItem, RS_DWORD dwId, RS_DWORD dwLevel, RS_DWORD dwStatus, RS_CHAR *pszName, RS_CHAR *pszMemo)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;
        if(!pCur->IsAccessMatchRule(CONFIG_ACCESS_WRITE))	// 不支持写
                return ERR_CFG_NODE_NORIGHT;

        pCur->SetID(dwId);
        pCur->SetLevel(dwLevel);
        MyTrace("CTree::SetItemInfo->CONFIG_NODE_STATUS status==%x",dwStatus);
        pCur->SetStatus(dwStatus);
        if(pszName)
                pCur->ResetName(pszName);

        if(pszMemo)
                pCur->ResetMemo(pszMemo);

        return ERR_SUCCESS;
}

RS_DWORD CTree::GetItemMemo(HITEM hItem, RS_CHAR* pszMemo)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;
        if(!pCur->IsAccessMatchRule(CONFIG_ACCESS_READ))	// 不支持读
                return ERR_CFG_NODE_NORIGHT;
        RS_DWORD dwMemoLen = 0;
        return pCur->GetMemo(pszMemo, dwMemoLen);
}

RS_DWORD CTree::GetItemValue(HITEM hItem, RS_DWORD &dwDataType, RS_BYTE *pbyData, RS_DWORD &dwSize)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;

        if(pCur->IsStatusMatchRule(CONFIG_STATE_IS_BRANCH))
                if(!pCur->IsAccessMatchRule(CONFIG_ACCESS_READ))	// 不支持读
                        return ERR_CFG_NODE_NORIGHT;
        dwDataType = pCur->GetDataType();
        return pCur->GetData(pbyData, dwSize);
}

RS_DWORD CTree::GetItemValueBYTE(HITEM hItem, RS_BYTE& byData)
{
        RS_DWORD dwLen = sizeof(RS_BYTE);
        RS_DWORD dwDataType = 0;
        return GetItemValue(hItem, dwDataType, &byData, dwLen);
}

RS_DWORD CTree::GetItemValueWORD(HITEM hItem, RS_WORD& wData)
{
        RS_DWORD dwLen = sizeof(RS_WORD);
        RS_DWORD dwDataType = 0;
        return GetItemValue(hItem, dwDataType, (RS_BYTE*)&wData, dwLen);
}

RS_DWORD CTree::GetItemValueDWORD(HITEM hItem, RS_DWORD& dwData)
{
        RS_DWORD dwLen = sizeof(RS_DWORD);
        RS_DWORD dwDataType = 0;
        return GetItemValue(hItem, dwDataType, (RS_BYTE*)&dwData, dwLen);
}

RS_DWORD CTree::GetItemValueBinary(HITEM hItem, RS_BYTE* pbyData, RS_DWORD& dwSize)
{
        RS_DWORD dwDataType = 0;
        return GetItemValue(hItem, dwDataType, pbyData, dwSize);
}

RS_DWORD CTree::GetItemValueString(HITEM hItem, RS_UCHAR* pszData, RS_DWORD& dwSize)
{
        RS_DWORD dwDataType = 0;
        return GetItemValue(hItem, dwDataType, pszData, dwSize);
}

RS_DWORD CTree::GetInfo(HITEM hItem, RS_DWORD dwInfoId, RS_BYTE* pbyBuffer, RS_DWORD& dwBufSize)
{
        if((hItem == RS_NULL) || (pbyBuffer == RS_NULL))
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD dwLen = 0;

        switch(dwInfoId)
        {
        case CONFIG_ID:
                *((RS_DWORD*)pbyBuffer) = pCur->GetID();
                dwBufSize = sizeof(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_NODE_LEVEL:
                *((RS_DWORD*)pbyBuffer) = pCur->GetLevel();
                dwBufSize = sizeof(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_NODE_STATUS:
                *((RS_DWORD*)pbyBuffer) = pCur->GetStatus();
                dwBufSize = sizeof(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_ITEM_NAME:
                if((dwRet = pCur->GetName((RS_CHAR*)pbyBuffer, dwBufSize)) != ERR_SUCCESS)
                        return dwRet;
                break;
        case CONFIG_ITEM_DATATYPE:
                *((RS_DWORD*)pbyBuffer) = pCur->GetDataType();
                dwBufSize = sizeof(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_ITEM_VALUE:
                if((dwRet = pCur->GetData(pbyBuffer, dwBufSize)) != ERR_SUCCESS)
                        return dwRet;
                break;
        case CONFIG_ITEM_MEMO:
                if((dwRet = pCur->GetMemo((RS_CHAR*)pbyBuffer, dwBufSize)) != ERR_SUCCESS)
                        return dwRet;
                break;
        default:
                return ERR_CFG_PARAM_INVALID;
        }
        return ERR_SUCCESS;
}

RS_DWORD CTree::SetInfo(HITEM hItem, RS_DWORD dwInfoId, const RS_BYTE* pbyBuffer, RS_DWORD dwBufSize)
{
        if((hItem == RS_NULL) || (pbyBuffer == RS_NULL))
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;
        RS_DWORD dwRet = ERR_SUCCESS;

        switch(dwInfoId)
        {
        case CONFIG_ID:
                pCur->SetID(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_NODE_LEVEL:
                pCur->SetLevel(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_NODE_STATUS:
                MyTrace("CTree::SetInfo->CONFIG_NODE_STATUS status==%x",*((RS_DWORD*)pbyBuffer));
                pCur->SetStatus(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_ITEM_NAME:
                {
                        dwRet = pCur->ResetName((RS_CHAR *)pbyBuffer);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;
                }
                break;
        case CONFIG_ITEM_DATATYPE:
                pCur->SetDataType(*((RS_DWORD*)pbyBuffer));
                break;
        case CONFIG_ITEM_VALUE:
                {
                        if(pCur->IsStatusMatchRule(CONFIG_STATE_LOCKED))
                                return ERR_CFG_NODE_LOCKED;
                        dwRet = pCur->ResetData(pbyBuffer, dwBufSize, CONFIG_DATA_BINARY);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;
                }
                break;
        case CONFIG_ITEM_MEMO:
                {
                        dwRet = pCur->ResetMemo((RS_CHAR *)pbyBuffer);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;
                }
                break;
        default:
                return ERR_CFG_PARAM_INVALID;
        }

        return ERR_SUCCESS;
}
RS_DWORD CTree::GetItemInfo(HITEM hItem, RS_DWORD& dwId, RS_DWORD& dwLevel, RS_DWORD& dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;
        if(!pCur->IsAccessMatchRule(CONFIG_ACCESS_READ))// 不支持读
                return ERR_CFG_NODE_NORIGHT;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD dwLen = 0;

        if(pszName != RS_NULL)
        {
                dwLen = 0;
                dwRet = pCur->GetName(pszName, dwLen);
                if(ERR_CFG_BUFFER_OVERFLOW==dwRet)
                {
                        dwRet = pCur->GetName(pszName, dwLen);
                        if(dwRet!= ERR_SUCCESS)
                                return dwRet;
                }
        }

        if(pszMemo != RS_NULL)
        {
                dwLen = 0;
                dwRet = pCur->GetMemo(pszMemo, dwLen);
                if(ERR_CFG_BUFFER_OVERFLOW==dwRet)
                {
                        dwRet = pCur->GetMemo(pszMemo, dwLen);
                        if(dwRet!= ERR_SUCCESS)
                                return dwRet;
                }
        }

        dwId = pCur->GetID();
        dwLevel = pCur->GetLevel();
        dwStatus = pCur->GetStatus();

        return ERR_SUCCESS;
}

HITEM CTree::SearchItemById(RS_DWORD dwId, HITEM hParent)
{
        if(hParent == RS_NULL)
                return RS_NULL;

        HITEM hFind = RS_NULL;
        CNode* pCur = (CNode*)hParent;
        CNode* pChild = RS_NULL;

        while(pCur)
        {
                if(pCur->GetID() == dwId)
                        return (HITEM)pCur;

                if(pChild = pCur->GetChild())
                {
                        hFind = SearchItemById(dwId, (HITEM)pChild);
                        if(hFind)
                                return hFind;
                }

                pCur = pCur->GetSibling();
        }
        return RS_NULL;
}

HITEM CTree::SearchItemByName(RS_CHAR* pszName, HITEM hParent)
{
        if(hParent == RS_NULL)
                return RS_NULL;
        return (HITEM)SearchChildByName(pszName, (CNode*)hParent);
}


RS_DWORD CTree::DeleteItem(HITEM hItem)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)hItem;
        if(	!pCur->IsAccessMatchRule(CONFIG_ACCESS_DELETE) )
                return ERR_CFG_NODE_NORIGHT;
        pCur->AppendStatus(CONFIG_STATE_DELETED);

        if(m_pControl)	//释放内存
        {
                RS_DDWORD dwAddr1 = (RS_DDWORD)((CNode *)pCur);
                RS_DDWORD dwAddr2 = (RS_DDWORD)((CNode *)m_pControl->GetChild());
                if(dwAddr1 == dwAddr2)
                        m_pControl->SetChild(pCur->GetSibling());
                else
                {
                        dwAddr2 = (RS_DDWORD)((CNode *)m_pControl->GetSibling());
                        if(dwAddr1 == dwAddr2)
                                m_pControl->SetSibling(pCur->GetSibling());
                }

                RS_DDWORD dwAddr = (RS_DDWORD)((RS_BYTE *)pCur->GetDataEntry());
                if(((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd)) && (dwAddr != 0))
                {
                        //MyTrace("DeleteData : delete(0x%08x)", dwAddr);
                        MYDELETE((RS_BYTE *)dwAddr);
                        pCur->SetData(RS_NULL,0);
                }
                dwAddr = (RS_DDWORD)((RS_CHAR *)pCur->GetMemoEntry());
                if(((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd)) && (dwAddr != 0))
                {
                        //MyTrace("DeleteMemo : delete(0x%08x)", dwAddr);
                        MYDELETE((RS_CHAR *)dwAddr);
                        pCur->SetMemo(RS_NULL);
                }
                dwAddr = (RS_DDWORD)((RS_CHAR *)pCur->GetNameEntry());
                if(((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd)) && (dwAddr != 0))
                {
                        //MyTrace("DeleteName : delete(0x%08x)", dwAddr);
                        MYDELETE((RS_CHAR *)dwAddr);
                        pCur->SetName(RS_NULL);
                }
                dwAddr = (RS_DDWORD)((CNode *)pCur);
                if(((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd)) && (dwAddr != 0))
                {
                        //MyTrace("DeleteNode : delete(0x%08x)", dwAddr);
                        MYDELETE((CNode *)dwAddr);
                }
        }

        return ERR_SUCCESS;
}
/*
函数功能说明：
1. 如结点存在, 则以ID路径方式打开一个结点;
2. 如结点不存在并且其父结点是打开的并具有创建结点权限, 则创建此结点;
3. 如路径为空并句柄不为空, 则打开此句柄的结点;
4. 如路径为空并句柄为空, 如存在给结点则打开,否则创建根结点;
5. 权限为CONFIG_ACCESS_QUERY是只查询此结点是否存在,如存在则返回句柄;
*/

RS_DWORD CTree::OpenItem(const RS_DWORD* pdwIdPath, PHITEM phItem, RS_DWORD &dwAccess, const RS_DWORD* pdwUser)
{
        CNode* pBase = m_pRoot;				// 父结点,初始为根结点
        CNode* pCur = RS_NULL;				// 当前结点
        CNode* pParent = RS_NULL;			// 查找到的结点的控制结点
        RS_DWORD dwUserAccess = 0;			// 用户固有的权限
        RS_INT i = 0;						// 数组下标
        RS_DWORD dwRet = ERR_SUCCESS;		// 返回值

//==============================================================
//处理路径为空的情况
//==============================================================
        if(pdwIdPath[0] == INVALID_ITEMID)
        {
                if(dwAccess == CONFIG_ACCESS_QUERY)
                {
                        //查询根结点
                        if(pBase != RS_NULL)
                        {
                                *phItem = (HITEM)pBase;
                                return ERR_SUCCESS;
                        }
                        return ERR_CFG_ITEM_NOTEXISTED;
                }
                else
                {
                        //句柄不为空时直接打开此结点
                        if(*phItem != INVALID_ITEMID)
                        {
                                pCur= (CNode*)(*phItem);
                                if(!pCur->IsStatusMatchRule(CONFIG_STATE_OPENED))
                                        pCur->AppendStatus(CONFIG_STATE_OPENED);
                                pCur->SetAccess(dwAccess);
                                return ERR_SUCCESS;
                        }
                        else
                        {
                                //路径为空时默认打开根结点，否则创建根结点
                                if(pBase != RS_NULL)
                                {
                                        pBase->AppendStatus(CONFIG_STATE_OPENED);
                                        pBase->SetAccess(dwAccess);
                                        *phItem = (HITEM)pBase;
                                        return ERR_SUCCESS;
                                }
                        }//if(*phItem != INVALID_ITEMID)
                }//if(dwAccess == CONFIG_ACCESS_QUERY)
        }//if(pdwIdPath[0] == INVALID_ITEMID)

//==============================================================
//按照路径逐级查找
//==============================================================
        while(pdwIdPath[i] && pBase)
        {
                pCur = SearchSiblingById(pdwIdPath[i], pBase);
                if(pCur == RS_NULL)					// 表示此路径不存在
                        break;

                if(pCur && (pdwIdPath[i+1]==0))		//表示已经找到了此结点
                        break;
                pParent = pCur;
                pBase = pCur->GetChild();
                i++;
        }

        //处理特殊情况
        if(pdwIdPath[i]==0)	i--;

//==============================================================
//处理查找到和没找到2种情况
//==============================================================
        if(pCur && pBase && (pdwIdPath[i + 1] == 0))	//真正找到了此结点
        {
                if(dwAccess == CONFIG_ACCESS_QUERY)	//相当于用来查询是否存在此项
                {
                        *phItem = (HITEM)pCur;
                        SetControlNode(pParent, pBase, pCur);
                        return ERR_SUCCESS;
                }

                //取得结点或者是项的固有权限
                if(pCur->IsStatusMatchRule(CONFIG_STATE_IS_BRANCH))
                {
                        //取得当前结点的权限
                        dwRet = GetUserAccess(pdwUser, pCur->GetLevel(), dwUserAccess);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;
                }
                else
                {
                        if(((CNode *)m_pRoot)!= pCur)
                        {
                                if(pParent==RS_NULL)	//不能在根下建立项
                                {
                                        MyTrace("CConfig::OpenItem1->ERR_CFG_NODE_NORIGHT");
                                        return ERR_CFG_NODE_NORIGHT;
                                }
                                //不是超级用户时
                                if((dwAccess & CONFIG_ACCESS_SUPER) == CONFIG_ACCESS_SUPER)
                                {
                                        dwAccess &= ~CONFIG_ACCESS_SUPER;
                                }
                                else
                                {
                                        // 判断父结点是否已经打开
                                        if(!pParent->IsStatusMatchRule(CONFIG_STATE_OPENED))
                                        {
                                                return ERR_CFG_NODE_ISNOTOPENED;
                                        }
                                }
                                //取得父结点的权限
                                dwRet = GetUserAccess(pdwUser, pParent->GetLevel(), dwUserAccess);
                                if(ERR_SUCCESS != dwRet)
                                        return dwRet;
                        }
                }//if(pCur->IsStatusMatchRule(CONFIG_STATE_IS_BRANCH))

                //判断是否能以此权限打开
                if(	(((CNode *)m_pRoot) != pCur) && ((dwAccess & dwUserAccess) != dwAccess) )
                {
                        dwAccess = dwUserAccess;		//返回需要的权限
                        return ERR_CFG_NODE_NORIGHT;
                }
#ifdef FOR_TEST
                if(	pCur->IsStatusMatchRule(CONFIG_STATE_IS_BRANCH) &&
                        pCur->IsStatusMatchRule(CONFIG_STATE_OPENED) &&
                        !pCur->IsAccessMatchRule(CONFIG_ACCESS_SHARE) )
                        return ERR_CFG_NODE_HASOPENED;
#endif
                //MyTrace("Open Node Level==%x", pCur->GetLevel());
        }
        else
        {
                if(dwAccess == CONFIG_ACCESS_QUERY)
                        return ERR_CFG_ITEM_NOTEXIST;
                if(pParent)
                {
                        //取得父结点的权限
                        dwRet = GetUserAccess(pdwUser, pParent->GetLevel(), dwUserAccess);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;

                        if(	((dwAccess & dwUserAccess) != dwAccess)					||			//判断是否与固有权限一致
                                (!pParent->IsStatusMatchRule(CONFIG_STATE_IS_BRANCH))	||			//父是否是结点
                                pParent->IsStatusMatchRule(CONFIG_STATE_LOCKED)	)					//父是否已锁定
                        {
                                MyTrace("CConfig::OpenItem7->ERR_CFG_NODE_NORIGHT");
                                return ERR_CFG_NODE_NORIGHT;
                        }

                        //判断是否有创建权限
                        if((dwAccess & CONFIG_ACCESS_CREATE) != CONFIG_ACCESS_CREATE)
                        {
                                dwAccess &= ~CONFIG_ACCESS_SUPER;
                                dwAccess |= CONFIG_ACCESS_CREATE;
                                return ERR_CFG_NODE_NORIGHT;
                        }

                        //创建新结点
                        pCur = InsertChildById(pdwIdPath[i], pParent);
                        if(!pCur)
                        {
                                MyTrace("CConfig::OpenItem4->ERR_CFG_ITEM_INSERTFAILED");
                                return ERR_CFG_ITEM_INSERTFAILED;
                        }
                        //MyTrace("Create Node Level==%x", pCur->GetLevel());
                }
                else
                {
                        if(pdwIdPath[1])	//此时需要循环创建各级路径结点
                                return ERR_CFG_ITEM_INSERTFAILED;

                        if((dwAccess & CONFIG_ACCESS_CREATE) != CONFIG_ACCESS_CREATE)
                        {
                                MyTrace("CConfig::OpenItem5->ERR_CFG_NODE_NORIGHT");
                                return ERR_CFG_NODE_NORIGHT;
                        }

                        // 创建新结点(根结点)
                        pParent = m_pRoot;
                        pCur = InsertSiblingById(pdwIdPath[0], pParent);
                        if(!pCur)
                        {
                                MyTrace("CConfig::OpenItem2->ERR_CFG_ITEM_INSERTFAILED");
                                return ERR_CFG_ITEM_INSERTFAILED;
                        }
                        if(!m_pRoot)  m_pRoot = pCur;		//第一次创建的根结点
                }//if(pParent)
        }//if(pCur && pBase && (pdwIdPath[i + 1] == 0))

//==============================================================
//保存当前结点的父结点,即控制结点 && 修改当前结点的状态
//==============================================================
        SetControlNode(pParent, pBase, pCur);
        pCur->AppendStatus(CONFIG_STATE_OPENED);
        pCur->SetAccess(dwAccess);
        *phItem = (HITEM)pCur;
        return ERR_SUCCESS;
}

// 以 ID 路径的方式打开一个结点
/*
RS_DWORD CTree::OpenItem(const RS_DWORD* pdwIdPath, PHITEM phItem, RS_DWORD dwAccess, const RS_DWORD* pdwUser, RS_DWORD dwNodeType)
{
        CNode* pParent = m_cTree.GetRoot();
        CNode* pCur = NULL;
        CNode* pParentBak = NULL;
        RS_DWORD dwUserAccess = 0;			// 用户固有权限
        INT i = 0;							// 数组下标

        if(pdwIdPath[0] == INVALID_ITEMID)
        {
                if(*phItem != INVALID_ITEMID)	// 句柄不为空时直接打开此结点
                {
                        pCur= (CNode*)(*phItem);
                        if(!(pCur->m_cdata.GetStatus() & STATE_OPENED))
                        {
                                pCur->m_cdata.SetStatus(STATE_OPENED);
                        }

                        pCur->m_cdata.SetAccess(dwAccess);

                        return ERR_SUCCESS;
                }
                else
                {
                        if(pParent != NULL)		// 路径为空时默认打开根结点，否则创建根结点
                        {
                                pCur = pParent;
                                pCur->m_cdata.SetStatus(STATE_OPENED);
                                pCur->m_cdata.SetAccess(dwAccess);

                                *phItem = (HITEM)pCur;

                                return ERR_SUCCESS;
                        }
                }
        }

        while(pdwIdPath[i] && pParent)
        {
                pCur = SearchSiblingById(pdwIdPath[i], pParent);

                if(pCur == NULL)			// 表示此路径不存在
                        break;

                pParent = pCur->GetChild();
                if(pParent == NULL)			// 表示已经没有孩子了
                        break;

                pParentBak = pCur;
                i++;
        }

        if(pdwIdPath[i]==0)
                i--;

        RS_BOOL bIsCreateNew = RS_FALSE;		// 是否是创建的新结点
        if(pCur)
        {
                if((pCur->m_cdata.GetStatus() & CONFIG_STATE_IS_BRANCH) == STATE_IS_LEAF)
                {
                        // 判断父结点是否已经打开
                        if(!(pParentBak->m_cdata.GetStatus() & STATE_OPENED))
                                return ERR_FAILED;

                        // 取得父结点的权限
                        if(ERR_SUCCESS != GetUserAccess(pdwUser, pParentBak->m_cdata.GetLevel(), dwUserAccess))
                                return ERR_FAILED;
                }
                else
                {
                        // 取得权限
                        if(ERR_SUCCESS != GetUserAccess(pdwUser, pCur->m_cdata.GetLevel(), dwUserAccess))
                                return ERR_FAILED;
                }

                // 判断是否能以此权限打开
                if((dwAccess & dwUserAccess) != dwAccess)
                        return ERR_CFG_NO_RIGHT;

                if(pdwIdPath[i+1] != 0)	// 如果没有找到此结点
                {
                        if(!(dwAccess & CONFIG_ACCESS_CREATE) || !(pCur->m_cdata.GetStatus() & STATE_IS_BRANCH))
                                return ERR_CFG_NO_RIGHT;

                        // 创建新结点
                        pCur = InsertChildById(pdwIdPath[i], pCur);
                        if(!pCur)
                                return ERR_FAILED;

                        pCur->m_cdata.SetAccess(dwAccess);
                        bIsCreateNew = RS_TRUE;
                }
        }
        else
        {
                if(!pParentBak)
                {
                        if(pdwIdPath[1] == 0)	// 表示是创建根结点
                        {
                                if(!(dwAccess & CONFIG_ACCESS_CREATE))
                                        return ERR_CFG_NO_RIGHT;

                                pParentBak = m_cTree.GetRoot();

                                // 创建新结点
                                pCur = InsertSiblingById(pdwIdPath[0], pParentBak);
                                if(!pCur)
                                        return ERR_FAILED;

                                pCur->m_cdata.SetAccess(dwAccess);
                                bIsCreateNew = RS_TRUE;
                        }
                        else					// 此时需要循环创建各级路径结点
                        {
                                return ERR_FAILED;
                        }
                }
                else
                {
                        if(ERR_SUCCESS != GetUserAccess(pdwUser, pParentBak->m_cdata.GetLevel(), dwUserAccess))
                                return ERR_FAILED;

                        if((dwAccess & dwUserAccess) != dwAccess)
                                return ERR_CFG_NO_RIGHT;

                        if(!(dwAccess & CONFIG_ACCESS_CREATE) || !(pParentBak->m_cdata.GetStatus() & STATE_IS_BRANCH))	//创建新结点
                                return ERR_CFG_NO_RIGHT;

                        // 创建新结点
                        pCur = InsertChildById(pdwIdPath[i], pParentBak);
                        if(!pCur)
                                return ERR_FAILED;

                        pCur->m_cdata.SetAccess(dwAccess);
                        bIsCreateNew = RS_TRUE;
                }
        }

        if(dwNodeType == STATE_IS_BRANCH)
        {
                // 打开已经存在的结点或者是新创建的结点
                pCur->m_cdata.SetStatus(STATE_OPENED | STATE_IS_BRANCH);
                pCur->m_cdata.SetAccess(dwAccess);
        }
        else
        {
                pCur->m_cdata.SetStatus(STATE_IS_LEAF);
        }

        *phItem = (HITEM)pCur;

        if(bIsCreateNew)	// 修改文件
        {
                if(ERR_SUCCESS != WriteInsertedNode(pCur, pParentBak))
                        return ERR_FAILED;
        }

        return ERR_SUCCESS;
}
*/
// 打开此结点的孩子结点
// Deleted by LauranceZhang 2004-6-24 15:34:29 <
/*
RS_DWORD CTree::OpenSubItem(RS_DWORD dwId, PHITEM phItem, RS_DWORD dwAccess, const RS_DWORD* pdwUser)
{
        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * RS_MAX_CFGPATH_LEN);
        m_nPosition ++;
        adwPath[m_nPosition] = dwId;
        return OpenItem(adwPath, phItem, dwAccess, pdwUser);
}
*/
// Deleted by LauranceZhang 2004-6-24 15:34:29 >

// Deleted by LauranceZhang 2004-6-24 15:29:45 <
/*
RS_DWORD CTree::GetItemPath(HITEM hItem, RS_DWORD* pdwPath, RS_DWORD& dwPathLen)
{
        if(hItem == NULL)
                return ERR_CFG_PARAM_INVALID;

        if(m_nPosition <0 )
                return ERR_CFG_PARAM_INVALID;

        if(dwPathLen < (sizeof(RS_DWORD) * (m_nPosition + 1)))
        {
                dwPathLen = sizeof(RS_DWORD) * (m_nPosition + 1);
                return ERR_CFG_BUFFER_OVERFLOW;
        }

        memcpy(pdwPath, m_adwPath, sizeof(RS_DWORD) * (m_nPosition + 1));
        return ERR_SUCCESS;
}
*/
// Deleted by LauranceZhang 2004-6-24 15:29:45 >

// Deleted by LauranceZhang 2004-6-24 15:34:37 <
/*
RS_DWORD CTree::CloseSubItem(PHITEM phItem)
{
        if(phItem == NULL)
                return ERR_SUCCESS;

        if(*phItem == NULL)
                return ERR_CFG_PARAM_INVALID;

        CNode* pCur = (CNode*)(*phItem);
        pCur->m_cdata.SetStatus(pCur->m_cdata.GetStatus() & ~STATE_OPENED);
        *phItem = 0;

        return ERR_SUCCESS;
}
*/
// Deleted by LauranceZhang 2004-6-24 15:34:37 >

RS_DWORD CTree::GetAccess(HITEM hItem, RS_DWORD& dwAccess)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        dwAccess = ((CNode*)hItem)->GetAccess();
        return ERR_SUCCESS;
}

RS_DWORD CTree::SetAccess(HITEM hItem, RS_DWORD dwAccess)
{
        if(hItem == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        return ((CNode*)hItem)->SetAccess(dwAccess);
}

RS_DWORD CTree::Import(HITEM hItemParent, HITEM hItemChild)
{
        MyTrace("Import : Enter :-(");
        if(hItemChild == RS_NULL)
        {
                MyTrace("Import : ERR_CFG_PARAM_INVALID");
                return ERR_CFG_PARAM_INVALID;
        }
        CNode* pParent = (CNode*)hItemParent;
        CNode* pChild = (CNode*)hItemChild;;

        RS_BOOL bIsToRoot = RS_FALSE;
        if(!pParent)
        {
                bIsToRoot = RS_TRUE;
                pParent = m_pRoot;
        }

        while(pChild)
        {
                CNode* pFind = RS_NULL;
                if(!bIsToRoot)
                        pFind = SearchChildById(pChild->GetID(), pParent);
                else
                        pFind = SearchSiblingById(pChild->GetID(), pParent);

                if(pFind)	//在原链表中找到了这个结点
                {
                        CNode *pChildChild = pChild->GetChild();
                        //复制一个结点
                        if(!CloneNode(pChild, pFind))
                        {
                                InnerTraceEx("Import : ERR_CFG_NODE_IMPORTFAILED 1");
                                MyTrace("Import : ERR_CFG_NODE_IMPORTFAILED 1");
                                return ERR_CFG_NODE_IMPORTFAILED;
                        }
                        //递归处理有孩子的情况
                        if(pChildChild)
                                if(ERR_SUCCESS != Import((HITEM)pFind, (HITEM)pChildChild))
                                {
                                        InnerTraceEx("Import : ERR_CFG_NODE_IMPORTFAILED 2");
                                        MyTrace("Import : ERR_CFG_NODE_IMPORTFAILED 2");
                                        return ERR_CFG_NODE_IMPORTFAILED;
                                }
                }
                else		//没找到则Clone出一棵树
                {
                        CTree *pTreeClone = new CTree;
                        if(!pTreeClone)
                        {
                                InnerTraceEx("Import : ERR_CFG_MEM_ALLOCFAILED");
                                MyTrace("Import : ERR_CFG_MEM_ALLOCFAILED");
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }
                        memset(pTreeClone,0,sizeof(CTree));

                        pTreeClone->m_dwSortType = m_dwSortType;	//排序方式
                        CNode* pHeadClone = Clone(pChild, pTreeClone);
                        if(!pHeadClone)
                        {
                                InnerTraceEx("Import : ERR_CFG_NODE_IMPORTFAILED 3");
                                MyTrace("Import : ERR_CFG_NODE_IMPORTFAILED 3");
                                return ERR_CFG_NODE_IMPORTFAILED;
                        }

                        //将Clone出来的这颗树与原来的树连接起来
                        if(!bIsToRoot)
                        {
                                CNode* pChildFirst = pParent->GetChild();

                                if(!InsertSibling(pHeadClone, pChildFirst, pParent))
                                {
                                        InnerTraceEx("Import : ERR_CFG_NODE_IMPORTFAILED 4");
                                        MyTrace("Import : ERR_CFG_NODE_IMPORTFAILED 4");
                                        return ERR_CFG_NODE_IMPORTFAILED;
                                }
                        }
                        else	//放置到根
                        {
                                CNode* pSiblingFirst = m_pRoot;
                                if(!pSiblingFirst)	//根为空
                                {
                                        m_pRoot = pHeadClone;
                                }
                                else
                                {
                                        if(!InsertSibling(pHeadClone, pSiblingFirst))
                                        {
                                                InnerTraceEx("Import : ERR_CFG_NODE_IMPORTFAILED 5");
                                                MyTrace("Import : ERR_CFG_NODE_IMPORTFAILED 5");
                                                return ERR_CFG_NODE_IMPORTFAILED;
                                        }
                                }
                        }
                        pTreeClone->m_pRoot = RS_NULL;
                        delete pTreeClone;
                }
                pChild = pChild->GetSibling();
        }
        MyTrace("Import : Exit0 :-)");
        return ERR_SUCCESS;
}

RS_DWORD CTree::Export(HITEM hItem, RS_CHAR *pszFileName)
{
        MyTrace("Export : Enter :-(");
        RS_DWORD dwRet = ERR_SUCCESS;
        if((hItem == RS_NULL) || (pszFileName == RS_NULL))
                return ERR_CFG_PARAM_INVALID;

        // 打开数据文件
        FILE* pFile = fopen(pszFileName, "w+b");
        if(!pFile)
                return ERR_CFG_FILE_OPENFAILED;

        fseek(pFile, 0, SEEK_SET);

        //write file header using dafault value
        dwRet = WriteFileHeader(pFile);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        CNode* pCur = (CNode*)hItem;
        dwRet = SaveTree(pCur, pFile, RS_FALSE);
        if(dwRet != ERR_SUCCESS)
        {
                fclose(pFile);
                return dwRet;
        }
        dwRet = EncodeFile(pFile);
        if(ERR_SUCCESS != dwRet)
        {
                fclose(pFile);
                return dwRet;
        }

        fclose(pFile);

        if(RS_FALSE == DisposeFile(pszFileName,0,RS_TRUE,RS_FALSE))
                return ERR_CFG_FILE_DISPOSEFAILED;
        MyTrace("Export : Enter :-)");
        return dwRet;
}
RS_DWORD CTree::ExportAll(HITEM hItem, RS_CHAR *pszFileName)
{
        RS_DWORD dwRet = ERR_SUCCESS;
        if((hItem == RS_NULL) || (pszFileName == RS_NULL))
                return ERR_CFG_PARAM_INVALID;

        // 打开数据文件
        FILE* pFile = fopen(pszFileName, "w+b");
        if(!pFile)
                return ERR_CFG_FILE_OPENFAILED;

        fseek(pFile, 0, SEEK_SET);

        dwRet = WriteFileHeader(pFile);				//write file header using dafault value
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        CNode* pCur = (CNode*)hItem;
        dwRet = SaveTree(pCur, pFile, RS_TRUE);
        if(dwRet != ERR_SUCCESS)
        {
                fclose(pFile);
                return dwRet;
        }
        dwRet = EncodeFile(pFile);
        if(dwRet != ERR_SUCCESS)
        {
                fclose(pFile);
                return dwRet;
        }
        fclose(pFile);
        if(RS_FALSE == DisposeFile(pszFileName,0,RS_TRUE,RS_FALSE))
                return ERR_CFG_FILE_DISPOSEFAILED;
        return dwRet;
}

RS_DWORD CTree::CloneTree(HITEM hItem, CTree **ppTree)
{

        if(RS_NULL == Clone((CNode *)hItem, *ppTree))
                return ERR_CFG_TREE_CLONEFAILED;

        return ERR_SUCCESS;
}
//按照输入的结点克隆出一棵树,但是树结点中的有关文件偏移的信息都清0
CNode *CTree::CloneAll(CNode *pHead, CTree *pTree, CNode *pParent)
{
        if(!pTree)	return RS_NULL;
        if(!pHead)	return RS_NULL;

        CNode* pstHead = RS_NULL;
        CNode* pstTmp = RS_NULL;
        CNode* pstCur = RS_NULL;

        while(pHead)
        {
                // Clone一个结点
                CNode* pstData = CloneNode(pHead);
                if(!pstData) return RS_NULL;

                // 将结点插入到树中
                pstCur = pTree->InsertSibling(*pstData, pstHead, pParent);
                if(!pstCur) return RS_NULL;

                if(!pstHead)
                        pstHead = pstCur;
                else
                {
                        if(pParent)
                                pstHead = pParent->GetChild();
                }

                // 有孩子结点时，则递归创建子树
                CNode *pChild = pHead->GetChild();
                if(pChild)
                {
                        pstTmp = CloneAll(pChild, pTree, pstCur);
                        if(!pstTmp)	break;

                        pstCur->SetChild(pstTmp);
                }

                // 有兄弟结点时，继续读取兄弟结点
                pHead = pHead->GetSibling();
                if(!pHead) break;
        }
        if(pParent)
        {
                pstHead = pParent->GetChild();
        }
        return pstHead;
}

CNode *CTree::Clone(CNode *pHead, CTree *pTree)
{
        if(!pTree)	return RS_NULL;
        if(!pHead)	return RS_NULL;

        CNode* pstHead = RS_NULL;
        CNode* pstTmp = RS_NULL;
        CNode* pstCur = RS_NULL;

        // Clone一个结点
        CNode* pstData = CloneNode(pHead);
        if(!pstData)
        {
                MyTrace("Clone : CloneNode Failed");
                return RS_NULL;
        }
        // 将结点插入到树中
        pstCur = pTree->InsertSibling(*pstData, pstHead);
        if(!pstHead)
                pstHead = pstCur;

        // 有孩子结点时，则递归创建子树
        CNode *pChild = pHead->GetChild();
        if(pChild)
        {
                pstTmp = CloneAll(pChild, pTree, pstCur);
                if(!pstTmp)
                {
                        MyTrace("Clone : CloneAll Failed");
                        return RS_NULL;
                }
                pstCur->SetChild(pstTmp);
        }
        return pstHead;
}

CNode* CTree::CloneNode(CNode* pcDataSrc, CNode* pcDataDes)
{
        CHAR szInfo[200] = {0};
        RS_DWORD dwReaded = 0;				// 所读出的数据数
        RS_DWORD dwFileEntry = 0;			// 在文件中的入口
        RS_DWORD dwOffset = 0;				// 文件偏移
        RS_BYTE* pData = RS_NULL;			// 数据缓冲区
        RS_CHAR* pchName = RS_NULL;			// 名字缓冲区

        CNode* pstData = RS_NULL;
        if(pcDataDes)
                pstData = pcDataDes;
        else
        {
                pstData = MYNEW(CNode,1);
                if(!pstData)
                {
                        m_dwErrCode = ERR_CFG_SHM_ALLOCFAILED;
                        InnerTraceEx("CloneNode : ERR_CFG_SHM_ALLOCFAILED");
                        MyTrace("CloneNode : ERR_CFG_SHM_ALLOCFAILED");
                        return RS_NULL;
                }
                memset(pstData,0,sizeof(CNode));
        }
        // Copy结点结构
        //=================================================//
        pstData->SetID(pcDataSrc->GetID());
        pstData->SetLevel(pcDataSrc->GetLevel());
        pstData->SetStatus(pcDataSrc->GetStatus());
        pstData->SetDataType(pcDataSrc->GetDataType());
        pstData->SetAccess(pcDataSrc->GetAccess());
        // Copy数据
        //=================================================//
        RS_DWORD dwDataLen = pcDataSrc->GetDataLen();
        if(dwDataLen > 0)
        {
                pData = new RS_BYTE[dwDataLen + 1];
                if(!pData)
                {
                        m_dwErrCode = ERR_CFG_MEM_ALLOCFAILED;
                        MyTrace("CloneNode : ERR_CFG_MEM_ALLOCFAILED");
                        return RS_NULL;
                }
                memset(pData, 0, dwDataLen + 1);
                pcDataSrc->GetData(pData, dwDataLen);
                pData[dwDataLen] = 0;
        }
        if(pcDataDes)
                pstData->ResetData(pData, dwDataLen, pcDataSrc->GetDataType());
        else
                pstData->SetData(pData, dwDataLen, pcDataSrc->GetDataType());
        if(dwDataLen > 0)
                delete[] pData;


        // 读取结点名字
        //=================================================//
        RS_DWORD dwNameLen = pcDataSrc->GetNameLen();
        if(dwNameLen > 0)
        {
                pchName = new RS_CHAR[dwNameLen + 1];
                if(!pchName)
                {
                        m_dwErrCode = ERR_CFG_MEM_ALLOCFAILED;
                        MyTrace("CloneNode : ERR_CFG_MEM_ALLOCFAILED");
                        return RS_NULL;
                }
                memset(pchName, 0, dwNameLen + 1);
                pcDataSrc->GetName(pchName, dwNameLen);
                pchName[dwNameLen] = 0;
        }
        if(pcDataDes)
                pstData->ResetName(pchName);
        else
                pstData->SetName(pchName);

//	sprintf(szInfo, "CloneNode : ID=%08x", pstData->GetID());
//	MyTrace(szInfo);

        if(dwNameLen > 0)
        {
//		MyTrace(pchName);
                delete[] pchName;
                pchName = RS_NULL;
        }
        // 读取备注信息
        //=================================================//
        dwNameLen = pcDataSrc->GetMemoLen();
        if(dwNameLen > 0)
        {
                pchName = new RS_CHAR[dwNameLen + 1];
                if(!pchName)
                {
                        m_dwErrCode = ERR_CFG_MEM_ALLOCFAILED;
                        MyTrace("CloneNode : ERR_CFG_MEM_ALLOCFAILED");
                        return RS_NULL;
                }
                memset(pchName, 0, dwNameLen + 1);
                pcDataSrc->GetMemo(pchName, dwNameLen);
                pchName[dwNameLen] = 0;
        }
        if(pcDataDes)
                pstData->ResetMemo(pchName);
        else
                pstData->SetMemo(pchName);
        if(dwNameLen > 0)
        {
                delete[] pchName;
                pchName = RS_NULL;
        }
        return pstData;
}

CNode* CTree::InsertChild(CNode& item)
{
        CNode* pTemp = RS_NULL;
        CNode* pTempPre = RS_NULL;
        CNode* pNew = RS_NULL;

        if(m_pRoot == RS_NULL)
        {
                m_dwErrCode = ERR_CFG_PARAM_INVALID;
                return RS_NULL;
        }

        if(m_pRoot->GetChild() == RS_NULL)	// 没有长子的情况
        {
                pNew = new CNode();
                if(!pNew)
                {
                        m_dwErrCode = ERR_CFG_MEM_ALLOCFAILED;
                        return RS_NULL;
                }
                *pNew = item;
                m_pRoot->SetChild(pNew);
        }
        else							// 如果已经有了长子则插入次子
        {
                CNode* pTmp = InsertSibling(item, m_pRoot->GetChild());
                if(RS_NULL == pTmp)
                        return RS_NULL;
                pNew = pTmp;
        }

        return pNew;
}
CNode* CTree::InsertChild(CNode& item, CNode* pParent)
{
        CNode* pTemp = NULL;
        CNode* pTempPre = NULL;
        CNode* pNew = NULL;

        if(pParent == NULL)
        {
                m_dwErrCode = ERR_CFG_PARAM_INVALID;
                return NULL;
        }

        if(pParent->GetChild() == NULL)	// 没有长子的情况
        {
                pNew = new CNode();
                if(!pNew)
                {
                        m_dwErrCode = ERR_CFG_MEM_ALLOCFAILED;
                        return NULL;
                }
                *pNew = item;
                pParent->SetChild(pNew);
        }
        else							// 如果已经有了长子则插入次子
        {
                pNew = InsertSibling(item, pParent->GetChild(), pParent);
        }

        return pNew;
}

CNode* CTree::InsertSibling(CNode& item, CNode* pSubHead, CNode* pParent)
{
        CNode* pTemp = RS_NULL;
        CNode* pTempPre = RS_NULL;
        CNode* pNew = RS_NULL;
        CNode* pHead = pSubHead;

        pNew = MYNEW(CNode, sizeof(CNode));
        if(!pNew)
        {
                MyTrace("CTree::InsertSibling->MYNEW Failed!");
                return RS_NULL;
        }
        *pNew = item;
        if(pHead == RS_NULL)
        {
                pHead = pNew;
                if(!m_pRoot)
                        m_pRoot = pHead;
                if(pParent)
                        pParent->SetChild(pHead);
                return pNew;
        }

        pTempPre = pHead;
        pTemp = RS_NULL;

        switch(m_dwSortType)
        {
        case ORDER_NO:
                while(pTempPre != RS_NULL)							// 找到插入的位置
                {
                        pTemp = pTempPre;
                        pTempPre = pTempPre->GetSibling();
                }
                break;
        case ORDER_ASCENDING:
                while(item.Compare(pTempPre) > 0)	// 找到插入的位置
                {
                        pTemp = pTempPre;
                        pTempPre = pTempPre->GetSibling();
                }
                break;
        case ORDER_DESCENDING:
                while(item.Compare(pTempPre) < 0)		// 找到插入的位置
                {
                        pTemp = pTempPre;
                        pTempPre = pTempPre->GetSibling();
                }
                break;
        default:
                return RS_NULL;
        }
        if(pTemp == RS_NULL)
        {
                if(!pParent)
                        pParent = SearchParent(m_pRoot, pSubHead);
                if(pParent)	//有父结点
                {
                        pParent->SetChild(pNew);
                        pNew->SetSibling(pTempPre);
                }
                else	//没有前置结点，又没有父结点，则插入到最前面
                {
                        pNew->SetSibling(pTempPre->GetSibling());
                        pTempPre->SetSibling(pNew);
                }
        }
        else
        {
                pTemp->SetSibling(pNew);
                pNew->SetSibling(pTempPre);
        }
        if(!m_pRoot)
                m_pRoot = pHead;
        return pNew;
}

CNode* CTree::InsertSibling(CNode* pItem, CNode* pSubHead, CNode* pParent)
{
        CNode* pTemp = RS_NULL;
        CNode* pTempPre = RS_NULL;
        CNode* pNew = pItem;
        CNode* pHead = pSubHead;

        if(!pNew)
                return RS_NULL;
        if(!pHead)
        {
                pHead = pNew;
                if(pParent)	//有父结点
                        pParent->SetChild(pNew);
                return pNew;
        }

        pTempPre = pHead;
        pTemp = RS_NULL;

        switch(m_dwSortType)
        {
        case ORDER_NO:
                while(pTempPre != RS_NULL)							// 找到插入的位置
                {
                        pTemp = pTempPre;
                        pTempPre = pTempPre->GetSibling();
                }
                break;
        case ORDER_ASCENDING:
                while(pItem->Compare(pTempPre) > 0)	// 找到插入的位置
                {
                        pTemp = pTempPre;
                        pTempPre = pTempPre->GetSibling();
                }
                break;
        case ORDER_DESCENDING:
                while(pItem->Compare(pTempPre) < 0)		// 找到插入的位置
                {
                        pTemp = pTempPre;
                        pTempPre = pTempPre->GetSibling();
                }
                break;
        default:
                return RS_NULL;
        }

        //插入结点
        if(pTemp == RS_NULL)								// 插入到最前面
        {
                if(pParent)	//有父结点
                {
                        pParent->SetChild(pNew);
                        pNew->SetSibling(pTempPre);
                }
                else
                {
                        pNew->SetSibling(pTempPre->GetSibling());
                        pTempPre->SetSibling(pNew);
                }
        }
        else
        {
                pTemp->SetSibling(pNew);
                pNew->SetSibling(pTempPre);
        }

        return pNew;
}
// Deleted by LauranceZhang 2004-8-4 13:14:04 <
/*
void *CTree::operator new(size_t size)
{
    void *p = CSharedMem::alloc(size);  // Replace this with alternative allocator
    return (p);
}
void CTree::operator delete(void *p)
{
    CSharedMem::free(p);  // Replace this with alternative de-allocator
}
void *CTree::operator new[](size_t size)
{
    void *p = CSharedMem::alloc(size);
    return (p);
}
void CTree::operator delete[](void *p)
{
    CSharedMem::free(p);
}
*/
// Deleted by LauranceZhang 2004-8-4 13:14:04 >

RS_RESULT CTree::EncodeFile(FILE *fp)
{
        if(fp==RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        STCONFIGHEADER stHeader = {0};

        fseek(fp, 0, SEEK_END);
        RS_DWORD dwFileLen = ftell(fp);

        fseek(fp, 0, SEEK_SET);
        if(1 != fread(&stHeader, sizeof(STCONFIGHEADER), 1, fp))
                return ERR_CFG_FILE_READFAILED;
        SwapHeader(&stHeader);
        stHeader.dwFileLen = dwFileLen;

        //写系统时间
#ifdef UNIXPLATFORM
        time_t lcTime;
        struct tm *stTime={0};
        char aszTime[20] = {0};
        lcTime=time(0);
        stTime=localtime(&lcTime);

        if(stHeader.szCreateTime[0]==1)
                sprintf(stHeader.szCreateTime, "%04d-%02d-%02d %02d:%02d:%02d", stTime->tm_year+1900,
                stTime->tm_mon+1, stTime->tm_mday, stTime->tm_hour, stTime->tm_min, stTime->tm_sec);

        if(stHeader.szReleaseTime[0]==1)
                sprintf(stHeader.szReleaseTime, "%04d-%02d-%02d %02d:%02d:%02d", stTime->tm_year+1900,
                        stTime->tm_mon+1, stTime->tm_mday, stTime->tm_hour, stTime->tm_min,stTime->tm_sec);
#else

        SYSTEMTIME tm;
        GetSystemTime(&tm);
        if(stHeader.szCreateTime[0]==1)
                sprintf(stHeader.szCreateTime, "%d-%02d-%02d %02d:%02d:%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
        if(stHeader.szReleaseTime[0]==1)
                sprintf(stHeader.szReleaseTime, "%d-%02d-%02d %02d:%02d:%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
#endif

        //进行加密
        if(stHeader.dwEncryptMethod == 1)
        {
                if(stHeader.dwEncryptKeyLen > 32)
                        stHeader.dwEncryptKeyLen = 32;

                //生成缺省的密钥
                if(stHeader.dwEncryptKeyLen==0)
                {
                        time_t t;
                        time(&t);
                        *(RS_DWORD *)stHeader.byEncryptKey = t;
                        *(RS_DWORD *)(&stHeader.byEncryptKey[4]) = dwFileLen;
                        stHeader.dwEncryptKeyLen = 8;
                        //写文件头
                        fseek(fp, 0, SEEK_SET);
                        SwapHeader(&stHeader);
                        if(1 != fwrite(&stHeader, sizeof(stHeader), 1, fp))
                                return ERR_CFG_FILE_WRITEFAILED;
                        SwapHeader(&stHeader);
                }

                RS_BYTE abyData[32] = {0};
                RS_INT nNumber = (dwFileLen-sizeof(STCONFIGHEADER)) / stHeader.dwEncryptKeyLen;
                RS_INT nLeft = (dwFileLen-sizeof(STCONFIGHEADER)) % stHeader.dwEncryptKeyLen;

                //循环加密
                RS_DWORD dwOffSetOld = 0, dwKeyLen = stHeader.dwEncryptKeyLen;
                for(RS_INT i=0; i < nNumber; i++)
                {
                        dwOffSetOld = ftell(fp);
                        if(1 != fread(abyData, dwKeyLen, 1, fp))
                                return ERR_CFG_FILE_READFAILED;
                        for(RS_DWORD j=0; j < dwKeyLen; j++)
                                abyData[j] ^=  stHeader.byEncryptKey[j];

                        fseek(fp, dwOffSetOld, SEEK_SET);
                        if(1 != fwrite(abyData, dwKeyLen, 1, fp))
                                return ERR_CFG_FILE_WRITEFAILED;
                        fseek(fp, dwOffSetOld+dwKeyLen, SEEK_SET);
                }
                if(nLeft)
                {
                        dwOffSetOld = ftell(fp);
                        if(1 != fread(abyData, nLeft, 1, fp))
                                return ERR_CFG_FILE_READFAILED;

                        for(RS_INT j=0; j < nLeft; j++)
                                abyData[j] ^=  stHeader.byEncryptKey[j];
                        fseek(fp, dwOffSetOld, SEEK_SET);
                        if(1 != fwrite(abyData, nLeft, 1, fp))
                                return ERR_CFG_FILE_WRITEFAILED;
                }
        }
        //写文件头
        SwapHeader(&stHeader);
        fseek(fp, 0, SEEK_SET);
        if(1 != fwrite(&stHeader, sizeof(stHeader), 1, fp))
                return ERR_CFG_FILE_WRITEFAILED;
        SwapHeader(&stHeader);

        //计算校验值
        if(stHeader.dwCheckSum1)
        {
                stHeader.dwCheckSum1 = 0;
                fseek(fp, sizeof(STCONFIGHEADER), SEEK_SET);

                RS_DWORD dwData = 0;
                RS_INT nNumber = (dwFileLen-sizeof(STCONFIGHEADER)) / sizeof(RS_DWORD);
                RS_INT nLeft = (dwFileLen-sizeof(STCONFIGHEADER)) % sizeof(RS_DWORD);

                RS_DWORD dwKeyLen = sizeof(RS_DWORD);
                for(RS_INT i=0; i < nNumber; i++)
                {
                        dwData = 0;

                        if(1 != fread(&dwData, dwKeyLen, 1, fp))
                                return ERR_CFG_FILE_READFAILED;

                        stHeader.dwCheckSum1 += RS_SWAPDWORD(dwData);
                }
                if(nLeft)
                {
                        dwData = 0;
                        if(1 != fread(&dwData, nLeft, 1, fp))
                                return ERR_CFG_FILE_READFAILED;
                        stHeader.dwCheckSum1 += RS_SWAPDWORD(dwData);
                }
        }

        //写文件头
        SwapHeader(&stHeader);
        fseek(fp, 0, SEEK_SET);
        if(1 != fwrite(&stHeader, sizeof(stHeader), 1, fp))
                return ERR_CFG_FILE_WRITEFAILED;
        SwapHeader(&stHeader);

        if(stHeader.dwCheckSum2)
        {
                stHeader.dwCheckSum2 = 0;
                fseek(fp, 0, SEEK_SET);

                RS_DWORD dwData = 0;
                RS_INT nNumber = sizeof(STCONFIGHEADER) / sizeof(RS_DWORD);
                RS_INT nLeft = sizeof(STCONFIGHEADER) % sizeof(RS_DWORD);

                SwapHeader(&stHeader);
                RS_DWORD dwKeyLen = sizeof(RS_DWORD);
                for(RS_INT i=0; i < nNumber; i++)
                {
                        dwData = 0;

                        if(1 != fread(&dwData, dwKeyLen, 1, fp))
                                return ERR_CFG_FILE_READFAILED;

                        if(0xb != i)	//跳过stHeader.dwCheckSum2本身
                                stHeader.dwCheckSum2 ^= dwData;
                }
                if(nLeft)
                {
                        dwData = 0;
                        if(1 != fread(&dwData, nLeft, 1, fp))
                                return ERR_CFG_FILE_READFAILED;
                        stHeader.dwCheckSum2 ^= dwData;
                }
                SwapHeader(&stHeader);
        }
        //写文件头
        SwapHeader(&stHeader);
        fseek(fp, 0, SEEK_SET);
        if(1 != fwrite(&stHeader, sizeof(stHeader), 1, fp))
                return ERR_CFG_FILE_WRITEFAILED;
        SwapHeader(&stHeader);

        return ERR_SUCCESS;
}
RS_VOID CTree::SwapHeader(STCONFIGHEADER *pstHeader)
{
        if(pstHeader)
        {
                RS_SWAPDWORDEX(pstHeader->dwCheckSum1);
                RS_SWAPDWORDEX(pstHeader->dwCheckSum2);
                RS_SWAPDWORDEX(pstHeader->dwCompressKeyLen);
                RS_SWAPDWORDEX(pstHeader->dwCompressMethod);
                RS_SWAPDWORDEX(pstHeader->dwDataOffset);
                RS_SWAPDWORDEX(pstHeader->dwEncryptKeyLen);
                RS_SWAPDWORDEX(pstHeader->dwEncryptMethod);
                RS_SWAPDWORDEX(pstHeader->dwFileFlag);
                RS_SWAPDWORDEX(pstHeader->dwFileLen);
                RS_SWAPDWORDEX(pstHeader->dwHeaderLen);
                RS_SWAPDWORDEX(pstHeader->dwPrivate );
                RS_SWAPDWORDEX(pstHeader->dwProductID);
                RS_SWAPDWORDEX(pstHeader->dwReserved1);
                RS_SWAPDWORDEX(pstHeader->dwReserved2);
                RS_SWAPDWORDEX(pstHeader->dwReserved3);
                RS_SWAPDWORDEX(pstHeader->dwReserved4);
                RS_SWAPDWORDEX(pstHeader->dwSHMLength);
                RS_SWAPDWORDEX(pstHeader->dwVersion);
        }
}

RS_RESULT CTree::DecodeDataOLD(RS_BYTE *pData)
{
        STCONFIGHEADEROLD *pstHeader = (STCONFIGHEADEROLD *)pData;
        RS_BYTE *pRealData = pData;

        //计算校验值
        if(pstHeader->dwCheckSum2)
        {
                RS_DWORD dwData = 0;
                RS_INT nNumber = pstHeader->dwHeaderLen / sizeof(RS_DWORD);
                RS_INT nLeft = pstHeader->dwHeaderLen % sizeof(RS_DWORD);

                RS_DWORD dwKeyLen = sizeof(RS_DWORD);
                for(RS_INT i=0; i < nNumber; i++)
                {
                        if(i != 7)	//跳过本身
                                dwData ^= *((RS_DWORD *)pRealData);
                        pRealData += sizeof(RS_DWORD);
                }
                if(nLeft)
                {
                        RS_DWORD dwLeft = 0;
                        memcpy(&dwLeft, pRealData, nLeft);
                        dwData += dwLeft;
                }

                if(dwData != pstHeader->dwCheckSum2)
                        return ERR_CFG_FILE_CHECKSUMFAILED;
        }

        //计算校验值
        pRealData = pData+sizeof(STCONFIGHEADEROLD);
        if(pstHeader->dwCheckSum1)
        {
                RS_DWORD dwData = 0;
                RS_INT nNumber = (pstHeader->dwFileLen-sizeof(STCONFIGHEADEROLD)) / sizeof(RS_DWORD);
                RS_INT nLeft = (pstHeader->dwFileLen-sizeof(STCONFIGHEADEROLD)) % sizeof(RS_DWORD);

                RS_DWORD dwKeyLen = sizeof(RS_DWORD);
                for(RS_INT i=0; i < nNumber; i++)
                {
                        dwData += *((RS_DWORD *)pRealData);
                        pRealData += sizeof(RS_DWORD);
                }
                if(nLeft)
                {
                        RS_DWORD dwLeft = 0;
                        memcpy(&dwLeft, pRealData, nLeft);
                        dwData += dwLeft;
                }

                if(dwData != pstHeader->dwCheckSum1)
                        return ERR_CFG_FILE_CHECKSUMFAILED;
        }

        //进行解密
        pRealData = pData+sizeof(STCONFIGHEADEROLD);
        if(pstHeader->dwEncryptMethod == 1)
        {
                if(pstHeader->dwEncryptKeyLen > 32)
                        pstHeader->dwEncryptKeyLen = 32;

                RS_BYTE abyData[32] = {0};
                RS_INT nNumber = (pstHeader->dwFileLen - sizeof(STCONFIGHEADEROLD)) / pstHeader->dwEncryptKeyLen;
                RS_INT nLeft = (pstHeader->dwFileLen - sizeof(STCONFIGHEADEROLD)) % pstHeader->dwEncryptKeyLen;

                //循环加密
                RS_DWORD dwKeyLen = pstHeader->dwEncryptKeyLen;
                for(RS_INT i=0; i < nNumber; i++)
                {
                        for(RS_DWORD j=0; j < dwKeyLen; j++)
                                pRealData[j] ^=  pstHeader->byEncryptKey[j];

                        pRealData += dwKeyLen;
                }
                if(nLeft)
                {
                        for(RS_INT j=0; j < nLeft; j++)
                                pRealData[j] ^=  pstHeader->byEncryptKey[j];
                }
        }
        return ERR_SUCCESS;
}

RS_RESULT CTree::DecodeData(RS_BYTE *pData)
{
#ifdef RS_BIG_ENDIAN
        STCONFIGHEADER stHeader = *((STCONFIGHEADER *)pData);
        SwapHeader(&stHeader);
        STCONFIGHEADER *pstHeader = &stHeader;
#else
        STCONFIGHEADER *pstHeader = (STCONFIGHEADER *)pData;
#endif
        RS_BYTE *pRealData = pData;

        //计算校验值
        if(pstHeader->dwCheckSum2)
        {
                RS_DWORD dwData = 0;
                RS_INT nNumber = pstHeader->dwHeaderLen / sizeof(RS_DWORD);
                RS_INT nLeft = pstHeader->dwHeaderLen % sizeof(RS_DWORD);

                RS_DWORD dwKeyLen = sizeof(RS_DWORD);
                for(RS_INT i=0; i < nNumber; i++)
                {
                        if(i != 0xb)	//跳过本身
                                dwData ^= RS_SWAPDWORD(*((RS_DWORD *)pRealData));
                        pRealData += sizeof(RS_DWORD);
                }
                if(nLeft)
                {
                        RS_DWORD dwLeft = 0;
                        memcpy(&dwLeft, pRealData, nLeft);
                        dwData += RS_SWAPDWORDEX(dwLeft);
                }

                if(dwData != pstHeader->dwCheckSum2)
                        return ERR_CFG_FILE_CHECKSUMFAILED;
        }

        //计算校验值
        pRealData = pData+sizeof(STCONFIGHEADER);
        if(pstHeader->dwCheckSum1)
        {
                RS_DWORD dwData = 0;
                RS_INT nNumber = (pstHeader->dwFileLen-sizeof(STCONFIGHEADER)) / sizeof(RS_DWORD);
                RS_INT nLeft = (pstHeader->dwFileLen-sizeof(STCONFIGHEADER)) % sizeof(RS_DWORD);

                RS_DWORD dwKeyLen = sizeof(RS_DWORD);
                for(RS_INT i=0; i < nNumber; i++)
                {
                        dwData += RS_SWAPDWORD(*((RS_DWORD *)pRealData));
                        pRealData += sizeof(RS_DWORD);
                }
                if(nLeft)
                {
                        RS_DWORD dwLeft = 0;
                        memcpy(&dwLeft, pRealData, nLeft);
                        dwData += dwLeft;
                }

                if(dwData != pstHeader->dwCheckSum1)
                        return ERR_CFG_FILE_CHECKSUMFAILED;
        }

        //进行解密
        pRealData = pData+sizeof(STCONFIGHEADER);
        if(pstHeader->dwEncryptMethod == 1)
        {
                if(pstHeader->dwEncryptKeyLen > 32)
                        pstHeader->dwEncryptKeyLen = 32;

                RS_BYTE abyData[32] = {0};
                RS_INT nNumber = (pstHeader->dwFileLen - sizeof(STCONFIGHEADER)) / pstHeader->dwEncryptKeyLen;
                RS_INT nLeft = (pstHeader->dwFileLen - sizeof(STCONFIGHEADER)) % pstHeader->dwEncryptKeyLen;

                //循环加密
                RS_DWORD dwKeyLen = pstHeader->dwEncryptKeyLen;
                for(RS_INT i=0; i < nNumber; i++)
                {
                        for(RS_DWORD j=0; j < dwKeyLen; j++)
                                pRealData[j] ^=  pstHeader->byEncryptKey[j];

                        pRealData += dwKeyLen;
                }
                if(nLeft)
                {
                        for(RS_INT j=0; j < nLeft; j++)
                                pRealData[j] ^=  pstHeader->byEncryptKey[j];
                }
        }

#ifdef RS_BIG_ENDIAN
        SwapHeader((STCONFIGHEADER *)pData);
#endif
        return ERR_SUCCESS;
}

CNode* CTree::SearchParent(CNode* pRoot, CNode* pNode)
{
        CNode* pCur = pRoot;
        CNode* pChild = RS_NULL;
        CNode* pFind = RS_NULL;

        if(pCur == RS_NULL)
                return RS_NULL;

        while(pCur)
        {
                if((pCur->GetStatus() & CONFIG_STATE_DELETED) != CONFIG_STATE_DELETED)
                {
                        pChild = pCur->GetChild();
                        if(pChild == pNode)
                        {
                                pFind = pCur;
                                break;
                        }
                }

                if(pChild)
                {
                        pFind = SearchParent(pChild, pNode);
                        if(pFind)
                                break;
                }
                pCur = pCur->GetSibling();
        }

        return pFind;
}

RS_RESULT CTree::GetUserAccess(const RS_DWORD* pdwUser, RS_DWORD dwItemLevel, RS_DWORD& dwAccess)
{
        if(pdwUser == RS_NULL)
                return ERR_CFG_PARAM_INVALID;

        RS_DWORD dwRet = ERR_SUCCESS;

        // 先查询出此项的句柄
        HITEM hItem = 0;
        RS_DWORD adwTmpPath[10] = {0xFFFFFFFF, 0xFFFFFFFF, 1, 1, 0};
        RS_DWORD dwOpAccess = CONFIG_ACCESS_QUERY;
        RS_DWORD adwUser[RS_USERID_LEN] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
        dwRet = OpenItem(adwTmpPath, &hItem, dwOpAccess, adwUser);
        if(!((ERR_SUCCESS == dwRet) && hItem))	//此项不存在
        {
                //如果是超级用户
                RS_DWORD adwSuperUser[RS_USERID_LEN+ RS_USERID_LEN] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
                if(0==memcmp(pdwUser, adwSuperUser, sizeof(adwSuperUser)))
                {
//			MyTrace("GetUserAccess : Using Super Access.");
                        dwAccess = CONFIG_ACCESS_ALL | CONFIG_ACCESS_SUPER;
                        return ERR_SUCCESS;
                }
                return dwRet; //没有这个结点
        }
        CNode *pNode = (CNode *)hItem;
        RS_DWORD dwSize = RS_USERID_LEN*sizeof(RS_DWORD);
        RS_BYTE abyData[RS_USERID_LEN*sizeof(RS_DWORD)+1] = {0};
        while(pNode)
        {
                dwRet = pNode->GetData(abyData, dwSize);
                if(dwRet!=ERR_SUCCESS)
                {
                        pNode = RS_NULL;
                        break;
                }
                //找到了此用户
#ifndef UNIXPLATFORM
                if(	(dwSize == RS_USERID_LEN*sizeof(RS_DWORD)) &&
                        (memcmp(pdwUser, abyData, dwSize)==0) )
                        break;
#else
                if(	(dwSize == RS_USERID_LEN*sizeof(RS_DWORD)) )
                {
                        RS_DWORD *pdwTmp=(RS_DWORD *)abyData;

                        for(int i=0;i<RS_USERID_LEN;i++)
                        {
                                RS_SWAPDWORDEX(pdwTmp[i]);
                        }
                        if(memcmp(pdwUser, abyData, dwSize)==0)
                                break;
                }
#endif
                pNode = pNode->GetSibling();
        }

        if(!pNode)
                return ERR_CFG_USER_NOTFOUND;

        CNode *pItem = RS_NULL;
        pItem = pNode->GetChild();
        if(!pItem)
                return dwRet;

        RS_DWORD dwValue = 0, dwAccessRead = 0;
        while(pItem)
        {
                dwSize = sizeof(RS_DWORD);
                dwRet = pItem->GetData((RS_BYTE *)&dwValue, dwSize);
                if(dwRet!=ERR_SUCCESS)
                        break;
                RS_SWAPDWORDEX(dwValue);
                if(dwValue <= dwItemLevel)
                        dwAccessRead |= pItem->GetID();
                pItem = pItem->GetSibling();
        }

        dwAccess = dwAccessRead;
        return ERR_SUCCESS;
}
RS_RESULT CTree::SetControlNode(CNode *pParent, CNode *pBase, CNode *pNode)
{
        //先在兄弟中找
        CNode *pHead = pBase;
        while(pHead)
        {
                if(pHead->GetSibling()==pNode)
                {
                        m_pControl = pHead;
                        return ERR_SUCCESS;
                }
                pHead = pHead->GetSibling();
        }

        if(!pParent)
        {
                m_pControl = RS_NULL;
                return ERR_SUCCESS;
        }
        if((pParent->GetChild() == pNode) || (pParent->GetSibling() == pNode))
        {
                m_pControl = pParent;
                return ERR_SUCCESS;
        }

        //在孩子中找
        pHead = pParent->GetChild();
        while(pHead)
        {
                if(pHead->GetSibling()==pNode)
                {
                        m_pControl = pHead;
                        return ERR_SUCCESS;
                }
                pHead = pHead->GetSibling();
        }
        return ERR_FAILED;
}


RS_BOOL CTree::DisposeFile(RS_CHAR *pszSrcFileName, RS_CHAR *pszDestFileName, RS_BOOL bIsCompress, RS_BOOL bDelSrcFile)
{
        RS_BOOL bRet = RS_FALSE;
        RS_DWORD dwInput = 0, dwOutput = 0;
        RS_BYTE *pbyInput = 0, *pbyOutput = 0;
        FILE *fp = 0;
        RS_CHAR szDestFile[2048] = {0};

        if(!pszSrcFileName)
                goto DISPOSE_ERROR;
        if(!pszDestFileName)
        {
                if(strlen(pszSrcFileName) + 4 > 2048)
                        goto DISPOSE_ERROR;
                strcpy(szDestFile, pszSrcFileName);
                RS_CHAR *pPos = strrchr(szDestFile, '.');
                if(bIsCompress)
                {
                    // TODO
                        if(!pPos || strcmp(pPos,".cps")!=0)
                                strcat(szDestFile,".cps");
                }
                else
                {
                        if(pPos && strcmp(pPos,".cps")==0)
                                pPos = 0;
                }
        }
        else
        {
                strcpy(szDestFile, pszDestFileName);
        }
        fp = fopen(pszSrcFileName, "rb");
        if(!fp)
                goto DISPOSE_ERROR;
        fseek(fp, 0, SEEK_END);

        dwInput = ftell(fp);
        pbyInput = new RS_BYTE[dwInput + 1];
        if(!pbyInput)
                goto DISPOSE_ERROR;
        memset(pbyInput, 0, dwInput + 1);
        fseek(fp, 0, SEEK_SET);

        if(bIsCompress)
        {
                dwOutput = dwInput*2;
        }
        else
        {
                //判断文件标志
                CHAR szFlag[16] = {0};
                if(1 != fread(szFlag, 12, 1, fp))
                        goto DISPOSE_ERROR;
                if(0 != strcmp("RCFGCOMPRESS",szFlag))
                {
                        bRet = RS_TRUE;
                        goto DISPOSE_ERROR;
                }

                //获得OutBuffer的长度
                if(1 != fread(&dwOutput, 4, 1, fp))
                        goto DISPOSE_ERROR;
                RS_SWAPDWORDEX(dwOutput);
                dwInput -= 16;
        }

        pbyOutput = new RS_BYTE[dwOutput + 12];
        if(!pbyOutput)
                goto DISPOSE_ERROR;

        //读取源数据
        if(1 != fread(pbyInput, dwInput, 1, fp))
                goto DISPOSE_ERROR;
        fclose(fp);
        fp = 0;

        if(!LoadZipDll())
        {
                bRet = RS_TRUE;
                goto DISPOSE_ERROR;
        }
        if(Z_OK != (bIsCompress?g_pCompress:g_pUncompress)(pbyOutput, &dwOutput, pbyInput, dwInput))
                goto DISPOSE_ERROR;
        fp = fopen(szDestFile, "wb");
        if(!fp)
                goto DISPOSE_ERROR;

        //写压缩后的文件标志
        if(bIsCompress)
        {
                CHAR szFlag[16] = {"RCFGCOMPRESS"};
                if(1 != fwrite(szFlag, 12, 1, fp))
                        goto DISPOSE_ERROR;
                RS_SWAPDWORDEX(dwInput);
                if(1 != fwrite(&dwInput, 4, 1, fp))
                        goto DISPOSE_ERROR;
        }
        //写压缩或解压后的数据
        if(1 != fwrite(pbyOutput, dwOutput, 1, fp))
                goto DISPOSE_ERROR;
        fclose(fp);
        fp = 0;

        //删除源文件
        if(bDelSrcFile)
        {
                if(0 != remove(pszSrcFileName))
                        goto DISPOSE_ERROR;
        }

        bRet = RS_TRUE;

DISPOSE_ERROR:
        UnloadZipDll();
#ifdef UNIXPLATFORM
        if(fp)
                fclose(fp);
        if(pbyInput)
                delete []pbyInput;
        if (pbyOutput)
                delete []pbyOutput;
#else
        fp?fclose(fp):0;
        pbyInput?delete []pbyInput:0;
        pbyOutput?delete []pbyOutput:0;
#endif
        return bRet;
}
