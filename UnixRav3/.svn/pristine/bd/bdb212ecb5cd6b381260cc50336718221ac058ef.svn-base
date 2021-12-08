#include "multivirusengine.h"
#include "scanengine_global.h"
#include "utils.h"
#include "scanengine.h"
#include "scanconfig.h"
#include "scanthreadvari.h"
#include "countfilevari.h"
#include "circlelink.h"
#include "get_cpu.h"
#include "scancache.h"
#include "commonlocker.h"
#include "baseconfig.h"
#include "commonapi.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fnmatch.h>
#include <utility>
#include <iostream>
#include <pthread.h>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

MultiVirusEngine* MultiVirusEngine::NEW(const string& rmd, const string& libpath, const string& cachepath) {
    MultiVirusEngine *engine = new MultiVirusEngine();
    if (engine) {
        if (engine->Construct(rmd, libpath, cachepath)) {
            delete engine;
            engine = NULL;
        }
    }
    return engine;
}

MultiVirusEngine::MultiVirusEngine()
{
    mConfig = NULL;
    mScanFileThread = NULL;
    mCountFileThread = NULL;
    mScanThreadCount = 0;
    mScanStopStatue = 0;
    mScanExitFlag = 0;
    mStatus = INIT;
    mScanCache = NULL;
    mCloudEngine = NULL;
    mReportLocker = NULL;
    mPauseLocker = NULL;
}

MultiVirusEngine::~MultiVirusEngine() {
    Release();
    SECURITY_FREE(mConfig);
    SECURITY_FREE(mPauseLocker);

    pthread_mutex_destroy(&mReportMutex);
    pthread_mutex_destroy(&mScanExitMutex);
    pthread_mutex_destroy(&mJudgeCacheMutex);
    pthread_mutex_destroy(&mScanStatusMutex);
    pthread_mutex_destroy(&mThreadCountMutex);
}

int MultiVirusEngine::Construct(const string& rmd, const string& libpath, const string& cachepath) {
    if (access(rmd.c_str(), R_OK|F_OK)) {
        return -1;
    }

    mRMDPath = rmd;
    mLibPath = libpath;
    mCachePath = cachepath;
    mConfig = new ScanConfig;

    pthread_mutex_init(&mReportMutex, NULL);
    pthread_mutex_init(&mScanExitMutex, NULL);
    pthread_mutex_init(&mJudgeCacheMutex, NULL);
    pthread_mutex_init(&mScanStatusMutex, NULL);
    pthread_mutex_init(&mThreadCountMutex, NULL);

    mPauseLocker = CondLocker::NEW();

    return 0;
}

bool MultiVirusEngine::DealFile(const char *dirName, const char *fileName) {
    bool deal = false;

	// here, ensure ok!!!
	while(START == mStatus) {
	    if (pthread_mutex_trylock(&mScanStatusMutex)) {
    		LOG_GJCZ(">> MultiVirusEngine::DealFile, query lock failed, to continue");
    		usleep(100000);
    		continue;
        }
		break;
    }

    do {
        for (int i=0; i<mScanThreadCount && !deal; ++i) {
            ScanThreadVari *vari = mScanThreadVari[i];
            deal = vari->Append(dirName, fileName);
            if (deal) {
                break;
            }
        }
        if (START != mStatus) {
            break;
        }
        if (!deal) {
            // 如果文件未能处理，延时100ms, 继续尝试
            usleep(100000);
        }
    }while(!deal);
    pthread_mutex_unlock(&mScanStatusMutex);

    return deal;
}

int MultiVirusEngine::Start(const std::string& path, int maxDepth, int threadCount) {
    std::list<pair<std::string,int> > paths;
    paths.push_back(make_pair(path, maxDepth));

    return Start(paths, threadCount, false);
}

