#ifndef SCANLOGENTRY_H
#define SCANLOGENTRY_H

#include "baseentry.h"

using namespace std;

class LogEventEntry : public BaseEntry, public Serializable
{
public:
    enum LogType {
        SPECIAL = 0,
        SCAN_LOG,
        UPDATE_LOG,
    } mLogType;

    typedef enum {
        MANUAL_SCAN = 0,
        REMOTE_SCAN,
        TIMER_SCAN,
        SPECIAL_FOLDER_SCAN,
        ACTIVE_DEFENCE,
        PLUGIN_SCAN,
    } ScanType;

    typedef enum {
        ALL_DISK = 0,
        QUICK_DISK,
        USER_DISK,
        SPECIAL_FILE,
        MONITOR_FILE,
        MEMORY
    }DiskType;

    int mLogExtend;

    enum Status {
        FINISHED,
        INTERRUPT,
        SCANNING,
    } mStatus;

    union {
        int mScanCount;
        int mReserved1;
    };
    int mVirusCount;
    int mKilledCount;
    int mDeletedCount;
    int mFailedCount;
    int mIgnoreCount;
    int mStartTime;
    int mEndTime;
    /**
     * 用户扫描
     * 如果为全盘/快速扫描，mScanPath只保存用户的发起者， 自定义扫描会保存路径
     * 以；号隔开
     */
    string mScanPath;
    int mTotalCount;
    int mReported;

public:
    LogEventEntry() {
        mLogType = SCAN_LOG;
        mLogExtend = 0;
        mStatus = FINISHED;
        mScanCount = 0;
        mVirusCount = 0;
        mKilledCount = 0;
        mDeletedCount = 0;
        mFailedCount = 0;
        mIgnoreCount = 0;
        mStartTime = 0;
        mEndTime = 0;
        mTotalCount = 0;
        mReported = 0;
    }

    ScanType GetScanType() {
        return (ScanType)(mLogExtend>>16 & 0xffff);
    }

    DiskType GetDiskType() {
        return (DiskType)(mLogExtend & 0xffff);
    }

    static int SetScanType(ScanType scanType, DiskType diskType) {
        return scanType<<16|diskType;
    }

    static ScanType GetScanType(int type) {
        return (ScanType)(type>>16 & 0xffff);
    }

    static DiskType GetDiskType(int type) {
        return (DiskType)(type & 0xffff);
    }

    string GetLogUser();

public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    string Last() const;

    void Mapping(CppSQLite3Query *query);

protected:
    void InitializeJson(Json::Value& json);
    void ExternlizeJson(Json::Value& json);
};

#endif // SCANLOGENTRY_H
