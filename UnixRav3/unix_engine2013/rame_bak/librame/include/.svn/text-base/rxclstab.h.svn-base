#ifndef __RX_CLASS_REGISTRY__
#define __RX_CLASS_REGISTRY__

#include "rxclsid.h"
#include "iextmail.h"

namespace RXCOM
{

	struct CLASS_TO_DLL						{	TCLSID	clsidClass;		LPCSTR	ansiModName;	};

	struct CLASS_CATELOG_ITEM				{	DWORD	ClassIndex;		TCLSID	ClassUniqID;	};

	struct CLASS_CATELOG					{	TCLSID	UniqID;			DWORD	Count;			CLASS_CATELOG_ITEM*	TablePtr;	};

#define DEF_CATELOG_ENTRY(cid,n)		{ cid, sizeof(n)/sizeof(n[0]), n }

	//////////////////////////////////////////////////////////////////////////
	//
	//
	//
	//						ALL NAMED MODULES
	//
	//
	//
	//////////////////////////////////////////////////////////////////////////
#if (TARGET_OS == OS_WINDOWS)

	static LPCSTR	MOD_R_RUNTIME					= "rxruntim.dll";
	static LPCSTR	MOD_R_CORE						= "rxcore.dll";
	static LPCSTR	MOD_R_EXTFILE					= "rxarch.dll";
	static LPCSTR	MOD_ARCH_PE						= "rxwinpe.dll";
	static LPCSTR	MOD_R_MSS						= "rxmss.dll";
	static LPCSTR	MOD_MDU_10						= "rxcoml.dll";
	static LPCSTR	MOD_R_OFFICE					= "rxoffice.dll";
	static LPCSTR	MOD_R_RXMAIL					= "rxmail.dll";
	static LPCSTR	MOD_R_MLTK						= "rxmltk.dll";
	static LPCSTR	MOD_R_AUTH						= "rxauth.dll";

#elif (TARGET_OS == OS_POSIX)

	static LPCSTR	MOD_R_RUNTIME					= "librxruntim.so";
	static LPCSTR	MOD_R_CORE						= "rxcore.so";
	static LPCSTR	MOD_R_EXTFILE					= "rxarch.so";
	static LPCSTR	MOD_ARCH_PE						= "rxwinpe.so";
	static LPCSTR	MOD_R_MSS						= "librxmss.so";
	static LPCSTR	MOD_MDU_10						= "librxcoml.so";
	static LPCSTR	MOD_R_OFFICE					= "rxoffice.so";
	static LPCSTR	MOD_R_RXMAIL					= "rxmail.so";
	static LPCSTR	MOD_R_MLTK						= "rxmltk.so";
	static LPCSTR	MOD_R_AUTH						= "rxauth.so";

#elif (TARGET_OS == OS_NATIVE)

	static LPCSTR	MOD_R_CORE						= "rxbcore.dll";
	static LPCSTR	MOD_R_EXTRACT					= "rxbcomp.dll";

#endif
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//
	//						ALL DEFINED CLASS
	//
	//
	//
	//////////////////////////////////////////////////////////////////////////

