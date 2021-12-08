#ifndef SCHEDULEENTRY_H
#define SCHEDULEENTRY_H

#include "baseentry.h"

#include <string>
#include <vector>

using namespace std;

class ScheduleEntry : public BaseEntry
{
public:
    ScheduleEntry() {
        mType = RESTART;
        mCrontab = 0;
        mLastTime = 0;
        mUserSet = false;
        mLocked = false;
        mFuncEnum = QUICK_SCAN;
    }

public:
    typedef enum ScheduleType{
        RESTART,
        DAYS,
        WEEKS,
        MONTH,
        ALL
    }ScheduleType;
    ScheduleType mType;

    union {
        int mCrontab;
        int mPeriod;
    };

    long mLastTime;

    typedef enum ScanType{
        QUICK_SCAN = 1,
        ALL_SCAN,
        LINUX_SCAN, //
        UPDATE
    }ScanType;
    ScanType  mFuncEnum;
    bool      mUserSet;
    bool      mLocked;
    std::string mScanPath;
    std::string mScanExts;

    bool operator == (ScheduleEntry& entry) {
        return mType == entry.mType &&
                mLastTime == entry.mLastTime &&
                mFuncEnum == entry.mFuncEnum &&
                mUserSet == entry.mUserSet &&
                mLocked == entry.mLocked &&
                mScanPath == entry.mScanPath &&
                mScanExts == entry.mScanExts &&
                mPeriod == entry.mPeriod;
    }

public:
    inline int GetCrontabMinute() const {
        return mCrontab&0x3F;
    }

    inline void SetCrontabMinute(int minute) {
        mCrontab |= minute&0x3F;
    }

    inline int GetCrontabHour() const {
        return mCrontab>>6&0x1F;
    }

    inline void SetCrontabHour(int hour) {
        mCrontab |= (hour&0x1F) << 6;
    }

    inline int GetCrontabDayInWeek(std::vector<int>& days) {
        int temp = mCrontab>>11&0x7F;
        for (int i=0; i<7; i++) {
            if (temp >> i & 0x1) {
                days.push_back(i);
            }
        }
        return days.size();
    }

    inline void SetCrontabDayInWeek(std::vector<int>& days) {
        int temp = 0;
        std::vector<int>::iterator iter;
        for (iter=days.begin(); iter!=days.end(); ++iter) {
            temp |= 0x1<<(*iter);
        }

        mCrontab |= (temp&0x7F)<<11;
    }


    inline int GetCrontabDayInMonth() const {
        return mCrontab>>18&0x1F;
    }

    inline void SetCrontabDayInMonth(int day) {
        mCrontab |= (day&0x1F)<<18;
    }

    inline int GetCrontabMonthInYear() const {
        return mCrontab>>22&0x1F;
    }

    bool operator <(const ScheduleEntry& entry) {
        return mCrontab < entry.mCrontab;
    }



public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    void Mapping(CppSQLite3Query *query);
};

#endif // SCHEDULEENTRY_H
