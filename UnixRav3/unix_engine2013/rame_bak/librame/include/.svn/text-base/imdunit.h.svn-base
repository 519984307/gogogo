#ifndef __RX_PROCESS_NODE__
#define __RX_PROCESS_NODE__

#include "inamedb.h"

typedef nam::MAIN_CLASS		MMCLS;

#define		HIT_MALWARE		nam::kMalware
#define		HIT_TRUSTED		nam::kTrusted
#define		HIT_HIDDEN		nam::kHidden
#define		HIT_INFECTED	nam::kVirus


typedef struct _DETECTR
{
	MMCLS	Class;
	SGID40	SigId;
} DETECTR, *LPDETECTR;

enum XDR_INDEX
{
	XDR_VID32					= 0,	// lpData = &UINT32;	
	XDR_VID40_SGID40,					// lpData = &SGID40;
	XDR_MALWCLASS_8,					// lpData = &UINT32;
	XDR_NGINNAME_CPTR,					// lpData = &LPCSTR;	15  max
	XDR_MALWNAME_CPTR,					// lpData = &LPCSTR;	255 max
	XDR_SIGTAG_CPTR,					// lpData = &LPCSTR;	16  max

	XDR_INDEX_MAX = 32,
};

struct IExtDetectionResult 
{
	STDMETHOD_(LPDETECTR, ResultV1Ptr)() PURE;
	STDMETHOD_(LPCSTR,	EngineName)()	PURE;
	STDMETHOD_(LPCSTR,	MalwareName)() PURE;
	STDMETHOD_(MMCLS,	MalwareClass)() PURE;
	STDMETHOD_(UINT32,	MalwareId32)()	PURE;
	STDMETHOD_(SGID40,	MalwareId40)()	PURE;
	STDMETHOD_(LPCSTR,	SignatureTag)(SIZE_T * lpSize = NULL)	PURE;
	// return DataSize, <=0 is Failed;
	STDMETHOD_(LONG, GetValue)( XDR_INDEX uIndex, PVOID lpDataRecv ) PURE;
};

struct IInnertDetectionResult : IExtDetectionResult
{
	STDMETHOD( SetMalwBasicInfo )( MMCLS mcls, UINT32 id32, UINT8 area ) PURE;
	STDMETHOD( SetMalwBasicInfo )( MMCLS mcls, UINT32 id32 ) PURE;
	STDMETHOD( SetEngineName )( LPCSTR engineName ) PURE;
	STDMETHOD( SetMalwareName )( LPCSTR malwareName ) PURE;
	STDMETHOD( SetSignatureTag )( LPCSTR signTag, SIZE_T size ) PURE;
};

static inline void ResetDetectResult ( LPDETECTR p ) 
{
	p->Class = nam::kTrait;
	p->SigId.AreaId = 0;
	p->SigId.Id32 = 0;
}

typedef enum TREAT_RESULT
{
	TR_S_TREAT = 0,
	TR_S_IGNORE,

	TR_E_FAIL = -1,
	// report @ kill virus/end scan type:
	TR_E_REOPEN = -2,			//写方式打开失败 (only used by proxy)
	TR_E_KILL = -3,				//杀毒失败	
	TR_E_DELETE = -4,			//文件删除失败
	// 
	TR_E_MAX_COUNT = -5,			//超过杀毒最大次数 (only used by proxy)
	TR_E_ROLLBACK = -6,			//回退到父对象失败 (only used by proxy)
	// report @ end scan type (if rollback, report @ kill virus):
	TR_E_POST_TREAT = -7,		//杀毒后处理失败 (only used by proxy)
	// report @ kill virus/end scan type:
	TR_E_COMPOUND_UPDATE = -8,	//复合文档回写失败 (only used by proxy)
} TRESULT;

enum 
{
	NOTIFY_ITEM_EXTRACT,
	NOTIFY_PE_REVMEXIT,
	NOTIFY_MP_PATTERN_HIT,
};

class IRXDetectContext;


class IRXDetectNode : public IUnknown
{
public:

	RE_DECLARE_IID;

	STDMETHOD(Detect)( IUnknown *punkArchive, LPDETECTR lpResult ) PURE;

	STDMETHOD_(TRESULT,Treat)( IUnknown *punkArchive, LPDETECTR lpResult ) PURE;

	STDMETHOD(Reset)() PURE;

};

