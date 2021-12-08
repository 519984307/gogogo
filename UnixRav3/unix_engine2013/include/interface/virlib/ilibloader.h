#ifndef __ILIBLOADER_H_10_11__
#define __ILIBLOADER_H_10_11__

#include "./irstable.h"

typedef enum _LIBTYPE
{
	//文件库
	LIBTYPE_COM = 0,
	LIBTYPE_MZ,
	LIBTYPE_PE,
	LIBTYPE_NE,
	LIBTYPE_MEM,
	LIBTYPE_MACRO,
	LIBTYPE_SCRIPT,
	LIBTYPE_LE,
	LIBTYPE_NET,
	LIBTYPE_NORMAL = 9,
	LIBTYPE_VBS,
	LIBTYPE_JS,
	LIBTYPE_BAT,
	LIBTYPE_HTM,
	LIBTYPE_PERL,
	LIBTYPE_SHELL,
	LIBTYPE_ELF,

	//引导区库

	LIBTYPE_BOOT,
	LIBTYPE_PACKER,			// 暂时不删(借用)
	LIBTYPE_CAF,			// 编译器库(caf.def)

	LIBTYPE_FBOOT = 0x15,	//floppy disk
	LIBTYPE_MBOOT,			//mbr
	LIBTYPE_HBOOT,			//hard disk

	//后处理库
	LIBTYPE_PTDICT,
	LIBTYPE_PTRECORD,
	LIBTYPE_PTMAP,

	//病毒ID
	LIBTYPE_VIRUSID = 0x1E,

	//病毒基本信息库
	LIBTYPE_INFO = 0x1F,

	//良民库
	LIBTYPE_NVLIST=0x20,

	//白名单
	LIBTYPE_WLLIST_95_CN=0x21,
	LIBTYPE_WLLIST_97_CN,
	LIBTYPE_WLLIST_98_CN,
	LIBTYPE_WLLIST_98SE_CN,
	LIBTYPE_WLLIST_ME_CN,
	LIBTYPE_WLLIST_NT4WS_CN,
	LIBTYPE_WLLIST_NT4SVR_CN,
	LIBTYPE_WLLIST_2KPRO_CN,
	LIBTYPE_WLLIST_2KSVR_CN,
	LIBTYPE_WLLIST_2KAS_CN,
	LIBTYPE_WLLIST_XPPRO_CN,
	LIBTYPE_WLLIST_XPHE_CN,
	LIBTYPE_WLLIST_SVR2003_CN,
	LIBTYPE_WLLIST_VISTA_CN,

	LIBTYPE_WLLIST_95_EN=0x31,
	LIBTYPE_WLLIST_97_EN,
	LIBTYPE_WLLIST_98_EN,
	LIBTYPE_WLLIST_98SE_EN,
	LIBTYPE_WLLIST_ME_EN,
	LIBTYPE_WLLIST_NT4WS_EN,
	LIBTYPE_WLLIST_NT4SVR_EN,
	LIBTYPE_WLLIST_2KPRO_EN,
	LIBTYPE_WLLIST_2KSVR_EN,
	LIBTYPE_WLLIST_2KAS_EN,
	LIBTYPE_WLLIST_XPPRO_EN,
	LIBTYPE_WLLIST_XPHE_EN,
	LIBTYPE_WLLIST_SVR2003_EN,
	LIBTYPE_WLLIST_VISTA_EN,

	//漏洞库
	LIBTYPE_BULLETIN = 0x64,
	LIBTYPE_SYSLEAK,
	LIBTYPE_LEAKSCAN,
	LIBTYPE_LEAKINFO,
	LIBTYPE_EXPLOITINFO,
	LIBTYPE_DESCRIPT,
	LIBTYPE_DOWNURL,

	///////////////////////////
	//09 PE lib
	LIBTYPE_PEFC = 0x70,   // pe fc lib
	LIBTYPE_PENORMAL,      // pe normal lib

	LIBTYPE_PEDYNAMIC_NORMAL,
	LIBTYPE_PEDYNAMIC,
	LIBTYPE_PEDYNAMIC_HOOK,
	LIBTYPE_PEDYNAMIC_ONCE,	
	LIBTYPE_SFX,
	LIBTYPE_SUSPICIOUS,
	LIBTYPE_CLSIDMAP,

	LIBTYPE_TROJANLIB_EXE_CRC,
	LIBTYPE_TROJANLIB_DLL_CRC,
	LIBTYPE_TROJANLIB_SYS_CRC,
	LIBTYPE_TROJANLIB_EXE_NOCRC,
	LIBTYPE_TROJANLIB_DLL_NOCRC,
	LIBTYPE_TROJANLIB_SYS_NOCRC,

	LIBTYPE_TROJANLIB2_EXE,
	LIBTYPE_TROJANLIB2_DLL,
	LIBTYPE_TROJANLIB2_SYS,

	//白名单总库
	LIBTYPE_WLLIST = 0xfd,
	//漏洞总库
	LIBTYPE_BDENGINE = 0xfe,
	LIBTYPE_VMDLL=0x7000,
	LIBTYPE_VMSTRING,
	LIBTYPE_VMAPID,
	//启发扫描库
	LIBTYPE_DEEPAPI=0x7100,
	LIBTYPE_DEEPSTR,
	LIBTYPE_DEEPSTRMAP,
	//
	LIBTYPE_NFMTLIB=0x7200,
	//钓鱼黑白名单库
	LIBTYPE_FISHWLSIMPLE=0x10007200,
	LIBTYPE_FISHWLCOMPLEX,
	LIBTYPE_FISHBLSIMPLE,
	LIBTYPE_FISHBLCOMPLEX,

	//恶意下载黑白名单库
	LIBTYPE_MALURLWLSIMPLE=0x10008200,
	LIBTYPE_MALURLWLCOMPLEX,
	LIBTYPE_MALURLBLSIMPLE,
	LIBTYPE_MALURLBLCOMPLEX,

	//新的PE库
	LIBTYPE_NEWPE = 0x7fff,
	//新NORMAL库
	LIBTYPE_NOR11 = 0x9001,

	// A000 - B000 被MPEnginez占用
	LIBTYPE_MP_NAME		= 0xA000,
	LIBTYPE_MP_PE		= 0xA001,
	LIBTYPE_MP_ANDROID	= 0xA002,	// Android/DEX/ARM-ELF
	LIBTYPE_MP_EXPL1	= 0xA003,	// SWF/PDF/DOC/MP3/
	LIBTYPE_MP_OTHER	= 0xA004,

	//所有库
	LIBTYPE_ALL = 0xff,
}LIBTYPE;

