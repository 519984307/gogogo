#include "mainengine.h"
#include "storage.h"
#include "multivirusengine.h"
#include "commonapi.h"
#include "scanengine.h"
#include "baseconfig.h"
#include "taskproxy.h"
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
#include "models/policy/policymodel.h"
#include "models/policy/commandmodel.h"
#include "models/log/logdatascanevent.h"
#include "scanconfig.h"
#include "policysyncparser.h"
#include "scannotifycallback.h"
#include "cinifile.h"
#include "pluginentry.h"
#include "handler.h"
#include "simplejudger.h"
#include "Common/updatevirusengine.h"
#include "json/json.h"
#include "cross.h"
#include "ravquafile.h"

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <fstream>
#include <algorithm>
#include <utility>

#include "scancache.h"


#ifndef SECURITY_FREE
    #define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#endif
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;


#define ASYNC_REPORT_SCAN_STOP      10001
#define ASYNC_REPORT_SCAN_LOG_START 10002
#define ASYNC_REPORT_DEFEN_LOG      10003

extern int g_running_state;

string rising_usb_path;


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
    mUSBChecker = NULL;
    mUpdateEngine = NULL;

    mHealthStatus = HEALTHY;
    mScanTaskID = 0;

#ifdef SECRET_MACHINE
    mCustomScan = false;
#endif
}

MainEngine::~MainEngine() {
    SECURITY_FREE(mUSBChecker);
    SECURITY_FREE(mUpdateEngine);
}

int MainEngine::Construct() {
	int ret;

    ClearLastState();
    ret = ServiceBinder::Construct(FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET);
    LOG_GJCZ("manengine contruct (%d,%d)\n", ret, errno);
    SECURITY_CHECK(ret, 0, -1);

    srand((int)time(0));

    return 0;
}

int MainEngine::DelayConstruct() {
    ServiceBinder::DelayConstruct();

    LOG_GJCZ("the second contruct");

    Utils::Path::CheckPath(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, NULL), 0777);
    SECURITY_CHECK(Controller::Construct(mAppConfig->GetInstallPath(), FOLDER_RJJH_GJCZ), 0, -1);

    if (mStorage->IsFirstCreate() || mStorage->GetSettingsCount() < 3) {
        ResetConfig(mStorage);
    }

    if (!GetActivePowerOnState()) {
        ActiveDefenConfig config;
        if (0 == GetDefenConfig(config)) {
            config.mFileDefenState.mValue = 0;
            SaveDefenConfig(config);
        }
    }

    // get the defen scan config
    GetDefenScanConfig(*mDefenScanConfig);

    if (mAppConfig->USBMonitor()) {
        // TODO, 在此设置U盘监控默认为0, 以后功能正常后去除
//        mStorage->SetUSBPluginMonitorSate(false, false);
        if (GetUSBPluginMonitorState()) {
            InitUSBPluginMonitor();
        }
    }

    // GJCZ started, repaire the config
    if (mAppConfig->GetSubProductGUID() != SECRET_MACHINE_ID) {
//        RepairConfig();
    }

    // import the cross update log
    string version, guid, sguid;
    if (ImportCrossUpdateInfo(version, guid, sguid)) {
//        // success, notify the ep info
//        CommandBundle command;
//        command.mCommandID = CommandBundle::REMOTE_COMMAND;
//        command.mIntegerValue = CommandBundle::DEPLOY_CROSS_SUCCESS;
//        command.mStringValue = version;

//        NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
    }
    return 0;
}

int MainEngine::Start() {
    ServiceBinder::Start();
    Controller::Start();

    if (mUSBChecker) {
        mUSBChecker->Start();
    }

    return 0;
}

int MainEngine::Stop() {
    ServiceBinder::Stop();
    Controller::Stop();

    if (mUSBChecker) {
        mUSBChecker->Stop();
    }

    return 0;
}

int MainEngine::ReloadConfig() {
    return CheckUSBMonitor() || GetDefenScanConfig(*mDefenScanConfig);
}

int MainEngine::CheckUSBMonitor() {
    if (GetUSBPluginMonitorState()) {
        if (mUSBChecker) {
            return 0;
        }
        else {
            if (0 == InitUSBPluginMonitor()) {
                mUSBChecker->Start();
            }
        }
    }
    else {
        if (mUSBChecker) {
            mUSBChecker->Stop();
            delete mUSBChecker;
            mUSBChecker = NULL;
        }
    }
    return 0;
}

int MainEngine::ProcessGlobalPolicy(PolicyModel *model, Storage *storage, int type) {
    if (0 == ServiceBinder::ProcessGlobalPolicy(model, storage, type)) {
        // TODO 在这里先注销, 防止下发策略再行打开U盘监控和文件监控
//        CheckUSBMonitor();
//        GetDefenScanConfig(*mDefenScanConfig);
    }

    return 0;
}

int MainEngine::ProcessLinuxPolicy(PolicyModel *model, Storage *storage) {
    bool changed = false;
    if (model->mPolicyProductID == mAppConfig->GetSubProductGUID() ||
            model->mPolicyProductID == LINUX_RAV_PRODUCTID) {
        // save
        if (!model->mPolicyMainContent.empty()) {
            PolicyScanModel policy;
            PolicyParser::ParseLinuxPolicy(model->mPolicyMainContent, policy);
            if (policy.mScanPoliy.mTimeScan.mState > 0) {
                // have schedule scan, then save it
                ScheduleEntry entry;
                GetScheduleEntry(ScheduleEntry::LINUX_SCAN, entry);
                entry.mFuncEnum = ScheduleEntry::LINUX_SCAN;
                entry.mType = ScheduleEntry::WEEKS;
                entry.mCrontab = 0;
                entry.mUserSet = false;
                entry.mLocked = false;
                entry.SetCrontabHour(policy.mScanPoliy.mTimeScan.mHours);
                entry.SetCrontabMinute(policy.mScanPoliy.mTimeScan.mMinite);
                entry.mScanPath = policy.mScanPath;
                std::vector<int> week;
                for (int i=0; i<7; i++) {
                    if (policy.mScanPoliy.mTimeScan.mWeeks>>i&0x1) {
                        week.push_back(i);
                    }
                }
                entry.SetCrontabDayInWeek(week);
                if (0 == SetScheduleEntry(entry)) {
                    changed = true;
                }
            }
            else {
                 DeleteScheuleType(ScheduleEntry::LINUX_SCAN);
            }

            if (policy.mExcludePaths.mState > 0) {
                std::list<WhiteListEntry> whitelist;
                GetWhiteList(whitelist);

                std::list<std::string>::iterator iter;
                for (iter=policy.mExcludePaths.mPaths.begin();iter!=policy.mExcludePaths.mPaths.end();++iter) {
                    bool found = false;
                    std::list<WhiteListEntry>::iterator it;
                    for (it=whitelist.begin();it!=whitelist.end();++it) {
                        if (it->mPath == *iter) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        WhiteListEntry entry;
                        entry.mListType = WhiteListEntry::WHITE;
                        entry.mBlockType = WhiteListEntry::FILE_SCAN;
                        entry.mPathType = WhiteListEntry::FILE_PATH;
                        entry.mPath = *iter;
                        int id = AddWhiteList(entry);
                        if (id > 0) {

                        }
                    }
                }
            }

            ScanConfig scanConfig;
            if (0 == GetScanConfig(scanConfig)) {
                scanConfig.mCompressSize = policy.mScanPoliy.mCompressSize.mSize;
                scanConfig.mVirusAction = (VIRUS_DEAL)policy.mScanPoliy.mDealVirusType;
                scanConfig.mCleanFailedAction = (CLEAN_FAILED)policy.mScanPoliy.mCleanFaild;
                scanConfig.mBackupFailedAction = (VIRUS_BACKUP_FAILED)policy.mScanPoliy.mBackupType.mFailMode;

                SaveScanConfig(scanConfig);
            }
        }
    }
    return changed?1:0;
}