// {D6B2E3DA-CB50-41FA-85FB-6CF1395F7A91}
RE_DEFINE_IID( IRXDetectNode, "{D6B2E3DA-CB50-41FA-85FB-6CF1395F7A91}",
			0xd6b2e3da, 0xcb50, 0x41fa, 0x85, 0xfb, 0x6c, 0xf1, 0x39, 0x5f, 0x7a, 0x91);



enum 
{
	PE_SCAN_DELETABLE	= 1,
	PE_SCAN_CLEANABLE	= 2,
	PE_SCAN_UNPACKABLE	= 4,

	PE_SCAN_MALWARE		= (PE_SCAN_DELETABLE|PE_SCAN_CLEANABLE),
	PE_SCAN_VIRUS		= PE_SCAN_CLEANABLE,
	PE_SCAN_PACKER		= PE_SCAN_UNPACKABLE,
	PE_SCAN_ALL			= PE_SCAN_MALWARE|PE_SCAN_VIRUS|PE_SCAN_PACKER,
};

class IRXDetectNode2 : public IRXDetectNode
{
public:

	RE_DECLARE_IID;

	STDMETHOD(SetContext)( IRXDetectContext * lpContext ) PURE;
	STDMETHOD(NotifyDetect)( IUnknown* lpSender, SIZE_T uNotifyCode, LPCVOID lpNotifyParam, LPDETECTR lpResult ) PURE;
	STDMETHOD(CaptureNotify)( IRXDetectNode2* oCapturer, SIZE_T uNotifyCode, LPCVOID lpCaptureParam ) PURE;
	STDMETHOD(DelayReset)() PURE;
	STDMETHOD(Control)( SIZE_T uControlCode, LPCVOID lpParam, LPVOID lpReturn ) PURE;
	STDMETHOD(NotifyTreat)( IUnknown* lpSender, LPDETECTR lpResult ) PURE;
};

// {3952E715-9760-401B-80A8-F0B44D6D92DA}
RE_DEFINE_IID( IRXDetectNode2, "{3952E715-9760-401B-80A8-F0B44D6D92DA}",
			0x3952e715, 0x9760, 0x401b, 0x80, 0xa8, 0xf0, 0xb4, 0x4d, 0x6d, 0x92, 0xda);
class IRXDetectNode3 : public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD(Detect)( IUnknown *punkArchive, IInnertDetectionResult * lpResult ) PURE;
	STDMETHOD_(TRESULT,Treat)( IUnknown *punkArchive, IExtDetectionResult * lpResult ) PURE;
};

// {AD23142F-046B-43B0-91A9-A489D3D92C92}
RE_DEFINE_IID(IRXDetectNode3, "{AD23142F-046B-43B0-91A9-A489D3D92C92}",
			0xad23142f, 0x46b, 0x43b0, 0x91, 0xa9, 0xa4, 0x89, 0xd3, 0xd9, 0x2c, 0x92);


class IRXWithParamNode : public IUnknown
{
public:

	RE_DECLARE_IID;

	STDMETHOD(FlushParam)( LPCVOID lpParam, SIZE_T cbParam ) PURE;

};

// {31ECB42B-687C-41BC-BC8D-BAADA39AFDBA}
RE_DEFINE_IID(IRXWithParamNode, "{31ECB42B-687C-41BC-BC8D-BAADA39AFDBA}",
			  0x31ecb42b, 0x687c, 0x41bc, 0xbc, 0x8d, 0xba, 0xad, 0xa3, 0x9a, 0xfd, 0xba);

class IRXDetectNodeGetSignKey : public IUnknown
{
public:
	STDMETHOD_(LPCSTR,GetCurrentSigKey)() PURE;
};

class IRXDetectContext
{
public:

	//
	//	只有支持IRXDetectNode2的DetectNode找出来才有意义
	//

	STDMETHOD( FindMduNode2 )( LPCSTR aMduName, IRXDetectNode2** ppNode2, BOOL bFullProgId = FALSE ) PURE;

	STDMETHOD( PutData )( LPCSTR aName, xv::Value& rValue ) PURE;

	STDMETHOD( GetData )( LPCSTR aName, xv::Value& rValue, BOOL bDelete = TRUE ) PURE;

	STDMETHOD( GetCallback )( IUnknown** ppCallback ) PURE;
};



class IRXEngEnv13 : public IUnknown
{
public:

