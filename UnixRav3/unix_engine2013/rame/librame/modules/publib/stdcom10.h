#ifndef __RE_STD_COM_H_10__
#define __RE_STD_COM_H_10__

namespace _10
{
	class CUnknownImp_Inner
	{
	public:
		ULONG m_RefCount;
		CUnknownImp_Inner(): m_RefCount(0), m_punkOuter(0) {}
	public:
		IUnknown *m_punkOuter;
		HRESULT init_class_inner(IUnknown *punkOuter)
		{
			m_punkOuter = punkOuter;
			return S_OK;
		}
	};

	class CUnknownImp_Outer
	{
	public:
		ULONG m_RefCount;
		CUnknownImp_Outer(): m_RefCount(0) {}
	public:
		UTIL::com_ptr<IUnknown> m_punkInner;
		HRESULT init_class_outer(IUnknown *prot, IUnknown *punkThisOuter, const CLSID& rclsid)
		{
			using namespace UTIL;
			RASSERT(prot, E_INVALIDARG);
			return RECreateInstance(rclsid, punkThisOuter, re_uuidof(IUnknown), (void**)&m_punkInner, prot );
		}
	};

	//////////////////////////////////////////////////////////////////////////

	//template<class CLS>
	//class TStdClsFactory : public IRXClassFactory, public IClassFactory, private CUnknownImp
	//{
	//public: // IUnknown:
	//	UNKNOWN_IMP2_(IRXClassFactory, IClassFactory);

	//public: // std factory invoke:
	//	STDMETHOD(init_class)(IUnknown* /*prot*/, IUnknown* punkOuter)
	//	{
	//		return !punkOuter ? S_OK : E_INVALIDARG;
	//	}

	//public: // IREClassFactory:
	//	STDMETHOD(CreateInstance)(IUnknown *prot, IUnknown *punkOuter, const IID& riid, void **ppv)
	//	{
	//		return create_instance(prot, punkOuter, riid, ppv);
	//	}
	//	STDMETHOD(LockServer)(BOOL fLock)
	//	{
	//		return lock_server(fLock);
	//	}

	//public: // IClassFactory:
	//	STDMETHOD(CreateInstance)(IUnknown *punkOuter, const IID& riid, void **ppv)
	//	{
	//		return create_instance(0, punkOuter, riid, ppv);
	//	}

	//public:
	//	static HRESULT create_instance(IUnknown *prot, IUnknown *punkOuter, const IID& riid, void **ppv)
	//	{
	//		*ppv = 0;
	//		// aggregation validate:
	//		RASSERT(!punkOuter || re_uuidof(IUnknown) == riid, E_INVALIDARG);
	//		// create new object/aggregation:
	//		UTIL::sentry<CLS*> p(RSNEW CLS);
	//		RASSERT(p, E_UNEXPECTED);
	//		((IUnknown_Nondelegate*)(CLS*)p)->AddRef_Nondelegate(); // nondelegation, protect reference count
	//		RFAILED(p->init_class(prot, punkOuter));
	//		HRESULT hr = ((IUnknown_Nondelegate*)(CLS*)p)->QueryInterface_Nondelegate(riid, ppv);
	//		((IUnknown_Nondelegate*)p.detach())->Release_Nondelegate(); // nondelegation, balance reference count or destroy.
	//		return hr;
	//	}
	//	static HRESULT lock_server(BOOL /*fLock*/)
	//	{
	//		return S_OK;
	//	}
	//	static HRESULT GetClassObject(REFIID riid, void **ppv)
	//	{
	//		return TStdClsFactory<TStdClsFactory<CLS> >::create_instance(0, 0, riid, ppv);
	//	}
	//};
};
#endif // duplicate inclusion protection
