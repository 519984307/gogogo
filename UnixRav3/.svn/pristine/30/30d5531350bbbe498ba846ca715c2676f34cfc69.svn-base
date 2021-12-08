#include "centerlistener.h"
#include "json/json.h"
#include "commonapi.h"
#include "communicatebundle.h"
#include "baseconfig.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

//#include <sys/epoll.h>

#define MAX_PACKET 1024

//typedef enum EPOLL_EVENTS EVENTMASK;

//static int setnonblocking(int fd){
//    int flags;
//    if((flags=fcntl(fd,F_GETFL,0))==-1){
//        perror("fcntl get");
//        return -1;
//    }
//    flags|=O_NONBLOCK;
//    if(fcntl(fd,F_SETFL,flags)==-1){
//        perror("fcntl set");
//        return -1;
//    }
//    return 0;
//}

//static int ep_event_add(int epollfd,int fd, EVENTMASK eventmask) {
//    struct epoll_event ev;
//    ev.data.fd=fd;
//    ev.events=eventmask;
//    return epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
//}

//static int ep_event_mod(int epollfd,int fd, EVENTMASK eventmask) {
//    struct epoll_event ev;
//    ev.data.fd=fd;
//    ev.events=eventmask;
//    return epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
//}

//static int ep_event_del(int epollfd,int fd){
//    return epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,NULL);
//}

////
//CenterListener::CenterListener(CenterCommandObserver *observer, int type, bool systemType) {
//    mObserver = observer;

//    mStatus = 0;
//    mType = type;
//    mSystemType = systemType;

//    mThread = 0;
//    mEpollFd = 0;
//}

//CenterListener::~CenterListener() {
//    Stop();
//    close(mEpollFd);
//}

//CenterListener* CenterListener::NEW(CenterCommandObserver *observer, int type, const char *sock, bool systemType) {
//    CenterListener *self = new CenterListener(observer, type, systemType);
//    if (self) {
//        if (self->Construct(sock)) {
//            delete self;
//            self = NULL;
//        }
//    }
//    return self;
//}

//int CenterListener::Start() {
//    mStatus = 1;

//    int err = 0;

//    err = InitLocalSock(&mSock, mSockPath);
//    ep_event_add(mEpollFd, mSock.sock, EPOLLIN);

//    if (err == 0) {
//        pthread_create(&mThread, NULL, Work, this);
//    }
//    return err;
//}

//void CenterListener::Stop() {
//    if (mStatus > 0) {
//        mStatus = 0;

//        ep_event_del(mEpollFd, mSock.sock);

//        if (mSock.sock > 0) {
//            close(mSock.sock);
//            mSock.sock = 0;
//        }
//        if (mThread > 0) {
//            pthread_cancel(mThread);
//            pthread_join(mThread, NULL);
//            mThread = 0;
//        }
//    }
//}

//int CenterListener::Construct(const char *sock) {
//    // if the socket create by root, then use the absolute path
//    if (mSystemType) {
//        strcpy(mSockPath, sock);
//    }
//    else {
//        // if the socket create by user, socket path will be set in /tmp
//        char temp[64];
//        sprintf(temp, "/tmp/%d", getpid());
//        mkdir(temp, 0755);
//        sprintf(mSockPath, "%s/%s", temp, sock);
//    }
//#ifdef LOW_LINUX_VERSION
//    mEpollFd = epoll_create(1000);
//#else
//    mEpollFd = epoll_create(EPOLL_CLOEXEC);
//#endif
//    return 0;
//}

//int CenterListener::InitLocalSock(LocalSocket *sock, const char *path) {
//    unlink(path);

//    sock->sock = socket(AF_UNIX, SOCK_STREAM, 0);
//    if (sock->sock <= 0) {
//        return -1;
//    }

//    sock->dest.sun_family = AF_UNIX;
//    strncpy(sock->dest.sun_path, path, sizeof(sock->dest.sun_path)-1);

