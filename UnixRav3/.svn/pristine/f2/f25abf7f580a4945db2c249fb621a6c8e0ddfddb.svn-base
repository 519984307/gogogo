#include "StdAfx.h"
#include "RisingAccredit.h"
#include "base64.h"
#include "AesCbc.h"
#include "RisingSN.h"
#include "xmltree.h"
#include "P12.h"
#include "md5_c.h"

//#include <syslog.h>
//#define  LOG(_fmt, args...)    syslog(LOG_ERR, _fmt, ##args)


CRisingAccredit::CRisingAccredit(void)
{
#ifdef linux_mode
	m_strUnZipPath = "\\temp";
#else
	std::string s = "d:\\test";
	m_strUnZipPath = s;
#endif
}

CRisingAccredit::~CRisingAccredit(void)
{
}

DWORD CRisingAccredit::VerifyAccredit(LPCSTR lpszBaseNumber, LPCSTR lpszLic)
{
	m_strBaseNumber = lpszBaseNumber;
	m_strLicPath	= lpszLic;

	HRESULT hr = DecryptLic();
	DeleteUnzipPath();
	if(FAILED(hr))
		return Accredit_Error;

	if(!m_vResults.size())
	{
		return Accredit_Nodata;
	}

	return Accredit_Success;
}

DWORD CRisingAccredit::ParseAccredit(LPCSTR lpszBaseNumber, LPCSTR lpszUnZipPath, DWORD dwType)
{
	m_strBaseNumber = lpszBaseNumber;
	m_strUnZipPath	= lpszUnZipPath;

	HRESULT hr = ParseAccredit(dwType);
	return  hr;
}

void  CRisingAccredit::SetMemoryFileList(MemoryFileList & lstMem)
{
	m_lstMem = lstMem;
}

void  CRisingAccredit::DumpXml()
{
	//syslog(LOG_ERR, ">> xml -->%s", m_strLicenseDatXml.GetBuffer());
}


HRESULT CRisingAccredit::ParseAccredit(DWORD dwType)
{
	HRESULT              hr;
	std::vector<P12ITEM> vP12Items;

	hr = DecryptLicenseDat();
	LOG(">> ParseAccredit, DecryptLicenseDat return %d\n", (int)hr);
	if(S_OK != hr) return hr;

	hr = ReadXml_LicenseDataXml(vP12Items);
	LOG(">> ParseAccredit, ReadXml_LicenseDataXml return %d\n", (int)hr);
	if(S_OK != hr) return hr;

	m_vResults.clear();
	std::vector<P12ITEM>::iterator iter;
	for(iter = vP12Items.begin(); iter != vP12Items.end(); ++iter)
	{
		if(iter->dwType == dwType)
		{
			P12 p12;

			RFAILED(DecryptP12(iter->strSN, iter->strP12, iter->dwSNVer, p12));
			m_vP12.push_back(p12);
			RFAILED(DecryptBin(iter->strBinFileName, iter->strBin, p12));
		}
	}
	LOG(">> ParseAccredit ok!\n");
	return S_OK;
}

HRESULT CRisingAccredit::DecryptLic()
{
	std::vector<P12ITEM> vP12Items;

	RFAILED(UnzipLic(m_strLicPath.GetBuffer()));
	RFAILED(DecryptLicenseDat());

	RFAILED(ReadXml_LicenseDataXml(vP12Items));
	return S_OK;
}

HRESULT CRisingAccredit::UnzipLic(LPCSTR lpszLic)
{
	return S_OK;
}

HRESULT CRisingAccredit::DecryptLicenseDat()
{
	BYTE  * pVoid = NULL;
	DWORD   dwVoidSize = 0;
	char    strFile[MAX_PATH];

	sprintf(strFile, "%s/license.dat", m_strUnZipPath.GetBuffer());
	LOG("ReadFileMemory(%s) +\n", strFile);
	RFAILED(ReadFileMemory(strFile, &pVoid, dwVoidSize));
	LOG("ReadFileMemory(%s) -\n", strFile);

	//base64 convert
	DWORD dwBase64DecodeSize	= dwVoidSize;
	BYTE* pBase64Decode			= new BYTE[dwVoidSize];
	memset(pBase64Decode, 0, dwVoidSize);
	dwBase64DecodeSize = Base64Decode((const char*)pVoid,  pBase64Decode, dwVoidSize);

	delete[] pVoid;
	pVoid = NULL;

	HRESULT hr = DecryptLicenseDat(pBase64Decode, dwBase64DecodeSize, TRUE);
	if(FAILED(hr))
	{
		hr = DecryptLicenseDat(pBase64Decode, dwBase64DecodeSize, FALSE);
	}
	delete[] pBase64Decode;
	pBase64Decode = NULL;
	return hr;
}