int MainEngine::SaveCommandLog(int logtype, const string& flag, const string& desc) {
    CommEventLog log;
    log.mType = (CommEventLog::CommEvent)logtype;
    log.mDate = Utils::Date::GetTimeStamp();
    log.mTarget = "完成";
    log.mAction = flag;
    log.mExtend = desc;
    return mStorage->InsertCommEvent(log);
}

int MainEngine::ProcessCommand(CommandModel *model) {
    switch (model->mCmdId) {
    case CommandModel::QUICK_SCAN_START:
    case CommandModel::ALL_SCAN_START: {
        if (CommandModel::QUICK_SCAN_START == model->mCmdId) {
            SaveCommandLog(CommEventLog::COMMAND, "quick", "快速扫描");
        }
        else if (CommandModel::ALL_SCAN_START == model->mCmdId) {
            SaveCommandLog(CommEventLog::COMMAND, "full", "全盘扫描");
        }

        LOG_GJCZ("receive a use command %d", model->mCmdId);

        RemoteScaner *scaner = RemoteScaner::NEW(this);
        scaner->SetCommandModel(*model);
        scaner->SetCurrentUser(mNotifierUID);
        mScanTaskPool.push(scaner);
        mScanPoolCond->Signal();

        LOG_GJCZ("send the scan command to queue");
        break;
    }
    case CommandModel::QUICK_SCAN_STOP:
    case CommandModel::ALL_SCAN_STOP: {
        SaveCommandLog(CommEventLog::COMMAND, "stop", "停止查杀");

        if (!mApplication) {
            break;
        }
        switch(mApplication->AppType()) {
        case Application::REMOTE_COMMAND: {
            StopScan();

            Message msg(ASYNC_REPORT_SCAN_STOP, 0, 0);
            CommandModel *feed = new CommandModel;
            *feed = *model;
            msg.arg3 = feed;
            mAsyncPool->PostMessage(msg);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return 0;
}

int MainEngine::StartUpdate(int/**UPDATE_VIRUS_TYPE*/ type, const string& navigate) {
    if (NULL== mUpdateEngine) {
        mUpdateEngine = GJCZUpdateVirusEngine::NEW(mAppConfig->GetInstallPath(), FOLDER_RJJH_GJCZ, mAppConfig->GetProductType());
        mUpdateEngine->SetNotifyUID(mNotifierUID);
    }
    mUpdateEngine->SetObserver(this);
    mUpdateEngine->SetNavigateData((GJCZUpdateVirusEngine::UPDATE_VIRUS_TYPE)type, navigate);
    return mUpdateEngine->Start(mAppConfig->GetVlibVersion());
}


int MainEngine::OnPluginEvent(enum PluginEvent event, const char *mapping) {
    if (event == USB_ADD) {
        LOG_GJCZ("get USB insert event");
        AntiVirusSetting plug_set;
        mStorage->GetAntiVirusSetting(plug_set, AntiVirusSetting::PLUGIN_MONITOR);

        bool locked = false;
        int sel = mStorage->GetUSBPluginDefaultState(locked);
        if (sel == 0) {
            if (plug_set.mPluginMonitor.mPluginDeal.mValue == AntiVirusSetting::PluginMonitor::QUERY_USER_SCAN) {
                sel = SendToNotifer(CommunicateBundle::CONFIRM,
                                    CommunicateBundle::NOTIFY_USB_PLUGIN,
                                  "U盘监控",
                                  "发现U盘被插入;设备中可能带有病毒文件！;移动设备接入是病毒传播的主要来源之一，建议先查杀再使用！",
                                  "立即查杀",
                                  "暂不查杀");
            }
            else {
                sel = 2;
            }
        }

        int defaultVavlue = sel>>16;
        if (defaultVavlue > 0) {
            mStorage->SetUSBPluginDefaultState(defaultVavlue);
        }

        if (sel == 2) {
            DevicePluginChecker *checker = DevicePluginChecker::NEW(this);
            checker->SetMountPluginPath(mapping, plug_set.mPluginMonitor.mScanDepth.mValue);
            mScanTaskPool.push(checker);

            mScanPoolCond->Signal();
            return 0;
        }
    }
    else if (USB_REMOVED == event) {
        LOG_GJCZ("get remove USB event");
        ScanProxy *app = mScanTaskPool.front();
        if (app) {
            if (app->AppType() == ScanProxy::PLUGIN_CHECK) {
                LOG_GJCZ("check the usb removed, stop the scaner");
                SetApplication(app);
                app->StopScan();
            }
            return 0;
        }
    }
    return -1;
}

int MainEngine::OnCommandObserver(int event, void *args, void **result, int *outlen) {    
    int err = ServiceBinder::OnCommandObserver(event, args, result, outlen);

    if (PUBLIC_API_SCAN == event) {
        PublicScanData *inputData = (PublicScanData*)args;
        PublicScanResult **outData = (PublicScanResult**)result;

        *outData = new PublicScanResult;
        std::list<std::string>::iterator iter;
        for (iter=inputData->files.begin(); iter!=inputData->files.end(); ++iter) {
            PublicScanResult::ScanResult scan;
            scan.file = *iter;
            scan.hash = Utils::Hash::GetFileMD5(scan.file);

            scan.virus = SimpleScan(*iter);
            scan.hits = !scan.virus.empty();
            if (!scan.virus.empty()) {
                int pos = scan.virus.find(".");
                if (pos > 0) {
                    scan.type = scan.virus.substr(0, pos);
                }
            }
            (*outData)->results.push_back(scan);
        }
        return 0;
    }
    else if (CLIENT_COMMAND == event) {
        CommandBundle *bundle = (CommandBundle*)args;

        if (bundle) {
            return RespondCientCommand(*bundle, result, outlen);
        }
    }
    return err;
}

int MainEngine::AddQuarantineFileToWhitelist(const QuarantineLog& log) {    
    list<WhiteListEntry> files;
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::WHITE;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    entry.mPathType = WhiteListEntry::FILE_PATH;
    if (mStorage->GetWhiteList(files, entry) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=files.begin(); iter!=files.end(); ++iter) {
            if (iter->mPath == log.mSource && iter->mReserved1 == 0) {
                return 0;
            }
        }
    }

    entry.mListType = WhiteListEntry::WHITE;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    entry.mPathType = WhiteListEntry::FILE_PATH;
    entry.mPath = log.mSource;
    entry.mReserved1 = 0;

    return mStorage->InsertWhiteList(entry);
}

int MainEngine::RespondQuarantineCommand(CommandBundle::CommandExtendType type, const string &data) {
    Json::Reader reader;
    Json::Value json;
    reader.parse(data, json);

    string logstr = json["log"].asString();
    int trust = json["extend"].asInt();

    QuarantineLog log;
    if (log.Initialize(logstr)) {
        switch (type) {
        case CommandBundle::RESTORE_QUARANTINE_LOG: {
            if(0 == RestoreQuarantine(log)) {
                // 恢复的时候, 将其加至白名单
                if (trust > 0) {
                    if (0 == AddQuarantineFileToWhitelist(log)) {
                        // 通知GLTX也加上此白名单
                        CommandBundle command;
                        command.mCommandID = CommandBundle::REMOTE_COMMAND;
                        command.mIntegerValue = CommandBundle::TRUST_QUARANTINE_LOG;
                        command.mStringValue = log.mSource;

                        NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
                    }
                }
            }
        }
        case CommandBundle::DELETE_QUARANTINE_LOG: {
            return DeleteQuarantine(log);
        }
        }
    }
    return -1;
}

int MainEngine::RespondCientCommand(const CommandBundle &bundle, void** result, int* outlen) {
    switch (bundle.mCommandID) {
    case CommandBundle::CONTEXT_SCAN: {
        return ProcessRightKeyContextMenuScan(bundle.mStringValue);
    }
    case CommandBundle::SCAN_EVENT: {
        return RespondScanCommand(bundle);
    }
    case CommandBundle::QUARANTINE_EVENT: {
        return RespondQuarantineCommand(
                    (enum CommandBundle::CommandExtendType)bundle.mIntegerValue,
                    bundle.mStringValue
        );
    }
    case CommandBundle::REMOTE_COMMAND: {
        return RespondCenterCommand(
                    (enum CommandBundle::CommandExtendType)bundle.mIntegerValue,
                    bundle.mStringValue,
                    result,
                    outlen
        );
    }
    case CommandBundle::RELOAD_CONFIG: {
        return ReloadConfig();
    }
    case CommandBundle::DAEMON_OFFLINE_UPDATE: {
        return RespondUpdateCommand(
                    (enum CommandBundle::CommandExtendType)bundle.mIntegerValue,
                    bundle.mStringValue,
                    result,
                    outlen
                    );
    }
    case CommandBundle::QEURY_PATH_INFO: {
        string paths = QueryPathInfoJsonString(bundle.mStringValue);
        *result = (unsigned char*)new char[paths.size() + 1];
        strcpy((char*)*result, paths.c_str());
        *outlen = paths.size();
        return 0;
    }
    default:
        break;
    }
    return 0;
}


int MainEngine::RespondScanCommand(const CommandBundle &command) {
    switch (command.mIntegerValue) {
    case CommandBundle::QUICK_SCAN:
    case CommandBundle::FULL_SCAN: {
        if (mScanTaskPool.size() > 0) {
            ScanProxy *app = mScanTaskPool.front();
            if (LogEventEntry::GetScanType(app->ScanType()) == LogEventEntry::MANUAL_SCAN) {
                return mMultiEngine->Status();
            }
            else {
                return -100;
            }
        }

        LOG_GJCZ("GJCZ get user scan command");

        UserScaner *scaner = UserScaner::NEW(this);
        if (scaner && mScanPoolCond) {
            scaner->SetCurrentUser(mNotifierUID);
            scaner->SetScanParams(command.mIntegerValue, mNotifierUID);

            LOG_GJCZ("create UID(%d) scan task", mNotifierUID);

            mScanTaskPool.push(scaner);
            mScanPoolCond->Signal();
        }
        else {
            return -1;
        }
//        MemoryScaner *scaner = MemoryScaner::NEW(this);
//        if (scaner) {
//            mScanTaskPool.push(scaner);
//            mScanPoolCond->Signal();
//        }
        break;
    }
    case CommandBundle::CUSTOM_SCAN: {
        if (mScanTaskPool.size() > 0) {
            ScanProxy *app = mScanTaskPool.front();
            if (LogEventEntry::GetScanType(app->ScanType()) == LogEventEntry::MANUAL_SCAN) {
                return mMultiEngine->Status();
            }
            else {
                return -100;
            }
        }

        UserScaner *scaner = UserScaner::NEW(this);
        LOG_GJCZ("get user scan command and path is %p,%p,%s", scaner, mScanPoolCond, command.mStringValue.c_str());

        if (scaner && mScanPoolCond) {
            scaner->SetScanParams(command.mIntegerValue, mNotifierUID, command.mStringValue.c_str());
            mScanTaskPool.push(scaner);

            LOG_GJCZ("add the scan entry to pool");

            mScanPoolCond->Signal();
        }
        else {
            return -1;
        }
        break;
    }
    case CommandBundle::STOP_SCAN: {
        printf("receive stop signal\n");
        if (mScanTaskPool.size() > 0) {
            ScanProxy *app = mScanTaskPool.front();
            if (LogEventEntry::GetScanType(app->ScanType()) == LogEventEntry::MANUAL_SCAN) {
                StopScan();
            }
        }
        break;
    }
    case CommandBundle::INTELLIGENT_SCAN: {
        ScanProxy *app = mScanTaskPool.front();
        if (LogEventEntry::GetScanType(app->ScanType()) == LogEventEntry::MANUAL_SCAN) {
            ScanIntelligentParams params;
            params.Initilize(command.mStringValue);

            mMultiEngine->SetScanDynamicStatus(params.thread_count, params.cpuload_precent);
        }
        break;
    }
    case CommandBundle::CONTINUE_SCAN: {
        if (mScanTaskPool.size() > 0) {
            ScanProxy *app = mScanTaskPool.front();
            if (LogEventEntry::GetScanType(app->ScanType()) == LogEventEntry::MANUAL_SCAN) {
                mMultiEngine->Continue();
            }
        }
        break;
    }
    case CommandBundle::PAUSE_SCAN: {
        if (mScanTaskPool.size() > 0) {
            ScanProxy *app = mScanTaskPool.front();
            if (LogEventEntry::GetScanType(app->ScanType()) == LogEventEntry::MANUAL_SCAN) {
                mMultiEngine->Pause();
            }
        }
        break;
    }
    default:
        break;
    }
    return 0;
}

int MainEngine::RespondUpdateCommand(CommandBundle::CommandExtendType type, const string &data, void **result, int *outlen) {
    switch (type) {
    case CommandBundle::UPDATE_OFFLINE_VIRLIB:
        // offline update
        StartUpdate(GJCZUpdateVirusEngine::UPDATE_BY_OFFLINE, data);
        break;
    default:
        break;
    }
    return 0;
}

int MainEngine::RespondCenterCommand(CommandBundle::CommandExtendType type, const string &data, void **result, int* outlen) {
    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(data, json)) {
        return -1;
    }

    int ret = 0;
    switch (type) {
    case CommandBundle::REMOTE_COMMAND_START: {
        CommandModel *model = CommandModel::NEW(json, true);
        ret = ProcessCommand(model);
        SECURITY_FREE(model);
        break;
    }
//    case CommandBundle::POLICY_LINUX_CHANGED: {
//        PolicyModel *model = PolicyModel::NEW(json);
//        ret = ProcessLinuxPolicy(model, mStorage);
//        SECURITY_FREE(model);
//        break;
//    }
    case CommandBundle::POLICY_GLOBAL_CHANGED: {
        PolicyModel *model = PolicyModel::NEW(json);
        ret = ProcessGlobalPolicy(model, mStorage, model->mPolicyType);
        SECURITY_FREE(model);
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
    case CommandBundle::DEFEN_FILE_CHANGED: {
        LOG_GJCZ("get a defen file scaner");
        DefenQuickScan(json["file"].asString());
        return 0;
    }
    case CommandBundle::DEFEN_PROC_STARTED: {
        return DefenQuickScan(json["file"].asString());
    }
    case CommandBundle::REMOTE_COMMAND_UPDATE: {
        // respond the virus lib update event
        return StartUpdate(GJCZUpdateVirusEngine::UPDATE_BY_CENTER, data);
    }
    case CommandBundle::SCHEDULE_SCAN: {
        return RespondScheduleScan(atoi(data.c_str()));
    }
    default:
        break;
    }
    return ret;
}

int MainEngine::FinishRemoteScan(const string& id) {
    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_COMMAND_FINISHED;
    command.mStringValue = id;

    return NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ReportRemoteStartEvent(int type) {
    mAsyncPool->PostMessage(Message(ASYNC_REPORT_SCAN_LOG_START, type, 0));
    return 0;
}

int MainEngine::ProcessRightKeyContextMenuScan(const string& uri) {
    LOG_GJCZ("context menu uri %s", uri.c_str());
    string path;
    if (Utils::String::StartsWith(uri, "file://")) {
        path = Utils::Hash::UrlDecode(uri.substr(7));
    }
    else if (Utils::String::StartsWith(uri, "://")) {
        path = Utils::Hash::UrlDecode(uri.substr(3));
    }
    else {
        return -1;
    }

    LOG_GJCZ("context menu sacn %s", path.c_str());

    string prompt = "准备扫描;状态:正在准备扫描;路径: " + path;
    SendToNotifer(CommunicateBundle::INFO,
                  CommunicateBundle::NOTIFY_SCANING,
                  "文件扫描",
                  prompt);

    LOG_GJCZ("get a context file scaner");

    FolderScaner *scaner = FolderScaner::NEW(this);
    if (scaner) {
        scaner->SetFolderPath(path, 0);

        mScanTaskPool.push(scaner);
        mScanPoolCond->Signal();
    }

    return 0;
}


int MainEngine::OnVirusEvent(EngineEvent event, EngineType type, void *data) {
    int err = Controller::OnVirusEvent(event, type, data);
    if (ALL_FINISHED == type) {
        mCustomScan = false;

        if (LogEventEntry::GetScanType(mApplication->ScanType())
                == LogEventEntry::ACTIVE_DEFENCE) {

            VirusReportData *scan = (VirusReportData*)data;
            VirusInfoEntry entry;
            if (entry.Initialize(scan->mVirus)) {
                LOG_GJCZ("defen finish, report the virus log %s", entry.mOriginPath.c_str());

                entry.mReported = 1;
                AppendVirusLog(entry);

                if (mAppConfig->GetProductType() != 0 /** 0 mean center less, define in setup.h, enum SoftType */) {
                    LOG_GJCZ("reporting...");
                    ReportVirusLog(mApplication->ScanType(), entry);
                }
            }
        }
        else {
            // send scan log to server
            if (mAppConfig->GetProductType() != 0 /** 0 mean center less, define in setup.h, enum SoftType */) {
                VirusReportData *scan = (VirusReportData*)data;
                if (scan->mSavedLogId > 0) {
                    int scantype = mApplication->ScanType();

                    // 在此保存日志的用户发起者
                    LogEventEntry scanlog;
                    if (0 == mStorage->GetScanLogs(scanlog, scan->mSavedLogId)) {
                        if (scanlog.GetScanType() == LogEventEntry::MANUAL_SCAN) {
                            scanlog.mScanPath = Utils::OS::GetUserFromUID(mNotifierUID);
                            mStorage->UpdateScanLog(scanlog);
                        }
                    }

                    LOG_GJCZ("report the scan and virus log...");
                    ReportScanLog(mScanTaskID, scan->mSavedLogId);
                    mScanTaskID = 0;

                    //printf("spent time!---\n");
                    //sleep(1);
                    if (GetLogedVirus()) {
                        pair<MainEngine*, pair<int, int> > *params = new pair<MainEngine*, pair<int, int> >();
                        *params = make_pair(this, make_pair(scantype, scan->mSavedLogId));

                        pthread_t report_thread;
                        pthread_create(&report_thread, NULL, ReportVirusLogThread, params);
                        pthread_detach(report_thread);
                    }

                    //printf("spent time!---222\n");
                    LOG_GJCZ("report the scan and virus log... complete");
                }
            }
        }

        // when scan finish, then signal the pool
        LOG_GJCZ("Scan finished, Notify the pool");
        mScanTaskCond->Signal();
    }
    else if (CLOUD_VIRUS == event) {
        if (CLOUD_POST == type) {
            // get cloud request, post it to GLTX
            pair<const char*,  char*>* params = (pair<const char*, char*>*)data;

            int buffer_len = strlen(params->second);

            Json::Value json;
            json["url"] = string(params->first);
            json["data"] = string("q=") + params->second;

            Json::FastWriter writer;

            CommandBundle command;
            command.mCommandID = CommandBundle::REMOTE_COMMAND;
            command.mIntegerValue = CommandBundle::REMOTE_COMMAND_CLOUD_POST;
            command.mStringValue = writer.write(json);
            if (0 == NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID)) {
                // report success
                memset(params->second, 0, buffer_len);
                strcpy(params->second, command.mStringValue.c_str());
                return 0;
            }
        }
        else if (CLOUD_CHECK == type) {
            CommandBundle command;
            command.mCommandID = CommandBundle::REMOTE_COMMAND;
            command.mIntegerValue = CommandBundle::REMOTE_COMMAND_CLOUD_CHECK;
            command.mStringValue = string((char*)data);
            err = NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
            if (0 == err) {
                // report success
                strcpy((char*)data, command.mStringValue.c_str());
                return 0;
            }
        }
    }
    else if (VIRUS_JUDGE == event) {
        if (JUDGE_START == type) {
            VirusReportData *scan = (VirusReportData*)data;
            if (scan->mType == VirusReportData::SCAN_LOG) {
                if (rising_usb_path.empty() || !mCustomScan) {
                    return 0;
                }

                int a1 = scan->mPath.size();
                int b1 = rising_usb_path.size();

                if (memcmp(scan->mPath.c_str(),
                           rising_usb_path.c_str(),
                           a1>b1?b1:a1)) {
                    // copy file to USB
                    char temp[1024] = {0};
                    strcpy(temp, scan->mPath.c_str());
                    char *pointer = temp;
                    int i=0;
                    while(pointer[i]) {
                        if (pointer[i] == '/') {
                            pointer[i] = '_';
                        }
                        i++;
                    }

                    Utils::Path::CopyFile(
                                scan->mPath,
                                Utils::Path::JoinPath(rising_usb_path.c_str(), temp, NULL)
                                );

                    LOG_GJCZ("%s file hash %s",
                             scan->mPath.c_str(),
                             Utils::Hash::GetFileMD5(scan->mPath).c_str());

                }
            }
        }
    }

    return err;
}

int MainEngine::ReportScanLog(long taskid, int logId, int extend) {
    if (0 == taskid) {
        taskid = Utils::Date::GetTimeStamp();
    }

    LOG_GJCZ("report the scan  time");
    LogEventEntry entry;
    if (logId > 0) {
        if (mStorage->GetScanLogs(entry, logId)) {
            return -1;
        }
    }
    else {
        entry.mLogType = LogEventEntry::SCAN_LOG;
        entry.mLogExtend = extend;
        entry.mStatus = logId>0?LogEventEntry::FINISHED:LogEventEntry::SCANNING;
        entry.mStartTime = Utils::Date::GetTimeStamp();
        entry.mEndTime = entry.mStartTime;
    }
    Json::Value json;
    json["type"] = Json::Value((int)taskid);
    json["entry"] = entry.Externlize();

    Json::FastWriter writer;

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_COMMAND_SCANLOG;
    command.mStringValue = writer.write(json);

    LOG_GJCZ("report the scan");
    if (0 == NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID)) {
        // report success
        entry.mReported = 1;
        return mStorage->UpdateScanLog(entry);
    }

    LOG_GJCZ("report the scan  NotifyData complete");

    return -1;
}

int MainEngine::ReportVirusLog(int type, VirusInfoEntry& entry) {
    Json::Value json;
    json["type"] = type;
    json["entry"] = entry.Externlize();

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_COMMAND_VIRUSLOG;

    Json::FastWriter writer;
    command.mStringValue = writer.write(json);

    if (0 == NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID)) {
        // report success
        if (entry.mReported == 0) {
            entry.mReported = 1;
            mStorage->UpdateVirusInfo(entry);
        }
    }
    return 0;
}

void* MainEngine::ReportVirusLogThread(void *params) {
    LOG_GJCZ("start the ReportVirusLogThread thread");
#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "VirusLog");
    if(pthread_setname_np(pthread_self(), threadName)< 0)
    {
        LOG_GJCZ("pthread_setname_np set name:%s failue", threadName);
    }
#endif

    pair<MainEngine*, pair<int,int> > *reported = (pair<MainEngine*, pair<int,int> > *)params;

    MainEngine *engine = reported->first;
    engine->ReportVirusLog(reported->second.first, reported->second.second);

    delete reported;
    return NULL;
}

int MainEngine::ReportVirusLog(int type, int scan_logid) {
    Json::FastWriter writer;

    std::list<VirusInfoEntry> entries;
    if (mStorage->GetVirusLogs(entries, scan_logid) > 0) {
        std::list<VirusInfoEntry>::iterator iter;
        for (iter=entries.begin(); iter!=entries.end(); ++iter) {
            Json::Value json;
            json["type"] = type;
            json["entry"] = iter->Externlize();

            CommandBundle command;
            command.mCommandID = CommandBundle::REMOTE_COMMAND;
            command.mIntegerValue = CommandBundle::REMOTE_COMMAND_VIRUSLOG;
            command.mStringValue = writer.write(json);

            if (0 == NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID)) {
                // report success
                iter->mReported = 1;
                mStorage->UpdateVirusInfo(*iter);
            }
        }
    }
    return 0;
}

