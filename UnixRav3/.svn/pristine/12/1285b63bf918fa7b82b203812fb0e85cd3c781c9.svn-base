#include <stdio.h>
#include <unistd.h>

#include "centerlistener.h"

class MainEngine : public CenterCommandObserver {
public:
    MainEngine() {
        ggfwrjjh = CenterListener::NEW(this,
                                       CenterCommandObserver::CLIENT_COMMAND,
                                       "/opt/BDFZ/RAV/GGFWRJJH/ggfwrjjh.sock",
                                       true);
        ggfwgjcz = CenterListener::NEW(this,
                                       CenterCommandObserver::CLIENT_COMMAND,
                                       "/opt/BDFZ/RAV/GGFWGJCZ/ggfwgjcz.sock",
                                       true);
        ggfwgltx = CenterListener::NEW(this,
                                       CenterCommandObserver::CLIENT_COMMAND,
                                       "/opt/BDFZ/RAV/GGFWGLTX/ggfwgltx.sock",
                                       true);
    }

    ~MainEngine() {

    }

    void start() {
        ggfwrjjh->Start();
        ggfwgjcz->Start();
        ggfwgltx->Start();
    }

    int OnCommandObserver(int event, void *args, void **result) {
        return 100;
    }

public:
    CenterListener *ggfwrjjh;
    CenterListener *ggfwgjcz, *ggfwgltx;
};


int main_func() {
    FILE *f = fopen("/opt/BDFZ/RAV/GGFWGLTX/ggfwgltx.txt", "w+");
    if (f) {
        fputs("ggfw put it", f);
        fclose(f);
    }

    f = fopen("/opt/BDFZ/RAV/GGFWRJJH/ggfwrjjh.txt", "w+");
        if (f) {
            fputs("ggfw put it", f);
            fclose(f);
        }

        f = fopen("/opt/BDFZ/RAV/GGFWGJCZ/ggfwgjcz.txt", "w+");
            if (f) {
                fputs("ggfw put it", f);
                fclose(f);
            }
            f = fopen("/opt/BDFZ/RAV/GGFWetc/ggfw.txt", "w+");
                if (f) {
                    fputs("ggfw put it", f);
                    fclose(f);
                }
    MainEngine *engine = new MainEngine;
    engine->start();

    while(1) {
        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "ggfw to gjcz";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/GGFWGJCZ/gjczggfw.sock", ret);

            char temp[62];
            sprintf(temp, "ggfw sebd gjcz err is %d -- %d\n", err,ret);

            FILE *f = fopen("/opt/BDFZ/RAV/GGFWGJCZ/ggfwgjcz.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }

        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "ggfw to rjjh";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/GGFWRJJH/rjjhggfw.sock", ret);

            char temp[62];
            sprintf(temp, "ggfw send rjjh err is %d -- %d\n", err,ret);

            FILE *f = fopen("/opt/BDFZ/RAV/GGFWRJJH/ggfwrjjh.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }

        {
            CommandBundle bundle;
            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
            bundle.mIntegerValue = 1000;
            bundle.mStringValue = "ggfw to gjcz";

            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
            int ret = 0;
            int err =  pack.Externalize("/opt/BDFZ/RAV/GGFWGLTX/gltxggfw.sock", ret);

            char temp[62];
            sprintf(temp, "ggfw send gltx err is %d -- %d\n", err,ret);

            FILE *f = fopen("/opt/BDFZ/RAV/GGFWGLTX/ggfwgltx.txt", "a+");
            if (f) {
                fputs(temp, f);
                fclose(f);
            }
        }


        sleep(1);
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
