#include "StdAfx.h"
#include <WinSock.h>
#include "getnetadapterinfo2.h"
#include <Windows.h>
#include <WinCon.h>
#include <comutil.h>  
#include <Wbemidl.h>
#include <WinIoCtl.h>
#include <setupapi.h>
#include <ntddndis.h>
#include <strsafe.h>



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

std::list<NETCARD_INFO_> CGetNetAdapterInfo2::s_listOfAdaptaer;

CGetNetAdapterInfo2::CGetNetAdapterInfo2(void)
{
	
}

CGetNetAdapterInfo2::~CGetNetAdapterInfo2(void)
{
	
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// WMI+SetupApi+WinApi获取网卡的MAC地址
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef struct _T_WQL_QUERY  
{  
	CHAR*   szSelect;       // SELECT语句  
	WCHAR*  szProperty;     // 属性字段  
} T_WQL_QUERY;  

// WQL查询语句  
const T_WQL_QUERY szWQLQuery[] = {  
	// 包含USB网卡  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (PhysicalAdapter = TRUE)",  
	L"PNPDeviceID",  

	// 不包含USB网卡  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (NOT (PNPDeviceID LIKE 'USB%')) AND (PhysicalAdapter = TRUE)",  
	L"PNPDeviceID"  
};

static BOOL IsInNetCardList(const std::list<NETCARD_INFO_>& listNetCard, LPCSTR szGUID, PNETCARD_INFO_* ppNetCardInfo)
{
	std::list<NETCARD_INFO_>::const_iterator it = listNetCard.begin();
	for (; it != listNetCard.end(); ++it)
	{
		if (::strcmp(it->NetCardName, szGUID) == 0)
		{
			if (ppNetCardInfo != NULL)
			{
				*ppNetCardInfo = const_cast<PNETCARD_INFO_>(&(*it));
			}
			return TRUE;
		}
	}

	return FALSE;
}

static BOOL IsInNetCardList(const std::list<NETCARD_INFO_>& listNetCard, LPBYTE pbMacAddress, PNETCARD_INFO_* ppNetCardInfo)
{
	if (pbMacAddress == NULL)
		return FALSE;

	std::list<NETCARD_INFO_>::const_iterator it = listNetCard.begin();
	for (; it != listNetCard.end(); ++it)
	{
		if (memcmp(it->PermanentAddress, pbMacAddress, MACADDRESS_BYTELEN) != 0)
		{
			if (memcmp(it->MacAddress, pbMacAddress, MACADDRESS_BYTELEN) == 0)
			{
				if (ppNetCardInfo != NULL)
				{
					*ppNetCardInfo = const_cast<PNETCARD_INFO_>(&(*it));
				}

				return TRUE;
			}
		}
		else
		{
			if (ppNetCardInfo != NULL)
			{
				*ppNetCardInfo = const_cast<PNETCARD_INFO_>(&(*it));
			}
			return TRUE;
		}
	}

	return FALSE;
}

static BOOL WMI_DoWithPNPDeviceID( const TCHAR *PNPDeviceID, T_MACADDRESS_ *pMacAddress, INT iIndex )  
{  
	TCHAR   DevicePath[MAX_PATH];  
	HANDLE  hDeviceFile;      
	BOOL    isOK = FALSE;  
	TCHAR* pszTemp;

	// 生成设备路径名  
	StringCchCopy( DevicePath, MAX_PATH, "//./" );  
	StringCchCat( DevicePath, MAX_PATH, PNPDeviceID );  
	StringCchCat( DevicePath, MAX_PATH, "#{ad498944-762f-11d0-8dcb-00c04fc3358c}" );  

	// 将“PNPDeviceID”中的“/”替换成“#”，以获得真正的设备路径名  
	pszTemp=strstr( DevicePath + 4, "/");
	while(pszTemp)
	{
		pszTemp[0]='#';
		pszTemp=strstr( pszTemp, "/");
	}
	pszTemp=strstr( DevicePath + 4, "\\");
	while(pszTemp)
	{
		pszTemp[0]='#';
		pszTemp=strstr( pszTemp, "\\");
	}

	// 获取设备句柄  
	hDeviceFile = CreateFile( DevicePath,  
		0,  
		FILE_SHARE_READ | FILE_SHARE_WRITE,  
		NULL,  
		OPEN_EXISTING,  
		0,  
		NULL);  

	if( hDeviceFile != INVALID_HANDLE_VALUE )  
	{     
		ULONG   dwID;  
		BYTE    ucData[8];  
		DWORD   dwByteRet;        
		// 获取当前MAC地址  
		dwID = OID_802_3_CURRENT_ADDRESS;  
		isOK = DeviceIoControl( hDeviceFile,
			IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );  
		if( isOK )  
		{     
			memcpy( pMacAddress[iIndex].MACAddress, ucData, dwByteRet );  

			// 获取原生MAC地址  
			dwID = OID_802_3_PERMANENT_ADDRESS;  
			isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );  
			if( isOK )  
			{     
				memcpy( pMacAddress[iIndex].PermanentAddress, ucData, dwByteRet );  
			}   
		}  

		CloseHandle( hDeviceFile );  
	}  
	return isOK;  
}  