int MainEngine::InitUSBPluginMonitor() {
    if (mUSBChecker) {
        delete mUSBChecker;
    }

    string exec = Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, DAEMON_GJCZ_ENTRY, NULL);
    string mountpath = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, SDCARD_MOUNT_PATH, NULL);
    mUSBChecker = USBPluginChecker::NEW(this, mountpath, Utils::OS::GetUserFromUID(mNotifierUID), Utils::OS::GetFileBits(exec.c_str()));
    if (NULL != mUSBChecker) {
        mHealthStatus &= ~PLUGIN_DISABLED;
        LOG_GJCZ("USB check init OK");

        return 0;
    }

    LOG_GJCZ("USB check init failed");

    mHealthStatus |= PLUGIN_DISABLED;
    return -1;
}

int MainEngine::RepairProcStartMonitor() {
    return 0;
}

int MainEngine::RepairFileMonitor() {
    return 0;
}

int MainEngine::RepairNetfilter() {
    return 0;
}

int MainEngine::RepairMonitor() {
    if (mHealthStatus & PLUGIN_DISABLED) {
        if (InitUSBPluginMonitor() == 0) {
            mUSBChecker->Start();
        }
    }
    return 0;
}

//int MainEngine::QueryUserForResult(int event,
//                                   const string& file,
//                                   const string& virus) {

