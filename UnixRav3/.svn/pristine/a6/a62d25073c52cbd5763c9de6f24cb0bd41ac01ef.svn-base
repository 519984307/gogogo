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
    virtual int OnCommandObserver(int event, void *args, void **result, int* outlen) = 0;
};

typedef struct local_sock_h {
    int sock;
    struct sockaddr_un dest;
    struct sockaddr_un src;

    local_sock_h() {
        sock = 0;
        memset(&dest, 0, sizeof(struct sockaddr_un));
        memset(&src, 0, sizeof(struct sockaddr_un));
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
                               const char *sock,
                               bool systemType=false);
    ~CenterListener();

    int Start();
    void Stop();

private:
    CenterListener(CenterCommandObserver *observer,  int type, bool systemType);
    int Construct(const char *sock);

    int InitLocalSock(LocalSocket *sock, const char *path);

private:
    LocalSocket mSock;
    int mEpollFd;

private:
    static void* Work(void *args);
    void WorkFunc();
    void RespAccept(int fd);

private:
    pthread_t   mThread;

    int mStatus;
private:
    CenterCommandObserver   *mObserver;
    int     mType;
    bool    mSystemType;
    bool    mQtApp;

    // sock path or dbus name
    char mSockPath[256];

    // for file monitor respond path
    char mTmpPath[1024];

private:
    class PollData {
    public:
        int fd;
        unsigned char *out;
        int outlen;
        int extend;

        PollData(int fd) {
            this->fd = fd;
            out = NULL;
            outlen = 0;
            extend = 0;
        }
        ~PollData() {
            if (out) {
                delete[] out;
            }
            if (fd) {
                close(fd);
            }
        }
    };
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
