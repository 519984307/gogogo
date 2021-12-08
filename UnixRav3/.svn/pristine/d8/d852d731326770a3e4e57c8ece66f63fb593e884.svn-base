#include "logdatascanevent.h"
#include "commonapi.h"
#include "../modeldefine.h"

#define TASKID       "taskid"
#define TASKNAME     "taskname"
#define APPID        "appid"
#define STARTTIME    "starttime"
#define RUNTIME      "runtime"
#define STATE        "state"
#define SCANCOUNT    "scancount"
#define VIRUSCOUNT   "viruscount"
#define TREATEDCOUNT  "treatedcount"
#define TOTALFILECOUNT  "totalfilecount"
#define SCANFILECOUNT  "scanfilecount"


#define TASK_NAME_ALLSCAN   "allscan"
#define TASK_NAME_QUICKSCAN "quickscan"
#define TASK_NAME_CUSTOM    "customscan"
#define TASK_NAME_FILEMON   "filemon"
#define TASK_NAME_MAILMON   "mailmon"

LogDataScanEvent::LogDataScanEvent(LogScanType type,
                                   CommandSender sender,
                                   const string& eventSource,
                                   const string& username,
                                   const string& mname, const string& guid,
                                   const string& version,
                                   long taskid,
                                   const string& source) {
    mEventtype = EVENTLOG_INFORMATION_TYPE;
    mEventlevel = INFORMATIONAL_MESSAGE;
    mCategory = 0;

    mEventsource = eventSource;
    mUsername = username;
    mMachineName = mname;
    mGUID = guid;
    mVersion = version;
    mSource = source;
    mTaskid = taskid;
    mAppid = sender;

    if (type == QUICK_SCAN) {
        mTaskname = TASK_NAME_QUICKSCAN;
    }
    else if (type == ALL_SCAN) {
        mTaskname = TASK_NAME_ALLSCAN;
    }
    else if (type == CUSTOM_SCAN) {
        mTaskname = TASK_NAME_CUSTOM;
    }
    else if (type == TIME_SCAN) {
        mTaskname = TASK_NAME_ALLSCAN;
    }
    else if (type == FILEMON_SCAN) {
        mTaskname = TASK_NAME_FILEMON;
    }

    mState = LOG_STATE_IDLE;
}

void LogDataScanEvent::SetLogData(string starttime,
                                  int runtime,
                                  int scancount,
                                  int viruscount,
                                  int treatedcount,
                                  int totalfilecount,
                                  int scanfilecount,
                                  LogDataScanEvent::LogScanState state) {
    mStarttime = starttime;
    mRuntime = runtime;
    mScancount = scancount;
    mViruscount = viruscount;
    mTreatedcount = treatedcount;
    mTotalfilecount = totalfilecount;
    mScanfilecount = scanfilecount;
    mState = state;
}

string LogDataScanEvent::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_TYPE_VIRUS_SCAN);
}

Json::Value LogDataScanEvent::toJSON() {
    Json::Value value = LogDataBasic::toJSON();

    value[TASKID] = Json::Value((int)mTaskid);
    value[TASKNAME] = Json::Value(mTaskname);
    value[APPID] = Json::Value(mAppid);
    value[STARTTIME] = Json::Value(mStarttime);
    value[RUNTIME] = Json::Value(mRuntime);
    value[STATE] = Json::Value(mState);
    value[SCANCOUNT] = Json::Value(mScancount);
    value[VIRUSCOUNT] = Json::Value(mViruscount);
    value[TREATEDCOUNT] = Json::Value(mTreatedcount);
    value[TOTALFILECOUNT] = Json::Value(mTotalfilecount);
    value[SCANFILECOUNT] = Json::Value(mScanfilecount);

    return value;
}

