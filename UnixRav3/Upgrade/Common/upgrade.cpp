#include "upgrade.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "curl/curl.h"
#include "updateobserver.h"
#include "net/httprequest.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <list>
#include <algorithm>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/utsname.h>
#include <sys/stat.h>

using namespace std;


#ifndef UPDATE_WITH_ENGINE_
static FILE * OpenCurlDebugLog()
{
	int  nRet;
	char szPath[256] = {0}, szTmp[32] = {0}, *p;
	std::string strLog;
	struct stat s;

	nRet = readlink("/proc/self/exe", szPath, sizeof(szPath)-1);
	if(nRet > 0 && nRet < int(sizeof(szPath)-1)) szPath[nRet] = 0;
	p    = strrchr(szPath, '/');
	if(!p) return 0;

	p[1] = 0; strLog = szPath; strLog += "../plugins/rscurl.log.enable";
	if(stat(strLog.c_str(), &s)!=0) return 0;

	strLog = szPath; strLog += "../RJJHetc/rscurl.log";
	sprintf(szTmp, ".%d", getpid()); strLog += szTmp;
	FILE * pf = fopen(strLog.c_str(), "ab");
	if(pf) fprintf(pf, "==================================================\n");
	return pf;
}

static int  CurlDebugLog(CURL * handle, curl_infotype type, char * data, size_t size, void * userptr)
{
	FILE * pf = (FILE*)userptr;
	if(!pf) return 0;

	switch(type)
	{
	case CURLINFO_TEXT:
	case CURLINFO_HEADER_OUT:
	case CURLINFO_HEADER_IN:
		fwrite(data, size, 1, pf);
		fflush(pf);
		break;
	default:
		break;
	}
	return 0;
}
#endif


UpdateEngine::UpdateEngine() {
    mStatus = 0;
    mObserver = NULL;
    mNotifierUID = 0;
}

UpdateEngine::~UpdateEngine() {

}

int UpdateEngine::Contruct(const std::string &installdir, const std::string& configpath) {
    mInstallDir = installdir;
    mOwnerPath = configpath;

    return 0;
}

int UpdateEngine::Start(const std::string& version) {
    mCurrentVersion = version;
    mStatus = 1;

    return pthread_create(&mUpdateThread, NULL, ThreadWork, this);
}

void* UpdateEngine::ThreadWork(void *params) {
    UpdateEngine *engine = (UpdateEngine*)params;
    engine->UpdateEntry();
    return NULL;
}


int UpdateEngine::Stop() {
    mStatus = 0;
    return pthread_cancel(mUpdateThread);
}

std::string UpdateEngine::MakeTmpFolder(const std::string& foldername) {
    std::string folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), foldername.c_str(), NULL);
    Utils::Path::DeleteFolder(folder.c_str());
    Utils::Path::mkdirs(folder);
    return folder;
}

bool UpdateEngine::IsHtmlErrorPage(const std::string& line) {
    bool found = false;
    if (line.find("<HTML>") != std::string::npos
            || line.find("<html>") != std::string::npos) {
        found = true;
    }
    return found;
}

int UpdateEngine::ParseNavigate(const std::string& navi, NavigateModel& model) {
    // define in policysyncparser.cpp
#define XML_NODE_STRING_VALUE(node, value) do {\
                                                 xmlChar *data = xmlNodeGetContent(node); \
                                                 value = (char*)data;\
                                                 xmlFree(data);\
                                              }while(0);
#define XML_PROP_STRING_VALUE(node, name, value) do {\
                                                 xmlChar *data = xmlGetProp(node, BAD_CAST(name)); \
                                                 value = (char*)data;\
                                                 xmlFree(data);\
                                              }while(0);

