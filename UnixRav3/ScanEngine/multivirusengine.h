#ifndef MULTIVIRUSENGINE_H
#define MULTIVIRUSENGINE_H

#include <queue>
#include <list>
#include <vector>
#include <string>
#include <string.h>
#include <sys/time.h>

#include "engineobserver.h"
#include "cloudengine.h"

using namespace std;

class VirusReportData {
public:
    enum {
        SCAN_LOG,
        VIRUS_LOG,
        PROGRESS,
        FINISHED,
    }mType;

    /**
     * @important The VirusStatus enum
     * be careful, this define must match the enum TEFC
     * TEFC define in unix engine 2013
     */
    enum VirusStatus{
        STATUS_DEFAULT,
        STATUS_IGNORE,
        STATUS_CLEAN,
        STATUS_REMOVE,
        STATUS_ABORT,
    }mStatus;
    unsigned int mProgress;
    string      mPath;
    string      mVirus;
    long        mStartTime;
    long        mVirusCount;
    long        mScanCount;
    long        mTotalCount;
    long        mVirusCleanCount;
    long        mVirusDeleteCount;
    long        mVirusIgnoreCount;
    long        mDuration;

    // for out data
    int         mSavedLogId;

public:
    VirusReportData() {
        mProgress = 0;
        mVirusCount = 0;
        mScanCount = 0;
        mDuration = 0;
        mVirusCleanCount = 0;
        mVirusDeleteCount = 0;
        mVirusIgnoreCount = 0;
        mSavedLogId = 0;
        mTotalCount = 0;
    }

public:
    void Scaning(const string& path) {
        mType = SCAN_LOG;
        mStatus = STATUS_DEFAULT;
        mPath = path;
    }

    void Virus(const string& path, const string& virus, int result) {
        mType = VIRUS_LOG;
        mPath = path;
        mVirus = virus;
        mStatus = (VirusStatus)result;
    }

    void Progress(int progress) {
        mType = PROGRESS;
        mProgress = progress;
    }

    void Started() {
        mProgress = 0;
        mVirusCount = 0;
        mScanCount = 0;
        mDuration = 0;
        mVirusCleanCount = 0;
        mVirusDeleteCount = 0;
        mVirusIgnoreCount = 0;

        struct timeval tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);
        mStartTime = tv.tv_sec;
    }

    void Finished() {
        mType = FINISHED;

        struct timeval tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);
        mDuration = tv.tv_sec - mStartTime;
    }
};

class ReportSummary {
public:
    const char *mFile;
    const unsigned int *mCount;
    const unsigned int *mVirus;
    const int        *mTag;

    ReportSummary(const char *file, const unsigned int *count, const unsigned int *virus, const int *tag)
        : mFile(file), mCount(count), mVirus(virus), mTag(tag) {

    }
};

class ScanEngine;
class ScanConfig;
class ScanThreadVari;
class CountFileVari;
class ScanCache;
class CondLocker;
class MultiVirusEngine : public EngineObserver
{
public:
    static MultiVirusEngine* NEW(const string& rmd, const string& libpath, const string& cachepath);
    virtual ~MultiVirusEngine();

private:
    MultiVirusEngine();
    int Construct(const string& rmd, const string& libpath, const string& cachepath);

public:
    int Start(const std::string& path, int maxDepth, int threadCount);
    int Start(const std::list<pair<std::string,int> >& path, int threadCount, bool intelligent);
    int Start(const std::list<pair<std::string,int> >& path);

    void Pause();
    void Continue();
    void Stop();
    void Release();
    int Status();

    void SetConfig(const ScanConfig& config);
    void SetWhiteList(std::list<pair<std::string,int> >& white);
    void SetScanFileType(const std::list<std::string>& filetype);

    void SetObserver(EngineObserver *observer) {
        mObserver = observer;
    }

    vector<ReportSummary>* ScanSummary() {
        return &mReportSummary;
    }

    /** set the scan dynamic status, include scan thread count , cpu load percent */
    int SetScanDynamicStatus(int thread, int cpuload);

private:
    bool DealFile(const char *dirName, const char *fileName);

    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

    void NotifyScanFinished();

private:
    static void* CPUPayloadThread(void *);
    void CheckCPUPayload();

    void AdjustCPUPayload(int updonw);

    static void* ScanReportThread(void*);
    void ScanReportFunc();

private:   
//    ScanThreadVari **mScanThreadVari;

    vector<ScanThreadVari*> mScanThreadVari;
    CountFileVari   *mScanFileThread;
    CountFileVari   *mCountFileThread;
    ScanCache       *mScanCache;
    CloudEngine     *mCloudEngine;

//    pthread_t       mCheckPayloadThread;

    pthread_t       mScanReportThread;
    CondLocker      *mReportLocker;
    /** 定义暂停信号量，防止在扫描暂停的时候CountFileVari线程还在工作 */
    CondLocker      *mPauseLocker;


    int    mScanThreadCount;
    int    mMaxDepth;

    string mRMDPath;
    string mLibPath;
    string mCachePath;

    ScanConfig  *mConfig;
    std::list<pair<std::string,int> > mWhiteList;
    std::list<std::string> mScanFileType;

    int mScanCount;
    int mVirusCount;
    int mAllCount;

    /**
     * @brief scan exit statue
     * 0x1 count file thread exit
     * 0x2 scan thread exit
     * 0x4++ virus judge thread
     * when the mScanStopStatue == mScanExitFlag, all thread exit
     * the scan finished
     */
    int mScanStopStatue;
    int mScanExitFlag;

    enum {
        INIT,
        START,
        PAUSE,
        STOP,
    }mStatus;

    EngineObserver  *mObserver;

private:
    VirusReportData mReportData;
    vector<ReportSummary>   mReportSummary;
    pthread_mutex_t mReportMutex;
    pthread_mutex_t mScanExitMutex;
    pthread_mutex_t mJudgeCacheMutex;
    pthread_mutex_t mScanStatusMutex;
    pthread_mutex_t mThreadCountMutex;
};

#endif // MULTIVIRUSENGINE_H
