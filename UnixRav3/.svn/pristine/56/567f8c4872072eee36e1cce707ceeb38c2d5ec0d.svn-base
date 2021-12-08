#include "notifycontroller.h"
#include "screenwidget.h"
#include "baseconfig.h"
#include "cinifile.h"
#include "storage.h"
#include "clientinfo.h"
#include "appconfig.h"

#include <string.h>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

NotifyController* NotifyController::NEW(const char *installdir) {
    NotifyController *self = new NotifyController(installdir);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

NotifyController::NotifyController(const char *installdir) {
    mWidget = NULL;
    mGJCZReceiver = NULL;
    mGLTXReceiver = NULL;
    mZDFYReceiver = NULL;
    mAppConfig = NULL;
    mAsyncPool = NULL;
    mLocker = NULL;

    mInstallDir = installdir;

    mState = false;
}

int NotifyController::Construct() {
    StorageBridge::Construct(mInstallDir, FOLDER_RJJH_GJCZ);

    mAsyncPool = Handler::NEW(this);
    mLocker = MutexLocker::NEW();

    char sockname[64];
    sprintf(sockname, RJJH_NOTIFY_SOCKET, getuid());
    mGJCZReceiver = CenterListener::NEW (
                this,
                CenterCommandObserver::CLIENT_COMMAND,
                Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, sockname, NULL).c_str(),
                true
    );
    mGLTXReceiver = CenterListener::NEW (
                this,
                CenterCommandObserver::CLIENT_COMMAND,
                Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GLTX, sockname, NULL).c_str(),
                true
    );

    mZDFYReceiver = CenterListener::NEW (
                this,
                CenterCommandObserver::CLIENT_COMMAND,
                Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_ZDFY, sockname, NULL).c_str(),
                true
    );

    string config = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_ETC, CONFIG_INI_FILE, NULL);
    mAppConfig = AppConfig::NEW(config);
    ASSERT_ZERO(mAppConfig, -1);

    return 0;
}

NotifyController::~NotifyController() {
    if (mGJCZReceiver) {
        delete mGJCZReceiver;
    }
    if (mGLTXReceiver) {
        delete mGLTXReceiver;
    }
    if (mZDFYReceiver) {
        delete mZDFYReceiver;
    }
    if (mAppConfig) {
        delete mAppConfig;
    }
    if (mAsyncPool) {
        delete mAsyncPool;
    }
    if (mLocker) {
        delete mLocker;
    }
}

int NotifyController::OnCommandObserver(int event, void *args, void **result) {
    CommandBundle *command = (CommandBundle*)args;
    switch (command->mCommandID) {
    case CommandBundle::DAEMON_EXIT:
        exit(0);
    case CommandBundle::FIND_PROXY_CLIENT: {
        Utils::LOG::PrintLog(1, "Proxy get the finder, back 999");
        return 999;
    }
    case CommandBundle::RELOAD_CONFIG: {
        mAppConfig->UpdateConfig(command->mStringValue);
        mAppConfig->StoreToConfig();

        mAsyncPool->PostMessage(Message(0,0,0));
        break;
    }
    case CommandBundle::NOTIFY_EVENT:
        return RespondNotify(command->mStringValue);
    case CommandBundle::REMOTE_COMMAND: {
        switch (command->mIntegerValue) {
        case CommandBundle::REMOTE_COMMAND_START:
        case CommandBundle::POLICY_DEPLOY_CHANGED:
        case CommandBundle::POLICY_GLOBAL_CHANGED:
        case CommandBundle::POLICY_LINUX_CHANGED:
        case CommandBundle::DEFEN_FILE_CHANGED:
        case CommandBundle::DEFEN_PROC_STARTED:
            return RespondProxyEvent(*command, TO_GJCZ, result);
        case CommandBundle::REMOTE_COMMAND_FINISHED:
        case CommandBundle::REMOTE_COMMAND_SCANLOG:
        case CommandBundle::REMOTE_COMMAND_VIRUSLOG:
            return RespondProxyEvent(*command, TO_GLTX, result);
        case CommandBundle::POLICY_LINUX_ZDFY_CHANGED:
        case CommandBundle::DEFEN_FILE_ZDFY_CHANGED:
        case CommandBundle::REMOTE_COMMAND_DEFEN:
            return RespondProxyEvent(*command, TO_ZDFY, result);
        default:
            break;
        }
    }
    default:
        break;
    }

    return 0;
}

int NotifyController::RespondProxyEvent(CommandBundle& bundle, enum NOTIFY_TO who, void **reslult) {
    string toPath;
    if (TO_GJCZ == who) {
        toPath = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    }
    else if(TO_GLTX == who) {
        toPath = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
    }
    else if(TO_ZDFY == who) {
        toPath = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_ZDFY, ZDFY_RJJH_SOCKET, NULL);
    }

    if (!toPath.empty()) {
        Utils::LOG::PrintLog(1, "Proxy received a remote command and resend to %s", toPath.c_str());

        int ret = 0;
        RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
        if (0 == pack.Externalize(toPath.c_str(), ret, reslult)) {
            return ret;
        }
    }
    return -1;
}

