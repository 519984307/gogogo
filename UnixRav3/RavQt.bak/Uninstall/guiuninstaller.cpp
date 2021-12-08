#include "guiuninstaller.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "appconfig.h"

#include <string>

using namespace std;

GuiUninstaller::GuiUninstaller()
{

}

GuiUninstaller::~GuiUninstaller()
{

}

int GuiUninstaller::GetInstallInfo()
{
    mInstallPath = "/opt/BDFZ/RAV";
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, CONFIG_DEF_POLICY, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(config);
    if(cfg == NULL) {
        return -1;
    }
    mInstallPath = cfg->GetInstallPath();
    //cfg->PrintConfig();
    delete cfg;
    return 0;
}

int GuiUninstaller::QuietUninstall()
{
    UninstallReport();
    StopAll();
    //don't reserve config file
    rm_dir(mInstallPath);
    if (DelSystemdScript(SYSTEMD_SERVICE_0) != 0) {
        return -1;
    }
    if (DelSystemdScript(SYSTEMD_SERVICE_1) != 0) {
        return -1;
    }
    if (DelSystemdScript(SYSTEMD_SERVICE_2) != 0) {
        return -1;
    }
    if (DelAutoStart(AUTOSTART_FILE_NAME) != 0){
        return -1;
    }
    if (DelApplicationIcon(APPLICATION_FILE_NAME) != 0) {
        return -1;
    }
    string conf = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (Utils::Path::FileAvailed(conf)) {
        unlink(conf.c_str());
    }
    return 0;
}
