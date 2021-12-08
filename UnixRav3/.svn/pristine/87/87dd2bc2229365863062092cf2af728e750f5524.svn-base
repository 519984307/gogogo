#ifndef __RSMISC_H__
#define __RSMISC_H__

#include "RSMEMORY.H"

#define DECLARE_OBJECT_GETTYPE virtual RS_DWORD GetType()
#define DECLARE_OBJECT_QI virtual RS_RESULT QueryInterface(RS_DWORD, void**)
#define DECLARE_OBJECT_RELEASE virtual void Release()
#define DECLARE_OBJECT_CREATEOBJ static RS_RESULT RS_STDCALL CreateObj(void**, IRSEnv*)

// NOTE: We MUST use the long version macro because of the silly gcc on SunOS.
/*
#define DECLARE_OBJECT_CREATEOBJ_WITH_MEMMGR(o) inline void* operator new(size_t siz, IRSMemory* p) \
{ \
	o* pI = (o*)p->alloc(siz); \
	pI->m_pMMgr = p; \
	return pI; \
} \
inline void operator delete(void* p, IRSMemory* p2) \
{ \
	p2->free(p); \
} \
inline void operator delete(void* p) \
{ \
	if(p) \
		((o*)p)->m_pMMgr->free(p); \
} \
IRSMemory* m_pMMgr; \
static RS_RESULT RS_STDCALL CreateObj(void**, IRSEnv*)

#define IMPLEMENT_OBJECT_GETTYPE(o) RS_DWORD o::GetType() \
{ \
	return RSID; \
}

#define IMPLEMENT_OBJECT_QI(o) RS_RESULT o::QueryInterface(RS_DWORD dwIID, void** ppv) \
{ \
	if(RSIID == dwIID) \
	{ \
		*ppv = this; \
		return ERR_SUCCESS; \
	} \
	return ERR_QI_IIDERR; \
}

#define IMPLEMENT_OBJECT_RELEASE(o) void o::Release() \
{ \
	if(m_pMgr) \
		m_pMgr->RemoveRef(this); \
	delete this; \
}

#define IMPLEMENT_OBJECT_CREATEOBJ(o) RS_RESULT RS_STDCALL o::CreateObj(void** ppv, IRSEnv* p) \
{ \
	if(!ppv) \
		return ERR_FAILED; \
	*ppv = (IRSObject*)new o(p); \
	return *ppv ? ERR_SUCCESS : ERR_FAILED; \
}

#define IMPLEMENT_OBJECT_CREATEOBJ_WITH_MEMMGR(o) RS_RESULT RS_STDCALL o::CreateObj(void** ppv, IRSEnv* p) \
{ \
	if(!ppv || !p) \
		return ERR_FAILED; \
	IRSMemory* pMMgr = NULL; \
	p->GetStockObject(IRSMemory::RSID, (void**)&pMMgr); \
	if(!pMMgr) \
		return ERR_OBJ_NOMEMMGR; \
	*ppv = (IRSObject*)new(pMMgr) o(p); \
	return *ppv ? ERR_SUCCESS : ERR_FAILED; \
}

// inline IRSObject implemetation
#define IMPLEMENT_OBJECT_GETTYPE_INLINE() RS_DWORD GetType() \
{ \
	return RSID; \
}

#define IMPLEMENT_OBJECT_QI_INLINE() RS_RESULT QueryInterface(RS_DWORD dwIID, void** ppv) \
{ \
	if(RSIID == dwIID) \
	{ \
		*ppv = this; \
		return ERR_SUCCESS; \
	} \
	return ERR_QI_IIDERR; \
}

#define IMPLEMENT_OBJECT_RELEASE_INLINE() void Release() \
{ \
	if(m_pMgr) \
		m_pMgr->RemoveRef(this); \
	delete this; \
}

#define IMPLEMENT_OBJECT_CREATEOBJ_INLINE(o) static RS_RESULT RS_STDCALL CreateObj(void** ppv, IRSEnv* p) \
{ \
	if(!ppv) \
		return ERR_FAILED; \
	*ppv = (IRSObject*)new o(p); \
	return *ppv ? ERR_SUCCESS : ERR_FAILED; \
}

#define IMPLEMENT_OBJECT_CREATEOBJ_WITH_MEMMGR_INLINE(o) static RS_RESULT RS_STDCALL CreateObj(void** ppv, IRSEnv* p) \
{ \
	if(!ppv || !p) \
		return ERR_FAILED; \
	IRSMemory* pMMgr = NULL; \
	p->GetStockObject(IRSMemory::RSID, (void**)&pMMgr); \
	if(!pMMgr) \
		return ERR_OBJ_NOMEMMGR; \
	*ppv = (IRSObject*)new(pMMgr) o(p); \
	return *ppv ? ERR_SUCCESS : ERR_FAILED; \
} \
inline void* operator new(size_t siz, IRSMemory* p) \
{ \
	o* pI = (o*)p->alloc(siz); \
	pI->m_pMMgr = p; \
	return pI; \
} \
inline void operator delete(void* p, IRSMemory* p2) \
{ \
	p2->free(p); \
} \
inline void operator delete(void* p) \
{ \
	if(p) \
		((o*)p)->m_pMMgr->free(p); \
} \
IRSMemory* m_pMMgr;
// */

