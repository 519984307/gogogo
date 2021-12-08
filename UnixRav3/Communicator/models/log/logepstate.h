#ifndef LOGEPSTATE_H
#define LOGEPSTATE_H

#include "logdatabasic.h"

using namespace std;
class LogEPState : public LogDataBasic
{
public:
    LogEPState(const string& eventSource,
               const string& username,
               const string& key,
               const string& value);
    LogEPState();

    Json::Value toJSON();
public:
    /** 表示各种组件的属性名称具体对应内容见下表 */
    string   mKey;
    /** 表示组件的启用状态 */
    string   mValue;
};

#endif // LOGEPSTATE_H
