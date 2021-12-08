#include "mainengine.h"
#include "storage.h"
#include "commonapi.h"
#include "appconfig.h"
#include "baseconfig.h"
#include "clientinfo.h"
#include "json/json.h"
#include "rserializer.h"
#include "net/httprequest.h"
#include "communicatebundle.h"
#include "models/policy/policymodel.h"
#include "models/policy/commandmodel.h"
#include "models/log/logdatascanevent.h"
#include "Common/updateappengine.h"
#include "Common/updatevirusengine.h"

#include "Entry/logevententry.h"
#include "Entry/virusinfoentry.h"
#include "Entry/whitelistentry.h"
#include "Entry/urlruleentry.h"
#include "Entry/netaccessentry.h"
#include "Entry/commeventlog.h"
#include "Entry/quarantinelog.h"
#include "Entry/commonentry.h"
#include "Entry/sourcelistentry.h"

#include "multivirusengine.h"
#include "setup.h"
#include "cross.h"
#include "regapi.h"
#include "unixrscfg.h"
#include "cinifile.h"
#include "handler.h"
#include "rslicense/rslicense.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <fstream>
#include <algorithm>

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;


#define ASYNC_REPORT_SCAN_STOP      10001

#define CONNECT_CENTER_MAX_RETRY    3

/** define the post message flag */
#define CHANGE_OTHRE_CENTER_ADDR    1
#define START_COMMUNICATOR          2
#define REPORT_RUA_LOG              3
#define CHECK_TRAY_PROCESS          4


//2021.10.20 by zhangdong
#define RSSUDO_EXECUTE				200

/** define in libravcommon.so */
extern "C" int decompress_7z(const char *zip, const char *outfolder);

extern int g_running_state;
using namespace Setup;


const char * GetUpdateErrorDescription(int  nErrorCode)
{
	const struct UpdateErrorInfo_t { const char * desc; int val1; int val2; }
	g_err[] = 
	{
		// 官网升级验证. 
		{ u8"授权过期",     -41, -41 }, { u8"序列号未注册", -42, -42 }, { u8"下载超过规定次数", -43, -43 }, 
		{ u8"用户被注销",   -44, -44 }, { u8"用户被暂停",   -45, -45 }, { u8"版本号不合法",     -46, -46 }, { u8"其它错误", -47, -47 }, 
		{ u8"授权验证失败", -50, -48 }, 
		// 
		{ u8"导航下载失败", -31, -31 }, 
		{ u8"导航解析失败", -40, -32 }, 
	};

	if(nErrorCode == 0) return 0;
	for(int i = 0; i < (int)(sizeof(g_err)/sizeof(g_err[0])); i++)
	{
		if(nErrorCode >= g_err[i].val1 && nErrorCode <= g_err[i].val2) return g_err[i].desc;
	}

	return 0;
}

int GetTypeFromOrigin(LogEventEntry::ScanType type,
                      LogEventEntry::DiskType disk,
                      LogDataScanEvent::LogScanType& scan,
                      LogDataScanEvent::CommandSender& sender) {
    switch (type) {
    case LogEventEntry::MANUAL_SCAN:
    case LogEventEntry::SPECIAL_FOLDER_SCAN:
        sender = LogDataScanEvent::USER_SCREEN;
        break;
    case LogEventEntry::REMOTE_SCAN:
        sender = LogDataScanEvent::REMOTE_CENTER;
        break;
    case LogEventEntry::TIMER_SCAN:
        sender = LogDataScanEvent::TIMER_SCHEDULE;
        break;
    case LogEventEntry::ACTIVE_DEFENCE:
        sender = LogDataScanEvent::FILE_MONITOR;
    default:
        break;
    }

    switch (disk) {
    case LogEventEntry::ALL_DISK:
        scan = LogDataScanEvent::ALL_SCAN;
        break;
    case LogEventEntry::QUICK_DISK:
        scan = LogDataScanEvent::QUICK_SCAN;
        break;
    case LogEventEntry::USER_DISK:
    case LogEventEntry::SPECIAL_FILE:
        scan = LogDataScanEvent::CUSTOM_SCAN;
        break;
    case LogEventEntry::MONITOR_FILE:
        scan = LogDataScanEvent::FILEMON_SCAN;
        break;
    default:
        break;
    }
    return 0;
}

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
    mCommunicator = NULL;
    mClientInfo = NULL;
    mCenterFinder = NULL;

    mConnectStatus = NOT_CONNECTED;
    mDisconnectTime = 0;
    mTempCommand = NULL;
    mUpdateAppEngine = NULL;
    mUpdateVirusEngine = NULL;
    mScheduleEngine = NULL;   
    mUpdateStep = UPDATE_STEP_INIT;
    mUpdateErrorCode = 0;
    mUpdateNeedRestart = false;

    mHealthStatus = HEALTHY;
    mLastAuthPromptTime = 0;

    mUpdateThread = 0;
    mUpdateLocker = NULL;
    mCheckThread = 0;
    mProxyThread = 0;
    mCenterFinderProxyState = false;
    mFinderProxyLocker = NULL;

    mCheckLogUpload = false;
    mUploadLogMutex = NULL;
}

MainEngine::~MainEngine() {
    SECURITY_FREE(mCenterFinder);
    SECURITY_FREE(mCommunicator);
    SECURITY_FREE(mClientInfo);
    SECURITY_FREE(mTempCommand);
    SECURITY_FREE(mUpdateAppEngine);
    SECURITY_FREE(mUpdateVirusEngine);
    SECURITY_FREE(mScheduleEngine);
    SECURITY_FREE(mUpdateLocker);
    SECURITY_FREE(mFinderProxyLocker);
    SECURITY_FREE(mUploadLogMutex);
}


int MainEngine::Construct() {
    SECURITY_CHECK(ServiceBinder::Construct(FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET), 0, -1);
    LOG_GLTX("create RJJH listener");

    mUploadLogMutex = MutexLocker::NEW();

//    // 修正问题，在此做一个强制性的修改，如果客户端端的function为127时，强行设为1
//    // TODO, 以后确认各功能无误后取消
//    if (mAppConfig->GetSubProductGUID() == "1AB67467-096C-4bea-B852-2CA73F6E854C"
//            || mAppConfig->GetSubProductGUID() == "962CF847-3D72-49d0-895A-16C91858A13D") {
////        mAppConfig->SetSubProductGUID("90DDA51B-33AB-4d94-8F04-0B980AD75CDE");
////        mAppConfig->SetAppType("linux:lcserver");
//        mAppConfig->SetSubComponents(3);
//        mAppConfig->SetFunctions(3);
//        mAppConfig->SetUpdateTime(Utils::Date::GetTimeStamp());
//    }
//    mAppConfig->StoreConfig();


    return 0;
}


int MainEngine::DelayConstruct() {
    LOG_GLTX("has received the RJJH, deconstruct");
    ServiceBinder::DelayConstruct();

    Utils::Path::CheckPath(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, NULL), 0777);
    SECURITY_CHECK(StorageBridge::Construct(mAppConfig->GetInstallPath(), FOLDER_RJJH_GLTX), 0, -1);

    LOG_GLTX("create the storage db");
    if (mStorage->IsFirstCreate() || 0 == mStorage->GetSettingsCount()) {
       // RJJH can init the rav.db. so in GLTX also init the db data
        ResetConfig(mStorage);

        // add deploy the record
        SaveCommandLog(CommEventLog::DEPLOY,
                       "Install",
                       "安装;软件;成功; ;" + mAppConfig->GetAppVersion() + ";", false);
        mCheckLogUpload = true;
    }

    // 在此备分一下版本的中心地址
    SetStorageCenterUrl(mAppConfig->GetCenterURL());
    SetStorageAppVersion(GetAppVersion());
    SetStorageVlibVersion(mAppConfig->GetVlibVersion());

    // 判断是否有跨版本的升级文件，如果有，则导入GUID， SGUID
    ImportCrossUpdateInfo();

    LOG_GLTX("reset the active state to db");
    if (!GetActivePowerOnState()) {
        ActiveDefenConfig config;
        if (0 == GetDefenConfig(config)) {
            config.mFileDefenState.mValue = 0;
            SaveDefenConfig(config);
        }
    }

    mClientInfo = new ClientInfo;
    string guid = mStorage->GetGUID();
    LOG_GLTX("get guid is %s", guid.c_str());
    if (guid.empty()) {
        MakeGUID(guid);
        mStorage->SetGUID(guid);
    }

    LOG_GLTX("GLTX GUID is %s", guid.c_str());

    mScheduleEngine = ScheduleEngine::NEW(mStorage);
    mScheduleEngine->SetObserver(this);
    ASSERT_ZERO(mScheduleEngine, -1);

    // check env
//    if (strcmp(mAppConfig->GetSubProductGUID().c_str(), SECRET_MACHINE_ID)) {
//        CheckEnvChange();
//    }

    // 初始化中心列表
    list<SourceListEntry> sources;
    mStorage->GetSourceList(sources, SourceListEntry::CENTER_PROXY_SOUCE);

    queue<SourceListEntry> empty;
    swap(empty, mCenterList);

    list<SourceListEntry>::iterator iter;
    for (iter=sources.begin(); iter!=sources.end(); ++iter) {
        mCenterList.push(*iter);
    }

    return 0;
}

bool MainEngine::RepairConfigFile() {
    // 配置文件被删除，在此进行修补
    string dbfile = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_GLTX, STORAGE_DB_FILE, NULL);
    if (Utils::Path::FileAvailed(dbfile)) {
        StorageBridge *storage = StorageBridge::NEW(RAV_DEFAULT_PATH, FOLDER_RJJH_GLTX);
        if (storage) {
            string url = storage->GetStorageCenterUrl();
            string version = storage->GetStorageAppVersion();

            if (url.empty() || version.empty()) {
                return false;
            }

            AppConfig *config = AppConfig::MakeGeneralConfig(
                        Utils::Path::FileAvailed(Utils::Path::JoinPath("/etc", CONFIG_DEF_FILE, NULL)),
                        url, version);
            if (config) {
                delete config;
                return true;
            }
        }
    }
    return false;
}

int MainEngine::Start() {   
    ServiceBinder::Start();

    mScheduleEngine->Start();
    if (0 == mAppConfig->GetProductType()) {
        // mean single
        return 0;
    }
    else {
        mCurrentCenter = mAppConfig->GetCenterURL();
        if (mCurrentCenter.empty()) {
            return -1;
        }
        if (1 == mAppConfig->GetProductType()) {
            // 1 mean ESM, define is setup.h
            mCurrentCenter += string(CENTER_ESM_URL);
        }
        else if (2 == mAppConfig->GetProductType()) {
            // 2 mean SOC
            mCurrentCenter += string(CENTER_SOC_URL);
        }
        else {
            return 0;
        }

        if (!mCommunicator && !mCurrentCenter.empty()) {
            if (0 == InitCommunication(mCurrentCenter)) {
                mCommunicator->Start();
            }
        }
    }

    return 0;
}


