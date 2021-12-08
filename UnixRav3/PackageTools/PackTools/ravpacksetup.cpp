#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "md5.h"
#include "unixzip.h"


extern int errno;

#define  SETUP_FILE                      "ravsetup.pck"
#define SIGNATURE                       "RISING-UNIX-VERSION-INSTALL"
#define RAVNETWRD_REGISTER_FILE         "/etc/.RavCfg"

const char *g_szLib[] = {};

struct SETUPTOC
{
    char strFile[256];	//文件名
    uint32_t  nSize;			// 大小
    uint16_t mode;		// 打包文件的属性
    char szOrigMd5[40]; //打包文件的原始签名
    char szZipMd5[40];  // 打包文件压缩之后的签名
};

// 为了与老版本兼容，该结构只能在首部增加成员
struct SETUPTAIL
{
    char strVersion[64];        // 当前版本
    char strPackedtime[20];     // 打包时间

    char strNetRegFile[256];	//网络版注册文件名
    char strSingleRegFile[256];	//单机版注册文件名
    char strSignature[64];		//rising安装包标志字符串
    uint32_t  nSize;					// 包的 大小
    uint32_t  nFileNum;				// 包中的文件数目
    uint32_t  nTOCOffSet;			// 有效文件开始位置（除掉第一个用于解压的文件）
};


int PackDirectory(FILE *fp,const char *strFile, SETUPTAIL *pSetupTail,const char *szInstPath);
int PackSetupTail(FILE *fp, SETUPTAIL *pSetupTail);
int PackFile(FILE *fp, const char *strFile, SETUPTAIL *pSetupTail, const char *szInstPath);


std::string GetFileMD5(const std::string& path) {
    unsigned char data[1024];
    FILE *file = fopen(path.c_str(), "rb");
    if (NULL == file) {
        return "";
    }

    MD5 md5;

    while (1) {
        int readed = fread(data, 1, 1024, file);
        if (readed <= 0) {
            fclose(file);
            break;
        }

        md5.update(data, readed);
    }

    md5.finalize();
    return md5.hexdigest();
}

static bool checkpath(const char *szOrgPath)
{
    // 检查几个关键路径是否存在
    char const *szCheckPath[] =
    {
        "bin","lib",NULL
    };
    char szDstPath[256];
    struct stat stBuf;

    int nI=0;

    while(szCheckPath[nI])
    {
        sprintf(szDstPath,"%s/%s",szOrgPath,szCheckPath[nI]);
        if(stat(szDstPath,&stBuf))
            return false;
        if(!S_ISDIR(stBuf.st_mode))
        {
            return false;
        }
        nI++;
    }

    return true;
}

static int GetFileType(const char *strFile)
{
    struct stat stBuf;
    if(stat(strFile,&stBuf)==-1)
        return -1;
    if(S_ISDIR(stBuf.st_mode))
        return 1;
    else if(S_ISREG(stBuf.st_mode))
        return 2;
    else if(S_ISLNK(stBuf.st_mode))
        return 3;
    return -1;
}



