#include "mainengine.h"
#include "storage.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "policysyncparser.h"
#include "appconfig.h"

#include "models/policy/policymodel.h"
#include "models/policy/commandmodel.h"

#include "json/json.h"
#include "rserializer.h"
#include "communicatebundle.h"
#include "Entry/logevententry.h"
#include "Entry/virusinfoentry.h"
#include "Entry/whitelistentry.h"
#include "Entry/urlruleentry.h"
#include "Entry/netaccessentry.h"
#include "Entry/commeventlog.h"
#include "Entry/quarantinelog.h"
#include "Entry/commonentry.h"
#include "scanconfig.h"

#include "cinifile.h"
#include "handler.h"

#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <fstream>
#include <algorithm>
#include <dlfcn.h>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;


#define ASYNC_REPORT_REMOTE_COMMAND      10001
#define ASYNC_REPORT_DEFEN_STATUS        10002



extern int g_running_state;


MainEngine* MainEngine::NEW() {
    MainEngine *self = new MainEngine;
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}


MainEngine::MainEngine() {
    mHealthStatus = HEALTHY;
    mWebServer = NULL;
    mFirefoxListener = NULL;
    mDb_bReady = false;
}

MainEngine::~MainEngine() {
    map<string, KernelModuleData*>::iterator iter;
    for (iter=mModules.begin(); iter!=mModules.end(); ++iter) {
        delete iter->second;
    }
    SECURITY_FREE(mWebServer);
    SECURITY_FREE(mFirefoxListener);
}


int MainEngine::Construct() {
    LOG_ZDFY("consturct the GLTX");
    mDb_bReady = false;
    SECURITY_CHECK(ServiceBinder::Construct(FOLDER_RJJH_ZDFY, ZDFY_RJJH_SOCKET), 0, -1);
    return 0;
}

int MainEngine::DelayConstruct() {
    LOG_ZDFY("into delay construct");
    ServiceBinder::DelayConstruct();

    Utils::Path::CheckPath(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, NULL), 0777);
    SECURITY_CHECK(StorageBridge::Construct(mAppConfig->GetInstallPath(), FOLDER_RJJH_ZDFY), 0, -1);

    mDb_bReady = true;

    LOG_ZDFY("service construct");
    if (mStorage->IsFirstCreate() || 0 == mStorage->GetSettingsCount()) {
        ResetConfig(mStorage);
    }

    // 如果文件监控没有设置开机启动，那么在开机时关掉已开启的文件监控项

    /*
    if (!GetActivePowerOnState()) {
        ActiveDefenConfig config;
        if (0 == GetDefenConfig(config)) {
            if (config.mFileDefenState.mValue) {
                config.mFileDefenState.mValue = 0;
                SaveDefenConfig(config);
            }
        }
    }
    */


    if (mAppConfig->GetSubComponents()&AppConfig::XAV
            && mAppConfig->GetFunctions() & AppConfig::FILE_DEFEN) {
        LOG_ZDFY("start FILE_DEFEN");
        //  computer power on, start the monitor
       // if (GetProcDefenceState() || GetFileDefenceState()) {
        if ( GetFileDefenceState()) {
            StartMonitor(FILE_DEFEN_MODULE);
        }
    } 

    if (mAppConfig->GetSubComponents()&AppConfig::XFW
            && mAppConfig->GetFunctions() & AppConfig::NET_FILTER) {
        LOG_ZDFY("start install filter kernel");
        StartMonitor(NET_FILTER_MODULE);
    }

    GetUrlBlackList(mUrlRuleLists);

    InitBrowserMonitor();
    if (mWebServer) {
        mWebServer->Start();
    }

    mFirefoxListener = CenterListener::NEW(this, CenterCommandObserver::PUBLIC_API_SCAN, "/var/ipc/ravblock", true);
    if (mFirefoxListener) {
        mFirefoxListener->Start();
    }

    return 0;
}

int MainEngine::StartMonitor(const string& module) {
    int err = -1;
    map<string, KernelModuleData*>::iterator iter = mModules.find(module);
    if (iter != mModules.end()) {
        LOG_ZDFY("find the installed mModules, reconnect it");
        err = iter->second->mDefenLoader(this, this, mAppConfig);
    }
    else {
        KernelModuleData *data = new KernelModuleData(mAppConfig->GetInstallPath());
        if (0 == data->Construct(module)) {
            LOG_ZDFY("load  module, connect it");
            mModules.insert(make_pair(module, data));
            err = data->mDefenLoader(this, this, mAppConfig);
        }
    }

    if (err >= 0) {
        if (module == FILE_DEFEN_MODULE) {
            Message msg(ASYNC_REPORT_DEFEN_STATUS, err, 0);
            mAsyncPool->PostMessage(msg);
        }
    }
    return err;
}

