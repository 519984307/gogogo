#ifndef __LMX__
#define __LMX__

typedef RPC_STATUS		LAC_STATUS;
typedef void *			HREMOTE;
typedef HREMOTE*		LPHREMOTE;
typedef void *			HINVOKE;
typedef HINVOKE	*		LPHINVOKE;

typedef RX_BLOB			LAC_BLOB;
typedef const LAC_BLOB	LAC_BLOB_C;

struct OUT_PARAM
{
	long *	outBytes;
	byte **	outParam;
};

typedef HRESULT ( WINAPI * PFN_LAC_PUBLISH_FUNC )( HINVOKE hInvoke, LAC_BLOB_C & InParam, OUT_PARAM & outParam, LPVOID vpOpaque );

typedef struct _LAC_CALLBACK : IRefable
{
	virtual void OnComplete( HREMOTE , LAC_STATUS , HRESULT , LAC_BLOB_C &  );
} LAC_CALLBACK, *LPLAC_CALLBACK;

//////////////////////////////////////////////////////////////////////////
//
//
//					Basic Function
//
//
//////////////////////////////////////////////////////////////////////////

//#ifdef LAC_IMPL

	#define USING_LAC			
	#define IMPORT_LAC(x)		(true)


	HRESULT WINAPI LacIsValid();

	//
	//					For Server Calls
	//


	//
	//	设置一个终端名字
	//

	HRESULT WINAPI LacSetPublishName( LPCWSTR lpcwName );

	//
	//	开始监听
	//

	HRESULT WINAPI LacListen( UINT uiMinThreads = 1, UINT uiMaxCalls = 10, BOOL bWait = FALSE );

	//
	//	关闭监听
	//

	HRESULT WINAPI LacShutdown();

	//
	//	注册一个命名函数
	//

	HRESULT WINAPI LacRegisterFunction( LPCSTR lpcName, PFN_LAC_PUBLISH_FUNC pfn, LPVOID vpOpaque);

	//
	//	吊销一个命名函数
	//

	HRESULT WINAPI LacRevokeFunction( LPCSTR lpcName );

	//
	//	
	//

	HRESULT WINAPI LacSetOutParam( OUT_PARAM & outParam, LAC_BLOB_C& blob );

	HRESULT WINAPI LacCompleteInvoke( HINVOKE hInvoke, OUT_PARAM & outParam, LPCVOID vpData, UINT uiBytes );

	//
	//	For Client Calls
	//

	HRESULT WINAPI LacOpenRemote( LPCWSTR lpcName, LPHREMOTE hRemote );

	HRESULT WINAPI LacCloseRemote( HREMOTE hRemote );

	HRESULT WINAPI LacInvokeSync( HREMOTE hRemote, LPCSTR fnName, LAC_BLOB_C & inParam, LPVOID reply, LAC_BLOB & outParam, DWORD dwTimeout = -1 );

	HRESULT WINAPI LacInvokeAsyn( HREMOTE hRemote, LPCSTR fnName, LAC_BLOB_C & inParam, LPLAC_CALLBACK cbComplete, DWORD dwTimeout = -1, LPHINVOKE hpInvoke = 0 );

	HRESULT WINAPI LacCancelInvoke( HINVOKE hInvoke );

	HRESULT WINAPI LacGetClientId( HINVOKE hInvoke, ULONG & rPID );
