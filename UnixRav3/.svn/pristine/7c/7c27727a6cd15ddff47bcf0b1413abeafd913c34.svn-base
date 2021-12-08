#include "cloudengine.h"
#include "commonapi.h"
#include "cloudnetproxy.h"
#include "rsupgradecode.h"
#include "engineobserver.h"
#include "scanengine_global.h"
#include "json/json.h"

#include <libxml/parser.h>
#include <pthread.h>


#define CHECK_STATUS_   "cloud.html"
#define NAVAGET_XML     "navigate.xml"

#define DEFAULT_GUID    "00000000-0000-0000-0000-000000000000"
#define DEFAULT_VERSION "1.0.0.0"
#define DEFAULT_PRODUCT "ESM"
#define CLOUD_VERSION   "3.1"

CloudEngine* CloudEngine::NEW(EngineObserver *observer) {
    CloudEngine *self = new CloudEngine(observer);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

CloudEngine::CloudEngine(EngineObserver *observer) : mObserver(observer) {
    mCheckThread = 0;
    mStatus = false;
    mProxy = NULL;
}

CloudEngine::~CloudEngine() {
    if (mCheckThread) {
        Stop();
    }
}

int CloudEngine::Construct() {
    return 0;
}

void CloudEngine::Start(vector<CloudConfig>& config) {
    mCloudConfig = config;
    mStatus = true;

    StartCheck();
}

void CloudEngine::Stop() {
    mStatus = false;
    pthread_cancel(mCheckThread);
    pthread_join(mCheckThread, NULL);
    mCheckThread = 0;
}

void CloudEngine::StartCheck() {
    if (0 == mCheckThread) {
        pthread_create(&mCheckThread, NULL, CheckThread, this);
    }
}

void* CloudEngine::CheckThread(void *params) {
    CloudEngine *self = (CloudEngine*)params;
    self->CheckThreadFunc();
    return NULL;
}

int CloudEngine::ParseNavigate(const string& data, Navigate& navi) {
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

    xmlDocPtr doc = xmlReadMemory(data.c_str(), data.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    for(xmlNodePtr child=root->children; child; child=child->next) {
        if(xmlStrcmp(BAD_CAST"SEARCH",child->name) == 0) {
            XML_NODE_STRING_VALUE(child, navi.search);
        }
        else if (xmlStrcmp(BAD_CAST"SWITCH",child->name) == 0) {
            string temp;
            XML_PROP_STRING_VALUE(child, "DEFAULTON", temp);
            navi.defaulton = atoi(temp.c_str());
            XML_PROP_STRING_VALUE(child, "CACHELIFE", temp);
            navi.cachelife = atoi(temp.c_str());
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

void CloudEngine::CheckThreadFunc() {
    vector<CloudConfig>::iterator iter;
    char buffer[1024];
    for (iter=mCloudConfig.begin(); iter!=mCloudConfig.end(); ++iter) {
        mCenterAddr = iter->mAddr;
        while(mStatus) {
            mStatus = false;
            if (Utils::String::StartsWith(mCenterAddr, "http")) {
                strcpy(buffer, mCenterAddr.c_str());
            }
            else {
                sprintf(buffer, "http://%s", mCenterAddr.c_str());
            }

            if (mObserver) {
                if (0 == mObserver->OnVirusEvent(EngineObserver::CLOUD_VIRUS, EngineObserver::CLOUD_CHECK, (void*)buffer)) {
                    if (ParseNavigate(buffer, mNavigate)) {
                        mStatus = true;
                    }
                }
            }

            mObserver->OnVirusEvent(EngineObserver::CLOUD_VIRUS, EngineObserver::CLOUD_STATE, &mStatus);
            pthread_testcancel();
            sleep(60);
        }
    }
//    printf("cloud check exit\n");
}

int CloudEngine::CloudCheck(vector<ScanEntry*> *data) {
    Json::Value total;
    int err = -1;

    Json::Value json;
    json["guid"] = mClientGUID.empty()?DEFAULT_GUID:mClientGUID;
    json["pro"] = DEFAULT_PRODUCT;
    json["ver"] = DEFAULT_VERSION;
    json["cloud"] = CLOUD_VERSION;

    total["uinfo"] = Json::Value(json);

    Json::Value crc;
    vector<ScanEntry*> cloudPool;
    vector<ScanEntry*>::iterator iter;
    for (iter=data->begin(); iter!=data->end(); ++iter) {
        if (strlen((*iter)->hash) > 0) {
            crc.append(Json::Value((*iter)->hash));
            cloudPool.push_back(*iter);
        }
    }
    total["crc"] = Json::Value(crc);

    Json::FastWriter writer;
    string value = writer.write(total);

    int maxlen = value.size()*2;
    char *decrypt = new char[maxlen];
    long outlen = RsUpgradeEncrypt(decrypt, maxlen, value.c_str());
    decrypt[outlen] = '\0';

    pair<const char*, char*> params = make_pair(mNavigate.search.c_str(), decrypt);
    if (mObserver) {
        err = mObserver->OnVirusEvent(EngineObserver::CLOUD_VIRUS, EngineObserver::CLOUD_POST, &params);
    }
    if (0 == err) {
        maxlen = strlen(decrypt);
        char *ret_str = new char[strlen(decrypt)];
        long outlen = RsUpgradeDecrypt(ret_str, maxlen, decrypt);
        ret_str[outlen] = '\0';
        err = ConfirmResult(decrypt, &cloudPool);
        delete[] ret_str;
    }
    delete [] decrypt;
    return err;
}

int CloudEngine::ConfirmResult(const string& result, vector<ScanEntry*> *data) {
    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(result, json)) {
        return -1;
    }

    vector<ScanEntry*> hash;

    Json::Value crcs = json["crc"];
    for (int i=0; i<crcs.size(); ++i) {
        (*data)[i]->state = crcs[i].asInt();
        if ((*data)[i]->state == ScanEntry::CLOUD_BLANK) {
            hash.push_back((*data)[i]);
        }
    }

    Json::Value virus = json["virusinfo"];
    for (int i=0; i<virus.size(); ++i) {
        Json::Value v = virus[i];
        Json::Value::Members keys = v.getMemberNames();
        Json::Value::Members::iterator iter;
        for (iter=keys.begin(); iter!=keys.end(); ++iter) {
            if (strcmp(hash[i]->hash, iter->c_str()) == 0) {
                Json::Value match = v[*iter];
                strcpy(hash[i]->virus, match["name"].asString().c_str());
            }
        }

    }
    return 0;
}
