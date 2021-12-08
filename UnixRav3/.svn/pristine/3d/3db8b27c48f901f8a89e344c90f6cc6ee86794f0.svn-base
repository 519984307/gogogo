//////////////////////////////////////////////////////////////////////////////////
////	项目名称：unix 产品升级打包程序
////	文件名称：autopack.cpp
////	实现功能：
////	作    者：许自龙
////	编写日期：2002-3-18 11:27:45
////
////	许自龙2004-03
////
//////////////////////////////////////////////////////////////////////////////////
////
////    关于versionnew.list文件格式的说明
////  versionnew.list 是binary文件,格式包括如下部分
////  第一个产品的tagVERSION(版本信息)
////	第一个产品第一个文件的tagFILEVERSION(文件信息)
////  第一个产品第二个文件的tagFILEVERSION(文件信息)
////  .....
//// 	第一个产品最后一个个文件的tagFILEVERSION(文件信息)
////  第二个产品的tagVERSION(版本信息)
////	第二个产品第一个文件的tagFILEVERSION(文件信息)
////  第二个产品第二个文件的tagFILEVERSION(文件信息)
////  .....
//// 	第二个产品最后一个个文件的tagFILEVERSION(文件信息)
////  .......
//////////////////////////////////////////////////////////////////////////////////////
////
////	[SOFTINFO]
////	SOFTNEWVERSION=19.20
////	SOFTOLDVERSION=
////	SOFTCOUNT=4
////	[SOFT01]
////	NAME=LINUXKERNEL.24.INTEL.CHAR.DEFAULT
////	PATH=ravsingle
////
////	[SOFT02]
////	NAME=LINUXKERNEL.24.INTEL.NETWORK.DEFAULT
////	PATH=ravnet
////
////	[SOFT03]
////	NAME=LINUXKERNEL.24.INTEL.GUI.DEFAULT
////	PATH=ravgui
////
////	[SOFT04]
////	NAME=LINUXKERNEL.24.INTEL.COMM.DEFAULT
////	PATH=.///////////////////////////////////////////////////////////////////////////////////////
////
////
////////////////////////////////////////////////////////////////////////////////////////
//#include <sys/types.h>
//#include <dirent.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <errno.h>
//extern int errno;
//#include "autopack.h"
//#include "md5.h"
//#include "ravunixutil.h"

//#if 0
//#if  defined(PLATFORM_TYPE_HP_RISC)
//extern "C"
//int *readdir_r(DIR *dirp, struct dirent *entry,struct dirent **result);
//#endif
//#endif

//#include <string>

//////////////////////////////////////////////////////////////////////////////////
////	函数名称：static int GetFileType(const char *strFile)
////	实现功能：获取指定的路径类型
////	对全局变量的影响：
////	参数说明：
////		strSrcFile :源路径名
////	返回结果说明：-1：失败 1：目录 2：文件 3：链接
//////////////////////////////////////////////////////////////////////////////////
//static int GetFileType(const char *strFile)
//{
//    struct stat status;
//    if(stat(strFile,&status)==-1)
//        return -1;
//    if(S_ISDIR(status.st_mode))
//        return 1;
//    if(S_ISREG(status.st_mode))
//        return 2;
//    if(S_ISLNK(status.st_mode))
//        return 3;

//    return -1;
//}

//////////////////////////////////////////////////////////////////////////////////
////	函数名称：int CompFileByBit(const char *strSrcFile,const char *strTargFile)
////	实现功能：比较两个文件的内容是否相同
////	对全局变量的影响：
////	参数说明：
////		strSrcFile :源文件
////		strTargFile :目的文件
////	返回结果说明：-1：失败 0：文件相同 1：文件不同
//////////////////////////////////////////////////////////////////////////////////
//static int CompFileByBit(const char *strSrcFile,const char *strTargFile)
//{
//    FILE *fpOld;
//    FILE *fpNew;
//    char strOldBuff[8192];
//    char strNewBuff[8192];
//    struct stat status;
//    off_t  nFileSize=0;

//    memset(&status,0,sizeof(struct stat));
//    if(stat(strSrcFile,&status)==-1)
//        return -1;

//    nFileSize=status.st_size;

//    memset(&status,0,sizeof(struct stat));
//    if(stat(strTargFile,&status)==-1)
//        return 1;

//    if(nFileSize != status.st_size)
//        return 1;

//    if((fpOld=fopen(strSrcFile,"r"))==NULL)
//        return -1;
//    if((fpNew=fopen(strTargFile,"r"))==NULL)
//        return -1;

