///////////////////////////////////////////////////////////////////////////
//  项目名	：瑞星2005版配置文件树模块组
//	文件名	：regapi.cpp
//	用  途	：实现各种unix产品对配置树的访问接口，导出的接口是C方式的
//	作  者	：许自龙
//	完成日期  : 2004-11-10
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <regapi.h>
#include <unixrscfg.h>
#include <checkvalid.h>

#define SN_FILE_NAME    "sn.txt"
#define PATH_FILE_NAME  "path.txt"

///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE CheckCfgSnValid(void *pConfig,int nCode,char *szSerial,
//			UNIXSNPARAM *pStSn)
//  用途： 检查序列号的合法性，全局导出接口，不要直接使用CUnixRsCfg类
//  对全局变量的影响：
//  参数说明 ：
//		pConfig :配置树结构指针
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)
//		szSerial : 要检查的序列号
//		pStSn ：返回检查结果数据结构，为NULL不返回
//  返回结果 ：
///////////////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CheckCfgSnValid(void *pConfig,int nCode, const char *szSerial,UNIXSNPARAM *pStSn)

{
    if(nCode!=0 && nCode!=1)
        return 	UNIX_RS_ERROR_INVALIDPRODUCT;

    UNIXSNPARAM stSerino;
    UNIX_RS_ERROR_TYPE nRet;

    CUnixRsCfg *pstCfg;
    pstCfg=(CUnixRsCfg *)pConfig;

    if(!pstCfg)
        return UNIX_RS_ERROR_INVALIDPARAM;

// 是否是一个合法序列号
    nRet=pstCfg->CheckSerino((char*)szSerial,stSerino);

    if(pStSn)
        memcpy(pStSn,&stSerino,sizeof(UNIXSNPARAM));

    if(nRet!=UNIX_RS_ERROR_OK || stSerino.szSn[0]=='\0')
    {
        return nRet;
    }

// 是否与本地系统相配
    nRet=pstCfg->CheckOsMatch(stSerino);
    return nRet;

}

///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE ReadCfgSnInfo(void *pConfig,int nCode,UNIXSNPARAM *pStSn)
//  用途：  读取当前安装的注册信息，
//  对全局变量的影响：
//  参数说明 ：
//		pConfig :配置树结构指针
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)
//		pStSn ：返回检查结果数据结构，为NULL不返回
//  返回结果 ：
//////////////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE ReadCfgSnInfo(void *pConfig,int nCode,UNIXSNPARAM *pStSn)
{
    if(nCode!=0 && nCode!=1)
    {
        return UNIX_RS_ERROR_INVALIDPRODUCT;
    }

    UNIX_RS_ERROR_TYPE nRet;

    CUnixRsCfg *pstCfg;
    pstCfg=(CUnixRsCfg *)pConfig;

    if(!pstCfg)
        return UNIX_RS_ERROR_INVALIDPARAM;

    UNIXSNPARAM stSerino;
    memset(&stSerino, 0, sizeof(stSerino));
    nRet=pstCfg->ReadSnInfo(stSerino);

    if(pStSn)
        memcpy(pStSn,&stSerino,sizeof(UNIXSNPARAM));
    return nRet;

}

///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：  extern UNIX_RS_ERROR_TYPE WriteCfgSnInfo(void *pConfig,int nCode,
//		char *szSerial	UNIXSNPARAM *pStSn)
//  用途： 检查序列号的合法性， 写注册文件和配置树中的序列号
//  对全局变量的影响：
//  参数说明 ：
//		pConfig ：配置树结构指针
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)
//		szSerial : 要检查的序列号
//		pStSn ：返回检查结果数据结构，为NULL不返回
//  返回结果 ：
///////////////////////////////////////////////////////////////////////////////////////////////

