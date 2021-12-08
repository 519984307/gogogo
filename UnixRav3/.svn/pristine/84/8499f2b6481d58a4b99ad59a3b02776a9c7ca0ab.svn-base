#include "quarantinelog.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string QuarantineLog::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS QuarantineTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            DATE INTEGER, \
                            FILE TEXT, \
                            SOURCE TEXT, \
                            TARGET TEXT, \
                            VIRUS TEXT, \
                            FSIZE INTEGER, \
                            UID INTEGER, \
                            GID INTEGER, \
                            PERM INTEGER, \
                            RESERVED1 TEXT,\
                            RESERVED2 TEXT);";
}


string QuarantineLog::Select() const {
    char sql[1024] = {0};
    if (mID == 0) {
//        sprintf(sql, "select * from QuarantineTable order by ID desc limit %d", LOG_PAGE_COUNT);
        if (mReserved.empty()) {
            sprintf(sql, "select * from QuarantineTable where RESERVED1 is NULL order by ID desc");
        }
        else {
            sprintf(sql, "select * from QuarantineTable where RESERVED1='%s' order by ID desc", mReserved.c_str());
        }
    }
    else {
//        sprintf(sql, "select * from QuarantineTable where ID<%ld order by ID desc limit %d",  mID, LOG_PAGE_COUNT);
        sprintf(sql, "select * from QuarantineTable where ID<%ld order by ID desc",  mID);
    }
    return string(sql);
}

string QuarantineLog::Insert() const {
    char sql[1024] = {0};
    sprintf(sql, "insert into QuarantineTable(DATE, FILE, SOURCE, TARGET, VIRUS, FSIZE, UID, GID, PERM) \
                    values(%d, '%s', '%s', '%s', '%s', %ld, %d, %d, %d)",
                     mDate, mFile.c_str(), mSource.c_str(), mTarget.c_str(), mVirusName.c_str(), mSize, mUID, mGID, mPermision);
    return string(sql);
}

string QuarantineLog::Update() const {
    char sql[1024] = {0};

    if (!mReserved.empty()) {
        sprintf(sql, "update QuarantineTable set DATE=%d,FILE='%s',SOURCE='%s',TARGET='%s',VIRUS='%s',FSIZE=%ld,UID=%d,GID=%d,PERM=%d,RESERVED1='%s' where ID=%ld",
                mDate, mFile.c_str(), mSource.c_str(), mTarget.c_str(), mVirusName.c_str(), mSize, mUID, mGID, mPermision, mReserved.c_str(), mID);
    }
    else {
        sprintf(sql, "update QuarantineTable set DATE=%d,FILE='%s',SOURCE='%s',TARGET='%s',VIRUS='%s',FSIZE=%ld, UID=%d, GID=%d, PERM=%d where ID=%ld",
                mDate, mFile.c_str(), mSource.c_str(), mTarget.c_str(), mVirusName.c_str(), mSize, mUID, mGID, mPermision, mID);
    }
    return string(sql);
}

string QuarantineLog::Delete() const {
    char sql[512] = {0};
    sprintf(sql, "delete from QuarantineTable where ID=%ld", mID);
    return string(sql);
}

void QuarantineLog::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mDate = query->getIntField(1, 0);
    mFile = query->getStringField(2, "");
    mSource = query->getStringField(3, "");
    mTarget = query->getStringField(4, "");
    mVirusName = query->getStringField(5, "");
    mSize = query->getIntField(6, 0);
    mUID = query->getIntField(7, 0);
    mGID = query->getIntField(8, 0);
    mPermision = query->getIntField(9, 0);
    mReserved = query->getStringField(10, "");
}

void QuarantineLog::InitializeJson(Json::Value& json) {
    mID = json["id"].asInt();
    mDate = json["date"].asInt();
    mFile = json["file"].asString();
    mSource = json["source"].asString();
    mTarget = json["target"].asString();
    mVirusName = json["virus"].asString();
    mSize = json["size"].asInt();
    mUID = json["uid"].asInt();
    mGID = json["gid"].asInt();
    mPermision = json["perm"].asInt();
    mReserved = json["resev"].asString();
}

void QuarantineLog::ExternlizeJson(Json::Value& json) {
    json["id"] = Json::Value((int)mID);
    json["date"] = Json::Value(mDate);
    json["file"] = Json::Value(mFile);
    json["source"] = Json::Value(mSource);
    json["target"] = Json::Value(mTarget);
    json["virus"] = Json::Value(mVirusName);
    json["size"] = Json::Value((int)mSize);
    json["uid"] = Json::Value(mUID);
    json["gid"] = Json::Value(mGID);
    json["perm"] = Json::Value(mPermision);
    json["resev"] = Json::Value(mReserved);
}

