#ifndef __IRSTABLE_H__
#define __IRSTABLE_H__




#define IN
#define OUT
#define INOUT





//----------------------------------------------------------------------------------
// Modify by Rangh 2008-7-28
// Modify begin
//表对外的结构(非库的原始结构)

struct LibTableInfo
{
//	LibTableInfo(WORD _wTableType, DWORD _dwRecordCount, DWORD _dwTableSize, DWORD _dwStartVersion, DWORD _dwEndVersion, 	DWORD _dwMemSize)	:wTableType(_wTableType), dwRecordCount(_dwRecordCount),dwTableSize(_dwTableSize),	dwStartVersion(_dwStartVersion),dwEndVersion(_dwEndVersion),dwMemSize(_dwMemSize){}
	DWORD		dwTableType;				//子库类型
	DWORD		dwRecordCount;			//子库中病毒记录的个数
	DWORD		dwTableSize;			
	DWORD		dwStartVersion;
	DWORD		dwEndVersion;
	DWORD		dwTimeoutVersion;
//	DWORD		dwMemSize;
};


//----------------------------------------------------------------------------------
//表的对外接口
class IBaseTable : public IUnknown
{
public:
	//access
	STDMETHOD(Seek)		(IN DWORD dwPosition, IN DWORD dwOptions)	PURE;
	STDMETHOD_(DWORD, Read)		(IN VOID *lpDestBuffer, IN DWORD dwSizeToRead)	PURE;
	STDMETHOD_(DWORD, Write)	(IN VOID *lpSourBuffer, IN DWORD dwSizeToWrite)	PURE;

	//fixed interface
	//取记录大小/Get table size
	STDMETHOD_(DWORD,  GetTableType)	() PURE;
	STDMETHOD_(DWORD, GetRecCount)	() PURE;
	STDMETHOD_(DWORD, GetTableSize) () PURE;	
	STDMETHOD_(DWORD, GetBeginVer)  () PURE;
	STDMETHOD_(DWORD, GetEndVer)	() PURE;
	STDMETHOD(SetBeginVer)	(IN DWORD dwBeginVer) PURE;
	STDMETHOD(SetEndVer)	(IN DWORD dwEndVer) PURE;

public:
	RE_DECLARE_IID;
// 	enum
// 	{
// 		PROPID_TALBE_TABLETYPE,
// 		PROPID_TALBE_BEGINVER,
// 		PROPID_TABLE_ENDVER,
// 		PROPID_TABLE_ANALYSER,
// 		PROPID_RECORD_COUNT,
// 	};
} ;

// Modify End

RE_DEFINE_IID(IBaseTable, "{D71015D8-705C-427a-A419-08E4C2BC97A2}",
			  0xd71015d8, 0x705c, 0x427a, 0xa4, 0x19, 0x8, 0xe4, 0xc2, 0xbc, 0x97, 0xa2);

// {A83C31EB-E2D7-4cd6-8B6D-E7C91B0B0159}
DEFINE_GUID(PID_IBASETABLE_TABLETYPE, 0xa83c31eb, 0xe2d7, 0x4cd6, 0x8b, 0x6d, 0xe7, 0xc9, 0x1b, 0xb, 0x1, 0x59);
// {4913872F-4793-4a1a-8B5E-7D0EA5B21E82}
DEFINE_GUID(PID_IBASETALBE_BEGINVER, 0x4913872f, 0x4793, 0x4a1a, 0x8b, 0x5e, 0x7d, 0xe, 0xa5, 0xb2, 0x1e, 0x82);
// {5AE51827-2767-4fac-B0F3-F04A4ED6A1C9}
DEFINE_GUID(PID_IBASETABLE_ENDVER,  0x5ae51827, 0x2767, 0x4fac, 0xb0, 0xf3, 0xf0, 0x4a, 0x4e, 0xd6, 0xa1, 0xc9);
// {4386BC1B-98DC-4ca1-8543-191851B11F3F}
DEFINE_GUID(PID_IBASETABLE_ANALYSER, 0x4386bc1b, 0x98dc, 0x4ca1, 0x85, 0x43, 0x19, 0x18, 0x51, 0xb1, 0x1f, 0x3f);
// {885371EE-88AB-4b09-8740-936901DBDA70}
DEFINE_GUID(PID_IBASETABLE_RECORDCOUNT, 0x885371ee, 0x88ab, 0x4b09, 0x87, 0x40, 0x93, 0x69, 0x1, 0xdb, 0xda, 0x70);

