#ifndef __RSUTIL_FAST_MUTEX__
#define __RSUTIL_FAST_MUTEX__

namespace syn
{

	#if (TARGET_OS==OS_WINDOWS) || !defined(TARGET_OS)

	struct FastMutex
	{
		CRITICAL_SECTION cri;
		FastMutex() { ::InitializeCriticalSection(&cri); }
		~FastMutex() { ::DeleteCriticalSection(&cri); }
		void Acquire() { ::EnterCriticalSection(&cri); }
		void Release() { ::LeaveCriticalSection(&cri); }
	};

	#elif (TARGET_OS==OS_POSIX)

	struct FastMutex
	{
		pthread_mutex_t _fmutex;
		FastMutex() { init(); }
		~FastMutex() { uninit(); }
		void Acquire() { lock(); }
		void Release() { unlock(); }

	private:
		virtual bool init()
		{
			pthread_mutexattr_t attr;    
			pthread_mutexattr_init(&attr);    
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);//设置锁的属性为可递归    
			if( pthread_mutex_init( &_fmutex, &attr ) < 0 )
				return false;
			return true;
		}
		virtual bool uninit()
		{
			if( pthread_mutex_destroy( &_fmutex ) < 0 )
				return false;
			return true;
		}
		virtual bool lock()
		{
			if( pthread_mutex_lock( &_fmutex ) < 0 )
				return false;
			return true;
		}
		virtual bool unlock()
		{
			if( pthread_mutex_unlock(&_fmutex ) < 0 )
				return false;
			return true;
		}
	};

	#elif (TARGET_OS==OS_NATIVE)

	struct FastMutex
	{
		FastMutex() {}
		~FastMutex() {}
		void Acquire() {}
		void Release() {}
	};

	#endif

	struct FastMutexScope
	{
		FastMutex& _Mutex;
		FastMutexScope( FastMutex& rMutex ) : _Mutex(rMutex)
		{
			_Mutex.Acquire();
		}
		~FastMutexScope()
		{
			_Mutex.Release();
		}
	};

};

#endif