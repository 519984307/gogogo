#ifndef __ALL_RECORD_FORMAT_DEFINE__
#define __ALL_RECORD_FORMAT_DEFINE__

#pragma pack(1)

// 32bit siguature id
typedef UINT32	SGID32, *LPSGID32;
typedef UINT8	AREAID;
typedef UINT64	SGID64;

// long siguature id
typedef struct _LONG_SIGID
{
	AREAID		AreaId;
	SGID32		Id32;
} __attribute__((packed)) SGID40, *LPSGID40;

namespace nam
{
	/*
		NAMTHDR
		SGIDAREA[]
		ID2NAME[]
		NAMES[]
	*/
	typedef struct _NAMINF_HDR
	{
		LE_UI16	cAreas;
		LE_UI32	cIndices;
	} __attribute__((packed)) NAMTHDR, *LPNAMTHDR;

	typedef struct _SIGID_AREA_MAP
	{
		AREAID	AreaId;
		CHAR	ShortName[4];
		LE_UI32	Id32Start;
		LE_UI32	Id32End;
	} __attribute__((packed)) SGIDAREA, *LPSGIDAREA;

	typedef struct _FAMILY_REC
	{
		le::U24	FamilyId;
		UINT8	Length;
		LE_UI32	Offset;
		//CHAR	Family[0];
	} __attribute__((packed)) FAMILY, * LPFAMILY;

	enum { MAX_FAMILY_NAME = 256 };

	//////////////////////////////////////////////////////////////////

	typedef struct _SGID2NAMEREC
	{
		LE_UI32	Id32;
		UINT8	Main;		// nam::MAIN_CLASS
		le::U24	FamilyId;	// 
	} __attribute__((packed)) ID2NAME, * LPID2NAME;
};

namespace tjh
{
	typedef struct _TJ_RECORD
	{
		LE_UI64	Tj11CRC;
		LE_UI32	Vid;
	}__attribute__((packed)) TJREC, *LPTJREC;
};

namespace fps
{
	typedef struct _XSIG_RECORD
	{
		UINT8			hash[sizeof(UINT8)+sizeof(UINT64)];
		LE_UI32			sigid;
	} __attribute__((packed)) H72REC, *LPH72REC;
};

namespace malspot
{
	enum 
	{
		MALFN_PE,
		MALFN_DEX,
		MALFN_PE64,
		MALFN_MSIL,
	};

	typedef struct _MALFUNC_RECORD
	{
		UINT8			hash[sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT64)];
		LE_UI32			sigid;
	} __attribute__((packed)) FNREC, *LPFNREC;

	// 0x12 Bytes
	enum {
		BSTR_CASE_SENS	= 1,
		BSTR_UTF16LE	= 2,
	};
	typedef struct _MALBSTR_RECORD
	{
		LE_UI16		Format;
		UINT8		Prefix6[6];
		//////////////////////////////////	BinarySearch + BloomFilter
		UINT8		Opt;					// 
		UINT8		cbLen;
		LE_UI32		bbHash;					// MurMurHash1
		//////////////////////////////////	TableDiff(F,14,18)
		LE_UI32		SigId;
	} __attribute__((packed)) BSREC, *LPBSREC;
};

namespace exd
{
	enum 
	{
		EXDU_POST_EVAL = 1,
		EXDU_WITH_PARAM = 2,
		EXDU_WITH_PRIORITIZED = 4,
	};
	typedef struct _REC_HDR
	{
		LE_UI32		SigId;
		UINT8		Typi;			// Malware/Trojan/...
		UINT8		cFormat;		// UINT16 * cFormat
		UINT8		cbProgId;		// Include '\0'
		UINT8		uOpt8;			// is Post Unit
		//le::U16	cParam;			// ParamSize, if EXDU_WITH_PARAM set
	} __attribute__((packed)) EXDUREC, *LPEXDUREC;
};

namespace tcr
{
	enum OPT8
	{
		TCR_CARE_EXE = 1,
		TCR_CARE_DLL = 2,
		TCR_REMOVE_FILE = 4,
		TCR_NATIVE = 8,
	};
	typedef struct TC11_REC
	{
		LE_UI32		Id32;
		LE_UI16		Length;
		UINT8		Type;
		UINT8		Flags;			// From Compiler
	} RS_PACK_ONEBYTE TCREC, *LPTCREC;
};

namespace fsm
{
	namespace nmp
	{
		enum { PREFIX_LENGTH = 6 };

		enum PTNFLAG	// 16 MAX
		{
			PTN_CASE_SENS = 1,
			PTN_MULTI_HIT = 2,
			//PTN_NOT_FLOAT = 2,
		};

		typedef struct _REC_HDR
		{
			LE_UI32			SigId;
			UINT8			Typi;			// Malware/Trojan/...
			UINT8			Formats;		
			UINT8			Patterns;
			LE_UI16			HitScore;
		} __attribute__((packed)) RECHDR, *LPRECHDR;