	static const CLASS_TO_DLL ALL_COMPONMENTS[] = 
	{
		//rxmail
		{CLSID_EMLFile,									MOD_R_RXMAIL    },
		{CLSID_FoxMail3,								MOD_R_RXMAIL    },
		{CLSID_Netscape,								MOD_R_RXMAIL    },
		{CLSID_OutlookExp4,								MOD_R_RXMAIL    },
		{CLSID_OutlookExp5,								MOD_R_RXMAIL    },
		{CLSID_Base64File,								MOD_R_RXMAIL    },
		{CLSID_OutlookPst,								MOD_R_RXMAIL    },
		//
		{CLSID_OLEArchive,								MOD_R_OFFICE	},
		{CLSID_XMLOfficeArchive,						MOD_R_OFFICE	},
		{CLSID_XDPFile,									MOD_R_OFFICE	},
		{CLSID_RTFArchive,								MOD_R_OFFICE	},
		{CLSID_WSFArchive,								MOD_R_OFFICE	},
		

		{CLSID_InitLegacy10,							MOD_MDU_10		},

		{CLSID_ObjectTable,								MOD_R_MSS		},
		{CLSID_PropertyX,								MOD_R_MSS		},
		{CLSID_Property,								MOD_R_MSS		},
		{CLSID_Property2,								MOD_R_MSS		},
		//
		{CLSID_VirtualFile,								MOD_R_MSS		},
		{CLSID_HandleFile,								MOD_R_MSS		},
		{CLSID_LocalFileSystem,							MOD_R_MSS		},
		{CLSID_MssLibPatcher,							MOD_R_MSS		},
		{CLSID_MemoryFile1,								MOD_R_MSS		},

		{CLSID_Sig64List,								MOD_R_CORE		},
		{CLSID_NormalArchive,							MOD_R_CORE		},
		{CLSID_FormatList,								MOD_R_CORE		},
		//
		{CLSID_ObjectDB,								MOD_R_CORE		},
		{CLSID_StoneProp,								MOD_R_CORE		},
		{CLSID_ODBServer,								MOD_R_CORE		},
		{CLSID_ODBClient,								MOD_R_CORE		},
		///
		{CLSID_Condition,								MOD_R_CORE		},
		{CLSID_CondFilter,								MOD_R_CORE		},
		{CLSID_Parameter,								MOD_R_CORE		},
		{CLSID_FrontEnd,								MOD_R_CORE		},

		{CLSID_MduLegacy,								MOD_R_CORE		},
		{CLSID_MduSig64,								MOD_R_CORE		},
		{CLSID_WduTrust,								MOD_R_CORE		},

		{CLSID_NameDB,									MOD_R_CORE		},
		{CLSID_MemBlock,								MOD_R_CORE		},
		
		{CLSID_PKCS7Verify,								MOD_R_CORE		},
		
		{CLSID_CertVerify,								MOD_R_AUTH		},
		{CLSID_CertVerifyW,								MOD_R_AUTH		},

		{CLSID_PEArchive,								MOD_ARCH_PE		},
		{CLSID_PEDotNetArchive,							MOD_ARCH_PE		},
		
		// archive
		{CLSID_7zArchive,								MOD_R_EXTFILE	},
		{CLSID_7zExeArchive,							MOD_R_EXTFILE	},
		{CLSID_AceArchive,								MOD_R_EXTFILE	},
		{CLSID_AceExeArchive,							MOD_R_EXTFILE	},
		{CLSID_ArcArchive,								MOD_R_EXTFILE	},
		{CLSID_ArjArchive,								MOD_R_EXTFILE	},
		{CLSID_ArjExeArchive,							MOD_R_EXTFILE	},
		{CLSID_Bz2Archive,								MOD_R_EXTFILE	},
		{CLSID_CabArchive,								MOD_R_EXTFILE	},
		{CLSID_CabExeArchive,							MOD_R_EXTFILE	},
		{CLSID_CabSetExeArchive,						MOD_R_EXTFILE	},
		{CLSID_ChmArchive,								MOD_R_EXTFILE	},
		{CLSID_GZipArchive,								MOD_R_EXTFILE	},
		{CLSID_LhaArchive,								MOD_R_EXTFILE	},
		{CLSID_LhaExeArchive,							MOD_R_EXTFILE	},
		{CLSID_NsisArchive,								MOD_R_EXTFILE	},
		{CLSID_RarExeArchive,							MOD_R_EXTFILE	},
		{CLSID_SeaArchive,								MOD_R_EXTFILE	},
		{CLSID_SisArchive,								MOD_R_EXTFILE	},
		{CLSID_TarArchive,								MOD_R_EXTFILE	},
		{CLSID_UueArchive,								MOD_R_EXTFILE	},
		{CLSID_ZipArchive,								MOD_R_EXTFILE	},
		{CLSID_ZipExeArchive,							MOD_R_EXTFILE	},
		{CLSID_ZooArchive,								MOD_R_EXTFILE	},
		{CLSID_InnoArchive,								MOD_R_EXTFILE	},
		{CLSID_RarArchive,								MOD_R_EXTFILE	},
		{CLSID_WiseArchive,								MOD_R_EXTFILE	},
		{CLSID_WiseAdvArchive,							MOD_R_EXTFILE	},
		{CLSID_AutoItArchive,							MOD_R_EXTFILE	},
		{CLSID_ElangArchive,							MOD_R_EXTFILE	},
		{CLSID_GenteeArchive,							MOD_R_EXTFILE	},
		{CLSID_PaquetArchive,							MOD_R_EXTFILE	},
		{CLSID_InstylerArchive,							MOD_R_EXTFILE	},
		{CLSID_QuickBatchArchive,						MOD_R_EXTFILE	},
		{CLSID_SfactoryArchive,							MOD_R_EXTFILE	},
		{CLSID_VolumeHistory,							MOD_R_EXTFILE	},
		{CLSID_PdfArchive,								MOD_R_EXTFILE	},
		{CLSID_DexArchive,                              MOD_R_EXTFILE    },
		{CLSID_SwcArchive,                              MOD_R_EXTFILE    },
		// compress
		{CLSID_CopyCoder,								MOD_R_EXTFILE	},
		{CLSID_CipherCoder,								MOD_R_EXTFILE	},
		{CLSID_ACEV1Decoder,							MOD_R_EXTFILE	},
		{CLSID_ACEV2Decoder,							MOD_R_EXTFILE	},
		{CLSID_Bz2Decoder,								MOD_R_EXTFILE	},
		{CLSID_Bz2Encoder,								MOD_R_EXTFILE	},
		{CLSID_DeflateEncoder,							MOD_R_EXTFILE	},
		{CLSID_DeflateDecoder,							MOD_R_EXTFILE	},
		{CLSID_Rar29Decoder,							MOD_R_EXTFILE	},
		{CLSID_Rar20Decoder,							MOD_R_EXTFILE	},
		{CLSID_Rar15Decoder,							MOD_R_EXTFILE	},
		{CLSID_UUDecoder,								MOD_R_EXTFILE	},
		{CLSID_UUEncoder,								MOD_R_EXTFILE	},
		{CLSID_Lh1Decoder,								MOD_R_EXTFILE	},
		{CLSID_Lh2Decoder,								MOD_R_EXTFILE	},
		{CLSID_Lh3Decoder,								MOD_R_EXTFILE	},
		{CLSID_Lh4Decoder,								MOD_R_EXTFILE	},
		{CLSID_Lh5Decoder,								MOD_R_EXTFILE	},
		{CLSID_Lh6Decoder,								MOD_R_EXTFILE	},
		{CLSID_Lh7Decoder,								MOD_R_EXTFILE	},
		{CLSID_LzsDecoder,								MOD_R_EXTFILE	},
		{CLSID_Lz5Decoder,								MOD_R_EXTFILE	},
		{CLSID_ArjDecoder,								MOD_R_EXTFILE	},
		{CLSID_ArjLzhDecoder,							MOD_R_EXTFILE	},
		{CLSID_LzxDecoder,								MOD_R_EXTFILE	},
		{CLSID_LzxEncoder,								MOD_R_EXTFILE	},
		{CLSID_QtmDecoder,								MOD_R_EXTFILE	},
		{CLSID_CabDeflateDecoder,						MOD_R_EXTFILE	},
		{CLSID_CabQuantumDecoder,						MOD_R_EXTFILE	},
		{CLSID_CabLzxDecoder,							MOD_R_EXTFILE	},
		{CLSID_ChmLzxDecoder,							MOD_R_EXTFILE	},
		{CLSID_ChmLzxEncoder,							MOD_R_EXTFILE	},
		{CLSID_ImplodeDecoder,       					MOD_R_EXTFILE	},
		{CLSID_ShrinkDecoder,							MOD_R_EXTFILE	},
		{CLSID_ZooLzwDecoder,							MOD_R_EXTFILE	},
		{CLSID_ZooLzhDecoder,							MOD_R_EXTFILE	},
		{CLSID_LzmaDecoder,								MOD_R_EXTFILE	},
		{CLSID_LzmaEncoder,								MOD_R_EXTFILE	},
		{CLSID_Lzma2Decoder,							MOD_R_EXTFILE	},
		{CLSID_Lzma2Encoder,							MOD_R_EXTFILE	},
		{CLSID_BcjDecoder,								MOD_R_EXTFILE	},
		{CLSID_BcjEncoder,								MOD_R_EXTFILE	},
		{CLSID_Bcj2Decoder,								MOD_R_EXTFILE	},
		{CLSID_Bcj2Encoder,								MOD_R_EXTFILE	},
		{CLSID_BcPpcDecoder,							MOD_R_EXTFILE	},
		{CLSID_BcPpcEncoder,							MOD_R_EXTFILE	},
		{CLSID_BcIa64Decoder,							MOD_R_EXTFILE	},
		{CLSID_BcIa64Encoder,							MOD_R_EXTFILE	},
		{CLSID_BcArmDecoder,							MOD_R_EXTFILE	},
		{CLSID_BcArmEncoder,							MOD_R_EXTFILE	},
		{CLSID_BcArmThumbDecoder,						MOD_R_EXTFILE	},
		{CLSID_BcArmThumbEncoder,						MOD_R_EXTFILE	},
		{CLSID_BcSparcDecoder,							MOD_R_EXTFILE	},
		{CLSID_BcSparcEncoder,							MOD_R_EXTFILE	},
		{CLSID_PpmdDecoder,								MOD_R_EXTFILE	},
		{CLSID_PpmdEncoder,								MOD_R_EXTFILE	},
		{CLSID_ArcRleDecoder,							MOD_R_EXTFILE	},
		{CLSID_ArcHuffmanDecoder,						MOD_R_EXTFILE	},
		{CLSID_ArcLzDecoder,							MOD_R_EXTFILE	},
		{CLSID_ArcLzRleDecoder,							MOD_R_EXTFILE	},
		{CLSID_ArcLz2RleDecoder,						MOD_R_EXTFILE	},
		{CLSID_ArcDlzRleDecoder,						MOD_R_EXTFILE	},
		{CLSID_ArcDlzDecoder,							MOD_R_EXTFILE	},
		{CLSID_GenteeDecoder,							MOD_R_EXTFILE	},
		{CLSID_AutoItDecoder,							MOD_R_EXTFILE	},
		{CLSID_SFactoryDecoder,							MOD_R_EXTFILE	},
		{CLSID_ASCIIHexDecoder,							MOD_R_EXTFILE	},
		// hash
		{CLSID_HashSha1,								MOD_R_EXTFILE	},
		{CLSID_HashSha256,								MOD_R_EXTFILE	},
		{CLSID_HashCRC16,								MOD_R_EXTFILE	},
		{CLSID_HashCRC32,								MOD_R_EXTFILE	},
		{CLSID_HashMD4,									MOD_R_EXTFILE	},
		{CLSID_HashMD5,									MOD_R_EXTFILE	},
		{CLSID_HashADLER32,								MOD_R_EXTFILE	},
		{CLSID_HashHmacSha1,							MOD_R_EXTFILE	},
		{CLSID_PKCSPBKDFHmacSha1,						MOD_R_EXTFILE	},
		// crypto
		{CLSID_AesCbcDecoder,							MOD_R_EXTFILE	},
		{CLSID_AesCbcEncoder,							MOD_R_EXTFILE	},
		{CLSID_AesEcbDecoder,							MOD_R_EXTFILE	},
		{CLSID_AesEcbEncoder,							MOD_R_EXTFILE	},
		{CLSID_AesCfbDecoder,							MOD_R_EXTFILE	},
		{CLSID_AesCfbEncoder,							MOD_R_EXTFILE	},
		{CLSID_AesOfbCoder,								MOD_R_EXTFILE	},
		{CLSID_AesCtrSimpleCoder,						MOD_R_EXTFILE	},
		{CLSID_Rar20CipherDecoder,						MOD_R_EXTFILE	},
		{CLSID_Rar20CipherEncoder,						MOD_R_EXTFILE	},
		{CLSID_RC4,                  					MOD_R_EXTFILE	},
		{CLSID_BlowFishEcbDecoder,						MOD_R_EXTFILE	},
		// search
		{CLSID_NaiveSearch,								MOD_R_EXTFILE	},
		{CLSID_RNaiveSearch,							MOD_R_EXTFILE	},
		{CLSID_MultiNaiveSearch,						MOD_R_EXTFILE	},
		{CLSID_CmpressBlockArchive , 					MOD_R_EXTFILE	},
		{CLSID_ApkArchive , 							MOD_R_EXTFILE	},

		{CLSID_SupportVectorMachine,					MOD_R_MLTK		},
		{CLSID_SmartInstallerArchive,					MOD_R_EXTFILE	},
		{CLSID_FasArchive,								MOD_R_EXTFILE	},
		{CLSID_JsArchive,								MOD_R_EXTFILE	},
		{CLSID_MSO,										MOD_R_EXTFILE	},
		{CLSID_CVbeArchive,								MOD_R_EXTFILE	},
		{CLSID_CSwfArchive2,                            MOD_R_EXTFILE	},
	};