int MainEngine::StopMonitor(const string& module) {
    int err = -1;
    map<string, KernelModuleData*>::iterator iter = mModules.find(module);
    if (iter != mModules.end()) {
        err = iter->second->mUnload();
        delete iter->second;
        mModules.erase(iter);
    }

    if (0 == err && module == FILE_DEFEN_MODULE) {
        Message msg(ASYNC_REPORT_DEFEN_STATUS, false, 0);
        mAsyncPool->PostMessage(msg);
    }

    return 0;
}



int MainEngine::Stop() {
    ServiceBinder::Stop();

    map<string, KernelModuleData*>::iterator iter;
    for (iter=mModules.begin(); iter!=mModules.end();) {
        StopMonitor(iter->first);
        mModules.erase(iter++);
    }

    mModules.clear();
    if (mWebServer) {
        mWebServer->Stop();
    }

    return 0;
}

int MainEngine::ReportKoState(const string& flag, bool state) {
    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_DEFEN_STATE_REPORT;

    Json::Value json;
    json["flag"] = Json::Value(flag);
    json["state"] = Json::Value(state);

    Json::FastWriter writer;
    command.mStringValue = writer.write(json);

    // report the ep state
    return NotifyData(mAppConfig->GetInstallPath(), FOLDER_RJJH_ZDFY, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::OnCommandObserver(int event, void *args, void **result, int *outlen) {
    //LOG_ZDFY("get the brower request");
    //printf("get the brower request event:%d\n", event);

    int err = ServiceBinder::OnCommandObserver(event, args, result, outlen);
    if (CLIENT_COMMAND == event) {
        CommandBundle *bundle = (CommandBundle*)args;

        if (bundle) {
            return RespondCientCommand(*bundle);
        }
    }
    else if (PUBLIC_API_SCAN == event) {
        CommandBundle *bundle = (CommandBundle*)args;
        LOG_ZDFY("get the brower request %s", bundle->mStringValue.c_str());
    }
    return err;
}

int MainEngine::ReloadConfig() {

    //waiting for the db init ready
    if(!mDb_bReady)
        sleep(3);
    //printf("++++++++++++++++ReloadConfig+\n");
    // file defen state
    if (mAppConfig->GetSubComponents()&AppConfig::XAV
            && mAppConfig->GetFunctions() & AppConfig::FILE_DEFEN) {
        if (GetProcDefenceState() || GetFileDefenceState()) {

            StopMonitor(FILE_DEFEN_MODULE);
            //sleep(1);
            StartMonitor(FILE_DEFEN_MODULE);
        }
        else {
            StopMonitor(FILE_DEFEN_MODULE);
        }
    }

    // net filter state
    if (mAppConfig->GetSubComponents()&AppConfig::XFW
                && mAppConfig->GetFunctions() & AppConfig::NET_FILTER) {
        if (true/*GetNetFilterState()*/) {
            StartMonitor(NET_FILTER_MODULE);
        }
        else {
            StopMonitor(NET_FILTER_MODULE);
        }
    }

    mUrlRuleLists.clear();
    GetUrlBlackList(mUrlRuleLists);

    return 0;
}

int MainEngine::RespondCientCommand(const CommandBundle &bundle) {
    switch (bundle.mCommandID) {
    case CommandBundle::QUERY_HEALTH:
        return mHealthStatus;
    case CommandBundle::RELOAD_CONFIG:
        return ReloadConfig();
    case CommandBundle::DAEMON_EXIT: {
        g_running_state = 0;
        exit(0);
    }
    case CommandBundle::REMOTE_COMMAND: {
        return RespondCenterCommand(
                    (enum CommandBundle::CommandExtendType)bundle.mIntegerValue,
                    bundle.mStringValue
        );
    }
    case CommandBundle::BROWSER_BLOCK: {
        //
        LOG_ZDFY("get url block %s", bundle.mStringValue.c_str());
        list<UrlRuleEntry>::iterator iter;
        for (iter=mUrlRuleLists.begin();iter!=mUrlRuleLists.end();++iter) {
            if (bundle.mStringValue == iter->mPath) {
                return 1;
            }
        }
        return -1;
    }
    default:
        break;
    }
    return 0;
}


int MainEngine::RespondCenterCommand(CommandBundle::CommandExtendType type, const string &data) {
    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(data, json)) {
        return -1;
    }

    int ret = -1;
    switch (type) {
    case CommandBundle::POLICY_GLOBAL_CHANGED: {
        PolicyModel *model = PolicyModel::NEW(json);
        ret = ProcessGlobalPolicy(model, mStorage, model->mPolicyType);
        SECURITY_FREE(model);

        //
        printf("ReloadMonitorList\n");
        ReloadMonitorList();
        break;
    }
    case CommandBundle::POLICY_DEPLOY_CHANGED: {
        PolicyModel *model = PolicyModel::NEW(json);
        ret = ProcessDeployPolicy(model, mStorage, model->mPolicyType);
        SECURITY_FREE(model);
        break;
    }
    case CommandBundle::POLICY_CENTER_CHANGED: {
        PolicyModel *model = PolicyModel::NEW(json);
        ret = ProcessCenterPolicy(model, mStorage, model->mPolicyType);
        SECURITY_FREE(model);
        break;
    }
    case CommandBundle::POLICY_FIREWALL_CHANGED: {
        PolicyModel *model = PolicyModel::NEW(json);
        ret = ProcessFirewallPolicy(model, mStorage, model->mPolicyType);
        SECURITY_FREE(model);
        break;
    }
    case CommandBundle::REMOTE_COMMAND_DEFEN: {
        CommandModel *model = CommandModel::NEW(json, true);
        ret = ProcessCommand(model);
        SECURITY_FREE(model);
        break;
    }
    default:
        break;
    }
    return ret;
}

int MainEngine::ProcessCommand(CommandModel *model) {
    printf("ProcessCommand:0x%x\n", model->mCmdId);

    switch (model->mCmdId) {
    case CommandModel::FILE_DEFEN_OPEN: {
        if (0 == StartMonitor(FILE_DEFEN_MODULE)) {

        }
        mTempBuffer = model->mId;
        Message msg(ASYNC_REPORT_REMOTE_COMMAND,0,0);
        msg.arg3 = reinterpret_cast<void*>(&mTempBuffer);
        mAsyncPool->PostMessage(msg);
        break;
    }
    case CommandModel::FILE_DEFEN_STOP: {
        if (0 == StopMonitor(FILE_DEFEN_MODULE)) {

        }
        mTempBuffer = model->mId;
        Message msg(ASYNC_REPORT_REMOTE_COMMAND,0,0);
        msg.arg3 = reinterpret_cast<void*>(&mTempBuffer);
        mAsyncPool->PostMessage(msg);
        break;
    }
    }
    return 0;
}


int MainEngine::SendToNotifer(CommunicateBundle::CommunicateType flag,
                              CommunicateBundle::NotifyType type,
                              const std::string& title,
                              const std::string& content,
                              const std::string& left,
                              const std::string& right) {

    CommunicateBundle bundle;
    bundle.mFlag = (CommunicateBundle::CommunicateType)flag;
    bundle.mType = type;
    bundle.mTitle = title;
    bundle.mContent = content;
    bundle.mButtonTitle1 = left;
    bundle.mButtonTitle2 = right;

#if defined(COMM_WITH_DBUS)
    unsigned char *buf = NULL;
    int len = 0;
    if (0 == bundle.Externalize(&buf, &len)) {
        int ret = CenterListener::SendDBusMethod(NOTIFY_DBUS_NAME, Application::NOTIFY_TRAY_COMPONENT, (const char*)buf, true);
        SECURITY_FREE(buf);
        return ret;
    }
#elif defined(COMM_WITH_SOCKET)
    RStreamPack<CommunicateBundle> pack(RStreamPack<CommunicateBundle>::STRING, bundle);

    if (mNotifierUID > 0) {
        char sockname[128];
        snprintf(sockname, 127, RJJH_NOTIFY_SOCKET, mNotifierUID);

        int ret = 0;
        if (0 == pack.Externalize(Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_ZDFY, sockname, NULL).c_str(), ret)) {
            return ret;
        }
        return -1;
    }
#endif
    return -1;
}