int MainEngine::MakeGUID(string &guid) {
    LOG_GLTX("make the guid");

    string d_uid;
    string b_uid;
    string m_uid;

    mClientInfo->GetDiskUUID(d_uid);
    LOG_GLTX("get the disk uuid:%s", d_uid.c_str());

    mClientInfo->GetBiosUUID(b_uid);
    LOG_GLTX("make bois uuid:%s", b_uid.c_str());

    m_uid = mClientInfo->GetDescription();
    LOG_GLTX("make custom uuid:%s", m_uid.c_str());

    string hash = Utils::Hash::GetMD5(d_uid + b_uid + m_uid);
    guid = hash.substr(0, 8)
                    + "-" + hash.substr(8, 4)
                    + "-" + hash.substr(12, 4)
                    + "-" + hash.substr(16, 4)
                    + "-" + hash.substr(20, 12);
    return 0;
}
int MainEngine::Stop() {
    ServiceBinder::Stop();


    if (mScheduleEngine) {
        mScheduleEngine->Stop();
    }
    if (mCommunicator) {
        mCommunicator->Stop();
    }

    if (mCenterFinder) {
        mCenterFinder->Stop();
    }

    return 0;
}

void* MainEngine::Schedule(void *args) {
    MainEngine *self = (MainEngine*)args;
    while (self->mStatus) {
        self->mScheduleEngine->Work();
        sleep(60);
    }
    return NULL;
}


int MainEngine::Uninstall() {
    if (mCommunicator) {
        mCommunicator->ReportUninstall();
    }
    return 0;
}

int MainEngine::InitCommunication(const string& center_url) {
	CheckDuplicateSGUID();

    SECURITY_FREE(mCommunicator);
    mCommunicator = Communicator::NEW(center_url,
                           mAppConfig->GetEID(),
                           mStorage->GetGUID(),
                           mStorage->GetSGUID(),
                           GetAppVersion(),
                           mAppConfig->GetAppType());

    if (NULL != mCommunicator) {
        mCommunicator->setObserver(this);
        mHealthStatus &= ~CENTER_DISABLED;

        LOG_GLTX("GLTX Communicator ok");

        return 0;
    }

    mHealthStatus |= CENTER_DISABLED;
    return -1;
}

int MainEngine::FindNeighbors() {
    mConnectStatus = ASK_NEIGHBORS;

    if (!mCenterFinder) {
        mCenterFinder = CenterFindProxy::NEW(this, mClientInfo->IP);
    }
    if (mCenterFinder) {
        mCenterFinder->SetEID(mAppConfig->GetEID());
        mCenterFinder->StartAsk();
    }
    return 0;
}

int MainEngine::SaveCommandLog(int logtype, const string& flag, const string& desc, bool reported) {
    CommEventLog log;
    log.mType = (CommEventLog::CommEvent)logtype;
    log.mDate = Utils::Date::GetTimeStamp();
    log.mTarget = "完成";
    log.mAction = flag;
    log.mExtend = desc;
    if (reported) {
        log.mReserved1 = "1";
    }
    return mStorage->InsertCommEvent(log);
}


int MainEngine::OnCommandObserver(int event, void *args, void **result, int *outlen) {
    int err = ServiceBinder::OnCommandObserver(event, args, result, outlen);

    if (CLIENT_COMMAND == event) {
        CommandBundle *bundle = (CommandBundle*)args;

        if (bundle) {
            return RespondClientCommand(*bundle, result, outlen);
        }
    }
    return err;
}

int MainEngine::StartUpdateFunc(bool offline, const string& uri) {
    if (mUpdateStep == UPDATE_STEP_INIT) {
        if (mUpdatePrompt) {
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "提示", "正在升级请稍候...");
        }

        mUpdateLocker = CondLocker::NEW();
        int center = GetConnectCenter(offline?SHARE_FOLDER:OTHER_HTTP, uri);
        if (center <= 0) {
            SendToNotifer(CommunicateBundle::TOAST,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "提示", "升级失败;获取升级地址失败;");
            LOG_GLTX("Update failed, invalid center address.\n\n");
            return -1;
        }

        return pthread_create(&mUpdateThread, NULL, UpdateThread, this);
    }
    return -1;
}

void* MainEngine::UpdateThread(void *params) {
    MainEngine *self = (MainEngine*)params;
    self->UpdateThreadFunc();
    return NULL;
}

void MainEngine::UpdateThreadFunc() {
    LOG_GLTX("start updating-----------------");

    string log;
    vector<std::pair<UPDATE_SOURCE, std::string> >::iterator iter;
    for (iter=mCenterAddr.begin(); iter!=mCenterAddr.end(); ++iter)
    {
        mUpdateState.clear();

        if (SHARE_FOLDER == iter->first) {
            // 离线升级采用esm的病毒库路径
            string sourcefile = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), iter->second.c_str(), NULL);
            string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
            Utils::Path::CheckPath(folder);
            int err = decompress_7z(sourcefile.c_str(), folder.c_str());
            if (err) {
                continue;
            }
        }

		// 先APP更新, 再病毒库. 因为单机要验证授权. 
        LOG_GLTX("start app/virus use %s", iter->second.c_str());
        if (!mAppConfig->SecretMachine() && !mUpdateState[UPDATE_STEP_APP]) {
            // 非保密机检查客户端的升级
            LOG_GLTX("start app use %s", iter->second.c_str());

            int err = StartUpdate(UPDATE_STEP_APP, *iter);
            if (0 == err) {
                mUpdateLocker->Wait();
            }

			if(mUpdateErrorCode >= -50 && mUpdateErrorCode <= -41) {
				// 升级授权验证失败. 发送给界面消息, 
				// 但是好像没有这个接口, 所以是 FOLDER_RJJH_GJCZ. 
				LOG_GLTX("update accredit validate failed --->%d", mUpdateErrorCode);
				SaveUpgradeInfo(AppConfig::SERIAL_NUMBER, "??????");
            	break;
            }
        }

        if (!mUpdateState[UPDATE_STEP_VIRUS_LIB] && 0 == StartUpdate(UPDATE_STEP_VIRUS_LIB, *iter)) {
            mUpdateLocker->Wait();
        }

        if ((mUpdateState.size() == 2 && mUpdateState[UPDATE_STEP_APP]) ||
                (mUpdateState.size() == 1 && mUpdateState[UPDATE_STEP_VIRUS_LIB])) {
            break;
        }
    }
    LOG_GLTX("update finished, exit thread, %d", mUpdatePrompt);
    std::string GltxTmpFolder = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GLTX, UPDATE_VIRUS_FOLDER, NULL);
    Utils::Path::DeleteFolder(GltxTmpFolder.c_str());

	// 增加发送升级结果给 RJJH2. 
	CommandBundle command;
	command.mCommandID = CommandBundle::UPDATE_FINISH_NOTIFY;
	command.mIntegerValue = mUpdateErrorCode;
	NotifyData(mInstallDir, FOLDER_RJJH_GJCZ, RJJH_MAJOR_SOCKET, command, false, mNotifierUID);

    if (mUpdatePrompt) {
        if ((mUpdateState.size() == 2 && mUpdateState[UPDATE_STEP_APP]) ||
                (mUpdateState.size() == 1 && mUpdateState[UPDATE_STEP_VIRUS_LIB])) {
            SendToNotifer(CommunicateBundle::TOAST,
                      CommunicateBundle::NOTIFY_UPDATE_FINISHED,
                      "消息通知",
                      "升级完成",
                      "我知道了",
                      "");
        }
        else {
            if (0 == mUpdateErrorCode) {
                SendToNotifer(CommunicateBundle::TOAST,
                              CommunicateBundle::NOTIFY_MESSAGE,
                              "升级",
                              "已是最新版本;当前版本为最新版本，不需要升级!");
            }
            else {
                string prompt = "升级失败!;请联系管理员;失败错误码:" + Utils::String::itoa(mUpdateErrorCode);
                const char * desc = GetUpdateErrorDescription(mUpdateErrorCode);
                if(desc) prompt += desc;
                SendToNotifer(CommunicateBundle::TOAST,
                              CommunicateBundle::NOTIFY_MESSAGE,
                              "升级",
                              prompt);
            }
        }
    }

    if (mTempCommand && mCommunicator) {
        LOG_GLTX("report the task state");

        mCommunicator->ReportCommandLog (
                    mAppConfig->GetSubProductGUID(),
                    "update",
                    mTempCommand->mId,
                    0, 0);
    }

    LOG_GLTX("update success, restart the app\n");
    if (mUpdateNeedRestart && mUpdateState[UPDATE_STEP_APP]) {
        std::string update = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),
                                                   DEPLOYED_BIN_FOLDER,
                                                   UPDATE_PROCESS_ENTRY, NULL);
        if (mAppConfig->GetAppVersion() == "3.0.0.1") {
            // change the Update time,
            // TODO 临时操作，在3.0.0.1的版本，修改版本没有写到RAVGLTX2里，所以再此修改
            // 风险: 如果后续代码出了错，升级未完民，但是版本已经修改了
            SaveUpgradeInfo(AppConfig::APP_VERSION, mUpdateAppEngine->CurrentNewVersion());
        }
        LOG_GLTX("start the exec command");
        pid_t pid;
        if ((pid=fork()) == 0) {
            char uid[32] = {0};
            sprintf(uid, "%d", mNotifierUID);
            if(execl(update.c_str(), UPDATE_PROCESS_ENTRY, "-r", "-v", mUpdateAppEngine->CurrentNewVersion().c_str(), "-i", uid, NULL)<0)  {
                //
            }
        }
    }
    else {
        if (mUpdateState[UPDATE_STEP_VIRUS_LIB]) {
            // 如果只更新了病毒库,也要改变更新时间
            LOG_GLTX("update virus lib success, change update time");

            mAppConfig->SetUpdateTime(Utils::Date::GetTimeStamp());

            LOG_GLTX("notify the rjjh0 to update version %s", mUpdateVirusEngine->CurrentNewVersion().c_str());
            SaveUpgradeInfo(AppConfig::VLIB_VERISON, mUpdateVirusEngine->CurrentNewVersion());
        }
        if (mUpdateState[UPDATE_STEP_APP] || mUpdateState[UPDATE_STEP_VIRUS_LIB]) {
            if (mCommunicator) {
                LOG_GLTX("report the client info after update");
                mCommunicator->ReportClientInfo();
            }
        }
    }

    string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
    if (Utils::Path::PathExists(folder)) {
        Utils::Path::DeleteFolder(folder.c_str());
    }
    mUpdateStep = UPDATE_STEP_INIT;
    SECURITY_FREE(mUpdateAppEngine);
    SECURITY_FREE(mUpdateVirusEngine);    
}