extern UNIX_RS_ERROR_TYPE WriteCfgSnInfo(void *pConfig,int nCode,char *szSerial,UNIXSNPARAM *pStSn)
{

    UNIX_RS_ERROR_TYPE nRet;
    UNIXSNPARAM stSerino;

    if(nCode!=0 && nCode!=1)
    {
        return UNIX_RS_ERROR_INVALIDPRODUCT;
    }

    if(!szSerial)
    {
        return UNIX_RS_ERROR_INVALIDSN;
    }

    CUnixRsCfg *pstCfg;
    pstCfg=(CUnixRsCfg *)pConfig;

    if(!pstCfg)
        return UNIX_RS_ERROR_INVALIDPARAM;


    //
    // 检查序列号的合法性
    //
    nRet=pstCfg->CheckSerino(szSerial,stSerino);
    if(nRet!=UNIX_RS_ERROR_OK)
    {
            return nRet;
    }

// 是否与本地系统相配
    nRet=pstCfg->CheckOsMatch(stSerino);
    if(nRet!=UNIX_RS_ERROR_OK)
    {
            return nRet;
    }


    //
    // 安装序列号
    //
    nRet=pstCfg->WriteSnInfo(szSerial);
    if(nRet!=UNIX_RS_ERROR_OK)
        return nRet;

    if(pStSn)
    {
        nRet=pstCfg->ReadSnInfo(stSerino);
        memcpy(	pStSn,	&stSerino,sizeof(UNIXSNPARAM));
    }
    return nRet;


}

///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE ReadCfgScanParam(void *pConfig,void *pParm)
//  用途： 读取查杀参数
//  对全局变量的影响：
//  参数说明 ：
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)和2（ravtimer)
//		pParm :通用的参数指针，函数内部根据nCode转换
//		pConfig ：配置文件指针
//  返回结果 ：
///////////////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE ReadCfgScanParam(void *pConfig,int nCode,void *pParm)
{
    UNIXRSRAVPARAM *pStRavScan = NULL;
    UNIXRSRAVPARAM  stRavScan;
    UNIX_RS_ERROR_TYPE nRet;

    CUnixRsCfg *pstCfg;
    pstCfg=(CUnixRsCfg *)pConfig;

    if(!pstCfg)
        return UNIX_RS_ERROR_INVALIDPARAM;

    switch(nCode)
    {
        case 0:
            pStRavScan=(UNIXRSRAVPARAM *)pParm;
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            return UNIX_RS_ERROR_INVALIDPRODUCT;
    }
    memset(&stRavScan, 0, sizeof(UNIXRSRAVPARAM));
    nRet=pstCfg->ReadRavService(stRavScan);
    if(nRet==UNIX_RS_ERROR_OK)
    {
        memcpy(pStRavScan,&stRavScan,sizeof(UNIXRSRAVPARAM));
    }
    return nRet;
}



///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：  WriteCfgScanParam(void *pConfig,int nCode,void *pParm)
//  用途： 写入查杀参数
//  对全局变量的影响：
//  参数说明 ：
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)和2（ravtimer)
//		pParm :通用的参数指针，函数内部根据nCode转换
//		pConfig ：配置文件指针
//  返回结果 ：
///////////////////////////////////////////////////////////////////////////////////////////////

extern UNIX_RS_ERROR_TYPE WriteCfgScanParam(void *pConfig,int nCode,void *pParm)
{
    char *szImportFile;
    UNIX_RS_ERROR_TYPE nRet;

    CUnixRsCfg *pstCfg;
    pstCfg=(CUnixRsCfg *)pConfig;

    if(!pstCfg)
        return UNIX_RS_ERROR_INVALIDPARAM;


    switch(nCode)
    {
        case EXPORTCONFIG:
        case EXPORTRAVSERVICE:
            // 网络版传入一个要导入的文件名
            szImportFile=(char *)pParm;
            break;

        default:
            return UNIX_RS_ERROR_INVALIDPRODUCT;
    }

    nRet=pstCfg->ImportRsCfg(nCode,szImportFile);
    return nRet;


}


/////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE ExportNode(int nCode,char *szFile)
//  用途：将指定的节点流导出到文件中
//  对全局变量的影响：
//  参数说明 ：
//
//  返回结果 ：0 ： OK
/////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE ExportNode(void *pConfig,int nCode,char *szExportFile)
{
    CUnixRsCfg *pstCfg;
    pstCfg=(CUnixRsCfg *)pConfig;

    if(!pstCfg)
        return UNIX_RS_ERROR_INVALIDPARAM;

    return pstCfg->ExportRsCfg(nCode,szExportFile);

}

