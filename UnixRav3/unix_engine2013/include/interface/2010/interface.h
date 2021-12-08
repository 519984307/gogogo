#ifndef _RE_EXPORT_INTERFACE_H_
#define _RE_EXPORT_INTERFACE_H_

//////////////////////////////////////////////////////////////////////////
// std com:

#include "../rxruntim.h"

//HRESULT WINAPI RECreateInstance( REFCLSID clsid, IUnknown* outer, REFIID riid, void ** ppv, IUnknown* prot );

//////////////////////////////////////////////////////////////////////////
// must be running objects:

// common platform components:
DEFINE_GUID(PID_ROT_WindowsRegistry,	0xa2ef8f70, 0xe5d2, 0x4ac5, 0xaf, 0xf8, 0xb6, 0x22, 0x99, 0x50, 0xa5, 0xb9);
// common engine component config:
DEFINE_GUID(PID_ROT_ComponentConfig,	0x80c5bade, 0x1c55, 0x4e3e, 0xa7, 0x77, 0x5d, 0xb2, 0xd2, 0x59, 0xfb, 0x2a);

DEFINE_GUID(PID_ROT_UnpackRoutineMap,	0x04adb887, 0x5dcb, 0x44a4, 0x92, 0x5b, 0x07, 0x51, 0x61, 0x6a, 0xd5, 0xa0);
// common engine dependency:
DEFINE_GUID(PID_ROT_LibProperty,		0x3139b192, 0xb0fe, 0x448a, 0x88, 0x55, 0x80, 0x98, 0x14, 0xff, 0xc3, 0x61);
DEFINE_GUID(PID_ROT_IndexObjCfg,		0xb960b138, 0x9094, 0x46ed, 0xb5, 0x9d, 0x46, 0x83, 0x35, 0x6f, 0x07, 0xdc);
//////////////////////////////////////////////////////////////////////////
// engine proxy dependency:
DEFINE_GUID(PID_ROT_ProxyCallback,		0x2e340b5c, 0x0b3b, 0x4309, 0x89, 0xd7, 0x68, 0x03, 0xe1, 0xb7, 0x9a, 0x11);
//////////////////////////////////////////////////////////////////////////
// optional running objects:
DEFINE_GUID(PID_ROT_FileSystemVirtual,	0x7caae9c0, 0x1465, 0x4a40, 0xbc, 0x4a, 0xd6, 0xc4, 0x7c, 0x23, 0x06, 0x33);
//////////////////////////////////////////////////////////////////////////
// 病毒库配置文件的路径(VT_LPSTR)
DEFINE_GUID(PID_ROT_VirusDB_ConfigFile, 0x20728056, 0x904, 0x4ab3, 0x85, 0xb9, 0x9a, 0xe2, 0x5d, 0xbb, 0xf5, 0x63);
// 引擎模块路径
DEFINE_GUID(PID_ROT_EngineModulePath,	0x3339e509, 0x1d4e, 0x4df9, 0xbf, 0x84, 0x5f, 0x7a, 0x8, 0x95, 0xe6, 0xe5);
//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
//// Property:
//
//template<class T>
//class __tIREProperty : public IUnknown
//{
//public:
//	typedef	T	KeyType;
//public:
//	STDMETHOD(SetProperty)(const KeyType& rpid, const PROPVARIANT *pv) = 0;
//	STDMETHOD(GetProperty)(const KeyType& rpid, const PROPVARIANT **ppv) = 0;
//	STDMETHOD(DeleteProperty)(const KeyType& rpid) = 0;
//	STDMETHOD(Clear)() = 0;
//};
//
//
//class IREProperty : public	__tIREProperty<GUID>
//{
//public:
//	RE_DECLARE_IID;
//};
//RE_DEFINE_IID(IREProperty, "{DF343B0B-D142-4e3c-BE9C-02BBED29D27E}",
//0xdf343b0b, 0xd142, 0x4e3c, 0xbe, 0x9c, 0x2, 0xbb, 0xed, 0x29, 0xd2, 0x7e);
//
//
//class IREProperty2 : public	__tIREProperty<DWORD>
//{
//public:
//	RE_DECLARE_IID;
//};
//RE_DEFINE_IID(IREProperty2, "{203ADD4C-48C7-486d-B62A-F22644ED1E1D}",
//0x203add4c, 0x48c7, 0x486d, 0xb6, 0x2a, 0xf2, 0x26, 0x44, 0xed, 0x1e, 0x1d);
//

//////////////////////////////////////////////////////////////////////////
// Function Config:

/*
	C	: Compress
	E	: E-mail/mailbox
	U	: Unpacker
	D	: DosExecute
	W	: WinExecute
	S	: Script
	M	: Macro
	N	: NORMAL
	B	: Boot
	L	: Linux-ELF
	S	: SFX
	P	: Packer as Virus
*/

