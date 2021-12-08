#pragma once

#include <string>
#include <list>

#include "RUserNFLink.h"

class NFLObserver {
public:
    enum FilterEvent {
        NET_BLOCK = 1,
    };
    enum FilterType {
        BLOCK_IP = 1,
        BLOCK_URL,
    };

public:
    virtual int OnNFLCommunicateEvent(enum FilterEvent event, enum FilterType type, const std::string& data) = 0;
};

class CNFLCommunicator: public CRUserNFLink
{
public:
    CNFLCommunicator(NFLObserver *observer) {
        mObserver = observer;
    }

    ~CNFLCommunicator();

    void ReceivedMsg(int nCmdType, void *Data, int nLen); //for class interface
    
    int UpdateBlackList(std::list<std::string> rules);
    int UpdateBlackList(const char *sFileName, int nLen);

protected:
    int ProcessBlockIPLog(unsigned int sIP, unsigned int dIP);
    int ProcessBlockUrlLog(const char* sLog, int nLen);

private:
    NFLObserver *mObserver;
};
