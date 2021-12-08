#ifndef __PUB_PEENG_H_10__
#define __PUB_PEENG_H_10__

/////////////////////////////////////////////////////////////////////////////////////////////////
// defined @2007/11/15
/////////////////////////////////////////////////////////////////////////////////////////////////

enum LOCATION_POINTER_TYPE
{
	POINTER_EOF = 0 ,  // -> 截至符
	POINTER_FILE ,     // -> 文件偏移
	POINTER_MEM ,      // -> 内存线性地址
	POINTER_VALUE ,    // -> 数值

	POINTER_INVALID = 0xFFFFFFFF,  // 无效
};

/////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagPELocation
{
	union
	{
		DWORD linear ; // 内存线性地址
		DWORD offset ; // 文件偏移
		DWORD data ;   // 数值数据
	};
	DWORD type ;
	/////////////////////////////
	static bool is_pelocation_valid( const struct tagPELocation *pstloc ) { return pstloc->type != POINTER_INVALID ; }
	static bool is_pelocation_value( const struct tagPELocation *pstloc ) { return pstloc->type == POINTER_VALUE ; }
	static bool is_pelocation_position( const struct tagPELocation *pstloc ) { return (pstloc->type == POINTER_FILE || pstloc->type == POINTER_MEM) ; }
} PELocation, *PPELocation ;


/////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct tagREVMRunStatus
{
	DWORD dwJmpTimes ;        // 跳转
	DWORD dwDecodeTimes ;     // 解密
	DWORD dwLoopTimes ;       // 循环  
	DWORD dwPageJmpTimes ;    // 内存块跳
	DWORD dwSdmTimes ;        // 自修改次数
	DWORD dwApiTimes ;        // API数
	DWORD dwExpTimes ;        // 异常数
} STREVMRunStatus, *PSTREVMRunStatus ;

typedef struct tagREVMRunStatusEx
{
	BYTE byIsVEInLoop ;   // 是否处于循环中
	BYTE byIsFinalState ; // 是否是最终状态
} STREVMRunStatusEx, *PSTREVMRunStatusEx ;


/////////////////////////////////////////////////////////////////////////////////////////////////


// class IREPEMethodParam : public IUnknown
// {
// public:
// 	STDMETHOD(Push)(PELocation item) = 0;
// 	STDMETHOD(Pop)(OUT PELocation& item) = 0;
// 	STDMETHOD(Get)(int idx, OUT PELocation& item) = 0;
// 	STDMETHOD_(DWORD, Size)() = 0;
// 	STDMETHOD(Clear)() = 0;
// public:
// 	RE_DECLARE_IID;
// };
// RE_DEFINE_IID(IREPEMethodParam, "{B1A29BCA-210C-4eeb-9FD9-4D7B8B1F1782}",
// 			  0xb1a29bca, 0x210c, 0x4eeb, 0x9f, 0xd9, 0x4d, 0x7b, 0x8b, 0x1f, 0x17, 0x82);
#define IREPEMethodParam	IUnknown


/////////////////////////////////////////////////////////////////////////////////////////////////


class IREPEStdMethod : public IUnknown
{
public:
	STDMETHOD(Invoke)(IUnknown *punkenv, IREPEMethodParam *pvreserved, OUT PELocation *presult) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREPEStdMethod, "{9A9B2485-119B-4dca-A3A1-997C09646D25}",
			  0x9a9b2485, 0x119b, 0x4dca, 0xa3, 0xa1, 0x99, 0x7c, 0x9, 0x64, 0x6d, 0x25);


// VT_UI1 | VT_VECTOR:
DEFINE_GUID(PID_MAKELIB_PEStdMethodExData,	0x31cfc5b2, 0x9f, 0x49e4, 0xb8, 0xc, 0x6b, 0x4d, 0x85, 0x1e, 0x71, 0x33);

class IREPEStdExDataParser : public IUnknown
{
public:
	// [in] punkXMLElement : IXMLDOMElement
	// [out] punkResult : IREProperty, use PID_MAKELIB_PEStdMethodExData
	STDMETHOD(Parse)(IUnknown* punkXMLDOMElement, OUT IUnknown* punkResult) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREPEStdExDataParser, "{39CD876B-150A-4420-AA50-35A8151C8C55}",
			0x39cd876b, 0x150a, 0x4420, 0xaa, 0x50, 0x35, 0xa8, 0x15, 0x1c, 0x8c, 0x55);


