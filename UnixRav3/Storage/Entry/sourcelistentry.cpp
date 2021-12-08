#include "sourcelistentry.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string SourceListEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS SourceListTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            STATE INTEGER, \
                            NAME TEXT, \
                            ADDRESS TEXT, \
                            PORT INTEGER, \
                            TYPE INTEGER, \
                            REQT INTEGER, \
                            PFNUM INTEGER, \
                            LOCK INTEGER, \
                            OFFIC INTEGER);";
}

string SourceListEntry::Select() const {
    char sql[256] = {0};
    if (mOffical < 0) {
        sprintf(sql, "select * from SourceListTable where TYPE=%d", mType);
    }
    else {
        sprintf(sql, "select * from SourceListTable where TYPE=%d and OFFIC=%d", mType, mOffical);
    }
    return string(sql);
}

string SourceListEntry::Insert() const {
    char sql[256] = {0};
    sprintf(sql, "insert into SourceListTable(STATE, NAME, ADDRESS, PORT, TYPE, REQT, PFNUM, LOCK, OFFIC) values(%d, '%s', '%s', %d, %d, %d, %d, %d, %d)",
            mState, mName.c_str(), mAddress.c_str(), mPort, mType, mRequestModel, mOncePostfileCount, mLocked, mOffical);
    return string(sql);
}

string SourceListEntry::Update() const {
    char sql[256] = {0};
    sprintf(sql, "update SourceListTable set STATE=%d, NAME='%s', ADDRESS='%s', PORT=%d, TYPE=%d, OFFIC=%d, REQT=%d, PFNUM=%d, LOCK=%d where ID=%ld",
                    mState, mName.c_str(), mAddress.c_str(), mPort, mType, mOffical, mRequestModel, mOncePostfileCount, mLocked, mID);
    return string(sql);
}
string SourceListEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from SourceListTable where ID=%ld", mID);
    return string(sql);
}

void SourceListEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mState = query->getIntField(1, 0);
    mName = query->getStringField(2, NULL);
    mAddress = query->getStringField(3, NULL);
    mPort = query->getIntField(4, 0);
    mType = (SourceListEntry::SourceType)query->getIntField(5, 0);
    mRequestModel = query->getIntField(6, 0);
    mOncePostfileCount = query->getIntField(7, 0);
    mLocked = query->getIntField(8, 0);
    mOffical = query->getIntField(9, 0);
}

