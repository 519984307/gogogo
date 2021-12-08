#include "mutex.h"

extern void MyTrace(char *pszInfo, ...);

#ifdef WIN32_DEBUG
Mutex MutexCreate(unsigned int type, const char* name)
{
	return CreateMutexA(CDefaultSecurityAttribtes(), FALSE, name);
}

BOOL MutexLock(Mutex mtx, unsigned long millisec_wait)
{
	DWORD wait;

	wait = (DWORD)millisec_wait;
	if(wait < 1)
		wait = INFINITE;

	switch(WaitForSingleObject(mtx, wait))
	{
	case WAIT_ABANDONED:
		// We now have ownership, but the object is not signalled
		ReleaseMutex(mtx);
		return TRUE;
	case WAIT_OBJECT_0:
		return TRUE;
	case WAIT_TIMEOUT:
		return FALSE;
	default:
		return FALSE;
	}

	return FALSE;
}

void MutexUnlock(Mutex mtx)
{
	BOOL ret = ReleaseMutex(mtx);
}

void MutexDestroy(Mutex mtx)
{
	CloseHandle(mtx);
}
#else
#ifndef WIN32
#include <stdlib.h>		// for rand()
#include <errno.h>		// for errno
#include <string.h>		// for strlen()
#include <sys/types.h>	// for key_t
#include <sys/ipc.h>	// for some macros
#include <sys/sem.h>	// for semaphore related stuff
#include <unistd.h>		// for usleep()
#include <pthread.h>	// for pthread_mutex_t

// xuzilong 2004-12-08 mod <
#ifndef PLATFORM_TYPE_FREEBSD
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif // PLATFORM_TYPE_FREEBSD
// xuzilong 2004-12-08 mod >

#ifndef SEM_A
#define SEM_A			0200
#endif // !SEM_A
#ifndef SEM_R
#define SEM_R			0400
#endif // !SEM_R
#ifndef SEM_MODE
#define SEM_MODE		((SEM_R | SEM_A) | ((SEM_R | SEM_A) >> 3) | ((SEM_R | SEM_A) >> 6))
#endif // !SEM_MODE
#else // !WIN32
#include "defsa.h"
#endif // !WIN32

/*************************************************************************
** Helper stuff
*************************************************************************/

#ifndef ASSERT
#define ASSERT(x)	((void)0)
#endif // ASSERT

#ifndef TRACE
void fakeTrace(char*, ...)
{
}

#define TRACE			fakeTrace
#endif // TRACE

#ifdef WIN32
#define PREFIX_GLOBAL	"Global\\"
#else
int GetUniqueKey(const char* name, key_t* pkey);
#endif // WIN32

unsigned long ThreadSleep(unsigned long milliseconds);


/*************************************************************************
** Mutexes
*************************************************************************/

/**
	Creates a new mutex.

	\param mtx
		Where to store the new mutex. Must point to a variable of type
		'Mutex'. Must not be NULL.
	\return
		TRUE if the new mutex was successfully created, FALSE otherwise.
	\sa
		MutexDestroy
*/

#if !defined( _NOUSESHM_) && !defined(UNIXPLATFORM)
Mutex MutexCreate(unsigned int type, const char* name)
{
	if(type != MT_INTRAPROCESS
		&& type != MT_INTERPROCESS
#ifdef WIN32
		&& type != MT_GLOBAL
#endif // WIN32
		)
	{
		return NULL;
	}

	Muteks* pmtx = new Muteks;
	if(pmtx == NULL)
		return NULL;
	//MyTrace("Create MUTEX=%08x", (DWORD)pmtx);

	pmtx->type = type;
	pmtx->pobj = NULL;

	bool success = false;

#ifdef WIN32
	{
		switch(type)
		{
		case MT_INTRAPROCESS:
			pmtx->pobj = new CRITICAL_SECTION;
			if(pmtx->pobj != NULL)
				InitializeCriticalSection((LPCRITICAL_SECTION)pmtx->pobj);
			break;
		case MT_INTERPROCESS:
		case MT_GLOBAL:
			if(name && *name)	// there must be a name for inter-process mutex
			{
				int length = lstrlenA(name);
				int offset = 0;
				if(type == MT_GLOBAL)
					offset = lstrlenA(PREFIX_GLOBAL);
				length += offset;
				length += 1;

				char* mutex = new char[length];
				if(mutex != NULL)
				{
					if(type == MT_GLOBAL)
						lstrcpyA(mutex, PREFIX_GLOBAL);

					lstrcpyA(mutex + offset, name);
					pmtx->pobj = CreateMutexA(CDefaultSecurityAttribtes(), FALSE, mutex);
					//extern void MyTrace(char *pszInfo, ...);
					//MyTrace("Mutex Real Name=%s", mutex);
					delete mutex;
				}
			}
			break;
		}
	}
#else // WIN32
	{
		if(type == MT_INTRAPROCESS)
		{
			pmtx->pobj = new pthread_mutex_t;
			if(pmtx->pobj != NULL)
			{
				if(pthread_mutex_init((pthread_mutex_t*)pmtx->pobj, NULL) != 0)
				{
					delete (pthread_mutex_t*)pmtx->pobj;
					pmtx->pobj = NULL;
				}
				else
				{
					success = true;
				}
			}
		}
		else // MT_INTERPROCESS
		{
			// convert the name to a unique key
			key_t key;
			if(GetUniqueKey(name, &key) == 0)
			{
				int semid = semget(key, 1, IPC_CREAT | SEM_MODE);
				if(semid != -1)
				{
					// initialize it
					union semun ctlarg;
					ctlarg.val = 1;
					if(semctl(semid, 0, SETVAL, ctlarg) != -1)
					{
						pmtx->pobj = (void*)semid;
						success = true;
					}
					else
						semctl(semid, 0, IPC_RMID, 0);
				}
			}
		}
	}
#endif // WIN32

#ifdef WIN32
	success = (pmtx->pobj != NULL);
#endif // WIN32

	if(!success)
	{
		delete pmtx;
		pmtx = NULL;
	}

	return (Mutex)pmtx;
}

