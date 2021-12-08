#include "mainengine.h"
#include "storage.h"
#include "cinifile.h"

#include "controller.h"
#include "baseconfig.h"
#include "config.ver"

#include "../CommonAPI/circlelink.h"
#include "../CommonAPI/handler.h"
#include "../CommonAPI/commonapi.h"
#include "../CommonAPI/communicatebundle.h"

#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/file.h>


#ifdef COMM_WITH_DBUS
    #include <QCoreApplication>
#endif

#ifdef MEM_CHECK
    #include <stdlib.h>
    #include <mcheck.h>
#endif


int g_running_state = 0;
MainEngine *engine;

enum _running_mode {
    RUNNING_MODE_POWER_ON = 1,
    RUNNING_MODE_DAEMON,
    RUNNING_MODE_UNINSTALL,
    RUNNING_MODE_KILL,
    RUNNING_MODE_DEBUG
};

int g_running_mode = 0;

void exit_signal(int signal) {
    if (signal == SIGUSR2) {
        g_running_state = 0;
    }
}

int MainFunc() {
    signal(SIGUSR2, exit_signal);
    signal(SIGPIPE, exit_signal);
    signal(SIGABRT, exit_signal);

    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        printf("block sigpipe error\n");
    }

    // 修改系统句柄数占用上限,QT进程，句柄占有用较大，有UOS上会导致出错
    system("ulimit -HSn 4096");

#ifdef COMM_WITH_DBUS
    char *params[] = {"rav"};
    int argc = 1;
    QCoreApplication app(argc, params);
#endif

    engine = MainEngine::NEW();
    if (NULL == engine) {
        return -1;
    }

#if defined(COMM_WITH_SOCKET)
    g_running_state = 1;
    while (g_running_state) {
        sleep(1);
    }
#elif defined(COMM_WITH_DBUS)
    app.exec();
#endif

    engine->Stop();
    delete engine;

    return 0;
}

int parse_args(char argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "rdhguk")) != -1) {
        switch (opt) {
        case 'd':
            g_running_mode = RUNNING_MODE_DAEMON;
            break;
        case 'r':
            g_running_mode = RUNNING_MODE_POWER_ON;
            break;
        case 'g':
            g_running_mode = RUNNING_MODE_DEBUG;
            break;
        case 'u':
            g_running_mode = RUNNING_MODE_UNINSTALL;
            break;
        case 'k':
            g_running_mode = RUNNING_MODE_KILL;
            break;
        case 'h': {
            cout << "Format: esmagent.bin -[d|r|h|g] -v version" << endl;
            cout << "d: start as a daemon" << endl;
            cout << "r: restart use it" << endl;
            cout << "g: debug it, no fork" << endl;
            cout << "u: uninstall the all" << endl;
            cout << "k: kill all proc" << endl;
            cout << "h: help info" << endl;
            return -1;
        }

        default:
            g_running_mode = RUNNING_MODE_DAEMON;
            break;
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    signal(SIGUSR2, exit_signal);
    signal(SIGPIPE, SIG_IGN);

    g_running_mode = RUNNING_MODE_DAEMON;
    int err = parse_args(argc, argv);
    if (err) {
        return err;
    }

#ifdef MEM_CHECK
    setenv("MALLOC_TRACE", "gjcz.log", 1);
    mtrace();
#endif

    switch (g_running_mode) {
    case RUNNING_MODE_DAEMON:
    case RUNNING_MODE_POWER_ON: {
        pid_t pid;
        if ((pid=fork()) == 0) {
            return MainFunc();
        }
        break;
    }
    case RUNNING_MODE_DEBUG: {
        return MainFunc();
    }
    default:
        break;
    }

    return 0;
}

