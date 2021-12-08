#include "finishapp.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "commonlocker.h"
#include "cinifile.h"
#include "appconfig.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <list>
#include <utility>
#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pwd.h>


extern CondLocker *gCondLocker;

FinishApplication::FinishApplication(AppConfig *config, const std::string& ownerpath, const std::string& version, uid_t uid) {
    mAppConfig = config;
    mOwnerPath = ownerpath;
    mVersion = version;
    mUid = uid;
}

FinishApplication::~FinishApplication() {

}

int FinishApplication::Start() {
    return pthread_create(&mWorkThread, NULL, ThreadWork, this);
}

bool FinishApplication::CheckUpdateValid(const std::string& updatedir) {
    if (access(updatedir.c_str(), R_OK) == 0){
        return true;
    }
    return false;
}

void* FinishApplication::ThreadWork(void *args) {
    FinishApplication *self = (FinishApplication*)args;
    self->WorkFunc();
    gCondLocker->Signal();
    return NULL;
}

int FinishApplication::WorkFunc() {
    std::string folder = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, UPDATE_TEMP_FOLDER, NULL);
    std::string backup = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, UPDATE_BACKUP_FOLDER, NULL);

    if (!CheckUpdateValid(folder)) {
        return -1;
    }

    if (KillAllProc()) {
        return -2;
    }

    sleep(1);
    bool finish = true;
    if (TravalDir(folder, &FinishApplication::UpdateFile)) {
        TravalDir(backup, &FinishApplication::RestoreFile);
        finish = false;
    }

    Utils::Path::DeleteFolder(folder.c_str());
    Utils::Path::DeleteFolder(backup.c_str());

    if (!mVersion.empty() && finish) {
        mAppConfig->SetAppVersion(mVersion);
        mAppConfig->SetUpdateTime(Utils::Date::GetTimeStamp());
        // TODO 在3.0.0.20的版本，升级后要去掉所有的监控功能，在此做个标记，以后改好后，要去掉此处
        mAppConfig->SetFunctions(AppConfig::X11_SUPPORT|AppConfig::PROC_GUARD);

        mAppConfig->StoreConfig();
    }
    sleep(2);
    StartServer();

    return 0;
}


bool FinishApplication::BackupFile(const std::string& file) {
    std::string backupFolder = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), UPDATE_BACKUP_FOLDER, NULL);
    Utils::Path::CheckPath(backupFolder);

    std::string name = file.substr(mAppConfig->GetInstallPath().size() + 1);

    std::string dest = Utils::Path::JoinPath(backupFolder.c_str(), name.c_str(), NULL);
    return Utils::Path::CopyFile(file, dest);
}

bool FinishApplication::FileCanReplaced(const std::string& file) {
    if (!Utils::Path::FileAvailed(file)) {
        return true;
    }

    if (access(file.c_str(), W_OK) == 0) {
        return true;
    }
    return false;
}

bool FinishApplication::ReplaceFile(const std::string& source, const std::string& dest) {
    if (!FileCanReplaced(dest)) {
        return false;
    }

	bool copy  = Utils::Path::FileAvailed(dest);
    bool state = Utils::Path::CopyFile(source, dest);
    if (state) {
        if(!copy) chmod(dest.c_str(), 0755);
    }
    return state;
}

int FinishApplication::TravalDir(const std::string& folder, UpdateFunc pFunc) {
    char tempPath[1024] = {0};
    DIR *dir = opendir(folder.c_str());
    if (NULL == dir) {
        return 0;
    }

    int error = 0;
    struct dirent *tmp = NULL;
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = false;
#if defined (COMMON_LINUX)
        isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", folder.c_str(), tmp->d_name);
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
            snprintf(tempPath, 1023, "%s/%s", folder.c_str(), tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isfile = bool(buf.st_mode & S_IFREG);
        }

        if (isfile) {
            if (NULL != pFunc) {
                if ((this->*pFunc)(folder, tmp->d_name)) {
                    error = -1;
                    break;
                }
            }
        } else if (isdir) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }
            else if (folder ==  "/")
                sprintf(tempPath, "/%s", tmp->d_name);
            else
                sprintf(tempPath, "%s/%s", folder.c_str(), tmp->d_name);

            if (TravalDir(tempPath, pFunc)) {
                error = -1;
                break;
            }
        }
    }
    closedir(dir);
    return error;
}