/*
┌───────────────────────────────────────────────┐
│F│E│D│C│B│A│9│8│7│6│5│4│3│2│1│0│F│E│D│C│B│A│9│8│7│6│5│4│3│2│1│0│
└───────────────────────────────────────────────┘
│ │ │ │ │ │ │ │ │                      └ 0.F 病毒库类型
│ │ │ │ │ │ │ │ └───────────── 0.7 保留字段，全部置0
│ │ │ │ │ │ │ └─────────────── 8 表table/索引index   \库标识
│ │ │ │ │ │ └───────────────── 9 增加或者修改的记录add\补丁标识（互斥）
│ │ │ │ │ └─────────────────── A 删除的记录		 del/
│ │ │ │ └───────────────────── B 使用Dummy格式	（如果置位则以CDummyTable来创建实例）
│ │ │ └─────────────────────── C 保留位2			（★★★必须置0）
│ │ └───────────────────────── D 自定义位1(可执行类：过API)
│ └─────────────────────────── E 自定义位2
└───────────────────────────── F 自定义位3
*/
//病毒名最大长度
#define MAX_NAME_LEN			64

// Modify by Rangh 2008-7-28
// Modify Begin
#define TABLETYPE_INDEX			0x1000000
#define TABLETYPE_TABLE			0x0000000
#define TABLETYPE_PATCH_MOD		0x2000000
#define TABLETYPE_PATCH_DEL		0x4000000
#define TABLETYPE_DUMMY			0x8000000
#define TABLETYPE_RESERVE2		0x10000000

#define TABLETYPE_NOIDFLAT		0x10000000

#define TABLETYPE_USER1			0x20000000
#define TABLETYPE_USER2			0x40000000
#define TABLETYPE_USER3			0x80000000
#define TABLETYPE_MASK			0xffff
#define TABLEGROUP_MASK			0xf0000000
#define IsIndex(w)					((w & TABLETYPE_INDEX) == TABLETYPE_INDEX)
#define IsTable(w)					((w & TABLETYPE_INDEX) != TABLETYPE_INDEX)
#define IsDummy(w)					((w & TABLETYPE_DUMMY) == TABLETYPE_DUMMY)
#define SwitchToTable(w)			(w & (~TABLETYPE_INDEX))
#define SwitchToIndex(w)			(w | TABLETYPE_INDEX)
#define SwitchToDummy(w)			(w | TABLETYPE_DUMMY)

