#ifndef POLICYSYNCPARSER_H
#define POLICYSYNCPARSER_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <typeinfo>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

using namespace std;

class Storage;
class ScheduleEntry;
class PolicyScanModel;

class XMLNode {
public:
    virtual void Parse(xmlNodePtr node) = 0;
};

class StoreProtocol {
public:
    virtual int SyncStorage(Storage *storage) = 0;
    int SyncStorage(Storage *storage, bool locked) { return 0; }
};

template<typename T>
class ConfigField : public XMLNode{
public:
    union {
        bool mLocked;
        bool mAdmin;
    };
    T    mValue;

public:
    ConfigField() {
        mLocked = 0;
    }
    void Parse(xmlNodePtr node) {
        if (node) {
            if (xmlHasProp(node, BAD_CAST"lock")) {
                xmlChar *data = xmlGetProp(node, BAD_CAST"lock");
                if (strlen((char*)data) > 0) {
                    mLocked = atoi((char*)data);
                }
                xmlFree(data);
            }
            else if (xmlHasProp(node, BAD_CAST"admin")) {
                xmlChar *data = xmlGetProp(node, BAD_CAST"admin");
                if (strlen((char*)data) > 0) {
                    mAdmin = atoi((char*)data);
                }
                xmlFree(data);
            }
        }
    }

    int SyncStorage(Storage *storage) {
        mValue.SyncStorage(storage, mLocked);
        return 0;
    }
};

class TimeValue : public XMLNode {
public:
    enum UpdateMode {
        POLICY_UPDATE_DAYS = 0,
        POLICY_UPDATE_WEEKS,
        POLICY_UPDATE_MAUALLY,
        POLICY_UPDATE_INTERVAL
    };

public:
    enum UpdateMode  mMode;
    string  mTime;
    int mDay;
    vector<int> mWeeks;

    void Parse(xmlNodePtr node);
};

/**
 * @brief The GlobalAntiVirusPolicy class
 */
class GlobalAntiVirusPolicy : public XMLNode, public StoreProtocol {
public:
    void Parse(xmlNodePtr node);

public:
    class ItemData : public XMLNode {
    public:
        string mPath;
        string mValue;

        void Parse(xmlNodePtr node);
    };

    class PublicPolicy : public XMLNode, public StoreProtocol {
    public:
        class ListUserLock : public XMLNode {
        public:
            ConfigField<int> mLockMode;
            ConfigField<string> mCustomPasswd;

            void Parse(xmlNodePtr node);
        };

        class WhiteList : public XMLNode, public StoreProtocol {
        public:
            string mExtType;
            vector<ItemData> mFileList;
            ConfigField<int> mIgnoreLocalWhitelist;
            ListUserLock     mListLock;

            void Parse(xmlNodePtr node);
            int SyncStorage(Storage *storage);
        };

        class BlackList : public XMLNode, public StoreProtocol {
        public:
            vector<ItemData> mFileList;
            vector<ItemData> mHashList;

            void Parse(xmlNodePtr node);
            int SyncStorage(Storage *storage);
        };

        class CloudSetting : public XMLNode, public StoreProtocol {
        public:
            class Server : public XMLNode {
            public:
                bool    mPrivate;
                bool    mEnable;
                string  mAddress;
                short   mPort;
                int     mMode;
                int     mCount;
                string  mName;

                void Parse(xmlNodePtr node);
            };

            void Parse(xmlNodePtr node);
            int SyncStorage(Storage *storage);

        public:
            ConfigField<int> mCPURadio;
            ConfigField<int> mConnectInterval;
            vector<ConfigField<Server> > mServerSetting;
        };

        class VirusDealSetting : public XMLNode, public StoreProtocol {
        public:
            ConfigField<int>    mSpaceLessSet;
            ConfigField<int>    mFailedSet;
            ConfigField<int>    mBigFileSet;
            ConfigField<int>    mBackupType;

            void Parse(xmlNodePtr node);
            int SyncStorage(Storage *storage);
        };

        class SmartCPUSetting : public XMLNode, public StoreProtocol {
        public:
            ConfigField<int>  mType;
            ConfigField<int>  mLevel;

