#ifndef MAINENGINE_H
#define MAINENGINE_H

#include "communicator.h"
#include "engineobserver.h"
#include "controller.h"
#include "service.h"
#include "commonlocker.h"
#include "usbpluginchecker.h"
#include "communicatebundle.h"
#include "Common/updateobserver.h"
#include "appconfig.h"

#include <queue>
#include <vector>

class ClientInfo;
class Storage;
class ScheduleEngine;
class MultiVirusEngine;
class CenterListener;
class DevicePluginChecker;
class ScheduleScaner;
class PolicyModel;
class CommandModel;
class PolicyScanModel;
class ScanProxy;
class GJCZUpdateVirusEngine;

class MainEngine :  public Controller,
                    public UpdateObserver,
                    public ServiceBinder,
                    public USBPluginObserver
{
public:
    static MainEngine*  NEW();
    ~MainEngine();

    // from ServiceBinder
    int Stop();

public:
    MainEngine();
    int Construct();

    int FinishRemoteScan(const string& id);

    /**
     * @brief ReportRemoteStartEvent
     * 上报远程扫描的开始事件
     * @param type 事件类型，定义见 ScanProxy ScanType()
     * @return
     */
    int ReportRemoteStartEvent(int type);
int NotifyStatusToApp();
protected:
    // from EngineObserver
    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result, int *outlen);

    // USBPluginObserver
    int OnPluginEvent(enum PluginEvent event, const char *mapping);

    // UpdateObserver
    int OnUpdateEvent(int event, int type, void *object);

    // HandleObserver
    void HandleMessage(int event, int type, void *object);

    // from ServiceBinder
    int DelayConstruct();
    int Start();
    int ProcessLinuxPolicy(PolicyModel *model, Storage *storage);
    int ProcessGlobalPolicy(PolicyModel *model, Storage *storage, int type);

    Storage* GetStorage() const {
        return mStorage;
    }

    string GetTitle();

public:
//    // from Controller,
//    int QueryUserForResult(int/** Application::QUERY_EVENT **/ event,
//                                       const string& file,
//                                       const string& virus);

private:
    int ExitProc(const string& sock, int command);

    int DefenQuickScan(const string& file);
    int GetDefenScanConfig(ScanConfig& config);

    int ImportCrossUpdateInfo(string& version, string& guid, string& sguid);

private:
    int ReportScanLog(long taskid, int logId, int extend=0);
    int ReportVirusLog(int type, int scan_logid);
    int ReportVirusLog(int type, VirusInfoEntry& entry);
    static void* ReportVirusLogThread(void *params);

    int ProcessCommand(CommandModel *model);

    int SaveCommandLog(int logtype, const string& flag, const string& desc);

    int ProcessRightKeyContextMenuScan(const string& file);

    int InitCommunication();
    int InitUSBPluginMonitor();

    int RepairProcStartMonitor();
    int RepairFileMonitor();
    int RepairNetfilter();

    int RepairMonitor();

    int ClearLastState();

    int RespondCientCommand(const CommandBundle& command, void** result, int* outlen);
    int RespondScanCommand(const CommandBundle& command);
    int RespondQuarantineCommand(CommandBundle::CommandExtendType type, const std::string& data);
    int RespondCenterCommand(CommandBundle::CommandExtendType type, const std::string& data, void **result, int* outlen);
    int RespondUpdateCommand(CommandBundle::CommandExtendType type, const std::string& data, void **result, int* outlen);

    /** type is UPDATE_VIRUS_TYPE, define in GJCZUpdateVirusEngine */
    int StartUpdate(int /*UPDATE_VIRUS_TYPE*/type, const string& navigate);

    int RespondScheduleScan(int event);

    int RespondDefenVirus(VirusInfoEntry& entry);

    int ReloadConfig();
    int CheckUSBMonitor();

    int AddQuarantineFileToWhitelist(const QuarantineLog& log);

    std::string QueryPathInfoJsonString(const std::string& paths);

    // 获取有指定文件的U盘地址, 指定文件为 .rising_test_file
    int GetMyUSB();
    // 检查是否为自定义扫描, 如果是自定义扫描, 结合其它选项,搞些事情
    bool mCustomScan;

    bool GetStatus() {
        return mStatus;
    }

private:
    USBPluginChecker        *mUSBChecker;
    GJCZUpdateVirusEngine   *mUpdateEngine;

private:
    std::string mPluginMountPath;

    long mScanTaskID;
};

#endif // MAINENGINE_H
