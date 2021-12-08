#include "policysyncparser.h"
#include "commonapi.h"
#include "storage.h"
#include "scanconfig.h"

#include "Entry/whitelistentry.h"
#include "Entry/scheduleentry.h"
#include "Entry/sourcelistentry.h"

#define CONFIG_STRING_VALUE(node, value) do {\
                                            value.Parse(node);\
                                            xmlChar *data = xmlNodeGetContent(node);\
                                            value.mValue = (char*)data;\
                                            xmlFree(data); \
                                         } while(0);

#define CONFIG_INTEGER_VALUE(node, value) do {\
                                            value.Parse(node);\
                                            xmlChar *data = xmlNodeGetContent(node);\
                                            value.mValue = atoi((char*)data);\
                                            xmlFree(data); \
                                          } while(0);

#define CONFIG_NODE_VALUE(node, value)    do {\
                                            value.Parse(node);\
                                            value.mValue.Parse(node);\
                                          } while(0);

#define XML_NODE_INT_VALUE(node, value) do {\
                                                 xmlChar *data = xmlNodeGetContent(node); \
                                                 if (data) {\
                                                      value = atoi((char*)data); \
                                                 }\
                                                 xmlFree(data);\
                                              }while(0);

#define XML_NODE_STRING_VALUE(node, value) do {\
                                                 xmlChar *data = xmlNodeGetContent(node); \
                                                 value = (char*)data;\
                                                 xmlFree(data);\
                                              }while(0);

#define XML_PROP_INT_VALUE(node, name, value) do {\
                                                 xmlChar *data = xmlGetProp(node, BAD_CAST(name)); \
                                                 if (data) {\
                                                      value = atoi((char*)data); \
                                                 }\
                                                 xmlFree(data);\
                                              }while(0);

#define XML_PROP_STRING_VALUE(node, name, value) do {\
                                                 xmlChar *data = xmlGetProp(node, BAD_CAST(name)); \
                                                 value = (char*)data;\
                                                 xmlFree(data);\
                                              }while(0);


void TimeValue::Parse(xmlNodePtr node) {
    if (!xmlHasProp(node, BAD_CAST"time")) {
        return ;
    }

    string time;
    XML_PROP_STRING_VALUE(node, "time", time);

    vector<string> datas;
    Utils::String::Split(time, "|", &datas);
    if (datas.size() > 2) {
        // week
        mMode = (enum UpdateMode)atoi(datas[0].c_str());
        mTime = datas[2];

        int week = atoi(datas[1].c_str());
        for (int i=0; i<7; i++) {
            if (week>>i & 0x1) {
                mWeeks.push_back(i);
            }
        }
    }
    else if (datas.size() > 1) {
        // days
        mMode = (enum UpdateMode)atoi(datas[0].c_str());
        mTime = datas[1];
    }
    else {
        mMode = (enum UpdateMode)atoi(datas[0].c_str());
    }
}



void GlobalAntiVirusPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"pub",ptr->name) == 0) {
            mPublicPolicy.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"scan",ptr->name) == 0) {
            mScanPolicy.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"filemon",ptr->name) == 0) {
            mFileMonitor.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"mailmon",ptr->name) == 0) {
            mMailMonitor.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"sharemon",ptr->name) == 0) {
            mShareMonitor.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"defmon",ptr->name) == 0) {
            mDefenMonitor.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"actanalyze",ptr->name) == 0) {
            mActionAnalyze.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"devmon",ptr->name) == 0) {
            mDeviceMonitor.Parse(ptr);
        }
    }
}

void GlobalAntiVirusPolicy::ItemData::Parse(xmlNodePtr node) {
    if (xmlHasProp(node, BAD_CAST"path")) {
        XML_PROP_STRING_VALUE(node, "path", mPath);
    }
    XML_NODE_STRING_VALUE(node, mValue);
}

void GlobalAntiVirusPolicy::PublicPolicy::WhiteList::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"ext",ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mExtType);
        }
        else if(xmlStrcmp(BAD_CAST"file",ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST"item",child->name) == 0) {
                    ItemData item;
                    item.Parse(child);

                    if (!item.mValue.empty()) {
                        mFileList.push_back(item);
                    }
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::PublicPolicy::WhiteList::SyncStorage(Storage *storage) {
    // delete the old
    list<WhiteListEntry> origin;
    WhiteListEntry seed;
    seed.mListType = WhiteListEntry::WHITE;
    seed.mBlockType = WhiteListEntry::FILE_SCAN;
    seed.mPathType = WhiteListEntry::UNSET;
    if (storage->GetWhiteList(origin, seed) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=origin.begin(); iter!=origin.end(); ++iter) {
            if (iter->mReserved1 == 1) {
                storage->DeleteWhiteList(*iter);
            }
        }
    }

    if (!mExtType.empty()) {
        vector<string> exts;
        Utils::String::Split(mExtType, ";", &exts);

        vector<string>::iterator iter;
        for (iter=exts.begin(); iter!=exts.end(); ++iter) {
            WhiteListEntry entry;
            entry.mListType = WhiteListEntry::WHITE;
            entry.mBlockType = WhiteListEntry::FILE_SCAN;
            entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
            entry.mPath = *iter;
            entry.mReserved1 = 1;

            storage->InsertWhiteList(entry);
        }
    }

    origin.clear();
    if (storage->GetWhiteList(origin, WhiteListEntry::WHITE) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=origin.begin(); iter!=origin.end(); ++iter) {
            if (iter->mReserved1 == 1) {
                storage->DeleteWhiteList(*iter);
            }
        }
    }

    vector<ItemData>::iterator iter;
    for (iter=mFileList.begin(); iter!=mFileList.end(); ++iter) {
        if (iter->mValue.find(":") != string::npos) {
            // maybe it is a windows path, ignore
            continue;
        }
        WhiteListEntry entry;
        entry.mListType = WhiteListEntry::WHITE;
        entry.mBlockType = WhiteListEntry::FILE_SCAN;

        int type = atoi(iter->mPath.c_str());
        if (type == 4) {
            type = 0;
        }
        entry.mPathType = (WhiteListEntry::PATH_TYPE)type;
        entry.mPath = iter->mValue;
        entry.mReserved1 = 1;

        storage->InsertWhiteList(entry);
    }

    return 0;
}

void GlobalAntiVirusPolicy::PublicPolicy::ListUserLock::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"lockmode",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLockMode);
        }
        else if(xmlStrcmp(BAD_CAST"custompwd",ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mCustomPasswd);
        }
    }
}

void GlobalAntiVirusPolicy::PublicPolicy::BlackList::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST"file",ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST"item",child->name) == 0) {
                    ItemData item;
                    item.Parse(child);

                    if (!item.mValue.empty()) {
                        mFileList.push_back(item);
                    }
                }
            }
        }
        if(xmlStrcmp(BAD_CAST"hash",ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST"item",child->name) == 0) {
                    ItemData item;
                    item.Parse(child);

                    if (!item.mValue.empty()) {
                        mFileList.push_back(item);
                    }
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::PublicPolicy::BlackList::SyncStorage(Storage *storage) {
    // delete the old
    list<WhiteListEntry> origin;
    if (storage->GetWhiteList(origin, WhiteListEntry::BLACK) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=origin.begin(); iter!=origin.end(); ++iter) {
            if (iter->mReserved1 == 1) {
                storage->DeleteWhiteList(*iter);
            }
        }
    }

    vector<ItemData>::iterator iter;
    for (iter=mFileList.begin(); iter!=mFileList.end(); ++iter) {
        WhiteListEntry entry;
        entry.mListType = WhiteListEntry::BLACK;
        entry.mBlockType = WhiteListEntry::FILE_SCAN;
        entry.mPathType = (WhiteListEntry::PATH_TYPE)atoi(iter->mPath.c_str());
        entry.mPath = iter->mValue;
        entry.mReserved1 = 1;

        storage->InsertWhiteList(entry);
    }

    origin.clear();
    WhiteListEntry seed;
    seed.mListType = WhiteListEntry::BLACK;
    seed.mBlockType = WhiteListEntry::FILE_SCAN;
    seed.mPathType = WhiteListEntry::FILE_HASH;
    if (storage->GetWhiteList(origin, seed) > 0) {
        list<WhiteListEntry>::iterator iter;
        for (iter=origin.begin(); iter!=origin.end(); ++iter) {
            if (iter->mReserved1 == 1) {
                storage->DeleteWhiteList(*iter);
            }
        }
    }
    for (iter=mHashList.begin(); iter!=mHashList.end(); ++iter) {
        WhiteListEntry entry;
        entry.mListType = WhiteListEntry::BLACK;
        entry.mBlockType = WhiteListEntry::FILE_SCAN;
        entry.mPathType = WhiteListEntry::FILE_HASH;
        entry.mReserved1 = 1;
        entry.mPath = iter->mValue;

        storage->InsertWhiteList(entry);
    }
    return 0;
}

void GlobalAntiVirusPolicy::PublicPolicy::CloudSetting::Server::Parse(xmlNodePtr node) {
    XML_PROP_INT_VALUE(node, "private", mPrivate)

    for(xmlNodePtr child = node->children; child; child = child->next) {
        if(xmlStrcmp(BAD_CAST"enable",child->name) == 0) {
            XML_NODE_INT_VALUE(child, mEnable)
        }
        else if(xmlStrcmp(BAD_CAST"addr",child->name) == 0) {
            XML_NODE_STRING_VALUE(child, mAddress)
        }
        else if(xmlStrcmp(BAD_CAST"port",child->name) == 0) {
            XML_NODE_INT_VALUE(child, mPort)
        }
        else if(xmlStrcmp(BAD_CAST"mode",child->name) == 0) {
            XML_NODE_INT_VALUE(child, mMode)
        }
        else if(xmlStrcmp(BAD_CAST"count",child->name) == 0) {
            XML_NODE_INT_VALUE(child, mCount)
        }
        else if(xmlStrcmp(BAD_CAST"name",child->name) == 0) {
            XML_NODE_STRING_VALUE(child, mName)
        }
    }
}