HRESULT CRisingAccredit::DecryptLicenseDat(BYTE* pBytes, int nBytesSize, BOOL bVer2)
{
	CAesCbc aes;
	aes.SetMd5Mode(bVer2);
	if(!aes.SetBaseNumber(m_strBaseNumber.GetBuffer()))
	{
		LOG(">> DecryptLicenseDat(), SetBaseNumber failed\n");
		return E_FAIL;
	}

	m_strLicenseDatXml = aes.Descrypt(pBytes, nBytesSize);
	TrimXmlString(m_strLicenseDatXml);
	LOG(">> xml --->%s\n", m_strLicenseDatXml.GetBuffer());

	CXMLTree xml;
	xml.LoadBuff(m_strLicenseDatXml.GetBuffer());	//校验xmlString合法性

	HXMLTREEITEM hRoot = xml.GetChild(xml.GetRoot(), "RISING");
	RASSERT(hRoot, E_FAIL);//悲剧的发现，即使是乱码LoadBuff也会成功！所以修改为读取里面结点方式！！！

	#ifdef __LOG_XML__
	char strSavePath[MAX_PATH] = {0};
	sprintf(strSavePath, "%s/license.dat.xml", m_strUnZipPath.GetBuffer());
	if(!xml.Save(strSavePath))
	{
		LOG(">> DecryptLicenseDat(), Save failed\n");
		return E_FAIL;
	}
	#endif
	return S_OK;
}

HRESULT CRisingAccredit::ReadFileMemory(LPCSTR lpszFile, BYTE** ppVoid, DWORD& dwVoidSize)
{
	// 首先从内存里读
	if(!m_lstMem.empty())
	{
		const char * p = strrchr(lpszFile, '/');
		if(!p) p = strrchr(lpszFile, '\\');
		if(p)
		{
			MemoryFileList::const_iterator it = m_lstMem.find(p+1);
			if(it != m_lstMem.end())
			{
				dwVoidSize = it->second.length();
				*ppVoid = new BYTE[dwVoidSize+1];
				if(*ppVoid)
				{
					memcpy(*ppVoid, it->second.c_str(), it->second.length());
					(*ppVoid)[dwVoidSize] = 0;
				}
				return *ppVoid ? S_OK:E_FAIL;
			}
		}
	}

	FILE* f = fopen(lpszFile, "rb");
	if(NULL == f)
	{
		printf("fopen failed : %s\n", lpszFile);
		return E_FAIL;
	}
	fseek(f,0L,SEEK_END);
	int size=ftell(f);
	fseek(f,0L,SEEK_SET);
	if(-1 == size || 0 == size)
	{
		fclose(f);
		return E_FAIL;
	}

	*ppVoid = new BYTE[size+1];
	memset(*ppVoid, 0, sizeof(BYTE)*(size+1));
	int readSize = fread(*ppVoid, sizeof(BYTE), size+1, f);
	fclose(f);
	dwVoidSize = size;
	return (size == readSize)?S_OK:E_FAIL;
}