int MainEngine::RespondClientCommand(const CommandBundle &bundle, void **result, int *outlen) {
    switch (bundle.mCommandID) {
    case CommandBundle::DAEMON_EXIT: {
        g_running_state = 0;
        exit(0);
    }
    case CommandBundle::CENTERLESS_REGISTER: {
        if (bundle.mIntegerValue == CommandBundle::CHECK_CLIENT_REGISTER) {
            int nRet = CheckRegisted(mAppConfig->GetSerialNumber(), 0);
			LOG_GLTX("CommandBundle CENTERLESS_REGISTER1 -->%d", nRet);
			return nRet;
        }
        else if (bundle.mIntegerValue == CommandBundle::REGISTER_USE_SERIAL) {
        	std::string  serial = bundle.mStringValue;
        	const char * p = strchr(bundle.mStringValue.c_str(), '|');
        	if(p) serial.assign(bundle.mStringValue.c_str(), p-bundle.mStringValue.c_str());

            int nRet = CheckRegisted(serial, p?p+1:0);
            if (nRet == 0) SaveUpgradeInfo(AppConfig::SERIAL_NUMBER, serial);
			LOG_GLTX("CommandBundle CENTERLESS_REGISTER2 -->%d", nRet);
            return nRet;
        }
    }
    case CommandBundle::DAEMON_CENTER_UPDATE: {
        mUpdatePrompt = true;
        return StartUpdateFunc(false);
    }
    case CommandBundle::DAEMON_OFFLINE_UPDATE: {
        mUpdatePrompt = true;
        return StartUpdateFunc(true, bundle.mStringValue);
    }
    case CommandBundle::UNINSTALL_REPORT: {
        if (mCommunicator) {
            return mCommunicator->ReportUninstall();
        }
        break;
    }    
    case CommandBundle::RELOAD_CONFIG: {
        LOG_GLTX("CommandBundle RELOAD_CONFIG\n");
        ReloadScheduleList();

        if (mAppConfig->SecretMachine()) {
            if (!bundle.mStringValue.empty()) {
                if (bundle.mStringValue != mAppConfig->GetCenterURL()) {
                    SourceListEntry entry;
                    entry.mType = SourceListEntry::CENTER_PROXY_SOUCE;
                    entry.mAddress = bundle.mStringValue;
                    mCenterList.push(entry);
                    mAsyncPool->PostMessage(Message(CHANGE_OTHRE_CENTER_ADDR, 0, 0));
                }
            }
        }
        else {
            list<SourceListEntry> sources;
            mStorage->GetSourceList(sources, SourceListEntry::CENTER_PROXY_SOUCE);

            queue<SourceListEntry> empty;
            swap(empty, mCenterList);

            list<SourceListEntry>::iterator iter;
            for (iter=sources.begin(); iter!=sources.end(); ++iter) {
                mCenterList.push(*iter);
            }

            mAsyncPool->PostMessage(Message(CHANGE_OTHRE_CENTER_ADDR, 0, 0));
        }

        break;
    }
    case CommandBundle::REPAIR: {
        return RespondRepairCommand(bundle, result, outlen);
    }
    case CommandBundle::REMOTE_COMMAND: {
        return RespondRemoteCommand(bundle, result, outlen);
    }
	case RSSUDO_EXECUTE: {
        if(!bundle.mStringValue.empty()) {
            system(bundle.mStringValue.c_str());
			printf("root exec\n");
        }
    	return 0;
    }
    }

    return 0;
}

int MainEngine::HttpDownload(const string& url, void **result, int *outlen) {
    if (mUpdateVirusEngine) {
        string::size_type lastSlash = url.rfind("/");
        string fileName = url.substr(lastSlash+1);
        string file =  Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),\
                        FOLDER_RJJH_GLTX, UPDATE_VIRUS_FOLDER, fileName.c_str(), NULL);
        string tmpFile = file + ".tmp";

        //downloading
        if(Utils::Path::FileAvailed(tmpFile)){
            return 0;
        }
        //start downloading
        else if (!Utils::Path::FileAvailed(file)){
            file = mUpdateVirusEngine->DownloadRPFile(url);
        }

        //download completed
        if (Utils::Path::FileAvailed(file)) {
            FILE *f = fopen(file.c_str(), "rb");
            if (f) {
                fseek(f, 0, SEEK_END);
                long size = ftell(f);
                *outlen = size;
                fseek(f, 0, SEEK_SET);
                *result = new unsigned char[size];
                int readed = 0;
                do {
                    readed += fread((unsigned char*)(*result)+readed, 1, size-readed, f);
                }while(readed<size);
                fclose(f);
            }
        }
        else if(strcmp(file.c_str(), "") == 0){
            *outlen = 6;
            *result = new char[6];
            strcpy((char *)*result, "ERROR");
            return -1;
        }
    }
    return 0;
}

int MainEngine::ClearLastState() {    

    return 0;
}

int MainEngine::CloudPostProxy(const string& data, void **result, int *outlen) {
    Json::Value json;
    Json::Reader reader;
    if (reader.parse(data, json)) {
        string header, body, conn;
        if (0 == http_post(json["url"].asString().c_str(), "", json["data"].asString().c_str(), header, body, conn)) {
            if (body.size() > 0) {              
                *outlen = body.size();
                *result = new char[(*outlen) + 1];
                strcpy((char*)(*result), body.c_str());
                ((char*)(*result))[(*outlen)] = '\0';
                return 0;
            }
        }
    }
    return -1;
}

int MainEngine::CloudPostCheck(const string& data, void **result, int *outlen) {
#define CHECK_STATUS_   "cloud.html"
#define NAVAGET_XML     "navigate.xml"

    string out;
    string url = Utils::Path::JoinPath(data.c_str(), CHECK_STATUS_, NULL);
    string header, body, info;
    if (0 == http_get(url, "", header, body, info)) {
        url = Utils::Path::JoinPath(data.c_str(), NAVAGET_XML, NULL);
        body.clear();
        if (0 == http_get(url, "", header, body, info)) {
            if (body.size() > 0) {
                *outlen = body.size();
                *result = new char[(*outlen) + 1];
                strcpy((char*)(*result), body.c_str());
                ((char*)(*result))[(*outlen)] = '\0';
                return 0;
            }
        }
    }
    return -1;
}

int MainEngine::ProcessReportScanLog(const string& data) {
    Json::Reader reader;
    Json::Value json;
    if (reader.parse(data, json)) {
        int taskid = json["type"].asInt();
        std::string tmp = json["entry"].asString();

        LogEventEntry entry;
        if (entry.Initialize(tmp)) {
            LogEventEntry::DiskType disk = entry.GetDiskType();
            LogEventEntry::ScanType scan = entry.GetScanType();

            LogDataScanEvent::LogScanType logtype;
            LogDataScanEvent::CommandSender sender;

            GetTypeFromOrigin(scan, disk, logtype, sender);
            if (mCommunicator) {
                return mCommunicator->ReportScanLog(logtype, sender,
                                             Utils::Date::FormatTimeStamp(entry.mStartTime),
                                             taskid,
                                             entry.mEndTime-entry.mStartTime,
                                             entry.mScanCount,
                                             entry.mVirusCount,
                                             entry.mKilledCount,
                                             entry.mScanCount,
                                             entry.mScanCount,
                                             entry.mStatus==LogEventEntry::FINISHED?LogDataScanEvent::LOG_STATE_FINISHED:LogDataScanEvent::LOG_STATE_RUNNING);
            }
            else {
                return 0;
            }
        }
    }
    return -1;
}

int MainEngine::ProcessReportVirusLog(const string& data) {
    Json::Reader reader;
    Json::Value json;
    if (reader.parse(data, json)) {
        int type = json["type"].asInt();
        std::string tmp = json["entry"].asString();

        LOG_GLTX("GLTX report virus log");

        LogEventEntry::DiskType disk = (LogEventEntry::DiskType)(type & 0xffff);
        LogEventEntry::ScanType scan = (LogEventEntry::ScanType)(type>>16 & 0xffff);

        LogDataScanEvent::LogScanType logtype;
        LogDataScanEvent::CommandSender sender;


        GetTypeFromOrigin(scan, disk, logtype, sender);

        VirusInfoEntry entry;
        if (entry.Initialize(tmp)) {
            int tread = 0, state = 0;
            switch (entry.mDealState) {
            case VirusReportData::STATUS_DEFAULT: {
                tread = 1;
                state = 1;
                break;
            }
            case VirusReportData::STATUS_CLEAN: {
                tread = 2;
                state = 1;
                break;
            }
            case VirusReportData::STATUS_REMOVE: {
                tread = 2;
                state = 2;
            }
            default:
                break;
            }
            if (mCommunicator) {
                return mCommunicator->ReportVirusLog(logtype, sender,
                                                       entry.mTime,
                                                       entry.mOriginPath,
                                                       entry.mID,
                                                       entry.mVirusName,
                                                       tread,
                                                       state,
                                                       entry.mHash);
            }
            else {
                return 0;
            }
        }
    }
    return -1;
}