	//////////////////////////////////////////////////////////////////////////

	static CLASS_CATELOG_ITEM _ARCHIVE_CLASS_CATELOG[]= 
	{
		//{ FFR::FMT_MZ,			CLSID_PEArchive			},
		{ FFR::FMT_NORMAL,			CLSID_NormalArchive		},
		{ FFR::FMT_XOFFICE,			CLSID_XMLOfficeArchive	},
		{ FFR::FMT_DOC,				CLSID_OLEArchive		},
		{ FFR::FMT_PE,				CLSID_PEArchive			},
		{ FFR::FMT_MSIL,			CLSID_PEDotNetArchive	},
		{ FFR::FMT_PDF,				CLSID_PdfArchive		},
		{ FFR::FMT_7Z,				CLSID_7zArchive			},
		{ FFR::FMT_ZIP,				CLSID_ZipArchive		},
		{ FFR::FMT_ZIPEXE,			CLSID_ZipExeArchive		},
		{ FFR::FMT_RAR,				CLSID_RarArchive		},
		{ FFR::FMT_NSIS,			CLSID_NsisArchive		},
		{ FFR::FMT_RAREXE,			CLSID_RarExeArchive		},
		{ FFR::FMT_CAB,				CLSID_CabArchive		},
		{ FFR::FMT_CABEXE,			CLSID_CabExeArchive		},
		{ FFR::FMT_7ZEXE,			CLSID_7zExeArchive		},
		{ FFR::FMT_ACE,				CLSID_AceArchive		},
		{ FFR::FMT_ACEEXE,			CLSID_AceExeArchive		},
		{ FFR::FMT_ARC,				CLSID_ArcArchive		},
		{ FFR::FMT_ARJ,				CLSID_ArjArchive		},
		{ FFR::FMT_ARJEXE,			CLSID_ArjExeArchive		},
		{ FFR::FMT_BZ2,				CLSID_Bz2Archive		},
		{ FFR::FMT_CABSETEXE,		CLSID_CabSetExeArchive	},
		{ FFR::FMT_CHM,				CLSID_ChmArchive		},
		{ FFR::FMT_GZIP,			CLSID_GZipArchive		},
		{ FFR::FMT_LZH,				CLSID_LhaArchive		},
		{ FFR::FMT_LZHEXE,			CLSID_LhaExeArchive		},
		{ FFR::FMT_SEAEXE,			CLSID_SeaArchive		},
		{ FFR::FMT_SIS,				CLSID_SisArchive		},		
		{ FFR::FMT_TAR,				CLSID_TarArchive		},	
		{ FFR::FMT_UUE,				CLSID_UueArchive		},
		{ FFR::FMT_ZOO,				CLSID_ZooArchive		},
		{ FFR::FMT_INNO,			CLSID_InnoArchive		},
		{ FFR::FMT_WISE,			CLSID_WiseArchive		},
		{ FFR::FMT_WISEA,			CLSID_WiseAdvArchive	},
		{ FFR::FMT_SWC,				CLSID_SwcArchive		},
		{ FFR::FMT_DEX,				CLSID_DexArchive		},
		{ FFR::FMT_AUTOIT,			CLSID_AutoItArchive		},
		//mail 
		{ FFR::FMT_EML,				CLSID_EMLFile			},
		//{ FFR::FMT_MAILBODY,    CLSID_NULL				},
		{ FFR::FMT_NWS,				CLSID_EMLFile			},
		{ FFR::FMT_MHT,				CLSID_EMLFile			},
		{ FFR::FMT_MSG,				CLSID_EMLFile			},

		{ FFR::FMT_OUTLOOKEX_V5 ,	CLSID_OutlookExp5       },
		{ FFR::FMT_OUTLOOKEX_V4,	CLSID_OutlookExp4		},
		{ FFR::FMT_FOXMAIL_V3,		CLSID_FoxMail3		    },
		{ FFR::FMT_NETSCAPE_V4,		CLSID_Netscape		    },
		{ FFR::FMT_BASE64,			CLSID_Base64File		},
		{ FFR::FMT_PST,				CLSID_OutlookPst		},
		//{ FFR::FMT_UCS16LE,		CLSID_},
		{ FFR::FMT_XDP,				CLSID_XDPFile			},
		{ FFR::FMT_RTF,				CLSID_RTFArchive		},
		{ FFR::FMT_COMPRESS_LZMA,	CLSID_CmpressBlockArchive		},
		{ FFR::FMT_WSF			,	CLSID_WSFArchive},

		{ FFR::FMT_APK,				CLSID_ApkArchive		},
		//{ FFR::FMT_IPA,				CLSID_CmpressBlockArchive		},
		{ FFR::FMT_SMARTINSTALLER,	CLSID_SmartInstallerArchive},
		{ FFR::FMT_FAS,				CLSID_FasArchive		},
		{ FFR::FMT_JS,				CLSID_JsArchive			},
		{ FFR::FMT_MSO,				CLSID_MSO				},
		{ FFR::FMT_VBSENC,			CLSID_CVbeArchive		},
		{ FFR::FMT_JSENC,			CLSID_CVbeArchive		},
		{ FFR::FMT_SWF,				CLSID_CSwfArchive2		},
	};
	/////////////////////////////////////////////////////////////////////////////

	static CLASS_CATELOG ALL_CATELOGS[] = 
	{
		DEF_CATELOG_ENTRY(CLSID_ARCHIVE_CLASS_CATELOG,	_ARCHIVE_CLASS_CATELOG),
	};
};
#endif