int MainEngine::ReloadMonitorList() {
    map<string, KernelModuleData*>::iterator iter;
    for (iter=mModules.begin(); iter!=mModules.end(); ++iter) {
        iter->second->mRefresh();
    }

    return 0;
}

void MainEngine::HandleMessage(int event, int type, void *object) {
    switch (event) {
    case ASYNC_REPORT_REMOTE_COMMAND: {
        string* data = (string*)object;
        if (!data->empty()) {
            FinishRemoteCommand(*data);
        }
        break;
    }
    case ASYNC_REPORT_DEFEN_STATUS: {
        ReportKoState(FILE_DEFEN_MODULE, type);
        break;
    }
    default:
        break;
    }
}

int MainEngine::FinishRemoteCommand(const string& command_id) {
    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_COMMAND_FINISHED;
    command.mStringValue = command_id;

    return NotifyData(mInstallDir, FOLDER_RJJH_ZDFY, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

MainEngine::KernelModuleData::KernelModuleData(const string& installdir) {
    mDefenProxyHandler = NULL;
    mUnload = NULL;
    mRefresh = NULL;
    mDefenLoader = NULL;
    mInstallDir = installdir;
}

MainEngine::KernelModuleData::~KernelModuleData() {
   if (mDefenProxyHandler) {
       dlclose(mDefenProxyHandler);
       mDefenProxyHandler = NULL;
       mUnload = NULL;
       mRefresh = NULL;
       mDefenLoader = NULL;
   }
}

int MainEngine::KernelModuleData::Construct(const string& module) {

    char name[64] = {0};
    string fanotify_exec=Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, FANOTIFYEXEC_BIN, NULL);
    if (!Utils::Path::FileAvailed(fanotify_exec)) {
        LOG_ZDFY("%s is not availed", fanotify_exec.c_str());
        return -1;
    }

    string sRet =  Utils::OS::GetCmdResult(fanotify_exec.c_str());
    if(strcmp(sRet.c_str(), "Success\n") == 0){
        sprintf(name, "lib%sbinder_fanotify.so", module.c_str());
    }else if(strcmp(sRet.c_str(), "Failue\n") == 0){
        LOG_ZDFY("load kernel defenmod error!");
        return -1;
    }
    else{
        sprintf(name, "lib%sbinder_inotify.so", module.c_str());
    }

    LOG_ZDFY("start load kernel binder %s", name);

    string so = Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_LIB_PATH, name, NULL);
    if (!Utils::Path::FileAvailed(so)) {
        return -1;
    }

    LOG_ZDFY("find the binder function");
    if (NULL == mDefenProxyHandler) {
        mDefenProxyHandler = dlopen(so.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (NULL == mDefenProxyHandler) {
            char *temp = dlerror();
            LOG_ZDFY("%s\n", temp);
            return -1;
        }

        mDefenLoader = (loader_pointer)dlsym(mDefenProxyHandler, "load_defen_module");
        ASSERT_ZERO(mDefenLoader, -1);
        mUnload = (proxy_pointer)dlsym(mDefenProxyHandler, "unload_defen_module");
        ASSERT_ZERO(mUnload, -1);
        mRefresh = (proxy_pointer)dlsym(mDefenProxyHandler, "refresh_defen_module");
        ASSERT_ZERO(mRefresh, -1);
    }
    return 0;
}

int MainEngine::InitBrowserMonitor() {
    if (/*GetBrowserBlockState()*/1) {
        if (mWebServer) {
            delete mWebServer;
        }
        std::string path = Utils::Path::JoinPath(mInstallDir.c_str(), BROWSER_WEB_DOMAIN, NULL);
        Utils::Path::mkdirs(path);
        mWebServer = WebService::Server::NEW(path.c_str(), BROWSER_IPC_PORT, this);
        if (NULL != mWebServer) {
            return 0;
        }
    }

    return -1;
}

int MainEngine::OnWebRequestObserver(int event, const char *header, const char */*body*/) {
    if (GET == event) {
        // browser block, record it
        NetAccessEntry entry;
        entry.mTime = Utils::Date::GetTimeStamp();
        entry.mUrl = header;
        entry.mProcess = "firefox";
        entry.mType = NetAccessEntry::URL;
        entry.mState = NetAccessEntry::REJECT;
        entry.mSource = NetAccessEntry::BROWSER;

        AppendNetAccessLog(entry);
    }
    return 0;
}

