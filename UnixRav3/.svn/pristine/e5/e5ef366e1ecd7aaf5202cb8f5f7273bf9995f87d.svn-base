#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "commonlocker.h"

#include <queue>
#include <stdio.h>
#include <exception>
#include <errno.h>
#include <pthread.h>
#include <iostream>

template<class T>
class ThreadPool
{
private:
    /** 线程池的线程数 */
    int mThreadNumber;

    /** 线程数组 */
    pthread_t   *mThreadPool;

    /** 任务队列 */
    std::queue<T*> mTaskQueue;

    /** 互斥锁 */
    MutexLocker *mQueueMutex;
    /** cond */
    CondLocker  *mQueueCond;

    /** 是否结束线程 */
    bool mStop;

private:
    int Construct(int thread_num) {
        if (thread_num<=0) {
            return -1;
        }

        mThreadNumber = thread_num;
        mQueueMutex = MutexLocker::NEW();
        if (NULL == mQueueMutex) {
            return -1;
        }
        mQueueCond = CondLocker::NEW();
        if (NULL == mQueueCond) {
            return -1;
        }
        mThreadPool = new pthread_t[thread_num];
        if (NULL == mThreadPool) {
            return -1;
        }
        return 0;
    }

    /** 线程运行的函数。执行run()函数 */
    static void *Worker(void *arg) {
        ThreadPool *pool = (ThreadPool*)arg;
        pool->Run();
        return pool;
    }

    void Run() {
        while(!mStop){
            T *task = GetTask();
            if (task == NULL) {
                mQueueCond->Wait();
            }
            else {
                task->Work();
            }
            delete task;
        }
    }

    T *GetTask() {
        T *task = NULL;
        mQueueMutex->Lock();
        if(!mTaskQueue.empty()) {
            task = mTaskQueue.front();
            mTaskQueue.pop();
        }
        mQueueMutex->Unlock();
        return task;
    }

public:
    static ThreadPool* NEW(int thread_num = 5) {
        ThreadPool *self = new ThreadPool;
        if (self) {
            if (self->Construct(thread_num)) {
                delete self;
                self = NULL;
            }
        }
        return self;
    }

    ThreadPool() {
        mThreadNumber = 0;
        mThreadPool = NULL;
        mStop = false;
    }

    ~ThreadPool() {
        Stop();
        if (mThreadPool) {
            delete []mThreadPool;
        }
        if (mQueueMutex) {
            delete mQueueMutex;
            mQueueMutex = NULL;
        }
        if (mQueueCond) {
            delete mQueueCond;
            mQueueCond = NULL;
        }
    }

    bool AppendTask(T *task) {
        //获取互斥锁
        mQueueMutex->Lock();

        bool is_signal = mTaskQueue.empty();
        //添加进入队列
        mTaskQueue.push(task);
        mQueueMutex->Unlock();
        //唤醒等待任务的线程
        if(is_signal) {
             mQueueCond->Signal();
        }
        return true;
    }

    void Start() {
        for(int i = 0; i < mThreadNumber; ++i) {
            if(pthread_create(mThreadPool + i, NULL, Worker, this) != 0) {
                //创建线程失败，清除成功申请的资源并抛出异常
                delete []mThreadPool;
                throw std::exception();
            }

            if(pthread_detach(mThreadPool[i])) {
                //将线程设置为脱离线程，失败则清除成功申请的资源并抛出异常
                delete []mThreadPool;
                throw std::exception();
            }
        }
    }

    void Stop() {
        mStop = true;
        mQueueCond->Broadcast();
    }


};


#endif // THREADPOOL_H
