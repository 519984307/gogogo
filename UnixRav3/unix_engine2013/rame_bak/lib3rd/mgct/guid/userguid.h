////////////////////////////////////////////////////////////////////////////////
/**
* @file
* 文件简要说明
* <p>项目名称：瑞星单机2008版杀毒软件
* <br>文件名称：userguid.h
* <br>实现功能：
* <br>作    者：onlyu
* <br>编写日期：2007-8-29 19:20:13
* <br>
* <br>修改历史（自下而上填写 内容包括：日期  修改人  修改说明）


０８使用的老版本，０９不再使用了！！！
０９请使用	userguid2.h



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

#define g_szHostClsid			   "{CAA2D3B1-4BB5-4a45-A17A-122773379D99}"
#define g_szHostKeyid		"CLSID\\{CAA2D3B1-4BB5-4a45-A17A-122773379D99}"
//杀软使用
#define g_szHostValueRav		   "AppID"		
//卡卡使用
#define g_szHostValueKaka		   "IID"
//防火墙使用
#define g_szHostValudeRfw         "CLSID"

class CUserGUID
{
public:
	CUserGUID(LPCSTR szHostValue = g_szHostValueRav):m_szHostValue(szHostValue){}

	//ravtask程序需要读，读不到需要创建
	GUID TryGetUserGUID()
	{
		GUID guid = ReadRegGuid();
		if(guid==GUID_NULL)
		{
			guid = CreateUserGUID();
			if(!WriteRegGuid(guid))
			{
				return GUID_NULL;
			}
		}
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

	BOOL myStringFromCLSID(IN REFCLSID rclsid, OUT LPSTR lpsz, IN INT iLen)
	{
		_snprintf(lpsz, iLen, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", rclsid.Data1, rclsid.Data2, rclsid.Data3, 
			rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3], 
			rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);		
		return TRUE;
	}

	BOOL myCLSIDFromString(IN LPCSTR lpsz, OUT LPCLSID pclsid)
	{
		if(lstrlen(lpsz)!=38) return FALSE;

		long  Data234[ 10 ] = {0}; 
		GUID rclsid = GUID_NULL;
#if     _MSC_VER > 1300			//(VC7.0)
		_snscanf(lpsz, lstrlen(lpsz), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", &rclsid.Data1,
#else
		_stscanf(lpsz, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", &rclsid.Data1,
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

protected:
	GUID CreateUserGUID()
	{
		srand(time(NULL));

		GUID guid = GUID_NULL;		
		::CoCreateGuid(&guid); 
		if (guid == GUID_NULL) 
		{ 
			guid.Data1 = rand();
			guid.Data2 = rand();
			guid.Data3 = rand();
			*(int*)guid.Data4 = rand();
			*(int*)(guid.Data4+4) = rand();
		}
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
			DWORD dwLength = strlen(szValue) + 1;
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
	LPCSTR m_szHostValue;
};


#endif // !defined(_INCLUDE_USERGUID_H_)