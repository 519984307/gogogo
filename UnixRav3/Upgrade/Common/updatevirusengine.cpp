#include "updatevirusengine.h"
#include "updateobserver.h"
#include "upgrade_global.h"
#include "updateconfig.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "communicatebundle.h"
#include "service.h"
#include "zip/unzip.h"

#ifdef UPDATE_WITH_ENGINE_
    #include "scanengine.h"
#else
    #include "net/httprequest.h"
#endif

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

#define ESM_VIRUS_NAVIGATE_FILE     "rsupdate.xml"
#define SOC_VIRUS_NAVIGATE_FILE     "esvirlib_rsupdate.xml"

#define VIRUS_ESM_NAVIGATE_URL  "/ruc/esm/esvirlib/vlesm3/rsupdate.xml"
#define VIRUS_SOC_NAVIGATE_URL  "/ruc/rec/src/rd3/esvirlib/vlesm3/esvirlib_rsupdate.xml"

#define OFFICAL_VIRUS3_URL  "http://rsup10.rising.com.cn/viruslib/vlesm3ver.xml"

using namespace std;

/** define in libravcommon.so */
extern "C" int decompress_7z(const char *zip, const char *outfolder);

int UpdateOfflineCallback::OnVirusEvent(EngineEvent event, EngineType type, void *data) {
    if (VIRUS_UPDATE == event && RP_DOWNLOAD == type) {
        char *filename = (char*)data;

        string dest = Utils::Path::JoinPath(mDestFolder.c_str(), filename, NULL);
        string source = Utils::Path::JoinPath(mSourceFolder.c_str(), "dat", filename, NULL);
        if (Utils::Path::FileAvailed(source)) {
            return Utils::Path::CopyFile(source, dest)?0:-1;
        }
    }
    else if (VIRUS_UPDATE == event && RP_COMBINE == type) {
        char *filename = (char*)data;

        return 0;
    }
    return -1;
}


int UpdateCenterCallback::OnVirusEvent(EngineEvent event, EngineType type, void *data) {
//    if (VIRUS_UPDATE == event && RP_DOWNLOAD == type) {
//        char *filename = (char*)data;

//        if (mObserver) {
//            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
//                                     UpdateObserver::UPDATA_VIRUS_RP,
//                                     (void*)filename);
//        }
//#ifdef UPDATE_WITH_ENGINE_
//        std::string url = Utils::Path::JoinPath(mUpdateVirusBaseUrl.c_str(),"dat", filename, NULL);
//        std::string path = Utils::Path::JoinPath(mUpdateVirusFolder.c_str(), filename, NULL);

//        return DownloadFile(url, path);
//#endif
//    }
//    else if (VIRUS_UPDATE == event && RP_COMBINE == type) {
//        char *filename = (char*)data;
//        if (mObserver) {
//            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
//                                     UpdateObserver::UPDATE_VIRUS_COMBINE,
//                                     (void*)filename);
//        }
//        return 0;
//    }
    return -1;
}

UpdateVirusEngine* UpdateVirusEngine::NEW(const std::string &installdir,
                                          const std::string& configpath,
                                          int center_type) {
    UpdateVirusEngine *self = new UpdateVirusEngine(center_type);
    if (self) {
        if (self->Contruct(installdir, configpath)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}


int UpdateVirusEngine::UpdateEntry() {
    int err = 0;
    LOG_GJCZ("<update virus engine>");
    LOG_GJCZ("my center's url: %s", mUpdateConfig.mCenterAddr.second.c_str());
    string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(),\
                                        UPDATE_VIRUS_FOLDER, NULL);
    Utils::Path::DeleteFolder(folder.c_str());
    string navigate;

    err = PrepareNavigate(mUpdateConfig.mCenterAddr.first,
                          mUpdateConfig.mCenterAddr.second,
                          mCurrentVersion,
                          navigate);
    if (0 == err) {
    	string newver;
        LOG_GJCZ("navigating preparation succeed, update navigation: %s", navigate.c_str());
        err = UpdateByEngine(mUpdateConfig.mCenterAddr.first,
                             mUpdateConfig.mCenterAddr.second,
                             navigate,
                             newver);
    };

    if (err < 0) {
        if (mObserver) {
            string errcode = Utils::String::itoa(err);
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                     UpdateObserver::UDPATE_VIRUS_FAILED,
                                     &errcode);
        }
    }
    else if (err > 0) {
        mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                 UpdateObserver::UPDATE_NEWEST,
                                 &mCurrentVersion);
    }
    return err;
}