		enum PTNAT	// 16 MAX
		{
			PTNAT_STUFF				= 0,
			PTNAT_DTF				= 1,
			PTNAT_SCRIPT_NORMALIZE	= 2,			// 脚本标准化(去空白字符)
			PTNAT_SCRIPT_ALPHANUM	= 3,			// 脚本仅字符
			PTNAT_SCRIPT_OPERATOR	= 4,			// 脚本仅操作符
			PTNAT_SCRIPT_KEYWORDS	= 5,			// 脚本仅关键字
		};

		typedef struct NMP_PTN_HDR
		{
#ifdef RS_BIG_ENDIAN
			UINT8			At:4;					// PTNAT
			UINT8			Flags:4;				// PTNFLAG
#else
			UINT8			Flags:4;				// PTNFLAG
			UINT8			At:4;					// PTNAT
#endif
			LE_UI16			Length;					// Pattern length
			le::I16			SScore;					// Pattern Score
		} __attribute__((packed)) PTNHDR, *LPPTNHDR;
	};
};

namespace svm
{
	typedef struct _SVM_MODEL_REC
	{
		LE_UI32			SigId;
		LE_UI16			Format;
		LE_UI32			ModelSize;
	} __attribute__((packed)) MDLHDR, *LPMDLHDR;
};

namespace minwise
{
	enum { MINWISE_63 = 63 };
	typedef struct _MINHASH_32_63_REC
	{
		LE_UI32			SigId;
		LE_UI32			Features[MINWISE_63];
	} __attribute__((packed)) H32N63REC, *LPH32N63REC;
};

namespace dt
{
	// 默认: 模型都在云激活的情况下才能运转，并且会避免报“好”文件
	enum
	{
		MDL_32BIT		= 1,
		MDL_PART		= 2,			// _DTM_INDEX
		MDL_NO_CLOUD	= 4,			// 该模型可以工作在无云的情况下
		MDL_NO_AVOID	= 8,			// 该模型可以不避开“好”文件
		MDL_NOT_REMOVE	= 16,			// 不要删除文件
	};

	typedef struct _DTMHDR_16
	{
		UINT8	cbProgId;
		LE_UI16	cnModelNodes;
		inline LPCSTR GetProgId()
		{
			if( cbProgId < 2 ) return NULL;
			LPCSTR aName = (LPCSTR)(this + 1);
			if( aName[cbProgId-1] ) return NULL;
			return aName;
		}
		inline LPCVOID ModelData()
		{
			return ((LPCSTR)(this + 1) + cbProgId);
		}
	}RS_PACK_ONEBYTE DTMHDR16, *LPDTMHDR16;

	typedef struct _DTMHDR_32
	{
		UINT8	cbProgId;
		LE_UI32	cnModelNodes;
		inline LPCSTR GetProgId()
		{
			if( cbProgId < 2 ) return NULL;
			LPCSTR aName = (LPCSTR)(this + 1);
			if( aName[cbProgId-1] ) return NULL;
			return aName;
		}
		inline LPCVOID ModelData()
		{
			return ((LPCSTR)(this + 1) + cbProgId);
		}
	}RS_PACK_ONEBYTE DTMHDR32, *LPDTMHDR32;

	typedef struct _DTM_PARTITION32
	{
		LE_UI32			PartId;
	} __attribute__((packed)) PART32, *LPPART32;

	typedef struct _DTM16_REC_HDR
	{
		LE_UI32			SigId;
		LE_UI16			Format;
		UINT8			MainClass;
		UINT8			Flags;

		inline LPPART32 GetPartition32()
		{
			if( Flags & MDL_PART ) 
				return (LPPART32)(this + 1);
			return NULL;
		}
		inline LPDTMHDR16 GetModelHeader16()
		{
			LPPART32 p = GetPartition32();
			if( !p ) return (LPDTMHDR16)(this + 1);
			else return (LPDTMHDR16)(p + 1);
		}
		inline LPDTMHDR32 GetModelHeader32()
		{
			LPPART32 p = GetPartition32();
			if( !p ) return (LPDTMHDR32)(this + 1);
			else return (LPDTMHDR32)(p + 1);
		}
	} __attribute__((packed)) RECHDR, *LPRECHDR;
};

namespace asg {

	namespace html {
	
		typedef struct _SCRIPT_SIGN_REC
		{
			LE_UI32		SigId;
			LE_UI64		Hash64;
		} __attribute__((packed)) SSR, *SSR_PTR;

		typedef struct _URL_SIGN_REC
		{
			LE_UI64		Hash64;
		} __attribute__((packed)) USR, *USR_HDR_PTR;

	};
};
#pragma pack()

#endif