#ifndef SCHEDULEENGINE_H
#define SCHEDULEENGINE_H

#include <list>
#include <time.h>
#include <pthread.h>

class ScheduleEntry;
class MutexLocker;
class ScheduleObserver {
public:
    virtual void OnScheduleEvent(int event, int type, void* object) = 0;
};

class MultiVirusEngine;
class Storage;
class ScheduleEngine
{
public:
    static ScheduleEngine* NEW(Storage *storage);
    ~ScheduleEngine();

    void SetObserver(ScheduleObserver *observer) {
        mObserver = observer;
    }

    void Work();
    int Start();
    int Stop();

    int Reload();

private:
    ScheduleEngine(Storage *storage);
    int Construct();

private:
    bool MatchCron(time_t time, ScheduleEntry& entry);

    static void *ThreadFunction(void *args);

private:
    Storage             *mStorage;

    std::list<ScheduleEntry> mScheduleTime;

    ScheduleObserver    *mObserver;

    pthread_t   mThread;

    bool mStatus;

    MutexLocker *mLocker;

};

#endif // SCHEDULEENGINE_H
