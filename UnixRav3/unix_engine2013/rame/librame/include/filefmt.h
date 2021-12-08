#ifndef __RX_FILE_FORMAT__
#define __RX_FILE_FORMAT__


typedef UINT64  FMTATTR;
typedef UINT32  FFMTID;


typedef struct _FORMAT_DETAIL
{
	SIZE_T		StruSize;				 // Fill First 
	FFMTID		FormatId;
	LPCSTR		ShortName;
	LPCSTR		LongDesc;
	FMTATTR		FormatClass;		   // UINT64
} FORMAT_DETAIL;



enum FFMTPROP
{
	FMTP_START	= 0,		// UINT64
	FMTP_LENGTH	= 1,		// UINT64
	FMTP_SUBTYPE = 2,		// UINT32 /sub-format type, WORD/EXCEL/PPT/VISIO
	FMTP_XVALUE  = 3,
	FMTP_MAX = 0xFF,
};

class IREFormats
{
public:
	STDMETHOD_( SIZE_T, Count )() PURE;
	STDMETHOD( Append )( FFMTID aFmtId ) PURE;
	STDMETHOD( Replace )( FFMTID aFmtIdOld, FFMTID aFmtIdNew, BOOL bInsertIfNotExist ) PURE;
	STDMETHOD( Remove )( FFMTID aFmtId ) PURE;
	STDMETHOD( Clear )() PURE;
	STDMETHOD( SetProp )( FFMTID aFmtId, FFMTPROP uPropId, xv::Value& rValue ) PURE; 
	STDMETHOD( GetProp )( FFMTID aFmtId, FFMTPROP uPropId, xv::Value& rValue) PURE; 
	STDMETHOD( DelProp )( FFMTID aFmtId, FFMTPROP uPropId ) PURE; 
	STDMETHOD( GetAt )( SIZE_T dwIndex, FFMTID& rFormat ) PURE;
	STDMETHOD( RemoveAt )( SIZE_T dwIndex ) PURE;
	STDMETHOD( Find )( FFMTID aFormat, SIZE_T * lpIndex = NULL ) PURE;
	STDMETHOD( Insert )( SIZE_T dwIndex, FFMTID rFormat ) PURE;
	STDMETHOD( Sort )() PURE;
};

class IREFormatList : public IUnknown, public IREFormats
{
public:
	RE_DECLARE_IID;
};

// {CCD5D887-D15E-417b-A3BF-EBCF27E01560}
RE_DEFINE_IID( IREFormatList, "{CCD5D887-D15E-417b-A3BF-EBCF27E01560}",
				0xccd5d887, 0xd15e, 0x417b, 0xa3, 0xbf, 0xeb, 0xcf, 0x27, 0xe0, 0x15, 0x60);

namespace FFR
{
	enum FCLS			 // It's MASK
	{
		FFLG_HAS_DEEP   = 0x00000001,
		FFLG_IGNORE		= 0x00000002,	// 不用查的格式
		FCLS_DATA		= 0x00000004,	// 任意的数据文件
		//FFLG_ADD_NORMAL	= 0x00000008,	// 需要增加NORMAL格式的格式

		FCLS_PROGRAM	= 0x00000010,	// DOSCOM/PE//ELF/JAVA
		FCLS_DOCUMENT   = 0x00000020,	// PDF/DOC/DOCX/rtf
		FCLS_PACKAGE	= 0x00000040,	// Zip/7z/...
		FCLS_MMEDIA		= 0x00000080,	// Image|Moive|Audio
		FCLS_SCRIPT		= 0x00000100,	// All script
		FCLS_EMAIL		= 0x00000200,	// E-Mail
		FCLS_DISKIMG	= 0x00000400,	// Disk in file, VHD, VMDK, ...
		FCLS_BOOT		= 0x00000800,	// Virtual Disk

		//////////////////////////////////////////////////////////////////////////
		FCLS_L_IMPORTANT= 0x00100000, 
		//////////////////////////////////////////////////////////////////////////
		// 0xFF
		FCLS_P_ADR		= 0x01000000,	// Android
		FCLS_P_SYB		= 0x02000000,	// Sybian
		FCLS_P_IOS		= 0x04000000,	// IOS
		FCLS_P_MACOS	= 0x08000000,	// MACOS
		//////////////////////////////////////////////////////////////////////////
		FCLS_P_NIX		= 0x10000000,	// Posix(Linux/xxx)
		FCLS_P_WIN		= 0x20000000,	// All Windows， include <Windows Phone>
		FCLS_P_DOS		= 0x40000000,	// DOS
		FCLS_P_OTHER	= 0x80000000,	// Other
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		FCLS_SFX		= FCLS_PACKAGE|FCLS_PROGRAM,
		FCLS_OS_MOB		= FCLS_P_ADR|FCLS_P_SYB|FCLS_P_IOS,
		//FCLS_OS_ANY		= 0xFF000000,	// 
	};

