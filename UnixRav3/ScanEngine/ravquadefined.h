#ifndef RAVQUADEFINED_H
#define RAVQUADEFINED_H

#ifdef __cplusplus
extern "C" {
#endif

#define RAVQUAINDEXNAME ".ravquaindex"
#define RAVQUAINDEXMAGIC "RAVQUAINX001"
#define RAVQUAMAGIC "RAVQUA01"

#define  LOCKFILENAME "ravqua.lock"
#define  RAVQUAINI "/sys/ravqua.ini"

#define RAVQUA_ACTION_BACKUP 0
#define RAVQUA_ACTION_RESTORE 1
#define RAVQUA_ACTION_RESTOREDEL 2
#define RAVQUA_ACTION_DELETE 3

#define RAVQUA_ERR_OK 0
#define RAVQUA_ERR_LOCK -1

#define RAVQUA_ERR_INDEX_OPEN -2
#define RAVQUA_ERR_INDEX_READ -3
#define RAVQUA_ERR_INDEX_INVALID -4
#define RAVQUA_ERR_NOINDEX -5

#define RAVQUA_ERR_SRCFAILED -6
#define RAVQUA_ERR_DSTFAILED -7
#define RAVQUA_ERR_SRCLARGE -8
#define RAVQUA_ERR_COPYFILE -9
#define RAVQUA_INVALID_FILE -10
#define RAVQUA_ERR_ALLOC -11

#define RAVQUA_MAX_INDEX 0xffffffff

#define RAVQUA_OPERVALUE 101

#define RAVQUA_REPLACE_OLD 0x01
#define RAVQUA_REPLACE_INCSPACE 0x02

#define RAVQUA_INVALID_INDEX 0xffffffff


struct STRAVQUAINDEXHEAD
{
    unsigned char m_uMagic[12];
    unsigned int m_nNextIndex;
    long m_nUseSpace;
    int m_nItem;
};

struct BACAKUPFILECONTENT
{
    std::string strSrcFileName;
    std::string strVirusName;
    std::string strDstFileName;
    int nRiskType;

};

struct STRAVQUAINDEXDATA
{
    unsigned long m_ulQuaTime;
    long m_nUseSpace ;
    char m_szQuaFile[12];

};

struct RAVQUAFILEHEAD
{
    unsigned char m_uMagic[8];

    int m_nHeadLen;
    unsigned long m_FileLen;
    int m_nOrigNameLen;

    unsigned long m_ulQuaTime;
    char m_szBackupName[4];
};


struct RAVQUAFILEINFO
{
    unsigned long nIndex;
    char szName[512];
    char szPath[512];
    char szTime[30];
    char szVirusName[256];
    unsigned long nfileSize;
    unsigned long nRiskType;
};

#ifdef __cplusplus
}
#endif

#endif // RAVQUADEFINED_H
