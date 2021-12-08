#include "mainengine.h"
#include "baseconfig.h"
#include "cinifile.h"
#include "storage.h"
#include "appconfig.h"
#include "storagebridge.h"
#include "Entry/quarantinelog.h"

#include <dirent.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define ASYNC_RELOAD_CONFIG   0
#define ASYNC_RESET_CONFIG    1

#ifdef WITH_QT
    extern void unload_widget();
    extern void show_notify(CommunicateBundle *bundle);
    extern void set_tray_icon(bool state);
    extern void* start_qt_env(void *params);
#endif

MainEngine* MainEngine::NEW(AppConfig *config, bool silence) {
    MainEngine *self = new MainEngine(config);
    if (self) {
        if (self->Construct(silence)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

MainEngine::MainEngine(AppConfig *config) {
    mGJCZReceiver = NULL;
    mGLTXReceiver = NULL;
    mZDFYReceiver = NULL;
    mAsyncPool = NULL;

    mState = false;

#ifndef WITH_QT
    mQtSupport = false;
    mDynamicHandler = NULL;
    mNotifyFunc = NULL;
    mSetTrayStateFunc = NULL;
#endif

    mAppConfig = config;
}

int MainEngine::InitializeGeneralPolicy() {
    string defaultini = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ETC, CONFIG_DEF_FILE, NULL);
    if (Utils::Path::FileAvailed(defaultini)) {
        Utils::Path::CopyFile(defaultini, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, CONFIG_DEF_FILE, NULL));
        Utils::Path::CopyFile(defaultini, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, CONFIG_DEF_FILE, NULL));
        Utils::Path::CopyFile(defaultini, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, CONFIG_DEF_FILE, NULL));
        unlink(defaultini.c_str());
    }

    string policy = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ETC, CONFIG_DEF_POLICY, NULL);
    if (Utils::Path::PathExists(policy)) {
        struct dirent *tmp = NULL;
        DIR *dir = opendir(policy.c_str());
        while (NULL != (tmp = readdir(dir))) {
            bool isfile = false;
            char tempPath[1024] = {0};
    #if defined (COMMON_LINUX)
            isfile = DT_REG==tmp->d_type;
            if (!isfile && DT_UNKNOWN==tmp->d_type)
    #endif
            {
                snprintf(tempPath, 1023, "%s/%s", policy.c_str(), tmp->d_name);
                struct stat buf;
                stat(tempPath, &buf);
                isfile = bool(buf.st_mode & S_IFREG);
            }

            if (isfile) {
                string file = Utils::Path::JoinPath(policy.c_str(), tmp->d_name, NULL);

                Utils::Path::CheckPath(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, CONFIG_DEF_POLICY, NULL));
                Utils::Path::CopyFile(file, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, CONFIG_DEF_POLICY, tmp->d_name, NULL));
                Utils::Path::CheckPath(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, CONFIG_DEF_POLICY, NULL));
                Utils::Path::CopyFile(file, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, CONFIG_DEF_POLICY, tmp->d_name, NULL));
                Utils::Path::CheckPath(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, CONFIG_DEF_POLICY, NULL));
                Utils::Path::CopyFile(file, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, CONFIG_DEF_POLICY, tmp->d_name, NULL));
            }
        }
        Utils::Path::DeleteFolder(policy.c_str());
        return 0;
    }
    return -1;
}

int MainEngine::Construct(bool silence) {
    // init the general policy, if the default.ini exist, then copy the default.ini to RJJHGJCZ RJJHGLTX RJJHZDFY
    // if the policy.rpk exist, then extract the rpk, then copy it to daemon policy folder
    InitializeGeneralPolicy();

    //
    StorageBridge::Construct(mAppConfig->GetInstallPath(), FOLDER_RJJH_GLTX);

    mAsyncPool = Handler::NEW(this);

    LOG_RJJH("create the listener");

    char sockname[64];
    sprintf(sockname, RJJH_NOTIFY_SOCKET, getuid());
    mGJCZReceiver = CenterListener::NEW (
                this,
                CenterCommandObserver::CLIENT_COMMAND,
                Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL).c_str(),
                true
    );
    mGLTXReceiver = CenterListener::NEW (
                this,
                CenterCommandObserver::CLIENT_COMMAND,
                Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, sockname, NULL).c_str(),
                true
    );

    mZDFYReceiver = CenterListener::NEW (
                this,
                CenterCommandObserver::CLIENT_COMMAND,
                Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, sockname, NULL).c_str(),
                true
    );

