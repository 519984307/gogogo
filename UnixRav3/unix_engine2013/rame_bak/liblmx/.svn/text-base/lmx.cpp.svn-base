#include "stdafx.h"

#if (CPP_COMPILER==CC_MSVC )

#include "lmx.h"
#if TARGET_BITS == 64
#include "64/lmxif_h.h"
#else
#include "32/lmxif_h.h"
#endif
#include <map>
#include <string>

#pragma comment( lib, "rpcrt4.lib" )

namespace lac
{
	struct Mutex
	{
		CRITICAL_SECTION _cs;
		Mutex()
		{
			::InitializeCriticalSection( &_cs );
		}
		~Mutex()
		{
			::DeleteCriticalSection( &_cs );
		}
		void enter()
		{
			::EnterCriticalSection( &_cs );
			//printf( "[%d] %d enter\n", ::GetCurrentThreadId(), clock() );
		}
		void leave()
		{
			::LeaveCriticalSection( &_cs );
			//printf( "[%d] leave\n", ::GetCurrentThreadId() );
		}
	};

	struct scope
	{
		Mutex & _mutex_;
		scope( Mutex & rm )
			: _mutex_( rm )
		{
			_mutex_.enter();
		}
		~scope()
		{
			_mutex_.leave();
		}
	};
	struct default_sentry
	{
		static void* default_value() { return 0; }
		template<class _Ptr> static bool equal_to(_Ptr l, _Ptr r) { return l == r; }
		template<class _Ptr> static void destroy(_Ptr p) { if(p) delete p; }
	};
	template<class _Ptr,
	class _Traits = default_sentry>
	struct sentry
	{
	public:
		sentry(const _Traits& tr = _Traits()) : m_tr(tr) { m_p = (_Ptr)m_tr.default_value(); }
		sentry(_Ptr p, const _Traits& tr = _Traits()) : m_p(p), m_tr(tr) {}
		~sentry() { dispose(); }
		void dispose() { m_tr.destroy(m_p); m_p = (_Ptr)m_tr.default_value(); };
		sentry& operator = (_Ptr p) { if(!m_tr.equal_to(m_p, p)) { m_tr.destroy(m_p); m_p = p; } return *this; }
		_Ptr detach() { _Ptr tmp = m_p; m_p = (_Ptr)m_tr.default_value(); return tmp; }
		operator _Ptr () const { return m_p; }
		_Ptr operator -> () const { return m_p; }
		//	_Ptr* operator & () { return &m_p; }
	public:
		_Ptr m_p;
		_Traits m_tr;
	private:
		sentry(const sentry&);
		sentry& operator = (const sentry&);
	};
};

struct PFN_REG_ITEM
{
	PFN_LAC_PUBLISH_FUNC	pfn;
	LPVOID					opaque;
};
typedef std::map< std::string, PFN_REG_ITEM >	EXPFUNMAP;

struct LAC_STARTUP
{
	EXPFUNMAP	EXPORT_FUNCTIONS;

	lac::Mutex	GEF_LOCK;

	RPC_STATUS	INIT_STATUS;

	LAC_STARTUP()
	{
		INIT_STATUS = RpcServerRegisterIf( IEndPoint_v1_0_s_ifspec, NULL, NULL);
	}
	~LAC_STARTUP()
	{
		if( INIT_STATUS != RPC_S_OK ) return ;
		RpcServerUnregisterIf( IEndPoint_v1_0_s_ifspec, 0, 1 );
	}
};

LAC_STARTUP	LAC_GLOBAL;

inline HRESULT HRESULT_FROM_RPC_STATUS( RPC_STATUS ls )
{
	return (ls==RPC_S_OK) ? S_OK : E_FAIL;
}

