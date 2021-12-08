#include "scannotifycallback.h"
#include "mainengine.h"
#include "scanconfig.h"

// TODO, 这个设计太差，前期没有考虑到单个文件， 后期有时间重新设计，从applicaon继承
ScanNotifyCallback::ScanNotifyCallback(MainEngine *major) : mMainEngine(major) {
    mVirused = false;
}

int ScanNotifyCallback::onEvent(EngineEvent event, EngineType type, void *data) {
    if (VIRUS_JUDGE == event) {
        switch (type) {
        case JUDGE_DANGE: {
            std::pair<std::string, std::string> *virusInfo = (std::pair<std::string, std::string>*)data;

            mVirused = !virusInfo->second.empty();
            int sel = mMainEngine->QueryUserForResult(0,
                                            virusInfo->first,
                                            virusInfo->second);


            return sel;
        }
        default: {
            break;
        }
        }
    }
    return 0;
}
