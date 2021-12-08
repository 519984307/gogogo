#include "storage.h"
#include "cinifile.h"
#include "commonapi.h"
#include "CPPSQLite/cppsqlite3.h"
#include "Entry/settingentry.h"
#include "Entry/logevententry.h"
#include "Entry/virusinfoentry.h"
#include "Entry/scheduleentry.h"
#include "Entry/whitelistentry.h"
#include "Entry/quarantinelog.h"
#include "Entry/urlruleentry.h"
#include "Entry/netaccessentry.h"
#include "Entry/sourcelistentry.h"
#include "Entry/commeventlog.h"

#include <map>

#define UPDATE_SETTING(name, value) do {\
                                            if (!SetSettingValue(name, value.mValue, value.mLocked)) { \
                                                return -1; \
                                            } \
                                          } while(0);
#define UPDATE_SETTING_QUICK(name, origin, value) do { \
                                            if (origin.mValue != value.mValue || origin.mLocked != value.mLocked) { \
                                                if (!SetSettingValue(name, value.mValue, value.mLocked)) { \
                                                    return -1; \
                                                } \
                                            } \
                                        } while(0);

#define GET_SETTING(name, value)  do {\
                                            GetSettingValue(name, value.mValue, value.mLocked); \
                                        }while(0);

template<typename T>
inline bool ArrayCompare(list<T>& source, list<T>& dest) {
    if (source.size() != dest.size()) {
        return false;
    }

    typename list<T>::iterator siter, diter;
    for (siter=source.begin(), diter=dest.begin(); siter!=source.end() && diter!=dest.end(); ++siter,++diter) {
        if (!(*siter == *diter)) {
            return false;
        }
    }
    return true;
}

inline bool ArrayCompare(list<pair<string,int> >& source, list<pair<string,int> >& dest) {
    if (source.size() != dest.size()) {
        return false;
    }

    list<pair<string,int> >::iterator siter, diter;
    for (siter=source.begin(), diter=dest.begin(); siter!=source.end() && diter!=dest.end(); ++siter,++diter) {
        if (!(siter->first == diter->first && siter->second == diter->second)) {
            return false;
        }
    }
    return true;
}

