////////////////////////////////////////////////////////////////////////////////
/**
* @file
* 文件简要说明
* <p>项目名称：瑞星单机2009版
* <br>文件名称：userguid2.h
* <br>实现功能：
* <br>作    者：hyj
* <br>编写日期：
* <br>
* <br>修改历史（自下而上填写 内容包括：日期  修改人  修改说明）
* 基于云体验数据提炼的新GUID算法@shangsh
*/
////////////////////////////////////////////////////////////////////////////////

#if !defined(_INCLUDE_USERGUID_H_)
#define _INCLUDE_USERGUID_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <stdlib.h>
#include <stdio.h>		// for _snscanf
#include <time.h>

#define g_szHostClsid			   _T("{CAA2D3B1-4BB5-4a45-A17A-122773379D99}")
#define g_szHostKeyid		_T("CLSID\\{CAA2D3B1-4BB5-4a45-A17A-122773379D99}")
//杀软使用
#define g_szHostValueRav		   _T("AppID")		
//卡卡使用
#define g_szHostValueKaka		   _T("IID")
//防火墙使用
#define g_szHostValudeRfw         _T("CLSID")
//公共组件
#define g_szHostValudeCommon         _T("ProcID")

#include "GetNetAdapterInfo.h"
#ifndef RS_GUID_VER_CUR
#define RS_GUID_VER_CUR 1
#endif

#define RS_GUID_VER_1 1
#define RS_GUID_VER_2 2

#if RS_GUID_VER_CUR > RS_GUID_VER_1
#pragma message("Attention！！你正在使用新版GUID算法")
#include "GetNetAdapterInfo2.h"
#endif

#include "GetDiskInfo.h"
#include "WmiGetInfo.h"


static USHORT CalcCheckSum(PBYTE pData, DWORD dwSize)
{
	USHORT uSum = 0;

	for(DWORD dw = 0; dw < dwSize; ++dw)
		uSum += pData[dw];

	return uSum;
}

static BOOL GenerialGuid(GUID *guid, const USHORT uGuidVer = RS_GUID_VER_CUR)
{
	if (guid == NULL)
		return FALSE;

	CGetDiskInfo cDiskInfo;
	INT iCount=0,iSize=0;
	BYTE bSize=0;
	BYTE pszDiskSerial[100];
	memset(guid,0,sizeof(GUID));
	bSize=sizeof(GUID);

	if (uGuidVer == RS_GUID_VER_1)
	{
		CGetNetAdapterInfo cAdapter;
		iCount = cAdapter.GetNetAdapterNum();
		if(iCount)
		{
			if(S_OK==cAdapter.GetNetAdapterMac(0,(BYTE*)guid,bSize))
				iSize=bSize;
		}
	}
#if RS_GUID_VER_CUR > RS_GUID_VER_1
	else
	{
		CGetNetAdapterInfo2 cAdapter2;
		if(cAdapter2.GetNetAdapterMac((BYTE*)guid, bSize, TRUE))
			iSize = bSize;
	}
#endif

	iCount=cDiskInfo.GetDriverNum();
	if(iCount > 0)
	{
		bSize=100;
		memset(pszDiskSerial, 0x00, bSize);
		if(S_OK==cDiskInfo.GetDriverSerial(0,pszDiskSerial,bSize))
		{
			int nCopySize = ((sizeof(GUID)-iSize)>bSize)?bSize:(sizeof(GUID)-iSize);
			memcpy((BYTE*)guid+iSize,pszDiskSerial,nCopySize);
			iSize += nCopySize;
		}
	}
	if(iSize<sizeof(GUID))
	{
		CWmiGetInfo wmiGetInfo;
		bSize=100;
		memset(pszDiskSerial, 0x00, bSize);
		if(S_OK==wmiGetInfo.GetInfo(CPU_ID,pszDiskSerial,bSize))
		{
			//最多取cpuid的后两位，发现一般cpuid不同仅仅是后两位，其他的不要 add by ysb 09-1-16
			int nOffset = bSize>2?bSize-2:0;
			bSize = bSize-nOffset;

			int nCopySize = ((sizeof(GUID)-iSize)>bSize)?bSize:(sizeof(GUID)-iSize);
			memcpy((BYTE*)guid+iSize,pszDiskSerial+nOffset,((sizeof(GUID)-iSize)>bSize)?bSize:(sizeof(GUID)-iSize));
			iSize += nCopySize;
		}
	}
	guid->Data1^=0xf07f5f46;
	guid->Data2^=0x1234;
	guid->Data3^=0x5678;

	if (uGuidVer != RS_GUID_VER_1)
	{
		// 校验和只计算除去最后两个字节之前的数据
		DWORD dwCheckLen = sizeof(GUID) - sizeof(USHORT);
		USHORT uCheckSum = CalcCheckSum((PBYTE)guid, dwCheckLen);
		uCheckSum = uCheckSum - uGuidVer;
		memcpy((PBYTE)guid + dwCheckLen , &uCheckSum, sizeof(USHORT));
	}

	return TRUE;
}