void GlobalAntiVirusPolicy::PublicPolicy::CloudSetting::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST"cpuradio",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mCPURadio);
        }
        else if(xmlStrcmp(BAD_CAST"connrate",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mConnectInterval);
        }
        else if(xmlStrcmp(BAD_CAST"servers",ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST"csrv",child->name) == 0) {
                    ConfigField<Server> setting;
                    CONFIG_NODE_VALUE(child, setting);
                    mServerSetting.push_back(setting);
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::PublicPolicy::CloudSetting::SyncStorage(Storage *storage) {
    storage->SetSettingValue(CPU_RADIO_ITEM, mCPURadio.mValue, mCPURadio.mAdmin);
    storage->SetSettingValue(CONNECT_RATE_ITEM, mConnectInterval.mValue, mConnectInterval.mAdmin);

    list<SourceListEntry> origin;
    SourceListEntry seed;
    seed.mType = SourceListEntry::CLOUD_SCAN_SOURCE;
    seed.mOffical = 1;
    if (storage->GetSourceList(origin, seed)) {
        list<SourceListEntry>::iterator iter;
        for (iter=origin.begin(); iter!=origin.end(); ++iter) {
            storage->DeleteSourceList(*iter);
        }
    }

    vector<ConfigField<Server> >::iterator iter;
    for (iter=mServerSetting.begin(); iter!=mServerSetting.end(); ++iter) {
        SourceListEntry entry;
        entry.mState = iter->mValue.mEnable;
        entry.mType = SourceListEntry::CLOUD_SCAN_SOURCE;
        entry.mName = iter->mValue.mName;
        entry.mAddress = iter->mValue.mAddress;
        entry.mPort = iter->mValue.mPort;
        entry.mOffical = 1;
        entry.mRequestModel = iter->mValue.mMode;
        entry.mOncePostfileCount = iter->mValue.mCount;

        storage->InsertSourceList(entry);

        if (!iter->mValue.mPrivate) {
            storage->SetSettingValue(PUBLIC_CLOUD_STATE, iter->mValue.mEnable, iter->mLocked);
        }
    }

    return 0;
}

void GlobalAntiVirusPolicy::PublicPolicy::VirusDealSetting::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST"nospace",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mSpaceLessSet);
        }
        else if(xmlStrcmp(BAD_CAST"storefailed",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mFailedSet);
        }
        else if(xmlStrcmp(BAD_CAST"bigfile",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mBigFileSet);
        }
        else if(xmlStrcmp(BAD_CAST"nobackup",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mBackupType);
        }
    }
}

int GlobalAntiVirusPolicy::PublicPolicy::VirusDealSetting::SyncStorage(Storage *storage) {
    storage->SetSettingValue(BACKUP_SPACE_LESS, mSpaceLessSet.mValue, mSpaceLessSet.mLocked);
    storage->SetSettingValue(BACKUP_FAIL_ACTION, mFailedSet.mValue, mFailedSet.mLocked);
    storage->SetSettingValue(LARGE_VIRUS_ACTION, mBigFileSet.mValue, mBigFileSet.mLocked);
    // remember, mBackupType get the nobackup value, so ...you know it
    storage->SetSettingValue(BACKUP_VIRUS_STATE, !mBackupType.mValue, mBackupType.mLocked);
    return 0;
}

void GlobalAntiVirusPolicy::PublicPolicy::SmartCPUSetting::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST"type",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mType);
        }
        else if(xmlStrcmp(BAD_CAST"level",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLevel);
        }
    }
}

int GlobalAntiVirusPolicy::PublicPolicy::SmartCPUSetting::SyncStorage(Storage *storage) {
    storage->SetSettingValue(SCAN_COMMON_TYPE, mType.mValue, mType.mLocked);
    storage->SetSettingValue(SCAN_CPU_RADIO, mLevel.mValue, mLevel.mLocked);
    return 0;
}

void GlobalAntiVirusPolicy::PublicPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST"whitelist",ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST"admin",child->name) == 0) {
                    CONFIG_NODE_VALUE(child, mWhiteList);
                }
                else if (xmlStrcmp(BAD_CAST"cancellocalwhite",child->name) == 0) {
                    ConfigField<string> temp;
                    CONFIG_STRING_VALUE(child, temp);
                    mWhiteList.mValue.mIgnoreLocalWhitelist.mLocked = temp.mLocked;
                    mWhiteList.mValue.mIgnoreLocalWhitelist.mValue = temp.mValue=="yes";
                }
                else if (xmlStrcmp(BAD_CAST"user", child->name) == 0) {
                    mWhiteList.mValue.mListLock.Parse(ptr);
                }
            }
        }
        else if (xmlStrcmp(BAD_CAST"blacklist",ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST"admin",child->name) == 0) {
                    CONFIG_NODE_VALUE(child, mBlackList);
                }
            }
        }
        else if (xmlStrcmp(BAD_CAST"cloud",ptr->name) == 0) {
            mCloudSetting.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST"vstore",ptr->name) == 0) {
            mVirusDealSet.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST"vtrack",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mVirusTrack);
        }
        else if (xmlStrcmp(BAD_CAST"writelog",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mWriteLog);
        }
        else if (xmlStrcmp(BAD_CAST"smartcpu",ptr->name) == 0) {
            mSmartCPUSet.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST"worm08067",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mWorm08067);
        }
        else if (xmlStrcmp(BAD_CAST"yunyu",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mYunYu);
        }
        else if (xmlStrcmp(BAD_CAST"lpktool",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLPKTool);
        }
        else if (xmlStrcmp(BAD_CAST"virut",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mVirusUt);
        }
        else if (xmlStrcmp(BAD_CAST"kvengine",ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mKVEngine);
        }
        else if (xmlStrcmp(BAD_CAST"closerfm",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mCloseRFM);
        }
        else if (xmlStrcmp(BAD_CAST"InnerWhiteList",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mInnerWhitelist);
        }
        else if (xmlStrcmp(BAD_CAST"ptcscan",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mThreadScanCommand);
        }
        else if (xmlStrcmp(BAD_CAST"enforcescan",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mStrongerKill);
        }
    }
}

int GlobalAntiVirusPolicy::PublicPolicy::SyncStorage(Storage *storage) {
    mWhiteList.mValue.SyncStorage(storage);
    mBlackList.mValue.SyncStorage(storage);
    mCloudSetting.SyncStorage(storage);
    mVirusDealSet.SyncStorage(storage);
    mSmartCPUSet.SyncStorage(storage);
    storage->SetSettingValue(VIRUS_TRACE, mVirusTrack.mValue, mVirusTrack.mLocked);
    storage->SetSettingValue(VIRUS_LOG_FIELD, mWriteLog.mValue, mWriteLog.mLocked);

    // rescan accelerate, use
    storage->SetSettingValue(RESCAN_ACCELERATE, !mCloseRFM.mValue, mCloseRFM.mLocked);
    storage->SetSettingValue(ENV_INTELLIGENT, mInnerWhitelist.mValue, mInnerWhitelist.mLocked);
    return 0;
}

void GlobalAntiVirusPolicy::EngineSet::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"pub",ptr->name) == 0) {
            for(xmlNodePtr cptr=ptr->children; cptr; cptr=cptr->next) {
                if(xmlStrcmp(BAD_CAST"filetype",cptr->name) == 0) {
                    CONFIG_INTEGER_VALUE(cptr, mFileType);
                }
                else if(xmlStrcmp(BAD_CAST"exts",cptr->name) == 0) {
                    XML_NODE_STRING_VALUE(cptr, mFileExts)
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST"killfailed",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mKillFailed);
        }
        else if(xmlStrcmp(BAD_CAST"id",ptr->name) == 0) {
            EnginConfig config;
            config.Parse(ptr);

            mEngine.insert(pair<string, EnginConfig>(config.mName, config));
        }
    }
}

int GlobalAntiVirusPolicy::EngineSet::SyncStorage(Storage *storage) {
    storage->SetScanFileType(mFileType.mValue, mFileType.mLocked);
    if (mFileType.mValue == SPECIAL_FILE_TYPE) {
        list<WhiteListEntry> files;
        WhiteListEntry entry;
        entry.mListType = WhiteListEntry::BLACK;
        entry.mBlockType = WhiteListEntry::FILE_SCAN;
        entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
        if (storage->GetWhiteList(files, entry) > 0) {
            list<WhiteListEntry>::iterator iter;
            for (iter=files.begin(); iter!=files.end(); ++iter) {
                storage->DeleteWhiteList(*iter);
            }
        }

        vector<string> exts;
        Utils::String::Split(mFileExts, ";", &exts);
        vector<string>::iterator it;
        for (it=exts.begin();it!=exts.end();++it) {
            WhiteListEntry entry;
            entry.mListType = WhiteListEntry::BLACK;
            entry.mBlockType = WhiteListEntry::FILE_SCAN;
            entry.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
            entry.mReserved1 = 1;
            entry.mPath = *it;
            storage->InsertWhiteList(entry);
        }
    }

    map<string, EnginConfig>::iterator iter;
    for (iter=mEngine.begin(); iter!=mEngine.end();++iter) {
        if (iter->first == "1") {
            iter->second.SyncStorage(storage);
        }
    }
    return 0;
}

