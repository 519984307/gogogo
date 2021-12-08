#include "application.h"
#include "cinifile.h"
#include "baseconfig.h"
#include "rserializer.h"
#include "scanconfig.h"
#include "communicatebundle.h"
#include "centerlistener.h"
#include "appconfig.h"
#include "storage_global.h"
#include "Entry/commonentry.h"
#include "Entry/whitelistentry.h"
#include "Entry/urlruleentry.h"
#include "Entry/quarantinelog.h"
#include "multivirusengine.h"
#include "json/json.h"
#include "handler.h"

#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <sys/utsname.h>
#include <sys/stat.h>

#define LOAD_SETTING_DEFAULT(x, y, z) if ((x=y) < 0) x=z;

#ifdef SECURITY_FREE
#undef SECURITY_FREE
#endif

#define VERIFY_SCAN_EVENT   50001
#define SCAN_ERROR_EVENT    50002
#define CHECK_REGISTED_EVENT 50003


#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}

Application::~Application() {
    if (mCenterListener) {
        mCenterListener->Stop();
    }
    SECURITY_FREE(mCenterListener);
    SECURITY_FREE(mScanLogs);
    SECURITY_FREE(mHandler);
    Utils::OS::ReleaseProcState(mLockFd);

    map<enum StorageType, StorageBridge*>::iterator iter;
    for (iter=mStorageBridges.begin(); iter != mStorageBridges.end(); ++iter) {
        delete iter->second;
    }
}

int Application::QuickScan(bool feedback) {
    mScanEventLastStamp = 0;

    SECURITY_FREE(mScanLogs);
    mScanLogs = CircleLink<string>::NEW(10);

#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::QUICK_SCAN, "quick scan", true);
#elif defined(COMM_WITH_SOCKET)
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::QUICK_SCAN;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        if (feedback) {
            mScanStatus = APP_SCAN_PREPARE;
            mHandler->PostMessage(Message(VERIFY_SCAN_EVENT, CommandBundle::QUICK_SCAN, 0));
        }
        return ret;
    }
    return -1;
#endif
}

int Application::FullScan(bool feedback) {
    mScanEventLastStamp = 0;
    SECURITY_FREE(mScanLogs);
    mScanLogs = CircleLink<string>::NEW(10);

#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::FULL_SCAN, "full scan", true);
#elif defined(COMM_WITH_SOCKET)
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::FULL_SCAN;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        if (feedback) {
            mScanStatus = APP_SCAN_PREPARE;
            mHandler->PostMessage(Message(VERIFY_SCAN_EVENT, CommandBundle::FULL_SCAN, 0));
        }
        return ret;
    }
    return -1;
#endif
}

int Application::StartScan(std::list<pair<std::string,int> >& path, bool feedback) {
    mScanEventLastStamp = 0;

    SECURITY_FREE(mScanLogs);
    mScanLogs = CircleLink<string>::NEW(10);

    Json::Value json;
    std::list<pair<string, int> >::iterator iter;
    for (iter=path.begin(); iter!=path.end(); ++iter) {
        Json::Value value;
        value["p"] = Json::Value(iter->first);
        value["d"] = Json::Value(iter->second);
        json.append(value);
    }

    Json::FastWriter writer;
    string result = writer.write(json);

#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::CUSTOM_SCAN, result.c_str(), true);
#elif defined(COMM_WITH_SOCKET)
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::CUSTOM_SCAN;
    command.mStringValue = result;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command, 0);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        if (feedback) {
            mScanStatus = APP_SCAN_PREPARE;
            mHandler->PostMessage(Message(VERIFY_SCAN_EVENT, CommandBundle::CUSTOM_SCAN, 0));
        }
        return ret;
    }
    return -1;
#endif
}

int Application::PauseScan() {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::PAUSE_SCAN;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}

int Application::Continue() {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::CONTINUE_SCAN;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}

int Application::StopScan() {
    printf("StopScan\n");
#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::STOP_SCAN, "stop", true);
#elif defined(COMM_WITH_SOCKET)
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::STOP_SCAN;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
#endif
}

int Application::Update(bool offline, const string& file) {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);

    string filename = "update.7z";
    string dest = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, "update.7z", NULL);
    Utils::Path::CopyFile(file, dest);
    dest = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, "update.7z", NULL);
    Utils::Path::CopyFile(file, dest);

    CommandBundle command;
    command.mCommandID = offline?CommandBundle::DAEMON_OFFLINE_UPDATE:CommandBundle::DAEMON_CENTER_UPDATE;
    if (offline) {
        command.mIntegerValue = CommandBundle::UPDATE_OFFLINE_VIRLIB;
        command.mStringValue = filename;
    }

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}