// 最多支持取8个物理网卡,23-16位网卡原始mac地址获取情况掩码1表示失败，15-8失败物理网卡个数，7-0总的物理网卡个数
static INT WMI_MacAddress(INT iQueryType, T_MACADDRESS_ *pMacAddress, INT iSize)  
{
	HRESULT hres;  
	INT iTotal = 0; 
	INT iFailed=0;
	TCHAR chTemp[10];
	INT iTemp;

	// 判断查询类型是否支持  
	if( (iQueryType < 0) || (iQueryType >= sizeof(szWQLQuery)/sizeof(T_WQL_QUERY)) )  
	{  
		return -1;  // 查询类型不支持  
	}  

	// 初始化COM  
	hres = CoInitializeEx( NULL, COINIT_MULTITHREADED );   
	if( FAILED(hres) )  
	{  
		return -2;  
	}  

	// 设置COM的安全认证级别  
	hres = CoInitializeSecurity(   
		NULL,   
		-1,   
		NULL,   
		NULL,   
		RPC_C_AUTHN_LEVEL_DEFAULT,   
		RPC_C_IMP_LEVEL_IMPERSONATE,  
		NULL,  
		EOAC_NONE,  
		NULL  
		);  
	if( FAILED(hres) )  
	{
		if (RPC_E_TOO_LATE != hres)
		{
			CoUninitialize();  
			return -2;  
		}
	}

	// 获得WMI连接COM接口  
	IWbemLocator *pLoc = NULL;  
	hres = CoCreateInstance(   
		CLSID_WbemLocator,               
		NULL,   
		CLSCTX_INPROC_SERVER,   
		IID_IWbemLocator,  
		reinterpret_cast<LPVOID*>(&pLoc)  
		);   
	if( FAILED(hres) )  
	{  
		CoUninitialize();  
		return -2;  
	}  

	// 通过连接接口连接WMI的内核对象名"ROOT\\CIMV2"  
	IWbemServices *pSvc = NULL;  
	hres = pLoc->ConnectServer(  
		_bstr_t( L"ROOT\\CIMV2" ),  
		NULL,  
		NULL,  
		NULL,  
		0,  
		NULL,  
		NULL,  
		&pSvc  
		);      
	if( FAILED(hres) )  
	{  
		pLoc->Release();   
		CoUninitialize();  
		return -2;  
	}  

	// 设置请求代理的安全级别  
	hres = CoSetProxyBlanket(  
		pSvc,  
		RPC_C_AUTHN_WINNT,  
		RPC_C_AUTHZ_NONE,  
		NULL,  
		RPC_C_AUTHN_LEVEL_CALL,  
		RPC_C_IMP_LEVEL_IMPERSONATE,  
		NULL,  
		EOAC_NONE  
		);  
	if( FAILED(hres) )  
	{  
		pSvc->Release();  
		pLoc->Release();       
		CoUninitialize();  
		return -2;  
	}  

	// 通过请求代理来向WMI发送请求  
	IEnumWbemClassObject *pEnumerator = NULL;  
	hres = pSvc->ExecQuery(  
		bstr_t("WQL"),   
		bstr_t( szWQLQuery[iQueryType].szSelect ),  
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,   
		NULL,  
		&pEnumerator  
		);  
	if( FAILED(hres) )  
	{  
		pSvc->Release();  
		pLoc->Release();  
		CoUninitialize();  
		return -3;  
	}  

	// 循环枚举所有的结果对象    
	while( pEnumerator )  
	{  
		IWbemClassObject *pclsObj = NULL;  
		ULONG uReturn = 0;  

		if( (pMacAddress != NULL) && ((iTotal&0xff) >= min(8,iSize)) )  
		{  
			break;  
		}  

		pEnumerator->Next(  
			WBEM_INFINITE,  
			1,   
			&pclsObj,  
			&uReturn  
			);  

		if( uReturn == 0 )  
		{  
			break;  
		}  

		VARIANT vtProperty;  
		TCHAR szProperty[128];  
		//获取网卡制造商（名称）, 过滤虚拟机
		VariantInit( &vtProperty );   
		pclsObj->Get( bstr_t("Manufacturer"), 0, &vtProperty, NULL, NULL );  
		if (VT_NULL != vtProperty.vt && VT_BSTR == vtProperty.vt)
		{
			StringCchCopyNA(pMacAddress[iTotal&0xff].Manufacturer, MAX_PATH, (LPCSTR)ATL::CW2A(vtProperty.bstrVal), MAX_PATH - 1);
		}
		VariantClear( &vtProperty );

		// 获取网卡类型
		VariantInit( &vtProperty );   
		pclsObj->Get( bstr_t("AdapterType"), 0, &vtProperty, NULL, NULL );
		if (VT_NULL != vtProperty.vt && VT_BSTR == vtProperty.vt)
		{
			StringCchCopyNA(pMacAddress[iTotal&0xff].NetCardType, MACADDRESS_TYPELEN, (LPCSTR)ATL::CW2A(vtProperty.bstrVal), MACADDRESS_TYPELEN - 1);
		}
		VariantClear( &vtProperty );

		// 获取网卡GUID
		VariantInit( &vtProperty );   
		pclsObj->Get( bstr_t("GUID"), 0, &vtProperty, NULL, NULL );
		if (VT_NULL != vtProperty.vt && VT_BSTR == vtProperty.vt)
		{
			StringCchCopyA(pMacAddress[iTotal&0xff].NetCardName, MAX_PATH, (LPCSTR)ATL::CW2A(vtProperty.bstrVal));
		}
		VariantClear( &vtProperty );

		// 获取网卡描述
		VariantInit( &vtProperty );   
		pclsObj->Get( bstr_t("Description"), 0, &vtProperty, NULL, NULL );
		if (VT_NULL != vtProperty.vt && VT_BSTR == vtProperty.vt)
		{
			StringCchCopyNA(pMacAddress[iTotal&0xff].Description, MAX_PATH, (LPCSTR)ATL::CW2A(vtProperty.bstrVal), MAX_PATH - 1);
		}
		VariantClear( &vtProperty );  

		// 获取网卡设备标识符  
		VariantInit( &vtProperty );   
		pclsObj->Get( szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL );  
		memset(szProperty,0,128*sizeof(TCHAR));
		StringCchCopy(szProperty, 128, (LPCTSTR)ATL::CW2A(vtProperty.bstrVal));
		VariantClear( &vtProperty );  

		//RSLOG(RSLOG_FATAL, TEXT("[HWBEE] | WMI_MacAddress() |  1-Network card failed %s"), szProperty);

		if( pMacAddress != NULL )  
		{   // 通过设备标识符获取原生MAC地址和当前MAC地址  
			if( !WMI_DoWithPNPDeviceID( szProperty, pMacAddress, (iTotal&0xff) ) )  
			{
				VariantInit( &vtProperty ); 
				pclsObj->Get(bstr_t("MACAddress"),0,&vtProperty,NULL,NULL);
				memset(szProperty,0,128*sizeof(TCHAR));
				StringCchCopy(szProperty, 128, (LPCTSTR)ATL::CW2T(vtProperty.bstrVal));
				VariantClear( &vtProperty );
				memset(chTemp,0,10);
				for(iTemp=0;iTemp<6;iTemp++)
				{
					chTemp[0]=szProperty[iTemp*3];
					chTemp[1]=szProperty[iTemp*3+1];
					pMacAddress[iTotal&&0xff].MACAddress[iTemp]=strtol(chTemp, NULL, 16);//strtol(chTemp,NULL,16);
				}
				iTotal+=(1<<(iFailed+16));
				iFailed++;
			}

			//g_aMacAddress[iTotal&&0xff] = pMacAddress[iTotal&&0xff];
			iTotal++;
		}  
		else  
		{  
			iTotal++;  
		}  

		pclsObj->Release();  

	} // End While  

	// 释放资源  
	pEnumerator->Release();  
	pSvc->Release();  
	pLoc->Release();      
	CoUninitialize();  

	return iTotal+(iFailed<<8);  
}