//    int ok = 1;
//    setsockopt(sock->sock, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok));

//    fcntl(sock->sock, F_SETFL, fcntl(sock->sock,F_GETFL) | O_NONBLOCK);

//    struct timeval timeout = {5, 0};
//    setsockopt(sock->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout,sizeof(struct timeval));
//    setsockopt(sock->sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout,sizeof(struct timeval));

//    int err = bind(sock->sock,(struct sockaddr*)&sock->dest,sizeof(sock->dest));
//    if(err < 0) {
//        return -1;
//    }

//    chmod(path, 0777);
//    err = listen(sock->sock, 1);
//    if (err < 0) {
//        return -1;
//    }
//    return 0;
//}

//void* CenterListener::Work(void *args) {
//    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

//    CenterListener *self = (CenterListener*)args;
//    self->WorkFunc();

//    pthread_exit(0);
//    return NULL;
//}

//void CenterListener::RespAccept(int cfd) {
//    int ret = 0;
//    unsigned char *out = NULL;
//    int outlen = 0;

//    CommandBundle command;
//    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
//    if (bundle.Internalize(cfd)) {
//        if (mObserver) {
//            ret = mObserver->OnCommandObserver(mType, &command, (void**)&out, (int*)&outlen);
//        }
//    }
//    int type = 0;
//    if (out) {
//        type = 1;
//        Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&type);
//        type = outlen;
//        Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&type);
//        Utils::OS::Write(cfd, type, (unsigned char*)out);
//        delete[] out;
//    }
//    else {
//        Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&type);
//        Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&ret);
//    }
//}

//void CenterListener:: WorkFunc() {
//    socklen_t len = sizeof(mSock.dest);

//    #define MAX_EVENTS 1000
//    struct epoll_event events[MAX_EVENTS];
//    int ret = 0;

//    while(mStatus > 0) {
//        int nev = epoll_wait(mEpollFd, events, MAX_EVENTS, 1000);
//        for (int i = 0; i < nev; ++i) {
//            if (events[i].data.fd == mSock.sock) {
//                while (true) {
//                    int cfd = accept(mSock.sock, (struct sockaddr*)&mSock.dest, &len);
//                    if (cfd > 0) {
//                        setnonblocking(cfd);

//                        struct epoll_event ev;
//                        ev.data.ptr = new PollData(cfd);
//                        ev.events = EPOLL_CTL_ADD;
//                        epoll_ctl(mEpollFd, EPOLL_CTL_ADD, cfd, &ev);
//                    }
//                    else {
//                        break;
//                    }
//                }
//            }
//            else if (events[i].events & EPOLLIN) {
//                CommandBundle command;
//                RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);

//                PollData *out = (PollData*)events[i].data.ptr;

//                if (bundle.Internalize(out->fd)) {
//                    command.mResendOvertime = bundle.GetOverTime();

//                    if (mObserver) {
//                        ret = mObserver->OnCommandObserver(mType, &command, (void**)&out->out, (int*)&out->outlen);
//                    }
//                    events[i].events = EPOLLOUT;
//                    epoll_ctl(mEpollFd, EPOLL_CTL_MOD, out->fd, &events[i]);
//                }
//                else {
//                    epoll_ctl(mEpollFd, EPOLL_CTL_DEL, out->fd, &events[i]);
//                    delete out;
//                }
//            }
//            else if (events[i].events & EPOLLOUT) {
//                int type = 0;
//                PollData *out = (PollData*)events[i].data.ptr;

//                bool finish = true;
//                do {
//                    if (out->out) {
//                        if (0 == out->extend) {
//                            type = 1;
//                            if (sizeof(int) != Utils::OS::Write(out->fd, sizeof(int), (unsigned char*)&type)) {
//                                break;
//                            }
//                            type = out->outlen;
//                            if (sizeof(int) != Utils::OS::Write(out->fd, sizeof(int), (unsigned char*)&type)) {
//                                break;
//                            }
//                        }

