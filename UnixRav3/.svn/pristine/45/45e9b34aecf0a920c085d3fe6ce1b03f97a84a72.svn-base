#include "scanthreadvari.h"
#include "scanengine.h"
#include "engineobserver.h"
#include "scanconfig.h"
#include "scanengine_global.h"
#include "commonlocker.h"
#include "commonapi.h"
#include "baseconfig.h"

#include <iostream>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

ScanThreadVari* ScanThreadVari::NEW(
        EngineObserver *observer,
        int tag,
        const string& rmd,
        const string& libpath,
        ScanConfig *config) {
    ScanThreadVari *self = new ScanThreadVari(observer, tag);
    if (self) {
        if (self->Construct(rmd, libpath, config)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int ScanThreadVari::Construct(const string& rmd, const string& libpath, ScanConfig *config) {
    mLogData = CircleLink<string>::NEW(MAX_LOG_POOL);

    mEmptyCond = CondLocker::NEW();
    mPauseCond = CondLocker::NEW();
    mFullScanMutex = MutexLocker::NEW();

    mScanEngine = ScanEngine::NEW(this, rmd, libpath, config);
    ASSERT_ZERO(mScanEngine, -1);

    mUseCache = config->mUseCache;
    mScanConfig = config;

    return pthread_create(&mThread, NULL, ThreadWork, this);
}

ScanThreadVari::ScanThreadVari(EngineObserver *observer, int tag) : mFinished(false), mSize(0), mTag(tag)  {
    for (int i=0;i<MAX_FILE_POOL;++i) {
        mScanPool.push_back(new ScanEntry(MAX_FILE_LEN));
    }

    mStatus = INITILIZE;
    mScanEngine = NULL;
    mObserver = observer;
    mLogData = NULL;
    mScanCount = 0;
    mVirusCount = 0;
    mThread = 0;
    mSleepTime = 100;
    mUseCache = false;
    mCloudValid = false;
    mLastRound = false;
    mScanConfig = NULL;
}

ScanThreadVari::~ScanThreadVari() {
    Stop();
    if (mThread > 0) {
        pthread_join(mThread, NULL);
    }

    SECURITY_FREE(mLogData);
    SECURITY_FREE(mEmptyCond);
    SECURITY_FREE(mPauseCond);
    SECURITY_FREE(mScanEngine);
    SECURITY_FREE(mFullScanMutex);
}

void* ScanThreadVari::ThreadWork(void *args) {
    ScanThreadVari *self = (ScanThreadVari*)args;
//#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "ScanThreadVari");
    pthread_setname_np(pthread_self(), threadName);
//#endif
    self->Work();

    if (self->mObserver) {
        self->mObserver->OnVirusEvent(VIRUS_JUDGE, JUDGE_EXIT, &self->mTag);
    }

    return NULL;
}

bool ScanThreadVari::Append(const char *dirName, const char *fileName) {
    bool resolve = false;
    if (STOP == mStatus) {
        return false;
    }

    if (!mFullScanMutex->TryLock()) {
        return resolve;
    }

    for (int i=0; i<MAX_FILE_POOL; i++) {
        if (mScanPool[i]->valid()) {
            if (0 == strcmp(dirName, "/")) {
                sprintf(mScanPool[i]->file, "/%s", fileName);
            }
            else {
                sprintf(mScanPool[i]->file, "%s/%s", dirName, fileName);
            }

            LOG_GJCZ_DEBUG("Append file array %s", mScanPool[i]->file);

            mScanPool[i]->state = ScanEntry::ENTRY_PREPARED;
            mSize++;
            resolve = true;
            break;
        }
    }
    if (Full()) {
        mEmptyCond->Signal();
    }
    mFullScanMutex->Unlock();

    return resolve;
}

int ScanThreadVari::AppendLog(const string &file) {
    if (file.find("::") == std::string::npos) {
        mScanCount++;
    }
    if (mLogData) {
        mLogData->Append(file);
    }
    return 0;
}

void ScanThreadVari::LocalScanFile(int pos) {
    if (strlen(mScanPool[pos]->file) == 0) {
        return;
    }

    bool need_scan = true;
    if (mUseCache) {
        if (mObserver) {
            need_scan = mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_CACHE, mScanPool[pos]->file) < 0;
        }
    }

    if (need_scan) {
        // 如果检查开启了智能扫描, 那么非以下路径的不过引擎
        if ((mScanConfig->mScanIntelligent) &&
             !(0 == memcmp(mScanPool[pos]->file, "/home", 5)
                    || 0 == memcmp(mScanPool[pos]->file, "/root", 5)
                    || 0 == memcmp(mScanPool[pos]->file, "/media", 6)
                    || 0 == memcmp(mScanPool[pos]->file, "/run/media", 10))) {
                srand((int)time(NULL));
                usleep(10000 + rand()%10000);

                // set the log show
                strcpy(mFileNameCache, mScanPool[pos]->file);

                AppendLog(mScanPool[pos]->file);
                if (mObserver) {
                    mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_START, mScanPool[pos]->file);
                }
        }
        else
        {
            LOG_GJCZ_DEBUG("into engine %s", mScanPool[pos]->file);
            HRESULT result = mScanEngine->ScanOneFile(mScanPool[pos]->file);

            LOG_GJCZ_DEBUG("out engine %s and ret is %d", mScanPool[pos]->file, result);

            if (S_OK == result) {
                // save the white file hash
                if (!mCurrentIsVirus) {
                    if (mObserver) {
                        mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::CACHE_FILE, mScanPool[pos]->file);
                    }
                }
            }
        }
    }
    else {
        // set the log show
        strcpy(mFileNameCache, mScanPool[pos]->file);

        AppendLog(mScanPool[pos]->file);
        if (mObserver) {
            mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_START, mScanPool[pos]->file);
        }
    }

    mScanPool[pos]->consume();
    mSize--;

    // jude the CPU
    usleep(mSleepTime);
    //
    if (mObserver) {
        mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_END, NULL);
    }
    LOG_GJCZ_DEBUG("thread %d scan file end", mTag);
}

