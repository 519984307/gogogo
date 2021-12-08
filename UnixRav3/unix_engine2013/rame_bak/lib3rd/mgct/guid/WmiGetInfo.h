#pragma once

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include <atlbase.h>
#include <atlcomcli.h>
#include <map>
#include <string>
#define FAILEXIT(x, _h_r_) { if(!(x)) return _h_r_; }
//#define SUCCEDDED(hr)  ((HRESULT)hr>=S_OK)
#define DISK_SERIAL 0
#define CPU_ID		1
#define MAC_ADDRESS 2
class CWmiGetInfo
{
public:
	CWmiGetInfo(void);
	~CWmiGetInfo(void);
public:
	BOOL Init();
	HRESULT GetInfo(INT InfoId,BYTE* pBuf,BYTE &bBufSize);
private:
    CComPtr<IWbemLocator> m_cWbemLocator;
	CComPtr<IWbemServices> m_cWbemServices;
	BOOL m_bInit;
	HRESULT GetWmiInfo( LPCSTR lpszClass);
	HRESULT GetClassInfo(IWbemClassObject *pClassObject);
	BYTE StrToHex(const CHAR* p);
	std::map<std::string ,std::string> m_InfoMap;
};
