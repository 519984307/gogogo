#ifndef __MSS_FILE_DEFINE__
#define __MSS_FILE_DEFINE__

#include "xsf.fmt.h"

// Table Attributes
enum
{
	MSTA_BAN		= 0x80000000,			// 该表 已经被禁止使用
	MSTA_COMPATIBLE = 0x01000000,			// 该表 兼容表，TABLENAME为LibType
											// {兼容表将采用可写方式映射}
											// 即默认包含了 MSTA_WRITEABLE

	MSTA_WINK		= 0x00000001,			// 该表 不常驻内存
	MSTA_WRITEABLE	= 0x00000004,			// 该表 在内存中是可写的
	MSTA_EXECUTABLE	= 0x00000008,			// 该表 在内存中是可执行的
	// 内部使用
	MSTA_SHMEM		= 0x00100000,			// 该表 使用共享内存，仅对创建表有用
	MSTA_FLY		= 0x00200000,			// 该表 不是库文件中的表
};

//
// IGlmLibFile::GetTable
//
enum
{
	GTF_WINK = 1,		// 取表后 不常驻内存
};

typedef xsf::SNAME			MSTNAME;
typedef MSTNAME*			LPMSTNAME;
 typedef const MSTNAME *	LPCMSTNAME;

#pragma pack(1)

typedef struct MSTINFO
{
	LE_UI32			AttrU;				// see Table Attributes

	LE_UI32			RNumU;				// if MSTA_COMPATIBLE set, else it's can be any value

	LE_UI32			VerEU;				// End Version
										// 结束版本可以以任意形式表示
										// 在兼容表中，为表的结束版本(拷贝自原表)
										// 在正常表中，该值建议使用表内容检验和

	LE_UI32			VerBU;				// Begin Version, Option, // if MSTA_COMPATIBLE set

	//////////////////////////////////////
	LE_UI16			TagU;				// Always set 0

	LE_UI16			DiffM;				// 'F' : FLRDiff, 'J' : JDiff(BinDiff), 'O': Override

	UINT8			DiffParam[8];		// 差异比较以及合并算法的相关参数，目前只有FLRDiff有参数，参数见XLR_PARAM结构
	
}RS_PACK_ONEBYTE MSTINFO, *LPMSTINFO;

//////////////////////////////////////////////////////////////////////////

namespace mss
{
	enum FMARK
	{
		MARK_MSLF		= 0x4D534C46,   //'MSLF'
		MARK_MSLP		= 0x4D534C50,   //'MSLP',
	};

	enum DIFFID
	{
		DID_INVALID		=  0 ,
		DID_JDIFF		= 'J',
		DID_FLRDIFF		= 'F',
		DID_VLRDIFF		= 'V',
		DID_OVERRIDE	= 'O',
	};

	// In SectionHeader.DiffParam
	struct XLR_PARAM
	{
		UINT8	KeyLen;
		LE_UI32	RecLen;	// Only DIFF_FLRDIFF
	} RS_PACK_ONEBYTE;

	enum _FIXMID8
	{
		CMD_DROP		= 'D',				// Section.Size === 0;
		CMD_OVERRIDE	= 'O',				// Section.Data = NewTable
		CMD_MODIFY		= 'M',				// Section.Data = delta
	};

	typedef UINT8	FIXMID8;
	typedef UINT8	DIFFID8;

	struct TABINF_HDR : xsf::SHDR
	{
		MSTINFO	Ext;
	} RS_PACK_ONEBYTE;

	struct TABFIX_HDR : xsf::SHDR
	{
		FIXMID8	FixCmd;
	} RS_PACK_ONEBYTE;

	struct LIBTYPENAME
	{
		LE_UI32 Mark;
		LE_UI32	LibType;
	} RS_PACK_ONEBYTE;

#pragma pack()

	enum { LIBTYPE_MARK = 0x4C425450 /*'LBTP'*/ };

	static inline VOID TabIdToName( DWORD dwId, MSTNAME& rName )
	{
		LIBTYPENAME& rTypeName = *(LIBTYPENAME*)&rName;
		LIBTYPENAME TypeName;
		TypeName.Mark = mss::LIBTYPE_MARK;
		TypeName.LibType = (UINT32)dwId;
		rTypeName = TypeName;
	}
	static inline HRESULT TabNameToId( DWORD& dwId, MSTNAME& rName )
	{
		LIBTYPENAME& rTypeName = *(LIBTYPENAME*)&rName;
		if( rTypeName.Mark != mss::LIBTYPE_MARK )
			return E_UNEXPECTED;
		dwId = (UINT32)rTypeName.LibType;
		return S_OK;
	}


};

#endif

