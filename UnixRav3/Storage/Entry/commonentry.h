#ifndef COMMONENTRY_H
#define COMMONENTRY_H

#include <string>
#include <list>
#include <map>
#include <vector>

#include "scheduleentry.h"
#include "sourcelistentry.h"

using namespace std;

class DBRelated {
public:
    long mDBID;

    DBRelated() {
        mDBID = 0;
    }
};

template<typename T, typename K>
void make_db_setting_consistent(list<T>& source, list<K> dest, list<int>& differ) {
    typename list<K>::iterator diter;
    map<int,bool> destmap;
    for (diter=dest.begin(); diter!=dest.end(); ++diter) {
        destmap.insert(make_pair(diter->mDBID, true));
    }

    map<int,bool>::iterator iter;
    typename list<T>::iterator siter;
    for (siter=source.begin(); siter!=source.end(); ++siter) {
        if (destmap.find(siter->mID) == destmap.end()) {
            differ.push_back(siter->mID);
        }
    }
}

class AntiVirusSetting {
public:
    class AntiCommon {
    public:
        typedef enum _ScanType {
            DEFAULT_MODE = 0,
            OFFICE_MODE = 10,
            AUTO_MODE   = 200,
            HIGH_SPEED_MODE = 100,
        }ScanType;

        typedef enum _CloudRequestType {
            CLOUD_GET,
            CLOUD_POST
        }CloudRequestType;

        typedef enum _CloudType {
            PUBLIC_CLOUD,
            PRIVATE_CLOUD
        }CloudType;

        class CloudModel : public DBRelated {
        public:
            CloudType   mType;
            string      mServerAddr;
            short       mPort;
            CloudRequestType mCloudRequestType;
            int         mCloudRequestFileCount;
            string      mServerName;
            bool        mState;

            CloudModel() {
                mType = PUBLIC_CLOUD;
                mPort = 0;
                mCloudRequestType = CLOUD_GET;
                mCloudRequestFileCount = 0;
                mState = true;
            }

            bool operator ==(const CloudModel& model) {
                return mType == model.mType &&
                        mServerAddr == model.mServerAddr &&
                        mPort == model.mPort &&
                        mCloudRequestType == model.mCloudRequestType &&
                        mCloudRequestFileCount == model.mCloudRequestFileCount &&
                        mServerName == model.mServerName;
            }
        };

    public:
        LockField<bool>    mRunEnvJudge;
        LockField<bool>    mVirusTrace;
        LockField<bool>    mVirusLoged;
        LockField<bool>    mRescanAccelerate;
        LockField<bool>    mFileAccelerate;
        LockField<int>     mScanType; // enum ScanType
        LockField<int>     mCPUPercent;
        LockField<int>     mCloudTestIntevel;
        LockField<bool>    mUsePublicCloud;
        LockField<bool>    mUsePrivateCloud;
        list<CloudModel>   mCloudSetting;
    };

    class WhiteList {
    public:
        list<pair<string, int> > mFilePaths;
        list<string>    mExtTypes;
    };

    class BlackList {
    public:
        list<pair<string, int> > mFilePaths;
    };

    class VirusBackup {
    public:
        LockField<bool>    mVirusBackup;
        // the value define in ScanEngine/scanconfig.h
        LockField<int>     mLargeFile;
        LockField<int>     mSpaceLess;
        LockField<int>     mBackupFailed;
    };

    class VirusScan {
    public:
        // define in ScanEngine/scanconfig.h
        LockField<int>     mFileType;
        LockField<bool>    mScanCommonVirus;
        LockField<bool>    mScanIntelligent;
        LockField<bool>    mScanCompressFile;
        LockField<int>     mZipFileSize;
        LockField<int>     mVirusDeal;
        LockField<int>     mCleanFailed;
    };

    class CloudEngine {
   	public:
   		LockField<bool>    mEnable;
   		LockField<string>  mUrl;
    };

    class TimeScan {
    public:
        typedef struct _time_scan_model {
            int         mScanTime;
            int         mScanHour;
            int         mScanMinute;
            vector<int>   mWeekdays;
        }TimeScanModel;

    public:
        void SetTimeScanEntry(ScheduleEntry::ScanType type, ScheduleEntry& entry) {
            entry.mFuncEnum  = type;
            mValue[type] = entry;
        }