int Application::Construct(AppConfig *config) {
    mAppConfig = config;
    if (NULL == mHandler) {
        mHandler = Handler::NEW(this);
    }
    int type = 0;
    switch (mApplicationType) {
    case NORMAL_APP: {
        type = GJCZ|GLTX|ZDFY;
        break;
    }
    case ACTIVE_DEFEN:
    case CONTEXT_MENU:
    case REMOTE_COMMAND:
    case PLUGIN_CHECK:
    case SCHEDULE_TASK: {
        type = GJCZ;
        break;
    }
    default:
        break;
    }

    if (type & GJCZ) {
        StorageBridge *storage = StorageBridge::NEW(GetInstallPath(), FOLDER_RJJH_GJCZ);
        if (storage) {
            mStorageBridges.insert(make_pair(GJCZ, storage));
        }
    }
    if (type & GLTX) {
        StorageBridge *storage = StorageBridge::NEW(GetInstallPath(), FOLDER_RJJH_GLTX);
        if (storage) {
            mStorageBridges.insert(make_pair(GLTX, storage));
        }
    }
    if (type & ZDFY) {
        StorageBridge *storage = StorageBridge::NEW(GetInstallPath(), FOLDER_RJJH_ZDFY);
        if (storage) {
            mStorageBridges.insert(make_pair(ZDFY, storage));
        }
    }

    // 初始启动，修改扫描参数，如果判断开启智能扫描模式，则设置智能扫描状态为自动
    {
        StorageBridge *storage = GetStorageBridget(GJCZ);
        if (storage) {
            if (storage->GetScanIntelligentState()) {
                bool locked = false;
                if (storage->GetScanIntelligentMode(locked) != AntiVirusSetting::AntiCommon::AUTO_MODE){
                    storage->SetScanIntelligentMode(AntiVirusSetting::AntiCommon::AUTO_MODE);
                }
            }
        }
    }
    return 0;
}

void Application::RefreshRegisted() {
    if (mAppConfig->GetProductType() == 0) {
        mHandler->PostMessage(Message(CHECK_REGISTED_EVENT, 0, 0));
    }
}

int Application::ResetClientProxy(bool qt_tray) {
    // check the RJJH0 is running
    int state = Utils::OS::CheckProcessState(
                        Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL).c_str()
                    );
    if (state <= 0) {
        string command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFY_RJJH_COMMAND, NULL);
        system(command.c_str());
    }
    else {
        // check the rjjh0's uid
        vector<int> pids;
        if (0 == Utils::OS::GetPid(
                    Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL).c_str(),
                    pids)) {
            uid_t notify_uid = -1;
            if (pids.size() > 0) {
                notify_uid = Utils::OS::GetProcessUID(pids[0]);
            }

            if (notify_uid != getuid() || pids.size()>1) {
                // notify the ep module to reset the tray process
                std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
                CommandBundle command;
                command.mCommandID = CommandBundle::REPAIR;
                command.mIntegerValue = qt_tray?CommandBundle::RESET_TRAY_PROCESS:CommandBundle::RESET_TRAY_PROCESS_SILENCE;
                command.mStringValue = Utils::String::itoa(getuid());

                int ret = 0;
                RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
                if (0 == bundle.Externalize(sock.c_str(), ret)) {

                }
            }

            state = Utils::OS::CheckProcessState(
                                Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL).c_str()
                            );
            if (state <= 0) {
                string command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFY_RJJH_COMMAND, NULL);
                system(command.c_str());
            }

        }
    }

    return 0;
}

int Application::Construct(const string& owner, bool singleton, const char *sockname) {
    mOwnerPath = owner;

    mHandler = Handler::NEW(this);
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    mAppConfig = AppConfig::NEW(config);
    if (NULL == mAppConfig) {
        return -1;
    }

    Utils::LOG::InitLog(mAppConfig->GetInstallPath().c_str());

    if (mAppConfig->GetSubProductGUID() != SECRET_MACHINE_ID) {
        ResetClientProxy(true);
    }

    if (singleton) {
        string folder = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ETC, NULL);
        FILE *lock = Utils::OS::LockProcState(folder.c_str());
        if (NULL == lock) {
            return -1;
        }
    }

    // init the sock listener
    if (sockname) {
        mSockName = sockname;
        InitListener(mOwnerPath.c_str(), mSockName.c_str());
    }

    return Construct(mAppConfig);
}

StorageBridge* Application::GetStorageBridget(enum StorageType type) {
    map<enum StorageType, StorageBridge*>::iterator iter = mStorageBridges.find(type);
    if (iter != mStorageBridges.end()) {
        return iter->second;
    }
    return NULL;
}

std::string Application::GetVersion() {
    return mAppConfig->GetAppVersion();
}

std::string Application::GetUpdateTime() {   
    int date = mAppConfig->GetUpdateTime();
    if (0 == date) {
        date = mAppConfig->GetInstallTime();
    }
    return Utils::Date::FormatTimeStamp(date);
}