//    mRJJHReceiver = ProxyInfoListener::NEW(inet_addr(RJJH_BROAD_ADDR), RJJH_BROAD_PORT);
//    ASSERT_ZERO(mRJJHReceiver, -1);
//    mRJJHReceiver->SetObserver(this);

    // 防止有无法转换的的文件UID不对的, 在此做一些补救
    RechownQuarantine();

#ifdef WITH_QT
    // 托盘程序做为第一个启动的程序，启动后首先判断是不是有当前需要恢复文件权限的
    // 在中标的保密机器上，后台进程无法修改文件所有者，所以在这里做一个补充修改

    start_qt_env(mAppConfig);

    // 此函数的作用是判断数据库存储的值是否和配置文件里的对应，
    // 如果不对应，以数据库里的为准，然后更新配置文件
    RepairConfig();
#else    
    if ((mAppConfig->GetFunctions() & AppConfig::X11_SUPPORT) && !silence) {
        LOG_RJJH("create the QT thread");

        LoadQtWidgetLib(GetTrayIconState());

        LOG_RJJH("create the QT finished");
    }    
#endif
    return 0;
}

MainEngine::~MainEngine() {
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
#ifndef WITH_QT
    UnloadQtWidget();
#endif
}

int MainEngine::RechownQuarantine() {
    StorageBridge *storage = StorageBridge::NEW(mAppConfig->GetInstallPath(), FOLDER_RJJH_GJCZ);
    if (storage) {
        std::list<QuarantineLog> rechown_files;
        QuarantineLog seed;
        seed.mReserved = Utils::OS::GetUserFromUID(getuid());
        if (storage->GetQuaLogs(rechown_files, seed) > 0) {
            list<QuarantineLog>::iterator iter;
            for (iter=rechown_files.begin();iter!=rechown_files.end(); ++iter) {
                string backupfile = iter->mSource + "backup";
                Utils::Path::CopyFile(iter->mSource, backupfile);
                unlink(iter->mSource.c_str());
                Utils::Path::CopyFile(backupfile, iter->mSource);
                unlink(backupfile.c_str());
                chmod(iter->mSource.c_str(), iter->mPermision);

                storage->DeleteQuaLog(*iter);
            }
        }
        SECURITY_FREE(storage);
    }
    return 0;
}

int MainEngine::RespondUpdateEvent(int type, const string &value) {
    switch (type) {
    case CommandBundle::UPGRADE_DOWNLOAD_RP:

        break;
    case CommandBundle::UPGRADE_COMBINE_RP:
        break;
    }
}

