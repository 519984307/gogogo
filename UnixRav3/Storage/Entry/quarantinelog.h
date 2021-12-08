#ifndef QUARANTINELOG_H
#define QUARANTINELOG_H

#include "baseentry.h"
#include <string>

class QuarantineLog : public BaseEntry, public Serializable
{
public:
    int mDate;
    std::string mFile;
    std::string mSource;
    std::string mTarget;
    std::string mVirusName;
    long mSize;
    int         mUID;
    int         mGID;
    int         mPermision;

    std::string mReserved;

public:
    QuarantineLog() {
        mDate = 0;
        mSize = 0;
        mUID = 0;
        mGID = 0;
        mPermision = 0;
    }

public:
    std::string CreateTable() const;
    std::string Select() const;
    std::string Insert() const;
    std::string Update() const;
    std::string Delete() const;

    void Mapping(CppSQLite3Query *query);

private:
    void InitializeJson(Json::Value& json);
    void ExternlizeJson(Json::Value& json);

};

#endif // QUARANTINELOG_H
