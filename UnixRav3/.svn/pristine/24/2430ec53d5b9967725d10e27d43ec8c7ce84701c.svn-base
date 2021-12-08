#ifndef UPDATEVIRUSENGINE_H
#define UPDATEVIRUSENGINE_H

#include "upgrade.h"

class UpdateOfflineCallback : public EngineObserver {
public:
    UpdateOfflineCallback(const std::string& sourcefolder, const std::string& destfolder) {
        mSourceFolder = sourcefolder;
        mDestFolder = destfolder;
    }

protected:
    // from EngineObserver
    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

private:
    std::string mSourceFolder;
    std::string mDestFolder;
};

class UpdateCenterCallback: public EngineObserver {
public:
    UpdateCenterCallback(UpdateObserver *observer, const std::string& url, const std::string& folder )
                    : mObserver(observer) {
        mUpdateVirusBaseUrl = url;
        mUpdateVirusFolder = folder;
    }
protected:
    // from EngineObserver
    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

private:
    UpdateObserver *mObserver;
    std::string     mUpdateVirusBaseUrl;
    std::string     mUpdateVirusFolder;
};

class UpdateVirusEngine : public UpdateEngine, public EngineObserver
{
public:
    static UpdateVirusEngine* NEW(const std::string &installdir, const std::string& configpath, int center_type);

protected:
    // from UpdateEngine
    int UpdateEntry();
    std::string GetDefaultAddress() {
        if (1 == mCenterType) {
            // ESM
            return "/ruc/esm/esvirlib";
        }
        else if (2 == mCenterType) {
            // SOC
            return "/ruc/rec/src/rd3/esvirlib";
        }
    }

    // from EngineObserver
    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

protected:
    UpdateVirusEngine(int center_type) {
        mNotifierUID = 0;
        mCenterType = center_type;
#ifdef UPDATE_WITH_ENGINE_
        mUpdateSource = OTHER_HTTP;
#endif
    }

protected:
    virtual int GetNavigate(UPDATE_SOURCE type, const std::string& url, const std::string& version, std::string& navigate);
    virtual int PrepareNavigate(UPDATE_SOURCE type, const std::string& url, const std::string& version, std::string& navigate);
    virtual int UpdateByEngine(UPDATE_SOURCE type, const std::string& updateurl, const std::string& navigate, std::string& newver);

protected:
#ifdef UPDATE_WITH_ENGINE_
    std::string mUpdateVirusBaseUrl;
    std::string mUpdateVirusFolder;
    UPDATE_SOURCE mUpdateSource;
#endif
};

class GLTXUpdateVirusEngine : public UpdateVirusEngine {
public:
    static GLTXUpdateVirusEngine* NEW(const std::string &installdir, const std::string& configpath, int center_type);
    std::string DownloadRPFile(const std::string& url);

    GLTXUpdateVirusEngine(int center_type) : UpdateVirusEngine(center_type) {}
protected:
    int UpdateByEngine(UPDATE_SOURCE type, const std::string& updateurl, const std::string& navigate, std::string& newver);
};

class GJCZUpdateVirusEngine : public UpdateVirusEngine {
public:
    typedef enum {
        UPDATE_BY_CENTER = 0,
        UPDATE_BY_OFFLINE
    }UPDATE_VIRUS_TYPE;
public:
    static GJCZUpdateVirusEngine* NEW(const std::string &installdir, const std::string& configpath, int center_type);
    int SetNavigateData(UPDATE_VIRUS_TYPE type, const std::string& navigate);

    /** 离线升级的代码，参数为解压后的文件夹，内必须有 rsupdate.xml, cfg, data */
    int UpdateOffline(const std::string& source, std::string& version);

    GJCZUpdateVirusEngine(int center_type) : UpdateVirusEngine(center_type) {}

protected:
    int PrepareNavigate(UPDATE_SOURCE type, const std::string& url, const std::string& version, std::string& navigate);
    int DownloadFile(const std::string& url, const std::string& file);

    int DownloadCommand(const std::string& url, const std::string& file);

    int UpdateEntry();

private:
    int ImportOfflinelib(std::string& version);

private:
    std::string         mNavigateData;
    UPDATE_VIRUS_TYPE   mUpdateType;

private:
    pthread_t   mUpdateOfflineThread;
    static void* UpdateOfflineWork(void *params);
};

#endif // UPDATEVIRUSENGINE_H