//    nFileSize=0;
//    while((nFileSize=fread(strOldBuff,1,sizeof(strOldBuff),fpOld))>0)
//    {
//        if(fread(strNewBuff,1,nFileSize,fpNew)!=(size_t)nFileSize)
//        {
//            fclose(fpOld);
//            fclose(fpNew);
//            return -1;
//        }
//        if(memcmp(strOldBuff,strNewBuff,nFileSize)==0)
//        {
//            memset(strOldBuff,0,sizeof(strOldBuff));
//            memset(strNewBuff,0,sizeof(strNewBuff));
//            nFileSize=0;
//            continue;
//        }
//        else
//        {
//            fclose(fpOld);
//            fclose(fpNew);
//            return 1;
//        }
//    }
//  fclose(fpOld);
//  fclose(fpNew);

//  return 0;
//}

//////////////////////////////////////////////////////////////////////////////////
////	函数名称：static int GetFileType(const char *strFile)
////	实现功能：获取指定的路径类型
////	对全局变量的影响：
////	参数说明：
////		strSrcFile :源路径名
////	返回结果说明：-1：失败 1：目录 2：文件 3：链接
//////////////////////////////////////////////////////////////////////////////////

//static int PartNameAndPath(const char *strFullFile,char *strName,char *strPath)
//{
//    char *szTmp;
//    int  nPos;

//    szTmp=(char *)strrchr(strFullFile,'/');

//    if(szTmp)
//    {
//        nPos=szTmp-strFullFile;
//        if(strName!=NULL)
//            strcpy(strName,szTmp+1);
//        //strFullPath[nPos]='\0';
//        if(strPath!=NULL)
//        {
//            strncpy(strPath,strFullFile,nPos);
//            strPath[nPos]='\0';
//        }
//    }
//    else
//    {
//        if(strName!=NULL)
//            strcpy(strName,strFullFile);
//        if(strPath!=NULL)
//            strcpy(strPath,"./");
//    }
//    return 0;
//}

//////////////////////////////////////////////////////////////////////////////////
////	函数名称：int main(int argc,char *argv[] )
////	实现功能：主函数
////	对全局变量的影响：无
////	参数说明：
////	返回结果说明：0 成功 -1 失败
//////////////////////////////////////////////////////////////////////////////////
//int main(int argc,char *argv[])
//{
//    if(argc==3 && strcmp(argv[1],"-l")==0)
//    {
//        showverinfo(argv[2]);
//        return 0;
//    }

//    string strNewVersion,strOldVersion,strUploadDir;

//    char szMd5File[256];
//    int nRet;

//    // 删除重建upload
//    strUploadDir=AUTOPACK_DIFF_DIRECTORY;
//    DeleteAllFile(strUploadDir.c_str(),RAV_DELETE_ALL);
//    if(mkdir(AUTOPACK_DIFF_DIRECTORY,S_IRWXU|S_IROTH|S_IRGRP|S_IXOTH|S_IXGRP))
//    {
//        perror("mkdir");
//        exit(1);
//    }

//    printf("START PACK RISING-ANTI-WARE FOR AUTOUPDATE\n");


//    std::string  cIniFIleName=AUTOPACK_INI_FILE;
//    try
//    {
//        CINIFile cIniInfo(cIniFIleName,true);

//        int  nSoftCount=-1;
//        if(!GetSoftInfomation(cIniInfo,strNewVersion,strOldVersion,nSoftCount))
//        {
//            printf("Get Version Information Faild\n");
//            return -1;
//        }
//        FILE *fp;
//        TOTMD5 stTotmd5; // 用于作md5签名

//        // 分配用于md5签名的数据
////		stTotmd5.stMd5=new MD5SIGNATURE [500];
//        stTotmd5.nTot=500;
//        stTotmd5.nCount=0;

//        // 逐个处理每个软件版本,生成包文件
//        for(int nI=0 ; nI< nSoftCount ; nI++)
//        {
//            if(!AutoPackUpdateSoft(cIniInfo,nI,strNewVersion,strOldVersion,stTotmd5))
//            {
//            //	delete []stTotmd5.stMd5;
//                printf("Auto Pack Soft Faild\n");
//                return -1;
//            }
//        }