std::string Application::GetVirusDbVer() {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        string version = storage->GetVlibVersion();
        if (Utils::String::CompareVersion(version, mAppConfig->GetVlibVersion()) > 0) {
            return version;
        }
    }
    return mAppConfig->GetVlibVersion();
}

std::string Application::GetVirusUpdateTime() {
    long last = 0;
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        last = storage->GetVlibUpdateTime();
    }
    if (last > 0) {
        return Utils::Date::FormatTimeStamp(last);
    }
    return GetUpdateTime();
}

int Application::GetDaemonStatus() {
#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::QUERY_HEALTH, "query", true);
#elif defined(COMM_WITH_SOCKET)
    CommandBundle command;
    command.mCommandID = CommandBundle::QUERY_HEALTH;

    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
    bundle.Externalize(sock.c_str(), ret);
#
#endif
    return -1;
}

int Application::RepairDaemonStatus(int status) {
#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::REPAIR, "repair", true);
#elif defined(COMM_WITH_SOCKET)
    CommandBundle command;
    command.mCommandID = CommandBundle::QUERY_HEALTH;
    command.mIntegerValue = status;

    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
    bundle.Externalize(sock.c_str(), ret);
#
#endif
    return -1;
}

string Application::GetInstallPath() {
    return mAppConfig->GetInstallPath();
}

bool Application::Registed() {
    CommandBundle command;
    command.mCommandID = CommandBundle::CENTERLESS_REGISTER;
    command.mIntegerValue = CommandBundle::CHECK_CLIENT_REGISTER;

    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
    bundle.Externalize(sock.c_str(), ret);
    return ret;
}

int Application::Register(const string &serial) {
    CommandBundle command;
    command.mCommandID = CommandBundle::CENTERLESS_REGISTER;
    command.mIntegerValue = CommandBundle::REGISTER_USE_SERIAL;
    command.mStringValue = serial;

    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
    int ret = -1;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
    bundle.Externalize(sock.c_str(), ret);
    return ret;
}

int Application::ReloadNetfilterConfig() {
    return ReloadConfig(ZDFY);
}

int Application::ReloadConfig(enum StorageType type) {
#if defined(COMM_WITH_DBUS)
    CenterListener::SendDBusMethod(DAEMON_DBUS_NAME, CommandBundle::RELOAD_CONFIG, "reload", true);
#elif defined(COMM_WITH_SOCKET)
    CommandBundle command;
    command.mCommandID = CommandBundle::RELOAD_CONFIG;

    std::string sock;
    switch (type) {
    case GJCZ: {
        sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
        break;
    }
    case GLTX: {
        sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
        break;
    }
    case ZDFY: {
        sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_ZDFY, ZDFY_RJJH_SOCKET, NULL);
        break;
    }
    default:
        break;
    }

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
    bundle.Externalize(sock.c_str(), ret);
#endif
    return -1;
}

int Application::InitListener(const char *owner, const char *sockname) {
    char temp[128];
    snprintf(temp, 128, sockname, getuid());
    std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), owner, temp, NULL);

    SECURITY_FREE(mCenterListener);
    mCenterListener = CenterListener::NEW(this, CenterCommandObserver::CLIENT_COMMAND, sock.c_str(), true);

    if (mCenterListener) {
        return mCenterListener->Start();
    }
    return 0;
}


int Application::RestoreQuarantine(QuarantineLog& log) {
    return RestoreQuarantine(log, true);
}

int Application::RestoreQuarantine(QuarantineLog& log, bool trust) {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::QUARANTINE_EVENT;
    command.mIntegerValue = CommandBundle::RESTORE_QUARANTINE_LOG;


    Json::Value params;
    params["log"] = Json::Value(log.Externlize());
    params["extend"] = Json::Value(trust);

    Json::FastWriter writer;
    command.mStringValue = writer.write(params);

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    bundle.Externalize(sock.c_str(), ret);

    if (-100 == ret && log.mUID == (int)getuid()) {
        // 有可能是出错了, 这个问题只在中标的保密机上出现
        string backupfile = log.mSource + "backup";
        Utils::Path::CopyFile(log.mSource, backupfile);
        unlink(log.mSource.c_str());
        Utils::Path::CopyFile(backupfile, log.mSource);
        unlink(backupfile.c_str());
        chmod(log.mSource.c_str(), log.mPermision);

        StorageBridge *storage = GetStorageBridget(GJCZ);
        if (storage) {
            return storage->DeleteQuaLog(log);
        }
        return 0;
    }

    return ret;
}

