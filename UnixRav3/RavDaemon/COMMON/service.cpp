#include "service.h"
#include "baseconfig.h"
#include "appconfig.h"
#include "commonlocker.h"
#include "policysyncparser.h"
#include "models/policy/policymodel.h"
#include "storage.h"

#include <dirent.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

ServiceBinder::ServiceBinder() {
    mCenterListener = NULL;
    mAsyncPool = NULL;
    mAppConfigLocker = NULL;
    mAppConfig = NULL;

    mNotifierUID = -1;
    mStatus = 0;
    mNeedRJJHSaveConfig = false;

    mHelperThread = 0;
    mConstructStep = FIRST_FIND_PROXY;
}

ServiceBinder::~ServiceBinder() {
    if (mStatus) {
        Stop();
    }

    if (mCenterListener) {
        delete mCenterListener;
    }
    if (mAsyncPool) {
        delete mAsyncPool;
    }

    if (mAppConfig) {
        delete mAppConfig;
    }

    if (mAppConfigLocker) {
        delete mAppConfigLocker;
    }
}

int ServiceBinder::Construct(const string& folder, const string& sock) {
    mAppConfigLocker = MutexLocker::NEW();
    mAsyncPool = Handler::NEW(this);

    mOwnerFolder = folder;
    mListenSock = sock;

    mStatus = 1;

    string install_path;
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    if (Utils::Path::FileAvailed(config)) {
        mAppConfig = AppConfig::NEW(config);
        ASSERT_ZERO(mAppConfig, -1);

        install_path = mAppConfig->GetInstallPath();
    }
    else {
        mAppConfig = new AppConfig;
        install_path = RAV_DEFAULT_PATH;
    }

    //
    Utils::LOG::InitLog(install_path.c_str());

    if (Utils::OS::GetProcState(Utils::Path::JoinPath(install_path.c_str(), mOwnerFolder.c_str(), NULL),true)) {
        // check the lock file, check the owner
        return -1;
    }

    std::string sockPath = Utils::Path::JoinPath(install_path.c_str(), mOwnerFolder.c_str(), mListenSock.c_str(), NULL);
    mCenterListener = CenterListener::NEW(this, CenterCommandObserver::CLIENT_COMMAND, sockPath.c_str(), true);
    ASSERT_ZERO(mCenterListener, -1);
    if (mCenterListener->Start()) {
        return -1;
    }

    // the process started, then locked
    FILE *f = Utils::OS::LockProcState(
                Utils::Path::JoinPath(install_path.c_str(), mOwnerFolder.c_str(), NULL).c_str());


    return 0;
}

std::string ServiceBinder::GetAppVersion() const {
    return mAppConfig->GetAppVersion();
}

std::string ServiceBinder::GetVirusVersion() const {
    string version;
    version  = mAppConfig->GetVlibVersion();
    if (version.empty()) {
        GetStorage()->GetSettingValue(VLIB_UPDATE_VERSION, version);
    }
    return version;
}

void ServiceBinder::SetVirusVersion(const string& version) {
    mAppConfig->SetVlibVersion(version);
    GetStorage()->SetSettingValue(VLIB_UPDATE_VERSION, version);
}

int ServiceBinder::RepairConfig() {
    // check the virus lib version
    string dbver;
    GetStorage()->GetSettingValue(VLIB_UPDATE_VERSION, dbver);

    if (Utils::String::CompareVersion(dbver, mAppConfig->GetVlibVersion()) > 0) {
        mAppConfig->SetVlibVersion(dbver);
    }
    return mAppConfig->StoreConfig();
}

void ServiceBinder::HandleMessage(int event, int type, void *object) {

}

