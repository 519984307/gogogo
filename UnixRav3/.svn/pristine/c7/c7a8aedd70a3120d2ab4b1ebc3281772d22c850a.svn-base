#include <stdio.h>

#include "mainengine.h"

int main() {
    MainEngine *engine = MainEngine::NEW();
    if (engine) {
        engine->Start();
        while(1) {
            sleep(10);
        }
    }
    printf("");

//    int err = engine->update_svn("/home/miwoo/workspace/rising/esm_dev/UnixRav2017", "48246");
//    int err = engine->compile("/home/miwoo/workspace/rising/esm_dev/build-UnixRav2017-unknown-Debug");
//    printf("err is %d\n", err);
    return 0;
}