void MainEngine::HandleMessage(int event, int type, void *object) {
    ServiceBinder::HandleMessage(event, type, object);

    switch (event) {
    case CHANGE_OTHRE_CENTER_ADDR: {

        LOG_GLTX("change center addr\n");
        // change other center
        if (!mCenterList.empty()) {
            SourceListEntry& entry = mCenterList.front();
            mCurrentCenter = entry.mAddress;

            SaveUpgradeInfo(AppConfig::CENTER_URL, mCurrentCenter);

            if (1 == mAppConfig->GetProductType()) {
                // 1 mean ESM, define is setup.h
                mCurrentCenter += string(CENTER_ESM_URL);
            }
            else if (2 == mAppConfig->GetProductType()) {
                // 2 mean SOC
                mCurrentCenter += string(CENTER_SOC_URL);
            }

            LOG_GLTX("change center current center addr:%s\n", mCurrentCenter.c_str());
            mStorage->SetSGUID("");
            InitCommunication(mCurrentCenter);
            mCommunicator->Start();
            mCenterList.pop();
        }
        break;
    }
    case START_COMMUNICATOR: {
        if (0 != mAppConfig->GetProductType()) {
            mCurrentCenter = mAppConfig->GetCenterURL();
            if (!mCommunicator) {
                InitCommunication(mCurrentCenter);
                mCommunicator->Start();
            }
        }
        break;
    }
    case REPORT_RUA_LOG: {
        pthread_create(&mUploadLogThread, NULL, UploadLogThreadFunc, this);
        break;
    }
    case CHECK_TRAY_PROCESS: {
        string tray_proc = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL);
        LOG_GLTX("HandleMessage CHECK_TRAY_PROCESS process %s ", tray_proc.c_str());

        int retry = 0;
        do {
            LOG_GLTX("process %s, killing", tray_proc.c_str());
            Utils::OS::KillProc(tray_proc.c_str(), 9);
            usleep(1000*100);
        }
        while(Utils::OS::CheckProcessState(tray_proc.c_str()) && retry++ < 3);

        LOG_GLTX("start process %s, starting uid:%d ", tray_proc.c_str(), mNotifierUID);
        StartTrayProcess(true, CommandBundle::RESET_TRAY_PROCESS_SILENCE == type, mNotifierUID);
        usleep(1000*100);

        // 测试在ROOT状态下启动客户端进程， 在龙芯飞腾的机器上不需要登录，但是在X86的机器需要登录， 所以在此做尝试
        if (Utils::OS::CheckProcessState(tray_proc.c_str()) < 1) {
            LOG_GLTX("start process %s, start by root uid:%d ", tray_proc.c_str(), mNotifierUID);

            StartTrayProcess(false, CommandBundle::RESET_TRAY_PROCESS_SILENCE == type, mNotifierUID);
        }

        LOG_GLTX("CHECK_TRAY_PROCESS process %s end", tray_proc.c_str());
        break;
    }
    default:
        break;
    }
}

int MainEngine::RespondRemoteCommand(const CommandBundle& bundle, void **result, int *outlen) {
    LOG_GLTX("received the remote command %d\n", bundle.mIntegerValue);
    switch (bundle.mIntegerValue) {
    case CommandBundle::REMOTE_COMMAND_FINISHED: {
        if (mCommunicator) {
            return mCommunicator->ReportCommandLog(
                        mAppConfig->GetSubProductGUID(),
                        "scan",
                        bundle.mStringValue,
                        0, 0);
        }
        break;
    }
    case CommandBundle::REMOTE_COMMAND_SCANLOG: {
        if (mCommunicator) {
            return ProcessReportScanLog(bundle.mStringValue);
        }
        break;
    }
    case CommandBundle::REMOTE_COMMAND_VIRUSLOG: {
        if (mCommunicator) {
            return ProcessReportVirusLog(bundle.mStringValue);
        }
        break;
    }    
    case CommandBundle::REMOTE_UPDATE_VIRUS_SUCCESS: {
        LOG_GLTX("receive the update virus lib from GJCZ and version is %s", bundle.mStringValue.c_str());        
        if (!bundle.mStringValue.empty()) {
            mUpdateState[UPDATE_STEP_VIRUS_LIB] = true;

            int err = 0;
            if (mCommunicator) {
                err = mCommunicator->ReportRUALog(
                                        mUpdatePrompt?RUA_SOURCE_MANUAL_START:RUA_SOURCE_REMOTE_START,
                                        RUA_ACTION_UPDATE,
                                        RUA_ROLE_VIRLIB,
                                        mAppConfig->GetVlibVersion(),
                                        bundle.mStringValue,
                                        false
                            );
            }

            SaveCommandLog(CommEventLog::DEPLOY,
                           "update",
                           "升级;病毒库;成功;" + mAppConfig->GetVlibVersion() + ";" + bundle.mStringValue + ";" + "否", err==0);

            SaveUpgradeInfo(AppConfig::VLIB_VERISON, bundle.mStringValue);
            if (mCommunicator) {
                mCommunicator->ReportVlibVersion(bundle.mStringValue);
            }
        }

        mUpdateLocker->Signal();
        break;
    }
    case CommandBundle::REMOTE_UPDATE_VIRUS_FAILED: {
        mUpdateState[UPDATE_STEP_VIRUS_LIB] = false;
        mUpdateErrorCode = atoi(bundle.mStringValue.c_str());
        mUpdateLocker->Signal();
        break;
    }
    case CommandBundle::REMOTE_UPDATE_VIRUS_DOWNLAOD: {
        RespondUpdateVirus(UPDATE_STEP_VIRUS_LIB, UPDATA_VIRUS_RP, (void*)&bundle.mStringValue);
        break;
    }
    case CommandBundle::REMOTE_UPDATE_VIRUS_COMBINE: {
        RespondUpdateVirus(UPDATE_STEP_VIRUS_LIB, UPDATE_VIRUS_COMBINE, (void*)&bundle.mStringValue);
        break;
    }
    case CommandBundle::REMOTE_DEFEN_STATE_REPORT : {
        if (mCommunicator) {
            Json::Reader reader;
            Json::Value json;
            if (reader.parse(bundle.mStringValue, json)) {
                mCommunicator->ReportDefenState(json["flag"].asString() , json["state"].asBool());
            }
        }
        break;
    }
    case CommandBundle::REMOTE_COMMAND_CLOUD_POST: {
        return CloudPostProxy(bundle.mStringValue, result, outlen);
    }
    case CommandBundle::REMOTE_COMMAND_CLOUD_CHECK: {
        return CloudPostCheck(bundle.mStringValue, result, outlen);
    }
    case CommandBundle::REMOTE_UPDATE_HTTP_DOWNLOAD: {
        return HttpDownload(bundle.mStringValue, result, outlen);
    }
    case CommandBundle::UNINSTALL_PROTECT: {
        string adminpass = GetAdminPasswd();
        if (!adminpass.empty()) {
            return 1;
        }
        return 0;
    }
    case CommandBundle::UNINSTALL_VERIFY: {
        string passwd = bundle.mStringValue;
        string adminpass = GetAdminPasswd();
        if (Utils::String::ToUpper(Utils::Hash::GetMD5(passwd)) == Utils::String::ToUpper(adminpass)) {
            return 1;
        }
        break;
    }
    case CommandBundle::DEPLOY_CROSS_SUCCESS: {
        int err = 0;
        if (mCommunicator) {
            err = mCommunicator->ReportRUALog(
                                mUpdatePrompt?RUA_SOURCE_MANUAL_START:RUA_SOURCE_REMOTE_START,
                                RUA_ACTION_UPDATE,
                                RUA_ROLE_REC,
                                mAppConfig->GetAppVersion(),
                                bundle.mStringValue,
                                false
                    );
        }
        SaveCommandLog(CommEventLog::DEPLOY,
                       "update",
                       "升级;软件;成功;" + mAppConfig->GetAppVersion() + ";" + bundle.mStringValue, err==0);
        break;
    }
    case CommandBundle::TRUST_QUARANTINE_LOG: {
        AddQuarantineFileToWhitelist(bundle.mStringValue);
        break;
    }
    default:
        break;
    }
    return 0;
}

int MainEngine::StartTrayProcess(bool shell_login, bool silence, uid_t uid) {
    string tray_command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFY_RJJH_COMMAND, NULL);

    string start = "su ";
    if (shell_login) {
        start += " - ";
    }
    start += Utils::OS::GetUserFromUID(uid) + " -c \'" + tray_command;
    if (silence) {
        start += " -s\'";
    }
    else {
        start += "\'";
    }
    LOG_GLTX(start.c_str());
    return system(start.c_str());
}

int MainEngine::RespondRepairCommand(const CommandBundle& command, void **result, int *outlen) {
    switch (command.mIntegerValue) {
    case CommandBundle::RESET_TRAY_PROCESS:
    case CommandBundle::RESET_TRAY_PROCESS_SILENCE: {
        mNotifierUID = atoi(command.mStringValue.c_str());
        int type = command.mIntegerValue;
        string tray_proc = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL);
        LOG_GLTX("CHECK_TRAY_PROCESS process %s ", tray_proc.c_str());

        int retry = 0;
        do {
            LOG_GLTX("process %s, killing", tray_proc.c_str());
            Utils::OS::KillProc(tray_proc.c_str(), 9);
            //usleep(1000*100);
        }
        while(Utils::OS::CheckProcessState(tray_proc.c_str()) && retry++ < 3);

        LOG_GLTX("start process %s, starting uid:%d ", tray_proc.c_str(), mNotifierUID);
        StartTrayProcess(true, CommandBundle::RESET_TRAY_PROCESS_SILENCE == type, mNotifierUID);
        //usleep(1000*100);



        int iRet = Utils::OS::CheckProcessState(tray_proc.c_str());
        LOG_GLTX("CheckProcessState %s,  return %d ", tray_proc.c_str(), iRet);

        // 测试在ROOT状态下启动客户端进程， 在龙芯飞腾的机器上不需要登录，但是在X86的机器需要登录， 所以在此做尝试
        if (Utils::OS::CheckProcessState(tray_proc.c_str()) < 1) {
            LOG_GLTX("start process %s, start by root uid:%d ", tray_proc.c_str(), mNotifierUID);

            StartTrayProcess(false, CommandBundle::RESET_TRAY_PROCESS_SILENCE == type, mNotifierUID);
        }

        //usleep(1000*100);
        if (Utils::OS::CheckProcessState(tray_proc.c_str()) < 1) {
            LOG_GLTX("start process %s, start by root uid:%d ", tray_proc.c_str(), mNotifierUID);

            //start silence
            StartTrayProcess(false, true, mNotifierUID);
        }

        LOG_GLTX("CHECK_TRAY_PROCESS process %s end", tray_proc.c_str());

        //mAsyncPool->PostMessage(Message(CHECK_TRAY_PROCESS, command.mIntegerValue, atoi(command.mStringValue.c_str())));
        break;
    }
    case CommandBundle::SHUTDOWN_DAEMON_TRAY: {
        Utils::OS::KillProc(atoi(command.mStringValue.c_str()), 9);
        break;
    }
    }
    return 0;
}