/* [async] */ void  invoke_s(
							 /* [in] */ PRPC_ASYNC_STATE invoke_AsyncHandle,
							 handle_t binding,
							 /* [string][in] */ const unsigned char *funName,
							 /* [in] */ long inBytes,
							 /* [size_is][in] */ const byte *inParam,
							 /* [out] */ long *outBytes,
							 /* [size_is][size_is][out] */ byte **outParam)
{
	outBytes[0] = 0;
	outParam[0] = 0;
	//////////////////////////////////////////////////////////////////////////
	EXPFUNMAP::iterator it = LAC_GLOBAL.EXPORT_FUNCTIONS.find( (const char*)funName );

	if( it ==  LAC_GLOBAL.EXPORT_FUNCTIONS.end() )
	{
		RpcAsyncAbortCall( invoke_AsyncHandle, RPC_E_NOT_REGISTERED );
		return ;
	}
	PFN_REG_ITEM entry = it->second;
	if( !entry.pfn  )
	{
		RpcAsyncAbortCall( invoke_AsyncHandle, RPC_E_NOT_REGISTERED );
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	LAC_BLOB lacIn( inParam, inBytes );
	OUT_PARAM lacOut = { outBytes, outParam };

	long lr = entry.pfn( invoke_AsyncHandle, lacIn, lacOut, entry.opaque );

	// if pending, do not call RpcAsyncCompleteCall
	// callee must call RpcAsyncCompleteCall;
	if( lr == E_PENDING ) return ;

	RpcAsyncCompleteCall( invoke_AsyncHandle, &lr );
	return ;
}

//////////////////////////////////////////////////////////////////////////
// 下面的函数是为了满足链接需要而写的，没有的话会出现链接错误
void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
	return (malloc(len));
}

void __RPC_USER midl_user_free( void __RPC_FAR *ptr )
{
	free(ptr);
}

//////////////////////////////////////////////////////////////////////////

#define RPC_USE_LPC		((RPC_WSTR)(L"ncalrpc"))

HRESULT WINAPI LacIsValid()
{
	return LAC_GLOBAL.INIT_STATUS == RPC_S_OK;
}

HRESULT WINAPI LacSetPublishName( LPCWSTR lpcwName )
{
	LAC_STATUS ls = RpcServerUseProtseqEpW( RPC_USE_LPC, RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (RPC_WSTR)lpcwName, NULL );
	return HRESULT_FROM_RPC_STATUS(ls);
}
HRESULT WINAPI LacListen( UINT uiMinThreads, UINT uiMaxCalls, BOOL bWait )
{
	LAC_STATUS ls = RpcServerListen( uiMinThreads, uiMaxCalls, bWait == FALSE );
	return HRESULT_FROM_RPC_STATUS(ls);
}
HRESULT WINAPI LacOpenRemote( LPCWSTR lpcName, LPHREMOTE hRemote )
{
	if( !lpcName || !hRemote ) return E_INVALIDARG;
	RPC_WSTR wsBinding = NULL;
	LAC_STATUS ls = RpcStringBindingComposeW( NULL, RPC_USE_LPC, NULL, (RPC_WSTR)lpcName, NULL, &wsBinding );
	if( ls != RPC_S_OK ) return HRESULT_FROM_RPC_STATUS(ls);
	handle_t hBinding = 0;
	ls = RpcBindingFromStringBindingW( wsBinding, &hBinding );
	if( ls != RPC_S_OK ) return HRESULT_FROM_RPC_STATUS(ls);
	RpcStringFreeW(&wsBinding);
	hRemote[0] = hBinding;
	return HRESULT_FROM_RPC_STATUS(ls);
};

