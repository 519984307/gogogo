#include <stdio.h>
#include <unistd.h>

#include "defenmodproxy.h"

class MainEngine : public DefenObserver {
public:
    MainEngine() {
        center = new ProcDefenModProxy;
        center->SetObserver(this);
    }

    ~MainEngine() {
        delete center;
    }

    void start() {
        center->Start();
    }

public:
    int onDefenEvent(DEFEN_TYPE event, const char *file) {
        return 0;
    }

public:
    ProcDefenModProxy *center;
};


int main_func() {
    while(1) {
        MainEngine *engine = new MainEngine;
        engine->start();

        sleep(10);

        delete engine;
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