int MainEngine::requestClientInfo(ClientInfoType type, string& data) {
    switch (type) {
    case CLIENT_NAME:
        return mClientInfo->GetClientName(data);
    case CLIENT_ARCH:
        return mClientInfo->GetClientArch(data);
    case CLIENT_IP: {
        int retry = 0;
		if(!data.empty()) mClientInfo->SetLocalIP(data.c_str());
        while(retry<CONNECT_CENTER_MAX_RETRY && data.empty()) {
            sleep(1*retry++);
            mClientInfo->GetLocalIP(data, GetCenterIP());
        }
        return data.empty()?1:0;
    }
    case CLIENT_ISVM:
        break;
    case CLIENT_MAC: {
        int ret;
        ret = mClientInfo->GetMacAddr(data, ClientInfo::IP);
        if(data[0]) {
            mAppConfigLocker->Lock();
            mAppConfig->ChangeConfigSection(AppConfig::APP_MAC, data);
            mAppConfigLocker->Unlock();
        }
        return ret;
    }
    case CLIENT_OS:
        return mClientInfo->GetOSName(data);
    case CLIENT_ROOT:
        return mClientInfo->GetUserRoot(data);
    case CLIENT_MEM:
        return mClientInfo->GetMemorySize(data);
    case CLIENT_DISK:
        return mClientInfo->GetDiskSize(data);
    case CLIENT_USER:
        return mClientInfo->GetCurrentUser(data);
    case CLIENT_VER:
        data = GetAppVersion();
        return 0;
    case CLIENT_SYSTEM:
        return mClientInfo->GetSystem(data);
    case CLIENT_PRODUCT_TYPE: {
        data = mAppConfig->GetSubProductGUID();
        return 0;
    }
    case CLIENT_VLIB_VERSION: {
        data = GetVirusVersion();
        return 0;
    }
    case CLIENT_SUB_COMPONENTS: {
        Json::Value json;
        if (mAppConfig->GetSubComponents() & AppConfig::XAV) {
            Json::Value sub;
            if (mAppConfig->GetSubProductGUID() == LINUX_GENERAL_SERVER_AUTHORIZATION) {
                sub["guid"] = SUB_PRODUCT_LGSERVER_XAV;
            }
            else if (mAppConfig->GetSubProductGUID() == LINUX_CHINA_SERVER_AUTHORIZATION) {
                sub["guid"] = SUB_PRODUCT_LCSERVER_XAV;
            }
            else if (mAppConfig->GetSubProductGUID() == LINUX_SECRET_AUTHORIZATION) {
                sub["guid"] = SUB_PRODUCT_LPSECRET_XAV;
            }
            sub["version"] = GetAppVersion();
            json.append(sub);
        }
        if (mAppConfig->GetSubComponents() & AppConfig::XFW) {
            Json::Value sub;
            if (mAppConfig->GetSubProductGUID() == LINUX_GENERAL_SERVER_AUTHORIZATION) {
                sub["guid"] = SUB_PRODUCT_LGSERVER_XFW;
            }
            else if (mAppConfig->GetSubProductGUID() == LINUX_CHINA_SERVER_AUTHORIZATION) {
                sub["guid"] = SUB_PRODUCT_LCSERVER_XFW;
            }
            else if (mAppConfig->GetSubProductGUID() == LINUX_SECRET_AUTHORIZATION) {
                sub["guid"] = SUB_PRODUCT_LPSECRET_XFW;
            }
            sub["version"] = GetAppVersion();
            json.append(sub);
        }

        Json::FastWriter writer;
        data = writer.write(json);
        break;
    }
    default:
        break;
    }
    return 0;
}

void MainEngine::onCommunicateCallback(COMM_EVENT event, COMM_TYPE type, void *object) {
    if (COMM_INIT == event) {
        if (GENERAL_SGUID==type) {
            string data = *(string*)object;
            mStorage->SetSGUID(data);

            LOG_GLTX("GLTX get new GENERAL_SGUID is %s", data.c_str());
        }
        else if (EID_CHANGED == type) {
            mAppConfigLocker->Lock();
            {
                // if it is a secret machine, send to RJJH0, RJJH0 is RJJHetc
                SaveUpgradeInfo(AppConfig::FLAG_EID, *(string*)object);
            }
            mAppConfigLocker->Unlock();
        }
        else if (NET_RECONNECT == type) {

        }
    }
    else if (COMM_ERROR == event) {
        if (NOT_CONNECT == type) {
            int errorCode = *(int*)object;
            LOG_GLTX("GLTX get communit error is %d", errorCode);

            RespondConnectError(errorCode);
        }
        else if (INFO_ERROR == type) {
            string *data = (string*)object;
            std::vector<string> arrays;
            Utils::String::Split(*data, ":", &arrays);
            if (arrays.size() == 2) {
                if (arrays[0] == "error") {
                    int errorcode = atoi(arrays[1].c_str());
                    if (errorcode == API_I_EID_ERROR) {
                        // EID error, maybe the center changed, then clear saved eid, retry
                        LOG_GLTX("EID error");
                    }

                    LOG_GLTX("GLTX community error: %d %d", errorcode);
                }
            }
        }
        else if (HEAD_ERROR == type) {
            LOG_GLTX("header is empty, maybe something's wrong");
            RespondConnectError(-1);
        }
    }
    else if (COMM_AUTH==event) {
        if (UNAUTHORIZE == type) {
            if (GetAuthPopState() && Utils::Date::GetTimeStamp() - mLastAuthPromptTime > 3600) {
                mLastAuthPromptTime = Utils::Date::GetTimeStamp();
                SendToNotifer(CommunicateBundle::TOAST,
                              CommunicateBundle::NOTIFY_AUTH,
                              "提示", "产品未授权;请联系管理员;");
            }
        }
        else {
            CommEventLog log;
            log.mType = CommEventLog::SERVER;
            log.mDate = Utils::Date::GetTimeStamp();
            log.mTarget = "完成";
            log.mAction = "登录";
            log.mExtend = "客户端登录成功";
            mStorage->InsertCommEvent(log);
        }
    }
    else if (COMM_REMOTE == event) {
        mConnectStatus = CONNECT_SUCCESS;

        // 连接正常, 判断是否需要检查未上传日志
        if (mCheckLogUpload) {
            mAsyncPool->PostMessage(Message(REPORT_RUA_LOG, 0, 0));
        }

        //judge the current url
        string domain = Utils::Path::GetDomainUrl(mCurrentCenter);
        if (!domain.empty() && domain != mAppConfig->GetCenterURL()) {
            SaveUpgradeInfo(AppConfig::CENTER_URL, domain);
        }

        if (POLICY == type) {
            PolicyModel *model = (PolicyModel*)object;

#if 1
            FILE *file = fopen(
                             Utils::Path::JoinPath(
                                 mInstallDir.c_str(),
                                 FOLDER_RJJH_GLTX,
                                 "avpolicy.xml", NULL).c_str(),
                             "w+");
            string content = model->GetPolicyContent();
            fwrite(content.c_str(), 1, content.size(), file);
            fclose(file);
#endif
            LOG_GLTX("get policy %s", model->mPolicyProductID.c_str());

            if (model->mPolicyProductID == LINUX_RAV_PRODUCTID) {
                if (ProcessLinuxPolicy(model, mStorage, model->mPolicyType) > 0) {

                }
            }
            else if (Utils::String::ToLower(model->mPolicyProductID) == Utils::String::ToLower(ANTIVIRUS_PRODUCTID)) {
                 ProcessGlobalPolicy(model, mStorage, model->mPolicyType);
            }
            else if (Utils::String::ToLower(model->mPolicyProductID) == Utils::String::ToLower(DEPLOYED_PRODUCTID)) {
                ProcessDeployPolicy(model, mStorage, model->mPolicyType);
            }
            else if (Utils::String::ToLower(model->mPolicyProductID) == Utils::String::ToLower(NETCENTER_PRODUCTID)) {
                ProcessCenterPolicy(model, mStorage, model->mPolicyType);
            }
            else if (Utils::String::ToLower(model->mPolicyProductID) == Utils::String::ToLower(RFW_PRODUCTID)) {
                ProcessFirewallPolicy(model, mStorage, model->mPolicyType);
            }
        }
        else if (COMMAND == type) {
            CommandModel *model = (CommandModel*)object;
            ProcessCenterCommand(model);
        }
    }
}

