#include "controller.h"
#include "scanproxy.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "simplejudger.h"

#include "Entry/whitelistentry.h"
#include "Entry/logevententry.h"
#include "Entry/quarantinelog.h"
#include "Entry/virusinfoentry.h"
#include "Entry/commonentry.h"
#include "multivirusengine.h"
#include "scanengine.h"
#include "scanconfig.h"
#include "ravquafile.h"
#include "Common/updateconfig.h"
#include "commonlocker.h"

#include "storage.h"
#include "cinifile.h"

#include <fstream>
#include <sys/types.h>
#include <dirent.h>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

Controller* Controller::NEW(const string &installPath, const string& owner) {
    Controller *self = new Controller;
    if (self) {
        int err = self->Construct(installPath, owner);
        if (err) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

Controller::Controller()
{
    mMultiEngine = NULL;
    mScanShowedLog = NULL;
    mApplication = NULL;
    mScanConfig = NULL;
    mDefenScanConfig = NULL;
    mSingleEngine = NULL;
    mScanProgress = 0;
    mAsyncScanThread = 0;
    mScanPoolCond = NULL;
    mScanTaskCond = NULL;
}

Controller::~Controller() {
    SECURITY_FREE(mScanConfig);
    SECURITY_FREE(mMultiEngine);
    SECURITY_FREE(mSingleEngine);
    SECURITY_FREE(mScanShowedLog);
    SECURITY_FREE(mQuaArea);
    SECURITY_FREE(mDefenScanConfig);
    SECURITY_FREE(mScanPoolCond);
    SECURITY_FREE(mScanTaskCond);
}

int Controller::Construct(const string &installPath, const string& owner) {
    SECURITY_CHECK(StorageBridge::Construct(installPath, owner), 0, -1);

    string cache = Utils::Path::JoinPath(mInstallDir.c_str(), owner.c_str(), SCAN_CACHE_PATH, NULL);
    Utils::Path::CheckPath(cache, 0777);

    mMultiEngine = MultiVirusEngine::NEW(
                      Utils::Path::JoinPath(mInstallDir.c_str(), owner.c_str(), VIRUS_DB_FILE, NULL),
                      Utils::Path::JoinPath(mInstallDir.c_str(), SCAN_ENGINE_LIB_PATH, NULL),
                      cache
                      );
    ASSERT_ZERO(mMultiEngine, -1);

    std::string quapath = Utils::Path::JoinPath(installPath.c_str(), QUARANTINE_PATH, NULL);
    Utils::Path::CheckPath(quapath, 0777);

    mQuaArea = RavQuaFile::NEW(quapath, 500, 20, 1);
    ASSERT_ZERO(mQuaArea, -1);

    mScanShowedLog = CircleLink<string>::NEW(10);

    mScanConfig = new ScanConfig;
    mDefenScanConfig = new ScanConfig;

    mScanPoolCond = CondLocker::NEW();
    mScanTaskCond = CondLocker::NEW();

    return 0;
}

int Controller::Start() {
    if (mAsyncScanThread == 0) {
        SECURITY_CHECK(pthread_create(&mAsyncScanThread, NULL, ScanFunc, this), 0, -1);
    }
    return -1;
}

int Controller::Stop() {
    if (mAsyncScanThread > 0) {
        pthread_cancel(mAsyncScanThread);
        pthread_join(mAsyncScanThread, NULL);
        mAsyncScanThread = 0;
    }
    return 0;
}

int Controller::Scan(const char *path, int depth) {
    std::list<pair<std::string,int> > paths;
    paths.push_back(make_pair(path, depth));
    return Scan(paths);
}

int Controller::Scan(const std::list<pair<std::string,int> >& path, const ScanConfig* config) {
    if (NULL == config) {
        GetScanConfig(*mScanConfig);
    }
    else {
        *mScanConfig = *config;
    }

    mVirusEntries.clear();
    mVirusShowedLog.clear();
    mScanFinished = false;

    mMultiEngine->SetConfig(*mScanConfig);
    mMultiEngine->SetObserver(this);

    list<WhiteListEntry> whitelist;
    if (GetWhiteList(whitelist) >= 0) {
        list<pair<string,int> > wstrlist;
        list<WhiteListEntry>::iterator iter;
        for (iter=whitelist.begin();iter!=whitelist.end();++iter) {
            if (iter->mPathType == WhiteListEntry::FOLDER_RECURSION
                    || iter->mPathType == WhiteListEntry::FOLDER_ONLY
                    || iter->mPathType == WhiteListEntry::FOLDER_CHILDERN) {
                wstrlist.push_back(make_pair((*iter).mPath + "/*", iter->mPathType));
                LOG_GJCZ("white list %s/*\n", iter->mPath.c_str());
            }
            else if (iter->mPathType == WhiteListEntry::FILE_PATH) {
                wstrlist.push_back(make_pair((*iter).mPath, iter->mPathType));
                LOG_GJCZ("white list %s/*\n", iter->mPath.c_str());
            }
            else if (iter->mPathType == WhiteListEntry::FILE_EXTEND_TYPE) {
                if (Utils::String::StartsWith(iter->mPath, "*")) {
                    wstrlist.push_back(make_pair((*iter).mPath, iter->mPathType));
                    LOG_GJCZ("white ext %s\n", iter->mPath.c_str());
                }
                else {
                    wstrlist.push_back(make_pair(string("*.") + (*iter).mPath, iter->mPathType));
                    LOG_GJCZ("white ext *.%s\n", iter->mPath.c_str());
                }
            }

        }
        mMultiEngine->SetWhiteList(wstrlist);
    }

    //
    if (mScanConfig->mScanFileType == SPECIAL_FILE_TYPE) {
        list<WhiteListEntry> filetype;
        int count = GetScanFileTypeList(filetype);
        if (count > 0) {
            list<string> typelist;
            list<WhiteListEntry>::iterator iter;
            for (iter=filetype.begin(); iter!=filetype.end(); ++iter) {
                if (Utils::String::StartsWith(iter->mPath, "*")) {
                    typelist.push_back((*iter).mPath);
                    LOG_GJCZ("only scan %s\n", iter->mPath.c_str());
                }
                else if (Utils::String::StartsWith(iter->mPath, ".")) {
                    typelist.push_back(string("*") + iter->mPath);
                }
                else {
                    typelist.push_back("*." + (*iter).mPath);
                    LOG_GJCZ("only scan *.%s\n", iter->mPath.c_str());
                }
            }
            mMultiEngine->SetScanFileType(typelist);
        }
    }
    LOG_GJCZ_DEBUG("start the multi scan...");
    return mMultiEngine->Start(path, mScanConfig->mThreadCount, mScanConfig->mIntelligentMode);
}

int Controller::ScanFile(const string &path, EngineObserver *observer, const ScanConfig& config, ScanEngine **engine) {
    unsigned int hash = Utils::Hash::StrCRC32(path);
    map<unsigned int, bool>::iterator iter = mWhiteHash.find(hash);
    if (iter != mWhiteHash.end()) {
        return 0;
    }

    if (!*engine) {
        *engine = ScanEngine::NEW(observer,
                                  Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), VIRUS_DB_FILE, NULL),
                                  Utils::Path::JoinPath(mInstallDir.c_str(), SCAN_ENGINE_LIB_PATH, NULL));
    }
    return (*engine)->ScanOneFile(path.c_str());
}

