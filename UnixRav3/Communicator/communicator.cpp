#include "communicator.h"
#include "communicator_global.h"
#include "net/apiservice.h"
#include "commonapi.h"
#include "baseconfig.h"

#include "models/messagepackage.h"
#include "models/messagepartbody.h"
#include "models/log/reportlogmodel.h"
#include "models/contentmodel.h"
#include "models/log/logdatavirus.h"
#include "models/log/logdatascanevent.h"
#include "models/log/logdatavirus.h"
#include "models/log/logepstate.h"
#include "models/log/logcommandstate.h"
#include "models/log/logruamodel.h"
#include "models/policy/commandmodel.h"
#include "models/policy/authorizationmodel.h"
#include "models/policy/policymodel.h"
#include "models/client/clientinfomodel.h"
#include "models/contentfactory.h"
#define  RS_MAIN_VERSION    "3.0.0.2"
#include "rsversion.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define EP_VIRUS_LIB_VERSION    "vlibver"
#define EP_FILE_MONITOR_STATE   "filemon"
#define EP_MAIL_MONITOR_STATE   "mailmon"
#define EP_SYS_MONITOR_STATE    "sysmon"
#define EP_APP_MONITOR_STATE    "virusaction"

Communicator* Communicator::NEW(const string& url,
                                const string& eid,
                                const string& guid,
                                const string& sguid,
                                const string& version,
                                const string& systype)
{
    Communicator *self = new Communicator;
    if (self) {
        if (self->Construct(url, eid, guid, sguid, version, systype)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

Communicator::Communicator() {
    mAPIService = NULL;
    mTespan = TESPAN_DEFAULT_VALUE;
    mAuthTespan = 0;
    mState = 0;
    mAuthDay = 0;
    mAuthState = false;

    mAuthThread = 0;
    mWorkThread = 0;
}

Communicator::~Communicator() {
    Stop();
    SECURITY_FREE(mAPIService);
}

int Communicator::Construct(const string& url,
                            const string& eid,
                            const string& guid,
                            const string& sguid,
                            const string& version,
                            const string& systype)
{
    mUrl = url;
    mEid = eid;
    mGUID = guid;
    mSGUID = sguid;
    mVersion = version;
    mSystem = systype;

    try {
        mAPIService = APIService::Builder()
                      .baseUrl(mUrl)
                      .eid(eid)
                      .guid(guid)
                      .sguid(sguid)
                      .version(version)
                      .system(systype)
                      .build();
        mAPIService->setObserver(this);
    }
    catch(...) {
        return -1;
    }

    srand(time(NULL));
    return 0;
}

int Communicator::HeartBeat() {
    int ret = mAPIService->heartBeat();////A curl state
    if (ret) {
        if (ret != mConnectError>>16) {
            // error change, recount
            mConnectError = 0;
            mConnectError |= ret<<16;
        }
        else {
            int count = (mConnectError&0xFFFF) + 1;
            if (count < 5) {
                mConnectError &= ~(0xFFFF);
                mConnectError |= count;
            }
            else {
                if (mObserver) {
                    mObserver->onCommunicateCallback(CommunicatorObserver::COMM_ERROR, CommunicatorObserver::NOT_CONNECT, &ret);
                }
            }
        }
    }
    else {
        if (mObserver) {
            mObserver->onCommunicateCallback(CommunicatorObserver::COMM_INIT, CommunicatorObserver::NET_RECONNECT, &ret);
        }
    }

    return ret;
}

int Communicator::UploadClientInfo(vector<pair<string,string> >& products) {
    if (NULL == mObserver) {
        return -1;
    }

    ClientInfoModel clientModel;
    mAPIService->GetLocalIp(clientModel.mIp);

    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_NAME, clientModel.mComputername);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_IP, clientModel.mIp);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_ARCH, clientModel.mCpuarchitecture);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_MAC, clientModel.mMac);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_VER, clientModel.mVersion);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, clientModel.mUsername);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_OS, clientModel.mOs);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_MEM, clientModel.mTotalmemory);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_DISK, clientModel.mDiskSize);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_ROOT, clientModel.mSystemdirectory);

