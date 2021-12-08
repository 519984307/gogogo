#ifndef __RE_FRONT__
#define __RE_FRONT__

#include "../../include/xvalue.h"
#include "recfmt.h"
#include "imdunit.h"
#include "filefmt.h"

//////////////////////////////////////////////////////////////////////////
class IRXParameters : public IUnknown
{
public:
	STDMETHOD(SetParam)( LPCSTR aParamPath, xv::Value& v ) PURE;
	STDMETHOD(GetParam)( LPCSTR aParamPath, xv::Value& v ) PURE;
	STDMETHOD(SetParamString)( LPCSTR aParamString ) PURE;
	STDMETHOD(Clear)() PURE;
	RE_DECLARE_IID;
};
// {26056B73-D5E5-437B-B4FE-D7F318F527DF}
RE_DEFINE_IID( IRXParameters, "{26056B73-D5E5-437B-B4FE-D7F318F527DF}",
			  0x26056b73, 0xd5e5, 0x437b, 0xb4, 0xfe, 0xd7, 0xf3, 0x18, 0xf5, 0x27, 0xdf);

class IRXParameters2 : public IRXParameters
{
public:
	STDMETHOD(Remove)(LPCSTR aParamPath)PURE;
	RE_DECLARE_IID;
};

// {BDC17B5A-EFBD-44A1-B4A7-5A84EB02B504}
RE_DEFINE_IID(IRXParameters2, "{BDC17B5A-EFBD-44A1-B4A7-5A84EB02B504}",
			0xbdc17b5a, 0xefbd, 0x44a1, 0xb4, 0xa7, 0x5a, 0x84, 0xeb, 0x2, 0xb5, 0x4);
			

class IRXAuthentication: public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD(Authenticate)(LPCSTR lpCertFilePath , LPCSTR lpJson)PURE;
	//从参数表中获取相关参数进行校验，并将校验后的某些信息设置到参数中，例如支持的格式
	//适合引擎内部证书校验
	STDMETHOD(Authenticate)(LPCSTR lpCertFilePath , IRXParameters* lpParam)PURE;
	STDMETHOD(Authenticate)(LPCSTR lpCertFilePath , LPCSTR lpJson,IRXParameters* lpParam)PURE;
};

// {330A778F-37AD-4886-8354-B43CCE4AF267}
RE_DEFINE_IID(IRXAuthentication, "{330A778F-37AD-4886-8354-B43CCE4AF267}",
			  0x330a778f, 0x37ad, 0x4886, 0x83, 0x54, 0xb4, 0x3c, 0xce, 0x4a, 0xf2, 0x67);


class IRXAuthenticationW: public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD(Authenticate)(LPCWSTR lpCertFilePath , LPCSTR lpJson)PURE;
	//从参数表中获取相关参数进行校验，并将校验后的某些信息设置到参数中，例如支持的格式
	//适合引擎内部证书校验
	STDMETHOD(Authenticate)(LPCWSTR lpCertFilePath , IRXParameters* lpParam)PURE;
	STDMETHOD(Authenticate)(LPCWSTR lpCertFilePath , LPCSTR lpJson,IRXParameters* lpParam)PURE;
};

// {6F64052C-F20A-4E05-A52B-0C0ED3B6811C}
RE_DEFINE_IID(IRXAuthenticationW, "6F64052C-F20A-4E05-A52B-0C0ED3B6811C",
			0x6f64052c, 0xf20a, 0x4e05, 0xa5, 0x2b, 0xc, 0xe, 0xd3, 0xb6, 0x81, 0x1c);


//class IRXStrategy : public IRXParameters
//{
//public:
//	STDMETHOD(EnableFormat)( FFMTID fmtid, BOOL bEnable ) PURE;
//	STDMETHOD(EnableMdu)( LPCSTR aMduName, BOOL bFull, BOOL bEnable ) PURE; 
//	STDMETHOD(EnableFormatClass)( UINT64 uMask, BOOL bEnable ) PURE;
//	STDMETHOD(SetKnownValue)( LPCSTR aKnownField, xv::Value& rValue ) PURE;
//	STDMETHOD(GetKnownValue)( LPCSTR aKnownField, xv::Value& rValue ) PURE;
//	RE_DECLARE_IID;
//};
//
//// {30ADE70A-22FA-4ADA-992E-7590855FA787}
//RE_DEFINE_IID( IRXStrategy, "{30ADE70A-22FA-4ADA-992E-7590855FA787}",
//			0x30ade70a, 0x22fa, 0x4ada, 0x99, 0x2e, 0x75, 0x90, 0x85, 0x5f, 0xa7, 0x87);