int MainEngine::ProcessLinuxPolicy(PolicyModel *model, Storage *storage, int type) {
    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::POLICY_LINUX_CHANGED;
    command.mStringValue = model->JSONString();

    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ProcessGlobalPolicy(PolicyModel *model, Storage *storage, int type) {
    if (0 == ServiceBinder::ProcessGlobalPolicy(model, storage, type)) {
        ReloadScheduleList();
    }

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mStringValue = model->JSONString();

    command.mIntegerValue = CommandBundle::POLICY_GLOBAL_CHANGED;
    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ProcessCenterPolicy(PolicyModel *model, Storage *storage, int type) {
    if (0 == ServiceBinder::ProcessCenterPolicy(model, storage, type)) {
        ReloadScheduleList();

        // TODO 在此处，接收策略的时候会收到两次，一次为1，一次为0，所以此处调用代码会有问题
        // 将在后续的版本，如果需要加中心发现的时候再加上
//         if (GetCenterProxyState()) {
//            if (0 == mProxyThread) {
//                SECURITY_FREE(mFinderProxyLocker);
//                mFinderProxyLocker = CondLocker::NEW();
//                mCenterFinderProxyState = true;
//                pthread_create(&mProxyThread, NULL, ProxyWork, this);
//            }
//        }
//        else {
//            // if the proxy running, stop it
//            if (mProxyThread) {
//                mCenterFinderProxyState = false;
//                mFinderProxyLocker->Signal();
//                pthread_cancel(mProxyThread);
//                pthread_join(mProxyThread, NULL);
//                mProxyThread = 0;
//            }
//        }
    }

    // TODO
//    mCenterList.empty();
//    vector<string> center;
//    if (0 == mStorage->GetConnectCenterList(center)) {
//        vector<string>::iterator iter;
//        for (iter=center.begin();iter!=center.end();++iter) {
//            mCenterList.push(*iter);
//        }
//    }

    if (GetLockGUIDState()) {
        SaveUpgradeInfo(AppConfig::APP_GUID, mCommunicator->GUID());
    }

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::POLICY_CENTER_CHANGED;
    command.mStringValue = model->JSONString();

    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ProcessDeployPolicy(PolicyModel *model, Storage *storage, int type) {
    if (0 == ServiceBinder::ProcessDeployPolicy(model, storage, type)) {
        // need  to refresh the schedule list
        ReloadScheduleList();
    }

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::POLICY_DEPLOY_CHANGED;
    command.mStringValue = model->JSONString();

    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ProcessFirewallPolicy(PolicyModel *model, Storage *storage, int type) {
    ServiceBinder::ProcessFirewallPolicy(model, storage, type);

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::POLICY_FIREWALL_CHANGED;
    command.mStringValue = model->JSONString();

    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ProcessCenterCommand(CommandModel *model) {
    switch (model->mCmdId) {
    case CommandModel::SHOW_MESSAGE: {
        string message = "<temp>" + model->GetMessageContent() + "</temp>";
        CommandMessage messageModel;
        PolicyParser::ParseCommandMessage(message, messageModel);

        if (!messageModel.mContent.empty()) {
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "消息通知",
                          "新消息;内容: " + messageModel.mContent + ";时间: " + Utils::Date::FormatTimeStamp(Utils::Date::GetTimeStamp()),
                          "我知道了",
                          "");
            return SaveCommandLog(CommEventLog::MESSAGE, "message", messageModel.mContent, true);
        }
        break;
    }
    case CommandModel::FILE_DEFEN_OPEN:
    case CommandModel::FILE_DEFEN_STOP: {
        CommandBundle command;
        command.mCommandID = CommandBundle::REMOTE_COMMAND;
        command.mIntegerValue = CommandBundle::REMOTE_COMMAND_DEFEN;
        command.mStringValue = model->JSONString();

        LOG_GLTX("GLTX received a remote command and resend to Proxy");

        return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
    }
    case CommandModel::UPDATE_START: {
        if (NULL == mTempCommand) {
            mTempCommand = new CommandModel;
        }
        *mTempCommand = *model;

        // TODO 在SOC中随升级会下发升级参数和策略冲突，谁也不知道，暂屏掉
//        string update = "<temp>" + model->GetMessageContent() + "</temp>";
//        UpdateCommand updateModel;
//        PolicyParser::ParseCommandUpdate(update, updateModel);

        if (UPDATE_STEP_INIT != mUpdateStep) {
            LOG_GLTX("updating .... %d, ignore the command", mUpdateStep);
            // maybe something updating...
//            StartUpdate(UPDATE_STEP_APP, false);
            return 0;
        }

        mUpdatePrompt = false;
        return StartUpdateFunc(false);
    }
    case CommandModel::REMOTE_EXE: {
        PolicyParser::ParseCommandExecute(model->GetMessageContent(), mExecuteModel);
        ParsePath(mExecuteModel.mMainPath);
        CmdId = model->mId;
        //Workpath execution not realized

        switch (mExecuteModel.mAction){
            //Download and execute
            case 0 :

            //Download
            case 1 :{
                pthread_create(&mDownloadThread  , NULL, DownloadThread  , this);
                if (mDownloadErr != 0 || mExecuteModel.mAction == 1)
                    return 0;
            }

            //Execute with a detached pthread
            case 2 :{
                pthread_create(&mExecuteThread   , NULL, ExecuteThread   , this);
                return 0;
            }

            //Upload with a new pthread
            case 3 :{
                pthread_create(&mUploadFileThread, NULL, UploadFileThread, this);
                return 0;
            }
        }
        break;
    }
    default: {
        CommandBundle command;
        command.mCommandID = CommandBundle::REMOTE_COMMAND;
        command.mIntegerValue = CommandBundle::REMOTE_COMMAND_START;
        command.mStringValue = model->JSONString();

        LOG_GLTX("GLTX received a remote command and resend to Proxy");

        return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
    }
    }
    return -1;
}

void MainEngine::ParsePath(string &path){
    //To analyze file path macros and relative path, and return an absolute path

    //Replace all "\" with "/" 
    for(string::size_type i(path.find("\\")); i!=string::npos; i=path.find("\\",++i)){
        path.replace(i, 1, "/");
    }

    //File name only
    string::size_type firstSlash=path.find("/");
    if(firstSlash == string::npos){
        path = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),
                                    FOLDER_RJJH_GLTX, "FileRecv/", NULL) + path;
        return ;
    }

    //File path macros and relative path 
    string prefix(path.substr(0, firstSlash));
    if(!prefix.compare("%ESM%")){
        path.replace(0, 5, mAppConfig->GetInstallPath());
    }
    else if(!prefix.compare("%TEMP%")){
        path.replace(0, 6, string("/tmp"));
    }
    else if(!prefix.compare("%TMP%")){
        path.replace(0, 5, string("/tmp"));
    }
    else if(!prefix.compare(".")){
        path.replace(0, 1, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),
                                                FOLDER_RJJH_GLTX, "FileRecv", NULL));
    }
    else if(!prefix.compare("..")){
        path.replace(0, 2, Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),
                                                FOLDER_RJJH_GLTX, NULL));
    }
    return ;
}

void *MainEngine::DownloadThread(void *params){
    MainEngine *self = (MainEngine*)params;
    self->DownloadThreadFunc();
    pthread_exit(NULL);
}

//Open and write file, permission: 0755
void MainEngine::DownloadThreadFunc(){
    //Make path
    string::size_type lastSlash=mExecuteModel.mMainPath.rfind("/");
    string dir(mExecuteModel.mMainPath.substr(0, lastSlash));
    string makeDirCmd("mkdir -m 777 -p "+dir);
    system(makeDirCmd.c_str());

    mDownloadErr = 1;
    int file=open(mExecuteModel.mMainPath.c_str(),O_WRONLY|O_CREAT|O_TRUNC);
    if(file<=0){
        LOG_GLTX("File creation failed. Download stopped: %s", mExecuteModel.mMainPath.c_str());
    }
    else if(write(file, mExecuteModel.mFile.c_str(), mExecuteModel.mFile.size())==-1){
        LOG_GLTX("File writing failed. Download stopped: %s",mExecuteModel.mMainPath.c_str());
    }
    else if(chmod(mExecuteModel.mMainPath.c_str(),0755)<0){
        LOG_GLTX("File permission modification failed: %s", mExecuteModel.mMainPath.c_str());
    }
    else{
        LOG_GLTX("Download succeeded: %s", mExecuteModel.mMainPath.c_str());
        mDownloadErr = 0;
        close(file);
    }
                
    if (mDownloadErr == 1){
        mCommunicator->ReportCommandLog(mAppConfig->GetSubProductGUID(),\
                                            "runprocess", CmdId, 0, -1);
    }
    else if(mExecuteModel.mAction == 1){
        mCommunicator->ReportCommandLog(mAppConfig->GetSubProductGUID(),\
                                            "runprocess", CmdId, 0, 0);
    }
}

void* MainEngine::ExecuteThread(void *params) {
    pthread_detach(pthread_self());
    
    MainEngine *self = (MainEngine*)params;
    self->ExecuteThreadFunc();
    pthread_exit(NULL);
}

void MainEngine::ExecuteThreadFunc(){
    string shellCmd(mExecuteModel.mMainPath+" " + mExecuteModel.mCmdPara);
    if(shellCmd.size()>=255){
        LOG_GLTX("Execution command overlong: %s", shellCmd.c_str());
        
    }
    int shellRet    = system(shellCmd.c_str());
    int normalExit  = WIFEXITED(shellRet);
    int exitState   = WEXITSTATUS(shellRet);
    string productID = mAppConfig->GetSubProductGUID();
    string action = "runprocess";

    if(shellRet<0 || !normalExit || exitState){
        LOG_GLTX("Execution failed: %s", shellCmd.c_str());
        mCommunicator->ReportCommandLog(productID, action, CmdId, 0, -1);
    }
    else{
        LOG_GLTX("Execution succeeded: %s",shellCmd.c_str());
        mCommunicator->ReportCommandLog(productID, action, CmdId, 0, 0);
    }
}

void *MainEngine::UploadFileThread(void *params){
    MainEngine *self = (MainEngine*)params;
    self->UploadFileThreadFunc();
    pthread_exit(NULL);
}

void MainEngine::UploadFileThreadFunc(){
    int err = 0;

    chmod(mExecuteModel.mMainPath.c_str(), S_IROTH|S_IRGRP|S_IRUSR);
    ifstream file(mExecuteModel.mMainPath.c_str());
    if(!file){
        LOG_GLTX("File opening failed: %s", mExecuteModel.mMainPath.c_str());
        err = 1;
    }
    istreambuf_iterator<char> beg(file), end;
    string upFileContent=(string(beg, end));
    file.close();

    if(err == 0)
        err = mCommunicator->ReportFile(Utils::Hash::Base64Encode(upFileContent));

    if (err == 0){
        LOG_GLTX("Upload file: %s", mExecuteModel.mMainPath.c_str());
        mCommunicator->ReportCommandLog(mAppConfig->GetSubProductGUID(),
                                        "runlog", CmdId, 0, 0);
    }
    else{ 
        LOG_GLTX("Upload file failed: %s", mExecuteModel.mMainPath.c_str());
        mCommunicator->ReportCommandLog(mAppConfig->GetSubProductGUID(),
                                        "runlog", CmdId, 0, -1);
    }
}

void* MainEngine::ThreadWork(void *params) {
    MainEngine *self = (MainEngine*)params;
    self->CommunicateWork();
    pthread_exit(NULL);
}

int MainEngine::CommunicateWork() {
    while (true) {
        CommandBundle command;
        command.mCommandID = CommandBundle::FIND_PROXY_CLIENT;

        if (mNotifierUID >= 0) {
            int ret = NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
            LOG_GLTX("GLTX send data to proxy and ret is %d", ret);

            if (999 == ret) {
                mCurrentCenter = mAppConfig->GetCenterURL();
                if (1 == mAppConfig->GetProductType()) {
                    // 1 mean ESM, define is setup.h
                    mCurrentCenter += string(CENTER_ESM_URL);
                }
                else if (2 == mAppConfig->GetProductType()) {
                    // 2 mean SOC
                    mCurrentCenter += string(CENTER_SOC_URL);
                }
                if (!mCommunicator && !mCurrentCenter.empty()) {
                    if (0 == InitCommunication(mCurrentCenter)) {
                        mCommunicator->Start();
                    }
                }
                break;
            }
        }

        LOG_GLTX("GLTX not find proxy uid, waitting");

        sleep(5);
    }
    return 0;
}

int MainEngine::onCenterFindEvent(int event, string &eid, vector<string> &centers) {
    switch (event) {
    case CENTER_FINDING:
        centers.push_back(mCurrentCenter);
        break;
    case CENTER_FOUND: {
        if (centers.size() > 0) {
            vector<string>::iterator iter;
            for (iter=centers.begin();iter!=centers.end();++iter) {
                SourceListEntry entry;
                entry.mType = SourceListEntry::CENTER_PROXY_SOUCE;
                entry.mAddress = *iter;
                mCenterList.push(entry);
            }
            mAsyncPool->PostMessage(Message(CHANGE_OTHRE_CENTER_ADDR, 0, 0));
        }
        break;
    }
    default:
        break;
    }
    return 0;
}

void* MainEngine::ProxyWork(void *params) {
    MainEngine *self = (MainEngine*)params;
    while (self->mCenterFinderProxyState) {
        if (0 == self->ProxyFunction()) {
            sleep(300);
        }
        else {
            self->BroadcastCenter(true);
            break;
        }
    }

    self->BroadcastCenter(false);
    return NULL;
}

