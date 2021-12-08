#ifndef SCANNOTIFIYCALLBACK_H
#define SCANNOTIFIYCALLBACK_H

#include "engineobserver.h"
#include <string>

using namespace std;

class MainEngine;
class ScanNotifyCallback : public EngineObserver
{
public:
    ScanNotifyCallback(MainEngine *major);
    bool GetVirusState() const {
        return mVirused;
    }

protected:
    int onEvent(EngineEvent event, EngineType type, void *data);

private:
    bool mVirused;
    MainEngine *mMainEngine;
};

#endif // SCANNOTIFIYCALLBACK_H
