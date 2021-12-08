#ifndef ABSTRACTAPPLICATION_H
#define ABSTRACTAPPLICATION_H

#include "storagebridge.h"
#include "circlelink.h"
#include "listdatadelegate.h"
#include "baseconfig.h"
#include "centerlistener.h"
#include "Entry/scheduleentry.h"
#include "Entry/commonentry.h"
#include "Entry/sourcelistentry.h"
#include "handler.h"

#include <string>
#include <list>
#include <vector>
#include <map>

class LogEventEntry;
class VirusInfoEntry;
class QuarantineLog;
class WhiteListEntry;
class CommEventLog;
class Controller;
class ScanConfig;
class UpdateConfig;
class VirusReportData;
class ScheduleEntry;
class UrlRuleEntry;
class NetAccessEntry;
class ActiveDefenConfig;
class AntiVirusSetting;
class NetfilterSetting;
class BaseSetting;
class DefenCenterSetting;
class AppConfig;
class ScanIntelligentParams;
class SourceListEntry;
class Handler;

class Application : public CenterCommandObserver, HandleObserver {
public:
    typedef enum {
        QUERY_FIND_VIRUS,
        QUERY_QUARANTINE_FAILED,        
    }QUERY_EVENT;

    typedef enum {
        NORMAL_APP = 0,
        SCHEDULE_TASK,
        PLUGIN_CHECK,
        REMOTE_COMMAND,
        CONTEXT_MENU,
        ACTIVE_DEFEN,
    }APP_TYPE;

    typedef enum {
        UPDATE_DOWNLOAD_RP,
        UPDATE_COMBINE_RP,
        UPDATE_VLIB_SUCCESS,
        UPDATE_VLIB__FAILED,
        UPDATE_APP_DOWNLOAD,
        UPDATE_APP_SUCCESS,
        UPDATE_APP_FAILED,
        // FIXME 这个值用于描述客户端的注册状态，放在这里不太合适，先借用一下
        RESPOND_REGISTER_STATE,
        UPDATE_FINISHED, 
    }UPDATE_EVENT;

    typedef enum {
        THREAD_STATUS,
        SCAN_PREPARE,
        SCAN_ALL_FILE,
        SCAN_FINISHED,
        SCAN_ERROR_EXIT,
    }SCAN_STATUS_TYPE;

protected:
    enum StorageType {
        GJCZ = 0x1,
        GLTX = 0x2,
        ZDFY = 0x4,
    };

    StorageBridge* GetStorageBridget(enum StorageType type);
    /** send the socket to */
    int ReloadConfig(enum StorageType type);

protected:
    typedef enum _UserType {
        ROOT = 0x0,
        SECADM,
        AUDITADM,
        USER
    }UserType;
    UserType GetUserType();

public:
    virtual void RefreshVirusLogs(const string& file, const string& virus, int/** VirusReportData::VirusStatus */ status) {}
    /**
     * scantype is ScanType() return
     * status == THREAD_STATUS, data is vector<ReportCache>
     * status == SCAN_FINISHED, progress is the scaned count
     */
    virtual void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE status, int progress, void *data=NULL) {}
    /** query the virus deal func  return : NOTIFY_CONFIRM_SELECT=确认 NOTIFY_IGNORE_SELECT=忽略 */
    virtual int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) { return 0; }

    /** define in logevententry.h, high 16 is LogEventEntry::ScanType, low 16 is LogEventEntry::DiskType */
    virtual int  ScanType() { return 0; }
    virtual int  GetScanPath(std::list<pair<std::string,int> >& path) { return 0; }

    virtual int  ScanFinished(int result) { return 0; }

    virtual int RefreshUpdateStatus(UPDATE_EVENT type, const string& data) { return 0; }

