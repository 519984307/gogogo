#ifndef __SIMPLE_SIGNATURE__
#define __SIMPLE_SIGNATURE__

#include "filefmt.h"

typedef UINT64	SIG64;

enum SIG64_CLASS
{
	SIG64_ANY_FCRC							=0x00,

	SIG64_PE_TJ11							=0x01,
	SIG64_CLOUD_SIG64						=0x01,
	SIG64_PE_HDR							=0x02,
	SIG64_PE_EPSEC							=0x03,
	SIG64_PE_RELOC							=0x04,	//
	SIG64_PE_RCDATN							=0x05,	// 所有RC-DATA的资源名
	SIG64_PE_IMPORTS						=0x06,
	SIG64_PE_EXPORTS						=0x07,
	SIG64_PE_TJ11_FWD						=0x08,
	SIG64_PE_SENSPOINT						=0x09,	//
	SIG64_PE_IMPORTS_API					=0x0A,

	SIG64_DOTNET_COMPRESSED_STREAM			=0x11,	// #~流
	SIG64_DOTNET_STRINGS_STREAM				=0x12,	// String流
	SIG64_DOTNET_BLOB_STREAM				=0x13,	// BLOB流
	SIG64_DOTNET_GUID_STREAM				=0x14,	// GUID流
	SIG64_DOTNET_US_STREAM					=0x15,	// US流

	SIG64_DEX_HEADER						=0x21,	//
	SIG64_DEX_STRING						=0x22,	//
	SIG64_DEX_FIDLD							=0x23,	//
	SIG64_DEX_METHOD						=0x24,	//
	SIG64_DEX_TYPES							=0x25,	//

	SIG64_SFX_FILE_NAMES					=0x31,	// 包内所有文件名


	SIG64_TFE_BASE							=0x40,
	SIG64_TFE_LAST							=0xA0,	// 6*16

	SIG64_SIM_BASE							=0xA0,
	SIG64_SIM_NSI							=0xA1,	// NSI Script

	SIG64_SIM_LAST							=0xC0,	// 2*16
	SIG64_CLASS_8_MAX						=0x100,
};



//////////////////////////////////////////////////////////////////////////
enum TFE_HASH_CLASS
{
	TFE_PE_ARCHITECH						=0x01,
	TFE_PE_ENTRY_INS8,
	TFE_PE_ENTRY_INS16,
	TFE_PE_ENTRY_INS32,
	TFE_PE_FUN_IMPORTTABLE,
	TFE_PE_FUN_EXPORTTABLE,
	TFE_PE_DIR_IMPORTTABLE,
	TFE_PE_DIR_EXPORTTABLE,
	TFE_PE_DIR_SIGN,
	TFE_PE_DIR_RESOURCE,
	TFE_PE_OVERLAY_DATA,
	TFE_DOTNET_STRINGTABLE,
	TFE_DOTNET_IMPMAP,
	TFE_PE_FUN_IMPORTTABLE_APINAME,
	TFE_PE_ENTRY_DATABLOCK,
	TFE_HASH_ALL_HASH,
	TFE_MAX_COUNT,

	TFE_ELF_ARCHITECH = 20,
	TFE_ELF_ENTRY_INS20,
	TFE_ELF_ENTRY_INS32,
	TFE_ELF_ENTRY_INS48,
	TFE_ELF_ENTRY_INS64,
	TFE_ELF_RAW_RODATA,
	TFE_ELF_RAW_STRTAB,
	TFE_ELF_RAW_DYNSTR,
	TFE_ELF_SYMSTR_OBJ_FUNC,
	TFE_ELF_SYMSTR_FILE_FUNC,
	TFE_ELF_SYMSTR_FUNC,
	TFE_ELF_SYMSTR_ALL,
	TFE_ELF_DYNSTR_OBJ_FUNC,
	TFE_ELF_DYNSTR_FILE_FUNC,
	TFE_ELF_DYNSTR_FUNC,
	TFE_ELF_DYNSTR_ALL,
	TFE_ELF_RODATA_ASC_ALL,
	TFE_ELF_RODATA_ASC_4GRAM,
	TFE_ELF_RODATA_ASC_6GRAM,
	TFE_ELF_RODATA_ASC_12GRAM,
	TFE_ELF_MAX_COUNT
};
//////////////////////////////////////////////////////////////////////////

#pragma pack(1)
typedef struct _LONG_SIG64
{
	UINT8	Type;
	FFMTID	Format;
	UINT64	Hash;
} __attribute__((packed)) SIG64EX, *LPSIG64EX;
#pragma pack()

struct ISig64ListW
{
	STDMETHOD(PushSig64)( LPSIG64EX lpHash ) PURE;
};

class IRXSig64List : public IUnknown, public ISig64ListW
{
public:
	STDMETHOD_(SIZE_T,Count)() PURE;
	STDMETHOD(GetByIndex)( SIZE_T uIndex, LPSIG64EX lpHash ) PURE;
	STDMETHOD(GetByClass)( SIZE_T uClass, LPSIG64EX lpHash ) PURE;
public:
	RE_DECLARE_IID;
};

// {A37DA0A7-39AD-4143-9AA1-D02056F57A6C}
RE_DEFINE_IID( IRXSig64List, "{A37DA0A7-39AD-4143-9AA1-D02056F57A6C}",
			  0xa37da0a7, 0x39ad, 0x4143, 0x9a, 0xa1, 0xd0, 0x20, 0x56, 0xf5, 0x7a, 0x6c);


//////////////////////////////////
class IRXArchiveHashes : public IUnknown
{
public:
	STDMETHOD(Fetch)( IRXSig64List * plist ) PURE;
	STDMETHOD(FetchEx)( SIZE_T uHashId, IRXSig64List * plist ) PURE;
	STDMETHOD(FetchSimHash)( IRXSig64List * plist ) PURE;
	STDMETHOD(FetchSimHashEx)( SIZE_T uHashId, IRXSig64List * plist ) PURE;
	RE_DECLARE_IID;
};
// {D1EF0924-3554-4A9A-9DD2-1923D241D692}
RE_DEFINE_IID( IRXArchiveHashes, "{D1EF0924-3554-4A9A-9DD2-1923D241D692}",
			  0xd1ef0924, 0x3554, 0x4a9a, 0x9d, 0xd2, 0x19, 0x23, 0xd2, 0x41, 0xd6, 0x92);


//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI sig64FetchA( IRXArchive * parc, IRXSig64List* pList );
HRESULT WINAPI sig64FetchS( IREStream * pstm, IRXSig64List* pList );
HRESULT WINAPI sig64FetchF( IREStream * pstm, IRXSig64List* pList );
HRESULT WINAPI FetchPEHash(IRXArchive * parc);
//////////////////////////////////////////////////////////////////////////

//typedef struct CSJR
//{
//	UINT64	uUniqID;
//};
//
//class IRxCustomSig64Judger : public IUnknown
//{
//public:
//	STDMETHOD() PURE;
//};
//////////////////////////////////////////////////////////////////////////

#endif