HRESULT CRisingAccredit::ReadXml_LicenseDataXml(std::vector<P12ITEM>& vItems)
{
	CXMLTree xml;
	RASSERT(xml.LoadBuff(m_strLicenseDatXml.GetBuffer()), E_FAIL);
	HXMLTREEITEM hRoot = xml.GetChild(xml.GetRoot(), "RISING");
	RASSERT(hRoot, E_FAIL);
	HXMLTREEITEM hItems = xml.GetChild(hRoot, "ITEMS");
	RASSERT(hItems, E_FAIL);
	HXMLTREEITEM hItem = xml.GetChild(hItems, "ITEM");
	while(hItem)
	{
		HXMLTREEITEM hType = xml.GetChild(hItem, "TYPE");
		RASSERT(hType,E_FAIL);
		CStringA strType = xml.GetText(hType);

		P12ITEM p12;
		HXMLTREEITEM hSN = xml.GetChild(hItem, "SN");
		RASSERT(hSN, E_FAIL);
		p12.strSN = xml.GetText(hSN);
		CStringA strSnVer = xml.GetAttribute(hSN, "VER");
		if(strSnVer == "2.0")
			p12.dwSNVer = P12ITEM::SN_VER2;
		else
			p12.dwSNVer = P12ITEM::SN_VER1;		//这里目前就两种版本，没有版本号也用1.0的方式进行处理

		HXMLTREEITEM hP12 = xml.GetChild(hItem, "FILENAME");
		CStringA strFileName = xml.GetText(hP12);
		char temp[MAX_PATH] = {0};
		sprintf(temp, "%s/%s", m_strUnZipPath.GetBuffer(), strFileName.GetBuffer());
		p12.strP12 = temp;
		strFileName.Replace("p12", "bin");
		sprintf(temp, "%s/%s", m_strUnZipPath.GetBuffer(), strFileName.GetBuffer());
		p12.strBin = temp;
		p12.strBinFileName = strFileName;
		p12.dwType = atoi(strType.GetBuffer());

		vItems.push_back(p12);
		hItem = xml.GetNextItem(hItem);
	}

	return vItems.size()?S_OK:E_FAIL;
}

void    CRisingAccredit::TrimXmlString(CStringA& strXml)
{
	strXml.Trim();
	int nCount = strXml.GetLength();
	for(int i = nCount - 1; i >= 0; i--)
	{
		if(strXml.GetAt(i) == '>')
		{
			if( i < nCount-1)
			{
				//strXml.SetAt(i+1, '\0');
				strXml.mString[i+1] = '\0';
				//strXml.ReleaseBuffer(i+1);
			}
			break;
		}
	}
}

HRESULT CRisingAccredit::DecryptP12(CStringA strSN, CStringA strP12, DWORD dwSnVer, P12& p12info)
{
	//get real sn
	CStringA strRealSN;
	CRisingSN sn;
	sn.SetMd5Mode(P12ITEM::SN_VER2 == dwSnVer);
	RASSERT(sn.Descrypt(strSN, strRealSN), E_FAIL);
	//read file buffer
	BYTE* pBuffer = NULL;
	DWORD  dwBufferLen;
	RFAILED(ReadFileMemory(strP12.GetBuffer(), &pBuffer, dwBufferLen));
	CP12 p12;
	BOOL bRes = p12.Decrypt(strRealSN.GetBuffer(), pBuffer, dwBufferLen, p12info);
	delete[] pBuffer;
	pBuffer = NULL;
	RASSERT(bRes, E_FAIL);
	#ifdef __LOG_XML__
	RASSERT(p12.SaveX509(strP12), E_FAIL);
	#endif
	return S_OK;
}


HRESULT CRisingAccredit::DecryptSN(CStringA strSrcSN, CStringA& strDstSN)
{
	//REMOVE -
	strSrcSN.Remove('-');

	return S_OK;
}