string Controller::SimpleScan(const std::string& file) {
    SimpleJudger judger;

    ScanConfig config;
    config.mVirusAction = IGNORE_DEAL;
    config.mBackupFailedAction = BACKUP_FAILED_IGNORE;
    config.mCompressSize = 0;

    if (ScanFile(file, &judger, config, &mSingleEngine) == 0) {
        return judger.mVirusName;
    }
    return "";
}

void Controller::PauseScan() {
    mMultiEngine->Pause();

}

void Controller::StopScan() {
    printf("call engine stop\n");
    mMultiEngine->Stop();
}

vector<ReportSummary>* Controller::ScanSummary() {
    return mMultiEngine->ScanSummary();
}

int Controller::OnVirusEvent(EngineEvent event, EngineType type, void *data) {
    if (ALL_FINISHED == type) {
        LOG_GJCZ("scan finished, all thread exit");

        VirusReportData *scan = (VirusReportData*)data;
        if (scan->mType == VirusReportData::FINISHED) {
            mScanFinished = true;
            try {
                LogEventEntry entry;
                entry.mLogType = LogEventEntry::SCAN_LOG;
                entry.mLogExtend = mApplication->ScanType();
                entry.mStatus = LogEventEntry::FINISHED;
                entry.mScanCount = scan->mScanCount;
                entry.mVirusCount = scan->mVirusCount;
                entry.mKilledCount = scan->mVirusCleanCount;
                entry.mDeletedCount = scan->mVirusDeleteCount;
                entry.mIgnoreCount = scan->mVirusIgnoreCount;
                entry.mStartTime = scan->mStartTime;
                entry.mEndTime = scan->mStartTime+scan->mDuration;
                entry.mTotalCount = scan->mTotalCount;

                if (LogEventEntry::GetScanType(mApplication->ScanType()) == LogEventEntry::ACTIVE_DEFENCE) {
                    // active defen donot save scan log
                    scan->mSavedLogId = -1;
                    if (mVirusEntries.size() > 0) {
                        scan->mVirus = mVirusEntries.front().Externlize();
                    }
                }
                else {
                    scan->mSavedLogId = this->AppendScanLog(entry);
                    if (scan->mSavedLogId > 0 && GetLogedVirus()) {
                        std::list<VirusInfoEntry>::iterator iter;
                        for (iter=mVirusEntries.begin(); iter!=mVirusEntries.end();++iter) {
                            iter->mScanLogID = scan->mSavedLogId;
                            // 由于此值没有用到，所有在此借用保存此病毒的扫描事件类型
                            iter->mVirusType = entry.mLogExtend;
                            AppendVirusLog(*iter);
                        }
                    }
                }
                mVirusEntries.clear();
            }
            catch(...) {

            }
        }

        if (mApplication) {
            mApplication->RefreshScanStatus(mApplication->ScanType(), Application::SCAN_ALL_FILE, scan->mScanCount);
        }
    }
    else if (VIRUS_JUDGE == event) {
        if (JUDGE_START == type) {
            VirusReportData *scan = (VirusReportData*)data;
            if (scan->mType == VirusReportData::SCAN_LOG) {
                mScanShowedLog->Append(scan->mPath);
            }
            else if (scan->mType == VirusReportData::PROGRESS) {
                if (scan->mProgress != mScanProgress) {
                    mScanProgress = scan->mProgress;
                }
            }
        }
        else if (JUDGE_DANGE == type) {
            VirusReportData *scan = (VirusReportData*)data;
            LOG_GJCZ_DEBUG("Controller JUDGE_DANGE file:%s", scan->mPath.c_str());
            if (scan->mType == VirusReportData::VIRUS_LOG) {
                std::string quaraname;
                scan->mStatus = (enum VirusReportData::VirusStatus)ProcessVirus(scan->mPath, scan->mVirus, quaraname, *mScanConfig);
                mVirusShowedLog.push_back(*scan);

                try {
                    VirusInfoEntry entry;
                    entry.mVirusName = scan->mVirus;
                    entry.mVirusType = 0;
                    entry.mOriginPath = scan->mPath;
                    entry.mOriginName = Utils::Path::GetFileName(scan->mPath);
                    entry.mDealState = scan->mStatus;
                    entry.mTime = Utils::Date::GetTimeStamp();
                    entry.mQuarantine = quaraname;
                    entry.mHash = Utils::String::ToUpper(Utils::Hash::GetFileMD5(scan->mPath));
                    entry.mFileSize = Utils::OS::GetFileSize(scan->mPath);

                    if (mApplication) {
                        mApplication->RefreshVirusLogs(scan->mPath, scan->mVirus, scan->mStatus);
                    }

                    // put the virus info to list, when the scan finished, save it
                    mVirusEntries.push_back(entry);
                }
                catch(...) {

                }
                return scan->mStatus;
            }
        }
    }    
    else if (REPORT == event) {
        if (SCAN_SUMMARY == type && !mScanFinished) {
            if (mApplication) {
                mApplication->RefreshScanStatus(mApplication->ScanType(), Application::THREAD_STATUS, mScanProgress, (void*)ScanSummary());
            }
        }
    }

    return 0;
}

