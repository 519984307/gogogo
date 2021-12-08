#include "StdAfx.h"
# pragma comment(lib, "wbemuuid.lib")
# pragma comment(lib, "comsvcs.lib")
#pragma comment(lib, "comsuppw.lib")
#include ".\wmigetinfo.h"
using namespace _com_util;
#include <comutil.h>
//#include <objbase.h>
//#include <objidl.h>
#pragma  comment(lib, "ole32.lib")
//typedef struct tagSOLE_AUTHENTICATION_SERVICE
//{
//	DWORD dwAuthnSvc;
//	DWORD dwAuthzSvc;
//	OLECHAR *pPrincipalName;
//	HRESULT hr;
//} 	SOLE_AUTHENTICATION_SERVICE;
//
//WINOLEAPI CoInitializeSecurity(
//							   IN PSECURITY_DESCRIPTOR         pSecDesc,
//							   IN LONG                         cAuthSvc,
//							   IN SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
//							   IN void                        *pReserved1,
//							   IN DWORD                        dwAuthnLevel,
//							   IN DWORD                        dwImpLevel,
//							   IN void                        *pAuthList,
//							   IN DWORD                        dwCapabilities,
//							   IN void                        *pReserved3 );

CWmiGetInfo::CWmiGetInfo(void)
{
	m_bInit=FALSE;
	m_InfoMap.clear();
}

