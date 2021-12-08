#pragma once

#ifndef WIN32
#define __int64 long long
#endif

typedef __int64  _long;
typedef unsigned __int64 _ulong;
#include <stdio.h>
/*
RsNetRsa含义是：翻译的瑞星信息中心实现的rsa算法，原版算法为C#实现
				Rs表示瑞星
                Net表示信息中心
				Rsa算法
*/

class CRsNetRsa
{
	enum
	{
		maxLength = 70,
	};
public:
	CRsNetRsa(void);
	~CRsNetRsa(void);
	CRsNetRsa(CStringA value, int radix);
	CRsNetRsa(_long value);
	CRsNetRsa(BYTE* inData, int inDataLen);
	CRsNetRsa(UINT* inData, int inDataLen);

	CRsNetRsa operator*(CRsNetRsa bi2);
	CRsNetRsa operator+(CRsNetRsa bi2);
	CRsNetRsa operator%(CRsNetRsa bi2);
	CRsNetRsa operator/(CRsNetRsa bi2);
	CRsNetRsa& operator-();
	bool operator<(CRsNetRsa bi2);
	bool operator>(CRsNetRsa bi2);
	CRsNetRsa operator<<(int shiftVal);
	CRsNetRsa operator-(CRsNetRsa bi2);
	bool operator>=(CRsNetRsa bi2);
	bool operator==(CRsNetRsa bi2);
	
	CRsNetRsa modPow(CRsNetRsa exp, CRsNetRsa n);
	CStringA  ToHexString();

	int bitCount();
	bool Equals(CRsNetRsa o);

	UINT data[maxLength];
	int dataLength;
private:
	void singleByteDivide(CRsNetRsa bi1, CRsNetRsa bi2,	CRsNetRsa& outQuotient, CRsNetRsa& outRemainder);
	void multiByteDivide(CRsNetRsa bi1, CRsNetRsa bi2,	CRsNetRsa& outQuotient, CRsNetRsa& outRemainder);
	int  shiftRight(UINT* buffer, int nBufferLen, int shiftVal);
	int  shiftLeft (UINT* buffer, int nBufferLen, int shiftVal);
	CRsNetRsa BarrettReduction(CRsNetRsa x, CRsNetRsa n, CRsNetRsa constant);

	
};

const CStringA rsa14PriKeyN = "A4C95658EFC8C382681F68973C1645";
const CStringA rsa14PriKeyE = "3A107DF8F9DF8BC91EA1892A63A0D5";

const CStringA rsa11PriKeyN = "568892F7D2A1952D13754179";
const CStringA rsa11PriKeyE = "104D9C66E80B8635DB5F7FE7";

class CRsNetRsaHelp
{
public:
	/// <summary>
	/// 用公钥解密
	/// </summary>
	/// <param name="bytes">加密后的字节数组</param>
	/// <param name="n">公钥中的n</param>
	/// <param name="e">公钥中的d</param>
	/// <returns>解密后的字节数组</returns>
	BOOL DecryptByPublicKey(BYTE* bytes, int nBytesLen, CStringA n, CStringA e, BYTE* out_bytes, int& out_byteslen);
};
