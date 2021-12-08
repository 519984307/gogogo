#ifndef __RSSYNC32_H__
#define __RSSYNC32_H__







class CRSSyncNotImp : public IRSSync
{
public:
	CRSSyncNotImp(){}
	virtual ~CRSSyncNotImp()	{}

	virtual BOOL Lock(DWORD dwTimeOut) const
	{
		return TRUE;
	}
};



class CRSMutexNotImp : public CRSSyncNotImp, public CUnknownImp
{
public:
	UNKNOWN_IMP_SPEC(;);
public:
	CRSMutexNotImp(){}
	virtual ~CRSMutexNotImp(){}

	virtual BOOL Unlock() const
	{
		return TRUE;
	}
};


inline
HRESULT CreateRSSyncObj(LPCSTR pszSyncName, IRSSync** ppSyncObj)
{
	*ppSyncObj = RSNEW CRSMutexNotImp();
	RASSERT(*ppSyncObj, E_OUTOFMEMORY);
	(*ppSyncObj)->AddRef();
	return S_OK;
}



#endif //#ifndef RSSYNC_H_
