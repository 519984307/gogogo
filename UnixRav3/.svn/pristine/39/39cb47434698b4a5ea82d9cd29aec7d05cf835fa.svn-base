#include "scheduleengine.h"
#include "multivirusengine.h"
#include "Entry/scheduleentry.h"
#include "storage.h"
#include "commonlocker.h"

#include <unistd.h>

ScheduleEngine* ScheduleEngine::NEW(Storage *storage) {
    ScheduleEngine *self = new ScheduleEngine(storage);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

ScheduleEngine::ScheduleEngine(Storage *storage) {
    mStorage = storage;
    mObserver = NULL;
    mLocker = NULL;
    mStatus = false;
}

ScheduleEngine::~ScheduleEngine() {
    if (mLocker) {
        delete mLocker;
        mLocker = NULL;
    }
}

int ScheduleEngine::Construct() {
    mLocker = MutexLocker::NEW();

    try {
        Reload();
    }
    catch(...) {
        return -1;
    }

    return 0;
}

void ScheduleEngine::Work() {
    while (mStatus) {
        std::list<ScheduleEntry>::iterator iter;
        for (iter=mScheduleTime.begin(); iter!=mScheduleTime.end(); ++iter) {
            time_t timep;
            time(&timep);

            if (MatchCron(timep, *iter)) {
                if (mObserver) {
                    ScheduleEntry &matched = *iter;
                    mObserver->OnScheduleEvent(iter->mFuncEnum, 0, (void*)&matched);
                }
            }
        }

        pthread_testcancel();
        sleep(60);
    }
}

int ScheduleEngine::Start() {
    mStatus = true;

    std::list<ScheduleEntry>::iterator iter;
    for (iter=mScheduleTime.begin(); iter!=mScheduleTime.end(); ++iter) {
        if (iter->mType == ScheduleEntry::RESTART) {
            if (mObserver) {
                mObserver->OnScheduleEvent(iter->mFuncEnum, 0, NULL);
            }
        }
    }

    return pthread_create(&mThread, NULL, ThreadFunction, this);
}

int ScheduleEngine::Stop() {
    mStatus = false;
    pthread_cancel(mThread);
    pthread_join(mThread, NULL);
}

void* ScheduleEngine::ThreadFunction(void *args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);

    ScheduleEngine *engine = (ScheduleEngine*)args;
    engine->Work();

    pthread_exit(0);
    return NULL;
}

int ScheduleEngine::Reload() {
    mLocker->Lock();
    {
        mScheduleTime.clear();
        mStorage->GetAvailableScheduleList(mScheduleTime);
    }
    mLocker->Unlock();
    return 0;
}

bool ScheduleEngine:: MatchCron(time_t time, ScheduleEntry& entry) {
  struct tm *p = localtime(&time);

    switch (entry.mType) {
//    case ScheduleEntry::RESTART: {
//        return true;
//    }
    case ScheduleEntry::DAYS: {
        if (p->tm_hour == entry.GetCrontabHour()
                && p->tm_min == entry.GetCrontabMinute()) {
            return true;
        }
        break;
    }
    case ScheduleEntry::WEEKS: {
        std::vector<int> days;
        entry.GetCrontabDayInWeek(days);
        std::vector<int>::iterator iter;
        for (iter=days.begin(); iter!=days.end(); ++iter) {
            if (*iter == p->tm_wday
                    && p->tm_hour == entry.GetCrontabHour()
                    && p->tm_min == entry.GetCrontabMinute()) {
                return true;
            }
        }
        break;
    }
    case ScheduleEntry::MONTH: {
        if (p->tm_mday == entry.GetCrontabDayInMonth()
                && p->tm_hour == entry.GetCrontabHour()
                && p->tm_min == entry.GetCrontabMinute()) {
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}
