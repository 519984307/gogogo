///////////////////////////////////////////////////////////////////////////
//  项目名	：瑞星2005版配置文件树
//	文件名	：unixrscfg.cpp
//	用  途	：实现 CUnixRsCfg
//	作  者	：许自龙
//	完成日期  : 2004-11-15
//
//	历史和原因说明：
//		由于瑞星unix2004eng 和2005的配置文件中有关的数据类型定义存在
//		存在冲突，只好将2005配置文件树的所有资源包括头文件严格封装，所有的
//		对2005配置树的访问只能通过CUnixRsCfg 类完成.
/////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#include <unixrscfg.h>
#include "RSCFGDEF.H"

IConfig* NewConfigObject();
void ReleaseConfigObject(IConfig** pConfig);
extern CCfgMutex g_cMutexMem;

CUnixRsCfg::CUnixRsCfg(const char *szFilename)
{
        memset((void *)m_szRsConfig,0,256);
        strncpy(m_szRsConfig,szFilename,255);
        pstOsType=NULL;
        m_pConfig=NULL;

/*
        //if(g_cMutexMem)
        {
                g_cMutexMem.Destroy();
        }*/
}

CUnixRsCfg::~CUnixRsCfg()
{
        //释放共享内存
        if(pstOsType)
                delete [] pstOsType ;

//
// 释放配置对象
        if(m_pConfig)
        {
                ReleaseConfigObject(&m_pConfig);
                m_pConfig=NULL;
        }

// 下面的一段我
        CSharedMem::Finalize();

/*	//if(g_cMutexMem)
        {
                g_cMutexMem.Destroy();
        }
        */
}

/////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE CUnixRsCfg::Init()
//  用途： 分配一个m_pConfig,加载配置树文件，所有的操作配置树的成员
//			函数中都调用了Init,不必显示调用该成员函数，该成员函数在一个类的
//			实例中只初始化一次，判断m_pConfig决定是否需要初始化
//  对全局变量的影响：
//  参数说明 ：
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////
UNIX_RS_ERROR_TYPE CUnixRsCfg::Init()
{
        UNIX_RS_ERROR_TYPE nRet;

        if(m_pConfig)
                return UNIX_RS_ERROR_OK; // 已经初始化了

        m_pConfig = NewConfigObject();

        if(!m_pConfig)
        {
                return UNIX_RS_ERROR_NEWOBJ;
        }

        // 加载配置文件
        nRet=m_pConfig->LoadConfig(m_szRsConfig,NULL);

        if(nRet!=0)
        {
                ReleaseConfigObject(&m_pConfig);
                m_pConfig=NULL;
                return UNIX_RS_ERROR_LOADCONF;
        }

        return UNIX_RS_ERROR_OK;

}