// from engine\publib\libloaderutil.h
#define IsSet(w, opt)		((w & opt) == opt)

#define Convert2DWord(wValue,dwValue)	\
{	\
	WORD hIndex = (WORD)((wValue) & 0xff00);	\
	WORD lIndex = (WORD)((wValue) & 0x00ff);	\
	(dwValue) = (hIndex << 16 | lIndex);	\
	}

#define Convert2Word(dwValue,wValue) \
{	\
	BYTE hIndex = (BYTE)(((dwValue) & 0xff000000) >> 24);	\
	BYTE lIndex = (BYTE)((dwValue) & 0x000000ff );	\
	(wValue) = (hIndex << 8 | lIndex );	\
	}

// Modify End

//==================================================================================

//----------------------------------------------------------------------------------

// 2013/3/21	ILibLoader::Init has dropped!!!

//库加载初始化选项	ILibLoader->Init(XXXX)
#define INIT_NORMAL					0x0

//使用共享内存
//win32下使用
#define INIT_SHARED_MEM				0x1

//是否一次全部加载
//用于linux下面的fork,将读写操作
#define INIT_NODELAY				0x2

//目前模式有   share / nodelay / none 3种。不可混用
// Modify by Rangh 2008-7-28
// Modify Begin 将wTableType由WORD类型修改为DWORD类型
class IPreLoader : public IUnknown
{
public:
	RE_DECLARE_IID;

	STDMETHOD(Init)			(IN LPCSTR pszCfgFile,IN DWORD dwOption = INIT_SHARED_MEM) PURE;
	STDMETHOD(GetTableInfo)	(IN DWORD dwTableType, IN OUT LibTableInfo *lpTableInfo) PURE;
	STDMETHOD(GetTableData)	(IN DWORD dwTableType, IN DWORD dwBufferSize , IN OUT VOID *pBuffer) PURE;
	STDMETHOD(SaveTables)	(IN IBaseTable **ppTableInfo,	IN DWORD dwTableCount) PURE;
	STDMETHOD(GetLibVer)(OUT WORD &wVer)PURE;

};

RE_DEFINE_IID(IPreLoader, "{2BE1A5F8-31B2-43bc-B455-4E7A78757702}",
			  0x2be1a5f8, 0x31b2, 0x43bc, 0xb4, 0x55, 0x4e, 0x7a, 0x78, 0x75, 0x77, 0x2);


//----------------------------------------------------------------------------------

class ILibLoader : public IUnknown
{
public:
	RE_DECLARE_IID;

	STDMETHOD(Init)			(IN IPreLoader* pPreLoad,			//必须参数
		IN DWORD dwOption = INIT_SHARED_MEM,					//3种加载方式
		DWORD* dwTableTypeList = NULL, DWORD dwTableCount = 0	//库支持列表。空表示全支持
		) PURE;
	STDMETHOD(GetTable)		(IN DWORD dwTableType, OUT IBaseTable** ppBaseTable) PURE;
	STDMETHOD(GetLibVer)	(OUT WORD &wVer) PURE;
	STDMETHOD(UnInit)		() PURE;
};

RE_DEFINE_IID(ILibLoader, "{3EC06CAA-F669-46c7-A91C-2B1525BBE12B}",
			  0x3ec06caa, 0xf669, 0x46c7, 0xa9, 0x1c, 0x2b, 0x15, 0x25, 0xbb, 0xe1, 0x2b);

class ILibLoaderUsePreLoader:public ILibLoader
{
public:
	RE_DECLARE_IID;
	STDMETHOD(GetPreLoader)(IN OUT IPreLoader** ppPreLoader) PURE;
};
RE_DEFINE_IID(ILibLoaderUsePreLoader, "{67AF86DC-0790-4f40-AA26-41DB3595F40A}",
			  0x67af86dc, 0x790, 0x4f40, 0xaa, 0x26, 0x41, 0xdb, 0x35, 0x95, 0xf4, 0xa);
// Modify End



