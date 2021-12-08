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
*
*		
*		注意使用此头文件时，必须也包含RavBase64.cpp
*/
////////////////////////////////////////////////////////////////////////////////

#if !defined(_INCLUDE_DETECTPRODUCT_H_)
#define _INCLUDE_DETECTPRODUCT_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <atlbase.h>
#include "RavBase64.h"


class CRegEncode
{
public:
	int Encode(LPCSTR pstr, LPSTR buf, INT len)
	{
		if(lstrlen(pstr)*4/3>=len) return 0;

		CRavBase64 base64;
		int iRet = base64.EncodeBase64((LPSTR)pstr, buf, lstrlen(pstr));
		ATLTRACE("Encode [%d->%d] %s\n", lstrlen(pstr), iRet, buf);
		ATLASSERT(iRet<len);
		return iRet;
	}
	int Decode(LPCSTR pstr, LPSTR buf, INT len)
	{		
		if(lstrlen(pstr)*3/4>=len) return 0;

		CRavBase64 base64;
		int iRet = base64.DecodeBase64((LPSTR)pstr, buf);
		ATLTRACE("Decode [%d->%d] %s\n", lstrlen(pstr), iRet, buf);
		ATLASSERT(iRet<len);		
		return iRet;
	}
	BOOL Verify(char* pstr, char* pstr1)
	{
		ATLASSERT(lstrlen(pstr)<260);

		CHAR buf[512];
		CHAR buf1[512];
		CRavBase64 base64;

		ZeroMemory(buf, sizeof(buf));
		int iRet = base64.EncodeBase64(pstr, buf, lstrlen(pstr));
		ATLTRACE("[%d->%d] %s\n", lstrlen(pstr), iRet, buf);
		ATLASSERT(iRet<512);

		ZeroMemory(buf1, sizeof(buf1));
		iRet = base64.DecodeBase64(pstr1, buf1);
		ATLTRACE("[%d->%d] %s\n", lstrlen(pstr1), iRet, buf1);
		ATLASSERT(iRet<512);
		ATLASSERT(lstrcmp(pstr, buf1)==0);
		return iRet>0;
	}
};

class CDetectProduct
{
public:
	//最多检测 31 个产品
	enum
	{
		PRODUCT_EMPTY			= 0,
		PRODUCT_SAFE360Kill		= 0x0001,			//360杀毒
		PRODUCT_SAFE360			= 0x0002,			//360安全卫士
		PRODUCT_SAFE360Explorer	= 0x0004,			//360浏览器
		PRODUCT_SAFE360BOX		= 0x0008,			//360保险箱

		PRODUCT_KAKA			= 0x0010,			//瑞星卡卡
		PRODUCT_KINGSOFTANTISPY = 0x0020,			//金山清理专家
		PRODUCT_VMWARE			= 0x0040,			//VMware
		PRODUCT_RAV				= 0x0080,			//瑞星杀毒
		PRODUCT_RFW				= 0x0100			//瑞星防火墙
	};

	enum
	{
		OS_WIN98         = 0x10000,
		OS_WINNT         = 0x20000,
		OS_WIN2000       = 0x30000,
		OS_WINXP         = 0x40000,
		OS_WIN2003       = 0x50000,
		OS_WINVISTA      = 0x60000,
		OS_WIN7          = 0x70000,
		OS_WIN2008       = 0x80000,
	};

