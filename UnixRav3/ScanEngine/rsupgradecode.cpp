#include "rsupgradecode.h"
#include "base64.h"
#include "time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ASSERT
	#include "assert.h"
	#define ASSERT assert
#endif

#define _tcslen strlen
#define _tcscat strcat
#define _tcschr strchr


#define PARAMETER_NAME_ID			_T("id=")
#define PARAMETER_NAME_SN			_T("&sn=")
#define PARAMETER_NAME_SETUP_TIME	_T("&Time_Setup=")
#define PARAMETER_NAME_OVER_TIME	_T("&Time_OverTime=")
#define PARAMETER_NAME_TYPE			_T("&type=")
#define PARAMETER_NAME_VALIDATE		_T("&Validate=")

//加密的结果：
//	6字节加解密随机数 ＋ 加密后的内容
#define CONVER_BOOK_SIZE		256

TCHAR g_szConvertBookD[CONVER_BOOK_SIZE + 1] = _T(	"welcome Rising*youarelawless!y2a"
													"3n4g5Y6U7q8i@S9I0N#A.C%O(M-)<>AB"
													"I993JIEM,;'{jkliewaqlsiqomv.z^iw"
													"aql}-_=+)_(l;2j2f90aslkjflkasjas"
													"32092JKLSJFbASAUI/Z/A[/,./|@~`FS"
													"'.Z,MF920SDLAFJKAL9320QFFMmlajfl"
													",.<>//|348q9729|fjlail3jo798,ksa"
													"fa302-s;akfa;=_++-0-_))0-0-p23is"
												);

TCHAR g_szConvertBook[CONVER_BOOK_SIZE + 1] = _T(	"welcome Rising*youarelawless!y2a"
													"$n4g5Y6U7q8i@S9I0N#A.C%O(M-)<>AB"
													"I99*JIEM,;'{jkliewaqlsiqomv.z^iw"
													"aql}-_=+)_(l;2j@f90aslkjflkasjas"
													"6j09kJKLSJFbASAUI/Z/A[/,./|@~`FS"
													"'.Z,MF920SDLAFJKAL9320QFFMmlajfl"
													",.<>//|348q9729|fjlail3jo798,ksa"
													"fa302-s;akfa;=_++-0-_))0-0-p^bis"
												);

LONG RsUpgradeEncrypt(LPTSTR pOut, LONG lOutBufSize, LPCTSTR pUserID, LPCTSTR pSn, DWORD dwTimeSetup, DWORD dwTimeOver, LONG lType, LONG lValidate)
{
	memset(pOut,0,lOutBufSize);
	if(!pOut || !pUserID || !pSn)
		return 0;

	TCHAR szSetupTime[33] = {0};
	TCHAR szOverTime[33] = {0};
	TCHAR szType[33] = {0};
	TCHAR szValidate[33] = {0};
	sprintf(szSetupTime, _T("%d"), dwTimeSetup);
	sprintf(szOverTime, _T("%d"), dwTimeOver);
	sprintf(szType, _T("%d"), lType);
	sprintf(szValidate, _T("%d"), lValidate);

	size_t lNeedLength = 0;
	lNeedLength += _tcslen(PARAMETER_NAME_ID) + _tcslen(pUserID);	//id=12345678&sn=xxxxxxxxx;xxxxx&Time_Setup=98765432
	lNeedLength += _tcslen(PARAMETER_NAME_SN) + _tcslen(pSn);
	lNeedLength += _tcslen(PARAMETER_NAME_SETUP_TIME) + _tcslen(szSetupTime);
	lNeedLength += _tcslen(PARAMETER_NAME_OVER_TIME) + _tcslen(szOverTime);
	lNeedLength += _tcslen(PARAMETER_NAME_TYPE) + _tcslen(szType);
	lNeedLength += _tcslen(PARAMETER_NAME_VALIDATE) + _tcslen(szValidate);

	TCHAR* szTemp = new TCHAR[lNeedLength + 1];
	memset(szTemp, 0, lNeedLength + 1);

	//填充内容
	_tcscat(szTemp, PARAMETER_NAME_ID);
	_tcscat(szTemp, pUserID);
	_tcscat(szTemp, PARAMETER_NAME_SN);
	_tcscat(szTemp, pSn);
	_tcscat(szTemp, PARAMETER_NAME_SETUP_TIME);
	_tcscat(szTemp, szSetupTime);
	_tcscat(szTemp, PARAMETER_NAME_OVER_TIME);
	_tcscat(szTemp, szOverTime);
	_tcscat(szTemp, PARAMETER_NAME_TYPE);
	_tcscat(szTemp, szType);
	_tcscat(szTemp, PARAMETER_NAME_VALIDATE);
	_tcscat(szTemp, szValidate);

	//加密
	LONG lRet = RsUpgradeEncrypt(pOut, lOutBufSize, szTemp);

	delete[] szTemp;

	return lRet;
}