static BOOL WMI_GetNetworkAdapterInfo(int iNetCardType, INT iCount, std::list<NETCARD_INFO_>& listNetCard)
{
	DWORD dwListSIze = listNetCard.size();
	NETCARD_INFO_ netCardInfo;
	T_MACADDRESS_* aMacAddress = new(std::nothrow) T_MACADDRESS_[iCount]();
	if (aMacAddress == NULL)
		return FALSE;

	int iRet = WMI_MacAddress(iNetCardType, aMacAddress, iCount);
	if(iRet==-2)
	{
		delete [] aMacAddress;
		return FALSE;
	}
	if(iRet==-4)
	{
		delete [] aMacAddress;
		return FALSE;
	}
	if(iRet>0)
	{
		CHAR szMacAddr[MAX_PATH] = {0};
		for(int i = 0; i < (iRet&0xff); i++)
		{
			if(iRet & (1<<(16+i))) //获取失败
			{
			}
			else
			{
				ZeroMemory(&netCardInfo, sizeof(NETCARD_INFO_));
				sprintf(netCardInfo.MacAddress, "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x",
					aMacAddress[i].PermanentAddress[0],
					aMacAddress[i].PermanentAddress[1],
					aMacAddress[i].PermanentAddress[2],
					aMacAddress[i].PermanentAddress[3],
					aMacAddress[i].PermanentAddress[4],
					aMacAddress[i].PermanentAddress[5]
				);

				// 如果mac地址为空或者全0，继续下一块网卡
				if (strlen(netCardInfo.MacAddress) == 0 
					|| strcmp(netCardInfo.MacAddress, "00-00-00-00-00-00") == 0)
				{
					continue;
				}
				else
				{
					if (strcmp(aMacAddress[i].Manufacturer, "Microsoft") == 0)
					{
						memcpy(netCardInfo.PermanentAddress, aMacAddress->PermanentAddress, MACADDRESS_BYTELEN);
						memcpy(netCardInfo.CurrentAddress, aMacAddress->MACAddress, MACADDRESS_BYTELEN);
						StringCchCopyNA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, aMacAddress[i].NetCardType, MACADDRESS_TYPELEN - 1);
						StringCchCopyNA(netCardInfo.NetCardName, MAX_PATH, aMacAddress[i].NetCardName, MAX_PATH - 1);
						StringCchCopyNA(netCardInfo.Description, MAX_PATH, aMacAddress[i].Description, MAX_PATH - 1);
						StringCchCopyNA(netCardInfo.Manufacturer, MAX_PATH, aMacAddress[i].Manufacturer, MAX_PATH - 1);

						PNETCARD_INFO_ pNetCardInfo = NULL;
						if (IsInNetCardList(listNetCard, netCardInfo.NetCardName, &pNetCardInfo))
							//if (IsInNetCardList(listNetCard, netCardInfo.PermanentAddress, &pNetCardInfo))
						{
							if (pNetCardInfo->NetCardType[0] == 0x00 && netCardInfo.NetCardType[0] != 0x00)
							{
								StringCchCopyA(pNetCardInfo->NetCardType, MAX_PATH, netCardInfo.NetCardType);
							}

							if (pNetCardInfo->Description[0] == 0x00 && netCardInfo.Description[0] != 0x00)
							{
								StringCchCopyA(pNetCardInfo->Description, MAX_PATH, netCardInfo.Description);
							}

							if (pNetCardInfo->Manufacturer[0] == 0x00 && netCardInfo.Manufacturer[0] != 0x00)
							{
								StringCchCopyA(pNetCardInfo->Manufacturer, MAX_PATH, netCardInfo.Manufacturer);
							}

							if (0 != memcmp(netCardInfo.CurrentAddress, pNetCardInfo->CurrentAddress, MACADDRESS_BYTELEN))
							{
								memcpy(pNetCardInfo->CurrentAddress, netCardInfo.CurrentAddress, MACADDRESS_BYTELEN);
							}
						}
						else
						{
							listNetCard.push_back(netCardInfo);
						}
					}
				}
			}
		}
	}

	delete [] aMacAddress;
	return dwListSIze > listNetCard.size();
}

