#include "StdAfx.h"
#include "AesCbc.h"
#include "openssl/aes.h"
#include "openssl/rc4.h"
#include "openssl/rsa.h"
#include "md5_c.h"

CAesCbc::CAesCbc():m_bVer2(FALSE)
{
	BYTE _key[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF };
	memcpy(m_key, _key, 16);
}

CAesCbc::~CAesCbc(void)
{
}

/*
BOOL CAesCbc::SetBaseNumber(LPCWSTR lpszBaseNumber)
{
	CStringA strBaseA(lpszBaseNumber);	
	return SetBaseNumber(strBaseA);
}
*/

BOOL   CAesCbc::ConvertSN2Bytes(std::string & strSN, BYTE** ppVoid, int& nByteSize)
{
	//strSN.Remove(_T('-'));
	remove(strSN, '-');
#ifdef _DEBUG
	int nSN = strSN.size();
#endif
	toupper_str(strSN);
	std::string strHaskTable = "23456789ABCDEFGHJKMNLPQRSTUVWXYZ";
	std::string strBinary;
	int i = 0;
	for(; i < strSN.size(); i++)
	{
		int nIndex = strHaskTable.find(strSN.at(i));
		strBinary += ConverInt32ToBinary(nIndex);
	}
	strBinary += "000";
#ifdef _DEBUG
	int nLen = strBinary.size();
#endif
	if(!BinaryToBytes(strBinary, ppVoid, nByteSize))
		return FALSE;
	return TRUE;
}

BOOL CAesCbc::SetBaseNumber(LPCSTR lpszBaseNumber)
{
	BYTE* pBytes = NULL;
	int   nBytes = 0;
	std::string strBaseNumber = lpszBaseNumber;
	if(!ConvertSN2Bytes(strBaseNumber, &pBytes, nBytes))
	  return FALSE;
	//VI
	int viCount = Byte2Int(pBytes[2]) % 8 + 1;
	memcpy(m_key, pBytes, 16);
	for(int i = 0; i < viCount; i++)
	{
		if(FAILED(GetMD5(m_key, 16, m_key, 16, m_bVer2)))
		{
			delete pBytes;
			return FALSE;
		}
	}

	//Key
	int k1Count = Byte2Int(pBytes[4]) % 8 + 2;
	BYTE k1b[16] = {0};
	//k1b = InitBytes;
	memcpy(k1b, pBytes, 16);
	for(int i = 0; i < k1Count; i++)
	{
		if(FAILED(GetMD5(k1b, 16, k1b, 16, m_bVer2)))
		{
			delete pBytes;
			return FALSE;
		}
	}
	int k2Count = Byte2Int(pBytes[6]) % 8 + 3;
	BYTE k2b[16] = {0};
	memcpy(k2b, pBytes, 16);
	for(int i = 0; i < k2Count; i++)
	{
		if(FAILED(GetMD5(k2b, 16, k2b, 16, m_bVer2)))
		{
			delete pBytes;
			return FALSE;
		}
	}
	delete pBytes;
	pBytes = NULL;
	int i = 0;
	for(int j = 0; j<16 ;j++)
	{
		m_kb[i++] = k1b[j];
	}
	for(int j = 0; j<16 ;j++)
	{
		m_kb[i++] = k2b[j];
	}
	return TRUE;
}

std::string CAesCbc::Descrypt(BYTE* pVoid, int nBytes)
{
	if(!pVoid)return "";
	BYTE* pResult = new BYTE[nBytes+1];
	memset(pResult, 0, sizeof(BYTE)*(nBytes+1));
	//½âÃÜ
	AES_KEY key;
	AES_set_decrypt_key(m_kb, 32*8, &key);
	AES_cbc_encrypt(pVoid, (unsigned char *)pResult, nBytes, &key, m_key, AES_DECRYPT);

	pResult[nBytes] = 0;
	std::string result = (char*)pResult;
	delete[] pResult;
	return result;
}

std::string CAesCbc::ConverInt32ToBinary(int nIndex)
{
	std::string strBin;
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

BYTE CAesCbc::Binary2Byte(std::string str)
{
	int nLen = str.size();
	BYTE byte = 0;
	for(int i = 0 ;i < nLen ; i++)
	{		
		byte = byte << 1;
		char c = str.at(i);
		if( c == '1')
			byte = byte | 0x01;
		else
			byte = byte | 0x00;
	}	
	return byte;
}

BOOL CAesCbc::BinaryToBytes(std::string & strBinary, BYTE** ppBuf, int& nBytes)
{
	if(!ppBuf)
		return FALSE;
	int nLen = strBinary.size()/8;
	*ppBuf = new BYTE[nLen];
	nBytes = nLen;
	for(int i = 0; i < nLen; i++)
	{
		(*ppBuf)[nLen - 1 - i] = Binary2Byte(strBinary.substr(i*8, 8));
	}
	return TRUE;
}

BOOL CAesCbc::SetMd5Mode(BOOL bVer2)
{
	m_bVer2 = bVer2;
	return TRUE;
}

int CAesCbc::Byte2Int(BYTE byte)
{
	int nByte = byte;
	nByte &= 0x000000ff;
	return nByte;
}

BOOL CAesCbc::SetBinKey(std::string strKey)
{
	if(strKey.size() != 32)
		return FALSE;

	for(int i = 0; i < 32; i++)
		m_kb[i] = strKey.at(i);
	return TRUE;
}

BOOL GetMD5(BYTE* pSrcData, int nSrcSize, BYTE* pDstData, int nDstData, BOOL bVer2)
{
	if(bVer2)
	{
		md5_byte_t buf[16];
		md5_state_s mst;
		md5_init(&mst);
		md5_append(&mst, (const md5_byte_t*)pSrcData, nSrcSize);
		md5_finish(&mst, buf);
		memcpy(pDstData, buf, 16);
	}else
	{
		return FALSE;//linux for soho ,so cancel ver1
		/*
		int wlen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pSrcData, nSrcSize, NULL, 0);
		if (wlen <= 0)
			return FALSE;
		wchar_t*  wbuff = NULL;
		if ((wbuff = new wchar_t[wlen + 1]) == NULL)
			return FALSE;
		int nRes = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pSrcData, nSrcSize, wbuff, wlen + 1);

		int ulen = ::WideCharToMultiByte(CP_UTF8, 0, wbuff, wlen, NULL, 0, NULL, NULL);
		if (ulen <= 0)
			return FALSE;
		char* ubuff = NULL;
		if ((ubuff = new  char[ulen + 1]) == NULL)
			return FALSE;
		::WideCharToMultiByte(CP_UTF8, 0, wbuff, wlen, ubuff, ulen + 1, NULL, NULL);


		md5_byte_t buf[16];
		md5_state_s mst;
		md5_init(&mst);
		md5_append(&mst, (const md5_byte_t*)ubuff, ulen);
		md5_finish(&mst, buf);
		memcpy(pDstData, buf, 16);
		*/
	}
	return TRUE;
}
