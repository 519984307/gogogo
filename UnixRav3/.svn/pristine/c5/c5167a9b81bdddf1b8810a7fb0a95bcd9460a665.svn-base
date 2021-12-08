#ifndef URLRULEENTRY_H
#define URLRULEENTRY_H

#include "baseentry.h"

#include <string>
using namespace std;

class UrlRuleEntry : public BaseEntry
{
public:
    UrlRuleEntry(){
        mID = 0;
        mType = UNSET;
        mCategory = BLOCK_ALL;
        mControl = 0;
        mAlert = 0;
    }

public:
    typedef enum RuleCategory {
        BLOCK_ALL = 0,
        BROWSER_ONLY,
        NETFILTER_ONLY,
    }RULE_CATEGORY;

    typedef enum RuleType {
        UNSET = 0,
        IP_ADDR,
        URL_MATCH,
        URL_REGEX
    }RULE_TYPE;

public:
    string          mPath;
    RULE_CATEGORY   mCategory;
    RULE_TYPE       mType;
    string          mSerial;
    int             mControl;
    int             mAlert;
    string          mRedirect;
    string          mValidTime;

public:
    string CreateTable() const;
    string Select() const;
    string Insert() const;
    string Update() const;
    string Delete() const;

    void Mapping(CppSQLite3Query *query);
};


#endif // URLRULEENTRY_H