//    if (event == Application::QUERY_FIND_VIRUS) {
//        if (SCAN_AUTO_KILL == mVirusAutoType) {
//            return NOTIFY_CONFIRM_SELECT;
//        }
//        else if (SCAN_AUTO_IGNORE == mVirusAutoType) {
//            return NOTIFY_IGNORE_SELECT;
//        }

//        string prompt = "发现病毒!;病毒名称: " + virus +";文件路径: " + file;
//        int sel = SendToNotifer(CommunicateBundle::QUERY,
//                                CommunicateBundle::NOTIFY_VIRUS,
//                                  "文件扫描",
//                                  prompt,
//                                  "立即查杀",
//                                  "忽略"
//                                  );

//        int defaultCheck = sel>>16;
//        if (defaultCheck > 0) {
//            if ((int)(sel & 0xffff) == NOTIFY_IGNORE_SELECT) {
//                mVirusAutoType = SCAN_AUTO_IGNORE;
//            }
//            else if ((int)(sel&0xffff) == NOTIFY_CONFIRM_SELECT) {
//                mVirusAutoType = SCAN_AUTO_KILL;
//            }
//        }

//        // this return value must correct the TEFC define
//        return sel&0xffff;
//    }
//    return NOTIFY_IGNORE_SELECT;
//}

