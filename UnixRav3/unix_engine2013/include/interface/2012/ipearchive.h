#ifndef __PE_ARCHIVE_2012__
#define __PE_ARCHIVE_2012__

#include "pedef.h"
#include "hdex64.h"


enum {
	FDT_BYTE = 0,			//	0
	FDT_WORD,
	FDT_DWORD,
	FDT_QWORD,
	FDT_MAX,
};

#define TblDatSize(t)				(1 << (t))

//////////////////////////////////////////////////////////////////////////
//
//	RELOC 
//

typedef struct _RELOCA_BLOCK_INFO
{
	UINT32 dwRelocaStartVa;			//重定位起始地址
	UINT32 dwSizeOfBlock;			//重定位块的长度
	UINT32 dwCurrBlockRva;			//当前重定位结构(块)的起始地址
	UINT32 dwCurrBlockItemNum;		//当前重定位块中重定位条目个数
}RELOCA_BLOCK_INFO;

typedef struct _RELOC_LOCATION
{
	UINT16	wType;
	UINT32	dwLocRVA;
	UINT32	dwLocRAW;
	UINT32	dwAbslDat;
} RELOC_LOCATION;

//////////////////////////////////////////////////////////////////////////
//
//	ADV INFO
//

typedef struct _PE_ARCHIVE_INFO
{
	/*00*/	DWORD	FileSize;				//
	/*04*/	DWORD	OverlayPos;			//
	/*08*/	DWORD	OverlaySize;
	/*0C*/	DWORD	NumRxSections;		//! count of rx section 
	/*10*/	DWORD	NumRwxSections;		//! count of rwx section 
	/*14*/	INT32	EntrySectId;			//! section id which entry point at
	/*18*/	DWORD	RawEntryPoint;		//! offset of entry point
	/*1C*/	DWORD	CrcOfPackName;		//?
	/*20*/	DWORD	CompilerId;		//?
	/*24*/	DWORD	CrcOfISCI;		// CRC of ImageSize + ComplierID
	/*28*/	DWORD	CrcOfISITS;		// CRC of ImageSize + ImportTableSize
	/*2C*/	DWORD	CrcOfISETS;		// CRC of ImageSize + ExportTableSize
	/*30*/	DWORD	CrcOfISRDS;		// CRC of ImageSize + ResouceDirectorySize
	/*34*/	DWORD	OfsNtHeader;
	/*38*/	DWORD	OfsSectionTable;
	/*3C*/	DWORD	FileIsBadPE;		// Bad PE, can't be run
	/*40*/	DWORD	FileAsImage;		// File-Image
	/*44*/	DWORD	FileIsResDll;		// 
	/*48*/	DWORD	FileIsDotNet;
	/*4C*/	DWORD	AlignedFileSize;	//
			DWORD	ExecDataSize;		// All Executable DATA(maybe)
} PE_ARCHIVE_INFO, * LPPE_ARCHIVE_INFO;

//////////////////////////////////////////////////////////////////////////
//
//	TLS
//

typedef struct _TLS_INFO
{
	uint32_t staticTlsRva;
	uint32_t staticTlsSize;
	uint32_t staticTlsZeroedSize;
	gcptr_t  tlsCallbackTblRva;
	uint32_t tlsCallbackNum;
} TLS_INFO, * LPTLS_INFO;

typedef const TLS_INFO *	LPCTLS_INFO;

//////////////////////////////////////////////////////////////////////////
//
//	RESOURCE ++
//

typedef struct PE_RESOURCE_ENTRY
{
	winpe::IMAGE_RESOURCE_DIRECTORY_ENTRY	EntRaw;
	const char *				EntName;
	uint32_t				EntIndex;
	uint32_t				EntParent;
	uint32_t				EntRVA;
	uint32_t				EntBodyRVA;
} PREI, *LPPREI;

struct PE_RESOURCE_CATELOOG : PREI	
{
	uint32_t	CountOfSubCatelog;
	uint32_t	CountOfSubData;
}; 
typedef PE_RESOURCE_CATELOOG PRC, *LPPRC;

typedef struct _RTName
{
	const char *	NamePtr;
	uint32_t	StdCRC;
	uint32_t	StdCRCW;
	_RTName() : NamePtr(0), StdCRC(0), StdCRCW(0){};
} RTNAME;

