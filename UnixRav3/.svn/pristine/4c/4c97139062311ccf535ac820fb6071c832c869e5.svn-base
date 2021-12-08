#pragma once
/*
CRisingSN含义是：翻译的瑞星信息中心实现的算法，原版算法为C#实现
				 作用是将license.dat中求出的sn转换为真正的p12文件的口令
*/

class CRisingSN
{
public:
	CRisingSN(void);
	~CRisingSN(void);
	BOOL Descrypt(CStringA strEncryptSN, CStringA& strDescryptSN);

	bool VaildMainSignByCrc(CStringA signString);   //验证基本号
	bool VaildAddSignByCrc(CStringA signString);	//验证扩容号

	void SetMd5Mode(BOOL bVer2);
private:
	//将32位整数转换为二级制字符串（5个字符）
	CStringA ConverInt32To5Binary(int nIndex);
	//将字节转化为二进制字符串（8个字符）
	CStringA ConverByte2Binary(BYTE _byte);
	//二级制字符串转化为字节
	BYTE Binary2Byte(CStringA str);
	//二进制字符串转化为字节流（大量）
	BOOL FromBinaryToBytes(CStringA Binarys, BYTE** ppBytes, int& nBytesLen);
	//字节流转化为二进制字符串(大量)
	CStringA FromBytesToBinary(BYTE* pBytes, int nBytesLen);


	CStringA RC4Descrypt(CStringA strSrc, CStringA default_key);
	CStringA RSADescrypt(CStringA strSrc, CStringA n, CStringA e);
	BOOL m_bVer2;
};