int MainEngine::OnUpdateEvent(int event, int type, void *data) {
    if (UPDATE_EVENT_VIRUS == event) {
        if (UPDATE_VIRUS_SUCCESS == type ||
                UDPATE_VIRUS_FAILED == type) {


            if (UPDATE_VIRUS_SUCCESS == type && data) {
                std::string *virver = (string*)data;
                LOG_GJCZ("change the config file, virus version is %s", virver->c_str());

                SetVirusVersion(*virver);
                mStorage->SetSettingValue(VLIB_UPDATE_TIME, Utils::Date::GetTimeStamp(), false);
            }

            CommandBundle command;
            command.mCommandID = CommandBundle::REMOTE_COMMAND;
            command.mIntegerValue = (UPDATE_VIRUS_SUCCESS==type)
                                            ?CommandBundle::REMOTE_UPDATE_VIRUS_SUCCESS
                                             :CommandBundle::REMOTE_UPDATE_VIRUS_FAILED;
            command.mStringValue = data?*(string*)data:"";

            LOG_GJCZ("notify GLTX");
            NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);

            if (UPDATE_VIRUS_SUCCESS == type) {
                std::string rmd = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, VIRUS_DB_FILE, NULL);
                chmod(rmd.c_str(), 0777);

                // TODO 在病毒库升级完成后, 应该做一次内存扫描, 以确定正在运行的没有病毒
                // 目前先把此处屏掉
//                    // scan finish, need a memory scan
//                    MemoryScaner *scaner = MemoryScaner::NEW(this);
//                    if (scaner) {
//                        mScanTaskPool.push(scaner);
//                        mScanPoolCond->Signal();
//                    }
            }
        }
        else if (UPDATA_VIRUS_RP == type) {
            const char *filename = (const char*)data;
            LOG_GJCZ("notify RJJH1, download rp file %s\n", filename);

            CommandBundle command;
            command.mCommandID = CommandBundle::REMOTE_COMMAND;
            command.mIntegerValue = CommandBundle::REMOTE_UPDATE_VIRUS_DOWNLAOD;
            command.mStringValue = filename;
            NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
        }
        else if (UPDATE_VIRUS_COMBINE == type) {
            const char *filename = (const char*)data;
            LOG_GJCZ("notify RJJH1, combine rp file %s\n", filename);

            CommandBundle command;
            command.mCommandID = CommandBundle::REMOTE_COMMAND;
            command.mIntegerValue = CommandBundle::REMOTE_UPDATE_VIRUS_COMBINE;
            command.mStringValue = filename;
            NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
        }
        else if (UPDATE_OFFLINE_VLIB == type || UPDATE_ERROR == type) {
            mNeedRJJHSaveConfig = true;

            if (UPDATE_OFFLINE_VLIB == type) {
                std::string *virver = (string*)data;

                SetVirusVersion(*virver);
                mAppConfig->SetVlibVersion(*(string*)data);
                mStorage->SetSettingValue(VLIB_UPDATE_TIME, Utils::Date::GetTimeStamp(), false);
                SaveCommandLog(CommEventLog::DEPLOY, "update", "升级;病毒库;"+GetVirusVersion()+";"+*virver);
            }

            if (mAppConfig->GetSubProductGUID() != SECRET_MACHINE_ID) {
                // not a secret machine, change the virus lib version
                mAppConfig->StoreConfig();
            }
            else {
                // notify the RAVRJJH0
                CommandBundle command;
                command.mCommandID = CommandBundle::RELOAD_CONFIG;
                command.mIntegerValue = mAppConfig->GetSubProductGUID() != SECRET_MACHINE_ID ? 0:1;
                command.mStringValue = mAppConfig->JsonString();
                NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
            }
            sleep(3);
            mNeedRJJHSaveConfig = false;

            {
                // notify the RAVRJJH2
                char sockname[128];
                sprintf(sockname, RJJH_MAJOR_SOCKET, mNotifierUID);
                std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

                CommandBundle command;
                command.mCommandID = CommandBundle::DAEMON_OFFLINE_UPDATE;
                command.mIntegerValue = UPDATE_OFFLINE_VLIB == type
                                            ? CommandBundle::UPDATE_OFFLINE_VIRLIB_SUCCESS
                                            : CommandBundle::UPDATE_OFFLINE_VIRLIB_ERROR;
                command.mStringValue = *(string*)data;

                int ret = 0;
                RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
                int err = bundle.Externalize(sock.c_str(), ret);
                if (0 == err) {
                    LOG_GJCZ("send to main screen %d", ret);
                }
                else {
                    LOG_GJCZ("sending error %d, main secreen not received", err);
                }
            }
        }
    }
    return 0;
}


