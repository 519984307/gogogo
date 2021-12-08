#ifndef SCANTHREADVARI_H
#define SCANTHREADVARI_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "circlelink.h"
#include "scanengine_global.h"

#define MAX_FILE_POOL   10
#define MAX_FILE_LEN    1024
#define MAX_LOG_POOL    10

#include "engineobserver.h"

using namespace std;

class ScanEngine;
class ScanConfig;
class ScanEntry;
class CondLocker;
class MutexLocker;
class ScanThreadVari : EngineObserver {
private:
    /** file name cache, for scan report, */
    char mFileNameCache[MAX_FILE_LEN];
    int  mSize;

    bool mFinished;
    bool mLastRound;
    int  mTag;

    /** use the cache file to incream the scan progress */
    bool mUseCache;
    /** define the scaned current file state */
    bool mCurrentIsVirus;

    bool mCloudValid;

    unsigned int mScanCount;
    unsigned int mVirusCount;

    pthread_t       mThread;

    CondLocker      *mEmptyCond;
    CondLocker      *mPauseCond;
    /** 扫描时的独占锁，保证在扫描时，不允许往文件池里增加文件 */
    MutexLocker     *mFullScanMutex;

    typedef enum {
        INITILIZE,
        SCANING,
        PAUSE,
        STOP
    }ScanStatus;
    ScanStatus mStatus;

    ScanEngine      *mScanEngine;
    EngineObserver  *mObserver;

    ScanConfig      *mScanConfig;

    CircleLink<string>   *mLogData;

    vector<ScanEntry*>  mScanPool;

public:
    static ScanThreadVari* NEW(EngineObserver *observer, int tag, const string& rmd, const string& libpath, ScanConfig *config);
    virtual ~ScanThreadVari();

    void Work();

    void Finished() {
        mFinished = true;
    }

    void SetObserver(EngineObserver *observer) {
        mObserver = observer;
    }

    void SetCloudState(bool state) {
        mCloudValid = state;
    }


private:
    ScanThreadVari(EngineObserver *observer, int tag);
    int Construct(const string& rmd, const string& libpath, ScanConfig *config);

    static void* ThreadWork(void *args);

    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

    inline void LocalScanFile(int pos);
    inline void CloudScanPool();
    inline bool CloudDataPrepared();
    inline bool NeedCloudScan(int pos);   

public:
    inline bool Full() const {
        return mSize == MAX_FILE_POOL;
    }
    inline bool Empty() const {
        int i=0;
        for (;i<MAX_FILE_POOL && mScanPool[i]->state==ScanEntry::ENTRY_EMPTY;++i);
        return mSize == 0 || i==MAX_FILE_POOL;
    }
    inline int Tag() const  {
        return mTag;
    }
    inline const int* TagPtr() const {
        return &mTag;
    }
    inline unsigned int ScanCount() const {
        return mScanCount;
    }
    inline unsigned int VirusCount() const {
        return mVirusCount;
    }
    inline const unsigned int* ScanCountPtr() const {
        return &mScanCount;
    }
    inline const unsigned int* VirusCountPtr() const {
        return &mVirusCount;
    }
    inline const string CurrentLog() const {
        return mLogData->Current();
    }
    inline const char* CurrentFile() const {
        return mFileNameCache;
    }

    bool Append(const char *dirName, const char *fileName);

    int AppendLog(const string& file);

public:
    void Pause();
    void Continue();
    void Stop();
    /** it mean the pool is the last file, scan finish, then can exit the thread */
    void Last();

    void AdjustPayload(int type) {
        if (type > 0) {
            mSleepTime += 10;
        }
        else if (type < 0) {
            mSleepTime -= 10;
            if (mSleepTime < 0) {
                mSleepTime = 0;
            }
        }
    }

private:
    int mSleepTime;
};

#endif // SCANTHREADVARI_H
