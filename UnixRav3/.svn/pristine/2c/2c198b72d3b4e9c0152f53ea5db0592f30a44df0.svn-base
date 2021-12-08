#include "filterengine.h"
#include "Entry/urlruleentry.h"
#include "Entry/netaccessentry.h"
#include "storagebridge.h"
#include "baseconfig.h"
#include "commonapi.h"

FilterEngine* FilterEngine::NEW(ServiceBinder *binder, StorageBridge *storage, AppConfig *config) {
    FilterEngine *self = new FilterEngine(binder, storage, config);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}
FilterEngine::FilterEngine(ServiceBinder *binder, StorageBridge *storage, AppConfig *config)
                        : KernelApplication (binder, storage, config) {
    mNetfilterProxy = NULL;
    mStatus = false;
}

FilterEngine::~FilterEngine() {
    if (mNetfilterProxy) {
        delete mNetfilterProxy;
        mNetfilterProxy = NULL;
    }
}

int FilterEngine::Construct() {
    KernelApplication::Construct();

    LOG_ZDFY("net filter create the connect proxy");

    mNetfilterProxy = new CNFLCommunicator(this);
    return 0;
}

int FilterEngine::StartNetFilter() {
    LOG_ZDFY("net filter install ko");
    if (KernelApplication::StartMonitor(NET_FILTER_MODULE) < 0) {
        return -1;
    }

    if (!mStatus) {
        if (mNetfilterProxy->Init() != 0) {
            return -1;
        }
    }
    mStatus = true;
    LOG_ZDFY("refresh the net filter list");
    return RefreshList();
}

int FilterEngine::StopNetFilter() {
    return 0;
}

int FilterEngine::RefreshList() {
    std::list<UrlRuleEntry> datas;
    std::list<std::string> rules;
    std::string stmp;

    if (mStorage->GetUrlBlackList(datas) > 0) {
        // create netlink socket
        std::list<UrlRuleEntry>::iterator iter;
        for (iter=datas.begin();iter!=datas.end();++iter) {
            if(iter->mType == UrlRuleEntry::IP_ADDR){
                stmp = "I#";
                stmp += iter->mPath;
            }else if(iter->mType == UrlRuleEntry::URL_MATCH){
                stmp = "U#";
                stmp += iter->mPath;
            }
            rules.push_back(stmp);
        }
    }
    if (mNetfilterProxy->UpdateBlackList(rules) != 0) {
        return -1;
    }
    return 0;
}

int FilterEngine::OnNFLCommunicateEvent(FilterEvent event, FilterType type, const string &data) {
    time_t timep;
    time(&timep);

    switch (type) {
    case  BLOCK_IP: {
        NetAccessEntry entry;
        entry.mType = NetAccessEntry::IP;
        entry.mUrl = data;
        entry.mTime = (long)timep;
        mStorage->AppendNetAccessLog(entry);
        break;
    }
    case BLOCK_URL: {
        NetAccessEntry entry;
        entry.mType = NetAccessEntry::URL;
        entry.mUrl = data;
        entry.mTime = (long)timep;
        mStorage->AppendNetAccessLog(entry);
        break;
    }
    default:
        break;
    }
    return 0;
}
