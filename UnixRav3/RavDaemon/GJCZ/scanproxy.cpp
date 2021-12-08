#include "scanproxy.h"
#include "scanconfig.h"
#include "controller.h"
#include "mainengine.h"

ScanProxy::ScanProxy(APP_TYPE type) : Application(type) {
    mScanConfig = NULL;
    mController = NULL;
    mVirusAutoType = SCAN_NOT_SET;
    mCurrentUID = -1;
}

ScanProxy::~ScanProxy() {
    if (mScanConfig) {
        delete mScanConfig;
        mScanConfig = NULL;
    }
}

void ScanProxy::SetCurrentUser(uid_t uid) {
    mCurrentUID = uid;
}

int ScanProxy::LoadScanConfig() {
    mScanConfig = new ScanConfig;
    int err = mController->GetScanConfig(*mScanConfig);

    MainEngine *engine = (MainEngine*)mController;
    // 暂时把保密机不能多线程扫描的去掉，观察下结果，如果还是会卡住再说
    if (engine->GetAppConfig()->SecretMachine()) {
        // 如果是保密机，扫描的时候强行打开智能扫描，不扫描系统的关键路径，只扫描用户可操作路径
        // 扫描的线程强得置为1， 不进行多线程扫描
        mScanConfig->mScanIntelligent = true;
        mScanConfig->mThreadCount = 1;
    }
    return err;
}

int ScanProxy::StartScan(std::list<pair<std::string,int> >& path) {
    LOG_GJCZ("start the ScanProxy StartScan");
    if (mController) {
        if (0 == LoadScanConfig()) {
            LOG_GJCZ_DEBUG("load scan config ok");
            return mController->Scan(path, mScanConfig);
        }
    }
    return -1;
}

void ScanProxy::StopScan() {
    if (mController) {
        mController->StopScan();
    }
}

int ScanProxy::OnCommandObserver(int event, void *args, void **result) {
    return 0;
}

void ScanProxy::SetController(Controller *controller) {
    mController = controller;
}

std::string ScanProxy::GetRemoteScanPath() {
    StorageBridge *storage = GetStorageBridget(Application::GJCZ);
    if (storage) {
        return storage->GetRemoteScanPath();
    }
    return "";
}

int ScanProxy::GetFullScanPaths(list<pair<string, int> >& path) {
    path.push_back(make_pair("/usr", -1));
    path.push_back(make_pair("/opt", -1));
    path.push_back(make_pair("/tmp", -1));
    path.push_back(make_pair("/home", -1));
    path.push_back(make_pair("/var", -1));
    path.push_back(make_pair("/etc", -1));
    path.push_back(make_pair("/mnt", -1));
    path.push_back(make_pair("/lib", -1));
    path.push_back(make_pair("/root", -1));
    path.push_back(make_pair("/sbin", -1));
    path.push_back(make_pair("/media", -1));
    path.push_back(make_pair("/run", -1));

    return path.size();
}

int ScanProxy::GetQuickScanPaths(list<pair<string, int> >& paths) {
    paths.push_back(make_pair("/etc", 5));
    //paths.push_back(make_pair("/usr", 5));
    //paths.push_back(make_pair("/opt", 5));
    paths.push_back(make_pair("/root", -1));
    paths.push_back(make_pair("/home", -1));
    //paths.push_back(make_pair("/media", -1));
    //paths.push_back(make_pair("/run", -1));

    return paths.size();
}
