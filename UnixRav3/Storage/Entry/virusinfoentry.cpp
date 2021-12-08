#include "virusinfoentry.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string VirusInfoEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS VirusTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            LOGID INTEGER, \
                            VNAME TEXT, \
                            VTYPE INTEGER, \
                            PATH TEXT, \
                            NAME TEXT, \
                            STATUS INTEGER,\
                            STIME INTEGER,\
                            FSIZE INTEGER, \
                            QUAPATH TEXT, \
                            HASH TEXT, \
                            REPORT INTEGER, \
                            RESERV1 INTEGER, \
                            RESERV2 INTEGER);";
}

string VirusInfoEntry::Select() const {
    char sql[128] = {0};
    if (mScanLogID > 0) {
        sprintf(sql, "select * from VirusTable where LOGID=%ld", mScanLogID);
    }
    else {
        if (mID == 0) {
//            sprintf(sql, "select * from VirusTable order by ID desc limit %d", LOG_PAGE_COUNT);
            sprintf(sql, "select * from VirusTable order by ID desc");
        }
        else {
            sprintf(sql, "select * from VirusTable where ID<%ld order by ID desc", mID);
        }
    }

    return string(sql);
}
string VirusInfoEntry::Insert() const {
    char sql[1024] = {0};
    sprintf(sql, "insert into VirusTable(LOGID, VNAME, VTYPE, PATH, NAME, STATUS, STIME, FSIZE, QUAPATH, HASH, REPORT) \
                    values(%ld, '%s', %d, '%s', '%s', %d, %ld, %ld, '%s', '%s', %d)",
            mScanLogID, mVirusName.c_str(), mVirusType, mOriginPath.c_str(), mOriginName.c_str(), mDealState, mTime, mFileSize, mQuarantine.c_str(), mHash.c_str(), mReported);
    return string(sql);
}
string VirusInfoEntry::Update() const {
    char sql[512] = {0};
    sprintf(sql, "update VirusTable set LOGID=%ld,VNAME='%s',VTYPE=%d,PATH='%s',NAME='%s',STATUS=%d,STIME=%ld,FSIZE=%ld,QUAPATH='%s',HASH='%s',REPORT=%d where ID=%ld",
            mScanLogID, mVirusName.c_str(), mVirusType, mOriginPath.c_str(), mOriginName.c_str(), mDealState, mTime, mFileSize,mQuarantine.c_str(), mHash.c_str(), mReported, mID);
    return string(sql);
}
string VirusInfoEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from VirusTable where ID=%ld", mID);
    return string(sql);
}

void VirusInfoEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mScanLogID = query->getIntField(1, 0);
    mVirusName = query->getStringField(2, NULL);
    mVirusType = query->getIntField(3, 0);
    mOriginPath = query->getStringField(4, NULL);
    mOriginName = query->getStringField(5, NULL);
    mDealState = query->getIntField(6, 0);;
    mTime = query->getIntField(7, 0);
    mFileSize = query->getIntField(8, 0);
    mQuarantine = query->getStringField(9, NULL);
    mHash = query->getStringField(10, NULL);
    mReported = query->getIntField(11, 0);
}

void VirusInfoEntry::InitializeJson(Json::Value& json) {
    mID = json["id"].asInt();
    mScanLogID = json["log"].asInt();
    mVirusName = json["virus"].asString();
    mVirusType = json["type"].asInt();
    mOriginPath = json["origin"].asString();
    mOriginName = json["name"].asString();
    mDealState = json["deal"].asInt();
    mTime = json["time"].asInt();
    mFileSize = json["size"].asInt();
    mQuarantine = json["qua"].asString();
    mHash = json["hash"].asString();
    mReported = json["report"].asInt();
}

void VirusInfoEntry::ExternlizeJson(Json::Value& json) {
    json["id"] = Json::Value((int)mID);
    json["log"] = Json::Value((int)mScanLogID);
    json["virus"] = Json::Value(mVirusName);
    json["type"] = Json::Value(mVirusType);
    json["origin"] = Json::Value(mOriginPath);
    json["name"] = Json::Value(mOriginName);
    json["deal"] = Json::Value(mDealState);
    json["time"] = Json::Value((int)mTime);
    json["size"] = Json::Value((int)mFileSize);
    json["qua"] = Json::Value(mQuarantine);
    json["hash"] = Json::Value(mHash);
    json["report"] = Json::Value(mReported);
}

