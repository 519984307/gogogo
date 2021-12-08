#include <stdio.h>

#include "centerlistener.h"

class MainEngine : public CenterCommandObserver {
public:
    MainEngine() {
        center = CenterListener::NEW(this, 0, "/opt/BDFZ/RAV/RJJHGJCZ/gjczrjjh.sock", true);
        ggfwgjcz = CenterListener::NEW(
                    this,
                    CenterCommandObserver::CLIENT_COMMAND,
                    "/opt/BDFZ/RAV/GGFWGJCZ/gjczggfw.sock",
                    true);
    }

    ~MainEngine() {
        delete center;
    }

    void start() {
        center->Start();
        ggfwgjcz->Start();
    }

public:
    int OnCommandObserver(int event, void *args, void **result) {

        return 100;
    }

public:
    CenterListener *center, *ggfwgjcz;
};


//int main_func() {
//    FILE *f = fopen("/opt/BDFZ/RAV/RJJHGJCZ/gjczrjjh.txt", "w+");
//    if (f) {
//        fputs("gjcz put it", f);
//        fclose(f);
//    }

//    f = fopen("/opt/BDFZ/RAV/GGFWGJCZ/gjczggfw.txt", "w+");
//    if (f) {
//        fputs("gjcz put it", f);
//        fclose(f);
//    }

//    f = fopen("/opt/BDFZ/RAV/GJCZetc/gjcz.txt", "w+");
//    if (f) {
//        fputs("gjcz put it", f);
//        fclose(f);
//    }

//    MainEngine *engine = new MainEngine();
//    engine->start();

//    while(1) {
//        {
//        CommandBundle bundle;
//        bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
//        bundle.mIntegerValue = 1000;
//        bundle.mStringValue = "gjcj to rjjh";

//        RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
//        int ret = 0;
//        int err =  pack.Externalize("/opt/BDFZ/RAV/RJJHGJCZ/rjjhgjcz.sock", ret);

//        char temp[62];
//        sprintf(temp, "err is %d -- %d\n", err,ret);

//        FILE *f = fopen("/opt/BDFZ/RAV/RJJHGJCZ/gjczrjjh.txt", "a+");
//        if (f) {
//            fputs(temp, f);
//            fclose(f);
//        }
//        }
//        {
//            CommandBundle bundle;
//            bundle.mCommandID = CommandBundle::RELOAD_CONFIG;
//            bundle.mIntegerValue = 1000;
//            bundle.mStringValue = "gjcj to ggfw";

//            RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
//            int ret = 0;
//            int err =  pack.Externalize("/opt/BDFZ/RAV/GGFWGJCZ/ggfwgjcz.sock", ret);

//            char temp[62];
//            sprintf(temp, "gjcz send ggfw err is %d -- %d\n", err,ret);
//            printf(temp);

//            FILE *f = fopen("/opt/BDFZ/RAV/RJJHGJCZ/gjczggfw.txt", "a+");
//            if (f) {
//                fputs(temp, f);
//                fclose(f);
//            }
//        }

//        sleep(5);
//    }
//    return 0;
//}

#include "regapi.h"
#include "Common/updateappengine.h"

int main() {
    UNIXSNPARAM params;
    memset(&params, 0, sizeof(params));
//    int err = ReadSNID("/opt/BDFZ/RAV/plugins/RsConfig.cfg", "/opt/BDFZ/RAV", "H6F1IJ-C97VCK-S2C5RK-EM5308", &params);

    void *handle = NULL;
    int ret = InitCfgConfig(0, "/opt/BDFZ/RAV/plugins/RsConfig.cfg", &handle);
    if (0 == ret) {
        if (0 == CheckCfgSnValid(handle, 0, "asdfasfd", &params)) {
            printf("%d\n", params.nOsVer);
        }

    }

//    UpdateAppEngine *engine = UpdateAppEngine::NEW("/opt/BDFZ/RAV", "RJJHGLTX", 0);
//    UpdateConfig config;
//    config.mCenterAddr = make_pair(OFFICAL_HTTP, "http://rsup10.rising.com.cn/rs2017/");
//    engine->SetUpdateConfig(config);

//    string version, folder;
//    engine->AppUpdate(version, folder);


//    string addr  = engine->GetOfficalAddress("http://rsup10.rising.com.cn/rs2017/", "H6F1IJ-C97VCK-S2C5RK-EM5308");
    return 0;
}