        bool GetTimeScanEntry(ScheduleEntry::ScanType type, ScheduleEntry& entry) {
            map<ScheduleEntry::ScanType, ScheduleEntry>::iterator iter;
            iter = mValue.find(type);
            if (iter != mValue.end()) {
                entry = iter->second;
                return true;
            }
            return false;
        }

    public:
        LockField<bool>    mFullScan;
        LockField<bool>    mQuickScan;
        map<ScheduleEntry::ScanType, ScheduleEntry> mValue;
    };

    class FileMonitor {
    public:
        typedef enum {
            GEEK            = 0,
            STANDENT        = 1,
            PROFESSIONAL    = 2,
            QUICK_MODE      = 3,
        }FileDefenType;
    public:
        LockField<bool>    mStartPowerOn;
        LockField<bool>    mIntelligentBlacklist;
        LockField<bool>    mKernelMonitor;
        LockField<bool>    mUserCanStop;
        /** FileDefenType */
        LockField<int>     mMonitorType;
        /** define in ScanConfig.h SCAN_TYPE */
        LockField<int>     mFileType;
        LockField<bool>    mMonitorAccelerate;
        LockField<bool>    mInnerScan;
        /** define in ScanConfig.h SCAN_ENGINE */
        LockField<bool>    mScanCommonVirus;
        LockField<bool>    mScanIntelligent;
        LockField<bool>    mZipScan;
        LockField<int>     mZipFileSize;
        /** define in ScanConfig.h VIRUS_DEAL */
        LockField<int>     mVirusDeal;
        /** define in ScanConfig.h CLEAN_FAILED */
        LockField<int>     mCleanFailed;
        LockField<bool>    mCleanPrompt;
        LockField<bool>    mCloudUse;
        LockField<int>     mMonitorExtsType;
        LockField<string>  mMonitorTypeDefineExts;
        LockField<string>  mFileTypeDefineExts;
        LockField<list<string> > mMonitorPaths;
        LockField<bool>    mResetKernelCache;
    };

    class PluginMonitor {
    public:
        typedef enum {
            AUTO_SCAN = 0,
            QUERY_USER_SCAN,

        }PluginQuery;
    public:
        LockField<bool>    mState;
        /** define in PluginQuery */
        LockField<int>     mPluginDeal;
        LockField<int>     mScanDepth;
    };

    class name
    {
    public:
        name() {}
    };

public:
    AntiCommon     mCommon;
    WhiteList  mWhiteList;
    BlackList  mBlackList;
    VirusBackup     mVirusBackup;
    VirusScan       mVirusScan;
    CloudEngine     mCloudEngine;
    TimeScan   mTimeScan;
    FileMonitor mFileMonitor;
    PluginMonitor mPluginMonitor;


    string mCenterAddr;

public:
    enum SettingPlugin{
        ANTI_COMMON     = 0x1,
        WHITE_LIST      = 0x2,
        BLACK_LIST      = 0x4,
        VIRUS_BACKUP    = 0x8,
        VIRUS_SCAN      = 0x10,
        TIME_SCAN       = 0x20,
        FILE_MONITOR    = 0x40,
        PLUGIN_MONITOR  = 0x80,
        ALL_SETTING     = 0xFF,
    };
};

class NetfilterSetting {
public:
    class NetfilterCommon {
    public:
        class FilterState {
        public:
            bool    mStartPowerOn;
            bool    mLogState;
        };
    public:
        bool        mNetfilterState;
        FilterState mTorjanUrl;
        FilterState mFakeUrl;
        FilterState mMalicious;
        FilterState mHackAttack;
        FilterState mCrossScript;
        FilterState mSearchResult;
        FilterState mAdFilter;
    };

    class AdfilterRule {
    public:
        string  mName;
        int     mState;
        string  mDescription;
        string  mRuleCount;
        string  mRuleOP;
    };

    class AntiHackAttack {
    public:
        bool    mPromptUser;
        int     mBlockIPDuration;
        bool    mTrojanRule;
        bool    mSQLInject;
        bool    mLeakRule;
    };

    class AntiARPAttack {
    public:
        bool    mARPState;
        bool    mIPConflictConnected;
        bool    mDisableSendAPR;
        bool    mAntiAttackPrompt;
        bool    mCheckFakeMac;
        int     mSendPacketRate;
        int     mSendPacketDuration;
        int     mCheckDuration;
        list<pair<string, string> > mIPBindMac;
    };

    class AntiOutAttack {
    public:
        class AttackType {
        public:
            bool    mType;
            int     mPacketRate;
        };