//        //
//        //	记录md5
//        //
//        sprintf( szMd5File, "%s/%s",AUTOPACK_DIFF_DIRECTORY,AUTOPACK_MD5_FILE);
//        fp=fopen(szMd5File,"w");
//        if(!fp)
//        {
//            fprintf(stderr,"error open %s\n",szMd5File);
//        //	free(stTotmd5.stMd5);
//            exit(1);
//        }
//        std::vector<MD5SIGNATURE>::iterator pos;
//        for (pos = stTotmd5.stMd5.begin(); pos != stTotmd5.stMd5.end(); pos++)
//        {

//            fprintf(fp,"%s=%s\n",pos->strFileName, pos->szMd5Buf);
//        }
///*		for(int nI=0;nI<stTotmd5.nCount;nI++)
//        {
//            fprintf(fp,"%s=%s\n",stTotmd5.stMd5[nI].strFileName,stTotmd5.stMd5[nI].szMd5Buf);

//        }
//*/
//        fclose(fp);
////		delete [] stTotmd5.stMd5;

//        // 压缩所有打包好的文件,排除了versionnew.list

//        if(ZipAllFile()==-1)
//        {
//            printf("Zip All Files Faild\n");
//            return -1;
//        }

//    //
//    // 对upload 目录的所有文件生成dos格式的文本签名文件
//    //
//    //  首先将 UPTOHIGH 拷贝到目录下
//        {
//            rename("cdrom/uptohigh","upload/uptohigh");
//        }
//        DIR *stDir;
//        struct dirent *stDirRead,*stResult;
//        FILE *fpMd5;
//        char  pBuffer[512];
//        char DirBuf[2048];
//        stDirRead=(struct dirent *)DirBuf;
//        CMD5Data  cCheckFile;
//        char szMd5Buf[250];

//        sprintf(szMd5File,"%s/%s", AUTOPACK_DIFF_DIRECTORY,AUTOPACK_CENTER_MD5_FILE);
//        fpMd5=fopen(szMd5File,"w");
//        if(!fpMd5)
//        {
//            fprintf(stderr,"Open %s error\n",szMd5File);
//            return -1;
//        }

//        printf("Begine create md5 sinature file for Agent:\n");
//        stDir=opendir(AUTOPACK_DIFF_DIRECTORY);
//        if(stDir==NULL)
//        {
//            printf("open directory %s error\n",AUTOPACK_DIFF_DIRECTORY);
//            perror("Open error:\n");
//            return -1;
//        }

//        while((readdir_r(stDir,stDirRead,&stResult))==0 && stResult!=NULL)
//        {
//            if(!strcmp(stDirRead->d_name,".")
//                ||!strcmp(stDirRead->d_name,"..")
//                ||!strcmp(stDirRead->d_name, AUTOPACK_CENTER_MD5_FILE))
//                continue;
//            sprintf(szMd5File,"%s/%s",AUTOPACK_DIFF_DIRECTORY,stDirRead->d_name);
//            cCheckFile.MD5File(szMd5File,szMd5Buf);
//            sprintf(pBuffer,"%s=%s",stDirRead->d_name,szMd5Buf);
//            nRet=strlen(pBuffer);
//            pBuffer[nRet]=0x0d;
//            pBuffer[nRet+1]=0x0a;
//            pBuffer[nRet+2]=0;
//            fprintf(fpMd5,"%s",pBuffer);
//        }
//        closedir(stDir);
//        fclose(fpMd5);


//        printf("PACK RISING-ANTI-WARE FOR AUTOUPDATE OK\n");
//        return 0;

//    }
//    catch(CINIFile::CIniFileException &cExpect)
//    {
//        printf("PACK RISING-ANTI-WARE FOR AUTOUPDATE Failed\n");
//        return -1;
//    }
//}


///////////////////////////////////////////////////////////////////////////////////
////   函数名： bool AutoPackUpdateSoft(UNIXINIFILE::CIniFile &cIniInfo,int nSoftNum,
////			const string &strNewVersion,
////			const string &strOldVersion,TOTMD5 &);
////
////   用途：  针对每个产品，产生升级包
////   对全局变量的影响：
////   参数说明 ：
////		cIniInfo ：ini 分析类
////		nSoftNum : 当前的产品号-1
////		strNewVersion : 新版本
////		strOldVersion : 老版本
////   返回结果 ： 0 --成功，-1 失败
///////////////////////////////////////////////////////////////////////////

//bool AutoPackUpdateSoft(CINIFile &cIniInfo,int nSoftNum,const std::string &strNewVersion,
//    const std::string &strOldVersion,
//    TOTMD5 &stTotmd5)
//{
//    PACKONEVERSION PackOneVersion;
//    static bool bHasGeneral=false;

