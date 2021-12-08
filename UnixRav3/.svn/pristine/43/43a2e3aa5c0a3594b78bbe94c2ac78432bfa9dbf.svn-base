#include "StdAfx.h"
#include "P12.h"
#include "base64.h"
#include "xmltree.h"

CP12::CP12(void)
{
	OpenSSL_add_all_algorithms();
	//SSLeay_add_all_algorithms();
}

CP12::~CP12(void)
{
}

BOOL CP12::Decrypt(CStringA strPwd, BYTE* pBuffer, int nBufferSize, P12& p12)
{
	CStringA strXml, strVerify, strKey;
	RASSERT(DecryptBuffer(strPwd, pBuffer, nBufferSize), FALSE);
	RASSERT(DecryptDescript(strXml), FALSE);
	RASSERT(ParseXml(strXml, strVerify, strKey), FALSE);
	p12.dwStartTime = GetStartTime();
	p12.strStartTime= m_info.szStartTime;
	p12.strEndTime  = m_info.szEndTime;
	p12.strVerify   = strVerify;
	p12.strKey		= strKey;
	return p12.dwStartTime?TRUE:FALSE;
}


void CP12::ParserBaseInfo(ST_X509_INFO& info, X509* lpX509)
{
	memset(&info,0,sizeof(info));
	if(lpX509 == NULL)
		return;	
	//版本号
	info.nVersion = X509_get_version(lpX509);
	//系列号
	ASN1_INTEGER * asn_Int = X509_get_serialNumber(lpX509);
	info.nSerialNumber = ASN1_INTEGER_get(asn_Int);

	//时间
	ASN1_TIME *asn_time = X509_get_notBefore(lpX509);
	memcpy(info.szStartTime, asn_time->data,asn_time->length);
	asn_time = X509_get_notAfter(lpX509);
	memcpy(info.szEndTime, asn_time->data,asn_time->length);

	//颁发者
	X509_NAME *pName = X509_get_issuer_name(lpX509);
	X509_NAME_get_text_by_NID(pName,NID_commonName,(char*)info.szIssuer,MAX_ISSUER_LEN);

	//主题
	pName = X509_get_subject_name(lpX509);
	int nCount = X509_NAME_entry_count(pName);

	//ASN1_GENERALIZEDTIME * g_time;
	//ASN1_TIME_
	//g_time = ASN1_TIME_to_generalizedtime(asn_time,&g_time);

	X509_NAME_get_text_by_NID(pName,NID_commonName,(char*)info.szSubjectName,MAX_ISSUER_LEN);
	X509_NAME_get_text_by_NID(pName,NID_countryName,(char*)info.szCountryName,MAX_NAME_LEN);
	X509_NAME_get_text_by_NID(pName,NID_localityName,(char*)info.szLocalityName,MAX_NAME_LEN);
	X509_NAME_get_text_by_NID(pName,NID_organizationName,(char*)info.szOrganizationName,MAX_NAME_LEN);


	X509_NAME_get_text_by_NID(pName,NID_description,(char*)info.szDescription,MAX_DESC_LEN);

	ASN1_OBJECT* obj = NULL;
	obj = OBJ_txt2obj("emailAddress",0);
	if(obj == NULL) 
	{
		return;
	}
	X509_NAME_get_text_by_OBJ(pName,obj,(char*)info.szEmail,MAX_ADDRESS_LEN);

	//obj = OBJ_txt2obj("Address",0);
	//if(obj == NULL)
	//{
	//	return;
	//}
	//X509_NAME_get_text_by_OBJ(pName,obj,(char*)m_Info.szAddress,MAX_ADDRESS_LEN);

}


BOOL CP12::ParserOneExt(X509_EXTENSION *ext,X509V3_EXT_METHOD * method,char*szOut,DWORD len)
{
	//const unsigned char *p;
	void *ext_str = NULL;
	char *value = NULL;
	STACK_OF(CONF_VALUE) *nval = NULL;
	BOOL ok = 1;
	int indent =0x10;

	ASN1_OCTET_STRING * ext_value = X509_EXTENSION_get_data(ext);

	if(method->it) ext_str = ASN1_item_d2i(NULL, (const unsigned char**)&ext_value->data, ext_value->length, ASN1_ITEM_ptr(method->it));
	else ext_str = method->d2i(NULL, (const unsigned char**)&ext_value->data, ext_value->length);

	if(!ext_str) {
		return FALSE;
	}

	if(method->i2s)
	{
		if(!(value = method->i2s(method, ext_str)))
		{
			ok = 0;
			goto err;
		}
		if(len >= strlen(value))
		{
			strcpy(szOut,value);
		}
	}
	else if(method->i2v) 
	{
		if(!(nval = method->i2v(method, ext_str, NULL))) 
		{
			ok = 0;
			goto err;
		}
		CONF_VALUE *conf_val =NULL;
		char * tmp;
		for(int i = 0; i < sk_CONF_VALUE_num(nval); i++) 
		{
			conf_val = sk_CONF_VALUE_value(nval, i);
			if(conf_val->name)
				tmp = conf_val->name;
			if(conf_val->value)
			{
				if(len >= strlen(conf_val->value))
				{
					strcpy(szOut,conf_val->value);
				}
			}
		}
	} 
	//else if(method->i2r)
	//{
	//	if(!method->i2r(method, ext_str, out, indent))
	//		ok = 0;
	//} 
	else 
		ok = 0;
err:
	sk_CONF_VALUE_pop_free(nval, X509V3_conf_free);
	if(value) OPENSSL_free(value);
	if(method->it) ASN1_item_free((ASN1_VALUE *)ext_str, ASN1_ITEM_ptr(method->it));
	else method->ext_free(ext_str);	
	return ok;
}

