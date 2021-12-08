#ifndef COMMONLOCKER_H
#define COMMONLOCKER_H

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

class SemLocker {
private:
    sem_t mSem;

public:
    //初始化信号量
    static SemLocker *NEW() {
        SemLocker *self = new SemLocker;
        if (self) {
            if (sem_init(&self->mSem, 0, 0)) {
                delete self;
                self = NULL;
            }
        }
        return self;
    }

    //销毁信号量
    ~SemLocker() {
        sem_destroy(&mSem);
    }

    //等待信号量
    bool wait() {
        return sem_wait(&mSem) == 0;
    }
    //添加信号量
    bool post() {
        return sem_post(&mSem) == 0;
    }
};

/*互斥 locker*/
class MutexLocker
{
private:
    pthread_mutex_t mMutex;

public:
    static MutexLocker *NEW() {
        MutexLocker *self = new MutexLocker;
        if (self) {
            if (pthread_mutex_init(&self->mMutex, NULL)) {
                delete self;
                self = NULL;
            }
        }
        return self;
    }

    ~MutexLocker() {
        pthread_mutex_destroy(&mMutex);
    }

    bool Lock() {
        return pthread_mutex_lock(&mMutex) == 0;
    }
    bool Unlock() {
        return pthread_mutex_unlock(&mMutex) == 0;
    }
    bool TryLock() {
        return pthread_mutex_trylock(&mMutex) == 0;
    }
};

/*条件变量 locker*/
class CondLocker {
private:
    pthread_mutex_t mMutex;
    pthread_cond_t  mCond;

public:
    // 初始化 m_mutex and m_cond
    static CondLocker* NEW() {
        CondLocker *self = new CondLocker;
        if (self) {
            if (pthread_mutex_init(&self->mMutex, NULL) || pthread_cond_init(&self->mCond, NULL)) {
                delete self;
                self = NULL;
            }
        }
        return self;
    }
    // destroy mutex and cond
    ~CondLocker() {
        pthread_mutex_destroy(&mMutex);
        pthread_cond_destroy(&mCond);
    }
    //等待条件变量
    bool Wait() {
        int ans = 0;
        pthread_mutex_lock(&mMutex);
        ans = pthread_cond_wait(&mCond, &mMutex);
        pthread_mutex_unlock(&mMutex);
        return ans == 0;
    }
    //唤醒等待条件变量的线程
    bool Signal() {
        return pthread_cond_signal(&mCond) == 0;
    }

    //唤醒all等待条件变量的线程
    bool Broadcast() {
        return pthread_cond_broadcast(&mCond) == 0;
    }
};

#endif // COMMONLOCKER_H