namespace _13
{
	enum EFC
	{
		EFC_CONTINUE,					// 继续查杀毒流程
		EFC_IGNORE,						// 忽略：忽略当前的扫描文件；
		EFC_ABORT,						// 终止：全部扫描

		EFC_INTERNAL = 0x80000000,	
		IEFC_RESCAN,
	};

	enum TEFC
	{
		TEFC_DEFAULT,					// 默认方式（根据配置）
		TEFC_IGNORE,					// 忽略
		TEFC_CLEAN,						// 清除
		TEFC_REMOVE,					// 删除
		TEFC_ABORT,						// 
	};

	typedef enum FILE_LEAVE_CASE
	{
		FLC_NORMAL,						// 正常扫描完成

		FLC_IGNORED,					// 被忽略
		FLC_IGNORED_BY_WL,				// 被白名单忽略
		FLC_IGNORED_BY_FILTER,			// 被过滤器过滤
		FLC_IGNORED_BY_USER,			// 被控制码忽略
		FLC_IGNORED_BY_PARAM,			// 被参数忽略

		FLC_BREAK_TREAT_FAILED,			

	} LEAVECASE;

	enum _PROCOPTS
	{
		PROC_WILL_TREAT = 1,
		PROC_CC_BOOST = 2,
		PROC_NO_MAPPING = 4,
		PROC_NO_SEH = 8,
		PROC_ALRAM_PASS = 16, // PROC_WILL_TREAT === 0
		PROC_NO_PREREAD = 32, // 
		PROC_LEGACY_FORMATS_MODE = 64,
		PROC_NO_COW_STREAM = 128,
	};
	typedef SIZE_T PROCOPT;

	class IRXCallback : public IUnknown
	{
	public:

		RE_DECLARE_IID;

		STDMETHOD_(PROCOPT, GetProcOptions )( ) PURE;

		STDMETHOD_(EFC,EnterFile)( IRXStream * pstm, SIZE_T uNest ) PURE;
		STDMETHOD_(VOID,LeaveFile)( IRXStream * pstm, SIZE_T uNest, LEAVECASE uCase ) PURE;

		STDMETHOD_(VOID,EnterArchive)( IRXArchive * arch ) PURE;
		STDMETHOD_(VOID,LeaveArchive)( IRXArchive * arch, TRESULT trRebuild ) PURE;

		STDMETHOD_(TEFC, OnAlarm)( IRXArchive * arch, SGID40 uSigId ) PURE;
		STDMETHOD_(TEFC, OnTreated)( IRXArchive * arch, SGID40 uSigId, TRESULT trTreat ) PURE;

		// 询问压缩包密码等功能
		STDMETHOD(OnComplexCallback)( xv::Value& inVar, xv::Value& outVar ) PURE;

		STDMETHOD(QueryLibFileVersion)( LPCSTR aLibFileName, UINT32 uCurrent, UINT32 * lpLastVer ) PURE;

		// it's first
		// if FAILED, call OnTreated 
		STDMETHOD(OnTreatedEx)( IRXArchive * arch, SGID40 uSigId, TEFC& rHowTreat, TRESULT trTreat, SIZE_T uTreatCount ) PURE;
	};

	// {1B23B5B8-DC1F-4D9B-84F1-428EF620C72D}
	RE_DEFINE_IID( IRXCallback, "{1B23B5B8-DC1F-4D9B-84F1-428EF620C72D}",
		0x1b23b5b8, 0xdc1f, 0x4d9b, 0x84, 0xf1, 0x42, 0x8e, 0xf6, 0x20, 0xc7, 0x2d);

	//////////////////////////////////////////////////////////////////////////

	class IRXCallback2 : public IUnknown
	{
	public:
		RE_DECLARE_IID;
		STDMETHOD_(VOID, BeforeAlarm)( IRXArchive * arch, LPDETECTR lpDetectResult ) PURE;
	};

	// {2E375592-E3BA-4986-9246-72ABB2E43625}
	RE_DEFINE_IID(IRXCallback2, "{2E375592-E3BA-4986-9246-72ABB2E43625}",
		0x2e375592, 0xe3ba, 0x4986, 0x92, 0x46, 0x72, 0xab, 0xb2, 0xe4, 0x36, 0x25);



