#ifndef __RXI_NAME_DB__
#define __RXI_NAME_DB__

#include "recfmt.h"
#include "mss.fmt.h"
#include "../../utils/libstr.hpp"

LPCSTR WINAPI MainClassName( SIZE_T uClass );
SIZE_T WINAPI MainClassId( LPCSTR aClassName, SIZE_T cClassName );

static inline SGID64 SIGID_40_TO_64( SGID40 & id )
{
	SGID64 tmp = id.AreaId;
	return (tmp << 32) + id.Id32;
};

enum { RX_MALW_NAME_MAX = 256 };

struct IDResult;

namespace nam
{
	enum MAIN_CLASS
	{
		kTrait		= 0,
		kHidden		= 1,
		kHiddenFU	= 2,
		kComplier	= 3,
		kPacker		= 4,
		kFormat		= 5,		// 文件格式
		kAppType	= 6,
		kTrusted	= 9,

		kMalware = 0x10,

		kTrojan,
		kBackdoor,
		kWorm,
		kRootkit,
		kExploit,
		kHackTool,
		kAdware,
		kStealer,
		kSpammer,
		kSpyware,
		kVirus,
		kJoke,
		kJunk,
		kPUA,
		kDownloader,
		kDropper,
		/// 2015-12-21
		kRansom,
		kHoax,
		kRiskware,
		// 2016-6-7
		kUnwanted,
		kMonetizer,				//	套现

		kMobileBase		= 0xC0,
		// Mobile
		//	摘自《移动互联网恶意代码描述规范》
		kPayment = kMobileBase,	//	恶意扣费
		kPrivacy,				//	隐私窃取
		kRemote, 				//	远程控制
		kSpread, 				//	恶意传播
		kExpense, 				//	资费消耗
		kSystem, 				//	系统破坏
		kFraud, 				//	诱骗欺诈
		kRogue,					//	流氓行为
		

		kAttention = 0xFE,		// 注意!
		kTypeMax = 0x100,
	};

	static inline BOOL IsHiddenClass( SIZE_T uClass )
	{
		return uClass == kHidden || uClass == kHiddenFU;
	}
	static inline BOOL IsHiddenClassNeedFile( SIZE_T uClass )
	{
		return uClass == kHiddenFU;
	}
	static inline BOOL IsMalwareClass( SIZE_T uClass )
	{
		return uClass >= kMalware;
	}

	struct STD13_MAINCLS_NAMETABLE
	{
		LPCSTR		p;
		MAIN_CLASS	t;
	};
	static STD13_MAINCLS_NAMETABLE _STD13_MAINCLS_NAMETABLE[] =
	{
		{ "Feature",		kTrait		},
		{ "Hidden",			kHidden		},
		{ "HiddenF",		kHiddenFU	},
		{ "Compiler",		kComplier	},
		{ "Packer",			kPacker		},
		{ "Trusted",		kTrusted	},
		{ "AppType",		kAppType	},
		//////////////////////////////////////////////////////////////////////////
		{ "Malware",		kMalware	},
		{ "Trojan",			kTrojan		},
		{ "Backdoor",		kBackdoor	},
		{ "Worm",			kWorm		},
		{ "Rootkit",		kRootkit	},
		{ "HackTool",		kHackTool	},
		{ "Exploit",		kExploit	},
		{ "Adware",			kAdware		},
		{ "Stealer",		kStealer	},
		{ "Spammer",		kSpammer	},
		{ "Spyware",		kSpyware	},
		{ "Virus",			kVirus		},
		{ "Joke",			kJoke		},
		{ "Junk",			kJunk		},
		{ "PUF",			kPUA		},
		{ "PUA",			kPUA		},
		{ "Downloader",		kDownloader	},
		{ "Dropper",		kDropper	},
		{ "Rogue",			kRogue		},
		{ "Ransom",			kRansom		},
		{ "Hoax",			kHoax		},
		{ "Riskware",		kRiskware	},
		//////////////////////////////////////////////////////////////////////////
		{ "Payment",		kPayment	},
		{ "Privacy",		kPrivacy	},
		{ "Remote",			kRemote		},
		{ "Spread",			kSpread		},
		{ "Expense",		kExpense	},
		{ "System",			kSystem		},
		{ "Fraud",			kFraud		},
		//////////////////////////////////////////////////////////////////////////
		{ "Attention",		kAttention	},
		{ "Unwanted",		kUnwanted	},
		{"Monetizer",		kMonetizer  },
	};

