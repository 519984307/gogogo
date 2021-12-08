#ifndef __RISING_MPE__
#define __RISING_MPE__

#include "inamedb.h"

namespace mp
{
	template < class T >
	struct AUTO_DELETE
	{
		T * _p_;
		AUTO_DELETE( T * p ) : _p_(p) {};
		~AUTO_DELETE() { if( _p_ ) delete[] _p_; };
		T * detach(){ T * ret = _p_; _p_ = 0; return ret; }
	};

	static inline char lowchar( char c )
	{
		unsigned char uc = (unsigned char)c;
		uc = (uc < 'A' || uc > 'Z' ) ? (uc)  :  (uc + 0x20);
		return (char)uc;
	};

	//////////////////////////////////////////////////////////////////////////

	enum CTRL_CODE
	{
		kRegion			= 0x70,		// Test MatchPoint in Special Region
		kJump1			= '?',
		kJump8			= 0x80,
		kJump16			= 0x81,
		kJump32			= 0x82,
		kJumpM8			= 0x83,
		kJumpM16		= 0x84,
		kJumpM32		= 0x85,
		kInSet8			= 0x86,
		kOutSet8		= 0x87,
		kInSetM8		= 0x88,
		kOutSetM8		= 0x89,
		kEither2		= 0x8A,
		kEitherN		= 0x8B,
		kBackRef		= 0x8C,
		kX86Jmp			= 0x8D,
		kX86JmpO		= 0x8E,
		kGetJmp8		= 0x8F,
		kGetJmp16		= 0x90,
		kGetJmp32		= 0x91,
		//////////////////////////////////////////////////////////////////////////
		kCRC32			= 0x92,		// I	!crc32	size	crc-value	/ 7 + 7 + 7 + 8 = 29 Bits
		kCRC32U			= 0x93,		// I	!crc32U	size	crc-value	/ 7 + 7 + 7 + 8 = 29 Bits, crc32 with length
		kCaseI			= 0x94,		// I
		kCaseS			= 0x95,		// I
		kSlotSet		= 0x96,		// P	!SavLoc 0 -> tinycode
		kSlotGet		= 0x97,		// P	!LodLoc 1
		//////////////////////////////////////////////////////////////////////////
		kPush			= 0x98,		// I	Stack size = 16, Push Current-Pos to stack
		kPop			= 0x99,		// I	Pop stack top to Current-Pos
		//////////////////////////////////////////////////////////////////////////
		kNot			= 0xA0,		// I
		kPE_AtIATOf		= 0xA1,		// I	offset is IAT of xxx ? (APINameCRC)
		kPE_IsIATOf		= 0xA2,		// I	[offset] is IAT of xxx?
		//kIsIATOf2		= 0xA3,		// O	Float search [offset] is IAT of xxx
		kRepeat			= 0xA4,		// I	repeat time(1,1)	REPORT(SUBPTN:UINT16,MIN:UINT16,MAX:UINT16)
		kReadUI			= 0xA5,		// P	Read a number from current position
		kTop			= 0xA7,		// I	Set stack top to Current-Pos
		kAddFeature		= 0xA8,		// P	Add FeatureString to DTFC
		//////////////////////////////////////////////////////////////////////////
		kSearch			= 0xAB,		// P	range:SLEB128, len:SLEB128		!search
		//kStdFindB		= 0xAC,		// P	range:SLEB128, len:SLEB128		!bfind
		kStdSeek		= 0xAD,		// P	dist:SLEB128,					!seek
		//kMostSeek		= 0xAE,		// 
		//////////////////////////////////////////////////////////////////////////
		kLCaseCRC32		= 0xAF,		// P	crc32:U32, len:SLEB128			!lcrc "12312312312"
		//////////////////////////////////////////////////////////////////////////
		kTraceMatch		= 0xB0,		// P	kTraceMatch[E8,E9,EB,0F7F,JmpIns]
		//////////////////////////////////////////////////////////////////////////
		fGoBase			= 0xC0,		// P
		fGoEntry		= 0xC1,		// P
		fGoSectionIdx	= 0xC2,		// 
		fGoSectionNam	= 0xC3,		// 
		fGoTail			= 0xC4,
		fGoIAT			= 0xC5,		// P	Only PE
		fGoFunction		= 0xC6,		// P	
		fFreeMatch		= 0xC7,		// P
		//////////////////////////////////////////////////////////////////////////
		kEntryTrace		= 0xF0,		// P
		kImpTabHash		= 0xF1,		// 
		kHsaFeature		= 0xF2,		// Add FeatureString to DTFC
		//////////////////////////////////////////////////////////////////////////
		// script helper
		kPosPut			= 0xF3,
		kPosGet			= 0xF4,

	};

	enum { MAX_SEARCH = 2 * 1024 * 1024 };
	
	class ISearchRegions : public IUnknown
	{
	public:
		STDMETHOD_(BOOL,IsFullData)() PURE;
		STDMETHOD_(SIZE_T,GetRegionsCount)() PURE;
		STDMETHOD(GetRegion)( SIZE_T uIndex, RX_BLOB& rRegionData, RX_BLOB& rRegionName ) PURE;
		STDMETHOD(Build)( IRXArchive* lpArchive, IRXStream* lpStream, SIZE_T uMaxSearch = MAX_SEARCH ) PURE;
		RE_DECLARE_IID;
	};
	// {BE18F931-BF18-4343-8AFC-59A5067AA4E1}
	RE_DEFINE_IID( ISearchRegions, "{BE18F931-BF18-4343-8AFC-59A5067AA4E1}",
		0xbe18f931, 0xbf18, 0x4343, 0x8a, 0xfc, 0x59, 0xa5, 0x6, 0x7a, 0xa4, 0xe1);

	class ISearchRegionMaker : public IUnknown
	{
	public:
		STDMETHOD(CreateRegion)( 
			IRXArchive* lpArchive, 
			IRXStream* lpFile, 
			ISearchRegions** ppRegion, 
			SIZE_T uMaxSearch = MAX_SEARCH ) PURE;
		RE_DECLARE_IID;
	};

	// {7F5702E8-A612-4DA9-8938-D2D8FF18336B}
	RE_DEFINE_IID( ISearchRegionMaker,  "{7F5702E8-A612-4DA9-8938-D2D8FF18336B}",
		0x7f5702e8, 0xa612, 0x4da9, 0x89, 0x38, 0xd2, 0xd8, 0xff, 0x18, 0x33, 0x6b);



};

#endif
