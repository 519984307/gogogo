#ifndef __RX_STD_COM_H__
#define __RX_STD_COM_H__

//////////////////////////////////////////////////////////////////////////

inline bool operator < (const GUID& left, const GUID& right)
{
	return memcmp(&left, &right, sizeof(GUID)) < 0;
}

#ifdef PLATFORM_TYPE_POSIX
inline bool operator == (const GUID& left, const GUID& right)
{
	return !memcmp(&left, &right, sizeof(GUID));
}

inline bool operator != (const GUID& left, const GUID& right)
{
	return !(left == right);
}
#endif

//////////////////////////////////////////////////////////////////////////


class CUnknownImp
{
public:
	ULONG m_RefCount;
	CUnknownImp(): m_RefCount(0) {}
};


#define QIBEGIN	\
	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv) {

#define QIUNKNOWN	\
	if(re_uuidof(IUnknown) == riid) { *ppv = static_cast<IUnknown*>(this); AddRef(); return S_OK; }

#define QIUNKNOWN_(icast)	\
	if(re_uuidof(IUnknown) == riid) { *ppv = static_cast<IUnknown*>(static_cast<icast*>(this)); AddRef(); return S_OK; }

#define QIENTRY(iface)	\
	if(re_uuidof(iface) == riid) { *ppv = static_cast<iface*>(this); AddRef(); return S_OK; }

#define QIENTRY_(iface, icast)	\
	if(re_uuidof(iface) == riid) { *ppv = static_cast<iface*>(static_cast<icast*>(this)); AddRef(); return S_OK; }

#define QIEND \
	return E_NOINTERFACE; }

#ifndef PLATFORM_TYPE_WINDOWS

#define ADDREF	\
	STDMETHOD_(ULONG, AddRef)()	\
	{ return ++this->m_RefCount; }

#define RELEASE	\
	STDMETHOD_(ULONG, Release)()	\
	{ if(--this->m_RefCount) return this->m_RefCount; delete this; return 0; }

#else

#define ADDREF		STDMETHOD_(ULONG, AddRef)()	\
					{ InterlockedIncrement( (PLONG)&m_RefCount ); return m_RefCount; }

#define RELEASE		STDMETHOD_(ULONG, Release )() {	\
					LONG lRet = InterlockedDecrement( (PLONG)&m_RefCount );\
					if( lRet == 0 ) delete this;\
					return lRet;\
}
#endif

#define UNKNOWN_IMP_SPEC(entrys) \
	QIBEGIN  QIUNKNOWN  entrys  QIEND  ADDREF  RELEASE

#define UNKNOWN_IMP_SPEC_(icast, entrys) \
	QIBEGIN  QIUNKNOWN_(icast)  entrys  QIEND  ADDREF  RELEASE

#define UNKNOWN_IMP0() \
	UNKNOWN_IMP_SPEC( ; )
#define UNKNOWN_IMP1(i1) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) )
#define UNKNOWN_IMP2(i1, i2) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_IMP3(i1, i2, i3) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_IMP4(i1, i2, i3, i4) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_IMP5(i1, i2, i3, i4, i5) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_IMP6(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_IMP7(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )


#define UNKNOWN_IMP1_(i1) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) )
#define UNKNOWN_IMP2_(i1, i2) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_IMP3_(i1, i2, i3) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_IMP4_(i1, i2, i3, i4) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_IMP5_(i1, i2, i3, i4, i5) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_IMP6_(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_IMP7_(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )
#define UNKNOWN_IMP8_(i1, i2, i3, i4, i5, i6, i7, i8) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7)  QIENTRY(i8) )
#define UNKNOWN_IMP9_(i1, i2, i3, i4, i5, i6, i7, i8, i9 ) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7)  QIENTRY(i8) QIENTRY(i9) )
#define UNKNOWN_IMP10_(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10 ) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7)  QIENTRY(i8) QIENTRY(i9) QIENTRY(i10) )
//////////////////////////////////////////////////////////////////////////


class IUnknown_Nondelegate
{
public:
	STDMETHOD(QueryInterface_Nondelegate)(REFGUID riid, void **ppv) = 0;
	STDMETHOD_(ULONG, AddRef_Nondelegate)() = 0;
	STDMETHOD_(ULONG, Release_Nondelegate)() = 0;
};


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