//                        int writed = Utils::OS::Write(out->fd, out->outlen-out->extend, (unsigned char*)(out->out+out->extend));
//                        if (out->outlen != writed+out->extend) {
//                            out->extend += writed;
//                            finish = false;
//                            break;
//                        }
//                    }
//                    else {
//                        if (sizeof(int) != Utils::OS::Write(out->fd, sizeof(int), (unsigned char*)&type)) {
//                            break;
//                        }
//                        if (sizeof(int) != Utils::OS::Write(out->fd, sizeof(int), (unsigned char*)&ret)) {
//                            break;
//                        }
//                    }
//                }while(0);
//                if (finish) {
//                    epoll_ctl(mEpollFd, EPOLL_CTL_DEL, out->fd, &events[i]);
//                    delete out;
//                }
//            }
//        }
//    }
//    printf("center listener thread exit\n");
//}


///// ProxyInfoListener
//ProxyInfoListener* ProxyInfoListener::NEW(uint32_t addr, short port) {
//    ProxyInfoListener *self = new ProxyInfoListener(addr, port);
//    if (self) {
//        if (self->Construct()) {
//            delete self;
//            self = NULL;
//        }
//    }
//    return self;
//}

//ProxyInfoListener::ProxyInfoListener(uint32_t addr, short port) {
//    mStatus = 0;
//    mAddr = addr;
//    mPort = port;
//}

//ProxyInfoListener::~ProxyInfoListener() {

//}

//int ProxyInfoListener::Construct() {
//    if ((mSockFd = socket(AF_INET,SOCK_DGRAM, 0)) < 0) {
//        return -1;
//    }

//    int yes=1;
//    if (setsockopt(mSockFd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
//        return -2;
//    }

//    memset(&mSockAddr, 0, sizeof(mSockAddr));
//    mSockAddr.sin_family = AF_INET;
//    mSockAddr.sin_addr.s_addr = mAddr;
//    mSockAddr.sin_port = htons(mPort);

//    if (bind(mSockFd, (struct sockaddr *)&mSockAddr,sizeof(mSockAddr))) {
//        return -1;
//    }

//    return 0;
//}

//void ProxyInfoListener::Start() {
//    mStatus = 1;
//    pthread_create(&mThread, NULL, ThreadWork, this);
//}

//void ProxyInfoListener::Stop() {
//    if (mStatus > 0) {
//        mStatus = 0;

//        pthread_cancel(mThread);
//        pthread_join(mThread, NULL);
//        mThread = 0;

//        if (mSockFd > 0) {
//            close(mSockFd);
//            mSockFd = 0;
//        }
//    }
//}

//void* ProxyInfoListener::ThreadWork(void *params) {
//    ProxyInfoListener *self = (ProxyInfoListener*)params;
//    self->WorkFunc();
//    return NULL;
//}

//void ProxyInfoListener::WorkFunc() {
//    while (mStatus) {
//        int addrlen = sizeof(mSockAddr);
//        int nbytes = 0;

//        if ((nbytes = recvfrom(mSockFd, mBuffer, 1024, 0, (struct sockaddr*)&mSockAddr, (socklen_t*)&addrlen)) > 0) {
//            mBuffer[nbytes] = '\0';
//            if (mObserver) {
//                char *out = NULL;
//                CommandBundle command;
//                if (command.Internalize((const char*)mBuffer, nbytes)) {
//                    if (mObserver) {
//                        int outlen = 0;
//                        mObserver->OnCommandObserver(CenterCommandObserver::CLINET_GROUP_BROADCAST, &command, (void**)&out, &outlen);
//                        if (out) {
//                            sendto(mSockFd, out, outlen, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
//                            delete[] out;
//                        }
//                    }
//                }
//             }
//        }
//    }
//}

//int ProxyInfoListener::SendBroadcast(uint32_t addr, short port, unsigned char *data, int len, char *outdata) {
//    struct sockaddr_in sock_addr;
//    int fd = 0;
//    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
//        return -1;
//    }