            void Parse(xmlNodePtr node);
            int SyncStorage(Storage *storage);
        };

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);

    public:
        ConfigField<WhiteList> mWhiteList;
        ConfigField<BlackList> mBlackList;
        CloudSetting mCloudSetting;
        VirusDealSetting mVirusDealSet;
        ConfigField<int> mVirusTrack;
        ConfigField<int> mWriteLog;
        SmartCPUSetting  mSmartCPUSet;
        ConfigField<int> mWorm08067;
        ConfigField<int> mYunYu;
        ConfigField<int> mLPKTool;
        ConfigField<int> mVirusUt;
        ConfigField<string> mKVEngine;
        ConfigField<int> mCloseRFM;
        ConfigField<int> mInnerWhitelist;
        ConfigField<int> mThreadScanCommand;
        ConfigField<int> mStrongerKill;
    };

    class EngineSet : public XMLNode {
    public:
        class EnginConfig : public XMLNode {
        public:
            string mName;
            int mEngineID;
            bool mEnable;
            ConfigField<bool> mHeuristic;
            ConfigField<bool> mPopVirus;
            ConfigField<bool> mScanZip;
            ConfigField<int>  mZipMaxSize;
            int mZipDepth;

            void Parse(xmlNodePtr node);
            int SyncStorage(Storage *storage);
        };

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);

    public:
        ConfigField<int>    mFileType;
        string              mFileExts;
        ConfigField<int>    mKillFailed;
        map<string, EnginConfig>   mEngine;
    };

    class ScanPolicy : public XMLNode, public StoreProtocol {
    public:
        class TimeScan : public XMLNode {
        public:
            bool mEnable;
            string mTaskID;
            int mKind;
            int mType;
            string mMsgID;
            int mNumber;
            string mStartDate;
            TimeValue mTime;

            bool mLocked;
            bool mEnablePath;
            std::list<string> mPaths;
            bool mEnableExt;
            std::string mExts;

            void Parse(xmlNodePtr node);
            void SyncModel(ScheduleEntry& entry);
        };
        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);

    public:
        ConfigField<int> mEfficient;
        ConfigField<TimeScan> mAllScan;
        ConfigField<TimeScan> mQuickScan;
        EngineSet   mEngineSet;
        ConfigField<int>    mFindVirus;
        int    mKillFailed;
        ConfigField<int>    mOperate;
    };

    class FileMonitor : public XMLNode , public StoreProtocol {
    public:
        ConfigField<bool>   mRundisable;
        ConfigField<bool>   mSmartBlock;
        ConfigField<int>    mRunMode;
        ConfigField<int>    mLockClose;
        ConfigField<int>    mMonitMode;
        ConfigField<int>    mMonitExtsMode;
        ConfigField<string> mMonitExts;
        ConfigField<bool>   mKernelState;
        ConfigField<bool>   mReportResult;
        ConfigField<int>    mFindVirusType;
        ConfigField<int>    mSigSource;
        int    mKillFailed;
        ConfigField<int>    mOLEMode;
        EngineSet           mEngineSet;
        // extend value
        ConfigField<bool>   mMonitorShareDoc;
        std::list<string>   mFolders;
        ConfigField<bool>   mClearSysCache;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);

    private:
        int UpdateMonitorFolders(Storage *storage, std::list<string>& folders);
    };

    class MailMonitor : public XMLNode, public StoreProtocol {
    public:
        ConfigField<bool>   mRundisable;
        ConfigField<int>    mLockClose;
        ConfigField<bool>   mReportResult;
        ConfigField<int>    mFindVirusType;
        EngineSet           mEngineSet;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class ShareMonitor : public XMLNode, public StoreProtocol {
    public:
        ConfigField<bool>   mRundisable;
        ConfigField<bool>   mReportResult;
        ConfigField<int>    mFindVirusType;
        EngineSet           mEngineSet;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class DefenMonitor : public XMLNode, public StoreProtocol {
    public:
        ConfigField<bool>    mEnabled;
        ConfigField<bool>    mNotified;
        ConfigField<bool>    mNeedlog;
        ConfigField<int>     mLevel;
        ConfigField<int>     mAuditMode;
        ConfigField<int>     mDigtSign;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class ActionAnalyze : public XMLNode, public StoreProtocol {
    public:
        ConfigField<bool>    mStatue;
        ConfigField<int>     mDealType;
        ConfigField<bool>    mNotify;
        ConfigField<bool>    mLog;
        ConfigField<bool>    mStartTip;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class DeviceMonitor : public XMLNode, public StoreProtocol {
    public:
        ConfigField<int>    mScanLevel;
        ConfigField<bool>   mEnabled;
        ConfigField<bool>   mAskUser;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

public:
    int SyncStorage(Storage *storage);

public:
    PublicPolicy mPublicPolicy;
    ScanPolicy   mScanPolicy;
    FileMonitor  mFileMonitor;
    MailMonitor  mMailMonitor;
    ShareMonitor mShareMonitor;
    DefenMonitor mDefenMonitor;
    ActionAnalyze mActionAnalyze;
    DeviceMonitor mDeviceMonitor;
};

/**
 * @brief The ConfigPolicy class
 */
class ConfigPolicy : public XMLNode, public StoreProtocol {
public:
    class ProxySetting : public XMLNode, public StoreProtocol {
    public:
        ConfigField<string> mIP;
        ConfigField<short>  mPort;
        ConfigField<bool>   mAuth;
        ConfigField<string> mUserName;
        ConfigField<string> mPasswd;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storeage);
    };

    class UpdateTime : public XMLNode, public StoreProtocol {
    public:
        ConfigField<TimeValue> mValue;
        ConfigField<string> mType;
        ConfigField<int>    mMaxDelay;
        ConfigField<bool>   mUpdateVirlib;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class SpeedLimit : public XMLNode, public StoreProtocol {
    public:
        bool mEnable;
        int  mSpeedKB;
        string mBeginTime;
        string mEndTime;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class ForceUpdate : public XMLNode, public StoreProtocol {
    public:
        bool mEnable;
        int  mMiniDay;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class NetConfig : public XMLNode, public StoreProtocol {
    public:
        ConfigField<int> mNetType;
        ProxySetting     mProxySetting;
        UpdateTime       mUpdateTime;
        ConfigField<SpeedLimit> mSpeedLimit;
        ConfigField<ForceUpdate> mForceUpdate;

        int mNetDataLimit;
        string mLimitStartTime;
        string mLimitEndTime;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    };

    class ConnectType : public XMLNode, public StoreProtocol {
    public:
        class Item : public XMLNode{
        public:
            string mName;
            string mType;
            string mPath;

            void Parse(xmlNodePtr node);
        };

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);
    public:
        map<string, Item> mValue;
    };

    class ShortLinkType : public XMLNode, public StoreProtocol {
    public:
        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage);

        bool mStartMenuLink;
        bool mDesktopIcon;
    };

    class SubProduct : public XMLNode {
    public:
        map<string, int> mSubProductMap;

        void Parse(xmlNodePtr node);
    };

    class ClearDisk : public XMLNode {
    public:
        bool mEnable;

        void Parse(xmlNodePtr node);
        int SyncStorage(Storage *storage, bool locked);
    };


public:
     void Parse(xmlNodePtr node);

public:
    int SyncStorage(Storage *storage);

public:
    ConfigField<ShortLinkType>  mLinkConfig;
    ConfigField<SubProduct>     mSubProducts;
    ConfigField<int>    mMaxSpeed;
    ConfigField<int>    mDownloadThreadCount;
    NetConfig           mNetConfig;
    ConfigField<ConnectType> mConnectType;
    ConfigField<ClearDisk> mClearDisk;
};

class PolicyScanModel {
public:
    struct ExcludePath {
        int mState;
        list<string> mPaths;
    };

    struct ScanKeepDay {
        int mState;
        int mDays;
    };

    struct CompressSize {
        int mState;
        int mSize;
    };

    struct BackupType {
        int mState;
        int mFailMode;
    };

    struct TimeScan {
        int mState;
        int mWeeks;
        int mHours;
        int mMinite;
    };

    struct ScanPolicy {
        struct ScanKeepDay mKeepDays;
        struct CompressSize mCompressSize;
        int mDealVirusType;
        int mCleanFaild;
        struct BackupType   mBackupType;
        struct TimeScan     mTimeScan;
    };

public:
    string              mScanPath;
    struct ExcludePath  mExcludePaths;
    struct ScanPolicy   mScanPoliy;
};

class CommandMessage : public XMLNode{
public:
    string mName;
    string mContent;

    void Parse(xmlNodePtr node);
};

class UpdateCommand : public XMLNode {
public:
    string mAdmin;
    bool   mOnlyVersion;
    int    mMaxDelay;

    void Parse(xmlNodePtr node);
    UpdateCommand() {
        mMaxDelay = 0;
        mOnlyVersion = false;
    }
};

class ExecuteCommand : public XMLNode {
public:
    int mAction;
    string mMainPath;        ////Download path
    string mCmdPara;         
    string mWorkPath;        ////Execute path
    string mFile;            ////File           

    void Parse(xmlNodePtr node);
};

/**
 * Policy ID EB8AFFA5-0710-47E6-8F53-55CAE55E1915
 * @brief The CenterConfigPolicy class
 */
class EnvironmentalPolicy : public XMLNode, public StoreProtocol {
private:
    class TrayPolicy : public XMLNode {
    public:
        ConfigField<bool> mTrayHide;
        ConfigField<bool> mAuthPopHide;
        ConfigField<string> mTitle;
        ConfigField<string> mSubTitle;        

        void Parse(xmlNodePtr node);
    };

    struct CenterConfig : public XMLNode {
        int mConnectRetryTime;
        int mUploadBindWidth;
        string mUploadLimitStartTime;
        string mUploadLimitEndTime;

        void Parse(xmlNodePtr node);
    };

    struct ClientLogClean {
        int mPID;
        int mTID;
        int mOverDays;
        string mRelateState;
        int mLimitCount;

        void Parse(xmlNodePtr node);
    };

public:
     void Parse(xmlNodePtr node);

public:
    int SyncStorage(Storage *storage);

public:
    TrayPolicy                          mTrayPolicy;
    ConfigField<bool>                   mLockGUID;
    ConfigField<list<string> >          mCenterList;
    ConfigField<bool>                   mCenterProxyState;
    ConfigField<bool>                   mCheckEIDState;
    ConfigField<CenterConfig>           mCenterConfig;
    ConfigField<list<ClientLogClean> >  mLogCleanConfig;
    ConfigField<string> mAdminPasswd;
};

class NetCenterPolicy : public XMLNode, public StoreProtocol {

};

class FirewallPolicy : public XMLNode, public StoreProtocol {
    class _BaseUrlEntry : public XMLNode {
    public:
        ConfigField<int>    mMonStatus;
        ConfigField<int>    mLogStatus;

    public:
         void Parse(xmlNodePtr node);
    };

    class RsIPRule : public _BaseUrlEntry {
    public:
        ConfigField<int> mAlertStatus;
        ConfigField<int> mBreakTimes;

    public:
         void Parse(xmlNodePtr node);
    };

    class AdFilter:public _BaseUrlEntry {
    public:
        list<string> mRule;

    public:
         void Parse(xmlNodePtr node);
    };

    class RsIPEntry : public XMLNode {
    public:
        string  mID;
        int     mStatus;
        string  mName;
    public:
         void Parse(xmlNodePtr node);
    };

    class NetProtect : public XMLNode, public StoreProtocol {
    public:
        ConfigField<bool>   mStatus;
        ConfigField<bool>   mWhiteUrlStatus;
        _BaseUrlEntry       mAntiEvilUrl;
        _BaseUrlEntry       mAntiFishUrl;
        _BaseUrlEntry       mAntiEvilDown;
        _BaseUrlEntry       mSearchUrlProtect;
        _BaseUrlEntry       mAntiXss;
        AdFilter            mAdfilter;
        RsIPRule            mRsIPRules;

    public:
         void Parse(xmlNodePtr node);
         int SyncStorage(Storage *storage);
    };

    class BrowserRule : public XMLNode {
    public:
        string  mID;
        string  mUrl;
        int     mControlModel;
        int     mAltert;
        string  mRedirect;

    public:
         void Parse(xmlNodePtr node);
    };

    template<class T>
    class TimeRule : public XMLNode {
    public:
        string mID;
        string mValidTime;
        list<T> mRuleList;

    public:
         void Parse(xmlNodePtr node);
    };

    class BrowserAudit : public XMLNode {
    public:
        ConfigField<int>    mMonStatus;
        ConfigField<int>    mLogAllWeb;
        list<ConfigField<TimeRule<BrowserRule> > > mTimeRuleList;

    public:
         void Parse(xmlNodePtr node);
    };

    class NetProcRule : public XMLNode {
    public:
        string mID;
        int mCheckMD5;
        int mAllowListen;
        int mAllowOutside;
        int mSoftType;
        string mSoftID;

    public:
        void Parse(xmlNodePtr node);
    };

    class NetProcAudit : public XMLNode, public StoreProtocol {
    public:
        ConfigField<int>    mMonStatus;
        ConfigField<int>    mLogStatus;
        ConfigField<int>    mCheckRsSign;
        ConfigField<int>    mCheckModule;
        ConfigField<int>    mUnknowAction;
        list<ConfigField<TimeRule<NetProcRule> > >  mTimeRuleList;

    public:
         void Parse(xmlNodePtr node);
         int SyncStorage(Storage *storage);
    };

    class FluxMgr : public XMLNode {
    public:
        ConfigField<int>    mMonStatus;
        ConfigField<int>    mLogTimer;

    public:
         void Parse(xmlNodePtr node);
    };

    class AdslShare : public XMLNode {
    public:
        ConfigField<int>    mMonStatus;
        ConfigField<int>    mTotalWidth;

    public:
         void Parse(xmlNodePtr node);
    };

    class AccessControl {
    public:
        ConfigField<int>    mMonStatus;
        ConfigField<int>    mAlertStatus;
        ConfigField<int>    mControlCode;
        list<string>        mRuleList;

    public:
         void Parse(xmlNodePtr node);
    };

    class ShareMgr {
    public:
        ConfigField<int>    mFileLogStatus;
        ConfigField<int>    mAccessLogStatus;
        ConfigField<int>    mDisableShare;
        ConfigField<AccessControl>  mAccessControl;

    public:
         void Parse(xmlNodePtr node);
         int SyncStorage(Storage *storage);
    };

public:
     void Parse(xmlNodePtr node);
     int SyncStorage(Storage *storage);

public:
    NetProtect  mNetProtect;
    ConfigField<list<string> > mWhiteUrlList;
    ConfigField<list<RsIPEntry> > mRsIPRuleList;

    ConfigField<BrowserAudit>   mBrowserAudit;
    ConfigField<NetProcAudit>   mNetProcAudit;
    ConfigField<FluxMgr>        mFluxMgr;
    AdslShare                   mAdslShare;
    ConfigField<ShareMgr>       mShareMgr;
};

class PolicyParser
{
public:
    static int ParseConfigPolicy(const string& filepath, ConfigPolicy& policy);
    static int ParseGlobalPolicy(const string& filepath, GlobalAntiVirusPolicy& policy);
    static int ParseFirewallPolicy(const string& filepath, FirewallPolicy& policy);

    static int ParseConfigPolicy(const char *content, ConfigPolicy& policy);
    static int ParseGlobalPolicy(const char *content, GlobalAntiVirusPolicy& policy);    
    static int ParseEnvrionPolicy(const char * content, EnvironmentalPolicy& policy);
    static int ParseFirewallPolicy(const char * content, FirewallPolicy& policy);

    static int ParseLinuxPolicy(const string& content, PolicyScanModel& model);


    static int ParseCommandMessage(const string& content, CommandMessage& message);
    static int ParseCommandExecute(const string& content, ExecuteCommand& execute);
    static int ParseCommandUpdate(const string& content, UpdateCommand& update);
};

#endif // POLICYSYNCPARSER_H
