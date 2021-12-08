#include "defenplugins.h"
#include "Entry/commonentry.h"
#include "defenmodproxy.h"
#include "commonapi.h"
#include "observer.h"
#include "Entry/whitelistentry.h"
#include "datacontainer.h"
#include "scanconfig.h"
#include "baseconfig.h"
#include "config.h"
#include "panzer_if.h"

#include <fnmatch.h>


const char *DOC_TYPE[] = {
    "*.doc",
    "*.docx",
    "*.ppt",
    "*.pptx",
};

const char *EXE_TYPE[] = {
    "*.bin",
    "*.so"
};

const char *ZIP_TYPE[] = {
    "*.zip",
    "*.tgz",
    "*.tar.gz",
    "*.tar",
    "*.bz2",
    "*.rar"
};

DefenPlugins* DefenPlugins::NEW(DefenDataContainer *setting) {
    DefenPlugins *self = new DefenPlugins;
    if (self) {
        if (self->Construct(setting)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

DefenPlugins::DefenPlugins() {

}

DefenPlugins::~DefenPlugins() {

}

int DefenPlugins::Construct(DefenDataContainer *setting) {
    mSettings = setting;
    return 0;
}

int DefenPlugins::GetCommonFolder(uid_t uid, list<string>& folder) {
    if (mUserName.empty()) {
        mUserName = Utils::OS::GetUserFromUID(uid);
    }
    if (mUserName.empty() || mUserName=="root") {
        folder.push_back("/root/*");
    }
    else {
        const char *names[] = {
            "Downloads",
            "Documents",
            "Desktop",
            "下载",
            "文档",
            "桌面"
        };
        for (int i=0; i<sizeof(names)/sizeof(char*); ++i) {
            string path = Utils::Path::JoinPath("/home", mUserName.c_str(), names[i], "*", NULL);
            if (Utils::Path::PathExists(path)) {
                folder.push_back(path);
            }
        }
    }
    return 0;
}

int DefenPlugins::LoadConfig(uid_t current) {
    mSettings->mFolderMonitorList.clear();
    mUserName = Utils::OS::GetUserFromUID(current);

    //awlays add /etc
    //if (mSettings->mFileMonitorSetting->mFileMonitor.mKernelMonitor.mValue) {
        // start the kernel monitor
        mSettings->mFolderMonitorList.push_back("/etc/*");
    //}
    switch(mSettings->mFileMonitorSetting->mFileMonitor.mMonitorType.mValue) {
    /*
    case AntiVirusSetting::FileMonitor::GEEK: {
        mSettings->mFolderMonitorList.push_back("/lib/*");
        mSettings->mFolderMonitorList.push_back("/bin/*");
        mSettings->mFolderMonitorList.push_back("/var/*");
        mSettings->mFolderMonitorList.push_back("/tmp/*");
    }
    */
    case AntiVirusSetting::FileMonitor::PROFESSIONAL: {
        mSettings->mFolderMonitorList.push_back("/usr/*");
        mSettings->mFolderMonitorList.push_back("/opt/*");
        mSettings->mFolderMonitorList.push_back("/lib/*");
        mSettings->mFolderMonitorList.push_back("/bin/*");
        mSettings->mFolderMonitorList.push_back("/var/*");
        mSettings->mFolderMonitorList.push_back("/tmp/*");
        break;
    }

    case AntiVirusSetting::FileMonitor::STANDENT: {
        string homePath;
        if(!mUserName.empty()){
             homePath = string("/home/") + mUserName + string("/*");
        }else{
            homePath = string("/home/root/*");
        }
        mSettings->mFolderMonitorList.push_back(homePath);
        break;
    }
        /*
    case AntiVirusSetting::FileMonitor::QUICK_MODE: {
        list<string> common;
        GetCommonFolder(current, common);
        mSettings->mFolderMonitorList.merge(common);
        break;
    }
    */
    }
    //mSettings->mFolderMonitorList.merge(mSettings->mFileMonitorSetting->mFileMonitor.mMonitorPaths.mValue);

    mSettings->mFileTypeMonitorList.clear();
    if (mSettings->mFileMonitorSetting->mFileMonitor.mFileType.mValue == EXE_DOC) {
        for (int i=0; i<sizeof(EXE_TYPE)/sizeof(char*); ++i) {
            mSettings->mFileTypeMonitorList.push_back(EXE_TYPE[i]);
        }

        for (int i=0; i<sizeof(DOC_TYPE)/sizeof(char*); ++i) {
            mSettings->mFileTypeMonitorList.push_back(DOC_TYPE[i]);
        }
    }
    return 0;
}

inline bool DefenPlugins::Trusted(int type, const string& file) {
    if (type == DefenObserver::DEFEN_FILE_WRITE) {
        const char* OWEN_PATH[] = {
            STORAGE_DB_PATH, STORAGE_GLTX_PATH, FOLDER_GGFW_GJCZ, FOLDER_RJJH_GLTX,
            FOLDER_GGFW_RJJH, FOLDER_RJJH_GJCZ, FOLDER_RJJH_ZDFY, FOLDER_GJCZ_ETC,
            FOLDER_RJJH_ETC, QUARANTINE_PATH
        };
        for (int i=0; i<sizeof(OWEN_PATH)/sizeof(char*); ++i) {
            if (file.find(OWEN_PATH[i]) != string::npos) {
                return true;
            }
        }

        // config file, ignore, tmp file ignore
        if (mHomeConfigPathRegex.empty()) {
            mHomeConfigPathRegex = Utils::Path::JoinPath("/home", mUserName.c_str(), ".*", NULL);
        }
        if (0 == fnmatch(mHomeConfigPathRegex.c_str(), file.c_str(), FNM_NOESCAPE)) {
            return true;
        }
        // temp file
        if (0 == fnmatch("*.swp", file.c_str(), FNM_NOESCAPE)) {
            return true;
        }
    }
    else if (type == DefenObserver::DEFEN_PROC_START) {
        const char* OWEN_EXEC_PATH[] = {
            DAEMON_GJCZ_ENTRY, DAEMON_GLTX_ENTRY,
            UPDATE_PROCESS_ENTRY, DAEMON_ZDFY_ENTRY,
            NOTIFYY_ENTRY, MAIN_SCREEN_ENTRY,
            MAIN_SETTING_ENTRY, MAIN_LOGVIEW_ENTRY,
            CONTEXT_DAEMON_ENTRY, MAIN_CONSOLE_ENTRY
        };
        for (int i=0; i<sizeof(OWEN_EXEC_PATH)/sizeof(char*); ++i) {
            if (file.find(OWEN_EXEC_PATH[i]) != string::npos) {
                return true;
            }
        }
    }
    return false;
}


DefenPlugins::JudgeResult DefenPlugins::Judge(int type, const string& filepath, void *data, string& out) {
//    if (!Utils::String::StartsWith(filepath, "/opt/BDFZ/RAV")) {
//        LOG_ZDFY("get the file notity %s", filepath.c_str());
//    }

    if (Trusted(type, filepath)) {
        return DEFEN_IGNORE;
    }

    //LOG_ZDFY("not a trust file, go on...");

    if (type == DefenObserver::DEFEN_FILE_WRITE) {
        string path, name;
        Utils::Path::SplitPath(filepath, path, name);

        bool matched = false;
        list<string>::iterator iter;
        for (iter=mSettings->mFolderMonitorList.begin();iter!=mSettings->mFolderMonitorList.end();++iter) {
            if (FNM_NOMATCH != fnmatch(iter->c_str(), filepath.c_str(), FNM_NOESCAPE)) {
                matched = true;
                break;
            }
        }

        if (matched) {
            if (mSettings->mFileTypeMonitorList.empty()) {
                return DEFEN_FILE_SCAN;
            }
            else {
                for (iter=mSettings->mFileTypeMonitorList.begin(); iter!=mSettings->mFileTypeMonitorList.end(); ++iter) {
                    if (FNM_NOMATCH != fnmatch(iter->c_str(), name.c_str(), FNM_NOESCAPE)) {
                        return DEFEN_FILE_SCAN;
                    }
                }
            }
        }
    }
    else if (type == DefenObserver::DEFEN_PROC_START) {
        string path, name;
        Utils::Path::SplitPath(filepath, path, name);

        LOG_ZDFY("exec proc is %s and params is %s\n", filepath.c_str(), data);

        if (name == "insmod") {
            vector<string> cmds;
            struct info_header *umsg = (struct info_header *)data;
            Utils::String::Split(string(umsg->execve.cmdline), " ", &cmds);

            LOG_ZDFY("cmdline is %s", umsg->execve.cmdline);
            if (cmds.size() == 2) {
                out = cmds[1];
                if (!Utils::String::StartsWith(out, "/")) {
                    out = Utils::Path::JoinPath(umsg->curdir,cmds[1].c_str(), NULL);
                }
                LOG_ZDFY("will scan kernel module %s", out.c_str());

                if (out.find(FILE_DEFEN_MODULE) != string::npos
                        || out.find(NET_FILTER_MODULE) != string::npos) {
                    return DEFEN_IGNORE;
                }
                return DEFEN_PROC_EXTENS;
            }
        }
        else if (DocOpener(name)) {
            vector<string> cmds;
            struct info_header *umsg = (struct info_header *)data;
            Utils::String::Split(string(umsg->execve.cmdline), " ", &cmds);

            LOG_ZDFY("scan documents file %s", umsg->execve.cmdline);

            if (cmds.size() == 3) {
                out = cmds[2];
                if (Utils::String::StartsWith(cmds[2], "file://")) {
                    out = cmds[2].substr(7);
                }
                return DEFEN_PROC_EXTENS;
            }
            else if (cmds.size() == 2) {
                out = cmds[1];
            }
        }
#ifdef RISING_TEST_
        else if (name == "RAVRJJH99") {
            out = filepath;
            return DEFEN_PROC_EXTENS;
        }
#endif
        list<string>::iterator iter;
        for (iter=mSettings->mProcMonitorList.begin();iter!=mSettings->mProcMonitorList.end();++iter) {
            if (Utils::Path::MatchPath(iter->c_str(), filepath.c_str())) {
                return DEFEN_PROC_DENY;
            }
        }
    }

    return DEFEN_IGNORE;
}

bool DefenPlugins::DocOpener(const string& name) {
    return name=="libreoffice"
            || name=="wps"
            || name=="oosplash"
            || name=="atril";
}