	static BOOL IsKakaInstalled()
	{
		//卡卡3查CURRENT_USER,并且文件ras.exe是否存在
		CRegKey reg;			
		if (reg.Open(HKEY_CURRENT_USER, "Software\\Rising\\KaKaToolBar", KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "path", &dwLength) == ERROR_SUCCESS)
			{
				lstrcat(szInstallPath, "\\ras.exe");
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}

		//卡卡4查 HKEY_LOCAL_MACHINE,并且文件ras.exe是否存在
		if (reg.Open(HKEY_LOCAL_MACHINE, "Software\\Rising\\KaKaToolBar", KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "installpath", &dwLength) == ERROR_SUCCESS)
			{
				lstrcat(szInstallPath, "\\ras.exe");
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}

		//卡卡6查 HKEY_LOCAL_MACHINE,并且文件ras.exe是否存在
		if (reg.Open(HKEY_LOCAL_MACHINE, "Software\\Rising\\KaKa", KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "installpath", &dwLength) == ERROR_SUCCESS)
			{
				lstrcat(szInstallPath, "\\ras.exe");
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	static BOOL IsRavInstalled()
	{
		//卡卡3查CURRENT_USER,并且文件ras.exe是否存在
		CRegKey reg;			
		if (reg.Open(HKEY_LOCAL_MACHINE, "Software\\Rising\\Rav", KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "InstallPath", &dwLength) == ERROR_SUCCESS)
			{
				lstrcat(szInstallPath, "\\rav.exe");
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}		
		
		return FALSE;
	}

	static BOOL IsRfwInstalled()
	{
		//卡卡3查CURRENT_USER,并且文件ras.exe是否存在
		CRegKey reg;			
		if (reg.Open(HKEY_LOCAL_MACHINE, "Software\\Rising\\Rfw", KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "InstallPath", &dwLength) == ERROR_SUCCESS)
			{
				lstrcat(szInstallPath, "\\rfwcfg.exe");
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}		
		
		return FALSE;
	}

	/*
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\360安全卫士]
	"DisplayName"="奇虎360安全卫士"
	"UninstallString"="C:\\Program Files\\360safe\\uninst.exe"
	"DisplayIcon"="C:\\Program Files\\360safe\\360safe.exe"
	"DisplayVersion"="3.6"
	"URLInfoAbout"="http://www.360safe.com"
	"Publisher"="奇虎网"
	*/
	static BOOL IsSafe360Installed()
	{
		CHAR buf[256];
		CRegEncode base;

		//"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\360安全卫士"
		ZeroMemory(buf, sizeof(buf));
		base.Decode("U29mdHdhcmVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXDM2MLCyyKvOwMq/", buf, sizeof(buf));

		//360安全卫士 查HKEY_LOCAL_MACHINE,并且文件360safe.exe是否存在
		CRegKey reg;			
		if (reg.Open(HKEY_LOCAL_MACHINE, buf, KEY_READ) == ERROR_SUCCESS)
		{
			ZeroMemory(buf, sizeof(buf));

			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "DisplayIcon", &dwLength) == ERROR_SUCCESS)
			{			
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				ZeroMemory(szInstallPath, sizeof(szInstallPath));
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}
		ZeroMemory(buf, sizeof(buf));
		return FALSE;
	}


	//[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\360SD]
	//"DisplayName"="360杀毒"
	//	"UninstallString"="C:\\Program Files\\360\\360sd\\uninst.exe"
	//	"DisplayIcon"="C:\\Program Files\\360\\360sd\\360sd.exe"
	//	"DisplayVersion"="1.0.0.1069"
	//	"URLInfoAbout"="http://www.360.cn"
	//	"Publisher"="360安全中心"
	//	"${PRODUCT_STARTMENU_REGVAL}"="360杀毒"
	static BOOL IsSafe360KillInstalled()
	{
		CHAR buf[256];
		CRegEncode base;

		//char szReg[256] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\360SD";
		//ZeroMemory(buf, sizeof(buf));
		//base.Encode(szReg,buf,sizeof(buf));
	
		base.Decode("U09GVFdBUkVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXDM2MFNE", buf, sizeof(buf));

		//360杀毒 查HKEY_LOCAL_MACHINE,并且文件360sd.exe是否存在
		CRegKey reg;			
		if (reg.Open(HKEY_LOCAL_MACHINE, buf, KEY_READ) == ERROR_SUCCESS)
		{
			ZeroMemory(buf, sizeof(buf));

			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "DisplayIcon", &dwLength) == ERROR_SUCCESS)
			{			
				HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				ZeroMemory(szInstallPath, sizeof(szInstallPath));
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}
		ZeroMemory(buf, sizeof(buf));
		return FALSE;
	}

	//[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{23F3F476-BE34-4f48-9C77-2806A8393EC4}]
	//"DisplayName"="360安全浏览器 3.1 Beta2版"
	//	"DisplayIcon"="C:\\Program Files\\360\\360se3\\360SE.exe"
	//	"UninstallString"="C:\\Program Files\\360\\360se3\\UnInst360SE.exe"
	//	"Publisher"="360安全中心"
	//	"HelpLink"="www.360.cn"