int Application::DeleteQuarantine(QuarantineLog& log) {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::QUARANTINE_EVENT;
    command.mIntegerValue = CommandBundle::DELETE_QUARANTINE_LOG;

    Json::Value params;
    params["log"] = Json::Value(log.Externlize());
    params["extend"] = Json::Value(0);

    Json::FastWriter writer;
    command.mStringValue = writer.write(params);

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    bundle.Externalize(sock.c_str(), ret);

    return ret;
}

int Application::RespondScanEvent(const int type, const string& data) {
    switch (type) {
    case CommandBundle::PREPARE_SCAN_SUCCESS: {
        mScanStatus = APP_SCAN_ING;
        RefreshScanStatus(0, SCAN_PREPARE, 0);
        break;
    }
    case CommandBundle::REFRESH_VIRUS_LOGS: {
        Json::Value json;
        Json::Reader reader;
        if (reader.parse(data, json)) {
            string file = Utils::JSON::GetJsonString(json, "file");
            string virus = Utils::JSON::GetJsonString(json, "virus");
            int result = Utils::JSON::GetJsonInt(json, "result");

            RefreshVirusLogs(file, virus, result);

            return 0;
        }

        break;
    }
    case CommandBundle::REFRESH_SCAN_STATUS: {
        mScanStatus = APP_SCAN_ING;
//        if (mScanEventLastStamp == 0) {
//            LOG_RJJH("start the check socket thread");
//            pthread_create(&mCheckThread, NULL, CheckScanSocketThread, this);
//        }
//        mScanEventLastStamp = Utils::Date::GetTimeStamp();

        Json::Value json;
        Json::Reader reader;
        if (reader.parse(data, json)) {
            int scantype = Utils::JSON::GetJsonInt(json, "type");
            int scanstatus = Utils::JSON::GetJsonInt(json, "status");
            int progress = Utils::JSON::GetJsonInt(json, "progress");
            Json::Value summ = json["summary"];
            if (!summ.empty()) {
                if (mReportCache.size() != summ.size()) {
                    mReportCache.clear();
                    for (int i=0; i<(int)summ.size(); ++i) {
                        mReportCache.push_back(ReportCache());
                    }
                }

                int totalcount = 0;
                for (int i=0; i<(int)summ.size(); ++i) {
                    Json::Value s = summ[i];
                    mReportCache[i].mFile = s["file"].asString();
                    mReportCache[i].mCount = s["count"].asInt();
                    mReportCache[i].mVirus = s["virus"].asInt();
                    mReportCache[i].mTag = s["tag"].asInt();

                    totalcount += s["count"].asInt();

                    LOG_RJJH("thread %d count %d file %s\n",
                           mReportCache[i].mTag,
                           mReportCache[i].mCount,
                           mReportCache[i].mFile.c_str());
                }
            }
            RefreshScanStatus(scantype, (SCAN_STATUS_TYPE)scanstatus, progress, &mReportCache);
        }
        break;
    }
    case CommandBundle::QUERY_VIRUS_RESULT: {
        Json::Value json;
        Json::Reader reader;
        if (reader.parse(data, json)) {
            string file = Utils::JSON::GetJsonString(json, "file");
            string virus = Utils::JSON::GetJsonString(json, "virus");
            int event = Utils::JSON::GetJsonInt(json, "event");

            return QueryUserForResult(QUERY_FIND_VIRUS, file, virus);
        }
        break;
    }
    case CommandBundle::SCAN_ALL_FINISHED: {
        mScanStatus = APP_SCAN_FINISHED;
        Json::Value json;
        Json::Reader reader;
        if (reader.parse(data, json)) {
            int scantype = Utils::JSON::GetJsonInt(json, "type");
            int progress = Utils::JSON::GetJsonInt(json, "progress");

            RefreshScanStatus(scantype, SCAN_FINISHED, progress, NULL);
        }
        mCheckState = false;
        break;
    }
    default:
        break;
    }
    return 0;
}

int Application::RespondUpdateEvent(const int type, const string& data) {
    switch (type) {
    case CommandBundle::UPGRADE_DOWNLOAD_RP: {
        RefreshUpdateStatus(UPDATE_DOWNLOAD_RP, data);
        break;
    }
    case CommandBundle::UPGRADE_COMBINE_RP: {
        RefreshUpdateStatus(UPDATE_COMBINE_RP, data);
        break;
    }
    case CommandBundle::UPGRADE_APP_FILE: {
        RefreshUpdateStatus(UPDATE_APP_DOWNLOAD, data);
        break;
    }
    case CommandBundle::UPGRADE_VLIB_SUCCESS: {
        mAppConfig->Reload();
        RefreshUpdateStatus(UPDATE_VLIB_SUCCESS, data);
        break;
    }
    case CommandBundle::UPDATE_OFFLINE_VIRLIB_SUCCESS: {
        if (mAppConfig->SecretMachine()) {
            mAppConfig->Reload();
            RefreshUpdateStatus(UPDATE_VLIB_SUCCESS, data);
        }
        break;
    }
    case CommandBundle::UPDATE_OFFLINE_VIRLIB_ERROR: {
        if (mAppConfig->SecretMachine()) {
            RefreshUpdateStatus(UPDATE_VLIB__FAILED, data);
        }
        break;
    }
    default:
        RefreshUpdateStatus((UPDATE_EVENT)type, data);
    }
    return 0;
}