HRESULT CRisingAccredit::DecryptBin(CStringA strBinFileName, CStringA strBin, P12 p12)
{
	CStringA strXml;
	RFAILED(GetXmlFromBin(strBin, p12, strXml));
	m_vBin.push_back(strXml);
	CXMLTree xml;
	RASSERT(xml.LoadBuff(strXml.GetBuffer()), E_FAIL);
	HXMLTREEITEM hExt = xml.GetChild(xml.GetRoot(), "Ext");

	#ifdef __LOG_XML__
	char savePath[256]={0};
	sprintf(savePath, "%s/%s.xml", m_strUnZipPath.GetBuffer(), strBinFileName.GetBuffer());
	xml.Save(savePath);
	#endif

	int nMonth = 0;
	AccreidtItem unit;
	RASSERT(hExt, E_FAIL);
	HXMLTREEITEM hMonth = xml.GetChild(hExt, "Month");
	RASSERT(hMonth, E_FAIL);
	LPCSTR lpszMonth = xml.GetText(hMonth);
	RASSERT(lpszMonth, E_FAIL);
	nMonth = atoi(lpszMonth);
	CTime tm(p12.dwStartTime);
	int nStartMonth = tm.GetMonth();
	int nStartYear  = tm.GetYear();
	int nEndMonth   = nStartMonth + nMonth;
	int nEndYear    = nStartYear;
	while(nEndMonth > 12)	//月份的取值范围是[1...12]
	{
		nEndYear++;
		nEndMonth-=12;
	}
	/* 下面是zhaocg提供的算法，先留着，先用自己的算法， 有问题在换这个。
	nEndMonth += nMonth;
	nEndYear  += nEndMonth / 12;
	nEndMonth  = nEndMonth % 12;
	if(!nEndMonth) { nEndYear--; nEndMonth = 12; }
	*/

	CTime tm2(nEndYear, nEndMonth, tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	unit.strEndTime  = p12.strEndTime.GetBuffer();
	unit.dwStartTime = p12.dwStartTime;
	unit.dwEndTime   = (DWORD)tm2.GetTime();
	unit.dwRet       = 0;

	HXMLTREEITEM hItems = xml.GetChild(hExt, "Items");
	HXMLTREEITEM hItem  = xml.GetChild(hItems, "Item");
	while(hItem)
	{
		HXMLTREEITEM hFunc = xml.GetChild(hItem, "Func");
		RASSERT(hFunc, E_FAIL);
		HXMLTREEITEM hCnt = xml.GetChild(hItem, "Cnt");
		RASSERT(hCnt, E_FAIL);

		LPCSTR lpszGuid = xml.GetText(hFunc);
		LPCSTR lpszCount = xml.GetText(hCnt);
		RASSERT(lpszGuid&&lpszCount, E_FAIL);

		unit.strGuid = lpszGuid;
		unit.dwCnt   = atoi(lpszCount);

		PushResult(unit);
		hItem = xml.GetNextItem(hItem, "Item");
	}

	return S_OK;
}

HRESULT CRisingAccredit::GetXmlFromBin(CStringA strBin, P12 p12, CStringA& strXml)
{
	BYTE* pBuffer = NULL;
	DWORD dwBufferLen = 0;
	RFAILED(ReadFileMemory(strBin.GetBuffer(), &pBuffer, dwBufferLen));

	BYTE* pDecode64 = new BYTE[dwBufferLen];
	memset(pDecode64, 0, sizeof(BYTE)*dwBufferLen);
	DWORD dwDecode64Len = Base64Decode((const char*)pBuffer, pDecode64, dwBufferLen);
	delete[] pBuffer;
	pBuffer = NULL;

	CAesCbc aes;
	aes.SetMd5Mode(FALSE);
	aes.SetBinKey(p12.strKey.GetBuffer());
	strXml = aes.Descrypt(pDecode64, dwDecode64Len);
	delete pDecode64;
	pDecode64 = NULL;

	TrimXmlString(strXml);
	//MD5
	int nLen = strXml.GetLength();
	md5_byte_t buf[16];
	md5_state_s mst;
	md5_init(&mst);
	md5_append(&mst, (const md5_byte_t*)strXml.GetBuffer(), strXml.GetLength());
	md5_finish(&mst, buf);

	CStringA strMD5;
	for(int i = 0; i < 16 ; i++)
	{
		char temp[5];
		sprintf(temp, "%02X", buf[i]);
		strMD5 += temp;
	}

	if(strMD5 == p12.strVerify.MakeUpper())
		return S_OK;

	return E_FAIL;
}

void CRisingAccredit::PushResult(AccreidtItem& item)
{
	CTime tm = CTime::GetCurrentTime();
	DWORD dwCurrentTime = (DWORD)tm.GetTime();
	if(dwCurrentTime <  item.dwStartTime) item.dwRet = 1;
	if(dwCurrentTime >= item.dwEndTime)   item.dwRet = 2;

	VAccreditItems::iterator iter;
	for(iter = m_vResults.begin(); iter != m_vResults.end(); ++iter)
	{
		if(iter->strGuid == item.strGuid)
		{
			iter->dwStartTime = std::min(iter->dwStartTime, item.dwStartTime);
			iter->dwEndTime   = std::max(iter->dwEndTime,   item.dwEndTime);
			return ;
		}
	}

	m_vResults.push_back(item);
}

BOOL CRisingAccredit::IsCanInstall(LPCSTR lpszSPGuid)
{
	VAccreditItems::iterator iter;
	for(iter = m_vResults.begin(); iter != m_vResults.end(); ++iter)
	{
		if(iter->strGuid == lpszSPGuid)
			return TRUE;
	}
	return FALSE;
}

void CRisingAccredit::DeleteUnzipPath()
{
	//CUtils::DeletePath(m_strUnZipPath);
}
