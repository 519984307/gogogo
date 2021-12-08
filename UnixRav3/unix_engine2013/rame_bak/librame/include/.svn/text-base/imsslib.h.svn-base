#ifndef __IMss_LIB__
#define __IMss_LIB__

#include "mss.fmt.h"

enum 
{
	MSS_CMP_S_SAME		= S_FALSE,
	MSS_CMP_S_DIFF		= S_OK,

	MSS_S_OK			= S_OK,
	MSS_E_FAIL			= E_FAIL,
	MSS_E_OUTOFMEMORY	= E_OUTOFMEMORY,
	MSS_E_INVALIDARG	= E_INVALIDARG,
	MSS_E_NOINTERFACE	= E_NOINTERFACE,
	MSS_E_UNEXPECTED	= E_UNEXPECTED,
	MSS_E_NOTIMPL		= E_NOTIMPL,

	MSS_E_CODER			= _HRESULT_TYPEDEF_(0xF0000001),
	MSS_E_FILE_IO		= _HRESULT_TYPEDEF_(0xF0000002),
	MSS_E_BAD_FORMAT	= _HRESULT_TYPEDEF_(0xF0000003),
	MSS_E_NO_TABLE		= _HRESULT_TYPEDEF_(0xF0000004),
	MSS_E_ERR_TABLE		= _HRESULT_TYPEDEF_(0xF0000005),
	MSS_E_BAN_TABLE		= _HRESULT_TYPEDEF_(0xF0000006),
	MSS_E_BAN_FILE		= _HRESULT_TYPEDEF_(0xF0000007),
	MSS_E_MAPPING		= _HRESULT_TYPEDEF_(0xF0000008),
	MSS_E_COMPATIBLE	= _HRESULT_TYPEDEF_(0xF0000009),
	MSS_E_BUFTOOSMALL	= _HRESULT_TYPEDEF_(0xF000000A),
	MSS_E_NOMOREDATA	= _HRESULT_TYPEDEF_(0xF000000B),
	MSS_E_DATACRASHED	= MSS_E_BAD_FORMAT,

	MSS_E_UNMATCHED		= _HRESULT_TYPEDEF_(0xF0000010), 

	MSS_E_IMPOSSIBLE	= _HRESULT_TYPEDEF_(0xF0000020), 

	MSS_E_BAD_DIFF		= MSS_E_BAD_FORMAT, //_HRESULT_TYPEDEF_(0xF0000021), 
	MSS_E_BAD_FIXCMD	= MSS_E_BAD_FORMAT, //_HRESULT_TYPEDEF_(0xF0000022), 
};

typedef struct MSTDAT
{
	LPCVOID	Base;
	SIZE_T	Size;
} MSTDAT, *LPMSTDAT;

class IMssLibFile;

class IMssTable : public IUnknown
{
public:
	STDMETHOD_(UINT32,Attributes)() PURE;
	STDMETHOD_(UINT32,GetVersion)() PURE;
	STDMETHOD(GetName)( MSTNAME* lpName ) PURE;
	STDMETHOD(GetContent)( LPMSTDAT lpDat ) PURE;
	STDMETHOD(GetInfo)( LPMSTINFO lpInfo ) PURE;
	STDMETHOD_(BOOL,IsSameTable)( IMssTable* lpTable ) PURE;
	RE_DECLARE_IID;
};

// {1ED0DE5D-D90E-4ddf-85CA-395CCDA22F43}
RE_DEFINE_IID(	IMssTable, 
				"{1ED0DE5D-D90E-4ddf-85CA-395CCDA22F43}", 
				0x1ed0de5d, 0xd90e, 0x4ddf, 0x85, 0xca, 0x39, 0x5c, 0xcd, 0xa2, 0x2f, 0x43);


//static LPCSTR TOID_TABLE_INDEX = "TOID_TABLE_INDEX";

// {43BC40E6-4123-468d-B0C5-379C052CE5DC}
DEFINE_GUID(TOID_TABLE_INDEX,	0x43bc40e6, 0x4123, 0x468d, 0xb0, 0xc5, 0x37, 0x9c, 0x5, 0x2c, 0xe5, 0xdc);