int MultiVirusEngine::Start(const std::list<pair<std::string,int> >& path, int threadCount, bool intelligent) {
    // before start, check the thread status
    // TODO: the scan thread must not to be released, you can reuse it
    LOG_GJCZ_DEBUG("init the scan engine");
    pthread_mutex_lock(&mScanStatusMutex);

    Release();
    mStatus = START;
    mScanCount = 0;
    mAllCount = 0;
    mReportSummary.clear();

    mScanStopStatue = 0;
    mScanExitFlag = 0x1|0x2;
    for (int i=2; i<threadCount+2; ++i) {
        mScanExitFlag |= 0x1<<i;
    }

    mScanThreadCount = threadCount; 
    for (int i=0; i<threadCount; ++i) {
        LOG_GJCZ_DEBUG("create the judge file thread %d(%d)", i, threadCount);

        ScanThreadVari *child = ScanThreadVari::NEW(this, i, mRMDPath, mLibPath, mConfig);
        if (NULL == child) {
        	mStatus = STOP;
        	pthread_mutex_unlock(&mScanStatusMutex);
        	LOG_GJCZ("create scan thread failed -->%d\n", errno);
            return -1;
        }
        mScanThreadVari.push_back(child);
        mReportSummary.push_back(
                    ReportSummary(
                         mScanThreadVari[i]->CurrentFile(),
                         mScanThreadVari[i]->ScanCountPtr(),
                         mScanThreadVari[i]->VirusCountPtr(),
                         mScanThreadVari[i]->TagPtr()
                    )
        );
    }

    // load the scan cache
    mScanCache = ScanCache::NEW(mCachePath);

    LOG_GJCZ_DEBUG("create the count file thread");
    // start count file thread
    mCountFileThread = CountFileVari::NEW(this, false, &mWhiteList, mConfig->mScanFileType, &mScanFileType, intelligent);
    mCountFileThread->Start(path);

    LOG_GJCZ_DEBUG("create the scan file thread");
    // start travel dictionary
    mScanFileThread = CountFileVari::NEW(this, true, &mWhiteList, mConfig->mScanFileType, &mScanFileType, intelligent);
    mScanFileThread->Start(path);

    // start the cloud engine
//    mCloudEngine = CloudEngine::NEW(this);
//    mCloudEngine->Start(mConfig->mCloudAddr);

//    pthread_create(&mCheckPayloadThread, NULL, CPUPayloadThread, this);
    pthread_create(&mScanReportThread, NULL, ScanReportThread, this);
    mReportLocker = CondLocker::NEW();

    pthread_mutex_unlock(&mScanStatusMutex);

    return 0;
}

int MultiVirusEngine::Start(const std::list<pair<std::string,int> >& path) {
    return Start(path, 1, false);
}

int MultiVirusEngine::SetScanDynamicStatus(int thread, int cpuload) {
    if (thread == mScanThreadCount) {
        return -1;
    }

    Pause();

    pthread_mutex_lock(&mScanStatusMutex);

    int origin_count = mScanThreadCount;
    mScanThreadCount = thread;

    if (thread > origin_count) {
        // need add some scan thread
        for (int i=origin_count;i<thread;i++) {
            ScanThreadVari *child = ScanThreadVari::NEW(this, i, mRMDPath, mLibPath, mConfig);
            if (NULL == child) {
                return -1;
            }
            mScanThreadVari.push_back(child);
            mReportSummary.push_back(
                        ReportSummary(
                             mScanThreadVari[i]->CurrentFile(),
                             mScanThreadVari[i]->ScanCountPtr(),
                             mScanThreadVari[i]->VirusCountPtr(),
                             mScanThreadVari[i]->TagPtr()
                        )
            );
        }
    }
    else if (thread < origin_count) {
        // descream the thread
        for (int i=thread; i<origin_count; ++i) {
            delete mScanThreadVari[i];
        }
        mScanThreadVari.erase(mScanThreadVari.begin()+thread, mScanThreadVari.end());
        mReportSummary.erase(mReportSummary.begin()+thread, mReportSummary.end());
    }

    mScanExitFlag = 0x1|0x2;
    for (int i=2; i<mScanThreadCount+2; ++i) {
        mScanExitFlag |= 0x1<<i;
    }

    pthread_mutex_unlock(&mScanStatusMutex);

    Continue();
    return 0;
}