class ITableIndex : public IUnknown
{
public:
	//创建索引，返回索引记录个数，负数表示失败，0表示内存不足 dwIndexSize中包含需要的内存大小，
	STDMETHOD_(LONG, MakeIndexBuf)(IN  BYTE *pbyBase, DWORD dwBufSize,IN DWORD dwVirusCount, 
		IN OUT BYTE *pbyIndexBuf, OUT DWORD &dwIndexSize) PURE;

	STDMETHOD_(LONG, CompareRecord)(IN  BYTE *pRecord1,IN DWORD dwLen1,IN  BYTE *pRecord2,IN DWORD dwLen2) PURE;
	STDMETHOD_(LONG,GetRecordSize)(IN  BYTE *pRecord ,IN DWORD dwBufSize) PURE;
	STDMETHOD_(LONG,MakeLibHead)(IN  BYTE* pbyBase1,DWORD dwLibSize1,IN  BYTE* pbyBase2, DWORD dwLibSize2,IN OUT BYTE* pbyHeadBuf,IN OUT DWORD &dwHeadSize)PURE;
	STDMETHOD(GetHeadSize)(IN  BYTE* pbyBase,IN DWORD dwLibSize,OUT DWORD &dwHeadSize)PURE;
	RE_DECLARE_IID;
};

RE_DEFINE_IID(ITableIndex, "{7D34ED9A-7515-407d-87B8-27688F5F8F35}",
			  0x7d34ed9a, 0x7515, 0x407d, 0x87, 0xb8, 0x27, 0x68, 0x8f, 0x5f, 0x8f, 0x35);

//==================================================================================
//GUID object

// {6805A6DF-F472-4304-BE5A-03C957D62B08}
DEFINE_GUID(CLSID_CREScriptIndexMaker, 0x6805a6df, 0xf472, 0x4304, 0xbe, 0x5a, 0x3, 0xc9, 0x57, 0xd6, 0x2b, 0x8);
// {372FB3F2-0684-4c85-B742-DBDA14FD7873}
DEFINE_GUID(CLSID_CREMemIndexMaker, 0x372fb3f2, 0x684, 0x4c85, 0xb7, 0x42, 0xdb, 0xda, 0x14, 0xfd, 0x78, 0x73);
// {D11A0854-EB97-43d5-A35B-ECD3B149031A}
DEFINE_GUID(CLSID_CREPeIndexTableMaker, 0xd11a0854, 0xeb97, 0x43d5, 0xa3, 0x5b, 0xec, 0xd3, 0xb1, 0x49, 0x3, 0x1a);
// {24386696-398E-406a-8A57-580186732519}
DEFINE_GUID(CLSID_CREVMIndexTableMaker, 0x24386696, 0x398e, 0x406a, 0x8a, 0x57, 0x58, 0x1, 0x86, 0x73, 0x25, 0x19);
// {0c2cad7f-7934-4d3f-adf6-f472947d339f} 
DEFINE_GUID(CLSID_CREDeepIndexTableMaker, 0x0c2cad7f, 0x7934, 0x4d3f, 0xad, 0xf6, 0xf4, 0x72, 0x94, 0x7d, 0x33, 0x9f);
// {2ee80963-44a7-4cd4-9b98-c146b321469c} 
DEFINE_GUID(CLSID_CRESTRIndexTableMaker, 0x2ee80963, 0x44a7, 0x4cd4, 0x9b, 0x98, 0xc1, 0x46, 0xb3, 0x21, 0x46, 0x9c);
// {2d0a851a-cc94-47cf-9f79-7abbcc01c391} 
DEFINE_GUID(CLSID_CRENfmtIndexTableMaker,0x2d0a851a, 0xcc94, 0x47cf, 0x9f, 0x79, 0x7a, 0xbb, 0xcc, 0x01, 0xc3, 0x91);
// {9e62ff08-eab7-4837-887d-2ad1c880b87d} 
DEFINE_GUID(CLSID_CREIndexMaker_Normal, 0x9e62ff08, 0xeab7, 0x4837, 0x88, 0x7d, 0x2a, 0xd1, 0xc8, 0x80, 0xb8, 0x7d);
// {80ed51ff-2032-43f7-a66b-c49e5fee78d7} 
DEFINE_GUID(CLSID_CREIndexMaker_FishSimple, 0x80ed51ff, 0x2032, 0x43f7, 0xa6, 0x6b, 0xc4, 0x9e, 0x5f, 0xee, 0x78, 0xd7);
// {602dca6d-836e-48b0-8882-c956c1bed650} 
DEFINE_GUID(CLSID_CREIndexMaker_FishComplex, 0x602dca6d, 0x836e, 0x48b0, 0x88, 0x82, 0xc9, 0x56, 0xc1, 0xbe, 0xd6, 0x50);
// {15543502-A5AD-4bc1-A118-BF68D00AE588}
DEFINE_GUID(CLSID_CREIndexMaker_MPELogic, 0x15543502, 0xa5ad, 0x4bc1, 0xa1, 0x18, 0xbf, 0x68, 0xd0, 0xa, 0xe5, 0x88);
// {23F29CA1-500F-48c8-9D85-B46509414074}
DEFINE_GUID(CLSID_CREIndexMaker_MPEName, 0x23f29ca1, 0x500f, 0x48c8, 0x9d, 0x85, 0xb4, 0x65, 0x9, 0x41, 0x40, 0x74);