//    printf("START COMP SOFT %02d Difference\n",nSoftNum+1);

//    memset(&PackOneVersion,0,sizeof(PACKONEVERSION));
//    strcpy(PackOneVersion.strNewVersion,strNewVersion.c_str());
//    strcpy(PackOneVersion.strOldVersion,strOldVersion.c_str());
//    PackOneVersion.nVersionNum=nSoftNum+1;


//// 提取该版本软件名称

//    char szSection[256];
//    string strFilter;
//    sprintf(szSection,"SOFT%02d",nSoftNum+1);
//    if(!cIniInfo.readitemstr(szSection,"NAME",strFilter))
//    {
//        printf("Can't get %s NAME\n",szSection);
//    }
//    else
//        strcpy(PackOneVersion.strVersionName,strFilter.c_str());


//// 提取该版本软件路径

//    sprintf(szSection,"SOFT%02d",nSoftNum+1);
//    if(!cIniInfo.readitemstr(szSection,"PATH",strFilter))
//        printf("Can't get %s NAME\n",szSection);
//    else
//        strcpy(PackOneVersion.strVersionPath,strFilter.c_str());

//    // 比较新旧版本差异
//    char strNewFile[256];
//    memset(strNewFile,0,sizeof(strNewFile));

//    bool bSkip=false;
//    if(PackOneVersion.strVersionPath[0]=='\0' || strcmp(PackOneVersion.strVersionPath,".")==0 ||
//         strcmp(PackOneVersion.strVersionPath,"./")==0)
//    {
//        if(bHasGeneral)
//        {
//            printf(" upload.ini has too many path is ./\n");
//            return false;
//            bHasGeneral=true;
//        }

//        bSkip=true;
//        strcpy(PackOneVersion.strVersionPath,"./");
//        sprintf(strNewFile,"%s",strNewVersion.c_str());
//    }
//    else
//        sprintf(strNewFile,"%s/%s",strNewVersion.c_str(),PackOneVersion.strVersionPath);

//    if(bSkip)
//    {
//        if(CompAllFilesOther(cIniInfo,PackOneVersion,strNewFile,stTotmd5)==-1)
//        {
//            printf("Comp The Old And New Version Dirfference Faild\n");
//            return false;
//        }
//    }
//    else
//    {
//        if(CompAllFiles(cIniInfo,PackOneVersion,strNewFile,stTotmd5)==-1)
//        {
//            printf("Comp The Old And New Version Dirfference Faild\n");
//            return false;
//        }
//    }

//// 生成该新版本下载信息
//    if(TotalOneVersion(PackOneVersion)==-1)
//    {
//        printf("===%d\n", PackOneVersion.nFileCount);
//        return false;
//    }

//    return true;
//}



///////////////////////////////////////////////////////////////////////
////   函数名： bool GetSoftInfomation(UNIXINIFILE::CIniFile &cIniInfo,string &strNewVersion ,string &strOldVersion ,int &nSoftCount)
////
////   用途：  从配置包中提取新旧版本和产品数
////   对全局变量的影响：
////   参数说明 ：
////			cIniInfo : ini 文件分析类
////			strNewVersion : 新版本
////			strOldVersion : 老版本
////			nSoftNum : 返回当前的产品数
////   返回结果 ：true  --成功，false 失败
///////////////////////////////////////////////////////////////////////////
//bool  GetSoftInfomation(CINIFile &cIniInfo,std::string &strNewVersion ,
//    std::string &strOldVersion ,int &nSoftCount)
//{

//// 读取软件分类数目
//    long lTmpVal;
//    std::string strFilter;
//    if(!cIniInfo.readitemlong("SOFTINFO","SOFTCOUNT",lTmpVal,-1) || lTmpVal<0)
//    {
//        printf("Get Soft number Fiaild\n");;
//        return false;
//    }
//    nSoftCount=lTmpVal;


//// 提取新版本号
//    if(!cIniInfo.readitemstr("SOFTINFO","SOFTNEWVERSION",strFilter))
//    {
//        printf("Get Soft new version Fiaild\n");;
//        return false;
//    }
//    strNewVersion=strFilter;



//// 提取老本号

//    if(!cIniInfo.readitemstr("SOFTINFO","SOFTOLDVERSION",strFilter))
//    {
//        printf("No Old Version Fiaild\n");
//    }
//    strOldVersion=strFilter;
//    return true;
//}