	class IRXCallbackX : public IRXCallback
	{
	public:
		RE_DECLARE_IID;
		STDMETHOD_(TEFC, OnAlarm)( IRXArchive * arch, IExtDetectionResult * result ) PURE;
		STDMETHOD_(TEFC, OnTreated)( IRXArchive * arch, IExtDetectionResult * result, TRESULT trTreat ) PURE;
	};

	// {BA70DD5D-D617-4B3D-830C-A725D1179FDA}
	RE_DEFINE_IID(IRXCallbackX, "{BA70DD5D-D617-4B3D-830C-A725D1179FDA}",
		0xba70dd5d, 0xd617, 0x4b3d, 0x83, 0xc, 0xa7, 0x25, 0xd1, 0x17, 0x9f, 0xda);



	//////////////////////////////////////////////////////////////////////////////////////////////

	class IRxFrontEnd : public IUnknown
	{
	public:
		
		RE_DECLARE_IID;

		STDMETHOD(Init)( LPCSTR aLibPath, IRXParameters* lpParam ) PURE;

		// if >, see FILE_LEAVE_CASE
		STDMETHOD(Process)( IUnknown * lpStream, IRXCallback * lpCallback ) PURE;

		STDMETHOD_(VOID, Clear)() PURE;

		// 传入病毒库对象opLibLoader
		STDMETHOD(Init2)( IUnknown* opLibLoader, IRXParameters* lpParam ) PURE;
	};

	// {98764591-5B98-4367-8647-5DA83BDB6549}
	RE_DEFINE_IID( IRxFrontEnd, "{98764591-5B98-4367-8647-5DA83BDB6549}",
		0x98764591, 0x5b98, 0x4367, 0x86, 0x47, 0x5d, 0xa8, 0x3b, 0xdb, 0x65, 0x49);

	//////////////////////////////////////////////////////////////////////////////////////////////

	static LPCSTR	PARAM_PROCESS_COMPOUND	= "scan-compound";				// input int
	static LPCSTR	PARAM_MAX_COMPOUND_SIZE	= "max-compound-size";			// input int
	static LPCSTR	PARAM_MAX_NEST_DEPTH	= "max-nest-depth";				// input int
	static LPCSTR	PARAM_MAX_RETREAT_COUNT	= "max-treat-times";			// input int
	static LPCSTR	PARAM_PRECISE_FORMATS	= "precise-format";				// input int
	static LPCSTR	PARAM_NORMAL_FORMAT		= "normal";						// input int
	//// Format/Size Condition;
	//// 
	//static LPCSTR	PARAM_FILTER_STAMP		= "filter/.stamp";				// change it after patameter updated
	//static LPCSTR	PARAM_FILTER_SIZE		= "filter/size";
	//static LPCSTR	PARAM_FILTER_FORMAT		= "filter/format";
	//static LPCSTR	PARAM_FILTER_CLASS		= "filter/class";
};

#if 0

class flat
{
	class IRXCallback : public IUnknown
	{
	public:

		RE_DECLARE_IID;

		STDMETHOD_(PROCOPT, GetProcOptions )( ) PURE;

		STDMETHOD_(EFC,EnterFile)( IRXStream * pstm, SIZE_T uNest ) PURE;
		STDMETHOD_(VOID,LeaveFile)( IRXStream * pstm, SIZE_T uNest, LEAVECASE uCase ) PURE;

		STDMETHOD_(VOID,EnterArchive)( IRXArchive * arch ) PURE;
		STDMETHOD_(VOID,LeaveArchive)( IRXArchive * arch, TRESULT trRebuild ) PURE;

		STDMETHOD_(TEFC, OnAlarm)( IRXArchive * arch, SGID40 uSigId ) PURE;
		STDMETHOD_(TEFC, OnTreated)( IRXArchive * arch, SGID40 uSigId, TRESULT trTreat ) PURE;

		// 询问压缩包密码等功能
		STDMETHOD(OnComplexCallback)( xv::Value& inVar, xv::Value& outVar ) PURE;

		STDMETHOD(QueryLibFileVersion)( LPCSTR aLibFileName, UINT32 uCurrent, UINT32 * lpLastVer ) PURE;

		// it's first
		// if FAILED, call OnTreated 
		STDMETHOD(OnTreatedEx)( IRXArchive * arch, SGID40 uSigId, TEFC& rHowTreat, TRESULT trTreat, SIZE_T uTreatCount ) PURE;
	};
};

#endif

LPCSTR WINAPI TreatMethodName( _13::TEFC tm );
_13::TEFC WINAPI TreatMethodId( LPCSTR tm );


#endif	// #ifndef _RE_EXPORT_INTERFACE_ENGINE_PROXY_H_