int MainEngine::ProxyFunction() {
    if (!mCenterFinder) {
        mCenterFinder = CenterFindProxy::NEW(this, mClientInfo->IP);
        if (!mCenterFinder) {
            return-1;
        }
    }

    CenterFindProxy::ClientCenterData center(mAppConfig->GetEID());
    if (0 == mCenterFinder->QueryCenter(center)) {
        if (center.center.size() > 0) {
            vector<string>::iterator iter;
            for (iter=center.center.begin(); iter!=center.center.end(); ++iter) {
                string one;
                if (mAppConfig->GetProductType() == SetupApplication::CENTER_ESM) {
                    one = Utils::Path::JoinPath(iter->c_str(), CENTER_ESM_URL, NULL);
                }
                else if (mAppConfig->GetProductType() == SetupApplication::CENTER_SOC) {
                    one = Utils::Path::JoinPath(iter->c_str(), CENTER_SOC_URL, NULL);
                }
                if (mCurrentCenter == one) {
                    // some one has proxied,
                    return 0;
                }
            }
        }
    }
    return -1;
}

int MainEngine::BroadcastCenter(bool status) {
    if (!mCenterFinder) {
        mCenterFinder = CenterFindProxy::NEW(this, mClientInfo->IP);
        if (!mCenterFinder) {
            return -1;
        }
    }

    if (status) {
        mCenterFinder->StartAnswer();
        mFinderProxyLocker->Wait();
    }
    else {
        mCenterFinder->Stop();
    }
    return 0;
}

int MainEngine::RespondConnectError(int error) {
    (void)(error);
    if (0 == mDisconnectTime) {
        mDisconnectTime = Utils::Date::GetTimeStamp();
        LOG_GLTX("GLTX RespondConnectError");
        return 0;
    }

    if (Utils::Date::GetTimeStamp()-mDisconnectTime > GetReconnectInterval()) {
        LOG_GLTX("GLTX RespondConnectError PostMessage");
        if (!mCenterList.empty()) {
            mConnectStatus = RETRY_OTHER_CENTER;
            mAsyncPool->PostMessage(Message(CHANGE_OTHRE_CENTER_ADDR, 0, 0));
        }
    }
    else {
        if (ASK_NEIGHBORS != mConnectStatus){
            mConnectStatus = ASK_NEIGHBORS;
            //FindNeighbors();
            LOG_GLTX("GLTX RespondConnectError FindNeighbors exit");
        }
    }
    return 0;
}

string MainEngine::GetCenterIP() {
    if (mCurrentCenter.empty()) {
        return "";
    }

    string ip;
    size_t pos = mCurrentCenter.find("//");
    if (pos != string::npos) {
        size_t end = mCurrentCenter.find("/", pos+2);
        if (end != string::npos) {
            ip = mCurrentCenter.substr(pos+2, end-pos-2);
        }
        else {
            ip = mCurrentCenter.substr(pos+2);
        }
    }
    if (!ip.empty()) {
        size_t port = ip.find(":");
        if (port != string::npos) {
            return ip.substr(0, port);
        }
    }
    return ip;
 }

//int MainEngine::SaveConfigField(const string& section, const string& key, const string& value) {
//    const string configfile = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_ETC, CONFIG_INI_FILE);
//    CINIFile file(configfile, true);
//    file.writeitemstr(section, key, value);
//    return 0;
//}

int MainEngine::GetConnectCenter(UPDATE_SOURCE type, const string& uri) {
    mCenterAddr.clear();
    if (SHARE_FOLDER == type) {
        mCenterAddr.insert(mCenterAddr.begin(), make_pair(SHARE_FOLDER, uri));
    }
    else {
        if (mAppConfig->SecretMachine()) {
            // 如果是保密机，则用这个位置做一个假的升级
            AntiVirusSetting setting;
            mStorage->GetAntiVirusSetting(setting, AntiVirusSetting::ANTI_COMMON);
            if (setting.mCommon.mVirusTrace.mValue) {
                mCenterAddr.insert(mCenterAddr.begin(), make_pair(FAKE_UPDATE, "http://127.0.0.1"));
                return mCenterAddr.size();
            }
        }

        if (mAppConfig->GetProductType() == 0) {
            mCenterAddr.push_back(make_pair(OFFICAL_HTTP, "http://rsup10.rising.com.cn/rs2017"));
        }
        else {
            map<string, bool> urlmap;
            list<SourceListEntry> sources;
            if (mStorage->GetSourceList(sources, SourceListEntry::CLOUD_UPDATE_SOURCE) > 0) {
                list<SourceListEntry>::iterator iter;
                for (iter=sources.begin();iter!=sources.end();++iter) {
                    map<string, bool>::iterator found = urlmap.find(iter->mAddress);
                    if (found == urlmap.end()) {
                    	// 临时方案(逻辑错误), 修正官网地址...
                    	if (strcmp(iter->mAddress.c_str(), "http://rsup10.rising.com.cn/rs2017") == 0)
                    	{
                    		mCenterAddr.push_back(make_pair(OFFICAL_HTTP, iter->mAddress));
                    		urlmap.insert(make_pair(iter->mAddress, true));
                    		continue;
                    	}

                        if (iter->mName.empty() || iter->mName == "http") {
                            mCenterAddr.insert(mCenterAddr.begin(), make_pair(OTHER_HTTP, iter->mAddress));
                        }
                        else if (iter->mName == "rshttp") {
                            mCenterAddr.push_back(make_pair(OFFICAL_HTTP, iter->mAddress));
                        }
                        urlmap.insert(make_pair(iter->mAddress, true));
                    }
                }
            }

            if (!mAppConfig->GetCenterURL().empty()) {
                map<string, bool>::iterator found = urlmap.find(mAppConfig->GetCenterURL());
                if (found == urlmap.end()) {
                    mCenterAddr.insert(mCenterAddr.begin(), make_pair(OTHER_HTTP, mAppConfig->GetCenterURL()));
                }
            }
        }
    }
    return mCenterAddr.size();
}

int MainEngine::RespondUpdateApp(int event, int type, void *data) {
    if (UPDATE_ERROR == type) {
        mUpdateStep = UPDATE_STEP_INIT;
        SaveCommandLog(CommEventLog::DEPLOY, "update", "升级;软件;失败", true);
        if (event == UPDATE_EVENT_APP) {
            mUpdateState[UPDATE_STEP_APP] = false;
        }
        else if (event == UPDATE_EVENT_VIRUS) {
            mUpdateState[UPDATE_STEP_VIRUS_LIB] = false;
        }

        mUpdateErrorCode = *(int*)data;

        LOG_GLTX("update error code is %d", mUpdateErrorCode);
        mUpdateLocker->Signal();
    }
    else if (UDDATE_GET_NEW_VERSION == type) {
        if (mUpdatePrompt) {
            string prompt = "软件升级;最新版本: " + *(std::string*)data + ";请不要做任何操作";
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "软件升级",
                          prompt
                          );
        }
    }
    else if (UPDATE_NEWEST == type) {
		string log = "升级;软件;忽略;";
    	log += *(std::string*)data; log +=";"; log += *(std::string*)data;
		SaveCommandLog(CommEventLog::DEPLOY, "update", log, true);

        mUpdateStep = UPDATE_STEP_INIT;
        if (mUpdatePrompt) {
            string prompt = "软件升级:当前版本为最新!";
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "软件升级",
                          prompt
                          );
        }
        LOG_GLTX("the app is the newest");
        mUpdateLocker->Signal();
    }
    else if (UPDATE_DOWNLOAD_SUCCESS == type) {
        string *version = (string*)data;
        int err = 0;
        if (mCommunicator) {
            err = mCommunicator->ReportRUALog(
                                mUpdatePrompt?RUA_SOURCE_MANUAL_START:RUA_SOURCE_REMOTE_START,
                                RUA_ACTION_UPDATE,
                                RUA_ROLE_REC,
                                mAppConfig->GetAppVersion(),
                                *version,
                                false
                    );
        }
        SaveCommandLog(CommEventLog::DEPLOY, "update", "升级;软件;成功;" + mAppConfig->GetAppVersion() + ";" + *version, err==0);
        mUpdateStep = UPDATE_STEP_INIT;      

        // signal the locker, then the update thread exit
        mUpdateState[UPDATE_STEP_APP] = true;

        LOG_GLTX("update success, version is %s", version->c_str());
        mUpdateLocker->Signal();
    }

    return 0;
}

int MainEngine::RespondUpdateVirus(int event, int type, void *data) {
    switch (type) {
    case UPDATE_NEWEST: {
    	string log = "升级;病毒库;忽略;";
    	log += *(std::string*)data; log +=";"; log += *(std::string*)data;
		SaveCommandLog(CommEventLog::DEPLOY, "update", log, true);

		LOG_GLTX("update virus, is newest!!!(%s)", log.c_str());
		mUpdateLocker->Signal();
    	break;
    }
    case UPDATE_VIRUS_SUCCESS: {
        if (data) {
            mUpdateState[UPDATE_STEP_VIRUS_LIB] = true;
            LOG_GLTX("update virus success");

            if (mUpdatePrompt) {
                SendToNotifer(CommunicateBundle::INFO,
                              CommunicateBundle::NOTIFY_MESSAGE,
                              "提示", "病毒库升级;病毒库升级成功");
            }
        }
        mUpdateLocker->Signal();
        break;
    }
    case UDPATE_VIRUS_FAILED: {
        mUpdateState[UPDATE_STEP_VIRUS_LIB] = false;
        mUpdateErrorCode = *(int*)data;
        LOG_GLTX("update virus error");
        if (mUpdatePrompt) {
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "提示", "病毒库升级;病毒库升级失败");
        }
        SaveCommandLog(CommEventLog::DEPLOY,
                       "update",
                       "升级;病毒库;失败;" + string("错误码:")+Utils::String::itoa(mUpdateErrorCode) + ";" + "否", true);
        mUpdateLocker->Signal();
        break;
    }
    case UDDATE_GET_NEW_VERSION:  {
        if (mUpdatePrompt) {
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "提示", string("病毒库升级;") + string("病毒库新版本:") + *(string*)data);
        }
        break;
    }
    case UPDATA_VIRUS_RP: {
        // the event from GJCZ->RJJH0->GLTX, not resend to RJJH0
        string *filename = (string*)data;
        LOG_GLTX("download rp file %s", filename->c_str());
        if (mUpdatePrompt) {
            string prompt = string("病毒库升级;下载病毒库:") + *filename;
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "提示", prompt);
        }
        break;
    }
    case UPDATE_VIRUS_COMBINE: {
        // the event from GJCZ->RJJH0->GLTX, not resend to RJJH0
        string *filename = (string*)data;
        LOG_GLTX("combine virus %s", filename->c_str());
        if (mUpdatePrompt) {
            string prompt = string("病毒库升级;合并病毒库:") + *filename;
            SendToNotifer(CommunicateBundle::INFO,
                          CommunicateBundle::NOTIFY_MESSAGE,
                          "提示", prompt);
        }
        break;
    }
    default:
        break;
    }
    return 0;
}

