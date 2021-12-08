#ifndef __XSF_FILE_FORMAT__
#define __XSF_FILE_FORMAT__

#pragma pack(1)

namespace xsf
{
	enum CODERID8
	{
		CODER8_COPY		= 0,
		CODER8_DEFLATE	= 'Z',
		CODER8_LZMA	= 'L',
		CODER8_ZSTD	= 'S',
	};

	typedef UINT8	CODERID;

	struct FHDR
	{
		LE_UI32		MARK;				// U Fill
		LE_UI32		VerB;				// U Fill
		LE_UI32		VerE;				// U Fill
		LE_UI16		SecNum;				//
		UINT8		Coder;				// Coder
		UINT8		ExtICB;				// Extend-Info Bytes
		LE_UI64		DatLen;				// Data Size / COPY(TableSize sum)
		UINT8		DatMD5[16];			// Data MD5
		LE_UI32		SecTabCRC;			// gltUpdateSum([TableItemTable])
		LE_UI32		HdrCRC;				// Header Count, Contain SectionTable, gltUpdateSum
	}RS_PACK_ONEBYTE;

	typedef struct SNAME
	{
		UINT8		NameB[8];
	}RS_PACK_ONEBYTE SNAME, *LPSNAME;

	struct SHDR
	{
		SNAME		Name;
		LE_UI32		Size;
	}RS_PACK_ONEBYTE;

	static inline UINT32 UpdateSum( UINT32 u32Prev, LPCVOID lpBuffer, SIZE_T cbData )
	{
		UINT8 * pBytes = (UINT8 *)lpBuffer;
		for( SIZE_T i = 0; i < cbData; ++ i )
		{
			u32Prev += pBytes[i];
			if( u32Prev & 1 ) (u32Prev <<= 1);
		}
		return u32Prev;
	}

	static inline bool operator < ( const SNAME& l, const SNAME& r )
	{
		return memcmp( l.NameB, r.NameB, sizeof(l.NameB) ) < 0;
	}
	static inline bool operator == ( const SNAME& l, const SNAME& r )
	{
		return 0 == memcmp( l.NameB, r.NameB, sizeof(l.NameB) );
	}

};

#if CPP_COMPILER==CC_GCC
static const xsf::SNAME TABNAME_NULL = { 0, 0, 0, 0, 0, 0, 0, 0 };
#else
static const xsf::SNAME TABNAME_NULL = {};
#endif
static const xsf::SNAME TABNAME_HEADER = { '!', 'H', 'D', 'R', 'D', 'I' ,'F', 'F' };

#pragma pack()

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

#endif