////////////////////////////////////////////////////////////////////////////////
//  函数名： int  GetInstallPath(int nCode,char *szInstallPath,int nLen)
//  用途： 取得安装路径,
//  对全局变量的影响：
//  参数说明 ：
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)和2（ravtimer)
//			引入该参数的目的是为了将来的扩充。
//		szInstallPath :返回安装路径的缓冲区。
//		nLen ：缓冲区的长度
//
//  返回结果 ：NULL :失败，szInstallPath :成功
////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE GetInstallPath(int nCode,char *szInstallPath,int nLen)
{
    return GetInstallPath_(nCode,szInstallPath,nLen,NULL);
}

////////////////////////////////////////////////////////////////////////////////
//  函数名： int  GetInstallPath_(int nCode,char *szInstallPath,int nLen,REGINFO *pReg)
//  用途： 取得安装路径
//  对全局变量的影响：
//  参数说明 ：
//		nCode ：产品代码,目前只支持0（ravservice)和1(ravwrd)和2（ravtimer)
//			引入该参数的目的是为了将来的扩充。
//		szInstallPath :返回安装路径的缓冲区。
//		nLen ：缓冲区的长度
//		pReg :如果pReg非空，返回注册信息
//  返回结果 ：NULL :失败，szInstallPath :成功
////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE GetInstallPath_(int nCode,char *szInstallPath,int nLen,REGINFO *pReg)
{
    REGINFO  RegInfo;
    UNIX_RS_ERROR_TYPE nRet;
    struct stat status;

    if(!szInstallPath)
        return UNIX_RS_ERROR_INVALIDPARAM;

    if(nCode<0 || nCode>2)
        return UNIX_RS_ERROR_INVALIDPRODUCT;

    if((nRet=readserial(&RegInfo)!=UNIX_RS_ERROR_OK))
        return nRet;

    if(stat(RegInfo.szInstallPath,&status))
    {
        return UNIX_RS_ERROR_INVALIDREGISTER;
    }

#ifdef GXB
    //no check inode
#else
    if(RegInfo.proino!=status.st_ino)
        return UNIX_RS_ERROR_INVALIDREGISTER;
#endif

    if(strlen(RegInfo.szInstallPath) >nLen-1)
        return UNIX_RS_ERROR_INVALIDPARAM;  // 输入缓冲区的长度不够
    else
        strcpy(szInstallPath,RegInfo.szInstallPath);

    if(pReg)
        memcpy(pReg,&RegInfo,sizeof(REGINFO));

    return UNIX_RS_ERROR_OK;

}

UNIX_RS_ERROR_TYPE readserial(REGINFO *pRegInfo)
{
    char szRegisterFile[256];
    FILE *fpReg;
    //UNIX_RS_ERROR_TYPE nRet;

    memset(pRegInfo,0,sizeof(REGINFO));

    strcpy(szRegisterFile,RAVNETWRD_REGISTER_FILE);

    if((fpReg=fopen(szRegisterFile,"r")) ==NULL)
    {
        return UNIX_RS_ERROR_OPENREGERROR;  // 打开注册文件错
    }


    if(fread(pRegInfo,sizeof(REGINFO),1,fpReg)!=1)
    {
        fclose(fpReg);
        return UNIX_RS_ERROR_INVALIDREGISTER;
    }
    fclose(fpReg);

    return UNIX_RS_ERROR_OK;
}


////////////////////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE InitCfgConfig(int nCode,const char *szConfigFile,void **pConfig)
//  用途：初始化一棵配置树
//  对全局变量的影响：
//  参数说明 ：
//		nCode :产品代号
//		szConfigFile :配置文件名称
//		pConfig : 返回的配置树结构
//  返回结果 ：
////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE InitCfgConfig(int nCode,const char *szConfigFile,void **pConfig)
{
    CUnixRsCfg *pstTree;
    UNIX_RS_ERROR_TYPE nRet = UNIX_RS_ERROR_OK;

    if(!pConfig ||!szConfigFile || szConfigFile[0]=='\0' )
        return UNIX_RS_ERROR_INVALIDPARAM;


    pstTree=new CUnixRsCfg(szConfigFile);
    if(!pstTree)
        return UNIX_RS_ERROR_NEW;

    nRet=pstTree->Init();
    if(nRet!=UNIX_RS_ERROR_OK)
    {
        delete pstTree;
        *pConfig=NULL;
    }
    else
    {
        *pConfig=(void *)pstTree;
    }
    return nRet;

}