//----------------------------------------------------------------------------------

enum LIBTABLESAVE_MOD
{
	LIBSAVE_DELAY,
	LIBSAVE_NODELAY,
};	

class ILibTable : public IBaseTable
{
public:
	//创建索引，创建到另外一个CBaseTable实例
	STDMETHOD(SaveNotify)	(IN DWORD dwOptions = LIBSAVE_DELAY) PURE;
	STDMETHOD_(VOID*, GetBase)	()	PURE;

public:
	RE_DECLARE_IID;
} ;
RE_DEFINE_IID(ILibTable, "{044E7022-68B7-4ef0-9FE2-12ACF1358D64}",
			   0x44e7022, 0x68b7, 0x4ef0, 0x9f, 0xe2, 0x12, 0xac, 0xf1, 0x35, 0x8d, 0x64);


//----------------------------------------------------------------------------------


class IPluginTable : public IBaseTable
{
public:
	STDMETHOD(GetUserData)	(void *pBuf, DWORD& dwBufSize) PURE;
	STDMETHOD(SetUserData)	(void *pBuf, DWORD dwBufSize)PURE;
	STDMETHOD(DumpToMem)	(BYTE* pData, INT nMaxLen)PURE;
	STDMETHOD(Append)		(BYTE *pRecord, INT nLen) PURE;
	STDMETHOD(GetFirst)		(BYTE* &pRecord, INT& nLen) PURE;
	STDMETHOD(GetNext)		(BYTE* &pRecord, INT& nLen) PURE;
	STDMETHOD(IsEof)		() PURE;
	STDMETHOD(GetAt)		(INT nNo, BYTE *pRecord, INT& nLen) PURE;
	//STDMETHOD(SetTableHeader)(BYTE *pRecord, INT nLen) PURE;
public:
	RE_DECLARE_IID;
// 	enum
// 	{
// 		PROPID_TABLE_HEADER=0x10,
// 	};
} ;
RE_DEFINE_IID(IPluginTable, "{B17C22AA-46AA-4e5b-8FE9-E0943ABE6EA3}",
			0xb17c22aa, 0x46aa, 0x4e5b, 0x8f, 0xe9, 0xe0, 0x94, 0x3a, 0xbe, 0x6e, 0xa3);



// {46401B5A-3618-4c0c-9A5D-2E6ECDC36992}
DEFINE_GUID(PID_IPLUGINTABLE_TABLEHEADER, 
			0x46401b5a, 0x3618, 0x4c0c, 0x9a, 0x5d, 0x2e, 0x6e, 0xcd, 0xc3, 0x69, 0x92);

// {BA6992D6-9E3A-4908-9A1C-F9E893A43F84}
DEFINE_GUID(PID_IPLUGINTABLE_SORTTYPE,0xba6992d6, 0x9e3a, 0x4908, 0x9a, 0x1c, 0xf9, 0xe8, 0x93, 0xa4, 0x3f, 0x84);
// {5EF9AB33-1924-4e64-9018-6DFFDE64FB77}
DEFINE_GUID(PID_IPLUGINTABLE_TEMPINDEXSIZE,0x5ef9ab33, 0x1924, 0x4e64, 0x90, 0x18, 0x6d, 0xff, 0xde, 0x64, 0xfb, 0x77);
// {164D586F-2754-4f55-A241-DB78FC48C495}
DEFINE_GUID(PID_IPLUGINTABLE_TEMPMEMSIZE,0x164d586f, 0x2754, 0x4f55, 0xa2, 0x41, 0xdb, 0x78, 0xfc, 0x48, 0xc4, 0x95);


//////////////////////////////////////////////////////////////////////////

#pragma pack(1)
typedef struct _STD_MPE_RECORD
{
	UINT32	UniqID;
	UINT16	Length;
} STD_MPE_RECORD, *LPSTD_MPE_RECORD;
#pragma pack()
//////////////////////////////////////////////////////////////////////////


#endif //__BASETABLE_H__


