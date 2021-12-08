#ifndef LOGDATASCANEVENT_H
#define LOGDATASCANEVENT_H

#include "logdatabasic.h"

class LogDataScanEvent : public LogDataBasic
{
public:
    typedef enum {
        QUICK_SCAN,
        ALL_SCAN,
        CUSTOM_SCAN,
        TIME_SCAN,
        FILEMON_SCAN,
    }LogScanType;

    typedef enum {
        USER_SCREEN     = 0,
        REMOTE_CENTER,
        TIMER_SCHEDULE,
        FILE_MONITOR    = 100,
        MAIL_MONITOR,
        SHARE_MONITOR,
        NETDISK_UPLOAD,
        NETDISK_DOWNLOAD,
        FTP_MONITOR,
        UDISK_MONITOR   = 200,
        EMBED_SCAN,
        DOC_SHARED     = 300,
    }CommandSender;

    typedef enum {
        LOG_STATE_IDLE  = 0,
        LOG_STATE_STARTING,
        LOG_STATE_RUNNING,
        LOG_STATE_PAUSE,
        LOG_STATE_STOPING,
        LOG_STATE_STOPED,
        LOG_STATE_FINISHED,
        LOG_STATE_TASK_FINISHED,
    }LogScanState;

public:
    LogDataScanEvent(LogScanType type,
                     CommandSender sender,
                     const string& eventSource,
                     const string& username,
                     const string& mname, const string& guid,
                     const string& version,
                     long taskid,
                     const string& source);

    void SetLogData(string starttime,
                    int runtime,
                    int scancount,
                    int viruscount,
                    int treatedcount,
                    int totalfilecount,
                    int scanfilecount,
                    LogScanState state);

public:
    string getLogType();
    Json::Value toJSON();

private:
    long mTaskid;
    string mTaskname;
    int mAppid;
    string mStarttime;
    int mRuntime;
    int mState;
    int mScancount;
    int mViruscount;
    int mTreatedcount;
    int mTotalfilecount;
    int mScanfilecount;
};

#endif // LOGDATASCANEVENT_H