int MainEngine::OnCommandObserver(int event, void *args, void **result, int *outlen) {
    CommandBundle *command = (CommandBundle*)args;
    switch (command->mCommandID) {
    case CommandBundle::DAEMON_OFFLINE_UPDATE:
    case CommandBundle::DAEMON_CENTER_UPDATE: {
        RespondUpdateEvent(command->mIntegerValue, command->mStringValue);
        break;
    }
    case CommandBundle::DAEMON_EXIT:
        exit(0);
    case CommandBundle::FIND_PROXY_CLIENT: {
        LOG_RJJH("Proxy get the finder, back 999");
        Json::Value json;
        json["uid"] = Json::Value(getuid());
        json["config"] = Json::Value(mAppConfig->JsonString());

        Json::FastWriter writer;
        string ret = writer.write(json);

        *result = new char[ret.size() + 1];
        *outlen = ret.size();
        strcpy((char*)(*result), ret.c_str());

        return 0;
    }
    case CommandBundle::RELOAD_CONFIG: {
        LOG_RJJH("store configfile %s", command->mStringValue.c_str());
        mAppConfig->UpdateConfig(command->mStringValue);
        if (command->mIntegerValue && mAppConfig->GetSubProductGUID() == SECRET_MACHINE_ID) {
            // only the secret machine need RJJH to save the config
            LOG_RJJH("store %s RJJHetc/.RavCfg", mAppConfig->mConfigFile.c_str());
            mAppConfig->StoreConfig();
        }

        mAsyncPool->PostMessage(Message(ASYNC_RELOAD_CONFIG, 0, 0));
        break;
    }
    case CommandBundle::NOTIFY_EVENT:
        return RespondNotify(command->mStringValue);
    case CommandBundle::REMOTE_COMMAND: {
        switch (command->mIntegerValue) {
        case CommandBundle::POLICY_GLOBAL_CHANGED:
        case CommandBundle::POLICY_DEPLOY_CHANGED:
        case CommandBundle::POLICY_LINUX_CHANGED:
        case CommandBundle::POLICY_CENTER_CHANGED: {
            RespondProxyEvent(*command, TO_GJCZ, result, outlen);
            RespondProxyEvent(*command, TO_ZDFY, result, outlen);
            break;
        }
        case CommandBundle::REMOTE_COMMAND_START:
        case CommandBundle::DEFEN_FILE_CHANGED:
        case CommandBundle::DEFEN_PROC_STARTED:
        case CommandBundle::REMOTE_COMMAND_UPDATE:
        case CommandBundle::SCHEDULE_SCAN:
            return RespondProxyEvent(*command, TO_GJCZ, result, outlen);
        case CommandBundle::REMOTE_COMMAND_FINISHED:
        case CommandBundle::REMOTE_COMMAND_SCANLOG:
        case CommandBundle::REMOTE_COMMAND_VIRUSLOG:
        case CommandBundle::REMOTE_UPDATE_VIRUS_SUCCESS:
        case CommandBundle::REMOTE_UPDATE_VIRUS_FAILED:
        case CommandBundle::REMOTE_DEFEN_STATE_REPORT:
        case CommandBundle::REMOTE_COMMAND_CLOUD_POST:
        case CommandBundle::REMOTE_COMMAND_CLOUD_CHECK:
        case CommandBundle::REMOTE_UPDATE_HTTP_DOWNLOAD:
        case CommandBundle::REMOTE_UPDATE_VIRUS_DOWNLAOD:
        case CommandBundle::REMOTE_UPDATE_VIRUS_COMBINE:
        case CommandBundle::DEPLOY_CROSS_SUCCESS:
        case CommandBundle::TRUST_QUARANTINE_LOG:
            return RespondProxyEvent(*command, TO_GLTX, result, outlen);
        case CommandBundle::REMOTE_COMMAND_DEFEN:
            return RespondProxyEvent(*command, TO_ZDFY, result, outlen);
        default:
            break;
        }

        if (CommandBundle::POLICY_CENTER_CHANGED == command->mIntegerValue) {
            // the deploy policy can change the tray icon state
            SetQtTrayIconState(GetTrayIconState());
        }
    }
    default:
        break;
    }

    return 0;
}

int MainEngine::RespondProxyEvent(CommandBundle& bundle, enum NOTIFY_TO who, void **result, int *outlen) {
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
        LOG_RJJH("Proxy received a remote command and resend to %s", toPath.c_str());

        int ret = 0;
        RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle, bundle.mResendOvertime);
        if (0 == pack.Externalize(toPath.c_str(), ret, result, outlen)) {
            return ret;
        }
    }
    return -1;
}

int MainEngine::RespondNotify(const string &data) {
    CommunicateBundle bundle;
    if (!bundle.Internalize(data.c_str(), data.size())) {
        return -1;
    }

    if (bundle.mFlag == CommunicateBundle::ISEEYOU
            && bundle.mType == CommunicateBundle::NOTIFY_EIXT) {
        exit(0);
    }
    else {
#ifdef WITH_QT
        show_notify(&bundle);
        return bundle.mSelected;
#else
        if (mNotifyFunc) {
            mNotifyFunc(&bundle);
            return bundle.mSelected;
        }
#endif
    }

    return 0;
}

void MainEngine::HandleMessage(int event, int type, void *object) {
    if (ASYNC_RELOAD_CONFIG == event) {
        // send the notify to main screen
        CommandBundle command;
        command.mCommandID = CommandBundle::DAEMON_CENTER_UPDATE;
        command.mIntegerValue = CommandBundle::UPGRADE_VLIB_SUCCESS;
        command.mStringValue = mAppConfig->JsonString();

        char temp[128];
        sprintf(temp, RJJH_MAJOR_SOCKET, getuid());
        std::string sock = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, temp, NULL);

        int ret = 0;
        RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
        if (0 == bundle.Externalize(sock.c_str(), ret)) {

        }
    }
    else if (ASYNC_RESET_CONFIG == event) {
        ResetConfig();
    }
}