//    if (mProductGUID.empty()) {
//        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_PRODUCT_TYPE, mProductGUID);
//    }

    Json::Value modulesJson;
    vector<pair<string,string> >::iterator iter;
    for (iter=products.begin();iter!=products.end();++iter) {
        ModuleInfoModel moduleModel(iter->first, iter->second);
        modulesJson.append(moduleModel.toJSON());
    }

    ContentBundle contentBundle(MODULE, modulesJson);
    contentBundle += ContentBundle(CLIENT, clientModel.toJSON(), true);

    MessagePartBundle part(COMMAND_REPORT_CLI_INFO, &contentBundle);
    MessageBundle message(&part);
    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REPORT_CLI_INFO, body);
}

int Communicator::RequestAuthorization() {
    if (NULL == mObserver) {
        return -1;
    }

    ClientInfoModel clientModel;
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_NAME, clientModel.mComputername);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_IP, clientModel.mIp);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_ARCH, clientModel.mCpuarchitecture);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_MAC, clientModel.mMac);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_VER, clientModel.mVersion);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, clientModel.mUsername);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_OS, clientModel.mOs);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_MEM, clientModel.mTotalmemory);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_DISK, clientModel.mDiskSize);
    mObserver->requestClientInfo(CommunicatorObserver::CLIENT_ROOT, clientModel.mSystemdirectory);

    if (mProductGUID.empty()) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_PRODUCT_TYPE, mProductGUID);
    }

    Json::Value modulesJson;
    ModuleInfoModel moduleModel(mProductGUID, mVersion);
    modulesJson.append(moduleModel.toJSON());

    ContentBundle contentBundle(MODULE, modulesJson);
    contentBundle += ContentBundle(CLIENT, clientModel.toJSON(), true);

    MessagePartBundle part(COMMAND_REQUEST_AUTH, &contentBundle);
    MessageBundle message(&part);
    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REQUEST_AUTH, body);
}

