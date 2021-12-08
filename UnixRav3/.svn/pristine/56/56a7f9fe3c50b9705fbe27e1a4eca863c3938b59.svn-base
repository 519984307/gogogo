#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <string>
#include <list>

#include "Entry/commonentry.h"

using namespace std;

class AntiVirusSetting;
class DefenDataContainer {
public:
    AntiVirusSetting    *mFileMonitorSetting;

    list<string> mProcMonitorList;
    list<string> mFolderMonitorList;
    list<string> mFileTypeMonitorList;

    DefenDataContainer() {
        mFileMonitorSetting = new AntiVirusSetting;
    }
    ~DefenDataContainer() {
        if (mFileMonitorSetting) {
            delete mFileMonitorSetting;
            mFileMonitorSetting = NULL;
        }
    }
};

#endif // DATACONTAINER_H