#define QIBEGIN_NONDELEGATE	\
	STDMETHOD(QueryInterface_Nondelegate)(REFGUID riid, void **ppv) {

#define QIUNKNOWN_NONDELEGATE	\
	if(re_uuidof(IUnknown) == riid) { *ppv = static_cast<IUnknown_Nondelegate*>(this); AddRef_Nondelegate(); return S_OK; }

#define QIUNKNOWN_NONDELEGATE_(icast)	\
	if(re_uuidof(IUnknown) == riid) { *ppv = static_cast<IUnknown_Nondelegate*>(static_cast<icast*>(this)); AddRef_Nondelegate(); return S_OK; }

#define ADDREF_NONDELEGATE	\
	STDMETHOD_(ULONG, AddRef_Nondelegate)()	\
	{ return ++this->m_RefCount; }

#define RELEASE_NONDELEGATE	\
	STDMETHOD_(ULONG, Release_Nondelegate)()	\
	{ if(--this->m_RefCount) return this->m_RefCount; delete this; return 0; }

#define QI_INNER	\
	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv)	\
	{ if(this->m_punkOuter) { return this->m_punkOuter->QueryInterface(riid, ppv); } else { return QueryInterface_Nondelegate(riid, ppv); } }

#define ADDREF_INNER	\
	STDMETHOD_(ULONG, AddRef)()	\
	{ if(this->m_punkOuter) { return this->m_punkOuter->AddRef(); } else { return AddRef_Nondelegate(); } }

#define RELEASE_INNER	\
	STDMETHOD_(ULONG, Release)()	\
	{ if(this->m_punkOuter) { return this->m_punkOuter->Release(); } else { return Release_Nondelegate(); } }

#define UNKNOWN_INNER_IMP_SPEC(entrys) \
	QIBEGIN_NONDELEGATE  QIUNKNOWN_NONDELEGATE  entrys  QIEND  ADDREF_NONDELEGATE  RELEASE_NONDELEGATE  QI_INNER  ADDREF_INNER  RELEASE_INNER

#define UNKNOWN_INNER_IMP_SPEC_(icast, entrys) \
	QIBEGIN_NONDELEGATE  QIUNKNOWN_NONDELEGATE_(icast)  entrys  QIEND  ADDREF_NONDELEGATE  RELEASE_NONDELEGATE  QI_INNER  ADDREF_INNER  RELEASE_INNER

#define UNKNOWN_INNER_IMP1(i) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i) )
#define UNKNOWN_INNER_IMP2(i1, i2) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_INNER_IMP3(i1, i2, i3) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_INNER_IMP4(i1, i2, i3, i4) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_INNER_IMP5(i1, i2, i3, i4, i5) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_INNER_IMP6(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_INNER_IMP7(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )

#define UNKNOWN_INNER_IMP2_(i1, i2) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_INNER_IMP3_(i1, i2, i3) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_INNER_IMP4_(i1, i2, i3, i4) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_INNER_IMP5_(i1, i2, i3, i4, i5) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_INNER_IMP6_(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_INNER_IMP7_(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )

class CUnknownImp_Outer
{
public:
	ULONG m_RefCount;
	CUnknownImp_Outer(): m_RefCount(0) {}
public:
	UTIL::com_ptr<IUnknown> m_punkInner;
	HRESULT init_class_outer(IUnknown *prot, IUnknown *punkThisOuter, TCLSIDREF rclsid)
	{
		return RoCreateInstance( rclsid,punkThisOuter,re_uuidof(IUnknown),(void**)&m_punkInner,prot);
	}
};

#define QIEND_OUTER \
	return this->m_punkInner->QueryInterface(riid, ppv); }

#define UNKNOWN_OUTER_IMP_SPEC(entrys) \
	QIBEGIN  QIUNKNOWN  entrys  QIEND_OUTER  ADDREF  RELEASE

#define UNKNOWN_OUTER_IMP_SPEC_(icast, entrys) \
	QIBEGIN  QIUNKNOWN_(icast)  entrys  QIEND_OUTER  ADDREF  RELEASE

#define UNKNOWN_OUTER_IMP0 \
	QIBEGIN  QIUNKNOWN  QIEND_OUTER  ADDREF  RELEASE
#define UNKNOWN_OUTER_IMP1(i) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i) )
#define UNKNOWN_OUTER_IMP2(i1, i2) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_OUTER_IMP3(i1, i2, i3) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_OUTER_IMP4(i1, i2, i3, i4) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_OUTER_IMP5(i1, i2, i3, i4, i5) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_OUTER_IMP6(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_OUTER_IMP7(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )

#define UNKNOWN_OUTER_IMP2_(i1, i2) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_OUTER_IMP3_(i1, i2, i3) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_OUTER_IMP4_(i1, i2, i3, i4) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_OUTER_IMP5_(i1, i2, i3, i4, i5) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_OUTER_IMP6_(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_OUTER_IMP7_(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )


//////////////////////////////////////////////////////////////////////////


template<class CLS>
class TStdClsFactory : public IRXClassFactory, private CUnknownImp
{
public: // IUnknown:
	UNKNOWN_IMP1(IRXClassFactory);

public: // std factory invoke:
    virtual ~TStdClsFactory(){}
	STDMETHOD(init_class)(IUnknown* /*prot*/, IUnknown* punkOuter)
	{
		return !punkOuter ? S_OK : E_INVALIDARG;
	}

public: // IRXClassFactory:
	STDMETHOD(CreateInstance)(IUnknown *prot, IUnknown *punkOuter, const IID& riid, void **ppv)
	{
		return create_instance(prot, punkOuter, riid, ppv);
	}
	STDMETHOD(LockServer)(BOOL fLock)
	{
		return lock_server(fLock);
	}
public:
	static HRESULT create_instance(IUnknown *prot, IUnknown *punkOuter, const IID& riid, void **ppv)
	{
		*ppv = 0;
		// aggregation validate:
		//RASSERT(!punkOuter || re_uuidof(IUnknown) == riid, E_INVALIDARG);
		// create new object/aggregation:
		UTIL::sentry<CLS*> p(new CLS);
		RASSERT(p, E_UNEXPECTED);
		((IUnknown_Nondelegate*)(CLS*)p)->AddRef_Nondelegate(); // nondelegation, protect reference count
		RFAILED(p->init_class(prot, punkOuter));
		HRESULT hr = ((IUnknown_Nondelegate*)(CLS*)p)->QueryInterface_Nondelegate(riid, ppv);
		((IUnknown_Nondelegate*)p.detach())->Release_Nondelegate(); // nondelegation, balance reference count or destroy.
		return hr;
	}
	static HRESULT lock_server(BOOL /*fLock*/)
	{
		return S_OK;
	}
	static HRESULT GetClassObject(const IID& riid, void **ppv)
	{
		return TStdClsFactory<TStdClsFactory<CLS> >::create_instance(0, 0, riid, ppv);
	}
};

//////////////////////////////////////////////////////////////////////////


template<class CLS>
class TStdClsFactory2 : public IRXClassFactory, private CUnknownImp
{
public: // IUnknown:
	UNKNOWN_IMP1(IRXClassFactory);

public: // IRXClassFactory:
	STDMETHOD(CreateInstance)(IRXRunTime *prot, IUnknown *punkOuter, const IID& riid, void **ppv)
	{
		return create_instance(prot, punkOuter, riid, ppv);
	}
	STDMETHOD(LockServer)(BOOL fLock)
	{
		return lock_server(fLock);
	}
public:
	static HRESULT create_instance(IRXRunTime *prot, IUnknown *punkOuter, const IID& riid, void **ppv)
	{
		*ppv = 0;
		// aggregation validate:
		RASSERT(!punkOuter || re_uuidof(IUnknown) == riid, E_INVALIDARG);
		// create new object/aggregation:
		UTIL::sentry<CLS*> p(new CLS);
		RASSERT(p, E_UNEXPECTED);
		((IUnknown_Nondelegate*)(CLS*)p)->AddRef_Nondelegate(); // nondelegation, protect reference count
		HRESULT hr = ((IUnknown_Nondelegate*)(CLS*)p)->QueryInterface_Nondelegate(riid, ppv);
		((IUnknown_Nondelegate*)p.detach())->Release_Nondelegate(); // nondelegation, balance reference count or destroy.
		return hr;
	}
	static HRESULT lock_server(BOOL /*fLock*/)
	{
		return S_OK;
	}
	static HRESULT GetClassObject(const IID& riid, void **ppv)
	{
		return TStdClsFactory2< TStdClsFactory2<CLS> >::create_instance(0, 0, riid, ppv);
	}
};


//////////////////////////////////////////////////////////////////////////