int UpdateVirusEngine::GetNavigate(UPDATE_SOURCE type, const std::string &url, const std::string &version, std::string &navigate) {
#ifndef UPDATE_WITH_ENGINE_
    if (SHARE_FOLDER == type) {
        ifstream ifs(url.c_str());
        navigate = string((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    }
    else {
    	int    ret;
        string header, conn;
        if ((ret = http_get(url, "", header, navigate, conn))) {
            LOG_GLTX("navigation getting error2 -->%d", ret);
            return -1;
        }
    }
#endif
    return 0;
}

int UpdateVirusEngine::PrepareNavigate(UPDATE_SOURCE type, const string& url, const string& version, string& navigate) {
    NavigateModel navimodel;
    string folder = MakeTmpFolder(UPDATE_VIRUS_FOLDER);

    std::string naviurl;
    if (OFFICAL_HTTP == type) {
        // offical update
        naviurl = OFFICAL_VIRUS3_URL;
    }
    else if (SHARE_FOLDER == type) {
        // 离线升级采用esm的病毒库路径
        string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
        if (Utils::Path::PathExists(folder)) {
            naviurl = Utils::Path::JoinPath(folder.c_str(), VIRUS_ESM_NAVIGATE_URL, NULL);
        }
    }
    else {
        string navigate_url, navigate_file;
        if (1 == mCenterType) {
            // ESM
            navigate_url = VIRUS_ESM_NAVIGATE_URL;
            navigate_file = ESM_VIRUS_NAVIGATE_FILE;
        }
        else if (2 == mCenterType) {
            //SOC
            navigate_url = VIRUS_SOC_NAVIGATE_URL;
            navigate_file = SOC_VIRUS_NAVIGATE_FILE;
        }
        else if (0 == mCenterType) {
            // SINGLE,
            navigate_url = VIRUS_SOC_NAVIGATE_URL;
            navigate_file = SOC_VIRUS_NAVIGATE_FILE;
        }

        naviurl = Utils::Path::JoinPath(url.c_str(), navigate_url.c_str(), NULL);
    }
    LOG_GJCZ("navigation file's url: %s", naviurl.c_str());

#ifndef UPDATE_WITH_ENGINE_
    LOG_GJCZ("UPDATE_WITH_ENGINE_");
    GetNavigate(type, naviurl, version, navigate);
    if (IsHtmlErrorPage(navigate)) {
        LOG_GJCZ("get an error html page: %s, , error code -1", navigate.c_str());
        return -1;
    }

    LOG_GJCZ("navigation: %s",navigate.c_str());

    if (navigate.empty()) {
        LOG_GJCZ("empty navigation, error code -2");
        return -2;
    }
    else if (ParseNavigate(navigate, navimodel)) {
        LOG_GJCZ("navigation parsing error, error code -3");
        return -3;
    }
#endif
    Utils::Path::DeleteFolder(folder.c_str());
    if (Utils::String::CompareVersion(version, navimodel.mVersion) >= 0) {
        return 1;
    }

    mCurrentVersion = navimodel.mVersion;
    LOG_GJCZ("get a new version of virus lib: %s", mCurrentVersion.c_str());

    if (mObserver) {
        mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                 UpdateObserver::UDDATE_GET_NEW_VERSION,
                                 &mCurrentVersion);
    }
    return 0;
}

int UpdateVirusEngine::UpdateByEngine(UPDATE_SOURCE type, const string& updateurl, const string& navigate, string& newver) {
    string folder = MakeTmpFolder(UPDATE_VIRUS_FOLDER);
#ifdef UPDATE_WITH_ENGINE_
    mUpdateSource = type;
#endif

    NavigateModel navimodel;
    if (ParseNavigate(navigate, navimodel)) {
        LOG_GJCZ("navigation parsing error, error code -11");
        return -11;
    }

    string virusurl;
    if (OFFICAL_HTTP == type) {
        virusurl = navimodel.mDownload;
    }
    else if (SHARE_FOLDER == type) {
        // 离线升级采用esm的病毒库路径
        string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
        if(Utils::Path::CheckPath(folder) == -1){
            LOG_GJCZ("can not make folder: %s, error code -12", folder.c_str());
            return -12;
        }

        string sourcefile = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), updateurl.c_str(), NULL);
        int err = decompress_7z(sourcefile.c_str(), folder.c_str());
        if(err == 1){
            LOG_GJCZ("decompression error, error code -13");
            return -13;
        }

        if (0 == err) {
            virusurl = Utils::Path::JoinPath(folder.c_str(), "/ruc/esm/esvirlib/vlesm3/", NULL);

            // 在这里处理直接替换病毒库的方式
            err = -1;
            string malware = Utils::Path::JoinPath(folder.c_str(), "/ruc/esm/esvirlib/vlesm3/dat/malware.rmd", NULL);
            if (Utils::Path::FileAvailed(malware)) {
                err = (Utils::Path::CopyFile(malware, Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, "malware.rmd", NULL))==true)?0:-1;
            }
            if (err == -1){
                LOG_GJCZ("failed to copy file %s, error code -14", malware.c_str());
                return -14;
            }
            if (mObserver) {
                mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                         (err==0)?UpdateObserver::UPDATE_VIRUS_SUCCESS:UpdateObserver::UDPATE_VIRUS_FAILED,
                                         &navimodel.mVersion);
            }
            return err;
        }
    }
    else if (FAKE_UPDATE == type) {
        char tempfile[128];
        for (int i=0; i<20; i++) {
            if (mObserver) {
                sprintf(tempfile, "%d-%d.rp", 701+i, 700+i);
                mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                         UpdateObserver::UPDATE_VIRUS_COMBINE,
                                         tempfile);
            }
            sleep(1);
        }

        newver = navimodel.mVersion;
        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                     UpdateObserver::UPDATE_VIRUS_SUCCESS,
                                     &newver);
        }
        return 0;
    }
    else {
        string baseurl = Utils::Path::JoinPath(updateurl.c_str(), GetDefaultAddress().c_str(), NULL);
        virusurl = Utils::Path::JoinPath(baseurl.c_str(), Utils::String::ToLower(navimodel.mProduct).c_str(), NULL);
    }

    std::string cfgfile = navimodel.mVersion + ".cfg";
    std::string newcfg = Utils::Path::JoinPath(folder.c_str(), cfgfile.c_str(), NULL);
    std::string downurl = Utils::Path::JoinPath(virusurl.c_str(), "cfg", cfgfile.c_str(), NULL);
    LOG_GJCZ("download from %s to %s", downurl.c_str(), newcfg.c_str());

    int err = 0;
    if (SHARE_FOLDER == type) {
        err = Utils::Path::CopyFile(downurl, newcfg)?0:-1;
    }
    else {
        err = DownloadFile(downurl, newcfg);
    }

    if (err) {
        LOG_GJCZ("cfg file downloading error: DownloadFile returned %d, errno %d, error code -15", err, errno);
        return -15;
    }