//////////////////////////////////////////////////////////////////////////
//SetupApi
//////////////////////////////////////////////////////////////////////////
//#include <setupapi.h>  


#pragma comment (lib, "Setupapi.lib") 

const GUID GUID_QUERYSET[] = {  
	// 网卡原生MAC地址（包含USB网卡）  
	{0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},  

	// 网卡原生MAC地址（剔除USB网卡）  
	{0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},  
};

static BOOL GetAdapterGUID_ByDevPath(LPCSTR szDevPath, LPSTR szServiceName, DWORD dwDescSize)
{
	if (szDevPath == NULL || *szDevPath == 0x00)
		return FALSE;

	LPCSTR szBegin = strrchr(szDevPath, '{');
	if (szBegin != NULL)
	{
		LPCSTR  szEnd = strchr(szBegin, '}');
		if (szEnd != NULL)
		{
			StringCchCopyNA(szServiceName, dwDescSize, szBegin, szEnd - szBegin + 1);
			return TRUE;
		}
	}
	return FALSE;
}

// 获取网卡原生MAC地址  
static BOOL WDK_GetMacAddress( TCHAR* DevicePath, T_MACADDRESS_ *pMacAddress, INT iIndex, BOOL isIncludeUSB )  
{  
	HANDLE  hDeviceFile;  
	BOOL    isOK = FALSE;  

	// 剔除虚拟网卡  ?!
	if(( strnicmp( DevicePath + 4, "usb", 3 ) != 0 )&&( strnicmp( DevicePath + 4, "pci", 3 ) != 0 ))
		return FALSE;
	if( !isIncludeUSB )  
	{   // 剔除USB网卡  
		if( strnicmp( DevicePath + 4, "usb", 3 ) == 0 )  
		{  
			return FALSE;  
		}  
	} 


	// 获取设备句柄  
	hDeviceFile = CreateFile( DevicePath,  
		0,  
		FILE_SHARE_READ | FILE_SHARE_WRITE,  
		NULL,  
		OPEN_EXISTING,  
		0,  
		NULL);  

	if( hDeviceFile != INVALID_HANDLE_VALUE )  
	{     
		ULONG   dwID;  
		BYTE    ucData[8];  
		DWORD   dwByteRet;        

		// 获取当前MAC地址  
		dwID = OID_802_3_CURRENT_ADDRESS;  
		isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );  
		if( isOK )  
		{     
			memcpy( pMacAddress[iIndex].MACAddress, ucData, dwByteRet );  

			// 获取原生MAC地址  
			dwID = OID_802_3_PERMANENT_ADDRESS;  
			isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );  
			if( isOK )  
			{     
				memcpy( pMacAddress[iIndex].PermanentAddress, ucData, dwByteRet ); 
			}  
		}

		// 获取网卡的GUID
		GetAdapterGUID_ByDevPath((LPCSTR)ATL::CT2A(DevicePath), pMacAddress[iIndex].NetCardName, MAX_PATH);

		CloseHandle( hDeviceFile );  
	}  

	return isOK;  
}  

