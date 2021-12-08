#include "taskproxy.h"
#include "baseconfig.h"
#include "scanconfig.h"
#include "Entry/logevententry.h"
#include "Entry/commonentry.h"
#include "models/policy/commandmodel.h"
#include "commonapi.h"
#include "mainengine.h"
#include "application.h"
#include "appconfig.h"
#include "circlelink.h"

#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <pthread.h>

//
ScheduleScaner* ScheduleScaner::NEW(Controller *controller) {
    ScheduleScaner *self = new ScheduleScaner;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

ScheduleScaner::ScheduleScaner() : ScanProxy(ScanProxy::SCHEDULE_TASK) {
    mType = 0;
}

ScheduleScaner::~ScheduleScaner() {

}

void ScheduleScaner::SetScanPath(const string &paths) {
    Utils::String::Split(paths, ";", &mScanPaths);
}

int ScheduleScaner::Construct(Controller *controller) {
    mController = controller;
    return 0;
}

int ScheduleScaner::ScanType() {
    return LogEventEntry::SetScanType(
                LogEventEntry::TIMER_SCAN,
                mType==ScheduleEntry::QUICK_SCAN?LogEventEntry::QUICK_DISK:LogEventEntry::ALL_DISK);
}

int ScheduleScaner::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) {
    (void)event;
    (void)file;
    (void)virus;
    return 0;
}


int ScheduleScaner::GetScanPath(std::list<pair<std::string,int> >& path) {
    if (mScanPaths.size() > 0) {
        std::vector<string>::iterator iter;
        for (iter=mScanPaths.begin(); iter!=mScanPaths.end(); ++iter) {
            path.push_back(make_pair(*iter, 2));
        }
    }
    else {
        if (mType == ScheduleEntry::QUICK_SCAN) {
            GetQuickScanPaths(path);
        }
        else if (mType == ScheduleEntry::ALL_SCAN) {
            GetFullScanPaths(path);
        }
    }
    return 0;
}

//
RemoteScaner* RemoteScaner::NEW(Controller *controller) {
    RemoteScaner *self = new RemoteScaner;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

RemoteScaner::RemoteScaner() : ScanProxy(ScanProxy::REMOTE_COMMAND) {
    mType = 0;
    mCommandModel = NULL;
}

RemoteScaner::~RemoteScaner() {
    if (mCommandModel) {
        delete mCommandModel;
        mCommandModel = NULL;
    }
}

void RemoteScaner::SetCommandModel(const CommandModel& model) {
    *mCommandModel = model;
}

int RemoteScaner::Construct(Controller *controller) {
    MainEngine *engine = (MainEngine*)controller;
    Application::Construct(engine->GetAppConfig());

    mController = controller;
    mCommandModel = new CommandModel;
    return 0;
}

void RemoteScaner::Prepare() {
    MainEngine *engine = (MainEngine*)mController;
    engine->ReportRemoteStartEvent(ScanType());
}

int RemoteScaner::ScanType() {
    return LogEventEntry::SetScanType(
                LogEventEntry::REMOTE_SCAN,
                (mCommandModel->mCmdId == (int)CommandModel::QUICK_SCAN_START)?LogEventEntry::QUICK_DISK:LogEventEntry::ALL_DISK
            );
}

int RemoteScaner::GetScanPath(std::list<pair<std::string,int> >& path) {
    if (mCommandModel->mCmdId == CommandModel::QUICK_SCAN_START) {
        GetQuickScanPaths(path);
    }
    else if (mCommandModel->mCmdId == CommandModel::ALL_SCAN_START) {
        std::string remote = GetRemoteScanPath();
        if (remote.empty()) {
            GetFullScanPaths(path);
        }
        else {
            path.push_back(make_pair(remote, -1));
        }
    }

    return 0;
}

int RemoteScaner::ScanFinished(int result) {
    LOG_GJCZ("remote command scan finished");

    MainEngine *engine = (MainEngine*)mController;
    return engine->FinishRemoteScan(GetCommandModel()->mId);
}

int RemoteScaner::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) {
    return 0;
}

