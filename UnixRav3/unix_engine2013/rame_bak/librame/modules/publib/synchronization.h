#ifndef __SYNCHRONIZATION_H__
#define __SYNCHRONIZATION_H__

#ifndef _WIN32
#include <pthread.h>
#endif

template<class SyncObj>
class TLock
{
	TLock(const TLock &);
	TLock & operator = (const TLock &);
public:
	SyncObj& m_so;
	TLock(SyncObj& so) : m_so(so) { m_so.Lock(); }
	~TLock() { m_so.Unlock(); }
};

struct CCriticalSection
{
#ifdef _WIN32
	CRITICAL_SECTION m_cs;
	CCriticalSection() { ::InitializeCriticalSection(&m_cs); }
	~CCriticalSection() { ::DeleteCriticalSection(&m_cs); }
	void Lock() { EnterCriticalSection(&m_cs); }
	void Unlock() { LeaveCriticalSection(&m_cs); }
#else
	pthread_mutex_t _mutex;
	CCriticalSection() { pthread_mutex_init(&_mutex, 0); }
	void Lock() { pthread_mutex_lock(&_mutex); }
	void Unlock() { pthread_mutex_unlock(&_mutex); }
#endif
};

typedef TLock<CCriticalSection> CCSLock;


#endif //__SYNCHRONIZATION_H__
