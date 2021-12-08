#ifndef CENTERFINDPROXY_H
#define CENTERFINDPROXY_H

#include <unistd.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <arpa/inet.h>

using namespace std;

#define MSG_BUF_SIZE    1024

class CenterFindObserver {
public:
    enum {
        CENTER_FINDING = 1,
        CENTER_FOUND
    };
    virtual int onCenterFindEvent(int event, string& eid, vector<string>& centers) = 0;
};

class CenterFindProxy {
public:
    class ClientCenterData {
    public:
    ClientCenterData(const string& eid);
    ClientCenterData(){}

    public:
        string eid;
        string nonce;
        vector<string> center;
    };

public:
    static CenterFindProxy* NEW(CenterFindObserver *observer, const string& ip);
    ~CenterFindProxy();

    void SetEID(const string& eid) {
        mClientCenterData.eid = eid;
    }

    void SetCenterAddr(const string& center) {
        mClientCenterData.center.push_back(center);
    }

    int StartAnswer();
    int StartAsk();

    int Stop();

    int QueryCenter(ClientCenterData& data);

private:
    CenterFindProxy();
    int Contruct(CenterFindObserver *observer, const string& ip);

    int AnswerWork();
    int AskWork();

    int InitSocket(uint32_t ipaddr, const int port);
    int SetMulitAddr(uint32_t multiaddr);
    int RecvData(ClientCenterData &data);

    int ParseData(const string& data, ClientCenterData& ret);
    int EncodeData(const ClientCenterData& data, string& ret);

private:
    pthread_t  mThread;
    static void* ThreadWork(void *params);
    void WorkFunc();

private:
    int mSockFd;
    struct sockaddr_in mSockAddr;

    string mCurrentIP;

private:
    enum {
        I_CAN_ANSWER = 1,
        I_NEED_ASK
    } mType;

    char mBuffer[MSG_BUF_SIZE];

    bool mState;

    ClientCenterData mClientCenterData;

    CenterFindObserver  *mFindObserver;
};

#endif // CENTERFINDPROXY_H
