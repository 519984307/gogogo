#ifndef __RE_IENGINE_2010__
#define __RE_IENGINE_2010__

enum VirusIdSpec
{
	// virus id for none virus:
	VID_NONE_VIRUS = 0,		// 没有病毒
	VID_JUDGE_COMPOUND = 1,	// 查出复合文档格式（例如：SFX、PEPacker）
	VID_UNEXPECTED = (DWORD)-1,	// 引擎扫描异常
	// bit mask for virus:
	//VID_MASK_CLEANABLE = 1<<30,	// 可清除（09PE记录）	//统一病毒处理状态，使用VID_MASK_DELETABLE
	VID_MASK_DELETABLE = 1<<30,	// 可删除（08记录）
	VID_MASK_UNKNOWN = 1<<31,	// 未知病毒
	VID_MASK_VAILD_ID = 0x1FFFFFFF,	// 有效病毒ID位模
};

//
// 云扫描返回是木马,木马引擎返回的固定的病毒ID
//

#define VID_FIXED_TROJANID_FROM_CLOUD	299999999		// 3亿减1

//
//	所有MP记录的区间，使用了200万
//

enum { VID_MPE_BASE = 167000000, VID_MPE_MAX = 189000000 };

//////////////////////////////////////////////////////////////////////////

enum VIRUSTYPE
{
	VIRUSTYPE_UNKNOWN=-1,		//未知类型
	VIRUSTYPE_DOSCOM=0,			//dos下的com病毒
	VIRUSTYPE_DOSEXE,			//dos下的exe病毒
	VIRUSTYPE_WINPE,			//windows下的pe病毒
	VIRUSTYPE_WINNE,			//windows下的ne病毒
	VIRUSTYPE_MEM,				//内存病毒
	VIRUSTYPE_MACRO,			//宏病毒
	VIRUSTYPE_SCRIPT,			//脚本病毒
	VIRUSTYPE_BOOT,				//引导区病毒
	VIRUSTYPE_WINLE,			//windows下的le病毒
	VIRUSTYPE_NORMAL,			//普通型病毒
	VIRUSTYPE_ELF,				//unix下的elf文件
	VIRUSTYPE_WINNET,			//windows下的.net病毒
};

//////////////////////////////////////////////////////////////////////////

enum KILLRESULT
{
	// report @ end scan type:
	KR_S_OK = 0,			// (only used by proxy)
	// report @ kill virus/end scan type:
	KR_S_KILL,				//杀毒成功	
	KR_S_DELETE,			//文件被删除
	KR_S_ROLLBACK,			//回退到父对象 (only used by proxy)
	KR_S_IGNORE,			//用户忽略 (only used by proxy)
	//
	KR_E_FAIL = (HRESULT)0x80000000L,
	// report @ kill virus/end scan type:
	KR_E_REOPEN,			//写方式打开失败 (only used by proxy)
	KR_E_KILL,				//杀毒失败	
	KR_E_DELETE,			//文件删除失败
	KR_E_MAX_COUNT,			//超过杀毒最大次数 (only used by proxy)
	KR_E_ROLLBACK,			//回退到父对象失败 (only used by proxy)
	// report @ end scan type (if rollback, report @ kill virus):
	KR_E_POST_TREAT,		//杀毒后处理失败 (only used by proxy)
	// report @ kill virus/end scan type:
	KR_E_COMPOUND_UPDATE,	//复合文档回写失败 (only used by proxy)
};

//////////////////////////////////////////////////////////////////////////

class IREEngine : public IUnknown
{
public:
	STDMETHOD_(DWORD, Scan)(IUnknown *punkArchive) = 0;
	STDMETHOD_(DWORD, Kill)(IUnknown *punkArchive = 0) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREEngine, "{EFA66224-C235-47be-AD36-AF2F270FA55B}",
0xefa66224, 0xc235, 0x47be, 0xad, 0x36, 0xaf, 0x2f, 0x27, 0xf, 0xa5, 0x5b);

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

