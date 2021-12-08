#include <stdio.h>
#include "storagebridge.h"
#include "appconfig.h"
#include "defenengine.h"
#include "baseconfig.h"

#include "service.h"

extern "C" {
    static DefenEngine *g_engine = NULL;

    int load_defen_module(ServiceBinder *binder, StorageBridge *storage, AppConfig *config) {
        if (NULL == g_engine) {
            g_engine = DefenEngine::NEW((ServiceBinder*)binder, (StorageBridge*)storage, (AppConfig*)config);
            if (NULL == g_engine) {
                return -1;
            }
        }

        return g_engine->StartFileMonitor();
    }

    int unload_defen_module() {
        if (g_engine) {
            g_engine->StopFileMonitor();
            delete g_engine;
            g_engine = NULL;
            return 0;
        }
        return -1;
    }

    int refresh_defen_module() {
        if (g_engine) {
            g_engine->LoadFileMonitorSetting();
        }
        return -1;
    }
}