void RemoteScaner::RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data) {
    LOG_GJCZ("remote scan progress %d", progress);
}

int FolderScaner::ScanType() {
    return LogEventEntry::SetScanType(
                LogEventEntry::SPECIAL_FOLDER_SCAN,
                LogEventEntry::SPECIAL_FILE
            );
}

int FolderScaner::LoadScanConfig() {
    mScanConfig = new ScanConfig;
    mScanConfig->mVirusAction = MANUAL_DEAL;
    mScanConfig->mScanCompressFile = true;
    mScanConfig->mCompressSize = 800;
    return 0;
}

//
FolderScaner* FolderScaner::NEW(Controller *controller) {
    FolderScaner *self = new FolderScaner;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

FolderScaner::FolderScaner() : ScanProxy(ScanProxy::CONTEXT_MENU) {
    mScanProgress = 0;
}

FolderScaner::~FolderScaner() {

}

int FolderScaner::Construct(Controller *controller) {
    MainEngine *engine = (MainEngine*)controller;
    Application::Construct(engine->GetAppConfig());

    mController = controller;
    return 0;
}

const string FolderScaner::GetScanFolder() const {
    return Utils::Hash::UrlDecode(mPath);
}

void FolderScaner::RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data) {
    MainEngine *engine = (MainEngine*)mController;
    if (THREAD_STATUS == scanstatus && data) {
        vector<ReportSummary> *summaries = (vector<ReportSummary>*)data;
        string prompt = "正在扫描;进度:" + Utils::String::itoa(mScanProgress) + "%;路径: " + (*summaries)[0].mFile;
        engine->SendToNotifer(CommunicateBundle::INFO,
                      CommunicateBundle::NOTIFY_SCANING,
                      "文件扫描",
                      prompt);
    }
    mScanProgress = progress;
}

int FolderScaner::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) {
    if (event == Application::QUERY_FIND_VIRUS) {
        if (SCAN_AUTO_KILL == mVirusAutoType) {
            return NOTIFY_CONFIRM_SELECT;
        }
        else if (SCAN_AUTO_IGNORE == mVirusAutoType) {
            return NOTIFY_IGNORE_SELECT;
        }

        string prompt = "发现病毒!;病毒名称: " + virus +";文件路径: " + file;
        MainEngine *engine = (MainEngine*)mController;
        int sel = engine->SendToNotifer(CommunicateBundle::QUERY,
                                CommunicateBundle::NOTIFY_VIRUS,
                                  "文件扫描",
                                  prompt,
                                  "立即查杀",
                                  "确定",
                                  5000*1000
                                  );

        int defaultCheck = sel>>16;
        if (defaultCheck > 0) {
            if ((int)(sel & 0xffff) == NOTIFY_IGNORE_SELECT) {
                mVirusAutoType = SCAN_AUTO_IGNORE;
            }
            else if ((int)(sel&0xffff) == NOTIFY_CONFIRM_SELECT) {
                mVirusAutoType = SCAN_AUTO_KILL;
            }
        }

        // this return value must correct the TEFC define
        return sel&0xffff;
    }
    return NOTIFY_IGNORE_SELECT;
}

void FolderScaner::RefreshVirusLogs(const string& file, const string& virus, int status) {
    VirusReportData data;
    data.Virus(file, virus, status);
    mVirusEntries.push_back(data);
}

int FolderScaner::ScanFinished(int result) {
    string prompt;
    list<VirusReportData>& datas = GetVirusData();
    if (datas.size() > 0) {
        string virus;
        list<VirusReportData>::iterator iter;
        for (iter=datas.begin();iter!=datas.end(); ++iter) {
            virus += iter->mVirus;
            virus += ",";
        }

        prompt = "已处理 " + Utils::String::itoa(datas.size()) + " 个病毒!;病毒类型: " + virus + ";扫描路径:" + GetScanFolder();
    }
    else {
        prompt = string("扫描完成!;未发现病毒;") + "扫描路径:" + GetScanFolder();
    }

    MainEngine *engine = (MainEngine*)mController;
    engine->SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_SCAN_FINISHED,
                          "文件扫描",
                          prompt,
                          "我知道了",
                          "");
}

