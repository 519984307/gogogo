#include "netaccessentry.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string NetAccessEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS NetLogTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            DATE INTEGER, \
                            URL TEXT, \
                            PROCESS TEXT, \
                            TYPE INTEGER, \
                            STATE INTEGER, \
                            SOURCE INTEGER);";
}


string NetAccessEntry::Select() const {
    char sql[128] = {0};
    if (mID == 0) {
        sprintf(sql, "select * from NetLogTable where TYPE=%d order by ID desc limit %d", mType, LOG_PAGE_COUNT);
    }
    else {
        sprintf(sql, "select * from NetLogTable where TYPE=%d and ID<%ld order by ID desc limit %d", mType, mID, LOG_PAGE_COUNT);
    }
    return string(sql);
}

string NetAccessEntry::Insert() const {
    char sql[512] = {0};
    sprintf(sql, "insert into NetLogTable(DATE, URL, PROCESS, TYPE, STATE, SOURCE) \
                    values(%ld, '%s', '%s', %d, %d, %d)",
            mTime, mUrl.c_str(), mProcess.c_str(), mType, mState, mSource);
    return string(sql);
}

string NetAccessEntry::Update() const {
    char sql[128] = {0};
    sprintf(sql, "update NetLogTable set DATE=%ld,URL='%s',PROCESS='%s',TYPE=%d,STATE=%d,SOURCE=%d where ID=%ld",
            mTime, mUrl.c_str(), mProcess.c_str(), mType, mState, mSource, mID);
    return string(sql);
}

string NetAccessEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from NetLogTable where ID=%ld", mID);
    return string(sql);
}

void NetAccessEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mTime = query->getIntField(1, 0);
    mUrl = query->getStringField(2, NULL);
    mProcess = query->getStringField(3, NULL);
    mType = (MatchType)query->getIntField(4, 0);
    mState = (MatchResult)query->getIntField(5, 0);
    mSource = (NetSource)query->getIntField(6, 0);
}