int NotifyController::RespondNotify(const string &data) {
    CommunicateBundle bundle;
    if (!bundle.Internalize(data.c_str(), data.size())) {
        return -1;
    }

    if (bundle.mFlag == CommunicateBundle::ISEEYOU
            && bundle.mType == CommunicateBundle::NOTIFY_EIXT) {
        exit(0);
    }
    else {
        mWidget->ShowTips(bundle);
        bundle.mSelected = mWidget->GetSelected();
    }

    return 0;
}

void NotifyController::HandleMessage(int event, int type, void *object) {
    if (0 == event) {
        BroadcastSelf();
    }
}

int NotifyController::OpenApp(const char *app, const char *command) {
    std::string main = Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, app, NULL);
    int state = Utils::OS::CheckProcessState(main.c_str());
    if (state > 0) {
        return -1;
    }
    else {
        system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, command, NULL).c_str());
    }
    return state;
}

int NotifyController::SendToDaemon(int cid) {
    CommandBundle command;
    command.mCommandID = cid;
    command.mIntegerValue = getuid();
    command.mStringValue = mAppConfig->JsonString();

    std::string sock = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}

int NotifyController::KillProc(const char *app, int signal) {
    int state = Utils::OS::CheckProcessState(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, app, NULL).c_str());
    if (state > 0) {
        char pid[12];
        if (0 == Utils::OS::GetPid(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, app, NULL).c_str(), pid)) {
            int status = kill(atoi(pid), signal);
            wait(&status);
        }
    }
    return state;
}

void NotifyController::OnWidgetEvent(int event, int type, void */*data*/) {
    if (MENU_EVENT == event) {
        switch (type) {
        case OPEN_MAIN_APP: {
            char lockfile[64];
            sprintf(lockfile, LOCK_RJJH_MAIN, getuid());
            printf("install dir is %s\n", mInstallDir.c_str());
            if (!Utils::OS::GetProcState(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, lockfile, NULL).c_str())) {
                printf("xxxxxxxxxxxxxxx\n");
                system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, MAIN_SCREEN_COMMAN, NULL).c_str());
            }
            break;
        }
        case OPEN_SETTING_APP: {
            char lockfile[64];
            sprintf(lockfile, LOCK_RJJH_SETTING, getuid());
            if (!Utils::OS::GetProcState(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, lockfile, NULL).c_str())) {
                system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, MAIN_SETTING_COMMAND, NULL).c_str());
            }
            break;
        }
        case OPEN_LOG_APP: {
            char lockfile[64];
            sprintf(lockfile, LOCK_RJJH_LOGS, getuid());
            if (!Utils::OS::GetProcState(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, lockfile, NULL).c_str())) {
                system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, MAIN_LOGVIEW_COMMAND, NULL).c_str());
            }
            break;
        }
        case OPEN_UPDATE: {
            SendToDaemon(CommandBundle::DAEMON_UPDATE);
            break;
        }
        case EXIT_ALL: {
//            KillProc(WATCHDOG_PROGUARD_PROCESS, SIGUSR2);
//            KillProc(DAEMON_PROCESS_ENTRY, SIGUSR2);
//            KillProc(MAIN_SCREEN_ENTRY, SIGKILL);
//            KillProc(MAIN_SETTING_ENTRY, SIGKILL);
//            KillProc(MAIN_LOGVIEW_ENTRY, SIGKILL);

            SendToDaemon(CommandBundle::DAEMON_EXIT);
            exit(0);
        }
        default:
            break;
        }
    }
}


void NotifyController::Start() {
    mState = true;

    mWidget->run();
    mGJCZReceiver->Start();
    mGLTXReceiver->Start();
    mZDFYReceiver->Start();

    pthread_create(&mCheckThread, NULL, ThreadWork, this);
}

void NotifyController::Stop() {
    mState = false;
    mGJCZReceiver->Stop();
    mGLTXReceiver->Stop();
    mZDFYReceiver->Stop();

    pthread_cancel(mCheckThread);
    pthread_join(mCheckThread, NULL);
}

void NotifyController::BroadcastSelf() {
    mLocker->Lock();
    {
        NotifyService(FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET);
        NotifyService(FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET);
        NotifyService(FOLDER_RJJH_ZDFY, ZDFY_RJJH_SOCKET);
    }
    mLocker->Unlock();
}

int NotifyController::NotifyService(const char *folder, const char *sockfile) {
    // notify the daemon notify uid
    std::string sock = Utils::Path::JoinPath(mInstallDir.c_str(), folder, sockfile, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::NOTIFY_READY;
    command.mIntegerValue = getuid();
    command.mStringValue = mAppConfig->JsonString();

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}

void* NotifyController::ThreadWork(void *params) {
    NotifyController *self = (NotifyController*)params;
    self->CommunicateWork();
    pthread_exit(NULL);
}

int NotifyController::CommunicateWork() {
    while (mState) {
        BroadcastSelf();
        sleep(COMMUNICATE_HEART_INTERVAL);
    }
    return 0;
}