/////////////////////////////////////////////////////////////////////////////////
////   函数名： int CompAllFilesOther(PACKONEVERSION *pPackOneVersion,char *strFullFile,
////		TOTMD5 *pstTotmd5)
////
////   用途：  对当前版本路径下除了ravnet 和ravsingle打包
////   对全局变量的影响：
////   参数说明 ： nSoftNum : 返回当前的产品数
////				strNewVersion : 新版本
////				strOldVersion : 老版本
////   返回结果 ： 0 --成功，-1 失败
///////////////////////////////////////////////////////////////////////////////

//int CompAllFilesOther(CINIFile &cIniFile,PACKONEVERSION &PackOneVersion,
//    char *strFullFile,TOTMD5 &stTotmd5)
//{
//    DIR *pDir;
//    struct dirent *stDirRead;
//    char strTempFile[256];

//    printf("Comp File %s \n",strFullFile);

//    pDir=opendir(strFullFile);
//    while((stDirRead=readdir(pDir))!=NULL)
//    {
//        if(  strcmp(stDirRead->d_name,".")==0  ||strcmp(stDirRead->d_name,"..")==0)
//            continue;


////		if(strcmp(stDirRead->d_name,"version.ini")==0) continue;
//        if(strcmp(stDirRead->d_name,"version.ini")==0)
//        {
//            if(strstr(strFullFile,"ravnet") || strstr(strFullFile,"ravsingle"))
//                continue;
//        }

////		if(strcmp(stDirRead->d_name,"version.ini")==0) continue;
//        if(strcmp(stDirRead->d_name,"ravnet")==0) continue;
//        if(strcmp(stDirRead->d_name,"ravsingle")==0) continue;
//        memset(strTempFile,0,sizeof(strTempFile));

//        sprintf(strTempFile,"%s/%s",strFullFile,stDirRead->d_name);
//        if(CompAllFiles(cIniFile,PackOneVersion,strTempFile,stTotmd5)==-1) return -1;
//    }
//    closedir(pDir);

//  return 0;
//}


/////////////////////////////////////////////////////////////////////////////////
////   函数名： int CompAllFiles(PACKONEVERSION *pPackOneVersion,char *strFullFile,
////		TOTMD5 *pstTotmd5)
////
////   用途：  从配置包中提取新旧版本和产品数
////   对全局变量的影响：
////   参数说明 ： nSoftNum : 返回当前的产品数
////				strNewVersion : 新版本
////				strOldVersion : 老版本
////   返回结果 ： 0 --成功，-1 失败
///////////////////////////////////////////////////////////////////////////////

//int CompAllFiles(CINIFile &cIniFile,PACKONEVERSION &PackOneVersion,char *strFullFile,TOTMD5 &stTotmd5)
//{
//    DIR *pDir;
//    struct dirent *stDirRead;
//    char strTempFile[256];
//    char strFullOldFile[256];
//    char *pChar;
//    int  nResult=-1;
//    CMD5Data  cCheckFile;

//    printf("Comp File %s \n",strFullFile);

//    switch(GetFileType(strFullFile))
//    {
//        case 1:
//            pDir=opendir(strFullFile);
//            while((stDirRead=readdir(pDir))!=NULL)
//            {
//                if(  strcmp(stDirRead->d_name,".")==0  ||strcmp(stDirRead->d_name,"..")==0)
//                    continue;
//                if(strcmp(stDirRead->d_name,"version.ini")==0)
//                {
//                    if(strstr(strFullFile,"ravnet")|| strstr(strFullFile,"ravsingle"))
//                        continue;
//                }
//                memset(strTempFile,0,sizeof(strTempFile));
//                sprintf(strTempFile,"%s/%s",strFullFile,stDirRead->d_name);
//                if(CompAllFiles(cIniFile,PackOneVersion,strTempFile,stTotmd5)==-1) return -1;
//            }
//            closedir(pDir);
//            break;
//        case 2:
//            memset(strFullOldFile,0,sizeof(strFullOldFile));
//            pChar=strchr(strFullFile,'/');
//            sprintf(strFullOldFile,"%s%s",PackOneVersion.strOldVersion,pChar);
//            nResult=CompFileByBit(strFullFile,strFullOldFile);
//            if(nResult==-1)
//            {
//                printf("Comp %s and %s diffence Faild\n",strFullFile,strFullOldFile);
//                return -1;
//            }
//            else
//            {
//                // 计算md5 签名
//                MD5SIGNATURE stFileFlag;
//                cCheckFile.MD5File(strFullFile,stFileFlag.szMd5Buf);
////				cCheckFile.MD5File(strFullFile,stTotmd5.stMd5[stTotmd5.nCount].szMd5Buf);
//                pChar=strchr(strFullFile,'/');
//                pChar++;
//                sprintf(stFileFlag.strFileName,"%s",pChar);
//            //	sprintf(stTotmd5.stMd5[stTotmd5.nCount].strFileName,"%s",pChar);
//                stTotmd5.stMd5.push_back(stFileFlag);
//                stTotmd5.nCount++;