public:
    Application() {
        mCenterListener = NULL;
        mScanLogs = NULL;
        mLockFd = 0;
        mScanEventLastStamp = 0;
        mCheckState = false;
        mHandler = NULL;
        mScanStatus = APP_SCAN_INIT;
        mActived = false;
    }
    Application(APP_TYPE type) : mApplicationType(type) {
        mCenterListener = NULL;
        mScanLogs = NULL;
        mLockFd = 0;
        mHandler = NULL;
        mScanStatus = APP_SCAN_INIT;
        mActived = false;
    }
    virtual ~Application();

    int Construct(const string& owner, bool singleton=true, const char *sockname=NULL);
    int Construct(AppConfig *config);

    int ResetClientProxy(bool qt_tray);

    virtual int QuickScan(bool feedback=false);
    virtual int FullScan(bool feedback=false);
    virtual int StartScan(std::list<pair<std::string,int> >& path, bool feedback=false);
    int PauseScan();
    int Continue();
    int StopScan();

    int Update(bool offline, const string& file);
    int UpdateOfflineVirlib(const string& file);

    int RestoreQuarantine(QuarantineLog& log);
    int RestoreQuarantine(QuarantineLog& log, bool trust);
    int DeleteQuarantine(QuarantineLog& log);

    int QueryPath(const string& folder, map<string, vector<string> >& infos);

    std::string GetVersion();
    std::string GetUpdateTime();
    std::string GetVirusDbVer();
    std::string GetVirusUpdateTime();

    /** the value refrence in baseconfig.h, HealthStatus */
    int GetDaemonStatus();
    int RepairDaemonStatus(int status);

    int ReloadNetfilterConfig();

    std::string GetInstallPath();

    bool Registed();
    int  Register(const string& serial);
    void RefreshRegisted();

    int  GetUrlBlackList(std::list<UrlRuleEntry>& white);
    int  AddUrlBlackList(const UrlRuleEntry& entry);
    int  DeleteUrlBlacklist(const UrlRuleEntry& entry);

    int  GetScanLogs(std::list<LogEventEntry>& logs);
    int  GetVirusLogs(std::list<VirusInfoEntry>& logs);
    int  GetNetAccessLog(std::list<NetAccessEntry>& logs);
    int  GetBaseLogs(std::list<CommEventLog>& logs, const CommEventLog& seed);
    int  GetQuaLogs(std::list<QuarantineLog>& logs);

    int  GetWhiteList(std::list<WhiteListEntry>& white);
    int  GetProcessBlackList(std::list<WhiteListEntry>& white);
    int  GetFolderMonitorList(std::list<WhiteListEntry>& white);
    int  GetScanFileTypeList(std::list<WhiteListEntry>& lists);

    int  AddWhiteList(const WhiteListEntry& entry);
    int  DeleteWhitelist(const WhiteListEntry& entry);

    int  AppendQuaLog(const QuarantineLog& log);
    int  AppendScanLog(const LogEventEntry& log);
    int  AppendVirusLog(const VirusInfoEntry& entry);

    int  DeleteQuaLog(const QuarantineLog& log);

    int AppendNetAccessLog(const NetAccessEntry& entry);

    int GetScanConfig(ScanConfig& config);
    int GetUpdateConfig(UpdateConfig& config);
    int SetUpdateConfig(const UpdateConfig& config);

    int GetUpdateConfig(UpgradeSetting& config);
    int SetUpdateConfig(const UpgradeSetting& config);

    int GetUpdateSource(list<SourceListEntry>& entries);
    int SetUpdateSource(list<SourceListEntry>& entries);

    int GetCenterSource(list<SourceListEntry>& entries);
    int SetCenterSource(list<SourceListEntry>& entries);

    int GetDefenConfig(ActiveDefenConfig& config);
    int SaveDefenConfig(const ActiveDefenConfig& config);

    int GetAntiVirusConfig(AntiVirusSetting& config, int flag=/*AntiVirusSetting::ALL_SETTING*/0xFF);
    int UpdateAntiVirusConfig(AntiVirusSetting& config);
    int UpdateScanConfig(AntiVirusSetting& config);

    int GetNetfilterConfig(NetfilterSetting& config);
    int UpdateNetfilterConfig(NetfilterSetting& config);

    int GetBaseConfig(BaseSetting& config);
    int UpdateBaseConfig(BaseSetting& config);

    int GetDefenCenterConfig(DefenCenterSetting& config);
    int UpdateDefenCenterConfig(DefenCenterSetting& config);

    bool GetActiveDefenceState();
    bool GetUSBPluginMonitorState();
    bool GetNetFilterState();
    bool GetBrowserBlockState();

    int GetScheduleEntry(/*ScheduleEntry::ScanType*/int type, ScheduleEntry& entry);
    int SetScheduleEntry(ScheduleEntry& entry, bool force=false);
    int DeleteScheuleType(int type);

    int GetTitle(string& major, string& minor);
    
    bool IsSecretMachine();
    bool IsCenterLess();
    bool IsChinaSystem();
    bool IsGeneralSystem();

    int ResetConfig();

    AntiVirusSetting::AntiCommon::ScanType GetScanIntelligentMode(
            ScanIntelligentParams& params,
            bool& locked);
    int SetScanIntelligentMode(AntiVirusSetting::AntiCommon::ScanType type);

    string GetScanFileCustomType();
    int SetScanFileCustomType(const string& params);

    int GetProductType() const;

