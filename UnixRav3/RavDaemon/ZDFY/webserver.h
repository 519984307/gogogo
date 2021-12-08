#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdio.h>
#include <pthread.h>

template<class T> class ThreadPool;
namespace WebService {
    class WebObserver {
    public:
        enum {
            GET,
            POST
        };
        virtual int OnWebRequestObserver(int event, const char *header, const char *body) = 0;
    };

    class Receiver {
    private:
        int mConnectFd;

    public:
        Receiver(char *domain_path);
        Receiver(int fd, char *domain_path);
        ~Receiver() {}

        void SetObserver(WebObserver *observer) {
            mObserver = observer;
        }

        void Response(char *message, int status);
        void FileResponse(int size, int status);
        void GetResponse(char *filename);
        void PostResponse(char *filename, char *argv);

        void Work();

    private:
        char mDomainPath[128];

        WebObserver *mObserver;
    };

    class Server {
    public:
        static Server* NEW(const char *path, unsigned short port, WebObserver* observer=NULL);
        ~Server();

        int Start();
        int Stop();

        void SetObserver(WebObserver *observer) {
            mObserver = observer;
        }

    private:
        Server(WebObserver* observer);
        int Construct(const char *path, unsigned short port);

        static void* ThreadWork(void *args);
        int Work();

    private:
        unsigned short mPort;
        char mDomainPath[256];

        ThreadPool<Receiver> *mThreadPool;

        int mServerFd;

        int mStatus;
        pthread_t mThread;

        WebObserver *mObserver;
    };
}

#endif // WEBSERVER_H