struct PE_RESOURCE_DATA : PREI
{
	winpe::IMAGE_RESOURCE_DATA_ENTRY	RawData;
	RTNAME					FullPathName;
	RTNAME					FullPathNameR;
	const char *			ShortFPName;
};

typedef PE_RESOURCE_DATA PRD, *LPPRD;

enum {
	ENUM_CATELOG = 1,
	ENUM_DATA_ENTRY = 2,
};

//////////////////////////////////////////////////////////////////////////
//
//	Import Table
//

typedef uint32_t	bool32_t;
enum 
{
	ENUM_ORDINAL_SYMBOL = 1,
	ENUM_NAME_SYMBOL = 2,
	ENUM_ALL_SYMBOL = ENUM_ORDINAL_SYMBOL|ENUM_NAME_SYMBOL,
};

typedef struct _PE_IMPORTED_LIBRARY
{
	uint32_t		CRCOfNameI;		// Always CRC of LibName(lowcase)
	uint32_t		DescRVA;		// RVA of IMAGE_IMPORT_DESCRIPTOR
	union {
		uint32_t	NameRVA;		// NameRVA
		bool32_t	NotAppend;		// 
	};
	const char *		NamePtr;		// Ptr of Name String 
	uint32_t		ThunkBase;		// Thunks of this Lib
	uint32_t		IATBase;		// IAT of this Lib
	uint32_t		CountOfSymbolsN;
	uint32_t		CountOfSymbolsO;
} PIL, * LPPIL;

typedef struct _PE_IMPORTED_SYMBOL_32
{
	union {
		uint32_t		ThunkRVA;
		bool32_t		NotAppend;
	};
	winpe::IMAGE_THUNK_DATA32	RawThunk;
	uint32_t			IAT;
	const char *		NamePtr;
	union {
		uint32_t		NameRVA;
		uint32_t		Ordinal;	// if imported by ordinal
	};
	uint32_t			CRCOfName;
	uint32_t			CRCOfNameI;
	uint32_t			OwnerLib;	// lib CRC(lowcase)
} PIS32, *LPPIS32;

typedef struct _SimpSymbol {
	const char *	name;
	uint32_t	ordinal;
	uint32_t	iat;
} SimpSymbol;

//////////////////////////////////////////////////////////////////////////

typedef struct _PELI
{
	winpe::IMAGE_EXPORT_DIRECTORY	RawDirData;
	uint32_t			LibNameRVA;
	const char *		LibNamePtr;
	uint32_t			LibNameCRC;
	uint32_t			ExpDirRVA;
	size_t				ExportedSymbols;
} PELI, * LPPELI;

typedef struct _PESI
{
	uint32_t			EntryRVA;
	uint32_t			EAT;
	uint32_t			Ordinal;
	const char *		NamePtr;
	uint32_t			NameRVA;
	uint32_t			CRCOfName;
	uint32_t			CRCOfNameI;
} PESI, *LPPESI;

enum PE_ATTR
{
	PEA_BAD = 1,
	PEA_NO_ENTRY = 2,
	PEA_DOT_NET = 4,
	PEA_NO_CODE = 8,
	PEA_DLL = 16,
	PEA_64BIT = 32,
	PEA_NOT_X86 = 64,
	PEA_VB = 128,
	PEA_NATIVE = 256,
};

enum PEESTYPE
{
	SST_UNPACKED,
	SST_RESOURCE,
	SST_OVERLAY,
	SST_DUMPPED,
	SST_OVERLAY_SCRIPT,
};

class I12PEArchive : public IRXArchive
{
public:
	// *** Table - Record - Field *** //
	enum TABLE_ENUM
	{
		TABLE_MZ_HEADER = 0,
		TABLE_FILE_HEADER,
		TABLE_OPTIONAL_HEADER,
		TABLE_SECTION_HEADER,
		TABLE_DATA_DIRECTORY,
		TABLE_NT_HEADERS,

		TABLE_ADJ_TABLE_BASE, TABLE_ADJ_TABLE_MAX = TABLE_ADJ_TABLE_BASE,
		
		TABLE_PEHEAD_SECTION = TABLE_ADJ_TABLE_BASE + TABLE_ADJ_TABLE_MAX,
		
		TABLE_ARCHIVE_INFOR,

		TABLE_PE_TABLE_MAX,

		TABLE_ADJ_SECTION_HEADER = TABLE_ADJ_TABLE_MAX + TABLE_SECTION_HEADER,
	};

public:

	// Archive baseics

