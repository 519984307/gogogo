#include <stdio.h>

#include "storagebridge.h"
#include "appconfig.h"
#include "baseconfig.h"
#include "filterengine.h"
#include "service.h"

extern "C" {
    static FilterEngine *g_engine = NULL;

    int load_defen_module(ServiceBinder *binder, StorageBridge *storage, AppConfig *config) {
        if (NULL == g_engine) {
            g_engine = FilterEngine::NEW(binder, storage, config);
            if (NULL == g_engine) {
                return -1;
            }
        }

        return g_engine->StartNetFilter();
    }

    int unload_defen_module() {
        if (g_engine) {
            g_engine->StopNetFilter();
            delete g_engine;
            g_engine = NULL;
            return 0;
        }
        return -1;
    }

    int refresh_defen_module() {
        if (g_engine) {
            g_engine->RefreshList();
        }
        return -1;
    }
}




