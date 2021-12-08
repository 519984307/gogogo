#include "settingentry.h"
#include "CPPSQLite/cppsqlite3.h"

string SettingEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS SettingTable( \
            ID INTEGER PRIMARY KEY AUTOINCREMENT, \
            NAME TEXT, \
            INTV INTEGER, \
            STRV TEXT, \
            INTU INTEGER, \
            STRU TEXT, \
            AVAIL INTEGER);";
}

string SettingEntry::Select() const {
    char sql[256] = {0};
    sprintf(sql, "select * from SettingTable where NAME='%s'", mName.c_str());
    return string(sql);
}

string SettingEntry::Insert() const {
    char sql[256] = {0};
    if (mType == INTEGER) {
        sprintf(sql, "insert into SettingTable(NAME, INTV, INTU, AVAIL) values ('%s', %d, %d, %d)",
                mName.c_str(), mValue.mIntegerValue, mUserValue.mUserIntValue, mAvailed);
    }
    else {
        sprintf(sql, "insert into SettingTable(NAME, STRV, STRU, AVAIL) values ('%s', '%s', '%s', %d)",
                mName.c_str(), mValue.mStringValue, mUserValue.mUserStrValue, mAvailed);
    }
    return string(sql);
}

string SettingEntry::Update() const {
    char sql[256] = {0};
    if (mType == INTEGER) {
        sprintf(sql, "update SettingTable set INTV=%d,INTU=%d,AVAIL=%d where NAME='%s'",
                mValue.mIntegerValue, mUserValue.mUserIntValue, mAvailed, mName.c_str());
    }
    else {
        sprintf(sql, "update SettingTable set STRV='%s',STRU='%s',AVAIL=%d where NAME='%s'",
                mValue.mStringValue,mUserValue.mUserStrValue,mAvailed, mName.c_str());
    }
    return string(sql);
}

string SettingEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete SettingTable where NAME='%s'", mName.c_str());
    return string(sql);
}

void SettingEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mName = query->getStringField(1, NULL);
    mValue.mIntegerValue = query->getIntField(2, DEFAULT_INT_VALUE);
    const char *value = query->getStringField(3, NULL);
    if (value) {
        strcpy(mValue.mStringValue, value);
    }
    mUserValue.mUserIntValue = query->getIntField(4, DEFAULT_INT_VALUE);
    const char *userv = query->getStringField(5, NULL);
    if (userv) {
        strcpy(mUserValue.mUserStrValue, userv);
    }
    mAvailed = query->getIntField(6, 0);
}