///
DevicePluginChecker::DevicePluginChecker()  {
    mApplicationType = ScanProxy::PLUGIN_CHECK;
    mScanProgress = 0;
    mScanDepth = -1;
}

DevicePluginChecker::~DevicePluginChecker() {

}

DevicePluginChecker* DevicePluginChecker::NEW(Controller *controller) {
    DevicePluginChecker *self = new DevicePluginChecker;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int DevicePluginChecker::Construct(Controller *controller) {
    mController = controller;
    return 0;
}

int DevicePluginChecker::ScanType() {
    return LogEventEntry::SetScanType(LogEventEntry::ACTIVE_DEFENCE, LogEventEntry::USER_DISK);
}

int DevicePluginChecker::QueryUserForResult(QUERY_EVENT event, const string &file, const string &virus) {
    return mController->QueryUserForResult(event, file, virus);
}

int DevicePluginChecker::LoadScanConfig() {
    ScanProxy::LoadScanConfig();

    // define scan u with one thread
    mScanConfig->mThreadCount = 1;
    mScanConfig->mCloudAddr.clear();
    mScanConfig->mScanCompressFile = false;
    mScanConfig->mCompressSize = 0;

    return 0;
}

int DevicePluginChecker::ScanFinished(int result) {
    string prompt;
    list<VirusReportData>& datas = GetVirusData();
    if (datas.size() > 0) {
        string virus;
        list<VirusReportData>::iterator iter;
        for (iter=datas.begin();iter!=datas.end(); ++iter) {
            virus += iter->mVirus;
            virus += ",";
        }

        prompt = "已处理 " + Utils::String::itoa(datas.size()) + " 个病毒!;病毒类型: " + virus + ";扫描路径:" + GetScanFolder();
    }
    else {
        prompt = string("扫描完成!;未发现病毒;") + "扫描路径:" + GetScanFolder();
    }

    MainEngine *engine = (MainEngine*)mController;
    engine->SendToNotifer(CommunicateBundle::INFO,
                  CommunicateBundle::NOTIFY_SCAN_FINISHED,
                  "U盘扫描",
                  prompt,
                  "我知道了",
                  "");
    return 0;
}

///
int DefenScaner::ScanType() {
    return LogEventEntry::SetScanType(
                LogEventEntry::ACTIVE_DEFENCE,
                LogEventEntry::MONITOR_FILE
            );
}

int DefenScaner::LoadScanConfig() {
    mScanConfig = new ScanConfig;

    AntiVirusSetting setting;
    if (mController) {
        mController->GetAntiVirusConfig(setting);

        mScanConfig->mScanFileType = (SCAN_TYPE)setting.mFileMonitor.mFileType.mValue;
        mScanConfig->mVirusAction = (VIRUS_DEAL)setting.mFileMonitor.mVirusDeal.mValue;
        mScanConfig->mScanCompressFile = setting.mFileMonitor.mZipScan.mValue;
        mScanConfig->mCleanFailedAction = (CLEAN_FAILED)setting.mFileMonitor.mCleanFailed.mValue;
        mScanConfig->mCompressSize = setting.mFileMonitor.mZipFileSize.mValue;

        mScanConfig->mUseCache = false;
        mScanConfig->mThreadCount = 1;
        mScanConfig->mCloudAddr.clear();

        mNotify = setting.mFileMonitor.mCleanPrompt.mValue;
    }
    return 0;
}

DefenScaner* DefenScaner::NEW(Controller *controller) {
    DefenScaner *self = new DefenScaner;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

DefenScaner::DefenScaner() : ScanProxy(ScanProxy::ACTIVE_DEFEN) {
    mNotify = false;
    mScanDepth = -1;
}

DefenScaner::~DefenScaner() {

}

int DefenScaner::Construct(Controller *controller) {
    MainEngine *engine = (MainEngine*)controller;
    Application::Construct(engine->GetAppConfig());

    mController = controller;
    return 0;
}

const string DefenScaner::GetScanFolder() const {
    return Utils::Hash::UrlDecode(mPath);
}

int DefenScaner::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) {
    return mController->QueryUserForResult(event, file, virus);
}

void DefenScaner::RefreshVirusLogs(const string& file, const string& virus, int status) {
    LOG_GJCZ("active define get virus %s from %s", virus.c_str(), file.c_str());

    VirusReportData data;
    data.Virus(file, virus, status);
    mVirusEntries.push_back(data);
}

int DefenScaner::ScanFinished(int result) {
    LOG_GJCZ("defen scan finish, notify user");
    if (GetFinishNotifyState()) {
        string prompt;
        list<VirusReportData>& datas = GetVirusData();
        if (datas.size() > 0) {
            string virus;
            list<VirusReportData>::iterator iter;
            for (iter=datas.begin();iter!=datas.end(); ++iter) {
                virus += iter->mVirus;
                virus += ",";
            }

            prompt = "已处理 " + Utils::String::itoa(datas.size()) + " 个病毒!;病毒类型: " + virus + ";扫描路径:" + GetScanFolder();

            LOG_GJCZ(prompt.c_str());

            MainEngine *engine = (MainEngine*)mController;
            engine->SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_SCAN_FINISHED,
                          "文件监控",
                          prompt,
                          "我知道了",
                          "");
        }
    }
    else {
        LOG_GJCZ("notify state is disable");
    }
}