//    memset(&sock_addr,0,sizeof(sock_addr));
//    sock_addr.sin_family = AF_INET;
//    sock_addr.sin_addr.s_addr = addr;
//    sock_addr.sin_port = htons(port);

//    struct timeval tv_timeout;
//    tv_timeout.tv_sec = 5;
//    tv_timeout.tv_usec = 0;
//    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

//    }
//    tv_timeout.tv_sec = 5;
//    tv_timeout.tv_usec = 0;
//    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv_timeout, sizeof(struct timeval)) < 0) {

//    }

//    if (sendto(fd, data, len, 0, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
//        return -1;
//    }

//    int nbytes = 0;
//    int addrlen = sizeof(addr);
//    if ((nbytes = recvfrom(fd, outdata, SOCKET_BUFFER_MAX, 0, (struct sockaddr*)&addr, (socklen_t*)&addrlen)) < 0) {
//        return -1;
//    }
//    outdata[nbytes] = '\0';

//    close(fd);
//    return 0;
//}

#include "centerlistener.h"
#include "json/json.h"
#include "commonapi.h"
#include "communicatebundle.h"
#include "baseconfig.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#if defined(COMM_WITH_SOCKET) || defined(COMM_WITH_EV)
    #include <sys/socket.h>
    #include <fcntl.h>
#elif defined(COMM_WITH_DBUS)
    #include  <QtDBus/QtDBus>

    #define CLIENT_SEND_SOURCE  "client.signal.source"
    #define CLIENT_SERV_SOURCE  "client.method.source"
    #define CLIENT_OBJECT_NAME  "/rav/signal/Object"
    #define CLIENT_SIGNAL_TYPE  "rav.signal.Type"
    #define CLIENT_METHOD_TYPE  "rav.method.Type"
    #define SERVICE_INTERFACE   "com.rising.antivirus.interface"
    #define REMOTE_SIGNAL_NAME  "SendSignal"
    #define REMOTE_METHOD_NAME  "RemoteCall"
#endif

#define MAX_PACKET 1024

//
CenterListener::CenterListener(CenterCommandObserver *observer, int type, bool systemType) {
    mObserver = observer;

    mStatus = 0;
    mType = type;
    mSystemType = systemType;

#if defined (COMM_WITH_SOCKET) || defined (COMM_WITH_EV)
    mThread = 0;
#endif

#ifdef COMM_WITH_DBUS
    mRavObject = NULL;
#endif
}

CenterListener::~CenterListener() {
    Stop();

#ifdef COMM_WITH_DBUS
    if (mRavObject) {
        delete mRavObject;
        mRavObject = NULL;
    }
#endif
}