	STDMETHOD(ReOpen)() = 0 ;
	STDMETHOD(Save)() = 0 ;
	STDMETHOD(InsertEmbeded)( IREStream* pstm, PEESTYPE uType ) = 0;
	// record table

	STDMETHOD_(DWORD, GetTableSize)( DWORD tblID ) = 0;
	STDMETHOD(SetTable)( DWORD tblID, LPVOID tblBody, DWORD tblSize ) = 0;
	STDMETHOD_(LPVOID, GetTable)( DWORD tblID, OPTIONAL PDWORD tblSize = 0 ) = 0;
	STDMETHOD(GetField)( DWORD tblID, DWORD fldOfs, DWORD datType, OUT PVOID pvData ) = 0;
	STDMETHOD(SetField)( DWORD tblID, DWORD fldOfs, DWORD datType, IN PVOID pvData ) = 0;

	// address convert

	STDMETHOD(RVAToOffset)(DWORD dwRva, OUT DWORD *pdwOffset) = 0;
	STDMETHOD(RVAToSectionId)(DWORD dwRva, OUT DWORD *pdwSectionId) = 0;

	STDMETHOD(OffsetToRVA)(DWORD dwOffset, OUT DWORD *pdwRva) = 0;
	STDMETHOD(OffsetToSectionId)(DWORD dwOffset, OUT DWORD *pdwSectionId) = 0;