struct REConfig
{
	enum
	{
		CFG_OTHER_BASE		= 0x80,
		//查压缩文件
		SCAN_COMPRESS		= 'C',			// VT_UI4
		//查邮箱文件
		SCAN_MAILBOX		= 'E',			// VT_UI4
		//查邮件文件
		SCAN_EML			= SCAN_MAILBOX,	// VT_UI4
		//脱壳
		SCAN_UNPACKER		= 'U',			// VT_UI4
		//查杀自解压压缩包
		SCAN_SFX			= 'I',			// VT_UI4

		//查普通文件
		SCAN_NORMAL			= 'N',			// VT_UI4
		//查DOS可执行文件
		SCAN_DOSEXECUTE		= 'D',			// VT_UI4
		//查Windows可执行文件
		SCAN_WINEXECUTE		= 'W',			// VT_UI4
		//查脚本文件
		SCAN_SCRIPT			= 'S',			// VT_UI4
		//查宏文件
		SCAN_MACRO			= 'M',			// VT_UI4
		//查已知引导型病毒
		SCAN_BOOT			= 'B',			// VT_UI4
		// 报壳名
		SCAN_PACKERNAME_AS_VIRUS = 'P',		// VT_UI4
		// 查可疑文件
		SCAN_SUSPICIOUS		= '?',			// VT_UI4
		//////////////////////////////////////////////////////////////////////////
		//查杀 net 文件
		SCAN_NET			= 'O',			// VT_UI4
		//查杀elf 文件
		SCAN_ELF			= 'L',			// VT_UI4
		//查未知DOS可执行文件
		SCAN_UNDOSEXECUTE	= 'd',			// VT_UI4
		//查未知Windows文件
		SCAN_UNWINEXECUTE	= 'w',			// VT_UI4
		//查未知脚本文件
		SCAN_UNSCRIPT		= 's',			// VT_UI4
		//查未知宏文件
		SCAN_UNMACRO		= 'm',			// VT_UI4
		//查未知引导型病毒
		SCAN_UNBOOT			= 'b',			// VT_UI4

		// 查未知木马
 		SCAN_UNKNOWN_TROJAN	= CFG_OTHER_BASE+3,	// VT_UI4
		// 查分类病毒记录
 		SCAN_RECORD_CATALOG	= CFG_OTHER_BASE+4,	// VT_UI4
		//2011+ for 深度启发扫描
		SCAN_DEEP_SCAN		= CFG_OTHER_BASE+5,
		//////////////////////////////////////////////////////////////////////////
//
//// 2008 added:
//		// 最大复合文档嵌套深度
//		MAX_NEST_DEPTH		= 0x1000,	// VT_UI4
//		// 同一文件最大杀毒次数
//		MAX_KILL_COUNT		= 0x1001,	// VT_UI4
//// 2011 added,for file size control
//		MAX_ZIP_SIZE		= 0x1002,	//VT_UI4
//		MAX_SFX_SIZE		= 0x1003,	//VT_UI4
//		MAX_MAILBOX_SIZE	= 0x1004,	//VT_UI4
//		CURRENT_FILE_SIZE	= 0x7000,	//VT_UI4
//		//////////////////////////////////////////////////////////////////////////
//// 2008 added, for debugging:
//		// 枚举所有匹配的病毒记录
		DBG_SCAN_ENUM_RECORD	= 0x8000,	// VT_UI4
//
//// 2008 added, runtime dynamic:
		// query nvlib, engine increment scan use:
		NV_RECORD_VERSION	= 0xFFFFFFFF,	// VT_UI2
//		// 2011 add, feature record for monitor
//		SCAN_MONITOR_FR		= 0x9000,
//		// 2011 add, 
//		SPECIAL_VIRUS_ID	= 0x9001,
//		// 2012 add,
//		DEFER_TIME			= 0x9002,
//		//木马库资源优化模式
//		SCAN_RESOURCE_OPTIMIZE = 0x9003,
//		// 
//		DEBUG_MODE				= 0x9004,
//		//////////////////////////////////////////////////////////////////////////
//		// VT_UI4[设置是否进行引擎内部的后处理]
//		RAE_DO_DEF_POSTTRT		= 0xA002,
	};
};


class IREConfigQuery : public IUnknown
{
public:
	STDMETHOD_(bool, IsScanKnownType)(WORD type) = 0;
	STDMETHOD_(bool, IsScanUnknownType)(WORD type) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREConfigQuery, "{536C0765-354D-4862-8228-02B73D74C117}",
0x536c0765, 0x354d, 0x4862, 0x82, 0x28, 0x2, 0xb7, 0x3d, 0x74, 0xc1, 0x17);


//////////////////////////////////////////////////////////////////////////
// Archive:
//
//class IREStream;
//
//class IREArchive : public IUnknown
//{
//public:
//	STDMETHOD(Open)(IUnknown *punk) = 0;
//	STDMETHOD(Close)() = 0;
//	STDMETHOD(GetStreamObject)(IREStream **ppstm) = 0;
//	// ......
//public:
//	RE_DECLARE_IID;
//};
//RE_DEFINE_IID(IREArchive, "{F0DE6717-1B00-4b9e-8E45-0212E029457B}",
//0xf0de6717, 0x1b00, 0x4b9e, 0x8e, 0x45, 0x2, 0x12, 0xe0, 0x29, 0x45, 0x7b);