int Communicator::onRequestCallback(CALLBACK_EVENT event, CALLBACK_TYPE type, const string& data) {
    if (BODY == event) {
        LOG_GLTX("%s \n", data.c_str());

        std::map<int, std::list<void*> > result;
        int err = ContentFactory::make(data, result);
        if (0 == err) {
            // report the result
            std::map<int,std::list<void*> >::iterator iter;
            for (iter=result.begin();iter!=result.end();++iter) {
                int type = iter->first;
                switch(type) {
                case COMMAND_REQUEST_AUTH: {
                    std::list<void*>::iterator it;
                    for (it=iter->second.begin();it!=iter->second.end();++it) {
                        AuthorizationModel *model = (AuthorizationModel*)(*it);
                        if (model->mGUID == mProductGUID && mObserver) {
                            mObserver->onCommunicateCallback(
                                        CommunicatorObserver::COMM_AUTH,
                                        model->mAuthorization?CommunicatorObserver::AUTHORIZED:CommunicatorObserver::UNAUTHORIZE,
                                        NULL);
                        }
                        mAuthTespan = model->mTespan;
                        mAuthState = (bool)model->mAuthorization;
                        delete model;
                    }
                    break;
                }
                case COMMAND_COMMAND: {
                    std::list<void*>::iterator it;
                    for (it=iter->second.begin();it!=iter->second.end();++it) {
                        CommandModel *model = (CommandModel*)(*it);
                        if (mObserver) {
                            mObserver->onCommunicateCallback(
                                        CommunicatorObserver::COMM_REMOTE,
                                        CommunicatorObserver::COMMAND,
                                        model);
                        }
                        delete model;
                    }
                    break;
                }
                case COMMAND_POLICY: {
                    std::list<void*>::iterator it;
                    for (it=iter->second.begin();it!=iter->second.end();++it) {
                        PolicyModel *model = (PolicyModel*)(*it);
                        if (mObserver) {
                            mObserver->onCommunicateCallback(
                                        CommunicatorObserver::COMM_REMOTE,
                                        CommunicatorObserver::POLICY,
                                        model);
                        }
                        delete model;
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
        else {
            if (mObserver) {
                mObserver->onCommunicateCallback(
                            CommunicatorObserver::COMM_ERROR,
                            CommunicatorObserver::INFO_ERROR,
                            (void*)&data);
            }
        }

        // delete the data
        result.clear();
    }
    else if (HEAD == event) {
        if (SGUID_INIT == type) {
            if (mSGUID != data) {
                mSGUID = data;
                // 强行设置  error, 以便让心跳后强行上报计算机信息
                mConnectError |= 100<<16;
                if (mObserver) {
                    mObserver->onCommunicateCallback(
                                CommunicatorObserver::COMM_INIT,
                                CommunicatorObserver::GENERAL_SGUID,
                                (void*)&mSGUID);
                }
            }
        }
        else if (TESPAN_CHANGE == type) {
            LOG_GLTX("mTespan = atoi:%s", data.c_str());
            this->mTespan = atoi(data.c_str());
        }
        else if (DATE_CHANGED == type) {
            mAuthState = false;
        }
        else if (HEAD_ERROR == type) {
            if (mObserver) {
                mObserver->onCommunicateCallback(CommunicatorObserver::COMM_ERROR, CommunicatorObserver::HEAD_ERROR, NULL);
            }
        }
        else if (EID_CHANGED == type) {
            mEid = data;
            // 强行设置  error, 以便让心跳后强行上报计算机信息
            mConnectError |= 100<<16;
            if (mObserver) {
                mObserver->onCommunicateCallback(
                            CommunicatorObserver::COMM_INIT,
                            CommunicatorObserver::EID_CHANGED,
                            (void*)&mEid);
            }
        }
    }

    return 0;
}

int Communicator::Work() {
    int err = -1;
    // 202104/13, heartbeat is alway the fisrt.
    HeartBeat();

    // first, send heartbeat, to request the SGUID
    while (err&&mSGUID.empty()) {
        err = HeartBeat();
        sleep(5);
    }

    if (mObserver && 0 == ReportClientInfo()) {
        mObserver->onCommunicateCallback(
                    CommunicatorObserver::COMM_INIT,
                    CommunicatorObserver::INFO_REPORTED,
                    NULL
        );
    }

    // first, need a heartbeat
    HeartBeat();
    pthread_create(&mAuthThread, NULL, Authrization, this);

    while (mState) {
        err = HeartBeat();
        if (0 == err && mConnectError>>16) {
            if (mObserver && 0 == ReportClientInfo()) {
                mObserver->onCommunicateCallback(
                            CommunicatorObserver::COMM_INIT,
                            CommunicatorObserver::INFO_REPORTED,
                            NULL
                );
            }

            // reset the error
            mConnectError = 0;
        }

        pthread_testcancel();

        sleep(HeartBeatInterval());
    }
    return err;
}

unsigned int Communicator::HeartBeatInterval() const {
    if (Utils::Path::FileAvailed("/tmp/rsdebug")) {
        return COMMUNICATE_HEART_INTERVAL;
    }
    LOG_GLTX("HeartBeatInterval: %d\n", mTespan);
    return mTespan>0?mTespan:TESPAN_DEFAULT_VALUE;
}

int Communicator::Auth() {
    while (mState) {
        time_t stamp = mAPIService->GetServerTime();
        struct tm *now;
        now = localtime(&stamp);

        if (mAuthDay != now->tm_mday) {
            mAuthState = false;
            mAuthDay = now->tm_mday;
        }

        if (!mAuthState) {
            RequestAuthorization();
        }

        pthread_testcancel();
        sleep(mAuthTespan>0?mAuthTespan:300);
    }
    return 0;
}

int Communicator::ReportRUALog(LogRuaSourceType source,
                               LogRuaActionType action,
                               LogRuaRoleType role,
                               const std::string& oldver,
                               const std::string& newver,
                               bool needreboot) {
    string system, user;
    if (mObserver) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_SYSTEM, system);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, user);
    }
    LogRuaModel log(source, action, role, oldver, newver, needreboot);

    std::list<LogRuaModel*> logs;
    logs.push_back(&log);

    ReportLogModel<LogRuaModel> report(LOG_RUA_EVENT, APPEND, logs);
    report.mEID = mEid;
    report.mSGUID = mSGUID;
    report.mSysType = mSystem;

    Json::Value json;
    json.append(report.toJSON());
    ContentBundle content(LOG, json);
    MessagePartBundle part(COMMAND_REPORT_LOG, &content);

    MessageBundle message(&part);
    string body = message.serialize();

    LOG_GLTX("body request %s", body.c_str());

    return mAPIService->reportData(COMMAND_REPORT_LOG, body);
}

int Communicator::ReportEPState(const string& state, const string& value, LogDealType dealtype) {
    string system, user;
    if (mObserver) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_SYSTEM, system);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, user);
    }
    LogEPState basicLog(system, user, state, value);

    std::list<LogEPState*> logs;
    logs.push_back(&basicLog);

    ReportLogModel<LogEPState> report(LOG_TYPE_BASIC_PLATFORM, dealtype, logs);
    report.mEID = mEid;
    report.mSGUID = mSGUID;
    report.mSysType = mSystem;

    Json::Value json;
    json.append(report.toJSON());
    ContentBundle content(LOG, json);
    MessagePartBundle part(COMMAND_REPORT_LOG, &content);

    MessageBundle message(&part);
    string body = message.serialize();

    LOG_GLTX("body request %s", body.c_str());

    return mAPIService->reportData(COMMAND_REPORT_LOG, body);
}

