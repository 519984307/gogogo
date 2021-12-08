#include "countfilevari.h"
#include "scanengine_global.h"
#include "engineobserver.h"
#include "scanconfig.h"
#include "commonlocker.h"
#include "commonapi.h"
#include "baseconfig.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fnmatch.h>
#include <iostream>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

const char *EXEC_DOC_TYPE[] = {
    // executable
    "*.bin",
    "*.exe",
    "*.com",
    "*.run"
    // document type
    "*.txt",
    "*.doc",
    "*.docx",
    "*.rtf",
    "*.ppt",
    "*.pptx",
    "*.xls",
    "*.xlsx",
    "*.pdf",
    "*.TXT",
    "*.DOC",
    "*.DOCX",
    "*.RTF",
    "*.PPT",
    "*.PPTX",
    "*.XLS",
    "*.XLSX",
    "*.PDF"
};

CountFileVari* CountFileVari::NEW(EngineObserver *observer,
                                  bool deal,
                                  std::list<pair<std::string,int> > *whitelist,
                                  int scantype,
                                  std::list<std::string> *filetype,
                                  bool intelligent) {
    CountFileVari *self = new CountFileVari(observer, deal, whitelist, intelligent);
    if (self) {
        if (self->Construct(scantype, filetype)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

CountFileVari::CountFileVari(EngineObserver *observer, bool deal, std::list<pair<std::string,int> > *whitelist, bool intelligent) {
    mStatus = COUNT_STATUS_INIT;
    mObserver = observer;
    mDealFile = deal;
    mWhiteList = whitelist;
    mScanType = 0;
    mSpecialFileType = NULL;
    mIntelligentMode = intelligent;
    mPauseCond = NULL;
}

int CountFileVari::Construct(int scantype, std::list<std::string> *filetype) {
    mScanType = scantype;
    mSpecialFileType = filetype;

    mPauseCond = CondLocker::NEW();

    return 0;
}

CountFileVari::~CountFileVari()  {
    Stop();
    pthread_join(mThread, NULL);

    SECURITY_FREE(mPauseCond);
}
int CountFileVari::Start(string path, int maxDepth) {
    mTempPath.clear();
    mTempPath.push_back(make_pair(path, maxDepth));

    return pthread_create(&mThread, NULL, ThreadWork, this);
}

int CountFileVari::Start(const list<pair<string, int> >& paths) {
    mTempPath = paths;

    return pthread_create(&mThread, NULL, ThreadWork, this);
}

void CountFileVari::Pause() {
    if (COUNT_STATUS_SCANING == mStatus) {
        mStatus = COUNT_STATUS_PAUSE;
    }
}

void CountFileVari::Continue() {
    if (COUNT_STATUS_PAUSE == mStatus) {
        mStatus = COUNT_STATUS_SCANING;
        mPauseCond->Signal();
    }
}

void CountFileVari::Stop() {
    mStatus = COUNT_STATUS_STOP;
    mPauseCond->Signal();
}


void *CountFileVari::ThreadWork(void *args) {
    CountFileVari *self = (CountFileVari*)args;

    if (NULL != self->mObserver) {
        if (self->mDealFile) {
            self->mObserver->OnVirusEvent(EngineObserver::SCAN_FILE, EngineObserver::SCAN_START, NULL);
        }
    }

    LOG_GJCZ("start the %s thread", self->mDealFile?"scan":"count");
#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "File%s", self->mDealFile?"Scan":"Count");
    if(pthread_setname_np(pthread_self(), threadName)< 0)
    {
        LOG_GJCZ("pthread_setname_np set name:%s failue", threadName);
    }
#endif
    self->mStatus = COUNT_STATUS_SCANING;
    long count = 0;
    std::list<pair<std::string, int> >::iterator iter;
    for (iter=self->mTempPath.begin(); iter!=self->mTempPath.end(); ++iter) {
        if (COUNT_STATUS_STOP != self->mStatus) {
            count += self->CountFile(iter->first.c_str(), 0,
                                     iter->second,
                                     self->mDealFile?(CountFunc)&CountFileVari::DealFile:NULL);
        }
    }

    if (NULL != self->mObserver) {
        if (self->mDealFile) {
            self->mObserver->OnVirusEvent(EngineObserver::SCAN_FILE, EngineObserver::SCAN_FINISH, &count);
        }
        else {
            self->mObserver->OnVirusEvent(EngineObserver::COUNT_PATH, EngineObserver::COUNT_FINISH, &count);
        }
    }

    LOG_GJCZ_DEBUG("%s thread finished", self->mDealFile?"scan":"count");
    return NULL;
}

int CountFileVari::DealFile(const char *dirName, const char *fileName) {
    if (COUNT_STATUS_PAUSE == mStatus) {
        mPauseCond->Wait();
    }

    if (NULL != mObserver) {
        std::pair<const char*, const char*> data = make_pair(dirName, fileName);
        mObserver->OnVirusEvent(EngineObserver::SCAN_FILE, EngineObserver::FILE_PATH, &data);
    }
    return 0;
}

long CountFileVari::CountFile(const char *dirName, int scanDepth, int MAX_DEPTH, CountFunc pFunc) {
    if(MAX_DEPTH>0 && scanDepth>MAX_DEPTH) {
        return 0;
    }

    long fileNum = 0;
    char tempPath[1024] = {0};
    DIR *dir = opendir(dirName);
    if (NULL == dir) {
        if (access(dirName, R_OK) == 0) {
            const char *temp = strrchr(dirName, '/');
            if (temp) {
                strncpy(tempPath, dirName, temp-dirName);
                // it is file
                if (NULL != pFunc) {
                    LOG_GJCZ_DEBUG("scan file %s", dirName);

                    (this->*pFunc)(tempPath, temp+1);
                }
            }
            return 1;
        }
        return 0;
    }

    struct dirent *tmp = NULL;
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = false;
#if defined (COMMON_LINUX)
        isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", dirName, tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
        }

        bool isfile = false;
#if defined (COMMON_LINUX)
        isfile = DT_REG==tmp->d_type;
        if (!isfile && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", dirName, tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isfile = bool(buf.st_mode & S_IFREG);
        }


        if (isfile) {
            if (AvailableFile(tmp->d_name)) {
                sprintf(tempPath, "%s/%s", dirName, tmp->d_name);
                if (!MatchWhite(tempPath)) {
                    fileNum++;
                    if (NULL != pFunc) {
                        LOG_GJCZ_DEBUG("scan file %s", dirName);
                        (this->*pFunc)(dirName, tmp->d_name);
                    }
                }
            }
        } else if (isdir) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }
            else if (0 == strcmp(dirName, "/"))
                sprintf(tempPath, "/%s", tmp->d_name);
            else
                sprintf(tempPath, "%s/%s", dirName, tmp->d_name);

            if (access(tempPath, R_OK)) {
                continue;
            }
            fileNum += CountFile(tempPath, scanDepth+1, MAX_DEPTH, pFunc);
        }

        if (COUNT_STATUS_STOP == mStatus) {
            break;
        }
    }
    closedir(dir);
    return fileNum;
}