///////////////////////////////////////////////////////////////////////////////////
//  函数名：  int main(int argc,char *argv[])
//  用途：  主程序，生成安装包文件(包括手动升级包中间的部分也是这个包)
//  对全局变量的影响：
//  参数说明 ： 命令行参数,
//		argv[1] :要打包的路径
//		argv[2] :out file
//  返回结果 ： 0: ok -1 :fialed
//////////////////////////////////////////////////////////////////////////////////
int main(int argc,char *argv[])
{
    SETUPTAIL  SetupTail;
    char szPackPath[256];
    char szMsg[256];

    memset(szPackPath,0,256);
    if(argc!=3)
    {
        fprintf(stderr,"Usage %s packed-path out_file \n",argv[0]);
        return -1;
    }

    FILE *fp=fopen(argv[2], "w");
    if(!fp)
    {
        sprintf(szMsg,"Open file %s failed:%s",SETUP_FILE,strerror(errno));
        printf("%s\n",szMsg);
    }

    strcpy(szPackPath,argv[1]);

//	检查指定的路径是否正确
//    if(!checkpath(szPackPath))
//    {
//        fprintf(stderr,"%s is not a rising packet path\n",szPackPath);
//        return -1;
//    }

//增添产生文件出厂标签
    memset(&SetupTail,0,sizeof(SETUPTAIL));


// 将配置树文件单独打包
//    if(PackFile(fp,"RsConfig.cfg",&SetupTail,szPackPath))
//    {
//        printf("Can't Opend RsConfig.cfg\n");
//        return -1;
//    }


    // 将指定的目录打包
    if(PackDirectory(fp,szPackPath,&SetupTail,szPackPath))
    {
        printf("pack directory file failed\n");
        return -1;
    }

    strcpy(SetupTail.strVersion,argv[2]);
    time_t curTime=time(NULL);
    struct tm *stTime=localtime(&curTime);
    sprintf(SetupTail.strPackedtime,"%04d-%02d-%02d %02d:%02d",stTime->tm_year+1900,stTime->tm_mon+1,stTime->tm_mday,
        stTime->tm_hour,stTime->tm_min);


    if(PackSetupTail(fp,&SetupTail))
    {
        printf("pack setup tail failed\n");
        return -1;
    }
    fclose(fp);
    printf("pack selfextract setup file ok\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////
//  函数名: int PackDirectory(FILE *fpSetup,const char *strFile,
//		SETUPTAIL *pSetupTail,const char *szInstPath)

//  用途：  遍历目录，对目录下的文件打包
//  对全局变量的影响：无
//  参数说明 ： strFile : 需要打包的目录名
//				pstTail	：包文件尾部结构
//  返回结果 ： 0：ok -1:failed
////////////////////////////////////////////////////////////////
int PackDirectory(FILE *fpSetup,const char *strFile,SETUPTAIL *pSetupTail,
    const char *szInstPath)
{
    DIR *pDir;
    struct dirent *stDirRead;
    char strTempFile[256];

    switch(GetFileType(strFile))
    {
        case 1:
            pDir=opendir(strFile);
            while((stDirRead=readdir(pDir))!=NULL)
            {
                if(  strcmp(stDirRead->d_name,".")==0
                    ||strcmp(stDirRead->d_name,"..")==0)
                        continue;
                else
                {
                    sprintf(strTempFile,"%s",strFile);
                    if(strTempFile[strlen(strTempFile)-1]=='/')
                        strTempFile[strlen(strTempFile)-1]='\0';

                    strcat(strTempFile,"/");
                    strcat(strTempFile,stDirRead->d_name);
                    if(PackDirectory(fpSetup,strTempFile,pSetupTail,szInstPath))
                        return -1;
                }
            }
            closedir(pDir);

            break;
        case 2:
            {
                int nNeedPack=1;
                if(strstr(strFile,"/lib/"))
                {
                    // 安装需要使用的动态库不再打入包中
                    int nI=0;
                    while(g_szLib[nI])
                    {
                        if(strstr(strFile,g_szLib[nI]))
                        {
                            nNeedPack=0;
                            break;
                        }
                        nI++;
                    }
                }
                if(nNeedPack)
                {
                    if(PackFile(fpSetup,strFile,pSetupTail,szInstPath))
                        return -1;
                }
            }
            break;
        case 3:
            break;
        default :
            break;
    }

    return 0;
}


////////////////////////////////////////////////////////////////
//  函数名：  int PackFile(const char *strFile,SETUPTAIL *pSetupTail,char *strPackPath)
//  用途：  对文件打包，打包文件经过压缩
//  对全局变量的影响：SetupTail
//  参数说明 ：
//		strFile : 需要打包的文件名
//  返回结果 ：0:OK ,-1 :failed
////////////////////////////////////////////////////////////////

int PackFile(FILE *fpSetup, const char *strFile,SETUPTAIL *pSetupTail,
    const char *strPackPath)
{
    FILE *fpTemp;
    CUNIXZIP cZipFile;

    char szTmpFile[256];

    SETUPTOC SetupTOC;//纪录该文件的信息
    size_t  nReturn;
    struct stat status;
    printf("packing file %s\n",strFile);

    memset(&SetupTOC,0,sizeof(SetupTOC));

// 检查打包文件
    if(stat(strFile,&status))
    {
        printf("stat file %s failed\n",strFile);
        return -1;
    }

// 就算原始文件的签名
    strcpy(SetupTOC.szOrigMd5, GetFileMD5(strFile).c_str());

// 保存原始文件的属性
    SetupTOC.mode=status.st_mode;

//	压缩文件
    char *szOnlyname=(char *)strrchr(strFile,'/');
    if(szOnlyname)
    {
        szOnlyname++;
        strcpy(szTmpFile,szOnlyname);
    }
    else
        strcpy(szTmpFile,strFile);

    strcat(szTmpFile,".GZ");
    cZipFile.gz_init(strFile,szTmpFile,false,0);
    if(cZipFile.gz_compress())
    {
        fprintf(stderr,"compress %s failed\n",strFile);
        return -1;
    }
    if(stat(szTmpFile,&status))
    {
        printf("stat file %s failed\n",szTmpFile);
        return -1;
    }

//  为计算压缩后的文件签名作准备
    strcpy(SetupTOC.szZipMd5, GetFileMD5(szTmpFile).c_str());

    SetupTOC.nSize=status.st_size;
    nReturn=strlen(strPackPath);
    if(strPackPath[nReturn-1] != '/')
        nReturn += 1;
    //
    //  复制相对路径名（相对输入的需要打包的目录）
    //
//    if(strcmp(strFile,"RsConfig.cfg")==0)
//        strcpy(SetupTOC.strFile,strFile);
//    else
        strcpy(SetupTOC.strFile,strFile+nReturn);

    printf("pack path:%s name:%s size:%d SetupTOC.szOrigMd5:%s SetupTOC.szZipMd5:%s\n", strPackPath,SetupTOC.strFile,SetupTOC.nSize,SetupTOC.szZipMd5,SetupTOC.szZipMd5);

    if((fpTemp=fopen(szTmpFile,"rb"))==NULL)
    {
        printf("open file %s failed\n",strFile);
        return -1;
    }


  //  写入该文件的相关信息
    if(fwrite(&SetupTOC,sizeof(SETUPTOC),1,fpSetup)!=1)
    {
        fclose(fpTemp);
        fclose(fpSetup);
        unlink(szTmpFile);
        return -1;
    }
    //
    // 复制该文件
    //

    int nReadSize=65536;
    char *pBuff=new char[nReadSize];

    memset(pBuff,0,nReadSize);
    nReturn=0;
    while((nReturn=fread(pBuff,1,nReadSize,fpTemp))>0)
    {
        if(fwrite(pBuff,1,nReturn,fpSetup)!=nReturn)
        {
            delete []pBuff;
            fclose(fpTemp);
            fclose(fpSetup);
            unlink(szTmpFile);
            return -1;
        }
    }

    fflush(NULL);
    fclose(fpTemp);

    unlink(szTmpFile);
    pSetupTail->nSize+=status.st_size;
    pSetupTail->nFileNum+=1;
    delete [] pBuff;
    return 0;

}

////////////////////////////////////////////////////////////////
//  函数名：  int PackSetupTail(SETUPTAIL *pstTail)
//  用途：  在自解压文件的尾部纪录一些信息
//  对全局变量的影响：SetupTail
//  参数说明 ： pstTail : 尾部结构
//  返回结果 ： 0 :OK  -1 :failed
////////////////////////////////////////////////////////////////

int PackSetupTail(FILE *fpSetup,SETUPTAIL *pstTail)
{


    strcpy(pstTail->strSignature,SIGNATURE);
    strcpy(pstTail->strNetRegFile,RAVNETWRD_REGISTER_FILE);
    strcpy(pstTail->strSingleRegFile,RAVNETWRD_REGISTER_FILE);

    printf("packing setup tail ,please wait\n");
    printf("TOCoffset %d\n",pstTail->nTOCOffSet);

    if(fwrite(pstTail,sizeof(SETUPTAIL),1,fpSetup)!=1)
    {
        fclose(fpSetup);
        printf("write file failed\n");
        return -1;
    }

    return 0;
}

