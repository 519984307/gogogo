#include "whitelistentry.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string WhiteListEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS WhiteTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            PATH TEXT, \
                            TYPE INTEGER, \
                            BLOCK INTEGER, \
                            CATEGORY INTEGER, \
                            REVD1 INTEGER, \
                            REVD2 INTEGER);";
}

string WhiteListEntry::Select() const {
    char sql[512] = {0};
    sprintf(sql, "select * from WhiteTable where TYPE=%d AND BLOCK=%d", mListType, mBlockType);
    if (mPathType != UNSET) {
        char category[64];
        sprintf(category, " AND CATEGORY=%d", mPathType);
        strcat(sql, category);
    }
    return string(sql);
}
string WhiteListEntry::Insert() const {
    char sql[512] = {0};
    sprintf(sql, "insert into WhiteTable(PATH, TYPE,BLOCK,CATEGORY, REVD1, REVD2) values('%s', %d, %d, %d, %d, %d)",
            mPath.c_str(), mListType, mBlockType, mPathType, mReserved1, mReserved2);
    return string(sql);
}

string WhiteListEntry::Update() const {
    char sql[512] = {0};
    sprintf(sql, "update WhiteTable set PATH='%s',TYPE=%d,CATEGORY=%d,BLOCK=%d where ID=%ld", mPath.c_str(), mListType, mBlockType, mPathType, mID);
    return string(sql);
}
string WhiteListEntry::Delete() const {
    char sql[512] = {0};
    sprintf(sql, "delete from WhiteTable where ID=%ld", mID);
    return string(sql);
}

void WhiteListEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mPath = query->getStringField(1, NULL);
    mListType = (LIST_TYPE)query->getIntField(2, 0);
    mBlockType = (BLOCK_TYPE)query->getIntField(3, 0);
    mPathType = (PATH_TYPE)query->getIntField(4, 0);
    mReserved1 = query->getIntField(5, 0);
    mReserved2 = query->getIntField(6, 0);
}

void WhiteListEntry::InitializeJson(Json::Value& json) {
    mID = json["id"].asInt();
    mPath = json["path"].asString();
    mListType = (LIST_TYPE)json["ltype"].asInt();
    mBlockType = (BLOCK_TYPE)json["btype"].asInt();
    mPathType = (PATH_TYPE)json["ptype"].asInt();
    mReserved1 = json["resev1"].asInt();
}

void WhiteListEntry::ExternlizeJson(Json::Value& json) {
    json["id"] = Json::Value((int)mID);
    json["path"] = Json::Value(mPath);
    json["ltype"] = Json::Value(mListType);
    json["btype"] = Json::Value(mBlockType);
    json["ptype"] = Json::Value(mPathType);
    json["resev1"] = Json::Value(mReserved1);
}
