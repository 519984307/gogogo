#pragma once
#include <vector>
#include <map>
#include <string>

#ifndef __NO_CRisingAccredit__
#include "P12.h"
#endif


enum
{
	Accredit_Success = 0,           //成功
	Accredit_Error   = -1,          //错误
	Accredit_Nodata  = 1,           //无有效授权
};

typedef struct  
{
	std::string strGuid;
	std::string strEndTime;
	int         dwCnt;              //运行的授权数量
	int         dwStartTime;
	int         dwEndTime;
	int         dwRet;              //0=valid, 1=nostart. 2=overdate.
}AccreidtItem;
typedef std::vector<AccreidtItem>	VAccreditItems;
typedef std::map<std::string, std::string> MemoryFileList;


#ifndef __NO_CRisingAccredit__
class CRisingAccredit
{
public:
	CRisingAccredit(void);
	~CRisingAccredit(void);

	// dwType: 0是功能模块, 1是中心模块
	DWORD VerifyAccredit(LPCSTR lpszBaseNumber, LPCSTR lpszLic);
	DWORD ParseAccredit(LPCSTR lpszBaseNumber, LPCSTR lpszDataDir, DWORD dwType);
	void  SetMemoryFileList(MemoryFileList & lstMem);
	void  DumpXml();
	VAccreditItems	m_vResults;


private:
	typedef struct  
	{
		enum
		{
			SN_VER1,
			SN_VER2,
		};
		CStringA strSN;
		CStringA strP12;
		CStringA strBin;
		CStringA strBinFileName;
		DWORD    dwSNVer, dwType;
	}P12ITEM;

	HRESULT DecryptLic();
	BOOL    IsCanInstall(LPCSTR lpszSPGuid);
	HRESULT ParseAccredit(DWORD dwType);

	HRESULT UnzipLic(LPCSTR lpszLic);
	HRESULT DecryptLicenseDat();
	HRESULT DecryptLicenseDat(BYTE* pBytes, int nBytesSize, BOOL bVer2);
	HRESULT DecryptP12(CStringA strSN, CStringA strP12, DWORD dwSnVer, P12& p12) ;
	HRESULT DecryptSN(CStringA strSrcSN, CStringA& strDstSN);
	HRESULT DecryptBin(CStringA strBinFileName, CStringA strBin, P12 p12);

	HRESULT GetXmlFromBin(CStringA strBin, P12 p12, CStringA& strXml);
	HRESULT ReadXml_LicenseDataXml(std::vector<P12ITEM>& vItems);
	HRESULT ReadFileMemory(LPCSTR lpszFile, BYTE** ppVoid, DWORD& dwVoidSize);
	void TrimXmlString(CStringA& strXml);
	void PushResult(AccreidtItem& item);
	void DeleteUnzipPath();

	CStringA  m_strBaseNumber;
	CStringA  m_strLicPath;

	CStringA  m_strLicenseDatXml;
	CStringA  m_strUnZipPath;

	//log data
	std::vector<P12> m_vP12;
	std::vector<CStringA> m_vBin;
	MemoryFileList   m_lstMem;
};
#endif