struct ITableProvider
{
	STDMETHOD( SetTableInfo )( IMssLibFile* lpLibFile, LPMSTINFO lpTabInfo, SIZE_T* lpSize ) PURE;
	STDMETHOD( ProductData )( IMssLibFile* lpLibFile, LPMSTINFO lpTabInfo, PVOID lpBuffer, SIZE_T cbSize ) PURE;
};

typedef HRESULT ( * PTN_PRIVATE_DATA_CREATOR )( IMssTable * lpTable, IUnknown * prot ) ;

/*
	MSLLibFile : 
		IMssLibFile
		IREProperty
		ILibLoader
*/

enum { INVALID_VERSION = -1 };

class IMssLibFile : public IUnknown
{
public:

	//
	//	
	//

	STDMETHOD_( LPCSTR, GetFilePath )() PURE;

	//
	//	if failed, return INVALID_VERSION
	//

	STDMETHOD_( UINT32, GetVersion )() PURE;

	//
	//	根据表名获得库文件中的表	
	//

	STDMETHOD(RequireTable)( LPCMSTNAME lpName, ULONG dwFlags, IMssTable ** ppTable, PTN_PRIVATE_DATA_CREATOR pfn = NULL ) PURE;

	//
	//	禁用/启用 指定的表
	//		禁用时，会设置表属性的MSTA_BAN位；若表已经加载，则从LibFile中摘除
	//

	STDMETHOD(EnableTable)( LPCMSTNAME lpName, BOOL bEnable ) PURE;

	//
	//	显式的将表从内存中卸载(如果表没有再被人使用)
	//

	STDMETHOD(FreeTable)( LPCMSTNAME lpName ) PURE;

	//
	//	创建一张基于本库文件的表
	//		if MSTA_WINK set, not PIN to LibFile
	//		if MSTA_SHARED set, create a shared-table
	//		if MSTA_BAN set, return E_UNEXPECTED directly
	//		if MSTA_COMPATIBLE set, 
	//	

	STDMETHOD(CreateTable)( LPCMSTNAME lpName, ULONG uAttr, ITableProvider* lpDataProvider, IMssTable** ppTable ) PURE;

	//
	// 要求给IRXCallback接口
	//	不给的话，使用内置机制进行新版本探测
	//

	STDMETHOD(HotSwitch)( IUnknown * lpVerQuery = NULL ) PURE;

	RE_DECLARE_IID;
};

// {8E732568-1C6F-49e7-A8E7-3935C38738CD}
RE_DEFINE_IID( IMssLibFile, "{8E732568-1C6F-49e7-A8E7-3935C38738CD}",
			0x8e732568, 0x1c6f, 0x49e7, 0xa8, 0xe7, 0x39, 0x35, 0xc3, 0x87, 0x38, 0xcd);

//////////////////////////////////////////////////////////////////////////

class IMssLibFileEx : public IMssLibFile
{
public:
	STDMETHOD_( LPCSTR, GetFilePathA)() PURE;
	STDMETHOD_( LPCWSTR, GetFilePathW)() PURE;
	RE_DECLARE_IID;
};

// {7954BCA4-3A98-4AF2-86D7-042F21F790AC}
RE_DEFINE_IID(IMssLibFileEx, "{7954BCA4-3A98-4AF2-86D7-042F21F790AC}",
			  0x7954bca4, 0x3a98, 0x4af2, 0x86, 0xd7, 0x4, 0x2f, 0x21, 0xf7, 0x90, 0xac);


//////////////////////////////////////////////////////////////////////////