static BOOL  SetupApi_GetProperty( TCHAR* DevicePath, INT iQueryType, T_MACADDRESS_ *pMacAddress, INT iIndex )  
{  
	BOOL isOK = FALSE;  

	switch( iQueryType )  
	{  
	case 0:     // 网卡原生MAC地址（包含USB网卡）         
		isOK = WDK_GetMacAddress( DevicePath, pMacAddress, iIndex, TRUE );  
		break;  

	case 1:     // 网卡原生MAC地址（剔除USB网卡）         
		isOK = WDK_GetMacAddress( DevicePath, pMacAddress, iIndex, FALSE );  
		break;  

	default:  
		break;  

	}  

	return isOK;  
}  

static INT SetupApi_GetMacAddress( INT iQueryType, T_MACADDRESS_ *pMacAddress, INT iSize )  
{     
	HDEVINFO    hDevInfo;  
	DWORD       MemberIndex, RequiredSize;  
	SP_DEVICE_INTERFACE_DATA            DeviceInterfaceData;  
	PSP_DEVICE_INTERFACE_DETAIL_DATA    DeviceInterfaceDetailData;  
	INT iTotal = 0;  

	// 判断查询类型是否支持  
	if( (iQueryType < 0) || (iQueryType >= sizeof(GUID_QUERYSET)/sizeof(GUID)) )  
	{  
		return -2;  // 查询类型不支持  
	}  

	// 获取设备信息集  
	hDevInfo = SetupDiGetClassDevs( GUID_QUERYSET + iQueryType, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE );  
	if( hDevInfo == INVALID_HANDLE_VALUE )  
	{  
		return -1;  
	}  

	// 枚举设备信息集中所有设备  
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);  
	for( MemberIndex = 0; ((pMacAddress == NULL) || (iTotal < iSize)); MemberIndex++ )  
	{   // 获取设备接口  
		if( !SetupDiEnumDeviceInterfaces( hDevInfo, NULL, GUID_QUERYSET + iQueryType, MemberIndex, &DeviceInterfaceData ) )  
		{   // 设备枚举完毕  
			break;  
		}  

		// 获取接收缓冲区大小，函数返回值为FALSE，GetLastError()=ERROR_INSUFFICIENT_BUFFER  
		SetupDiGetDeviceInterfaceDetail( hDevInfo, &DeviceInterfaceData, NULL, 0, &RequiredSize, NULL );  

		// 申请接收缓冲区  
		DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc( RequiredSize );  
		DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);  
		SP_DEVINFO_DATA devicedata = {sizeof(SP_DEVINFO_DATA)};

		// 获取设备细节信息  
		if( SetupDiGetDeviceInterfaceDetail( hDevInfo, &DeviceInterfaceData, DeviceInterfaceDetailData, RequiredSize, NULL, &devicedata ) )  
		{
			if( pMacAddress != NULL )  
			{  
				if( SetupApi_GetProperty( DeviceInterfaceDetailData->DevicePath, iQueryType, pMacAddress, iTotal ) )  
				{
					char devname[512];
					if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devicedata, SPDRP_DEVICEDESC, NULL, (LPBYTE)devname, sizeof(devname), NULL))
					{
						StringCchCopyNA(pMacAddress[iTotal].Description, MAX_PATH, devname, MAX_PATH - 1);
					}

					ZeroMemory(devname, 512);
					if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devicedata, SPDRP_MFG, NULL, (LPBYTE)devname, sizeof(devname), NULL))
					{
						StringCchCopyNA(pMacAddress[iTotal].Manufacturer, MAX_PATH, devname, MAX_PATH - 1);
					}

					iTotal++;  
				}  
			}  
			else  
			{  
				iTotal++;  
			}  
		}

		free( DeviceInterfaceDetailData );  
	}  

	SetupDiDestroyDeviceInfoList( hDevInfo );  

	return iTotal;  
}  