////////////////////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE ExitCfgConfig(int nCode,char *szConfigFile,void **pConfig)
//  用途：初始化一棵配置树
//  对全局变量的影响：
//  参数说明 ：
//		pConfig : 配置树结构
//  返回结果 ：
////////////////////////////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE ExitCfgConfig(int nCode,void **pConfig)
{
    CUnixRsCfg *pstTree;

    if(!pConfig  || *pConfig==NULL)
        return UNIX_RS_ERROR_OK;


    pstTree=(CUnixRsCfg *) *pConfig;
    delete pstTree;

    *pConfig=NULL;

    return UNIX_RS_ERROR_OK;
}



/////////////////////////////////////////////////////////////////
//  函数名： UNIX_RS_ERROR_TYPE  ReadSNID_(int nCode,UNIXSNPARAM *pStSn)
//  用途： 读取序列号，如果配置文件的序列号不存在，安装之
//  对全局变量的影响：
//  参数说明 ：nCode ：产品代号
//		 pStSn :返回读取的序号
//
//  返回结果 ：
/////////////////////////////////////////////////////////////
UNIX_RS_ERROR_TYPE  ReadSNID_(int nCode, const char *configfile, const char *installpath, const char *serial, UNIXSNPARAM *pStSn)
{
    UNIX_RS_ERROR_TYPE nRet;
    char szInstallPath[256],szConfigFile[256];

    void *pHandle;
    CUnixRsCfg *pstCfg;
    UNIXSNPARAM stSerino;
    REGINFO stRegInfo;

    if(!pStSn)
        return -1;

// 获取产品安装路径, // 读取序列号
    memset(&stRegInfo, 0, sizeof(stRegInfo));
    strcpy(stRegInfo.szInstallPath, installpath);
    strcpy(stRegInfo.szSerial, serial);

    strcpy(szConfigFile, configfile);

    nRet=InitCfgConfig(nCode,szConfigFile,&pHandle);
    if(nRet!=UNIX_RS_ERROR_OK )
    {
        return nRet;
    }
    if(!pHandle)
        return UNIX_RS_ERROR_NEW;

    nRet=ReadCfgSnInfo(pHandle,0,pStSn);

// 检查序列号
    if(nRet==UNIX_RS_ERROR_OK)
    {


        pstCfg=(CUnixRsCfg *)pHandle;

        if(!pstCfg)
            return UNIX_RS_ERROR_INVALIDPARAM;

        nRet=pstCfg->CheckSerino((char*)serial, stSerino); // 检查序列号的合法性
        if(nRet!=UNIX_RS_ERROR_OK)
        {
            ExitCfgConfig(0,&pHandle);
            return nRet;
        }

        nRet=pstCfg->CheckOsMatch(*pStSn); // 是否与本地系统相配
        if(nRet!=UNIX_RS_ERROR_OK)
        {
            ExitCfgConfig(0,&pHandle);
            return nRet;
        }
    }

    ExitCfgConfig(0,&pHandle);
    return nRet;

}

UNIX_RS_ERROR_TYPE  ReadSNID_(int nCode, UNIXSNPARAM *pStSn) {
    // 这是从老版拷贝的代码, 在此半配置文件改为参数传入, 由于是老版代码, 所以先屏掉,以待后续排查
    char szInstallPath[256], szConfigFile[256];
    int nRet = -1;
    REGINFO stRegInfo;

    if((nRet=GetInstallPath_(RAVNET_PRODUCT,szInstallPath,sizeof(szInstallPath),&stRegInfo)))
    {
        return nRet;
    }


    switch(nCode)
    {
        case RAVNET_PRODUCT:
            sprintf(szConfigFile,"%s/ravnet/sys/RsConfig.cfg",szInstallPath);
            break;
        case RAVWRD_PRODUCT:
        case RAVTIMER_PRODUCT:
            sprintf(szConfigFile,"%s/rasingle/conf/RsConfig.cfg",szInstallPath);
            break;
    }
    return ReadSNID_(nCode, szConfigFile,stRegInfo.szInstallPath, stRegInfo.szSerial, pStSn);
}

