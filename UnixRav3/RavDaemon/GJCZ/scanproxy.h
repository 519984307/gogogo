#ifndef SCANAPP_H
#define SCANAPP_H

#include "application.h"

class Controller;
class ScanConfig;

/**
 * @brief The ScanProxy class
 * the class from Application
 * this will link the unix_engine2013
 */
class ScanProxy : public Application {
public:
    ScanProxy(APP_TYPE type);
    ~ScanProxy();

    int StartScan(std::list<pair<std::string,int> >& path);
    void StopScan();

    ScanConfig* GetCurrentScanConfig() const {
        return mScanConfig;
    }

    void SetController(Controller *controller);
    void SetCurrentUser(uid_t uid);

    std::string GetRemoteScanPath();

    virtual int GetFullScanPaths(list<pair<string, int> >& paths);
    virtual int GetQuickScanPaths(list<pair<string, int> >& paths);

    virtual void Prepare() {}

public:
    virtual int LoadScanConfig();

protected:
    int OnCommandObserver(int event, void *args, void **result);

protected:
    Controller  *mController;
    ScanConfig  *mScanConfig;

    uid_t mCurrentUID;

protected:
    enum {
        SCAN_NOT_SET,
        SCAN_AUTO_KILL,
        SCAN_AUTO_IGNORE
    }mVirusAutoType;
};

#endif // SCANAPP_H