void MultiVirusEngine::Pause() {
    return;
    pthread_mutex_lock(&mScanStatusMutex);

    mStatus = PAUSE;

    mScanFileThread->Pause();
    for (int i=0; i<mScanThreadCount; ++i) {
        if (NULL != mScanThreadVari[i]) {
            mScanThreadVari[i]->Pause();
        }
    }

    pthread_mutex_unlock(&mScanStatusMutex);
}

void MultiVirusEngine::Stop() {
    NotifyScanFinished();
    mStatus = STOP;

    if (mCountFileThread){
        mCountFileThread->Stop();
    }
    if (mScanFileThread) {
        mScanFileThread->Stop();
    }

    for (int i=0; i<mScanThreadCount; ++i) {
        if (mScanThreadVari[i]) {
            mScanThreadVari[i]->Stop();
        }
    }
}

void MultiVirusEngine::Release() {
    mStatus = STOP;
    for (int i=0; i<mScanThreadCount; ++i) {
        SECURITY_FREE(mScanThreadVari[i]);
    }
    mScanThreadVari.clear();

    SECURITY_FREE(mScanFileThread);
    SECURITY_FREE(mCountFileThread);
    SECURITY_FREE(mScanCache);
    SECURITY_FREE(mCloudEngine);
    SECURITY_FREE(mReportLocker);

    mReportSummary.clear();
}

void MultiVirusEngine::Continue() {
    return;
    pthread_mutex_lock(&mScanStatusMutex);
    mStatus = START;
    mScanFileThread->Continue();
    for (int i=0; i<mScanThreadCount; ++i) {
        mScanThreadVari[i]->Continue();
    }
    mReportLocker->Signal();
    pthread_mutex_unlock(&mScanStatusMutex);
}

