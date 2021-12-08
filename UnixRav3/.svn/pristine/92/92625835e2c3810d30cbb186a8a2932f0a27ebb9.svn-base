#include "defenengine.h"
#include "defenplugins.h"
#include "defenmodproxy.h"
#include "json/json.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "appconfig.h"
#include "communicatebundle.h"
#include "storagebridge.h"
#include "service.h"
#include "Entry/commonentry.h"
#include "Entry/whitelistentry.h"
#include "datacontainer.h"
#include "service.h"
#include "defenmod.ver"
extern "C" {
    #include "crypt/crc32.h"
}

#include <sys/types.h>
#include <dirent.h>
#include <assert.h>


#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

#ifndef Assert
#define Assert assert
#endif


void   calc_string_hash_ex(const unsigned char * str, int len, int hash[], int n)
{
	// php ?
	unsigned long  h = 0, g,   i = 0, mn = 0;
	const unsigned char * begin = str, *end, *mid;

	if(len <= 0) len = strlen((const char*)str); mn = len/n;
	end = begin+len; mid = begin+mn;

	while(begin < end)
	{
		h = (h << 4) + *begin++; 
		if((g = (h & 0xF0000000)))
		{ 
			h = h ^ (g >> 24); 
			h = h ^ g; 
		}

		if(begin == mid)
		{
			mid += mn;
			hash[i++] = h;
		}
	}

	if(end != mid) hash[n-1] = h;
}



