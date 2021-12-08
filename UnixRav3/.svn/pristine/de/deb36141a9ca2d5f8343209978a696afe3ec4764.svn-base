#ifndef GLTX_MAINENGINE_H
#define GLTX_MAINENGINE_H

#include "policysyncparser.h"
#include "service.h"
#include "commonlocker.h"
#include "communicatebundle.h"
#include "communicator.h"
#include "storagebridge.h"
#include "centerfindproxy.h"
#include "scheduleengine.h"
#include "Common/updateobserver.h"
#include "Common/updateconfig.h"

#include <queue>
#include <vector>

class ClientInfo;
class Storage;
class CenterListener;
class PolicyModel;
class CommandModel;
class PolicyScanModel;
class UpdateAppEngine;
class GLTXUpdateVirusEngine;

class MainEngine :  public StorageBridge,
                    public ServiceBinder,
                    public CommunicatorObserver,
                    public CenterFindObserver,
                    public ScheduleObserver,
                    public UpdateObserver
{
public:
    static MainEngine*  NEW();
    ~MainEngine();
    MainEngine();

    // from ServiceBinder
    int Stop();
    int Uninstall();

    Communicator* GetCommunicator() const {
        return mCommunicator;
    }
protected:
    // from CommunicatorObserver
    void onCommunicateCallback(COMM_EVENT event, COMM_TYPE type, void *object);
    int requestClientInfo(ClientInfoType type, string& data);

    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result, int *outlen);

    // HandleObserver
    void HandleMessage(int event, int type, void *object);

    // CenterFindObserver
    int onCenterFindEvent(int event, string& eid, vector<string>& centers);

    // from ScheduleObserver
    void OnScheduleEvent(int event, int type, void* object);

    // from UpdateObserver
    int OnUpdateEvent(int event, int type, void *object);

    int GetConnectCenter(UPDATE_SOURCE type, const string& uri);

    // from ServiceBinder
    int DelayConstruct();
    int Start();
    int ProcessLinuxPolicy(PolicyModel *model, Storage *storage, int type);
    int ProcessGlobalPolicy(PolicyModel *model, Storage *storage, int type);
    int ProcessDeployPolicy(PolicyModel *model, Storage *storage, int type);
    int ProcessCenterPolicy(PolicyModel *model, Storage *storage, int type);
    int ProcessFirewallPolicy(PolicyModel *model, Storage *storage, int type);

    Storage* GetStorage() const {
        return mStorage;
    }

private:
    int Construct();

    int MakeGUID(string& guid);

    int InitCommunication(const string& center_url);
    int InitCommConfig(const string& config_file);
    int FindNeighbors();

    static void *ThreadWork(void *params);
    int CommunicateWork();

    static void *ProxyWork(void *params);
    int ProxyFunction();

    int ClearLastState();

    int RespondClientCommand(const CommandBundle& command, void **result, int *outlen);

    int RespondRepairCommand(const CommandBundle& command, void **result, int *outlen);
    int RespondRemoteCommand(const CommandBundle& command, void **result, int *outlen);

    int SaveCommandLog(int logtype, const string& flag, const string& desc, bool reported);

    int ProcessCenterCommand(CommandModel *model);

    /**  respond the virus log from GJCZ */
    int ProcessReportVirusLog(const string& data);
    /**  respond the scan log from GJCZ */
    int ProcessReportScanLog(const string& data);

    int BroadcastCenter(bool status);

    int RespondConnectError(int error);

    string GetCenterIP();

    int ReloadScheduleList();

    static void* Schedule(void*);

    int StartUpdateFunc(bool offline, const string& uri = "");

    int CloudPostProxy(const string& data, void **result, int *outlen);
    int CloudPostCheck(const string& data, void **result, int *outlen);
    int HttpDownload(const string& url, void **result, int *outlen);

    int RespondUpdateApp(int event, int type, void *data);
    int RespondUpdateVirus(int event, int type, void *data);

    int SendInfoToNotifyApp(int command, int type, const string& data);

    int ImportCrossUpdateInfo();

    bool RepairConfigFile();
    int  AddQuarantineFileToWhitelist(const string& file);
    void AsyncUploadLog();

    int  StartTrayProcess(bool shell_login, bool silence, uid_t uid);

    int  CheckRegisted(const string& serial, const char * license);
    bool CheckDuplicateSGUID();