bool CountFileVari::MatchRule(const char *filename, std::list<std::string>& rules) {
    bool match = false;
    std::list<std::string>::iterator iter;
    for (iter=rules.begin(); iter!=rules.end(); ++iter) {
        if (0 == fnmatch(iter->c_str(), filename, FNM_NOESCAPE)) {
            return true;
        }
    }
    return false;
}

bool CountFileVari::MatchRule(const char *filename, std::list<pair<std::string, int> >& rules) {
    bool match = false;
    std::list<pair<std::string,int> >::iterator iter;
    for (iter=rules.begin(); iter!=rules.end(); ++iter) {
        // type define in enum PathType whitelistentry.h
        switch (iter->second) {
        case 0: {
            // PathType::FILE_PATH
            if (iter->first == string(filename)) {
                return true;
            }
            break;
        }
        case 1: {
            // PathType::FOLDER_ONLY
            if (0 == fnmatch(iter->first.c_str(), filename, FNM_PATHNAME)) {
                return true;
            }
            break;
        }
        case 2: {
            // PathType::FOLDER_RECURSION
            if (0 == fnmatch(iter->first.c_str(), filename, FNM_NOESCAPE)) {
                return true;
            }
            break;
        }
        case 3: {
            // PathType::FOLDER_CHILDERN
            if (fnmatch(iter->first.c_str(), filename, FNM_PATHNAME)
                    && (0 == fnmatch(iter->first.c_str(), filename, FNM_NOESCAPE))) {
                return true;
            }
            break;
        }
        case 4: {
            string path, file;
            Utils::Path::SplitPath(filename, path, file);
            if (0 == fnmatch(iter->first.c_str(), file.c_str(), FNM_PATHNAME)) {
                return true;
            }
            break;
        }
        default:
            break;
        }
    }
    return match;
}

bool CountFileVari::MatchRule(const char *filename, const char **rules, int len) {
    bool match = false;
    for (int i=0; i<len; ++i) {
        if (0 == fnmatch(rules[i], filename, FNM_NOESCAPE)) {
            match = true;
            break;
        }
    }
    return match;
}

bool CountFileVari::MatchWhite(const char *fileName) {
    bool result = MatchRule(fileName, *mWhiteList);
    if (result) {
        return result;
    }
    else {
        if (mIntelligentMode) {
            string path, file;
            Utils::Path::SplitPath(fileName, path, file);
            if ((Utils::String::StartsWith(path, "/usr") || Utils::String::StartsWith(path, "/opt"))
                    && (Utils::String::EndsWith(file, ".jar")
                        || Utils::String::EndsWith(file, ".JAR")
                        || Utils::String::EndsWith(file, ".js"))) {
                return true;
            }
        }
    }
    return false;
}

bool CountFileVari::AvailableFile(const char *filename) {
    if (ALL_FILE == mScanType) {
        return true;
    }
    else if (EXE_DOC == mScanType) {
        return MatchRule(filename, EXEC_DOC_TYPE, sizeof(EXEC_DOC_TYPE)/sizeof(char*));
    }
    else if (SPECIAL_FILE_TYPE == mScanType) {
        if (mSpecialFileType == NULL) {
            return false;
        }
        return MatchRule(filename, *mSpecialFileType);
    }
    return false;
}