void GlobalAntiVirusPolicy::EngineSet::EnginConfig::Parse(xmlNodePtr node) {
    if (xmlHasProp(node, BAD_CAST"name")) {
        XML_PROP_STRING_VALUE(node, "name", mName)
    }

    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"engid",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mEngineID)
        }
        else if(xmlStrcmp(BAD_CAST"enable",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mEnable)
        }
        else if(xmlStrcmp(BAD_CAST"cfg",ptr->name) == 0) {
            for(xmlNodePtr cptr=ptr->children; cptr; cptr=cptr->next) {
                if(xmlStrcmp(BAD_CAST"heuristic",cptr->name) == 0) {
                    CONFIG_INTEGER_VALUE(cptr, mHeuristic);
                }
                else if(xmlStrcmp(BAD_CAST"popvirus",cptr->name) == 0) {
                    CONFIG_INTEGER_VALUE(cptr, mPopVirus);
                }
                else if(xmlStrcmp(BAD_CAST"zip",cptr->name) == 0) {
                    for(xmlNodePtr grand = cptr->children; grand; grand = grand->next) {
                        if(xmlStrcmp(BAD_CAST"scanzip",grand->name) == 0) {
                            CONFIG_INTEGER_VALUE(grand, mScanZip);
                        }
                        else if(xmlStrcmp(BAD_CAST"filesize",grand->name) == 0) {
                            CONFIG_INTEGER_VALUE(grand, mZipMaxSize);
                        }
                        else if(xmlStrcmp(BAD_CAST"nestdepth",grand->name) == 0) {
                            XML_NODE_INT_VALUE(grand, mZipDepth)
                        }
                    }
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::EngineSet::EnginConfig::SyncStorage(Storage *storage) {
    storage->SetCommonVirusState(mPopVirus.mValue, mPopVirus.mLocked);
    storage->SetIntelligentScanState(mHeuristic.mValue, mHeuristic.mLocked);
    storage->SetCompressScanState(mScanZip.mValue, mScanZip.mLocked);
    storage->SetScanCompressFileSize(mZipMaxSize.mValue, mZipMaxSize.mLocked);
    storage->SetScanCompressFileDepth(mZipDepth, mZipMaxSize.mLocked);
    return 0;
}

void GlobalAntiVirusPolicy::ScanPolicy::TimeScan::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"enable",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mEnable)
        }
        else if(xmlStrcmp(BAD_CAST"RsTimer",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"Task",child->name) == 0) {
                    XML_PROP_STRING_VALUE(child, "id", mTaskID)
                    XML_PROP_INT_VALUE(child, "kind", mKind)
                    XML_PROP_INT_VALUE(child, "type", mType)
                    XML_PROP_STRING_VALUE(child, "msgid", mMsgID)

                    for(xmlNodePtr grand=child->children; grand; grand=grand->next) {
                        if(xmlStrcmp(BAD_CAST"Time",grand->name) == 0) {
                            XML_PROP_STRING_VALUE(grand, "startdate", mStartDate)
                            for(xmlNodePtr gg=grand->children; gg; gg=gg->next) {
                                if(xmlStrcmp(BAD_CAST"EveryWeek",gg->name) == 0) {
                                    string hour, minite;
                                    XML_PROP_STRING_VALUE(gg, "hour", hour)
                                    XML_PROP_STRING_VALUE(gg, "minute", minite)
                                    mTime.mTime = hour + ":" + minite;
                                    XML_PROP_INT_VALUE(gg, "number", mNumber)
                                    int week = 0;
                                    XML_PROP_INT_VALUE(gg, "weekmark", week)
                                    for (int i=0; i<7; i++) {
                                        if (week>>i & 0x1) {
                                            mTime.mWeeks.push_back(i);
                                        }
                                    }
                                }
                                else if (xmlStrcmp(BAD_CAST"EveryDay",gg->name) == 0) {
                                    string hour, minite;
                                    XML_PROP_STRING_VALUE(gg, "hour", hour)
                                    XML_PROP_STRING_VALUE(gg, "minute", minite)
                                    mTime.mTime = hour + ":" + minite;
                                    XML_PROP_INT_VALUE(gg, "number", mNumber)
                                }
                                else if (xmlStrcmp(BAD_CAST"EveryMonth",gg->name) == 0) {
                                    string day, hour, minite;
                                    XML_PROP_STRING_VALUE(gg, "day", day);
                                    XML_PROP_STRING_VALUE(gg, "hour", hour);
                                    XML_PROP_STRING_VALUE(gg, "minute", minite);

                                    mTime.mTime = hour + ":" + minite;
                                    mTime.mDay = atoi(day.c_str());
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST"Data",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"adminlock",child->name) == 0) {
                    XML_NODE_INT_VALUE(child, mLocked)
                }
                else if(xmlStrcmp(BAD_CAST"custom",child->name) == 0) {
                    for(xmlNodePtr grand=child->children; grand; grand=grand->next) {
                        if(xmlStrcmp(BAD_CAST"path",grand->name) == 0) {
                            for(xmlNodePtr gg=grand->children; gg; gg=gg->next) {
                                if(xmlStrcmp(BAD_CAST"enable",gg->name) == 0) {
                                    XML_NODE_INT_VALUE(gg, mEnablePath)
                                }
                                else if (xmlStrcmp(BAD_CAST"list",gg->name) == 0) {
                                    for(xmlNodePtr ggg=gg->children; ggg; ggg=ggg->next) {
                                        if (xmlStrcmp(BAD_CAST"item",ggg->name) == 0) {
                                            string tmp;
                                            XML_NODE_STRING_VALUE(ggg, tmp)
                                            mPaths.push_back(tmp);
                                        }
                                    }
                                }
                            }
                        }
                        else if(xmlStrcmp(BAD_CAST"ext",grand->name) == 0) {
                            for(xmlNodePtr gg=grand->children; gg; gg=gg->next) {
                                if(xmlStrcmp(BAD_CAST"enable",gg->name) == 0) {
                                    XML_NODE_INT_VALUE(gg, mEnableExt)
                                }
                                else if (xmlStrcmp(BAD_CAST"exts",gg->name) == 0) {
                                    XML_NODE_STRING_VALUE(gg, mExts)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GlobalAntiVirusPolicy::ScanPolicy::TimeScan::SyncModel(ScheduleEntry& entry) {
    if (!mEnable) {
        return;
    }

    entry.mType = (ScheduleEntry::ScheduleType)mType;
    if (mEnablePath) {
        std::list<string>::iterator iter;
        for (iter=mPaths.begin(); iter!=mPaths.end(); ++iter) {
            entry.mScanPath += *iter;
            entry.mScanPath += ";";
        }
    }
    if (mEnableExt) {
        entry.mScanExts = mExts;
    }
    switch(mType) {
    case 2: {
        entry.mType = ScheduleEntry::RESTART;
        break;
    }
    case 6: {
        vector<string> times;
        Utils::String::Split(mTime.mTime, ":", &times);

        entry.mType = ScheduleEntry::DAYS;
        entry.SetCrontabHour(atoi(times[0].c_str()));
        entry.SetCrontabMinute(atoi(times[1].c_str()));
        break;
    }
    case 5: {
        vector<string> times;
        Utils::String::Split(mTime.mTime, ":", &times);

        entry.mType = ScheduleEntry::WEEKS;
        entry.SetCrontabHour(atoi(times[0].c_str()));
        entry.SetCrontabMinute(atoi(times[1].c_str()));
        entry.SetCrontabDayInWeek(mTime.mWeeks);
        break;
    }
    case 4: {
        vector<string> times;
        Utils::String::Split(mTime.mTime, ":", &times);

        entry.mType = ScheduleEntry::MONTH;
        entry.SetCrontabHour(atoi(times[0].c_str()));
        entry.SetCrontabMinute(atoi(times[1].c_str()));
        entry.SetCrontabDayInMonth(mTime.mDay);
        break;
    }
    default: {
        break;
    }
    }
}

void GlobalAntiVirusPolicy::ScanPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"efficient",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mEfficient);
        }
        else if(xmlStrcmp(BAD_CAST"timerscan",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"allscan",child->name) == 0) {
                    CONFIG_NODE_VALUE(child, mAllScan);
                }
                else if(xmlStrcmp(BAD_CAST"quickscan",child->name) == 0) {
                    CONFIG_NODE_VALUE(child, mQuickScan);
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST"engine",ptr->name) == 0) {
            mEngineSet.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"findvirus",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mFindVirus);
        }
        else if(xmlStrcmp(BAD_CAST"killfailed",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mKillFailed)
        }
        else if(xmlStrcmp(BAD_CAST"adminscanoper",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mOperate);
        }
    }
}

int GlobalAntiVirusPolicy::ScanPolicy::SyncStorage(Storage *storage) {
    storage->SetSettingValue(EFFICIENT_ITEM, mEfficient.mValue, mEfficient.mAdmin);
    storage->SetSettingValue(SCAN_VIRUS_ACTION, mFindVirus.mValue, mFindVirus.mLocked);
    storage->SetSettingValue(CLEAN_FAIL_ACTION, mKillFailed, false);
    storage->SetSettingValue(ADMIN_OPERATOR_LOCK, mOperate.mValue, mOperate.mAdmin);

    mEngineSet.SyncStorage(storage);

    storage->SetSettingValue(TIME_QUICK_SCAN_FLAG, mQuickScan.mValue.mEnable, mQuickScan.mLocked);
    storage->SetSettingValue(TIME_FULL_SCAN_FLAG, mAllScan.mValue.mEnable, mAllScan.mLocked);

    map<ScheduleEntry::ScanType, ScheduleEntry> maps;

    if (mAllScan.mValue.mEnable) {
        ScheduleEntry entry;
        entry.mFuncEnum = ScheduleEntry::ALL_SCAN;
        mAllScan.mValue.SyncModel(entry);
        maps.insert(pair<ScheduleEntry::ScanType, ScheduleEntry>(ScheduleEntry::ALL_SCAN, entry));
    }
    if (mQuickScan.mValue.mEnable) {
        ScheduleEntry entry;
        entry.mFuncEnum = ScheduleEntry::QUICK_SCAN;
        mQuickScan.mValue.SyncModel(entry);
        maps.insert(pair<ScheduleEntry::ScanType, ScheduleEntry>(ScheduleEntry::QUICK_SCAN, entry));
    }

    storage->UpdateCommonCloudSetting(maps, ScheduleEntry::ALL_SCAN);
    storage->UpdateCommonCloudSetting(maps, ScheduleEntry::QUICK_SCAN);

    return 0;
}

void GlobalAntiVirusPolicy::FileMonitor::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"rundisable",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mRundisable);
        }
        else if(xmlStrcmp(BAD_CAST"smartblack",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mSmartBlock);
        }
        else if(xmlStrcmp(BAD_CAST"runmode",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mRunMode);
        }
        else if(xmlStrcmp(BAD_CAST"lockclose",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLockClose);
        }
        else if(xmlStrcmp(BAD_CAST"monmode",ptr->name) == 0) {
            mMonitMode.Parse(ptr);
            char *data = (char*)xmlNodeGetContent(ptr);
            mMonitMode.mValue = (int)(data[0] - 48);
            xmlFree(data);

            for(xmlNodePtr cp=ptr->children; cp; cp=cp->next) {
                if(xmlStrcmp(BAD_CAST "monexts", cp->name) == 0) {
                    for(xmlNodePtr exp=cp->children; exp; exp=exp->next) {
                        if(xmlStrcmp(BAD_CAST "type", exp->name) == 0) {
                            XML_NODE_INT_VALUE(exp, mMonitExtsMode.mValue)
                             mMonitExtsMode.mLocked = mMonitMode.mLocked;
                        }
                        else if(xmlStrcmp(BAD_CAST "exts", exp->name) == 0) {
                            XML_NODE_STRING_VALUE(exp, mMonitExts.mValue)
                            mMonitExts.mLocked = mMonitMode.mLocked;
                        }
                    }
                }
                else if(XML_ELEMENT_NODE != cp->type) {
                    continue;
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST"enablekernel",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mKernelState);
        }
        else if(xmlStrcmp(BAD_CAST"reportresult",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mReportResult);
        }
        else if(xmlStrcmp(BAD_CAST"findvirus",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mFindVirusType);
        }
        else if(xmlStrcmp(BAD_CAST"sigsource",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mSigSource);
        }
        else if(xmlStrcmp(BAD_CAST"killfailed",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mKillFailed)
        }
        else if(xmlStrcmp(BAD_CAST"ole",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mOLEMode);
        }
        else if(xmlStrcmp(BAD_CAST"engine",ptr->name) == 0) {
            mEngineSet.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST"sharedoc",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonitorShareDoc);
        }
        else if(xmlStrcmp(BAD_CAST"sharedir",ptr->name) == 0) {
            for(xmlNodePtr cp=ptr->children; cp; cp=cp->next) {
                if(xmlStrcmp(BAD_CAST "item", cp->name) == 0) {
                    char *value = (char*)xmlNodeGetContent(cp);
                    if (strlen(value) > 0) {
                        if (strchr(value, ':')) {
                            mFolders.push_back(value);
                        }
                    }
                    xmlFree(value);
                }
                else if(XML_ELEMENT_NODE != cp->type) {
                    continue;
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST"syscache",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mClearSysCache);
        }
    }
}