DefenEngine* DefenEngine::NEW(ServiceBinder *binder, StorageBridge *storage, AppConfig *config) {
    DefenEngine *self = new DefenEngine(binder, storage, config);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

DefenEngine::DefenEngine(ServiceBinder *binder, StorageBridge *storage, AppConfig *config)
                        : KernelApplication (binder, storage, config) {
    mDefenProxy = NULL;
    mFileJudge = NULL;
	m_run = 0;
	pthread_mutex_init(&m_lock,  0);
	pthread_cond_init(&m_event, 0);
}

DefenEngine::~DefenEngine() {
    SECURITY_FREE(mDefenProxy);
    SECURITY_FREE(mFileJudge);
	pthread_cond_destroy(&m_event);
	pthread_mutex_destroy(&m_lock);
}

int DefenEngine::Construct() {
    KernelApplication::Construct();

    mDataContainer = new DefenDataContainer;
    mFileJudge = DefenPlugins::NEW(mDataContainer);

    LoadFileMonitorSetting();

    return 0;
}


list<string> &DefenEngine::GetFolderMonitorList()
{
    return mDataContainer->mFolderMonitorList;
}


int DefenEngine::LoadFileMonitorSetting() {
    mStorage->GetAntiVirusConfig(*mDataContainer->mFileMonitorSetting, AntiVirusSetting::FILE_MONITOR);
    mFileJudge->LoadConfig(mBinder->GetClientUID());

    std::list<WhiteListEntry> datas;
    if (mStorage->GetProcessBlackList(datas) > 0) {
        std::list<WhiteListEntry>::iterator iter;
        for (iter=datas.begin();iter!=datas.end();++iter) {
            mDataContainer->mProcMonitorList.push_back(iter->mPath);
        }
    }

    datas.clear();
    if (mStorage->GetFolderMonitorList(datas) > 0) {
        std::list<WhiteListEntry>::iterator iter;
        for (iter=datas.begin();iter!=datas.end();++iter) {
            mDataContainer->mFolderMonitorList.push_back( string(iter->mPath+string("/*")) );
        }
    }


    list<string> fl = GetFolderMonitorList();
    for(list<string>::iterator it = fl.begin(); it != fl.end(); it++)
    {
        LOG_ZDFY("LoadFileMonitorSetting folder Monitor List: %s", it->c_str());
    }

    return 0;
}

/********************************************************************
 * old filemon handler.
int DefenEngine::onDefenEvent(DEFEN_TYPE event, const char *file, void *data) {
    if (NULL == mFileJudge) {
        return 0;
    }

    if (event == DEFEN_PROC_START) {
        // filter the scaned process in cache
        unsigned int hash = rs_crc32((unsigned char*)file, strlen(file));
        map<unsigned int, bool>::iterator iter;
        if (mScanCache.find(hash) != mScanCache.end()) {
            return 0;
        }
    }

    string out;
    //LOG_ZDFY("file %s is changed, will be matched", file);
    DefenPlugins::JudgeResult resp = mFileJudge->Judge(event, string(file), data, out);
    switch (resp) {
    case DefenPlugins::DEFEN_PROC_DENY: {
        return 1;
    }
    case DefenPlugins::DEFEN_FILE_SCAN:
    case DefenPlugins::DEFEN_PROC_EXTENS: {
        LOG_ZDFY("get %s changed, will be scan by GJCZ and type is %d", file, resp);
        // match the rule
        CommandBundle command;
        command.mCommandID = CommandBundle::REMOTE_COMMAND;
        if (resp == DefenPlugins::DEFEN_FILE_SCAN) {
            command.mIntegerValue = CommandBundle::DEFEN_FILE_CHANGED;
            command.mStringValue = GetScanParams(file);
        }
        else if (resp == DefenPlugins::DEFEN_PROC_EXTENS) {
            command.mIntegerValue = CommandBundle::DEFEN_PROC_STARTED;
            command.mStringValue = GetScanParams(out);
        }


        int err = mBinder->NotifyData(FOLDER_RJJH_ZDFY, RJJH_NOTIFY_SOCKET, command);
        if (err>0 && resp == DefenPlugins::DEFEN_PROC_EXTENS) {
            // proc started, detect the virus            
            return 1;
        }
        else if (err == 0 && event == DEFEN_PROC_START) {
            unsigned int hash = rs_crc32((unsigned char*)file, strlen(file));
            mScanCache.insert(make_pair(hash, true));
        }
        LOG_ZDFY("defen scan ret value %d", err);
        break;
    }
    default:{
        break;
    }
    }

    return 0;
}*/

inline string DefenEngine::GetScanParams(const string& file) {
    // file need scan
    Json::Value json;
    json["file"] = Json::Value(file);

    Json::FastWriter writer;
    return writer.write(json);
}

int DefenEngine::StartFileMonitor() {
    printf("StartFileMonitor1\n");

    if (!mDefenProxy) {
        mDefenProxy = new DefenModProxy;
        mDefenProxy->SetObserver(this);
    }

	m_run = 1;
	InitDefaultRule();
	pthread_create(&m_thread, 0, &DefenEngine::ThreadMainProxy, this);

    if (mDefenProxy && mDefenProxy->Status()==0) {
        //printf("StartFileMonitor3\n");
        list<string> fl = GetFolderMonitorList();
        for(list<string>::iterator it = fl.begin(); it != fl.end(); it++)
        {
            LOG_ZDFY("folder Monitor List: %s", it->c_str());
        }

        LOG_ZDFY("StartFileMonitor");
        return mDefenProxy->Start()?-1:1;
    }
    else {
        return LoadFileMonitorSetting();
    }

    printf("StartFileMonitor4\n");
}

int DefenEngine::StopFileMonitor() {
    LOG_ZDFY("StopFileMonitor");

	m_run = 0;
	pthread_mutex_lock(&m_lock);
	pthread_cond_broadcast(&m_event);
	pthread_mutex_unlock(&m_lock);
	pthread_join(m_thread, 0);

    if (mDefenProxy) {
        mDefenProxy->Stop();
    }

	for(std::map<int, SecondCacheFileList_t>::iterator it1 = m_caches.begin(); it1 != m_caches.end(); it1++)
	{
		for(SecondCacheFileList_t::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
		{
			for(CacheFileList_t::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++) delete *it3;
		}
	}
    return 0;
}


void * DefenEngine::ThreadMainProxy(void * obj)
{
	DefenEngine * pObj = (DefenEngine*)(obj);
	pObj->ThreadMain();
	return 0;
}

void DefenEngine::ThreadMain()
{
	int    result = 0, size = 0, count = 0;
	struct CacheFileInfo_t * finfo = 0;

	while(m_run)
	{
		pthread_mutex_lock(&m_lock);
		if(finfo)
		{
			finfo->result = result;
			finfo = 0;
		}

		if(m_scans.empty() && m_run)
		{
			pthread_cond_wait(&m_event, &m_lock);
		}

		if(!m_scans.empty() && m_run)
		{
			size  = m_caches.size(); count = m_scans.size();
			finfo = m_scans.front();
			finfo->result = 1;
			m_scans.pop_front();
		}
		pthread_mutex_unlock(&m_lock);

		if(!finfo) continue;
		//LOG_ZDFY(">> scan file ++++>%d:%d:%d:%s", size, count, finfo->pid, finfo->path.c_str());

		CommandBundle command;
		command.mCommandID    = CommandBundle::REMOTE_COMMAND;
		command.mIntegerValue = CommandBundle::DEFEN_FILE_CHANGED;
		command.mStringValue  = GetScanParams(finfo->path);
        int ret = mBinder->NotifyData(FOLDER_RJJH_ZDFY, RJJH_NOTIFY_SOCKET, command);
		result = ret > 0 ? 9:(ret == 0 ? 2:0);
		//LOG_ZDFY(">> scan file ---->%d,%d,%s", m_run, ret, command.mStringValue.c_str());
	}
}

int  DefenEngine::onDefenEvent(DEFEN_TYPE event, const char *file, void *data) {
    if(NULL == mFileJudge)
    {
        return 0;
    }

	int res, length = strlen(file);

	// deleted file ignore ???
	if(strstr(file, " (deleted)")) return JUDGE_IGNORE;

	if(event == DEFEN_FILE_WRITE)
	{
		pthread_mutex_lock(&m_lock);
		res = JudgeChange(file, length, (int)(long)data);
		if(res == JUDGE_SCAN) pthread_cond_broadcast(&m_event);
		pthread_mutex_unlock(&m_lock);
	}
	if(event == DEFEN_FILE_ACCESS)
	{
		pthread_mutex_lock(&m_lock);
		res = JudgeAccess(file, length, (int)(long)data);
		pthread_mutex_unlock(&m_lock);
		return res;
	}

	return res;
}

int  DefenEngine::JudgeChange(const char * filepath, int pathlen, int pid)
{
	#define MAX_CACHE_NUMBER 1024*1024
	// result: 0 = ignore, 1 = deny, 2 = scan.
	int    hash[2] = {0};
	struct CacheFileInfo_t * finfo = 0;

	// 1 pid.
	if(pid)
	{
		std::set<int>::iterator it = m_rulew_pids.find(pid);
		if(it != m_rulew_pids.end()) return JUDGE_IGNORE;
	}

	// 2 file_path, file_name.
	for(std::map<std::string, int>::iterator it = m_rulew_files.begin(); it != m_rulew_files.end(); it++)
	{
		if(MatchFilePath(filepath, pathlen, it->first.c_str(), it->first.length(), it->second)) return JUDGE_IGNORE;
	}

	// 3, proc_path, proc_name.
	if(pid)
	{
		char tmp[32] = {0}, proc_path[256] = {0};
		snprintf(tmp, sizeof(tmp)-1, "/proc/%d/exe", pid);
		readlink(tmp, proc_path, sizeof(proc_path)-1);
		if(*proc_path)
		{
			std::set<std::string>::iterator it = m_rulew_procs.find(proc_path);
			if(it != m_rulew_procs.end()) return JUDGE_IGNORE;

			char * p = strrchr(proc_path, '/');
			it = m_rulew_procs.find(p+1);
			if(it != m_rulew_procs.end()) return JUDGE_IGNORE;
		}
	}

	// 3 file cache.
	calc_string_hash_ex((const unsigned char*)filepath, pathlen, hash, sizeof(hash)/sizeof(hash[0]));
	std::map<int, SecondCacheFileList_t>::iterator it1 = m_caches.find(hash[1]);
	if(it1 != m_caches.end())
	{
		SecondCacheFileList_t::iterator it2 = it1->second.find(hash[0]);
		if(it2 != it1->second.end())
		{
			for(CacheFileList_t::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++)
			{
				if(strcmp(filepath, (*it3)->path.c_str()) == 0 && (*it3)->result != 9)
				{
					(*it3)->count++; finfo = *it3;
					if((*it3)->result == 0) return JUDGE_IGNORE;
					if(pid == (*it3)->pid)  return JUDGE_IGNORE;
					if((*it3)->result == 2 && (*it3)->count > 10) return JUDGE_IGNORE;
					(*it3)->result = 0; (*it3)->pid = pid;
				}
			}
		}
	}

	if(finfo == 0)
	{
		finfo = new CacheFileInfo_t;
		finfo->path = filepath;
		finfo->result = 0; finfo->count = 1; finfo->pid  = pid;
		m_caches[hash[1]][hash[0]].push_back(finfo);
		if(m_caches.size() > MAX_CACHE_NUMBER)
		{
			for(std::map<int, SecondCacheFileList_t>::iterator it1 = m_caches.begin(); it1 != m_caches.end(); it1++)
			{
				if(it1->second.size() == 1 && it1->second.begin()->second.size() == 1)
				{
					CacheFileList_t::iterator it3 = it1->second.begin()->second.begin();
					if((*it3)->result == 2 && (*it3)->count == 1)
					{
						delete *it3;
						m_caches.erase(it1);
						break;
					}
				}
			}
		}
	}

	// 4 push scan file.
	m_scans.push_back(finfo);
	return JUDGE_SCAN;
}

int  DefenEngine::JudgeAccess(const char * filepath, int pathlen, int pid)
{
	for(std::map<std::string, int>::iterator it = m_ruleb_files.begin(); it != m_ruleb_files.end(); it++)
	{
		if(MatchFilePath(filepath, pathlen, it->first.c_str(), it->first.length(), it->second)) return JUDGE_DENY;
	}

	if(pid && !m_ruleb_procs.empty())
	{
		char tmp[32] = {0}, proc_path[256] = {0};
		snprintf(tmp, sizeof(tmp)-1, "/proc/%d/exe", pid);
		readlink(tmp, proc_path, sizeof(proc_path)-1);
		if(*proc_path)
		{
			std::set<std::string>::iterator it = m_ruleb_procs.find(proc_path);
			if(it != m_ruleb_procs.end()) return JUDGE_DENY;

			char * p = strrchr(proc_path, '/');
			it = m_ruleb_procs.find(p+1);
			if(it != m_ruleb_procs.end()) return JUDGE_DENY;
		}
	}

	return JUDGE_IGNORE;
}


void DefenEngine::InitDefaultRule()
{
	// 实际上应该把 RAV的进程PID放到 m_rule_pids中. 
	// 暂时使用路径吧. 
	std::string  strRsBinPath;
	DIR * dir;
	struct dirent * ent;

	Assert(mAppCfg);
	strRsBinPath = mAppCfg->GetInstallPath();
	if(strRsBinPath.empty()) return;

	strRsBinPath += "/bin";
	dir = opendir(strRsBinPath.c_str());
	if(!dir) return;

	while((ent = readdir(dir)))
	{
		FILE * pf = 0;
		char   buf[256] = {0};
		std::string strTmp;

		if((ent->d_name[0] == '.' && ent->d_name[1] == 0) || (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0)) continue;
		strTmp = "file " + strRsBinPath + "/"; strTmp += ent->d_name;
		pf = popen(strTmp.c_str(), "r");
		if(!pf) continue;

		fgets(buf, sizeof(buf)-1, pf);
		if(strstr(buf, "ELF ")) 
		{
			strTmp = strRsBinPath + "/"; strTmp += ent->d_name;
			m_rulew_procs.insert(strTmp);
		}
		fclose(pf);
	}
}

int  DefenEngine::MatchFilePath(const char * szPath, int nPathLen, const char * szRule, int nRuleLen, int nType)
{
	switch(nType)
	{
	case 0:  // 全路径
		{
			if(!strcmp(szPath, szRule)) return 1;
		}
		break;
	case 1:  // 文件名
		{
			const char  * szName = strrchr(szPath, '/');
			if(!strcmp(szName+1, szRule)) return 1;
		}
		break;
	case 2:  // 文件路径
		{
			const char  * szName = strrchr(szPath, '/');
			int nDirLen = szName - szPath;
			if(nDirLen == nRuleLen && !memcmp(szPath, szRule, nDirLen)) return 1;
		}
		break;
	case 3:  // 前匹配
		{
			if(nPathLen >= nRuleLen && !memcmp(szPath, szRule, nRuleLen)) return 1;
		}
		break;
	case 4:  // 尾匹配
		{
			if(nPathLen >= nRuleLen && !memcmp(szPath+nPathLen-nRuleLen, szRule, nRuleLen)) return 1;
		}
		break;
	default: // 任意包含
		{
			if(strstr(szPath, szRule)) return 1;
		}
	}

	return 0;
}
