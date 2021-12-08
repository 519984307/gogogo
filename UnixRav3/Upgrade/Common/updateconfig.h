#ifndef UPDATECONFIG_H
#define UPDATECONFIG_H

#include <string>
#include <vector>

typedef enum {
    // MUST same as TimeValue::UpdateMode define in policysyncparse.h
    UPGRADE_EVERY_DAY,
    UPGRADE_EVERY_WEEK,
    UPGRADE_MANUALLY,
    UPGRADE_INTERVAL    
}UPGRADE_MODE;

typedef enum {
    DELAY_DISABLE = 0,
    DELAY_AUTOMATIC,
    DELAY_INTIME
}DELAY_START;

typedef enum {
    UPDATE_VIRUS_DB,
    UPDATE_ALL,
}UPDATE_CONTENT;

typedef enum {
    CONNECT_BROWSER = 0,
    CONNECT_DIRECT,
    CONNECT_PROXY
}PROXY_SET;

typedef enum {
    OFFICAL_HTTP,
    OTHER_HTTP,
    SHARE_FOLDER,
    FAKE_UPDATE,
}UPDATE_SOURCE;

class UpdateConfig {
public:
    UPGRADE_MODE    mUpdateMode;
    DELAY_START     mDelayStartAction;
    UPDATE_CONTENT  mUpdateContent;
    PROXY_SET       mProxySetting;
    UPDATE_SOURCE   mUpdateSourceAction;
    std::string     mUpdateModeExtend;
    int             mDelayStartTime;
    std::string     mProxyAddr;
    std::string     mProxyPort;
    std::string     mProxyAccount;
    std::string     mProxyPasswd;
    bool            mBandLimit;
    int             mBandWidth;
    std::string     mStartTime;
    std::string     mEndTime;
    std::string     mUpdateProduct;
    int             mForceUpdateDelay;
    bool            mClearUpdateCache;
    bool            mCheckVirusDbRealTime;
    std::pair<UPDATE_SOURCE, std::string> mCenterAddr;

public:
    UpdateConfig() {
        mUpdateMode = UPGRADE_MANUALLY;
        mDelayStartAction = DELAY_DISABLE;
        mUpdateContent = UPDATE_ALL;
        mProxySetting = CONNECT_DIRECT;
        mUpdateSourceAction = OTHER_HTTP;
        mBandWidth = 0;
        mForceUpdateDelay= 1;
        mClearUpdateCache = false;
    }

    void operator=(const UpdateConfig& config) {
        mUpdateMode = config.mUpdateMode;
        mDelayStartAction = config.mDelayStartAction;
        mUpdateContent = config.mUpdateContent;
        mProxySetting = config.mProxySetting;
        mUpdateSourceAction = config.mUpdateSourceAction;
        mUpdateModeExtend = config.mUpdateModeExtend;
        mDelayStartTime = config.mDelayStartTime;
        mProxyAddr = config.mProxyAddr;
        mProxyPort = config.mProxyPort;
        mProxyAccount = config.mProxyAccount;
        mProxyPasswd = config.mProxyPasswd;
        mBandWidth = config.mBandWidth;
        mStartTime = config.mStartTime;
        mEndTime = config.mEndTime;
        mCenterAddr = config.mCenterAddr;
        mUpdateProduct = config.mUpdateProduct;
        mForceUpdateDelay = config.mForceUpdateDelay;
        mClearUpdateCache = config.mClearUpdateCache;
        mCheckVirusDbRealTime = config.mCheckVirusDbRealTime;
    }
};

#endif // UPDATECONFIG_H