//////////////////////////////////////////////////////////////////////////
// Component:

// {084053C3-65BA-48f9-98A4-A164EDE5E819}
DEFINE_GUID(CLSID_CREObjectLoader, 
0x84053c3, 0x65ba, 0x48f9, 0x98, 0xa4, 0xa1, 0x64, 0xed, 0xe5, 0xe8, 0x19);

// {06FC0945-75B0-4297-B5F7-322881ABAE92}
DEFINE_GUID(CLSID_CREArchiveMap, 
0x6fc0945, 0x75b0, 0x4297, 0xb5, 0xf7, 0x32, 0x28, 0x81, 0xab, 0xae, 0x92);

// {B04A404E-640F-4eaf-B80F-5F8AC72A67CE}
DEFINE_GUID(CLSID_CREURoutineMap,
0xb04a404e, 0x640f, 0x4eaf, 0xb8, 0xf, 0x5f, 0x8a, 0xc7, 0x2a, 0x67, 0xce);

// {01ABA3DD-7F54-4f0d-8B2D-CA628000CF4A}
DEFINE_GUID(CLSID_CREEngineMap, 
0x1aba3dd, 0x7f54, 0x4f0d, 0x8b, 0x2d, 0xca, 0x62, 0x80, 0x0, 0xcf, 0x4a);

// {3325B440-A8D3-4d12-A7F1-F9486CC68363}
DEFINE_GUID(CLSID_CRETableIndexMap, 
			0x3325b440, 0xa8d3, 0x4d12, 0xa7, 0xf1, 0xf9, 0x48, 0x6c, 0xc6, 0x83, 0x63);


#include "ipearchive.h"
//////////////////////////////////////////////////////////////////////////
//

#ifndef DISABLE_REVM_2009
#include "ivm.h"
#endif

//////////////////////////////////////////////////////////////////////////
// debugging:

class IREDbgMemTrace : public IUnknown
{
public:
	enum
	{
		// internal
		PID_ScanFileName, // VT_LPSTR
		// base info
		PID_AllocSize, // VT_UI4
		PID_SourceFileName, // VT_LPSTR
		PID_SourceFileLine, // VT_UI4
		// overflow
		PID_Magic,			// VT_LPSTR
		PID_CookieErrorCode, // VT_UI4
		PID_CookieErrorData, // VT_UI1|VT_VECTOR
		// boundary for extend:
		PID_ExtendBegin,
	};
	enum
	{
		CookieError_Unknow = -1,
		CookieError_Overflow = 0,
		CookieError_Underflow = 1,
	};
public:
	STDMETHOD(SetItem)(void *addr, DWORD pid, const PROPVARIANT *pvar) = 0;
	STDMETHOD(GetItem)(void *addr, DWORD pid, OUT const PROPVARIANT **ppvar) = 0;
	STDMETHOD(DeleteItem)(void *addr) = 0;
	STDMETHOD_(BOOL, IsItemExist)(void *addr) = 0;
	STDMETHOD(NextItem)(void *addr, void **addr_next) = 0; // iterate
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREDbgMemTrace, "{E709AF99-9624-4efc-83EF-B9976CBB8BA0}",
0xe709af99, 0x9624, 0x4efc, 0x83, 0xef, 0xb9, 0x97, 0x6c, 0xbb, 0x8b, 0xa0);


class IREDbgMemTraceManager : public IUnknown
{
public:
	STDMETHOD_(IREDbgMemTrace*, GetMemTrace)(const char *log_name) = 0;
	STDMETHOD(SaveLog)(const char *log_name) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREDbgMemTraceManager, "{52560AD6-7769-41bd-854C-25376334A0F7}",
0x52560ad6, 0x7769, 0x41bd, 0x85, 0x4c, 0x25, 0x37, 0x63, 0x34, 0xa0, 0xf7);


// debug mem trace export:
extern "C" IREDbgMemTraceManager* WINAPI REGetDbgMemTraceMgr();
typedef IREDbgMemTraceManager* (WINAPI * PFNREGetDbgMemTraceMgr)();

// VT_LPSTR / VT_LPWSTR
DEFINE_GUID(PID_DBG_LastFile,	0xaa4209c0, 0x9c7a, 0x4311, 0xa4, 0xb9, 0x7b, 0xaf, 0x35, 0x5a, 0xb3, 0xf1);

// debug info export, thread relative:
extern "C" IREProperty* WINAPI REGetDbgInfo();
typedef IREProperty* (WINAPI * PFNREGetDbgInfo)();


//////////////////////////////////////////////////////////////////////////
//

/*#include "unix.libeng.h"*/


//////////////////////////////////////////////////////////////////////////


#endif // duplicate inclusion protection