/////////////////////////////////////////////////////////////////
//  函数名： UNIX_RS_ERROR_TYPE ReadSNID(UNIXSNPARAM *pStSn)
//  用途： 读取序列号，主要为升级使用，实际上是ReadSNID_的壳
//  对全局变量的影响：
//  参数说明 ： pStSn ：返回序列号结构
//
//  返回结果 ：返回ReadSNID_的返回值
/////////////////////////////////////////////////////////////
UNIX_RS_ERROR_TYPE  ReadSNID(const char *configfile, const char *installpath, const char *serial, UNIXSNPARAM *pStSn)
{
    return ReadSNID_(RAVNET_PRODUCT, configfile, installpath, serial, pStSn);
}




/////////////////////////////////////////////////////////////////
//  函数名：  UNIX_RS_ERROR_TYPE WriteRegInfo(int nCode,char char *szInstPath,char *szSerial)
//  用途： 写注册文件
//  对全局变量的影响：
//  参数说明 ：
//			nCode ：产品代号
//			szInstPath: 安装路径
//			szSerial：序列号
//  返回结果 ：
/////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE WriteRegInfo(void *pConfig,int nCode,char *szInstPath,char *szSerial)
{
    UNIX_RS_ERROR_TYPE nRet;

    if(pConfig) // 已经指定了配置树结构指针，将序列号写入配置树文件
    {
        nRet=WriteCfgSnInfo(pConfig,nCode,szSerial,NULL);
        if(nRet!=UNIX_RS_ERROR_OK)
            {
            return nRet;
            }
    }

//
// 写注册文件
//
    REGINFO  RegInfo;
    char szRegisterFile[256];
    FILE *fpReg;
    struct stat status;

    if(stat(szInstPath,&status))
    {
        return UNIX_RS_ERROR_INVALIDREGISTER;
    }

    memset(&RegInfo,0,sizeof(REGINFO));
    strcpy(RegInfo.szInstallPath,szInstPath);
    strcpy(RegInfo.szSerial,szSerial);
    RegInfo.proino=status.st_ino;

    strcpy(szRegisterFile,RAVNETWRD_REGISTER_FILE);
    if((fpReg=fopen(szRegisterFile,"w"))==NULL)
    {
        return UNIX_RS_ERROR_OPENREGERROR;
    }

    if(fwrite(&RegInfo,sizeof(REGINFO),1,fpReg)!=1)
    {
        fclose(fpReg);
        unlink(szRegisterFile);
        return UNIX_RS_ERROR_WRITEREGFAILED;
    }
    fflush(fpReg);
    fclose(fpReg);
    return UNIX_RS_ERROR_OK;

}


/////////////////////////////////////////////////////////////////
//  函数名：static UNIX_RS_ERROR_TYPE CopyNewRsCfgFile(int nCode,
//		const char *szFile,char *szConfigFile)
//  用途： 安装新的配置文件RsConfig.cfg
//  对全局变量的影响：
//  参数说明 ：
//			nCode ：产品代号
//			szFile: 要安装的文件
//			szConfigFile：安装到的文件
//  返回结果 ：
/////////////////////////////////////////////////////////////

static UNIX_RS_ERROR_TYPE CopyNewRsCfgFile(int nCode,const char *szFile,char *szConfigFile)
{
    char szBuf[8192];
    FILE *fp,*fin;
    int nRet,nLen;
    UNIXSNPARAM stSn;

// 复制文件
    if( (fin=fopen(szFile,"r"))==NULL)
        return -1;

    if( (fp=fopen(szConfigFile,"w"))==NULL)
    {
        fclose(fin);
        return -1;
    }

    nLen=sizeof(szBuf);
    while( (nRet=fread(szBuf,1,nLen,fin))>0)
    {
        if( (fwrite(szBuf,nRet,1,fp))!=1)
        {
            fclose(fin);
            fclose(fp);
            unlink(szFile);
            return -1;
        }
    }
    fflush(NULL);
    fclose(fin);
    fclose(fp);

// 检查配置文件中的序列号，如果配置文件中不存在，安装上
    if(nCode==RAVNET_PRODUCT)
    {
        if( ReadSNID_(nCode,&stSn)==0)
            return 0;
        else
            return -1;
    }
    return 0;

}