//void MainEngine::UpdateCallback(UpdateEvent event, UpdateType type, void *data) {
//    if (UPDATE_EVENT_CHECK == event) {
//        if (UPDATE_ERROR == type) {
//            SaveCommandLog(CommEventLog::DEPLOY, "update", "升级;失败");

//            string prompt = "升级失败!;请联系管理员;失败错误码:" + Utils::String::itoa(*(int*)data);
//            SendToNotifer(CommunicateBundle::TOAST,
//                          CommunicateBundle::NOTIFY_MESSAGE,
//                          "瑞星杀毒软件_升级",
//                          prompt,
//                          "我知道了",
//                          ""
//                          );
//        }
//        else if (UDDATE_GET_NEW_VERSION == type) {
//            string prompt = "正在升级;最新版本: " + *(std::string*)data + ";请不要做任何操作";
//            SendToNotifer(CommunicateBundle::TOAST,
//                          CommunicateBundle::NOTIFY_MESSAGE,
//                          "瑞星杀毒软件_升级",
//                          prompt,
//                          "我知道了",
//                          ""
//                          );
//        }
//        else if (UPDATE_NEWEST == type) {
//            string prompt = "当前版本为最新!";
//            SendToNotifer(CommunicateBundle::TOAST,
//                          CommunicateBundle::NOTIFY_MESSAGE,
//                          "瑞星杀毒软件_升级",
//                          prompt,
//                          "我知道了",
//                          ""
//                          );
//        }
//    }
//    else if (UPDATE_EVENT_VIRUS == event) {
//        if (UPDATE_VIRUS_SUCCESS == type ||
//                UDPATE_VIRUS_FAILED == type) {
//            std::string *virver = (string*)data;
//            SetVirusVersion(*virver);

//            CommandBundle command;
//            command.mCommandID = CommandBundle::REMOTE_COMMAND;
//            command.mIntegerValue = (UPDATE_VIRUS_SUCCESS==type)
//                                            ?CommandBundle::REMOTE_UPDATE_VIRUS_SUCCESS
//                                             :CommandBundle::REMOTE_UPDATE_VIRUS_FAILED;
//            command.mStringValue = *virver;