int GlobalAntiVirusPolicy::FileMonitor::SyncStorage(Storage *storage) {
    storage->SetSettingValue(FILE_MONITOR_POWERON, !mRundisable.mValue, mRundisable.mLocked);
    storage->SetSettingValue(FILE_INTELLIGENT_BL, mSmartBlock.mValue, mSmartBlock.mLocked);
    storage->SetSettingValue(MONITOR_KERNEL_STATE, mKernelState.mValue, mKernelState.mLocked);
    storage->SetSettingValue(MONITOR_WATCH_MODE, mMonitMode.mValue, mMonitMode.mLocked);
    storage->SetSettingValue(MONITOR_DEF_TYPE, mMonitExtsMode.mValue, mMonitExtsMode.mLocked);
    storage->SetSettingValue(MONITOR_DEF_EXTS, mMonitExts.mValue, mMonitExts.mLocked);
    storage->SetSettingValue(FILE_MONITOR_FILE_TYPE, mEngineSet.mFileType.mValue, mEngineSet.mFileType.mLocked);
    storage->SetSettingValue(FILE_MONITOR_FILE_EXTS, mEngineSet.mFileExts, mEngineSet.mFileType.mLocked);
    storage->SetSettingValue(FILE_EMBED_SCAN, mEngineSet.mFileExts, mEngineSet.mFileType.mLocked);
    storage->SetSettingValue(FILE_MONITOR_KILL_PROMPT, mReportResult.mValue, mReportResult.mLocked);
    storage->SetSettingValue(FILE_MONITOR_ACCELERATE, mSigSource.mValue, mSigSource.mLocked);

    map<string, EngineSet::EnginConfig>::iterator iter = mEngineSet.mEngine.find("1");
    if (iter != mEngineSet.mEngine.end()) {
        storage->SetSettingValue(FILE_MONITOR_COMMON_VIRUS, iter->second.mPopVirus.mValue, iter->second.mPopVirus.mLocked);
        storage->SetSettingValue(FILE_MONITOR_SCAN_INTELLIGENT, iter->second.mHeuristic.mValue, iter->second.mHeuristic.mLocked);
        storage->SetSettingValue(FILE_MONITOR_SCAN_ZIP, iter->second.mScanZip.mValue, iter->second.mScanZip.mLocked);
        storage->SetSettingValue(FILE_MONITOR_ZIP_SIZE, iter->second.mZipMaxSize.mValue, iter->second.mZipMaxSize.mLocked);
        storage->SetSettingValue(FILE_MONITOR_ZIP_DEPTH, iter->second.mZipDepth, iter->second.mZipMaxSize.mLocked);
    }

    storage->SetSettingValue(FILE_MONITOR_SCAN_ZIP, iter->second.mScanZip.mValue, iter->second.mScanZip.mLocked);
    storage->SetSettingValue(FILE_MONITOR_ZIP_SIZE, iter->second.mZipMaxSize.mValue, iter->second.mZipMaxSize.mLocked);
    storage->SetSettingValue(FILE_MONITOR_ZIP_DEPTH, iter->second.mZipMaxSize.mValue, iter->second.mZipMaxSize.mLocked);
    storage->SetSettingValue(FILE_MONITOR_VIRUS, mFindVirusType.mValue, mFindVirusType.mLocked);
    storage->SetSettingValue(FILE_MONITOR_CLEAN_FAIL, mEngineSet.mKillFailed.mValue, mEngineSet.mKillFailed.mLocked);

    iter = mEngineSet.mEngine.find("2");
    if (iter != mEngineSet.mEngine.end()) {
        storage->SetSettingValue(FILE_MONITOR_USE_CLOUD, iter->second.mEnable, 0);
    }

    storage->SetSettingValue(FILE_MONITOR_DOC_FOLDER, mMonitorShareDoc.mValue, mMonitorShareDoc.mLocked);
    storage->SetSettingValue(FILE_MONITOR_CLEAR_CACHE, mClearSysCache.mValue, mClearSysCache.mLocked);

    return UpdateMonitorFolders(storage, mFolders);
}

int GlobalAntiVirusPolicy::FileMonitor::UpdateMonitorFolders(Storage *storage, std::list<string>& folders) {
    WhiteListEntry entry;
    entry.mListType = WhiteListEntry::BLACK;
    entry.mBlockType = WhiteListEntry::FOLDER_MONITOR;
    entry.mPathType = WhiteListEntry::CENTER_MONITOR_FOLDER;
    entry.mReserved1 = 1;

    std::list<WhiteListEntry> exists;
    std::map<string, int> emaps;
    if(storage->GetWhiteList(exists, entry) > 0) {
        std::list<WhiteListEntry>::iterator iter;
        for (iter=exists.begin(); iter!=exists.end(); ++iter) {
            emaps.insert(make_pair(iter->mPath, iter->mID));
        }
    }

    std::map<string, int> dmaps;
    std::list<string>::iterator iter;
    for (iter=folders.begin(); iter!=folders.end(); ++iter) {
        dmaps.insert(make_pair(*iter, 0));
        if (emaps.find(*iter) == emaps.end()) {
            WhiteListEntry entry;
            entry.mListType = WhiteListEntry::BLACK;
            entry.mBlockType = WhiteListEntry::FOLDER_MONITOR;
            entry.mPathType = WhiteListEntry::CENTER_MONITOR_FOLDER;
            entry.mReserved1 = 1;
            entry.mPath = *iter;
            storage->InsertWhiteList(entry);
        }
    }
    {
        std::list<WhiteListEntry>::iterator iter;
        for (iter=exists.begin(); iter!=exists.end(); ++iter) {
            if (dmaps.find(iter->mPath) == dmaps.end()) {
                storage->DeleteWhiteList(*iter);
            }
        }
    }
    return 0;
}

void GlobalAntiVirusPolicy::MailMonitor::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"rundisable",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mRundisable);
        }
        else if(xmlStrcmp(BAD_CAST"lockclose",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLockClose);
        }
        else if(xmlStrcmp(BAD_CAST"reportresult",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mReportResult);
        }
        else if(xmlStrcmp(BAD_CAST"findvirus",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mFindVirusType);
        }
        else if(xmlStrcmp(BAD_CAST"engine",ptr->name) == 0) {
            mEngineSet.Parse(ptr);
        }
    }
}

int GlobalAntiVirusPolicy::MailMonitor::SyncStorage(Storage *storage) {
    return 0;
}



void GlobalAntiVirusPolicy::ShareMonitor::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"rundisable",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mRundisable);
        }
        else if(xmlStrcmp(BAD_CAST"reportresult",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mReportResult);
        }
        else if(xmlStrcmp(BAD_CAST"findvirus",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mFindVirusType);
        }
        else if(xmlStrcmp(BAD_CAST"engine",ptr->name) == 0) {
            mEngineSet.Parse(ptr);
        }
    }
}

