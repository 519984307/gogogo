#ifndef DEFENENGINE_H
#define DEFENENGINE_H

#include <string>
#include <map>
#include <list>
#include <set>
#include <pthread.h>

#include "observer.h"
#include "kernelapplication.h"

using namespace std;

class DefenModProxy;
class DefenPlugins;
class StorageBridge;
class ServiceBinder;
class AntiVirusSetting;
class AppConfig;
class DefenDataContainer;

class DefenEngine : public DefenObserver, public KernelApplication {
public:
    static DefenEngine* NEW(ServiceBinder *binder, StorageBridge *storage, AppConfig *config);
    virtual ~DefenEngine();

    /**
     * @brief StartFileMonitor
     * @return 0 have matched ko, but not running, 1 running, < 0 error
     */
    int StartFileMonitor();
    int StopFileMonitor();

    int LoadFileMonitorSetting();

    list<string> &GetFolderMonitorList();


    DefenPlugins        *mFileJudge;
private:
    DefenEngine(ServiceBinder *binder, StorageBridge *storage, AppConfig *config);
    int Construct();

    inline string GetScanParams(const string& file);

	static void * ThreadMainProxy(void *);
	void   ThreadMain();
	int    JudgeChange(const char * filepath, int pathlen, int pid);
	int    JudgeAccess(const char * filepath, int pathlen, int pid);

	void   InitDefaultRule();
	int    MatchFilePath(const char * szPath, int nPathLen, const char * szRule, int nRuleLen, int nType);

	
protected:
    // from DefenObserver
    int onDefenEvent(DEFEN_TYPE event, const char *file, void *data=NULL);


private:
    DefenModProxy       *mDefenProxy;
    DefenDataContainer  *mDataContainer;


	// -------------------------------------------------------------
	// cache and rule.
	enum   enumJudgeResult
	{
		JUDGE_IGNORE = 0, 
		JUDGE_DENY, 
		JUDGE_SCAN, 
	};
	struct CacheFileInfo_t
	{
		std::string  path;
		unsigned int result:8;  // 0=Î´É¨Ãè, 1=É¨ÃèÖÐ, 2=°×, 9=ºÚ
		unsigned int count:24;
		int          pid;       // last
	};
	typedef std::list<struct CacheFileInfo_t*>       CacheFileList_t;
	typedef std::map<int, CacheFileList_t>           SecondCacheFileList_t;

    std::set<int>                                    m_rulew_pids;
    std::set<std::string>                            m_rulew_procs, m_ruleb_procs;
    std::map<std::string, int>                       m_rulew_files, m_ruleb_files;
    std::map<int, SecondCacheFileList_t>             m_caches;
    std::list<CacheFileInfo_t*>                      m_scans;
    pthread_mutex_t                                  m_lock;
	pthread_cond_t                                   m_event;
	pthread_t                                        m_thread;
	volatile long                                    m_run;
};

#endif // DEFENENGINE_H