#if RS_GUID_VER_CUR > RS_GUID_VER_1
static BOOL VerifyGuid(const DWORD dwGuidVer, const GUID& guid, BOOL bReGet = FALSE)
{
	if (dwGuidVer != RS_GUID_VER_1)
	{
		GUID src_guid = guid;

		// 先校验版本号是否正确
		DWORD dwCheckLen = sizeof(GUID) - sizeof(USHORT);
		USHORT uCurCheckSum = 0;
		memcpy(&uCurCheckSum, (PBYTE)&src_guid + dwCheckLen, sizeof(USHORT));
		USHORT uNewCheckSum = CalcCheckSum((PBYTE)&src_guid, dwCheckLen);
		
		if ((dwGuidVer + uCurCheckSum) != uNewCheckSum)
		{
			return FALSE;
		}
		
		// 再校验MAC地址是否正确
		src_guid.Data1 ^= 0xf07f5f46;
		src_guid.Data2 ^= 0x1234;
		src_guid.Data3 ^= 0x5678;

		CGetNetAdapterInfo2 cAdapter2;
		return cAdapter2.VerifyNetAdapterMac((PBYTE)&src_guid, MACADDRESS_BYTELEN, bReGet);
	}
	
	return TRUE;
}
#endif
class CUserGUID2
{
public:
	CUserGUID2(LPCTSTR szHostValue = g_szHostValueRav):m_szHostValue(g_szHostValudeCommon){}

	//ravtask程序需要读，读不到需要创建
	GUID TryGetUserGUID(DWORD dwVer = RS_GUID_VER_CUR, BOOL bValidate = FALSE)
	{
		GUID guid = ReadRegGuid();
		if(guid == GUID_NULL)
		{
			printf("create guid\n");
			guid = CreateUserGUID(dwVer);
			if(!WriteRegGuid(guid))
			{
				return GUID_NULL;
			}
		}
#if RS_GUID_VER_CUR > RS_GUID_VER_1
		else
		{
			// 如果不是老版本的话，则进行版本校验。然后进行MAC地址校验
			if (dwVer != RS_GUID_VER_1)
			{
				if (bValidate) 
				{
					if (!VerifyGuid(dwVer, guid))
					{
						guid = CreateUserGUID(dwVer);
						if(!WriteRegGuid(guid))
						{
							return GUID_NULL;
						}
					}
				}
			}
		}
#endif

		m_guid=guid;
		return m_guid;
	}

	//升级程序需要读一下有没有
	inline BOOL GetcurrentUserGUID()
	{
		m_guid = ReadRegGuid();
		return m_guid!=GUID_NULL;
	}