int GlobalAntiVirusPolicy::ShareMonitor::SyncStorage(Storage *storage) {
    return 0;
}

void GlobalAntiVirusPolicy::DefenMonitor::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"sysdef",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"enable",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mEnabled);
                }
                else if(xmlStrcmp(BAD_CAST"notify",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mNotified);
                }
                else if(xmlStrcmp(BAD_CAST"needlog",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mNeedlog);
                }
                else if(xmlStrcmp(BAD_CAST"level",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mLevel);
                }
                else if(xmlStrcmp(BAD_CAST"auditmode",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mAuditMode);
                }
                else if(xmlStrcmp(BAD_CAST"digitalsignature",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mDigtSign);
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::DefenMonitor::SyncStorage(Storage *storage) {
    return 0;
}

void GlobalAntiVirusPolicy::ActionAnalyze::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"apt",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"status",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mStatue);
                }
                else if(xmlStrcmp(BAD_CAST"notify",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mNotify);
                }
                else if(xmlStrcmp(BAD_CAST"deal",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mDealType);
                }
                else if(xmlStrcmp(BAD_CAST"log",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mLog);
                }
                else if(xmlStrcmp(BAD_CAST"starttip",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mStartTip);
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::ActionAnalyze::SyncStorage(Storage *storage) {
    return 0;
}

void GlobalAntiVirusPolicy::DeviceMonitor::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr=node->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"usbmon",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"scanlevel",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mScanLevel);
                }
                else if(xmlStrcmp(BAD_CAST"enable",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mEnabled);
                }
                else if(xmlStrcmp(BAD_CAST"askuser",child->name) == 0) {
                    CONFIG_INTEGER_VALUE(child, mAskUser);
                }
            }
        }
    }
}

int GlobalAntiVirusPolicy::DeviceMonitor::SyncStorage(Storage *storage) {
    storage->SetUSBPluginMonitorSate(mEnabled.mValue, mEnabled.mLocked);
    storage->SetSettingValue(U_PLUGIN_INSERT, mAskUser.mValue, mAskUser.mLocked);
    storage->SetSettingValue(U_PLUGIN_SCAN_DEPTH, mScanLevel.mValue, mScanLevel.mLocked);
    return 0;
}

int GlobalAntiVirusPolicy::SyncStorage(Storage *storage) {
    mPublicPolicy.SyncStorage(storage);
    mScanPolicy.SyncStorage(storage);
    mFileMonitor.SyncStorage(storage);
    mMailMonitor.SyncStorage(storage);
    mShareMonitor.SyncStorage(storage);
    mDefenMonitor.SyncStorage(storage);
    mActionAnalyze.SyncStorage(storage);
    mDeviceMonitor.SyncStorage(storage);

    return 0;
}


int ConfigPolicy::NetConfig::SyncStorage(Storage *storage) {
    mUpdateTime.SyncStorage(storage);
    mProxySetting.SyncStorage(storage);
    mSpeedLimit.mValue.SyncStorage(storage);
    mForceUpdate.mValue.SyncStorage(storage);
//    storage->SetSettingValue(UPDATE_NET_BAND_LIMIT, mNetDataLimit, false);
//    storage->SetSettingValue(UPDATE_NET_LIMIT_START, mLimitStartTime, false);
//    storage->SetSettingValue(UPDATE_NET_LIMIT_END, mLimitEndTime, false);
    storage->SetSettingValue(UPDATE_PROXY_ACTION, mNetType.mValue, mNetType.mLocked);
    return 0;
}

int ConfigPolicy::SyncStorage(Storage *storage) {
    mNetConfig.SyncStorage(storage);
    mClearDisk.SyncStorage(storage);
    mConnectType.mValue.SyncStorage(storage);
    mLinkConfig.mValue.SyncStorage(storage);
    storage->SetSettingValue(UPDATE_MAX_SPEED_KB, mMaxSpeed.mValue, mMaxSpeed.mLocked);
    storage->SetSettingValue(UPDATE_DOWN_THREAD, mDownloadThreadCount.mValue, mDownloadThreadCount.mLocked);

    return 0;
}

int ConfigPolicy::ConnectType::SyncStorage(Storage *storage) {
    list<SourceListEntry> entries;
    SourceListEntry seed;
    seed.mType = SourceListEntry::CLOUD_UPDATE_SOURCE;
    seed.mOffical = 1;
    if (storage->GetSourceList(entries, seed) > 0) {
        list<SourceListEntry>::iterator iter;
        for (iter=entries.begin(); iter!=entries.end(); ++iter) {
            storage->DeleteSourceList(*iter);
        }
    }

    map<string, Item>::iterator iter;
    for (iter=mValue.begin();iter!=mValue.end();++iter) {
        SourceListEntry entry;
        entry.mState = true;
        entry.mOffical = 1;
        entry.mName = iter->second.mType;
        entry.mType = SourceListEntry::CLOUD_UPDATE_SOURCE;
        entry.mAddress = iter->second.mPath;
        storage->InsertSourceList(entry);
    }
    return 0;
}

void ConfigPolicy::SubProduct::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "subproduct",ptr->name) == 0) {
            int state = 0;
            if (xmlHasProp(ptr, BAD_CAST"state")) {
                xmlChar *data = xmlGetProp(ptr, BAD_CAST"state");
                if (strlen((char*)data) > 0) {
                    state = atoi((char*)data);
                }
                xmlFree(data);
            }
            string sub;
            XML_NODE_STRING_VALUE(ptr, sub)
            mSubProductMap.insert(make_pair(sub, state));
        }
    }
}

void ConfigPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "connect",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mConnectType);
        }
        else if(xmlStrcmp(BAD_CAST "netconfig",ptr->name) == 0) {
            mNetConfig.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "lnk",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mLinkConfig);
        }
        else if(xmlStrcmp(BAD_CAST "subproductlist",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mSubProducts);
        }
        else if(xmlStrcmp(BAD_CAST "maxspeed",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMaxSpeed);
        }
        else if(xmlStrcmp(BAD_CAST "count",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mDownloadThreadCount);
        }
        else if(xmlStrcmp(BAD_CAST "cleardisk",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mClearDisk);
        }
    }
}

void ConfigPolicy::NetConfig::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "nettype",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mNetType);
        }
        else if(xmlStrcmp(BAD_CAST "proxy",ptr->name) == 0) {
            mProxySetting.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "update",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mUpdateTime);
        }
        else if(xmlStrcmp(BAD_CAST "datalimit",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mNetDataLimit)
        }
        else if(xmlStrcmp(BAD_CAST "begintime",ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mLimitStartTime)
        }
        else if(xmlStrcmp(BAD_CAST "endtime",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mLimitEndTime)
        }
        else if(xmlStrcmp(BAD_CAST "speedlimit",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mSpeedLimit)
        }
        else if(xmlStrcmp(BAD_CAST "forceupdate",ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mForceUpdate)
        }
    }
}

void ConfigPolicy::ProxySetting::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "ip",ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mIP);
        }
        else if (xmlStrcmp(BAD_CAST "port",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mPort);
        }
        else if (xmlStrcmp(BAD_CAST "Authentication",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mAuth);
        }
        else if (xmlStrcmp(BAD_CAST "username",ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mUserName);
        }
        else if (xmlStrcmp(BAD_CAST "password",ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mPasswd);
        }
    }
}

int ConfigPolicy::ProxySetting::SyncStorage(Storage *storage) {
    storage->SetSettingValue(UPDATE_PROXY_ADDR, mIP.mValue, mIP.mLocked);
    storage->SetSettingValue(UPDATE_PROXY_PORT, mPort.mValue, mPort.mLocked);
    storage->SetSettingValue(UPDATE_PROXY_ACCOUNT, mUserName.mValue, mUserName.mLocked);
    storage->SetSettingValue(UPDATE_PROXY_PASSWD, mPasswd.mValue, mPasswd.mLocked);
    storage->SetSettingValue(UPDATE_PROXY_AUTH, mAuth.mValue, mAuth.mLocked);
    return 0;
}

void ConfigPolicy::UpdateTime::Parse(xmlNodePtr node) {
    CONFIG_NODE_VALUE(node, mValue);

    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "type",ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr,  mType);
        }
        else if(xmlStrcmp(BAD_CAST "MaxDelay",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr,  mMaxDelay);
        }
        else if(xmlStrcmp(BAD_CAST "virslib",ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr,  mUpdateVirlib);
        }
    }
}

int ConfigPolicy::UpdateTime::SyncStorage(Storage *storage) {
    if (mValue.mValue.mMode < TimeValue::POLICY_UPDATE_MAUALLY) {
        ScheduleEntry entry;
        entry.mFuncEnum = ScheduleEntry::UPDATE;

        vector<string> times;
        Utils::String::Split(mValue.mValue.mTime, ":", &times);
        if (times.size() != 2) {
            return -1;
        }
        entry.SetCrontabHour(atoi(times[0].c_str()));
        entry.SetCrontabMinute(atoi(times[1].c_str()));
        if (mValue.mValue.mMode == TimeValue::POLICY_UPDATE_DAYS) {
            entry.mType = ScheduleEntry::DAYS;
        }
        else if (mValue.mValue.mMode == TimeValue::POLICY_UPDATE_WEEKS) {
            entry.mType = ScheduleEntry::WEEKS;
            entry.SetCrontabDayInWeek(mValue.mValue.mWeeks);
        }
        storage->SetScheduleEntry(entry);
    }
    else if (mValue.mValue.mMode == TimeValue::POLICY_UPDATE_INTERVAL) {

    }
    else {
        storage->DeleteScheduleEntry(ScheduleEntry::UPDATE);
    }

    int updateContent = 0;
    if (mType.mValue == "all") {
        updateContent = 1;
    }
    else {
        updateContent = 0;
    }

    storage->SetSettingValue(UPDATE_MODE, mValue.mValue.mMode);
    storage->SetSettingValue(UPDATE_CONTENT_ACTION, updateContent, mType.mLocked);
    if (mMaxDelay.mValue > 0) {
        storage->SetSettingValue(UPDATE_DELAY_START, 2, mMaxDelay.mLocked);
        storage->SetSettingValue(UPDATE_DELAY_TIME, mMaxDelay.mValue, mMaxDelay.mLocked);
    }
    else if (mMaxDelay.mValue == 0) {
        storage->SetSettingValue(UPDATE_DELAY_START, 0, mMaxDelay.mLocked);
    }
    else if (mMaxDelay.mValue == -1) {
        storage->SetSettingValue(UPDATE_DELAY_START, 1, mMaxDelay.mLocked);
    }
    storage->SetSettingValue(VIRUS_LIB_UPDATE_INTIME, mUpdateVirlib.mValue, mUpdateVirlib.mLocked);

    return 0;
}


