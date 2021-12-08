#include <iostream>

#include "cross.h"

int main() {
    CrossVersion version;
    if (version.Exist()) {
        string type, url;
        version.GetCenterAddr(type, url);
        version.ExportLogs("/tmp/out.xml");
        
        version.UninstallOldVersion();
    }

    return 0;
}