#ifdef UPDATE_WITH_ENGINE_
    mUpdateVirusBaseUrl = virusurl;
    mUpdateVirusFolder = folder;

    sleep(1);

    ScanEngine *engine = ScanEngine::NEW(this, "", "", NULL);
    if (NULL == engine) {
        LOG_GJCZ("scan engine creation error, error code -16");
        return -16;
    }
    sleep(1);
    err = engine->UpdateVirusLib(newcfg.c_str(),
                                 folder.c_str(),
                                 Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), NULL).c_str());

    delete engine;
#endif        
    Utils::Path::DeleteFolder(folder.c_str());

    string offline = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
    if (Utils::Path::PathExists(offline)) {
        Utils::Path::DeleteFolder(offline.c_str());
    }
    if (err >= 0) {       
        newver = navimodel.mVersion;
        LOG_GLTX("get newver %s", newver.c_str());
        LOG_GJCZ("update virus success %s", newver.c_str());

        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                     UpdateObserver::UPDATE_VIRUS_SUCCESS,
                                     &newver);
        }
    }
    return err;
}

//int UpdateVirusEngine::HttpVirusEntry(const std::string& url, const std::string& version) {
//    LOG_GLTX("update virus url %s", url.c_str());

//    string navigate;
//    int err = PrepareNavigate(url, version, navigate);
//    if (err) {
//        return err;
//    };