int Controller::DealVirusWithConfig(const string& path, const string& virus, const ScanConfig& config, std::string& backupName) {
    if (BACKUP_ORIGIN == config.mBackupVirusFile) {
        if (QuarantineVirus(path, virus, backupName)) {
            // quarantine failed
            switch (config.mBackupFailedAction) {
            case BACKUP_FAILED_QUERY:
                if (mApplication) {
                    int sel = mApplication->QueryUserForResult(Application::QUERY_QUARANTINE_FAILED, path, virus);
                    if (NOTIFY_CONFIRM_SELECT == sel) {
                        return VirusReportData::STATUS_REMOVE;
                    }
                    else if (NOTIFY_IGNORE_SELECT == sel) {
                        return VirusReportData::STATUS_IGNORE;
                    }
                    return VirusReportData::STATUS_IGNORE;
                }
                break;
            case BACKUP_FAILED_DELETE:
                return VirusReportData::STATUS_REMOVE;
            case BACKUP_FAILED_IGNORE:
                break;
            default:
                break;
            }
        }
    }
    return VirusReportData::STATUS_CLEAN;
}

int Controller::ProcessVirus(const string& path, const string& virus, std::string& outBackupName, const ScanConfig& config) {
    if (AUTO_DEAL == config.mVirusAction) {
        return DealVirusWithConfig(path, virus, config, outBackupName);
    }
    else if (MANUAL_DEAL == mScanConfig->mVirusAction) {
        if (mApplication) {
            if (NOTIFY_CONFIRM_SELECT == mApplication->QueryUserForResult(Application::QUERY_FIND_VIRUS, path, virus)) {
                return DealVirusWithConfig(path, virus, config, outBackupName);
            }
        }
    }
    return VirusReportData::STATUS_IGNORE;
}

