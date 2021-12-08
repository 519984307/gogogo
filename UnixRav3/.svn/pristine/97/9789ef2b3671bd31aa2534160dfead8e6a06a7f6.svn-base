#include "StdAfx.h"
#include "RisingSN.h"
#include "RsNetRsa.h"
#include "RsNetCRC16.h"
// #include <util/utility.h>
#include "AesCbc.h"

#include "openssl/rc4.h"
#include "openssl/rsa.h"
// #pragma comment(lib, "ssleay32.lib")
// #pragma comment(lib, "libeay32.lib")

char rc4_default_key1[] = "QAZDCXSWERTGFVBHNJKLUYTRFCDERTY";
char rc4_default_key2[] = "QSDERFVBGFDILKOJHUKMNHHTYUJDSHR";


CRisingSN::CRisingSN(void):m_bVer2(FALSE)
{
}

CRisingSN::~CRisingSN(void)
{
}

BOOL CRisingSN::Descrypt(CStringA strEncryptSN, CStringA& strDescryptSN)
{
	if(strEncryptSN.GetLength() != 29 && strEncryptSN.GetLength() != 23)
		return false;
	strEncryptSN.Remove('-');
	int nEncryptSnLen = strEncryptSN.GetLength();
	if(strEncryptSN.GetLength() == 25)
	{
		CStringA strHaskTable = "JTCE7DYB5GHKF8N3PRZUA4V2L9MXS6WQ";
		CStringA strBinary;
		int i = 0;
		for(; i < strEncryptSN.GetLength(); i++)
		{
			int nIndex = strHaskTable.Find(strEncryptSN.GetAt(i));
			strBinary.mString += ConverInt32To5Binary(nIndex).mString;
		}

		int nBinLen = strBinary.GetLength();//125?
		//RC4 Right
		CStringA rightbin = strBinary.Right(8*8);
		rightbin = RC4Descrypt(rightbin, rc4_default_key2);
		strBinary = strBinary.Left(strBinary.GetLength() - (8*8));
		strBinary += rightbin;
		//RC4 Left
		CStringA leftbin = strBinary.Left(8*8);
		leftbin = RC4Descrypt(leftbin, rc4_default_key1);
		strBinary = strBinary.Right(strBinary.GetLength() - (8*8));
		strBinary.mString.insert(0, leftbin.mString);
		//strBinary = leftbin + strBinary;
		//RSA
		CStringA left15bin = strBinary.Left(15*8);
		CStringA left14bin = RSADescrypt(left15bin, rsa14PriKeyN, rsa14PriKeyE);
		CStringA strTemp = left14bin;
		strTemp += strBinary.Mid(15*8);
		strBinary = strTemp;
		//验证签名
		if(VaildMainSignByCrc(strBinary))
		{
			CStringA strPwdBinary = strBinary;
			strPwdBinary += "000";
			BYTE* pBytes = NULL;
			int nBytesLen = 0;
			FromBinaryToBytes(strPwdBinary, &pBytes, nBytesLen);
			BYTE md5[16];
			GetMD5(pBytes, nBytesLen, md5, 16, m_bVer2);
			CStringA strPWD;
			for(int i = 0 ;i < 16; i++)
			{
				char temp[4] = {0};
				sprintf(temp, "%02X", md5[i]);
				strPWD += temp;
			}
			strDescryptSN = strPWD.Left(30);	
			return TRUE;
		}	
	}else
	{
		//20
		strEncryptSN.Remove('-');
		int nEncryptSnLen = strEncryptSN.GetLength();

		CStringA strHaskTable = "JTCE7DYB5GHKF8N3PRZUA4V2L9MXS6WQ";
		CStringA strBinary;
		int i = 0;
		for(; i < strEncryptSN.GetLength(); i++)
		{
			int nIndex = strHaskTable.Find(strEncryptSN.GetAt(i));
			strBinary += ConverInt32To5Binary(nIndex);
		}

		int nBinLen = strBinary.GetLength();
		strBinary = strBinary.Left(96);
		nBinLen = strBinary.GetLength();

		//RC4 Right
		CStringA rightbin = strBinary.Right(8*8);
		rightbin = RC4Descrypt(rightbin, rc4_default_key2);
		strBinary = strBinary.Left(strBinary.GetLength() - (8*8));
		strBinary += rightbin;
		//RC4 Left
		CStringA leftbin = strBinary.Left(8*8);
		leftbin = RC4Descrypt(leftbin, rc4_default_key1);
		strBinary = strBinary.Right(strBinary.GetLength() - (8*8));
		strBinary = leftbin + strBinary;
		//RSA
#ifdef _DEBUG
		int nLen = strBinary.GetLength();
#endif
		CStringA left15bin = strBinary;
		CStringA left14bin = RSADescrypt(left15bin, rsa11PriKeyN, rsa11PriKeyE);
		if(strBinary.GetLength() > 15*8)
			strBinary = left14bin + strBinary.Mid(15*8);
		else
			strBinary = left14bin;
		//验证签名
		if(VaildAddSignByCrc(strBinary))
		{
			CStringA strPwdBinary = strBinary + "000";
			BYTE* pBytes = NULL;
			int nBytesLen = 0;
			FromBinaryToBytes(strPwdBinary, &pBytes, nBytesLen);
			BYTE md5[16];
			GetMD5(pBytes, nBytesLen, md5, 16, m_bVer2);
			CStringA strPWD;
			for(int i = 0 ;i < 16; i++)
			{
				char temp[4] = {0};
				sprintf(temp, "%02X", md5[i]);
				strPWD += temp;
// 				CStringA str;
// 				str.Format("%02X", md5[i]);
// 				strPWD += str;
			}
			strDescryptSN = strPWD.Left(30);	
			return TRUE;
		}	
	}
	
	return FALSE;
}