LONG RsUpgradeEncrypt(LPTSTR pOut, LONG lOutBufSize, LPCTSTR pIn)
{
	memset(pOut,0,lOutBufSize);
	if(!pOut || !pIn)
		return 0;
	if(1 > _tcslen(pIn))
		return 0;

	LONG lNeedLength = 7 + (LONG)_tcslen(pIn);	// 加解密随机数在开始6个字符, 最后一个字节是校验码
	LONG lBeforeBase64Length = lNeedLength;
	//BASE64后的长度
	lNeedLength = (lNeedLength + 2) / 3;
	lNeedLength *= 4;

	if(lNeedLength >= lOutBufSize)
		return 0 - lNeedLength - 1;	//返回需要的内存大小

	TCHAR* szTemp = new TCHAR[lNeedLength + 1];
	memset(szTemp, 0, lNeedLength + 1);

	//生成随机数
	srand( (unsigned)time( NULL ) );
	WORD wTemp;
	for(int i = 0; i < 6; i++)
	{
		wTemp = rand();
		szTemp[i] = 0x00ff & wTemp;
	}

	//填充内容
	_tcscat(szTemp + 6, pIn);
	//计算校验和
	for(int i = 0; i < (int)_tcslen(pIn); i++)
	{
		szTemp[lBeforeBase64Length-1] += pIn[i];
	}

	//加密
	g_szConvertBook[32] = '3';
	g_szConvertBook[67] = '3';
	g_szConvertBook[111] = '2';
	g_szConvertBook[128] = '3';
	g_szConvertBook[129] = '2';
	g_szConvertBook[132] = '2';
	g_szConvertBook[252] = '2';
	g_szConvertBook[253] = '3';
	int nRandomIndex = 0;
	for(int i = 6; i < lBeforeBase64Length; i++)
	{
		szTemp[i] ^= g_szConvertBook[(BYTE)szTemp[nRandomIndex]];
		nRandomIndex ++;
		if(nRandomIndex > 5)
			nRandomIndex = 0;
	}

	//BASE64
	memset(pOut, 0, lOutBufSize);
	LONG lOutLength = Base64Encode((BYTE*)szTemp, lBeforeBase64Length, pOut);

	//‘+’号在传输时有问题，变成‘-’就没问题了。
	TCHAR* pFind = _tcschr(pOut, _T('+'));
	while(pFind)
	{
		*pFind = _T('-');
		pFind = _tcschr(pFind, _T('+'));
	}

	delete[] szTemp;

	return lOutLength;
}

LONG RsUpgradeDecrypt(LPTSTR pOut, LONG lOutBufSize, LPCTSTR pIn)
{
	memset(pOut,0,lOutBufSize);
	
	LONG lOutNeedSize = (LONG)_tcslen(pIn) * 3 / 4 - 6;
	if(lOutNeedSize < 1)
		return 0;
	if(lOutNeedSize >= lOutBufSize)
		return 0 - lOutNeedSize - 1;	//返回需要的内存大小

	//‘+’号在传输时有问题，现在把‘-’变回‘+’。
	TCHAR* pFind = (char*)_tcschr(pIn, _T('-'));
	while(pFind)
	{
		*pFind = _T('+');
		pFind = _tcschr(pFind, _T('-'));
	}

	TCHAR* szTemp = new TCHAR[_tcslen(pIn)];
	memset(szTemp, 0, _tcslen(pIn));

	//BASE64
	LONG lBase64After = Base64Decode(pIn, (BYTE*)szTemp);
	if(lBase64After > 7)	//6字节的随机数，1字节的校验和
	{
		//解密
		BYTE bytCheck = 0;
		int nRandomIndex = 0;
		for(int i = 6; i < lBase64After; i++)
		{
			szTemp[i] ^= g_szConvertBookD[(BYTE)szTemp[nRandomIndex]];
			nRandomIndex ++;
			if(nRandomIndex > 5)
				nRandomIndex = 0;

			//计算校验和
			if(i < lBase64After - 1)
				bytCheck += szTemp[i];
		}

		//判校验和
		if(bytCheck != (BYTE)szTemp[lBase64After - 1])
			lBase64After = 7;

		memcpy(pOut, szTemp + 6, lBase64After - 7);
	}
	delete[] szTemp;

	return lBase64After - 7;
}

