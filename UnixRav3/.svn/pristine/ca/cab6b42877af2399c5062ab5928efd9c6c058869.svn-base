#include "logruamodel.h"

#include "commonapi.h"

LogRuaModel::LogRuaModel(LogRuaSourceType source,
                        LogRuaActionType action,
                        LogRuaRoleType role,
                        const std::string& oldver,
                        const std::string& newver,
                        bool needreboot) {
    mSource = source;
    mAction = action;
    mRole = role;
    mOldVersion = oldver;
    mNewVersion = newver;
    mNeedReboot = needreboot;

    mFlowID = Utils::Date::GetTimeStamp();
    mAfterReboot = 0;
}

Json::Value LogRuaModel::toJSON() {
    Json::Value value = LogDataBasic::toJSON();
    value["flowid"] = Json::Value(mFlowID);
    value["source"] = Json::Value(mSource);
    value["action"] = Json::Value(mAction);
    value["role"] = Json::Value(mRole);
    value["oldver"] = Json::Value(mOldVersion);
    value["newver"] = Json::Value(mNewVersion);
    value["needreboot"] = Json::Value(mNeedReboot);
    value["afterreboot"] = Json::Value(mAfterReboot);
    value["info"] = Json::Value(mInfo);
    return value;
}
