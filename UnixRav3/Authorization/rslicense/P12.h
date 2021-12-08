#pragma once

//#include "IX509.h"

/*
封装的p12文件的解密类
*/

#include <openssl/x509v3.h>
#include <openssl/pkcs12.h>
#include <openssl/err.h>


#define  MAX_DESC_LEN (12*1024)
#define  MAX_NAME_LEN        120
#define  MAX_TIME_LEN        20
#define  MAX_ADDRESS_LEN     260
#define  MAX_SERRIAL_LEN     50
#define  MAX_ISSUER_LEN      50

typedef struct tag_x509_info
{	//	证书需要的数据，需要明确
	int  nVersion;		
	long nSerialNumber;
	int  nDays;
	//Issuer
	BYTE szIssuer[MAX_ISSUER_LEN];

	//subject
	//CN
	BYTE szSubjectName[MAX_ISSUER_LEN];
	//C 国家/地区
	BYTE szCountryName[MAX_NAME_LEN];
	//L 城市
	BYTE szLocalityName[MAX_NAME_LEN];
	//O 组织名
	BYTE szOrganizationName[MAX_NAME_LEN];
	//email
	BYTE szEmail[MAX_ADDRESS_LEN];
	//ext
	BYTE  szSerial[MAX_SERRIAL_LEN];
	BYTE  szParentSerial[MAX_SERRIAL_LEN];
	DWORD dwType;
	DWORD dwKind;
	DWORD dwAuthTotal;

	//Time
	char  szStartTime[MAX_TIME_LEN];
	char  szEndTime[MAX_TIME_LEN];

	char  szDescription[MAX_DESC_LEN];
} ST_X509_INFO, *PST_X509_INFO;


typedef struct  
{
	DWORD    dwStartTime;
	CStringA strStartTime;//方便调试时候 看着数据
	CStringA strEndTime;
	CStringA strVerify;
	CStringA strKey;
}P12;

class CP12
{
public:
	CP12(void);
	~CP12(void);

	BOOL Decrypt(CStringA strPwd, BYTE* pBuffer, int nBufferSize, P12& p12);
	BOOL  SaveX509(CStringA strPath);
private:
	void ParserBaseInfo(ST_X509_INFO& info, X509* lpX509);
	void ParserExtInfo(ST_X509_INFO& info, X509* lpX509);
	BOOL ParserOneExt(X509_EXTENSION *ext,X509V3_EXT_METHOD * method,char*szOut,DWORD len);
	BOOL ParsePKCS12(PKCS12* pPkcs12, CStringA strPwd, BYTE* pBuffer, int nBufferSize);
	BOOL DecryptBuffer(CStringA strPwd, BYTE* pBuffer, int nBufferSize);
	BOOL DecryptDescript(CStringA& strXml);
	BOOL ParseXml(CStringA strXml, CStringA& strVerify, CStringA& strKey);	
	DWORD GetStartTime();
private:

	ST_X509_INFO	m_info;
};