#ifndef GUID_NULL
DEFINE_GUID(GUID_NULL,
			0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
#endif

//#define DEFAULT_QUERY_OUTER STDMETHOD(QueryOuter)( REFIID riid, void ** ppv ){ return E_FAIL; }
//
//template < class I, class T >
//HRESULT RefOuter( UTIL::com_ptr<I>& op, T* p )
//{
//	return p->QueryOuter( re_uuidof(I), (void**)&op.m_p );
//}
//
//struct WithOuterImp
//{
//	HRESULT PreBind( IUnknown * punk ) { return S_OK; }
//};
//
//template < class T >
//class WithOuter : public IWithOuter, public T
//{
//protected:
//	IUnknown*	_outer;
//	ULONG		_outref;
//public:
//	WithOuter(): _outer(0), _outref(0) {}
//
//	STDMETHOD(QueryOuter)( REFIID riid, void ** ppv )
//	{
//		if( !_outer ) return E_UNEXPECTED;
//		return _outer->QueryInterface( riid, ppv );
//	}
//	STDMETHOD(init_class)( IUnknown*prot, IUnknown* pout)
//	{
//		RFAILED( T::init_class(prot,NULL) );
//		return (pout)?BindOuter(pout):S_OK;
//	}
//	STDMETHOD_(ULONG,AddRef)()
//	{
//		OutAddRef();
//		return T::AddRef();
//	}
//	STDMETHOD_(ULONG,Release)()
//	{
//		OutRelease();
//		return T::Release();
//	}
//	STDMETHOD(QueryInterface)( REFIID riid, void ** ppv )
//	{
//		QIENTRY(IWithOuter);
//		RSUCCEEDED( T::QueryInterface(riid,ppv) );
//		if( !_outer ) return E_NOINTERFACE;
//		return _outer->QueryInterface(riid,ppv);
//	}
//	STDMETHOD(InternalQueryInterface)( REFIID riid, void ** ppv )
//	{
//		QIENTRY(IWithOuter);
//		return( T::QueryInterface(riid,ppv) );
//	}
//	STDMETHOD(BindOuter)( IUnknown * punk )
//	{
//		RASSERT( punk, E_INVALIDARG );
//		RASSERT( !_outer, E_UNEXPECTED );
//		RFAILED( T::PreBind(punk) );
//		_outer = punk;
//		return S_OK;
//	}
//
//protected:
//
//	VOID OutAddRef()
//	{
//		if( !_outer ) return ;
//		_outer->AddRef();
//		_outref ++;
//	}
//	VOID OutRelease()
//	{
//		if( !_outer || !_outref ) return ;
//		--_outref;
//		_outer->Release();
//	}
//};
//
//template < class T >
//class WithImplStack : public IDynamicImplement, public T
//{
//public:
//	typedef typename T					TSuper;
//protected:
//	typedef UTIL::com_ptr<IWithOuter>	OWithOuterPtr;
//	typedef std::vector<OWithOuterPtr>	OWithOuterPtrVector;
//	OWithOuterPtrVector					_Inners;
//public:
//	virtual ~WithImplStack() {};
//	STDMETHOD_(ULONG,AddRef)()
//	{
//		return T::AddRef();
//	}
//	STDMETHOD_(ULONG,Release)()
//	{
//		return T::Release();
//	}
//	STDMETHOD(QueryInterface)( REFIID riid, void ** ppv )
//	{
//		QIENTRY(IDynamicImplement);
//		OWithOuterPtrVector::reverse_iterator it = _Inners.rbegin();
//		for( ; it != _Inners.rend(); ++ it )
//		{
//			RSUCCEEDED( (*it)->InternalQueryInterface( riid, ppv ) );
//		}
//		return T::QueryInterface(riid,ppv);
//	}
//	STDMETHOD(PushImplement)( IWithOuter* pimpl )
//	{
//		RASSERT( pimpl, E_INVALIDARG );
//		RFAILED( pimpl->BindOuter( (IDynamicImplement*)this ) );
//		_Inners.push_back( pimpl );
//		return S_OK;
//	}
//};

template < SIZE_T FACE >
class UnknownImplement : public IUnknown, CUnknownImp
{
protected:
	//////////////////////////////////////////////////////////////////////////
	struct IID2PTR
	{
		IID		iid;
		void*	ptr;
	};
private:
	IID2PTR		face_impl[FACE];
public:
	//////////////////////////////////////////////////////////////////////////
	ADDREF;
	RELEASE;
	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv)
	{
		QIUNKNOWN;
		for( size_t i = 0; i < FACE; ++ i )
		{
			if( riid == face_impl[i].iid )
			{
				AddRef();
				*ppv = face_impl[i].ptr;
				return S_OK;
			}
		}
		return E_NOINTERFACE;
	}
protected:
	template < class I >
	HRESULT Implement( size_t idx, I * p )
	{
		if( idx >= FACE ) return E_UNEXPECTED;
		face_impl[idx].iid = re_uuidof(I);
		face_impl[idx].p = p;
		return S_OK;
	}
};