//    LOG_GLTX("update navigate is %s", navigate.c_str());

//    string newver;
//    return UpdateByEngine(url, navigate, newver);
//}

//int UpdateVirusEngine::OfflineVirusEntry(const std::string& url, const std::string& version) {
//    return 0;
//}

int UpdateVirusEngine::OnVirusEvent(EngineEvent event, EngineType type, void *data) {
    if (VIRUS_UPDATE == event && RP_DOWNLOAD == type) {
        char *filename = (char*)data;

        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                     UpdateObserver::UPDATA_VIRUS_RP,
                                     (void*)filename);
        }
#ifdef UPDATE_WITH_ENGINE_
        std::string url = Utils::Path::JoinPath(mUpdateVirusBaseUrl.c_str(),"dat", filename, NULL);
        std::string path = Utils::Path::JoinPath(mUpdateVirusFolder.c_str(), filename, NULL);

        LOG_GJCZ("download rp from %s to %s", url.c_str(), path.c_str());

        int err = -1;
        if (SHARE_FOLDER == mUpdateSource) {
            err = Utils::Path::CopyFile(url, path)?0:-1;
        }
        else {
            err = DownloadFile(url, path);
            if (err) {
                LOG_GJCZ("cfg downloading error: DownloadFile returned %d, errno %d", err, errno);
            }
        }
        return err;
#endif
    }
    else if (VIRUS_UPDATE == event && RP_COMBINE == type) {
        char *filename = (char*)data;

        LOG_GJCZ("combine the RP file %s", filename);

        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS,
                                     UpdateObserver::UPDATE_VIRUS_COMBINE,
                                     (void*)filename);
        }
        return 0;
    }
    return -1;
}

