#ifndef LOGDATAVIRUS_H
#define LOGDATAVIRUS_H

#include "logdatabasic.h"
#include "logdatascanevent.h"

class LogDataVirus : public LogDataBasic
{
public:
    LogDataVirus(LogDataScanEvent::LogScanType type,
                 LogDataScanEvent::CommandSender sender,
                 const string& eventSource,
                 const string& username,
                 const string& mname,
                 const string& guid,
                 const string& version,
                 const string& source);
    void SetVirusData(long foundtime, const std::string& filepath, int virusid, const std::string& virusname, int deal, int state);

public:
    string getLogType();
    Json::Value toJSON();

private:
    int mTaskid;
    string mTaskname;
    int mAppid;
    string mFindtime;
    string mFilepath;
    int mVirusid;
    int mEngid;
    int mVirusclass;
    string mVirusname;
    int mTreatmethod;
    int mState;
    int mParentid;
    int mUniquevalue;
};

#endif // LOGDATAVIRUS_H