	enum FMT
	{
		FMT_UNKNOWN				= 0x0000,
		FMT_NORMAL				= FMT_UNKNOWN,
		// 可识别的格式
		FMT_BOOT				,
		FMT_MZ					,
		FMT_PE					,
		FMT_DEX					,
		//////////////////////////////////////////////////////////////////////////
		FMT_ZIP					,
		FMT_CAB					,
		FMT_ARJ					,
		FMT_RAR					,
		FMT_ARC					,
		FMT_ZOO					,
		FMT_LZH					,
		FMT_TAR					,
		FMT_GZIP				,
		FMT_UUE					,
		FMT_ACE					,
		FMT_BZ2					,
		FMT_CHM					,
		FMT_7Z					,
		FMT_SIS					,
		FMT_DOC					,
		FMT_MDB					,
		//////////////////////////////////////////////////////////////////////////
		FMT_OUTLOOKEX_V5		,
		FMT_OUTLOOKEX_V4		,
		FMT_FOXMAIL_V3			,
		FMT_NETSCAPE_V4			,
		FMT_MAILBODY			,
		FMT_NWS                 ,
		FMT_MHT                 ,
		FMT_MSG                 ,
		FMT_BASE64              ,
		FMT_UCS16LE				,
		//////////////////////////////////////////////////////////////////////////
		FMT_HTA					,
		FMT_REG					,
		FMT_VBSENC				,
		FMT_RSBIN				,
		FMT_JPEG				,
		FMT_SWF					,
		FMT_SWC					,
		FMT_PDF					,
		FMT_MP3					,//应该不是MP3格式呀
		FMT_LNK					,
		FMT_TTF					,
		FMT_ELF					,
		FMT_CLASS				,
		FMT_BMP					,
		FMT_GIF					,
		FMT_PNG					,
		FMT_ICO					,
		FMT_CDR					,
		FMT_MIDI				,
		FMT_MPEG				,
		FMT_HLP					,
		FMT_WPG					,
		FMT_CPT					,
		FMT_TEX					,
		FMT_CNT					,
		FMT_ISU					,
		FMT_WBM					,
		FMT_PNF					,
		FMT_APM					,
		FMT_WMF					,	//placeable forms of WMF metafile
		FMT_ICM					,
		FMT_MDF					,
		FMT_RDM					,
		FMT_JPG					,
		FMT_APP					,
		FMT_CAT					,
		FMT_RSFS				,
		FMT_MBX					,
		FMT_WAV					,
		FMT_WDL					,
		FMT_MPQ					,
		FMT_ECF					,
		FMT_BDR					,
		FMT_DBF					,
		FMT_REALPLER			,
		FMT_RPM					,
		FMT_IDB					,
		FMT_PDB					,
		FMT_BOX					,
		FMT_IND					,
									//
		FMT_APK					,	// Android DEX
		FMT_XOFFICE				,	// Office 2003 +, zipped office	FMT_XOFFICE

		FMT_JAR					,

        // SFX
        FMT_RAREXE              ,
        FMT_ACEEXE              ,
        FMT_ZIPEXE				,
        FMT_7ZEXE				,
        FMT_CABEXE				,
        FMT_SEAEXE              ,
        FMT_ARJEXE				,
        FMT_ARCEXE				,
        FMT_ZOOEXE				,
        FMT_LZHEXE				,
        FMT_ZIPNE				,
        FMT_ZIPPE				,
        FMT_CABSETEXE			, 
        FMT_WISE				,
        FMT_WISEA			    ,
        FMT_ELANG				,
        FMT_GENTEE				,
        FMT_PAQUET				,
        FMT_SFACTORY			,
        FMT_AUTOIT              ,
        FMT_INNO                 ,
        FMT_INSTYLER            ,
        FMT_NSIS                ,
        FMT_QUICKBATCH          ,
        FMT_LOTUSPIC            ,

		FMT_AU3SCRIPT			,

		FMT_DOSCOM				,	
		///////////////////////////
		FMT_VBA					,
        FMT_TEXT				,
        FMT_VBS					,
        FMT_HTML				,
        FMT_BAT					,
        FMT_JS					,
		FMT_NS					,				// Nullsoft Scirpt

		FMT_SHELL				,
		FMT_PERL				,
		FMT_EML					,
		FMT_IRC					,

		FMT_JSENC				,
		///////////////////////////
		// VirutalDisk
		//
		FMT_VHD					,
		FMT_VMDK				,

		//////////////////////////////////////////////////////////////////////////
		FMT_NTFS				,
		FMT_FAT					,

