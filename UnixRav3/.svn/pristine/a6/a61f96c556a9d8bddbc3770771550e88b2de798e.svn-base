#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setup.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "appconfig.h"
#include "communicator_global.h"

using namespace std;
using namespace Setup;

class RavConsoleUnsetup : public SetupApplication {
protected:
    int ShowWelcomeScreen() {
        fprintf(stdout,"************************************************************************\n");
        fprintf(stdout,"*                                                                      *\n");
        fprintf(stdout,"*                                                                      *\n");
        fprintf(stdout,"*                 RISING ANTIVIRUS SETUP SOFTWARE                      *\n");
        fprintf(stdout,"*       COPYRIGHT FOR BEIJING RISING TECHNOLOGY CO.,LTD. 2015-2018     *\n");
        fprintf(stdout,"*                                                                      *\n");
        fprintf(stdout,"************************************************************************\n");

        return 1;
    }

    int Prepare() {
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
        mCenterType = (SoftType)cfg->GetProductType();
        mCenterAddr = cfg->GetCenterURL();

        delete cfg;
        return 0;
    }
    int AgreeLisence() {
        return 0;
    }
    int StartApplicationPrompt() {
        return 0;
    }
    int ReInstallPromtp() {
        return 0;
    }

    int GetInstallPathPrompt(std::string& installPath) {
        installPath = mInstallPath;
        return 0;
    }

    int GetInstallPath(std::string& installPath) {
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
        installPath = cfg->GetInstallPath();
        //cfg->PrintConfig();
        delete cfg;
        return 0;
    }

    SoftType GetSoftwareType() {
        return mCenterType;
    }

    int GetSerialNumber(std::string& /*serial*/) {
        return 0;
    }

    int GetCenterAddr(std::string& center) {
        center = mCenterAddr;
        return 0;
    }

    int GetInstallComponents(int &) {
        return 0;
    }
    
    int GetInstallFunctionsCodePrompt(int &/*installFunctions*/) {
        return 0;
    }

    int UninstallPrompt() { 
        string config = Utils::Path::JoinPath(mInstallPath.c_str(), PLUGINS_PATH, "autoinstall", NULL);
        if (Utils::Path::FileAvailed(config)) {
            return 0;
        }

        fprintf(stdout, "Are you realy want to uninstall this software?(yes/no)>");
        char stmp[10];
        if (NULL == fgets(stmp, 10, stdin)) {
            return -1;
        }
        if (stmp[strlen(stmp)-1] == '\n') {
            stmp[strlen(stmp)-1] = 0;
        }
        if (0 == strcasecmp(stmp, "Y") || 0 == strcasecmp(stmp, "yes")){
            return 0;
        }
        return -1;
    }

    int ReserveConfig() { 
        fprintf(stdout, "Do you want to reserve the config file?(yes/no)>");
        char stmp[10];
        if (NULL == fgets(stmp, 10, stdin)) {
            return -1;
        }
        if (stmp[strlen(stmp)-1] == '\n') {
            stmp[strlen(stmp)-1] = 0;
        }
        if (strcasecmp(stmp, "Y") == 0 || strcasecmp(stmp, "yes") == 0){
            return 0;
        }
        return -1;
    }

    int OnError(SetupStep step, int error, void * /*data*/) {
        switch (step) {
        case SETUP_UNINSTALL_CHECKING:{
            fprintf(stdout, "Checking uninstall permission...\n");
            break;
        }
        case SETUP_UNINSTALL_REPORT: {
            fprintf(stdout, "Report the uninstall to center\n");
            break;
        }
        case SETUP_UNINSTALL_REMOVE_CONFIG: {
            fprintf(stdout, "Removing file...\n");
            break;
        }
        case SETUP_UNINSTALL_FINISHED: {
            fprintf(stdout, "Uninstall finished\n");
            break;
        }
        case STEP_STOP_RUNNING: {
            fprintf(stdout, "Stop the running process\n");
            break;
        }
//        case NO_ERROR:
//            fprintf(stdout, "uninstall finished, thank you for use it\n");
//            break;
//        case VERSION_TOO_LOW:
//            fprintf(stdout, "The version is lower than installed\n");
//            break;
//        case PRODUCT_NOT_ASSISTANT:
//            fprintf(stdout, "Product not assistant\n");
//            break;
        default:
            break;
        }

        if (error) {
            printf("Error\n");
        }
        return 0;
    }
};

int main(int /*argc*/, char ** /*argv*/) {
    RavConsoleUnsetup setup;
    return setup.StartUninstall();
}
