#ifndef	__RS_Interlocked_H_H__
#define __RS_Interlocked_H_H__
//////////////////////////////////////////////////////////////////////////

#if TARGET_OS != OS_WINDOWS

#if (defined RS_BIG_ENDIAN) || (__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 1) || (__GNUC__ == 4 && __GNUC_MINOR__ == 1 && __GNUC_PATCHLEVEL__ < 2)

#warning ("not support sync series")

static FORCEINLINE LONG WINAPI InterlockedIncrement( LONG volatile *dest )
{
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&lock);
	LONG r = ++(*dest);
	pthread_mutex_unlock(&lock);
	return r;
}
static FORCEINLINE LONG WINAPI InterlockedDecrement( LONG volatile *dest )
{
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&lock);
	LONG r = --(*dest);
	pthread_mutex_unlock(&lock);
	return r;
}

#else

static FORCEINLINE LONG WINAPI InterlockedExchangeAdd( LONG volatile *dest, LONG incr )
{
	return __sync_fetch_and_add( dest, incr );
}
static FORCEINLINE LONG WINAPI InterlockedIncrement( LONG volatile *dest )
{
	return __sync_add_and_fetch( dest, 1 );
}
static FORCEINLINE LONG WINAPI InterlockedDecrement( LONG volatile *dest )
{
	return __sync_sub_and_fetch( dest, 1 );
}

#endif //defined RS_BIG_ENDIAN ...

#endif //TARGET_OS != OS_WINDOWS


#define REFABLE_IMP_MT	\
	virtual LONG _reference() { return InterlockedIncrement( &_ref_count ); };	\
	virtual LONG _release(){ LONG r = InterlockedDecrement( &_ref_count ); if( !r ) delete this; return r; };


#define REFABLE_IMP_ST REFABLE_IMP_MT 

#endif //__RS_Interlocked_H_H__