//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
class IIniFile:public IUnknown
{
public:
	STDMETHOD(Open)(IN const CHAR* pszIniFileName) PURE;
	STDMETHOD_(DWORD, GetPrivateProfileString)(IN const CHAR* lpAppName,IN const CHAR* lpKeyName,IN const CHAR* lpDefault,OUT CHAR* lpReturnString,IN DWORD nSize)PURE;
	STDMETHOD(WritePrivateProfileString)(IN const CHAR* lpAppName,IN const CHAR* lpKeyName,IN const CHAR* lpString)PURE;
	STDMETHOD_(INT, GetPrivateProfileInt)(IN const CHAR* lpAppName,IN const CHAR* lpKeyName,IN INT nDefault)PURE;
	STDMETHOD( WritePrivateProfileInt)(IN const CHAR* lpAppName,IN const CHAR* lpKeyName,IN INT nValue)PURE;
	STDMETHOD(DeleteIniSection)(IN const CHAR *pSection)PURE;
	STDMETHOD(DeleteIniKey)(IN const CHAR *pSection, IN const CHAR *pKeyName)PURE;
	STDMETHOD(Commit)(const CHAR* pszFileName=NULL)PURE;
	RE_DECLARE_IID;

};

RE_DEFINE_IID(IIniFile, "{BFCC8EFB-B845-426d-8E0C-880C30FC8757}",
			  0xbfcc8efb, 0xb845, 0x426d, 0x8e, 0xc, 0x88, 0xc, 0x30, 0xfc, 0x87, 0x57);


//----------------------------------------------------------------------------------
typedef struct 
{
	CHAR pszLibFilename[MAX_PATH];
	BYTE pbyMd5Crc[0x10];
}STLibFileList,*PSTLibFileList;


// 在ROT中只要有此PID,则表明要创建
// {A6F0588D-5053-4461-83CD-929791CDC525}
DEFINE_GUID(PID_LibTool_CfgDisableTrojanIndex, 0xa6f0588d, 0x5053, 0x4461, 0x83, 0xcd, 0x92, 0x97, 0x91, 0xcd, 0xc5, 0x25);

class ILibTool:public IUnknown
{
	//基本要求 cfg文件和库文件在同一目录下
public:
	STDMETHOD(Init)(IN const CHAR* pszBaseCfgFileName) PURE;
	STDMETHOD(Install)(IN const CHAR* pszDestPath)PURE;
	//如果传入参数指针参数为空，dwCount 返回需要下载的文件列表大小
	STDMETHOD(GetDownFileList)(IN const CHAR* pszNewCfgFileName,OUT CHAR pszOutputPath[MAX_PATH],OUT PSTLibFileList pstList,OUT DWORD &dwCount)PURE;
	STDMETHOD(UpdateLib)()PURE;
	STDMETHOD(AddDaily)(const CHAR* pszLibFileName)PURE;
	RE_DECLARE_IID;
	enum
	{
		VIRUSLIB_ERROR_BAD_CONFIG=0x8f000000,
		VIRUSLIB_ERROR_BAD_DESTPATH,
		VIRUSLIB_ERROR_BAD_LIBFILE,
		VIRUSLIB_UPDATE_FILE_FAIL,
	};
};

