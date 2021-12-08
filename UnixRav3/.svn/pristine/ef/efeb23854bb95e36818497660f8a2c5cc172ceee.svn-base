#include "scheduleentry.h"
#include "CPPSQLite/cppsqlite3.h"

string ScheduleEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS ScheduleTable( \
            ID INTEGER PRIMARY KEY AUTOINCREMENT, \
            TYPE INTEGER, \
            CRON INTEGER, \
            FUNC INTEGER, \
            USER INTEGER, \
            LOCK INTEGER, \
            LAST INTEGER, \
            PATH TEXT, \
            EXTS TEXT);";
}

string ScheduleEntry::Select() const {
    char sql[128] = {0};
    if (mFuncEnum > 0) {
        sprintf(sql, "select * from ScheduleTable where FUNC=%d", mFuncEnum);
    }
    else {
        sprintf(sql, "select * from ScheduleTable");
    }
    return string(sql);
}

string ScheduleEntry::Insert() const {
    char sql[1024] = {0};
    sprintf(sql, "insert into ScheduleTable(TYPE, CRON, FUNC, USER, LOCK, LAST, PATH, EXTS) \
                    values(%d, %d, %d, %d, %d, %ld, '%s', '%s')",
            mType, mCrontab, mFuncEnum, mUserSet, mLocked, mLastTime, mScanPath.c_str(), mScanExts.c_str());
    return string(sql);
}

string ScheduleEntry::Update() const {
    char sql[1024] = {0};
    sprintf(sql, "update ScheduleTable set TYPE=%d,CRON=%d,FUNC=%d,USER=%d,LOCK=%d,LAST=%ld,PATH='%s',EXTS='%s' where ID=%ld",
            mType, mCrontab, mFuncEnum, mUserSet, mLocked, mLastTime, mScanPath.c_str(), mScanExts.c_str(), mID);

    return string(sql);
}

string ScheduleEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from ScheduleTable where ID=%ld", mID);
    return string(sql);
}

void ScheduleEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mType = (ScheduleType)query->getIntField(1, 0);
    mCrontab = query->getIntField(2, -1);
    mFuncEnum = (ScanType)query->getIntField(3, -1);
    mUserSet = (ScanType)query->getIntField(4, -1);
    mLocked = (ScanType)query->getIntField(5, -1);
    mLastTime = query->getIntField(6, -1);
    mScanPath = query->getStringField(7);
    mScanExts = query->getStringField(8);
}