static BOOL SetupApi_GetNetworkAdapterInfo(int iNetCardType, INT iCount, std::list<NETCARD_INFO_>& listNetCard)
{
	DWORD dwCountBegin = listNetCard.size();

	NETCARD_INFO_ netCardInfo;
	T_MACADDRESS_* aMacAddress = new(std::nothrow) T_MACADDRESS_[iCount]();
	if (aMacAddress == NULL)
		return FALSE;

	int iRet = SetupApi_GetMacAddress(iNetCardType, aMacAddress, iCount);
	if(iRet==-2)
	{
		return FALSE;
	}
	if(iRet==-4)
	{
		return FALSE;
	}
	if(iRet>0)
	{

		for(int i = 0; i < (iRet&0xff); i++)
		{
			if(iRet & (1<<(16+i))) //获取失败
			{
			}
			else
			{
				ZeroMemory(&netCardInfo, sizeof(NETCARD_INFO_));
				sprintf(netCardInfo.MacAddress, "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x",
					aMacAddress[i].PermanentAddress[0],
					aMacAddress[i].PermanentAddress[1],
					aMacAddress[i].PermanentAddress[2],
					aMacAddress[i].PermanentAddress[3],
					aMacAddress[i].PermanentAddress[4],
					aMacAddress[i].PermanentAddress[5]
				);

				// 如果mac地址为空或者全0，继续下一块网卡
				if (strlen(netCardInfo.MacAddress) == 0 
					|| strcmp(netCardInfo.MacAddress, "00-00-00-00-00-00") == 0)
				{
					continue;
				}
				else
				{
					if (strcmp(aMacAddress[i].Manufacturer, "Microsoft") == 0)
					{
						StringCchCopyA(netCardInfo.NetCardName, MAX_PATH, aMacAddress->NetCardName);
						StringCchCopyA(netCardInfo.Manufacturer, MAX_PATH, aMacAddress->Manufacturer);
						StringCchCopyA(netCardInfo.Description, MAX_PATH, aMacAddress->Description);
						memcpy(netCardInfo.CurrentAddress, aMacAddress->MACAddress, MACADDRESS_BYTELEN);
						memcpy(netCardInfo.PermanentAddress, aMacAddress->PermanentAddress, MACADDRESS_BYTELEN);

						PNETCARD_INFO_ pNetCardInfo = NULL;
						if (IsInNetCardList(listNetCard, netCardInfo.PermanentAddress, &pNetCardInfo))
						{
							memcpy(netCardInfo.CurrentAddress, aMacAddress->MACAddress, MACADDRESS_BYTELEN);
						}
						else
						{
							listNetCard.push_back(netCardInfo);
						}
					}	
				}
			}
		}
	}

	return dwCountBegin > listNetCard.size();
}

//////////////////////////////////////////////////////////////////////////
// Win32 API
//////////////////////////////////////////////////////////////////////////

//#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库