// User scaner
int UserScaner::ScanType() {
    LogEventEntry::DiskType logType = LogEventEntry::QUICK_DISK;
    switch (mScanType) {
    case CommandBundle::QUICK_SCAN:
        logType = LogEventEntry::QUICK_DISK;
        break;
    case CommandBundle::FULL_SCAN:
        logType = LogEventEntry::ALL_DISK;
        break;
    case CommandBundle::CUSTOM_SCAN:
        logType = LogEventEntry::USER_DISK;
        break;
    default:
        break;
    }
    return LogEventEntry::SetScanType(
                LogEventEntry::MANUAL_SCAN,
                logType
            );
}

void UserScaner::Prepare() {
    // 在用户扫描时，上报一条通信回答消息以确定通讯是畅通的
    char sockname[128];
    sprintf(sockname, RJJH_MAJOR_SOCKET, mUserProcPid);
    std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::PREPARE_SCAN_SUCCESS;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    int err = bundle.Externalize(sock.c_str(), ret);
    if (0 == err) {
        LOG_GJCZ("prepare scan event success");
    }
    else {
        LOG_GJCZ("prepare scan event error %d, not received", err);
    }
}

//
UserScaner* UserScaner::NEW(Controller *controller) {
    UserScaner *self = new UserScaner;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int UserScaner::GetScanPath(std::list<pair<std::string,int> >& path) {
    switch (mScanType) {
    case CommandBundle::QUICK_SCAN: {
        GetQuickScanPaths(path);
        break;
    }
    case CommandBundle::FULL_SCAN: {
        GetFullScanPaths(path);
        break;
    }
    case CommandBundle::CUSTOM_SCAN: {
        if (!mScanPaths.empty()) {
            Json::Reader reader;
            Json::Value json;
            if (reader.parse(mScanPaths, json)) {
                for (int i=0; i<(int)json.size(); ++i) {
                    Json::Value p = json[i];
                    path.push_back(make_pair(Utils::JSON::GetJsonString(p, "p"), Utils::JSON::GetJsonInt(p, "d")));
                }
            }
        }
        break;
    }
    default:
        break;
    }

    return 0;
}

UserScaner::UserScaner() : ScanProxy(ScanProxy::REMOTE_COMMAND) {
    mScanProgress = 0;
    mScanFileCount = 0;
    mUserProcPid = -1;
}

UserScaner::~UserScaner() {

}

int UserScaner::Construct(Controller *controller) {
    MainEngine *engine = (MainEngine*)controller;
    Application::Construct(engine->GetAppConfig());

    mController = controller;
    return 0;
}


void UserScaner::SetScanParams(int type, int extend, const string& params) {
    mScanType = type;
    mUserProcPid = extend;
    mScanPaths = params;
}

void UserScaner::SetScanParams(int type, int params) {
    mScanType = type;
    switch(type) {
    case CommandBundle::QUICK_SCAN:
    case CommandBundle::FULL_SCAN:
        mUserProcPid = params;
        break;
    }
}

int UserScaner::StartScan(std::list<pair<std::string,int> >& path, bool feedback) {
    return Application::StartScan(path, true);
}

void UserScaner::RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data) {
    if (mUserProcPid < 0) {
        return;
    }
    if (SCAN_ALL_FILE == scanstatus) {
        mScanFileCount = progress;
    }

    Json::Value json;
    json["type"] = Json::Value(scantype);
    json["status"] = Json::Value(scanstatus);
    json["progress"] = Json::Value(progress);
    if (NULL != data) {
        Json::Value summary;
        vector<ReportSummary> *summaries = (vector<ReportSummary>*)data;
        vector<ReportSummary>::iterator iter;
        for (iter=summaries->begin(); iter!=summaries->end(); ++iter) {
            Json::Value summ;
            summ["tag"] = Json::Value(*iter->mTag);
            summ["file"] = Json::Value(iter->mFile);
            summ["count"] = Json::Value(*iter->mCount);
            summ["virus"] = Json::Value(*iter->mVirus);

            summary.append(summ);
        }
        json["summary"] = summary;
    }

    Json::FastWriter writer;
    string result = writer.write(json);

    char sockname[128];
    sprintf(sockname, RJJH_MAJOR_SOCKET, mUserProcPid);
    std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

    LOG_GJCZ("RJJH_MAJOR_SOCKET:%s", sock.c_str());

    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::REFRESH_SCAN_STATUS;
    command.mStringValue = result;

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    int err = bundle.Externalize(sock.c_str(), ret);
    if (0 == err) {
        LOG_GJCZ("send the mainscand %s", result.c_str());
    }
    else {
        LOG_GJCZ("send main screen error %d, not received", err);
    }
    mScanProgress = progress;
}