	//[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{66D8959E-B7E9-4cd4-BC16-98711D815F2A}]
	//"DisplayName"="360安全浏览器 2.2 正式版"
	//	"DisplayIcon"="C:\\Program Files\\360\\360se\\360SE.exe"
	//	"UninstallString"="C:\\Program Files\\360\\360se\\UnInst360SE.exe"
	//	"Publisher"="360安全中心"
	//	"HelpLink"="www.360.cn"

	static BOOL IsSafe360ExplorerInstalled()
	{
		CHAR buf[256];
		CRegEncode base;
		//char szReg[256] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{66D8959E-B7E9-4cd4-BC16-98711D815F2A}";
		//ZeroMemory(buf, sizeof(buf));
		//base.Encode(szReg,buf,sizeof(buf));
		char szBase3[]="U09GVFdBUkVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXHsyM0YzRjQ3Ni1CRTM0LTRmNDgtOUM3Ny0yODA2QTgzOTNFQzR9";
		char szBase2[]="U09GVFdBUkVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXHs2NkQ4OTU5RS1CN0U5LTRjZDQtQkMxNi05ODcxMUQ4MTVGMkF9";
	
		for(int i=0;i<2;i++)
		{
			char *szBaseBuff =NULL;
			if(i == 0) 	szBaseBuff = szBase3;
			else szBaseBuff = szBase2;

			ZeroMemory(buf, sizeof(buf));
			base.Decode(szBaseBuff, buf, sizeof(buf));

			//360安全浏览器 查HKEY_LOCAL_MACHINE,并且文件360SE.exe是否存在
			CRegKey reg;			
			if (reg.Open(HKEY_LOCAL_MACHINE, buf, KEY_READ) == ERROR_SUCCESS)
			{
				ZeroMemory(buf, sizeof(buf));

				TCHAR szInstallPath[MAX_PATH] = {0};
				DWORD dwLength = MAX_PATH;
				if (reg.QueryValue(szInstallPath, "DisplayIcon", &dwLength) == ERROR_SUCCESS)
				{			
					HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
					ZeroMemory(szInstallPath, sizeof(szInstallPath));
					if (hFile!=INVALID_HANDLE_VALUE )
					{
						CloseHandle(hFile);
						return TRUE;
					}
				}
			}
		}
		ZeroMemory(buf, sizeof(buf));
		return FALSE;

	}
	/*
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\奇虎360保险箱]
	"DisplayName"="奇虎360保险箱"
	"UninstallString"="C:\\Program Files\\360safe\\safebox\\uninst.exe"
	"DisplayIcon"="C:\\Program Files\\360safe\\safebox\\360safebox.exe"
	"DisplayVersion"="${PRODUCT_VERSION}"
	"URLInfoAbout"="http://www.360safe.com"
	"Publisher"="奇虎网"
	*/

	//[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\360保险箱]
	//"DisplayName"="360保险箱"
	//	"UninstallString"="C:\\Program Files\\360Safebox\\uninst.exe"
	//	"DisplayIcon"="C:\\Program Files\\360Safebox\\360safebox.exe"
	//	"DisplayVersion"="3.0.0.1005"
	//	"URLInfoAbout"="http://www.360.cn"
	//	"Publisher"="360安全中心"

