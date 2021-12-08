#ifndef _SYNC_OS_TRAITS__
#define _SYNC_OS_TRAITS__

namespace syncost
{
#if TARGET_OS == OS_WINDOWS

	struct EveryOne
	{
		SECURITY_ATTRIBUTES mSecAttrs;
		SECURITY_DESCRIPTOR mSecDesc;
		LPSECURITY_ATTRIBUTES GetSecurityAttributes() 
		{
			return &mSecAttrs;
		}
		EveryOne()
		{
			mSecAttrs.nLength = sizeof(SECURITY_ATTRIBUTES);
			mSecAttrs.lpSecurityDescriptor = NULL;
			mSecAttrs.bInheritHandle = TRUE;
			if( InitializeSecurityDescriptor( &mSecDesc, SECURITY_DESCRIPTOR_REVISION ) &&
				SetSecurityDescriptorDacl( &mSecDesc, TRUE,(PACL)NULL, FALSE) )
				mSecAttrs.lpSecurityDescriptor = &mSecDesc;
		}
	};

	static EveryOne EVERYONE;

#endif

	struct ZMutex
	{
		virtual bool init( const void* p, const void * prefix ){ return false; }
		virtual bool uninit(){ return false; }
		virtual bool lock(){ return false; }
		virtual bool unlock(){ return false; }
		virtual ~ZMutex() { };
	};

	//struct ZMutexPolicy
	//{
	//	ZMutex& _mutex;
	//	ZMutexPolicy( ZMutex& mutex ) : _mutex(mutex)
	//	{}
	//	~ZMutexPolicy()
	//	{
	//		_mutex.uninit();
	//	}
	//};

	struct MutexScope
	{
	protected:
		ZMutex& _z;
		bool	_locked;
	public:
		bool	locked() { return _locked; }
		MutexScope( ZMutex& z ) : _z(z)
		{
			_locked = _z.lock();
		}
		~MutexScope()
		{
			if( _locked ) _z.unlock();
		}
	};

#if (TARGET_OS == OS_WINDOWS)

	struct FastMutex : ZMutex
	{
		CRITICAL_SECTION _fmutex;
		~FastMutex() { uninit(); };
		virtual bool init( const void* p, const void * prefix )
		{
			InitializeCriticalSection(&_fmutex);
			return true;
		}
		virtual bool uninit()
		{
			DeleteCriticalSection(&_fmutex);
			return true;
		}
		virtual bool lock()
		{
			EnterCriticalSection( &_fmutex );
			return true;
		}
		virtual bool unlock()
		{
			LeaveCriticalSection( &_fmutex );
			return true;
		}
	};

	struct NamedMutex : ZMutex
	{
		HANDLE	_hMutex;
		NamedMutex() : _hMutex(NULL)
		{
		}
		~NamedMutex() { uninit(); }
		virtual bool init( const void* p, const void * prefix )
		{
			if( !prefix )
			{
				_hMutex = ::CreateMutexA( EVERYONE.GetSecurityAttributes(), 0, (LPCSTR)p );
			}
			else
			{
				std::string aName = (LPCSTR)prefix;
				aName.append( (LPCSTR)p );
				_hMutex = ::CreateMutexA( EVERYONE.GetSecurityAttributes(), 0, aName.c_str() );
			}
			return _hMutex != NULL;
		}
		virtual bool uninit()
		{
			if( _hMutex ) ::CloseHandle(_hMutex);
			_hMutex = NULL;
			return true;
		}
		virtual bool lock()
		{
			if( !_hMutex ) return false;
			DWORD wr = WaitForSingleObject( _hMutex, -1 );
			if( wr != WAIT_OBJECT_0 ) return false;
			return true;
		}
		virtual bool unlock()
		{
			if( !_hMutex ) return false;
			return ReleaseMutex(_hMutex)!=FALSE;
		}
	};

#else