int MainEngine::OpenApp(const char *app, const char *command) {
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

int MainEngine::SendToDaemon(int cid) {
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

int MainEngine::KillProc(const char *app, int signal) {
    int state = Utils::OS::CheckProcessState(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, app, NULL).c_str());
    if (state > 0) {
        vector<int> pids;
        if (0 == Utils::OS::GetPid(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, app, NULL).c_str(), pids)) {
            vector<int>::iterator iter;
            for (iter=pids.begin();iter!=pids.end(); ++iter) {
                int status = kill(*iter, signal);
                wait(&status);
            }
        }
    }
    return state;
}

// TODO
//void MainEngine::OnWidgetEvent(int event, int type, void */*data*/) {
//    if (MENU_EVENT == event) {
//        switch (type) {
//        case OPEN_MAIN_APP: {
//            char lockfile[64];
//            sprintf(lockfile, LOCK_RJJH_MAIN, getuid());
//            if (!Utils::OS::GetProcState(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, lockfile, NULL).c_str())) {
//                system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, MAIN_SCREEN_COMMAN, NULL).c_str());
//            }
//            break;
//        }
//        case OPEN_SETTING_APP: {
//            char lockfile[64];
//            sprintf(lockfile, LOCK_RJJH_SETTING, getuid());
//            if (!Utils::OS::GetProcState(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, lockfile, NULL).c_str())) {
//                system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, MAIN_SETTING_COMMAND, NULL).c_str());
//            }
//            break;
//        }
//        case OPEN_LOG_APP: {
//            char lockfile[64];
//            sprintf(lockfile, LOCK_RJJH_LOGS, getuid());
//            if (!Utils::OS::GetProcState(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, lockfile, NULL).c_str())) {
//                system(Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, MAIN_LOGVIEW_COMMAND, NULL).c_str());
//            }
//            break;
//        }
//        case OPEN_UPDATE: {
//            SendToDaemon(CommandBundle::DAEMON_UPDATE);
//            break;
//        }
//        case EXIT_ALL: {
////            KillProc(WATCHDOG_PROGUARD_PROCESS, SIGUSR2);
////            KillProc(DAEMON_PROCESS_ENTRY, SIGUSR2);
////            KillProc(MAIN_SCREEN_ENTRY, SIGKILL);
////            KillProc(MAIN_SETTING_ENTRY, SIGKILL);
////            KillProc(MAIN_LOGVIEW_ENTRY, SIGKILL);

//            SendToDaemon(CommandBundle::DAEMON_EXIT);
//            exit(0);
//        }
//        default:
//            break;
//        }
//    }
//}


void MainEngine::Start() {
    mState = true;

    LOG_RJJH("start the RJJH0 engine");
    mGJCZReceiver->Start();
    mGLTXReceiver->Start();
    mZDFYReceiver->Start();

    LOG_RJJH("start the check ROOT process");
    pthread_create(&mCheckThread, NULL, ThreadWork, this);
}

void MainEngine::Stop() {
    mState = false;
    mGJCZReceiver->Stop();
    mGLTXReceiver->Stop();
    mZDFYReceiver->Stop();
    pthread_join(mCheckThread, NULL);

#ifdef WITH_QT
    unload_widget();
#else
    if (mUnloadFunc) {
        mUnloadFunc();
    }
#endif
}

int MainEngine::SendGroupBroadcast(uint32_t addr, short port, unsigned char *data, int len) {
    struct sockaddr_in sock_addr;
    int fd = 0;
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        return -1;
    }

    memset(&sock_addr,0,sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = addr;
    sock_addr.sin_port = htons(port);

    struct timeval tv_timeout;
    tv_timeout.tv_sec = 1;
    tv_timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

    }
    tv_timeout.tv_sec = 1;
    tv_timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

    }

    if (sendto(fd, data, len, 0, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        return -1;
    }

    close(fd);
    return 0;
}

int MainEngine::NotifyService(uint32_t addr, short port) {
    // notify the daemon notify uid
    CommandBundle command;
    command.mCommandID = CommandBundle::NOTIFY_READY;
    command.mIntegerValue = (int)getuid();
    command.mStringValue = mAppConfig->JsonString();

    char *data = NULL;
    int len = 0;
    if (0 == command.Externalize(&data, &len)) {
        SendGroupBroadcast(addr, port, (unsigned char*)data, len);
        delete[] data;
        return 0;
    }

    return -1;
}

int MainEngine::NotifyService(const char *folder, const char *sock) {
    // notify the daemon notify uid
    CommandBundle command;
    command.mCommandID = CommandBundle::NOTIFY_READY;
    command.mIntegerValue = (int)getuid();
    command.mStringValue = mAppConfig->JsonString();

    LOG_RJJH("start notify the process uid:%d", (int)getuid());
    string sockpath = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), folder, sock, NULL);
    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sockpath.c_str(), ret)) {
        return ret;
    }
    return -1;
}