private:
    ScheduleEngine      *mScheduleEngine;
    Communicator        *mCommunicator;
    ClientInfo          *mClientInfo;
    CenterFindProxy     *mCenterFinder;
    //To save command info for further ReportCommandLog
    CommandModel        *mTempCommand;

    UpdateAppEngine         *mUpdateAppEngine;
    GLTXUpdateVirusEngine   *mUpdateVirusEngine;

    pthread_t mCheckThread;
    pthread_t mProxyThread;

    bool mCenterFinderProxyState;
    CondLocker  *mFinderProxyLocker;

    pthread_t mUploadLogThread;
    static void* UploadLogThreadFunc(void *params);

private:
    /** center url list */
    std::queue<SourceListEntry> mCenterList;
    std::string mCurrentCenter;

    bool        mUpdatePrompt;
    enum UpdateStep {
        UPDATE_STEP_INIT         = -1,
        UPDATE_STEP_VIRUS_LIB    = 0,
        UPDATE_STEP_APP
    } mUpdateStep;

    int mUpdateErrorCode;
    bool mUpdateNeedRestart;

    int StartUpdate(enum UpdateStep step, pair<UPDATE_SOURCE,string>& url);
    /** 更新升级的结果，包括病毒库版本和应用版本
     * 在更新时，默认会在/ETC下找RAVCFG， 如果有，则
     * 写入配置，通知RJJH更新，因为RJJH做为用户进程可能
     * 没有读取权限， 如果/ETC下没有，则此安装方式可能为保密
     * 机安装，那么将数据传递给RJJH，由其写入
     * params sectionflag, define in AppConfig SECTION_FLAG
     */
    int SaveUpgradeInfo(int sectionflag, const string& value);

private:
    int     mHealthStatus;
    /** 定义授权框弹出的间隔时长， 每一小时提醒一次 */
    time_t  mLastAuthPromptTime;

private:
    /**
     * the status to connect the center
     */
    enum {
        NOT_CONNECTED = -1,
        CONNECT_SUCCESS = 0,            // Connect center success
        RETRY_OTHER_CENTER,     // retry other center, center from policy
        ASK_NEIGHBORS           // ask other neighbors
    }mConnectStatus;

    /** connect center, if error retry it */
    time_t mDisconnectTime;

    /** update pool */
    vector<std::pair<UPDATE_SOURCE, std::string> > mCenterAddr;

    pthread_t    mUpdateThread;
    static void* UpdateThread(void *params);
    void UpdateThreadFunc();
    CondLocker  *mUpdateLocker;

    /** 升级的状态，用以标注当次升级的状态 */
    map<enum UpdateStep, bool> mUpdateState;

    /** 是否检查并上传日志 */
    bool  mCheckLogUpload;
    MutexLocker *mUploadLogMutex;

private:
    /*For ReportCommandLog(command guid needed).
    To avoid pointer memory release fault, don't use mTempCommand*/
    string CmdId;
    ExecuteCommand mExecuteModel;
    void ParsePath(string &path);

	/** ProcessCenterCommand:REMOTE_EXE:Execute */
    pthread_t    mExecuteThread;
    static void* ExecuteThread(void *params);
    void ExecuteThreadFunc();

    /** ProcessCenterCommand:REMOTE_EXE:Download */
    pthread_t   mDownloadThread;
    static void* DownloadThread(void *params);
    void DownloadThreadFunc();
    int mDownloadErr;

    /** ProcessCenterCommand:REMOTE_EXE:Upload */
    pthread_t    mUploadFileThread;
    static void* UploadFileThread(void *params);
    void UploadFileThreadFunc();
};

#endif // ZDFY_MAINENGINE_H