int Application::OnCommandObserver(int /*event*/, void *args, void **/*result*/, int */*outlen*/) {
    CommandBundle *bundle = (CommandBundle*)args;
    switch(bundle->mCommandID) {
    case CommandBundle::SCAN_EVENT: {
        return RespondScanEvent(bundle->mIntegerValue, bundle->mStringValue);
    }
    case CommandBundle::DAEMON_OFFLINE_UPDATE:
    case CommandBundle::DAEMON_CENTER_UPDATE: {
        return RespondUpdateEvent(bundle->mIntegerValue, bundle->mStringValue);
    }
   	case CommandBundle::UPDATE_FINISH_NOTIFY: {
   	    char desc[256]; sprintf(desc, "Update finished (%d)!!!", bundle->mIntegerValue);
   	    if(bundle->mIntegerValue >= -50 && bundle->mIntegerValue <= -41) RefreshRegisted();
   	    RefreshUpdateStatus(UPDATE_FINISHED, desc);
   	    return 0;
   	}
    default:
        break;
    }

    return 0;
}

int Application::GetDefenCenterConfig(DefenCenterSetting& config) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetDefenCenterConfig(config);
    }
    return -1;
}

int Application::UpdateDefenCenterConfig(DefenCenterSetting& config) {
    int changed = 0;
    LOG_RJJH("UpdateDefenCenterConfig");

    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        DefenCenterSetting tempSett;
        if (0 == GetDefenCenterConfig(tempSett)) {
            changed = tempSett.Changed(config);
        }
        storage->UpdateDefenCenterConfig(config);
    }
    storage = GetStorageBridget(GJCZ);
    if (storage) {
        storage->UpdateDefenCenterConfig(config);
    }
    storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->UpdateDefenCenterConfig(config);
    }

    if (changed&DefenCenterSetting::USB_MONITOR_CHANGED) {
        ReloadConfig(GJCZ);
    }

    if (changed&DefenCenterSetting::FILE_DEFEN_CHANGED) {
        LOG_RJJH("DefenCenterSetting::FILE_DEFEN_CHANGED");
        ReloadConfig(ZDFY);
    }

    return 0;
}

int Application::GetUrlBlackList(std::list<UrlRuleEntry>& white) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetUrlBlackList(white);
    }
    return -1;
}

int Application::AddUrlBlackList(const UrlRuleEntry& entry) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->AddUrlBlackList(entry);
    }
    return -1;
}

int Application::DeleteUrlBlacklist(const UrlRuleEntry& entry) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->DeleteUrlBlacklist(entry);
    }
    return -1;
}

int Application::GetScanLogs(std::list<LogEventEntry>& logs) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetScanLogs(logs);
    }
    return -1;
}

int Application::GetVirusLogs(std::list<VirusInfoEntry>& logs) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetVirusLogs(logs);
    }
    return -1;
}

int Application::GetNetAccessLog(std::list<NetAccessEntry>& logs) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetNetAccessLog(logs);
    }
    return -1;
}

int Application::GetBaseLogs(std::list<CommEventLog>& logs, const CommEventLog& seed) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetBaseLogs(logs, seed);
    }
    return -1;
}

int Application::GetQuaLogs(std::list<QuarantineLog>& logs) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetQuaLogs(logs);
    }
    return -1;
}

int Application::GetWhiteList(std::list<WhiteListEntry>& white) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetWhiteList(white);
    }
    return -1;
}

int Application::GetProcessBlackList(std::list<WhiteListEntry>& white) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetProcessBlackList(white);
    }
    return -1;
}

int Application::GetFolderMonitorList(std::list<WhiteListEntry>& white) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetFolderMonitorList(white);
    }
    return -1;
}

int Application::GetScanFileTypeList(std::list<WhiteListEntry>& lists) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        storage->GetScanFileTypeList(lists);
    }
    storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->GetScanFileTypeList(lists);
    }
    storage = GetStorageBridget(ZDFY);
    if (storage) {
        storage->GetScanFileTypeList(lists);
    }
    return -1;
}

int Application::AddWhiteList(const WhiteListEntry& entry) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->AddWhiteList(entry);
    }

    storage = GetStorageBridget(GJCZ);
    if (storage) {
        storage->AddWhiteList(entry);
    }

    storage = GetStorageBridget(ZDFY);
    if (storage) {
        storage->AddWhiteList(entry);
    }
    return 0;
}