#define DECLARE_OBJECT_CREATEOBJ_WITH_MEMMGR(o) inline void* operator new(size_t siz, IRSMemory* p) { o* pI = (o*)p->alloc(siz); pI->m_pMMgr = p; return pI; } inline void operator delete(void* p, IRSMemory* p2) { p2->free(p); } inline void operator delete(void* p) { if(p) ((o*)p)->m_pMMgr->free(p); } IRSMemory* m_pMMgr; static RS_RESULT RS_STDCALL CreateObj(void**, IRSEnv*)
#define IMPLEMENT_OBJECT_GETTYPE(o) RS_DWORD o::GetType() { return RSID; }
#define IMPLEMENT_OBJECT_QI(o) RS_RESULT o::QueryInterface(RS_DWORD dwIID, void** ppv) { if(RSIID == dwIID) { *ppv = this; return ERR_SUCCESS; } return ERR_QI_IIDERR; }
#define IMPLEMENT_OBJECT_RELEASE(o) void o::Release() { if(m_pMgr) m_pMgr->RemoveRef(this); delete this; }
#define IMPLEMENT_OBJECT_CREATEOBJ(o) RS_RESULT RS_STDCALL o::CreateObj(void** ppv, IRSEnv* p) { if(!ppv) return ERR_FAILED; *ppv = (IRSObject*)new o(p); return *ppv ? ERR_SUCCESS : ERR_FAILED; }
#define IMPLEMENT_OBJECT_CREATEOBJ_WITH_MEMMGR(o) RS_RESULT RS_STDCALL o::CreateObj(void** ppv, IRSEnv* p) { if(!ppv || !p) return ERR_FAILED; IRSMemory* pMMgr = NULL; p->GetStockObject(IRSMemory::RSID, (void**)&pMMgr); if(!pMMgr) return ERR_OBJ_NOMEMMGR; *ppv = (IRSObject*)new(pMMgr) o(p); return *ppv ? ERR_SUCCESS : ERR_FAILED; }

// inline IRSObject implemetation
#define IMPLEMENT_OBJECT_GETTYPE_INLINE() RS_DWORD GetType() { return RSID; }
#define IMPLEMENT_OBJECT_QI_INLINE() RS_RESULT QueryInterface(RS_DWORD dwIID, void** ppv) { if(RSIID == dwIID) { *ppv = this; return ERR_SUCCESS; } return ERR_QI_IIDERR; }
#define IMPLEMENT_OBJECT_RELEASE_INLINE() void Release() { if(m_pMgr) m_pMgr->RemoveRef(this); delete this; }
#define IMPLEMENT_OBJECT_CREATEOBJ_INLINE(o) static RS_RESULT RS_STDCALL CreateObj(void** ppv, IRSEnv* p) { if(!ppv) return ERR_FAILED; *ppv = (IRSObject*)new o(p); return *ppv ? ERR_SUCCESS : ERR_FAILED; }
#define IMPLEMENT_OBJECT_CREATEOBJ_WITH_MEMMGR_INLINE(o) static RS_RESULT RS_STDCALL CreateObj(void** ppv, IRSEnv* p) { if(!ppv || !p) return ERR_FAILED; IRSMemory* pMMgr = NULL; p->GetStockObject(IRSMemory::RSID, (void**)&pMMgr); if(!pMMgr) return ERR_OBJ_NOMEMMGR; *ppv = (IRSObject*)new(pMMgr) o(p); return *ppv ? ERR_SUCCESS : ERR_FAILED; } inline void* operator new(size_t siz, IRSMemory* p) { o* pI = (o*)p->alloc(siz); pI->m_pMMgr = p; return pI; } inline void operator delete(void* p, IRSMemory* p2) { p2->free(p); } inline void operator delete(void* p) { if(p) ((o*)p)->m_pMMgr->free(p); } IRSMemory* m_pMMgr;

template<typename _TI>
class RSObjPtr
{
	_TI* m_pInteface;
	RS_RESULT m_dwStatus;

public:
	explicit RSObjPtr(IRSObject* p)
	{
		m_dwStatus = p->QueryInterface(_TI::RSIID, (void**)&m_pInteface);
	}

	inline RS_RESULT GetStatus()
	{
		return m_dwStatus;
	}

	inline _TI* operator->() const
	{
		return m_pInteface;
	}

	inline _TI& operator*() const
	{
		return *m_pInteface;
	}

	inline _TI** operator&()
	{
		return &m_pInteface;
	}

	inline _TI* operator=(_TI* lp)
	{
		m_pInteface = lp;
		return lp;
	}

	inline bool operator!() const
	{
		return (m_pInteface == NULL);
	}

