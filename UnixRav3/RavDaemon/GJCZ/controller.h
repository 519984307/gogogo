#ifndef CONTROLLER_H
#define CONTROLLER_H

using namespace std;

#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>

#include "circlelink.h"
#include "engineobserver.h"
#include "storagebridge.h"

class MultiVirusEngine;
class ScanEngine;
class VirusReportData;
class Storage;
class RavQuaFile;
class ScanProxy;
class ScanConfig;
class QuarantineLog;
class VirusInfoEntry;
class ReportSummary;
class CondLocker;

class Controller : public EngineObserver, public StorageBridge
{
    friend class ScanProxy;
public:
    static Controller *NEW(const string& installPath, const string& owner);
    virtual ~Controller();

    void SetApplication(ScanProxy *app) {
        mApplication = app;
    }

    virtual int QueryUserForResult(int/** Application::QUERY_EVENT **/ /*event*/,
                                   const string& /*file*/,
                                   const string& /*virus*/) { return 0; }

public:
    int  Scan(const char *path, int depth);
    int  Scan(const std::list<pair<std::string,int> >& path, const ScanConfig* config=NULL);
    string SimpleScan(const std::string& file);
    int  ScanFile(const std::string& path, EngineObserver *observer, const ScanConfig& config, ScanEngine **engine);
    void PauseScan();
    void StopScan();

    int RestoreQuarantine(QuarantineLog& log);
    int DeleteQuarantine(const QuarantineLog& log);

    vector<ReportSummary>* ScanSummary();

protected:
    Controller();
    virtual int Construct(const string& installPath, const string& owner);

    int Start();
    int Stop();

    virtual bool GetStatus() { return false; }

protected:
    // EngineObserver
    virtual int OnVirusEvent(EngineEvent event, EngineType type, void *data);

protected:
    int ProcessVirus(const string& path, const string& virus, std::string& quaname, const ScanConfig& config);
    int DealVirusWithConfig(const string& path, const string& virus, const ScanConfig& config, std::string& quaraname);
    int QuarantineVirus(const string& path, const string& virus, std::string& quaname);

protected:
    MultiVirusEngine    *mMultiEngine;
    ScanEngine          *mSingleEngine;
    RavQuaFile          *mQuaArea;

    ScanProxy         *mApplication;

    CircleLink<string>  *mScanShowedLog;
    std::list<VirusReportData> mVirusShowedLog;

    std::list<VirusInfoEntry> mVirusEntries;

    ScanConfig  *mScanConfig;
    ScanConfig  *mDefenScanConfig;

    /** 此变量用于保存有些文件的白名单，比如从隔离区恢复的文件，要将哈希放在此中 */
    std::map<unsigned int, bool>    mWhiteHash;

    int     mScanProgress;
    bool    mScanFinished;

protected:
    pthread_t   mAsyncScanThread;

    std::queue<ScanProxy*> mScanTaskPool;

    CondLocker  *mScanPoolCond;
    CondLocker  *mScanTaskCond;

    void ScanPoolWork();
    static void* ScanFunc(void *args);
};


#endif // CONTROLLER_H