RE_DEFINE_IID(ILibTool, "{3EE0394B-B413-4d22-9A7F-68CFB0223553}",
			  0x3ee0394b, 0xb413, 0x4d22, 0x9a, 0x7f, 0x68, 0xcf, 0xb0, 0x22, 0x35, 0x53);




class IUpLibLoader : public ILibLoader
{
public:
	//STDMETHOD(GetFirstTable)(IN DWORD dwTableType,IN OUT IBaseTable** ppBaseTable) PURE;
	//STDMETHOD(GetNextTable)(IN DWORD dwTableType,IN OUT IBaseTable** ppBaseTable)PURE;
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IUpLibLoader, "{A9D8D395-6E84-46bb-866D-4AFCCCB291E5}",
			  0xa9d8d395, 0x6e84, 0x46bb, 0x86, 0x6d, 0x4a, 0xfc, 0xcc, 0xb2, 0x91, 0xe5);


//----------------------------------------------------------------------------------

class IUpPreLoader: public IPreLoader
{
public:
	STDMETHOD(Init)(IN ILibLoader* pLibLoader,IN DWORD dwOption = INIT_SHARED_MEM) PURE;
	STDMETHOD(Load)(IN CHAR* pszFileName,IN DWORD dwOptions = INIT_SHARED_MEM) PURE;
	RE_DECLARE_IID;
	enum
	{
		PRELOAD_OPTION_LIBFILE,
		PRELOAD_OPTION_PATCH,
		PRELOAD_OPTION_ALLS,
		PRELOAD_OPTION_LOCAL,
		PRELOAD_OPTION_DAILY,

	};
};

RE_DEFINE_IID(IUpPreLoader, "{742AE5EA-0435-4f29-BC35-36C48A847535}",
			  0x742ae5ea, 0x435, 0x4f29, 0xbc, 0x35, 0x36, 0xc4, 0x8a, 0x84, 0x75, 0x35);


//----------------------------------------------------------------------------------

//病毒库生成模式
#define	GENERATE_CRYPT      0x10000		//加密
#define	GENERATE_ZIP		0x20000	    //以ZIP格式压缩

#define DEFMOD_CRW		(OMRead|OMWrite|OMCreate)
#define DEFMOD_RW			(OMRead|OMWrite)		


class ILibFile: public IUnknown
{
public:
	STDMETHOD_(BOOL, Load)(const CHAR* pszLibFileName, DWORD dwDefOpenMod = DEFMOD_RW) PURE;
	STDMETHOD_(INT, GetTableCount)(VOID) PURE;
	STDMETHOD(GetTableInfo)(INT iIndex,LibTableInfo* pTableInfo) PURE;
	STDMETHOD(GetTableData)(INT iIndex, BYTE* pData, DWORD& nLen) PURE;
	//	STDMETHOD(Append)(PSTTABLEINFO pInsertTable) PURE;
	STDMETHOD(Append)(const LibTableInfo* pTableinfo,DWORD dwGenerateOption, const BYTE* pData) PURE;
	STDMETHOD(Delete)(DWORD dwTableType) PURE;	// Modify by Rangh 2008-7-28
	STDMETHOD(Save)(VOID) PURE;
	RE_DECLARE_IID;

};

RE_DEFINE_IID(ILibFile, "{5D1AFDFB-659A-4267-9549-905CC3C15923}",
			  0x5d1afdfb, 0x659a, 0x4267, 0x95, 0x49, 0x90, 0x5c, 0xc3, 0xc1, 0x59, 0x23);
//////////////////////////////////////////////////////////////////////////

//==================================================================================
//GUID object

// {291CCAC2-F69C-428d-BE3A-074504F29E28}
DEFINE_GUID(CLSID_CREPreLoader, 
			0x291ccac2, 0xf69c, 0x428d, 0xbe, 0x3a, 0x7, 0x45, 0x4, 0xf2, 0x9e, 0x28);
// {2BA011B0-81D3-49a8-85D8-392100DCB9CE}
DEFINE_GUID(CLSID_CRELibLoader, 
			0x2ba011b0, 0x81d3, 0x49a8, 0x85, 0xd8, 0x39, 0x21, 0x0, 0xdc, 0xb9, 0xce);
// {56D2C94C-A662-4cf4-9077-9530000CD448}
DEFINE_GUID(CLSID_CREDelIndexMaker, 
			0x56d2c94c, 0xa662, 0x4cf4, 0x90, 0x77, 0x95, 0x30, 0x0, 0xc, 0xd4, 0x48);