int UserScaner::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) {
    if (event == Application::QUERY_FIND_VIRUS) {
        if (SCAN_AUTO_KILL == mVirusAutoType) {
            return NOTIFY_CONFIRM_SELECT;
        }
        else if (SCAN_AUTO_IGNORE == mVirusAutoType) {
            return NOTIFY_IGNORE_SELECT;
        }

        Json::Value json;
        json["file"] = Json::Value(file);
        json["virus"] = Json::Value(virus);
        json["event"] = Json::Value(event);

        Json::FastWriter writer;
        string result = writer.write(json);

        char sockname[128];
        sprintf(sockname, RJJH_MAJOR_SOCKET, mUserProcPid);
        std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

        LOG_GJCZ("RJJH_MAJOR_SOCKET:%s", sock.c_str());

        CommandBundle command;
        command.mCommandID = CommandBundle::SCAN_EVENT;
        command.mIntegerValue = CommandBundle::QUERY_VIRUS_RESULT;
        command.mStringValue = result;

        int sel = 0;
        RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command, 0);
        if (bundle.Externalize(sock.c_str(), sel)) {
            return NOTIFY_IGNORE_SELECT;
        }

        int defaultCheck = sel>>16;
        if (defaultCheck > 0) {
            if ((int)(sel & 0xffff) == NOTIFY_IGNORE_SELECT) {
                mVirusAutoType = SCAN_AUTO_IGNORE;
            }
            else if ((int)(sel&0xffff) == NOTIFY_CONFIRM_SELECT) {
                mVirusAutoType = SCAN_AUTO_KILL;
            }
        }

        // this return value must correct the TEFC define
        return sel&0xffff;
    }
    return NOTIFY_IGNORE_SELECT;
}

void UserScaner::RefreshVirusLogs(const string& file, const string& virus, int status) {
    Json::Value json;
    json["file"] = Json::Value(file);
    json["virus"] = Json::Value(virus);
    json["result"] = Json::Value(status);

    Json::FastWriter writer;
    string result = writer.write(json);

    char sockname[128];
    sprintf(sockname, RJJH_MAJOR_SOCKET, mUserProcPid);
    std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::REFRESH_VIRUS_LOGS;
    command.mStringValue = result.c_str();

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    int err = bundle.Externalize(sock.c_str(), ret);

    VirusReportData data;
    data.Virus(file, virus, status);
    mVirusEntries.push_back(data);
}