	STDMETHOD_(DWORD, OffsetToRVAAll)(DWORD dwOffset, OUT DWORD *pdwRva, IN DWORD dwNum ) = 0;
	STDMETHOD_(DWORD, OffsetToSectionIdAll)(DWORD dwOffset, OUT DWORD *pdwSectionId, IN DWORD dwNum ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//
	// 导入表
	//

	// return LibMark
	STDMETHOD_(INT32, AppendSymbols)( LPCSTR lpLibName, SimpSymbol * lpSSymbs, INT32 iCount ) = 0;
	STDMETHOD_(UINT32,EnumLibrary)( UINT32 Index, LPPIL * lppLibrary ) = 0;
	STDMETHOD_(UINT32,EnumSymbol)( UINT32 uiIndex, LPPIS32 * lppSymbol ) = 0;
	STDMETHOD_(UINT32,EnumSymbolOfLibrary)( LPPIL lpLibrary, UINT32 uiEnumOption, LPPIS32 * lppSymbol, UINT32 uiCount ) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//
	// 导出表
	//

	STDMETHOD_(DWORD, IsExistingExportApi )(LPCSTR strApiName,LPCSTR strModuleName = NULL) PURE;
	STDMETHOD_(DWORD, IsExistingExportApi )(DWORD dwApiNameCrc,DWORD dwModuleNameCrc = 0) PURE;
	STDMETHOD_(DWORD, GetExportApiByOrdinal )(DWORD dwOrdinals) PURE;
	STDMETHOD(RavIsExportApi )(DWORD dwRAV,BOOL *pbIsExportApi,LPSTR strApiName = NULL,DWORD dwApiNameSize = 0,LPSTR strModuleName = NULL,DWORD dwModuleNameSize = 0) PURE;
	STDMETHOD_(DWORD, GetExportApiNum)() PURE;
	STDMETHOD_( LPCSTR, GetExportModuleNameAndCrc)( DWORD * pdwNameCrc = NULL, DWORD * lpwRVA = NULL ) PURE;
	STDMETHOD_(BOOL, FindSubStrInApiName)(LPCTSTR lpSubStr) PURE;

	//////////////////////////////////////////////////////////////////////////
	//
	// 资源表表
	//

	STDMETHOD_(UINT32,EnumRcData)( UINT32 DataEntryIndex, LPPRD* lppDataInfo ) = 0;
	STDMETHOD_(UINT32,GetRootRVA)() = 0;
	STDMETHOD_(LPPRC,GetRcCatelog)( UINT32 uiDirRVA ) = 0;
	STDMETHOD_(UINT32,EnumChildren)( LPPRC lpDir, UINT32 uiEnumOption, LPPREI* lpRecvArray, UINT32 uiCount ) = 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// 重定位表
	//

	STDMETHOD_(DWORD,GetCountOfRelocItems)() PURE;
	STDMETHOD_(DWORD,GetXRefByRelocInfo)( DWORD dwAddress, DWORD dwIndex, PDWORD pdwXRefAddr ) PURE;
	STDMETHOD(GetRelocItem)( DWORD dwIndex, RELOC_LOCATION * pItem ) PURE;
	STDMETHOD_(DWORD,GetRelocPositions)( DWORD dwIndex, DWORD dwCount, PDWORD pRelocLocs ) PURE;

	//////////////////////////////////////////////////////////////////////////
	//
	//	TLS
	//

	STDMETHOD_(LPCTLS_INFO, QueryInforTLS )() PURE;

	STDMETHOD_(UINT64, GetCallbackEntry )( DWORD idxTls ) PURE;

	//////////////////////////////////////////////////////////////////////////
	//
	//
	//

	STDMETHOD_(DWORD, ReadContentByRVA )( DWORD dwRVA, PVOID lpBuffer, DWORD dwBytes ) PURE;

	//////////////////////////////////////////////////////////////////////////
	//
	//
	//

	STDMETHOD_(LPPESI, EnumExportedSymbols)( UINT32 uiIndex ) PURE;
	STDMETHOD_(LPPELI, GetLibraryInfo)() PURE;

	STDMETHOD_(SIZE_T, GetAttr)() PURE;
	//////////////////////////////////////////////////////////////////////////
	//
	// 通用个数
	//

	enum VALUE_CLASS 
	{
		IMPORT_SYMBOL_COUNT_32,
		IMPORT_LIBRARY_COUNT_32,
		ENTRYPOINT_ADDRESS_32,
		IMAGE_BASE_64,
		IMAGE_SIZE_32,
		SECTION_COUNT_32,
	};

	STDMETHOD(EasyGetValue)( VALUE_CLASS cls, VOID * lpParam ) = 0;

public:
	RE_DECLARE_IID;
};
// {6C1BDD1B-B9C4-4a5c-B617-58B2FBB47526}
RE_DEFINE_IID(I12PEArchive, "{6C1BDD1B-B9C4-4a5c-B617-58B2FBB47526}",
			0x6c1bdd1b, 0xb9c4, 0x4a5c, 0xb6, 0x17, 0x58, 0xb2, 0xfb, 0xb4, 0x75, 0x26);

//////////////////////////////////////////////////////////////////////////

class IPREPEFixup : public IUnknown
{
public:
	STDMETHOD( FormatedDeleteData )( DWORD dwOffset, DWORD dwBytes ) PURE;
	STDMETHOD( FormatedWriteData )( DWORD dwOffset, PVOID lpData, DWORD dwBytes, PDWORD pdwWritten ) PURE;
	RE_DECLARE_IID;
};

// {28A9FBFE-B31D-4aa1-90E7-D656FC6340F0}
RE_DEFINE_IID( IPREPEFixup, "{28A9FBFE-B31D-4aa1-90E7-D656FC6340F0}",
	0x28a9fbfe, 0xb31d, 0x4aa1, 0x90, 0xe7, 0xd6, 0x56, 0xfc, 0x63, 0x40, 0xf0);


class IPREPEFixup2 : public IUnknown
{
public:
	STDMETHOD( CalculateCheckSum )() PURE;
	RE_DECLARE_IID;
};
// {297BB1C0-1E4C-439D-836B-1A39FBADEE47}
RE_DEFINE_IID(IPREPEFixup2, "{297BB1C0-1E4C-439D-836B-1A39FBADEE47}" ,
			0x297bb1c0, 0x1e4c, 0x439d, 0x83, 0x6b, 0x1a, 0x39, 0xfb, 0xad, 0xee, 0x47);


class IREPESignatureComputer : public IUnknown
{
public:
	STDMETHOD( CalcSignature2010 )( ULARGE_INTEGER * liSignature ) PURE;
	STDMETHOD( CalcSignature2011 )( ULARGE_INTEGER * liSignature ) PURE;
	RE_DECLARE_IID;
};

// {D9E471B2-41D7-4cfe-9545-DB55DF4CCF8C}
RE_DEFINE_IID( IREPESignatureComputer, "{D9E471B2-41D7-4cfe-9545-DB55DF4CCF8C}",
			  0xd9e471b2, 0x41d7, 0x4cfe, 0x95, 0x45, 0xdb, 0x55, 0xdf, 0x4c, 0xcf, 0x8c);


//////////////////////////////////////////////////////////////////////////

class IREPEResourceTable : public IUnknown
{
public:
	// 指定资源类型id,资源项id:
	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
								   WORD idRcItem, WORD idRcDir,
								   WORD wLanguage = (WORD)(-1)) = 0;
	// 指定资源类型id,资源项名称:
	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
								   const CHAR* pszRcItemName, WORD idRcDir,
								   WORD wLanguage = (WORD)(-1)) = 0;
	// 指定资源类型名称,资源项id:
	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
								   WORD idRcItem, const CHAR* pszRcDirName,
								   WORD wLanguage = (WORD)(-1)) = 0;
	// 指定资源类型名称,资源项名称:
	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
								   const CHAR* pszRcItemName, const CHAR* pszRcDirName,
								   WORD wLanguage = (WORD)(-1)) = 0;

