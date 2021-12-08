#ifndef COUNTFILEVARI_H
#define COUNTFILEVARI_H

using namespace std;

#include <pthread.h>
#include <string>
#include <list>
#include <utility>

class EngineObserver;
class CondLocker;
class CountFileVari {
public:
    /**
     * @brief NEW
     * @param observer
     * @param deal
     * @param whitelist
     * @param scantype define in scanconfig.h SCAN_TYPE
     * @param filetype if scantype set SPECIAL_FILE, this must be set
     * @return
     */
    static CountFileVari* NEW(EngineObserver *observer,
                              bool deal,
                              std::list<pair<std::string,int> > *whitelist,
                              int scantype,
                              std::list<std::string> *filetype,
                              bool intelligent);
    ~CountFileVari();

    int Start(string path, int maxDepth);
    int Start(const list<pair<string, int> >& paths);
    void Stop();
    void Pause();
    void Continue();

    int Count(string path, int maxDepth);

private:
    CountFileVari(EngineObserver *observer, bool deal, std::list<pair<std::string,int> > *whitelist, bool intelligent);
    int Construct(int scantype, std::list<std::string> *filetype);

    typedef int (CountFileVari::*CountFunc)(const char*, const char*);
    long CountFile(const char *dirName, int scanDepth, int MAX_DEPTH, CountFunc pFunc);

    int DealFile(const char *dirName, const char *fileName);

    inline bool AvailableFile(const char *filename);
    inline bool MatchWhite(const char *fileName);

    inline bool MatchRule(const char *filename, std::list<pair<std::string,int> >& rules);
    inline bool MatchRule(const char *filename, std::list<std::string>& rules);
    inline bool MatchRule(const char *filename, const char** rules, int len);

    static void* ThreadWork(void *args);

private:
    pthread_t       mThread;

    CondLocker      *mPauseCond;

    bool    mDealFile;

    enum {
        COUNT_STATUS_INIT = 0,
        COUNT_STATUS_SCANING,
        COUNT_STATUS_PAUSE,
        COUNT_STATUS_STOP
    }mStatus;

    EngineObserver  *mObserver;
    std::list<pair<std::string,int> > *mWhiteList;

    std::list<pair<std::string, int> >  mTempPath;

    /**
     * scan type, define in ScanConfig.h
     * enum SCAN_TYPE
     */
    int mScanType;
    std::list<std::string>* mSpecialFileType;

    /** 是否是智能模式，智能模式下，对 usr opt 下的压缩文件不进行扫描 */
    bool mIntelligentMode;
};

#endif // COUNTFILEVARI_H
