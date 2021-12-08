#ifndef VIRUSINFOENTRY_H
#define VIRUSINFOENTRY_H

#include "baseentry.h"

#include <string>

using namespace std;

class VirusInfoEntry : public BaseEntry, public Serializable
{
public:
    VirusInfoEntry() {
        mScanLogID = 0;
        mVirusType = 0;
        mDealState = 0;
        mTime = 0;
        mFileSize = 0;
        mReported = 0;
        mReserved1 = 0;
        mReserved2 = 0;
    }

public:
    long    mScanLogID;
    string  mVirusName;
    int     mVirusType;
    string  mOriginPath;
    string  mOriginName;
    int     mDealState;
    long    mTime;
    long    mFileSize;
    string  mQuarantine;
    string  mHash;
    int     mReported;
    int     mReserved1;
    int     mReserved2;

public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    void Mapping(CppSQLite3Query *query);

private:
    void InitializeJson(Json::Value& json);
    void ExternlizeJson(Json::Value& json);
};

#endif // VIRUSINFOENTRY_H