//                if(nResult==0)
//                {
//                    if(AutoPackFile(cIniFile,strFullFile,-1,PackOneVersion)==-1)
//                    {
//                      printf("AutoPack FALSE File %s Faild\n",strFullFile);
//                      return -1;
//                    }
//                }
//                else // 对原来不存在的文件升级
//                {
//                    if(AutoPackFile(cIniFile,strFullFile,0,PackOneVersion)==-1)
//                    {
//                        printf("AutoPack TRUE File %s Faild\n",strFullFile);
//                        return -1;
//                    }
//                }
//            }

//            break;
//        default:
//            return -1;
//    }

//  return 0;
//}

//int UpgradeFilesVersion(CINIFile &cIniInfo,const FILEHEAD &stFileHead,const PACKONEVERSION &PackOneVersion)
//{
//    char szSection[256];
//    sprintf(szSection,"SOFT%02d",PackOneVersion.nVersionNum);
//    cIniInfo.writeitemstr(szSection,stFileHead.strFileName,stFileHead.strVersion);
//    return 0;
//}

//int AutoPackFile(CINIFile &cIniFile,const char *strFile,int nNewFileFlag,PACKONEVERSION &PackOneVersion)
//{
//    FILEHEAD FileHead;
//    struct stat status;

//    memset(&FileHead,0,sizeof(FILEHEAD));

//    if(strcmp(PackOneVersion.strVersionPath,"./")==0)
//    {
//        strcpy(FileHead.strFileName,strFile+strlen(PackOneVersion.strNewVersion)+1);
//    }
//    else
//    {
//        strcpy(FileHead.strFileName,strFile+strlen(PackOneVersion.strNewVersion)
//            +strlen(PackOneVersion.strVersionPath)+2);
//    }

//    if(nNewFileFlag==0)
//    {
//        strcpy(FileHead.strVersion,PackOneVersion.strNewVersion);
//        if(UpgradeFilesVersion(cIniFile,FileHead,PackOneVersion)==-1)
//            return -1;
//    }
//    else
//    {
//        if(GetFileOldVersion(cIniFile,PackOneVersion,FileHead)==-1)
//        {
//            printf("Get Old File %s Version Faild\n",FileHead.strFileName);
//            return -1;
//        }
//    }
//    if(stat(strFile,&status)==-1)
//        return -1;

//    FileHead.nSize=status.st_size;
//    if(AutoPackCopyFile(PackOneVersion,FileHead,strFile)==-1)
//    {
//        printf("Copy File %s Faild\n",FileHead.strFileName);
//        return -1;
//    }
//    PackOneVersion.nFileCount+=1;

//    return 0;
//}

//int TotalOneVersion(const PACKONEVERSION &PackOneVersion)
//{
//    UPLISTVERSION  stSoftWare;
//    UPFILEVERSION stFileInfo;
//    FILEHEAD FileHead;
//    char strListFile[MIN_BUFF];
//    char strNewPath[MIN_BUFF];
//    char strTempFile[MIN_BUFF];
//    char strDownName[MIN_BUFF];
//    int  nFileCount=0;
//    FILE *fpList;
//    DIR *pDir;
//    struct dirent *stDirRead;


//// 生成文件头
//    memset(&stSoftWare,0,sizeof(UPLISTVERSION));
//    strcpy(stSoftWare.strVersionName,PackOneVersion.strVersionName);

//    strcpy(stSoftWare.strVersion,PackOneVersion.strNewVersion);

////  文件最后更新日期

//    time_t curtime=time(0);
//    struct tm *sttm=localtime(&curtime);
//    sprintf(&stSoftWare.strVersion[40],"%4d-%02d-%02d",1900+sttm->tm_year,sttm->tm_mon+1,
//        sttm->tm_mday);

//    stSoftWare.nFileNumber=PackOneVersion.nFileCount;
//    stSoftWare.nSize=stSoftWare.nFileNumber*sizeof(UPFILEVERSION);