	static BOOL IsSafe360BoxInstalled()
	{
		CHAR buf[256];
		CRegEncode base;

		//char szReg[256] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\360保险箱";
		//ZeroMemory(buf, sizeof(buf));
		//base.Encode(szReg,buf,sizeof(buf));
		char szBase3[]="U09GVFdBUkVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXDM2MLGjz9XP5A";

		//"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\奇虎360保险箱"
		char szBase2[]="U29mdHdhcmVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXMbmu6IzNjCxo8/Vz+Q";
		for(int i=0;i<2;i++)
		{
			char* szBaseBuf=NULL;
			if(i == 0)
				szBaseBuf = szBase3;
			else
				szBaseBuf = szBase2;

			ZeroMemory(buf, sizeof(buf));
			base.Decode(szBaseBuf, buf, sizeof(buf));

			//奇虎360保险箱 查HKEY_LOCAL_MACHINE,并且文件360safebox.exe是否存在	
			CRegKey reg;			
			if (reg.Open(HKEY_LOCAL_MACHINE, buf, KEY_READ) == ERROR_SUCCESS)
			{
				ZeroMemory(buf, sizeof(buf));

				TCHAR szInstallPath[MAX_PATH] = {0};
				DWORD dwLength = MAX_PATH;
				if (reg.QueryValue(szInstallPath, "DisplayIcon", &dwLength) == ERROR_SUCCESS)
				{			
					HANDLE hFile=CreateFile(szInstallPath,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
					ZeroMemory(szInstallPath, sizeof(szInstallPath));
					if (hFile!=INVALID_HANDLE_VALUE )
					{
						CloseHandle(hFile);
						return TRUE;
					}
				}
			}
		}
		ZeroMemory(buf, sizeof(buf));
		return FALSE;
	}

	/*
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\KingsoftAntispy20_is1]
	"Inno Setup: Setup Version"="5.1.11"
	"Inno Setup: App Path"="d:\\Program Files\\Kingsoft Antispy"
	"InstallLocation"="d:\\Program Files\\Kingsoft Antispy\\"
	"Inno Setup: Icon Group"="金山清理专家"
	"Inno Setup: User"="盛颖"
	"Inno Setup: Selected Tasks"="desktopicon"
	"Inno Setup: Deselected Tasks"="quicklaunchicon"
	"DisplayName"="金山清理专家"
	"UninstallString"="\"d:\\Program Files\\Kingsoft Antispy\\unins000.exe\""
	"QuietUninstallString"="\"d:\\Program Files\\Kingsoft Antispy\\unins000.exe\" /SILENT"
	"Publisher"="Kingsoft Corp."
	"URLInfoAbout"="http://www.duba.net/"
	"HelpLink"="http://www.duba.net/"
	"URLUpdateInfo"="http://www.duba.net/"
	"NoModify"=dword:00000001
	"NoRepair"=dword:00000001
	"InstallDate"="20070831"
	*/

	static BOOL IsKingsoftAntispyInstalled()
	{
		CHAR buf[256];
		CRegEncode base;

		//"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\KingsoftAntispy20_is1"
		ZeroMemory(buf, sizeof(buf));
		base.Decode("U09GVFdBUkVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cVW5pbnN0YWxsXEtpbmdzb2Z0QW50aXNweTIwX2lzMQ", buf, sizeof(buf));

		//金山清理专家 查HKEY_LOCAL_MACHINE,并且文件unins000.exe是否存在	
		CRegKey reg;			
		if (reg.Open(HKEY_LOCAL_MACHINE, buf, KEY_READ) == ERROR_SUCCESS)
		{
			ZeroMemory(buf, sizeof(buf));

			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "UninstallString", &dwLength) == ERROR_SUCCESS && *szInstallPath)
			{
				//先要去掉首尾的引号！
				szInstallPath[lstrlen(szInstallPath)-1] = NULL;
				szInstallPath[0] = NULL;
				HANDLE hFile=CreateFile(szInstallPath+1,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				ZeroMemory(szInstallPath, sizeof(szInstallPath));
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}
		ZeroMemory(buf, sizeof(buf));
		return FALSE;
	}

	/*
	[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\VMTools]
	"Type"=dword:00000110
	"Start"=dword:00000002
	"ErrorControl"=dword:00000000
	"ImagePath"=hex(2):22,00,43,00,3a,00,5c,00,50,00,72,00,6f,00,67,00,72,00,61,00,\
	6d,00,20,00,46,00,69,00,6c,00,65,00,73,00,5c,00,56,00,4d,00,77,00,61,00,72,\
	00,65,00,5c,00,56,00,4d,00,77,00,61,00,72,00,65,00,20,00,54,00,6f,00,6f,00,\
	6c,00,73,00,5c,00,56,00,4d,00,77,00,61,00,72,00,65,00,53,00,65,00,72,00,76,\
	00,69,00,63,00,65,00,2e,00,65,00,78,00,65,00,22,00,00,00
	"DisplayName"="VMware Tools Service"
	"ObjectName"="LocalSystem"
	"Description"="Provides support for synchronizing objects between the host and guest operating systems."
	*/
	static BOOL IsVMwareInstalled()
	{
		CHAR buf[256];
		CRegEncode base;

		//"SYSTEM\\CurrentControlSet\\Services\\VMTools"
		ZeroMemory(buf, sizeof(buf));
		base.Decode("U1lTVEVNXEN1cnJlbnRDb250cm9sU2V0XFNlcnZpY2VzXFZNVG9vbHM", buf, sizeof(buf));

		//VMware 查HKEY_LOCAL_MACHINE,并且文件VMwareService.exe是否存在	
		CRegKey reg;			
		if (reg.Open(HKEY_LOCAL_MACHINE, buf, KEY_READ) == ERROR_SUCCESS)
		{
			ZeroMemory(buf, sizeof(buf));

			TCHAR szInstallPath[MAX_PATH] = {0};
			DWORD dwLength = MAX_PATH;
			if (reg.QueryValue(szInstallPath, "ImagePath", &dwLength) == ERROR_SUCCESS && *szInstallPath)
			{
				//先要去掉首尾的引号！
				szInstallPath[lstrlen(szInstallPath)-1] = NULL;
				szInstallPath[0] = NULL;
				HANDLE hFile=CreateFile(szInstallPath+1,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				ZeroMemory(szInstallPath, sizeof(szInstallPath));
				if (hFile!=INVALID_HANDLE_VALUE )
				{
					CloseHandle(hFile);
					return TRUE;
				}
			}
		}
		ZeroMemory(buf, sizeof(buf));
		return FALSE;
	}
protected:

	typedef BOOL (*IsInstalled)();
	struct st_scan_product
	{
		int id;
		IsInstalled fnIsInstalled;
	};

	st_scan_product* GetProductList()
	{
		static st_scan_product m_arProduct[32] = {			
//			{PRODUCT_KAKA, IsKakaInstalled},
			{PRODUCT_SAFE360Kill, IsSafe360KillInstalled},
			{PRODUCT_SAFE360, IsSafe360Installed},
			{PRODUCT_SAFE360Explorer, IsSafe360ExplorerInstalled},
			{PRODUCT_SAFE360BOX, IsSafe360BoxInstalled},
//			{PRODUCT_KINGSOFTANTISPY, IsKingsoftAntispyInstalled},
//			{PRODUCT_VMWARE, IsVMwareInstalled},			
//			{PRODUCT_RAV, IsRavInstalled},
//			{PRODUCT_RFW, IsRfwInstalled},			
			{PRODUCT_EMPTY, NULL},
		};
		return m_arProduct;
	}

public:

	DWORD GetOsVersion()
	{
		OSVERSIONINFOEX osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		BOOL bOsVersionInfoEx;

		if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi)))
		{
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx((OSVERSIONINFO *)&osvi);
		}
		UINT m_uMajor = osvi.dwMajorVersion;
		UINT m_uMinor = osvi.dwMinorVersion;