int UserScaner::ScanFinished(int result) {
    printf("ScanFinished\n");
    char sockname[128];
    sprintf(sockname, RJJH_MAJOR_SOCKET, mUserProcPid);
    std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::SCAN_EVENT;
    command.mIntegerValue = CommandBundle::SCAN_ALL_FINISHED;

    Json::Value json;
    json["type"] = Json::Value(ScanType());
    json["progress"] = Json::Value(mScanFileCount);

    Json::FastWriter writer;
    command.mStringValue = writer.write(json);

    int ret = 0;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sock.c_str(), ret)) {
        return ret;
    }
    return -1;
}


MemoryScaner* MemoryScaner::NEW(Controller *controller) {
    MemoryScaner *self = new MemoryScaner;
    if (self) {
        if (self->Construct(controller)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}


int MemoryScaner::Construct(Controller *controller) {
    mController = controller;
    return 0;
}

int MemoryScaner::ScanType() {
    return LogEventEntry::SetScanType(LogEventEntry::ACTIVE_DEFENCE, LogEventEntry::MEMORY);
}

int MemoryScaner::QueryUserForResult(QUERY_EVENT event, const string &file, const string &virus) {
    return 0;
}

int MemoryScaner::LoadScanConfig() {
    mScanConfig = new ScanConfig;
    mScanConfig->mVirusAction = AUTO_DEAL;
    mScanConfig->mScanFileType = ALL_FILE;
    mScanConfig->mCompressSize = 0;
    mScanConfig->mThreadCount = 1;
    mScanConfig->mUseCache = false;
    mScanConfig->mCloudAddr.clear();
    return 0;
}

int MemoryScaner::ScanFinished(int result) {
    string prompt;
    list<VirusReportData>& datas = GetVirusData();
    if (datas.size() > 0) {
        string virus;
        list<VirusReportData>::iterator iter;
        for (iter=datas.begin();iter!=datas.end(); ++iter) {
            virus += iter->mVirus;
            virus += ",";
        }

        prompt = "已处理 " + Utils::String::itoa(datas.size()) + " 个病毒!;病毒类型: " + virus + ";扫描路径:" + GetScanFolder();
    }
    else {
        prompt = string("扫描完成!;未发现病毒;") + "扫描路径:" + GetScanFolder();
    }

    MainEngine *engine = (MainEngine*)mController;
    engine->SendToNotifer(CommunicateBundle::INFO,
                  CommunicateBundle::NOTIFY_SCAN_FINISHED,
                  "内存扫描",
                  prompt,
                  "我知道了",
                  "");
    return 0;
}

int MemoryScaner::GetScanPath(std::list<pair<std::string,int> >& path) {
    struct dirent *tmp = NULL;
    char tempPath[1024] = {0};

    map<string, bool> process;
    DIR *dir = opendir("/proc");
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = false;
#if defined (COMMON_LINUX)
        isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", "/proc", tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
        }
        if (isdir) {
            if (tmp->d_name[0] > '0' && tmp->d_name[1] <= '9') {
                int pid = atoi(tmp->d_name);
                if (pid < 3000) {
                    continue;
                }
                sprintf(tempPath, "/proc/%s/cmdline", tmp->d_name);
                if (Utils::Path::FileAvailed(tempPath)) {
                    FILE *f = fopen(tempPath, "rb");
                    if (f) {
                        int readed = fread(tempPath, 1, 1023, f);
                        tempPath[readed] = '\0';
                        if (strlen(tempPath) > 0) {
                            process.insert(make_pair(tempPath, true));
                        }
                    }
                }
            }
        }
    }
    closedir(dir);

    map<string, bool>::iterator iter;
    for (iter=process.begin();iter!=process.end();++iter) {
        if (Utils::String::StartsWith(iter->first, "/")) {
            path.push_back(make_pair(iter->first, 0));
        }
    }
    return path.size();
}
