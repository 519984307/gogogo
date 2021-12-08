#ifndef _I_RX_CLD_QRY_H
#define _I_RX_CLD_QRY_H

namespace rxtsn {

	class IRxCloudPredictor : public IUnknown
	{
	public:
		enum ENGINE_VER
		{
			Ecloudqry2011,
			Ecloudqry2017
		};
		struct ICallback
		{
			STDMETHOD_(VOID*,GetCallbackByVersion)( DWORD dwVersion ) = 0;
			STDMETHOD_(VOID, OnPredictReturn)( ENGINE_VER dwEngineVer, HRESULT err, LPCSTR lpResult, SIZE_T dwBytes ) = 0;
		};
	public:
		STDMETHOD(PredictOne)( ENGINE_VER enEngVer, LPCSTR lpObjectText, SIZE_T dwBytes, ICallback * pcb ) = 0;
		RE_DECLARE_IID;
	};

	class Irxmsgp : public IUnknown
	{
	public:
		virtual long rstsn_post_message_notime(const char* topic, const char* body, const char* uuid) = 0;
		virtual long rstsn_post_message(const char* topic, const char* body, const char* uuid, const char* time) = 0;
		RE_DECLARE_IID;
	};

	class ItaskAux : public IUnknown
	{
	public:
		virtual void starttask() = 0;
		virtual void stoptask() = 0;

		// jsconfig["url"] = "http://118.145.23.94/rdm/predict3.php"; jsconfig["timeout"] = 3000; jsconfig["conntimeout"] = 1000;
		// jsconfig["err_timeout"] = 2000; jsconfig["headers"] =  "["User-Agent: TsnClien","Content-Type: application/octet-stream"]"; 
		// jsconfig["proxyswitch"] = 1; jsconfig["proxyaddrinfo"] = "193.168.18.80:808";jsconfig["proxyuserpwd"] = "user:password"; 
		// jsconfig["proxytype"] = 0; jsconfig["tsn.splitsize"] = 4096
		virtual long rscav_config_update(const char* pszconfig) = 0;
		RE_DECLARE_IID;
	};

	// {D4E22098-0C63-4A0E-A38A-A3755F7250DD}
	RE_DEFINE_IID( IRxCloudPredictor, "{D4E22098-0C63-4A0E-A38A-A3755F7250DD}",
		0xd4e22098, 0xc63, 0x4a0e, 0xa3, 0x8a, 0xa3, 0x75, 0x5f, 0x72, 0x50, 0xdd );

	// {0EE6F542-CC86-4B11-9A88-C8610C248BEE}
	DEF_CLASSID(CloudEngine, 0xee6f542, 0xcc86, 0x4b11, 0x9a, 0x88, 0xc8, 0x61, 0xc, 0x24, 0x8b, 0xee);

	// {6E44B067-3BF6-4828-BB45-04785584900E}
	RE_DEFINE_IID( Irxmsgp, "{6E44B067-3BF6-4828-BB45-04785584900E}",
		0x6e44b067, 0x3bf6, 0x4828, 0xbb, 0x45, 0x4, 0x78, 0x55, 0x84, 0x90, 0xe );

	// {5AA10F2D-0D45-4BBD-BB52-7CC40D91E5E2}
	DEF_CLASSID(TsnMessagePush, 0x5aa10f2d, 0xd45, 0x4bbd, 0xbb, 0x52, 0x7c, 0xc4, 0xd, 0x91, 0xe5, 0xe2);

	// {6706060F-58F6-432C-BBF5-10207847451A}
	RE_DEFINE_IID(ItaskAux, "{6706060F-58F6-432C-BBF5-10207847451A}",
		0x6706060f, 0x58f6, 0x432c, 0xbb, 0xf5, 0x10, 0x20, 0x78, 0x47, 0x45, 0x1a );

	class IHttpRequest : public IUnknown
	{
	public:
		enum METHOD
		{
			mAUTO, mGET, mPOST,
		};
		STDMETHOD_(LPVOID,GetHandle)() PURE;
		STDMETHOD_(LONG,SetURL)( LPCSTR lpURL ) PURE;
		STDMETHOD_(LONG,SetProxy)( LPCSTR lpAddress, LPCSTR lpUserPass ) PURE;
		STDMETHOD_(LONG,SetTimeout)( SIZE_T msTimeout ) PURE;
		STDMETHOD_(LONG,AddHeader)( LPCSTR lpHeader ) PURE;
		STDMETHOD_(LONG,AddHeader)( LPCSTR lpHeader, LPCSTR lpVal ) PURE;
		STDMETHOD_(LONG,Request)( METHOD eMethod, LPCVOID lpData, SIZE_T nBytes, LPCSTR lpContentType = NULL ) PURE;
		STDMETHOD_(LONG, ResponseCode)() PURE;
		STDMETHOD_(LPCSTR,ResponseData)( SIZE_T * lpDataSize ) PURE;
		RE_DECLARE_IID;
	};

	class IHttpUtil : public IUnknown
	{
	public:
		STDMETHOD( CreateRequest )( IHttpRequest ** ppRequest ) PURE;
		RE_DECLARE_IID;
	};

	// {2FEFE98F-DC52-4BA6-90C6-753ECBA1D407}
	RE_DEFINE_IID( IHttpRequest, "{2FEFE98F-DC52-4BA6-90C6-753ECBA1D407}",
		0x2fefe98f, 0xdc52, 0x4ba6, 0x90, 0xc6, 0x75, 0x3e, 0xcb, 0xa1, 0xd4, 0x7);

	// {2610664B-C4FE-4BA4-BB5B-73847EDCA19C}
	RE_DEFINE_IID( IHttpUtil, "{2610664B-C4FE-4BA4-BB5B-73847EDCA19C}",
		0x2610664b, 0xc4fe, 0x4ba4, 0xbb, 0x5b, 0x73, 0x84, 0x7e, 0xdc, 0xa1, 0x9c);

	// {78A146E9-9237-4806-8A14-EDE437660A5F}
	DEF_CLASSID(TsnHttpUtil, 0x78a146e9, 0x9237, 0x4806, 0x8a, 0x14, 0xed, 0xe4, 0x37, 0x66, 0xa, 0x5f);

}

#endif // _I_RX_CLD_QRY_H