	struct FastMutex : ZMutex
	{
		pthread_mutex_t _fmutex;
		~FastMutex() { uninit(); };
		virtual bool init( const void* p, const void * prefix )
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

#ifdef USE_SEMAPHORE
	struct NamedMutex : ZMutex
	{
		sem_t*			_sem;
		std::string		_nam;
		NamedMutex() :	_sem(0) {};
		~NamedMutex() { uninit(); };
		virtual bool init( const void* p, const void * prefix )
		{
			_nam = "/"; 
			if( prefix ) _nam.append( (const char*)prefix);
			_nam.append( (const char*)p );
			sem_t* sem = sem_open( _nam.c_str(), O_CREAT, 0777, 1 );
			if( !sem ) return false;
			_sem = sem;
			return true;
		}
		virtual bool uninit()
		{
			if( !_sem ) return false;
			sem_close(_sem);
			sem_unlink(_nam.c_str());
			return true;
		}
		virtual bool lock()
		{
			if( _sem < 0 ) return false;
			int r = sem_wait( _sem );
			if( r < 0 ) return false;
			return true;
		}
		virtual bool unlock()
		{
			if( _sem < 0 ) return false;
			int r = sem_post( _sem );
			if( r < 0 ) return false;
			return true;
		}
	};
#else
	#include<sys/file.h>
	struct NamedMutex : ZMutex
	{
		int				_fd;
		std::string		_nam;
		NamedMutex() :	_fd(0) {};
		~NamedMutex() { uninit(); };
		virtual bool init( const void* p, const void * prefix )
		{
			int fd = init_and_open("/dev/shm/", p, prefix );
			if( -1 == fd )
			{
				fd = init_and_open("/", p, prefix );
				if( -1 == fd )
					return false;
			}
			_fd = fd;
			return true;
		}
		virtual bool uninit()
		{
			if( !_fd ) return false;
			close(_fd);
			unlink(_nam.c_str());
			return true;
		}
		virtual bool lock()
		{
			if( _fd < 0 ) return false;
			int r = flock( _fd, LOCK_EX );
			if( r != 0 ) return false;
			return true;
		}
		virtual bool unlock()
		{
			if( _fd < 0 ) return false;
			int r = flock( _fd, LOCK_UN );
			if( r < 0 ) return false;
			return true;
		}
	private:
		int init_and_open(const void* root, const void* p, const void * prefix)
		{
			_nam = (const char*)root;
			if( prefix ) _nam.append( (const char*)prefix);
			_nam.append( (const char*)p );
			int fd = open( _nam.c_str(), O_WRONLY|O_CREAT );
			//if( -1 == fd)
			//	printf("open %s failed. errorno(%d):%s\n", _nam.c_str(), errno, strerror(errno));
			return fd;
		}
	};
#endif //#ifdef USE_SEM

#endif

	struct FileMutex : NamedMutex
	{
		virtual bool init( const void* fn, const void * prefix )
		{
			LPCSTR aName = (const char* )fn;
			std::string aMutexName;
			for( ; *aName; ++aName )
			{
				char ch = ::toupper(*aName);
				if( ch=='\\' || ch == '/' ) aMutexName.push_back('!');
				else aMutexName.push_back(ch);
			}
			return NamedMutex::init( aMutexName.c_str(), prefix );
		}

	};


	//struct FileMutexW : NamedMutex
	//{
	//	virtual bool init( const void* fn, const void * prefix )
	//	{
	//		LPCWSTR wName = (LPCWSTR)fn;
	//		std::string aMutexName;
	//		for( ; *wName; ++wName )
	//		{
	//			char ch = ::toupper(*wName);
	//			if( ch=='\\' || ch == '/' ) 
	//			{
	//				aMutexName.push_back('!');
	//				break;
	//			}
	//			else aMutexName.push_back(ch);
	//		}


	//		md5_byte_t  digest[16];
	//		md5_state_s md5;
	//		md5_init(&md5);
	//		md5_append(&md5 , (BYTE*)wName , rs_wcslen(wName) * 2);
	//		md5_finish(&md5 , digest);

	//		CHAR chs[4] = {0};
	//		for (UINT16 idx = 0 ; idx < 16; idx++)
	//		{
	//			sprintf( chs, "%02X", digest[idx] );
	//			aMutexName.append(chs);
	//		}



	//		LPCWSTR lpSlash = NULL;
	//		for (INT32 idx = rs_wcslen(wName) ; idx > 0 ; idx--)
	//		{
	//			if(*(wName+idx)=='\\' || *(wName+idx) == '/' )
	//			{
	//				lpSlash = wName+idx;
	//				break;
	//			}
	//		}


	//		for( ; *lpSlash; ++ lpSlash )
	//		{
	//			char ch = ::toupper( *lpSlash );
	//			if( ch==L'\\' || ch == L'/' )
	//			{
	//				aMutexName.push_back('!');
	//			}
	//			else
	//				aMutexName.push_back(ch);
	//		}


	//		return NamedMutex::init( aMutexName.c_str(), prefix );
	//	}

	//};

};

#endif