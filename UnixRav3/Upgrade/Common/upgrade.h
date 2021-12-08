#ifndef UPGRADE_H
#define UPGRADE_H

#include "engineobserver.h"
#include "updateconfig.h"

#include <pthread.h>
#include <string>
#include <map>
#include <vector>
#include <string.h>

class UpdateObserver;
class ScanEngine;
class EngineObserver;

class UpdateEngine
{
public:
    static UpdateEngine* NEW(const std::string& installdir, const std::string& configpath);
    virtual ~UpdateEngine();

    virtual int Start(const std::string& version);
    virtual int Stop();

    void SetObserver(UpdateObserver *observer) {
        mObserver = observer;
    }

    void SetUpdateConfig(UpdateConfig& config) {
        mUpdateConfig = config;
    }

    UpdateConfig& GetUpdateConfig() {
        return mUpdateConfig;
    }

    inline std::string CurrentNewVersion() const {
        return mCurrentVersion;
    }

    void SetNotifyUID(int uid) {
        mNotifierUID = uid;
    }


protected:
    struct UPLISTVERSION {
        char vername[64];
        char version[64];
        int  filenum;
        int  size;
    };

    struct UPFILEVERSION {
        char filename[256];
        char filever[64];
        char downname[256];
    };

    enum UpdateFileType {
        UPDATE_SELF = 0,
        UPDATE_INDEPENDENT_SO,
        UPDATE_APPLICATION,
        UPDATE_VIRUSDB,
        UPDATE_KERNEL_MODULE
    };

    struct UPDATALISTINFO {
        int type;
        std::string filetype;
        struct UPFILEVERSION filelist;

        friend bool operator<(const UPDATALISTINFO& x, const UPDATALISTINFO& y) {
            return x.type < y.type;
        }

        UPDATALISTINFO() {
            type = 0;
            memset(&filelist, 0, sizeof(struct UPFILEVERSION));
        }
    };

    typedef struct {
        PROXY_SET mConnectType;
        int mBandWidth;
        std::string mBeginTime;
        std::string mEndType;
        std::string mProxyIP;
        int mProxyPort;
        bool mUseAuth;
        std::string mUserName;
        std::string mPassword;
    }ConnectInfo;

protected:
    struct  NavigateModel {
        std::string mProduct;
        std::string mVersion;
        std::string mValidate;
        std::string mFinish;
        std::string mOvertime;
        std::string mDownload;
    };

    struct ConfigInfo {
        struct Client {
            std::string mVersion;
            std::string mPath;
            std::string mPlatform;
            std::string mSerial;
            std::string mVirusVer;
        }mClient;

        struct Server {
            std::string mUrl;
            std::string mUpdate;
        }mServer;
    };

protected:
    UpdateEngine();
    int Contruct(const std::string& installdir, const std::string& configpath);

    int GetClientInfo(ConfigInfo& info);

    int GetNavigate(UPDATE_SOURCE type,
                    const std::string &url,
                    std::string &navigate);

    const std::string GetDownloadUrl(UPDATE_SOURCE sourcetype,
                                     const std::string& url,
                                     const std::string& naviurl,
                                     NavigateModel& navimodel);

    virtual std::string GetDefaultAddress() { return ""; }
    virtual int DownloadFile(const std::string& url, const std::string& file);
    int GetRedirectUrl(const std::string& url, std::string& redirect);

    std::string MakeTmpFolder(const std::string& foldername);

    bool IsHtmlErrorPage(const std::string& file);
    int ParseNavigate(const std::string& navi, NavigateModel& model);

    static int WriteData(void* buffer, size_t size, size_t nmemb, FILE* file);

protected:
    virtual int UpdateEntry() = 0;

    UpdateObserver *mObserver;
protected:
    int mStatus;

    std::string mInstallDir;
    std::string mOwnerPath;
    std::string mCurrentVersion;

    UpdateConfig mUpdateConfig;

    /** define in setup.h SoftType */
    int mCenterType;
    int mNotifierUID;


public:
    pthread_t       mUpdateThread;
    static void*    ThreadWork(void* args);
};



#endif // UPGRADE_H
