#ifndef LOGDATABASIC_H
#define LOGDATABASIC_H

#include "abstractlogdatamodel.h"
#include "../modeldefine.h"

class LogDataBasic
{
public:
    LogDataBasic();

public:
    string getLogType();
    Json::Value toJSON();

public:
    /** 日志的生成时间 */
    string   mTime;
    /** 事件类型 */
    EventType mEventtype;
    /** 定义为优先级。代表了消息的紧急程度 */
    EventLevel mEventlevel;
    /** 事件来源产生事件的子产品名、子系统名、子功能名、模块名、进程名、服务名等 */
    string   mEventsource;
    /** 标记来源于windows系统中的EventID事件ID ＝日志表的编号×100 ＋ ID编号 */
    int      mCategory;
    /** 用户名每个日志表自己定义要取的用户账号 */
    string   mUsername;
    /** 研发提供(类似主键)bigint型 */
    int     mUniquevalue;
    /**  描述 */
    string  mDescription;

    string  mMachineName;
    string  mGUID;
    string  mVersion;
    string  mSource;
};

#endif // LOGDATABASIC_H
