#ifndef RJJH_MAINENGINE_H
#define RJJH_MAINENGINE_H

#include "centerlistener.h"
#include "baseconfig.h"
#include "communicatebundle.h"
#include "communicator.h"
#include "storagebridge.h"
#include "handler.h"
#include "commonlocker.h"

#include <queue>

class CenterListener;
class ProxyInfoListener;
class ScreenWidget;
class ClientInfo;
class AppConfig;

class WidgetObserver {
public:
    enum {
        MENU_EVENT = 1,
    };

    enum {
        OPEN_MAIN_APP = 1,
        OPEN_SETTING_APP,
        OPEN_LOG_APP,
        OPEN_UPDATE,
        EXIT_ALL
    };
public:
    virtual void OnWidgetEvent(int event, int type, void *data) = 0;
};

class MainEngine: public StorageBridge,
                  public CenterCommandObserver,
                  public HandleObserver
{
public:
    static MainEngine* NEW(AppConfig *config, bool silence);
    virtual ~MainEngine();

public:
    MainEngine(AppConfig *config);
    int Construct(bool silence);

    void Start();
    void Stop();

private:
    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result, int *outlen);

    // HandleObserver
    void HandleMessage(int event, int type, void *object);

private:
    int OpenApp(const char *app, const char *command);
    int KillProc(const char *app, int signal);
    int SendToDaemon(int command);

    int NotifyService(const char *folder, const char *sock);
    int NotifyService(uint32_t addr, short port);

    int RespondNotify(const std::string& data);

    int InitializeGeneralPolicy();
    int ResetConfig();

    enum NOTIFY_TO{
        TO_GJCZ,
        TO_GLTX,
        TO_ZDFY,
    };
    int RespondProxyEvent(CommandBundle& bundle, enum NOTIFY_TO who, void **result, int *outlen);

    int RespondUpdateEvent(int type, const string& value);

    int SendGroupBroadcast(uint32_t addr, short port, unsigned char *data, int len);

    /**
     * @brief RechownQuarantine
     *  此函数的作用是修复从隔离区恢复文件的文件所有者属性
     *  在中标麒麟上GJCZ恢复的文件所有者无法修改，所以在此做补救
     * @return
     */
    int RechownQuarantine();

private:
    CenterListener      *mGJCZReceiver;
    CenterListener      *mGLTXReceiver;
    CenterListener      *mZDFYReceiver;

    pthread_t       mCheckThread;

    static void *ThreadWork(void *params);
    void NotifyThreadFunc();

    Handler *mAsyncPool;

    AppConfig   *mAppConfig;

    std::queue<CommunicateBundle> mCommPool;

    bool mState;

#ifdef WITH_QT
private:
    int RepairConfig();
#endif

private:
#ifndef WITH_QT
    bool mQtSupport;

    void *mDynamicHandler;

    typedef void(*qt_creator)(bool, AppConfig*);
    typedef void(*qt_toast)(CommunicateBundle*);
    typedef void(*qt_tray)(bool);
    typedef void(*qt_unload)();

    qt_toast    mNotifyFunc;
    qt_tray     mSetTrayStateFunc;
    qt_unload   mUnloadFunc;

    int LoadQtWidgetLib(bool traystate);
    int UnloadQtWidget();
#endif

    int SetQtTrayIconState(bool state);
    int ShowNotifiy(CommunicateBundle *bundle);
};

#endif // RJJH_MAINENGINE_H