#ifndef WITH_QT
int MainEngine::LoadQtWidgetLib(bool traystate) {
    string qtlib = Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_LIB_PATH, QT_WIDGET_SUPPORT_LIB, NULL);
    if (!Utils::Path::FileAvailed(qtlib)) {
        return -1;
    }

    mDynamicHandler = dlopen(qtlib.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (NULL == mDynamicHandler) {
        LOG_RJJH("%s\n", dlerror());
        return -1;
    }

    qt_creator construct;
    construct = (qt_creator)dlsym(mDynamicHandler, "initialize_widget");
    ASSERT_ZERO(construct, -1);

    mNotifyFunc = (qt_toast)dlsym(mDynamicHandler, "show_notify");
    ASSERT_ZERO(mNotifyFunc, -1);

    mSetTrayStateFunc = (qt_tray)dlsym(mDynamicHandler, "set_tray_icon");
    ASSERT_ZERO(mSetTrayStateFunc, -1);

    mUnloadFunc = (qt_unload)dlsym(mDynamicHandler, "unload_widget");
    ASSERT_ZERO(mUnloadFunc, -1);

    construct(traystate, mAppConfig);

    return 0;
}

int MainEngine::UnloadQtWidget() {
    if (mSetTrayStateFunc) {
        mSetTrayStateFunc(false);        
    }
    if (mUnloadFunc) {
        mUnloadFunc();
    }
    if (mDynamicHandler) {
        dlclose(mDynamicHandler);
        mDynamicHandler = NULL;
    }

    mSetTrayStateFunc = NULL;
    mUnloadFunc = NULL;
    mNotifyFunc = NULL;

    return 0;
}
#endif

int MainEngine::SetQtTrayIconState(bool state) {
#ifdef WITH_QT
    set_tray_icon(state);
#else
    if (mSetTrayStateFunc) {
        mSetTrayStateFunc(state);
    }
#endif
    return 0;
}

int MainEngine::ShowNotifiy(CommunicateBundle *bundle) {
#ifdef WITH_QT
    show_notify(bundle);
    return 0;
#else
    if (mNotifyFunc) {
        LOG_RJJH("start show the popup");
        mNotifyFunc(bundle);
        LOG_RJJH("end show the popup");
        return 0;
    }
#endif
    return -1;
}

void* MainEngine::ThreadWork(void *params) {
    MainEngine *self = (MainEngine*)params;
    self->NotifyThreadFunc();
    return NULL;
}

void MainEngine::NotifyThreadFunc() {
    LOG_RJJH("start notify the ROOT process");
    while (mState) {
        sleep(1);

        LOG_RJJH("Notify to GJCZ.");
        if (NotifyService(FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET)) {
            LOG_RJJH("send the GJCZ error");
        }

        sleep(1);
        LOG_RJJH("Notify to GLTX.");
        if (NotifyService(FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET)) {
            LOG_RJJH("send the GLTX error");
        }

        sleep(1);
        LOG_RJJH("Notify to ZDFY.");
        if (NotifyService(FOLDER_RJJH_ZDFY, ZDFY_RJJH_SOCKET)) {
            LOG_RJJH("send the ZDFY error");
        }
        sleep(5);
    }
}

int MainEngine::ResetConfig() {
    CommandBundle command;
    command.mCommandID = CommandBundle::RESET_CONFIG;

    int ret = 0;
    RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, command);
    // notify the GJCZ load scan params
    pack.Externalize(Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL).c_str(), ret);
    // notify the ZDFY load file monitor
    pack.Externalize(Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, ZDFY_RJJH_SOCKET, NULL).c_str(), ret);
    // notify the GLTX load file monitor
    pack.Externalize(Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL).c_str(), ret);

    return ret;
}

#ifdef WITH_QT
int MainEngine::RepairConfig() {
    StorageBridge *storage = StorageBridge::NEW(mAppConfig->GetInstallPath(), FOLDER_RJJH_GJCZ);
    if (storage) {
        string dbver = storage->GetVlibVersion();
        if (Utils::String::CompareVersion(dbver, mAppConfig->GetVlibVersion()) > 0) {
            mAppConfig->SetVlibVersion(dbver);
        }
    }
    SECURITY_FREE(storage);
    return mAppConfig->StoreConfig();
}
#endif
