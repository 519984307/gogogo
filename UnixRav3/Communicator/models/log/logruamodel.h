#ifndef LOGRUAMODEL_H
#define LOGRUAMODEL_H

#include "logdatabasic.h"

#include <string>


class LogRuaModel : public LogDataBasic {
public:

public:
    LogRuaModel(LogRuaSourceType source,
                LogRuaActionType action,
                LogRuaRoleType role,
                const std::string& oldver,
                const std::string& newver,
                bool needreboot);

    Json::Value toJSON();

public:
    int mFlowID;
    LogRuaSourceType mSource;
    LogRuaActionType mAction;
    LogRuaRoleType mRole;
    std::string mOldVersion;
    std::string mNewVersion;
    int mNeedReboot;
    int mAfterReboot;
    std::string mInfo;
};

#endif // LOGRUAMODEL_H
