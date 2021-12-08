#ifndef REPORTLOGMODEL_H
#define REPORTLOGMODEL_H

#include "../modeldefine.h"
#include "abstractlogdatamodel.h"

#include <list>

#define LOGTYPE  "logtype"
#define DWITH    "dwith"
#define DATA     "data"

template<class T>
class ReportLogModel
{
public:
    ReportLogModel(LogType type, LogDealType mode, T* log) {
        mLogType = type;
        mDealType = mode;
        mDatas.push_back(log);
    }

    ReportLogModel(LogType type, LogDealType mode, std::list<T*> logs){
        mLogType = type;
        mDealType = mode;
        mDatas.merge(logs);
    }

public:
    Json::Value toJSON(){
        Json::Value value;

        value[LOGTYPE] = Json::Value(ENUM_VALUE(LogTypeContent, mLogType));
        value[DWITH] = Json::Value(mDealType);
        if (!mEID.empty()) {
            value["eid"] = Json::Value(mEID);
        }
        if (!mSGUID.empty()) {
            value["sguid"] = Json::Value(mSGUID);
        }
        if (!mSysType.empty()) {
            value["systype"] = Json::Value(mSysType);
        }

        Json::Value jarray;
        typename std::list<T*>::iterator iter;
        for (iter=mDatas.begin(); iter!=mDatas.end(); ++iter) {
            jarray.append((*iter)->toJSON());
        }
        if (jarray.size() > 0) {
            value[DATA] = jarray;
        }
         return value;
    }
    bool validate() {
        return false;
    }

public:
    LogType      mLogType;
    LogDealType  mDealType;

    // 以下代码是为epstate特制，搞不清楚，为什么在epstate里，这些定义在上面
    std::string  mEID;
    std::string  mSGUID;
    std::string  mSysType;

    std::list<T*> mDatas;
};

#endif // REPORTLOGMODEL_H