void CP12::ParserExtInfo(ST_X509_INFO& info, X509* lpX509)
	{
		X509_EXTENSION *ext;
		ASN1_OBJECT * obj;
		//X509V3_EXT_METHOD * method;
		//X509_get_ext_by_NID(m_pX509,);
		int nNum = X509_get_ext_count(lpX509);
		char buf[256];
		for(int i =0;i<nNum;i++)
		{
			ext = X509_get_ext(lpX509,i);
			obj = X509_EXTENSION_get_object(ext);
			//ss
			OBJ_obj2txt(buf,256,obj,0);
			int nID =OBJ_obj2nid(obj);
			X509V3_EXT_METHOD * method =  (X509V3_EXT_METHOD *)X509V3_EXT_get(ext);
			if(nID == NID_netscape_comment)
			{

                ////////没按如此定义了
                /*
				char szTmp[100]={0};
				ParserOneExt(ext,method,szTmp,100);
				char szFormat[] = "%d,%d,%d";
				_snscanf_s(szTmp,100,szFormat,
					&(m_Info.dwType),&(m_Info.dwKind),&(m_Info.dwAuthTotal) );
                */

			}
			else if(nID == NID_issuer_alt_name)
			{
				ParserOneExt(ext,method,(char*)info.szParentSerial,MAX_SERRIAL_LEN);
			}
			else if(nID == NID_subject_alt_name)
			{
				ParserOneExt(ext,method,(char*)info.szSerial,MAX_SERRIAL_LEN);
			}

			//method =  X509V3_EXT_get_nid(NID_netscape_comment);
			//void *pData = X509V3_EXT_d2i(ext);
		}

	}

BOOL CP12::ParsePKCS12(PKCS12* pPkcs12, CStringA strPwd, BYTE* pBuffer, int nBufferSize)
{
	char  szPwd[100];
	memset(szPwd,0,100);
	memcpy(szPwd, strPwd.GetBuffer(),strPwd.GetLength()*sizeof(char));

	X509	*pCert=NULL;	
	STACK_OF(X509)* pCA=NULL;
	EVP_PKEY	*pKey=NULL;
	int ret=PKCS12_parse(pPkcs12, szPwd,&pKey,&pCert,&pCA);
	DWORD dwErr=  ERR_get_error();
	if(ret!=1 || !pCert)		
		return FALSE;		

	ST_X509_INFO x509Info;
	ParserBaseInfo(x509Info, pCert);
	ParserExtInfo(x509Info, pCert);

	memset(&m_info,0,sizeof(m_info));
	memcpy(&m_info,&x509Info,sizeof(ST_X509_INFO));
	return TRUE;
}