CenterListener* CenterListener::NEW(CenterCommandObserver *observer, int type, const char *sock, bool systemType) {
    CenterListener *self = new CenterListener(observer, type, systemType);
    if (self) {
        if (self->Construct(sock)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int CenterListener::Start() {
    mStatus = 1;

    int err = 0;
#if defined(COMM_WITH_SOCKET) || defined (COMM_WITH_EV)
    err = InitLocalSock(&mSock, mSockPath);
    if (err == 0) {
    #if defined(COMM_WITH_EV)
        looper = ev_loop_new(EVFLAG_AUTO);
    #endif
        pthread_create(&mThread, NULL, Work, this);
    }
#elif defined(COMM_WITH_DBUS)
    start();
#endif
    return err;
}

void CenterListener::Stop() {
    if (mStatus > 0) {
        mStatus = 0;

#if defined(COMM_WITH_SOCKET) || defined (COMM_WITH_EV)
        if (mSock.sock > 0) {
            close(mSock.sock);
            mSock.sock = 0;
        }
        if (mThread > 0) {
            pthread_cancel(mThread);
            pthread_join(mThread, NULL);
            mThread = 0;
        }
    #if defined (COMM_WITH_EV)
        ev_loop_destroy(looper);
    #endif
#elif defined(COMM_WITH_DBUS)
        QDBusConnection connection = mSystemType?QDBusConnection::systemBus():QDBusConnection::sessionBus();

        connection.unregisterObject(CLIENT_OBJECT_NAME);
        connection.unregisterService(mSockPath);
        connection.disconnectFromBus(connection.name());
        QThread::quit();
#endif

    }
}

int CenterListener::Construct(const char *sock) {
    // if the socket create by root, then use the absolute path
#if defined(COMM_WITH_SOCKET) || defined (COMM_WITH_EV)
    if (mSystemType) {
        strcpy(mSockPath, sock);
    }
    else {
        // if the socket create by user, socket path will be set in /tmp
        char temp[64];
        sprintf(temp, "/tmp/%d", getpid());
        mkdir(temp, 0755);
        sprintf(mSockPath, "%s/%s", temp, sock);
    }
#elif defined(COMM_WITH_DBUS)
    strcpy(mSockPath, sock);
#endif
    return 0;
}

#if defined (COMM_WITH_SOCKET) || defined (COMM_WITH_EV)
int CenterListener::InitLocalSock(LocalSocket *sock, const char *path) {
    unlink(path);

    sock->sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock->sock <= 0) {
        return -1;
    }

#ifdef COMM_WITH_EV
    SetNonblock(sock->sock);
    SetReuseAddr(sock->sock);
#endif

    sock->dest.sun_family = AF_UNIX;
    strncpy(sock->dest.sun_path, path, sizeof(sock->dest.sun_path)-1);

    int err = bind(sock->sock,(struct sockaddr*)&sock->dest,sizeof(sock->dest));
    if(err < 0) {
        return -1;
    }

    chmod(path, 0777);
    err = listen(sock->sock, 1);
    if (err < 0) {
        return -1;
    }
    return 0;
}
#endif

#ifdef COMM_WITH_EV
void CenterListener::SetNonblock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd,F_GETFL) | O_NONBLOCK);
}

void CenterListener::SetReuseAddr(int fd) {
    int ok=1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok));
}
#endif

#ifdef COMM_WITH_DBUS
int CenterListener::InitDBus(const char *conn_name) {
#ifdef LOW_DBUS_API
    DBusError err;
    dbus_error_init(&err);

    mDBusConnection = dbus_bus_get(mSystemType?DBUS_BUS_SYSTEM:DBUS_BUS_SESSION , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }

    if (NULL == mDBusConnection) {
        return -1;
    }

    dbus_bus_register(mDBusConnection, &err);
    dbus_bus_set_unique_name(mDBusConnection, "rav.comm.dbus");

    // request our name on the bus and check for errors
    int ret = dbus_bus_request_name(mDBusConnection, conn_name, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }
#endif

    return 0;
}
#endif


#ifdef COMM_WITH_DBUS
    void CenterListener::run(void) {
        QDBusConnection connection = mSystemType?QDBusConnection::systemBus():QDBusConnection::sessionBus();
        if(!connection.registerService(mSockPath)) {
            return;
        }

        if (mRavObject) {
            delete mRavObject;
        }
        mRavObject = new RavDBusObject(mObserver);
        connection.registerObject(CLIENT_OBJECT_NAME, mRavObject, QDBusConnection::ExportAllSlots);

        exec();
    }
#endif

#ifdef COMM_WITH_EV
CenterListener *global_self = NULL;
void CenterListener::do_accept(struct ev_loop* reactor, ev_io* w, int events) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    int conn = accept(w->fd, (struct sockaddr*)&addr,&addr_size);
    if (conn > 0 && global_self) {
        global_self->RespAccept(conn);
    }
    close(conn);
  }
#endif

#if defined (COMM_WITH_SOCKET) || defined (COMM_WITH_EV)
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