public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREPEResourceTable, "{3308ADE0-BC60-4d48-B033-188D34248415}",
0x3308ade0, 0xbc60, 0x4d48, 0xb0, 0x33, 0x18, 0x8d, 0x34, 0x24, 0x84, 0x15);


//////////////////////////////////////////////////////////////////////////


/*
// IExportTable
class IExportTable : public IUnknown
{
};

// IImportTable
class IImportTable : public IUnknown
{
};
*/


//////////////////////////////////////////////////////////////////////////


class IREX86Assembler : public IUnknown
{
public:
	// 反汇编函数:
	STDMETHOD_(int,Disassembler) (BYTE *incode, char *outbuff = 0, DWORD eip = 0) = 0 ;
	// 正汇编函数:
	STDMETHOD_(int,Assembler) (char *pasm, BYTE *outcode, DWORD eip) = 0 ;
	// 反汇编函数修正:
	STDMETHOD_(int,Disassembler) (BYTE *incode, DWORD dwLength, char *outbuff = 0, DWORD eip = 0) = 0 ;

public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREX86Assembler, "{F9BB3482-6A35-404c-B6E8-C83E9FCC8DFB}",
0xf9bb3482, 0x6a35, 0x404c, 0xb6, 0xe8, 0xc8, 0x3e, 0x9f, 0xcc, 0x8d, 0xfb);


//////////////////////////////////////////////////////////////////////////


// 分析函数的opcode列表:
// struct OPCode
// {
// 	DWORD rva;
// 	DWORD len;
// };
// <PROPVARIANT>.vt = VT_UI4 | VT_VECTOR
// <PROPVARIANT>.caul.cElems = <struct OPCode总数x2>
// <PROPVARIANT>.caul.pElems = <struct OPCode列表>
DEFINE_GUID(PID_PE_AnalysisProcess_OPCodeList,
			0x18894d11, 0x9e76, 0x4695, 0x9f, 0xbb, 0x6a, 0x28, 0x32, 0x6f, 0xca, 0xb1);

class IRECompilerSearcher : public IUnknown
{
public:
	enum MAJOR_COMPILER_TYPE
	{
		cmpl_vcplusplus = 0x01, //VC++
		cmpl_delphi     = 0x02, //Delphi
		cmpl_bcplusplus = 0x03, //BC++
		cmpl_other      = 0x04  //other
	};
	enum MINOR_COMPILER_VC_TYPE
	{
		cmpl_vc_unkcmpl = 0x00,//Microsoft Visual C++
		cmpl_vc_4x,			   //Microsoft Visual C++ 4.x
		cmpl_vc_50,			   //Microsoft Visual C++ 5.0
		cmpl_vc_60,			   //Microsoft Visual C++ 6.0
		cmpl_vc_70,			   //Microsoft Visual C++ 7.0
		cmpl_vc_v71,		   //Microsoft Visual C++ v7.1
		cmpl_vc_80,			   //Microsoft Visual C++ 8.0
		cmpl_vc_cab			   //Microsoft CAB
	};
	enum MINOR_COMPILER_DELPHI_TYPE
	{
		cmpl_delphi_unkcmpl = 0x00,//Borland Delphi
		cmpl_delphi_20,			   //Borland Delphi 2.0
		cmpl_delphi_30,			   //Borland Delphi 3.0
		cmpl_delphi_40,			   //Borland Delphi 4.0
		cmpl_delphi_50,			   //Borland Delphi 5.0
		cmpl_delphi_60,			   //Borland Delphi 6.0
		cmpl_delphi_70,			   //Borland Delphi 7.0
		cmpl_delphi_80			   //Borland Delphi 8.0
	};
	enum MINOR_COMPILER_BC_TYPE
	{
		cmpl_bc_unkcmpl = 0x00,//Borland C++
		cmpl_bc_1999,		   //Borland C++ 1999
		cmpl_bc_component_cn,   //Borland 组件
		cmpl_bc_component	   //Borland Component
	};
	enum MINOR_COMPILER_OTHER_TYPE
	{
		cmpl_other_unkcmpl = 0x00,     //unknown
		cmpl_other_winzip_unkcmpl,     //winzip
		cmpl_other_winrar_unkcmpl,     //winrar
		cmpl_other_wise_ins_unkcmpl,   //wise
		cmpl_other_ins_shelld_unkcmpl, //InstallShelld
		cmpl_other_nullsoft_PiMP,      //Nullsoft PiMP
		cmpl_other_inno_setup,         //Inno Setup
		cmpl_other_watcom_c_cpp,       //Watcom C/C++
	};
	static BYTE major_ver(WORD ver) { return (BYTE)(ver>>8); }
	static BYTE minor_ver(WORD ver) { return (BYTE)(ver); }

public:
	STDMETHOD(FindCompiler)(PDWORD pdwRvaMain = NULL, PWORD pwCompilerID = NULL) = 0 ;
	STDMETHOD(GetOpcode16)(BYTE* pbyOpCode) = 0 ;
	STDMETHOD(AnalysisProcess)(DWORD entry_rva, DWORD code_len, OUT IREProperty *presult) = 0 ;

public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IRECompilerSearcher, "{7D158BFE-0E39-4d3a-AAFD-47FA3B1CAC77}",
			  0x7d158bfe, 0xe39, 0x4d3a, 0xaa, 0xfd, 0x47, 0xfa, 0x3b, 0x1c, 0xac, 0x77);