/**
	Attempts to aquire a lock for a given mutex. Waits for up to \p
	millisec_wait milliseconds for the lock, and then returns. If
	millisec_wait is zero, the function waits indefinately.

	\param mtx
		Address of the mutex to lock.
	\param millisec_wait
		How long to wait for a lock, in milliseconds. Use zero to have it
		wait indefinately.
	\return
		TRUE if the mutex was locked, FALSE if the wait time ran out or if
		there was an error.
*/
BOOL MutexLock(Mutex mtx, unsigned long millisec_wait)
{
	if(mtx == NULL)
		return FALSE;

	Muteks* pmtx = (Muteks*)mtx;

#ifdef WIN32
	{
		if(pmtx->type == MT_INTRAPROCESS)
		{
			BOOL ret = TRUE;
			__try
			{
				EnterCriticalSection((LPCRITICAL_SECTION)pmtx->pobj);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				ret = FALSE;
			}

			return ret;
		}
		else
		{
			int abandoned = 0;
			DWORD wait;

			wait = (DWORD)millisec_wait;
			if(wait < 1)
				wait = INFINITE;

			while(abandoned < 2)
			{
				switch(WaitForSingleObject((HANDLE)pmtx->pobj, wait))
				{
				case WAIT_ABANDONED:
					// We now have ownership, but the object is not signalled
					TRACE("MutexLock: WaitForSingleObject returned WAIT_ABANDONED.\n");
					// added by zhanglingang 2004.10.13 <
					//此为程序非法退出时状态，即无信号状态，这时应释放Mutex，获得信号，否则程序再一次启动时就会出现死等
					ReleaseMutex((HANDLE)pmtx->pobj);
					abandoned ++;
					return TRUE;
					// added by zhanglingang 2004.10.13 >
					break;
				case WAIT_OBJECT_0:
					return TRUE;
				case WAIT_TIMEOUT:
					return FALSE;
				default:
					TRACE("MutexLock: WaitForSingleObject returned unknown value.\n");
					return FALSE;
				}
			}
			TRACE("MutexLock: WaitForSingleObject won't stop returning WAIT_ABANDONED.\n");
		}
	}
#else // WIN32
	{
		if(pmtx->type == MT_INTRAPROCESS)
		{
			int err;
			pthread_mutex_t* p_mtx = (pthread_mutex_t*)pmtx->pobj;

			if(!millisec_wait)
			{
				err = pthread_mutex_lock(p_mtx);
				if(err == 0)
					return TRUE;

				return FALSE;
			}

			unsigned long waited = 0;
			while(waited < millisec_wait)
			{
				err = pthread_mutex_trylock(p_mtx);
				switch(err)
				{
				case 0:
					return TRUE;
				case EBUSY:
					waited += ThreadSleep(rand() % 10 + 5);
					break;
				case EINVAL:
				default:
					return FALSE;
				}
			}
		}
		else
		{
			int semid = (int)pmtx->pobj;

			unsigned long waited = 0;
			struct sembuf oparg;
			while(waited < millisec_wait)
			{
				oparg.sem_num = 0;
				oparg.sem_op = -1;
				oparg.sem_flg = IPC_NOWAIT;
				if(semop(semid, &oparg, 1) != -1)
					return TRUE;

				waited += ThreadSleep(rand() % 10 + 5);
			}
		}
	}
#endif // WIN32
	return FALSE;
}

