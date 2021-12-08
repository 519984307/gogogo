#ifndef __I_RS_ODB__
#define __I_RS_ODB__

typedef struct DB_OPTION
{
	UINT	Capacity;
	UINT	CacheSize;
	UINT	Compress;
	UINT	FlushTime;
} DB_OPTION, *LPDB_OPTION;

class IODBCreate : public IUnknown
{
public:
	STDMETHOD(Create)( LPCSTR aFileName, SIZE_T cbInMem, LPDB_OPTION lpOption ) PURE;
	STDMETHOD(Open)( LPCSTR aFileName, SIZE_T cbInMem, LPDB_OPTION lpOption ) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Remove)() PURE;
	RE_DECLARE_IID;
};

// {FA752082-A90C-4513-A8FE-FC58281CFB14}
RE_DEFINE_IID( IODBCreate, "{FA752082-A90C-4513-A8FE-FC58281CFB14}",
			  0xfa752082, 0xa90c, 0x4513, 0xa8, 0xfe, 0xfc, 0x58, 0x28, 0x1c, 0xfb, 0x14);

//////////////////////////////////////////////////////////////////////////

typedef const RX_BLOB&	REFCBLOB;
typedef RX_BLOB&		REFBLOB;

class IODBAccess : public IUnknown
{
public:
	STDMETHOD(SetProp)( REFCBLOB rKey, LPCSTR aAttr, REFCBLOB rValue ) PURE;
	STDMETHOD(GetProp)( REFCBLOB rKey, LPCSTR aAttr, REFBLOB rValue ) PURE;
	STDMETHOD(DelProp)( REFCBLOB rKey, LPCSTR aAttr ) PURE;
	STDMETHOD(DelPropEx)( REFCBLOB rKey ) PURE;
	STDMETHOD(FlushProp)( BOOL bDiskFlush ) PURE;
	RE_DECLARE_IID;
};

// {AD8A4D78-04C0-44dc-8409-B909BEA6EA3F}
RE_DEFINE_IID( IODBAccess, "{AD8A4D78-04C0-44dc-8409-B909BEA6EA3F}",
			  0xad8a4d78, 0x4c0, 0x44dc, 0x84, 0x9, 0xb9, 0x9, 0xbe, 0xa6, 0xea, 0x3f);

//////////////////////////////////////////////////////////////////////////

class IODBServer : public IUnknown
{
public:
	STDMETHOD( Manage )( IODBAccess* odb, LPCWSTR lpcAlais ) PURE;
	STDMETHOD( UnManage )( LPCWSTR lpcAlais, IODBAccess** odb ) PURE;
	RE_DECLARE_IID;
};

// {FDB5079D-BF5D-4174-8F3B-019E9D0A6484}
RE_DEFINE_IID( IODBServer, "{FDB5079D-BF5D-4174-8F3B-019E9D0A6484}",
			0xfdb5079d, 0xbf5d, 0x4174, 0x8f, 0x3b, 0x1, 0x9e, 0x9d, 0xa, 0x64, 0x84);


class IODBClient : public IUnknown
{
public:
	STDMETHOD( Open )( LPCWSTR wServerPoint, LPCWSTR wAlais ) PURE;
	STDMETHOD( Close )() PURE;
	RE_DECLARE_IID;
};

// {4AB43C70-27E7-4fd4-8334-604A62E6DA4D}
RE_DEFINE_IID( IODBClient, "{4AB43C70-27E7-4fd4-8334-604A62E6DA4D}",
			0x4ab43c70, 0x27e7, 0x4fd4, 0x83, 0x34, 0x60, 0x4a, 0x62, 0xe6, 0xda, 0x4d);


#endif