int Communicator::ReportClientInfo() {
    // upload the client info

    vector<pair<string,string> > subproducts;
    if (mObserver) {
        string data;
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_SUB_COMPONENTS, data);
        Json::Value values;
        Json::Reader reader;
        if (reader.parse(data, values)) {
            for (int i=0; i<values.size(); ++i) {
                Json::Value sub = values[i];
                subproducts.push_back(make_pair(sub["guid"].asString(), sub["version"].asString()));
            }
        }
    }

    int err = UploadClientInfo(subproducts);
    SECURITY_CHECK(err, 0, err);

    // report the EP state
    string vlibver;
    if (mObserver) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_VLIB_VERSION, vlibver);
    }
    if (!vlibver.empty()) {
        err = ReportEPState(EP_VIRUS_LIB_VERSION, vlibver);
    }

    return 0;
}


int Communicator::ReportVlibVersion(const string& version) {
    return ReportEPState(EP_VIRUS_LIB_VERSION, version, UPDATE_APPEND);
}

int Communicator::ReportDefenState(const string& defen, bool state) {
    if (defen == FILE_DEFEN_MODULE) {
        return ReportEPState(EP_FILE_MONITOR_STATE, Utils::String::itoa(state));
    }
    return -1;
}

int Communicator::ReportScanLog(LogDataScanEvent::LogScanType logtype,
                    LogDataScanEvent::CommandSender sender,
                    string starttime,
                    long taskid,
                    int runtime,
                    int scancount,
                    int viruscount,
                    int treatedcount,
                    int totalfilecount,
                    int scanfilecount,
                    LogDataScanEvent::LogScanState state) {

    string system, user, computer;
    if (mObserver) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_SYSTEM, system);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, user);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_NAME, computer);
    }

    LogDataScanEvent scanLog((LogDataScanEvent::LogScanType)logtype, sender, system, user, computer, mGUID, mVersion, taskid, mSystem);
    scanLog.SetLogData(starttime, runtime, scancount, viruscount, treatedcount, totalfilecount, scanfilecount, state);

    std::list<LogDataScanEvent*> logs;
    logs.push_back(&scanLog);

    ReportLogModel<LogDataScanEvent> report(LOG_TYPE_SCAN_EVENT, UPDATE_APPEND, logs);

    Json::Value json;
    json.append(report.toJSON());
    ContentBundle content(LOG, json);
    MessagePartBundle part(COMMAND_REPORT_LOG, &content);

    MessageBundle message(&part);
    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REPORT_LOG, body);
}