CStringA CRisingSN::RC4Descrypt(CStringA strSrc, CStringA default_key)
{

	int nLen;
	BYTE* pBuf = NULL;
	RASSERT(FromBinaryToBytes(strSrc, &pBuf, nLen), strSrc);//错误了 就随意返回一个
	BYTE* pTemp = new BYTE[nLen];

	RC4_KEY rc4_key;
	RC4_set_key(&rc4_key, default_key.GetLength(), (const unsigned char*)default_key.GetBuffer());
	RC4(&rc4_key, nLen, pBuf, pTemp);
	
	CStringA strBinary = FromBytesToBinary(pTemp, nLen);
// 	CStringA strBinary;
// 	for(int i = 0; i < nLen; i++)
// 	{
// 		strBinary = ConverByte2Binary(pTemp[i]) + strBinary;
// 	}	
	delete[] pTemp;
	delete[] pBuf;
	return strBinary;
}	

CStringA CRisingSN::ConverByte2Binary(BYTE _byte)
{
	CStringA strBin;
	if(_byte & 0x00000080)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000040)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000020)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000010)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000008)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000004)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000002)
		strBin += "1";
	else
		strBin += "0";
	if(_byte & 0x00000001)
		strBin += "1";
	else
		strBin += "0";
	return strBin;
}

CStringA CRisingSN::ConverInt32To5Binary(int nIndex)
{
	CStringA strBin;
	if(nIndex & 0x00000010)
		strBin += "1";
	else
		strBin += "0";
	if(nIndex & 0x00000008)
		strBin += "1";
	else
		strBin += "0";
	if(nIndex & 0x00000004)
		strBin += "1";
	else
		strBin += "0";
	if(nIndex & 0x00000002)
		strBin += "1";
	else
		strBin += "0";
	if(nIndex & 0x00000001)
		strBin += "1";
	else
		strBin += "0";
	return strBin;
}

BYTE CRisingSN::Binary2Byte(CStringA str)
{
	int nLen = str.GetLength();
	BYTE byte = 0;
	for(int i = 0 ;i < nLen ; i++)
	{		
		byte = byte << 1;
		char c = str.GetAt(i);
		if( c == '1')
			byte = byte | 0x01;
		else
			byte = byte | 0x00;
	}	
	return byte;
}

CStringA CRisingSN::RSADescrypt(CStringA strSrc, CStringA n, CStringA e)
{
	int nSrcLen = 0;
	BYTE* pSrc  = NULL;
	//二进制字符串 -> 字节流
	RASSERT(FromBinaryToBytes(strSrc, &pSrc, nSrcLen), strSrc);//错误了 就随意返回一个
	RASSERT(pSrc, strSrc);//错误了 就随意返回一个
	
	//RSA解密
	BYTE* dst = new BYTE[nSrcLen] ;
	memset(dst, 0, nSrcLen);
	int nDstLen = nSrcLen;
	CRsNetRsaHelp rsa;
	if(!rsa.DecryptByPublicKey(pSrc, nSrcLen, n, e, dst, nDstLen))
	{
		delete[] dst;
		delete[] pSrc;
		return strSrc;//错误了 就随意返回一个
	}
	delete[] pSrc;
	pSrc = NULL;
	
	CStringA strResult =  FromBytesToBinary(dst, nDstLen);
	delete[] dst;
	dst = NULL;
	return strResult;
}

BOOL CRisingSN::FromBinaryToBytes(CStringA Binarys, BYTE** ppBytes, int& nBytesLen)
{
	RASSERT(ppBytes, FALSE);
	int length = Binarys.GetLength() / 8;
	*ppBytes = new BYTE[length];
	for (int i = 0; i < length; i++)
	{
		(*ppBytes)[length - 1 - i] = Binary2Byte(Binarys.Mid(i * 8, 8));
	}
	nBytesLen = length;
	return TRUE;
}

CStringA CRisingSN::FromBytesToBinary(BYTE* pBytes, int nBytesLen)
{
	CStringA strBinary;
	for(int i = 0; i < nBytesLen; i++)
	{
		strBinary.mString.insert(0, ConverByte2Binary(pBytes[i]).mString);
		//CStringA
		//strBinary = ConverByte2Binary(pBytes[i]) + strBinary;
	}	
	return strBinary;
}

bool CRisingSN::VaildMainSignByCrc(CStringA signString)
{
	bool result = true;
	CStringA signBinary ;
	CStringA signStr = "000" + signString.Mid(0, 101);
	
	CRsNetCRC16 crc;
	unsigned short u = crc.CalculateCrc16(signStr);
	signBinary = FromBytesToBinary((BYTE*)&u, sizeof(unsigned short));
	if(signBinary != signString.Mid(101))
		return false;
	return true;
}

bool CRisingSN::VaildAddSignByCrc(CStringA signString)
{
	CStringA signStr = signString.Mid(0,72);
	CRsNetCRC16 crc;
	unsigned short u = crc.CalculateCrc16(signStr);
	CStringA signBinary  = FromBytesToBinary((BYTE*)&u, sizeof(unsigned short));
	if(signBinary != signString.Mid(72))
		return false;
	return true;
}

void CRisingSN::SetMd5Mode(BOOL bVer2)
{
	m_bVer2 = bVer2;
}