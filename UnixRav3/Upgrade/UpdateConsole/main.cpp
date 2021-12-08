#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "baseconfig.h"
#include "finishapp.h"
#include "commonlocker.h"
#include "configfile.h"
#include "commonapi.h"
#include "appconfig.h"

using namespace std;

//
bool g_download_flag    = false;
bool g_replace_flag     = false;
string g_version;
long   g_uid;

CondLocker *gCondLocker = NULL;

class ESMUpdate : public FinishApplication
{
public:
    ESMUpdate(AppConfig *config, const string& version, uid_t uid) : FinishApplication(config, "", version, uid) {}
    ~ESMUpdate() {}

private:
    void OnUpdateSuccess() {
        gCondLocker->Signal();
    }
    void OnUpdateFailed() {
        gCondLocker->Signal();
    }
};


void exit_signal(int signal) {

}

int parse_args(char argc, char *argv[]) {
    if (argc < 3) {
        cout << "command need params!\n" << endl;
        return -1;
    }

    int opt = 0;
    while ((opt = getopt(argc, argv, "drv:i:")) != -1) {
        switch (opt) {
        case 'd':
            g_download_flag = true;
            break;
        case 'r':
            g_replace_flag = true;
            break;
        case 'v':
            g_version = optarg;
            break;
        case 'i':
            g_uid = atoi((char*)optarg);
            break;
        case 'h': {
            cout << "Format: esmupdate.bin -[d|r] -v version" << endl;
            cout << "d: download" << endl;
            cout << "r: replace the file" << endl;
            return -1;
        }

        default:
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    signal(SIGUSR2, exit_signal);

    int err = parse_args(argc, argv);
    if (err) {
        return err;
    }

    gCondLocker = CondLocker::NEW();
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(config);
    ESMUpdate *update = new ESMUpdate(cfg, g_version, g_uid);
    if (!update)  {
        return -1;
    }

    update->Start();
    gCondLocker->Wait();

    delete update;
    delete gCondLocker;
    delete cfg;

    return 0;
}