void ConfigPolicy::SpeedLimit::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "enable",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr,  mEnable);
        }
        else if(xmlStrcmp(BAD_CAST "speed",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr,  mSpeedKB);
        }
        else if(xmlStrcmp(BAD_CAST "begintime",ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr,  mBeginTime);
        }
        else if(xmlStrcmp(BAD_CAST "endtime",ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr,  mEndTime);
        }
    }
}

int ConfigPolicy::SpeedLimit::SyncStorage(Storage *storage) {
    storage->SetSettingValue(UPDATE_NET_BAND_LIMIT, mEnable);
    storage->SetSettingValue(UPDATE_BAND_WIDTH, mSpeedKB);
    storage->SetSettingValue(UPDATE_NET_LIMIT_START, mBeginTime);
    storage->SetSettingValue(UPDATE_NET_LIMIT_END, mEndTime);
    return 0;
}

void ConfigPolicy::ForceUpdate::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "enable",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr,  mEnable);
        }
        else if(xmlStrcmp(BAD_CAST "minday",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr,  mMiniDay);
        }
    }
}

int ConfigPolicy::ForceUpdate::SyncStorage(Storage *storage) {
    storage->SetSettingValue(UPDATE_FORCE_SWITCH, mEnable);
    storage->SetSettingValue(UPDATE_FORCE_DELAY_DAY, mMiniDay);
    return 0;
}

void ConfigPolicy::ConnectType::Item::Parse(xmlNodePtr node) {
    XML_PROP_STRING_VALUE(node, "name", mName)
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "type",ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mType)
        }
        else if (xmlStrcmp(BAD_CAST "path",ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mPath)
        }
    }
}

void ConfigPolicy::ConnectType::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        if(xmlStrcmp(BAD_CAST "item",ptr->name) == 0) {
            Item item;
            item.Parse(ptr);
            mValue.insert(pair<string, Item>(item.mName, item));
        }
    }
}

void ConfigPolicy::ShortLinkType::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "menu",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mStartMenuLink)
        }
        else if(xmlStrcmp(BAD_CAST "desktop",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mDesktopIcon)
        }
    }
}

int ConfigPolicy::ShortLinkType::SyncStorage(Storage *storage) {
    storage->SetSettingValue(DEPLOY_START_MENU, mStartMenuLink, false);
    storage->SetSettingValue(DEPLOY_DESKTOP_ICON, mDesktopIcon, false);
    return 0;
}


void ConfigPolicy::ClearDisk::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "enable",ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mEnable)
        }
    }
}

int ConfigPolicy::ClearDisk::SyncStorage(Storage *storage, bool locked) {
    storage->SetSettingValue(UPDATE_CLEAR_SYS_CACHE, mEnable, locked);
    return 0;
}

