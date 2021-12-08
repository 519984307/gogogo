#ifndef SCANCONFIG_H
#define SCANCONFIG_H

#include <vector>
#include <string>

#include "Entry/baseentry.h"

using namespace std;

typedef enum {
    ALL_FILE = 0,  
    EXE_DOC  = 3,
    SPECIAL_FILE_TYPE
}SCAN_TYPE;

typedef enum {
    ONLY_POP_VIRUS = 0x1,
    AI_SCAN         = 0x2,
    COMPRESS_SCAN   = 0x4
}SCAN_ENGINE;

typedef enum {
    AUTO_DEAL   = 0,
    MANUAL_DEAL,
    IGNORE_DEAL
}VIRUS_DEAL;

typedef enum {
    CLEAN_FAILED_DELETE,
    CLEAN_FAILED_IGNORE
}CLEAN_FAILED;

typedef enum {
    NOT_BACKUP = 0,
    BACKUP_ORIGIN
}VIRUS_BACKUP;

typedef enum {
    LARGE_FILE_QUERY   = 0,
    LARGE_FILE_DELETE,
    LARGE_FILE_IGNORE
}LARGE_FILE;

typedef enum {
    OVERRIDE_OLD    = 0,
    INCREAM_SPACE
}SPACE_LESS;

typedef enum {
    BACKUP_FAILED_QUERY   = 0,
    BACKUP_FAILED_DELETE,
    BACKUP_FAILED_IGNORE
}VIRUS_BACKUP_FAILED;

typedef enum {
    DUBIOUS = 0,
    VIRUS,
    WORM,
    ROOTKIT,
    ADWARE,
    TROJAN,
    BACKDOOR,
    SHELL
}VIURS_TYPE;

class CloudConfig : public Serializable {
public:
    string mAddr;
    int    mPort;
    int    mRequestType;
    int    mPostCount;

public:
    CloudConfig() {
        mPort = 80;
        mRequestType = 1; //mean POST
        mPostCount = 80;
    }

    void InitializeJson(Json::Value& json) {
        mPort = json["port"].asInt();
        mRequestType = json["type"].asInt();
        mPostCount = json["count"].asInt();
        mAddr = json["addr"].asString();
    }

    void ExternlizeJson(Json::Value& json) {
        json["port"] = Json::Value(mPort);
        json["type"] = Json::Value(mRequestType);
        json["count"] = Json::Value(mPostCount);
        json["addr"] = Json::Value(mAddr);
    }
};

