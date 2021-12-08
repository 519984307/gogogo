#include "kernelapplication.h"
#include "appconfig.h"
#include "storage_global.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "storagebridge.h"

#include <unistd.h>

KernelApplication::KernelApplication(ServiceBinder *binder, StorageBridge *storage, AppConfig *config)
                                     : mStorage(storage), mAppCfg(config), mBinder(binder) {
    mStopModuleRemoved = false;
}


int KernelApplication::InstallModule(const string& modulefile) {
    const char *INSMOD[] = {"/sbin/insmod", "/bin/insmod", "/usr/bin/insmod", "/usr/sbin/insmod"};
    for (int i=0; i<sizeof(INSMOD)/sizeof(char*); ++i) {
	if (Utils::Path::FileAvailed(INSMOD[i])) {
            string command = string(INSMOD[i]) + " " + modulefile + " &";
            LOG_ZDFY(command.c_str());
            return system(command.c_str());
        }
    }
    return -1;
}

int KernelApplication::UninstallModule(const string& modulename) {
    string shell = Utils::Path::JoinPath(mAppCfg->GetInstallPath().c_str(), DEPLOYED_BIN_FOLDER, DEPLOYED_MODULE_SHELL, NULL);
    if (Utils::Path::FileAvailed(shell)) {
        string command = shell + " -r " + modulename;
        return system(command.c_str());
    }
    return -1;
}

int KernelApplication::StartMonitor(const string& module) {
    pair<string,string> versions;
    Utils::OS::GetKernelVersions(versions);

    string kofile = module + "_" + Utils::OS::GetMachineArch() + "_" + versions.first + "_" + Utils::Hash::GetMD5(versions.second) + ".ko";
    string ko = Utils::Path::JoinPath(mAppCfg->GetInstallPath().c_str(), PLUGINS_PATH, kofile.c_str(), NULL);

    LOG_ZDFY("ko file is %s", ko.c_str());
    printf("ko file is %s\n", ko.c_str());

    if (!Utils::Path::FileAvailed(ko)) {
        LOG_ZDFY("not found the ko");
        return 0;
    }

    int retry = 0;
    while (Utils::OS::CheckModuleState(module) < 0) {
        InstallModule(ko);
        sleep(1);
        if (retry++ > 3) {
            return -1;
        }
    }
    return 0;
}

int KernelApplication::StopMonitor(const string& module) {
    int retry = 0;
    do {
        if (mStopModuleRemoved) {
            int state = Utils::OS::CheckModuleState(module);
            if(state == 0) {
                UninstallModule(module);
            }
            else if (state > 0) {
                sleep(1);
            }
            else {
                break;
            }
        }
    }while(retry++ < 3 && mStopModuleRemoved);
    return 0;
}

int KernelApplication::Construct() {
    mStopModuleRemoved = mStorage->GetModuleStopToRemoveState();
    return 0;
}
