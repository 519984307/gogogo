#include "centerfindproxy.h"
#include "commonapi.h"
#include "json/json.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#define GROUP_ADDR  "239.193.168.87"
#define GROUP_PORT  7767

#define MSGBUFSIZE 256

#define SECURITY_CHECK(x, y, z) if(x != y) return z;


CenterFindProxy::ClientCenterData::ClientCenterData(const string &eid) {
    this->eid = eid;
    this->nonce = Utils::String::RandomString();
}

CenterFindProxy* CenterFindProxy::NEW(CenterFindObserver *observer, const string& ip) {
    CenterFindProxy *self = new CenterFindProxy;
    if (self) {
        if (self->Contruct(observer, ip)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

CenterFindProxy::CenterFindProxy() {
    mType = I_NEED_ASK;
    mState = false;
    mFindObserver = NULL;
}

CenterFindProxy::~CenterFindProxy() {

}

int CenterFindProxy::Contruct(CenterFindObserver *observer, const string& ip) {
    mFindObserver = observer;
    if (ip.empty()) {
        return -1;
    }
    mCurrentIP = ip;
    return 0;
}

int CenterFindProxy::StartAnswer() {
    mType = I_CAN_ANSWER;
    mState = true;
    return pthread_create(&mThread, NULL, ThreadWork, this);
}

int CenterFindProxy::StartAsk() {
    mType = I_NEED_ASK;
    mState = true;
    return pthread_create(&mThread, NULL, ThreadWork, this);
}

int CenterFindProxy::Stop() {
    if (mState) {
        mState = false;
        pthread_cancel(mThread);
        pthread_join(mThread, NULL);
    }
    return 0;
}

void* CenterFindProxy::ThreadWork(void *params) {
    CenterFindProxy *self = (CenterFindProxy*)params;
    self->WorkFunc();
    return NULL;
}

void CenterFindProxy::WorkFunc() {
    if (I_NEED_ASK == mType) {
        AskWork();
    }
    else if(I_CAN_ANSWER == mType) {
        AnswerWork();
    }
}

int CenterFindProxy::InitSocket(uint32_t ipaddr, const int port) {
    if ((mSockFd = socket(AF_INET,SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    int yes=1;
    if (setsockopt(mSockFd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        return -2;
    }

    memset(&mSockAddr, 0, sizeof(mSockAddr));
    mSockAddr.sin_family = AF_INET;
    mSockAddr.sin_addr.s_addr = ipaddr;
    mSockAddr.sin_port = htons(port);

    return 0;
}

int CenterFindProxy::SetMulitAddr(uint32_t multiaddr) {
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = (in_addr_t)multiaddr;
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(mSockFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        return -1;
    }
    return 0;
}

int CenterFindProxy::RecvData(ClientCenterData &data) {
    int addrlen = sizeof(mSockAddr);
    int nbytes = 0;

    if ((nbytes = recvfrom(mSockFd, mBuffer, MSG_BUF_SIZE, 0, (struct sockaddr*)&mSockAddr, (socklen_t*)&addrlen)) < 0) {
        return -1;
    }

    mBuffer[nbytes] = '\0';
    return ParseData(mBuffer, data);
}

int CenterFindProxy::AnswerWork() {
    SECURITY_CHECK(InitSocket(htonl(INADDR_ANY), GROUP_PORT), 0, -1);
    SECURITY_CHECK(bind(mSockFd, (struct sockaddr *)&mSockAddr,sizeof(mSockAddr)), 0, -2);
    SECURITY_CHECK(SetMulitAddr(inet_addr(GROUP_ADDR)), 0, -3);

    ClientCenterData recvClient;
    while (mState) {
        if (RecvData(recvClient)) {
            continue;
        }

        if (mFindObserver) {
            if (mFindObserver->onCenterFindEvent(
                        CenterFindObserver::CENTER_FINDING,
                        recvClient.eid,
                        recvClient.center)) {
                continue;
            }

            string success;
            if (0 == EncodeData(recvClient, success)) {
                sendto(mSockFd, success.c_str(), success.size(), 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
            }
        }
        sleep(30);
    }
    return 0;
}

int CenterFindProxy::AskWork() {
    SECURITY_CHECK(InitSocket(inet_addr(GROUP_ADDR), GROUP_PORT), 0, -1);

    ClientCenterData center(mClientCenterData.eid);
    while (mState) {
        int err = QueryCenter(center);
        if (0 == err && center.center.size() > 0) {
            if (mFindObserver) {
                if (mFindObserver->onCenterFindEvent(
                            CenterFindObserver::CENTER_FOUND,
                            center.eid,
                            center.center)) {
                    break;
                }
                else {
                    // retry the center, exit thread
                    mState = false;
                    continue;
                }
            }
        }

        sleep(10);
    }
    return 0;
}

int  CenterFindProxy::QueryCenter(ClientCenterData& data) {
    struct sockaddr_in addr;
    int fd = 0;
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        return -1;
    }

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(GROUP_ADDR);
    addr.sin_port = htons(GROUP_PORT);

    struct timeval tv_timeout;
    tv_timeout.tv_sec = 5;
    tv_timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

    }
    tv_timeout.tv_sec = 5;
    tv_timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

    }

    struct in_addr localInterface;
    localInterface.s_addr = inet_addr(mCurrentIP.c_str());
    if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
        return -1;
    }

    string source;
    SECURITY_CHECK (EncodeData(data, source), 0, -1);

    if (sendto(fd,source.c_str(), source.size(), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        return -1;
    }

    char buffer[MSG_BUF_SIZE];
    int nbytes = 0;
    int addrlen = sizeof(addr);
    if ((nbytes = recvfrom(fd, buffer, MSG_BUF_SIZE, 0, (struct sockaddr*)&addr, (socklen_t*)&addrlen)) < 0) {
        return -1;
    }
    buffer[nbytes] = '\0';
    close(fd);

    SECURITY_CHECK(ParseData(buffer, data), 0, -1);   
    return 0;
}

int CenterFindProxy::ParseData(const string &data, ClientCenterData& ret) {
    string result = Utils::Hash::RC4Decrypt(data);

    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(result, json)) {
        return -1;
    }

    if (ret.nonce.empty()) {
        ret.eid = json["eid"].asString();
        ret.nonce = json["key"].asString();
        return 0;
    }
    else {
        if (ret.nonce == json["key"].asString()) {
            ret.eid = json["eid"].asString();
            Json::Value centers = json["hosts"];
            for (int i=0; i<centers.size(); ++i) {
                ret.center.push_back(centers[i].asString());
            }
            return 0;
        }
    }

    return -1;
}

int CenterFindProxy::EncodeData(const ClientCenterData &data, string &ret) {
    Json::Value json;
    json["eid"] = Json::Value(data.eid);
    json["key"] = Json::Value(data.nonce);

    for (int i=0; i<data.center.size(); ++i) {
        json["hosts"].append(Json::Value(data.center[i]));
    }

    Json::FastWriter writer;
    ret = Utils::Hash::RC4Encrypt(writer.write(json));
    return 0;
}