int Controller::QuarantineVirus(const string& path, const string& virus, std::string& backupName) {
    int err = mQuaArea->BackUpFile(path, virus, backupName);
    if (0 == err) {
        QuarantineLog log;
        log.mDate = Utils::Date::GetTimeStamp();
        log.mFile = Utils::Hash::GetFileMD5(path);
        log.mVirusName = virus;
        log.mSource = path;
        log.mTarget = backupName;

        struct stat buf;
        memset(&buf,0,sizeof(struct stat));

        int ret = stat(path.c_str(), &buf);
        if (ret == 0) {
            log.mUID = buf.st_uid;
            log.mGID = buf.st_gid;
            log.mPermision = buf.st_mode;
            log.mSize = (buf.st_blocks * buf.st_size)/1024;
        }

        LOG_GJCZ("%s mode is %d - %d", path.c_str(), buf.st_uid, buf.st_gid);

        try {
            AppendQuaLog(log);
        }
        catch(...) {
            return -1;
        }
    }
    return err;
}

int Controller::RestoreQuarantine(QuarantineLog& log) {
    if (mQuaArea->RestoreFile(RAVQUA_ACTION_RESTOREDEL/*log.mPermision*/, log.mTarget, NULL, true, log.mPermision, log.mGID, log.mUID) == 0) {
        // check
        struct stat buf;
        memset(&buf,0,sizeof(struct stat));

        int ret = stat(log.mSource.c_str(), &buf);
        if (ret == 0) {
            if(buf.st_uid == log.mUID
                    && buf.st_gid == log.mGID
                    && buf.st_mode == log.mPermision) {
                mWhiteHash.insert(make_pair(Utils::Hash::StrCRC32(log.mSource), true));
                DeleteQuarantine(log);

                return 0;
            }
        }

        ret = chmod(log.mSource.c_str(), 0777);;
        LOG_GJCZ("change the %s mode to 777 and ret is %d", log.mSource.c_str(), ret);

        log.mReserved = Utils::OS::GetUserFromUID(log.mUID);
        UpdateQuaLog(log);

        return -100;
    }
    return -1;
}

int Controller::DeleteQuarantine(const QuarantineLog& log) {
    unlink(log.mTarget.c_str());
    return mStorage->DeleteQuarantine(log);
}

void Controller::ScanPoolWork() {
    while (GetStatus()) {
        if (mScanTaskPool.empty()) {
            mScanPoolCond->Wait();
        }

        LOG_GJCZ("GJCZ start to scan");

        ScanProxy *app = mScanTaskPool.front();
        SetApplication(app);

        // prepare the scan work, in remote scan will report the scan event
        app->Prepare();;

        std::list<pair<std::string,int> > paths;
        app->GetScanPath(paths);
        int err = app->StartScan(paths);
        if (0 == err) {
            mScanTaskCond->Wait();
        }

        LOG_GJCZ("GJCZ scaner finish");

        sleep(1);
        mScanTaskPool.pop();

        app->ScanFinished(err);
        delete app;
        app = NULL;

		// 删除掉引擎的缓存文件, 否则会导致扫描不到病毒. 
		std::string strCache = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), NULL);
		DIR * dir = opendir(strCache.c_str());
		if(dir) {
			struct dirent * ent;
			while(ent = readdir(dir)) { 
				if(strstr(ent->d_name, VIRUS_DB_FILE ".shm")) {
					std::string strFile = strCache+"/"; strFile += ent->d_name;
					unlink(strFile.c_str());
					LOG_GJCZ("GJCZ, delete cache file -->%s", strFile.c_str());
				}
			}
			closedir(dir);
		}
    }
	LOG_GJCZ("GJCZ scaner thread exit!!!");
}


void* Controller::ScanFunc(void *args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);

#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "ScanFunc");
    pthread_setname_np(pthread_self(), threadName);
#endif
    Controller *engine = (Controller*)args;
    engine->ScanPoolWork();

    pthread_exit(0);
    return NULL;
}