    public:
        bool    mAntiState;
        bool    mPromptState;
        AttackType  mSYNAttack;
        AttackType  mICMPAttack;
        AttackType  mUDPAttack;
    };

    class LimitNetUrl {
    public:
        enum TimeType {
            EVERY_DAY,
            EVERY_WEEK,
            TIME_SECTION
        };

        class LimitUrl {
        public:
            string  mUrl;
            enum {
                PROMPT,
                BLOCK,
            }mOperation;
        };

        class LimitRule {
        public:
            string  mStartTime;
            string  mEndTime;
            enum TimeType mType;
            int     mWeekDay;
            list<LimitUrl> mUrls;
        };
    public:
        bool    mLimitState;
        bool    mLogState;
        list<LimitRule>  mLimitUrl;
        string  mRedirectUrl;
    };

    class LimitApp {
        enum BlockType {
            ALLOW,
            REJECT,
            PROMPT
        };

        class LimitRule {
        public:
            string mAppPath;
            bool    mAntiRewrite;
            bool    mDisableListen;
            bool    mDisableConnect;
        };

    public:
        bool    mLimitState;
        bool    mLogState;
        bool    mIntelligentJudge;
        enum BlockType  mUnknowAppRule;
        list<LimitRule> mLimitRule;
    };

    class NetFlowManager {
    public:
        bool    nState;
        int     mCountIntervel;
    };

    class NetShared {
    public:
        bool    mCloseDefaultShared;
        bool    mCloseRemoteShared;
        bool    mLogLocalShared;
        bool    mLogState;
        bool    mAccessState;
        bool    mRejectPrompt;
        bool    mUnlimitRuleAccessed;
        list<pair<string, bool> > mIPList;
    };

    class ADSLShared {
    public:
        bool    mADSLState;
        int     mTotalBand;
    };

    class IllegalConnect {
    public:
        bool    mState;
        bool    mPrompt;
        enum {
            INTELLIGENT_CHECK,
            TIME_CHECK
        }mCheckType;
    };

    class AntiIPRewrite {
    public:
        bool    mState;
        bool    mAntiMacRewrite;
        bool    mLogState;
    };

    class PortRule {
        short   mPort;
        int     mProtocol;
        int     mDirection;
        bool    mAllowConnect;
    };

    class CustomIPRule {
    public:
        class IPAddr {
        public:
            int     mIPType;
            string  mIPAddr;
        };

    public:
        string  mName;
        bool    mBlockPrompt;
        bool    mAllowAccess;
        int     mDirection;
        IPAddr  mLocalIPAddr;
        IPAddr  mRemoteAddr;
        int     mProtocol;
        list<int> mLocalPortList;
        list<int> mRemotePortList;
    };

    class IPRule {
    public:
        bool            mWhiteListState;
        list<string>    mWhiteList;
        bool            mBlacklistState;
        bool            mUrlBlockPrompt;
        bool            mLogState;
        list<string>    mBlackList;
        bool            mPortState;
        bool            mPortBlockPrompt;
        list<PortRule>  mPortRule;
        list<CustomIPRule> mCustomIPRule;
    };

public:
    NetfilterCommon     mCommon;
    list<string>        mWhitelist;
    list<AdfilterRule>  mAdRuleList;
    AntiHackAttack      mAntiHackAttack;
    AntiARPAttack       mAntiFakeARP;
    AntiOutAttack       mAntiOutAttack;
    LimitNetUrl         mLimitNetUrl;
    LimitApp            mLimitApplication;
    NetFlowManager      mNetFlowManager;
    NetShared           mNetShared;
    ADSLShared          mADSLShared;
    AntiIPRewrite       mAntiIPRewrite;
    IllegalConnect      mIllegelConnect;
    IPRule              mIPRule;
};

class UpgradeSetting {
public:
    typedef enum {
        // MUST same as TimeValue::UpdateMode define in policysyncparse.h
        UPGRADE_EVERY_DAY,
        UPGRADE_EVERY_WEEK,
        UPGRADE_MANUALLY,
        UPGRADE_INTERVAL
    }UPGRADE_MODE;

    typedef enum {
        DELAY_DISABLE = 0,
        DELAY_AUTOMATIC,
        DELAY_INTIME
    }DELAY_START;

    typedef enum {
        UPDATE_VIRUS_DB,
        UPDATE_ALL,
    }UPDATE_CONTENT;

    typedef enum {
        CONNECT_BROWSER = 0,
        CONNECT_DIRECT,
        CONNECT_PROXY
    }PROXY_SET;