/////////////////////////////////////////////////////////////////////////////////////////////////


//cfg path///////////////////////////////////////////////////////////////////////////////////////
const WCHAR * const INIT_PEENG_DYN_CLSIDS_CFGDIR  = L"scanpe" ;
#if !defined(PLATFORM_TYPE_POSIX)
const WCHAR * const INIT_PEENG_DYN_CLSIDS_CFGPATH = L"scanpe\\scanexinit" ;
const WCHAR * const INIT_PEENG_COMPILER_LIBTYPE   = L"scanpe\\cmpllibdef" ;
#else
const WCHAR * const INIT_PEENG_DYN_CLSIDS_CFGPATH = L"scanpe/scanexinit" ;
const WCHAR * const INIT_PEENG_COMPILER_LIBTYPE   = L"scanpe/cmpllibdef" ;
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////

class IREPEDynNotify : public IUnknown
{
public:
	// 返回对应的动态记录索引
	STDMETHOD_(DWORD, Notify)(IUnknown* punk) = 0 ; 
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREPEDynNotify, "{F25AE8E1-CBC4-4f55-A758-97C682689BA4}",
			  0xf25ae8e1, 0xcbc4, 0x4f55, 0xa7, 0x58, 0x97, 0xc6, 0x82, 0x68, 0x9b, 0xa4);


class IREPEDynInit : public IUnknown
{
public:
	STDMETHOD(Init)(IUnknown* punk) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREPEDynInit, "{66559F1B-93DF-46ad-A88C-445F21BE1DF1}",
			  0x66559f1b, 0x93df, 0x46ad, 0xa8, 0x8c, 0x44, 0x5f, 0x21, 0xbe, 0x1d, 0xf1);


/////////////////////////////////////////////////////////////////////////////////////////////////
enum STATE_REG
{
	STATE_REG_INITIAL = 0 , // 起始状态
	///////////////////////////////////

	STATE_REG_JUMP ,
	STATE_REG_PAGEJUMP ,
	STATE_REG_LOOP_START ,
	STATE_REG_LOOP_END ,
	STATE_REG_DECODED ,
	STATE_REG_INDECODEMEM ,
	STATE_REG_GR_API ,
	STATE_REG_GR_EXCEPTION ,
	// add new state here...

	///////////////////////////////////
	STATE_REG_FINAL ,       // 最终状态

	///////////////////////////////////
	STATE_REG_INVALID = 0xFFFFFFFF ,
} ;

class IREPEDynHost : public IUnknown
{
public:


	enum { INVALID_DYNOBJ_ID = 0xFFFFFFFF } ; // 非法id

public:
	static DWORD VmRunStateToRegState( DWORD dwVmGoResult, DWORD dwVmRunState )
	{
		switch( dwVmGoResult )
		{
		case GR_OK:
			{
				switch( dwVmRunState )
				{
				case RUN_JUMP:
					return STATE_REG_JUMP ;
				case RUN_PAGEJUMP:
					return STATE_REG_PAGEJUMP ;
				case RUN_LOOP_START:
					return STATE_REG_LOOP_START ;
				case RUN_LOOP_END:
					return STATE_REG_LOOP_END ;
				case RUN_DECODED:
					return STATE_REG_DECODED ;
				case RUN_INDECODEMEM:
					return STATE_REG_INDECODEMEM ;
				/////////////////////////////
				// add new run state here...
				/////////////////////////////
				}
			}
			break ;
		case GR_API:
			return STATE_REG_GR_API ;
		case GR_EXCEPTION:
			return STATE_REG_GR_EXCEPTION ;
			/////////////////////////////
			// add new go result here...
			/////////////////////////////
		}

		return DWORD(STATE_REG_INVALID);
	}

public:
	// 挂接动态对象，返回id
	STDMETHOD_(DWORD, Register)(DWORD state, IREPEDynNotify* pi) = 0; // return id
	// 注销动态对象
	STDMETHOD(Revoke)(DWORD state, DWORD id) = 0;

public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREPEDynHost, "{ECACC9AD-9AF6-4aef-BB25-C7D6447A582A}",
			  0xecacc9ad, 0x9af6, 0x4aef, 0xbb, 0x25, 0xc7, 0xd6, 0x44, 0x7a, 0x58, 0x2a);


