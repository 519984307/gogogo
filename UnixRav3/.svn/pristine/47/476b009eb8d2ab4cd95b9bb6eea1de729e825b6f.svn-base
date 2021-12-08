#include "centerlistener.h"
#include "json/json.h"
#include "commonapi.h"
#include "communicatebundle.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#if defined(COMM_WITH_SOCKET)
    #include <sys/socket.h>
#elif defined(COMM_WITH_DBUS)
    #include  <QtDBus/QtDBus>
#endif

#define MAX_PACKET 1024

#define CLIENT_SEND_SOURCE  "client.signal.source"
#define CLIENT_SERV_SOURCE  "client.method.source"
#define CLIENT_OBJECT_NAME  "/rav/signal/Object"
#define CLIENT_SIGNAL_TYPE  "rav.signal.Type"
#define CLIENT_METHOD_TYPE  "rav.method.Type"
#define SERVICE_INTERFACE   "com.rising.antivirus.interface"
#define REMOTE_SIGNAL_NAME  "SendSignal"
#define REMOTE_METHOD_NAME  "RemoteCall"

//
CenterListener::CenterListener(CenterCommandObserver *observer, int type) {
    mObserver = observer;

    mStatus = 0;
    mType = type;

    mThread = 0;
}

CenterListener::~CenterListener() {
    Stop();
}

CenterListener* CenterListener::NEW(CenterCommandObserver *observer, int type, short port) {
    CenterListener *self = new CenterListener(observer, type);
    if (self) {
        if (self->Construct(port)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int CenterListener::Start() {
    mStatus = 1;

    int err = 0;

    err = InitServerSock(&mSock, mPort);
    if (mThread == 0 && err == 0) {
        pthread_create(&mThread, NULL, Work, this);
    }

    return err;
}

void CenterListener::Stop() {
    if (mStatus > 0) {
        mStatus = 0;

        if (mSock.sock > 0) {
            close(mSock.sock);
            mSock.sock = 0;
        }
        if (mThread > 0) {
            pthread_cancel(mThread);
            pthread_join(mThread, NULL);
            mThread = 0;
        }
    }
}

int CenterListener::Construct(short port) {
    // if the socket create by root, then use the absolute path
    mPort = port;
    return 0;
}

int CenterListener::InitServerSock(LocalSocket *sock, short port) {
    sock->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->sock < 0) {
        return -1;
    }

    ///定义sockaddr_in
    sock->dest.sin_family = AF_INET;
    sock->dest.sin_port = htons(port);
    sock->dest.sin_addr.s_addr = htonl(INADDR_ANY);

    int err = bind(sock->sock,(struct sockaddr*)&sock->dest,sizeof(sock->dest));
    if(err < 0) {
        return -1;
    }

    err = listen(sock->sock, 1);
    if (err < 0) {
        return -1;
    }
    return 0;
}


void* CenterListener::Work(void *args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "Listener");
    pthread_setname_np(pthread_self(), threadName);
#endif

    CenterListener *self = (CenterListener*)args;
    self->WorkFunc();

    pthread_exit(0);
    return NULL;
}

void CenterListener:: WorkFunc() {
    socklen_t len = sizeof(mSock.dest);
    while(mStatus > 0) {
        int cfd = accept(mSock.sock, (struct sockaddr*)&mSock.dest, &len);
        if (cfd < 0) {
            break;
        }

        printf("received a request\n");

        int ret = 0;
        unsigned char *out = NULL;
        int outlen = 0;

        CommandBundle command;
        RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
        if (bundle.Internalize(cfd)) {
            if (mObserver) {
                ret = mObserver->OnCommandObserver(mType, &command, cfd);
            }
        }
        sleep(1);
        close(cfd);
    }
}


/// ProxyInfoListener
ProxyInfoListener* ProxyInfoListener::NEW(uint32_t addr, short port) {
    ProxyInfoListener *self = new ProxyInfoListener(addr, port);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

ProxyInfoListener::ProxyInfoListener(uint32_t addr, short port) {
    mStatus = 0;
    mAddr = addr;
    mPort = port;
}

ProxyInfoListener::~ProxyInfoListener() {

}

int ProxyInfoListener::Construct() {
    if ((mSockFd = socket(AF_INET,SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    int yes=1;
    if (setsockopt(mSockFd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        return -2;
    }

    memset(&mSockAddr, 0, sizeof(mSockAddr));
    mSockAddr.sin_family = AF_INET;
    mSockAddr.sin_addr.s_addr = mAddr;
    mSockAddr.sin_port = htons(mPort);

    if (bind(mSockFd, (struct sockaddr *)&mSockAddr,sizeof(mSockAddr))) {
        return -1;
    }

    return 0;
}

void ProxyInfoListener::Start() {
    mStatus = 1;
    pthread_create(&mThread, NULL, ThreadWork, this);
}

void ProxyInfoListener::Stop() {
    if (mStatus > 0) {
        mStatus = 0;

        pthread_cancel(mThread);
        pthread_join(mThread, NULL);
        mThread = 0;

        if (mSockFd > 0) {
            close(mSockFd);
            mSockFd = 0;
        }
    }
}

void* ProxyInfoListener::ThreadWork(void *params) {
    ProxyInfoListener *self = (ProxyInfoListener*)params;
    self->WorkFunc();
    return NULL;
}

void ProxyInfoListener::WorkFunc() {

}

int ProxyInfoListener::SendBroadcast(uint32_t addr, short port, unsigned char *data, int len, char *outdata) {
    struct sockaddr_in sock_addr;
    int fd = 0;
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        return -1;
    }

    memset(&sock_addr,0,sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = addr;
    sock_addr.sin_port = htons(port);

    struct timeval tv_timeout;
    tv_timeout.tv_sec = 5;
    tv_timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

    }
    tv_timeout.tv_sec = 5;
    tv_timeout.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

    }

    if (sendto(fd, data, len, 0, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        return -1;
    }

    int nbytes = 0;
    int addrlen = sizeof(addr);
    if ((nbytes = recvfrom(fd, outdata, 2048, 0, (struct sockaddr*)&addr, (socklen_t*)&addrlen)) < 0) {
        return -1;
    }
    outdata[nbytes] = '\0';

    close(fd);
    return 0;
}