//    memset(strListFile,0,sizeof(strListFile));
//    sprintf(strListFile,"%s/%s",AUTOPACK_DIFF_DIRECTORY,AUTOUPDATE_LIST_FILE);
//    if((fpList=fopen(strListFile,"ab+"))==NULL)
//        return -1;
//    if(fwrite(&stSoftWare,1,sizeof(UPLISTVERSION),fpList)!=sizeof(UPLISTVERSION))
//        return -1;
//    memset(strNewPath,0,sizeof(strNewPath));
//    strcpy(strNewPath,AUTOPACK_DIFF_DIRECTORY);

//    pDir=opendir(strNewPath);
//    while((stDirRead=readdir(pDir))!=NULL)
//    {
//        if(  strcmp(stDirRead->d_name,".")==0
//            ||strcmp(stDirRead->d_name,"..")==0
//            ||strcmp(stDirRead->d_name,"versionnew.list")==0)
//            continue;

//            sprintf(strTempFile,"%s/%s",strNewPath,stDirRead->d_name);

//        //寻找适合的文件
//        if(strstr(strTempFile,PackOneVersion.strVersionName)!=NULL)
//        {

//        //	 写入文件下载信息
//            memset(&stFileInfo,0,sizeof(UPFILEVERSION));
//            memset(&FileHead,0,sizeof(FILEHEAD));
//            if(GetFileInstallName(strTempFile,FileHead)==-1)
//                return -1;
//            strcpy(stFileInfo.strFileName,FileHead.strFileName);
//            strcpy(stFileInfo.strFileVersion,FileHead.strVersion);
//            memset(strDownName,0,sizeof(strDownName));
//            if(PartNameAndPath(strTempFile,strDownName,NULL)==-1)
//                return -1;

//            strcat(strDownName,".zip");
//            strcpy(stFileInfo.strDownLoadName,strDownName);
//            if(fwrite(&stFileInfo,1,sizeof(UPFILEVERSION),fpList)!=sizeof(UPFILEVERSION))
//                return -1;
//            nFileCount++;
//        }
//    }
//    closedir(pDir);

//    fclose(fpList);

//// 两次文件数不符
//    if(nFileCount!=PackOneVersion.nFileCount)
//        return -1;

//    return 0;
//}


//int GetFileOldVersion(CINIFile &cIniInfo,const PACKONEVERSION &PackOneVersion,FILEHEAD &stFileHead)
//{

//// 提取旧版本号
//    string strFilter;
//    char szSection[256];
//    sprintf(szSection,"SOFT%02d",PackOneVersion.nVersionNum);
//    if(cIniInfo.readitemstr(szSection,stFileHead.strFileName,strFilter))
//    {
//        strcpy(stFileHead.strVersion,strFilter.c_str());
//        return 0;
//    }
//    return -1;

//}

//int AutoPackCopyFile(PACKONEVERSION &PackOneVersion,FILEHEAD &stFileHead,const char *strFile)
//{
//    char strTargetFile[256];
//    char *pChar;
//    FILE *fpSrc;
//    FILE *fpTarg;

//    memset(strTargetFile,0,sizeof(strTargetFile));
//    pChar=(char *)strrchr(strFile,'/');
//    if(pChar==NULL)
//        return -1;

//    sprintf(strTargetFile,"%s/%s.%s",AUTOPACK_DIFF_DIRECTORY,pChar+1,PackOneVersion.strVersionName);

//    if((fpSrc=fopen(strFile,"rb+"))==NULL)
//    {
//        printf("Open File %s Failed\n",strFile);
//        return -1;
//    }
//    if((fpTarg=fopen(strTargetFile,"wb+"))==NULL)
//    {
//        printf("Open File %s Failed",strTargetFile);
//        return -1;
//    }

//// 先写入文件头
//    if(fwrite(&stFileHead,1,sizeof(FILEHEAD),fpTarg)!=sizeof(FILEHEAD))
//        return -1;


//// 拷贝数据
//    char Buff[8192];
//    int  nReturnSize;

//    memset(Buff,0,sizeof(Buff));
//    while((nReturnSize=fread(Buff,1,sizeof(Buff),fpSrc))>0)
//    {
//        if(fwrite(Buff,1,nReturnSize,fpTarg)!=(size_t)nReturnSize)
//        {
//            fclose(fpSrc);
//            return -1;
//        }
//        memset(Buff,0,sizeof(Buff));
//    }
//    fclose(fpSrc);
//    fclose(fpTarg);

