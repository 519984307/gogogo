// RavBase64.cpp: implementation of the CRavBase64 class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RavBase64.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef	ERROR_WRONGCHARCODE
#define ERROR_WRONGCHARCODE	-1
#endif

#ifndef ERROR_EXCEPTION
#define ERROR_EXCEPTION		-2
#endif

CRavBase64::CRavBase64()
{
	InitialBaseTable();
}

CRavBase64::~CRavBase64()
{

}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：void InitialBaseTable()
//	实现功能：初始化Base64对应表
//	对全局变量的影响：无
//	参数说明：无
//	返回结果说明：
////////////////////////////////////////////////////////////////////////////////
void CRavBase64::InitialBaseTable()
{
	int nPos = 0;
	int i = 0;
	for(i = 0;i < 26;i ++)
		chBaseTable[nPos++]  = static_cast<char>(0x41 + i);
	for(i = 0;i < 26;i ++)
		chBaseTable[nPos++]  = static_cast<char>(0x61 + i);
	for(i = 0;i < 10;i ++)
		chBaseTable[nPos ++] = static_cast<char>(0x30 + i);
	chBaseTable[nPos ++] = 0x2b;
	chBaseTable[nPos] = 0x2f;
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：int EncodeBase64(
//				char * chInBuf,
//				char *chOutBuf,
//				int nInLen ) 
//	实现功能：按Base64方式加密
//	对全局变量的影响：无
//	参数说明：
//		chInBuf				入串
//		chOutBuf			出串
//		nInLen				入串长度
//	返回结果说明：返回串长度,-1为原串不正确,-2为系统出错，可能是串的内存分配错误
////////////////////////////////////////////////////////////////////////////////
int CRavBase64::EncodeBase64(char * chInBuf, char *chOutBuf , int nInLen /* = 0 */)
{
	try
	{
		if(!nInLen)
			nInLen = strlen(chInBuf);
		if(nInLen < 1)
		{
			chOutBuf[0] ='\0';
			return 0;
		}
		else
		{
			int nFree = 0;
			int nInPos = 0 , nOutPos = 0;
			int nValue = 0;
			do
			{
				
				if(nFree >= 6)
				{
					chOutBuf[nOutPos ++] = chBaseTable[(nValue >> (nFree - 6)) & 0x3f];
					nFree -= 6;
				}
				else
				{
					if(nInPos < nInLen)
					{
						nFree += 8;
						nValue <<= 8;
						nValue += (unsigned char)chInBuf[nInPos ++];
					}
					else
					{
						if(nFree > 0)
							chOutBuf[nOutPos ++] = chBaseTable[(nValue << (6 - nFree))&0x3f];
						break;
					}
				}
			}while(true);
			chOutBuf[nOutPos] = '\0';
			return nOutPos;
		}
	}
	catch(...)
	{
		return ERROR_EXCEPTION;
	}
}

#define DBASE64(a) (a>='A' && a<='Z')?a-'A': \
                   (a>='a'&&a<='z')?(a-'a')+26: \
                   (a>='0'&&a<='9')?(a-'0')+52: \
                   (a=='+')?(62):(a=='/')?(63): -1

////////////////////////////////////////////////////////////////////////////////
//	函数名称：int DecodeBase64(
//				char * chInBuf,
//				char *chOutBuf ) 
//	实现功能：按Base64方式解密
//	对全局变量的影响：无
//	参数说明：
//		chInBuf				输入串
//		chOutBuf			输出串
//	返回结果说明：返回串长度,-1为原串不正确,-2为系统出错，可能是串的内存分配错误
////////////////////////////////////////////////////////////////////////////////
int CRavBase64::DecodeBase64(char * chInBuf, char *chOutBuf)
{
	try
	{	
		int nInLen = strlen(chInBuf);
		if(nInLen < 1)
		{
			chOutBuf[0] = '\0';
			return 0;
		}
		else
		{
			int nValue = 0;
			int nFree = 0;
			int nInPos = 0 , nOutPos = 0;
			do
			{
				if(nFree >= 8)
				{
					chOutBuf[nOutPos ++] = static_cast<char>(nValue >> (nFree - 8));
					nFree -= 8;
				}
				else
				{
					if(nInPos < nInLen)
					{
						nValue <<= 6;
						int nTempValue = DBASE64((unsigned int)chInBuf[nInPos]);
						if(nTempValue < 0)
							return ERROR_WRONGCHARCODE;
						else
						{
							nValue |= nTempValue & 0x3f;
							nFree += 6;
							nInPos ++;
						}
					}
					else
						break;
				}
			}while(true);
			chOutBuf[nOutPos] = '\0';
			return nOutPos;
		}
	}
	catch(...)
	{
		return ERROR_EXCEPTION;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：int EncodeLen( int nLen ) 
//	实现功能：计算加密后的串长度(不包括结束符)
//	对全局变量的影响：无
//	参数说明：
//		nLen				原串长
//	返回结果说明：返回串长
////////////////////////////////////////////////////////////////////////////////
int CRavBase64::EncodeLen(int nLen)
{
	return (nLen / 3) * 4 + ((nLen % 3) ? (nLen % 3 + 1) : 0);
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：int DecodeLen( int nLen ) 
//	实现功能：计算解密后的串长度(不包括结束符)
//	对全局变量的影响：无
//	参数说明：
//		nLen				原串长
//	返回结果说明：返回串长
////////////////////////////////////////////////////////////////////////////////
int CRavBase64::DecodeLen(int nLen)
{
	return (nLen / 4) * 3 + ((nLen % 4) ? (nLen % 4 - 1) : 0);
}
