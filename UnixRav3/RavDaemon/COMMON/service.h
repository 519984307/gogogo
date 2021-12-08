#ifndef SERVICE_H
#define SERVICE_H

#include "centerlistener.h"
#include "handler.h"

#include <string>

using namespace std;

class AppConfig;
class Storage;
class MutexLocker;
class PolicyModel;
class ServiceBinder : public CenterCommandObserver, public HandleObserver
{
public:
    ServiceBinder();
    virtual ~ServiceBinder();

    int SendToNotifer(CommunicateBundle::CommunicateType flag,
                      CommunicateBundle::NotifyType type,
                         const std::string& title,
                         const std::string& content,
                         const std::string& left="",
                         const std::string& right="",
                         int overtime = 0);

    AppConfig* GetAppConfig() const {
        return mAppConfig;
    }

    int InitializeGeneralPolicy(Storage*);
    int NotifyData(const string& folder, const string& sock, CommandBundle& data);

    int GetClientUID() {
        return mNotifierUID;
    }

protected:
    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result, int *outlen);

    // HandleObserver
    void HandleMessage(int event, int type, void *object);

public:
    int Construct(const string& folder, const string& sock);

    static int NotifyData(
            const std::string& install,
            const std::string& folder,
            const std::string& sock,
            CommandBundle& data,
            bool system = true,
            int uid = 0);  

    std::string GetAppVersion() const;
    std::string GetVirusVersion() const;
    void SetVirusVersion(const string& version);

    /**
     * @brief RepairConfig
     * 此函数的作用是判断数据库存储的值是否和配置文件里的对应，
     * 如果不对应，以数据库里的为准，然后更新配置文件
     * @return
     */
    int RepairConfig();

    virtual int DelayConstruct();
    virtual int Start();
    virtual int Stop();

    virtual Storage* GetStorage() const = 0;

    virtual int ProcessLinuxPolicy(PolicyModel *model, Storage *storage, int type);
    virtual int ProcessGlobalPolicy(PolicyModel *model, Storage *storage, int type);
    virtual int ProcessDeployPolicy(PolicyModel *model, Storage *storage, int type);
    virtual int ProcessCenterPolicy(PolicyModel *model, Storage *storage, int type);
    virtual int ProcessFirewallPolicy(PolicyModel *model, Storage *storage, int type);

    int ResetConfig(Storage *storage);

    std::string GetFileOwner(const std::string& filepath);

protected:
    string GetTitle();

protected:
    int mNotifierUID;
    int mHealthStatus;

    int mStatus;

    enum CONSTRUCT_STEP {
        FIRST_FIND_PROXY = 0,
        SECOND_CONSTRUCT
    }mConstructStep;

    AppConfig   *mAppConfig;
    MutexLocker *mAppConfigLocker;

    std::string mOwnerFolder;
    std::string mListenSock;

    /** 是否需要RJJH进程去保存config文件，如果需要，则置此变量为true, 此时后台不再同步config */
    bool mNeedRJJHSaveConfig;
protected:
    CenterListener  *mCenterListener;
    Handler         *mAsyncPool;

    pthread_t  mHelperThread;
    static void* ProcessHelperThread(void *params);
    void ProcessHelperWork();
    /** check the envirtiment, include the usb, defen, filter */
    bool CheckEnvChange();
};

#endif // SERVICE_H