int PolicyParser::ParseConfigPolicy(const string& filepath, ConfigPolicy& policy) {
    xmlDocPtr doc = xmlReadFile(filepath.c_str(), NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int PolicyParser::ParseConfigPolicy(const char *content, ConfigPolicy& policy) {
    xmlDocPtr doc = xmlReadMemory(content, strlen(content), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int PolicyParser::ParseGlobalPolicy(const string& filepath, GlobalAntiVirusPolicy& policy) {
    xmlDocPtr doc = xmlReadFile(filepath.c_str(), NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}



int PolicyParser::ParseGlobalPolicy(const char *content, GlobalAntiVirusPolicy &policy) {
    xmlDocPtr doc = xmlReadMemory(content, strlen(content), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}


int PolicyParser::ParseEnvrionPolicy(const char *content, EnvironmentalPolicy &policy) {
    xmlDocPtr doc = xmlReadMemory(content, strlen(content), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int PolicyParser::ParseFirewallPolicy(const char * content, FirewallPolicy& policy) {
    xmlDocPtr doc = xmlReadMemory(content, strlen(content), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}
int PolicyParser::ParseFirewallPolicy(const string& filepath, FirewallPolicy& policy) {
    xmlDocPtr doc = xmlReadFile(filepath.c_str(), NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    policy.Parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int PolicyParser::ParseCommandMessage(const string& content, CommandMessage& message) {
    xmlDocPtr doc = xmlReadMemory(content.c_str(), content.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr node = xmlDocGetRootElement(doc);
    message.Parse(node);

    return 0;
}

int PolicyParser::ParseCommandUpdate(const string& content, UpdateCommand& update) {
    xmlDocPtr doc = xmlReadMemory(content.c_str(), content.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr node = xmlDocGetRootElement(doc);
    update.Parse(node);

    return 0;
}

int PolicyParser::ParseCommandExecute(const string& content, ExecuteCommand &execute){
    xmlDocPtr doc = xmlReadMemory(content.c_str(), content.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr node = xmlDocGetRootElement(doc);
    execute.Parse(node);
    execute.mFile=Utils::Hash::Base64Decode(execute.mFile);
    return 0;
}

int PolicyParser::ParseLinuxPolicy(const string& content, PolicyScanModel& model) {
    int res = 0;

    xmlDocPtr  doc = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr child = NULL;
    xmlNodePtr ptr = NULL;
    xmlNodePtr grandson = NULL;

    doc = xmlReadMemory(content.c_str(), content.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }

    root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    for(child = root->children; child; child = child->next) {
        if (XML_ELEMENT_NODE != child->type) {
            continue;
        }

        if(xmlStrcmp(BAD_CAST "scanpath",child->name) == 0) {
            xmlChar * value = xmlNodeGetContent(child);
            if(NULL == value) {
                res = -3;
                goto OVER;
            }
            else {
                model.mScanPath = (char*)value;
                xmlFree(value);
            }
        }
        else if(xmlStrcmp(BAD_CAST "excludepath",child->name) == 0) {
            for(ptr = child->children; ptr; ptr = ptr->next) {
                if(XML_ELEMENT_NODE != ptr->type) {
                    continue;
                }
                if(xmlStrcmp(BAD_CAST "enable",ptr->name) == 0) {
                    xmlChar * value = xmlNodeGetContent(ptr);
                    if(NULL == value) {
                        res = -3;
                        goto OVER;
                    }
                    else {
                        // print the exclude path count
                        model.mExcludePaths.mState = atoi((char*)value);
                        xmlFree(value);
                    }
                }
                else if(xmlStrcmp(BAD_CAST "excludelist",ptr->name) == 0) {
                    xmlChar * value = xmlNodeGetContent(ptr);
                    if(NULL == value) {
                        res = -3;
                        goto OVER;
                    }
                    else {
                        // print the exclude path
                        model.mExcludePaths.mPaths.push_back((char*)value);
                        xmlFree(value);
                    }
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST "scanpolicy",child->name) == 0) {
            for(ptr = child->children; ptr; ptr = ptr->next) {
                if(XML_ELEMENT_NODE != ptr->type) {
                    continue;
                }
                if(xmlStrcmp(BAD_CAST "keepday",ptr->name) == 0) {
                    for(grandson = ptr->children; grandson; grandson = grandson->next) {
                        if(xmlStrcmp(BAD_CAST "enable",grandson->name) == 0) {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value) {
                                res = -3;
                                goto OVER;
                            }
                            else {
                                model.mScanPoliy.mKeepDays.mState = atoi((char *)value);
                                xmlFree(value);
                            }
                        }
                        else if(xmlStrcmp(BAD_CAST "keep",grandson->name) == 0) {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value) {
                                res = -3;
                                goto OVER;
                            }
                            else {
                                model.mScanPoliy.mKeepDays.mDays = time(NULL) - atoi((char *)value) * 3600 * 24;
                                xmlFree(value);
                            }
                        }
                    }
                }
                else if(xmlStrcmp(BAD_CAST "compressfile",ptr->name) == 0) {
                    for(grandson = ptr->children; grandson; grandson = grandson->next) {
                        if(xmlStrcmp(BAD_CAST "enable",grandson->name) == 0) {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value) {
                                res = -3;
                                goto OVER;
                            }
                            else {
                                model.mScanPoliy.mCompressSize.mState = atoi((char *)value);
                                xmlFree(value);
                            }
                        }
                        else if(xmlStrcmp(BAD_CAST "compressfilesize",grandson->name) == 0) {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value) {
                                res = -3;
                                goto OVER;
                            }
                            else {
                                model.mScanPoliy.mCompressSize.mSize = strtoul((char *)value, 0, 10);
                                xmlFree(value);
                            }
                        }
                    }
                }
                else if(xmlStrcmp(BAD_CAST "procmod",ptr->name) == 0)
                {
                    xmlChar * value = xmlNodeGetContent(ptr);
                    if(NULL == value)
                    {
                        res = -3;
                        goto OVER;
                    }
                    else
                    {
                        model.mScanPoliy.mDealVirusType = atoi((char *)value);
                        if(model.mScanPoliy.mDealVirusType == 2)
                            model.mScanPoliy.mBackupType.mState = 1;
                        else
                            model.mScanPoliy.mBackupType.mState = 0;
                        xmlFree(value);
                    }
                }
                else if(xmlStrcmp(BAD_CAST "deletefailmode",ptr->name) == 0)
                {
                    xmlChar * value = xmlNodeGetContent(ptr);
                    if(NULL == value)
                    {
                        res = -3;
                        goto OVER;
                    }
                    else
                    {
                        model.mScanPoliy.mCleanFaild = atoi((char *)value);
                    }
                }
                else if(xmlStrcmp(BAD_CAST "backup",ptr->name) == 0)
                {
                    for(grandson = ptr->children; grandson; grandson = grandson->next)
                    {
                        if(xmlStrcmp(BAD_CAST "backupenable",grandson->name) == 0)
                        {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value)
                            {
                                res = -3;
                                goto OVER;
                            }
                            else
                            {
                                model.mScanPoliy.mBackupType.mState = atoi((char *)value);
                                xmlFree(value);
                            }
                        }
                        else if(xmlStrcmp(BAD_CAST "backupfailmode",grandson->name) == 0)
                        {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value)
                            {
                                res = -3;
                                goto OVER;
                            }
                            else
                            {
                                model.mScanPoliy.mBackupType.mFailMode = atoi((char *)value);
                                xmlFree(value);
                            }
                        }
                    }
                }
                else if(xmlStrcmp(BAD_CAST "TimerScan",ptr->name) == 0)
                {
                    for(grandson = ptr->children; grandson; grandson = grandson->next)
                    {
                        if(xmlStrcmp(BAD_CAST "enable",grandson->name) == 0)
                        {
                            xmlChar * value = xmlNodeGetContent(grandson);
                            if(NULL == value)
                            {
                                res = -3;
                                goto OVER;
                            }
                            else
                            {
                                model.mScanPoliy.mTimeScan.mState = atoi((char *)value);
                                xmlFree(value);
                            }
                        }
                        else if(xmlStrcmp(BAD_CAST "everyweek",grandson->name) == 0)
                        {
                            xmlNodePtr sub = NULL;
                            for (sub=grandson->children; sub; sub=sub->next) {
                                if (xmlStrcmp(BAD_CAST "weekmark", sub->name) == 0) {
                                    xmlChar *value = xmlNodeGetContent(sub);
                                    model.mScanPoliy.mTimeScan.mWeeks = atoi((char*)value);
                                }
                                else if (xmlStrcmp(BAD_CAST "hour", sub->name) == 0) {
                                    xmlChar *value = xmlNodeGetContent(sub);
                                    model.mScanPoliy.mTimeScan.mHours = atoi((char*)value);
                                }
                                else if (xmlStrcmp(BAD_CAST "min", sub->name) == 0) {
                                    xmlChar *value = xmlNodeGetContent(sub);
                                    model.mScanPoliy.mTimeScan.mMinite = atoi((char*)value);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
OVER:
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return res;
}

void CommandMessage::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "Field", ptr->name) == 0) {
            XML_PROP_STRING_VALUE(ptr, "name", mName)
            XML_NODE_STRING_VALUE(ptr, mContent)
        }
        else if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
     }
}

void UpdateCommand::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "Admin", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mAdmin)
        }
        else if(xmlStrcmp(BAD_CAST "OnlyVer", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mOnlyVersion)
        }
        else if(xmlStrcmp(BAD_CAST "MaxDelay", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mMaxDelay)
        }
        else if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
    }
}

void ExecuteCommand::Parse(xmlNodePtr node){
    if(xmlStrcmp(BAD_CAST "runprocess", node->name) == 0){
        XML_PROP_INT_VALUE(node, "action", mAction)
        for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next){
            if     (xmlStrcmp(BAD_CAST "mainpath", ptr->name) == 0) {
                XML_NODE_STRING_VALUE(ptr, mMainPath)
            }
            else if(xmlStrcmp(BAD_CAST "cmdpara" , ptr->name) == 0) {
                XML_NODE_STRING_VALUE(ptr, mCmdPara)
            }
            else if(xmlStrcmp(BAD_CAST "workpath", ptr->name) == 0) {
                XML_NODE_STRING_VALUE(ptr, mWorkPath)
            }
            else if(xmlStrcmp(BAD_CAST "file"    , ptr->name) == 0) {
                XML_NODE_STRING_VALUE(ptr, mFile)
            }
            else if(XML_ELEMENT_NODE != ptr->type) {
                continue;
            }
        }
    }
    else{
        printf("%s",node->name);
    }
}

void EnvironmentalPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "eptray", ptr->name) == 0) {
            mTrayPolicy.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "s", ptr->name) == 0) {
            for(xmlNodePtr sptr = ptr->children; sptr; sptr = sptr->next) {
                if(xmlStrcmp(BAD_CAST "k", sptr->name) == 0) {
                    XML_NODE_STRING_VALUE(sptr, mAdminPasswd.mValue);
                }
                else if(XML_ELEMENT_NODE != ptr->type) {
                    continue;
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST "ClientGUID", ptr->name) == 0) {
            mLockGUID.Parse(ptr);
            for(xmlNodePtr sptr = ptr->children; sptr; sptr = sptr->next) {
                if(xmlStrcmp(BAD_CAST "lock", sptr->name) == 0) {
                    XML_NODE_INT_VALUE(sptr, mLockGUID.mValue)
                }
                else if(XML_ELEMENT_NODE != ptr->type) {
                    continue;
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST "centerlist", ptr->name) == 0) {
            XML_PROP_INT_VALUE(ptr, "lock", mCenterList.mLocked);

            for(xmlNodePtr sptr = ptr->children; sptr; sptr = sptr->next) {
                if(xmlStrcmp(BAD_CAST "item", sptr->name) == 0) {
                    string value;
                    XML_NODE_STRING_VALUE(sptr, value);
                    mCenterList.mValue.push_back(value);
                }
                else if(XML_ELEMENT_NODE != sptr->type) {
                    continue;
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST "epproxy", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mCenterProxyState);
        }
        else if(xmlStrcmp(BAD_CAST "checkeid", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mCheckEIDState);
        }
        else if(xmlStrcmp(BAD_CAST "CenterConfig", ptr->name) == 0) {
            mCenterConfig.Parse(ptr);
            mCenterConfig.mValue.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "ClientLogClean", ptr->name) == 0) {
            mLogCleanConfig.Parse(ptr);

        }
        else if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
    }
}

int EnvironmentalPolicy::SyncStorage(Storage *storage) {
    storage->SetSettingValue(TRAY_ICON_STATE, mTrayPolicy.mTrayHide.mValue, mTrayPolicy.mTrayHide.mLocked);
    storage->SetSettingValue(SHOW_AUTH_STATE, mTrayPolicy.mAuthPopHide.mValue, mTrayPolicy.mAuthPopHide.mLocked);
    storage->SetSettingValue(CLIENT_TITLE, mTrayPolicy.mTitle.mValue, mTrayPolicy.mTitle.mLocked);
    storage->SetSettingValue(CLIENT_SUB_TITLE, mTrayPolicy.mSubTitle.mValue, mTrayPolicy.mSubTitle.mLocked);
    storage->SetSettingValue(ADMIN_PASSWORD, mAdminPasswd.mValue, mAdminPasswd.mLocked);

    storage->SetSettingValue(CLIENT_LOCK_GUID, mLockGUID.mValue, mLockGUID.mLocked);
    storage->SetSettingValue(CENTER_PROXY_STATE, mCenterProxyState.mValue, mCenterProxyState.mLocked);
    storage->SetSettingValue(CHECK_CENTER_EID, mCheckEIDState.mValue, mCheckEIDState.mLocked);
    storage->SetSettingValue(RECONNECT_INTERVAL, mCenterConfig.mValue.mConnectRetryTime, mCenterConfig.mLocked);
    storage->SetSettingValue(UPLOAD_BANDWIDTH_LIMIT, mCenterConfig.mValue.mUploadBindWidth, mCenterConfig.mLocked);
    storage->SetSettingValue(BW_LIMIT_START_TIME, mCenterConfig.mValue.mUploadLimitStartTime, mCenterConfig.mLocked);
    storage->SetSettingValue(BW_LIMIT_END_TIME, mCenterConfig.mValue.mUploadLimitEndTime, mCenterConfig.mLocked);

    list<SourceListEntry> centers;
    if (storage->GetSourceList(centers, SourceListEntry::CENTER_PROXY_SOUCE) > 0) {
        list<SourceListEntry>::iterator iter;
        for (iter=centers.begin(); iter!=centers.end(); ++iter) {
            if (iter->mOffical == 1) {
                storage->DeleteSourceList(*iter);
            }
        }
    }

    string centerlist;
    list<string>::iterator iter;
    for (iter=mCenterList.mValue.begin(); iter!=mCenterList.mValue.end(); ++iter) {
        SourceListEntry entry;
        entry.mState = true;
        entry.mType = SourceListEntry::CENTER_PROXY_SOUCE;
        entry.mName = "http";
        entry.mAddress = *iter;
        entry.mOffical = 1;

        storage->InsertSourceList(entry);
    }
    storage->SetSettingValue(EP_CENTER_LIST, mCenterList.mLocked, 0);
    
    return 0;
}

void EnvironmentalPolicy::CenterConfig::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "TimerReVLinkConn", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mConnectRetryTime)
        }
        else if(xmlStrcmp(BAD_CAST "AverageSendData", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mUploadBindWidth)
        }
        else if(xmlStrcmp(BAD_CAST "BeginTime", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mUploadLimitStartTime)
        }
        else if(xmlStrcmp(BAD_CAST "EndTime", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mUploadLimitEndTime)
        }
        else if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
    }
}

void EnvironmentalPolicy::ClientLogClean::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "Item", ptr->name) == 0) {
            if (xmlHasProp(ptr, BAD_CAST"PID")) {
                XML_PROP_INT_VALUE(ptr, "PID", mPID)
            }
            else if (xmlHasProp(ptr, BAD_CAST"TID")) {
                XML_PROP_INT_VALUE(ptr, "TID", mTID)
            }
            else if (xmlHasProp(ptr, BAD_CAST"CountOfDays")) {
                XML_PROP_INT_VALUE(ptr, "CountOfDays", mOverDays)
            }
            else if (xmlHasProp(ptr, BAD_CAST"Operator")) {
                XML_PROP_STRING_VALUE(ptr, "Operator", mRelateState)
            }
            else if (xmlHasProp(ptr, BAD_CAST"TotalCount")) {
                XML_PROP_INT_VALUE(ptr, "TotalCount", mLimitCount)
            }
        }
        else if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
    }
}