/////////////////////////////////////////////////////////////////////////////////////////
//  函数名：static UNIX_RS_ERROR_TYPE CopyAddRsCfgFile(int nCode,
//		const char *szFile,char *szSnFile,char *szRavFile,char *szConfigFile)
//  用途： 将新的配置文件和已经存在的配置文件合并
//  对全局变量的影响：
//  参数说明 ：
//		nCode ：产品代号
//		szFile: 要安装的配置文件
//		szSnFile ：从已经存在的配置文件中导出的0xfffffff->0x02节点文件，主要是序列号的信息
//		szRavFile: 从已经存在的配置文件中导出的0x6000->0x02文件，主要是网络版的查杀配置
//		szConfigFile : 安装后的配置文件
//  返回结果 ：0 : 成功，－1：失败
/////////////////////////////////////////////////////////////////////////////////////////
static UNIX_RS_ERROR_TYPE CopyAddRsCfgFile(int nCode,const char *szFile,char *szSnFile,char *szRavFile,char *szConfigFile)
{
//	UNIXRSRAVPARAM stParam;
    UNIX_RS_ERROR_TYPE nRet;
    UNIXSNPARAM stSn;

    void *pHandle;
    char szBuf[8192];
    FILE *fp,*fin;

    nRet=InitCfgConfig(0,szFile,&pHandle);
    if(nRet!=UNIX_RS_ERROR_OK )
    {
        fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
        return -1;
    }
    if(!pHandle)
        return -1;


    switch(nCode)
    {
        case RAVNET_PRODUCT:
            nRet=WriteCfgScanParam(pHandle,EXPORTCONFIG,szSnFile);
            if(nRet)
                return nRet;
            nRet=WriteCfgScanParam(pHandle,EXPORTRAVSERVICE,szRavFile);
            if(nRet)
                return nRet;
            break;
        default:
            break;
    }


    ExitCfgConfig(0,&pHandle);

    if( (fin=fopen(szFile,"r"))==NULL)
        return -1;

    if( (fp=fopen(szConfigFile,"w"))==NULL)
    {
        fclose(fin);
        return -1;
    }

    while( (nRet=fread(szBuf,1,8192,fin))>0)
    {
        if( (fwrite(szBuf,nRet,1,fp))!=1)
        {
            fclose(fin);
            fclose(fp);
            unlink(szFile);
            return -1;
        }
    }
    fflush(NULL);
    fclose(fin);
    fclose(fp);


    if(nCode==RAVNET_PRODUCT) // 只对ravnet检查序列号
    {
        if( ReadSNID_(nCode,&stSn)==0)
            return 0;
        else
            return -1;
    }
    return 0;


}

/////////////////////////////////////////////////////////////////
//  函数名：UNIX_RS_ERROR_TYPE CopyRsCfg(int nCode,const char *szFile)
//  用途： 复制配置文件，策略如下：
//		如果指定的配置文件存存在，并且有正确的序列号，将老的配置
//		文件的内容导出，加入新的配置文件。
//		否则重新安装配置文件
//  对全局变量的影响：
//  参数说明 ：
//
//  返回结果 ：
/////////////////////////////////////////////////////////////