GJCZUpdateVirusEngine* GJCZUpdateVirusEngine::NEW(const std::string &installdir, const std::string& configpath, int center_type) {
    GJCZUpdateVirusEngine *self = new GJCZUpdateVirusEngine(center_type);
    if (self) {
        if (self->Contruct(installdir, configpath)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int GJCZUpdateVirusEngine::PrepareNavigate(UPDATE_SOURCE type, const std::string& url, const std::string& version, std::string& navigate) {
    navigate = mNavigateData;
    return 0;
}

int GJCZUpdateVirusEngine::DownloadFile(const std::string &url, const std::string &file) {
    int err = 0;
    int retry = 0;
    do {
        err = DownloadCommand(url, file);
        if (err == -2){
            LOG_GJCZ("download failed");
        }
        else if (err) {
            LOG_GJCZ("download overtime, retry ...");
            sleep(1);
        }
    }while((err != 0) && (err != -2));
    return err;
}

int GJCZUpdateVirusEngine::DownloadCommand(const std::string& url, const std::string& file) {
    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_UPDATE_HTTP_DOWNLOAD;
    command.mStringValue = url;

    char temp[128];
    sprintf(temp, RJJH_NOTIFY_SOCKET, mNotifierUID);
    string sock = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GJCZ, temp, NULL);

    RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, command);
    int ret = -1;
    unsigned char *ret_data = NULL;
    int outlen = 0;

    if (0 == pack.Externalize(
                sock.c_str(),
                ret,
                (void**)&ret_data,
                &outlen)) {
        if (ret == 0 && ret_data && outlen) {
            if(strcmp((char *)ret_data, "ERROR") == 0){
                delete[] ret_data;
                return -2;
            }
        
            FILE *f = fopen(file.c_str(), "wb+");
            if (f) {
                int writed = 0;
                do {
                    writed += fwrite(ret_data+writed, 1, outlen-writed, f);
                }while(writed<outlen);
                fclose(f);
                delete[] ret_data;
                return 0;
            }
            else{
                LOG_GJCZ("cannot open destination file, ");
                delete[] ret_data;
                return -1;
            }
        }
        else{
            LOG_GJCZ("empty returned data");
            return -1;
        }
    }
    LOG_GJCZ("externalization error");
    return -1;
}

int GJCZUpdateVirusEngine::ImportOfflinelib(string& version) {
    string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
    Utils::Path::CheckPath(folder);

    if (!Utils::Path::PathExists(folder)) {
        LOG_GJCZ("virus lib update can not make folder");
        return -1;
    }

    int err = decompress_7z(mNavigateData.c_str(), folder.c_str());
    if (!err) {
        err = UpdateOffline(folder, version);
    }
    Utils::Path::DeleteFolder(folder.c_str());

    return err;
}

int GJCZUpdateVirusEngine::UpdateEntry() {
    // TODO 目前离线升级分为两种
    // 保密机的离线升级, 因为涉及到文件权限的问题, 所以目前暂保密保密机的代码
    // 普通机的离线升级, 这个解压文件放在了 /tmp/offline文件夹下
//#ifdef SECRET_MACHINE
//    if (UPDATE_BY_CENTER == mUpdateType) {
//        return UpdateVirusEngine::UpdateEntry();
//    }
//    else if (UPDATE_BY_OFFLINE == mUpdateType) {
//        string version;
//        int err = ImportOfflinelib(version);
//        if (mObserver) {
//            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_VIRUS, err==0?UpdateObserver::UPDATE_OFFLINE_VLIB:UpdateObserver::UPDATE_ERROR, &version);
//        }
//    }
//    return 0;
//#else

    int iRet = UpdateVirusEngine::UpdateEntry();

    if((iRet == 0) && (UPDATE_BY_OFFLINE == mUpdateType))
    {
        //for 保密机的离线升级
        std::string update = Utils::Path::JoinPath(mInstallDir.c_str(),
                                                   DEPLOYED_BIN_FOLDER,
                                                   UPDATE_PROCESS_ENTRY, NULL);
        LOG_GJCZ("start the exec command");
        pid_t pid;
        if ((pid=fork()) == 0) {
            char uid[32] = {0};
            sprintf(uid, "%d", mNotifierUID);
            if(execl(update.c_str(), UPDATE_PROCESS_ENTRY, "-i", uid, NULL)<0)  {
                //
            }
        }
    }

    return iRet;
//#endif
}

int GJCZUpdateVirusEngine::SetNavigateData(UPDATE_VIRUS_TYPE type, const string& navigate) {
    Json::Value json;
    Json::Reader reader;
    if (!reader.parse(navigate, json)) {
        return -1;
    }

    if (json.isMember("type")) {
        mUpdateType = json["type"].asInt() == SHARE_FOLDER ? UPDATE_BY_OFFLINE : UPDATE_BY_CENTER;
    }
    else {
        mUpdateType = type;
    }

    string url = json["url"].asString();
    mNavigateData = json["navigate"].asString();

    if (UPDATE_BY_CENTER == mUpdateType) {
        mUpdateConfig.mCenterAddr = make_pair((UPDATE_SOURCE)json["type"].asInt(), url);
    }
    else if (UPDATE_BY_OFFLINE == mUpdateType) {
        mUpdateConfig.mCenterAddr = make_pair(SHARE_FOLDER, url);
    }

    return 0;
}

int GJCZUpdateVirusEngine::UpdateOffline(const string &source, string& version) {
    string dest = MakeTmpFolder(UPDATE_VIRUS_FOLDER);

    string navifile = Utils::Path::JoinPath(source.c_str(), "rsupdate.xml", NULL);
    if (!Utils::Path::FileAvailed(navifile)) {
        return -1;
    }

    ifstream stream(navifile.c_str());
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string navigate(buffer.str());

    NavigateModel navimodel;
    if (ParseNavigate(navigate, navimodel)) {
        return -1;
    }

    std::string cfgfile = navimodel.mVersion + ".cfg";
    std::string newcfg = Utils::Path::JoinPath(source.c_str(), "cfg", cfgfile.c_str(), NULL);
    if (!Utils::Path::FileAvailed(newcfg)) {
        return -1;
    }

#ifdef UPDATE_WITH_ENGINE_
    UpdateOfflineCallback callback(source, dest);
    ScanEngine *engine = ScanEngine::NEW(&callback,
                                  Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), VIRUS_DB_FILE, NULL),
                                  Utils::Path::JoinPath(mInstallDir.c_str(), SCAN_ENGINE_LIB_PATH, NULL));
    if (NULL == engine) {
        return -1;
    }

    int err = engine->UpdateVirusLib(newcfg.c_str(),
                                 dest.c_str(),
                                 Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), NULL).c_str());

    delete engine;