HRESULT WINAPI LacCloseRemote( HREMOTE hRemote )
{
	handle_t hBinding = (handle_t)hRemote;
	LAC_STATUS ls = RpcBindingFree(&hBinding);
	return HRESULT_FROM_RPC_STATUS(ls);
}
HRESULT WINAPI LacRegisterFunction( LPCSTR lpcName, PFN_LAC_PUBLISH_FUNC pfn, LPVOID vpOpaque )
{
	if( !lpcName || !pfn ) return E_INVALIDARG;
	lac::scope _scope( LAC_GLOBAL.GEF_LOCK );
	std::string strName = lpcName;
	EXPFUNMAP::iterator it = LAC_GLOBAL.EXPORT_FUNCTIONS.find(lpcName);
	if( it != LAC_GLOBAL.EXPORT_FUNCTIONS.end() ) return E_UNEXPECTED;
	PFN_REG_ITEM item = { pfn, vpOpaque };
	LAC_GLOBAL.EXPORT_FUNCTIONS[strName] = item;
	return S_OK;
}
HRESULT WINAPI LacRevokeFunction( LPCSTR lpcName )
{
	if( !lpcName ) return E_INVALIDARG;
	lac::scope _scope( LAC_GLOBAL.GEF_LOCK );
	std::string strName = lpcName;
	EXPFUNMAP::iterator it = LAC_GLOBAL.EXPORT_FUNCTIONS.find(lpcName);
	if( it == LAC_GLOBAL.EXPORT_FUNCTIONS.end() ) return E_UNEXPECTED;
	LAC_GLOBAL.EXPORT_FUNCTIONS.erase( strName );
	return S_OK;
}
HRESULT WINAPI LacSetOutParam( OUT_PARAM & outParam, LAC_BLOB_C& OutData )
{
	outParam.outBytes[0] = 0;
	outParam.outParam[0] = 0;
	if( !OutData.Length ) return S_OK;
	void * p = midl_user_allocate( OutData.Length );
	if( !p ) return E_OUTOFMEMORY;
	memcpy( p, OutData.Data, OutData.Length );
	outParam.outBytes[0] = OutData.Length;
	outParam.outParam[0] = (byte*)p;
	return S_OK;
}

HRESULT WINAPI LacCompleteInvoke( HINVOKE hInvoke, OUT_PARAM & outParam, HRESULT& hr, LAC_BLOB_C & OutData )
{
	if( SUCCEEDED(hr) )
	{
		HRESULT hr2 = LacSetOutParam( outParam, OutData );
		if( FAILED(hr2) ) hr = hr2;
	}
	return RpcAsyncCompleteCall( (PRPC_ASYNC_STATE)hInvoke, &hr );
}

//////////////////////////////////////////////////////////////////////////

LAC_STATUS __invoke_c( PRPC_ASYNC_STATE hAsync,
					  handle_t hBinding,
					  const char *funName,
					  const void *inParam,
					  long inBytes,
					  long *outBytes,
					  byte **outParam )
{
	RpcTryExcept
	{
		invoke_c(
			hAsync,
			hBinding,
			( const unsigned char *)funName,
			inBytes, ( const byte * )inParam, outBytes, outParam );
	}
	RpcExcept(1)
	{
		return RpcExceptionCode();
	}
	RpcEndExcept;
	return RPC_S_OK;
}


struct INVOKE_DESC : RPC_ASYNC_STATE
{
	lac::Mutex				mLock;
	handle_t				hBinding;
	long					aOutBytes;
	byte *					aOutData;
	refp<LAC_CALLBACK>		cCallback;
	long					bCompleted;		// SET BY TIMEOUT_PROC
	HANDLE					hTimer;
	~INVOKE_DESC()
	{
		if( !hTimer ) return ;
		lac::scope _sync_on_oinvoke( mLock );
		bCompleted = TRUE;
		if( !DeleteTimerQueueTimer( NULL, hTimer, INVALID_HANDLE_VALUE ) )
		{
			DWORD dwErr = GetLastError();
			//__asm int 3;
		}
	}
};
typedef INVOKE_DESC*		LPINVOKE;
typedef LPINVOKE*			PPINVOKE;

static void _free_midl_blob( LAC_BLOB * pb )
{
	if( !pb ) return;
	if( pb->Data ) midl_user_free( (void*)(pb->Data) );
	pb->Data = 0;
	pb->Length = 0;
}