//            if (0 == NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID)) {
//                if (UPDATE_VIRUS_SUCCESS == type) {
//                    SaveCommandLog(CommEventLog::DEPLOY, "update", "升级;病毒库;"+GetVirusVersion()+";"+*virver);

//                    std::string rmd = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, VIRUS_DB_FILE, NULL);
//                    chmod(rmd.c_str(), 0777);
//                }
//            }
//        }
//        else if (UPDATA_VIRUS_RP == type) {
//            const char *filename = (const char*)data;
//            LOG_GJCZ("download rp file %s\n", filename);
//        }
//        else if (UPDATE_VIRUS_COMBINE == type) {
//            const char *filename = (const char*)data;
//            LOG_GJCZ("combine rp file %s\n", filename);
//        }
//    }
//}

int MainEngine::RespondScheduleScan(int event) {

    LOG_GJCZ("add the scan entry to pool");
    ScheduleScaner *scaner = ScheduleScaner::NEW(this);
    scaner->mType = event;
    scaner->SetCurrentUser(mNotifierUID);

    SetApplication(scaner);

    mScanTaskPool.push(scaner);
    mScanPoolCond->Signal();
    return 0;
}

int MainEngine::ExitProc(const string& sock, int cid) {
    CommandBundle command;
    command.mCommandID = cid;

    int ret = 0;
    RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, command);
    if (pack.Externalize(sock.c_str(), ret) == 0) {
        return ret;
    }
    return -1;
}

int MainEngine::ClearLastState() {
#if 0
    string sock = Utils::Path::JoinPath(mInstallDir.c_str(), SOCKET_PATH, P2W_SOCKET, NULL);
    if (Utils::Path::FileAvailed(sock)) {
        unlink(sock.c_str());
    }
    sock = Utils::Path::JoinPath(mInstallDir.c_str(), SOCKET_PATH, DOMAIN_LISTEN_LOCAL_SOCKET, NULL);
    if (Utils::Path::FileAvailed(sock)) {
        unlink(sock.c_str());
    }
    sock = Utils::Path::JoinPath(mInstallDir.c_str(), SOCKET_PATH, D2N_SOCKET, NULL);
    if (Utils::Path::FileAvailed(sock)) {
        unlink(sock.c_str());
    }

    Utils::Path::mkdirs(Utils::Path::JoinPath(mInstallDir.c_str(), SOCKET_PATH, NULL).c_str());
#endif
    return 0;
}

int MainEngine::RespondDefenVirus(VirusInfoEntry &entry) {

    LOG_GJCZ("RespondDefenVirus the virus log %s begin!\n", entry.mOriginPath.c_str());
    if(Utils::Path::PathExists(entry.mOriginPath.c_str())){
        LOG_GJCZ("the virus %s not Exists!\n", entry.mOriginPath.c_str());
        return -1;
    }

    entry.mDealState = ProcessVirus(entry.mOriginPath, entry.mVirusName, entry.mQuarantine, *mDefenScanConfig);

    //LOG_GJCZ("save the virus log %s", entry.mOriginPath.c_str());
    if (AppendVirusLog(entry) >= 0) {
        LOG_GJCZ("save the virus log %s success!\n", entry.mOriginPath.c_str());

        // add log success, delete the origin file
        unlink(entry.mOriginPath.c_str());

        /** 0 mean center less, define in setup.h, enum SoftType */
        if (mAppConfig->GetProductType() != 0 ) {
         LOG_GJCZ("reporting %s and %s", entry.mOriginName.c_str(), entry.mVirusName.c_str());
         ReportVirusLog(LogEventEntry::SetScanType(
                            LogEventEntry::ACTIVE_DEFENCE,
                            LogEventEntry::MONITOR_FILE
                        ),

                        entry);
        }

        if (mDefenScanConfig->mFinishNotify) {
         string prompt = "已处理 1 个病毒!;病毒类型: " + entry.mVirusName + ";扫描路径:" + entry.mOriginPath;

         LOG_GJCZ(prompt.c_str());
         SendToNotifer(CommunicateBundle::TOAST,
                       CommunicateBundle::NOTIFY_SCAN_FINISHED,
                       "文件监控",
                       prompt,
                       "我知道了",
                       "");
        }
    }
    else{
        LOG_GJCZ("error save the virus log failue: %s!\n", entry.mOriginPath.c_str());
        unlink(entry.mOriginPath.c_str());
    }
    return 0;
}

int MainEngine::NotifyStatusToApp() {
    while (true) {
        Json::Value json;
        json["type"] = 2;
        json["status"] = 1;
        json["progress"] = 20;

            Json::Value summary;

                Json::Value summ;
                summ["tag"] = Json::Value(0);
                summ["file"] = Json::Value("/home/miwoo/test.fi");
                summ["count"] = Json::Value(100);
                summ["virus"] = Json::Value(1);

                summary.append(summ);

            json["summary"] = summary;


        Json::FastWriter writer;
        string result = writer.write(json);

        char sockname[128];
        sprintf(sockname, RJJH_MAJOR_SOCKET, mNotifierUID);
        std::string sock = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, sockname, NULL);

        LOG_GJCZ("send ............ %ld", Utils::Date::GetTimeStamp());

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

        LOG_GJCZ("send end ............ %ld", Utils::Date::GetTimeStamp());

        sleep((1));
    }
    return 0;
}

void MainEngine::HandleMessage(int event, int type, void *object) {
    ServiceBinder::HandleMessage(event, type, object);

    if (ASYNC_REPORT_SCAN_LOG_START == event) {
        mScanTaskID = Utils::Date::GetTimeStamp();
        ReportScanLog(mScanTaskID, -1, type);
    }
    else if (ASYNC_REPORT_SCAN_STOP == event) {
        CommandModel *model = (CommandModel*)object;

        CommandBundle command;
        command.mCommandID = CommandBundle::REMOTE_COMMAND;
        command.mIntegerValue = CommandBundle::REMOTE_COMMAND_FINISHED;
        command.mStringValue = model->mId;
        NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);

        delete model;
    }
    else if (ASYNC_REPORT_DEFEN_LOG == event) {
        char *report = (char*)object;
        VirusInfoEntry entry;
        if (entry.Initialize(report)) {
            RespondDefenVirus(entry);
        }
        delete[] report;
    }
    else if (1000 == event) {
        NotifyStatusToApp();
    }
}