#endif
    Utils::Path::DeleteFolder(dest.c_str());
    version = navimodel.mVersion;
    return 0;
}


GLTXUpdateVirusEngine* GLTXUpdateVirusEngine::NEW(const std::string &installdir, const std::string& configpath, int center_type) {
    GLTXUpdateVirusEngine *self = new GLTXUpdateVirusEngine(center_type);
    if (self) {
        if (self->Contruct(installdir, configpath)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int GLTXUpdateVirusEngine::UpdateByEngine(UPDATE_SOURCE type, const string& updateurl, const string& navigate, string& newver) {
    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::REMOTE_COMMAND_UPDATE;
    Json::Value json;
    json["url"] = updateurl;
    json["navigate"] = navigate;
    json["type"] = type;
    Json::FastWriter writer;
    command.mStringValue = writer.write(json);

    LOG_GLTX("send the update virus signal to GJCZ");
    MakeTmpFolder(UPDATE_VIRUS_FOLDER);
    int ret = ServiceBinder::NotifyData(mInstallDir, FOLDER_RJJH_GLTX, RJJH_NOTIFY_SOCKET, command, false, mNotifierUID);
    return ret;
}

string GLTXUpdateVirusEngine::DownloadRPFile(const string& url) {
    //Download file from url and name it "file.tmp"
    string folder = Utils::Path::JoinPath(mInstallDir.c_str(),  mOwnerPath.c_str(),\
                                        UPDATE_VIRUS_FOLDER, NULL);
    string::size_type lastSlash = url.rfind("/");
    string fileName = url.substr(lastSlash+1);
    string file = Utils::Path::JoinPath(folder.c_str(), fileName.c_str(), NULL);
    string tmpFile = file + ".tmp";

    LOG_GLTX("Download RPFile from %s to %s", url.c_str(), file.c_str());
    int err = DownloadFile(url, tmpFile);

    //Download completed, remove file's ".tmp" suffix. In "file.size -4",
    //"4" means the length of ".tmp"
    if (0 == err) {
        for(int i=0; i<3 && rename(tmpFile.c_str(), file.c_str()); i++){
            sleep(1);
        }
        if(Utils::Path::FileAvailed(tmpFile)){
            LOG_GLTX("tmp RPFile renaming error");
            unlink(tmpFile.c_str());
            return "";
        }
        return file;
    }
    LOG_GLTX("RPFile downloading error, DownloadFile returned %d, errno %d", err, errno);
    unlink(tmpFile.c_str());
    return "";
}

