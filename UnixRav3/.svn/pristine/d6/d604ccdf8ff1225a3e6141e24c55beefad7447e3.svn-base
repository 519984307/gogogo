#ifndef WHITELISTENTRY_H
#define WHITELISTENTRY_H

#include "baseentry.h"

#include <string>
using namespace std;

class WhiteListEntry : public BaseEntry, public Serializable
{
public:
    WhiteListEntry(){
        mID = 0;
        mListType = WHITE;
        mBlockType = FILE_SCAN;
        mPathType = UNSET;
        mReserved1 = 0;
        mReserved2 = 0;
    }

    bool operator<(const WhiteListEntry& entry) {
        return mID < entry.mID;
    }
public:
    typedef enum ListType {
        WHITE = 0,
        BLACK
    }LIST_TYPE;

    typedef enum BlockType {
        FILE_SCAN = 0,
        PROCESS_BLOCK,
        FOLDER_MONITOR,
        URL_BLOCK,
        IP_BLOCK
    }BLOCK_TYPE;

    typedef enum PathType {
        UNSET           = -1,
        FILE_PATH       = 0,
        FOLDER_ONLY,
        FOLDER_RECURSION,
        FOLDER_CHILDERN,
        // 上面为和中心对应的类型，下面为自定义
        FILE_EXTEND_TYPE,
        FILE_HASH,
        PROCESS_PATH,
        PROCESS_NAME,
        CENTER_MONITOR_FOLDER,
        URL,
        IP,
    }PATH_TYPE;

public:
    string      mPath;
    LIST_TYPE   mListType;
    BLOCK_TYPE  mBlockType;
    PATH_TYPE   mPathType;

    int mReserved1;
    int mReserved2;

public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    void Mapping(CppSQLite3Query *query);

public:
    void InitializeJson(Json::Value& json);
    void ExternlizeJson(Json::Value& json);
};

#endif // WHITELISTENTRY_H
