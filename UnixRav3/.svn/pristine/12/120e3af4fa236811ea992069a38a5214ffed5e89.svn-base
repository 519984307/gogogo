#ifndef FILTERENGINE_H
#define FILTERENGINE_H

#include "kernelapplication.h"
#include "NFLCommunicator.h"

#include <string>

class StorageBridge;
class ServiceBinder;
class AppConfig;

class FilterEngine : public KernelApplication, public NFLObserver
{
public:
    static FilterEngine* NEW(ServiceBinder *binder, StorageBridge *storage, AppConfig *config);
    virtual ~FilterEngine();

    int StartNetFilter();
    int StopNetFilter();

    int RefreshList();

private:
    FilterEngine(ServiceBinder *binder, StorageBridge *storage, AppConfig *config);
    int Construct();

private:
    // from NFLObserver
    int OnNFLCommunicateEvent(enum FilterEvent event, enum FilterType type, const std::string& data);

private:
    CNFLCommunicator    *mNetfilterProxy;

    bool mStatus;
};

#endif // FILTERENGINE_H