UNIX_RS_ERROR_TYPE CopyRsCfg(int nCode,const char *szFile)
{
    UNIX_RS_ERROR_TYPE nRet;
    char szInstallPath[256],szConfigFile[256];
    char szRavFile[50],szSnFile[50];

    void *pHandle;
    CUnixRsCfg *pstCfg;
    UNIXSNPARAM stSerino;
    REGINFO stRegInfo;

    if(access(szFile,F_OK))
        return -1;

// 获取产品安装路径

    if((nRet=GetInstallPath_(RAVNET_PRODUCT,szInstallPath,sizeof(szInstallPath),&stRegInfo)))
    {
        return nRet;
    }

// 根据产品代号查找原来的配置文件
    switch(nCode)
    {
        case RAVNET_PRODUCT:
            sprintf(szConfigFile,"%s/ravnet/sys/RsConfig.cfg",szInstallPath);
            break;
        case RAVWRD_PRODUCT:
        case RAVTIMER_PRODUCT:
            sprintf(szConfigFile,"%s/ravsingle/conf/RsConfig.cfg",szInstallPath);
            break;
        default :
            return -1;

    }

    if(access(szConfigFile,F_OK))
    {
        // 配置文件不存在，新安装
        return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
    }

    nRet=InitCfgConfig(RAVNET_PRODUCT,szConfigFile,&pHandle);
    if(nRet!=UNIX_RS_ERROR_OK )
    {
        return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
    }
    if(!pHandle)
        return CopyNewRsCfgFile(nCode,szFile,szConfigFile);

// 检查序列号,对于网络版需要导出老的配置文件的内容
    if(nCode==RAVNET_PRODUCT)
    {
        nRet=ReadCfgSnInfo(pHandle,0,&stSerino);

        if(nRet!=UNIX_RS_ERROR_OK) // 配置文件中没有正确的序列号
        {
            ExitCfgConfig(0,&pHandle);
            return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
        }

        pstCfg=(CUnixRsCfg *)pHandle;

        if(!pstCfg)
        {
            ExitCfgConfig(0,&pHandle);
            return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
        }

        nRet=pstCfg->CheckSerino(stSerino.szSn,stSerino); // 检查序列号的合法性
        if(nRet!=UNIX_RS_ERROR_OK)
        {
            ExitCfgConfig(0,&pHandle);
            return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
        }

        nRet=pstCfg->CheckOsMatch(stSerino); // 是否与本地系统相配
        if(nRet!=UNIX_RS_ERROR_OK)
        {
            ExitCfgConfig(0,&pHandle);
            return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
        }

        sprintf(szSnFile,"0xffffffff.cfg");
        sprintf(szRavFile,"0x6000.cfg");

        nRet=ExportNode(pHandle,EXPORTCONFIG,szSnFile);
        if(nRet)
        {
            ExitCfgConfig(0,&pHandle);
            return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
        }


        nRet=ExportNode(pHandle,EXPORTRAVSERVICE,szRavFile);
        if(nRet)
        {
            ExitCfgConfig(0,&pHandle);
            return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
        }

// 如果走到这里，表示合并
        ExitCfgConfig(0,&pHandle);
        nRet=CopyAddRsCfgFile(nCode,szFile,szSnFile,szRavFile,szConfigFile);
        unlink(szSnFile);
        unlink(szRavFile);
        strcat(szSnFile,".cps");
        strcat(szRavFile,".cps");
        unlink(szSnFile);
        unlink(szRavFile);

    }
    else
    {
        // TODO其它的产品现在没有定
        ExitCfgConfig(0,&pHandle);
        nRet=CopyAddRsCfgFile(nCode,szFile,NULL,NULL,szConfigFile);
    }

    if(nRet)
         return CopyNewRsCfgFile(nCode,szFile,szConfigFile);
    else
        return 0;

}



/////////////////////////////////////////////////////////////////
//  函数名：  char *unixrscfgerr(UNIX_RS_ERROR_TYPE nErr,char *szBuf,int nLen)
//  用途： 返回CUnixRsCfg类的成员函数的错误信息
//  对全局变量的影响：
//  参数说明 ：
//			nErr :错误代码
//			szBuf: 返回错误信心的缓冲区
//			nLen：缓冲区的长度
//  返回结果 ：
/////////////////////////////////////////////////////////////