//    xmlDocPtr doc = xmlParseMemory(navi.c_str(), navi.size());
    xmlDocPtr doc = xmlReadMemory(navi.c_str(), navi.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    for(xmlNodePtr ptr=root->children; ptr; ptr=ptr->next) {
        if (xmlStrcmp(BAD_CAST"PRODUCT", ptr->name) == 0) {
            XML_PROP_STRING_VALUE(ptr, "NAME", model.mProduct);
            XML_PROP_STRING_VALUE(ptr, "VERSION", model.mVersion);
        }
        else if(xmlStrcmp(BAD_CAST"URLLIST",ptr->name) == 0) {
            for(xmlNodePtr p=ptr->children; p; p=p->next) {
                if(xmlStrcmp(BAD_CAST"ITEM",p->name) == 0) {
                    string name;
                    XML_PROP_STRING_VALUE(p, "KEY", name);

                    if (name == "Validate") {
                        XML_NODE_STRING_VALUE(p, model.mValidate);
                    }
                    else if (name == "Finish") {
                        XML_NODE_STRING_VALUE(p, model.mFinish);
                    }
                    else if (name == "Overtime") {
                        XML_NODE_STRING_VALUE(p, model.mOvertime);
                    }
                    else if (name == "Download" || name == "DownCert") {
                        XML_NODE_STRING_VALUE(p, model.mDownload);
                    }
                }
            }
            break;
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int UpdateEngine::GetNavigate(UPDATE_SOURCE type, const std::string &url, std::string &navigate) {
#ifndef UPDATE_WITH_ENGINE_
    if (SHARE_FOLDER == type) {
        if (Utils::Path::FileAvailed(url.c_str())) {
            ifstream ifs(url.c_str());
            navigate = string((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
        }
    }
    else {
    	int    ret;
        string header, info;
        if ((ret = http_get(url, "", header, navigate, info))) {
            LOG_GLTX("navigation getting error1 -->%d,%s", ret, url.c_str());
            return -1;
        }
    }
#endif
    return 0;
}


const std::string UpdateEngine::GetDownloadUrl(UPDATE_SOURCE sourcetype,
                                               const std::string& updateurl,
                                               const std::string& naviurl,                 
                                               NavigateModel& navimodel) {
    std::string navigate = Utils::Path::JoinPath(updateurl.c_str(), naviurl.c_str(), NULL);
    std::string navicontent;
    //LOG_GLTX("get url --->%s", navigate.c_str());
#ifndef UPDATE_WITH_ENGINE_
    GetNavigate(sourcetype, navigate, navicontent);
    if (IsHtmlErrorPage(navicontent)) {
        return "";
    }

    if (ParseNavigate(navicontent, navimodel)) {
        return "";
    }
#endif

    // 此处判断是否为离线升级, 如果为离线升级,则不使用XML的导航
    if (SHARE_FOLDER == sourcetype) {
        navimodel.mDownload.clear();
    }

    // FIXME 此处屏掉是因为官网升级此处是错的, 所以为了正确的执行, 此处先屏掉
//    if (!navimodel.mDownload.empty()) {
//        struct utsname uts;
//        if (0 == uname(&uts)) {
//            string arch = uts.machine;
//            if (Utils::String::StartsWith(arch, "mips64")) {
//                arch = "mips64";
//            }
//            return Utils::Path::JoinPath(navimodel.mDownload.c_str(), arch.c_str(), NULL);
//        }
//    }

    return Utils::Path::JoinPath(updateurl.c_str(), GetDefaultAddress().c_str(), NULL);
}

int UpdateEngine::GetRedirectUrl(const std::string& url, std::string& redirect) {
#ifndef UPDATE_WITH_ENGINE_
    CURLcode res;
    CURL* curl = curl_easy_init();
    if(NULL == curl)  {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (url.find("https") == 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }

    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        char *rurl = NULL;
        if (CURLE_OK == curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &rurl) && rurl) {
            redirect = rurl;
        }
    }
    curl_easy_cleanup(curl);

    return res;
#else
    return 0;
#endif
}

int UpdateEngine::DownloadFile(const std::string &url, const std::string &file) {
#ifndef UPDATE_WITH_ENGINE_
    CURLcode res;
    FILE *fp;

    res = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != res)  {
        curl_global_cleanup();
        return CURLE_FAILED_INIT;
    }

    CURL* curl = curl_easy_init();
    if(NULL == curl)  {
        return CURLE_FAILED_INIT;
    }

    fp = fopen(file.c_str(), "wb");
    if (fp == NULL) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }

	FILE * pfLog = OpenCurlDebugLog();
	if(pfLog) {
		res = curl_easy_setopt(curl, CURLOPT_VERBOSE,        1L);
		res = curl_easy_setopt(curl, CURLOPT_DEBUGDATA,      pfLog);
		res = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION,  CurlDebugLog);
	}

    // 设置代理
    if (mUpdateConfig.mProxySetting == CONNECT_PROXY ) {
        if (!mUpdateConfig.mProxyAddr.empty() && !mUpdateConfig.mProxyPort.empty()) {
            string proxy = mUpdateConfig.mProxyAddr+":"+mUpdateConfig.mProxyPort;
            curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
        }
        if (!mUpdateConfig.mProxyAccount.empty() && !mUpdateConfig.mProxyPasswd.empty()) {
            string proxy = mUpdateConfig.mProxyAccount+":"+mUpdateConfig.mProxyPasswd;
            curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxy.c_str());
        }

        if (mUpdateConfig.mBandWidth > 0) {
            bool intime = true;
            if (!mUpdateConfig.mStartTime.empty() && !mUpdateConfig.mEndTime.empty()) {
                intime = Utils::Date::NowInTimeArea(mUpdateConfig.mStartTime, mUpdateConfig.mEndTime);
            }
            if (intime) {
                curl_easy_setopt (curl, CURLOPT_MAX_RECV_SPEED_LARGE, (curl_off_t)mUpdateConfig.mBandWidth*1024);
            }
        }
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (url.find("https") == 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        if (mUpdateConfig.mProxySetting == CONNECT_PROXY ) {
            curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1L);
        }
    }

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UpdateEngine::WriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);


    res = curl_easy_perform(curl);

    int size = ftell(fp);
    fclose(fp); if(pfLog) fclose(pfLog);
    curl_easy_cleanup(curl);
    curl_global_cleanup();   

    return res? res:(size>0?0:-2);
#else
    return 0;
#endif
}

int UpdateEngine::WriteData(void* buffer, size_t size, size_t nmemb, FILE* file) {
    return fwrite(buffer, size, nmemb, file);
}