void RPC_ENTRY LAC_RPCNOTIFICATION_ROUTINE( PRPC_ASYNC_STATE hAsync, void * Context, RPC_ASYNC_EVENT Event )
{
	lac::sentry<LPINVOKE> oInvoke = (LPINVOKE)hAsync;
	{
		lac::scope _sync_on_oinvoke( oInvoke->mLock );
		oInvoke->bCompleted = TRUE;
	}
	refp<LAC_CALLBACK> cbp = ( oInvoke->cCallback );
	switch( Event )
	{
	case RpcCallComplete:
		{
			long lr = 0;
			LAC_STATUS ls = RpcAsyncCompleteCall( hAsync, &lr );
			if( ls == RPC_S_OK )
			{
				LAC_BLOB outParam( oInvoke->aOutData, oInvoke->aOutBytes, _free_midl_blob );
				if( cbp ) cbp->OnComplete( oInvoke->hBinding, ls, lr, outParam );
			}
			else
			{
				LAC_BLOB outParam;
				if( cbp ) cbp->OnComplete( oInvoke->hBinding, ls, E_UNEXPECTED, outParam );
			}
		} break;
	case RpcClientDisconnect:
		{
			LAC_BLOB outParam;
			if( cbp ) cbp->OnComplete( oInvoke->hBinding, RPC_E_CLIENT_DIED, E_UNEXPECTED, outParam );
		}break;
	};
}


VOID CALLBACK LacCancelAsyncRpcCall( __in  PVOID lpParameter,__in  BOOLEAN TimerOrWaitFired)
{
	if( !lpParameter ) return ;
	INVOKE_DESC * oInvoke = (INVOKE_DESC *)lpParameter;
	{
		lac::scope _sync_on_this_oinvoke( oInvoke->mLock );
		if( oInvoke->bCompleted ) return;
	}
	/*LAC_STATUS ls = */RpcAsyncCancelCall( oInvoke, TRUE );
}

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI LacInvokeSync( HREMOTE hRemote, LPCSTR fnName, LAC_BLOB_C & inParam, LPVOID reply, LAC_BLOB & outParam, DWORD dwTimeout )
{
	if( !hRemote || !fnName ) return E_INVALIDARG;
	handle_t hBinding = (handle_t)hRemote;

	lac::sentry< INVOKE_DESC* > oInvoke = new INVOKE_DESC();
	if( !oInvoke ) return E_OUTOFMEMORY;

	LAC_STATUS ls = RpcAsyncInitializeHandle( oInvoke, sizeof(RPC_ASYNC_STATE) );
	if( ls != RPC_S_OK ) return RPC_E_FAULT;

	HANDLE hEvent = CreateEventA(0,FALSE,FALSE,0);
	if( !hEvent ) return RPC_E_OUT_OF_RESOURCES;

	oInvoke->UserInfo = NULL;
	oInvoke->NotificationType = RpcNotificationTypeEvent;
	oInvoke->u.hEvent = hEvent;
	oInvoke->cCallback = 0;
	oInvoke->aOutBytes = 0;
	oInvoke->aOutData = 0;
	oInvoke->hBinding  = hBinding;
	oInvoke->bCompleted = FALSE;
	oInvoke->hTimer = 0;

	ls = __invoke_c( oInvoke, hBinding, fnName, inParam.Data, inParam.Length, &(oInvoke->aOutBytes), &(oInvoke->aOutData) );
	if( ls != RPC_S_OK )
	{
		CloseHandle(hEvent);
		return RPC_E_FAULT;
	}

	while(1)
	{
		DWORD wr = WaitForSingleObject( hEvent, dwTimeout );
		if( wr != WAIT_OBJECT_0 )
		{
			// 超时，取消异步调用
			RpcAsyncCancelCall( oInvoke, TRUE );
			dwTimeout = -1;
			continue;										// ------^
		}
		if( oInvoke->Event == RpcCallComplete )
		{
			// 完成了
			ls = RpcAsyncCompleteCall( oInvoke, reply );
			if( ls == RPC_S_ASYNC_CALL_PENDING )
			{
				// 还没完成，则取消
				RpcAsyncCancelCall( oInvoke, true );
				dwTimeout = -1;
				continue;									// ------^
			}
			if( ls == RPC_S_OK )
			{
				// 成功完成
				outParam.Data = oInvoke->aOutData;
				outParam.Length = oInvoke->aOutBytes;
				outParam.FreePtr = _free_midl_blob;
			}
			else if( ls == RPC_S_CALL_CANCELLED )
			{
				// 取消完成
				ls = RPC_E_TIMEOUT;
			}
			else
			{
				ls = RPC_E_FAULT;
			}
		}
		else if( oInvoke->Event == RpcClientDisconnect )
		{
			ls = RPC_E_CLIENT_DIED;
		}
		break;												// ------v
	}
	CloseHandle(hEvent);
	return ls;
}

