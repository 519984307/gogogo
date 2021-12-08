#include "logcommandstate.h"
#include "commonapi.h"


LogCommandState::LogCommandState(
        const string& eventSource,
        const string& username,
        const string& mname,
        const string& guid,
        const string& version,
        const string& source,
        const string& eid,
        const string& sguid,
        const string& productid) {
    mEventtype = EVENTLOG_INFORMATION_TYPE;
    mEventlevel = INFORMATIONAL_MESSAGE;
    mCategory = 0;
    mDescription = "reportevent";

    mEventsource = eventSource;
    mUsername = username;
    mMachineName = mname;
    mGUID = guid;
    mVersion = version;
    mSource = source;

    mEID = eid;
    mSGUID = sguid;

    mProductID = productid;

    mInsertTime = Utils::Date::FormatTimeStamp(time(0));
}

void LogCommandState::SetCommandData(const std::string& action, const std::string& cmdid, int type, int result, const std::string& reason) {
    mAction = action;
    mCmdID = cmdid;
    mType = type;
    mResult = result;
    mReason = reason;
}

string LogCommandState::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_PC_COMMAND_EVENT);
}

Json::Value LogCommandState::toJSON() {
    Json::Value value = LogDataBasic::toJSON();

    value["inserttime"] = Json::Value(mInsertTime);
    value["eid"] = Json::Value(mEID);
    value["sguid"] = Json::Value(mSGUID);
    value["productid"] = Json::Value(mProductID);
    value["type"] = Json::Value(mType);
    value["result"] = Json::Value(mResult);
    value["action"] = Json::Value(mAction);
    value["cmdid"] = Json::Value(mCmdID);
    value["reason"] = Json::Value(mReason);

    return value;
}