int FinishApplication::KillAllProc() {
    const char *commands[] = {
        MAIN_CONSOLE_ENTRY,
        MAIN_LOGVIEW_ENTRY,
        MAIN_SCREEN_ENTRY,
        MAIN_SETTING_ENTRY,
        DAEMON_GJCZ_ENTRY,
        DAEMON_GLTX_ENTRY,
        DAEMON_ZDFY_ENTRY,
        NOTIFYY_ENTRY
    };

    for (int i=0; i<sizeof(commands)/sizeof(const char*); ++i) {
        std::string process = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, commands[i], NULL);
        KillProc(commands[i], SIGKILL);
        sleep(2);
    }

    return 0;
}

int FinishApplication::KillProc(const char *app, int signal) {
    int state = Utils::OS::CheckProcessState(
                    Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, app, NULL).c_str()
                );
    if (state > 0) {
        vector<int> pid;
        if (0 == Utils::OS::GetPid(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, app, NULL).c_str(), pid)) {
            vector<int>::iterator iter;
            for (iter=pid.begin();iter!=pid.end(); ++iter) {
                int status = kill(*iter, signal);
                wait(&status);
            }
        }
    }
    return state;
}

int FinishApplication::StartServer() {
    const char *commands[] = {
        DAEMON_GJCZ_COMMAND,
        DAEMON_GLTX_COMMAND,
        DAEMON_ZDFY_COMMAND
    };

    const char *userproc[] = {
        NOTIFY_RJJH_COMMAND,
    };

    for (int i=0; i<sizeof(commands)/sizeof(const char*); ++i) {
        std::string process = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, commands[i], NULL);
        system(process.c_str());
        sleep(3);
    }

    sleep(3);

    char username[64] = {0};
    struct passwd *pw_ptr;
    if((pw_ptr = getpwuid(mUid))) {
         strcpy(username, pw_ptr->pw_name);
    }

    for (int i=0; i<sizeof(userproc)/sizeof(const char*); ++i) {
        std::string process = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, userproc[i], NULL);
        char temp[128] = {0};
        sprintf(temp, "%s -s", process.c_str());
        system(temp);
//        pid_t pid;
//        if ((pid=fork()) == 0) {
//              if(execlp("su", "su", "-", username, "-c", process.c_str(), "-s", NULL)<0)  {
//                //
//            }
//        }
        sleep(3);
    }

    return 0;
}

int FinishApplication::UpdateFile(const std::string& dir, const std::string& file) {
    std::string source = dir + "/" + file;
    std::string dest;
    int pos = source.find(UPDATE_TEMP_FOLDER);
    if (pos != -1) {
        dest = mAppConfig->GetInstallPath() + "/" + source.substr(pos + strlen(UPDATE_TEMP_FOLDER) + 1);
    }

    if (Utils::Path::FileAvailed(dest)) {
        if (!BackupFile(dest)) {
            return -1;
        }
    }

    if (!ReplaceFile(source, dest)) {
        return -1;
    }

    return 0;
}

int FinishApplication::RestoreFile(const std::string& dir, const std::string& file) {
    std::string source = dir + "/" + file;
    std::string dest;
    int pos = source.find(UPDATE_BACKUP_FOLDER);
    if (pos != -1) {
        dest = mAppConfig->GetInstallPath() + "/" + source.substr(pos + strlen(UPDATE_BACKUP_FOLDER) + 1);
    }

    if (!ReplaceFile(source, dest)) {
        return -1;
    }

    return 0;
}