template < class T, class TCAST, SIZE_T COMPLEXNUM = 1 >
class CoComplex : public T
{
protected:
	struct IID2PTR
	{
		IID						iid;
		UTIL::com_ptr<IUnknown>	ptr;
	};
	IID2PTR	_complex[COMPLEXNUM];
public:
	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv)
	{
		// 不要传递到INNER去QueryInterface
		for( size_t i = 0; i < COMPLEXNUM; ++ i )
		{
			if( riid == _complex[i].iid )
			{
				_complex[i].ptr->AddRef();
				*ppv = _complex[i].ptr.m_p;
				return S_OK;
			}
		}
		// 查询自身实现
		return T::QueryInterface( riid, ppv );
	}
protected:
	template< class I >
	HRESULT ComplexIt( size_t idx, TCLSID rclsid )
	{
		return ComplexIt( idx, re_uuidof(I), rclsid );
	}
	template< class I >
	HRESULT ComplexIt( size_t idx, I * p )
	{
		if( idx >= COMPLEXNUM ) return E_UNEXPECTED;
		_complex[idx].iid = re_uuidof(I);
		_complex[idx].ptr.dispose();
		UTIL::com_ptr<I> ip(p);
		_complex[idx].ptr.m_p = ip.detach();
		return S_OK;
	}
	HRESULT ComplexIt( size_t idx, REFIID riid, TCLSID rclsid )
	{
		if( idx >= COMPLEXNUM ) return E_UNEXPECTED;
		UTIL::com_ptr<IUnknown> punk;
		RFAILED( ::RoCreateInstance( rclsid, (TCAST*)this, riid, (void**)&punk.m_p ) );
		_complex[idx].iid = riid;
		_complex[idx].ptr.dispose();
		_complex[idx].ptr.m_p = punk.detach();
		return S_OK;
	}
};

template < class T >
class CoWithOuter : public T
{
private:
	IUnknown*	_outer;
	ULONG		_outref;
public:
	typedef T	super;
	CoWithOuter(): _outer(0), _outref(0) {}
	STDMETHOD(init_class)( IUnknown * rot, IUnknown * outer )
	{
		RASSERT( outer, E_UNEXPECTED );
		RFAILED( T::init_class(rot,0) );
		_outer = outer;
		return S_OK;
	}
	STDMETHOD_(ULONG,AddRef)()
	{
		OutAddRef();
		return super::AddRef();
	}
	STDMETHOD_(ULONG,Release)()
	{
		OutRelease();
		return super::Release();
	}
	STDMETHOD(QueryInterface)( REFIID riid, void ** ppv )
	{
		if( _outer ) RSUCCEEDED( _outer->QueryInterface(riid,ppv) );
		return super::QueryInterface(riid,ppv);
	}
protected:
	VOID OutAddRef()
	{
		if( !_outer ) return ;
		_outer->AddRef();
		_outref ++;		// FIXME
	}
	VOID OutRelease()
	{
		if( !_outer || !_outref ) return ;
		--_outref;		// FIXME
		_outer->Release();
	}
};
//
//template < SIZE_T FACE, SIZE_T DYNFACE = 1 >
//class UnknownImplementWithOuterInner : public UnknownImplementWithOuter<FACE>
//{
//protected:
//	IID2PTR	dyn_face[FACE];
//public:
//	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv)
//	{
//		if( riid == re_uuidof(IUnknown) ) return this;
//		for( size_t i = 0; i < FACE; ++ i )
//		{
//			if( riid == dyn_face[i].iid )
//			{
//				AddRef();
//				*ppv = dyn_face[i].p;
//				return S_OK;
//			}
//		}
//		return UnknownImplement<FACE>::QueryInterface( iid, ppv );
//	}
//protected:
//	template< class I >
//	HRESULT SetInner( size_t idx, I * p )
//	{
//		if( idx >= FACE ) return E_UNEXPECTED;
//		dyn_face[i].iid = re_uuidof(I);
//		dyn_face[i].p = p;
//		return S_OK;
//	}
//};

#define		DEFAULT_INIT_CLASS()	HRESULT init_class( IUnknown * , IUnknown * ) { return S_OK;}

#endif // duplicate inclusion protection
