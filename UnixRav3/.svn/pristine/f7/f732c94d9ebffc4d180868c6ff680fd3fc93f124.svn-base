#ifndef PLUGINENTRY_H
#define PLUGINENTRY_H

#include "rserializer.h"

#include <list>

class PublicScanData : public RSerializer {
public:
    std::list<std::string> files;
    bool memory;

    bool Internalize(const unsigned char *data, int len);
    bool Internalize(int fd);
    int  CalculateSize();

private:
    std::string JSON();
};

class PublicScanResult : public RSerializer {
public:
    virtual ~PublicScanResult() {
        results.clear();
    }

    struct ScanResult {
        std::string file;
        std::string hash;
        std::string virus;
        std::string type;
        bool        hits;
    };

    std::list<struct ScanResult> results;

    bool Externalize(int fd);
    int  CalculateSize();

private:
    std::string JSON();
};


class DevicePluginInstallData : public RSerializer {
public:
    std::string mPluginPath;

    bool Internalize(const unsigned char *data, int len) {
        ((char*)data)[len] = '\0';
        mPluginPath = (char*)data;
        return true;
    }

    int  CalculateSize() {
        return 0;
    }
};

#endif // PLUGINENTRY_H
