#ifndef __RSSYNC32_H__
#define __RSSYNC32_H__
#if defined(PLATFORM_TYPE_WINDOWS) && !defined(PLATFORM_TYPE_NATIVE)


#include <assert.h>




class CSecurityACL
{
private:
	SECURITY_ATTRIBUTES m_stSecurityAttr;
	SECURITY_DESCRIPTOR m_stSecurityDesc;

	static CSecurityACL sm_Instance;

	CSecurityACL();

	BOOL		InitSecurityDACL();

public:
	static CSecurityACL* Instance();
	SECURITY_ATTRIBUTES* GetSA() { return &m_stSecurityAttr; }
};

//////////////////////////////////////////////////////////////////////////

class CRSSyncWin32 : public IRSSync
{
public:
	CRSSyncWin32():m_h(NULL){}
	virtual ~CRSSyncWin32()
	{
		if (m_h)
		{
			::CloseHandle(m_h);
			m_h = NULL;
		}
	}

	virtual BOOL Lock(DWORD dwTimeOut) const
	{
		assert(m_h);

		DWORD dwRet = ::WaitForSingleObject(m_h, dwTimeOut);

		return (WAIT_ABANDONED == dwRet || WAIT_OBJECT_0 == dwRet);
	}
protected:
	HANDLE m_h;
};



class CRSMutexWin32 : public CRSSyncWin32, public CUnknownImp
{
public:
	//UNKNOWN_IMP1(IRSSync);
	UNKNOWN_IMP_SPEC(;);
public:
	CRSMutexWin32(const CHAR *pszName, 
		BOOL bInitOwn = FALSE, 
		LPSECURITY_ATTRIBUTES pSecurAtt = NULL)
	{
		assert(NULL == m_h);
		OSVERSIONINFO osver;

		osver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

	if (	::GetVersionEx( &osver ) && 
		osver.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		(osver.dwMajorVersion >= 6 ) )//vista
	{
		CHAR pszTemp[MAX_PATH];
		sprintf(pszTemp,"Global\\%s",pszName);
		m_h = CreateMutex(pSecurAtt, bInitOwn, pszTemp);
	}
	else
		m_h = CreateMutex(pSecurAtt, bInitOwn, pszName);
	}
	virtual ~CRSMutexWin32()
	{}

	virtual BOOL Unlock() const
	{
		assert(m_h);
		return ReleaseMutex(m_h);
	}
};



inline
HRESULT CreateRSSyncObj(LPCSTR pszSyncName, IRSSync** ppSyncObj)
{
	*ppSyncObj = RSNEW CRSMutexWin32(pszSyncName,FALSE, CSecurityACL::Instance()->GetSA());
	RASSERT(*ppSyncObj, E_OUTOFMEMORY);
	(*ppSyncObj)->AddRef();
	return S_OK;
}



#endif
#endif //#ifndef RSSYNC_H_
