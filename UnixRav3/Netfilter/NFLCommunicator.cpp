#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "PubDef.h"
#include "NFLCommunicator.h"


CNFLCommunicator::~CNFLCommunicator()
{

}

void CNFLCommunicator::ReceivedMsg(int nCmdType, void *Data, int nLen)
{
    unsigned int sip, dip;
    //DEBUG("received:%s\n", (char*) Data);
    switch(nCmdType){
    case NFL_BLACKLIST_SET_ACK:
        DEBUG("SET ACK: %s\n", (char*)Data);
        break;
    case NFL_BLACKLIST_BLOCK_IP:
        if (Data == NULL || nLen < 8) {
            DEBUG("Error, ReceivedMsg NFL_BLACKLIST_BLOCK_IP invalid parameter.\n");
            break;
        }
        memcpy(&sip, Data, 4);
        memcpy(&dip, ((unsigned char*)Data)+4, 4);
        ProcessBlockIPLog(sip, dip);
        break;
    case NFL_BLACKLIST_BLOCK_URL:
        ProcessBlockUrlLog((const char*)Data, nLen);
        break;
    default:
        DEBUG("Error, ReceivedMsg Invalid parameter.\n");
        break;
    }
}

int CNFLCommunicator::ProcessBlockIPLog(unsigned int sip, unsigned int dip)
{
    DEBUG("ProcessBlockIPLog: %u  %u\n", sip, dip);
    uint32_t hip = ntohl(dip);
    char sdip[20];
    sprintf(sdip, "%u.%u.%u.%u", NIPQUAD(hip));
    if (mObserver) {
        mObserver->OnNFLCommunicateEvent(NFLObserver::NET_BLOCK, NFLObserver::BLOCK_IP, sdip);
    }
    return 0;
}

int CNFLCommunicator::ProcessBlockUrlLog(const char* sLog, int nLen)
{
    if (sLog == NULL || nLen <= 0) {
        DEBUG("Error, ProcessBlockUrlLog invalid parameter\n");
        return -1;
    }
    DEBUG("ProcessBlockUrlLog: %s  %d\n", sLog, nLen);
    if (mObserver) {
        mObserver->OnNFLCommunicateEvent(NFLObserver::NET_BLOCK, NFLObserver::BLOCK_URL, sLog);
    }
    return 0;
}

int CNFLCommunicator::UpdateBlackList(std::list<std::string> rules)
{
    char sbuffer[MAX_MSG_LEN];  
    int ret = SendLine(NFL_BLACKLIST_SET_START, sbuffer, 0);
    if(ret == -1){
        //DEBUG("error, Send msg fail\n");
        return -1;
    }
    if (rules.size() <= 0) {
        return -1;
    }
    std::list<std::string>::iterator it = rules.begin();
    while (it != rules.end()) {
        std::string rule = *it;
        if (rule.length() > 0) {
            ret = SendLine(NFL_BLACKLIST_SET_REQ, rule.c_str(), rule.length());
            if (ret == -1) {
                //DEBUG("error, Send msg fail\n");
            }
        }
        it++;
    }
    ret = SendLine(NFL_BLACKLIST_SET_END, sbuffer, 0);
    if (ret == -1) {
        //DEBUG("error, Send msg fail\n");
        return -1;
    }
    return 0;
}

int CNFLCommunicator::UpdateBlackList(const char *sData, int nLen)
{
    int i,len,nstart=0;
    char sbuffer[MAX_MSG_LEN];  
    if (sData == NULL || nLen < 1) {
        DEBUG("Error, UpdataBlackList invalid parameter.\n");
        return -1;
    }
    int ret = SendLine(NFL_BLACKLIST_SET_START, sbuffer, 0);
    if(ret == -1){
        //DEBUG("error, Send msg fail\n");
        return -1;
    }
    for(i=0; i<nLen-1; i++){
        if(sData[i] == '\n' || sData[i] == '\r'){
            len = i - nstart;
            if(len < 7 || len > MAX_MSG_LEN){
                nstart = i + 1;
                continue;
            }
            memcpy(sbuffer, sData+nstart, len);
            sbuffer[len] = '\0';
            ret = SendLine(NFL_BLACKLIST_SET_REQ, sbuffer, len + 1);
            if(ret == -1){
                //DEBUG("error, Send msg fail\n");
            }
            if(sData[i+1] == '\n' || sData[i+1] == '\r')
                i++;
            nstart = i+1;
        }
    }  
    ret = SendLine(NFL_BLACKLIST_SET_END, sbuffer, 0);
    if(ret == -1){
        //DEBUG("error, Send msg fail\n");
        return -1;
    }
    return 0;
}