/////////////////////////////////////////////////////////////////////////////////////////////////

class IREEngineContext : public IUnknown
{
public:
	// 获得当前引擎查毒的原始ARCHIVE
	STDMETHOD(GetArchive)(IREArchive ** pparc ) PURE;
	// 标准方法的扩展数据指针和大小
	STDMETHOD_(const BYTE*,GetStdExtLibData)(OUT WORD &wSize) = 0 ;  // return NULL if failed
	// 获取前面执行方法的地址或数据结果(-1表示上一个方法):
	STDMETHOD(GetLocationData)( int index, PELocation &loc ) = 0;
	RE_DECLARE_IID;
};

// {3A6D85B7-785F-4e68-8D26-D40712F415CB}
RE_DEFINE_IID( IREEngineContext, "{3A6D85B7-785F-4e68-8D26-D40712F415CB}",
			0x3a6d85b7, 0x785f, 0x4e68, 0x8d, 0x26, 0xd4, 0x7, 0x12, 0xf4, 0x15, 0xcb);


class IREPEEngineContext : public IREEngineContext
{
public:
	// 
	STDMETHOD(CreateVritualStream)( IREStream** pstm, LPCSTR caName ) PURE;
	// 
	STDMETHOD(PushAdditionalStream)( IREStream* pstm, BOOL bIntactPE ) PURE;
	// 获得最后的那个完整脱壳对象
	STDMETHOD(GetLastIntactStream)( IREStream** pstm ) PURE;
	// pe文件对象
	STDMETHOD(GetCurrentPEArc)(IREPEArchive **pppearc)  PURE;
	// 虚拟机对象
	STDMETHOD(GetVMObj)(IREVM **ppvmobj) = 0;
	// 虚拟机虚拟执行状态
	STDMETHOD(GetVMRunState)(OUT STREVMRunStatus *pstate) PURE;
	// 虚拟机上一次的运行结果
	STDMETHOD_(DWORD,GetVMLastGoResult)(void) = 0;
	// buffer长度同virusinfo最大长度64byte
	STDMETHOD(GetPackerName)(OUT char *szBuffer) = 0 ;
	// 虚拟机虚拟执行状态(扩展)
	STDMETHOD(GetVMRunStateEx)(OUT STREVMRunStatusEx *pstateex) = 0;
	//
	STDMETHOD_(SIZE_T,GetNestScanDepth)() PURE;
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREPEEngineContext, "{36EA966F-E05A-419a-A429-54B8D31455DB}",
			  0x36ea966f, 0xe05a, 0x419a, 0xa4, 0x29, 0x54, 0xb8, 0xd3, 0x14, 0x55, 0xdb);

//typedef IREPEEngineContext IREPEEngineContext;

//////////////////////////////////////////////////////////////////////////////////////////////////

//class IPEEngNotify : public IUnknown
//{
//public:
//	// 通知DUMP完成:
//	STDMETHOD(NotifyAlreadyDump)(void) = 0 ;
//
//public:
//	RE_DECLARE_IID;
//};
//
//RE_DEFINE_IID(IPEEngNotify, "{9950CF63-8FC0-4d59-BB27-48070818C649}",
//			  0x9950cf63, 0x8fc0, 0x4d59, 0xbb, 0x27, 0x48, 0x7, 0x8, 0x18, 0xc6, 0x49);

//////////////////////////////////////////////////////////////////////////////////////////////////


// {232CEE8A-FFCD-4be4-AB2C-E1DCA7ABB4A4}
DEFINE_GUID(CLSID_PESTD_DeleteFile,
0x232cee8a, 0xffcd, 0x4be4, 0xab, 0x2c, 0xe1, 0xdc, 0xa7, 0xab, 0xb4, 0xa4);


//////////////////////////////////////////////////////////////////////////

#endif // __PUB_PEENG_H__