HRESULT WINAPI LacInvokeAsyn( HREMOTE hRemote, LPCSTR fnName, LAC_BLOB_C & inParam, LPLAC_CALLBACK cbComplete, DWORD dwTimeout, LPHINVOKE hpInvoke )
{
	if( !hRemote || !fnName ) return E_INVALIDARG;

	handle_t hBinding = (handle_t)hRemote;

	enum { MINI_TIMEOUT = 80 };
	dwTimeout += MINI_TIMEOUT;

	lac::sentry< INVOKE_DESC* > oInvoke = new INVOKE_DESC();
	if( !oInvoke ) return E_OUTOFMEMORY;

	lac::scope _sync_on_this_oinvoke( oInvoke->mLock );

	LAC_STATUS ls = RpcAsyncInitializeHandle( oInvoke, sizeof(RPC_ASYNC_STATE) );
	if( ls != RPC_S_OK ) return ls;

	oInvoke->UserInfo = NULL;
	oInvoke->NotificationType = RpcNotificationTypeCallback;
	oInvoke->u.NotificationRoutine = LAC_RPCNOTIFICATION_ROUTINE;
	oInvoke->cCallback = cbComplete;
	oInvoke->aOutBytes = 0;
	oInvoke->aOutData = 0;
	oInvoke->hBinding = hBinding;
	oInvoke->bCompleted = FALSE;
	oInvoke->hTimer = NULL;

	ls = __invoke_c( oInvoke, hBinding, fnName, inParam.Data, inParam.Length, &(oInvoke->aOutBytes), &(oInvoke->aOutData) );

	if( ls != RPC_S_OK )
		return HRESULT_FROM_RPC_STATUS(ls);

	if( dwTimeout != -1 )
	{
		HANDLE hTimer = NULL;
		if( CreateTimerQueueTimer( &hTimer, NULL, LacCancelAsyncRpcCall, oInvoke.m_p, dwTimeout, 0, 0 ) )
		{
			oInvoke->hTimer = hTimer;
		}
		else
		{
			RpcAsyncCancelCall( oInvoke, true );
			ls = RPC_E_OUT_OF_RESOURCES;
		}
	}
	if( hpInvoke ) hpInvoke[0] = oInvoke;

	oInvoke.detach();
	return ls;
}

HRESULT WINAPI LacCancelInvoke( HINVOKE hInvoke )
{
	if( !hInvoke ) return E_INVALIDARG;
	INVOKE_DESC * oInvoke = (INVOKE_DESC*)hInvoke;
	LAC_STATUS ls = RpcAsyncCancelCall( oInvoke, TRUE );
	return HRESULT_FROM_RPC_STATUS(ls);
}

HRESULT WINAPI LacGetClientId( HINVOKE hInvoke, ULONG & rPID )
{
	if( !hInvoke ) return E_INVALIDARG;
	HANDLE hCallHandle = RpcAsyncGetCallHandle(hInvoke);
	return ( I_RpcBindingInqLocalClientPID(hCallHandle, &rPID ) );
}

HRESULT WINAPI LacShutdown()
{
	RpcMgmtStopServerListening(0);
	return S_OK;
}

#endif