class IMssLibFilePatcher : public IUnknown
{
public:
	STDMETHOD(Init)( IREStream* file, SIZE_T cbDeltaMAX = 8*1024 ) PURE;
	STDMETHOD(Verify)( IREStream* file ) PURE;
	STDMETHOD(AddPatch)( IREStream* delta ) PURE;
	STDMETHOD(Apply)( IREStream* outf, xsf::CODERID uCoderId ) PURE;
	STDMETHOD(GetVer)( UINT32 * puVer ) PURE;
	RE_DECLARE_IID;
};
// {CE1E999C-0961-4bf3-8279-117D5AE09F41}
RE_DEFINE_IID( IMssLibFilePatcher, "{CE1E999C-0961-4bf3-8279-117D5AE09F41}",
			0xce1e999c, 0x961, 0x4bf3, 0x82, 0x79, 0x11, 0x7d, 0x5a, 0xe0, 0x9f, 0x41);


class IRXParameters;
class IMssLibFileEngineConfig : public IUnknown
{
public:
	STDMETHOD(GetEngineParam)(IRXParameters* param)PURE;
	RE_DECLARE_IID;
};
// {04BFE02E-1586-4F7C-8B6C-AABB075B57AE}
RE_DEFINE_IID(IMssLibFileEngineConfig, "04BFE02E-1586-4F7C-8B6C-AABB075B57AE",
			0x4bfe02e, 0x1586, 0x4f7c, 0x8b, 0x6c, 0xaa, 0xbb, 0x7, 0x5b, 0x57, 0xae);

//////////////////////////////////////////////////////////////////////////
//	TODO
// 解决多文件组合库的问题, 也继承ILibLoader
//
class IMssLibLoader : /*public ILibLoader*/ public IUnknown
{
public:
	STDMETHOD(Takeover)( IMssLibFile* opLibFile ) PURE;
	STDMETHOD(RequireTable)( LPMSTNAME lpName, ULONG dwFlags, IMssTable ** ppTable ) PURE;
	STDMETHOD(RequireFile)( LPMSTNAME lpName, IMssLibFile** ppLibFile ) PURE;
	STDMETHOD(GetFileCount)( SIZE_T* pFileCount ) PURE;
	STDMETHOD(GetLibFile)( SIZE_T uIndex, IMssLibFile** ppLibFile ) PURE;
};

//////////////////////////////////////////////////////////////////////////
//
//
//

class IMssToolkit : public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD( LibCompare )( IREStream* lib1, IREStream* lib2, IREStream* diff, xsf::CODERID8 coder ) PURE;
	STDMETHOD( MakeDeltaNameA )( IREStream* lib1, IREStream* lib2, CHAR rDeltaName[128] ) PURE;
	STDMETHOD( MakeDeltaNameW )( IREStream* lib1, IREStream* lib2, WCHAR rDeltaName[128] ) PURE;
	STDMETHOD( VerifyFile )( LPCSTR aFullName ) PURE;
	STDMETHOD( VerifyFile )( IREStream * lpLib ) PURE;
};

// {E67FFBDE-62BE-4801-BC9B-BC87F76F684E}
RE_DEFINE_IID( IMssToolkit, "{E67FFBDE-62BE-4801-BC9B-BC87F76F684E}",
			0xe67ffbde, 0x62be, 0x4801, 0xbc, 0x9b, 0xbc, 0x87, 0xf7, 0x6f, 0x68, 0x4e);

#if 0
class IMssCallback : public IUnknown
{
public:

	STDMETHOD(QueryLibFileVersion)( IMssLibFile* opLibFile, UINT32 uCurrent, UINT32 * lpLastVer ) PURE;

	// 热升级完成，返回值不理会
	STDMETHOD(AfterHotUpdated)( IMssLibFile* opLibFile, UINT32 uOldVersion ) PURE; 

	// 进入直接加载模式，返回值不理会
	STDMETHOD(AfterFileMappingClosed)( IMssLibFile* opLibFile, LPCSTR aDirectFileName ) PURE; 

	// 退出直接加载模式，此时aDirectFileName可以进行删除或更新，返回值不理会
	// 可能是关闭了库、进行了热升级等等
	STDMETHOD(AfterDirectModeExit)( IMssLibFile* opLibFile, LPCSTR aDirectFileName ) PURE; 

};

#endif

#endif