#ifndef SOURCELISTENTRY_H
#define SOURCELISTENTRY_H

#include "baseentry.h"

#include <string>
using namespace std;

class SourceListEntry : public BaseEntry
{
public:
    SourceListEntry(){
        mID = 0;
        mType = CLOUD_SCAN_SOURCE;
        mPort = 0;
        mOffical = -1;
        mRequestModel = 0;
        mOncePostfileCount = 0;
        mLocked = 0;
    }

    void operator =(const SourceListEntry& entry) {
        this->mID = entry.mID;
        this->mType = entry.mType;
        this->mPort = entry.mPort;
        this->mOffical = entry.mOffical;
        this->mRequestModel = entry.mRequestModel;
        this->mOncePostfileCount = entry.mOncePostfileCount;
        this->mLocked = entry.mLocked;
        this->mName = entry.mName;
        this->mAddress = entry.mAddress;
    }

public:
    typedef enum SourceType {
        CLOUD_SCAN_SOURCE,
        CLOUD_UPDATE_SOURCE,
        CENTER_PROXY_SOUCE,
    }SOURCE_TYPE;

public:
    bool            mState;
    string          mName;
    string          mAddress;
    int             mPort;
    SOURCE_TYPE     mType;
    /** 1 from cloud or 0 form manually */
    int             mOffical;
    int             mRequestModel;
    int             mOncePostfileCount;
    bool            mLocked;

public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    void Mapping(CppSQLite3Query *query);
};

#endif // SOURCELISTENTRY_H
