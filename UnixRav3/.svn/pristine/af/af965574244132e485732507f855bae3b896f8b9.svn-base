#ifndef LOGCOMMANDSTATE_H
#define LOGCOMMANDSTATE_H

#include "logdatabasic.h"

#include <string>

class LogCommandState : public LogDataBasic
{
public:
    LogCommandState(const string& eventSource,
                    const string& username,
                    const string& mname,
                    const string& guid,
                    const string& version,
                    const string& source,
                    const string& eid,
                    const string& sguid,
                    const string& productid);

    void SetCommandData(const std::string& action, const std::string& cmdid, int type, int result, const std::string& reason);

public:
    string getLogType();
    Json::Value toJSON();

private:
    std::string mEID;
    std::string mSGUID;
    std::string mInsertTime;
    std::string mProductID;
    std::string mAction;
    std::string mReason;
    std::string mCmdID;
    int mType;
    int mResult;
};

#endif // LOGCOMMANDSTATE_H