class ScanConfig : public Serializable {
public:
    SCAN_TYPE           mScanFileType;
    bool                mScanCommonVirus;
    bool                mScanIntelligent;
    bool                mScanCompressFile;
    VIRUS_DEAL          mVirusAction;
    CLEAN_FAILED        mCleanFailedAction;
    VIRUS_BACKUP_FAILED mBackupFailedAction;
    VIRUS_BACKUP         mBackupVirusFile;
    LARGE_FILE          mLargeVirusAction;
    SPACE_LESS          mSpaceLessAction;
    int                 mScanCommonType;
    int                 mCompressSize;
    int                 mCPUPayloadPercent;
    int                 mThreadCount;
    bool                mUseCache;
    bool                mFinishNotify;
    bool                mIntelligentMode;
    vector<CloudConfig> mCloudAddr;
    bool                mCloudEnable;
    string              mCloudUrl;

public:
    ScanConfig() {
        mScanFileType = ALL_FILE;
        mScanCommonVirus = false;
        mScanIntelligent = false;
        mScanCompressFile = false;
        mVirusAction = AUTO_DEAL;
        mCleanFailedAction = CLEAN_FAILED_IGNORE;
        mBackupFailedAction = BACKUP_FAILED_QUERY;
        mBackupVirusFile = BACKUP_ORIGIN;
        mSpaceLessAction = OVERRIDE_OLD;
        mLargeVirusAction = LARGE_FILE_QUERY;
        mCompressSize = 500;
        mCPUPayloadPercent = 80;
        mThreadCount = 1;
        mUseCache = false;
        mFinishNotify = true;
        mCloudAddr.clear();
        mIntelligentMode = true;
        mCloudEnable = false;
    }

public:
    void operator = (const ScanConfig& config) {
        mVirusAction = config.mVirusAction;
        mCleanFailedAction = config.mCleanFailedAction;
        mBackupFailedAction = config.mBackupFailedAction;
        mBackupVirusFile = config.mBackupVirusFile;
        mLargeVirusAction = config.mLargeVirusAction;
        mScanFileType = config.mScanFileType;
        mScanCommonVirus = config.mScanCommonVirus;
        mScanIntelligent = config.mScanIntelligent;
        mScanCompressFile = config.mScanCompressFile;
        mSpaceLessAction = config.mSpaceLessAction;
        mCompressSize = config.mCompressSize;
        mCPUPayloadPercent = config.mCPUPayloadPercent;
        mThreadCount = config.mThreadCount;
        mUseCache = config.mUseCache;
        mCloudAddr = config.mCloudAddr;
        mIntelligentMode = config.mIntelligentMode;
        mCloudEnable = config.mCloudEnable;
        mCloudUrl = config.mCloudUrl;
    }

private:
    void InitializeJson(Json::Value& json) {
        mScanFileType = (SCAN_TYPE)json["scan"].asInt();
        mScanCommonVirus = json["populate"].asInt();
        mScanIntelligent = json["scanintelligent"].asInt();
        mScanCompressFile = json["scancompress"].asInt();
        mVirusAction = (VIRUS_DEAL)json["virus"].asInt();
        mCleanFailedAction = (CLEAN_FAILED)json["cleanf"].asInt();
        mBackupFailedAction = (VIRUS_BACKUP_FAILED)json["backupf"].asInt();
        mBackupVirusFile = (VIRUS_BACKUP)json["backupv"].asInt();
        mLargeVirusAction = (LARGE_FILE)json["large"].asInt();
        mSpaceLessAction = (SPACE_LESS)json["space"].asInt();
        mCompressSize = json["compress"].asInt();
        mCPUPayloadPercent = json["cpu"].asInt();
        mThreadCount = json["thread"].asInt();
        mIntelligentMode = json["intelligent"].asInt();

        Json::Value cloud = json["cloud"];
        for (int i=0; i<cloud.size(); ++i) {
            Json::Value child = cloud[i];
            CloudConfig config;
            config.InitializeJson(child);
            mCloudAddr.push_back(config);
        }
    }

    void ExternlizeJson(Json::Value& json) {
        json["scan"] = Json::Value(mScanFileType);
        json["populate"] = Json::Value(mScanCommonVirus);
        json["scanintelligent"] = Json::Value(mScanIntelligent);
        json["scancompress"] = Json::Value(mScanCompressFile);
        json["virus"] = Json::Value(mVirusAction);
        json["cleanf"] = Json::Value(mCleanFailedAction);
        json["backupf"] = Json::Value(mBackupFailedAction);
        json["backupv"] = Json::Value(mBackupVirusFile);
        json["large"] = Json::Value(mLargeVirusAction);
        json["space"] = Json::Value(mSpaceLessAction);
        json["compress"] = Json::Value(mCompressSize);
        json["cpu"] = Json::Value(mCPUPayloadPercent);
        json["thread"] = Json::Value(mThreadCount);
        json["intelligent"] = Json::Value(mIntelligentMode);

        Json::Value cloud;
        vector<CloudConfig>::iterator iter;
        for (iter=mCloudAddr.begin(); iter!=mCloudAddr.end(); ++iter) {
            Json::Value child;
            iter->ExternlizeJson(child);
            cloud.append(child);
        }
        json["cloud"] = Json::Value(cloud);
    }
};

#endif // SCANCONFIG_H
