#ifndef STORAGE_BRIDGE_H
#define STORAGE_BRIDGE_H

#include <list>
#include <string>
#include <vector>

class Storage;
class LogEventEntry;
class VirusInfoEntry;
class WhiteListEntry;
class CommEventLog;
class QuarantineLog;
class ActiveDefenConfig;
class ScheduleEntry;
class UrlRuleEntry;
class NetAccessEntry;
class AntiVirusSetting;
class NetfilterSetting;
class BaseSetting;
class DefenCenterSetting;
class ScanConfig;
class UpdateConfig;
class SourceListEntry;
class UpgradeSetting;


class StorageBridge {
public:
    static StorageBridge* NEW(const std::string& installPath, const std::string& typepath);

    StorageBridge();
    ~StorageBridge();

    virtual int Construct(const std::string& installPath, const std::string& typepath);

    int  GetScanLogs(std::list<LogEventEntry>& logs);
    int  GetVirusLogs(std::list<VirusInfoEntry>& logs);
    int  GetNetAccessLog(std::list<NetAccessEntry>& logs);
    int  GetBaseLogs(std::list<CommEventLog>& logs, const CommEventLog& seed);
    int  GetQuaLogs(std::list<QuarantineLog>& logs);
    int  GetQuaLogs(std::list<QuarantineLog>& logs, const QuarantineLog& seed);

    int  GetWhiteList(std::list<WhiteListEntry>& white);
    int  GetProcessBlackList(std::list<WhiteListEntry>& white);
    int  GetFolderMonitorList(std::list<WhiteListEntry>& white);
    int  GetScanFileTypeList(std::list<WhiteListEntry>& lists);
    int  SetScanFileTypeList(std::vector<std::string>& lists);

    int  AddWhiteList(const WhiteListEntry& entry);
    int  DeleteWhitelist(const WhiteListEntry& entry);

    int  GetUrlBlackList(std::list<UrlRuleEntry>& white);
    int  AddUrlBlackList(const UrlRuleEntry& entry);
    int  DeleteUrlBlacklist(const UrlRuleEntry& entry);

    int  AppendQuaLog(const QuarantineLog& log);
    int  AppendScanLog(const LogEventEntry& log);
    int  AppendVirusLog(const VirusInfoEntry& entry);

    int  DeleteQuaLog(const QuarantineLog& log);
    int  UpdateQuaLog(const QuarantineLog& log);

    int AppendNetAccessLog(const NetAccessEntry& entry);

    int GetScanConfig(ScanConfig& config);
    int SaveScanConfig(const ScanConfig& config);

    // 以下两个函数的
    // GetUpgradeSetting/SetUpgradeSetting
    // 如果这两个函数在后期测试成功, 那么相应的这两个函数要删除掉
    int GetUpdateConfig(UpdateConfig& config);
    int SaveUpdateConfig(const UpdateConfig& config);

    int GetUpgradeSetting(UpgradeSetting& setting);
    int SetUpgradeSetting(const UpgradeSetting& setting);

    int GetDefenConfig(ActiveDefenConfig& config);
    int SaveDefenConfig(const ActiveDefenConfig& config);

    int GetAntiVirusConfig(AntiVirusSetting& config, int flag=/*AntiVirusSetting::ALL_SETTING*/0xFF);
    int UpdateAntiVirusConfig(AntiVirusSetting& config);

    int GetNetfilterConfig(NetfilterSetting& config);
    int UpdateNetfilterConfig(NetfilterSetting& config);

    int GetBaseConfig(BaseSetting& config);
    int UpdateBaseConfig(BaseSetting& confi);

    int GetDefenCenterConfig(DefenCenterSetting& config);
    int UpdateDefenCenterConfig(DefenCenterSetting& config);

    bool GetLogedVirus() const;

    bool GetProcDefenceState() const;
    bool GetActivePowerOnState() const;
    bool GetFileDefenceState() const;
    bool GetUSBPluginMonitorState() const;
    bool GetNetFilterState() const;
    bool GetBrowserBlockState() const;

    int GetScheduleEntry(/*ScheduleEntry::ScanType*/int type, ScheduleEntry& entry);
    int SetScheduleEntry(ScheduleEntry& entry);
    int DeleteScheuleType(int type);

    std::string GetInstallPath();
    std::string GetRemoteScanPath();

    bool GetModuleStopToRemoveState();

    int GetTitle(std::string& major, std::string& minor);
    std::string GetAdminPasswd();
    bool GetTrayIconState();
    bool GetAuthPopState();
    bool GetLockGUIDState();
    bool GetCenterProxyState();

    bool GetScanIntelligentState();
    int /**AntiVirusSetting::AntiCommon::ScanType*/ GetScanIntelligentMode(bool& locked);
    bool SetScanIntelligentMode(int /**AntiVirusSetting::AntiCommon::ScanType**/ type);

    int GetSettingsCount();

    long GetVlibUpdateTime();
    bool SetStorageVlibVersion(const std::string& version);
    std::string GetVlibVersion();

    std::string GetStorageAppVersion();
    bool SetStorageAppVersion(const std::string& version);

    std::string GetStorageCenterUrl();
    bool SetStorageCenterUrl(const std::string& url);

    int GetSourceList(std::list<SourceListEntry>& entries, int type);
    int SetSourceList(std::list<SourceListEntry>& entries, int type, bool manual=false);

    int GetReconnectInterval();

    int UpdateSetting(const std::string& name, int value);
protected:
    Storage *mStorage;

protected:
    std::string mInstallDir;
    std::string mOwnerPath;
};

#endif // STORAGE_BRIDGE_H
