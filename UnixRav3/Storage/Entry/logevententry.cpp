#include "logevententry.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string LogEventEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS LogTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            TYPE INTEGER, \
                            EXTEND INTEGER, \
                            STATUS INTEGER, \
                            FILE INTEGER, \
                            VIRUS INTEGER, \
                            KILLED INTEGER,\
                            DELETED INTEGER,\
                            FAILED INTEGER,\
                            IGNORE INTEGER,\
                            STIME INTEGER,\
                            ETIME INTEGER, \
                            PATH TEXT, \
                            TOTAL INTEGER, \
                            REPORT INTEGER);";
}


string LogEventEntry::Select() const {
    char sql[128] = {0};
    if (mLogType > 0) {
        if (mID == 0) {
//            sprintf(sql, "select * from LogTable where TYPE=%d order by ID desc limit %d", mLogType, LOG_PAGE_COUNT);
            sprintf(sql, "select * from LogTable where TYPE=%d order by ID desc", mLogType);
        }
        else {
//            sprintf(sql, "select * from LogTable where TYPE=%d and ID<%ld order by ID desc limit %d", mLogType, mID, LOG_PAGE_COUNT);
            sprintf(sql, "select * from LogTable where TYPE=%d and ID<%ld order by ID desc", mLogType, mID);
        }
    }
    else {
        if (mID == 0) {
            sprintf(sql, "select * from LogTable where order by ID desc");
        }
        else {
            sprintf(sql, "select * from LogTable where ID=%ld", mID);
        }
    }

    return string(sql);
}

string LogEventEntry::Insert() const {
    char sql[512] = {0};
    sprintf(sql, "insert into LogTable(TYPE, EXTEND, STATUS, FILE, VIRUS, KILLED, DELETED, FAILED, IGNORE, STIME, ETIME, PATH, TOTAL, REPORT) \
                    values(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d)",
            mLogType, mLogExtend, mStatus, mScanCount, mVirusCount, mKilledCount, mDeletedCount, mFailedCount, mIgnoreCount, mStartTime, mEndTime, mScanPath.c_str(), mTotalCount, mReported);
    return string(sql);
}

string LogEventEntry::Update() const {
    char sql[256] = {0};
    sprintf(sql, "update LogTable set TYPE=%d,EXTEND=%d,STATUS=%d,FILE=%d,VIRUS=%d,KILLED=%d,DELETED=%d,FAILED=%d,IGNORE=%d,STIME=%d,ETIME=%d,PATH='%s',TOTAL=%d,REPORT=%d where ID=%ld",
            mLogType, mLogExtend, mStatus, mScanCount, mVirusCount, mKilledCount, mDeletedCount, mFailedCount, mIgnoreCount, mStartTime, mEndTime, mScanPath.c_str(),mTotalCount,mReported, mID);
    return string(sql);
}

string LogEventEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from LogTable where ID=%ld", mID);
    return string(sql);
}

string LogEventEntry::Last() const {
    return "select ID from LogTable order by ID desc limit 1";
}

void LogEventEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mLogType = (enum LogType)query->getIntField(1, 0);
    mLogExtend = query->getIntField(2, 0);
    mStatus = (enum Status)query->getIntField(3, 0);
    mScanCount = query->getIntField(4, 0);
    mVirusCount = query->getIntField(5, 0);
    mKilledCount = query->getIntField(6, 0);
    mDeletedCount = query->getIntField(7, 0);
    mFailedCount = query->getIntField(8, 0);
    mIgnoreCount = query->getIntField(9, 0);
    mStartTime = query->getIntField(10, 0);
    mEndTime = query->getIntField(11, 0);
    mScanPath = query->getStringField(12, NULL);
    mTotalCount = query->getIntField(13, 0);
    mReported = query->getIntField(14, 0);
}

void LogEventEntry::InitializeJson(Json::Value& json) {
    mID = json["id"].asInt();
    mLogType = (enum LogType)json["type"].asInt();
    mLogExtend = json["extend"].asInt();
    mStatus = (enum Status)json["status"].asInt();
    mScanCount = json["scan"].asInt();
    mVirusCount = json["virus"].asInt();
    mKilledCount = json["kill"].asInt();
    mDeletedCount = json["del"].asInt();
    mFailedCount = json["fail"].asInt();
    mIgnoreCount = json["ignore"].asInt();
    mStartTime = json["start"].asInt();
    mEndTime = json["end"].asInt();
    mScanPath = json["path"].asString();
    mTotalCount = json["total"].asInt();
    mReported = json["report"].asInt();
}

void LogEventEntry::ExternlizeJson(Json::Value& json) {
    json["id"] = Json::Value((int)mID);
    json["type"] = Json::Value(mLogType);
    json["extend"] = Json::Value(mLogExtend);
    json["status"] = Json::Value(mStatus);
    json["scan"] = Json::Value(mScanCount);
    json["virus"] = Json::Value(mVirusCount);
    json["kill"] = Json::Value(mKilledCount);
    json["del"] = Json::Value(mDeletedCount);
    json["fail"] = Json::Value(mFailedCount);
    json["ignore"] = Json::Value(mIgnoreCount);
    json["start"] = Json::Value(mStartTime);
    json["end"] = Json::Value(mEndTime);
    json["path"] = Json::Value(mScanPath);
    json["total"] = Json::Value(mTotalCount);
    json["report"] = Json::Value(mReported);
}

string LogEventEntry::GetLogUser() {
    if (mScanPath.empty()) {
        return "";
    }
    int pos = mScanPath.find(";");
    if (pos != string::npos) {
        return mScanPath.substr(0, pos);
    }
    return mScanPath;
}
