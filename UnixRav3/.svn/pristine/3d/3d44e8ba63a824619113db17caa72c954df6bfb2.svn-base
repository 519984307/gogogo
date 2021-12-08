#pragma once
/*
   终端授权文件中的Aes算法，具体实现模型为cbc，这里对其进行封装，方便使用
*/

class CAesCbc
{
public:
	CAesCbc();
	~CAesCbc(void);

	//BOOL SetBaseNumber(LPCWSTR lpszBaseNumber);
	BOOL SetBaseNumber(LPCSTR lpszBaseNumber);
	BOOL SetBinKey(std::string strKey);
	std::string Descrypt(BYTE* pVoid, int nBytes);
	BOOL SetMd5Mode(BOOL bVer2);
private:
	BOOL    ConvertSN2Bytes(std::string & strSN, BYTE** ppVoid, int& nByteSize);
	std::string ConverInt32ToBinary(int nIndex);
	BYTE	Binary2Byte(std::string str);
	int     Byte2Int(BYTE byte);
	BOOL	BinaryToBytes(std::string & strBinary, BYTE** ppBuf, int& nBytes);
	BOOL    m_bVer2;

	BYTE	m_key[16];
	BYTE    m_kb[32];
};


extern BOOL GetMD5(BYTE* pSrcData, int nSrcSize, BYTE* pDstData, int nDstData, BOOL bVer2);