	inline bool operator<(_TI* pT) const
	{
		return (m_pInteface < pT);
	}

	inline bool operator==(_TI* pT) const
	{
		return (m_pInteface == pT);
	}
};

//////////////////////////////////////////////////////////////////////////

template<class _TCObj>
class tIRSObject : public IRSObject
{
public:
	virtual RS_DWORD GetType()
	{
		return _TCObj::RSID;
	}

	virtual RS_RESULT QueryInterface(RS_DWORD dwIID, void** ppv)
	{
		if(_TCObj::RSIID == dwIID)
		{
			*ppv = static_cast<IRSObject*>(this);
			return ERR_SUCCESS;
		}

		return ERR_QI_IIDERR;
	}

	void Release()
	{
		IRSObject* pObj = static_cast<IRSObject*>(this);
		if(pObj && pObj->m_pMgr)
			pObj->m_pMgr->RemoveRef(pObj);

		delete this;
	}

	static RS_RESULT RS_STDCALL CreateObj(void** ppv, IRSEnv* p)
	{
		*ppv = static_cast<IRSObject*>(new _TCObj(p));
		return 0;
	}
};

template<class _TCObj>
class tmIRSObject : public IRSObject
{
public:
	virtual RS_DWORD GetType()
	{
		return _TCObj::RSID;
	}

	virtual RS_RESULT QueryInterface(RS_DWORD dwIID, void** ppv)
	{
		if(_TCObj::RSIID == dwIID)
		{
			*ppv = static_cast<IRSObject*>(this);
			return ERR_SUCCESS;
		}

		return ERR_QI_IIDERR;
	}

	virtual void Release()
	{
		IRSObject* pObj = static_cast<IRSObject*>(this);
		if(pObj && pObj->m_pMgr)
			pObj->m_pMgr->RemoveRef(pObj);

		delete this;
	}

	static RS_RESULT RS_STDCALL CreateObj(void** ppv, IRSEnv* p)
	{
		if(!ppv || !p)
			return ERR_FAILED;

		IRSMemory* pMMgr = NULL;
		p->GetStockObject(IRSMemory::RSID, (void**)&pMMgr);
		if(!pMMgr)
			return ERR_OBJ_NOMEMMGR;

		*ppv = static_cast<IRSObject*>(new(pMMgr) _TCObj(p));
		return *ppv ? ERR_SUCCESS : ERR_FAILED;
	}

	inline void* operator new(size_t siz, IRSMemory* p)
	{
		_TCObj* pI = (_TCObj*)p->alloc(siz);
		pI->m_pMMgr = p;
		return pI;
	}

	inline void operator delete(void* p, IRSMemory* p2)
	{
		p2->free(p);
	}

	inline void operator delete(void* p)
	{
		if(p)
			((_TCObj*)p)->m_pMMgr->free(p);
	}

public:
	IRSMemory* m_pMMgr;
};

//////////////////////////////////////////////////////////////////////////


template<typename _TI>
class RSModPtr
{
	_TI* m_pInteface;
	RS_RESULT m_dwStatus;

public:
	~RSModPtr()
	{
		m_pInteface->Release();
	}

	explicit RSModPtr(const IRSModule* pRSMod, IRSEnv* pEnv);	
	explicit RSModPtr(const IRSModule* (RS_STDCALL* pGetRSModule)(), IRSEnv* pEnv);

	inline RS_RESULT GetStatus()
	{
		return m_dwStatus;
	}
	inline _TI* operator->() const
	{
		return m_pInteface;
	}
	inline _TI& operator*() const
	{
		return *m_pInteface;
	}
	inline _TI** operator&()
	{
		return &m_pInteface;
	}
	inline _TI* operator=(_TI* lp)
	{
		m_pInteface = lp;
		return lp;
	}
	inline bool operator!() const
	{
		return (m_pInteface == NULL);
	}
	inline bool operator<(_TI* pT) const
	{
		return (m_pInteface < pT);
	}
	inline bool operator==(_TI* pT) const
	{
		return (m_pInteface == pT);
	}
};

template<typename _TI>
RSModPtr<_TI>::RSModPtr(const IRSModule* pRSMod, IRSEnv* pEnv)
{
	IRSObject *pObj = NULL;
	m_dwStatus = pRSMod->CreateObject(_TI::RSID, pEnv, (void**)&pObj);
	if(pObj)
		m_dwStatus = pObj->QueryInterface(_TI::RSIID, (void**)&m_pInteface);
}

template<typename _TI>
RSModPtr<_TI>::RSModPtr(const IRSModule* (RS_STDCALL* pGetRSModule)(), IRSEnv* pEnv)
{
	const IRSModule* pRSMod = pGetRSModule();
	IRSObject* pObj = NULL;

	m_dwStatus = pRSMod->CreateObject(_TI::RSID, pEnv, (void**)&pObj);
	if(pObj)
		m_dwStatus = pObj->QueryInterface(_TI::RSIID, (void**)&m_pInteface);
}

#endif // __RSMISC_H__
