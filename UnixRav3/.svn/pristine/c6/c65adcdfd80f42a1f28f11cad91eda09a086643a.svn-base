#include "ravquafile.h"
#include "ravlockfile.h"
#include <public/rame.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <sys/time.h>

RavQuaFile* RavQuaFile::NEW(const string &strQuaPath, long ulMaxSpace, int nMaxBackupSize, int nReplaceMethod) {
    RavQuaFile *self = new RavQuaFile;
    if (self) {
        if (!self->Init(strQuaPath, ulMaxSpace, nMaxBackupSize, nReplaceMethod)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

RavQuaFile::RavQuaFile()
{
    m_nMaxBackupSize=-1;
}


RavQuaFile::~RavQuaFile()
{

}


bool RavQuaFile::Init(const string &strQuaPath,long ulMaxSpace,int nMaxBackupSize,int nReplaceMethod) {
    m_strQuaPath=strQuaPath;

    m_pstIndexData=0;
    m_nMaxSpace=ulMaxSpace*1024*1024;
    m_nReplaceMethod=nReplaceMethod;
    m_nMaxBackupSize=nMaxBackupSize*1024*1024;

    if (access(strQuaPath.c_str(), F_OK))
    {
        mkdir(strQuaPath.c_str(),S_IRWXU);
    }

    struct stat stBuf;

    if(stat(strQuaPath.c_str(),&stBuf))
        return false;

    if(!(S_ISDIR(stBuf.st_mode)))
        return false;

    return true;

}


bool RavQuaFile::IsValidIndex(const STRAVQUAINDEXHEAD &stIndexHead,long nFileSize)
{
    if(memcmp(stIndexHead.m_uMagic,RAVQUAINDEXMAGIC,12))
        return false;

    long nLen=stIndexHead.m_nItem*sizeof(STRAVQUAINDEXDATA)+sizeof(STRAVQUAINDEXHEAD);
    if(nLen!=nFileSize)
        return false;

    return true;
}

bool RavQuaFile::cleanspace(long curSize)
{
    int nI;
    char szFileName[256];
    for(nI=0;nI<m_stIndexHead.m_nItem;nI++)
    {
        m_stIndexHead.m_nUseSpace-=m_pstIndexData[nI].m_nUseSpace;

        sprintf(szFileName,"%s/%s",m_strQuaPath.c_str(),m_pstIndexData[nI].m_szQuaFile);
        unlink(szFileName);
        m_pstIndexData[nI].m_ulQuaTime=RAVQUA_INVALID_INDEX;
        if( (m_stIndexHead.m_nUseSpace+curSize+sizeof(RAVQUAFILEHEAD))<m_nMaxSpace)
            break;
    }

    if(nI>=m_stIndexHead.m_nItem)
    {
        delete [] m_pstIndexData;
        m_pstIndexData=NULL;
    }
    return true;

}

int RavQuaFile::ReadRavQuaIndex()
{
    int nRet=0;

    char szIndexName[256];
    sprintf(szIndexName,"%s/%s",m_strQuaPath.c_str(),RAVQUAINDEXNAME);

    struct stat stBuf;
    FILE *fp;

    if(stat(szIndexName,&stBuf)==0)
    {
        if(!(S_ISREG(stBuf.st_mode)))
        {
            nRet=RAVQUA_ERR_INDEX_INVALID;
            goto errorreturn;
        }


        fp=fopen(szIndexName,"rb");
        if(!fp)
        {
            nRet=RAVQUA_ERR_INDEX_OPEN;
            goto errorreturn;

        }

        if(fread(&m_stIndexHead,sizeof(STRAVQUAINDEXHEAD),1,fp)!=1 || !IsValidIndex(m_stIndexHead,stBuf.st_size))
        {
            nRet=RAVQUA_ERR_INDEX_INVALID;
            goto errorexit;
        }

        m_pstIndexData=new STRAVQUAINDEXDATA[m_stIndexHead.m_nItem];
        if(!m_pstIndexData)
        {
            nRet=RAVQUA_ERR_ALLOC;
            goto  errorexit;
        }

        if(fread(m_pstIndexData,sizeof(STRAVQUAINDEXDATA),m_stIndexHead.m_nItem,fp)!= (size_t)(m_stIndexHead.m_nItem) )
        {
            nRet=RAVQUA_ERR_INDEX_READ;
            goto  errorexit;
        }
        fclose(fp);
    }
    else
    {
        memset(&m_stIndexHead,0,sizeof(STRAVQUAINDEXHEAD));
        memcpy(m_stIndexHead.m_uMagic,RAVQUAINDEXMAGIC,12);
    }


    return 0;


errorexit:
    fclose(fp);
errorreturn:
    return nRet;

}

int RavQuaFile::RestoreFile(int nMode,const string &strSrcFileName,const string *pstrDstFile,bool bLock,int perm, int gid, int uid)
{

    int nI,nFind=-1;
    int nRet=0;
    struct stat stBuf;

    string strAbsSrcPath=m_strQuaPath;

    strAbsSrcPath.append("/");
    strAbsSrcPath.append(strSrcFileName);

    if (access(strAbsSrcPath.c_str(), F_OK))
        return RAVQUA_ERR_SRCFAILED;


    if(stat(strAbsSrcPath.c_str(),&stBuf))
        return RAVQUA_ERR_SRCFAILED;

    if(bLock && !RavQuaLockFile())
    {
        return RAVQUA_ERR_LOCK;
    }

    if(!m_pstIndexData)
    {
        nRet=ReadRavQuaIndex();
        if(nRet<0)
            goto errorreturn;
    }

    if(!m_pstIndexData)
    {
        if(bLock)
            RavQuaUnLockFile();
        return RAVQUA_ERR_INDEX_OPEN;
    }

    for(nI=0;nI<m_stIndexHead.m_nItem;nI++)
    {
        if(strcmp(m_pstIndexData[nI].m_szQuaFile,strSrcFileName.c_str())==0)
        {
            nFind=nI;
            if(nMode==RAVQUA_ACTION_RESTOREDEL)
                m_pstIndexData[nI].m_ulQuaTime=RAVQUA_INVALID_INDEX;
            break;
        }
    }

    if(nFind==-1) {
        if(bLock)
            RavQuaUnLockFile();
        return RAVQUA_ERR_NOINDEX;
    }

    nRet=CopyRestoreFile(strAbsSrcPath,pstrDstFile, perm, gid, uid);
    if(nRet<0)
        goto errorreturn;

    if(nMode==RAVQUA_ACTION_RESTOREDEL )
    {
        if(bLock)
            WriteRavQuaIndex();
        unlink(strAbsSrcPath.c_str());
    }

    if(bLock)
        RavQuaUnLockFile();
    return 0;

errorreturn:
    if(bLock)
        RavQuaUnLockFile();
    return nRet;
}

int RavQuaFile::DeleteFile(const string &strSrcFileName,bool bLock)
{
    int nI,nFind=-1;
    int nRet=0;
    struct stat stBuf;

    string strAbsSrcPath=m_strQuaPath;

    strAbsSrcPath.append("/");
    strAbsSrcPath.append(strSrcFileName);

    if (access(strAbsSrcPath.c_str(), F_OK))
        return RAVQUA_ERR_SRCFAILED;


    if(stat(strAbsSrcPath.c_str(),&stBuf))
        return RAVQUA_ERR_SRCFAILED;

    if(bLock && !RavQuaLockFile())
    {
        return RAVQUA_ERR_LOCK;
    }

    if(!m_pstIndexData)
    {
        nRet=ReadRavQuaIndex();
        if(nRet<0)
            goto errorreturn;
    }

    if(!m_pstIndexData)
    {
        if(bLock)
            RavQuaUnLockFile();
        return RAVQUA_ERR_INDEX_OPEN;
    }

    for(nI=0;nI<m_stIndexHead.m_nItem;nI++)
    {
        if(strcmp(m_pstIndexData[nI].m_szQuaFile,strSrcFileName.c_str())==0)
        {
            nFind=nI;
            m_pstIndexData[nI].m_ulQuaTime=RAVQUA_INVALID_INDEX;
            break;
        }
    }

    if(nFind==-1)
    {
        if(bLock)
            RavQuaUnLockFile();
        return RAVQUA_ERR_NOINDEX;
    }


    if(bLock)
        WriteRavQuaIndex();

    unlink(strAbsSrcPath.c_str());

    if(bLock)
        RavQuaUnLockFile();
    return 0;

errorreturn:
    if(bLock)
        RavQuaUnLockFile();
    return nRet;

}


int RavQuaFile::BackUpFile(const string &strSrcFileName,const string& strVirusName,string &strDstFileName)
{
    printf("BackUpFile:%s, %s\n", strSrcFileName.c_str(), strVirusName.c_str());

    if (access(strSrcFileName.c_str(), F_OK))
        return 0;

    int nRet=0,curFileSize;
    struct stat stBuf;
    char szIndexName[20];

    if(stat(strSrcFileName.c_str(),&stBuf))
        return RAVQUA_ERR_SRCFAILED;

    if(!(S_ISREG(stBuf.st_mode)))
        return RAVQUA_ERR_SRCFAILED;

    if(stBuf.st_size==0)
        return RAVQUA_ERR_SRCFAILED;

    curFileSize=stBuf.st_size;
    if(curFileSize>m_nMaxBackupSize)
        return RAVQUA_ERR_SRCLARGE;


    if(!RavQuaLockFile())
    {
        return RAVQUA_ERR_LOCK;
    }

    if(!m_pstIndexData)
    {
        nRet=ReadRavQuaIndex();
        if(nRet<0)
            goto errorreturn;
    }

    if(m_nReplaceMethod==RAVQUA_REPLACE_OLD && m_pstIndexData)
    {
        if((curFileSize+m_stIndexHead.m_nUseSpace +sizeof(RAVQUAFILEHEAD))>m_nMaxSpace)
            cleanspace(curFileSize);
    }

    sprintf(szIndexName,"%08X",m_stIndexHead.m_nNextIndex);
    strDstFileName=m_strQuaPath;
    strDstFileName.append("/");
    strDstFileName.append(szIndexName);

    nRet=CopyBackupFile(strSrcFileName,strDstFileName,strVirusName);
    if(nRet<0)
        goto errorreturn;

    strDstFileName=szIndexName;

    m_stIndexHead.m_nUseSpace+=(m_stIndexHead.m_nUseSpace +sizeof(RAVQUAFILEHEAD));

    STRAVQUAINDEXDATA m_curStData;
    memset(&m_curStData,0,sizeof(STRAVQUAINDEXDATA));

    m_curStData.m_ulQuaTime=time(0);
    strcpy(m_curStData.m_szQuaFile,szIndexName);
    m_curStData.m_nUseSpace=nRet;

    m_stIndexHead.m_nNextIndex++;

    if(m_stIndexHead.m_nNextIndex==RAVQUA_MAX_INDEX)
        m_stIndexHead.m_nNextIndex=0;

    WriteRavQuaIndex(&m_curStData);

    RavQuaUnLockFile();
    return 0;


errorreturn:
    RavQuaUnLockFile();
    return nRet;


}

int RavQuaFile::CopyRestoreFile(const string &strSrcFileName,const string  *pstrDstFileName, int perm, int gid, int uid)
{
    FILE	*fp, *fo;
    char	*pBuffer=NULL;
    int  ret,nI;
    int nRetValue=0;
    int nReadLen=65536;
    RAVQUAFILEHEAD *pStHead=NULL;
    string strdstFile;
    struct stat stBuf;

    if((fp = fopen(strSrcFileName.c_str(), "rb"))==NULL)
    {
        return RAVQUA_ERR_SRCFAILED;
    }

    stat(strSrcFileName.c_str(), &stBuf);

    pBuffer=new char [nReadLen];

    if(!pBuffer)
    {
        fclose(fp);
        return RAVQUA_ERR_ALLOC;
    }

    pStHead=(RAVQUAFILEHEAD *)pBuffer;

    ret = fread(pStHead,1,nReadLen,fp);
    if(ret<(int)sizeof(RAVQUAFILEHEAD) || ret<pStHead->m_nHeadLen || (pStHead->m_nHeadLen+pStHead->m_FileLen)!=(unsigned long)stBuf.st_size)
    {
        fclose(fp);
        return RAVQUA_INVALID_FILE;
    }

    if(memcmp(pStHead->m_uMagic,RAVQUAMAGIC,strlen(RAVQUAMAGIC))!=0)
    {
        fclose(fp);
        return RAVQUA_INVALID_FILE;
    }

    if(!pstrDstFileName)
    {
        char *szBackName=(char *)pStHead->m_szBackupName;
        char szPath[1024]={0};

        if(strstr(szBackName,">>"))
        {
            char *szTmp2=strstr(szBackName,">>");
            int nLen=szTmp2-szBackName;
            for(int j=nLen-1;j>=0;j--)
            {
                if(szBackName[j]=='/')
                {
                    strncpy(szPath,szBackName,j+1);
                    szPath[j+1]='\0';
                    break;
                }
            }
            if(szPath[0]=='\0')
                strcpy(szPath,"/");

            char *szTmp1=strrchr(szTmp2,'/');
            char *szName;
            if(szTmp1)
            {
                szTmp1++;
                szName=szTmp1;
            }
            else
            {
                szTmp2++;
                szTmp2++;
                szName=szTmp2;
            }
            strdstFile=szPath;
            strdstFile+=szName;

        }
        else
        {
            char *szTmp1=strrchr(szBackName,'/');
            int nLen=szTmp1-szBackName+1;
            strncpy(szPath,szBackName,nLen);
            szPath[nLen]='\0';
            strdstFile=szBackName;
        }

        char szCmd[256];
        sprintf(szCmd,"/bin/mkdir -p \"%s\"",szPath);
        system(szCmd);

    }
    else
        strdstFile=*pstrDstFileName;


    fseek(fp, pStHead->m_nHeadLen, SEEK_SET);

    if((fo = fopen(strdstFile.c_str(), "wb"))==NULL)
    {
        fclose(fp);
        return RAVQUA_ERR_DSTFAILED;
    }



    while ((ret = fread(pBuffer,1,nReadLen,fp)) > 0)
    {
        for (nI= 0;nI < ret;nI++)
            pBuffer[nI] = RAVQUA_OPERVALUE^pBuffer[nI];
        if (fwrite(pBuffer, 1 , ret, fo) < (size_t)ret)
            goto errorexit;
    }

    fclose(fo);
    fclose(fp);
    delete [] pBuffer;

    if (perm >= 0) {
        chmod(strdstFile.c_str(), perm);
    }
    if (uid>=0 && gid >= 0) {
        chown(strdstFile.c_str(), uid, gid);
    }

    return nRetValue;

errorexit:
    fclose(fo);
    fclose(fp);
    delete [] pBuffer;
    unlink(strdstFile.c_str());
    return RAVQUA_ERR_COPYFILE;

}
bool RavQuaFile::WriteRavQuaIndex(const STRAVQUAINDEXDATA *pCurItem)
{
    char szIndexName[256],szTmpFile[256];
    sprintf(szIndexName,"%s/%s",m_strQuaPath.c_str(),RAVQUAINDEXNAME);
    sprintf(szTmpFile,"%s/%s.tmp",m_strQuaPath.c_str(),RAVQUAINDEXNAME);

    FILE *fp;
    int nI;
    int nItem,nOldItem;


    nItem=0;
    nOldItem=m_stIndexHead.m_nItem;

    m_stIndexHead.m_nUseSpace=0;

    if(m_pstIndexData)
    {
        for(nI=0;nI<nOldItem;nI++)
        {
            if(m_pstIndexData[nI].m_ulQuaTime!=RAVQUA_INVALID_INDEX)
            {
                nItem++;
                m_stIndexHead.m_nUseSpace+=m_pstIndexData[nI].m_nUseSpace;
            }
        }
        if(nItem==0)
        {
            delete [] m_pstIndexData;
            m_pstIndexData=NULL;
            nOldItem=0;
        }

    }

    if(pCurItem)
    {
            m_stIndexHead.m_nUseSpace+=pCurItem->m_nUseSpace;

        nItem++;
    }


    if(nItem==0)
    {
        unlink(szIndexName);
        return true;
    }

    fp=fopen(szTmpFile,"w");
    if(!fp)
        return false;

    m_stIndexHead.m_nItem=nItem;

    if(fwrite(&m_stIndexHead,sizeof(STRAVQUAINDEXHEAD),1,fp)!=1)
        goto errorexit;

    if(m_pstIndexData  && nOldItem>0)
    {
        for(nI=0;nI<nOldItem;nI++)
        {
            if(m_pstIndexData[nI].m_ulQuaTime==RAVQUA_INVALID_INDEX)
                continue;

            if(fwrite(&m_pstIndexData[nI],sizeof(STRAVQUAINDEXDATA),1,fp)!=1)
                goto errorexit;
        }
    }

    if(pCurItem)
    {
        if(fwrite(pCurItem,sizeof(STRAVQUAINDEXDATA),1,fp)!=1)
            goto errorexit;
    }

    fclose(fp);
    fflush(NULL);
    unlink(szIndexName);
    rename(szTmpFile,szIndexName);

    if(m_pstIndexData)
    {
        delete [] m_pstIndexData;
        m_pstIndexData=NULL;
    }

    return true;

errorexit:
    if(m_pstIndexData)
    {
        delete [] m_pstIndexData;
        m_pstIndexData=NULL;
    }
    fclose(fp);
    unlink(szTmpFile);
    return false;

}

bool RavQuaFile::RavQuaLockFile()
{
    char szLockFile[256];
    sprintf(szLockFile,"%s/ravqua.lock",m_strQuaPath.c_str());

    m_nLockfd=ravlockfile(szLockFile,true);
    return true;
}

bool RavQuaFile::RavQuaUnLockFile()
{
    ravunlockfile(m_nLockfd);
    return true;
}

int RavQuaFile::SaveIndex()
{
    WriteRavQuaIndex();
    return 0;
}

int RavQuaFile::CopyBackupFile(const string &strSrcFileName,const string &strDstFileName,const string &strVirusName)
{
    FILE	*fp, *fo;
    char	*pBuffer=NULL;
    int  ret,nI;
    int nRetValue=0;
    int nReadLen=65536;
    RAVQUAFILEHEAD *pStHead=NULL;

    struct stat stBuf;
    if((fp = fopen(strSrcFileName.c_str(), "rb"))==NULL)
    {
        return RAVQUA_ERR_SRCFAILED;
    }

    if((fo = fopen(strDstFileName.c_str(), "wb"))==NULL)
    {
        fclose(fp);
        return RAVQUA_ERR_DSTFAILED;
    }

    stat(strSrcFileName.c_str(), &stBuf);

    pBuffer=new char [nReadLen];

    if(!pBuffer)
    {
        fclose(fp);
        fclose(fo);
        return RAVQUA_ERR_ALLOC;
    }

    pStHead=(RAVQUAFILEHEAD *)pBuffer;
    int nLen=sizeof(RAVQUAFILEHEAD);;
    nLen+=strSrcFileName.length();
    nLen+=strVirusName.length();
    nLen=nLen-4+2;
    nLen=(nLen/4+1)*4;
    memset(pStHead,0,nLen);

    pStHead->m_nHeadLen=nLen;
    nRetValue=nLen+stBuf.st_size;

    nLen=strlen(RAVQUAMAGIC);
    memcpy(pStHead->m_uMagic,RAVQUAMAGIC,nLen);
    pStHead->m_ulQuaTime=time(0);
    pStHead->m_FileLen=stBuf.st_size;
    pStHead->m_nOrigNameLen=strSrcFileName.length();

    char *szTmp=(char *)pStHead->m_szBackupName;
    strcpy(szTmp,strSrcFileName.c_str());
    szTmp+=(pStHead->m_nOrigNameLen+1);
    strcpy(szTmp,strVirusName.c_str());
    if(fwrite(pStHead, pStHead->m_nHeadLen, 1, fo)!=1)
        goto errorexit;

    while ((ret = fread(pBuffer,1,nReadLen,fp)) > 0)
    {
        for (nI= 0;nI < ret;nI++)
            pBuffer[nI] = RAVQUA_OPERVALUE^pBuffer[nI];
        if (fwrite(pBuffer, 1 , ret, fo) < (size_t)ret)
            goto errorexit;
    }

    fclose(fo);
    fclose(fp);
    delete [] pBuffer;


    return nRetValue;

errorexit:
    fclose(fo);
    fclose(fp);
    delete [] pBuffer;
    unlink(strDstFileName.c_str());
    return RAVQUA_ERR_COPYFILE;
}

int RavQuaFile::GetIndexCount()
{
    int nRet;
    if(!m_pstIndexData)
    {
        nRet=ReadRavQuaIndex();
        if(nRet<0)
            return nRet;
    }
    return m_stIndexHead.m_nItem;
}

bool RavQuaFile::GetIndexItem(int nIndex,RAVQUAFILEINFO &stFileInfo)
{

    if(!m_pstIndexData)
        return false;

    if(nIndex>=m_stIndexHead.m_nItem)
        return false;

    memset(&stFileInfo,0,sizeof(RAVQUAFILEINFO));

    struct stat stBuf;
    string strAbsSrcPath=m_strQuaPath;
    strAbsSrcPath.append("/");
    strAbsSrcPath.append(m_pstIndexData[nIndex].m_szQuaFile);

    if (access(strAbsSrcPath.c_str(), F_OK))
        return false;

    if(stat(strAbsSrcPath.c_str(),&stBuf))
        return false;


    FILE	*fp;
    if((fp = fopen(strAbsSrcPath.c_str(), "rb"))==NULL)
        return false;

    char szBuf[4096];
    int nReadLen=4096;
    RAVQUAFILEHEAD *pFileHead=(RAVQUAFILEHEAD *)szBuf;

    int ret = fread(pFileHead,1,nReadLen,fp);
    fclose(fp);
    if(ret<(int)sizeof(RAVQUAFILEHEAD) || ret<pFileHead->m_nHeadLen || (pFileHead->m_nHeadLen+pFileHead->m_FileLen)!=(unsigned long)stBuf.st_size)
        return false;

    if(memcmp(pFileHead->m_uMagic,RAVQUAMAGIC,strlen(RAVQUAMAGIC))!=0)
        return false;


    char *szTmp=pFileHead->m_szBackupName;
    char *szTmp1=strrchr(szTmp,'/');

    if(szTmp1)
    {

        int nLen=szTmp1-szTmp;
        strncpy(stFileInfo.szPath,szTmp,nLen);
        szTmp1++;
        strcpy(stFileInfo.szName,szTmp1);
    }
    else
    {
        strcpy(stFileInfo.szPath,szTmp);
        strcpy(stFileInfo.szName,szTmp);

    }
    time_t ttime=pFileHead->m_ulQuaTime;
    struct tm *stTime=localtime(&ttime);

    sprintf(stFileInfo.szTime,"%4d-%02d-%02d %02d:%02d:%02d",stTime->tm_year+1900,
        stTime->tm_mon+1,stTime->tm_mday,stTime->tm_hour,stTime->tm_min,stTime->tm_sec);

    szTmp+=(pFileHead->m_nOrigNameLen+1);
    strcpy(stFileInfo.szVirusName,szTmp);

    stFileInfo.nfileSize=pFileHead->m_FileLen;

    char *szRem;
    stFileInfo.nIndex=strtol(m_pstIndexData[nIndex].m_szQuaFile,&szRem,16);

    szTmp+=(strlen(szTmp) + 1);
    int nPos = szTmp - (char *)pFileHead;
    if (pFileHead->m_nHeadLen - nPos > 4)
        stFileInfo.nRiskType = atol(szTmp);
    return true;
}

int RavQuaFile::RestroreOrDeleteFile(int nMode,const string *pRestoreFile,int nItem,bool bLock)
{

    int nRet=0;
    int nI,nJ;
    string strAbsSrcPath;

    if(bLock && !RavQuaLockFile())
    {
        return RAVQUA_ERR_LOCK;
    }

    if(m_pstIndexData)
        delete [] m_pstIndexData;

    m_pstIndexData=NULL;
    nRet=ReadRavQuaIndex();
    if(nRet<0)
        goto errorexit;

    strAbsSrcPath=m_strQuaPath;

    for(nI=0;nI<nItem;nI++)
    {
        strAbsSrcPath=m_strQuaPath;
        strAbsSrcPath.append("/");
        strAbsSrcPath.append(pRestoreFile[nI]);

        if(m_pstIndexData)
        {

            for(nJ=0;nJ<m_stIndexHead.m_nItem;nJ++)
            {
                if(strcmp(m_pstIndexData[nJ].m_szQuaFile,pRestoreFile[nI].c_str())==0)
                {
                    m_pstIndexData[nJ].m_ulQuaTime=RAVQUA_INVALID_INDEX;
                }
            }
        }

        if(nMode==RAVQUA_ACTION_RESTOREDEL)
            nRet=CopyRestoreFile(strAbsSrcPath, NULL, -1, -1, -1);

        unlink(strAbsSrcPath.c_str());
    }

    if(m_pstIndexData)
        WriteRavQuaIndex();

    delete [] m_pstIndexData;
    m_pstIndexData=NULL;

errorexit:
    if(bLock)
        RavQuaUnLockFile();

    return nRet;
}

void RavQuaFile::emptyqua()
{
    RavQuaLockFile();

    DIR *pDir;
    struct dirent	*stDirRead;
    char strTempFile[256];
    char szBaseDir[256];

    sprintf(szBaseDir,"%s/",m_strQuaPath.c_str());
    pDir=opendir(szBaseDir);
    while((stDirRead=readdir(pDir))!=NULL)
    {
        if(  strcmp(stDirRead->d_name,".")==0
            ||strcmp(stDirRead->d_name,"..")==0 || strcmp(stDirRead->d_name,LOCKFILENAME)==0)
            continue;
        sprintf(strTempFile,"%s%s",szBaseDir,stDirRead->d_name);
        unlink(strTempFile);
    }
    closedir(pDir);

    RavQuaUnLockFile();

}

int RavQuaFile::CopyBackupFileFromFileObject(const string &strSrcFileName,
        const string  &strDstFileName,const string& strVirusName,IREStream *pstm, BACAKUPFILECONTENT *pstBackupFileContent)
{
    FILE	 *fo;
#if 1
    char	*pBuffer=NULL;
    int  nI;
    int nRetValue=0;
    int nReadLen=65536;
    long curFileSize;
    RAVQUAFILEHEAD *pStHead=NULL;


// Žò¿ªÄ¿±êÎÄŒþ
    if((fo = fopen(strDstFileName.c_str(), "wb"))==NULL)
    {
        return RAVQUA_ERR_DSTFAILED;
    }

    curFileSize = CFileAccessor(pstm).GetSize();

    pBuffer=new char [nReadLen];
    if(!pBuffer)
    {
        fclose(fo);
        return RAVQUA_ERR_ALLOC;
    }

    pStHead=(RAVQUAFILEHEAD *)pBuffer;
    int nLen=sizeof(RAVQUAFILEHEAD);;
    nLen+=strSrcFileName.length();
    nLen+=strVirusName.length();
    // add liht by 2011-01-12
    nLen+=4;
    nLen=nLen-4+3;
//	nLen=(nLen/4+1)*4;
    memset(pStHead,0,nLen);

    pStHead->m_nHeadLen=nLen;
    nRetValue=nLen+curFileSize;

    nLen=strlen(RAVQUAMAGIC);
    memcpy(pStHead->m_uMagic,RAVQUAMAGIC,nLen);
    pStHead->m_ulQuaTime=time(0);
    pStHead->m_FileLen=curFileSize;
    pStHead->m_nOrigNameLen=strSrcFileName.length();

    char *szTmp=(char *)pStHead->m_szBackupName;
    strcpy(szTmp,strSrcFileName.c_str());
    szTmp+=(pStHead->m_nOrigNameLen+1);
    strcpy(szTmp,strVirusName.c_str());
    // add liht by 2011-01-12
    szTmp+=(strlen(strVirusName.c_str())+1);
    sprintf(szTmp, "%04d", pstBackupFileContent->nRiskType);

    if(fwrite(pStHead, pStHead->m_nHeadLen, 1, fo)!=1)
        goto errorexit;

// ÎÄŒþ¶ÔÏó¶šÎ»
    CFileAccessor(pstm).Seek(0);

// ŒÓÃÜžôÀë
    DWORD nRead;
#if 1
    while ((pstm->Read(pBuffer,nReadLen,&nRead)) ==0 && nRead>0)
    {
        for (nI= 0;nI < (int)nRead;nI++)
            pBuffer[nI] = RAVQUA_OPERVALUE^pBuffer[nI];
        if (fwrite(pBuffer, 1 , nRead, fo) < nRead)
            goto errorexit;
        nRead=nReadLen;
    }
#endif
    fclose(fo);
    delete [] pBuffer;


    return nRetValue;

errorexit:
    fclose(fo);
    delete [] pBuffer;
    unlink(strDstFileName.c_str());

    return RAVQUA_ERR_COPYFILE;

}

int RavQuaFile::backupFileFromFileObject(const string &strSrcFileName,const string& strVirusName,
    string &strDstFileName	,IREStream *pstm, BACAKUPFILECONTENT *pstBackupFileContent)
{

#if 1
    if(!pstm)
        return 0;

    unsigned long nRet=0,curFileSize=0;
    char szIndexName[20];


    curFileSize = CFileAccessor(pstm).GetSize();

    if(curFileSize==0 || curFileSize>=m_nMaxBackupSize)
        return RAVQUA_ERR_SRCLARGE;


    if(!RavQuaLockFile())
    {
        return RAVQUA_ERR_LOCK;
    }

    if(!m_pstIndexData)
    {
        nRet=ReadRavQuaIndex();
        if(nRet<0)
            goto errorreturn;
    }


    if(m_nReplaceMethod==RAVQUA_REPLACE_OLD && m_pstIndexData)
    {
        if((curFileSize+m_stIndexHead.m_nUseSpace +sizeof(RAVQUAFILEHEAD))>m_nMaxSpace)
            cleanspace(curFileSize);
    }

    sprintf(szIndexName,"%08X",m_stIndexHead.m_nNextIndex);
    strDstFileName=m_strQuaPath;
    strDstFileName.append("/");
    strDstFileName.append(szIndexName);

    nRet=CopyBackupFileFromFileObject(strSrcFileName,strDstFileName,strVirusName,pstm, pstBackupFileContent);
    if(nRet<0)
        goto errorreturn;

    strDstFileName=szIndexName;

    m_stIndexHead.m_nUseSpace+=(m_stIndexHead.m_nUseSpace +sizeof(RAVQUAFILEHEAD));

    STRAVQUAINDEXDATA m_curStData;
    memset(&m_curStData,0,sizeof(STRAVQUAINDEXDATA));

    m_curStData.m_ulQuaTime=time(0);
    strcpy(m_curStData.m_szQuaFile,szIndexName);
    m_curStData.m_nUseSpace=nRet;

    m_stIndexHead.m_nNextIndex++;

    if(m_stIndexHead.m_nNextIndex==RAVQUA_MAX_INDEX)
        m_stIndexHead.m_nNextIndex=0;

    WriteRavQuaIndex(&m_curStData);

    RavQuaUnLockFile();
    return 0;

errorreturn:
    RavQuaUnLockFile();

    return nRet;
#endif
return 0;
}

int RavQuaFile::CopyRestoreFileToFileObject(const string &strSrcFileName,IREStream *pstm)
{
#if 1
    FILE	*fp;
    char	*pBuffer=NULL;
    int  ret,nI;
    int nRetValue=0;
    int nReadLen=65536;
    RAVQUAFILEHEAD *pStHead=NULL;
    string strdstFile;
    struct stat stBuf;

    if((fp = fopen(strSrcFileName.c_str(), "rb"))==NULL)
    {
        return RAVQUA_ERR_SRCFAILED;
    }

    stat(strSrcFileName.c_str(), &stBuf);

    pBuffer=new char [nReadLen];

    if(!pBuffer)
    {
        fclose(fp);
        return RAVQUA_ERR_ALLOC;
    }

    pStHead=(RAVQUAFILEHEAD *)pBuffer;


    ret = fread(pStHead,1,nReadLen,fp);
    if(ret<(int)sizeof(RAVQUAFILEHEAD) || ret<pStHead->m_nHeadLen || (pStHead->m_nHeadLen+pStHead->m_FileLen)!=(unsigned long)stBuf.st_size)
    {
        fclose(fp);
        return RAVQUA_INVALID_FILE;
    }

    if(memcmp(pStHead->m_uMagic,RAVQUAMAGIC,strlen(RAVQUAMAGIC))!=0)
    {
        fclose(fp);
        return RAVQUA_INVALID_FILE;
    }

    fseek(fp, pStHead->m_nHeadLen, SEEK_SET);

    CFileAccessor(pstm).Seek(0);
    CFileAccessor(pstm).SetSize(pStHead->m_FileLen);

    DWORD nWrite;

    while ((ret = fread(pBuffer,1,nReadLen,fp)) > 0)
    {
        for (nI= 0;nI < ret;nI++)
            pBuffer[nI] = RAVQUA_OPERVALUE^pBuffer[nI];
        if (pstm->Write(pBuffer, ret ,&nWrite) !=0)
        {
            fclose(fp);
            return -1;
        }

            goto errorexit;
    }

    fclose(fp);
    delete [] pBuffer;


    return nRetValue;

errorexit:
    fclose(fp);
    delete [] pBuffer;
#endif
    return RAVQUA_ERR_COPYFILE;

}

int RavQuaFile::RestoreFileToFileObject(const string &strSrcFileName,IREStream *pstm)
{
    int nI,nFind=-1;
    int nRet=0;
#if 1

    struct stat stBuf;

    string strAbsSrcPath=m_strQuaPath;

    strAbsSrcPath.append("/");
    strAbsSrcPath.append(strSrcFileName);

    if (access(strAbsSrcPath.c_str(), F_OK))
        return RAVQUA_ERR_SRCFAILED;


    if(stat(strAbsSrcPath.c_str(),&stBuf))
        return RAVQUA_ERR_SRCFAILED;

    if(!RavQuaLockFile())
    {
        return RAVQUA_ERR_LOCK;
    }

    if(!m_pstIndexData)
    {
        nRet=ReadRavQuaIndex();
        if(nRet<0)
            goto errorreturn;
    }

    if(!m_pstIndexData)
    {
        RavQuaUnLockFile();
        return RAVQUA_ERR_INDEX_OPEN;
    }

    for(nI=0;nI<m_stIndexHead.m_nItem;nI++)
    {
        if(strcmp(m_pstIndexData[nI].m_szQuaFile,strSrcFileName.c_str())==0)
        {
            nFind=nI;
            m_pstIndexData[nI].m_ulQuaTime=RAVQUA_INVALID_INDEX;
            break;
        }
    }

    if(nFind==-1)
    {
        RavQuaUnLockFile();
        return RAVQUA_ERR_NOINDEX;
    }

    nRet=CopyRestoreFileToFileObject(strAbsSrcPath,pstm);
    if(nRet<0)
        goto errorreturn;


    WriteRavQuaIndex();
    unlink(strAbsSrcPath.c_str());

    RavQuaUnLockFile();
    return 0;

errorreturn:
    RavQuaUnLockFile();
#endif
    return nRet;
}

#endif

#ifdef __MAIN_TEST__
int main(int argc,char *argv[])
{
    int nMode=0;
    if(argc>=2)
    {
        if(strcmp(argv[1],"-a")==0)
            nMode=0;
        if(strcmp(argv[1],"-r")==0)
            nMode=1;

        if(strcmp(argv[1],"-d")==0)
            nMode=2;
    }

    RavQuaFile cRavBackup;
//	string strPath="/home/wxwidget/ravunix/virbak";
    string strPath="/home/wxwidget/Rising/Rav/virbak";
    long maxSpace=100;
    int nMaxBackupSize=1;
    int nReplaceMethod=RAVQUA_REPLACE_OLD;

    if(!cRavBackup.Init(strPath,maxSpace,nMaxBackupSize,nReplaceMethod))
    {
        printf("Init failed\n");
        return -1;
    }

    int nRet;
    nRet=cRavBackup.GetIndexCount();
    printf("count=%d\n",nRet);
    RAVQUAFILEINFO stInfo;
    cRavBackup.GetIndexItem(10,stInfo);
    // test backup
    char szBuf[20];
    if(nMode==0)
    {
        string strbackname;
        for(int nI=0;nI<20;nI++)
        {
            sprintf(szBuf,"vir%02d",nI+1);
            if(nI%2)
            {
                nRet=cRavBackup.BackUpFile("/home/wxwidget/unixgroup/wxrav2007/ravunix/pubsrc/aba",szBuf,strbackname);
            }
            else
            {
                nRet=cRavBackup.BackUpFile("/home/wxwidget/unixgroup/wxrav2007/ravunix/pubsrc/aaa",szBuf,strbackname);
            }

            printf("backup nRet=%d %s\n",nRet,strbackname.c_str());
        }
    }

    // test restore

    if(nMode==1)
    {

        nRet=cRavBackup.RestoreFile(RAVQUA_ACTION_RESTOREDEL,argv[2]);
        printf("restore nRet=%d\n",nRet);
    }


    // test delete
    if(nMode==2)
    {
        char szBuf[256];
        cRavBackup.RavQuaLockFile();
        for(int nI=0;nI<20000;nI+=2)
        {
            sprintf(szBuf,"%08X",nI);
            nRet=cRavBackup.DeleteFile(szBuf,false);
        }
        cRavBackup.SaveIndex();
        cRavBackup.RavQuaUnLockFile();
    }

}
#endif

