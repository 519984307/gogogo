#ifndef __LIB_RAPI_INTERFACE__
#define __LIB_RAPI_INTERFACE__

#include "refable.h"

enum CF_DISPOSITION
{
	CFM_DISPOSITION			= 0x0000FFFF,
	CF_CREATE_NEW			= 0x00000002, //FILE_CREATE,
	CF_OPEN_EXISTING		= 0x00000001, //FILE_OPEN,
	CF_OPEN_ALWAYS			= 0x00000003, //FILE_OPEN_IF,
	CF_TRUNCATE_EXISTING	= 0x00000004, //FILE_OVERWRITE,
	CF_CREATE_ALWAYS		= 0x00000005, //FILE_OVERWRITE_IF,
	//////////////////////////////////////////////////////////////////////////
	CFM_ATTR				= 0xFFFF0000,
	CF_QA					= 0x00000000,
	CF_READ					= 0x00010000,
	CF_WRITE				= 0x00020000,
	CF_WRITECOPY			= 0x10000000,
	CF_NOBUFFERING			= 0x20000000,
};

enum OBJECT_TYPE
{
	kFileObject,
	kVolumeObject,
	kMapObject,
	kFindObject,

	kAnyObject = -1,
};

struct QObject : IRefable
{
	virtual OBJECT_TYPE GetObjectType() = 0;
	virtual VOID Close() = 0;
};

struct IIoFileObject;

struct IIoMapObject : QObject
{
	virtual HANDLE	GetOsHandle() = 0;
	virtual HRESULT Create( HANDLE hFile, UINT64 uMax, DWORD fProtect ) = 0;
	virtual HRESULT Create( IIoFileObject* pfo, UINT64 uMax, DWORD fProtect ) = 0;
	virtual HRESULT GetFileObject( IIoFileObject** ppFileObj ) = 0;
	virtual DWORD	ProtectMode() = 0;
	virtual LPCVOID	MapView( DWORD dwAccess, UINT64 uOffset, SIZE_T cbMap ) = 0;
	virtual HRESULT	UnmapView( LPCVOID lpView ) = 0;

};

enum
{
	IO_TECH_WIN32	= 1,
	IO_TECH_RSU		= 2,
	IO_TECH_FSP		= 4,
	IO_TECH_ALL		= -1,
};

struct IIoFileObject : QObject
{
	virtual HANDLE	GetOsHandle() = 0;
	virtual LPCWSTR FileName() = 0;
	virtual DWORD	OpenMode() = 0;
	virtual HRESULT Create( LPCWSTR wFileName, DWORD aCreationFlags, DWORD IoTech = IO_TECH_ALL ) = 0;
	virtual HRESULT ReOpen( DWORD dwAccess ) = 0;
	virtual HRESULT Read( LPVOID lpBuffer, SIZE_T cbData, SIZE_T& cbRead ) = 0;
	virtual	HRESULT Write( LPCVOID lpBuffer, SIZE_T cbData, SIZE_T & cbWritten ) = 0;
	virtual	HRESULT Flush() = 0;
	virtual	HRESULT SetEOF() = 0;
	virtual	HRESULT Seek( INT64 uOffset, INT dwMethod ) = 0;
	virtual	HRESULT Tell( UINT64 & uPointer ) = 0;
	virtual	HRESULT GetSize( UINT64 & uFileSize ) = 0;
	virtual HRESULT Remove( BOOL bForce ) = 0;
	virtual HRESULT CreateMap( DWORD dwAccess, UINT64 uMaxSize, IIoMapObject** ppMap, BOOL bRefFileObj = FALSE ) = 0;
};

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI RCreateFile( LPCWSTR wFileName, DWORD dwCreationFlags, IIoFileObject** ppFileObject, UINT32 uTechMask = IO_TECH_ALL );


//////////////////////////////////////////////////////////////////////////

typedef HRESULT ( WINAPI * RCreateFilePtr )( LPCWSTR , DWORD , IIoFileObject** , UINT32 );

#endif