private:
    int GetSourceList(list<SourceListEntry>& entries, SourceListEntry::SOURCE_TYPE type);
    int SetSourceList(list<SourceListEntry>& entries, SourceListEntry::SOURCE_TYPE type, bool manul);

protected:
    int InitListener(const char *owner, const char *sock);

    int RespondScanEvent(const int type, const string& data);
    int RespondUpdateEvent(const int type, const string& data);

public:
    virtual int OnCommandObserver(int event, void *args, void **result, int *outlen);

    virtual void HandleMessage(int event, int type, void *object);

public:
    APP_TYPE AppType() const {
        return mApplicationType;
    }

protected:
    CenterListener      *mCenterListener;
    CircleLink<string>  *mScanLogs;
    struct ReportCache {
        string  mFile;
        int     mCount;
        int     mVirus;
        int     mTag;
    };

    vector<struct ReportCache>   mReportCache;

    Handler *mHandler;
    /**
     * @brief The AppScanStatus enum
     * 此状态值是扫描的主动状态，用于用户进行主动扫描时，状态值的判断
     */
    enum AppScanStatus {
        APP_SCAN_INIT = 0,
        APP_SCAN_PREPARE,
        APP_SCAN_ING,
        APP_SCAN_FINISHED
    }mScanStatus;

protected:
    APP_TYPE mApplicationType;
    AppConfig   *mAppConfig;
    string  mOwnerPath;
    string  mSockName;

    map<enum StorageType, StorageBridge*> mStorageBridges;

    /** 软件是否激活，对于单机版，是否已注册，对于网络版，是否连接至中心 */
    bool mActived;
private:
    FILE *mLockFd;

private:
    static void* CheckScanSocketThread(void *params);
    void CheckScanSocketFunction();

    pthread_t mCheckThread;
    long mScanEventLastStamp;
    bool mCheckState;
};

template<class T, class K>
class LogDataBridge : public ListViewDataDelegate<K> {
public:
    virtual ~LogDataBridge() {
        mDataArray.clear();
    }

public:
    int Count() {
        return mDataArray.size();
    }

    virtual void Content(int pos, K& data) = 0;

    bool GetData(int pos, T& data) {
        typename std::list<T>::iterator iter;
        int i;
        for (iter=mDataArray.begin(), i=0; iter!=mDataArray.end(); ++iter, ++i) {
            if (i == pos) {
                data = *iter;
                return true;
            }
        }
        return false;
    }

    virtual void SelectData(int pos) {

    }

    virtual void DeleteData(int pos) {
        typename std::list<T>::iterator iter;
        int i;
        for (iter=mDataArray.begin(),i=0; iter!=mDataArray.end(); ++iter, ++i) {
            if (i == pos) {
                mDataArray.erase(iter);
                break;
            }
        }
    }

public:
    LogDataBridge(Application *application) {
        mApplication = application;
    }

public:
    std::list<T> mDataArray;

protected:
    Application *mApplication;
};

#endif // ABSTRACTAPPLICATION_H
