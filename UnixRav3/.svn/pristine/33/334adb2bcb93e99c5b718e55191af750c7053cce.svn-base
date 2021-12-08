#include "circlelink.h"
#include "handler.h"
#include "commonapi.h"
#include "communicatebundle.h"
#include "baseconfig.h"

#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <ostream>
#include <iostream>

#include <execinfo.h>

#ifndef DEBUG__PRINT_LOG
#define DEBUG__PRINT_LOG
#endif

#ifdef DEBUG__PRINT_LOG
#define LOG_PATH "/opt/BDFZ/RAV/RJJHetc/rjjh8.log"
#endif

using namespace std;

std::string g_install_path;
std::string g_file_path;

enum _running_mode {
    RUNNING_MODE_POWER_ON = 1,
    RUNNING_MODE_DAEMON,
    RUNNING_MODE_UNINSTALL,
    RUNNING_MODE_KILL,
    RUNNING_MODE_DEBUG
}g_running_mode;

int MainFunc(const string& filepath) {
    if (g_install_path.empty() || g_file_path.empty()) {
        return -1;
    }

    string sock = Utils::Path::JoinPath(g_install_path.c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);

    CommandBundle bundle;
    bundle.mCommandID = CommandBundle::CONTEXT_SCAN;
    bundle.mStringValue = filepath;

#ifdef DEBUG__PRINT_LOG
    FILE *fp = fopen(LOG_PATH, "a+");
    if(fp){
        fwrite("filepath:", 1, strlen("filepath:"), fp);
        fwrite(filepath.c_str(), 1, filepath.size(), fp);
        fwrite("\n", 1, strlen("\n"), fp);
    }
    fclose(fp);
#endif

    int ret = 0;
    RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, bundle);
    if (0 == pack.Externalize(sock.c_str(), ret)) {
        return ret;
    }

    return 0;
}

int parse_args(char argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "rdhgukp:")) != -1) {
        switch (opt) {
        case 'p': {
            g_file_path = optarg;
            break;
        }
        default:
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

    g_install_path = RAV_DEFAULT_PATH;
    if (g_install_path.empty()) {
        return -1;
    }

    switch (g_running_mode) {
    case RUNNING_MODE_DAEMON:
    case RUNNING_MODE_POWER_ON: {
        pid_t pid;
        if ((pid=fork()) == 0) {
            return MainFunc(g_file_path);
        }
        break;
    }
    case RUNNING_MODE_DEBUG: {
        return MainFunc(g_file_path);
    }
    default:
        break;
    }

    return 0;
}