/////////////////////////////////////////////////////////////////
//  函数名：  int CUnixRsCfg::ReadRavService(UNIXRSRAVPARAM &uParam)
//  用途： 读取配置树中有关ravservice节点和子节点的参数接口
//			操作的节点是 0x6000->0x02
//  对全局变量的影响：
//  参数说明 ： uParam ：返回给用户的参数结构
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::ReadRavService(UNIXRSRAVPARAM &uParam)
{
//	读取的节点路径
        RS_DWORD adwNodePath[]= {0x6000,0x02, 0,0,0,0};
        RS_DWORD dwAccess ;
        UNIX_RS_ERROR_TYPE nRet;

        long m_lUserID=0x6000;
        RS_DWORD adwUser[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,m_lUserID, 0};

        RS_DWORD dwSuiteId = 1000;
        long unsigned int myId;
        RS_DWORD nLen=1024;
        int nI;

        char szBuf[1024];
        IConfig *pMidConfig=NULL;
        IConfig *pLastConfig=NULL;

        if(!m_pConfig) // 没有调用Init()
        {
                nRet=Init();
                if(nRet!=UNIX_RS_ERROR_OK)
                        return nRet;
        }


        dwAccess=CONFIG_ACCESS_READ;

        // 打开节点
        nRet=m_pConfig->CreateNode((const RS_DWORD *)adwNodePath,dwAccess,adwUser,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
                return nRet;


//
// 获取发现病毒的处理方式
//
        dwSuiteId=99999999;
        myId=RS_RAVMAIN_CFG_METHOD_FINDVIURS;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillMode=dwSuiteId;


        // 杀毒失败的处理方式
        dwSuiteId=99999999;
        myId=RS_RAVMAIN_CFG_METHOD_KILLFAILED;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKFailedMode=dwSuiteId;

        // 杀毒完成的处理方式
        dwSuiteId=99999999;
        myId=RS_RAVMAIN_CFG_METHOD_SCANFINISH;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nFinishMode=dwSuiteId;


        // 强制的杀毒类型
        dwSuiteId=99999999;
        myId=RS_RAVMAIN_CFG_SCANTYPE;
        nLen=1024;
        nRet=m_pConfig->GetItemValue(myId, (RS_BYTE*) szBuf,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        memset(uParam.szForceType,0,256);
    if (nLen > 256)
            memcpy(uParam.szForceType,szBuf,256);
    else
            memcpy(uParam.szForceType,szBuf,nLen);

        //
        // 获取中间层
        //
        myId=0x4018;
        nRet=m_pConfig->GetChildNode(myId, dwAccess,adwUser, &pMidConfig);
        if(!pMidConfig)goto Error;

        // 获取中间层文件过虑类型
        dwSuiteId=99999999;
        myId=RS_SCANNER_CFG_SCAN_FILETYPE;
        nRet=pMidConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nFilterType=dwSuiteId;

        // 获取中间层所有文件过虑字符串
        nLen=1024;
        myId=RS_SCANNER_CFG_ALL_FILTER;
        nRet=pMidConfig->GetItemValueString(myId, szBuf,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        memset(uParam.szAllFilter,0,256);
    if (nLen > 256)
            memcpy(uParam.szAllFilter,szBuf,256);
    else
            memcpy(uParam.szAllFilter,szBuf,nLen);


        // 获取中间层可执行文件过滤串
        myId=RS_SCANNER_CFG_EXECUTE_FILTER;
        nLen=1024;
        nRet=pMidConfig->GetItemValueString(myId, szBuf,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        memset(uParam.szExeFilter,0,256);
    if (nLen > 256)
            memcpy(uParam.szExeFilter,szBuf,256);
    else
            memcpy(uParam.szExeFilter,szBuf,nLen);

        // 获取中间层文件后缀描述
        myId=RS_SCANNER_CFG_FILENAME_FILTER;
        nLen=1024;
        nRet=pMidConfig->GetItemValue(myId, (RS_BYTE *)szBuf,nLen);
        szBuf[nLen]=0;
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        memset(uParam.szExtFilter,0,256);
    if (nLen > 256)
            memcpy(uParam.szExtFilter,szBuf,256);
    else
            memcpy(uParam.szExtFilter,szBuf,nLen);



        // 获取中间层是否备份文件
        dwSuiteId=99999999;
        myId=RS_SCANNER_CFG_STORE_FILE;
        nRet=pMidConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.BackupType=dwSuiteId;

        // 获取中间层是否只扫描病毒
        dwSuiteId=99999999;
        myId=RS_SCANNER_CFG_ONLY_SCAN;
        nRet=pMidConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nScanOnly=dwSuiteId;

        //
        // 获取最后一层（0x6000->0x02->0x4018->0x1001)
        //
        myId=0x1001;
        nRet=pMidConfig->GetChildNode(myId, dwAccess,adwUser, &pLastConfig);
        if(!pLastConfig)goto Error;

        // 获取最后一层查杀文件类型
    // 是否查压缩文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_COMPRESSED;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillCompress=dwSuiteId;

    // 是否查邮箱文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_MAILBOX;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillMbox=dwSuiteId;

    // 是否查邮件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_MAIL;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillMail=dwSuiteId;

    // 是否进行虚拟脱壳
        dwSuiteId=99999999;
        myId=RS_FILETYPE_UNPACK;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillUnpack=dwSuiteId;

    // 是否查杀Dos可执行文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_DOSEXECUTE;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillDos=dwSuiteId;

    // 是否查杀Windows可执行文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_WINEXECUTE;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillWin=dwSuiteId;

    // 是否查杀脚本文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_SCRIPT;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillScript=dwSuiteId;

    // 是否查杀宏文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_MACRO;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillMacro=dwSuiteId;

    // 是否查杀普通文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_NORMAL;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillGeneral=dwSuiteId;

    // 是否查杀Elf文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_ELF;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillElf=dwSuiteId;

    // 是否查杀自解压文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_SELFEXTRACT;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillSelfExtract=dwSuiteId;

    // 是否查杀未知Dos文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_UNKNOWNDOS;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillUnknownDos=dwSuiteId;

    // 是否查杀未知Windows文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_UNKNOWNWIN;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillUnknownWin=dwSuiteId;

    // 是否查杀未知脚本文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_UNKNOWNSCRIPT;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillUnknownScript=dwSuiteId;

    // 是否查杀未知宏文件
        dwSuiteId=99999999;
        myId=RS_FILETYPE_UNKNOWNMACRO;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nKillUnknownMacro=dwSuiteId;


        // 获取最后一层是否使用白名单
        dwSuiteId=99999999;
        myId=0x0f;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nWhiteLib=dwSuiteId;

        // 获取最后一层是否使用良民库
        dwSuiteId=99999999;
        myId=0x10;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nGoodLib=dwSuiteId;

        // 获取最后一层是否异常文件入良民库
        dwSuiteId=99999999;
        myId=0x11;
        nRet=pLastConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        uParam.nExceptLib=dwSuiteId;

        m_pConfig->CloseNode();
        ReleaseConfigObject(&pLastConfig);
        ReleaseConfigObject(&pMidConfig);

        return UNIX_RS_ERROR_OK;

Error:
        m_pConfig->CloseNode();

        if(pLastConfig)ReleaseConfigObject(&pLastConfig);
        if(pMidConfig)ReleaseConfigObject(&pMidConfig);
        return UNIX_RS_ERROR_GETVALUE;


}

/////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE CUnixRsCfg::ImportRsCfg(unsigned int nCode,char *szImportFile)
//  用途： 导入一些配置节点
//			操作的节点是 0x6000->0x02
//  对全局变量的影响：
//  参数说明 ：
//		nCode :导入的节点类型
//		szImportFile :导入的文件
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::ImportRsCfg(unsigned long nCode,char *szImportFile)
{
        RS_DWORD adwNodePath[]= {0x6000,0, 0,0,0,0};
        RS_DWORD dwAccess = CONFIG_ACCESS_READWRITE;
        UNIX_RS_ERROR_TYPE nRet;

        long m_lUserID=0x6000;
        RS_DWORD adwUser[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,m_lUserID, 0};

        if(!m_pConfig) // 没有调用Init()
        {
                nRet=Init();
                if(nRet!=UNIX_RS_ERROR_OK)
                        return nRet;
        }

        dwAccess=CONFIG_ACCESS_READWRITE|CONFIG_ACCESS_IMPORT;

        switch(nCode)
        {
                // 导入0x6000->0x02
                case EXPORTRAVSERVICE:
                        adwNodePath[0]=0x6000;
                        break;
                // 导入0xffffffff->0x02
                case EXPORTCONFIG:
                        adwNodePath[0]=0xFFFFFFFF;
                        break;
                default:
                        return UNIX_RS_ERROR_EXPORT;

        }

        adwNodePath[1]=0;
        nRet=m_pConfig->CreateNode((const RS_DWORD *)adwNodePath,dwAccess,adwUser,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
                return nRet;

        if( (m_pConfig->Import(szImportFile)) !=UNIX_RS_ERROR_OK )
                return UNIX_RS_ERROR_IMPORT;
        if( (m_pConfig->Synchronize(NULL)) !=UNIX_RS_ERROR_OK )
                return UNIX_RS_ERROR_SYNC;

        m_pConfig->CloseNode();

        return UNIX_RS_ERROR_OK;
}

/////////////////////////////////////////////////////////////////
//  函数名： UNIX_RS_ERROR_TYPE CUnixRsCfg::ExportRsCfg(unsigned long  nCode,char *szExportFile)
//  用途： 导出一些节点
//
//  对全局变量的影响：
//  参数说明 ：
//		nCode : 到处的节点类型
//		szExportFile ：导出的文件名
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::ExportRsCfg(unsigned long  nCode,char *szExportFile)
{

        RS_DWORD adwNodePath[]= {0x6000,0, 0,0,0,0};
        RS_DWORD dwAccess ;
        UNIX_RS_ERROR_TYPE nRet;

        long m_lUserID=0x6000;
        RS_DWORD adwUser[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,m_lUserID, 0};

        if(!m_pConfig) // 没有调用Init()
        {
                nRet=Init();
                if(nRet!=UNIX_RS_ERROR_OK)
                        return nRet;
        }

        dwAccess = CONFIG_ACCESS_EXPORT;

// 设置节点号
        switch(nCode)
        {
                // 导出0x6000->0x02
                case EXPORTRAVSERVICE:
                        adwNodePath[0]=0x6000;
                        adwNodePath[1]=0x02;
                        break;
                // 导出0xffffffff->0x02
                case EXPORTCONFIG:
                        adwNodePath[0]=0xFFFFFFFF;
                        adwNodePath[1]=0x02;
                        break;
                default:
                        return UNIX_RS_ERROR_EXPORT;
        }


        adwNodePath[2]=0;
        nRet=m_pConfig->CreateNode((const RS_DWORD *)adwNodePath,dwAccess,adwUser,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
                return nRet;

        if( (m_pConfig->Export(szExportFile)) !=UNIX_RS_ERROR_OK )
        {
                m_pConfig->CloseNode();
                return UNIX_RS_ERROR_EXPORT;
        }

        m_pConfig->CloseNode();


        return UNIX_RS_ERROR_OK;
}

/////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE ReadSnInfo(UNIXSNPARAM &stSerino)
//  用途： 读取配置树中有关序列号节点和子节点的内容
//  对全局变量的影响：
//  参数说明 ： stSerino ：返回给用户的参数结构
//  返回结果 ：
//		0: 成功
//		-1 :
///////////////////////////////////////////////////////////////////
UNIX_RS_ERROR_TYPE CUnixRsCfg::ReadSnInfo(UNIXSNPARAM &stSerino)
{
//	读取的节点路径
        RS_DWORD adwNodePath[]= {0xFFFFFFFF,0x02, 0,0,0,0};
        RS_DWORD dwAccess ;
        UNIX_RS_ERROR_TYPE nRet;

        long m_lUserID=0x6000;
        RS_DWORD adwUser[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,m_lUserID, 0};

        RS_DWORD dwSuiteId = 1000;
        long unsigned int myId;
        RS_DWORD nLen=1024;

        char szBuf[1024];
        IConfig *pChildConfig;

        if(!m_pConfig) // 没有调用Init()
        {
                nRet=Init();
                if(nRet!=UNIX_RS_ERROR_OK)
                        return nRet;
        }



        dwAccess=CONFIG_ACCESS_READ;

        nRet=m_pConfig->CreateNode((const RS_DWORD *)adwNodePath,dwAccess,adwUser,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
                return UNIX_RS_ERROR_CREATENODE;


//
// 获取序列号
//
        myId=0x01;
        nRet=m_pConfig->GetChildNode(myId, dwAccess,adwUser, &pChildConfig);
        if(!pChildConfig)
                return UNIX_RS_ERROR_GETCHILD;

        memset(stSerino.szSn,0,50);
        nLen=1024;
        myId=0x01;
        nRet=pChildConfig->GetItemValueString(myId, szBuf,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        strncpy(stSerino.szSn,szBuf,50);

// 获取产品大类
        dwSuiteId=99999999;
        myId=0x02;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nBigProdId=dwSuiteId;

// 获取产品类型
        dwSuiteId=99999999;
        myId=0x03;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nProdId=dwSuiteId;

// 获取产品子类型
        dwSuiteId=99999999;
        myId=0x04;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nChildProdId=dwSuiteId;

// 获取过期类型
        dwSuiteId=99999999;
        myId=0x05;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nExpireType=dwSuiteId;

// 获取批号
        dwSuiteId=99999999;
        myId=0x06;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nPNo=dwSuiteId;

// 获取流水号
        dwSuiteId=99999999;
        myId=0x07;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nStreamNo=dwSuiteId;

// 获取地区号
        dwSuiteId=99999999;
        myId=0x08;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nAreaNo=dwSuiteId;

// 获取OEM号
        dwSuiteId=99999999;
        myId=0x09;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nOEMId=dwSuiteId;

// 获取硬件平台
        dwSuiteId=99999999;
        myId=0x0a;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nHardWare=dwSuiteId;

// 获取OS 类型
        dwSuiteId=99999999;
        myId=0x0b;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nOsType=dwSuiteId;

// 获取OS 类型
        dwSuiteId=99999999;
        myId=0x0c;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nOsVer=dwSuiteId;

//
// 获取产品序号
        ReleaseConfigObject(&pChildConfig);
        myId=0x03;
        nRet=m_pConfig->GetChildNode(myId, dwAccess,adwUser, &pChildConfig);
        if(!pChildConfig)
                return UNIX_RS_ERROR_GETCHILD;

        dwSuiteId=99999999;
        myId=0x01;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nPrId=dwSuiteId;

// 获取过期标志
        dwSuiteId=99999999;
        myId=CONFIG_ITEMNO_FLAG_EXPIRED;
        nRet=pChildConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nExpireFlag=dwSuiteId;

        ReleaseConfigObject(&pChildConfig);
        m_pConfig->CloseNode();
        return UNIX_RS_ERROR_OK;

SnError:

        if(pChildConfig)
                ReleaseConfigObject(&pChildConfig);
        m_pConfig->CloseNode();

        return UNIX_RS_ERROR_GETVALUE;


}

/////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE CUnixRsCfg::WriteSnInfo(char *szSerialNo)
//  用途： 设置0xFFFFFFFF->0x02->0x01->0x01的序列号，设置成功后0xFFFFFFFF->0x02->0x01的
//			其它项会随之改变。一般在安装时使用该接口
//  对全局变量的影响：
//  参数说明 ： uParam ：返回给用户的参数结构
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::WriteSnInfo(char *szSerialNo)
{

//	操作的节点路径
//	RS_DWORD adwNodePath[]= {0xFFFFFFFF,0x02, 0x01,0,0,0};
        RS_DWORD adwNodePath[]= {0xFFFFFFFF,0x02, 0,0,0,0};
        RS_DWORD dwAccess ;
        UNIX_RS_ERROR_TYPE nRet;
        IConfig *pChildConfig=NULL;

        //long m_lUserID=0x6000;
        long m_lUserID=0xFFFFFFFF;
        RS_DWORD adwUser[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,m_lUserID, 0};

        long unsigned int myId;
        RS_DWORD nLen=1024;


        if(!m_pConfig) // 没有调用Init()
        {
                nRet=Init();
                if(nRet!=UNIX_RS_ERROR_OK)
                        return nRet;
        }


// 设置读写操作权限
//
        dwAccess=CONFIG_ACCESS_READWRITE;

// 打开节点
        nRet=m_pConfig->CreateNode((const RS_DWORD *)adwNodePath,dwAccess,adwUser,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
                return UNIX_RS_ERROR_CREATENODE;


//
// 	将序列号写入0xFFFFFFFF->0x02->0x01->0x01，设置序列号时
//	需要保证0xFFFFFFFF->0x02->0x03->0x01项的产品ID为空。
//

//
// 获取0xFFFFFFFF->0x02->0x01节点
//
        myId=0x01;
        nRet=m_pConfig->GetChildNode(myId, dwAccess,adwUser, &pChildConfig);
        if(!pChildConfig)goto Error;

        nLen=strlen(szSerialNo);
        myId=0x01;
        nRet=pChildConfig->SetItemValueString(myId, szSerialNo,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        ReleaseConfigObject(&pChildConfig);

//  获取0xFFFFFFFF->0x02->0x03节点

        myId=0x03;
        nRet=m_pConfig->GetChildNode(myId, dwAccess,adwUser, &pChildConfig);
        if(!pChildConfig)goto Error;

        nLen=strlen(szSerialNo);
        myId=CONFIG_ITEMNO_CURRENT_SN;
        nRet=pChildConfig->SetItemValueString(myId, szSerialNo,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto Error;
        ReleaseConfigObject(&pChildConfig);


        if(m_pConfig->Synchronize(NULL))
        {
                m_pConfig->CloseNode();
                return UNIX_RS_ERROR_SYNC;
        }

        m_pConfig->CloseNode();

        return UNIX_RS_ERROR_OK;

Error:
        m_pConfig->CloseNode();
        if(pChildConfig)ReleaseConfigObject(&pChildConfig);
        return UNIX_RS_ERROR_SETVALUE;

}

/////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE CUnixRsCfg::CheckSerino(char *szSerialNo,UNIXSNPARAM &stSerino)
//  用途： 检测序列号，使用节点0xFFFFFFFF->0x02->0x8001,这个接口只用于序列号的检测
//  对全局变量的影响：
//  参数说明 ： szSerialNo ：要检测的序列号
//				stSerino : 返回测试的结果
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::CheckSerino(char *szSerialNo,UNIXSNPARAM &stSerino)
{


//	读取的节点路径
        RS_DWORD adwNodePath[]= {0xFFFFFFFF,0x02, 0x80000001,0,0,0};
        RS_DWORD dwAccess ;
        UNIX_RS_ERROR_TYPE nRet;

        long m_lUserID=0x6000;
        RS_DWORD adwUser[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,m_lUserID, 0};

        RS_DWORD dwSuiteId = 1000;
        long unsigned int myId;
        RS_DWORD nLen=1024;

        char szBuf[1024];


        if(!m_pConfig) // 没有调用Init()
        {
                nRet=Init();
                if(nRet!=UNIX_RS_ERROR_OK)
                        return nRet;
        }
        dwAccess=CONFIG_ACCESS_READWRITE;

        nRet=m_pConfig->CreateNode((const RS_DWORD *)adwNodePath,dwAccess,adwUser,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
                return UNIX_RS_ERROR_CREATENODE;

//
//  首先设置序列号
//
//
// 	将序列号写入0xFFFFFFFF->0x02->0x80000001->0x01，设置序列号时
//	需要保证0xFFFFFFFF->0x02->0x03->0x01项的产品ID为空。
//
        nLen=strlen(szSerialNo);
        myId=0x01;
        nRet=m_pConfig->SetItemValueString(myId, szSerialNo,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
        {
                m_pConfig->CloseNode();
                return UNIX_RS_ERROR_SETVALUE;
        }

        if( (m_pConfig->Synchronize(NULL)) !=UNIX_RS_ERROR_OK)
        {
                m_pConfig->CloseNode();
                return UNIX_RS_ERROR_SYNC;
        }


// 放在这里是为了安全，如果szSerialNo是stSerino的成员时存在问题
        memset(&stSerino,0,sizeof(UNIXSNPARAM));

//
// 开始检查
//

        nLen=1024;
        myId=0x01;
        memset(stSerino.szSn,0,50);
        nRet=m_pConfig->GetItemValueString(myId, szBuf,nLen);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        strncpy(stSerino.szSn,szBuf,50);

// 获取产品大类
        dwSuiteId=99999999;
        myId=0x02;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nBigProdId=dwSuiteId;

// 获取产品类型
        dwSuiteId=99999999;
        myId=0x03;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nProdId=dwSuiteId;

// 获取产品子类型
        dwSuiteId=99999999;
        myId=0x04;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nChildProdId=dwSuiteId;

// 获取过期类型
        dwSuiteId=99999999;
        myId=0x05;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nExpireType=dwSuiteId;

// 获取批号
        dwSuiteId=99999999;
        myId=0x06;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nPNo=dwSuiteId;

// 获取流水号
        dwSuiteId=99999999;
        myId=0x07;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nStreamNo=dwSuiteId;

// 获取地区号
        dwSuiteId=99999999;
        myId=0x08;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nAreaNo=dwSuiteId;

// 获取OEM号
        dwSuiteId=99999999;
        myId=0x09;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nOEMId=dwSuiteId;

// 获取硬件平台
        dwSuiteId=99999999;
        myId=0x0a;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nHardWare=dwSuiteId;

// 获取OS 类型
        dwSuiteId=99999999;
        myId=0x0b;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nOsType=dwSuiteId;

// 获取OS 类型
        dwSuiteId=99999999;
        myId=0x0c;
        nRet=m_pConfig->GetItemValueDWORD(myId, dwSuiteId);
        if(nRet!=UNIX_RS_ERROR_OK)
                goto SnError;
        stSerino.nOsVer=dwSuiteId;

        m_pConfig->CloseNode();

        return UNIX_RS_ERROR_OK;

SnError:

        m_pConfig->CloseNode();
        return UNIX_RS_ERROR_GETVALUE;

}

/////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE CUnixRsCfg::InitOsSnMap()
//  用途： 产生一张unix os 信息和序列号信息的映射表，增加新的
//			版本需要修改这个成员函数
//  对全局变量的影响：
//  参数说明 ：
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::InitOsSnMap()
{

        int nI;

        if(!pstOsType)
        {
                pstOsType =new OSSNMAP [CURRENT_OS_NUMBER];
                if(!pstOsType)
                        return UNIX_RS_ERROR_NEW;
                memset(pstOsType,0,sizeof(OSSNMAP)*CURRENT_OS_NUMBER);
        }


//
// 形成SunOs平台,对应序列号中OS ID为 1
//
        strcpy(pstOsType[0].strOsName,"SunOS");

    strcpy(pstOsType[0].strOsVersion[0],"5.5");		// 2.5 对应SN中OsVer 1
    strcpy(pstOsType[0].strOsVersion[1],"5.6");		//2.6	对应SN中OsVer 2
    strcpy(pstOsType[0].strOsVersion[2],"5.7");		//2.7 对应SN中OsVer 3
    strcpy(pstOsType[0].strOsVersion[3],"5.8"); 	//2.8 对应SN中OsVer 4
    strcpy(pstOsType[0].strOsVersion[4],"5.9"); 	//2.9 对应SN中OsVer 5
    strcpy(pstOsType[0].strOsVersion[5],"5.10");	//3.0,solrais10,对应SN中OsVer 6

        // 下面的os版本，目前没有为solaris定义,如果有新的定义
        //	需要将下面的nI初始化值修改，增加对应的项

        for(nI=6;nI<  CURRENT_OS_NUMBER;nI++)
            strcpy(pstOsType[0].strOsVersion[nI],DEFAULT_STRING);

        //目前SunOs  有二种平台
        for(nI=0;nI<MAX_PLATFORM_NUMBER;nI++)
                strcpy(pstOsType[0].strPlatName[nI],DEFAULT_STRING);

    strcpy(pstOsType[0].strPlatName[0],"sun");	//SPARC，对应SN中PLATFORM 1
    strcpy(pstOsType[0].strPlatName[1],"i86");	//INTEL,对应SN中PLATFORM 2



//
// 形成AIX平台,对应序列号中OS ID为 4
//
        strcpy(pstOsType[3].strOsName,"AIX");

        //目前4 种版本 其值为utsname 结构 version 和 release 联合 格式：version:release
        // 实际上只发布了4.3和5.1

    strcpy(pstOsType[3].strOsVersion[0],"4.0");	// 4.0 对应SN中OsVer 1
    strcpy(pstOsType[3].strOsVersion[1],"4.2");	// 4.2 对应SN中OsVer 2
    strcpy(pstOsType[3].strOsVersion[2],"4.3");	// 4.3 对应SN中OsVer 3
    strcpy(pstOsType[3].strOsVersion[3],"5.1");	// 5.1 对应SN中OsVer 4
    strcpy(pstOsType[3].strOsVersion[4],"5.2");	// 5.2 对应SN中OsVer 5

        //保留
    for(nI=5;nI<CURRENT_OS_NUMBER;nI++)
                strcpy(pstOsType[3].strOsVersion[nI],DEFAULT_STRING);

    //目前1 种平台
        for(nI=0;nI<MAX_PLATFORM_NUMBER;nI++)
                strcpy(pstOsType[3].strPlatName[nI],DEFAULT_STRING);


//
// 形成FreeBSd平台,对应序列号中OS ID为 5
//
        strcpy(pstOsType[4].strOsName,"FreeBSD");

        //目前4个版本
        strcpy(pstOsType[4].strOsVersion[0],"4.3"); 	//4.3  对应SN中OsVer 1
        strcpy(pstOsType[4].strOsVersion[1],"4.4");	//4.4  对应SN中OsVer 2
        strcpy(pstOsType[4].strOsVersion[2],"4.5");	//4.5  对应SN中OsVer 3
        strcpy(pstOsType[4].strOsVersion[3],"4.6");	//4.6  对应SN中OsVer 4
        strcpy(pstOsType[4].strOsVersion[4],"6.2");	//6.2  对应SN中OsVer 4

    //保留
        for(nI=5;nI<  CURRENT_OS_NUMBER;nI++)
            strcpy(pstOsType[4].strOsVersion[nI],DEFAULT_STRING);

    //目前1 种平台
        for(nI=0;nI<MAX_PLATFORM_NUMBER;nI++)
                strcpy(pstOsType[4].strPlatName[nI],DEFAULT_STRING);
    //INTEL
    strcpy(pstOsType[4].strPlatName[1],"i386");

//
// 形成HP-UX平台,对应序列号中OS ID为 7
//

        strcpy(pstOsType[6].strOsName,"HP-UX");

        //目前3 种版本 其值为utsname 结构 version 和 release 联合 格式：version:release
        // 实际出品的只有11
        strcpy(pstOsType[6].strOsVersion[0],"08");	//8.x  对应SN中OsVer 1
        strcpy(pstOsType[6].strOsVersion[1],"09");	//9.x  对应SN中OsVer 2
        strcpy(pstOsType[6].strOsVersion[2],"10");	//10.x  对应SN中OsVer 3
    strcpy(pstOsType[6].strOsVersion[3],"11");	//11.x  对应SN中OsVer 4

    //保留
        for(nI=4;nI<  CURRENT_OS_NUMBER;nI++)
            strcpy(pstOsType[6].strOsVersion[nI],DEFAULT_STRING);


    //目前1 种平台
        for(nI=0;nI<MAX_PLATFORM_NUMBER;nI++)
                strcpy(pstOsType[6].strPlatName[nI],DEFAULT_STRING);


//
// 形成Linux-KERNEL 平台,对应序列号中OS ID为 8
//
        strcpy(pstOsType[7].strOsName,"Linux");

    //目前5 种版本
    //包容2.0~2.8的所有版本

    strcpy(pstOsType[7].strOsVersion[0],"20");	//2.0.x  对应SN中OsVer 1
    strcpy(pstOsType[7].strOsVersion[1],"22");	//2.2.x  对应SN中OsVer 2
    strcpy(pstOsType[7].strOsVersion[2],"24");	//2.4.x  对应SN中OsVer 3
    strcpy(pstOsType[7].strOsVersion[3],"26");	//2.6.x  对应SN中OsVer 4
    strcpy(pstOsType[7].strOsVersion[4],"28");	//2.8.x  对应SN中OsVer 5

    //保留
        for(nI=5;nI<  CURRENT_OS_NUMBER;nI++)
            strcpy(pstOsType[7].strOsVersion[nI],DEFAULT_STRING);

    //目前1 种平台
        for(nI=0;nI<MAX_PLATFORM_NUMBER;nI++)
                strcpy(pstOsType[7].strPlatName[nI],DEFAULT_STRING);
    strcpy(pstOsType[7].strPlatName[1],"i686");


//
// 对应OS ID为2 是RedHat,3是turboLinux,6是xteamlinux
//  由于LINUX 目前按照kernel版本出，不考虑

  return UNIX_RS_ERROR_OK;
}


/////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE CUnixRsCfg::GetSystemInfo()
//  用途： 获取本地系统信息
//  对全局变量的影响：
//  参数说明 ：
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::GetSystemInfo()
{
        struct utsname stName;

        //提取操作系统信息
        memset(&stName,0,sizeof(struct utsname));
        if(uname(&stName)==-1)
        {
                perror("uname:");
                return UNIX_RS_ERROR_SYSINFO;
        }
        else
        {

                memset(szSelfOsName,0,sizeof(szSelfPlatName));
                memset(szSelfOsVersion,0,sizeof(szSelfOsVersion));
                memset(szSelfPlatName,0,sizeof(szSelfPlatName));

                if(!strcmp(stName.sysname,"FreeBSD"))
                {
                        if(strcmp(stName.release,"6.2")<0)
                                strcpy(stName.release,"4.5");
                        else
                                strcpy(stName.release,"6.2");
                }

                strcpy(szSelfOsName,stName.sysname);

        //AIX系统版本为 VERSION.RELEASE
                if(strcmp(stName.sysname,"AIX")==0)
                {

                        sprintf(szSelfOsVersion,"%s.%s",stName.version,stName.release);
                        if(strcmp(szSelfOsVersion,"5.3")>=0)
                                strcpy(szSelfOsVersion,"5.2");
                }
                else if(strcmp(stName.sysname,"SunOS")==0)
                {
                        if(strcmp(stName.release,"5.6") ==0||
                                strcmp(stName.release,"5.7")==0 ||strcmp(stName.release,"5.8")==0 || strcmp(stName.release,"5.9")==0)
                                strcpy(szSelfOsVersion,stName.release);
                        else
                                strcpy(szSelfOsVersion,"5.10");
                }
                else
                {
                         strcpy(szSelfOsVersion,stName.release);
                }

        //SunOS系统平台只检查前三位
                if(strcmp(stName.sysname,"SunOS")==0)
                {
                        strncpy(szSelfPlatName,stName.machine,3);
                }
        else if(strcmp(stName.sysname,"AIX")==0)
        {
           //AIX不检硬件平台
           strcpy(szSelfPlatName,DEFAULT_STRING);
        }
                else
                {
                        strcpy(szSelfPlatName,stName.machine);
                }

        if(strcmp(stName.sysname,"FreeBSD")==0)
        {
                strcpy(szSelfOsVersion,strtok(stName.release,"-"));
        }

                //HP-UX平台
                if(strcmp(stName.sysname,"HP-UX")==0)
                {
                        //HP-UX不检查硬件平台
                        strcpy(szSelfPlatName,DEFAULT_STRING);

                        //求版本号 从A.11.11 中取第二字段
                        char *szVersion;
                        szVersion=strchr(stName.release,'.');
                        if(szVersion!=NULL) szVersion++;
                        int index=0;
                        while(szVersion[index]!='.'&&szVersion[index]!=0){
                                szSelfOsVersion[index]=szVersion[index];
                                index++;
                        }
                        szSelfOsVersion[index]=0;
        }
        }

        return UNIX_RS_ERROR_OK;

}

/////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE CUnixRsCfg::UNIX_RS_ERROR_TYPE CheckOsMatch(UNIXSNPARAM stSerino)
//  用途： 根据序列号结构，检查匹配情况
//  对全局变量的影响：
//  参数说明 ： stSerino：序列号结构
//  返回结果 ：
//		UNIX_RS_ERROR_OK: 成功
//		其它 :失败
///////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CUnixRsCfg::CheckOsMatch(UNIXSNPARAM stSerino)
{
        UNIX_RS_ERROR_TYPE nRet;
        int nOsID=-1;
        int nOsVersionID=-1;
        int nPlatFormID=-1;
        char strTemp[128];

        int nTemp=-1;
//  取得系统信息
        nRet=GetSystemInfo();
        if(nRet!=UNIX_RS_ERROR_OK)
                return nRet;

//
//  初始化unix Os 和序列号对应表
//
        nRet=InitOsSnMap();
        if(nRet!=UNIX_RS_ERROR_OK || (!pstOsType))
        {
                return UNIX_RS_ERROR_NEW;
        }


        nOsID=stSerino.nOsType;
        nOsVersionID=stSerino.nOsVer;
        nPlatFormID=stSerino.nHardWare;


        nRet=UNIX_RS_ERROR_OK;
        //序列号的OS平台或os版本或硬件平台
        if( (nOsID > CURRENT_OS_NUMBER ||nOsID<1 ) ||
                (nOsVersionID>MAX_VERSION_NUMBER || nOsVersionID<1) ||
                (nPlatFormID>MAX_PLATFORM_NUMBER || nPlatFormID <1)	)
        {
                nRet=UNIX_RS_ERROR_SNNOSUPPORTOS;
                goto MatchError;
        }
        else
        {
                nOsID=nOsID-1;
                nOsVersionID--;
                nPlatFormID--;
        }

// Add for aarch(eg. feiteng) version, SN AreaNo is Holland
#if defined(PRODUCT)
#if (PRODUCT==1) || (PRODUCT==3)
    // do nothing
#elif (PRODUCT==6)
    if(stSerino.nAreaNo != CONFIG_SNREGION_GERMANY){
        nRet=UNIX_RS_ERROR_SNNOSUPPORTOS;
                goto MatchError;
    }
#elif (PRODUCT==2)
    if(stSerino.nAreaNo != CONFIG_SNREGION_MIPS) {
        nRet=UNIX_RS_ERROR_SNNOSUPPORTOS;
        goto MatchError;
    }
#elif (PRODUCT==4)
        if(stSerino.nAreaNo != CONFIG_SNREGION_MIPS64){
                nRet=UNIX_RS_ERROR_SNNOSUPPORTOS;
                goto MatchError;
        }
#elif (PRODUCT==5)
    if(stSerino.nAreaNo != CONFIG_SNREGION_HOLLAND){
            nRet=UNIX_RS_ERROR_SNNOSUPPORTOS;
                goto MatchError;
    }
#endif
#endif
/*	printf("hardware:%s\n", szSelfPlatName);
        printf("os_name:%s\n", szSelfOsName);
        printf("os_verison:%s\n", szSelfOsVersion);
        printf("sn_hardware:%s\n", pstOsType[nOsID].strPlatName[nPlatFormID]);
        printf("sn_os_name:%s\n", pstOsType[nOsID].strOsName);
        printf("sn_os_verison:%s:id:%d\n", pstOsType[nOsID].strOsVersion[nOsVersionID], nOsVersionID);
*/
//
// 对应 Linux Kernel 版本
//
        if(nOsID == 7)
        {
                memset(strTemp,0,sizeof(strTemp));

                if(strlen(szSelfOsVersion) < 5) //linux版本为x.y.z-u格式
                {
                        nRet=UNIX_RS_ERROR_STRANGELINUX;
                        goto MatchError;
                }

                //以整数形式处理版本的奇偶
                sprintf(strTemp,"%c%c",szSelfOsVersion[0],szSelfOsVersion[2]);
                nTemp=atoi(strTemp);
                if(nTemp%2!=0)
                {
                        //奇数加一，比如2.3规到2.4
                        nTemp=nTemp+1;
                }
                //修改计算机信息
                memset(szSelfOsVersion,0,sizeof(szSelfOsVersion));
                sprintf(szSelfOsVersion,"%d",nTemp);
        }


  //操作系统是否一致
        if( strcmp(pstOsType[nOsID].strOsName,szSelfOsName) !=0 )
        {
                // 不匹配的系统OS
                nRet=UNIX_RS_ERROR_SNNOMATCHOS;
                goto MatchError;

        }

#if 0
//liuqingxia 20131031 不检查内核版本号
        //操作系统版本是否一致
        if(strcmp(pstOsType[nOsID].strOsVersion[nOsVersionID],szSelfOsVersion) != 0)
        {
                // 不匹配的系统OS 版本
                nRet=UNIX_RS_ERROR_SNNOMATCHVER;
                goto MatchError;
        }

#endif
        //平台是否一致
        if(strcmp(pstOsType[nOsID].strPlatName[nPlatFormID],szSelfPlatName)!=0)
        {

                if(nOsID == 7)
                {
// xuzilong 2006-07-21 modified <
// 为了支持64位机，对于linux 系统不检查该项信息
#if 0
                        if( !(strcmp(szSelfPlatName,"i386")==0 || strcmp(szSelfPlatName,"i686")==0 ||
                                strcmp(szSelfPlatName,"i?86")==0) )
                        {
                                // 不匹配的系统硬件平台
                                nRet=UNIX_RS_ERROR_SNNOMATCHPLAT;
                                goto MatchError;
                        }
#endif
// xuzilong 2006-07-21 modified >
                }
                else
                {
                        // 不匹配的系统硬件平台
                        nRet=UNIX_RS_ERROR_SNNOMATCHPLAT;
                        goto MatchError;
                }
        }

MatchError:
        delete  [] pstOsType;
        pstOsType=NULL;
        return nRet;



}
