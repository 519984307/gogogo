#ifndef SETTINGENTRY_H
#define SETTINGENTRY_H

#include "baseentry.h"

#include <string.h>

//class SettingEntry
//{
//    friend class hiberlite::access;
//    template<class Archive>
//    void hibernate(Archive & ar) {
//        ar & HIBERLITE_NVP(name);
//        ar & HIBERLITE_NVP(mIntValue);
//        ar & HIBERLITE_NVP(mStrValue);
//    }
//public:
//    string  name;
//    int     mIntValue;
//    string  mStrValue;
//};
//HIBERLITE_EXPORT_CLASS(SettingEntry)

using namespace std;
class SettingEntry : public BaseEntry {
public:
    const int DEFAULT_INT_VALUE;
    string mName;
    enum {
        INTEGER,
        STRING
    }mType;
    union {
        char mStringValue[128];
        int  mIntegerValue;
    }mValue;
    union {
        char mUserStrValue[128];
        int  mUserIntValue;
    }mUserValue;
    int mAvailed;

    SettingEntry():DEFAULT_INT_VALUE(-999) {
        mAvailed = 0;
        memset(mValue.mStringValue, 0, 128);
        memset(mUserValue.mUserStrValue, 0, 128);
        mValue.mIntegerValue = DEFAULT_INT_VALUE;
        mUserValue.mUserIntValue = DEFAULT_INT_VALUE;
    }

public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    void Mapping(CppSQLite3Query *query);
};

#endif // SETTINGENTRY_H
