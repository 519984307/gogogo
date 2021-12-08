#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <list>

#include "storage_global.h"
#include "Entry/commonentry.h"

using namespace std;

class CppSQLite3DB;
class LogEventEntry;
class VirusInfoEntry;
class CommEventLog;
class QuarantineLog;
class WhiteListEntry;
class ScheduleEntry;
class UrlRuleEntry;
class NetAccessEntry;
class SourceListEntry;

class Storage
{
public:
    static Storage *NEW(const std::string& db);
    ~Storage();

private:
    Storage();
    int Construct(const std::string& db);

public:
    bool   IsFirstCreate() const {
        return mIsFirstCreated;
    }

    int InitDefaultValue(const string& configfile);
    int GetSettingsCount();

    string GetSGUID();
    bool   SetSGUID(const string& sguid);

    string GetGUID();
    bool   SetGUID(const string& guid);

    string GetRemoteScanPath();
    bool   SetRemoteScanPath(const string& guid);

    int GetScanLogs(list<LogEventEntry>& logs, int last=0);
    int GetScanLogs(LogEventEntry& logs, int id);
    int InsertScanLog(const LogEventEntry& log);
    int UpdateScanLog(const LogEventEntry& log);
    int DeleteScanLog(const LogEventEntry& log);

    int GetQuarantineLogs(list<QuarantineLog>& logs, int page=1);
    int GetQuarantineLogs(list<QuarantineLog> &logs, const QuarantineLog& seed);
    int InsertQuarantine(const QuarantineLog& log);
    int DeleteQuarantine(const QuarantineLog& log);
    int UpdateQuarantine(const QuarantineLog& log);

    int GetCommEventLogs(list<CommEventLog>& logs, const CommEventLog& seed);
    int InsertCommEvent(const CommEventLog& log);
    int DeleteCommEvent(const CommEventLog& log);
    int UpdateCommEvent(const CommEventLog& log);

    int GetVirusLogs(list<VirusInfoEntry>& logs, int scanid=0, int page=1);
    int InsertVirusInfo(const VirusInfoEntry& log);
    int UpdateVirusInfo(const VirusInfoEntry& log);
    int DeleteVirusInfo(const VirusInfoEntry& log);

    int GetNetAccessLogs(list<NetAccessEntry>& logs, int page=1);
    int InsertNetAccessInfo(const NetAccessEntry& log);
    int DeleteNetAccessInfo(const NetAccessEntry& log);

    int GetWhiteList(list<WhiteListEntry>& white, const WhiteListEntry& seed);
    int GetWhiteList(list<WhiteListEntry>& white, int/* WhiteListEntry::LIST_TYPE */ type);
    int InsertWhiteList(const WhiteListEntry& white);
    int DeleteWhiteList(const WhiteListEntry& white);
    int UpdateWhiteList(const WhiteListEntry& white);

    int GetUrlBlackList(list<UrlRuleEntry>& white);
    int InserUrlBlackList(const UrlRuleEntry& white);
    int DeleteUrlBlackList(const UrlRuleEntry& white);
    int UpdateUrlBlackList(const UrlRuleEntry& white);

    int GetAvailableScheduleList(list<ScheduleEntry> &list);
    int GetScheduleList(list<ScheduleEntry>& white, int type=0);
    int InsertScheduleList(const ScheduleEntry& white);
    int DeleteScheduleList(const ScheduleEntry& white);
    int UpdateScheduleList(const ScheduleEntry& white);
    int UpdateSchedule(const ScheduleEntry& entry);
    int UpdateCommonCloudSetting(map<ScheduleEntry::ScanType, ScheduleEntry>& cloud, ScheduleEntry::ScanType type);

    int GetSourceList(list<SourceListEntry>& entries, int type=0);
    int GetSourceList(list<SourceListEntry>& entries, const SourceListEntry& entry);
    int InsertSourceList(const SourceListEntry& entry);
    int DeleteSourceList(const SourceListEntry& entry);
    int UpdateSourceList(const SourceListEntry& entry);

    int GetAntiVirusSetting(AntiVirusSetting& setting, int flag);
    int UpdateAntiVirusSetting(AntiVirusSetting& setting);

    int GetNetFilterSetting(NetfilterSetting& setting);
    int UpdateNetFilterSetting(const NetfilterSetting& setting);

    int GetCommonBaseSetting(BaseSetting& setting);
    int UpdateCommonBaseSetting(BaseSetting& setting);

    int GetDefenCenterSetting(DefenCenterSetting& setting);
    int UpdateDefenCenterSetting(DefenCenterSetting& setting);

