#ifndef __H_UPDATER__H__
#define __H_UPDATER__H__

#include "dlparser.h"
#include "httpdownloader.h"
#include "ravconsolesetup.h"
#include "cross.h"

#include <string>

using namespace std;

#define SERVER_SOC_FLAG_FILE            "/Api/Api.php"

#define TMP_BACKUP_FILES_PATH           "/tmp/ravcrossupdater/"
#define TMP_DOWNLOAD_FILES_PATH         "/tmp/ravcdownloader/"
#define OLD_VERSION_ISOLATION_ZONE_PATH "virbak"
#define NEW_VERSION_ISOLATION_ZONE_PATH "RAVRJJH/.quarzone"
#define LOG_XML_FILE_NAME               "log.xml"

#define DEFAULT_INSTALL_PATH            "/opt/BDFZ/RAV"
 

class Updater{
public:
    Updater();
    ~Updater();

    void SetTailInfo(int type, const string center_addr);
    int  InitInfo(bool is_secret_machine = false);

    int BackupData();
    int DownloadInstallFiles();
    int GetServerType();
    int UninstallOldFiles();
    int RestoreBackupedData();
    int InstallNewFiles();
    int BackupConfigFile();
    int RestoreConfigFile();
    int StartDaemons();

protected:
    int DownloadCommonAndParse();
    int CopyDir(const char* srcDir, const char* dstDir);

public:
    DlParser        mDlParser;
    HTTPDownloader  mDownloader;
    RavConsoleSetup mSetup;
    CrossVersion    mCVersion;

    string mVersion;
    string mVDBVersion;

    bool   mHaveTail;
    int    mServerType;
    string mTailCenterAddr;
};


#endif