void CenterListener::RespAccept(int cfd) {
    int ret = 0;
    unsigned char *out = NULL;
    int outlen = 0;

    CommandBundle command;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::MEMORY, command);
    if (bundle.Internalize(cfd)) {
        if (mObserver) {
            ret = mObserver->OnCommandObserver(mType, &command, (void**)&out, (int*)&outlen);
        }
    }
    int type = 0;
    if (out) {
        type = 1;
        do {
            if (sizeof(int) != Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&type)) {
                break;
            }
            type = outlen;
            if (sizeof(int) != Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&type)) {
                break;
            }
            if (type != Utils::OS::Write(cfd, type, (unsigned char*)out)) {
                break;
            }
        }while(0);
        delete[] out;
    }
    else {
        if (sizeof(int) != Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&type)) {
            return;
        }
        if (sizeof(int) != Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&ret)) {
            return;
        }
    }
}

void CenterListener:: WorkFunc() {
#ifdef COMM_WITH_SOCKET
    socklen_t len = sizeof(mSock.dest);
    while(mStatus > 0) {
        int cfd = accept(mSock.sock, (struct sockaddr*)&mSock.dest, &len);
        if (cfd < 0) {
            break;
        }

        //中航财务，程序block at read function问题
        if(0)
        {
            struct timeval timeout = {0, 1000*1000};
            setsockopt(cfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
            setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));
            struct linger lger;
            lger.l_onoff = 1;
            lger.l_linger = 1;
            setsockopt(cfd, SOL_SOCKET, SO_LINGER, &lger, sizeof(lger));
        }

        RespAccept(cfd);
        close(cfd);
#elif defined (COMM_WITH_EV)
        global_self = this;

        printf("socket is %d\n", mSock.sock);

        ev_io_init(&evio, do_accept, mSock.sock, EV_READ);
        ev_io_start(looper, &evio);
        ev_run(looper, 0);
#endif
    }

#ifdef LOW_DBUS_API
//    for low level API
    DBusError err;
    dbus_error_init(&err);

    if (CenterCommandObserver::CLIENT_SIGNAL == mType) {
        char temp[128] = {0};
        sprintf(temp, "type='signal',interface='%s'", CLIENT_SIGNAL_TYPE);
        dbus_bus_add_match(mDBusConnection, temp, &err); // see signals from the given interface
        dbus_connection_flush(mDBusConnection);
        if (dbus_error_is_set(&err)) {
            return;
        }
    }

    while(mStatus > 0) {
        DBusMessage *msg;

        dbus_connection_read_write(mDBusConnection, 0);
        msg = dbus_connection_pop_message(mDBusConnection);
        if(msg == NULL) {
            usleep(1000);
            continue;
        }

        if(dbus_message_is_signal(msg, CLIENT_SIGNAL_TYPE, CLIENT_SIGNAL_NAME)){
            CommandBundle command;
            if (0 == GetMethodParams(msg, command.mCommandID, command.mStringValue, PATH_MAX_LENGTH)) {
                int err = 0;
                if (mObserver) {
                    err = mObserver->OnCommandObserver(mType, &command, NULL);
                }
            }
        }
        else if(dbus_message_is_method_call(msg, CLIENT_METHOD_TYPE, CLIENT_METHOD_NAME)) {
            if(strcmp(dbus_message_get_path(msg), CLIENT_OBJECT_NAME) == 0) {
                CommandBundle command;
                if (0 == GetMethodParams(msg, command.mCommandID, command.mStringValue, PATH_MAX_LENGTH)) {
                    int err = 0;
                    if (mObserver) {
                        err = mObserver->OnCommandObserver(mType, &command, NULL);

                    }
                    ReplyMethodCall(msg, mDBusConnection, err, NULL);
                }
            }
        }

        dbus_message_unref(msg);
    }
#endif
    printf("center listener thread exit\n");
}
#endif


