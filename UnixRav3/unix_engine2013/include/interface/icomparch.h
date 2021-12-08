#ifndef _RX_EXPORT_MASS_ARCHIVE_H_
#define _RX_EXPORT_MASS_ARCHIVE_H_

//////////////////////////////////////////////////////////////////////////

class IRXDexArchive : public IRXArchive
{
public:
    struct HEADER_INFO
    {
        SIZE_T	file_size;
        SIZE_T  header_size;
        SIZE_T  string_ids_size;
        SIZE_T  type_ids_size;
        SIZE_T  proto_ids_size;
        SIZE_T  field_ids_size;
        SIZE_T  method_ids_size;
        SIZE_T  class_defs_size;
        SIZE_T  data_size;
    };

    enum DEXSTRATTR {
		STR_USER                = 1,
		STR_PROP_PARAMETERS     = 2,
		STR_PROP_RETURN_TYPE    = 4,
		STR_TYPE                = 8,
		STR_METHOD              = 16, 
    };

    typedef struct tagSTRING_INFO
    {
        LPCSTR      lpdexstring;
        UINT32      str_offset;
        SIZE_T      str_length;
        DEXSTRATTR  str_attrib;
    } STRING_INFO;

    typedef struct tagMETHOD_INFO
    {
        LPCSTR      lpfunname;
        UINT32      code_offset;
        SIZE_T      code_size_byte;
        UINT32      access_flags;
    } METHOD_INFO;

    STDMETHOD(QueryInfo)( HEADER_INFO * lpInfo ) PURE;
    STDMETHOD(DeepParse)() PURE;
    
    STDMETHOD(GetStringCount)( SIZE_T &c ) PURE;
    STDMETHOD(GetString)( SIZE_T uIndex, STRING_INFO* lpStringInfo ) PURE;
    
    STDMETHOD(GetMethodCount)( SIZE_T &c ) PURE;
    STDMETHOD(GetMethod)( SIZE_T uIndex, METHOD_INFO* lpMethodInfo ) PURE;
    
    STDMETHOD(FindMethod)( LPCSTR lpName, METHOD_INFO* lpMethodInfo ) PURE;

    RE_DECLARE_IID;
};

// {5C8911F2-9C29-41c5-B29B-E148205B2F07}
RE_DEFINE_IID( IRXDexArchive, "{5C8911F2-9C29-41c5-B29B-E148205B2F07}",
              0x5c8911f2, 0x9c29, 0x41c5, 0xb2, 0x9b, 0xe1, 0x48, 0x20, 0x5b, 0x2f, 0x7);




//////////////////////////////////////////////////////////////////////////

class IRXMassArchive : public IUnknown
{
public:
	RE_DECLARE_IID;
	enum
	{
		FileAttributes	, // DWORD
		CreationTime	, // FILETIME	
		LastAccessTime	, // FILETIME	
		LastWriteTime	, // FILETIME	
		FileSize		, // UINT64	
		FileName		, // LPCWSTR
	};
	enum 
	{
		CA_HAS_PASSWORD,
		CA_READ_ONLY,
		CA_MODIFIED = 0x80000000,
	};
	enum 
	{
		EF_FOR_TREAT = 1,
		EF_INTERNAL_ONLY = 2,
		EF_WANT_INTERNAL = 2,
	};
	typedef union _TPROP
	{
		UINT32		Attributes;
		UINT64		SizeOfFile;
		FILETIME	TimeCreate;
		FILETIME	TimeLastAccess;
		FILETIME	TimeLastWrite;
		FILETIME	TimeAny;
		XSTRP		NameOfFile;
	} TPROP;
	enum { E_ENUMNOMORE = ENUM_E_LAST };
	STDMETHOD_(UINT32, GetAttribute)() PURE;
	STDMETHOD(GetItemCount)( DWORD * pItemCount ) PURE;
	STDMETHOD(GetItemProperty)( DWORD dwIndex, DWORD dwPropId, TPROP* lpProp ) PURE;
	STDMETHOD(ExtractItem)( DWORD dwIndex, IRXStream** ppstm, DWORD dwExtractFlags = 0 ) PURE;
	STDMETHOD(BuildAll)() PURE;
	
};
RE_DEFINE_IID(IRXMassArchive, "{58422548-C016-4e37-904B-410795DF18A7}",
			0x58422548, 0xc016, 0x4e37, 0x90, 0x4b, 0x41, 0x7, 0x95, 0xdf, 0x18, 0xa7);

//////////////////////////////////////////////////////////////////////////
class IRXMassArchive2 : public IUnknown
{
public:
	STDMETHOD(GetInternalArchive)( IUnknown ** ppunk ) PURE;
	RE_DECLARE_IID;
};
// {102ECDA0-010A-47A5-87B7-7F8B38C315F8}
RE_DEFINE_IID(IRXMassArchive2, "{102ECDA0-010A-47A5-87B7-7F8B38C315F8}",
			0x102ecda0, 0x10a, 0x47a5, 0x87, 0xb7, 0x7f, 0x8b, 0x38, 0xc3, 0x15, 0xf8);

////////////////////////////////////////////////////////////////////////////

DEFINE_GUID(PID_SFX_FFRType,				0x9f64ee1b, 0xa387, 0x4e93, 0x8b, 0xb2, 0x9f, 0xdc, 0xef, 0x60, 0xaa, 0xa3);
DEFINE_GUID(PID_SFX_BeginPos,				0xf1c7bd58, 0xbaf3, 0x4e35, 0x88, 0x94, 0xdf, 0xbc, 0xbc, 0xc0, 0x97, 0x53);
DEFINE_GUID(PID_SFX_ArchiveSize, 			0xd3026d0f, 0xf28c, 0x4a01, 0x87, 0xc4, 0xe6, 0xa9, 0xe3, 0xe, 0x2e, 0x7b);

//////////////////////////////////////////////////////////////////////////

DEFINE_GUID(PID_SFX_AssistantVA, 			0x1c7dffb9, 0x9c3b, 0x4799, 0x88, 0x24, 0x61, 0xd7, 0xdb, 0x2c, 0xf7, 0xa1);
DEFINE_GUID(PID_SFX_SearchMark, 			0xc790beb2, 0xcdfd, 0x4a6b, 0xa3, 0x60, 0x9c, 0x68, 0x7b, 0x3e, 0xe2, 0xf8);
DEFINE_GUID(PID_SFX_SfxData,				0x4c3c5165, 0x8d45, 0x44a3, 0x87, 0x15, 0x3f, 0x36, 0xf9, 0xe1, 0x01, 0x18);
DEFINE_GUID(PID_SFX_SfxProp, 				0xbe80a65b, 0x5638, 0x4f4a, 0xbe, 0xa7, 0xdc, 0xcc, 0x71, 0x6a, 0xaa, 0xd8);
DEFINE_GUID(PID_SFX_MethodProp, 			0x000d94d7, 0xcec9, 0x45f9, 0x84, 0xaf, 0x2d, 0x07, 0x58, 0x13, 0x94, 0x23);

////////////////////////////////////////////////////////////////
// file system property
// VT_UNKNOWN
DEFINE_GUID(PID_FileSystem_VolumeHistory,	0x29d9199f, 0x5fb, 0x44cb, 0xbe, 0x8e, 0x4e, 0x4, 0xa8, 0xd0, 0xec, 0x10);

#endif