class IREUnpackCallback : public IUnknown
{
public:
	// return S_OK, continue
	// return E_FAIL, stop scan
	STDMETHOD( OnFileUnpacked )(
		DWORD dwDeep,
		const char * lpcPackerName, 
		IREArchive * pOrigianlArchive,
		IUnknown * pUnpackedObj
		) = 0;
	STDMETHOD( OnPackerDetected )(
		DWORD dwDeep,
		const char * lpcPackerName, 
		IREArchive * pOrigianlArchive ) = 0;
public:
	RE_DECLARE_IID;
};
// {4C5C387C-AEA1-438c-9684-6B18D7D88984}
RE_DEFINE_IID( IREUnpackCallback, "{4C5C387C-AEA1-438c-9684-6B18D7D88984}", 
0x4c5c387c, 0xaea1, 0x438c, 0x96, 0x84, 0x6b, 0x18, 0xd7, 0xd8, 0x89, 0x84);

// {724AC627-96FE-4892-A8F5-434FE2350F6E}
DEFINE_GUID( PID_ROT_UnpackCallback, 0x724ac627, 0x96fe, 0x4892, 0xa8, 0xf5, 0x43, 0x4f, 0xe2, 0x35, 0xf, 0x6e);


//////////////////////////////////////////////////////////////////////////


//已知宏扫描引擎
// {6E523558-A79E-4439-9F4A-119E1EC21329}
DEFINE_GUID(CLSID_CREScanMacro, 
0x6e523558, 0xa79e, 0x4439, 0x9f, 0x4a, 0x11, 0x9e, 0x1e, 0xc2, 0x13, 0x29);

//未知宏病毒信息
enum UNK_MACRO_INFO
{
	UNMACRO_WRITETEMPLATE		= 0x1,		//改写模板动作
	UNMACRO_WRITEBINFILE		= 0x2,		//有写大的二进制文件的动作	通常写木马
	UNMACRO_DELFILES			= 0x4,		//有删除文件动作	*,?
	UNMACRO_EXEFILES			= 0x8,		//有写可以运行的文件动作	vbs,bat,com,exe,scr,js,reg
	UNMACRO_RUNDANGERDOS		= 0x10,		//有执行危险dos命令的动作	del,format,deltree
	UNMACRO_COPYMACRO			= 0x20,		//有拷贝宏脚本的动作
	UNMACRO_OTHERDANGER			= 0x40,		//有其他危险的病毒常用的宏脚本操作	笼统的概括其他类型
	UNMACRO_RESERVERFORMACRO	= 0x80,		//为macro未知病毒保留
};
//未知宏扫描引擎
// {66B38C1D-84B9-4c5e-B355-231D9785E73C}
DEFINE_GUID(CLSID_CREScanUnMacro, 
0x66b38c1d, 0x84b9, 0x4c5e, 0xb3, 0x55, 0x23, 0x1d, 0x97, 0x85, 0xe7, 0x3c);


//////////////////////////////////////////////////////////////////////////

//已知脚本扫描引擎
// {72F60076-1364-4139-88E2-AC4A960E7A54}
DEFINE_GUID(CLSID_CREScriptEngine, 
0x72f60076, 0x1364, 0x4139, 0x88, 0xe2, 0xac, 0x4a, 0x96, 0xe, 0x7a, 0x54);

// {BF3D810B-EA65-4c0d-BE21-07C515140E21}
DEFINE_GUID(CLSID_CREMPSEngine4X, 
			0xbf3d810b, 0xea65, 0x4c0d, 0xbe, 0x21, 0x7, 0xc5, 0x15, 0x14, 0xe, 0x21);

// {2B8277E4-3904-4dac-94F4-FA471385689B}
DEFINE_GUID(CLSID_CREMPSEngine4PE, 
			0x2b8277e4, 0x3904, 0x4dac, 0x94, 0xf4, 0xfa, 0x47, 0x13, 0x85, 0x68, 0x9b);

