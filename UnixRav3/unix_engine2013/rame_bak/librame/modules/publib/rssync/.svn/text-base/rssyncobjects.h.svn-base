#ifndef __SHARE_ALLOC__
#define __SHARE_ALLOC__





#define LOCK_TIMEOUT 5000

//////////////////////////////////////////////////////////////////////////



class IRSSync : public IUnknown 
{
public:
	virtual BOOL Lock(DWORD dwTimeOut) const= 0;
	virtual BOOL Unlock() const = 0;
};



//////////////////////////////////////////////////////////////////////////

class CRSSLock
{
public:
	CRSSLock(IRSSync* psync, DWORD dwInitLock = 0)
		:m_psync(psync), m_bLocked(FALSE)
	{
		if(dwInitLock)
		{
			Lock(dwInitLock);
		}
	}
	virtual ~CRSSLock()
	{
		Unlock();
	}

	BOOL Lock(DWORD dwTimeOut)
	{
		return (m_bLocked = m_psync->Lock(dwTimeOut));
	}

	BOOL Unlock()
	{
		if (m_bLocked)
			m_bLocked = !m_psync->Unlock();
		return !m_bLocked;
	}

	BOOL IsLocked() const
	{
		return m_bLocked;
	}

protected:
	const IRSSync *m_psync;
	BOOL m_bLocked;
};


//////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_TYPE_WIN32)
#include "rssync32.h"
#else
#include "rssyncnotimp.h"
#endif













#endif //_SHARE_ALLOC_

