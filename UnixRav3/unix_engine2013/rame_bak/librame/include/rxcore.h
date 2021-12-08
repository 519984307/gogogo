#ifndef __R4_CORE__
#define __R4_CORE__


#include "r4com.h"
#include "r4fmt.h"

class R4Global;
class R4FileScan;
class R4StreamScan;

struct CHAIN_INFO
{
	SIZE_T	Length;
	LPCGUID	HandleIDs;
};
HRESULT WINAPI r4QueryHandlerChain( UINT32 uFmtID, CHAIN_INFO & rInfo );

class IR4HandlerChain : public IUnknown
{
public:
	STDMETHOD(Assign)( CHAIN_INFO & rInfo ) PURE;
	STDMETHOD(Remove)( REFGUID uuidHandler ) PURE;
	STDMETHOD(Queued)( REFGUID uuidHandler ) PURE;
	STDMETHOD(Dequeued)( GUID& uuidHandler ) PURE;
	STDMETHOD(Insert)( REFGUID uuidInsert, REFGUID uuidHandler, BOOL bQueued ) PURE;
	STDMETHOD(Replace)( REFGUID uuidInsert, REFGUID uuidHandler, BOOL bQueued ) PURE;
public:
	RE_DECLARE_IID;
};

// {B9D490D7-D457-4a1b-80CA-BDF304366787}
RE_DEFINE_IID( IR4HandlerChain, "{B9D490D7-D457-4a1b-80CA-BDF304366787}",
			  0xb9d490d7, 0xd457, 0x4a1b, 0x80, 0xca, 0xbd, 0xf3, 0x4, 0x36, 0x67, 0x87);

//////////////////////////////////////////////////////////////////////////
enum ALARM_LEVEL
{
	AL_TEST			= 0x20,		// 测试
	AL_SUBMIT		= 0x60,		// 提交

	AL_ALARMABLE	= 0x80,		// --- 可提示用户 ---

	AL_HEUR			= 0xB0,		// 启发
	AL_VARIANT		= 0xD0,		// 变种
	AL_EXACT		= 0xF0,		// 精确
};

enum ALARM_RESPONSE
{
	ALR_IGNORE,
	ALR_TREAT,
};

enum RISK_TREAT
{
	RT_REMOVE,
	RT_FIX,
};

enum FLOWCC
{
	FC_CONTINUE,
	FC_SKIP_THIS,
	FC_STOP_ALL,
};

struct IR4VirtualPath : IUnknown
{
	STDMETHOD_(DWORD,GetFileNestDepth)( DWORD dwIndex ) PURE;
	STDMETHOD_(LPCWSTR,GetFileNameAt)( DWORD dwIndex ) PURE;
	STDMETHOD(ToBuffer)( PVOID vpBuffer, SIZE_T * upBytes ) PURE;
	STDMETHOD(FromBuffer)( PVOID vpBuffer, SIZE_T * ioBytes ) PURE;
	STDMETHOD_(SIZE_T, LengthOfReadablePath)( ) PURE;
	STDMETHOD(ToReadablePath)( LPWSTR wName, SIZE_T cCharNumIZ ) PURE;
	STDMETHOD(Clone)( IR4VirtualPath** ppVirtualPath ) PURE;
};

enum { ANAME_LEN = 64 };
struct ALARM_INFO
{
	UUID							uidRoot;
	SIZE_T							aDepth;
	UINT32							aId;
	ALARM_LEVEL						aLevel;
	CHAR							aName[ANAME_LEN];
};

//////////////////////////////////////////////////////////////////////////

struct IR4AlarmCache : public IUnknown
{
	STDMETHOD( GetAlarmInfo )() PURE;
	STDMETHOD( SetAlarmInfo )() PURE;
};

//////////////////////////////////////////////////////////////////////////
struct R4FileFrame
{
	UUID							uidRoot;
	UTIL::co<IR4RunTime>			oRuntime;
};

struct R4ScanFrame
{
	SIZE_T							uDepth;
	UTIL::co<IR4VirtualPath>		oCurrentPath;
	UTIL::co<IR4Context>			oContext;
	UTIL::co<IRXStream>				oTarget;
	UTIL::co<IR4FormatList>			oFormatList;
	SIZE_T							uFormatIndex;
	R4_FORMAT						oCurrentFormat;
	UTIL::com_ptr<IR4HandlerChain>	oHandlerChain;
	UTIL::com_ptr<IRXArchive>		oArchive;
	FLOWCC							ccFlow;
};

// IUnknown
// {F8382076-8902-464d-843A-E2445DDB868F}
DEFINE_GUID( CTX_ORGINAL_STREAM,	0xf8382076, 0x8902, 0x464d, 0x84, 0x3a, 0xe2, 0x44, 0x5d, 0xdb, 0x86, 0x8f);
// IUnknown
// {E7FAA806-53B0-41b8-8F57-D542F4D9A97A}
DEFINE_GUID( CTX_HANDLER_CHAIN,		0xe7faa806, 0x53b0, 0x41b8, 0x8f, 0x57, 0xd5, 0x42, 0xf4, 0xd9, 0xa9, 0x7a);
// IUnknown
// {F828BCE2-03D0-4c03-A50C-C8ACA2F43558}
DEFINE_GUID( CTX_FORMAT_LIST,		0xf828bce2, 0x3d0, 0x4c03, 0xa5, 0xc, 0xc8, 0xac, 0xa2, 0xf4, 0x35, 0x58);
// IUnknown
// {221B2684-B429-4d99-B541-5F18880E1EBC}
DEFINE_GUID( CTX_STREAM_TRATIS,		0x221b2684, 0xb429, 0x4d99, 0xb5, 0x41, 0x5f, 0x18, 0x88, 0xe, 0x1e, 0xbc);

//////////////////////////////////////////////////////////////////////////

class IR4Handler : public IUnknown
{
public:
	STDMETHOD( Handle )( R4ScanFrame& rFrame ) PURE;
public:
	RE_DECLARE_IID;
};

// {DA5A6653-4FAB-40de-B72D-EC968B6E0696}
RE_DEFINE_IID(IR4Handler, "{DA5A6653-4FAB-40de-B72D-EC968B6E0696}",
			  0xda5a6653, 0x4fab, 0x40de, 0xb7, 0x2d, 0xec, 0x96, 0x8b, 0x6e, 0x6, 0x96);

//////////////////////////////////////////////////////////////////////////

#endif