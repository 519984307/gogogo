#ifndef CENTERLISTENER_H
#define CENTERLISTENER_H

#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <list>
#include <string>
#include <arpa/inet.h>

#include "rserializer.h"
#include "communicatebundle.h"


class CenterCommandObserver {
public:
    enum {
        PUBLIC_API_SCAN,
        CLIENT_COMMAND,
        CLIENT_SIGNAL,
        CLINET_GROUP_BROADCAST
    };
    virtual int OnCommandObserver(int event, void *args, int fds) = 0;
};


typedef struct local_sock_h {
    int sock;
    struct sockaddr_in dest;
    struct sockaddr_in src;

    local_sock_h() {
        sock = 0;
        memset(&dest, 0, sizeof(struct sockaddr_in));
        memset(&src, 0, sizeof(struct sockaddr_in));
    }
    ~local_sock_h() {
        if (sock > 0) {
            close(sock);
        }
    }
} LocalSocket;

class CenterListener        
{
public:
    static CenterListener* NEW(CenterCommandObserver *observer,
                               int/*CenterCommandObserver*/ type,                    
                               short port);
    ~CenterListener();

    int Start();
    void Stop();

private:
    CenterListener(CenterCommandObserver *observer,  int type);
    int Construct(short port);

    int InitServerSock(LocalSocket *sock, short port);

private:
    LocalSocket mSock;


private:
    static void* Work(void *args);
    void WorkFunc();

private:
    pthread_t   mThread;
    int mStatus;

private:
    CenterCommandObserver   *mObserver;
    int     mType;
    bool    mSystemType;
    bool    mQtApp;

    // sock path or dbus name
    short mPort;

    // for file monitor respond path
    char mTmpPath[1024];

};


class ProxyInfoListener {
public:
    static ProxyInfoListener*  NEW(uint32_t addr, short port);
    ~ProxyInfoListener();

    void SetObserver(CenterCommandObserver *observer) {
        mObserver = observer;
    }

    void Start();
    void Stop();

    static int SendBroadcast(uint32_t addr, short port, unsigned char *data, int len, char *outdata);

private:
    ProxyInfoListener(uint32_t addr, short port);
    int Construct();

private:
    CenterCommandObserver *mObserver;
    uint32_t mAddr;
    short mPort;

    int mStatus;
    int mSockFd;
    struct sockaddr_in mSockAddr;

    unsigned char mBuffer[1024];

private:
    pthread_t   mThread;
    static void* ThreadWork(void *args);
    void WorkFunc();
};

#endif // CENTERLISTENER_H
