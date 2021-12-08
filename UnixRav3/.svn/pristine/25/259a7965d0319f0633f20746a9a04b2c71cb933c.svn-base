#ifndef ZDFY_MAINENGINE_H
#define ZDFY_MAINENGINE_H

#include "service.h"
#include "storagebridge.h"
#include "webserver.h"

#include <queue>
#include <map>

class ClientInfo;
class Storage;
class MultiVirusEngine;
class CenterListener;
class ProcDefenModProxy;
class NetfilterModProxy;
class PolicyModel;
class CommandModel;
class PolicyScanModel;
class ScanProxy;
class DefenPlugins;

class MainEngine :  public StorageBridge,
                    public ServiceBinder,
                    public WebService::WebObserver
{
public:
    static MainEngine*  NEW();
    ~MainEngine();

    // from ServiceBinder
    int Stop();

protected:
    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result, int *outlen);

    // HandleObserver
    void HandleMessage(int event, int type, void *object);

    // from ServiceBinder
    int DelayConstruct();

    // from WebService::WebObserver
    int OnWebRequestObserver(int event, const char *header, const char *body);

private:
    MainEngine();
    int Construct();

    int StartMonitor(const string& module);
    int StopMonitor(const string& module);

    int ReloadMonitorList();

    int InitBrowserMonitor();

    int RespondCientCommand(const CommandBundle &bundle);
    int RespondCenterCommand(CommandBundle::CommandExtendType type, const string &data);

    int ProcessCommand(CommandModel *model);

    int FinishRemoteCommand(const string& command_id);

    int ReportKoState(const string& flag, bool state);
    /** reload the config, first check storage */
    int ReloadConfig();

    //数据sqlite没有初始化完成，就对其进行使用（增删改查），会导致段错误。 todo
    bool mDb_bReady;
    Storage* GetStorage() const {
        return mStorage;
    }    

public:
    int SendToNotifer(CommunicateBundle::CommunicateType flag,
                      CommunicateBundle::NotifyType type,
                         const std::string& title,
                         const std::string& content,
                         const std::string& left="",
                         const std::string& right="");


private:
    string mTempBuffer;
    std::list<UrlRuleEntry> mUrlRuleLists;

    WebService::Server  *mWebServer;
    CenterListener      *mFirefoxListener;

private:
    class KernelModuleData {
    private:
        typedef int(*proxy_pointer)();
        typedef int(*loader_pointer)(ServiceBinder*, StorageBridge*, AppConfig*);

    public:
        void *mDefenProxyHandler;

        proxy_pointer   mUnload;
        proxy_pointer   mRefresh;
        loader_pointer  mDefenLoader;
    private:
        string mInstallDir;
        string mKOFilename;

    public:
        KernelModuleData(const string& installdir);
        ~KernelModuleData();
        int Construct(const string& module);
    };

    map<string, KernelModuleData*> mModules;
};

#endif // ZDFY_MAINENGINE_H