int MultiVirusEngine::OnVirusEvent(EngineEvent event, EngineType type, void *data) {
    int err = 0;
    if (COUNT_PATH == event && COUNT_FINISH == type) {
        mAllCount = *(long*)data;
        mReportData.mTotalCount = mAllCount;

        pthread_mutex_lock(&mScanExitMutex);
        mScanStopStatue |= 0x1;

        int finish_mask = 0;
        for (int i=0;i<mScanThreadCount+2;i++){
            finish_mask|=0x1<<i;
        }
        if ((mScanStopStatue&finish_mask) == mScanExitFlag) {
            NotifyScanFinished();
            if (mObserver) {
                mObserver->OnVirusEvent(event, ALL_FINISHED, &mReportData);
            }
        }
        pthread_mutex_unlock(&mScanExitMutex);
    }
    else if (SCAN_FILE == event) {
        switch (type) {
        case FILE_PATH: {
            if (mStatus == START) {
                pair<const char*, const char*> filePath = *((pair<const char*, const char*>*)data);
                DealFile(filePath.first, filePath.second);
            }
            break;
        }
        case SCAN_START: {
            mReportData.Started();
            break;
        }
        case SCAN_FINISH: {
            pthread_mutex_lock(&mScanExitMutex);
            mScanStopStatue |= 0x2;

            // travel thread finish, notify the check thread
            for (int i=0; i<mScanThreadCount; ++i) {
                mScanThreadVari[i]->Last();
            }

            int scanCount = *(int*)data;
            int finish_mask = 0;
            for (int i=0;i<mScanThreadCount+2;i++){
                finish_mask|=0x1<<i;
            }
            if ((mScanStopStatue&finish_mask) == mScanExitFlag || scanCount == 0) {
                NotifyScanFinished();
                if (mObserver) {
                    mObserver->OnVirusEvent(event, ALL_FINISHED, &mReportData);
                }
            }
            pthread_mutex_unlock(&mScanExitMutex);
            break;
        }
        default:
            break;
        }
    }
    else if (VIRUS_JUDGE == event) {
        switch (type) {
        case JUDGE_DANGE: {
            int resp = VirusReportData::STATUS_IGNORE;
            pthread_mutex_lock(&mReportMutex);
            {
                int virusCount = 0;
                for (int i=0; i<mScanThreadCount; ++i) {
                    virusCount += mScanThreadVari[i]->VirusCount();
                }
                mReportData.mVirusCount = virusCount;

                std::pair<std::string, std::string> *virusInfo = (std::pair<std::string, std::string>*)data;
                mReportData.Virus(virusInfo->first, virusInfo->second, resp);

                if (mObserver) {
                    resp = mObserver->OnVirusEvent(event, type, &mReportData);
                    switch (resp) {
                    case VirusReportData::STATUS_IGNORE:
                        mReportData.mVirusIgnoreCount++;
                        break;
                    case VirusReportData::STATUS_CLEAN:
                        mReportData.mVirusCleanCount++;
                        break;
                    case VirusReportData::STATUS_REMOVE:
                        mReportData.mVirusDeleteCount++;
                        break;
                    default:
                        break;
                    }
                }
            }
            pthread_mutex_unlock(&mReportMutex);
            return resp;
        }
        case JUDGE_START: {
            if (0 == pthread_mutex_trylock(&mReportMutex))
            {
                mScanCount = 0;
                for (int i=0; i<mScanThreadCount; ++i) {
                    mScanCount += mScanThreadVari[i]->ScanCount();

                    if (mObserver) {
                        mReportData.Scaning(mScanThreadVari[i]->CurrentLog());
                        mObserver->OnVirusEvent(event, type, &mReportData);
                    }
                }

                // set the scan report data
                mReportData.mScanCount = mScanCount;

                if (mObserver) {
                    mReportData.Progress(mAllCount?mScanCount*100/mAllCount:0);
                    mObserver->OnVirusEvent(event, type, &mReportData);
                }
                pthread_mutex_unlock(&mReportMutex);
            }            
            break;
        }
        case JUDGE_OUT: {
//            mAppendCond->Signal();

//            if (mScanStopStatue&0x2) {
//                pthread_mutex_lock(&mScanExitMutex);
//                for (int i=0; i<mScanThreadCount; ++i) {
//                    if (mScanThreadVari[i]->Empty()) {
//                        // this thread finished
//                        mScanThreadVari[i]->Finished();
//                        // TODO, 这里只是粗略的标记一个扫描线程完成
//                        for (int j=2; j<30; j++) {
//                            if ((mScanStopStatue>>j&0x1) == 0) {
//                                mScanStopStatue |= 0x1<<j;
//                                break;
//                            }
//                        }
//                    }
//                }

//                if (mScanStopStatue == mScanExitFlag) {
//                    NotifyScanFinished();
//                    if (mObserver) {
//                        mObserver->OnVirusEvent(event, ALL_FINISHED, &mReportData);
//                    }
//                }
//                pthread_mutex_unlock(&mScanExitMutex);
//            }
            break;
        }
        case JUDGE_EXIT: {
            pthread_mutex_lock(&mScanExitMutex);
            int tag = *(int*)data;
            if (tag>=0 && tag < 30) {
                if ((mScanStopStatue>>(tag+2)&0x1) == 0) {
                    mScanStopStatue |= 0x1<<(tag+2);;
                }
            }

            //TODO 粗略的标记一个线程完成，如果符合退出标志，那么认为扫描全部完成
            int finish_mask = 0;
            for (int i=0;i<mScanThreadCount+2;i++){
                finish_mask|=0x1<<i;
            }
            if ((mScanStopStatue&finish_mask) == mScanExitFlag) {
                NotifyScanFinished();
                if (mCloudEngine) {
                    mCloudEngine->Stop();
                }
                if (mObserver) {
                    mObserver->OnVirusEvent(event, ALL_FINISHED, &mReportData);
                }                
            }
            pthread_mutex_unlock(&mScanExitMutex);
            break;
        }
        case JUDGE_CACHE: {
            int ret = -1;
            pthread_mutex_lock(&mJudgeCacheMutex);
            {
                const char *file = (const char*)data;
                if (mScanCache) {
                    ret = mScanCache->ScanFile(file)?-1:0;
                }
            }
            pthread_mutex_unlock(&mJudgeCacheMutex);
            return ret;
        }
        case JUDGE_CLOUD: {
            if (mCloudEngine) {
                return mCloudEngine->CloudCheck((vector<ScanEntry*>*)data);
            }
        }
        case CACHE_FILE: {
            pthread_mutex_lock(&mJudgeCacheMutex);
            {
                const char *file = (const char*)data;
                if (mScanCache) {
                    mScanCache->CacheFile(file);
                }
            }
            pthread_mutex_unlock(&mJudgeCacheMutex);
            break;
        }
        default:
            break;
        }
    }
    else if (CLOUD_VIRUS == event) {
        if (CLOUD_CHECK == type) {
            if (mObserver) {
                err = mObserver->OnVirusEvent(event, type, data);
            }
        }
        else if (CLOUD_POST == type) {
            if (mObserver) {
                mObserver->OnVirusEvent(event, type, data);
            }
        }
        else if (CLOUD_STATE == type) {
            vector<ScanThreadVari*>::iterator iter;
            for (iter=mScanThreadVari.begin(); iter!=mScanThreadVari.end(); ++iter) {
                bool state = *(bool*)data;
                (*iter)->SetCloudState(state);
            }
        }
    }
    return err;
}