		switch(osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			{

				if( m_uMajor <= 4)
				{
					return OS_WINNT;
				}
				if( m_uMajor == 5 && m_uMinor == 0)
				{
					return OS_WIN2000;
				}
				if( m_uMajor == 5 && m_uMinor == 1 )
				{
					return OS_WINXP;
				}
				if( m_uMajor == 5 && m_uMinor == 2 )
				{
					return OS_WIN2003;
				}
				if( m_uMajor == 6 && m_uMinor == 0 )
				{
					if( (osvi.wProductType == VER_NT_SERVER) || (osvi.wProductType == VER_NT_DOMAIN_CONTROLLER ) )
					{
						return OS_WIN2008;
					}
					return OS_WINVISTA;
				}
				if( m_uMajor == 6 && m_uMinor == 1 )
				{
					return OS_WIN7;
				}
				break;
			}
		case VER_PLATFORM_WIN32_WINDOWS:
			{
				if(m_uMajor == 4 && m_uMinor == 0)			// Windows 95
				{
					return 0;
				}
				else if(m_uMajor == 4 && (m_uMinor == 10 || m_uMinor == 90) )	// Windows 98/me
				{
					return OS_WIN98;
				}
				break;
			}
		case VER_PLATFORM_WIN32s:
			{
				return 0;
			}
		}
		return 0;
	}

	//function
	DWORD Detect()
	{
		st_scan_product *pro = GetProductList();
		
		DWORD dwRet = GetOsVersion();

		for(int i=0; pro[i].fnIsInstalled!=NULL; i++) 
			dwRet |= pro[i].fnIsInstalled()?pro[i].id:0;
		return dwRet;
	}	
};


#endif // !defined(_INCLUDE_DETECTPRODUCT_H_)