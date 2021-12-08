#ifndef CLOUDVIRUSENGINE_H
#define CLOUDVIRUSENGINE_H

#include <string>
#include <vector>

#include "scanconfig.h"

using namespace std;

class EngineObserver;
class CloudNetProxy;
class ScanEntry;
class CloudEngine
{
public:
    static CloudEngine* NEW(EngineObserver *observer);
    ~CloudEngine();

    void SetCloudCenterAddr(const string& ip) {
        mCenterAddr = ip;
    }

    void SetNetProxy(CloudNetProxy *proxy) {
        mProxy = proxy;
    }

    void SetVersion(const string& version) {
        mClientVersion = version;
    }
    void SetGUID(const string& guid) {
        mClientGUID = guid;
    }

    int CloudCheck(vector<ScanEntry*> *data);

    void Start(vector<CloudConfig>& config);
    void Stop();

private:
    CloudEngine(EngineObserver *proxy);
    int Construct();

    void StartCheck();

private:
    class Navigate {
    public:
        string  search;
        int     defaulton;
        int     cachelife;
    };

    int ParseNavigate(const string& data, Navigate& navi);

private:
    pthread_t   mCheckThread;
    static void* CheckThread(void* params);
    void CheckThreadFunc();

    int ConfirmResult(const string& result, vector<ScanEntry*> *data);

private:
    bool mStatus;
    CloudNetProxy   *mProxy;
    EngineObserver  *mObserver;

    string mCenterAddr;
    string mCurrentCloudAddr;

    string mClientVersion;
    string mClientGUID;

    vector<CloudConfig> mCloudConfig;
    Navigate    mNavigate;
};

#endif // CLOUDVIRUSENGINE_H