void EnvironmentalPolicy::TrayPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "Hide", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mTrayHide);
        }
        else if(xmlStrcmp(BAD_CAST "ShowAuthTip", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mAuthPopHide);
        }
        else if(xmlStrcmp(BAD_CAST "Title", ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mTitle);
        }
        else if(xmlStrcmp(BAD_CAST "SubTitle", ptr->name) == 0) {
            CONFIG_STRING_VALUE(ptr, mSubTitle);
        }
        else if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
    }
}

void FirewallPolicy::RsIPEntry::Parse(xmlNodePtr node) {
    mID = (char*)xmlGetProp(node, BAD_CAST"id");
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "Status", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mStatus)
        }
        else if(xmlStrcmp(BAD_CAST "Name", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mName)
        }
    }
}

void FirewallPolicy::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "NetProtect", ptr->name) == 0) {
            mNetProtect.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "WhiteUrlList", ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child=child->next) {
                if (xmlStrcmp(BAD_CAST "Rule", child->name) == 0) {
                    mWhiteUrlList.mValue.push_back((char*)xmlGetProp(child, BAD_CAST"id"));
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST "RsIpRuleList", ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child=child->next) {
                if (xmlStrcmp(BAD_CAST "Rule", child->name) == 0) {
                    RsIPEntry entry;
                    entry.Parse(child);
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST "BrowserAudit", ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mBrowserAudit);
        }
        else if(xmlStrcmp(BAD_CAST "NetProcAudit", ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mNetProcAudit);
        }
        else if(xmlStrcmp(BAD_CAST "FluxMgr", ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mFluxMgr);
        }
        else if(xmlStrcmp(BAD_CAST "AdslShare", ptr->name) == 0) {
            mAdslShare.Parse(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "ShareMgr", ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mShareMgr);
        }
    }
}

int FirewallPolicy::SyncStorage(Storage *storage) {
    return 0;
}

void FirewallPolicy::_BaseUrlEntry::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if (xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if (xmlStrcmp(BAD_CAST "LogStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLogStatus);
        }
    }
}

void FirewallPolicy::RsIPRule::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if (xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if (xmlStrcmp(BAD_CAST "LogStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLogStatus);
        }
        else if (xmlStrcmp(BAD_CAST "AlertStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mAlertStatus);
        }
        else if (xmlStrcmp(BAD_CAST "BreakTimes", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mBreakTimes);
        }
    }
}

void FirewallPolicy::AdFilter::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if (xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if (xmlStrcmp(BAD_CAST "LogStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLogStatus);
        }
        else if (xmlStrcmp(BAD_CAST "RuleList", ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child=child->next) {
                if (xmlStrcmp(BAD_CAST "Rule", child->name) == 0) {
                    string tmp;
                    XML_PROP_STRING_VALUE(child, "id", tmp)
                    mRule.push_back(tmp);
                }
            }
        }
    }
}

void FirewallPolicy::NetProtect::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if (xmlStrcmp(BAD_CAST "FwStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mStatus);
        }
        else if (xmlStrcmp(BAD_CAST "WhiteUrlStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mWhiteUrlStatus);
        }
        else if (xmlStrcmp(BAD_CAST "AntiEvilUrl", ptr->name) == 0) {
            mAntiEvilUrl.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "AntiFishUrl", ptr->name) == 0) {
            mAntiFishUrl.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "AntiEvilDown", ptr->name) == 0) {
            mAntiEvilDown.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "SearchUrlProtect", ptr->name) == 0) {
            mSearchUrlProtect.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "AntiXss", ptr->name) == 0) {
            mAntiXss.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "AdFilter", ptr->name) == 0) {
            mAdfilter.Parse(ptr);
        }
        else if (xmlStrcmp(BAD_CAST "RsIpRule", ptr->name) == 0) {
            mRsIPRules.Parse(ptr);
        }
    }
}

int FirewallPolicy::NetProtect::SyncStorage(Storage *storage) {

    return 0;
}

void FirewallPolicy::NetProcAudit::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if (xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if (xmlStrcmp(BAD_CAST "LogStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLogStatus);
        }
        else if (xmlStrcmp(BAD_CAST "CheckRsSign", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mCheckRsSign);
        }
        else if (xmlStrcmp(BAD_CAST "CheckModule", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mCheckModule);
        }
        else if (xmlStrcmp(BAD_CAST "UnknowAction", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mUnknowAction);
        }
        else if(xmlStrcmp(BAD_CAST "TimeRuleList", ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST "TimeRule", child->name) == 0) {
                   ConfigField<TimeRule<NetProcRule> > tr;
                   CONFIG_NODE_VALUE(child, tr);
                   mTimeRuleList.push_back(tr);
                }
            }
        }
    }
}

int FirewallPolicy::NetProcAudit::SyncStorage(Storage *storage) {
    return 0;
}

void FirewallPolicy::AccessControl::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if(xmlStrcmp(BAD_CAST "AlertStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mAlertStatus);
        }
        else if(xmlStrcmp(BAD_CAST "ControlCode", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mControlCode);
        }
        else if(xmlStrcmp(BAD_CAST "RuleList", ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST "Admin", child->name) == 0) {
                    for(xmlNodePtr cptr = child->children; cptr; cptr = cptr->next) {
                        if(xmlStrcmp(BAD_CAST "Rule", cptr->name) == 0) {
                            string tmp;
                            XML_NODE_STRING_VALUE(cptr, tmp)
                            mRuleList.push_back(tmp);
                        }
                    }
                }
            }
        }
    }
}

void FirewallPolicy::ShareMgr::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "FileLogStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mFileLogStatus);
        }
        else if(xmlStrcmp(BAD_CAST "AccessLogStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mAccessLogStatus);
        }
        else if(xmlStrcmp(BAD_CAST "DisableDefaultShare", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mDisableShare);
        }
        else if(xmlStrcmp(BAD_CAST "AccessControl", ptr->name) == 0) {
            CONFIG_NODE_VALUE(ptr, mAccessControl);
        }
    }
}

int FirewallPolicy::ShareMgr::SyncStorage(Storage *storage) {
    return 0;
}

void FirewallPolicy::FluxMgr::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if(xmlStrcmp(BAD_CAST "LogTimer", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mLogTimer);
        }
    }
}

void FirewallPolicy::AdslShare::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mMonStatus);
        }
        else if(xmlStrcmp(BAD_CAST "TotalWidth", ptr->name) == 0) {
            CONFIG_INTEGER_VALUE(ptr, mTotalWidth);
        }
    }
}

void FirewallPolicy::BrowserRule::Parse(xmlNodePtr node) {
    mID = (char*)xmlGetProp(node, BAD_CAST"id");
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "Url", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mUrl)
        }
        else if(xmlStrcmp(BAD_CAST "ControlMode", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mControlModel)
        }
        else if(xmlStrcmp(BAD_CAST "Alert", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mAltert)
        }
        else if(xmlStrcmp(BAD_CAST "Redirect", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mRedirect)
        }
    }
}

template <class T>
void FirewallPolicy::TimeRule<T>::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "ValidTime", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mValidTime)
        }
        else if(xmlStrcmp(BAD_CAST "RuleList", ptr->name) == 0) {
            for(xmlNodePtr child = ptr->children; child; child = child->next) {
                if(xmlStrcmp(BAD_CAST "Rule", child->name) == 0) {
                    T rule;
                    rule.Parse(child);
                    mRuleList.push_back(rule);
                }
            }
        }
    }
}

void FirewallPolicy::NetProcRule::Parse(xmlNodePtr node) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(xmlStrcmp(BAD_CAST "CheckMd5", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mCheckMD5)
        }
        else if(xmlStrcmp(BAD_CAST "AllowListen", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mAllowListen)
        }
        else if(xmlStrcmp(BAD_CAST "AllowOutside", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mAllowOutside)
        }
        else if(xmlStrcmp(BAD_CAST "SoftType", ptr->name) == 0) {
            XML_NODE_INT_VALUE(ptr, mSoftType)
        }
        else if(xmlStrcmp(BAD_CAST "SoftId", ptr->name) == 0) {
            XML_NODE_STRING_VALUE(ptr, mSoftID)
        }
    }
}

void FirewallPolicy::BrowserAudit::Parse(xmlNodePtr node) {
     for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
         if(xmlStrcmp(BAD_CAST "MonStatus", ptr->name) == 0) {
             CONFIG_INTEGER_VALUE(ptr, mMonStatus);
         }
         else if(xmlStrcmp(BAD_CAST "LogAllWeb", ptr->name) == 0) {
             CONFIG_INTEGER_VALUE(ptr, mLogAllWeb);
         }
         else if(xmlStrcmp(BAD_CAST "TimeRuleList", ptr->name) == 0) {
             for(xmlNodePtr child = ptr->children; child; child = child->next) {
                 if(xmlStrcmp(BAD_CAST "TimeRule", child->name) == 0) {
                    ConfigField<TimeRule<BrowserRule> > tr;
                    CONFIG_NODE_VALUE(child, tr);
                    mTimeRuleList.push_back(tr);
                 }
             }
         }
     }
}