int MainEngine::OnUpdateEvent(int event, int type, void *data) {
    if (UPDATE_EVENT_APP == event) {
        return RespondUpdateApp(event, type, data);
    }
    else if (UPDATE_EVENT_VIRUS == event) {
        return RespondUpdateVirus(event, type, data);
    }

    return 0;
}

void MainEngine::OnScheduleEvent(int event, int /*type*/, void *object) {
    if (ScheduleEntry::QUICK_SCAN == event
            || ScheduleEntry::ALL_SCAN == event) {
        //ScheduleEntry *entry = (ScheduleEntry*)object;

        LOG_GLTX("time on, send schedule scan to GJCZ");

        CommandBundle command;
        command.mCommandID = CommandBundle::REMOTE_COMMAND;
        command.mIntegerValue = CommandBundle::SCHEDULE_SCAN;
        command.mStringValue = Utils::String::itoa(event);

        if (0 == NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID)) {

        }
    }
    else if (ScheduleEntry::UPDATE == event) {
        LOG_GLTX("time on, send schedule update to GLTX");

        mUpdatePrompt = false;
        SECURITY_FREE(mTempCommand);
        if (UPDATE_STEP_INIT != mUpdateStep) {
            // maybe something updating...

        }
        else {
            StartUpdateFunc(false);
        }
    }
}

int MainEngine::SendInfoToNotifyApp(int cmd, int type, const string& data) {
    CommandBundle command;
    command.mCommandID = cmd;
    command.mIntegerValue = type;
    command.mStringValue = data;
    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ReloadScheduleList() {
    if (NULL != mScheduleEngine) {
        mScheduleEngine->Reload();
    }
    return 0;
}

int MainEngine::StartUpdate(enum UpdateStep step, pair<UPDATE_SOURCE,string>& url) {    
    mUpdateStep = step;
    if (UPDATE_STEP_VIRUS_LIB == step) {
        if (NULL== mUpdateVirusEngine) {
            mUpdateVirusEngine = GLTXUpdateVirusEngine::NEW(mAppConfig->GetInstallPath(), FOLDER_RJJH_GLTX, mAppConfig->GetProductType());
            mUpdateVirusEngine->SetNotifyUID(mNotifierUID);
        }
        mUpdateVirusEngine->SetObserver(this);

        UpdateConfig config;
        GetUpdateConfig(config);
        config.mCenterAddr = url;

        mUpdateVirusEngine->SetUpdateConfig(config);
        return mUpdateVirusEngine->Start(mAppConfig->GetVlibVersion());
    }
    else if (UPDATE_STEP_APP == step) {
        if (mAppConfig->SecretMachine()) {
            return 1;
        }

        UpdateConfig config;
        GetUpdateConfig(config);
        if (config.mUpdateContent == UPDATE_VIRUS_DB) {
            return 1;
        }

        vector<string> arch;
        Utils::String::Split(mAppConfig->GetAppType(), ":", &arch);
        if (arch.size() == 2) {
            config.mUpdateProduct = Utils::String::ToUpper(arch[1]);
        }
        config.mCenterAddr = url;

        LOG_GLTX("start url is %s", url.second.c_str());

        if (NULL == mUpdateAppEngine) {
            mUpdateAppEngine = UpdateAppEngine::NEW(mAppConfig->GetInstallPath(), FOLDER_RJJH_GLTX, mAppConfig->GetProductType());
        }
        mUpdateAppEngine->SetObserver(this);
        mUpdateAppEngine->SetUpdateMethod(mAppConfig->GetSubProductGUID());
        mUpdateNeedRestart = config.mUpdateContent != UPDATE_VIRUS_DB;
        mUpdateAppEngine->SetUpdateConfig(config);
        mUpdateAppEngine->SetAppConfig(mAppConfig);
        mUpdateAppEngine->SetNotifyUID(mNotifierUID);

        return mUpdateAppEngine->Start(mAppConfig->GetAppVersion());
    }
    return 0;
}

int MainEngine::SaveUpgradeInfo(int sectionflag, const string& value) {

    //printf("SaveUpgradeInfo\n");
    int needRJJHSave = 0;
    if (mAppConfig->SecretMachine()) {
        needRJJHSave = 1;
        mAppConfig->ChangeConfigSection(sectionflag, value, false);
    }
    else {
        //printf("SaveUpgradeInfo------\n");
        mAppConfig->ChangeConfigSection(sectionflag, value);
    }

    CommandBundle command;
    command.mCommandID = CommandBundle::RELOAD_CONFIG;
    command.mIntegerValue = needRJJHSave;
    command.mStringValue = mAppConfig->JsonString();
    return NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
}

int MainEngine::ImportCrossUpdateInfo() {
    string crossxml = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GLTX, CROSS_VERSION_LOG_FILE, NULL);
    if (!Utils::Path::FileAvailed(crossxml)) {
        return -1;
    }

    string version, guid, sguid;
    vector<CrossVersion::LOG_INFO> scanlogs, viruslog;
    CrossVersion cross;
    if (0 == cross.ImportLogs(crossxml, version, guid, sguid, scanlogs, viruslog)) {
        mStorage->SetGUID(guid);
        mStorage->SetSGUID(sguid);
    }
    return 0;
}

int MainEngine::AddQuarantineFileToWhitelist(const string& log) {
    list<WhiteListEntry> files;
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::WHITE;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    entry.mPathType = WhiteListEntry::FILE_PATH;
    if (mStorage->GetWhiteList(files, entry) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=files.begin(); iter!=files.end(); ++iter) {
            if (iter->mPath == log && iter->mReserved1 == 0) {
                return 0;
            }
        }
    }

    entry.mListType = WhiteListEntry::WHITE;
    entry.mBlockType = WhiteListEntry::FILE_SCAN;
    entry.mPathType = WhiteListEntry::FILE_PATH;
    entry.mPath = log;
    entry.mReserved1 = 0;

    return mStorage->InsertWhiteList(entry);
}

void* MainEngine::UploadLogThreadFunc(void *params) {
    MainEngine *engine = (MainEngine*)params;
    engine->AsyncUploadLog();
    return NULL;
}

void MainEngine::AsyncUploadLog() {
    mUploadLogMutex->Lock();

    // 上报完成, 置为false
    mCheckLogUpload = false;

    list<CommEventLog> logs;
    CommEventLog seed;
    seed.mType = CommEventLog::DEPLOY;
    if (GetBaseLogs(logs, seed) > 0) {
        list<CommEventLog>::iterator iter;
        for (iter=logs.begin(); iter!=logs.end(); ++iter) {
            if (mCommunicator && iter->mReserved1.empty()) {
                vector<string> segments;
                Utils::String::Split(iter->mExtend, ";", &segments);

                int err = mCommunicator->ReportRUALog(
                                            (iter->mAction=="Install")?RUA_SOURCE_MANUAL_START:RUA_SOURCE_REMOTE_START,
                                            (iter->mAction=="Install")?RUA_ACTION_INSTALL:RUA_ACTION_UPDATE,
                                            RUA_ROLE_REC,
                                            segments.size()>3?segments[3]:"",
                                            segments.size()>4?segments[4]:"",
                                            false
                                );
                if (0 == err) {
                    iter->mReserved1 = "1";
                    mStorage->UpdateCommEvent(*iter);
                }
            }
        }
    }
    mUploadLogMutex->Unlock();
}

int  MainEngine::CheckRegisted(const string& serial, const char * license) {
    if (serial.empty() || 0 != mAppConfig->GetProductType()) {
        // 0 means CenterLess
        return -11;
    }

    if (serial.length() != 27) {
    	// new validate...
    	std::string     file, product;
		AccreidtItem    result;
		int             nRet;

		if(license && *license) file = license;
		else file = mAppConfig->GetAppLicenseFile();
    	if (!Utils::Path::FileAvailed(file)) return -9;

		nRet = RsEsmAccredit(serial.c_str(), file.c_str(), result);
		if(nRet) return nRet;

		//LOG_GLTX("active -->%d,%s,%s", result.dwRet, result.strGuid.c_str(), result.strEndTime.c_str());
		product = mAppConfig->GetProductGuid();
		if (product.empty())  product = "5A55A15F-6664-41cf-8C17-47CC98751132";
		if (strcasecmp(result.strGuid.c_str(), product.c_str())) return -8;
		if (result.dwRet == 2) {
			// old outdate. == 1 ???
			return -7;
		}

		assert(!result.strEndTime.empty());
		mAppConfig->SetAppDeadline(result.strEndTime.c_str());
		if (license && *license)
		{
			char szName[32] = {0};
			sprintf(szName, "%u.lic", (int)time(0));
			std::string strTmp = Utils::Path::JoinPath(mInstallDir.c_str(), PLUGINS_PATH, szName, NULL);
			Utils::Path::CopyFile(file, strTmp);
			mAppConfig->SetAppLicenseFile(strTmp);
		}
		return 0;
    }

    string configfile = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), PLUGINS_PATH, "RsConfig.cfg", NULL);
    if (false == Utils::Path::FileAvailed(configfile)) {
        return -12;
    }

    if (!serial.empty()) {
        UNIXSNPARAM params;
        memset(&params, 0, sizeof(params));

        int matched = -13;
        void *handle = NULL;
        int err = InitCfgConfig(0, configfile.c_str(), &handle);
        if (UNIX_RS_ERROR_OK == err) {
            if (UNIX_RS_ERROR_OK == CheckCfgSnValid(handle, 0, serial.c_str(), &params)) {
                switch (params.nOsVer) {
                case 12:
                case 13:
                case 14:
                case 15:
                    matched = params.nOsVer;
                default:
                    matched = 0;
                    break;
                }
            }
        }
        else {
            matched = -14;
        }
        ExitCfgConfig(0, &handle);
        return matched;
    }
    return -15;
}

bool MainEngine::CheckDuplicateSGUID() {
    std::string strMac, strNull;

    mAppConfigLocker->Lock();
    strMac = mAppConfig->GetMac();
    mAppConfigLocker->Unlock();

    if(!strMac.empty() && !mClientInfo->CheckLocalMac(strMac.c_str())) {
        LOG_GLTX("check duplication sguid by mac, to clear them --->(%s)", strMac.c_str());
        mAppConfigLocker->Lock();
        mStorage->SetSGUID(strNull);
        mAppConfig->ChangeConfigSection(AppConfig::APP_SGUID, strNull);
        mAppConfig->ChangeConfigSection(AppConfig::APP_MAC, strNull);
        mAppConfigLocker->Unlock();
        return false;
    }

    LOG_GLTX("check duplication sguid by mac, ok -->%s", strMac.c_str());
    return true;
}
