#ifndef KERNELAPPLICATION_H
#define KERNELAPPLICATION_H

#include <string>

using namespace std;

class AppConfig;
class StorageBridge;
class ServiceBinder;

class KernelApplication
{
public:
    KernelApplication(ServiceBinder *binder, StorageBridge *storage, AppConfig *config);

protected:
    int InstallModule(const string& modulefile);
    int UninstallModule(const string& modulefile);

    int StartMonitor(const string& name);
    int StopMonitor(const string& module);

    virtual int Construct();

protected:
    AppConfig     *mAppCfg;
    StorageBridge *mStorage;
    ServiceBinder *mBinder;

    /** 标志位，判断当内核模块卸载时是否RMMOD内核模块 */
    bool mStopModuleRemoved;
};

#endif // KERNELAPPLICATION_H