	RE_DECLARE_IID;

	STDMETHOD(Init)(LPCSTR lpLibPath) PURE;
};


//{9F7CC16B-ECE3-4EBB-8B48-D65A5E2DC1BE}
RE_DEFINE_IID(IRXEngEnv13, "{9F7CC16B-ECE3-4EBB-8B48-D65A5E2DC1BE}",
			  0x9f7cc16b, 0xece3, 0x4ebb, 0x8b, 0x48, 0xd6, 0x5a, 0x5e, 0x2d, 0xc1, 0xbe);


class IRXEngScanScope : public IUnknown
{
public:

	RE_DECLARE_IID;

	STDMETHOD(Enter)() PURE;
	STDMETHOD(Leave)() PURE;
};

// {0E3BF2CF-DBBC-461B-B9C6-344AB0FDBB42}
RE_DEFINE_IID(IRXEngScanScope, "{0E3BF2CF-DBBC-461B-B9C6-344AB0FDBB42}",
			0xe3bf2cf, 0xdbbc, 0x461b, 0xb9, 0xc6, 0x34, 0x4a, 0xb0, 0xfd, 0xbb, 0x42);



class IRXEng13VDB : public IUnknown
{
public:

	RE_DECLARE_IID;

	STDMETHOD(GetVirusName)(UINT32 vid ,LPSTR LpName , size_t cbName) PURE;
};

// {7FF655C2-8830-4D05-8D40-D436F61128AE}
RE_DEFINE_IID(IRXEng13VDB, "{7FF655C2-8830-4D05-8D40-D436F61128AE}",
			  0x7ff655c2, 0x8830, 0x4d05, 0x8d, 0x40, 0xd4, 0x36, 0xf6, 0x11, 0x28, 0xae);


class IREStreamPrxy13 : public IUnknown
{
public:
	//
	// read / write / flush
	//
	STDMETHOD(Read)(
		LPVOID lpBuffer, 
		DWORD cbBytesToRead, 
		OUT LPDWORD lpBytesRead OPTIONAL
		) PURE;
	STDMETHOD(Write)(
		LPCVOID lpBuffer, 
		DWORD cbBytesToWrite,
		OUT LPDWORD lpBytesWritten OPTIONAL
		) PURE;
	STDMETHOD(Flush)(
		) PURE;
	//
	// position / length
	//
	STDMETHOD(Seek)(
		LONG lDistanceToMove,
		PLONG lpDistanceToMoveHigh OPTIONAL,
		DWORD dwMoveMethod
		) PURE;
	STDMETHOD(Tell)(
		OUT LPDWORD lpOffsetLow,
		OUT LPDWORD lpOffsetHigh OPTIONAL
		) PURE;
	STDMETHOD(GetSize)(
		OUT LPDWORD lpSizeLow,
		OUT LPDWORD lpSizeHigh OPTIONAL
		) PURE;
	STDMETHOD(SetSize)(
		DWORD dwSizeLow,
		LPDWORD lpSizeHigh OPTIONAL
		) PURE;
	//
	// status
	//
	STDMETHOD_(DWORD, GetStatus)( 
		) PURE;

	STDMETHOD(Remove)( 
		) PURE;
	STDMETHOD(Close)( 
		) PURE;
	STDMETHOD(SetName)( REFCXSTRP rName )PURE;
	STDMETHOD(GetName)( REFXSTRP rName )PURE;
	STDMETHOD(GetFormat)(DWORD* pFmt)PURE;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREStreamPrxy13, "{FF659A67-7C16-4e4d-BD2C-4EE827A822F8}",
			  0xff659a67, 0x7c16, 0x4e4d, 0xbd, 0x2c, 0x4e, 0xe8, 0x27, 0xa8, 0x22, 0xf8);



class IREArchivePrx13 : public IUnknown
{
public:
	STDMETHOD(Open)(IUnknown *punk) = 0;
	STDMETHOD(Close)() = 0;
	STDMETHOD(GetStreamObject)(IREStreamPrxy13 **ppstm) = 0;
	// ......
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREArchivePrx13, "{69C7750C-139D-4651-92CA-33419C54D364}",
			  0x69c7750c, 0x139d, 0x4651, 0x92, 0xca, 0x33, 0x41, 0x9c, 0x54, 0xd3, 0x64);

#include "mducc.h"

#endif