    typedef enum {
        OFFICAL_HTTP,
        OTHER_HTTP,
        SHARE_FOLDER,
    }UPDATE_SOURCE;

public:
    LockField<UPGRADE_MODE>    mUpdateMode;
    LockField<DELAY_START>     mDelayStartAction;
    LockField<UPDATE_CONTENT>  mUpdateContent;
    LockField<PROXY_SET>       mProxySetting;
    LockField<UPDATE_SOURCE>   mUpdateSourceAction;
    LockField<std::string>     mUpdateModeExtend;
    LockField<int>             mDelayStartTime;
    LockField<std::string>     mProxyAddr;
    LockField<std::string>     mProxyPort;
    LockField<std::string>     mProxyAccount;
    LockField<std::string>     mProxyPasswd;
    LockField<bool>            mBandLimit;
    LockField<int>             mBandWidth;
    LockField<std::string>     mStartTime;
    LockField<std::string>     mEndTime;
    LockField<std::string>     mUpdateProduct;
    LockField<int>             mForceUpdateDelay;
    LockField<bool>            mForceUpdate;
    LockField<bool>            mClearUpdateCache;
    LockField<bool>            mCheckVirusDbRealTime;
};

class BaseSetting {
public:
    class CenterFinder {
    public:
        LockField<list<string> > mCenterAddr;
        int mCenterRetryInterval;
        int mUploadBandWidthType;
        string mBandWidthLimitTime;
    };

public:
    LockField<string>  mAdminPasswd;
    LockField<bool>    mTrayIconHide;

    // center finder
    LockField<list<SourceListEntry> > mCenterAddr;
    LockField<int> mCenterRetryInterval;
    LockField<int> mUploadBandWidthType;
    string mBandWidthStartTime;
    string mBandWidthEndTime;
    LockField<bool> mCenterProxyState;
    LockField<bool> mCheckEIDState;

    // update setting
    LockField<int>     mUpdateMode;
    LockField<string>  mUpdateTime;
    LockField<vector<int> > mWeekDays;
//    vector<int> mWeekDays;
    LockField<int>     mDelayStart;
    LockField<int>     mDelayTime;
    LockField<int>     mUpdateComponents;
    LockField<bool>    mCheckViruslibInTime;
    LockField<bool>    mUrlLibValid;
    LockField<int>     mProxySetting;
    LockField<string>  mProxyAddr;
    LockField<int>     mProxyPort;
    LockField<string>  mProxyUser;
    LockField<string>  mProxyPasswd;
    LockField<bool>    mOfficalSource;
    LockField<list<SourceListEntry> >  mOtherUpdateCenter;
};


class DefenCenterSetting {
public:
    LockField<bool>     mFileMonitorState;
    LockField<bool>     mMailMonitorState;
    LockField<bool>     mShareMonitorState;
    LockField<bool>     mUSBPluginMonitorState;

    LockField<bool>     mFakeWormState;
    LockField<bool>     mRainCloudVirusState;
    LockField<bool>     mWeikeWormState;
    LockField<bool>     mDLLInjectLessState;

    enum DefenChangeFlag {
        USB_MONITOR_CHANGED = 1,
        FILE_DEFEN_CHANGED  = 2,
        MAIL_MONITOR_CHANGED= 4,

    };
    int Changed(const DefenCenterSetting& other) {
        int changed = 0;
        if (mFileMonitorState.mValue != other.mFileMonitorState.mValue) {
            changed |= FILE_DEFEN_CHANGED;
        }
        if (mUSBPluginMonitorState.mValue != other.mUSBPluginMonitorState.mValue) {
            changed |= USB_MONITOR_CHANGED;
        }
        if (mMailMonitorState.mValue != other.mMailMonitorState.mValue) {
            changed |= MAIL_MONITOR_CHANGED;
        }
        return changed;
    }
};


class ActiveDefenConfig {
public:
    LockField<int> mProcDefenState;
    LockField<int> mFileDefenState;
    LockField<int> mPluginDefenState;
    LockField<int> mBrowserDefenState;
    LockField<int> mNetBlockState;

    ActiveDefenConfig() {
        mProcDefenState.mValue = 0;
        mFileDefenState.mValue = 0;
        mPluginDefenState.mValue = 0;
        mBrowserDefenState.mValue = 0;
        mNetBlockState.mValue = 0;
    }
};

#endif // COMMONENTRY_H
