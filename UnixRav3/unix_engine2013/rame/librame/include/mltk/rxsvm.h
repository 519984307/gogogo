#ifndef __RX_SVM__
#define __RX_SVM__

#include "../imsslib.h"
#include "svm.h"

namespace svm
{
	typedef int				AttrType;
	typedef	unsigned int	UAttrType;

	template < class T>
	struct IPointerWrapper : IRefable
	{
		STDMETHOD_(T*,GetPointer)() PURE;
		STDMETHOD_(const T*,GetConstPointer)() const PURE;
	};

	struct ISample : IPointerWrapper<SVMNode>
	{
		STDMETHOD(Set)( AttrType attr, Float64 valu ) PURE;
		STDMETHOD(Get)( AttrType attr, Float64& valu ) const PURE;
		STDMETHOD_(SIZE_T,Size)() const PURE;
		STDMETHOD(EnumGet)( SIZE_T uIdx, SVMNode& node ) const PURE;
		STDMETHOD(Clear)() PURE;
		STDMETHOD(Confirm)() PURE;
	};
	
	struct IModel : IPointerWrapper<SVMModel>
	{
		// return a ProgId
		STDMETHOD_(LPCSTR, GetSampleAdapter)() PURE;
		STDMETHOD(Load)( LPCSTR aFileName ) PURE;
		STDMETHOD(Load)( LPCSTR aText, SIZE_T cbText ) PURE;
		STDMETHOD(Load)( RX_BLOB& rBlob ) PURE;
		STDMETHOD(Save)( RX_BLOB& rBlob ) PURE;
		STDMETHOD(Clear)() PURE;
		STDMETHOD(Predict)( const ISample * lpSample, Float64& uClass ) PURE;
	};

	enum PrivateClassId
	{
		SVMCLS_SAMPLE,
		SVMCLS_MODEL,
	};

	class ISampleProductor : public IUnknown
	{
	public:
		STDMETHOD(Product)( IUnknown *pArchiveAsUnknown, ISample* pSample ) PURE;
		RE_DECLARE_IID;
	};
};

// {8CB89FFC-B167-46BB-A1D0-B99BFD8C6F0E}
RE_DEFINE_IID( svm::ISampleProductor, "{8CB89FFC-B167-46BB-A1D0-B99BFD8C6F0E}",
			0x8cb89ffc, 0xb167, 0x46bb, 0xa1, 0xd0, 0xb9, 0x9b, 0xfd, 0x8c, 0x6f, 0xe);

namespace mltk
{
	class ICompactVector : public IUnknown
	{
	public:
		STDMETHOD(Clear)( SIZE_T uSize ) PURE;
		STDMETHOD(Resize)( SIZE_T uSize ) PURE;
		STDMETHOD(Get)( SIZE_T uIndex, Float64& rValue ) PURE;
		STDMETHOD(Set)( SIZE_T uIndex, Float64 rValue ) PURE;
		RE_DECLARE_IID;
	};

	class IFlatVector : public ICompactVector
	{
	public:
		STDMETHOD_(Float64*, GetBase)() PURE;
		RE_DECLARE_IID;
	};

	class IVectorization : public IUnknown
	{
	public:
		STDMETHOD(Vectorization)( IUnknown * lpArchive, ICompactVector * lpVector ) PURE;
	};

};



#endif