void MultiVirusEngine::NotifyScanFinished() {
    mReportData.Finished();
    //pthread_mutex_lock(&mScanStatusMutex);
     //如果线程处于 signal wait状态，让该线程退出
    if(mStatus != STOP){
        //pthread_cancel(mScanReportThread);
    }
    //否则自行退出，在STOP状态下,不知道该线程是否已经退出了
    mStatus = STOP;
    //pthread_mutex_unlock(&mScanStatusMutex);

    if (mScanCache) {
        mScanCache->Finished();
    }

    // scan exit, also exit the check thread
//    pthread_cancel(mCheckPayloadThread);
   //
}

void MultiVirusEngine::SetConfig(const ScanConfig &config) {
    *mConfig = config;
}

void MultiVirusEngine::SetWhiteList(std::list<pair<std::string,int> >& white) {
    mWhiteList.clear();
    mWhiteList = white;
}

void MultiVirusEngine::SetScanFileType(const std::list<std::string>& filetype) {
    mScanFileType.clear();
    mScanFileType = filetype;
}

int MultiVirusEngine::Status() {
    return mStatus;
}

void* MultiVirusEngine::CPUPayloadThread(void *args) {
    MultiVirusEngine *self = (MultiVirusEngine*)args;
    self->CheckCPUPayload();
    return NULL;
}

void MultiVirusEngine::CheckCPUPayload() {
    while(mStatus != STOP) {
//        float percent = get_pcpu(getpid());

//        FILE *file = fopen("cpu.txt", "a+");
//        if (file) {
//            fprintf(file, "CPU Payload %.2f\n", percent);
//            fclose(file);
//        }
        sleep(3);
    }
}

void MultiVirusEngine::AdjustCPUPayload(int updonw) {
    for (int i=0; i<mScanThreadCount; ++i) {
        mScanThreadVari[i]->AdjustPayload(updonw);
    }
}

void* MultiVirusEngine::ScanReportThread(void *params) {
    MultiVirusEngine *engine = (MultiVirusEngine*)params;

    LOG_GJCZ("start the ScanReportThread thread");
#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "ScanReport");
    if(pthread_setname_np(pthread_self(), threadName)< 0)
    {
        LOG_GJCZ("pthread_setname_np set name:%s failue", threadName);
    }
#endif

    engine->ScanReportFunc();
    return NULL;
}

void MultiVirusEngine::ScanReportFunc() {
    while(mStatus != STOP) {
        if (mStatus == PAUSE) {
            mReportLocker->Wait();
        }

        sleep(1);
        //printf("ScanReportFunc running\n");
        if (mObserver && mScanStopStatue!=mScanExitFlag && mStatus != STOP) {
            mObserver->OnVirusEvent(EngineObserver::REPORT, EngineObserver::SCAN_SUMMARY, &mReportSummary);
        }
    }
}
