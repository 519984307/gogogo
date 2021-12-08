#ifndef __IRE_EXTRACT_FEATURE__
#define __IRE_EXTRACT_FEATURE__

enum	// BYTE
{
	FCAT_STRX,				// 字符串(MBS)
	FCAT_FUNC,				// 函数
	FCAT_RESD,				// 资源数据
	FCAT_RESP,				// 资源路径
	FCAT_BB512,				// 函数二进制块 00:EP, 01:EXP, 02: TLS, 03: Section
	FCAT_IEMP,				// 导入导出信息
	FCAT_FACE,				// PE结构信息
	FCAT_PACKER,			// 壳信息, 00:PackerName, 01:PackerDepth
	FCAT_EPOPL,				// 入口的OPLIST		08,InstructCount(1),OPLIST-CRC64, XTZZZZZZZZ
	FCAT_ETOPL,				// Export And TLS 
	FCAT_MAX,
};

//////////////////////////////////////////////////////////////////////////
//
// FCAT_BB512
//

enum
{
	BB512_EP,
	BB512_EXP,
	BB512_TLS,
	BB512_SECT_HDR,
	BB512_SECT_TAIL,
	BB512_EP_UP,
	BB512_OVERLAY,
};

enum
{
	FT_EXPLIB,
	FT_EXPFUN,
	FT_IMPAPI,
};

enum
{
	CE_ENTRY,
	CE_EXPORT,
	CE_TLS,
};

enum
{
	PK_COUNT,
	PK_NAMELIST,
};

//////////////////////////////////////////////////////////////////////////
//
// FCAT_FACE
//
enum
{
	F_IMAGE_SIZE,				// ImageSize/0x20
	F_FILE_SIZE,				// FileSize/0x20
	F_RAW_SIZE,					// EntryPoint/0x20
	F_IMG_RAW_RATIO,			// ImageSize/RawSize
	F_CODE_SIZE,				// EntryPoint/0x20
	F_INIT_DATA_SIZE,			// EntryPoint/0x20
	F_ENTRY_POINT,				// EntryPoint/0x20
	F_EP_POSITION,				// (EntryPoint/EntrySectionSize)
	F_EPSEC_RIDX,				//
	F_EPSEC_ATTRIB,				//
	F_WRITABLE_SECNUM,			//
	F_SECTION_NUM,				//
	F_IMPORT_FUN_COUNT,			//
	F_IMPORT_LIB_COUNT,			//
	F_RESOURCE_COUNT,			//
	F_EXPORT_COUNT,				//
	F_SECTION_NAME,
	F_SECTION_RAW_SIZE,
	F_SECTION_IMG_SIZE,
	F_SECTION_IMG_RAW_RATIO,
	F_SECTION_ATTRIBUTES,
	F_OVERLY_SIZE,
};

enum
{
	RVA_ALIGN = 0x20,
};


typedef UINT8		FCAT;
typedef SIZE_T		FCATT;

// MAX is sizeof(MD5)
enum { CHECKSUM_MAX = 16 };

#pragma pack(1)
typedef struct _KFEATURE
{
	FCAT	FeatCat;
	ULONG	RawOffset;								// Position in File
	ULONG	MemOffset;								// Position in Image
	ULONG	FeatureLen;								// Length of feature
	ULONG	StuffLen;								// Length of stuf data
	LPCSTR	TextShow;								// 特征的文本展示
	UINT8	CheckSumUsed;							// <= CHECKSUM_MAX+1
	UINT8	CheckSum[sizeof(FCAT)+CHECKSUM_MAX];	// 特征校验和（变长）
} RS_PACK_ONEBYTE KFEATURE, *LPKFEATURE;
#pragma pack()

enum EF_CTRL
{
	EF_CONTINUE,
	EF_BREAK_THIS,
	EF_BREAK_ALL,
};

#define RCONTINUE(x)	{ do { EF_CTRL ec = (x); if( EF_CONTINUE != ec ) return ec; } while(0); }
#define IS_CONTINUE(x)	((x)==EF_CONTINUE)
class IREFeatureExtractorCallback
{
public:
	// TRUE, Continue;
	// FALSE, Break;
	STDMETHOD_( EF_CTRL, OnFeatureExtract )( IREArchive* lpArchive, LPKFEATURE lpFeature, LPCSTR lpcFCat = 0 ) = 0;
	// TRUE, Continue;
	// FALSE, Break;
	STDMETHOD_( EF_CTRL, OnBeginExtract )( IREArchive* lpArchive, FCATT fCat ) = 0;
	STDMETHOD_( VOID, OnEndExtract )( IREArchive* lpArchive, FCATT fCat ) = 0; 
};

#define IFeatureExtractCallback	IREFeatureExtractorCallback

// {7F75E130-61AB-4c2e-A782-E535571CE83E}
DEFINE_GUID( EXT_PID_File_PackTimes, 
			0x7f75e130, 0x61ab, 0x4c2e, 0xa7, 0x82, 0xe5, 0x35, 0x57, 0x1c, 0xe8, 0x3e);

// {E61C6774-ADDD-40a0-97F9-6748C533106A}
DEFINE_GUID( EXT_PID_File_PackerNames, 
			0xe61c6774, 0xaddd, 0x40a0, 0x97, 0xf9, 0x67, 0x48, 0xc5, 0x33, 0x10, 0x6a);

struct EXTRACT_WHAT
{
	UINT32		Items;
	const FCAT*	FeatCat;
};

class IREFeatureExtractor : public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD( Extract )( IREArchive* lpArchive, IFeatureExtractCallback * lpCallback,  EXTRACT_WHAT* lpWhat, LPCVOID * lpcArgs  ) = 0;
	STDMETHOD( SetSleepTime )( DWORD dwTime ) = 0;
	STDMETHOD( EnableMakeTextShow )( BOOL bMake ) = 0;
};

// {5D1D9EFA-B884-46f8-9EBD-CC4F5021E4D5}
RE_DEFINE_IID( IREFeatureExtractor, "{5D1D9EFA-B884-46f8-9EBD-CC4F5021E4D5}",
			0x5d1d9efa, 0xb884, 0x46f8, 0x9e, 0xbd, 0xcc, 0x4f, 0x50, 0x21, 0xe4, 0xd5);

#define IFeatureExtractor IREFeatureExtractor

// {72979918-3691-4878-BE23-5B424429CB3F}
DEFINE_GUID( CLSID_CREFeatureExtractorV1, 0x72979918, 0x3691, 0x4878, 0xbe, 0x23, 0x5b, 0x42, 0x44, 0x29, 0xcb, 0x3f);

#define CLSID_CFeatureExtractorV1 CLSID_CREFeatureExtractorV1


#endif