    int GetVirusAction(bool &locked);
    void SetVirusAction(int action, bool locked=false);

    int GetCleanFailedAction(bool &locked);
    void SetCleanFailedAction(int action, bool locked=false);

    int GetBackupFailedAction(bool &locked);
    void SetBackupFailedAction(int action, bool locked=false);

    bool GetBackupVirusFile(bool &locked);
    void SetBackupVirusFile(bool backup, bool locked=false);

    int GetLargeVirusAction(bool &locked);
    void SetLargeVirusAction(int action, bool locked=false);

    int GetScanFileType(bool &locked);
    void SetScanFileType(int action, bool locked=false);

    int GetScanEngineType(bool &locked);
    void SetScanEngineType(int action, bool locked=false);

    int GetCommonVirusState(bool &locked);
    void SetCommonVirusState(int action, bool locked=false);

    int GetIntelligentScanState(bool &locked);
    void SetIntelligentScanState(int action, bool locked=false);

    int GetCompressScanState(bool &locked);
    void SetCompressScanState(int action, bool locked=false);

    int GetBackupSpaceLessAction(bool &locked);
    void SetBackupSpaceLessAction(int action, bool locked=false);

    int GetScanCompressFileSize(bool &locked);
    void SetScanCompressFileSize(int action, bool locked=false);

    int GetScanCompressFileDepth(bool &locked);
    void SetScanCompressFileDepth(int depth, bool locked=false);

    int GetUpdateMode(bool &locked);
    void SetUpdateMode(int action, bool locked=false);

    int GetDelayStartAction(bool &locked);
    void SetDelayStartAction(int action, bool locked=false);

    int GetUpdateContent(bool &locked);
    void SetUpdateContent(int action, bool locked=false);

    int GetProxySetting(bool &locked);
    void SetProxySetting(int action, bool locked=false);

    int GetUpdateSourceAction(bool &locked);
    void SetUpdateSourceAction(int action, bool locked=false);

    std::string GetUpdateModeExtend(bool &locked);
    void SetUpdateModeExtend(std::string action, bool locked=false);

    int GetDelayStartTime(bool &locked);
    void SetDelayStartTime(int action, bool locked=false);

    std::string GetProxyAddr(bool &locked);
    void SetProxyAddr(std::string action, bool locked=false);

    std::string GetProxyPort(bool &locked);
    void SetProxyPort(std::string action, bool locked=false);

    std::string GetProxyAccount(bool &locked);
    void SetProxyAccount(std::string action, bool locked=false);

    std::string GetProxyPassword(bool &locked);
    void SetProxyPassword(std::string action, bool locked=false);

    std::string GetUpdateSource(bool &locked);
    void SetUpdateSource(std::string action, bool locked=false);

    bool GetProcDefenceState(bool &locked);
    void SetProcDefenceSate(bool state, bool locked=false);

    bool GetFileDefenceState(bool &locked);
    void SetFileDefenceSate(bool state, bool locked=false);

    bool GetUSBPluginMonitorState(bool &locked);
    void SetUSBPluginMonitorSate(bool state, bool locked=false);

    int GetUSBPluginDefaultState(bool &locked);
    void SetUSBPluginDefaultState(int state, bool locked=false);

    bool GetNetFilterState(bool &locked);
    void SetNetFilterSate(bool state, bool locked=false);

    bool GetBrowserBlockState(bool &locked);
    void SetBrowserBlockSate(bool state, bool locked=false);

    int GetScheduleEntry(int type, ScheduleEntry& entry);
    int SetScheduleEntry(ScheduleEntry& entry);
    int DeleteScheduleEntry(int type);

    int GetConnectCenterList(vector<string>& center);
    int GetCenterProxyState(bool &locked);

    bool GetModuleStopToRemoveState(bool &locked);

    bool GetTrayIconState();
    int  GetTitle(string& major, string& minor);

public:
    bool GetSettingValue(const string& name, int& value);
    bool GetSettingValue(const string& name, bool& value);
    bool GetSettingValue(const string& name, string& value);

    bool GetSettingValue(const string& name, int& value, bool& locked);
    bool GetSettingValue(const string& name, bool& value, bool& locked);
    bool GetSettingValue(const string& name, string& value, bool& locked);

    bool SetSettingValue(const string& name, int value, bool locked=false);
    bool SetSettingValue(const string& name, const string& value, bool locked=false);

    bool GetLogEventLogs(list<LogEventEntry>& logs, const LogEventEntry& seed);

private:
    CppSQLite3DB    *mDatabase;
    bool    mIsFirstCreated;
};

#endif // STORAGE_H