		FMT_RTF,
		FMT_PST,
		// 新添加的都加后面
		FMT_VBVDI				,

		FMT_SQLITE				,
		FMT_CDFMS				,

		FMT_LIBA				,

		FMT_TIFF				,

		FMT_FAS					,
		FMT_LSP					,

		FMT_XDP				,		// XML-Base PDF
		////////////scanex
		FMT_EOT,
		FMT_ATL,
		FMT_ASF,
		FMT_RIFF,
		FMT_QTFF,
		FMT_TORRENT,			//种子文件
		FMT_MP3_2,				//新增的MP3格式
		FMT_WMF_ORIGINAL,		//original WMF metafile

		FMT_SCT,
		//FMT_VBE, FMT_VBS_ENC
		FMT_JSE__, //same to FMT_JSENC
		FMT_PIF,
		FMT_PLG,
		FMT_COM,
		FMT_WSH,
		FMT_ANI,
		FMT_INI,
		FMT_LIM,
		FMT_HTM,
		FMT_IDS,
		FMT_CSS,
		FMT_PPT,
		FMT_HTT,
		FMT_TXT,
		FMT_MAP,
		FMT_PHP,

		FMT_NOT_SCAN, // c:/windows/Prefetch

		FMT_FLV,

		FMT_MACH_O,

		FMT_BASE64_PE,

		//////// compress data file ///////////////
		FMT_COMPRESS_LZMA,

		/////////////////

		FMT_IPA,

		FMT_SMARTINSTALLER,
		
		FMT_MSIL,
		FMT_MSO,     //ActiveMine

		FMT_MSCFB,

		FMT_PS,
		FMT_EPS,
		FMT_WSF,
		FMT_NSI_UNUSED,
		FMT_INF,
		FMT_FLSM,
		FMT_RAR5,
		FMT_RAR5EXE,
		FMT_XZ,
		FMT_ZLIBB,
		FMT_XBAT,		//无法识别的脚本的统称
		FMT_ODF,
		FMT_OXML,
		FMT_ISO,
		FMT_RMEMB,
		FMT_DEB,
		FMT_CPIO,
		FMT_CRAMFS,
		FMT_SQL,
		FMT_SYLK,
		FMT_XDMP,
		FMT_PYPEPACKAGE,
		FMT_PYZ,
		FMT_CMDL,
		FMT_HWP,
		FMT_CRX,
		FMT_OFD,
		FMT_PYC,
		FMT_FLUENT,
		FMT_AIXCOFF,
		FMT_OXML03,
		FMT_ENCPS,
		FMT_ODTT,
		FMT_IPCH,
		FMT_PCAP,
		FMT_BMV,
		FMT_AHKARCH,
		FMT_AHKSCRIPT,
		FMT_XLSX,
		FMT_PPTX,
		FMT_DOCX,
		FMT_DEEPTEXT,   //需要深度处理
		FMT_XCODE,		// 无法识别出具体格式，但具有代码特性
		FMT_URLLNK,
		FMT_WAUX,
		FMT_PRI,
		FMT_PCM,
		FMT_WCAT,
		FMT_MAX_COUNT		   
	};
};


HRESULT WINAPI fmtrec_s( IREStream* lpStream, IREFormats* pFormats );
HRESULT WINAPI fmtrec_b( LPCVOID lpBuffer, SIZE_T cbBytes, IREFormats* pFormats );
HRESULT WINAPI fmtrec_a( IRXArchive* lpArchive, IREFormats* pFormats );
HRESULT WINAPI fmtrec_f( FILE* cFile, IREFormats* pFormats );
HRESULT WINAPI fmtdetail( FFMTID aFmtID, FORMAT_DETAIL & rDetails );
HRESULT WINAPI fmttestbit( FFMTID aFmtID, SIZE_T uClass );
HRESULT WINAPI fmttest( FFMTID aFmtID, SIZE_T uClassMask, SIZE_T UnUsed = 0 );
HRESULT WINAPI fmtid2ffrid( FFMTID fmt, PULONG id2010 );
HRESULT WINAPI ffrid2fmtid( FFMTID fmt, PULONG id2012 );
HRESULT WINAPI name2fmtid( LPCSTR name, FFMTID* fmtid );
LPCSTR  WINAPI fmtid2name( FFMTID fmtid );
FFMTID	WINAPI fmtidmax();
LPCSTR	WINAPI mask2name( UINT64 uMask );
UINT64	WINAPI name2mask( LPCSTR name );
// FFR 逻辑版本，只有当修改的代码可能影响之前的缓存时，才需要修改此版本
UINT32	WINAPI ffrver();

#endif	// #ifndef _RE_EXPORT_INTERFACE_FFR_H_