Storage* Storage::NEW(const std::string& db) {
    Storage *self = new Storage;
    if (self) {
        if (self->Construct(db)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

Storage::Storage() {
    mDatabase = NULL;
    mIsFirstCreated = false;
}

Storage::~Storage() {
    mDatabase->close();
    if (mDatabase) {
        delete mDatabase;
    }
}

int Storage::Construct(const std::string& db) {
    try {
        mDatabase = new CppSQLite3DB;
        mDatabase->open(db.c_str());

        CppSQLite3Query query = mDatabase->execQuery("select count(*) from sqlite_master where type='table' and name='SettingTable'");
        while (!query.eof()) {
            int count = query.getIntField(0, 0);
            if (count > 0) {
                // table has exist
                return 0;
            }
            query.nextRow();
        }

        mIsFirstCreated = true;
        SettingEntry setting;
        mDatabase->execDML(setting.CreateTable().c_str());

        LogEventEntry log;
        mDatabase->execDML(log.CreateTable().c_str());

        VirusInfoEntry virus;
        mDatabase->execDML(virus.CreateTable().c_str());

        ScheduleEntry schedule;
        mDatabase->execDML(schedule.CreateTable().c_str());

        WhiteListEntry white;
        mDatabase->execDML(white.CreateTable().c_str());

        UrlRuleEntry urlrule;
        mDatabase->execDML(urlrule.CreateTable().c_str());

        QuarantineLog entry;
        mDatabase->execDML(entry.CreateTable().c_str());

        NetAccessEntry netaccess;
        mDatabase->execDML(netaccess.CreateTable().c_str());

        CommEventLog eventlog;
        mDatabase->execDML(eventlog.CreateTable().c_str());

        SourceListEntry sourceEntry;
        mDatabase->execDML(sourceEntry.CreateTable().c_str());

        // init the defalut setting

    }
    catch(...) {
        return -1;
    }

    return 0;
}

int Storage::GetSettingsCount() {
    CppSQLite3Query query = mDatabase->execQuery("select count(*) from SettingTable");
    while (!query.eof()) {
        return query.getIntField(0, 0);
    }
    return -1;
}

bool Storage::GetSettingValue(const string& name, int& value) {
    bool locked = false;
    return GetSettingValue(name, value, locked);
}

bool Storage::GetSettingValue(const string& name, bool& value) {
    bool locked = false;
    return GetSettingValue(name, value, locked);
}

bool Storage::GetSettingValue(const string& name, string& value) {
    bool locked = false;
    return GetSettingValue(name, value, locked);
}

bool Storage::GetSettingValue(const string& name, int& value, bool& locked) {
    SettingEntry entry;
    entry.mType = SettingEntry::INTEGER;
    entry.mName = name;

    CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
    while (!query.eof()) {
        entry.Mapping(&query);
        value = entry.mValue.mIntegerValue;
        locked = entry.mAvailed;

        return true;
    }
    value = 0;
    return false;
}

bool Storage::GetSettingValue(const string& name, bool& value, bool& locked) {
    int temp = 0;
    bool err = GetSettingValue(name, temp, locked);
    value = (bool)temp;
    return err;
}

bool Storage::GetSettingValue(const string& name, string& value, bool& locked) {
    SettingEntry entry;
    entry.mType = SettingEntry::STRING;
    entry.mName = name;

    CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
    while (!query.eof()) {
        entry.Mapping(&query);
        value = entry.mValue.mStringValue;
        locked = entry.mAvailed;

        return !value.empty();
    }
    return false;
}

bool  Storage::SetSettingValue(const string& name, int value, bool locked) {
    SettingEntry entry;
    entry.mName = name;
    entry.mType = SettingEntry::INTEGER;
    entry.mValue.mIntegerValue = value;
    entry.mAvailed = locked;

    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        if (query.eof()) {
            mDatabase->execDML(entry.Insert().c_str());
        }
        else {
            entry.Mapping(&query);

            entry.mType = SettingEntry::INTEGER;
            entry.mValue.mIntegerValue = value;
            entry.mAvailed = locked;

            mDatabase->execDML(entry.Update().c_str());
        }
        return true;
    }
    catch(...) {

    }

    return false;
}

bool Storage::SetSettingValue(const string& name, const string& value, bool locked) {
    SettingEntry entry;
    entry.mName = name;
    entry.mType = SettingEntry::STRING; 
    strcpy(entry.mValue.mStringValue, value.c_str());
    entry.mAvailed = locked;

    //printf("SetSettingValue: %s\n", entry.Select().c_str());
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        if (query.eof()) {
            mDatabase->execDML(entry.Insert().c_str());
        }
        else {
            entry.Mapping(&query);

            entry.mType = SettingEntry::STRING;
            strcpy(entry.mValue.mStringValue, value.c_str());
            entry.mAvailed = locked;

            mDatabase->execDML(entry.Update().c_str());
        }
        return true;
    }
    catch(...) {

    }

    return false;
}

bool Storage::GetLogEventLogs(list<LogEventEntry>& logs, const LogEventEntry& seed) {
    try {
        CppSQLite3Query query = mDatabase->execQuery(seed.Select().c_str());
        while (!query.eof()) {
            LogEventEntry log;
            log.Mapping(&query);
            logs.push_back(log);

            query.nextRow();
        }

        return true;
    }
    catch(...) {

    }
    return false;
}

int Storage::GetScanLogs(list<LogEventEntry>& logs, int last) {
    LogEventEntry entry;
    entry.mLogType = LogEventEntry::SCAN_LOG;
    entry.mID = last;

    bool err = GetLogEventLogs(logs, entry);
    return err?logs.size():-1;
}

int Storage::GetScanLogs(LogEventEntry& log, int id) {
    log.mLogType = LogEventEntry::SPECIAL;
    log.mID = id;

    try {
        CppSQLite3Query query = mDatabase->execQuery(log.Select().c_str());
        while (!query.eof()) {
            log.Mapping(&query);
            break;
        }

        return 0;
    }
    catch(...) {

    }
    return -1;
}

int Storage::InsertScanLog(const LogEventEntry& log) {
    try {
        if (SQLITE_OK == mDatabase->execDML(log.Insert().c_str())) {
            CppSQLite3Query query = mDatabase->execQuery(log.Last().c_str());
            while (!query.eof()) {
                return query.getIntField(0, 0);
            }
        }
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateScanLog(const LogEventEntry& log) {
    try {
        return mDatabase->execDML(log.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::DeleteScanLog(const LogEventEntry& log) {
    try {
        return mDatabase->execDML(log.Delete().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetQuarantineLogs(list<QuarantineLog> &logs, const QuarantineLog &seed) {
    try {
        CppSQLite3Query query = mDatabase->execQuery(seed.Select().c_str());
        while (!query.eof()) {
            QuarantineLog log;
            log.Mapping(&query);
            logs.push_back(log);

            query.nextRow();
        }

        return logs.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetQuarantineLogs(list<QuarantineLog>& logs, int page) {
    QuarantineLog entry;
    return GetQuarantineLogs(logs, entry);
}

int Storage::InsertQuarantine(const QuarantineLog& log) {
    try {
        return mDatabase->execDML(log.Insert().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::DeleteQuarantine(const QuarantineLog& log) {
    try {
        return mDatabase->execDML(log.Delete().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateQuarantine(const QuarantineLog& log) {
    try {
        return mDatabase->execDML(log.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetVirusLogs(list<VirusInfoEntry>& logs, int scanid, int page) {
    VirusInfoEntry entry;
    entry.mScanLogID = scanid;
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        while (!query.eof()) {
            VirusInfoEntry log;
            log.Mapping(&query);
            logs.push_back(log);

            query.nextRow();
        }

        return logs.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::InsertVirusInfo(const VirusInfoEntry& log) {
    try {
        return mDatabase->execDML(log.Insert().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateVirusInfo(const VirusInfoEntry& log) {
    try {
        return mDatabase->execDML(log.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::DeleteVirusInfo(const VirusInfoEntry& log) {
    return 0;
}

int Storage::GetNetAccessLogs(list<NetAccessEntry>& logs, int page) {
    NetAccessEntry entry;
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        while (!query.eof()) {
            NetAccessEntry log;
            log.Mapping(&query);
            logs.push_back(log);

            query.nextRow();
        }

        return logs.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::InsertNetAccessInfo(const NetAccessEntry& log) {
    try {
        return mDatabase->execDML(log.Insert().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::DeleteNetAccessInfo(const NetAccessEntry& log) {
    return 0;
}



int Storage::GetCommEventLogs(list<CommEventLog>& logs, const CommEventLog& seed) {
    try {
        CppSQLite3Query query = mDatabase->execQuery(seed.Select().c_str());
        while (!query.eof()) {
            CommEventLog log;
            log.Mapping(&query);
            logs.push_back(log);

            query.nextRow();
        }

        return logs.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::InsertCommEvent(const CommEventLog& log) {
    try {
        return mDatabase->execDML(log.Insert().c_str());
    }
    catch(CppSQLite3Exception e) {

    }
    return -1;
}

int Storage::DeleteCommEvent(const CommEventLog& log) {
    try {
        return mDatabase->execDML(log.Delete().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateCommEvent(const CommEventLog &log) {
    try {
        return mDatabase->execDML(log.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetWhiteList(list<WhiteListEntry>& whites, const WhiteListEntry& seed) {
    try {
        CppSQLite3Query query = mDatabase->execQuery(seed.Select().c_str());
        while (!query.eof()) {
            WhiteListEntry log;
            log.Mapping(&query);
            whites.push_back(log);

            query.nextRow();
        }

        return whites.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetWhiteList(list<WhiteListEntry>& whites, int type) {
    WhiteListEntry feed;
    feed.mListType = (WhiteListEntry::LIST_TYPE)type;
    feed.mBlockType = WhiteListEntry::FILE_SCAN;

    list<WhiteListEntry> child;
    for (int i=WhiteListEntry::FILE_PATH; i<=WhiteListEntry::FOLDER_CHILDERN; ++i) {
        whites.clear();
        feed.mPathType = (WhiteListEntry::PathType)i;
        GetWhiteList(child, feed);
        whites.merge(whites);
    }
    return whites.size();
}

int Storage::InsertWhiteList(const WhiteListEntry& white) {
    try {
        return mDatabase->execDML(white.Insert().c_str());
    }
    catch(CppSQLite3Exception e) {

    }
    return -1;
}

int Storage::DeleteWhiteList(const WhiteListEntry& white) {
    try {
        return mDatabase->execDML(white.Delete().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateWhiteList(const WhiteListEntry& white) {
    try {
        return mDatabase->execDML(white.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetUrlBlackList(list<UrlRuleEntry>& whites) {
    UrlRuleEntry entry;
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        while (!query.eof()) {
            UrlRuleEntry log;
            log.Mapping(&query);
            whites.push_back(log);

            query.nextRow();
        }

        return whites.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::InserUrlBlackList(const UrlRuleEntry& white) {
    try {
        return mDatabase->execDML(white.Insert().c_str());
    }
    catch(...) {
        return -1;
    }
    return 0;
}

int Storage::DeleteUrlBlackList(const UrlRuleEntry& white) {
    try {
        return mDatabase->execDML(white.Delete().c_str());
    }
    catch(...) {
        return -1;
    }
    return 0;
}

int Storage::UpdateUrlBlackList(const UrlRuleEntry& white) {
    try {
        return mDatabase->execDML(white.Update().c_str());
    }
    catch(...) {}
    return -1;
}

int Storage::GetAvailableScheduleList(list<ScheduleEntry> &available) {
    int value = 0;
    bool locked = false;
    GetSettingValue(TIME_QUICK_SCAN_FLAG, value, locked);
    if (value > 0) {
        GetScheduleList(available, ScheduleEntry::QUICK_SCAN);
    }

    GetSettingValue(TIME_FULL_SCAN_FLAG, value, locked);
    if (value > 0) {
        GetScheduleList(available, ScheduleEntry::ALL_SCAN);
    }
    GetSettingValue(TIME_LINUX_SCAN_FLAG, value, locked);
    if (value > 0) {
        GetScheduleList(available, ScheduleEntry::LINUX_SCAN);
    }

    GetScheduleList(available, ScheduleEntry::UPDATE);


//    int value = 0;
//    GetSettingValue(TIME_QUICK_SCAN_FLAG, value);
//    if (value > 0) {
//        list<ScheduleEntry> entries;
//        if (GetScheduleList(entries, ScheduleEntry::QUICK_SCAN, false) <= 0) {
//            GetScheduleList(entries, ScheduleEntry::QUICK_SCAN, true);
//        }

//        available.merge(entries);
//    }



//    GetSettingValue(TIME_FULL_SCAN_FLAG, value);
//    if (value > 0) {
//        list<ScheduleEntry> entries;
//        if (GetScheduleList(entries, ScheduleEntry::ALL_SCAN, false) <= 0) {
//            GetScheduleList(entries, ScheduleEntry::ALL_SCAN, true);
//        }
//        available.merge(entries);
//    }

//    GetSettingValue(TIME_LINUX_SCAN_FLAG, value);
//    if (value > 0) {
//        list<ScheduleEntry> entries;
//        if (GetScheduleList(entries, ScheduleEntry::LINUX_SCAN, false) <= 0) {
//            GetScheduleList(entries, ScheduleEntry::LINUX_SCAN, true);
//        }
//        available.merge(entries);
//    }

//    list<ScheduleEntry> entries;
//    if (GetScheduleList(entries, ScheduleEntry::UPDATE, false) <= 0) {
//        GetScheduleList(entries, ScheduleEntry::UPDATE, true);
//    }
//    available.merge(entries);
    return 0;
}

int Storage::GetScheduleList(list<ScheduleEntry>& schedule, int type) {
    ScheduleEntry entry;
    entry.mFuncEnum = (ScheduleEntry::ScanType)type;
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        while (!query.eof()) {
            ScheduleEntry log;
            log.Mapping(&query);
            schedule.push_back(log);

            query.nextRow();
        }

        return schedule.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::InsertScheduleList(const ScheduleEntry& schedule) {
    try {
        return mDatabase->execDML(schedule.Insert().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::DeleteScheduleList(const ScheduleEntry& schedule) {
    try {
        return mDatabase->execDML(schedule.Delete().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateScheduleList(const ScheduleEntry& schedule) {
    try {
        return mDatabase->execDML(schedule.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetScheduleEntry(int type, ScheduleEntry& entry) {
    entry.mFuncEnum = (ScheduleEntry::ScanType)type;
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        while (!query.eof()) {
            entry.Mapping(&query);
            return 1;
        }
    }
    catch(...) {

    }
    return -1;
}

int Storage::SetScheduleEntry(ScheduleEntry& entry) {
    ScheduleEntry temp;
    if (GetScheduleEntry(entry.mFuncEnum, temp) < 0) {
        return InsertScheduleList(entry);
    }
    else {
        entry.mID = temp.mID;
        return UpdateScheduleList(entry);
    }
}

int Storage::DeleteScheduleEntry(int type) {
    ScheduleEntry temp;
    if (GetScheduleEntry(type, temp) > 0) {
        return DeleteScheduleList(temp);
    }
}


int Storage::GetSourceList(list<SourceListEntry>& entries, const SourceListEntry& entry) {
    try {
        CppSQLite3Query query = mDatabase->execQuery(entry.Select().c_str());
        while (!query.eof()) {
            SourceListEntry log;
            log.Mapping(&query);
            entries.push_back(log);

            query.nextRow();
        }

        return entries.size();
    }
    catch(...) {

    }
    return -1;
}

int Storage::GetSourceList(list<SourceListEntry>& entries, int type) {
    SourceListEntry entry;
    entry.mType = (SourceListEntry::SOURCE_TYPE)type;
    return GetSourceList(entries, entry);
}

int Storage::InsertSourceList(const SourceListEntry& entry) {
    try {
        return mDatabase->execDML(entry.Insert().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::DeleteSourceList(const SourceListEntry& entry) {
    try {
        return mDatabase->execDML(entry.Delete().c_str());
    }
    catch(...) {

    }
    return -1;
}

int Storage::UpdateSourceList(const SourceListEntry& entry) {
    try {
        return mDatabase->execDML(entry.Update().c_str());
    }
    catch(...) {

    }
    return -1;
}

string Storage::GetSGUID() {
    string sguid;
    bool state = GetSettingValue(SGUID_TAG, sguid);
    return state?sguid:"";
}

bool Storage::SetSGUID(const string &sguid) {
    return SetSettingValue(SGUID_TAG, sguid, true);
}

string Storage::GetGUID() {
    string guid;
    bool state = GetSettingValue(GUID_TAG, guid);
    return state?guid:"";
}

bool Storage::SetGUID(const string& guid) {
    return SetSettingValue(GUID_TAG, guid, true);
}

string Storage::GetRemoteScanPath() {
    string value;
    bool state = GetSettingValue(REMOTE_SCAN_PATH, value);
    return state?value:"";
}

bool Storage::SetRemoteScanPath(const string& guid) {
    return SetSettingValue(REMOTE_SCAN_PATH, guid);
}

int Storage::GetVirusAction(bool &locked) {
    int value = -1;
    GetSettingValue(SCAN_VIRUS_ACTION, value);
    return value;
}

void Storage::SetVirusAction(int action, bool locked) {
    SetSettingValue(SCAN_VIRUS_ACTION, action, locked);
}

int Storage::GetCleanFailedAction(bool &locked) {
    int value = -1;
    GetSettingValue(CLEAN_FAIL_ACTION, value, locked);
    return value;
}

void Storage::SetCleanFailedAction(int action, bool locked) {
    SetSettingValue(CLEAN_FAIL_ACTION, action, locked);
}

int Storage::GetBackupFailedAction(bool &locked) {
    int value = -1;
    GetSettingValue(BACKUP_FAIL_ACTION, value, locked);
    return value;
}

void Storage::SetBackupFailedAction(int action, bool locked) {
    SetSettingValue(BACKUP_FAIL_ACTION, action, locked);
}

bool Storage::GetBackupVirusFile(bool &locked) {
    int value = -1;
    GetSettingValue(BACKUP_VIRUS_STATE, value, locked);
    return value;
}

void Storage::SetBackupVirusFile(bool backup, bool locked) {
    SetSettingValue(BACKUP_VIRUS_STATE, backup, locked);
}

int Storage::GetLargeVirusAction(bool &locked) {
    int value = -1;
    GetSettingValue(LARGE_VIRUS_ACTION, value, locked);
    return value;
}

void Storage::SetLargeVirusAction(int action, bool locked) {
    SetSettingValue(LARGE_VIRUS_ACTION, action, locked);
}

int Storage::GetScanFileType(bool &locked) {
    int value = -1;
    GetSettingValue(SCAN_FILE_TYPE, value, locked);
    return value;
}

void Storage::SetScanFileType(int action, bool locked) {
    SetSettingValue(SCAN_FILE_TYPE, action, locked);
}

int Storage::GetCommonVirusState(bool &locked) {
    int value = -1;
    GetSettingValue(SCAN_COMMON_VIRUS, value, locked);
    return value;
}
void Storage::SetCommonVirusState(int action, bool locked) {
    SetSettingValue(SCAN_COMMON_VIRUS, action, locked);
}

int Storage::GetIntelligentScanState(bool &locked) {
    int value = -1;
    GetSettingValue(SCAN_INTELLIGENT, value, locked);
    return value;
}
void Storage::SetIntelligentScanState(int action, bool locked) {
    SetSettingValue(SCAN_INTELLIGENT, action, locked);
}

int Storage::GetCompressScanState(bool &locked) {
    int value = -1;
    GetSettingValue(SCAN_COMPRESS_STATE, value, locked);
    return value;
}
void Storage::SetCompressScanState(int action, bool locked) {
    SetSettingValue(SCAN_COMPRESS_STATE, action, locked);
}


int Storage::GetBackupSpaceLessAction(bool &locked) {
    int value = -1;
    GetSettingValue(BACKUP_SPACE_LESS, value, locked);
    return value;
}

void Storage::SetBackupSpaceLessAction(int action, bool locked) {
    SetSettingValue(BACKUP_SPACE_LESS, action, locked);
}

int Storage::GetScanCompressFileSize(bool &locked) {
    int value = -1;
    GetSettingValue(COMPRESS_FILE_SIZE, value, locked);
    return value;
}

void Storage::SetScanCompressFileSize(int action, bool locked) {
    SetSettingValue(COMPRESS_FILE_SIZE, action, locked);
}

int Storage::GetScanCompressFileDepth(bool &locked) {
    int value = -1;
    GetSettingValue(COMPRESS_FILE_DEPTH, value, locked);
    return value;
}

void Storage::SetScanCompressFileDepth(int depth, bool locked) {
    SetSettingValue(COMPRESS_FILE_DEPTH, depth, locked);
}

int Storage::GetUpdateMode(bool &locked) {
    int value = -1;
    GetSettingValue(UPDATE_MODE, value, locked);
    return value;
}

void Storage::SetUpdateMode(int action, bool locked) {
    SetSettingValue(UPDATE_MODE, action, locked);
}

int Storage::GetDelayStartAction(bool &locked) {
    int value = -1;
    GetSettingValue(UPDATE_DELAY_START, value, locked);
    return value;
}

void Storage::SetDelayStartAction(int action, bool locked) {
    SetSettingValue(UPDATE_DELAY_START, action, locked);
}

int Storage::GetUpdateContent(bool &locked) {
    int value = -1;
    GetSettingValue(UPDATE_CONTENT_ACTION, value, locked);
    return value;
}

void Storage::SetUpdateContent(int action, bool locked) {
    SetSettingValue(UPDATE_CONTENT_ACTION, action, locked);
}

int Storage::GetProxySetting(bool &locked) {
    int value = -1;
    GetSettingValue(UPDATE_PROXY_ACTION, value, locked);
    return value;
}

void Storage::SetProxySetting(int action, bool locked) {
    SetSettingValue(UPDATE_PROXY_ACTION, action, locked);
}

int Storage::GetUpdateSourceAction(bool &locked) {
    int value = -1;
    GetSettingValue(UPDATE_SOURCE_ACTION, value, locked);
    return value;
}

void Storage::SetUpdateSourceAction(int action, bool locked) {
    SetSettingValue(UPDATE_SOURCE_ACTION, action);
}

std::string Storage::GetUpdateModeExtend(bool &locked) {
    string sguid;
    bool state = GetSettingValue(UPDATE_EXTEND_DATA, sguid, locked);
    return state?sguid:"";
}

void Storage::SetUpdateModeExtend(std::string action, bool locked) {
    SetSettingValue(UPDATE_EXTEND_DATA, action, locked);
}

int Storage::GetDelayStartTime(bool &locked) {
    int data;
    bool state = GetSettingValue(UPDATE_DELAY_TIME, data, locked);
    return state?data:0;
}

void Storage::SetDelayStartTime(int data, bool locked) {
    SetSettingValue(UPDATE_DELAY_TIME, data, locked);
}

std::string Storage::GetProxyAddr(bool &locked) {
    string data;
    bool state = GetSettingValue(UPDATE_PROXY_ADDR, data, locked);
    return state?data:"";
}

void Storage::SetProxyAddr(std::string data, bool locked) {
    SetSettingValue(UPDATE_PROXY_ADDR, data, locked);
}

std::string Storage::GetProxyPort(bool &locked) {
    int data;
    bool state = GetSettingValue(UPDATE_PROXY_PORT, data, locked);
    return state?Utils::String::itoa(data):"";
}

void Storage::SetProxyPort(std::string data, bool locked) {
    SetSettingValue(UPDATE_PROXY_PORT, data, locked);
}

std::string Storage::GetProxyAccount(bool &locked) {
    string data;
    bool state = GetSettingValue(UPDATE_PROXY_ACCOUNT, data, locked);
    return state?data:"";
}

void Storage::SetProxyAccount(std::string data, bool locked) {
    SetSettingValue(UPDATE_PROXY_ACCOUNT, data, locked);
}

std::string Storage::GetProxyPassword(bool &locked) {
    string data;
    bool state = GetSettingValue(UPDATE_PROXY_PASSWD, data, locked);
    return state?data:"";
}

void Storage::SetProxyPassword(std::string data, bool locked) {
    SetSettingValue(UPDATE_PROXY_PASSWD, data, locked);
}

std::string Storage::GetUpdateSource(bool &locked) {
    string data;
    bool state = GetSettingValue(UPDATE_SOURCE_EXTEND, data, locked);
    return state?data:"";
}

void Storage::SetUpdateSource(std::string data, bool locked) {
    SetSettingValue(UPDATE_SOURCE_EXTEND, data, locked);
}

bool Storage::GetProcDefenceState(bool &locked) {
    int value = 0;
    if (GetSettingValue(ACTIVE_DEFENCE_SWITCH, value, locked)) {
        return value > 0;
    }
    return false;
}

void Storage::SetProcDefenceSate(bool state, bool locked) {
    SetSettingValue(ACTIVE_DEFENCE_SWITCH, (int)state, locked);
}

bool Storage::GetFileDefenceState(bool &locked) {
    int value = 0;
    if (GetSettingValue(FILE_DEFEN_SWITCH, value, locked)) {
        return value > 0;
    }
    return false;
}

void Storage::SetFileDefenceSate(bool state, bool locked) {
    SetSettingValue(FILE_DEFEN_SWITCH, (int)state, locked);
}

bool Storage::GetUSBPluginMonitorState(bool &locked) {
    int value = 0;
    if (GetSettingValue(U_PLUGIN_MONITOR, value, locked)) {
        return value > 0;
    }
    return false;
}
void Storage::SetUSBPluginMonitorSate(bool state, bool locked) {
    SetSettingValue(U_PLUGIN_MONITOR, (int)state, locked);
}

int Storage::GetUSBPluginDefaultState(bool &locked) {
    int value = 0;
    if (GetSettingValue(USB_PROMPT_STATE_SWITCH, value, locked)) {
        return value;
    }
    return 0;
}

void Storage::SetUSBPluginDefaultState(int state, bool locked) {
    SetSettingValue(USB_PROMPT_STATE_SWITCH, (int)state, locked);
}


bool Storage::GetNetFilterState(bool &locked) {
    int value = 0;
    if (GetSettingValue(NETFILTER_SWITCH, value, locked)) {
        return value > 0;
    }
    return false;
}

void Storage::SetNetFilterSate(bool state, bool locked) {
    SetSettingValue(NETFILTER_SWITCH, (int)state, locked);
}

bool Storage::GetBrowserBlockState(bool &locked) {
    int value = 0;
    if (GetSettingValue(BROWSER_BLOCK_SWITCH, value, locked)) {
        return value > 0;
    }
    return false;
}

void Storage::SetBrowserBlockSate(bool state, bool locked) {
    SetSettingValue(BROWSER_BLOCK_SWITCH, (int)state, locked);
}

bool Storage::GetModuleStopToRemoveState(bool& locked) {
    int value = 0;
    if (GetSettingValue(MODULE_STOP_TO_REMOVE, value, locked)) {
        return value > 0;
    }
    return false;
}

bool Storage::GetTrayIconState() {
    int value = 0;
    bool locked = false;
    // 这个值读取的是图标的状态，但是从策略里下发的是隐藏图标的状态，所以返回值应该和读取的值是相反的
    if (GetSettingValue(TRAY_ICON_STATE, value, locked)) {
        return value == 0;
    }
    return true;
}

int Storage::GetTitle(string& major, string& minor) {
    GetSettingValue(CLIENT_TITLE, major);
    GetSettingValue(CLIENT_SUB_TITLE, minor);
    return 0;
}

int Storage::GetAntiVirusSetting(AntiVirusSetting& setting, int flag) {
    if ((flag & AntiVirusSetting::ANTI_COMMON) > 0) {
        GET_SETTING(ENV_INTELLIGENT,    setting.mCommon.mRunEnvJudge);
        GET_SETTING(VIRUS_TRACE,        setting.mCommon.mVirusTrace);
        GET_SETTING(VIRUS_LOG_FIELD,    setting.mCommon.mVirusLoged);
        GET_SETTING(RESCAN_ACCELERATE,  setting.mCommon.mRescanAccelerate);
        GET_SETTING(FILE_ACCELERATE,    setting.mCommon.mFileAccelerate);
        GET_SETTING(SCAN_COMMON_TYPE,   setting.mCommon.mScanType);

        GET_SETTING(PRIVATE_CLOUD_STATE, setting.mCommon.mUsePrivateCloud);
        GET_SETTING(PUBLIC_CLOUD_STATE, setting.mCommon.mUsePublicCloud);

        // cloud source
        {
            list<SourceListEntry> entries;
            GetSourceList(entries, SourceListEntry::CLOUD_SCAN_SOURCE);

            list<SourceListEntry>::iterator iter;
            for (iter=entries.begin();iter!=entries.end();++iter) {
                AntiVirusSetting::AntiCommon::CloudModel model;
                model.mServerName = iter->mName;
                model.mServerAddr = iter->mAddress;
                model.mPort = iter->mPort;
                model.mDBID = iter->mID;
                model.mState = iter->mState;

                setting.mCommon.mCloudSetting.push_back(model);
            }
        }
    }

    if ((flag & AntiVirusSetting::WHITE_LIST) > 0) {
        // white list
        WhiteListEntry feed;
        feed.mListType = WhiteListEntry::WHITE;
        feed.mBlockType = WhiteListEntry::FILE_SCAN;
        feed.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
        list<WhiteListEntry> whites;
        GetWhiteList(whites, feed);

        list<WhiteListEntry>::iterator iter;
        for (iter=whites.begin();iter!=whites.end();++iter) {
            setting.mWhiteList.mExtTypes.push_back(iter->mPath);
        }

        for (int i=WhiteListEntry::FILE_PATH; i<=WhiteListEntry::FOLDER_CHILDERN; ++i) {
            whites.clear();
            feed.mPathType = (WhiteListEntry::PathType)i;
            GetWhiteList(whites, feed);
            for (iter=whites.begin();iter!=whites.end();++iter) {
                setting.mWhiteList.mFilePaths.push_back(make_pair(iter->mPath, i));
            }
        }
    }

    if ((flag & AntiVirusSetting::BLACK_LIST) > 0) {
        // black list
        WhiteListEntry feed;
        list<WhiteListEntry> whites;

        list<WhiteListEntry>::iterator iter;

        feed.mListType = WhiteListEntry::BLACK;
        feed.mBlockType = WhiteListEntry::FILE_SCAN;
        for (int i=WhiteListEntry::FILE_PATH; i<=WhiteListEntry::FOLDER_CHILDERN; ++i) {
            whites.clear();
            feed.mPathType = (WhiteListEntry::PathType)i;
            GetWhiteList(whites, feed);
            for (iter=whites.begin();iter!=whites.end();++iter) {
                setting.mBlackList.mFilePaths.push_back(make_pair(iter->mPath, i));
            }
        }
    }

    if ((flag & AntiVirusSetting::VIRUS_BACKUP) > 0) {
        //
        GET_SETTING(BACKUP_VIRUS_STATE,     setting.mVirusBackup.mVirusBackup);
        GET_SETTING(LARGE_VIRUS_ACTION,     setting.mVirusBackup.mLargeFile);
        GET_SETTING(BACKUP_SPACE_LESS,      setting.mVirusBackup.mSpaceLess);
        GET_SETTING(BACKUP_FAIL_ACTION,     setting.mVirusBackup.mBackupFailed);
    }

    if ((flag & AntiVirusSetting::VIRUS_SCAN) > 0) {
        //
        GET_SETTING(SCAN_FILE_TYPE,         setting.mVirusScan.mFileType);
        GET_SETTING(SCAN_COMMON_VIRUS,      setting.mVirusScan.mScanCommonVirus);
        GET_SETTING(SCAN_INTELLIGENT,       setting.mVirusScan.mScanIntelligent);
        GET_SETTING(SCAN_COMPRESS_STATE,    setting.mVirusScan.mScanCompressFile);
        GET_SETTING(COMPRESS_FILE_SIZE,     setting.mVirusScan.mZipFileSize);
        GET_SETTING(SCAN_VIRUS_ACTION,      setting.mVirusScan.mVirusDeal);
        GET_SETTING(CLEAN_FAIL_ACTION,      setting.mVirusScan.mCleanFailed);

        GET_SETTING(CLOUD_ENGINE_ENABLE,    setting.mCloudEngine.mEnable);
        GET_SETTING(CLOUD_ENGINE_URL,       setting.mCloudEngine.mUrl);
    }

    if ((flag & AntiVirusSetting::TIME_SCAN) > 0) {
        // time scan
        GET_SETTING(TIME_QUICK_SCAN_FLAG,   setting.mTimeScan.mQuickScan);
        GET_SETTING(TIME_FULL_SCAN_FLAG,    setting.mTimeScan.mFullScan);

        {
            list<ScheduleEntry> entries;
            GetScheduleList(entries, ScheduleEntry::QUICK_SCAN);
            GetScheduleList(entries, ScheduleEntry::ALL_SCAN);

            list<ScheduleEntry>::iterator iter;
            for (iter=entries.begin();iter!=entries.end();++iter) {
                setting.mTimeScan.mValue.insert(pair<ScheduleEntry::ScanType, ScheduleEntry>(iter->mFuncEnum, *iter));
            }
        }
    }

    if ((flag & AntiVirusSetting::FILE_MONITOR) > 0) {
        GET_SETTING(FILE_MONITOR_POWERON,   setting.mFileMonitor.mStartPowerOn);
        GET_SETTING(FILE_INTELLIGENT_BL,    setting.mFileMonitor.mIntelligentBlacklist);
        GET_SETTING(MONITOR_KERNEL_STATE,   setting.mFileMonitor.mKernelMonitor);
        GET_SETTING(MONITOR_WATCH_MODE,     setting.mFileMonitor.mMonitorType);
        GET_SETTING(FILE_MONITOR_FILE_TYPE, setting.mFileMonitor.mFileType);
        GET_SETTING(FILE_MONITOR_ACCELERATE, setting.mFileMonitor.mMonitorAccelerate);
        GET_SETTING(FILE_EMBED_SCAN,        setting.mFileMonitor.mInnerScan);
        GET_SETTING(FILE_MONITOR_COMMON_VIRUS,    setting.mFileMonitor.mScanCommonVirus);
        GET_SETTING(FILE_MONITOR_SCAN_INTELLIGENT,    setting.mFileMonitor.mScanIntelligent);
        GET_SETTING(FILE_MONITOR_SCAN_ZIP,  setting.mFileMonitor.mZipScan);
        GET_SETTING(FILE_MONITOR_ZIP_SIZE,  setting.mFileMonitor.mZipFileSize);
        GET_SETTING(FILE_MONITOR_VIRUS,     setting.mFileMonitor.mVirusDeal);
        GET_SETTING(FILE_MONITOR_CLEAN_FAIL, setting.mFileMonitor.mCleanFailed);
        GET_SETTING(FILE_MONITOR_KILL_PROMPT, setting.mFileMonitor.mCleanPrompt);
        GET_SETTING(FILE_MONITOR_CLEAR_CACHE,setting.mFileMonitor.mResetKernelCache);        
        GET_SETTING(MONITOR_DEF_TYPE, setting.mFileMonitor.mMonitorExtsType);
        GET_SETTING(MONITOR_DEF_EXTS, setting.mFileMonitor.mMonitorTypeDefineExts);
        GET_SETTING(FILE_MONITOR_FILE_EXTS, setting.mFileMonitor.mFileTypeDefineExts);


        WhiteListEntry entry;
        entry.mListType = WhiteListEntry::BLACK;
        entry.mBlockType = WhiteListEntry::FOLDER_MONITOR;
        std::list<WhiteListEntry> white;
        if (GetWhiteList(white, entry) > 0) {
            std::list<WhiteListEntry>::iterator iter;
            for (iter=white.begin(); iter!=white.end(); ++iter) {
                setting.mFileMonitor.mMonitorPaths.mValue.push_back(iter->mPath);
            }
        }
    }

    if ((flag & AntiVirusSetting::PLUGIN_MONITOR) > 0) {
        GET_SETTING(U_PLUGIN_MONITOR,       setting.mPluginMonitor.mState);
        GET_SETTING(U_PLUGIN_INSERT,        setting.mPluginMonitor.mPluginDeal);
        GET_SETTING(U_PLUGIN_SCAN_DEPTH,    setting.mPluginMonitor.mScanDepth);
    }

    return 0;
}

int Storage::UpdateCommonCloudSetting(map<ScheduleEntry::ScanType, ScheduleEntry>& cloud, ScheduleEntry::ScanType type) {
    map<ScheduleEntry::ScanType, ScheduleEntry>::iterator iter;
    iter = cloud.find(type);
    if (iter != cloud.end()) {
        list<ScheduleEntry> entries;
        if (GetScheduleList(entries, type) <= 0) {
            return InsertScheduleList(iter->second);
        }
        else {
            list<ScheduleEntry>::iterator it;
            for (it=entries.begin();it!=entries.end();++it) {
                if (!(*it == iter->second)) {
                    iter->second.mID = it->mID;
                    return UpdateScheduleList(iter->second);
                }
            }
        }
    }
    else {
        DeleteScheduleEntry(type);
    }

    return -1;
}

int Storage::UpdateAntiVirusSetting(AntiVirusSetting& setting) {
    AntiVirusSetting origin;
    GetAntiVirusSetting(origin, AntiVirusSetting::ALL_SETTING);

    UPDATE_SETTING_QUICK(ENV_INTELLIGENT,     origin.mCommon.mRunEnvJudge,      setting.mCommon.mRunEnvJudge);
    UPDATE_SETTING_QUICK(VIRUS_TRACE,         origin.mCommon.mVirusTrace,       setting.mCommon.mVirusTrace);
    UPDATE_SETTING_QUICK(VIRUS_LOG_FIELD,     origin.mCommon.mVirusLoged,       setting.mCommon.mVirusLoged);
    UPDATE_SETTING_QUICK(SCAN_COMMON_TYPE,    origin.mCommon.mScanType,         setting.mCommon.mScanType);
    UPDATE_SETTING_QUICK(RESCAN_ACCELERATE,   origin.mCommon.mRescanAccelerate, setting.mCommon.mRescanAccelerate);
    UPDATE_SETTING_QUICK(FILE_ACCELERATE,     origin.mCommon.mFileAccelerate,   setting.mCommon.mFileAccelerate);
    UPDATE_SETTING_QUICK(PUBLIC_CLOUD_STATE,  origin.mCommon.mUsePublicCloud,   setting.mCommon.mUsePublicCloud);
    UPDATE_SETTING_QUICK(PRIVATE_CLOUD_STATE, origin.mCommon.mUsePrivateCloud,  setting.mCommon.mUsePrivateCloud);

    // cloud source
    if (!ArrayCompare<AntiVirusSetting::AntiCommon::CloudModel>(origin.mCommon.mCloudSetting, setting.mCommon.mCloudSetting)) {
        list<SourceListEntry> origin;
        GetSourceList(origin, SourceListEntry::CLOUD_SCAN_SOURCE);

        list<int> will_delete;
        make_db_setting_consistent<SourceListEntry, AntiVirusSetting::AntiCommon::CloudModel>(origin, setting.mCommon.mCloudSetting, will_delete);

        list<int>::iterator dter;
        for (dter=will_delete.begin(); dter!=will_delete.end(); ++dter) {
            SourceListEntry entry;
            entry.mID = *dter;
            DeleteSourceList(entry);
        }

        map<string, int> newMap;
        list<AntiVirusSetting::AntiCommon::CloudModel>::iterator iter;
        for (iter=setting.mCommon.mCloudSetting.begin(); iter!=setting.mCommon.mCloudSetting.end(); iter++) {
            if (iter->mDBID == 0) {
                SourceListEntry entry;
                entry.mType = SourceListEntry::CLOUD_SCAN_SOURCE;
                entry.mName = iter->mServerName;
                entry.mAddress = iter->mServerAddr;
                entry.mPort = iter->mPort;
                entry.mOffical = 0;
                entry.mState = iter->mState;

                InsertSourceList(entry);
            }
        }
    }

    // white list
    if (!ArrayCompare<string>(origin.mWhiteList.mExtTypes, setting.mWhiteList.mExtTypes)) {
        map<string, bool> newPool;
        list<string>::iterator iter;
        for (iter=setting.mWhiteList.mExtTypes.begin(); iter!=setting.mWhiteList.mExtTypes.end(); ++iter) {
            newPool.insert(make_pair(*iter, true));

        }

        list<WhiteListEntry> origin;
        map<string, bool> originPool;
        {
            WhiteListEntry seed;
            seed.mListType = WhiteListEntry::WHITE;
            seed.mBlockType = WhiteListEntry::FILE_SCAN;
            seed.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
            if (GetWhiteList(origin, seed) > 0) {
                list<WhiteListEntry>::iterator iter;
                for (iter=origin.begin(); iter!=origin.end();) {
                    if (newPool.find(iter->mPath) == newPool.end()) {
                        // not found, delete it
                        DeleteWhiteList(*iter);
                        origin.erase(iter++);
                    }
                    else {                        
                        originPool.insert(make_pair(iter->mPath, true));
                        iter++;
                    }
                }
            }
        }

        for (iter=setting.mWhiteList.mExtTypes.begin(); iter!=setting.mWhiteList.mExtTypes.end(); ++iter) {
            if (originPool.find(*iter) == originPool.end()) {
                WhiteListEntry entry;
                entry.mListType = WhiteListEntry::WHITE;
                entry.mBlockType = WhiteListEntry::FILE_SCAN;
                entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
                entry.mPath = *iter;
                InsertWhiteList(entry);
            }
        }
    }

    if (!ArrayCompare(origin.mWhiteList.mFilePaths, setting.mWhiteList.mFilePaths)) {
        map<string, int> newPool;
        list<pair<string,int> >::iterator iter;
        for (iter=setting.mWhiteList.mFilePaths.begin(); iter!=setting.mWhiteList.mFilePaths.end(); ++iter) {
            newPool.insert(make_pair(iter->first, iter->second));

        }

        list<WhiteListEntry> origin;
        map<string, bool> originPool;
        {
            WhiteListEntry seed;
            seed.mListType = WhiteListEntry::WHITE;
            seed.mBlockType = WhiteListEntry::FILE_SCAN;
            if (GetWhiteList(origin, seed) > 0) {
                list<WhiteListEntry>::iterator iter;
                for (iter=origin.begin(); iter!=origin.end();) {
                    map<string, int>::iterator it = newPool.find(iter->mPath);
                    if (it == newPool.end()) {
                        // not found, delete it
                        DeleteWhiteList(*iter);
                        origin.erase(iter++);
                    }
                    else {
                        if (it->second == iter->mPathType) {                            
                            originPool.insert(make_pair(iter->mPath, true));
                            iter++;
                        }
                        else {
                            DeleteWhiteList(*iter);
                            origin.erase(iter++);
                        }
                    }
                }
            }
        }

        for (iter=setting.mWhiteList.mFilePaths.begin(); iter!=setting.mWhiteList.mFilePaths.end(); ++iter) {
            if (originPool.find(iter->first) == originPool.end()) {
                WhiteListEntry entry;
                entry.mListType = WhiteListEntry::WHITE;
                entry.mBlockType = WhiteListEntry::FILE_SCAN;
                entry.mPathType = (WhiteListEntry::PATH_TYPE)iter->second;
                entry.mPath = iter->first;
                InsertWhiteList(entry);
            }
        }
    }

    // black list
    if (!ArrayCompare(origin.mBlackList.mFilePaths, setting.mBlackList.mFilePaths)) {
        map<string, int> newPool;
        list<pair<string,int> >::iterator iter;
        for (iter=setting.mBlackList.mFilePaths.begin(); iter!=setting.mBlackList.mFilePaths.end(); ++iter) {
            newPool.insert(make_pair(iter->first, iter->second));

        }

        list<WhiteListEntry> origin;
        map<string, bool> originPool;
        {
            WhiteListEntry seed;
            seed.mListType = WhiteListEntry::BLACK;
            seed.mBlockType = WhiteListEntry::FILE_SCAN;
            if (GetWhiteList(origin, seed) > 0) {
                list<WhiteListEntry>::iterator iter;
                for (iter=origin.begin(); iter!=origin.end();) {
                    map<string, int>::iterator it = newPool.find(iter->mPath);
                    if (it == newPool.end()) {
                        // not found, delete it
                        DeleteWhiteList(*iter);
                        origin.erase(iter++);
                    }
                    else {
                        if (it->second == iter->mPathType) {
                            originPool.insert(make_pair(iter->mPath, true));
                            iter++;
                        }
                        else {
                            DeleteWhiteList(*iter);
                            origin.erase(iter++);
                        }
                    }
                }
            }
        }

        for (iter=setting.mBlackList.mFilePaths.begin(); iter!=setting.mBlackList.mFilePaths.end(); ++iter) {
            if (originPool.find(iter->first) == originPool.end()) {
                WhiteListEntry entry;
                entry.mListType = WhiteListEntry::BLACK;
                entry.mBlockType = WhiteListEntry::FILE_SCAN;
                entry.mPathType = (WhiteListEntry::PATH_TYPE)iter->second;
                entry.mPath = iter->first;
                InsertWhiteList(entry);
            }
        }
    }

    //
    UPDATE_SETTING_QUICK(BACKUP_VIRUS_STATE,  origin.mVirusBackup.mVirusBackup, setting.mVirusBackup.mVirusBackup);
    UPDATE_SETTING_QUICK(LARGE_VIRUS_ACTION,  origin.mVirusBackup.mLargeFile, setting.mVirusBackup.mLargeFile);
    UPDATE_SETTING_QUICK(BACKUP_SPACE_LESS,   origin.mVirusBackup.mSpaceLess, setting.mVirusBackup.mSpaceLess);
    UPDATE_SETTING_QUICK(BACKUP_FAIL_ACTION,  origin.mVirusBackup.mBackupFailed, setting.mVirusBackup.mBackupFailed);

    //
    UPDATE_SETTING_QUICK(SCAN_FILE_TYPE,      origin.mVirusScan.mFileType, setting.mVirusScan.mFileType);
    UPDATE_SETTING_QUICK(SCAN_COMMON_VIRUS,   origin.mVirusScan.mScanCommonVirus, setting.mVirusScan.mScanCommonVirus);
    UPDATE_SETTING_QUICK(SCAN_INTELLIGENT,    origin.mVirusScan.mScanIntelligent, setting.mVirusScan.mScanIntelligent);
    UPDATE_SETTING_QUICK(SCAN_COMPRESS_STATE, origin.mVirusScan.mScanCompressFile, setting.mVirusScan.mScanCompressFile);
    UPDATE_SETTING_QUICK(COMPRESS_FILE_SIZE,  origin.mVirusScan.mZipFileSize, setting.mVirusScan.mZipFileSize);
    UPDATE_SETTING_QUICK(SCAN_VIRUS_ACTION,   origin.mVirusScan.mVirusDeal, setting.mVirusScan.mVirusDeal);
    UPDATE_SETTING_QUICK(CLEAN_FAIL_ACTION,   origin.mVirusScan.mCleanFailed, setting.mVirusScan.mCleanFailed);

    UPDATE_SETTING_QUICK(CLOUD_ENGINE_ENABLE, origin.mCloudEngine.mEnable, setting.mCloudEngine.mEnable);
    UPDATE_SETTING_QUICK(CLOUD_ENGINE_URL,    origin.mCloudEngine.mUrl,    setting.mCloudEngine.mUrl);

    // time scan
    UPDATE_SETTING_QUICK(TIME_QUICK_SCAN_FLAG, origin.mTimeScan.mQuickScan, setting.mTimeScan.mQuickScan);
    UPDATE_SETTING_QUICK(TIME_FULL_SCAN_FLAG, origin.mTimeScan.mFullScan, setting.mTimeScan.mFullScan);

    {
        UpdateCommonCloudSetting(setting.mTimeScan.mValue, ScheduleEntry::QUICK_SCAN);
        UpdateCommonCloudSetting(setting.mTimeScan.mValue, ScheduleEntry::ALL_SCAN);
    }

    UPDATE_SETTING_QUICK(FILE_MONITOR_POWERON,    origin.mFileMonitor.mStartPowerOn, setting.mFileMonitor.mStartPowerOn);
    UPDATE_SETTING_QUICK(FILE_INTELLIGENT_BL,     origin.mFileMonitor.mIntelligentBlacklist, setting.mFileMonitor.mIntelligentBlacklist);
    UPDATE_SETTING_QUICK(MONITOR_KERNEL_STATE,    origin.mFileMonitor.mKernelMonitor, setting.mFileMonitor.mKernelMonitor);
    UPDATE_SETTING_QUICK(MONITOR_WATCH_MODE,      origin.mFileMonitor.mMonitorType, setting.mFileMonitor.mMonitorType);
    UPDATE_SETTING_QUICK(FILE_MONITOR_FILE_TYPE,  origin.mFileMonitor.mFileType, setting.mFileMonitor.mFileType);
    UPDATE_SETTING_QUICK(FILE_MONITOR_ACCELERATE, origin.mFileMonitor.mMonitorAccelerate, setting.mFileMonitor.mMonitorAccelerate);
    UPDATE_SETTING_QUICK(FILE_EMBED_SCAN,         origin.mFileMonitor.mInnerScan, setting.mFileMonitor.mInnerScan);
    UPDATE_SETTING_QUICK(FILE_MONITOR_COMMON_VIRUS,origin.mFileMonitor.mScanCommonVirus, setting.mFileMonitor.mScanCommonVirus);
    UPDATE_SETTING_QUICK(FILE_MONITOR_SCAN_INTELLIGENT, origin.mFileMonitor.mScanIntelligent, setting.mFileMonitor.mScanIntelligent);
    UPDATE_SETTING_QUICK(FILE_MONITOR_SCAN_ZIP,   origin.mFileMonitor.mZipScan, setting.mFileMonitor.mZipScan);
    UPDATE_SETTING_QUICK(FILE_MONITOR_ZIP_SIZE,   origin.mFileMonitor.mZipFileSize, setting.mFileMonitor.mZipFileSize);
    UPDATE_SETTING_QUICK(FILE_MONITOR_VIRUS,      origin.mFileMonitor.mVirusDeal, setting.mFileMonitor.mVirusDeal);
    UPDATE_SETTING_QUICK(FILE_MONITOR_CLEAN_FAIL, origin.mFileMonitor.mCleanFailed, setting.mFileMonitor.mCleanFailed);
    UPDATE_SETTING_QUICK(FILE_MONITOR_KILL_PROMPT, origin.mFileMonitor.mCleanPrompt, setting.mFileMonitor.mCleanPrompt);

    UPDATE_SETTING_QUICK(U_PLUGIN_MONITOR,        origin.mPluginMonitor.mState, setting.mPluginMonitor.mState);
    UPDATE_SETTING_QUICK(U_PLUGIN_INSERT,         origin.mPluginMonitor.mPluginDeal, setting.mPluginMonitor.mPluginDeal);
    UPDATE_SETTING_QUICK(U_PLUGIN_SCAN_DEPTH,     origin.mPluginMonitor.mScanDepth, setting.mPluginMonitor.mScanDepth);

    return 0;
}

int Storage::GetNetFilterSetting(NetfilterSetting& setting) {

    return 0;
}

int Storage::UpdateNetFilterSetting(const NetfilterSetting& setting) {

    return 0;
}

int Storage::GetCommonBaseSetting(BaseSetting& setting) {
    GET_SETTING(ADMIN_PASSWORD,         setting.mAdminPasswd);
    GET_SETTING(TRAY_ICON_STATE,        setting.mTrayIconHide);

    GET_SETTING(UPDATE_MODE,            setting.mUpdateMode);
    GET_SETTING(UPDATE_DELAY_START,     setting.mDelayStart);
    GET_SETTING(UPDATE_DELAY_TIME,      setting.mDelayTime);
//    GET_SETTING(UPDATE_CONTENT_ACTION,  setting.mUpdateComponents);
    GET_SETTING(VIRUS_LIB_UPDATE_INTIME, setting.mCheckViruslibInTime);
    GET_SETTING(URL_LIB_VALID_ATONCE,   setting.mUrlLibValid);
    GET_SETTING(UPDATE_PROXY_ACTION,    setting.mProxySetting);
    GET_SETTING(UPDATE_PROXY_ADDR,      setting.mProxyAddr);
    GET_SETTING(UPDATE_PROXY_PORT,      setting.mProxyPort);
    GET_SETTING(UPDATE_PROXY_ACCOUNT,   setting.mProxyUser);
    GET_SETTING(UPDATE_PROXY_PASSWD,    setting.mProxyPasswd);
    GET_SETTING(OFFICE_UPDATE_SOURCE,   setting.mOfficalSource);

    {
        ScheduleEntry entry;
        if (GetScheduleEntry(ScheduleEntry::UPDATE, entry) > 0) {
            setting.mUpdateTime.mValue = entry.GetCrontabHour() + ":" + entry.GetCrontabMinute();
            entry.GetCrontabDayInWeek(setting.mWeekDays.mValue);
        }
    }

    {
        list<SourceListEntry> entries;
        GetSourceList(entries, SourceListEntry::CLOUD_UPDATE_SOURCE);

        list<SourceListEntry>::iterator iter;
        for (iter=entries.begin();iter!=entries.end();++iter) {
            setting.mOtherUpdateCenter.mValue.push_back(*iter);
        }
    }

    {
        list<SourceListEntry> entries;
        GetSourceList(entries, SourceListEntry::CENTER_PROXY_SOUCE);

        list<SourceListEntry>::iterator iter;
        for (iter=entries.begin();iter!=entries.end();++iter) {
            setting.mCenterAddr.mValue.push_back(*iter);
        }

        GetSettingValue(EP_CENTER_LIST,     setting.mCenterAddr.mLocked);
        GET_SETTING(CENTER_PROXY_STATE,     setting.mCenterProxyState);
        GET_SETTING(CHECK_CENTER_EID,     setting.mCheckEIDState);
        GET_SETTING(RECONNECT_INTERVAL, setting.mCenterRetryInterval);
        GET_SETTING(UPLOAD_BANDWIDTH_LIMIT, setting.mUploadBandWidthType);
        GetSettingValue(BW_LIMIT_START_TIME, setting.mBandWidthStartTime);
        GetSettingValue(BW_LIMIT_END_TIME, setting.mBandWidthEndTime);
    }

    return 0;
}

int Storage::UpdateCommonBaseSetting(BaseSetting& setting) {
    UPDATE_SETTING(ADMIN_PASSWORD,      setting.mAdminPasswd);
    UPDATE_SETTING(TRAY_ICON_STATE,     setting.mTrayIconHide);
//    UPDATE_SETTING(UPDATE_MODE,         setting.mUpdateTime,    locked);
//    UPDATE_SETTING(UPDATE_HOUR_MINUTE,  setting.mUpdateTimeValue, locked);
//    UPDATE_SETTING(UPDATE_WEEKS,        setting.mWeekDays,      locked);
    UPDATE_SETTING(UPDATE_DELAY_START,  setting.mDelayStart);
    UPDATE_SETTING(UPDATE_DELAY_TIME,   setting.mDelayTime);
//    UPDATE_SETTING(UPDATE_CONTENT_ACTION, setting.mUpdateComponents);
    UPDATE_SETTING(VIRUS_LIB_UPDATE_INTIME, setting.mCheckViruslibInTime);
    UPDATE_SETTING(URL_LIB_VALID_ATONCE, setting.mUrlLibValid);
    UPDATE_SETTING(UPDATE_PROXY_ACTION, setting.mProxySetting);
    UPDATE_SETTING(UPDATE_PROXY_ADDR,   setting.mProxyAddr);
    UPDATE_SETTING(UPDATE_PROXY_PORT,   setting.mProxyPort);
    UPDATE_SETTING(UPDATE_PROXY_ACCOUNT, setting.mProxyUser);
    UPDATE_SETTING(UPDATE_PROXY_PASSWD, setting.mProxyPasswd);
    UPDATE_SETTING(OFFICE_UPDATE_SOURCE, setting.mOfficalSource);

    {
        list<SourceListEntry> olds;
        GetSourceList(olds, SourceListEntry::CLOUD_UPDATE_SOURCE);

        std::list<SourceListEntry>::iterator iter;
        for (iter=olds.begin();iter!=olds.end();++iter) {
            if (iter->mOffical == 0) {
                DeleteSourceList(*iter);
            }
        }
        for (iter=setting.mOtherUpdateCenter.mValue.begin();iter!=setting.mOtherUpdateCenter.mValue.end();++iter) {
            if (iter->mOffical == 0) {
                SourceListEntry entry;
                entry.mState = true;
                entry.mAddress = iter->mAddress;
                entry.mType = SourceListEntry::CLOUD_UPDATE_SOURCE;
                entry.mOffical = 0;
                InsertSourceList(entry);
            }
        }

    }

    {
        list<SourceListEntry> olds;
        GetSourceList(olds, SourceListEntry::CENTER_PROXY_SOUCE);

        std::list<SourceListEntry>::iterator iter;
        for (iter=olds.begin();iter!=olds.end();++iter) {
            if (iter->mOffical == 0) {
                DeleteSourceList(*iter);
            }
        }

        for (iter=setting.mCenterAddr.mValue.begin();iter!=setting.mCenterAddr.mValue.end();++iter) {
            if (iter->mOffical == 0) {
                InsertSourceList(*iter);
            }
        }

        UPDATE_SETTING(CENTER_PROXY_STATE,     setting.mCenterProxyState);
        UPDATE_SETTING(CHECK_CENTER_EID,     setting.mCheckEIDState);
        UPDATE_SETTING(RECONNECT_INTERVAL, setting.mCenterRetryInterval);
        UPDATE_SETTING(UPLOAD_BANDWIDTH_LIMIT, setting.mUploadBandWidthType);
        SetSettingValue(BW_LIMIT_START_TIME, setting.mBandWidthStartTime);
        SetSettingValue(BW_LIMIT_END_TIME, setting.mBandWidthEndTime);
    }

    return 0;
}

int Storage::GetDefenCenterSetting(DefenCenterSetting& setting) {
    GET_SETTING(FILE_DEFEN_SWITCH,      setting.mFileMonitorState);
    GET_SETTING(MAIL_DEFEN_SWITCH,      setting.mMailMonitorState);
    GET_SETTING(U_PLUGIN_MONITOR,       setting.mUSBPluginMonitorState);
    GET_SETTING(SHARED_DEFEN_SWITCH,    setting.mShareMonitorState);
    GET_SETTING(SPECIAL_FAKE_WORM,      setting.mFakeWormState);
    GET_SETTING(SPECIAL_VIRUS_RAIN,     setting.mRainCloudVirusState);
    GET_SETTING(SPECIAL_WEIKE_WORM,     setting.mWeikeWormState);
    GET_SETTING(SPECIAL_DLL_INJECTLESS, setting.mDLLInjectLessState);
    return 0;
}

int Storage::UpdateDefenCenterSetting(DefenCenterSetting& setting) {
    UPDATE_SETTING(FILE_DEFEN_SWITCH,      setting.mFileMonitorState);
    UPDATE_SETTING(MAIL_DEFEN_SWITCH,      setting.mMailMonitorState);
    UPDATE_SETTING(U_PLUGIN_MONITOR,       setting.mUSBPluginMonitorState);
    UPDATE_SETTING(SHARED_DEFEN_SWITCH,    setting.mShareMonitorState);
    UPDATE_SETTING(SPECIAL_FAKE_WORM,      setting.mFakeWormState);
    UPDATE_SETTING(SPECIAL_VIRUS_RAIN,     setting.mRainCloudVirusState);
    UPDATE_SETTING(SPECIAL_WEIKE_WORM,     setting.mWeikeWormState);
    UPDATE_SETTING(SPECIAL_DLL_INJECTLESS, setting.mDLLInjectLessState);
    return 0;
}

int Storage::InitDefaultValue(const string &configfile) {
    CINIFile ini(configfile, true);

    int value = 0;

    #define DEFALUT_INIT_SETTING(x, y) do { \
                            ini.readitemlong("SETTING", x, y, 0); \
                            SetSettingValue(x, y); \
                        } while(0); \

    DEFALUT_INIT_SETTING(ENV_INTELLIGENT,       value);
    DEFALUT_INIT_SETTING(VIRUS_TRACE,           value);
    DEFALUT_INIT_SETTING(VIRUS_LOG_FIELD,       value);
    DEFALUT_INIT_SETTING(RESCAN_ACCELERATE,     value);
    DEFALUT_INIT_SETTING(FILE_ACCELERATE,       value);
    DEFALUT_INIT_SETTING(PUBLIC_CLOUD_STATE,    value);
    DEFALUT_INIT_SETTING(PRIVATE_CLOUD_STATE,   value);
    DEFALUT_INIT_SETTING(BACKUP_VIRUS_STATE,    value);
    DEFALUT_INIT_SETTING(LARGE_VIRUS_ACTION,    value);
    DEFALUT_INIT_SETTING(BACKUP_SPACE_LESS,     value);
    DEFALUT_INIT_SETTING(BACKUP_FAIL_ACTION,    value);
    DEFALUT_INIT_SETTING(SCAN_FILE_TYPE,        value);
    DEFALUT_INIT_SETTING(SCAN_COMMON_VIRUS,     value);
    DEFALUT_INIT_SETTING(SCAN_INTELLIGENT,      value);
    DEFALUT_INIT_SETTING(SCAN_COMPRESS_STATE,   value);
    DEFALUT_INIT_SETTING(COMPRESS_FILE_SIZE,    value);
    DEFALUT_INIT_SETTING(TIME_QUICK_SCAN_FLAG,  value);
    DEFALUT_INIT_SETTING(TIME_FULL_SCAN_FLAG,   value);
    //DEFALUT_INIT_SETTING(FILE_MONITOR_POWERON,  value);
    DEFALUT_INIT_SETTING(FILE_INTELLIGENT_BL,   value);
    DEFALUT_INIT_SETTING(MONITOR_KERNEL_STATE,  value);
    DEFALUT_INIT_SETTING(MONITOR_WATCH_MODE,    value);
    DEFALUT_INIT_SETTING(FILE_MONITOR_FILE_TYPE, value);
    DEFALUT_INIT_SETTING(FILE_MONITOR_ZIP_SIZE, value);
    DEFALUT_INIT_SETTING(FILE_MONITOR_VIRUS,    value);
    DEFALUT_INIT_SETTING(FILE_MONITOR_CLEAN_FAIL, value);
    DEFALUT_INIT_SETTING(FILE_MONITOR_KILL_PROMPT, value);
    DEFALUT_INIT_SETTING(U_PLUGIN_MONITOR,      value);
    DEFALUT_INIT_SETTING(U_PLUGIN_INSERT,       value);
    DEFALUT_INIT_SETTING(U_PLUGIN_SCAN_DEPTH,   value);
    DEFALUT_INIT_SETTING(MODULE_STOP_TO_REMOVE, value);
    DEFALUT_INIT_SETTING(TRAY_ICON_STATE,       value);
    DEFALUT_INIT_SETTING(SHOW_AUTH_STATE,       value);
    DEFALUT_INIT_SETTING(UPDATE_MODE,           value);
    DEFALUT_INIT_SETTING(UPDATE_CONTENT_ACTION, value);
    DEFALUT_INIT_SETTING(ACTIVE_DEFENCE_SWITCH, value);
    //DEFALUT_INIT_SETTING(FILE_DEFEN_SWITCH,     value);
    DEFALUT_INIT_SETTING(MAIL_DEFEN_SWITCH,     value);
    DEFALUT_INIT_SETTING(NETFILTER_SWITCH,      value);

    return 0;
}

int Storage::GetConnectCenterList(vector<string> &center) {
    string centerlist;
    if (GetSettingValue(CLIENT_CENTER_LIST, centerlist)) {
        Utils::String::Split(centerlist, ";", &center);
        return center.size();
    }
    return -1;
}

int Storage::GetCenterProxyState(bool &locked) {
    int state = -1;
    if (0 == GetSettingValue(CENTER_PROXY_STATE, state, locked)) {
        return state;
    }
    return -1;
}
