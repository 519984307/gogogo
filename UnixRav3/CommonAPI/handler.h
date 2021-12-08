#ifndef HANDLER_H
#define HANDLER_H

#include <queue>
#include <map>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

class HandleObserver {
public:
    virtual void HandleMessage(int event, int type, void *object) = 0;
};

class Message {
public:
    Message(int what, int arg1, int arg2) {
        this->what = what;
        this->arg1 = arg1;
        this->arg2 = arg2;

//        srand((int)time(0));
        hash = what + arg1 + arg2 + (int)(1000000*rand());
    }

public:
    int what;
    int arg1;
    int arg2;
    void *arg3;

    inline long Hash() const {
        return hash;
    }

private:
    long hash;
};

class Handler
{
public:
    static Handler* NEW(HandleObserver *observer);
    virtual ~Handler();

private:
    Handler(HandleObserver *observer);
    int Construct();

public:
    void PostMessage(const Message& message);

private:
    static void* Looper(void *);
    void HandleFunction();

private:
    HandleObserver *mObserver;

    pthread_t       mLoopThread;
    pthread_mutex_t mMutex;
    pthread_cond_t  mCond;

    bool mFinishFlag;

    std::queue<Message>     mMessageQueue;
    std::map<long, bool>    mQueueHash;
};

#endif // HANDLER_H