static BOOL WinAPI_GetNetworkAdapterInfo(std::list<NETCARD_INFO_>& listNetCard, BOOL bAddNew)
{
	BOOL bResult = FALSE;
	NETCARD_INFO_ netCardInfo;

	PIP_ADAPTER_INFO pIpAdapterInfo = new(std::nothrow) IP_ADAPTER_INFO();
	if (pIpAdapterInfo == NULL)
		return FALSE;

	DWORD stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = ::GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;

	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		if (pIpAdapterInfo != NULL)
		{
			delete pIpAdapterInfo;
		}


		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}

	PIP_ADAPTER_INFO pHeadIpAdapterInfo = pIpAdapterInfo;
	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			if (strcmp(pIpAdapterInfo->Description, "Microsoft") == 0)
			{
				ZeroMemory(&netCardInfo, sizeof(NETCARD_INFO_));

				switch(pIpAdapterInfo->Type)
				{
				case MIB_IF_TYPE_OTHER:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "OTHER");
					break;
				case MIB_IF_TYPE_ETHERNET:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "ETHERNET");
					break;
				case MIB_IF_TYPE_TOKENRING:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "TOKENRING");
					break;
				case MIB_IF_TYPE_FDDI:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "FDDI");
					break;
				case MIB_IF_TYPE_PPP:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "PPP");
					break;
				case MIB_IF_TYPE_LOOPBACK:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "LOOPBACK");
					break;
				case MIB_IF_TYPE_SLIP:
					StringCchCopyA(netCardInfo.NetCardType, MACADDRESS_TYPELEN, "SLIP");
					break;
				default:
					break;
				}

				StringCchCopyNA(netCardInfo.Description, MAX_PATH, pIpAdapterInfo->Description, MAX_PATH - 1);
				StringCchCopyNA(netCardInfo.NetCardName, MAX_PATH, pIpAdapterInfo->AdapterName, MAX_PATH - 1);

				CHAR szMacAddr[MACADDRESS_TYPELEN] = {0};
				CHAR szTempAddr[8] = {0};
				ZeroMemory(szMacAddr, MACADDRESS_TYPELEN);
				for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
				{
					ZeroMemory(szTempAddr, 8);
					if (i < pIpAdapterInfo->AddressLength-1)
					{
						StringCchPrintfA(szTempAddr, 8, "%02X-", pIpAdapterInfo->Address[i]);
					}
					else
					{
						StringCchPrintfA(szTempAddr, 8, "%02X", pIpAdapterInfo->Address[i]);
					}
					StringCchCatA(szMacAddr, MACADDRESS_TYPELEN, szTempAddr);
				}

				if (strlen(szMacAddr) != 0 
					&& strcmp(szMacAddr, "00-00-00-00-00-00") != 0)
				{
					PNETCARD_INFO_ pNetCardInfo = NULL;
					if (IsInNetCardList(listNetCard, netCardInfo.NetCardName, &pNetCardInfo))
					{
						if (pNetCardInfo->NetCardType[0] == 0x00 && netCardInfo.NetCardType[0] != 0x00)
						{
							StringCchCopyA(pNetCardInfo->NetCardType, MACADDRESS_TYPELEN, netCardInfo.NetCardType);
						}

						if (pNetCardInfo->Description[0] == 0x00 && netCardInfo.Description[0] != 0x00)
						{
							StringCchCopyA(pNetCardInfo->Description, MAX_PATH, netCardInfo.Description);
						}

						if (pNetCardInfo->Manufacturer[0] == 0x00 && netCardInfo.Manufacturer[0] != 0x00)
						{
							StringCchCopyA(pNetCardInfo->Manufacturer, MAX_PATH, netCardInfo.Manufacturer);
						}
					}
					else
					{
						if (bAddNew)
						{
							StringCchCopyA(netCardInfo.MacAddress, MAX_PATH, szMacAddr);
							listNetCard.push_back(netCardInfo);
						}
					}
				}
			}

			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}

	//释放内存空间
	if (pHeadIpAdapterInfo != NULL)
	{
		delete pHeadIpAdapterInfo;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////

static DWORD CalcAsc2OfStr(LPCSTR str)
{
	if (str == NULL || str[0] == 0x00)
		return DWORD(-1);

	DWORD dwLen = strlen(str);

	DWORD dwCount = 0;
	for (DWORD dw = 0; dw < dwLen; ++dw)
	{
		dwCount += str[dw];
	}

	return dwCount;
}

static LPCSTR VirtualAdapterTag[] = 
{
	"VMware", "Microsoft", "Windows", "Virtual", "pppoe", "宽带连接", "拨号", "Broadband Connection"
};

static BOOL IsContainTagsVAT(LPCSTR szDecription)
{
	if (szDecription == NULL || *szDecription == 0x00)
		return FALSE;

	for (int i = 0; i < _countof(VirtualAdapterTag); ++i)
	{
		if (strcmp(szDecription, VirtualAdapterTag[i]) != 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

static LPCSTR AdapterManufacturer[] = 
{
	"Realtek", "Broadcom", "Intel", "Marvell", "Atheros", "Ralink", "Nvidia", "Lenovo"
};

static BOOL IsContainTagsVF(LPCSTR szManufacturer)
{
	if (szManufacturer == NULL || *szManufacturer == 0x00)
		return FALSE;

	for (int i = 0; i < _countof(AdapterManufacturer); ++i)
	{
		if (strcmp(szManufacturer, AdapterManufacturer[i]) != 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

#define NETCARD_MAX 8

BOOL CGetNetAdapterInfo2::GetNetAdapterMac(BYTE* pBuf, BYTE &bBufSize, BOOL bReGet)
{
	if (pBuf == NULL)
		return FALSE;

	if (bReGet) // 重新读取一次
	{
		GetNetAdapters();
	}
	else
	{
		if (s_listOfAdaptaer.empty())
		{
			GetNetAdapters();
		}
	}

	if (!s_listOfAdaptaer.empty())
	{
		// 先找Intel的网卡
		std::list<NETCARD_INFO_> listNetCard_4;
		std::list<NETCARD_INFO_>::iterator find_it = s_listOfAdaptaer.begin();
		for (; find_it != s_listOfAdaptaer.end(); ++find_it)
		{
			// 计算MAC地址的ASC编码的总和
			find_it->MacAddrAsc2 = CalcAsc2OfStr(find_it->MacAddress);

			if (strcmp(find_it->Manufacturer, "Intel") == 0
				|| strcmp(find_it->Description, "Intel") == 0)
			{
				listNetCard_4.push_back(*find_it);
			}
		}

		// 没找到Intel的网卡，则
		if (listNetCard_4.empty())
		{
			// 找到最小的MAC地址
			std::list<NETCARD_INFO_>::const_iterator min_it = s_listOfAdaptaer.begin();
			std::list<NETCARD_INFO_>::const_iterator find_min_it = s_listOfAdaptaer.begin();
			for (; find_min_it != s_listOfAdaptaer.end(); ++find_min_it)
			{
				if (min_it->MacAddrAsc2 > find_min_it->MacAddrAsc2)
				{
					min_it = find_min_it;
				}
			}

			memcpy(pBuf, min_it->PermanentAddress, MACADDRESS_BYTELEN);
			bBufSize = MACADDRESS_BYTELEN;
		}
		else
		{
			// 找到最小的MAC地址
			std::list<NETCARD_INFO_>::const_iterator min_it = listNetCard_4.begin();
			std::list<NETCARD_INFO_>::const_iterator find_min_it = listNetCard_4.begin();
			for (; find_min_it != listNetCard_4.end(); ++find_min_it)
			{
				if (min_it->MacAddrAsc2 > find_min_it->MacAddrAsc2)
				{
					min_it = find_min_it;
				}
			}

			memcpy(pBuf, min_it->PermanentAddress, MACADDRESS_BYTELEN);
			bBufSize = MACADDRESS_BYTELEN;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CGetNetAdapterInfo2::VerifyNetAdapterMac(BYTE* pBuf, DWORD dwBufSize, BOOL bReGet)
{
	if (pBuf == NULL)
		return FALSE;

	if (bReGet) // 重新读取一次
	{
		GetNetAdapters();
	}
	else
	{
		if (s_listOfAdaptaer.empty())
		{
			GetNetAdapters();
		}
	}

	std::list<NETCARD_INFO_>::const_iterator find_it = s_listOfAdaptaer.begin();
	for (; find_it != s_listOfAdaptaer.end(); ++find_it)
	{
		if (find_it->PermanentAddress != NULL)
		{
			if (0 == memcmp(pBuf, find_it->PermanentAddress, dwBufSize))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CGetNetAdapterInfo2::GetNetAdapters(VOID)
{
	// 先通过WMI获取网卡信息，如果获取为空则用SetupAPI再获取一次网卡信息
	std::list<NETCARD_INFO_> listNetCard;
	WMI_GetNetworkAdapterInfo(1, NETCARD_MAX, listNetCard);
	if (listNetCard.empty())
	{
		SetupApi_GetNetworkAdapterInfo(1, NETCARD_MAX, listNetCard);
	}

	//剔除USB方式获取不到网卡信息的获取，则获取USB网卡的信息
	if (listNetCard.empty())
	{
		WMI_GetNetworkAdapterInfo(0, NETCARD_MAX, listNetCard);
		if (listNetCard.empty())
		{
			SetupApi_GetNetworkAdapterInfo(0, NETCARD_MAX, listNetCard);
		}
	}

	BOOL bUseWin32Api = listNetCard.empty();
	WinAPI_GetNetworkAdapterInfo(listNetCard, listNetCard.empty());

	// 过滤虚拟网卡
	std::list<NETCARD_INFO_> listNetCard_2;
	std::list<NETCARD_INFO_>::const_iterator iter = listNetCard.begin();
	for (; iter != listNetCard.end(); ++iter)
	{
		if (!IsContainTagsVAT(iter->Description) 
			&& !IsContainTagsVAT(iter->Manufacturer))
		{
			listNetCard_2.push_back(*iter);
		}
	}

	// 如果过滤之后为空，则读取厂商信息再筛选一次
	if (listNetCard_2.empty()) 
	{
		iter = listNetCard.begin();
		for (DWORD dw = 1; iter != listNetCard.end(); ++iter, ++dw)
		{
			if (IsContainTagsVF(iter->Manufacturer) 
				|| IsContainTagsVF(iter->Description))
			{
				// 过滤掉网卡类型是ppp的
				if (stricmp(iter->NetCardType, "ppp") != 0)
				{
					listNetCard_2.push_back(*iter);
				}
			}
		}
	}

	if (!listNetCard_2.empty())
	{
		s_listOfAdaptaer.clear();
		s_listOfAdaptaer = listNetCard_2;
	}

	return TRUE;
}