char *unixrscfgerr(UNIX_RS_ERROR_TYPE nErr,char *szBuf,int nLen)
{
    if(!szBuf)
        return NULL;

    memset(szBuf,0,nLen);
    switch(nErr)
    {
        case UNIX_RS_ERROR_OK :
            strncpy(szBuf,"Call RsConfig Ok",nLen-1);
            break;

        case UNIX_RS_ERROR_NEWOBJ :
            strncpy(szBuf,"Call NewConfigObject Failed",nLen-1);
            break;

        case UNIX_RS_ERROR_LOADCONF :
            strncpy(szBuf,"RsConfig Load Config File Failed",nLen-1);
            break;

        case UNIX_RS_ERROR_CREATENODE :
            strncpy(szBuf,"RsConfig CreateNod Failed",nLen-1);
            break;

        case UNIX_RS_ERROR_GETVALUE :
            strncpy(szBuf,"RsConfig Get  Value Failed ",nLen-1);
            break;

        case UNIX_RS_ERROR_IMPORT :
            strncpy(szBuf,"RsConfig import File Error",nLen-1);
            break;

        case UNIX_RS_ERROR_SYNC :
            strncpy(szBuf,"RsConfig Sync File Error",nLen-1);
            break;

        case UNIX_RS_ERROR_GETCHILD:
            strncpy(szBuf,"Get Child Node Error",nLen-1);
            break;

        case UNIX_RS_ERROR_SETVALUE:
            strncpy(szBuf,"RsConfig Set Value Error",nLen-1);
            break;

        case UNIX_RS_ERROR_SYSINFO:
            strncpy(szBuf,"RsConfig Get Local Os Info Error",nLen-1);
            break;

        case UNIX_RS_ERROR_NEW:
            strncpy(szBuf,"RsConfig New Object Error",nLen-1);
            break;

        case UNIX_RS_ERROR_STRANGELINUX:
            strncpy(szBuf,"RsConfig Local Os Information For Linux Error",nLen-1);
            break;

        case UNIX_RS_ERROR_SNNOSUPPORTOS:
            strncpy(szBuf,"RsConfig Os Id in Sn Currently Not Supported",nLen-1);
            break;

        case UNIX_RS_ERROR_SNNOMATCHOS:
            strncpy(szBuf,"RsConfig Os ID in Sn Not match Local System",nLen-1);
            break;

        case UNIX_RS_ERROR_SNNOMATCHVER:
            strncpy(szBuf,"RsConfig  Os Version in Sn Not match Local System",nLen-1);
            break;

        case UNIX_RS_ERROR_SNNOMATCHPLAT:
            strncpy(szBuf,"RsConfig Os PlatForm in Sn Not match Local System",nLen-1);
            break;

        case UNIX_RS_ERROR_INVALIDPRODUCT:
            strncpy(szBuf,"RsConfig Invalid Product",nLen-1);
            break;

        case UNIX_RS_ERROR_INVALIDREGISTER:
            strncpy(szBuf,"RsConfig Invalid Register Information or no Register",nLen-1);
            break;

        case UNIX_RS_ERROR_OPENREGERROR:
            strncpy(szBuf,"Open Register File Error",nLen-1);
            break;

        case UNIX_RS_ERROR_INVALIDSN:
            strncpy(szBuf,"Input Serial Invalid",nLen-1);
            break;

        case UNIX_RS_ERROR_WRITEREGFAILED:
            strncpy(szBuf,"Write Register File Error",nLen-1);
            break;

        case UNIX_RS_ERROR_INVALIDPARAM:
            strncpy(szBuf,"Input Invalid Param",nLen-1);
            break;


        default :
            strncpy(szBuf,"RsConfig Unknown  Error",nLen-1);
            break;

    }

    return szBuf;

}

bool checkCfgExpired(const char *sn) {
    CCheckValid cv;
    char install[12] = {0};
    char end[12] = {0};
    cv.GetInstallTime(sn, install, 12);
    cv.GetEndTime(sn, end, 12);

    int itime = atoi(install);
    int etime = atoi(end);

    if (etime < itime) {
        return true;
    }

    time_t now;
    time(&now);

    if(now < itime) {
        return true;
    }
    if (now > etime) {
        return true;
    }
    return false;
}

bool warningExpired(const char *sn) {
    CCheckValid cv;
    char install[12] = {0};
    char end[12] = {0};
    cv.GetInstallTime(sn, install, 12);
    cv.GetEndTime(sn, end, 12);

    int itime = atoi(install);
    int etime = atoi(end);


    time_t now;
    time(&now);

    if (now < etime && (etime-now)<14*24*60*60) {
        return true;
    }
    return false;
}

bool resetSN(const char *sn) {
    char install[128] = {0};
    if (UNIX_RS_ERROR_OK != GetInstallPath(0, install, 128)) {
        return false;
    }

    void *handle = NULL;
    char configFile[128] = {0};
    sprintf(configFile, "%s/ravnet/sys/RsConfig.cfg", install);
    int err = UNIX_RS_ERROR_OK;
    if (InitCfgConfig(0, configFile, &handle) == UNIX_RS_ERROR_OK) {
        err  = WriteRegInfo(handle, 0, install, (char*)sn);
    }

    ExitCfgConfig(0, &handle);
    return err==UNIX_RS_ERROR_OK;
}

bool isTrialSnVersion(const char *sn) {
    bool ret = false;
#if defined(PRODUCT)
#if (PRODUCT==1)
    return strcmp(sn, "1QNKI0-GEQVKG-43JKRH-L75308") == 0;
#elif (PRODUCT==3)
    return strcmp(sn, "F7QJ3B-JQA98R-TVF8SA-1L5308") == 0;
#endif
#endif
    return ret;
}
