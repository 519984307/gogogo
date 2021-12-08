#ifndef TASKPROXYCHECKER_H
#define TASKPROXYCHECKER_H

#include "scanproxy.h"
#include "controller.h"
#include "multivirusengine.h"
#include "circlelink.h"

#include <vector>


class ScheduleScaner : public ScanProxy {
public:
    static ScheduleScaner* NEW(Controller *controller);
    virtual ~ScheduleScaner();

    int mType;
    void SetScanPath(const std::string& paths);

private:
    ScheduleScaner();
    int Construct(Controller *controller);

private:
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int GetScanPath(std::list<pair<std::string,int> >& path);


    //int GetQuickScanPaths(list<pair<string, int> >& paths);
    int ScanType();
private:
    std::vector<std::string> mScanPaths;
};

class CommandModel;
class RemoteScaner : public ScanProxy {
public:
    static RemoteScaner* NEW(Controller *controller);
    virtual ~RemoteScaner();

    void SetCommandModel(const CommandModel& model);

    CommandModel* GetCommandModel() const {
        return mCommandModel;
    }

private:
    RemoteScaner();
    int Construct(Controller *controller);

protected:
    void Prepare();

private:
    void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data=NULL);
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int  GetScanPath(std::list<pair<std::string,int> >& path);
    int  ScanType();
    int  ScanFinished(int result);

private:
    CommandModel    *mCommandModel;
    // scan type
    int mType;
};


class FolderScaner : public ScanProxy {
public:
    static FolderScaner* NEW(Controller *controller);
    virtual ~FolderScaner();

    void SetFolderPath(const string& path, int depth) {
        mPath = path;
        mScanDepth = depth;
    }

    const string GetScanFolder() const;

    list<VirusReportData>& GetVirusData() {
        return mVirusEntries;
    }

protected:
    FolderScaner();
    int Construct(Controller *controller);

protected:
    void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data=NULL);
    void RefreshVirusLogs(const string& file, const string& virus, int status);
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);

    int GetScanPath(std::list<pair<std::string,int> >& path) {
        path.push_back(make_pair(mPath, mScanDepth));
        return 0;
    }

    int ScanType();
    int LoadScanConfig();
    int ScanFinished(int result);

protected:
    string  mPath;
    int     mScanDepth;
    int     mScanProgress;

    std::list<VirusReportData> mVirusEntries;
};

class DevicePluginChecker : public FolderScaner
{
public:
    static DevicePluginChecker* NEW(Controller *controller);
    virtual ~DevicePluginChecker();

private:
    DevicePluginChecker();
    int Construct(Controller *controller);

private:
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int  ScanType();
    int  LoadScanConfig();
    int  ScanFinished(int result);

public:
    void SetMountPluginPath(const std::string& path, int depth=-1) {
        mPath = path;
        mScanDepth = -1;
    }
};

class MemoryScaner : public FolderScaner {
public:
    static MemoryScaner* NEW(Controller *controller);

private:
    int Construct(Controller *controller);

private:
    int  GetScanPath(std::list<pair<std::string,int> >& path);
    void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data=NULL) {}
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int  ScanType();
    int  LoadScanConfig();
    int  ScanFinished(int result);
};

class DefenScaner : public ScanProxy {
public:
    static DefenScaner* NEW(Controller *controller);
    virtual ~DefenScaner();

    void SetFolderPath(const string& path, int depth) {
        mPath = path;
        mScanDepth = depth;
    }

    const string GetScanFolder() const;

    list<VirusReportData>& GetVirusData() {
        return mVirusEntries;
    }

    const bool GetFinishNotifyState() const {
        return mNotify;
    }

private:
    DefenScaner();
    int Construct(Controller *controller);

private:
    void RefreshVirusLogs(const string& file, const string& virus, int status);
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int GetScanPath(std::list<pair<std::string,int> >& path) {
        path.push_back(make_pair(mPath, mScanDepth));
        return 0;
    }

    int ScanType();
    int LoadScanConfig();
    int ScanFinished(int result);

private:
    string  mPath;
    int     mScanDepth;

    bool    mNotify;

    std::list<VirusReportData> mVirusEntries;
};

class UserScaner : public ScanProxy {
public:
    static UserScaner* NEW(Controller *controller);
    virtual ~UserScaner();

    list<VirusReportData>& GetVirusData() {
        return mVirusEntries;
    }

    void Prepare();

    void SetScanParams(int type, int extend, const string& params);
    void SetScanParams(int type, int params);

    int StartScan(std::list<pair<std::string,int> >& path, bool feedback);
private:
    UserScaner();
    int Construct(Controller *controller);

private:
    void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE scanstatus, int progress, void *data=NULL);
    void RefreshVirusLogs(const string& file, const string& virus, int status);
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int GetScanPath(std::list<pair<std::string,int> >& path);

    int ScanType();
    int ScanFinished(int result);

private:
    int     mScanProgress;
    int     mScanFileCount;

    int     mScanType;
    string  mScanPaths;

    uid_t   mUserProcPid;

    std::list<VirusReportData> mVirusEntries;
};

#endif // TASKPROXYCHECKER_H