	//网站需要的32个字符串格式
	BOOL FormatString(OUT LPSTR lpsz, IN INT iLen)
	{
		_snprintf(lpsz, iLen, "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", m_guid.Data1, m_guid.Data2, m_guid.Data3, 
			m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3], 
			m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);		
		return TRUE;
	}

	inline GUID UserGUID(){return m_guid;}

	BOOL myStringFromCLSID(IN REFCLSID rclsid, OUT LPTSTR lpsz, IN INT iLen)
	{
		_sntprintf(lpsz, iLen, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"), rclsid.Data1, rclsid.Data2, rclsid.Data3, 
			rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3], 
			rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);		
		return TRUE;
	}

	BOOL myCLSIDFromString(IN LPCTSTR lpsz, OUT LPCLSID pclsid)
	{
		if(lstrlen(lpsz)!=38) return FALSE;

		long  Data234[ 10 ] = {0};
		GUID rclsid = GUID_NULL;
#if     _MSC_VER > 1300			//(VC7.0)
		_sntscanf(lpsz, lstrlen(lpsz), _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"), &rclsid.Data1,
#else
		_stscanf(lpsz, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"), &rclsid.Data1,
#endif
			Data234, Data234+1,
			Data234+2, Data234+3, Data234+4, Data234+5, Data234+6, Data234+7, Data234+8, Data234+9);
		// 搞不懂为啥会越界了， 所以只好用long先读出来
		//&rclsid.Data2, &rclsid.Data3, 
		//(unsigned char*)&rclsid.Data4[0], (unsigned char*)&rclsid.Data4[1], (unsigned char*)&rclsid.Data4[2], (unsigned char*)&rclsid.Data4[3], 
		//(unsigned char*)&rclsid.Data4[4], (unsigned char*)&rclsid.Data4[5], (unsigned char*)&rclsid.Data4[6], (unsigned char*)&rclsid.Data4[7]);
		rclsid.Data2 = (unsigned short)Data234[0];
		rclsid.Data3 = (unsigned short)Data234[1];
		rclsid.Data4[0] = (unsigned char)Data234[2];
		rclsid.Data4[1] = (unsigned char)Data234[3];
		rclsid.Data4[2] = (unsigned char)Data234[4];
		rclsid.Data4[3] = (unsigned char)Data234[5];
		rclsid.Data4[4] = (unsigned char)Data234[6];
		rclsid.Data4[5] = (unsigned char)Data234[7];
		rclsid.Data4[6] = (unsigned char)Data234[8];
		rclsid.Data4[7] = (unsigned char)Data234[9];		

		*pclsid = rclsid;
		return TRUE;
	}

public:

	GUID CreateUserGUID(const DWORD dwGuidVer = RS_GUID_VER_1)
	{
		GUID guid = GUID_NULL;	
		GenerialGuid(&guid, dwGuidVer);
		return guid;
	}

	GUID ReadRegGuid()
	{
		GUID guid = GUID_NULL;

		HKEY hkKey;
		long lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, g_szHostKeyid, 0, KEY_READ,&hkKey);
		if(ERROR_SUCCESS == lResult)
		{
			TCHAR szValue[MAX_PATH]={0};
			DWORD dwRead=MAX_PATH;
			DWORD dwType = NULL;
			if(ERROR_SUCCESS==RegQueryValueEx(hkKey, m_szHostValue, NULL, &dwType, (LPBYTE)szValue, &dwRead))
			{
				myCLSIDFromString(szValue, &guid);				
			}
			lResult = RegCloseKey(hkKey);
			return guid;
		}

		return guid;
	}

	BOOL WriteRegGuid(GUID guid)
	{
		ATLASSERT(guid!=GUID_NULL);

		TCHAR szValue[MAX_PATH]={0};
		if(myStringFromCLSID(guid, szValue, MAX_PATH))
		{
			long lResult;
			HKEY hkKey;
			lResult = RegOpenKey(HKEY_CLASSES_ROOT, g_szHostKeyid, &hkKey);
			if(ERROR_SUCCESS != lResult)
			{
				lResult = RegCreateKey(HKEY_CLASSES_ROOT, g_szHostKeyid, &hkKey);
				if(ERROR_SUCCESS != lResult)	// Create Key error 
				{	
					return FALSE;
				}
			}

			BOOL bRetVal = TRUE;
			DWORD dwLength = (_tcslen(szValue) + 1) * sizeof(TCHAR);
			lResult = RegSetValueEx(hkKey, m_szHostValue , 0, REG_SZ, (LPBYTE)szValue, dwLength);
			if(ERROR_SUCCESS != lResult)
			{
				bRetVal = FALSE;
			}

			lResult = RegCloseKey(hkKey);
			return bRetVal;
		}
		return FALSE;
	}

private:
	GUID m_guid;
	LPCTSTR m_szHostValue;
};


#endif // !defined(_INCLUDE_USERGUID_H_)