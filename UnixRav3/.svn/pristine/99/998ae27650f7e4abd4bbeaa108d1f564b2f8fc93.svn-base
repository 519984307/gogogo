#include "commeventlog.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"
#include "json/json.h"

string CommEventLog::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS CommEventTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            DATE INTEGER, \
                            TYPE INTEGER, \
                            ACTION TEXT, \
                            TARGET TEXT, \
                            EXTEND TEXT, \
                            RESERVED1 TEXT,\
                            RESERVED2 TEXT);";
}


string CommEventLog::Select() const {
    char sql[256] = {0};
    if (mID == 0) {
        sprintf(sql, "select * from CommEventTable where TYPE=%d order by ID desc limit %d", mType, LOG_PAGE_COUNT);
    }
    else {
        sprintf(sql, "select * from CommEventTable where TYPE=%d and ID<%ld order by ID desc limit %d", mType, mID, LOG_PAGE_COUNT);
    }
    return string(sql);
}

string CommEventLog::Insert() const {
    char sql[512] = {0};
    sprintf(sql, "insert into CommEventTable(DATE, TYPE, ACTION, TARGET, EXTEND, RESERVED1, RESERVED2) \
                    values(%d, %d, '%s', '%s', '%s', '%s', '%s')",
                     mDate, mType, mAction.c_str(), mTarget.c_str(), mExtend.c_str(), mReserved1.c_str(), mReserved2.c_str());
    return string(sql);
}

string CommEventLog::Update() const {
    char sql[512] = {0};
    sprintf(sql, "update CommEventTable set DATE=%d,TYPE=%d,ACTION='%s',TARGET='%s',EXTEND='%s',RESERVED1='%s',RESERVED2='%s' where ID=%ld",
            mDate, mType, mAction.c_str(), mTarget.c_str(), mExtend.c_str(), mReserved1.c_str(), mReserved2.c_str(),mID);
    return string(sql);
}

string CommEventLog::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from CommEventTable where ID=%ld", mID);
    return string(sql);
}

void CommEventLog::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mDate = query->getIntField(1, 0);
    mType = (enum CommEvent)query->getIntField(2, 0);
    mAction = query->getStringField(3, NULL);
    mTarget = query->getStringField(4, NULL);
    mExtend = query->getStringField(5, NULL);
    mReserved1 = query->getStringField(6, NULL);
    mReserved2 = query->getStringField(7, NULL);
}

void CommEventLog::InitializeJson(Json::Value& json) {
    mID = json["id"].asInt();
    mDate = json["date"].asInt();
    mType = (enum CommEvent)json["type"].asInt();
    mAction = json["action"].asString();
    mTarget = json["target"].asString();
    mExtend = json["extend"].asString();
    mReserved1 = json["reserved1"].asString();
    mReserved2 = json["reserved2"].asString();
}

void CommEventLog::ExternlizeJson(Json::Value& json) {
    json["id"] = Json::Value((int)mID);
    json["date"] = Json::Value(mDate);
    json["type"] = Json::Value(mType);
    json["action"] = Json::Value(mAction);
    json["target"] = Json::Value(mTarget);
    json["extend"] = Json::Value(mExtend);
    json["reserved1"] = Json::Value(mReserved1);
    json["reserved2"] = Json::Value(mReserved2);
}