BOOL CP12::DecryptBuffer(CStringA strPwd, BYTE* pBuffer, int nBufferSize)
{
	BYTE* pTmpBuf = pBuffer;
	PKCS12* pPkcs12 = NULL;
	pPkcs12 = d2i_PKCS12(&pPkcs12,( const unsigned char **)&pTmpBuf,nBufferSize);
	if(pPkcs12 == NULL)
	{
		return FALSE;
	}
	
	BOOL bRes = ParsePKCS12(pPkcs12, strPwd, pBuffer, nBufferSize);
	PKCS12_free(pPkcs12);
	pPkcs12 = NULL;
	return bRes;
	/*
	m_cldr.LoadRSCom(_T("rsssl.dll"));

	m_pIX509 = (IUnknown*) NULL;
	HRESULT hr = m_cldr.CreateInstance(CLSID_RSX509,NULL,NULL, __uuidof(IX509), (void**)&m_pIX509);
	if(hr != S_OK || m_pIX509==NULL)
	{
		return FALSE;
	}

	m_pCertificate =(IUnknown*)  NULL;
	hr = m_cldr.CreateInstance(CLSID_RSCertificate,NULL,NULL, __uuidof(ICertificate), (void**)&m_pCertificate);
	if(hr != S_OK || m_pCertificate==NULL)
	{
		return FALSE;
	}

	m_pCertParser =(IUnknown*)  NULL;
	hr = m_cldr.CreateInstance(CLSID_RSCertParser,NULL,NULL, __uuidof(ICertParser), (void**)&m_pCertParser);
	if(hr != S_OK || m_pCertParser==NULL)
	{
		return FALSE;
	}

	RS_SERIAL_NO serial;
	strcpy((char*)serial.szSerial,strPwd.GetBuffer());
	strPwd.ReleaseBuffer();


	hr  = m_pCertificate->Import(pBuffer,nBufferSize,&serial);
	if(FAILED(hr))
	{
		return FALSE;
	}

	m_pCertParser->SetPassword((BYTE*)serial.szSerial,strlen((char*)serial.szSerial));
	hr = m_pCertParser->Parse(m_pCertificate);
	if(FAILED(hr))
	{
		return FALSE;
	}

	IX509*  pIX509 = m_pCertParser->GetIX509();
	if(NULL == pIX509)
		return FALSE;

	
	memset(&m_info,0,sizeof(m_info));
	pIX509->GetX509Info(&m_info);
	if(strlen(m_info.szDescription) && strlen(m_info.szStartTime))
	{
		return TRUE;
	}	
	*/
	return FALSE;
}

BOOL CP12::DecryptDescript(CStringA& strXml)
{
	DWORD dwLen	= strlen(m_info.szDescription);
	BYTE* pResult = new BYTE[dwLen];
	memset(pResult, 0, dwLen*sizeof(BYTE));
	dwLen = Base64Decode(m_info.szDescription,  pResult, dwLen);
	strXml = (char*)pResult;
	delete[] pResult;
	return TRUE;
}

BOOL CP12::ParseXml(CStringA strXml, CStringA& strVerify, CStringA& strKey)
{
	CXMLTree xml;
	BOOL bRes = xml.LoadBuff(strXml.GetBuffer());
	RASSERT(bRes, FALSE);

	HXMLTREEITEM hInfo = xml.GetChild(xml.GetRoot(), "info");
	RASSERT(hInfo, FALSE);
	HXMLTREEITEM hVerify = xml.GetChild(hInfo, "verify");
	RASSERT(hVerify, FALSE);
	HXMLTREEITEM hKey = xml.GetChild(hInfo, "key");
	RASSERT(hKey, FALSE);

	strVerify = xml.GetText(hVerify);
	strKey    = xml.GetText(hKey);
	return TRUE;
}

DWORD CP12::GetStartTime()
{
	CStringA strStartTime;
	strStartTime = m_info.szStartTime;
	strStartTime.Remove('z');
	strStartTime.Remove('Z');

	CTime tm = CTime::GetCurrentTime();
	//0000 00 00 00 00 00
	int nLen = strStartTime.GetLength();
	if(12 == nLen)
	{
		char szYear[10];
		sprintf(szYear, "%d", tm.GetYear());
		szYear[2] = '\0';
		char temp[100] = {0};
		sprintf(temp, "%s%s", szYear, strStartTime.GetBuffer());
		strStartTime = temp;
	}
	nLen = strStartTime.GetLength();
	if(14 != nLen)
		return 0;

	int nYear  = atoi(strStartTime.Mid(0,4).c_str());
	int nMon   = atoi(strStartTime.Mid(4,2).c_str());
	int nDay   = atoi(strStartTime.Mid(6,2).c_str());
	int nHour  = atoi(strStartTime.Mid(8,2).c_str());
	int nMin   = atoi(strStartTime.Mid(10,2).c_str());
	int nSec   = atoi(strStartTime.Mid(12,2).c_str());
	
	CTime tm2(nYear, nMon, nDay, nHour, nMin, nSec);
	return tm2.GetTime();
}

BOOL  CP12::SaveX509(CStringA strPath)
{
	CXMLTree xml;
	HXMLTREEITEM hRoot = xml.InsertChild(xml.GetRoot(), "root");
	RASSERT(hRoot, FALSE);
	HXMLTREEITEM hNotBefore = xml.InsertChild(hRoot, "notbefore");
	RASSERT(hNotBefore, FALSE);
	HXMLTREEITEM hNotAfter = xml.InsertChild(hRoot, "notafter");
	RASSERT(hNotAfter, FALSE);
	RASSERT(xml.SetText(hNotBefore, m_info.szStartTime), FALSE);
	RASSERT(xml.SetText(hNotAfter, m_info.szEndTime), FALSE);
	char szSavePath[MAX_PATH] = {0};
	sprintf(szSavePath, "%s.xml", strPath.GetBuffer());
	return xml.Save(szSavePath);
}
