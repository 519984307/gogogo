#include "mainengine.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "appconfig.h"

#include <signal.h>

#ifdef MEM_CHECK
    #include <stdlib.h>
    #include <mcheck.h>
#endif

static bool g_debug_mode = false;
static bool g_running_state = false;
/** 是否静默方式启动，RJJH是否启动显示托盘 */
static bool g_silence_state = false;

#ifdef WITH_QT
    #include <QApplication>
#endif

int print_config() {
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(config);
    ASSERT_ZERO(cfg, -1);

    cfg->PrintConfig();

    delete cfg;
    return 0;
}

int change_config(const char *key, const char *value) {
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(config);
    ASSERT_ZERO(cfg, -1);

    cfg->ChangeConfigSection(atoi(key), value);
    delete cfg;
    return 0;
}

int delete_config(int key) {
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(config);
    ASSERT_ZERO(cfg, -1);

    cfg->DeleteSection(key);
    delete cfg;
    return 0;
}

int parse_args(int argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "gslc:n:d:")) != -1) {
        switch (opt) {
        case 'l':
            print_config();
            return -1;
        case 's':
            g_silence_state = true;
            break;
        case 'c': {
            const char *params = optarg;
            const char *data = strchr(params, '=');
            if (data) {
                char temp[12] = {0};
                memcpy(temp, params, data-params);

                change_config(temp, data+1);
            }
            return -1;
        }
        case 'n': {
            const char *filename = optarg;
            AppConfig::GeneralConfig(filename);
            return -1;
        }
        case 'd': {
            const char *key = optarg;
            int kv = atoi(key);
            delete_config(kv);
            return -1;
        };
        case 'g':
            g_debug_mode = true;
            g_running_state = true;
            break;
        default:
            g_debug_mode = false;
            g_running_state = true;
            break;
        }
    }
    return 0;
}

void exit_signal(int signal) {
    LOG_RJJH("get exit signal %d", signal);

    g_running_state = false;
    exit(0);
}

int main_func(int argc, char *argv[]) {
    signal(SIGUSR2, exit_signal);
    signal(SIGHUP,  exit_signal);
    signal(SIGQUIT, exit_signal);
    signal(SIGPIPE, SIG_IGN);

    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        printf("block sigpipe error\n");
    }

#ifdef WITH_QT
    QApplication application(argc, argv);
#endif

    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(config);
    ASSERT_ZERO(cfg, -1);

    Utils::LOG::InitLog(cfg->GetInstallPath().c_str());

    LOG_RJJH("start the RJJH0 %d", (int)getuid());

    if (getuid() && Utils::OS::GetProcState(Utils::Path::JoinPath(cfg->GetInstallPath().c_str(), FOLDER_RJJH_ETC,NULL),
                                cfg->GetSubProductGUID() != SECRET_MACHINE_ID)) {
        LOG_RJJH("has lock the file (%d)", getuid());
        return -1;
    }


    // the process started, then locked
    FILE *f = Utils::OS::LockProcState(
                Utils::Path::JoinPath(cfg->GetInstallPath().c_str(), FOLDER_RJJH_ETC, NULL).c_str());
    LOG_RJJH("Locked the %s/RJJH0_%d.lock",
             Utils::Path::JoinPath(cfg->GetInstallPath().c_str(), FOLDER_RJJH_ETC, NULL).c_str(), (int)getuid());

    // 检查是否RJJH0启动, 如果已经有, 检查其是否为开机的初始启动,如果是,
    // 刚向GLTX发送关闭消息
    int state = Utils::OS::CheckProcessState(
                        Utils::Path::JoinPath(cfg->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL).c_str()
                    );
    if (state > 1) {
        // check the rjjh0's uid
        vector<pair<int,string> > pids;
        vector<string> params;
        if (0 == Utils::OS::GetPid(
                    Utils::Path::JoinPath(cfg->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, NOTIFYY_ENTRY, NULL).c_str(),
                    pids)) {
            vector<pair<int, string> >::iterator iter;
            for (iter=pids.begin();iter!=pids.end(); ++iter) {
                uid_t notify_uid = Utils::OS::GetProcessUID(iter->first);
                if (notify_uid == 0 && iter->second.find("-s") != string::npos) {
                    // 0 mean root
                    // notify the ep module to reset the tray process
                    std::string sock = Utils::Path::JoinPath(cfg->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
                    CommandBundle command;
                    command.mCommandID = CommandBundle::REPAIR;
                    command.mIntegerValue = CommandBundle::SHUTDOWN_DAEMON_TRAY;
                    command.mStringValue = Utils::String::itoa(iter->first);

                    int ret = 0;
                    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
                    if (0 == bundle.Externalize(sock.c_str(), ret)) {

                    }
                }
            }
        }
    }

    LOG_RJJH("new the main engine");
    MainEngine *notify = MainEngine::NEW(cfg, g_silence_state);
    if (notify) {
        notify->Start();
    }


    g_running_state = true;

#ifdef WITH_QT
    application.exec();
    notify->Stop();
#else
    while (g_running_state) {
        sleep(1);
    }
#endif

    LOG_RJJH("Exit the rjjh state");

    Utils::OS::ReleaseProcState(f);
    delete notify;

    return 0;
}

int main(int argc, char *argv[]) {

    if (parse_args(argc, argv) < 0) {
        return 0;
    }

    if (g_debug_mode)  {
        return main_func(argc, argv);
    }
    else {
        int pid;
        if ((pid=fork()) == 0) {
            return main_func(argc, argv);
        }
    }
    return 0;
}