//未知脚本病毒信息
enum UNK_SCRIPT_INFO
{
	UNSCRIPT_MAIL			= 0x1,		//发送邮件操作
	UNSCRIPT_FILESART 		= 0x2,		//写文件，并设置启动项
	UNSCRIPT_FILEREGENC		= 0x4,		//写文件，写注册表，加解密串
	UNSCRIPT_FILEWRITEIRC	= 0x8,		//写IRC文件
	UNSCRIPT_REGKEY			= 0x10,		//修改注册表关键项目
	UNSCRIPT_FILEWRITEBAT	= 0x20,		//写批处理文件
	UNSCRIPT_FILEEXECUTE	= 0x40,		//写文件，执行命令execute，解密串 
	UNSCRIPT_STRINGENC		= 0x80,		//执行一个特长的字符串，并解密（误报比较多正在解决）
	UNSCRIPT_FILEOPCHR		= 0x100,	//写文件，并有大量的CHR编码操作
	UNSCRIPT_EXESTART		= 0x200,	//启动程序，发送消息，写注册表
	UNSCRIPT_OPFLODER		= 0x400,	//遍历目录，删除文件
	UNSCRIPT_SHELLDOS		= 0x800,	//循环执行一个dos命令
	UNSCRIPT_SYSCALL		= 0x1000,	//打开系统管理对象，操作系统的东西，例如进程等
	UNSCRIPT_CALLOUTLOOK	= 0x2000,	//打开outlook程序，发送消息
	UNSCRIPT_SHELLDANGCMD	= 0x4000,	//执行破坏命令
	UNSCRIPT_CREATESHORTSTR	= 0x8000,	//创建快捷方式
	UNSCRIPT_CHGWININI		= 0x10000,	//修改配置文件win.ini
	UNSCRIP_USEAPPLET		= 0x20000,
};
//未知脚本扫描引擎
// {DB33AE6E-401A-4c4e-AE19-3F6071BBB6AA}
DEFINE_GUID(CLSID_CREUnScriptEngine, 
0xdb33ae6e, 0x401a, 0x4c4e, 0xae, 0x19, 0x3f, 0x60, 0x71, 0xbb, 0xb6, 0xaa);


//////////////////////////////////////////////////////////////////////////


// Normal文件扫描引擎
DEFINE_GUID(CLSID_CRENormalEngine, 
// {E766D8A9-C62B-4b43-B0DD-F5EEDFCCB6CA}
0xe766d8a9, 0xc62b, 0x4b43, 0xb0, 0xdd, 0xf5, 0xee, 0xdf, 0xcc, 0xb6, 0xca);

// PE文件扫描主控引擎
// {07E29A24-51FB-4bb6-A5E2-5E29E71F512A}
DEFINE_GUID(CLSID_CREPEEngine, 
0x7e29a24, 0x51fb, 0x4bb6, 0xa5, 0xe2, 0x5e, 0x29, 0xe7, 0x1f, 0x51, 0x2a);

// PE文件最终扫描一次引擎
// {E03D96B7-A55D-463b-B648-2E4BD8FECF4A}
DEFINE_GUID(CLSID_CREPEOnceEngine, 
0xe03d96b7, 0xa55d, 0x463b, 0xb6, 0x48, 0x2e, 0x4b, 0xd8, 0xfe, 0xcf, 0x4a);

// 08 已知PE文件扫描引擎
// {51DE2620-B5C7-4823-8883-4F8782ABB4FD}
DEFINE_GUID(CLSID_CREPEEngine_08, 
0x51de2620, 0xb5c7, 0x4823, 0x88, 0x83, 0x4f, 0x87, 0x82, 0xab, 0xb4, 0xfd);

