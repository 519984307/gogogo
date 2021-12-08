#include "StdAfx.h"
#include <Windows.h>
#include <WinCon.h>
#include <Iphlpapi.h>
#include ".\getnetadapterinfo.h"
#include ".\wmigetinfo.h"
#include "Rpcdce.h"
#pragma comment(lib, "Rpcrt4.lib")

// http://msdn.microsoft.com/en-us/library/aa366062(VS.85).aspx
// time_t在2008默认是64位，微软返回的是32位的
// When using Visual Studio 2005 and later,
// the time_t datatype defaults to an 8-byte datatype,
// not the 4-byte datatype used for the LeaseObtained and LeaseExpires members on a 32-bit platform.
#if _MSC_VER >= 1400
#define SIZEOF_IP_ADAPTER_INFO (sizeof(IP_ADAPTER_INFO) - 2 * (sizeof(time_t) - sizeof(__time32_t)))
#else
#define SIZEOF_IP_ADAPTER_INFO sizeof(IP_ADAPTER_INFO)
#endif

CGetNetAdapterInfo::CGetNetAdapterInfo(void)
{
	m_bIs9x=TRUE;
    m_bIs95=FALSE;

	m_hDll = ::LoadLibrary(_T("Iphlpapi.dll"));
	if (m_hDll)
	{
		(FARPROC&)m_pGetAdaptersInfo = ::GetProcAddress(m_hDll, "GetAdaptersInfo");
	}
	if (m_hDll == NULL || m_pGetAdaptersInfo == NULL)
	{
		m_bIs95=TRUE;
	}

	OSVERSIONINFO version;
	memset (&version, 0, sizeof (version));
	version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	GetVersionEx (&version);
	
	if (version.dwPlatformId==VER_PLATFORM_WIN32_NT) 
	{
		m_bIs9x=FALSE;
	}
	else
	{
		if((version.dwMajorVersion==4)&&(version.dwMinorVersion==0))
			m_bIs95=TRUE;	
	}	
}

CGetNetAdapterInfo::~CGetNetAdapterInfo(void)
{
	if (m_hDll) ::FreeLibrary(m_hDll);
}
INT CGetNetAdapterInfo::GetNetAdapterNum()
{
	ULONG ulLen=0;
	int iRet=0;
	DWORD dwError;
	
	
	
	if(m_bIs9x)
	{
		if(m_bIs95)
			return 1;
		dwError = m_pGetAdaptersInfo( NULL, &ulLen );

		if( dwError == ERROR_BUFFER_OVERFLOW ) 
		{
			iRet=ulLen/SIZEOF_IP_ADAPTER_INFO;
		}
		return iRet;
	}
	
	else
		return 1;

}
HRESULT CGetNetAdapterInfo::GetNetAdapterMac(INT iIndex,BYTE* pBuf,BYTE &bBufSize)
{
	ULONG ulLen=0;
	int iAdapterNum=0;
	DWORD dwError;
	IP_ADAPTER_INFO* pAdpInfo;

	if((pBuf==NULL)||(bBufSize<6))
	{
		bBufSize=6;
		return E_OUTOFMEMORY;
	}
/*
		if(!m_bIs9x)
		{
			CWmiGetInfo wmiGetInfo;
			return wmiGetInfo.GetInfo(MAC_ADDRESS,pBuf,bBufSize);
		}*/
	
	
	if(m_bIs95)
	{
		UUID uuid;
		uuid = GUID_NULL;
		UuidCreate( &uuid );    // 要求操作系统创建一个新的UUID	
		for (int i=2; i<8; i++) // 获取本机MAC地址 
			pBuf[i - 2] = uuid.Data4[i];
		bBufSize=6;
		return S_OK;

	}
	dwError = m_pGetAdaptersInfo( NULL, &ulLen );


	if( dwError == ERROR_BUFFER_OVERFLOW ) 
	{
		iAdapterNum=ulLen/SIZEOF_IP_ADAPTER_INFO;
	}
	else
		return E_FAIL;
	if(iIndex>=iAdapterNum)
		return E_FAIL;
	pAdpInfo=(IP_ADAPTER_INFO*)(new BYTE[ulLen]);
	dwError=m_pGetAdaptersInfo(pAdpInfo,&ulLen);
	if(dwError!=S_OK)
	{
		delete[] pAdpInfo;
		return E_FAIL;
	}
	if(pAdpInfo[iIndex].AddressLength<6)
	{
		return E_FAIL;
		delete[] pAdpInfo;
	}

	memcpy(pBuf,pAdpInfo[iIndex].Address,6);
	delete[] pAdpInfo;
	bBufSize=6;
	return S_OK;
}