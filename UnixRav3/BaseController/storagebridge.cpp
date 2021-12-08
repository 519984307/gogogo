#include "storagebridge.h"
#include "storage.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "cinifile.h"
#include "scanconfig.h"
#include "storage_global.h"
#include "appconfig.h"

#include "Entry/whitelistentry.h"
#include "Entry/quarantinelog.h"
#include "Entry/logevententry.h"
#include "Entry/virusinfoentry.h"
#include "Entry/commonentry.h"
#include "Entry/sourcelistentry.h"
#include "Common/updateconfig.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define ASSERT_ZERO(x, y) if(!x) return y;

#define LOAD_SETTING_DEFAULT(x, y, z) if ((x=y) < 0) x=z;

StorageBridge::StorageBridge() {
    mStorage = NULL;
}

StorageBridge::~StorageBridge() {
    if (NULL != mStorage) {
        delete mStorage;
        mStorage = NULL;
    }
}

StorageBridge* StorageBridge::NEW(const string &installPath, const string &typepath) {
    StorageBridge *self = new StorageBridge;
    if (self) {
        if (self->Construct(installPath, typepath)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int StorageBridge::Construct(const string &installPath, const string& typepath) {
    if (installPath.empty()) {
        mInstallDir = GetInstallPath();
    }
    else {
        mInstallDir = installPath;
    }

    mOwnerPath = typepath;
    string dbName = Utils::Path::JoinPath(installPath.c_str(), mOwnerPath.c_str(), STORAGE_DB_FILE, NULL);
    mStorage = Storage::NEW(dbName);
    ASSERT_ZERO(mStorage, -1);

    chmod(dbName.c_str(), 0777);

    return 0;
}

int StorageBridge::GetSettingsCount() {
    return mStorage->GetSettingsCount();
}


int  StorageBridge::AppendQuaLog(const QuarantineLog& log) {
    return mStorage->InsertQuarantine(log);
}

int  StorageBridge::DeleteQuaLog(const QuarantineLog& log) {
    return mStorage->DeleteQuarantine(log);
}

int  StorageBridge::UpdateQuaLog(const QuarantineLog& log) {
    return mStorage->UpdateQuarantine(log);
}

int  StorageBridge::GetScanLogs(std::list<LogEventEntry>& logs) {
    return mStorage->GetScanLogs(logs);
}

int  StorageBridge::GetVirusLogs(std::list<VirusInfoEntry>& logs) {
    return mStorage->GetVirusLogs(logs);
}

int StorageBridge::GetNetAccessLog(std::list<NetAccessEntry>& logs) {
    return mStorage->GetNetAccessLogs(logs);
}

int  StorageBridge::GetBaseLogs(std::list<CommEventLog>& logs, const CommEventLog& seed) {
    return mStorage->GetCommEventLogs(logs, seed);
}

int  StorageBridge::GetQuaLogs(std::list<QuarantineLog>& logs) {
    return mStorage->GetQuarantineLogs(logs, 0);
}

int StorageBridge::GetQuaLogs(std::list<QuarantineLog> &logs, const QuarantineLog &seed) {
    return mStorage->GetQuarantineLogs(logs, seed);
}

int  StorageBridge::GetWhiteList(std::list<WhiteListEntry>& white) {
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::WHITE;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    return mStorage->GetWhiteList(white, entry);
}

int  StorageBridge::GetProcessBlackList(std::list<WhiteListEntry>& white) {
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::BLACK;
    entry.mBlockType = WhiteListEntry::PROCESS_BLOCK;
    return mStorage->GetWhiteList(white, entry);
}

int  StorageBridge::GetFolderMonitorList(std::list<WhiteListEntry>& white) {
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::BLACK;
    entry.mBlockType = WhiteListEntry::FOLDER_MONITOR;
    return mStorage->GetWhiteList(white, entry);
}

int  StorageBridge::GetScanFileTypeList(std::list<WhiteListEntry>& white) {
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::BLACK;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
    return mStorage->GetWhiteList(white, entry);
}

int  StorageBridge::SetScanFileTypeList(std::vector<std::string>& exts) {
    list<WhiteListEntry> files;
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::BLACK;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
    if (mStorage->GetWhiteList(files, entry) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=files.begin(); iter!=files.end(); ++iter) {
            mStorage->DeleteWhiteList(*iter);
        }
    }

    vector<string>::iterator it;
    for (it=exts.begin();it!=exts.end();++it) {
        WhiteListEntry entry;
        entry.mListType = WhiteListEntry::BLACK;
        entry.mBlockType = WhiteListEntry::FILE_SCAN;
        entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
        entry.mReserved1 = 1;
        entry.mPath = *it;
        mStorage->InsertWhiteList(entry);
    }
    return 0;
}

int  StorageBridge::AddWhiteList(const WhiteListEntry& entry) {
    return mStorage->InsertWhiteList(entry);
}

int  StorageBridge::DeleteWhitelist(const WhiteListEntry& entry) {
    return mStorage->DeleteWhiteList(entry);
}

int  StorageBridge::GetUrlBlackList(std::list<UrlRuleEntry>& white) {
    return mStorage->GetUrlBlackList(white);
}

int  StorageBridge::AddUrlBlackList(const UrlRuleEntry& entry) {
    return mStorage->InserUrlBlackList(entry);
}

int  StorageBridge::DeleteUrlBlacklist(const UrlRuleEntry& entry) {
    return mStorage->DeleteUrlBlackList(entry);
}

int StorageBridge::AppendScanLog(const LogEventEntry& log) {
    return mStorage->InsertScanLog(log);
}

int StorageBridge::AppendVirusLog(const VirusInfoEntry &entry) {
    return mStorage->InsertVirusInfo(entry);
}

int StorageBridge::AppendNetAccessLog(const NetAccessEntry& entry) {
    return mStorage->InsertNetAccessInfo(entry);
}

bool StorageBridge::GetLogedVirus() const {
    int value = 0;
    if (mStorage->GetSettingValue(VIRUS_LOG_FIELD, value)) {
        return bool(value);
    }
    return false;
}

bool StorageBridge::GetProcDefenceState() const {
    bool locked = false;
    return mStorage->GetProcDefenceState(locked);
}

bool StorageBridge::GetFileDefenceState() const {
    bool locked = false;
    return mStorage->GetFileDefenceState(locked);
}

bool StorageBridge::GetActivePowerOnState() const {
    int value = 0;
    if (mStorage->GetSettingValue(FILE_MONITOR_POWERON, value)) {
        return value>0;
    }
    return false;
}

bool StorageBridge::GetUSBPluginMonitorState() const {
    bool locked = false;
    return mStorage->GetUSBPluginMonitorState(locked);
}

bool StorageBridge::GetNetFilterState() const {
    bool locked = false;
    return mStorage->GetNetFilterState(locked);
}


bool StorageBridge::GetBrowserBlockState() const {
    bool locked = false;
    return mStorage->GetBrowserBlockState(locked);
}


std::string StorageBridge::GetInstallPath() {
    if (mInstallDir.empty()) {

    }
    return mInstallDir;
}

std::string StorageBridge::GetRemoteScanPath() {
    return mStorage->GetRemoteScanPath();
}

int StorageBridge::UpdateSetting(const std::string& name, int value) {
    return mStorage->SetSettingValue(name, value);
}

int StorageBridge::GetScheduleEntry(int type, ScheduleEntry& entry) {
    entry.mFuncEnum = (ScheduleEntry::ScanType)type;
    return mStorage->GetScheduleEntry(type, entry);
}

int StorageBridge::SetScheduleEntry(ScheduleEntry& entry) {
    return mStorage->SetScheduleEntry(entry);
}

int StorageBridge::DeleteScheuleType(int type) {
    return mStorage->DeleteScheduleEntry(type);
}

int StorageBridge::GetAntiVirusConfig(AntiVirusSetting& config, int flag) {
    return mStorage->GetAntiVirusSetting(config, flag);
}

int StorageBridge::UpdateAntiVirusConfig(AntiVirusSetting& config) {
    return mStorage->UpdateAntiVirusSetting(config);
}

int StorageBridge::GetNetfilterConfig(NetfilterSetting& config) {
    return 0;
}

int StorageBridge::UpdateNetfilterConfig(NetfilterSetting& config) {
    return 0;
}

int StorageBridge::GetBaseConfig(BaseSetting& config) {
    return mStorage->GetCommonBaseSetting(config);
}

int StorageBridge::UpdateBaseConfig(BaseSetting& config) {
    return mStorage->UpdateCommonBaseSetting(config);
}

int StorageBridge::GetDefenCenterConfig(DefenCenterSetting& config) {
    return mStorage->GetDefenCenterSetting(config);
}

int StorageBridge::UpdateDefenCenterConfig(DefenCenterSetting& config) {
    return mStorage->UpdateDefenCenterSetting(config);
}

int StorageBridge::GetScanConfig(ScanConfig& config) {
    bool locked = false;
    try {
        LOAD_SETTING_DEFAULT(config.mVirusAction, (VIRUS_DEAL)mStorage->GetVirusAction(locked), AUTO_DEAL);
        LOAD_SETTING_DEFAULT(config.mCleanFailedAction,(CLEAN_FAILED)mStorage->GetCleanFailedAction(locked), CLEAN_FAILED_IGNORE);
        LOAD_SETTING_DEFAULT(config.mBackupFailedAction, (VIRUS_BACKUP_FAILED)mStorage->GetBackupFailedAction(locked), BACKUP_FAILED_QUERY);
        LOAD_SETTING_DEFAULT(config.mBackupVirusFile, (VIRUS_BACKUP)mStorage->GetBackupVirusFile(locked), BACKUP_ORIGIN);
        LOAD_SETTING_DEFAULT(config.mLargeVirusAction, (LARGE_FILE)mStorage->GetLargeVirusAction(locked), LARGE_FILE_QUERY);
        LOAD_SETTING_DEFAULT(config.mScanFileType, (SCAN_TYPE)mStorage->GetScanFileType(locked), ALL_FILE);
        LOAD_SETTING_DEFAULT(config.mScanCommonVirus, mStorage->GetCommonVirusState(locked), 0);
        LOAD_SETTING_DEFAULT(config.mScanIntelligent, mStorage->GetIntelligentScanState(locked), 0);
        LOAD_SETTING_DEFAULT(config.mScanCompressFile, mStorage->GetCompressScanState(locked), 0);
        LOAD_SETTING_DEFAULT(config.mSpaceLessAction, (SPACE_LESS)mStorage->GetBackupSpaceLessAction(locked), OVERRIDE_OLD);
        LOAD_SETTING_DEFAULT(config.mCompressSize, mStorage->GetScanCompressFileSize(locked), 100);

		mStorage->GetSettingValue(CLOUD_ENGINE_ENABLE, config.mCloudEnable);
		mStorage->GetSettingValue(CLOUD_ENGINE_URL,    config.mCloudUrl);

        mStorage->GetSettingValue(SCAN_COMMON_TYPE, config.mCPUPayloadPercent);
        mStorage->GetSettingValue(ENV_INTELLIGENT, config.mIntelligentMode);

        ScanIntelligentParams param;
        if (config.mIntelligentMode) {            
            param.Detail(config.mCPUPayloadPercent);
        }
        else {
            AntiVirusSetting::AntiCommon::ScanType type = (AntiVirusSetting::AntiCommon::ScanType)GetScanIntelligentMode(locked);
            param.Detail(type);
        }
        config.mThreadCount = param.thread_count;
        config.mCPUPayloadPercent = param.cpuload_precent;

        mStorage->GetSettingValue(RESCAN_ACCELERATE, config.mUseCache);

        bool state = false;;
        mStorage->GetSettingValue(PUBLIC_CLOUD_STATE, state);
        if (state) {
            list<SourceListEntry> sources;
            if (mStorage->GetSourceList(sources, SourceListEntry::CLOUD_SCAN_SOURCE) > 0) {
                list<SourceListEntry>::iterator iter;
                CloudConfig cloud;
                for (iter=sources.begin();iter!=sources.end();++iter) {
                    if (iter->mState) {
                        cloud.mAddr = iter->mAddress;
                        cloud.mPort = iter->mPort;
                        cloud.mPostCount = iter->mOncePostfileCount;
                        cloud.mRequestType = iter->mRequestModel;

                        config.mCloudAddr.push_back(cloud);
                    }
                }
            }
        }
    }
    catch(...) {
        return -1;
    }

    return 0;
}

int StorageBridge::SaveScanConfig(const ScanConfig &config) {
    try {
        mStorage->SetVirusAction((int)config.mVirusAction, false);
        mStorage->SetCleanFailedAction((int)config.mCleanFailedAction, false);
        mStorage->SetBackupFailedAction((int)config.mBackupFailedAction, false);
        mStorage->SetBackupVirusFile((int)config.mBackupVirusFile, false);
        mStorage->SetLargeVirusAction((int)config.mLargeVirusAction, false);
        mStorage->SetScanFileType((int)config.mScanFileType, false);
        mStorage->SetCommonVirusState((int)config.mScanCommonVirus, false);
        mStorage->SetIntelligentScanState((int)config.mScanIntelligent, false);
        mStorage->SetCompressScanState((int)config.mScanCompressFile, false);
        mStorage->SetBackupSpaceLessAction((int)config.mSpaceLessAction, false);
        mStorage->SetScanCompressFileSize(config.mCompressSize, false);
    }catch(...) {
        return -1;
    }
    return 0;
}

int StorageBridge::GetUpdateConfig(UpdateConfig& config) {
    bool locked = false;
    try {
        LOAD_SETTING_DEFAULT(config.mUpdateMode, (UPGRADE_MODE)mStorage->GetUpdateMode(locked), UPGRADE_MANUALLY);
        LOAD_SETTING_DEFAULT(config.mDelayStartAction, (DELAY_START)mStorage->GetDelayStartAction(locked), DELAY_DISABLE);
        LOAD_SETTING_DEFAULT(config.mUpdateContent, (UPDATE_CONTENT)mStorage->GetUpdateContent(locked), UPDATE_ALL);
        LOAD_SETTING_DEFAULT(config.mProxySetting,(PROXY_SET)mStorage->GetProxySetting(locked),CONNECT_DIRECT);
        LOAD_SETTING_DEFAULT(config.mUpdateSourceAction, (UPDATE_SOURCE)mStorage->GetUpdateSourceAction(locked), OFFICAL_HTTP);
        config.mUpdateModeExtend = mStorage->GetUpdateModeExtend(locked);
        config.mDelayStartTime = mStorage->GetDelayStartTime(locked);
        config.mProxyAddr = mStorage->GetProxyAddr(locked);
        config.mProxyPort = mStorage->GetProxyPort(locked);
        config.mProxyAccount = mStorage->GetProxyAccount(locked);
        config.mProxyPasswd = mStorage->GetProxyPassword(locked);
    }
    catch(...) {
        return -1;
    }

    return 0;
}

int StorageBridge::SaveUpdateConfig(const UpdateConfig &config) {
    try {
        mStorage->SetUpdateMode((int)config.mUpdateMode);
        mStorage->SetDelayStartAction((int)config.mDelayStartAction);
        mStorage->SetUpdateContent((int)config.mUpdateContent);
        mStorage->SetProxySetting((int)config.mProxySetting);
        mStorage->SetUpdateSourceAction((int)config.mUpdateSourceAction);
        mStorage->SetUpdateModeExtend(config.mUpdateModeExtend);
        mStorage->SetDelayStartTime(config.mDelayStartTime);
        mStorage->SetProxyAddr(config.mProxyAddr);
        mStorage->SetProxyPort(config.mProxyPort);
        mStorage->SetProxyAccount(config.mProxyAccount);
        mStorage->SetProxyPassword(config.mProxyPasswd);
    }
    catch(...) {
        return -1;
    }
    return 0;
}

int StorageBridge::GetDefenConfig(ActiveDefenConfig& config) {
    try {
        LOAD_SETTING_DEFAULT(config.mProcDefenState.mValue, mStorage->GetProcDefenceState(config.mProcDefenState.mLocked), 0);
        LOAD_SETTING_DEFAULT(config.mFileDefenState.mValue, mStorage->GetFileDefenceState(config.mFileDefenState.mLocked), 0);
        LOAD_SETTING_DEFAULT(config.mPluginDefenState.mValue, mStorage->GetUSBPluginMonitorState(config.mPluginDefenState.mLocked), 0);
        LOAD_SETTING_DEFAULT(config.mBrowserDefenState.mValue, mStorage->GetBrowserBlockState(config.mBrowserDefenState.mLocked), 0);
        LOAD_SETTING_DEFAULT(config.mNetBlockState.mValue, mStorage->GetNetFilterState(config.mNetBlockState.mLocked), 0);
    }
    catch(...) {
        return -1;
    }

    return 0;
}

int StorageBridge::SaveDefenConfig(const ActiveDefenConfig& config) {
    ActiveDefenConfig origin;
    if (GetDefenConfig(origin)) {
        return -1;
    }

    try {
        if (origin.mProcDefenState.mValue != config.mProcDefenState.mValue
                || origin.mProcDefenState.mLocked != config.mProcDefenState.mLocked) {
            mStorage->SetProcDefenceSate((int)config.mProcDefenState.mValue, (int)config.mProcDefenState.mLocked);
        }
        if (origin.mFileDefenState.mValue != config.mFileDefenState.mValue
                || origin.mFileDefenState.mLocked != config.mFileDefenState.mLocked) {
            mStorage->SetFileDefenceSate((int)config.mFileDefenState.mValue, (int)config.mFileDefenState.mLocked);
        }
        if (origin.mPluginDefenState.mValue != config.mPluginDefenState.mValue
                || origin.mPluginDefenState.mLocked != config.mPluginDefenState.mLocked) {
            mStorage->SetUSBPluginMonitorSate((int)config.mPluginDefenState.mValue, (int)config.mFileDefenState.mLocked);
        }
        if (origin.mBrowserDefenState.mValue != config.mBrowserDefenState.mValue
                || origin.mBrowserDefenState.mLocked != config.mBrowserDefenState.mLocked) {
            mStorage->SetBrowserBlockSate((int)config.mBrowserDefenState.mValue, (int)config.mFileDefenState.mLocked);
        }
        if (origin.mNetBlockState.mValue != config.mNetBlockState.mValue
                || origin.mNetBlockState.mLocked != config.mNetBlockState.mLocked) {
            mStorage->SetNetFilterSate((int)config.mNetBlockState.mValue, (int)config.mFileDefenState.mLocked);
        }
    }
    catch(...) {
        return -1;
    }
    return 0;
}

bool StorageBridge::GetModuleStopToRemoveState() {
    bool locked = false;
    return mStorage->GetModuleStopToRemoveState(locked);
}

int StorageBridge::GetTitle(std::string& major, std::string& minor) {
    return mStorage->GetTitle(major, minor);
}

std::string StorageBridge::GetAdminPasswd() {
    string passwd;
    mStorage->GetSettingValue(ADMIN_PASSWORD, passwd);
    return passwd;
}

bool StorageBridge::GetTrayIconState() {
    return mStorage->GetTrayIconState();
}

bool StorageBridge::GetAuthPopState() {
    bool state = false;
    mStorage->GetSettingValue(SHOW_AUTH_STATE, state);
    return state;
}

bool StorageBridge::GetLockGUIDState() {
    bool state = false;
    mStorage->GetSettingValue(CLIENT_LOCK_GUID, state);
    return state;
}
bool StorageBridge::GetCenterProxyState() {
    bool state = false;
    mStorage->GetSettingValue(CENTER_PROXY_STATE, state);
    return state;
}

bool StorageBridge::GetScanIntelligentState() {
    int state = 0;
    mStorage->GetSettingValue(ENV_INTELLIGENT, state);
    return state>0;
}

int StorageBridge::GetScanIntelligentMode(bool& locked) {
    int state = 0;
    mStorage->GetSettingValue(SCAN_COMMON_TYPE, state, locked);
    return state;
}

bool StorageBridge::SetScanIntelligentMode(int type) {
    return mStorage->SetSettingValue(SCAN_COMMON_TYPE, type);
}

long StorageBridge::GetVlibUpdateTime() {
    int last = 0;
    mStorage->GetSettingValue(VLIB_UPDATE_TIME, last);
    return last;
}

std::string StorageBridge::GetVlibVersion() {
    string version;
    mStorage->GetSettingValue(VLIB_UPDATE_VERSION, version);
    return version;
}

bool StorageBridge::SetStorageVlibVersion(const std::string& version) {
    return mStorage->SetSettingValue(VLIB_UPDATE_VERSION, version);
}

std::string StorageBridge::GetStorageAppVersion() {
    string version;
    mStorage->GetSettingValue(APP_VERSION_ITEM, version);
    return version;
}

std::string StorageBridge::GetStorageCenterUrl() {
    string version;
    mStorage->GetSettingValue(CENTER_URL_ITEM, version);
    return version;
}

bool StorageBridge::SetStorageAppVersion(const std::string& version) {
    return mStorage->SetSettingValue(APP_VERSION_ITEM, version);
}

bool StorageBridge::SetStorageCenterUrl(const std::string& url) {
    return mStorage->SetSettingValue(CENTER_URL_ITEM, url);
}

int StorageBridge::GetSourceList(std::list<SourceListEntry>& entries, int type) {
    return mStorage->GetSourceList(entries, type);
}

int StorageBridge::SetSourceList(std::list<SourceListEntry>& entries, int type, bool manual) {
    std::list<SourceListEntry> olds;
    GetSourceList(olds, type);

    std::list<SourceListEntry>::iterator iter;
    for (iter=olds.begin(); iter!=olds.end(); ++iter) {
        if (manual && iter->mOffical == 0) {
            mStorage->DeleteSourceList(*iter);
        }
    }

    for (iter=entries.begin(); iter!=entries.end(); ++iter) {
        if (iter->mOffical == 0) {
            mStorage->InsertSourceList(*iter);
        }
    }
    return 0;
}

int StorageBridge::GetReconnectInterval() {
    int value = 0;
    if (mStorage->GetSettingValue(RECONNECT_INTERVAL, value)) {
        return value;
    }
    return -1;
}

int StorageBridge::GetUpgradeSetting(UpgradeSetting& config) {
    try {
        config.mUpdateMode.mValue = (UpgradeSetting::UPGRADE_MODE)mStorage->GetUpdateMode(config.mUpdateMode.mLocked);
        config.mDelayStartAction.mValue = (UpgradeSetting::DELAY_START)mStorage->GetDelayStartAction(config.mDelayStartAction.mLocked);
        config.mUpdateContent.mValue = (UpgradeSetting::UPDATE_CONTENT)mStorage->GetUpdateContent(config.mUpdateContent.mLocked);
        config.mProxySetting.mValue = (UpgradeSetting::PROXY_SET)mStorage->GetProxySetting(config.mProxySetting.mLocked);
        config.mUpdateSourceAction.mValue = (UpgradeSetting::UPDATE_SOURCE)mStorage->GetUpdateSourceAction(config.mUpdateSourceAction.mLocked);
        config.mUpdateModeExtend.mValue = mStorage->GetUpdateModeExtend(config.mUpdateModeExtend.mLocked);
        config.mDelayStartTime.mValue = mStorage->GetDelayStartTime(config.mDelayStartTime.mLocked);
        config.mProxyAddr.mValue = mStorage->GetProxyAddr(config.mProxyAddr.mLocked);
        config.mProxyPort.mValue = mStorage->GetProxyPort(config.mProxyPort.mLocked );
        config.mProxyAccount.mValue = mStorage->GetProxyAccount(config.mProxyAccount.mLocked);
        config.mProxyPasswd.mValue = mStorage->GetProxyPassword(config.mProxyPasswd.mLocked);
        mStorage->GetSettingValue(UPDATE_NET_BAND_LIMIT, config.mBandLimit.mValue, config.mBandLimit.mLocked);
        mStorage->GetSettingValue(UPDATE_BAND_WIDTH, config.mBandWidth.mValue, config.mBandWidth.mLocked);
        mStorage->GetSettingValue(UPDATE_NET_LIMIT_START, config.mStartTime.mValue, config.mStartTime.mLocked);
        mStorage->GetSettingValue(UPDATE_NET_LIMIT_END, config.mEndTime.mValue, config.mEndTime.mLocked);
        mStorage->GetSettingValue(UPDATE_FORCE_DELAY_DAY, config.mForceUpdateDelay.mValue, config.mForceUpdateDelay.mLocked);
        mStorage->GetSettingValue(UPDATE_CLEAR_SYS_CACHE, config.mClearUpdateCache.mValue, config.mClearUpdateCache.mLocked);
        mStorage->GetSettingValue(VIRUS_LIB_UPDATE_INTIME, config.mCheckVirusDbRealTime.mValue, config.mCheckVirusDbRealTime.mLocked);
        mStorage->GetSettingValue(UPDATE_FORCE_SWITCH, config.mForceUpdate.mValue, config.mForceUpdate.mLocked);
    }
    catch(...) {
        return -1;
    }

    return 0;
}

int StorageBridge::SetUpgradeSetting(const UpgradeSetting& config) {
    try {
        mStorage->SetUpdateMode((int)config.mUpdateMode.mValue);
        mStorage->SetDelayStartAction((int)config.mDelayStartAction.mValue);
        mStorage->SetUpdateContent((int)config.mUpdateContent.mValue);
        mStorage->SetProxySetting((int)config.mProxySetting.mValue);
        mStorage->SetUpdateSourceAction((int)config.mUpdateSourceAction.mValue);
        mStorage->SetUpdateModeExtend(config.mUpdateModeExtend.mValue);
        mStorage->SetDelayStartTime(config.mDelayStartTime.mValue);
        mStorage->SetProxyAddr(config.mProxyAddr.mValue);
        mStorage->SetProxyPort(config.mProxyPort.mValue);
        mStorage->SetProxyAccount(config.mProxyAccount.mValue);
        mStorage->SetProxyPassword(config.mProxyPasswd.mValue);
        mStorage->SetSettingValue(UPDATE_NET_BAND_LIMIT, config.mBandLimit.mValue, config.mBandLimit.mLocked);
        mStorage->SetSettingValue(UPDATE_MAX_SPEED_KB, config.mBandWidth.mValue, config.mBandWidth.mLocked);
        mStorage->SetSettingValue(UPDATE_NET_LIMIT_START, config.mStartTime.mValue, config.mStartTime.mLocked);
        mStorage->SetSettingValue(UPDATE_NET_LIMIT_END, config.mEndTime.mValue, config.mEndTime.mLocked);
        mStorage->SetSettingValue(UPDATE_FORCE_DELAY_DAY, config.mForceUpdateDelay.mValue, config.mForceUpdateDelay.mLocked);
        mStorage->SetSettingValue(UPDATE_CLEAR_SYS_CACHE, config.mClearUpdateCache.mValue, config.mClearUpdateCache.mLocked);
        mStorage->SetSettingValue(VIRUS_LIB_UPDATE_INTIME, config.mCheckVirusDbRealTime.mValue, config.mCheckVirusDbRealTime.mLocked);
        mStorage->SetSettingValue(UPDATE_FORCE_SWITCH, config.mForceUpdate.mValue, config.mForceUpdate.mLocked);
    }
    catch(...) {
        return -1;
    }

    return 0;

}
