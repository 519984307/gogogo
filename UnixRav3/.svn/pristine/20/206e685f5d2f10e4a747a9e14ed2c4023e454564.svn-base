#ifndef NOTIFYCONTROLLER_H
#define NOTIFYCONTROLLER_H

#include "centerlistener.h"
#include "dbuslistener.h"
#include "baseconfig.h"
#include "communicatebundle.h"
#include "storagebridge.h"
#include "handler.h"
#include "commonlocker.h"

#include <queue>

class CenterListener;
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

class NotifyController: public StorageBridge,
                        public CenterCommandObserver,
                        public WidgetObserver,
                        public HandleObserver
{
public:
    static NotifyController* NEW(const char *installdir);
    ~NotifyController();

public:
    NotifyController(const char *installdir);
    int Construct();

    void SetWidget(ScreenWidget *widget) {
        mWidget = widget;
    }

    void Start();
    void Stop();

private:
    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result);

    // HandleObserver
    void HandleMessage(int event, int type, void *object);

    // WidgetObserver
    void OnWidgetEvent(int event, int type, void *data);

private:
    int OpenApp(const char *app, const char *command);
    int KillProc(const char *app, int signal);
    int SendToDaemon(int command);

    int NotifyService(const char *folder, const char *sock);

    int RespondNotify(const std::string& data);

    void BroadcastSelf();

    enum NOTIFY_TO{
        TO_GJCZ,
        TO_GLTX,
        TO_ZDFY,
    };
    int RespondProxyEvent(CommandBundle& bundle, enum NOTIFY_TO who, void **reslult);

private:
    ScreenWidget    *mWidget;

    CenterListener  *mGJCZReceiver;
    CenterListener  *mGLTXReceiver;
    CenterListener  *mZDFYReceiver;

    Handler *mAsyncPool;

    std::string mInstallDir;
    AppConfig   *mAppConfig;

    std::queue<CommunicateBundle> mCommPool;

    pthread_t mCheckThread;

    MutexLocker *mLocker;

    static void *ThreadWork(void *params);
    int CommunicateWork();

    bool mState;
};

#endif // NOTIFYCONTROLLER_H