int ServiceBinder::OnCommandObserver(int event, void *args, void **/*result*/, int */*outlen*/) {
    if (CLIENT_COMMAND == event) {
        CommandBundle *bundle = (CommandBundle*)args;
        if (CommandBundle::NOTIFY_READY == bundle->mCommandID && !mNeedRJJHSaveConfig) {
            mAppConfigLocker->Lock();
            {
                mNotifierUID = bundle->mIntegerValue;
                mAppConfig->UpdateConfig(bundle->mStringValue);
            }
            mAppConfigLocker->Unlock();

            if (mNotifierUID>=0 && !mAppConfig->GetInstallPath().empty() && FIRST_FIND_PROXY==mConstructStep) {
                if (0 == DelayConstruct()) {
                    mConstructStep = SECOND_CONSTRUCT;
                    Start();
                }
            }
        }
        else if (CommandBundle::DAEMON_EXIT == bundle->mCommandID) {
            exit(0);
        }
        else if (CommandBundle::RESET_CONFIG == bundle->mCommandID) {
            ResetConfig(GetStorage());
        }
    }
    else if (CLINET_GROUP_BROADCAST == event) {
        CommandBundle *bundle = (CommandBundle*)args;
        if (CommandBundle::NOTIFY_READY == bundle->mCommandID) {
            mAppConfigLocker->Lock();
            {
                mNotifierUID = bundle->mIntegerValue;
                mAppConfig->UpdateConfig(bundle->mStringValue);
            }
            mAppConfigLocker->Unlock();
        }
    }
    return 0;
}

int ServiceBinder::NotifyData(const std::string& install, const std::string& folder, const std::string& sock, CommandBundle& command,  bool system, int uid) {
    char temp[128];
    if (system) {
        strncpy(temp, sock.c_str(), 128);
    }
    else {
        sprintf(temp, sock.c_str(), uid);
    }

    //printf("NotifyData:%s\n", Utils::Path::JoinPath(install.c_str(), folder.c_str(), temp, NULL).c_str());
    RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, command, 1000*1000);
    string result;
    int err = 0;
    int retry = 0;
    do {
        // 在此做一个循环十次的通知，一定要通知到，否则会影响后续操作
        err = pack.Externalize(
              Utils::Path::JoinPath(install.c_str(), folder.c_str(), temp, NULL).c_str(),
              result);
        if (err) {
            printf("NotifyData:%s/%s, %s error!!\n", install.c_str(), folder.c_str(), temp);
            usleep(10000);
        }
    }while(err && retry++ < 3);

    if (0 == err) {
        command.mStringValue = result;
        return 0;
    }
    return err;
}

int ServiceBinder::NotifyData(const string& folder, const string& sock, CommandBundle& data) {
    return NotifyData(mAppConfig->GetInstallPath(), folder, sock, data, false, mNotifierUID);
}

int ServiceBinder::SendToNotifer(CommunicateBundle::CommunicateType flag,
                              CommunicateBundle::NotifyType type,
                              const std::string& title,
                              const std::string& content,
                              const std::string& left,
                              const std::string& right,
                              int overtime) {

    CommunicateBundle bundle;
    bundle.mFlag = (CommunicateBundle::CommunicateType)flag;
    bundle.mType = type;
    bundle.mTitle = GetTitle() + "_" + title;
    bundle.mContent = content;
    bundle.mButtonTitle1 = left;
    bundle.mButtonTitle2 = right;

#if defined(COMM_WITH_DBUS)
    char *buf = NULL;
    int len = 0;
    if (0 == bundle.Externalize(&buf, &len)) {
        int ret = CenterListener::SendDBusMethod(NOTIFY_DBUS_NAME, Application::NOTIFY_TRAY_COMPONENT, (const char*)buf, true);
        SECURITY_FREE(buf);
        return ret;
    }
#elif defined(COMM_WITH_SOCKET)
    if (mNotifierUID >= 0) {
        char sockname[128];
        sprintf(sockname, RJJH_NOTIFY_SOCKET, mNotifierUID);

        char *buf = NULL;
        int len = 0;
        if (bundle.Externalize(&buf, &len)) {
            return -1;
        }

        CommandBundle command;
        command.mCommandID = CommandBundle::NOTIFY_EVENT;
        command.mIntegerValue = 0;
        if (buf && len) {
            command.mStringValue = buf;
            delete[] buf;
        }

        int ret = 0;
        RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, command, overtime);
        if (0 == pack.Externalize(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), mOwnerFolder.c_str(), sockname, NULL).c_str(), ret)) {
            return ret;
        }
    }
#endif
    return -1;
}

int ServiceBinder::ResetConfig(Storage *storage) {
    string defaultConfig = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), mOwnerFolder.c_str(), CONFIG_DEF_FILE, NULL);
    if (Utils::Path::FileAvailed(defaultConfig) && storage) {
        //printf("ResetConfig\n");
        storage->InitDefaultValue(defaultConfig);
    }

    InitializeGeneralPolicy(storage);
    return 0;
}