int Application::DeleteWhitelist(const WhiteListEntry& entry) {
    int err = -1;
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        err = storage->DeleteWhitelist(entry);
    }
    storage = GetStorageBridget(GLTX);
    if (storage) {
        err = storage->DeleteWhitelist(entry);
    }
    storage = GetStorageBridget(ZDFY);
    if (storage) {
        err = storage->DeleteWhitelist(entry);
    }
    return 0;
}

int Application::AppendQuaLog(const QuarantineLog& log) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->AppendQuaLog(log);
    }
    return -1;
}

int Application::AppendScanLog(const LogEventEntry& log) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->AppendScanLog(log);
    }
    return -1;
}

int Application::AppendVirusLog(const VirusInfoEntry& entry) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->AppendVirusLog(entry);
    }
    return -1;
}

int Application::DeleteQuaLog(const QuarantineLog& log) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->DeleteQuaLog(log);
    }
    return -1;
}

int Application::AppendNetAccessLog(const NetAccessEntry& entry) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->AppendNetAccessLog(entry);
    }
    return -1;
}


int Application::GetScanConfig(ScanConfig& config) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetScanConfig(config);
    }
    return -1;
}

int Application::GetUpdateConfig(UpdateConfig& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetUpdateConfig(config);
    }
    return -1;
}

int Application::SetUpdateConfig(const UpdateConfig& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->SaveUpdateConfig(config);
    }
    return -1;
}

int Application::GetUpdateConfig(UpgradeSetting& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetUpgradeSetting(config);
    }
    return -1;
}

int Application::SetUpdateConfig(const UpgradeSetting& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->SetUpgradeSetting(config);
    }
    return -1;
}

int Application::GetDefenConfig(ActiveDefenConfig& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetDefenConfig(config);
    }
    return -1;
}

int Application::SaveDefenConfig(const ActiveDefenConfig& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->SaveDefenConfig(config);
    }
    return -1;
}

int Application::GetAntiVirusConfig(AntiVirusSetting& config, int flag) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetAntiVirusConfig(config);
    }
    return -1;
}

int Application::UpdateAntiVirusConfig(AntiVirusSetting& config) {
    LOG_RJJH("start te update the antivirus");

    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->UpdateAntiVirusConfig(config);
    }
    storage = GetStorageBridget(ZDFY);
    if (storage) {
        storage->UpdateAntiVirusConfig(config);
    }
    storage = GetStorageBridget(GJCZ);
    if (storage) {
        storage->UpdateAntiVirusConfig(config);
    }
    return 0;
}

int Application::UpdateScanConfig(AntiVirusSetting& config) {
    int err = UpdateAntiVirusConfig(config);
    if (err == 0) {
        LOG_RJJH("save sucess, notify ravdaemon");

        CommandBundle command;
        command.mCommandID = CommandBundle::RELOAD_CONFIG;

        if (mAppConfig->SecretMachine()) {
            command.mStringValue = config.mCenterAddr;
            mAppConfig->ChangeConfigSection(AppConfig::CENTER_URL, command.mStringValue);
        }

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
    return -1;
}

int Application::GetNetfilterConfig(NetfilterSetting& config) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetNetfilterConfig(config);
    }
    return -1;
}

int Application::UpdateNetfilterConfig(NetfilterSetting& config) {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->UpdateNetfilterConfig(config);
    }
    return -1;
}

int Application::GetBaseConfig(BaseSetting& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetBaseConfig(config);
    }
    return -1;
}

int Application::UpdateBaseConfig(BaseSetting& config) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->UpdateBaseConfig(config);
    }
    storage = GetStorageBridget(ZDFY);
    if (storage) {
        storage->UpdateBaseConfig(config);
    }
    storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->UpdateBaseConfig(config);
    }

    return -1;
}

bool Application::GetActiveDefenceState() {
    StorageBridge *storage = GetStorageBridget(ZDFY);
    if (storage) {
        return storage->GetFileDefenceState()
                || storage->GetProcDefenceState();
    }
    return false;
}

bool Application::GetUSBPluginMonitorState() {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetUSBPluginMonitorState();
    }
    return false;
}

bool Application::GetNetFilterState() {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetNetFilterState();
    }
    return false;
}

bool Application::GetBrowserBlockState() {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->GetBrowserBlockState();
    }
    return false;
}

int Application::GetScheduleEntry(/*ScheduleEntry::ScanType*/int type, ScheduleEntry& entry) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        return storage->GetScheduleEntry(type, entry);
    }
    return -1;
}

