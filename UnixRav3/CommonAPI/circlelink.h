#ifndef CIRCLELINK_H
#define CIRCLELINK_H

#include <stdio.h>
#include <pthread.h>

template<class T>
class CircleLinkNode {
public:
    T       node;
    CircleLinkNode<T>   *next;
    CircleLinkNode<T>   *previous;
};

template<class T>
class CircleLink
{
public:
    static CircleLink* NEW(int length) {
        CircleLink *self = new CircleLink();
        if (self) {
            int err = self->Construct(length);
            if (err) {
                delete self;
                self = NULL;
            }
        }
        return self;
    }

    ~CircleLink() {
        pthread_mutex_destroy(&mDataMutex);

        CircleLinkNode<T> *current = mDatas;
        for (int i=0; i<mLength; ++i) {
            CircleLinkNode<T> *next = current->next;
            delete current;
            current = next;
        }
    }

    void Append(const T& data) {
        if (0 == pthread_mutex_trylock(&mDataMutex)) {
            mDatas = mDatas->next;
            mDatas->node = data;

            pthread_mutex_unlock(&mDataMutex);
        }
    }

    void Reset(const T& data) {
        CircleLinkNode<T> *current = mDatas;
        for (int i=0; i<mLength; ++i) {
            CircleLinkNode<T> *next = current->next;
            next->node = data;
        }
    }

    inline unsigned int Length() const {
        return mLength;
    }

    inline CircleLinkNode<T>* Datas() const {
        return mDatas;
    }

    T Current() {
        T temp;
        if (0 == pthread_mutex_trylock(&mDataMutex)) {
            temp = mDatas->node;
            pthread_mutex_unlock(&mDataMutex);
        }
        return temp;
    }


protected:
    CircleLink() {
        mLength = 0;
        mDatas = NULL;
    }

    int Construct(int length) {
        mLength = length;
        pthread_mutex_init(&mDataMutex, NULL);

        CircleLinkNode<T> *current = NULL;
        for (int i=0; i<length; ++i) {
            CircleLinkNode<T> *data = new CircleLinkNode<T>();
            if (NULL == data) {
                return -1;
            }

            if (NULL == mDatas) {
                mDatas = data;
            }
            data->next = current;
            current = data;
        }
        mDatas->next = current;

        for (int i=0; i<length; ++i) {
            current->previous = mDatas;
            mDatas = current;
            current = current->next;
        }
        return 0;
    }

private:
    int mLength;
    CircleLinkNode<T> *mDatas;

    pthread_mutex_t mDataMutex;
};

#endif // CIRCLELINK_H