int ServiceBinder::InitializeGeneralPolicy(Storage *storage) {
    struct dirent *tmp = NULL;
    string path = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), mOwnerFolder.c_str(), CONFIG_DEF_POLICY, NULL);
    DIR *dir = opendir(path.c_str());
    if (!dir) {
        return -1;
    }
    while (NULL != (tmp = readdir(dir))) {
        bool isfile = false;
        char tempPath[1024] = {0};
#if defined (COMMON_LINUX)
        isfile = DT_REG==tmp->d_type;
        if (!isfile && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", path.c_str(), tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isfile = bool(buf.st_mode & S_IFREG);
        }

        if (isfile) {
            string file = Utils::Path::JoinPath(path.c_str(), tmp->d_name, NULL);

            int policy_type = 0;
            vector<string> filenames;
            vector<string> policies;
            Utils::String::Split(tmp->d_name, ".", &filenames);
            if (filenames.size() == 2) {                
                Utils::String::Split(filenames[0], "_", &policies);
                if (policies.size() == 2) {
                    policy_type = atoi(policies[1].c_str());
                }
            }

            std::ifstream stream(file.c_str());
            std::stringstream buffer;
            buffer << stream.rdbuf();
            std::string contents(buffer.str());

            if (policies[0] == string(ANTIVIRUS_PRODUCTID)&& policy_type==1) {
                GlobalAntiVirusPolicy globalPolicy;
                if (0 == PolicyParser::ParseGlobalPolicy(contents.c_str(), globalPolicy)) {
                    globalPolicy.SyncStorage(storage);
                }
            }
            else if (policies[0]==string(DEPLOYED_PRODUCTID) && policy_type==1) {
                ConfigPolicy configPolicy;
                int err = PolicyParser::ParseConfigPolicy(contents.c_str(), configPolicy);
                if (0 == err) {
                    configPolicy.SyncStorage(storage);
                }
            }
            else if (policies[0]==string(NETCENTER_PRODUCTID) && policy_type==1) {
                EnvironmentalPolicy envPolicy;
                if (0 == PolicyParser::ParseEnvrionPolicy(contents.c_str(), envPolicy)) {
                   envPolicy.SyncStorage(storage);
                }
            }
            else if (policies[0]==string(RFW_PRODUCTID) && policy_type == 2) {
                FirewallPolicy firewallPolicy;
                if (0 == PolicyParser::ParseFirewallPolicy(contents.c_str(), firewallPolicy)) {
                    firewallPolicy.SyncStorage(storage);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}


void* ServiceBinder::ProcessHelperThread(void *params) {
#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "Helper");
    pthread_setname_np(pthread_self(), threadName);
#endif
    ServiceBinder* self = (ServiceBinder*)params;
    self->ProcessHelperWork();
    return NULL;
}

void ServiceBinder::ProcessHelperWork() {
    // check process
    const char *process[] = { DAEMON_GJCZ_ENTRY, DAEMON_GLTX_ENTRY, DAEMON_ZDFY_ENTRY };
    const char *command[] = { DAEMON_GJCZ_COMMAND, DAEMON_GLTX_COMMAND, DAEMON_ZDFY_COMMAND };

    while(mStatus) {
        for (int i=0; i<sizeof (process)/sizeof(const char*) && mStatus; ++i) {
            string proc = Utils::Path::JoinPath(
                        mAppConfig->GetInstallPath().c_str(),
                        DEPLOYED_BIN_FOLDER,
                        process[i],
                        NULL);

            string mine = Utils::OS::GetProcessName();
            if (mine == proc) {
                continue;
            }
            if (0 == Utils::OS::CheckProcessState(proc.c_str())) {
                string start = Utils::Path::JoinPath(
                            mAppConfig->GetInstallPath().c_str(),
                            DEPLOYED_BIN_FOLDER,
                            command[i],
                            NULL);
                system(start.c_str());
            }
            sleep(3);
        }
        sleep(COMMUNICATE_HEART_INTERVAL * 60);
    }
}

int ServiceBinder::DelayConstruct() {
    if (mAppConfig->GetSubProductGUID() != SECRET_MACHINE_ID
            && mAppConfig->WatchGuard()) {
        pthread_create(&mHelperThread, NULL, ProcessHelperThread, this);
    }

    return 0;
}

int ServiceBinder::Start() {
    return 0;
}

int ServiceBinder::Stop() {
    mStatus = 0;
    if (mCenterListener) {
        mCenterListener->Stop();
    }

    if (mHelperThread > 0) {
        pthread_cancel(mHelperThread);
        pthread_join(mHelperThread, NULL);
    }
    return 0;
}

int ServiceBinder::ProcessLinuxPolicy(PolicyModel *model, Storage *storage, int type) {
    return 0;
}

int ServiceBinder::ProcessGlobalPolicy(PolicyModel *model, Storage *storage, int type) {
    if (1 == type) {
        GlobalAntiVirusPolicy globalPolicy;
        int err = PolicyParser::ParseGlobalPolicy(model->GetPolicyContent().c_str(), globalPolicy);
        if (0 == err) {
            return globalPolicy.SyncStorage(storage);
        }
    }

    return -1;
}

int ServiceBinder::ProcessCenterPolicy(PolicyModel *model, Storage *storage, int type) {
    if (1 == type) {
        EnvironmentalPolicy envPolicy;
        if (0 == PolicyParser::ParseEnvrionPolicy(model->GetPolicyContent().c_str(), envPolicy)) {
            return envPolicy.SyncStorage(storage);
        }
    }

    return -1;
}

int ServiceBinder::ProcessDeployPolicy(PolicyModel *model, Storage *storage, int type) {
    if (1 == type) {
        ConfigPolicy configPolicy;
        int err = PolicyParser::ParseConfigPolicy(model->GetPolicyContent().c_str(), configPolicy);
        if (0 == err) {
            return configPolicy.SyncStorage(storage);
        }
    }

    return -1;
}

int ServiceBinder::ProcessFirewallPolicy(PolicyModel *model, Storage *storage, int type) {
    if (2 == type) {
        FirewallPolicy configPolicy;
        int err = PolicyParser::ParseFirewallPolicy(model->GetPolicyContent().c_str(), configPolicy);
        if (0 == err) {
            return configPolicy.SyncStorage(storage);
        }
    }
    return -1;
}

bool ServiceBinder::CheckEnvChange() {
    int archbits = Utils::OS::GetFileBits(Utils::OS::GetProcessName().c_str());
    if (archbits == 0) {
        archbits = 64;
    }

    std::vector<std::string> fs;
    Utils::OS::GetLibraryFolder(fs);
    if (fs.empty()) {
        fs.push_back("/usr");
    }

    int env = 0;
    // x11 support, libxcb.so, libX11.so
    string xcbpath = Utils::OS::GetSoPath("libxcb.so", archbits, fs);
    string x11path = Utils::OS::GetSoPath("libX11.so", archbits, fs);
    if (!xcbpath.empty() && !x11path.empty()) {
        env |= AppConfig::X11_SUPPORT;
    }

    // file defen plugins/ravdefen_xxxxx.ko
    string koname = string("ravdefen_") + Utils::OS::GetKernelVersionString() + ".ko";
    string defenko = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), PLUGINS_PATH, koname.c_str(), NULL);
    if (Utils::Path::FileAvailed(defenko)) {
        env |= AppConfig::FILE_DEFEN;
    }

    // net filter plugins/ravfilter_xx.ko
    koname = string("ravfilter_") + Utils::OS::GetKernelVersionString() + ".ko";
    defenko = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), PLUGINS_PATH, koname.c_str(), NULL);
    if (Utils::Path::FileAvailed(defenko)) {
        env |= AppConfig::NET_FILTER;
    }

    // udisk check libudev.so
    string udevpath = Utils::OS::GetSoPath("libudev.so", archbits, fs);
    if (!udevpath.empty()) {
        env |= AppConfig::PLUGIN_CHECKER;
    }

    int functions = mAppConfig->GetFunctions();
    if (functions != env) {
        mAppConfig->SetFunctions(env);
        mAppConfig->StoreConfig();
    }

    return functions != env;
}

string ServiceBinder::GetTitle() {
    if (mAppConfig->SecretMachine() || mAppConfig->CenterLess()) {
        return "瑞星ESM防病毒系统";
    }
    else {
        string major, minor;
        Storage *storage = GetStorage();
        if (storage) {
            storage->GetTitle(major, minor);
        }
        if (major.empty()) {
        	switch(mAppConfig->GetProductType()) {
        	case 1: major = "瑞星ESM防病毒终端安全防护系统"; break; 
        	case 2: major = "瑞星ESM防病毒系统"; break;
        	}
        }

        // 202109/27去掉子标题. 
        //if (minor.empty()) {
        //    minor = "下一代网络版---------";
        //}

		if (minor.empty()) return major;
        return major + "-" + minor;
    }
}
