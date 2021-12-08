#ifndef COMMEVENTLOG_H
#define COMMEVENTLOG_H

#include "baseentry.h"

#include <string>

class CommEventLog : public BaseEntry, public Serializable
{
public:
    enum CommEvent {
        DEPLOY,
        COMMAND,
        MESSAGE,
        SERVER
    };

public:
    int mDate;
    enum CommEvent mType;
    std::string mAction;
    std::string mTarget;
    std::string mExtend;
    std::string mReserved1;
    std::string mReserved2;

public:
    CommEventLog() {
        mDate = 0;
        mType = DEPLOY;
    }

private:
    void InitializeJson(Json::Value& json);
    void ExternlizeJson(Json::Value& json);

public:
    std::string CreateTable() const;
    std::string Select() const;
    std::string Insert() const;
    std::string Update() const;
    std::string Delete() const;

    void Mapping(CppSQLite3Query *query);
};

#endif // COMMEVENTLOG_H
