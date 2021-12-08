#include "handler.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

Handler* Handler::NEW(HandleObserver *observer) {
    Handler *handler = new Handler(observer);
    if (handler) {
        if (handler->Construct()) {
            delete handler;
            handler = NULL;
        }
    }
    return handler;
}

Handler::Handler(HandleObserver *observer) {
    mObserver = observer;
    mFinishFlag = false;
}

Handler::~Handler() {
    mFinishFlag = true;
    pthread_cond_signal(&mCond);

    pthread_join(mLoopThread, NULL);
    pthread_mutex_destroy(&mMutex);
    pthread_cond_destroy(&mCond);
}

int Handler::Construct() {
    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mCond, NULL);
    return pthread_create(&mLoopThread, NULL, Looper, this);
}

void* Handler::Looper(void *data) {
#if defined (COMMON_LINUX)
    char threadName[32] = {0};
    sprintf(threadName, "Handler");
    pthread_setname_np(pthread_self(), threadName);
#endif

    Handler *handler = (Handler*)data;
    handler->HandleFunction();
    return NULL;
}

void Handler::HandleFunction() {
    do {
        pthread_mutex_lock(&mMutex);
        if (mMessageQueue.empty()) {
            pthread_cond_wait(&mCond, &mMutex);
        }

        if (!mFinishFlag) {
            Message& message = mMessageQueue.front();
            if (mObserver) {
                mObserver->HandleMessage(message.what, message.arg1, message.arg3);
            }

            //printf("qhash:%d. qmesg:%d\n", mQueueHash.size(), mMessageQueue.size());
            std::map<long, bool>::iterator iter = mQueueHash.find(message.Hash());
            if (iter != mQueueHash.end()) {
                mQueueHash.erase(iter);
            }
            mMessageQueue.pop();

        }
        pthread_mutex_unlock(&mMutex);

        //usleep(500000);
    }while (!mFinishFlag);
}

void Handler::PostMessage(const Message& message) {

    pthread_mutex_lock(&mMutex);
    std::map<long, bool>::iterator iter = mQueueHash.find(message.Hash());
    if (iter != mQueueHash.end()) {
        printf("the same message hash here!!!\n");
        pthread_mutex_unlock(&mMutex);
        return;
    }

    {
        mMessageQueue.push(message);
        mQueueHash.insert(std::pair<long, bool>(message.Hash(), true));
    }

    pthread_cond_signal(&mCond);

    pthread_mutex_unlock(&mMutex);

}