//    return 0;
//}

//int GetFileInstallName(const char *strFile,FILEHEAD &stFileHead)
//{
//    FILE *fp;
//    memset(&stFileHead,0,sizeof(FILEHEAD));
//    if((fp=fopen(strFile,"rb+"))==NULL)
//    {
//        return -1;
//    }
//    if(fread(&stFileHead,1,sizeof(FILEHEAD),fp)!=sizeof(FILEHEAD))
//    {
//        fclose(fp);
//        return -1;
//    }

//    fclose(fp);
//    return 0;
//}

//int ZipAllFile()
//{
//    DIR *pDir;
//    struct dirent *stDirRead;
//    char strDirectory[MIN_BUFF];
//    char strTempFile[MIN_BUFF];
//    char strTempZipFile[MIN_BUFF];
//    UnZip *DealZipFile=NULL;
//    DealZipFile = new UnZip;


//    if(mkdir(AUTOPACK_DIFF_TEMP_DIRECTORY,0777)==-1)
//    {
//        perror("mkdir temp dir");
//        return -1;
//    }

//    sprintf(strTempFile,"%s/%s",AUTOPACK_DIFF_DIRECTORY,AUTOUPDATE_LIST_FILE);
//    sprintf(strTempZipFile,"%s/%s",AUTOPACK_DIFF_TEMP_DIRECTORY,AUTOUPDATE_LIST_FILE);

//    if(rename(strTempFile,strTempZipFile)==-1)
//    {
//        printf("Move List File Failed\n");
//        return -1;
//    }

//    strcpy(strDirectory,AUTOPACK_DIFF_DIRECTORY);

//    pDir=opendir(strDirectory);
//    while((stDirRead=readdir(pDir))!=NULL)
//    {
//        if(  strcmp(stDirRead->d_name,".")==0
//            ||strcmp(stDirRead->d_name,"..")==0
//            ||strcmp(stDirRead->d_name,AUTOUPDATE_LIST_FILE)==0)
//        continue;
//        sprintf(strTempFile,"%s/%s",strDirectory,stDirRead->d_name);
//        sprintf(strTempZipFile,"%s/%s.zip",AUTOPACK_DIFF_TEMP_DIRECTORY,stDirRead->d_name);

//// 压缩文件

//        if(DealZipFile->CreateNewZip(strTempZipFile,strTempFile)==-1)
//        {
//            printf(" zip file %s failed\n",strTempFile);
//            return -1;
//        }
//        unlink(strTempFile);
//    }
//    closedir(pDir);

//    delete DealZipFile;



//    if(rmdir(AUTOPACK_DIFF_DIRECTORY)==-1)
//    {
//        printf("rm dir failed");
//        return -1;
//    }
//    if(rename(AUTOPACK_DIFF_TEMP_DIRECTORY,AUTOPACK_DIFF_DIRECTORY)==-1)
//    {
//        printf("move Faield\n");
//        return -1;
//    }

//    return 0;
//}

//void showverinfo(const char *szVername)
//{
//    FILE *fpList=fopen(szVername,"r");
//    if(!fpList)
//    {
//        printf("Open %s failed!\n",szVername);
//        return ;
//    }

//    UPLISTVERSION stSoftWare;
//    UPFILEVERSION *pstFileInfo=NULL;

//    memset(&stSoftWare,0,sizeof(UPLISTVERSION));
//    while(fread(&stSoftWare,sizeof(UPLISTVERSION),1,fpList)==1)
//    {
//        printf("\n=== softname %s version=%s last update=%s nFile=%d\n",stSoftWare.strVersionName,
//                stSoftWare.strVersion,&stSoftWare.strVersion[40],stSoftWare.nFileNumber);

//        pstFileInfo=new UPFILEVERSION [stSoftWare.nFileNumber];

//        if(fread(pstFileInfo,stSoftWare.nFileNumber*sizeof(UPFILEVERSION),1,fpList)!=1)
//        {
//            printf("fread list file information failed\n");
//            fclose(fpList);
//            return ;
//        }
//        for(int nI=0 ; nI< stSoftWare.nFileNumber ; nI ++)
//        {
//            printf("%s %s %s\n",pstFileInfo[nI].strFileName,   pstFileInfo[nI].strFileVersion,
//                pstFileInfo[nI].strDownLoadName);
//        }

//        delete [] pstFileInfo;

//    }


//    fclose(fpList);
//}

int main() {
    return 0;
}