//
//#else
//
//	typedef HRESULT (WINAPI * ImpLacIsValid)();
//	typedef HRESULT (WINAPI * ImpLacSetPublishName)( LPCWSTR  );
//	typedef HRESULT (WINAPI * ImpLacListen)( UINT , UINT , BOOL  );
//	typedef HRESULT (WINAPI * ImpLacShutdown)();
//	typedef HRESULT (WINAPI * ImpLacRegisterFunction)( LPCSTR , PFN_LAC_PUBLISH_FUNC , LPVOID );
//	typedef HRESULT (WINAPI * ImpLacRevokeFunction)( LPCSTR  );
//	typedef HRESULT (WINAPI * ImpLacSetOutParam)( LPCVOID , UINT , LAC_BLOB &  );
//	typedef HRESULT (WINAPI * ImpLacOpenRemote)( LPCWSTR , LPHREMOTE  );
//	typedef HRESULT (WINAPI * ImpLacCloseRemote)( HREMOTE );
//	typedef HRESULT (WINAPI * ImpLacInvokeSync)( HREMOTE , LPCSTR , LAC_BLOB_C & , LAC_BLOB & , DWORD  );
//	typedef HRESULT (WINAPI * ImpLacInvokeAsyn)( HREMOTE , LPCSTR , LAC_BLOB_C & , LPLAC_CALLBACK , DWORD , LPHINVOKE  );
//	typedef HRESULT (WINAPI * ImpLacCancelInvoke)( LPHINVOKE );
//
//	//////////////////////////////////////////////////////////////////////////
//	//
//	//
//	//						动态导入API
//	//
//	//
//	//////////////////////////////////////////////////////////////////////////
//
//	struct LAC_MODULE
//	{
//		HMODULE					LmxModule;
//
//		ImpLacIsValid			_LacIsValid;
//		ImpLacSetPublishName	_LacSetPublishName;
//		ImpLacListen			_LacListen;
//		ImpLacShutdown			_LacShutdown;
//		ImpLacRegisterFunction	_LacRegisterFunction;
//		ImpLacRevokeFunction	_LacRevokeFunction;
//		ImpLacSetOutParam		_LacSetOutParam;
//		ImpLacOpenRemote		_LacOpenRemote;
//		ImpLacCloseRemote		_LacCloseRemote;
//		ImpLacInvokeSync		_LacInvokeSync;
//		ImpLacInvokeAsyn		_LacInvokeAsyn;
//		ImpLacCancelInvoke		_LacCancelInvoke;
//
//		template < class T >
//		T GetProcEntryPoint( T & ep, HMODULE mod, LPCSTR name )
//		{
//			ep = (T)GetProcAddress( mod, name );
//			return ep;
//		}
//		LAC_MODULE() : LmxModule(0)
//		{
//		}
//		~LAC_MODULE()
//		{
//			if( LmxModule ) FreeLibrary( LmxModule );
//			LmxModule = 0;
//		}
//		BOOL Import( LPCSTR modName )
//		{
//			if( LmxModule ) return TRUE;
//			if( !modName ) return FALSE;
//
//			HMODULE hMod = LoadLibraryA( modName );
//			if( !hMod ) return FALSE;
//
//			if( !GetProcEntryPoint(_LacIsValid,			hMod, "LacIsValid" )			||
//				!GetProcEntryPoint(_LacSetPublishName,	hMod, "LacSetPublishName" )		||
//				!GetProcEntryPoint(_LacListen,			hMod, "LacListen" )				||
//				!GetProcEntryPoint(_LacShutdown,		hMod, "LacShutdown" )			||
//				!GetProcEntryPoint(_LacRegisterFunction,hMod, "LacRegisterFunction" )	||
//				!GetProcEntryPoint(_LacRevokeFunction,	hMod, "LacRevokeFunction" )		||
//				!GetProcEntryPoint(_LacSetOutParam,		hMod, "LacSetOutParam" )		||
//				!GetProcEntryPoint(_LacOpenRemote,		hMod, "LacOpenRemote" )			||
//				!GetProcEntryPoint(_LacCloseRemote,		hMod, "LacCloseRemote" )		||
//				!GetProcEntryPoint(_LacInvokeSync,		hMod, "LacInvokeSync")			||
//				!GetProcEntryPoint(_LacInvokeAsyn,		hMod, "LacInvokeAsyn")			||
//				!GetProcEntryPoint(_LacCancelInvoke,	hMod, "LacCancelInvoke")			)
//			{
//				_LacIsValid = 0;
//				_LacSetPublishName = 0;	
//				_LacListen = 0;
//				_LacShutdown = 0;
//				_LacRegisterFunction = 0;
//				_LacRevokeFunction = 0;	
//				_LacSetOutParam = 0;
//				_LacOpenRemote = 0;
//				_LacCloseRemote = 0;
//				_LacInvokeSync = 0;
//				_LacInvokeAsyn = 0;
//				_LacCancelInvoke = 0;
//
//				FreeLibrary( hMod );
//				return FALSE;
//			}
//			LmxModule = hMod;
//			return TRUE;
//		}
//	};
//
//	extern LAC_MODULE		MODULE_LAC;
//
//	#define USING_LAC			LAC_MODULE		MODULE_LAC;
//	#define IMPORT_LAC(x)		(MODULE_LAC.Import((x)))
//
//	#define LacIsValid			(MODULE_LAC._LacIsValid)
//	#define LacSetPublishName	(MODULE_LAC._LacSetPublishName)
//	#define LacListen			(MODULE_LAC._LacListen)
//	#define LacShutdown			(MODULE_LAC._LacShutdown)
//	#define LacRegisterFunction	(MODULE_LAC._LacRegisterFunction)
//	#define LacRevokeFunction	(MODULE_LAC._LacRevokeFunction)
//	#define LacSetOutParam		(MODULE_LAC._LacSetOutParam)
//	#define LacOpenRemote		(MODULE_LAC._LacOpenRemote)
//	#define LacCloseRemote		(MODULE_LAC._LacCloseRemote)
//	#define LacInvokeSync		(MODULE_LAC._LacInvokeSync)
//	#define LacInvokeAsyn		(MODULE_LAC._LacInvokeAsyn)
//	#define	LacCancelInvoke		(MODULE_LAC._LacCancelInvoke)
//
//
//#endif

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
//
//
//		Helper Template Class
//
//
//////////////////////////////////////////////////////////////////////////

template < class T, class PFN >
struct method_t : LAC_CALLBACK
{
	long	_cookie;
	T		_this;
	PFN		_pfn;
	method_t( T& p, PFN pfn, long cookie = 0 )
	{
		_this = p;
		_pfn = pfn;
		_cookie = cookie;
	}
	~method_t()
	{
	}
	virtual void OnComplete( HREMOTE hRemote, LAC_STATUS status, HRESULT lr, LAC_BLOB_C & outParam )
	{
		(_this->*_pfn)( hRemote, status, E_UNEXPECTED, outParam );
	}
};

template < class T, class PFN >
refp<LAC_CALLBACK> Method( T obj, PFN f, long cookie = 0 )
{
	return refp<LAC_CALLBACK>( new method_t<T,PFN>( obj, f, cookie ) );
}



#endif