//未知PE病毒信息
enum UNK_PE_INFO
{
	UNPE_WIN95LOADEREXPLOIT	= 0x1,	//利用Win95装载器漏洞
	UNPE_WIN9XRING0EXPLOIT 	= 0x2,	//利用Win9x漏洞得到系统级权限
	UNPE_WIN9XABNORMAPICALL	= 0x4,	//Win9x型病毒，采用非法手段获得API调用，调用文件相关函数，感染文件
	UNPE_WIN32ABNORMAPICALL	= 0x8,	//Win32型病毒，采用非法手段获得API调用，调用文件相关函数，感染文件
	UNPE_WINNTABNORMSYSCALL	= 0x10,	//利用NT Service Call直接调用系统内核代码
	UNPE_WIN32BINDER		= 0x20,	//释放文件并调用执行
};
// PE Unknown engine
// {5812BC51-14E7-4c1f-8077-B34CFAE7E22F}
DEFINE_GUID(CLSID_CREPEUnkEngine_07, 
0x5812bc51, 0x14e7, 0x4c1f, 0x80, 0x77, 0xb3, 0x4c, 0xfa, 0xe7, 0xe2, 0x2f);


// 已知MZ文件扫描引擎
DEFINE_GUID(CLSID_CREMZEngine, 
// {8D903B3E-E14A-4e56-B14C-2ECB22D2ABC8}
0x8d903b3e, 0xe14a, 0x4e56, 0xb1, 0x4c, 0x2e, 0xcb, 0x22, 0xd2, 0xab, 0xc8);

// 已知COM文件扫描引擎
DEFINE_GUID(CLSID_CRECOMEngine, 
// {542D7EE1-6430-492d-804A-9EA8B98837ED}
0x542d7ee1, 0x6430, 0x492d, 0x80, 0x4a, 0x9e, 0xa8, 0xb9, 0x88, 0x37, 0xed);

//未知DOS病毒信息
enum UNK_DOS_INFO
{
	UNDOS_COM	= 0x1,	//感染DOS可执行文件的COM病毒
	UNDOS_EXE	= 0x2,	//感染DOS可执行文件的EXE病毒
	UNDOS_MEM	= 0x4,	//DOS内存驻留型病毒
};
// DOS Unknown engine
// {DE41355B-D972-4ec1-8979-8FCA7A181DE8}
DEFINE_GUID(CLSID_CREDOSUnkEngine_07, 
0xde41355b, 0xd972, 0x4ec1, 0x89, 0x79, 0x8f, 0xca, 0x7a, 0x18, 0x1d, 0xe8);


// 已知Boot映象文件扫描引擎
DEFINE_GUID(CLSID_CREBootImgEngine, 
// {B15A4711-C3E2-4369-8D97-0070FDFD31F7}
0xb15a4711, 0xc3e2, 0x4369, 0x8d, 0x97, 0x0, 0x70, 0xfd, 0xfd, 0x31, 0xf7);

//未知引导病毒信息
enum UNK_BOOT_INFO
{
	UNBOOT_MBR		= 0x1,	//感染硬盘主引导的病毒
	UNBOOT_HBOOT	= 0x2,	//感染硬盘分区引导的病毒
	UNBOOT_FLOPPY	= 0x4,	//感染软盘引导区的病毒
};
// Boot Unknown engine
// {B01543A1-6E3E-4354-A0FB-69190312FD5C}
DEFINE_GUID(CLSID_CREBootUnkEngine_07, 
0xb01543a1, 0x6e3e, 0x4354, 0xa0, 0xfb, 0x69, 0x19, 0x3, 0x12, 0xfd, 0x5c);


//////////////////////////////////////////////////////////////////////////


// scanelf
// {BBCB5F25-A9C3-488c-9D67-010FF9909FE9}
DEFINE_GUID(CLSID_CREElfEngine, 
0xbbcb5f25, 0xa9c3, 0x488c, 0x9d, 0x67, 0x1, 0xf, 0xf9, 0x90, 0x9f, 0xe9);


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////