//////////////////////////////////////////////////////////////////////////

enum 
{
	FNF_CROSS_SECTION_REF = 1,			// 该函数存在跨节引用
	FNF_IN_DATA_SECTION = 2,			// 该函数存在于非执行节中
	FNF_IN_WRITABLE_SECTION = 4,		// 该函数存在于可写节中

	FNF_END_BY_RET = 8,
	FNF_END_BY_JMP = 16,
	FNF_END_BY_32INS = 32,
};

struct FNHASH
{
	UINT8	Bin[10];
};

struct FND
{
	UINT64	_VA;
	UINT64	_RVA;
	UINT64	_RAW;
	SIZE_T	_Len;						// 
	UINT64	_Hash;						// 该函数的HASH值
	UINT64	_Ref1;						// 记录第一个调用点
	SIZE_T	_RefNum;						
	SIZE_T	_Flags;
	SIZE_T	_InsCount;
	//FND():RVA(0), RAW(0), Len(0), Hash(0), Ref1(0), RefNum(0), Flags(0), InsCount(0) {};
	FND(){ memset(this, 0, sizeof(*this)); };

};

struct FUN_FETCH_INFO
{
	SIZE_T	FunNum;
	SIZE_T	FunCodeSize;
	SIZE_T	FunCandEntries;
};

struct BB_INTRUC
{
	UINT8 l;		// 16 MAX
	UINT8 b[16];
};

FORCEINLINE bool operator < ( const BB_INTRUC & left, const BB_INTRUC & right )
{
	if( left.l < right.l ) return true;
	if( left.l > right.l ) return false;
	return memcmp( left.b, right.b, left.l ) < 0;
}

struct IC_PAIR
{
	BB_INTRUC	Instrc;
	SIZE_T		Times;
};

typedef struct INSTRUC_HISGRAM
{
	SIZE_T		Length;
	IC_PAIR*	Pairs;
} INSTRUC_HISGRAM, *LPINSTRUC_HISGRAM;

typedef struct DENSE_INS_DESC
{
	UINT32	RVA;
	UINT16	Opcode;
	UINT8	ILen;
	UINT8	IRMLen;
} X86DID, *LPX86DID;

typedef struct DENSE_INS_DESC_EX : DENSE_INS_DESC
{
	hde64s	HdeIns;
} X86DIDEX, *LPX86DIDEX;

enum GOLMODE
{
	GOL_JMP				= 1,		// 需要进入JMP,所有可以跳到本映像内的CALL
	GOL_CALL			= 2,		// 需要进入CALL,所有可以跳到本映像内的CALL
	GOL_JCC_T			= 4,		// 需要走条件跳的成功分支（跳转）
	GOL_BRACH_FIRST		= 16,		// 需要走条件跳的成功分支（跳转）,并且优先于不跳转的流程处理
	GOL_RVA_ONCE		= 32,		// 同一个RVA地址只处理一次
	GOL_ZERO_BREAK		= 64,		// 遇到OPCODE为0就不在继续
	GOL_X86DID_EX		= 128,
};

