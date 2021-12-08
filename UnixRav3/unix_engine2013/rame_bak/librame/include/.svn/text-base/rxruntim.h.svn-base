#ifndef __RX_RUN_TIME__
#define __RX_RUN_TIME__

#include "rxiface.h"

// COM
HRESULT STDAPICALLTYPE RoCreateInstanceNCT( LPCSTR progid, IUnknown* outer, REFIID riid, void** ppv, IUnknown* rot = 0 );
HRESULT STDAPICALLTYPE RoCreateInstance( TCLSIDREF rclsid, IUnknown* outer, REFIID riid, void** ppv, IUnknown* rot = 0 );
HRESULT STDAPICALLTYPE RoCreateInstanceEx( TCLSIDREF rcatid, DWORD idcls, IUnknown* outer, REFIID riid, void** ppv, IUnknown* rot = 0 );
HRESULT STDAPICALLTYPE RoRegisterClass( TCLSIDREF rclsid, HMODULE hModule );
HRESULT STDAPICALLTYPE RoRevokeClass( TCLSIDREF rclsid );
// Variant
HRESULT STDAPICALLTYPE RoSetVariant( PROPVARIANT* propOut, const PROPVARIANT*  propIn );
HRESULT STDAPICALLTYPE RoClearVariant( PROPVARIANT* propIn );
// MSS Lib
HRESULT STDAPICALLTYPE MssLoadLibFile( LPCSTR wFileName, IMssLibFile** ppLibFile, IUnknown* prot );

HRESULT STDAPICALLTYPE MssLoadLibFile2W( LPCWSTR wFileName, LPCSTR aObNamePrefix, IMssLibFile** ppLibFile, IUnknown* prot );

HRESULT STDAPICALLTYPE MssLoadLibFile2( LPCSTR wFileName, LPCSTR aObNamePrefix, IMssLibFile** ppLibFile, IUnknown* prot );

enum  LOADOPTS
{
	MSFLOAD_DIRECT		= 1,		// 直接使用文件加载，库文件必须是不压缩的；库文件将按
	MSFLOAD_EXCLUSIVE	= 2,		// 依赖MSFLOAD_DIRECT，有此位表明则以独占方式打开库文件
};

typedef struct MSF_LOAD_PARAM
{
	SIZE_T		cbStruct;
	LPCSTR		aFileName;			// 原始病毒库文件所在路径
	LPCSTR		aDriectFile;		// 存放用于直接加载的影子文件（不压缩）
									// 不指定的情况下，使用aFileName创建内存影子文件后加载
									// 指定后，使用该文件加载，但不会更新VCCB
									// 如果使用了aShadowFile加载，当VCCB更新时，此
	IUnknown*	oLibCallback;		// 
	IUnknown*	oObjTable;			// 
	LPCSTR		aObNamePrefix;		// Mutex/Section... Name-Prefix

	OUT	BOOL	aDirectFileUsed;	// 
} MLPARAM, *LPMLPARAM;

HRESULT STDAPICALLTYPE MssLoadLibFileEx( LPMLPARAM lpParam, IMssLibFile** ppLibFile );

HRESULT STDAPICALLTYPE MssCreateInstance( TCLSIDREF rclsid, IUnknown* outer, REFIID riid, void** ppv, IUnknown* rot );

HANDLE	STDAPICALLTYPE MssCreateMutexOfFile( LPCWSTR wFileName, BOOL bInitOwner, LPSECURITY_ATTRIBUTES psa );

HRESULT STDAPICALLTYPE MssSetCacheSize( SIZE_T uCacheBytes );

// 2010
HRESULT STDAPICALLTYPE REInitROT10( IREROT* lpROT, ILibLoader* lpLibLoader, IREProperty2** ppConfig );
HRESULT STDAPICALLTYPE RECreateInstance( REFCLSID clsid, IUnknown* outer, REFIID riid, void ** ppv, IUnknown* prot );
// Home
LPCSTR	STDAPICALLTYPE RxHome();		// return rxtuntim path
LPCWSTR	STDAPICALLTYPE RxHomeW();		// return rxtuntim path
VOID	STDAPICALLTYPE RxSetHome( LPCSTR aHome );
#endif