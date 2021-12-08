#ifndef NETACCESSENTRY_H
#define NETACCESSENTRY_H

#include "baseentry.h"

class NetAccessEntry : public BaseEntry
{
public:
    NetAccessEntry() {
        mType = URL;
        mState = REJECT;
        mSource = PROCESS;
    }

public:
    long    mTime;
    std::string mUrl;
    std::string mProcess;
    typedef enum {
        URL,
        IP
    }MatchType;
    MatchType   mType;
    typedef enum {
        REJECT = 1
    }MatchResult;
    MatchResult mState;
    typedef enum {
        BROWSER,
        DOWNLOAD,
        PROCESS
    }NetSource;
    NetSource   mSource;

public:
    std::string CreateTable() const;
    std::string Select() const;
    std::string Insert() const;
    std::string Update() const;
    std::string Delete() const;

    void Mapping(CppSQLite3Query *query);
};

#endif // NETACCESSENTRY_H