	static inline LPCSTR _MCLS2NAME( SIZE_T main )
	{
		for( size_t i = 0; i < sizeof(_STD13_MAINCLS_NAMETABLE)/sizeof(_STD13_MAINCLS_NAMETABLE[0]); ++ i )
		{
			if( (MAIN_CLASS)main == _STD13_MAINCLS_NAMETABLE[i].t )
			{
				return _STD13_MAINCLS_NAMETABLE[i].p;
			}
		}
		return "Malware";
	};

	static inline LPCSTR _MCLS2NAME2( SIZE_T main )
	{
		for( size_t i = 0; i < sizeof(_STD13_MAINCLS_NAMETABLE)/sizeof(_STD13_MAINCLS_NAMETABLE[0]); ++ i )
		{
			if( (MAIN_CLASS)main == _STD13_MAINCLS_NAMETABLE[i].t )
			{
				return _STD13_MAINCLS_NAMETABLE[i].p;
			}
		}
		return NULL;
	};

	static inline MAIN_CLASS _NAME2MCLS( LPCSTR name )
	{
		for( size_t i = 0; i < sizeof(_STD13_MAINCLS_NAMETABLE)/sizeof(_STD13_MAINCLS_NAMETABLE[0]); ++ i )
		{
			if( tcsicmp( name, _STD13_MAINCLS_NAMETABLE[i].p ) )
				continue;
			return _STD13_MAINCLS_NAMETABLE[i].t;
		}
		return kTypeMax;
	};
	static inline MAIN_CLASS _NAME2MCLS2( LPCSTR name, SIZE_T cb )
	{
		for( size_t i = 0; i < sizeof(_STD13_MAINCLS_NAMETABLE)/sizeof(_STD13_MAINCLS_NAMETABLE[0]); ++ i )
		{
			if( tcsnicmp( name, _STD13_MAINCLS_NAMETABLE[i].p, cb ) )
				continue;
			
			if( _STD13_MAINCLS_NAMETABLE[i].p[cb] )
				continue;

			return _STD13_MAINCLS_NAMETABLE[i].t;
		}
		return kTypeMax;
	};

	static inline LPCSTR _GET_MAIN_NAME( SIZE_T main )
	{
		return _MCLS2NAME(main);
	};

};

class IRXNameDB : public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD(SetLib)( IUnknown* lpMalLib ) PURE;
	STDMETHOD(Id32To40)( SGID32 SId32, LPSGID40 lpLongSid ) PURE;
	STDMETHOD(Id40To32)( SGID40 SId40, LPSGID32 lpSId32 ) PURE;
	STDMETHOD(GetNameById32)( SGID32 uVid, LPSTR lpName, SIZE_T cbName ) PURE;
	STDMETHOD(GetNameById40)( SGID40 uVid, LPSTR lpName, SIZE_T cbName ) PURE;
	STDMETHOD(Id40FromName)( LPCSTR aName, LPSGID40 lpSId40 ) PURE;
	STDMETHOD(BuildNameFree)( LPSTR lpName, SIZE_T cbName, SGID40 uSid40, nam::MAIN_CLASS malClass, UINT32 uFamilyId ) PURE;
};
// {214E797B-B94C-44C3-83BF-50C7772524D5}
RE_DEFINE_IID( IRXNameDB, "{214E797B-B94C-44C3-83BF-50C7772524D5}",
			0x214e797b, 0xb94c, 0x44c3, 0x83, 0xbf, 0x50, 0xc7, 0x77, 0x25, 0x24, 0xd5);



/*
	NameTable命名规则:
		N,A,M,E,{AreaId}
*/
static inline CHAR HEXCHAR4( UINT8 u )
{
	u &= 0xF;
	return ( u < 10 ) ? (u + '0') : ('A' + u - 10);
}

static inline void MAKE_NAME_TABLE_NAME( LPMSTNAME lpName, AREAID areaId )
{
	lpName->NameB[0] = '!';
	lpName->NameB[1] = 'N';
	lpName->NameB[2] = 'A';
	lpName->NameB[3] = 'M';
	lpName->NameB[4] = '0';
	lpName->NameB[5] = '0';
	lpName->NameB[6] = HEXCHAR4( areaId>>4 ); 
	lpName->NameB[7] = HEXCHAR4( areaId ); 
}

static const MSTNAME NAME_TABLE_00	= { 'N', 'A', 'M', 'E', 'T', 'A', 'B', '0' };

//////////////////////////////////////////////////////////////////////////

enum 
{
	SIGAREA_2010		= 0,
	SIGAREA_LOGIC		= 1,
	SIGAREA_NPE_HASH_1	= 2,
	SIGAREA_MALSPOT_FN	= 3,
	SIGAREA_BAD_LUCK	= 4,
	SIGAREA_DTREE		= 5,
	SIGAREA_MHCLS01BOT	= 6,
	SIGAREA_HIDDEN		= 255,
};

#endif