class IREAsyncEngine : public IUnknown
{
public:
	STDMETHOD( Initialize )() PURE;

	//
	//	if returned S_OK, Scan has been Completed immediately, engine returned virus id to [virusid].
	//
	//	if returned E_PENDING, Scan is not Completed, but post successfully.
	//
	//	else returned E_XXX, Failed
	//

	STDMETHOD( PostToScan )( IUnknown * pstm,OUT DWORD& virusid) PURE;
	STDMETHOD( Scan )( IUnknown * pstm,OUT DWORD& virusid) PURE;

	STDMETHOD( UnInit )() PURE;
	
public:
	RE_DECLARE_IID;
};
// {00EC7645-1B42-4af1-867C-34F8E84D4D7B}
RE_DEFINE_IID( IREAsyncEngine, "{00EC7645-1B42-4af1-867C-34F8E84D4D7B}",
			  0xec7645, 0x1b42, 0x4af1, 0x86, 0x7c, 0x34, 0xf8, 0xe8, 0x4d, 0x4d, 0x7b);
class EngineResult;
class EngineResult2;
class IRSClientCallBackInfo;
class IAsyncScanManager : public IUnknown
{
public:
	STDMETHOD_(bool, Initialize )() PURE;

	STDMETHOD_(size_t, AllocContext )( uint64_t crc,void *pdata) PURE;
	STDMETHOD_(bool, SetScanComplete )( uint64_t crc, size_t engineid) PURE;
	STDMETHOD_(bool, SetScanResult )( uint64_t crc, size_t engineid, EngineResult *pScanret) PURE;
	STDMETHOD_(bool, SetScanResult )( uint64_t crc, size_t engineid, EngineResult2 *pScanret) PURE;
	STDMETHOD_(bool, SetVirusInfo )(uint64_t crc,IRSClientCallBackInfo* pInfo)PURE;
	STDMETHOD_(bool, Finish )() PURE;
	STDMETHOD_(bool, Clear )() PURE;
	STDMETHOD_(size_t, GetCount )() PURE;

public:
	RE_DECLARE_IID;
};
// {43F13C3E-C105-40d3-A218-DC9F15A9B2A8}
RE_DEFINE_IID(IAsyncScanManager, "{43F13C3E-C105-40d3-A218-DC9F15A9B2A8}",
			0x43f13c3e, 0xc105, 0x40d3, 0xa2, 0x18, 0xdc, 0x9f, 0x15, 0xa9, 0xb2, 0xa8);

// {2F6AFF8E-34A5-4405-A0BE-7AB28081457F}
DEFINE_GUID( PID_ROT_RssCloudEngine, 
			0x2f6aff8e, 0x34a5, 0x4405, 0xa0, 0xbe, 0x7a, 0xb2, 0x80, 0x81, 0x45, 0x7f);

// {2EF3B504-8F43-4da0-8F42-7357B45429A5}
DEFINE_GUID(PID_ROT_Ansync_Manager, 
			0x2ef3b504, 0x8f43, 0x4da0, 0x8f, 0x42, 0x73, 0x57, 0xb4, 0x54, 0x29, 0xa5);

//////////////////////////////////////////////////////////////////////////

struct EngineType
{
	enum
	{
		ENGINE_ORTHODOX = 0,
		ENGINE_CLOUD,
		ENGINE_COUNT,
	};
	enum {
		ALL_ENGINE_MASK = ((1<<ENGINE_ORTHODOX)|(1<<ENGINE_CLOUD)),
	};
};

struct AsyncEngineMode
{
	enum
	{
		ModeNone		= 0,
		ModeOnline		= 1,
		ModeOnlineSync	= 2,
		//ModeOffline		= 3,
	};
};

//////////////////////////////////////////////////////////////////////////
#endif	// #ifndef _RE_EXPORT_INTERFACE_ENGINE_H_

