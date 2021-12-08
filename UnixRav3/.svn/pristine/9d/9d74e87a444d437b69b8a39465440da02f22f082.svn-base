#include <stdio.h>
#include <unistd.h>

#include "communicator.h"
#include "centerlistener.h"

class MainEngine : public CommunicatorObserver, public CenterCommandObserver {
public:
    MainEngine() {
        center = Communicator::NEW("https://193.168.14.15/Api/Api.php", "99B51EC462882537", "1231-1231-12311313-1231-1231", "", "26.03.05", "20.02.1521");
        listener = CenterListener::NEW(
                    this,
                    CenterCommandObserver::CLIENT_COMMAND,
                    "/opt/BDFZ/RAV/GGFWGLTX/gltxggfw.sock",
                    true);

        rjjh = CenterListener::NEW(
                    this,
                    CenterCommandObserver::CLIENT_COMMAND,
                    "/opt/BDFZ/RAV/RJJHGLTX/gltxrjjh.sock",
                    true);
    }

    ~MainEngine() {
        delete center;
        delete listener;
    }

    void start() {
        center->Start();
        listener->Start();
        rjjh->Start();
    }

public:
    void onCommunicateCallback(COMM_EVENT event, COMM_TYPE type, void *object) {

        return;
    }

    int requestClientInfo(ClientInfoType type, string& data) {
        data = "fuck";
        return 0;
    }

    int OnCommandObserver(int event, void *args, void **result) {
        return 100;
    }

public:
    Communicator *center;
    CenterListener  *listener, *rjjh;
};


int main_func() {
    FILE *f = fopen("/opt/BDFZ/RAV/GGFWGLTX/gltxggfw.txt", "w+");
    if (f) {
        fputs("gltx put it", f);
        fclose(f);
    }

    f = fopen("/opt/BDFZ/RAV/RJJHGLTX/gltxrjjh.txt", "w+");
        if (f) {
            fputs("gltx put it", f);
            fclose(f);
        }
        f = fopen("/opt/BDFZ/RAV/GLTXetc/gltxs.txt", "w+");
            if (f) {
                fputs("gltx put it", f);
                fclose(f);
            }

    MainEngine *engine = new MainEngine;
    engine->start();

    while(1) {
        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "gltx to ggfw";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/GGFWGLTX/ggfwgltx.sock", ret);

            char temp[128];
            sprintf(temp, "gltx send ggfw err is %d -- %d\n", err,ret);

            FILE *f = fopen("/opt/BDFZ/RAV/GGFWGLTX/gltxggfw.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }

        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "gltx to rjjh";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/RJJHGLTX/rjjhgltx.sock", ret);

            char temp[128];
            sprintf(temp, "gltx send rjjh err is %d -- %d\n", err,ret);

            FILE *f = fopen("/opt/BDFZ/RAV/RJJHGLTX/gltxrjjh.txt", "a+");
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