int Application::SetScheduleEntry(ScheduleEntry& entry, bool force) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        if (force) {
            if (entry.mFuncEnum == ScheduleEntry::QUICK_SCAN) {
                storage->UpdateSetting(TIME_QUICK_SCAN_FLAG, true);
            }
            else if (entry.mFuncEnum == ScheduleEntry::ALL_SCAN) {
                storage->UpdateSetting(TIME_FULL_SCAN_FLAG, true);
            }
        }
        storage->SetScheduleEntry(entry);
    }
    storage = GetStorageBridget(GLTX);
    if (storage) {
        if (force) {
            if (entry.mFuncEnum == ScheduleEntry::QUICK_SCAN) {
                storage->UpdateSetting(TIME_QUICK_SCAN_FLAG, true);
            }
            else if (entry.mFuncEnum == ScheduleEntry::ALL_SCAN) {
                storage->UpdateSetting(TIME_FULL_SCAN_FLAG, true);
            }
        }
        storage->SetScheduleEntry(entry);
    }
    return 0;
}

int Application::DeleteScheuleType(int type) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        return storage->DeleteScheuleType(type);
    }
    return -1;
}

int Application::GetTitle(string& major, string& minor) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (storage) {
        storage->GetTitle(major, minor);
    }
    if (major.empty()) {
        if (IsSecretMachine() || IsCenterLess()) {
            major = "瑞星ESM防病毒系统";
        }
        else {
        	switch(mAppConfig->GetProductType()) {
        	case 1: major = "瑞星ESM防病毒终端安全防护系统"; break; 
        	case 2: major = "瑞星ESM防病毒系统"; break;
        	}
        }
    }
    if (minor.empty()) {
        if (IsSecretMachine()) {
            if (mAppConfig->GetProductType() == 0) {
                minor = "(涉密专用计算机单机版)V3.0";
            }
            else {
                minor = "(涉密专用计算平台网络版)V3.0";
            }
        }
        else if (IsCenterLess()) {
            if (IsGeneralSystem()) {
                minor = "通用单机版";
            }
            else if (IsChinaSystem()) {
                minor = "国产单机版";
            }
            else {
                 minor = "通用单机版";
            }
        }
        // 20210927去掉子标题
        //else {
        //    minor = "下一代网络版";
        //}
    }
    return 0;
}

bool Application::IsSecretMachine() {
    return mAppConfig->SecretMachine();
}

bool Application::IsCenterLess() {
    return mAppConfig->GetProductType() == 0;
}

bool Application::IsGeneralSystem(){
    return mAppConfig->GetSubProductGUID() == SUB_PRODUCT_LGSERVER_XAV
            || mAppConfig->GetSubProductGUID() == SUB_PRODUCT_LGDESKTOP_XAV;
}
bool Application::IsChinaSystem() {
    return mAppConfig->GetSubProductGUID() == SUB_PRODUCT_LCSERVER_XAV
            || mAppConfig->GetSubProductGUID() == SUB_PRODUCT_LCDESKTOP_XAV;
}

int Application::ResetConfig() {
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

AntiVirusSetting::AntiCommon::ScanType Application::GetScanIntelligentMode(
        ScanIntelligentParams& params,
        bool& locked) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (storage) {
        if (storage->GetScanIntelligentState()) {
            ScanIntelligentParams param;
            param.Detail(AntiVirusSetting::AntiCommon::AUTO_MODE);
            return AntiVirusSetting::AntiCommon::AUTO_MODE;
        }
        else
        {
             AntiVirusSetting::AntiCommon::ScanType type = (AntiVirusSetting::AntiCommon::ScanType)storage->GetScanIntelligentMode(locked);
             params.Detail(type);
             return type;
        }
    }
    params.Detail(AntiVirusSetting::AntiCommon::AUTO_MODE);
    return AntiVirusSetting::AntiCommon::AUTO_MODE;
}



int Application::SetScanIntelligentMode(AntiVirusSetting::AntiCommon::ScanType type) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (NULL == storage) {
        return -1;
    }

    if (storage->SetScanIntelligentMode(type)) {
        ScanIntelligentParams param;
        param.Detail(type);

        std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
        CommandBundle command;
        command.mCommandID = CommandBundle::SCAN_EVENT;
        command.mIntegerValue = CommandBundle::INTELLIGENT_SCAN;
        command.mStringValue = param.Externalize();

        int ret = 0;
        RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
        if (0 == bundle.Externalize(sock.c_str(), ret)) {
            return ret;
        }
    }
    return -1;
}

Application::UserType Application::GetUserType() {
    string user = Utils::OS::GetUserFromUID(getuid());
    if (user == "root") {
        return Application::ROOT;
    }
    else if (user == "secadm") {
        return Application::SECADM;
    }
    else if (user == "auditadm") {
        return Application::AUDITADM;
    }
    return Application::USER;
}