#ifdef COMM_WITH_DBUS
int CenterListener::SendDBusSignal(int method, const char *data, bool toSystem) {
#ifdef LOW_DBUS_API
//    this block use libdbus low level function
//    error is listen is root , can't receive signal
//
    // initialise the error value
    DBusError err;
    dbus_error_init(&err);

    // connect to the DBUS system bus, and check for errors
    DBusConnection *conn = dbus_bus_get(toSystem?DBUS_BUS_STARTER:DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (NULL == conn) {
        return -1;
    }

    // register our name on the bus, and check for errors
    char tmpName[128] = {0};
    snprintf(tmpName, sizeof(tmpName), "%s%ld", CLIENT_SEND_SOURCE, Utils::Date::GetTimeStamp());
    int ret = dbus_bus_request_name(conn, tmpName, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }

    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }

    // create a signal & check for errors
    DBusMessage *msg = dbus_message_new_signal(CLIENT_OBJECT_NAME,   // object name of the signal
                                                CLIENT_SIGNAL_TYPE,  // interface name of the signal
                                                CLIENT_SIGNAL_NAME); // name of the signal
    if (NULL == msg) {
        return -1;
    }

    // append arguments onto signal
    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &method)) {
        dbus_message_unref(msg);
        dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);

        return -1;
    }

    if (dbus_validate_utf8(data, NULL)) {
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &data)) {
            dbus_message_unref(msg);
            dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);
            return -1;
        }
    }

    // send the message and flush the connection
    dbus_uint32_t serial = 0;
    if (!dbus_connection_send(conn, msg, &serial)) {
        dbus_message_unref(msg);
        dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);

        return -1;
    }
    dbus_connection_flush(conn);

    dbus_message_unref(msg);
    dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);
#endif
    return 0;
}

int CenterListener::SendDBusMethod(const char *dbus, int method, const char *params, bool toSystem) {
    // QtDBus
    QDBusMessage message = QDBusMessage::createMethodCall(
                dbus,
                CLIENT_OBJECT_NAME,
                SERVICE_INTERFACE,
                REMOTE_METHOD_NAME);

    message << method;
    if (NULL != params) {
        message << params;
    }

    QDBusMessage response = (toSystem?QDBusConnection::systemBus():QDBusConnection::sessionBus()).call(message);
    if (response.type() == QDBusMessage::ReplyMessage) {
        return response.arguments().takeFirst().toInt();
    }

#ifdef LOW_DBUS_API
    // this block use libdbus low level function
    // error is listen is root , can't receive signal
    // initialiset the errors
    DBusError err;
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    DBusConnection* conn = dbus_bus_get(toSystem?DBUS_BUS_STARTER:DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (NULL == conn) {
        return -1;
    }

    // request our name on the bus
    char tmpName[128] = {0};
    snprintf(tmpName, sizeof(tmpName), "%s%ld", CLIENT_SERV_SOURCE, Utils::Date::GetTimeStamp());
    int ret = dbus_bus_request_name(conn, tmpName, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }

    // create a new method call and check for errors
     DBusMessage* msg = dbus_message_new_method_call(
                            dbus,               // target for the method call
                            CLIENT_OBJECT_NAME, // object to call on
                            CLIENT_METHOD_TYPE, // interface to call on
                            CLIENT_METHOD_NAME); // method name
    if (NULL == msg) {
        return -1;
    }

    // append arguments
    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &method)) {
        return -1;
    }
    if (NULL != params) {
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &params)) {
            return -1;
        }
    }

    // send message and get a handle for a reply
    DBusPendingCall* pending;
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
        return -1;
    }
    if (NULL == pending) {
        return -1;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    // block until we recieve a reply
    dbus_pending_call_block(pending);
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg) {
        return -1;
    }
    // free the pending message handle
    dbus_pending_call_unref(pending);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args)) {
        return -1;
    }

    do  {
        if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&args)) {
            dbus_message_iter_get_basic(&args, &back);
        }
        else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&args)) {
            char *p = NULL;
            dbus_message_iter_get_basic(&args, &p);
            if (NULL != p && NULL != data) {
                strcpy(data, p);
            }
        }
    }while(dbus_message_iter_next(&args));

    // free reply
    dbus_message_unref(msg);
    dbus_bus_release_name(conn, CLIENT_SERV_SOURCE, &err);
