#include "mainengine.h"
#include "storage.h"
#include "cinifile.h"

#include "baseconfig.h"

#include "circlelink.h"
#include "handler.h"
#include "commonapi.h"
#include "communicatebundle.h"

#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

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
    g_running_state = 0;
}

int MainFunc() {
    signal(SIGUSR2, exit_signal);
    signal(SIGPIPE, SIG_IGN);

    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        printf("block sigpipe error\n");
    }

#ifdef MEM_CHECK
    setenv("MALLOC_TRACE", "zdfy.log", 1);
    mtrace();
#endif

    engine = MainEngine::NEW();
    if (NULL == engine) {
        return -1;
    }

    g_running_state = 1;
    while (g_running_state) {
        sleep(1);
    }

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
    g_running_mode = RUNNING_MODE_DAEMON;
    int err = parse_args(argc, argv);
    if (err) {
        return err;
    }
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