int Communicator::ReportVirusLog(LogDataScanEvent::LogScanType logtype,
                                 LogDataScanEvent::CommandSender sender,
                                 long foundtime,
                                 const std::string& filepath,
                                 int virusid,
                                 const std::string& virusname,
                                 int deal,
                                 int state,
                                 const std::string& hash) {

    string system, user, computer;
    if (mObserver) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_SYSTEM, system);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, user);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_NAME, computer);
    }

    LogDataVirus scanLog(logtype, sender, system, hash, computer, mGUID, mVersion, mSystem);
    scanLog.SetVirusData(foundtime, filepath, virusid, virusname, deal, state);

    std::list<LogDataVirus*> logs;
    logs.push_back(&scanLog);

    ReportLogModel<LogDataVirus> report(LOG_PC_VIRUS_EVENT, UPDATE_APPEND, logs);

    Json::Value json;
    json.append(report.toJSON());
    ContentBundle content(LOG, json);
    MessagePartBundle part(COMMAND_REPORT_LOG, &content);

    MessageBundle message(&part);
    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REPORT_LOG, body);
}

int Communicator::ReportCommandLog(const string& productid, const std::string& action, const std::string& cmdid, int type, int result) {
    string system, user, computer;
    if (mObserver) {
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_SYSTEM, system);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_USER, user);
        mObserver->requestClientInfo(CommunicatorObserver::CLIENT_NAME, computer);
    }

    LogCommandState commandLog(system, user, computer, mGUID, mVersion, mSystem, mEid, mSGUID, productid);
    commandLog.SetCommandData(action, cmdid, type, result, "");

    std::list<LogCommandState*> logs;
    logs.push_back(&commandLog);

    ReportLogModel<LogCommandState> report(LOG_PC_COMMAND_EVENT, UPDATE_APPEND, logs);

    Json::Value json;
    json.append(report.toJSON());
    ContentBundle content(LOG, json);

    MessagePartBundle part(COMMAND_REPORT_LOG, &content);

    MessageBundle message(&part);
    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REPORT_LOG, body);
}

int Communicator::ReportFile(const string& file){
    Json::Value fileJson;
    fileJson["file"]=Json::Value(file);
    ContentBundle contentBundle(UPLOAD, fileJson);
    MessagePartBundle part(COMMAND_REPORT_FILE, &contentBundle);
    MessageBundle message(&part);
    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REPORT_FILE, body);
}

void* Communicator::WorkThread(void *args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);

    Communicator *self = (Communicator*)args;
    int err = self->Work();
    if (err) {
        self->mObserver->onCommunicateCallback(CommunicatorObserver::COMM_ERROR, CommunicatorObserver::NOT_CONNECT, (void*)&err);
    }

    pthread_exit(0);
    return NULL;
}

void* Communicator::Authrization(void *args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);

    Communicator *self = (Communicator*)args;
    self->Auth();

    pthread_exit(0);
    return NULL;
}

int Communicator::Start() {
    mState = 1;
    SECURITY_CHECK(pthread_create(&mWorkThread, NULL, WorkThread, this), 0, -1);
    return 0;
}

int Communicator::Stop() {
    mState = 0;

    if (mWorkThread > 0) {
        pthread_cancel(mWorkThread);
        pthread_join(mWorkThread, NULL);

        mWorkThread = 0;
    }

    if (mAuthThread > 0) {
        pthread_cancel(mAuthThread);
        pthread_join(mAuthThread, NULL);

        mAuthThread = 0;
    }

    return 0;
}

int Communicator::ReportUninstall() {
//    MessagePartBundle part(COMMAND_REPORT_UNINSTALL, NULL);

//    MessageBundle message(&part);
//    string body = message.serialize();

    return mAPIService->reportData(COMMAND_REPORT_UNINSTALL, "");
}

