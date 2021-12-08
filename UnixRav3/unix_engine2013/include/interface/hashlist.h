#ifndef __RX_HASH_LIST__
#define __RX_HASH_LIST__

enum HASH_CLASS_8
{
	// PE NORMAL
	HASH_PE_SCANTJ_10					=0x00,	// 10木马库
	HASH_PE_SCANTJ_11					=0x01,	// 11木马库

	// PE EXE
	HASH_EXE_FULLTEXT_CRC64				=0x02,	// EXE全文
	HASH_EXE_NTHDR_SECTION				=0x03,	// Nt 头信息和部分节表信息CRC64
	HASH_EXE_SECTION_INFO				=0x04,	// 部分节表信息CRC64, 例如: “ 5:.text!24064|.rdata!5120|.data!1024|.ndata!0|.rsrc!16384:VirusName ”
	HASH_EXE_OEP_SECTION				=0x05,	// 入口点所在节数据CRC64
	HASH_EXE_IMP_LIB_API_STR			=0x06,	// 导入库名称所有API名称CRC
	HASH_EXE_RES_CATELOG				=0x07,	// 资源结构CRC64
	HASH_EXE_ALL_RESDATA				=0x08,	// 所有资源数据CRC64
	
	// PE DLL
	HASH_DLL_FULLTEXT_CRC64				=0x09,	// DLL全文
	HASH_DLL_NTHDR_SECTION				=0x0A,	// Nt 头信息和部分节表信息CRC64
	HASH_DLL_SECTION_INFO				=0x0B,	// 部分节表信息CRC64, 例如: “ 5:.text!24064|.rdata!5120|.data!1024|.ndata!0|.rsrc!16384:VirusName ”
	HASH_DLL_OEP_SECTION				=0x0C,	// 入口点所在节数据CRC64
	HASH_DLL_EXP_NAME_FUNC				=0x0D,	// DLL的所有导出函数名
	HASH_DLL_EXP_FUNC_CODE				=0x0E,	// 所有导出函数的前四个字节
	HASH_DLL_RES_CATELOG				=0x0F,	// 资源结构CRC64
	HASH_DLL_ALL_RESDATA				=0x10,	// 所有资源数据CRC64
	HASH_DLL_RELOC_INFO					=0x11,	// Dll的重定位信息

	// dotnet
	HASH_DOTNET_FULLTEXT_CRC64			=0x12,	// .net全文
	HASH_DOTNET_COMPRESSED_STREAM		=0x13,	// #~流
	HASH_DOTNET_STRINGS_STREAM			=0x14,	// String流
	HASH_DOTNET_BLOB_STREAM				=0x15,	// BLOB流
	HASH_DOTNET_GUID_STREAM				=0x16,	// GUID流
	HASH_DOTNET_US_STREAM				=0x17,	// US流
	// DEX
	HASH_DEX_FULLTEXT_CRC64				=0x18,	// 整个文件的CRC64
	HASH_DEX_HEADER						=0x19,	//
	HASH_DEX_STRING						=0x1A,	//
	HASH_DEX_FIDLD						=0x1B,	//
	HASH_DEX_METHOD						=0x1C,	//
	// Compressed Archive
	HASH_COMPRESSED_FILENAME			=0x1D,	// 包内所有文件名
	// LNK File
	HASH_LNK_COMMANDLINE_ARGUMENTS		=0x1E,	// LNK文件的命令行参数
	// APK File
	HASH_APK_CLASSESDEX					=0x1F,	// APK中DEX的全文CRC64
	// SWF File
	HASH_SWF_HEADER						=0x20,	// SWF的Header信息
	// .......

	HASH_CLASS_8_MAX					=0x100,
};

struct IRXHashListW
{
	STDMETHOD( PushHash )( SIZE_T clsHash, UINT64 uHash ) PURE;
};

//////////////////////////////////////////////////////////////////////////

typedef struct _XHASH
{
	SIZE_T	Cls;
	UINT64	Hash;
} XHASH, *LPXHASH;

class IRXHashList : public IUnknown, public IRXHashListW
{
public:
	STDMETHOD(GetCount)( SIZE_T * lpCount ) PURE;
	STDMETHOD(GetHashByIndex)( SIZE_T uIndex, LPXHASH lpHash ) PURE;
	STDMETHOD(GetHashByClass)( SIZE_T uClass, LPXHASH lpHash ) PURE;
public:
	RE_DECLARE_IID;
};

// {A37DA0A7-39AD-4143-9AA1-D02056F57A6C}
RE_DEFINE_IID( IRXHashList, "{A37DA0A7-39AD-4143-9AA1-D02056F57A6C}",
			0xa37da0a7, 0x39ad, 0x4143, 0x9a, 0xa1, 0xd0, 0x20, 0x56, 0xf5, 0x7a, 0x6c);

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI RxGetArchiveHash(UINT32 ArchiveFormat, IRXArchive * parc, IRXHashListW* pList);

//////////////////////////////////////////////////////////////////////////

#endif