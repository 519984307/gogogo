#ifndef RAVQUAFILE_H
#define RAVQUAFILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

//
#include "ravquadefined.h"

using namespace std;

class IREStream;

class RavQuaFile
{
public:
    static RavQuaFile* NEW(const string &strQuaPath,long ulMaxSpace=0,int nMaxBackupSize=0, int nReplaceMethod=RAVQUA_REPLACE_OLD);
    ~RavQuaFile();

private:
    RavQuaFile();

private:
    int ReadRavQuaIndex();
    bool WriteRavQuaIndex(const STRAVQUAINDEXDATA *pCurItem=NULL);

    bool IsValidIndex(const STRAVQUAINDEXHEAD &stIndexHead,long nFileSize);

    bool cleanspace(long curSize);

    int CopyBackupFile(const string &strSrcFileName, const string  &pstrDstFileName,const string& strVirusName);
    int CopyRestoreFile(const string &strSrcFileName, const string  *pstrDstFileName, int perm, int gid, int uid);

public:
    bool Init(const string &strQuaPath,long ulMaxSpace=0,int nMaxBackupSize=0, int nReplaceMethod=RAVQUA_REPLACE_OLD);

    bool RavQuaLockFile();
    bool RavQuaUnLockFile();

    int BackUpFile(const string &strSrcFileName,const string& strVirusName,string &strDstFileName);
    int RestoreFile(int nMode,const string &strSrcFileName,const string *pstrDstFile=NULL,bool bLock=true, int perm=-1, int gid=-1, int uid=-1);
    int RestroreOrDeleteFile(int nMode,const string *pRestoreFile,int nItem,bool bLock=true);

    int DeleteFile(const string &strSrcFileName,bool bLock=true);
    void emptyqua();

    int  GetIndexCount();
    bool GetIndexItem(int nI,RAVQUAFILEINFO &stFileInfo);
    int SaveIndex();


private:
    STRAVQUAINDEXHEAD m_stIndexHead;
    STRAVQUAINDEXDATA *m_pstIndexData;

    long long m_nMaxSpace;

    string m_strQuaPath;
    int m_nReplaceMethod;
    long long m_nMaxBackupSize;

    int m_nLockfd;

private:
    int CopyBackupFileFromFileObject(const string &strSrcFileName,
        const string  &strDstFileName,const string& strVirusName,IREStream *pstm, BACAKUPFILECONTENT *pstBackupFileContent);

    int CopyRestoreFileToFileObject(const string &strSrcFileName,IREStream *pstm);


public:
    int backupFileFromFileObject(const string &strSrcFileName,const string& strVirusName,string &strDstFileName
            ,IREStream *pstm, BACAKUPFILECONTENT *pstBackupFileContent);
    int RestoreFileToFileObject(const string &strSrcFileName,IREStream *pstm);

};

#endif // RAVQUAFILE_H