int MainEngine::DefenQuickScan(const string &file) {
    if (!mDefenScanConfig) {
        return -1;
    }

    LOG_GJCZ("start the defen scan %s", file.c_str());

    SimpleJudger judger;
    if (ScanFile(file, &judger, *mDefenScanConfig, &mSingleEngine) == 0) {
#ifdef RISING_TEST_
        if (judger.mVirusName.empty()) {
            if (file.find("RAVRJJH99") != string::npos) {
                judger.mVirusName = "Adware.test.hacker.01";
            }
        }
#endif
        if(!judger.mVirusName.empty()) {
            LOG_GJCZ("%s is a virus %s", file.c_str(), judger.mVirusName.c_str());

            // it is a virus
            if (mDefenScanConfig->mVirusAction == AUTO_DEAL) {
                VirusInfoEntry entry;
                entry.mVirusName = judger.mVirusName;
                entry.mVirusType = -100;
                entry.mOriginPath = file;
                entry.mOriginName = Utils::Path::GetFileName(file);
                entry.mDealState = 0;
                entry.mTime = Utils::Date::GetTimeStamp();
                entry.mHash = Utils::String::ToUpper(Utils::Hash::GetFileMD5(file));
                entry.mFileSize = Utils::OS::GetFileSize(file);
                entry.mReported = 1; //default has reported

                //
//                sleep(1);

                Message msg(ASYNC_REPORT_DEFEN_LOG, 0, 0);
                string report = entry.Externlize();
                char *buffer = new char[report.size() + 1];
                strcpy(buffer, report.c_str());
                msg.arg3 = buffer;

                LOG_GJCZ("message hash is %ld", msg.Hash());

                mAsyncPool->PostMessage(msg);
            }
            else {
                // manully
                DefenScaner *scaner = DefenScaner::NEW(this);
                if (scaner) {
                    scaner->SetFolderPath(file, 1);

                    mScanTaskPool.push(scaner);
                    mScanPoolCond->Signal();
                }
            }
            return 1;
        }
    }
    return 0;
}

int MainEngine::GetDefenScanConfig(ScanConfig &config) {
    AntiVirusSetting setting;
    if (0 == GetAntiVirusConfig(setting, AntiVirusSetting::FILE_MONITOR)) {
        mDefenScanConfig->mScanFileType = (SCAN_TYPE)setting.mFileMonitor.mFileType.mValue;
        mDefenScanConfig->mVirusAction = (VIRUS_DEAL)setting.mFileMonitor.mVirusDeal.mValue;
        mDefenScanConfig->mScanCommonVirus = setting.mFileMonitor.mScanCommonVirus.mValue;
        mDefenScanConfig->mScanIntelligent = setting.mFileMonitor.mScanIntelligent.mValue;
        mDefenScanConfig->mScanCompressFile = setting.mFileMonitor.mZipScan.mValue;
        mDefenScanConfig->mCleanFailedAction = (CLEAN_FAILED)setting.mFileMonitor.mCleanFailed.mValue;
        mDefenScanConfig->mCompressSize = setting.mFileMonitor.mZipFileSize.mValue;
        mDefenScanConfig->mBackupVirusFile = (VIRUS_BACKUP)setting.mVirusBackup.mVirusBackup.mValue;

        mDefenScanConfig->mUseCache = false;
        mDefenScanConfig->mThreadCount = 1;
        mDefenScanConfig->mCloudAddr.clear();

        mDefenScanConfig->mFinishNotify = setting.mFileMonitor.mCleanPrompt.mValue;
        return 0;
    }
    return -1;
}

int MainEngine::ImportCrossUpdateInfo(string& version, string& guid, string& sguid) {
    string crossxml = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, CROSS_VERSION_LOG_FILE, NULL);
    if (!Utils::Path::FileAvailed(crossxml)) {
        return -1;
    }

    vector<CrossVersion::LOG_INFO> scanlogs, viruslog;
    CrossVersion cross;
    if (0 == cross.ImportLogs(crossxml, version, guid, sguid, scanlogs, viruslog)) {
        vector<CrossVersion::LOG_INFO>::iterator iter;
        for (iter=scanlogs.begin(); iter!=scanlogs.end(); ++iter) {
            LogEventEntry entry;
            entry.mLogExtend = LogEventEntry::SetScanType(
                        iter->info6=="REMOTE"?LogEventEntry::REMOTE_SCAN:LogEventEntry::MANUAL_SCAN,
                        LogEventEntry::ALL_DISK);
            entry.mStartTime = atoi(iter->info1.c_str());
            entry.mEndTime = atoi(iter->info2.c_str());
            entry.mVirusCount = atoi(iter->info3.c_str());
            entry.mKilledCount = atoi(iter->info4.c_str());
            entry.mScanCount = atoi(iter->info5.c_str());
            entry.mTotalCount = entry.mScanCount;
            mStorage->InsertScanLog(entry);
        }
        for (iter=viruslog.begin(); iter!=viruslog.end(); ++iter) {
            VirusInfoEntry entry;
            entry.mVirusName = iter->info1;
            entry.mDealState = atoi(iter->info2.c_str());
            entry.mTime = atoi(iter->info3.c_str());
            entry.mOriginPath = iter->info4;

            mStorage->InsertVirusInfo(entry);
        }
    }

    if (NULL != mQuaArea) {
        int count = mQuaArea->GetIndexCount();
        for (int i=0; i<count; ++i) {
            RAVQUAFILEINFO quainfo;
            if (mQuaArea->GetIndexItem(i, quainfo)) {
                QuarantineLog qlog;
                qlog.mDate = atoi(quainfo.szTime);
                qlog.mSource = Utils::Path::JoinPath(quainfo.szPath, quainfo.szName, NULL);
                qlog.mVirusName = quainfo.szVirusName;
                char temp[32];
                sprintf(temp, "%08X", quainfo.nIndex);
                qlog.mTarget = temp;
                qlog.mPermision = 0777;
                qlog.mSize = quainfo.nfileSize;
                mStorage->InsertQuarantine(qlog);
            }
        }
    }
    return 0;
}


string MainEngine::QueryPathInfoJsonString(const string& paths) {
    vector<string> folders;
    if (Utils::Path::GetDictDictionaryInfo(paths, folders)) {
        return "";
    }

    Json::Value json;
    vector<string>::iterator iter;
    for (iter=folders.begin(); iter!=folders.end();++iter) {
        vector<string> subfolders;
        string subpath = Utils::Path::JoinPath(paths.c_str(), iter->c_str(), NULL);

        Utils::Path::GetDictDictionaryInfo(subpath, subfolders);

        Json::Value subjson;
        vector<string>::iterator it;
        for (it=subfolders.begin(); it!=subfolders.end(); ++it) {
            subjson.append(Json::Value(*it));
        }
        json[*iter] = subjson;
    }
    Json::FastWriter writer;
    return writer.write(json);
}

int match_usb(const char *file, const char *name) {
    if (0 == strcmp(name, ".rising_test_file")) {
        string filename;
        Utils::Path::SplitPath(file, rising_usb_path, filename);
        return -1;
    }
    return 0;
}

int MainEngine::GetMyUSB() {
    const char * UPATH[] = {"/media", "/run/media"};

    for (int i=0; i<sizeof (UPATH)/sizeof(const char*); ++i) {
        if (Utils::Path::TravelDictionary(UPATH[i], match_usb) < 0) {
            break;
        }
    }
    return 0;
}


