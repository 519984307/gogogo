#pragma once
#include <Iphlpapi.h>
#include <list>
#include <atlconv.h>
#include <atlexcept.h>

#define MACADDRESS_BYTELEN      6   // MAC地址字节长度  
#define MACADDRESS_TYPELEN		32

typedef struct _T_MACADDRESS_ {  
	CHAR	NetCardType[MACADDRESS_TYPELEN];
	CHAR	NetCardName[MAX_PATH];
	CHAR	Description[MAX_PATH];
	CHAR	Manufacturer[MAX_PATH];
	BYTE    PermanentAddress[MACADDRESS_BYTELEN];   // 原生MAC地址  
	BYTE    MACAddress[MACADDRESS_BYTELEN];         // 当前MAC地址 

} T_MACADDRESS_;  

typedef struct networkCardInfo_ {
	CHAR	NetCardType[MACADDRESS_TYPELEN];
	CHAR	NetCardName[MAX_PATH];
	CHAR	Description[MAX_PATH];
	CHAR	Manufacturer[MAX_PATH];
	CHAR	MacAddress[MAX_PATH];
	BYTE    PermanentAddress[MACADDRESS_BYTELEN];   // 原生MAC地址  
	BYTE    CurrentAddress[MACADDRESS_BYTELEN];         // 当前MAC地址 
	DWORD   MacAddrAsc2;
} NETCARD_INFO_,* PNETCARD_INFO_;  

class CGetNetAdapterInfo2
{
public:
	CGetNetAdapterInfo2(void);
	~CGetNetAdapterInfo2(void);

	BOOL GetNetAdapterMac(BYTE* pBuf, BYTE &bBufSize, BOOL bReGet = FALSE /*是否重新获取网卡列表*/);
	BOOL VerifyNetAdapterMac(BYTE* pBuf, DWORD dwBufSize, BOOL bReGet = FALSE /*是否重新获取网卡列表*/);

private:
	BOOL GetNetAdapters(VOID);
	
	static std::list<NETCARD_INFO_> s_listOfAdaptaer;
};