/**
	Unlocks (releases) the specified mutex.

	\param mtx
		Address of the mutex to unlock.
*/
void MutexUnlock(Mutex mtx)
{
	if(mtx == NULL)
		return;

	Muteks* pmtx = (Muteks*)mtx;

#ifdef WIN32
	{
		if(pmtx->type == MT_INTRAPROCESS)
			LeaveCriticalSection((LPCRITICAL_SECTION)pmtx->pobj);
		else
		{
			if(ReleaseMutex((HANDLE)pmtx->pobj) == FALSE)
			{
				TRACE("MutexUnlock: ReleaseMutex(%p) failed with code %d.\n", mtx, GetLastError());
			}
		}
	}
#else // WIN32
	{
		if(pmtx->type == MT_INTRAPROCESS)
		{
			int errcode;
			pthread_mutex_t* p_mtx = (pthread_mutex_t*)pmtx->pobj;

			if((errcode = pthread_mutex_unlock(p_mtx)) != 0)
			{
				TRACE("MutexUnlock: pthread_mutex_unlock(%p) failed with code %d.\n", mtx, errcode);
			}
		}
		else
		{
			int semid = (int)pmtx->pobj;

			struct sembuf oparg;
			oparg.sem_num = 0;
			oparg.sem_op = 1;
			oparg.sem_flg = IPC_NOWAIT;	// no wait, or 0
			if(semop(semid, &oparg, 1) != 0)
			{
				TRACE("MutexUnlock: semop(%p) failed with code %d.\n", mtx, errno);
			}
		}
	}
#endif // WIN32
	return;
}

/**
	Destroys (removes from memory) a mutex. Attempting to use the mutex
	after it's been destroyed results in undefined behaviour.
	\param mtx
		Address of the mutex to destroy.
	\sa MutexCreate
*/
void MutexDestroy(Mutex mtx)
{
	if(mtx == NULL)
		return;

	Muteks* pmtx = (Muteks*)mtx;

#ifdef WIN32
	{
		if(pmtx->type == MT_INTRAPROCESS)
		{
			DeleteCriticalSection((LPCRITICAL_SECTION)pmtx->pobj);
			delete (LPCRITICAL_SECTION)pmtx->pobj;
		}
		else
			CloseHandle((HANDLE)pmtx->pobj);
	}
#else // WIN32
	{
		if(pmtx->type == MT_INTRAPROCESS)
		{
			pthread_mutex_t* p_mtx = (pthread_mutex_t*)pmtx->pobj;

			if(pthread_mutex_destroy(p_mtx) != 0)
			{
				TRACE("MutexUnlock: pthread_mutex_destroy(%p) failed.\n", mtx);
			}

			delete (pthread_mutex_t*)p_mtx;
		}
		else
		{
			int semid = (int)pmtx->pobj;
			if(semctl(semid, 0, IPC_RMID, 0) == -1)
			{
				TRACE("MutexUnlock: semctl(%p) failed.\n", mtx);
			}
		}
	}
#endif // WIN32

	//MyTrace("Delete MUTEX=%08x", (DWORD)pmtx);
	delete pmtx;
	return;
}

unsigned long ThreadSleep(unsigned long milliseconds)
{
	unsigned long slept = 0;
#ifdef WIN32
	Sleep((DWORD)milliseconds);
	slept = milliseconds;
#else // WIN32
	{
		while(milliseconds > 100)
		{
			slept += 100;
			milliseconds -= 100;
			usleep(100000);
		}
		
		usleep(milliseconds * 1000);
		slept += milliseconds;
	}
#endif // WIN32
	return slept;
}

#ifndef WIN32
unsigned int DJBHash(const char* str)
{
	unsigned int hash = 5381;
	unsigned int length = strlen(str);
	for(unsigned int i = 0; i < length; i++)
		hash = ((hash << 5) + hash) + str[i];

	return (hash & 0x7FFFFFFF);
}

int GetUniqueKey(const char* name, key_t* pkey)
{
	// convert the name to a unique key(a hash algorithm used)
	if(name == NULL || pkey == NULL)
		return -1;

	*pkey = (key_t)DJBHash(name);
	return 0;
}
#endif //#if !defined( _NOUSESHM_) && !defined(UNIXPLATFORM)
#endif // !WIN32

#endif // WIN32_DEBUG

/* EOF */
