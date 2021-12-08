#pragma once
#include <Iphlpapi.h>
class CGetNetAdapterInfo
{
public:
	CGetNetAdapterInfo(void);
	~CGetNetAdapterInfo(void);
	INT GetNetAdapterNum();
	HRESULT GetNetAdapterMac( INT iIndex, BYTE* pBuf, BYTE &bBufSize);
private:
	BOOL m_bIs9x;
	BOOL m_bIs95;
	DWORD (WINAPI *m_pGetAdaptersInfo)(
		PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen
		);
	HMODULE m_hDll;
};
