#include <stdio.h>

#include "centerlistener.h"
#include "communicatebundle.h"
#include "communicator.h"
#include "clientinfo.h"


class MainEngine : public CenterCommandObserver, public CommunicatorObserver {
public:
    MainEngine() {
        mListener = CenterListener::NEW(
                    this,
                    CenterCommandObserver::CLIENT_COMMAND,
                    "/opt/BDFZ/RAV/RJJHGJCZ/rjjhgjcz.sock",
                    true);

        ggfwrjjh = CenterListener::NEW(
                    this,
                    CenterCommandObserver::CLIENT_COMMAND,
                    "/opt/BDFZ/RAV/GGFWRJJH/rjjhggfw.sock",
                    true);

        rjjhgltx = CenterListener::NEW(
                    this,
                    CenterCommandObserver::CLIENT_COMMAND,
                    "/opt/BDFZ/RAV/RJJHGLTX/rjjhgltx.sock",
                    true);

        mClientInfo = new ClientInfo;
        center = Communicator::NEW("https://193.168.14.15/Api/Api.php", "99B51EC462882537", "d3e43712-9259-c999-d494-9acc5847d9c2", "", "26.03.05", "20.02.1521");
        center->setObserver(this);
    }


    ~MainEngine() {
        delete mListener;
    }

    int OnCommandObserver(int event, void *args, void **result) {
        return 100;
    }

    void onCommunicateCallback(COMM_EVENT event, COMM_TYPE type, void *object) {

    }

    int requestClientInfo(ClientInfoType type, string& data) {
        switch (type) {
        case CLIENT_NAME:
            return mClientInfo->GetClientName(data);
        case CLIENT_ARCH:
            return mClientInfo->GetClientArch(data);
        case CLIENT_IP: {
            int retry = 0;
            while(retry<3 && data.empty()) {
                sleep(1*retry++);
                mClientInfo->GetLocalIP(data);
            }
            return data.empty()?1:0;
        }
        case CLIENT_ISVM:
            break;
        case CLIENT_MAC:
            return mClientInfo->GetMacAddr(data);
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
        case CLIENT_VER: {
            data = "26.03.08";
            return true;
        }
        case CLIENT_SYSTEM:
            return mClientInfo->GetSystem(data);
        case CLIENT_PRODUCT_TYPE: {
            data = "1AB67467-096C-4bea-B852-2CA73F6E854C";
            return 0;
        }
        case CLIENT_VLIB_VERSION: {
            data = "30.12.2563";
            return 0;
        }
        default:
            break;
        }
        return 0;
    }

    void Start() {
        mListener->Start();
        ggfwrjjh->Start();
        rjjhgltx->Start();
        center->Start();
    }

public:
    CenterListener *mListener, *ggfwrjjh, *rjjhgltx;
    Communicator *center;
    ClientInfo          *mClientInfo;
};

int main_func() {
    FILE *f = fopen("/opt/BDFZ/RAV/RJJHGJCZ/rjjhgjcz.txt", "w+");
    if (f) {
        fputs("rjjh put it", f);
        fclose(f);
    }


    f = fopen("/opt/BDFZ/RAV/GGFWRJJH/rjjhggfw.txt", "w+");
    if (f) {
        fputs("rjjh put it", f);
        fclose(f);
    }

    f = fopen("/opt/BDFZ/RAV/RJJHGLTX/rjjhgltx.txt", "w+");
    if (f) {
        fputs("rjjh put it", f);
        fclose(f);
    }
    f = fopen("/opt/BDFZ/RAV/RJJHetc/rjjh.txt", "w+");
    if (f) {
        fputs("rjjh put it", f);
        fclose(f);
    }

    MainEngine *engine = new MainEngine();
    engine->Start();

    while(1) {
        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "rjjh to gjcz";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/RJJHGJCZ/gjczrjjh.sock", ret);
            char temp[128];
            sprintf(temp, "rjjh send gjcz err is %d -- %d\n", err,ret);
            printf(temp);

            FILE *f = fopen("/opt/BDFZ/RAV/RJJHGJCZ/rjjhgjcz.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }

        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "gltx to ggfw";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/GGFWRJJH/ggfwrjjh.sock", ret);

            char temp[128];
            sprintf(temp, "rjjh send ggfw err is %d -- %d\n", err,ret);
            printf(temp);
            FILE *f = fopen("/opt/BDFZ/RAV/GGFWRJJH/rjjhggfw.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }

        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "rjjh to gltx";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/RJJHGLTX/gltxrjjh.sock", ret);

            char temp[128];
            sprintf(temp, "rjjh send gltx err is %d -- %d\n", err,ret);
            printf((temp));
            FILE *f = fopen("/opt/BDFZ/RAV/RJJHGLTX/rjjhgltx.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }

        sleep(5);
    }
    return 0;
}


int main() {
    pid_t pid;
    if ((pid=fork()) == 0) {
        return main_func();
    }
    return 0;
}