typedef SIZE_T		GOLMODEMASK;

enum STRACEC
{
	STRACE_TRACED,
	STRACE_NEW_BRANCH,
};

class IREPECodeParser32 : public IUnknown
{
public:
	struct IFetchInstructionsCallback
	{
		STDMETHOD(PushInstruction)( LPX86DID lpX86Did ) PURE;
		STDMETHOD(PushControl)( STRACEC cc ) PURE;
	};
public:
	RE_DECLARE_IID;
	STDMETHOD( GetSymbolByIAT )( SIZE_T uIAT, LPPIS32 pSymbol, BOOL bIsVA ) PURE;
	STDMETHOD( GetFunctionByHash )( UINT64 uHash, FND* lpFND = NULL ) PURE;
	STDMETHOD( GetNextFunction)( UINT64 uRAW, FND* lpFND ) PURE;
	STDMETHOD( GetFunctionInfo )( FUN_FETCH_INFO * lpInfo ) PURE;
	STDMETHOD( GetInstructionHisgram )( LPINSTRUC_HISGRAM lpHisgram ) PURE;
	STDMETHOD( FetchInstructionList )( UINT32 uRVA, GOLMODEMASK uModes, SIZE_T cMaxCount, RX_BLOB& rDidArr ) PURE;
	STDMETHOD( FetchInstructions )( UINT32 uRVA, GOLMODEMASK uModes, LPX86DID lpDids, SIZE_T& cCount ) PURE;
	STDMETHOD( FetchInstructionsEx )( UINT32 uRVA, GOLMODEMASK uModes, IFetchInstructionsCallback* pcb ) PURE;
};

// {2CBD15FE-F232-4ad0-88C1-410D7312781B}
RE_DEFINE_IID( IREPECodeParser32, "{2CBD15FE-F232-4ad0-88C1-410D7312781B}",
			  0x2cbd15fe, 0xf232, 0x4ad0, 0x88, 0xc1, 0x41, 0xd, 0x73, 0x12, 0x78, 0x1b);
//////////////////////////////////////////////////////////////////////////

enum PE_TRUNK_CLS 
{
	PE_TRUNK_STRA	= 2,
	PE_TRUNK_STRW	= 4,

	PE_TRUNK_API_E = 0x10,
	PE_TRUNK_API_X = 0x20,
	PE_TRUNK_API_D = 0x40,

	PE_TRUNK_APIs	= PE_TRUNK_API_E|PE_TRUNK_API_X|PE_TRUNK_API_D,
	PE_TRUNK_ALL	= PE_TRUNK_APIs|PE_TRUNK_STRA|PE_TRUNK_STRW,
};

enum
{
	CALL_CALL = 1,
	CALL_JMP = 2,
};

struct PE_TRUNK_ITEM
{
	PE_TRUNK_CLS		kls;
	SIZE_T				uniq;
	UINT64				fptr;
	LPCSTR				txt;
	LPCSTR				txt2;
	SIZE_T				traits;
};

class IPETrunkFetcher : public IUnknown
{
public:
	STDMETHOD(Open)( IUnknown * archp ) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Fetch)( SIZE_T klsmask ) PURE;
	STDMETHOD_(LPCSTR,GetAsText)() PURE;
	STDMETHOD_(SIZE_T,GetCount)() PURE;
	STDMETHOD(GetItem)( SIZE_T idx, PE_TRUNK_ITEM * lpItem ) PURE;
	STDMETHOD(GetEntrySectionProcessedData)( SIZE_T * lpFilePos, SIZE_T * lpBytes, SIZE_T * lpSecSize = NULL ) PURE;
	RE_DECLARE_IID;
};
// {C2071198-B8AB-4050-AB60-E0852A38AA50}
RE_DEFINE_IID(IPETrunkFetcher, "{C2071198-B8AB-4050-AB60-E0852A38AA50}",
			0xc2071198, 0xb8ab, 0x4050, 0xab, 0x60, 0xe0, 0x85, 0x2a, 0x38, 0xaa, 0x50);
//////////////////////////////////////////////////////////////////////////

#endif // #ifndef _RE_EXPORT_INTERFACE_PE_ARCHIVE_H_