//defined by liufeng 2007-4-11
// {E6EEDEE1-4980-4013-878F-8D38A9B2D3F5}
DEFINE_GUID(CLSID_CREPlugInTable, 
			0xe6eedee1, 0x4980, 0x4013, 0x87, 0x8f, 0x8d, 0x38, 0xa9, 0xb2, 0xd3, 0xf5);
// {AD0BA09D-9A8B-465e-AAD9-1E466C6739B1}
DEFINE_GUID(CLSID_CREUpLibLoader, 
			0xad0ba09d, 0x9a8b, 0x465e, 0xaa, 0xd9, 0x1e, 0x46, 0x6c, 0x67, 0x39, 0xb1);
// {D77A314E-B731-4ff4-AD34-78A862618A7F}
DEFINE_GUID(CLSID_CREUpPreLoader, 
			0xd77a314e, 0xb731, 0x4ff4, 0xad, 0x34, 0x78, 0xa8, 0x62, 0x61, 0x8a, 0x7f);
// {5F8B1770-EF38-4d14-A995-071667B390F8}
DEFINE_GUID(CLSID_CRELibFile, 
			0x5f8b1770, 0xef38, 0x4d14, 0xa9, 0x95, 0x7, 0x16, 0x67, 0xb3, 0x90, 0xf8);
// {A3DE5D2A-4631-4fb3-91DD-A61C6819C7F1}
DEFINE_GUID(CLSID_CRELibIndexFile, 
			0xa3de5d2a, 0x4631, 0x4fb3, 0x91, 0xdd, 0xa6, 0x1c, 0x68, 0x19, 0xc7, 0xf1);
// Add By Rangh 2008-7-29
// {EA422442-75A8-42e5-B3B4-31A16C0510D4}
DEFINE_GUID(CLSID_CREWORDLibFile, 
			0xea422442, 0x75a8, 0x42e5, 0xb3, 0xb4, 0x31, 0xa1, 0x6c, 0x5, 0x10, 0xd4);
// {E1A43D30-0270-492e-A1BD-43784B809856}
DEFINE_GUID(CLSID_CREUpLibTable, 
			0xe1a43d30, 0x270, 0x492e, 0xa1, 0xbd, 0x43, 0x78, 0x4b, 0x80, 0x98, 0x56);
// {C5B18E80-F2EE-4a64-AF2A-C8CC82B281D3}
DEFINE_GUID(CLSID_CREIndexObjConfig, 
			0xc5b18e80, 0xf2ee, 0x4a64, 0xaf, 0x2a, 0xc8, 0xcc, 0x82, 0xb2, 0x81, 0xd3);
// {C9F22BA6-4F19-44ce-9BF7-8EE3BC85F3E0}
DEFINE_GUID(CLSID_CRELibTool, 
			0xc9f22ba6, 0x4f19, 0x44ce, 0x9b, 0xf7, 0x8e, 0xe3, 0xbc, 0x85, 0xf3, 0xe0);
// {37A7E1C4-DF35-4be4-83CF-52008267A592}
DEFINE_GUID(CLSID_CREIniFile, 
			0x37a7e1c4, 0xdf35, 0x4be4, 0x83, 0xcf, 0x52, 0x0, 0x82, 0x67, 0xa5, 0x92);
// {8898BB96-6BD5-46ea-A676-7D566CC793D1}
DEFINE_GUID(CLSID_CREShareNameAlloc, 
			0x8898bb96, 0x6bd5, 0x46ea, 0xa6, 0x76, 0x7d, 0x56, 0x6c, 0xc7, 0x93, 0xd1);

// {EE146F88-7188-4211-86C9-A39AA7E9AE94}
DEFINE_GUID(CLSID_CRELocalNameAlloc, 
			0xee146f88, 0x7188, 0x4211, 0x86, 0xc9, 0xa3, 0x9a, 0xa7, 0xe9, 0xae, 0x94);

// {BFD71250-C77D-4534-8F3F-35B6C05369BB}
DEFINE_GUID(CLSID_CRENativeNameAlloc, 
			0xbfd71250, 0xc77d, 0x4534, 0x8f, 0x3f, 0x35, 0xb6, 0xc0, 0x53, 0x69, 0xbb);
#endif //__ILIBLOADER_H__