inline void ScanThreadVari::CloudScanPool() {
    if (!mObserver) {
        return;
    }

    // send the multiengine to mainengine, send to GLTX
    if (mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_CLOUD, (void*)&mScanPool)) {
        mCloudValid = false;
    }
    else {
        for (int i=0; i<mSize; ++i) {
            if (mScanPool[i]->state == ScanEntry::CLOUD_BLANK) {
                pair<string, string> info = make_pair(string(mScanPool[i]->file), string(mScanPool[i]->virus));
                mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_DANGE, &info);
                mScanPool[i]->consume();
                mSize--;
            }
            else if (mScanPool[i]->state == ScanEntry::CLOUD_WHITE) {
                mScanPool[i]->consume();
                mSize--;
            }
        }
    }
}

inline bool ScanThreadVari::CloudDataPrepared() {
    if (!mCloudValid) {
        return false;
    }
    if (mSize == 0) {
        return false;
    }
    if (mLastRound) {
        return true;
    }
    for (int i=0; i<MAX_FILE_POOL; ++i) {
        if (strlen(mScanPool[i]->hash) == 0) {
            return  false;
        }
    }
    return true;
}

inline bool ScanThreadVari::NeedCloudScan(int pos) {
    if (mCloudValid && mScanPool[pos]->state==ScanEntry::ENTRY_PREPARED) {
        if (!strlen(mScanPool[pos]->hash)) {
            string hash;
            if (0 == mScanEngine->GetFileSIG(mScanPool[pos]->file, hash)) {
                strcpy(mScanPool[pos]->hash, hash.c_str());
                return true;
            }
        }
        else {
            return true;
        }
    }
    return false;
}

void ScanThreadVari::Work() {
    mStatus = SCANING;
    while (!mFinished) {
        // scan file
        if (Full()) {
            mFullScanMutex->Lock();
            for (int i=0; i<MAX_FILE_POOL; ++i) {
                // judge the scan status
                if (PAUSE == mStatus) {
                    mPauseCond->Wait();
                }
                else if (STOP == mStatus) {
                    mFinished = true;
                    continue;
                }

                if (!NeedCloudScan(i)) {
                    LocalScanFile(i);
                }
            }
            if (CloudDataPrepared()) {
                CloudScanPool();
            }
            if (mObserver) {
                mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_OUT, NULL);
            }
            mFullScanMutex->Unlock();
        }
        else {
            if (mLastRound) {
                mFinished = true;
            }
            else {
                mEmptyCond->Wait();
            }
        }
    }

    // status is STOP, return immediately
    if (STOP == mStatus) {
        return;
    }
    // other, scan the last
    do {
        for (int i=0; i<MAX_FILE_POOL; ++i) {
            if (!NeedCloudScan(i)) {
                LocalScanFile(i);
            }
        }
        if (CloudDataPrepared()) {
            CloudScanPool();
        }
    }while(!Empty());
}

void ScanThreadVari::Pause() {
    if (SCANING == mStatus) {
        mStatus = PAUSE;
    }
}

void ScanThreadVari::Continue() {
    if (PAUSE == mStatus) {
        mStatus = SCANING;
        mPauseCond->Signal();
    }
}

void ScanThreadVari::Last() {
     mLastRound = true;
     mEmptyCond->Signal();
}

void ScanThreadVari::Stop() {
    mStatus = STOP;
    mFinished = true;
    if (mScanEngine) {
        mScanEngine->Terminal();
    }

    // broadcast the wait
    mEmptyCond->Broadcast();
    mPauseCond->Broadcast();
}

int ScanThreadVari:: OnVirusEvent(EngineEvent event, EngineType type, void *data) {
    if (VIRUS_JUDGE == event) {
        switch (type) {
        case JUDGE_DANGE: {
            mCurrentIsVirus = true;
            mVirusCount++;
            if (mObserver) {
                return mObserver->OnVirusEvent(event, type, data);
            }
            break;
        }
        case JUDGE_START: {
            mCurrentIsVirus = false;
            string& fileName = *(string*)data;

            // set the log show
            strncpy(mFileNameCache, fileName.c_str(), 1023);

            AppendLog(fileName);

            if (mObserver) {
                mObserver->OnVirusEvent(event, type, data);
            }
            break;
        }
        case JUDGE_END: {
            if (mObserver) {
                mObserver->OnVirusEvent(event, type, data);
            }
            break;
        }
        default:
            break;
        }
    }
    return 0;
}