CWmiGetInfo::~CWmiGetInfo(void)
{
	m_cWbemLocator=NULL;
	m_cWbemServices=NULL;
	m_InfoMap.clear();
	CoUninitialize(); 


}
BYTE CWmiGetInfo::StrToHex(const CHAR* p)
{
	BYTE bRet=0;
	if(('0'<=p[0])&&(p[0]<='9'))
	{
		bRet=(BYTE)(p[0]-'0');
	}
	else if(('a'<=p[0])&&(p[0]<='f'))
	{
		bRet=(BYTE)(p[0]-'a'+10);
	}
	else if(('A'<=p[0])&&(p[0]<='F'))
	{
		bRet=(BYTE)(p[0]-'A'+10);
	}
	bRet<<=4;
	if(('0'<=p[1])&&(p[1]<='9'))
	{
		bRet+=(BYTE)(p[1]-'0');
	}
	else if(('a'<=p[1])&&(p[1]<='f'))
	{
		bRet+=(BYTE)(p[1]-'a'+10);
	}
	else if(('A'<=p[1])&&(p[1]<='F'))
	{
		bRet+=(BYTE)(p[1]-'A'+10);
	}
	return bRet;
}
BOOL CWmiGetInfo::Init()
{
	HRESULT hr=S_OK;
	if(m_bInit)
		return m_bInit;
	hr=CoInitialize(NULL);
	FAILEXIT(SUCCEEDED(hr),FALSE);
	// 获取访问 WMI 权限
	hr=CoInitializeSecurity( NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,RPC_C_IMP_LEVEL_IMPERSONATE,NULL, 0/*EOAC_NONE*/, 0 );
	//RPC_E_TOO_LATE：CoInitializeSecurity has already been called. 
	//add by ysb 2009-1-8 否则一个进程使用第二次就失败了
	if(RPC_E_TOO_LATE != hr)
		FAILEXIT(SUCCEEDED(hr),FALSE);
	hr=m_cWbemLocator.CoCreateInstance( CLSID_WbemAdministrativeLocator , 0 , 	CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER );
	FAILEXIT(SUCCEEDED(hr),FALSE);
	hr=m_cWbemLocator->ConnectServer( L"root\\cimv2" , NULL, NULL, NULL, 0, NULL, NULL, &m_cWbemServices );
	FAILEXIT(SUCCEEDED(hr),FALSE);
	m_bInit=TRUE;
	GetWmiInfo(" Win32_Processor");
	GetWmiInfo(" Win32_NetworkAdapter");
	//GetWmiInfo(" Win32_DiskDrive");
	return TRUE;
}
HRESULT CWmiGetInfo::GetInfo(INT InfoId,BYTE* pBuf,BYTE &bBufSize)
{
	string str;
	DWORD i;
	DWORD dwNeedSize;
	if(pBuf==NULL)
		return E_INVALIDARG;
	if(!m_bInit)
		Init();
	switch(InfoId)
	{
	case DISK_SERIAL:
		return E_NOTIMPL;
		break;
	case CPU_ID:
		str=m_InfoMap["ProcessorId"];
		dwNeedSize=(DWORD)str.size()/2;
		break;
	case MAC_ADDRESS:
		str=m_InfoMap["MACAddress"];
		dwNeedSize=6;
		break;
	default:
		return E_INVALIDARG;
		
	}
	if(bBufSize<dwNeedSize)
	{
		bBufSize=(BYTE)dwNeedSize;
		return E_OUTOFMEMORY;
	}
	if(InfoId==MAC_ADDRESS)
	{
		for(i=0;i<(min(6,(str.size()+1)/3));i++)
		{
			pBuf[i]=StrToHex(str.c_str()+i*3);
		}
	}
	if(InfoId==CPU_ID)
	{
		for(i=0;i<str.size()/2;i++)
		{
			pBuf[i]=StrToHex(str.c_str()+i*2);
		}
	}
	bBufSize=(BYTE)dwNeedSize;
	return S_OK;

}
HRESULT CWmiGetInfo::GetWmiInfo( LPCSTR lpszClass)
{
	HRESULT hr=E_FAIL;
	CComPtr<IEnumWbemClassObject> spEnumWbemClassObject ;
	CComBSTR bstrQuery ( L"Select * from " ) ;
	CComBSTR bstrTemp;
	ULONG uCount = 1, uReturned;
	int iEnumIdx = 0;
	CComPtr<IWbemClassObject> spClassObject ;

	FAILEXIT(lpszClass,E_FAIL);

	bstrTemp=lpszClass;
	bstrQuery += bstrTemp;

	hr=m_cWbemServices->ExecQuery( L"WQL" , bstrQuery , WBEM_FLAG_RETURN_IMMEDIATELY , NULL, &spEnumWbemClassObject ); 
	FAILEXIT(SUCCEEDED(hr),hr);

	
	hr=spEnumWbemClassObject->Reset();
	FAILEXIT(SUCCEEDED(hr),hr);
	

	while( spEnumWbemClassObject->Next( WBEM_INFINITE, uCount, &spClassObject, &uReturned) == S_OK )

	{
			hr=GetClassInfo( spClassObject) ;

			spClassObject.Release() ;

	}
	return hr;

	
}
HRESULT CWmiGetInfo::GetClassInfo(IWbemClassObject *pClassObject)
{
	HRESULT hr=E_FAIL;
	SAFEARRAY *pvNames = NULL;
	CHAR *strName,*strValue;
	FAILEXIT(pClassObject,E_INVALIDARG);
	hr=pClassObject->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_MASK_CONDITION_ORIGIN, NULL, &pvNames);
	FAILEXIT(SUCCEEDED(hr),hr);
	long vbl, vbu;		
	SafeArrayGetLBound(pvNames, 1, &vbl);
	SafeArrayGetUBound(pvNames, 1, &vbu);
	for(long i = vbl ; i<= vbu; i++ )
	{

		long index = i ;
		WCHAR *wsName = NULL ;
		CComVariant varValue ;
		SafeArrayGetElement( pvNames, &index , &wsName ) ;
		CComBSTR bstrName ( wsName ) ;
		if( pClassObject->Get( bstrName , 0 , &varValue , NULL , 0 ) == S_OK )
		{
			CComBSTR str ;
			if( varValue.vt == VT_BSTR )
			{
				strName=ConvertBSTRToString(bstrName);
				strValue=ConvertBSTRToString(varValue.bstrVal);
				if(stricmp("ProcessorId",strName)==0)
				{
					m_InfoMap["ProcessorId"]=strValue;
				}
				else if(stricmp("Model",strName)==0)
				{
					m_InfoMap["DiskSerial"]=strValue;
				}
				else if(stricmp("MACAddress",strName)==0)
				{
					m_InfoMap["MACAddress"]=strValue;
				}
				str = varValue.bstrVal ;
			}
			else if( varValue.vt == VT_ARRAY )
			{
				long iLowBound = 0 , iUpBound = 0 ;
				SafeArrayGetLBound( varValue.parray , 1 , &iLowBound ) ;
				SafeArrayGetUBound( varValue.parray , 1 , &iUpBound ) ;
				for( long j = iLowBound ; j <= iUpBound ; j ++ )
				{
					VARIANT *pvar = NULL ;
					long temp = j ;
					if( SafeArrayGetElement( varValue.parray , &temp , pvar ) == S_OK &&pvar )
					{
							CComVariant varTemp ;
							if( varTemp.ChangeType( VT_BSTR , pvar ) == S_OK )
							{
								if( !str.Length() )
									str += _T(",") ;
								str += varTemp.bstrVal ;
							}                                                                

					}

				 }

			}
			else
			{
				str=L"unknown";
			}
			
/*
						 strName=ConvertBSTRToString(bstrName);
						 strValue=ConvertBSTRToString(str);
						 if(bstrName.Length()&&str.Length())
						 {
			
							// 打印出硬件信息
							 printf("%s %s \r\n",strName,strValue);
							 delete[] strName;
							 delete[] strValue;
						 }
						*/
			
			
			

		}                            

	}

	if(pvNames)
	{
		SafeArrayDestroy(pvNames);
		return S_OK ;
	}

	return E_FAIL ;


	
}