string Application::GetScanFileCustomType() {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (NULL == storage) {
        return "";
    }

    string ret = "";
    std::list<WhiteListEntry> lists;
    int err = storage->GetScanFileTypeList(lists);
    if (err > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=lists.begin(); iter!=lists.end(); ++iter) {
            ret += iter->mPath + ";";
        }
    }
    return ret;
}

int Application::SetScanFileCustomType(const string& params) {
    StorageBridge *storage = GetStorageBridget(GJCZ);
    if (NULL == storage) {
        return -1;
    }

    vector<string> types;
    Utils::String::Split(params, ";", &types);

    return storage->SetScanFileTypeList(types);
}

int Application::GetUpdateSource(list<SourceListEntry>& entries) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (NULL == storage) {
        return -1;
    }
    return storage->GetSourceList(entries, SourceListEntry::CLOUD_UPDATE_SOURCE);
}

int Application::SetUpdateSource(list<SourceListEntry>& entries) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (NULL == storage) {
        return -1;
    }
    return storage->SetSourceList(entries, SourceListEntry::CLOUD_UPDATE_SOURCE, true);
}

int Application::GetCenterSource(list<SourceListEntry>& entries) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (NULL == storage) {
        return -1;
    }
    return storage->GetSourceList(entries, SourceListEntry::CENTER_PROXY_SOUCE);
}

int Application::SetCenterSource(list<SourceListEntry>& entries) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (NULL == storage) {
        return -1;
    }
    return storage->SetSourceList(entries, SourceListEntry::CENTER_PROXY_SOUCE, true);
}


int Application::GetSourceList(list<SourceListEntry>& entries, SourceListEntry::SOURCE_TYPE type) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (NULL == storage) {
        return -1;
    }
    return storage->GetSourceList(entries, type);
}


int Application::SetSourceList(list<SourceListEntry>& entries, SourceListEntry::SOURCE_TYPE type, bool manul) {
    StorageBridge *storage = GetStorageBridget(GLTX);
    if (NULL == storage) {
        return -1;
    }
    return storage->SetSourceList(entries, type, true);
}

void *Application::CheckScanSocketThread(void *params) {
    Application *self = (Application*)params;
    self->CheckScanSocketFunction();
    return NULL;
}

void Application::CheckScanSocketFunction() {
//    while (mCheckState) {
//        long now = Utils::Date::GetTimeStamp();
//        if (now - mScanEventLastStamp > 10) {
//            // not respond, maybe the socket disconnect, reconnect
//            LOG_RJJH("recreate the the socket connection");
//            InitListener(mSockName.c_str());
//        }
//        sleep(5);
//    }
}

int Application::UpdateOfflineVirlib(const string &file) {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::DAEMON_OFFLINE_UPDATE;
    command.mIntegerValue = CommandBundle::UPDATE_OFFLINE_VIRLIB;
    command.mStringValue = file;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}

int Application::QueryPath(const string &folder, map<string, vector<string> >& infos) {
    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);
    CommandBundle command;
    command.mCommandID = CommandBundle::QEURY_PATH_INFO;
    command.mStringValue = folder;

    string path_json;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), path_json)) {
        Json::Value json;
        Json::Reader reader;
        if (reader.parse(path_json, json)) {
            Json::Value::Members members = json.getMemberNames();
            vector<string>::iterator iter;
            for (iter=members.begin(); iter!=members.end();++iter) {
                Json::Value value = json[*iter];
                vector<string> subfolder;
                for (int i=0; i<(int)value.size(); ++i) {
                    subfolder.push_back(value[i].asString());
                }
                infos.insert(make_pair(*iter, subfolder));
            }
        }
        return 0;
    }
    return -1;
}

void Application::HandleMessage(int event, int type, void *object) {
    switch (event) {
    case VERIFY_SCAN_EVENT: {
        // 在此做一个延时判断,如果在5秒种内，状态未改变，则认为通讯出错，上报
		// zcg+, 等待时间为3s的时候, 因为引擎加载有时较慢在3s左右, 会调用了 StopScan. 导致不能开启扫描. 
		// 时间较长的话 ???
        for (int i=0; (i<7) && (mScanStatus==APP_SCAN_PREPARE); ++i) {
            sleep(1);
        }
        if (mScanStatus == APP_SCAN_PREPARE) {
            StopScan();
	    //double lock
            //mHandler->PostMessage(Message(SCAN_ERROR_EVENT, type, 0));
        }
        break;
    }
    case SCAN_ERROR_EVENT: {
        RefreshScanStatus(0, SCAN_ERROR_EXIT, 0);
        break;
    }
    case CHECK_REGISTED_EVENT: {
        mActived = (Registed() == 0)?1:0;
        RefreshUpdateStatus(RESPOND_REGISTER_STATE, mActived?"":"未激活");
        break;
    }
    }
}

int Application::GetProductType() const {
    return mAppConfig->GetProductType();
}