#endif
    return -1;
}

#endif

#ifdef COMM_WITH_DBUS
#ifdef LOW_DBUS_API
int CenterListener::GetMethodParams(DBusMessage *msg, int &method, char *data, int max_len) {
    DBusMessageIter arg;
    if(!dbus_message_iter_init(msg, &arg)) {
        return -1;
    }

    do {
        if(dbus_message_iter_get_arg_type(&arg) == DBUS_TYPE_STRING) {
            char *temp = NULL;
            dbus_message_iter_get_basic(&arg, &temp);
            if (NULL != temp) {
                strncpy(data, temp, max_len-1);
            }
        }
        else if(dbus_message_iter_get_arg_type(&arg) == DBUS_TYPE_UINT32) {
            dbus_message_iter_get_basic(&arg, &method);
        }
    } while(dbus_message_iter_next(&arg));
    return 0;
}

int CenterListener::ReplyMethodCall(DBusMessage * msg, DBusConnection * conn, int state, const char *data) {
    //创建返回消息reply
    DBusMessageIter arg;
    if(!dbus_message_iter_init(msg, &arg)) {
        return -1;
    }

    DBusMessage *reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &arg);

    if(!dbus_message_iter_append_basic(&arg, DBUS_TYPE_UINT32, &state)){
        return -1;
    }
    if (NULL != data) {
        if (!dbus_message_iter_append_basic(&arg, DBUS_TYPE_STRING, &data)) {
            return -1;
        }
    }

    //发送返回消息
    uint32_t serial = 0;
    if( !dbus_connection_send(conn, reply, &serial)){
        return -1;
    }
    dbus_connection_flush (conn);
    dbus_message_unref (reply);
    return 0;
}
#endif
#endif

//void* CenterListener::Work(void *args) {
//    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);

//    CenterListener *self = (CenterListener*)args;
//    self->WorkFunc();

//    pthread_exit(0);
//    return NULL;
//}


//void CenterListener::WorkFunc() {
//    socklen_t len = sizeof(mAPISock.dest);
//    while(mStatus > 0) {
//        int cfd = accept(mAPISock.sock, (struct sockaddr*)&mAPISock.dest, &len);
//        if (cfd < 0) {
//            break;
//        }

//        PublicScanData scanfiles;
//        if (scanfiles.Internalize(cfd)) {
//            if (mObserver) {
//                PublicScanResult *result = NULL;
//                if (0 == mObserver->OnCommandObserver(
//                            CenterCommandObserver::PUBLIC_API_SCAN, (void*)&scanfiles, (void**)&result)) {

//                    if (result) {
//                        result->Externalize(cfd);
//                    }
//                }
//                if (result) {
//                    delete result;
//                }
//            }
//        }
//        close(cfd);
//    }
//}


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
    while (mStatus) {
        int addrlen = sizeof(mSockAddr);
        int nbytes = 0;

        if ((nbytes = recvfrom(mSockFd, mBuffer, 1024, 0, (struct sockaddr*)&mSockAddr, (socklen_t*)&addrlen)) > 0) {
            mBuffer[nbytes] = '\0';
            if (mObserver) {
                char *out = NULL;
                CommandBundle command;
                if (command.Internalize((const char*)mBuffer, nbytes)) {
                    if (mObserver) {
                        int outlen = 0;
                        mObserver->OnCommandObserver(CenterCommandObserver::CLINET_GROUP_BROADCAST, &command, (void**)&out, &outlen);
                        if (out) {
                            sendto(mSockFd, out, outlen, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
                            delete[] out;
                        }
                    }
                }
             }
        }
    }
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
    if ((nbytes = recvfrom(fd, outdata, SOCKET_BUFFER_MAX, 0, (struct sockaddr*)&addr, (socklen_t*)&addrlen)) < 0) {
        return -1;
    }
    outdata[nbytes] = '\0';

    close(fd);
    return 0;
}

