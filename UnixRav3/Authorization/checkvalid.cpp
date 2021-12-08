#ifdef UNIXPLATFORM
    #include <time.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    #define _MAX_PATH 512
    #define lstrcmpA strcmp
#else
    #include "stdafx.h"
    #include "time.h"
#endif

#include "checkvalid.h"
#include "ravbase64.h"
#include "cfgdef2.h"
#include "extern.h"
#include "checksn.h"

//产品ID和GUID的对照表
STPRODUCTGUID   g_aGUIDTable[PRODUCT_TOTAL_NUM] =
{
        {0x10100000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机千禧版
        {0x10200000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机版2001
        {0x10300000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机版2002
        {0x10400000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机版2002增强
        {0x10500000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机专用版
        {0x10600000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机2002服务器版
        {0x10700000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机版2003
        {0x10900000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机版2004
        {0x10800000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//rfw
        {0x10a00000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//单机版2005
        {0x20100000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//网络版多中心版
        {0x20200000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//网络版单中心版
        {0x20300000, "{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}"},		//网络版超级中心版
        {0x20400000, "{B7B4C60A-E930-48c1-8EF5-C710BF326BE7}"},		//Exchange
        {0x20500000, "{5E655840-114E-4bbb-A252-3A139E7E6DFD}"},		//Domino
        {0x20600000, "{5E655840-114E-4bbb-A252-3A139E7E6DFD}"},		//邮件服务器
        {0x20700000, "{5E655840-114E-4bbb-A252-3A139E7E6DFD}"},		//Unix
        {0x20800000, "{5E655840-114E-4bbb-A252-3A139E7E6DFD}"},		//Novell
        {0x20900000, "{5E655840-114E-4bbb-A252-3A139E7E6DFD}"},		//插件
        {0x20b00000, "{5E655840-114E-4bbb-A252-3A139E7E6DFD}"}		//Unix-KDE
};

CCheckValid::CCheckValid()
{
        memset(m_szProductGUID,0,40);
        m_lProductID = 0;
}

CCheckValid::~CCheckValid()
{
}

BOOL CCheckValid::SetGUID(LONG lProductID)
{
        BOOL bRet = FALSE;
        if(lProductID==0)
                return bRet;
        for(int i=0; i<PRODUCT_TOTAL_NUM; i++)
        {
                //if(g_aGUIDTable[i].dwProductID ==(DWORD)(lProductID & 0xFFFF0000))
                if(g_aGUIDTable[i].dwProductID ==(DWORD)(lProductID & 0xFFF00000))
                {
                        memcpy(m_szProductGUID, g_aGUIDTable[i].szProductGUID, 40);
                        m_lProductID = lProductID;
                        bRet = TRUE;
                        break;
                }
        }
        return bRet;
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：bool SetInstallTime(
//				const char* czSn,
//				const char* czInstallTime )
//	实现功能：设置指定序列号的安装时间。
//	对全局变量的影响：无
//	参数说明：
//		czSn				- 输入，指定的序列号
//		czInstallTime		- 输入，安装时间（十进制的字符串）
//	返回结果说明：成功true,失败false
////////////////////////////////////////////////////////////////////////////////
bool CCheckValid::SetInstallTime( const char* czSn, const char* czInstallTime )
{
#ifdef UNIXPLATFORM
        STSNINFO_UNIX stInfo = {0};
        long lOffset = SearchSn(czSn, &stInfo);
        if(lOffset < 0)
                strcpy(stInfo.szSN, czSn);
        strcpy(stInfo.szInstallTime, czInstallTime);
        if(0 != WriteSn(lOffset, &stInfo, false))
                return 0;
        return 1;
#else

        char	szKey[_MAX_PATH] = {0}, *pszKeyWorkBuf = szKey;
        char	szEncodeBuf[_MAX_PATH] = {0};
        char*	lpszEncWorkBuf = szEncodeBuf;

        strcpy(szKey, "CLSID\\");
        strcat(szKey, m_szProductGUID);
        strcat(szKey, "\\");

        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(czSn));
        if (nLen >= _MAX_PATH)
        {
                lpszEncWorkBuf = new char[nLen+1];
                if (!lpszEncWorkBuf)	return false;
        }

        base.EncodeBase64((char*)czSn, lpszEncWorkBuf);

        //判断_MAX_PATH的主键缓冲区是否够用，不够则重新分配
        int nEncLen = strlen(pszKeyWorkBuf) + strlen(lpszEncWorkBuf);
        if ( nEncLen < _MAX_PATH)
                strcat(pszKeyWorkBuf, lpszEncWorkBuf);
        else
        {
                pszKeyWorkBuf = new char[nEncLen + 1];
                if (!pszKeyWorkBuf)
                {
                        if (nLen >= _MAX_PATH)
                                delete [] lpszEncWorkBuf;

                        return false;
                }

                strcpy(pszKeyWorkBuf, szKey);
                strcat(pszKeyWorkBuf, lpszEncWorkBuf);
        }

        if (nLen >= _MAX_PATH)
                delete [] lpszEncWorkBuf;

        int i = 0;
        for (i = 0; i < WRITE_FAILED_RETRY_TIMES; i++)
        {
                if (REGWriteString (HKEY_CLASSES_ROOT, pszKeyWorkBuf, SETUP_TIME, czInstallTime))
                        break;

                Sleep(100);
        }

        if (nEncLen >= _MAX_PATH)
                delete [] pszKeyWorkBuf;

        return i < WRITE_FAILED_RETRY_TIMES ? true : false;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：bool SetLimitType(
//				const char* czSn,
//				const char* czLimitType )
//	实现功能：设置指定序列号的限时类型。
//	对全局变量的影响：无
//	参数说明：
//		czSn			- 输入，指定的序列号
//		czLimitType		- 输入，限时类型（十进制的字符串）
//						0 : 不限时
//						1 : 限时一个月
//						2 : 限时三个月
//						3 : 限时一年
//	返回结果说明：成功true,失败false
////////////////////////////////////////////////////////////////////////////////
bool CCheckValid::SetLimitType( const char* czSn, const char* czLimitType )
{
#ifdef UNIXPLATFORM
        return true;
#else
        char szKeyBuf[_MAX_PATH] = {0}, *pszKeyWorkBuf = szKeyBuf;
        char szEncodeBuf[_MAX_PATH] = {0}, *pszEncWorkBuf = szEncodeBuf;

        strcpy(szKeyBuf, "CLSID\\");
        strcat(szKeyBuf, m_szProductGUID);
        strcat(szKeyBuf, "\\");

        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(czSn));
        if (nLen >= _MAX_PATH)
        {
                pszEncWorkBuf = new char[nLen+1];
                if (!pszEncWorkBuf)	return false;
        }

        base.EncodeBase64((char*)czSn, pszEncWorkBuf);
        int nKeyLen = strlen(pszKeyWorkBuf) + strlen(pszEncWorkBuf);
        if ( nKeyLen < _MAX_PATH)
                strcat(pszKeyWorkBuf, pszEncWorkBuf);
        else
        {
                pszKeyWorkBuf = new char[nKeyLen + 1];
                if (!pszKeyWorkBuf)
                {
                        if (nLen >= _MAX_PATH)
                                delete [] pszEncWorkBuf;

                        return false;
                }

                strcpy(pszKeyWorkBuf, szKeyBuf);
                strcat(pszKeyWorkBuf, pszEncWorkBuf);
        }

        if (nLen >= _MAX_PATH)
                delete []pszEncWorkBuf;

        int i = 0;
        for ( i = 0; i < WRITE_FAILED_RETRY_TIMES; i++)
        {
                if (REGWriteString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, SETUP_TYPE, czLimitType))
                        break;

                Sleep(100);
        }

        if (nKeyLen >= _MAX_PATH)
                delete [] pszKeyWorkBuf;

        return i < WRITE_FAILED_RETRY_TIMES ? true : false;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：bool SetEndTime(
//				const char* czSn,
//				const char* czEndTime )
//	实现功能：设置指定序列号的截止时间。
//	对全局变量的影响：无
//	参数说明：
//		czSn				- 输入，指定的序列号
//		czEndTime			- 输入，截止时间（十进制的字符串）
//	返回结果说明：成功true,失败false
////////////////////////////////////////////////////////////////////////////////
bool CCheckValid::SetEndTime( const char* czSn, const char* czEndTime )
{
#ifdef UNIXPLATFORM
        STSNINFO_UNIX stInfo = {0};
        long lOffset = SearchSn(czSn, &stInfo);
        if(lOffset < 0)
                strcpy(stInfo.szSN, czSn);
        strcpy(stInfo.szEndTime, czEndTime);
        if(0 != WriteSn(lOffset, &stInfo, false))
                return 0;
        return 1;
#else

        char szKeyBuf[_MAX_PATH] = {0}, *pszKeyWorkBuf = szKeyBuf;
        char szEncBuf[_MAX_PATH] = {0}, *pszEncWorkBuf = szEncBuf;

        strcpy(szKeyBuf, "CLSID\\");
        strcat(szKeyBuf, m_szProductGUID);
        strcat(szKeyBuf, "\\");

        CRavBase64 base;
        int nEncLen = base.EncodeLen(strlen(czSn));
        if (nEncLen >= _MAX_PATH)
        {
                pszEncWorkBuf = new char[nEncLen+1];
                if (!pszEncWorkBuf)	return false;
        }

        base.EncodeBase64((char*)czSn, pszEncWorkBuf);

        int nEncKeyLen = strlen(pszKeyWorkBuf) + strlen(pszEncWorkBuf);
        if (nEncKeyLen < _MAX_PATH)
                strcat(pszKeyWorkBuf, pszEncWorkBuf);
        else
        {
                pszKeyWorkBuf = new char[nEncKeyLen + 1];
                if (!pszKeyWorkBuf)
                {
                        if (nEncLen >= _MAX_PATH)
                                delete [] pszEncWorkBuf;

                        return false;
                }

                strcpy(pszKeyWorkBuf, szKeyBuf);
                strcat(pszKeyWorkBuf, pszEncWorkBuf);
        }

        if (nEncLen >= _MAX_PATH)
                delete []pszEncWorkBuf;

        int i = 0;
        for ( i = 0; i < WRITE_FAILED_RETRY_TIMES; i++)
        {
                if (REGWriteString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, END_TIME, czEndTime))
                        break;

                Sleep(100);
        }

        if (nEncKeyLen >= _MAX_PATH)
                delete [] pszKeyWorkBuf;

        return i < WRITE_FAILED_RETRY_TIMES ? true : false;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：long GetInstallTime(
//				const char* czSn,
//				char* czInstallTime,
//				int nBuffLen )
//	实现功能：获得指定序列号的安装时间
//	对全局变量的影响：无
//	参数说明：
//		czSn				- 输入，指定的序列号
//		czInstallTime		- 输入/输出，安装时间串的首地址
//		nBuffLen			- 输入，czInstallTime的buffer长度
//	返回结果说明：实际需要的长度（不包括结尾的'\0'），
//					-1表示没有在注册表中找到安装时间
////////////////////////////////////////////////////////////////////////////////
long CCheckValid::GetInstallTime( const char* czSn, char* czInstallTime, int nBuffLen )
{
#ifdef UNIXPLATFORM
        STSNINFO_UNIX stInfo = {0};
        if(SearchSn(czSn, &stInfo) < 0)
                return -1;
        long nActualLen = strlen(stInfo.szInstallTime);
        if(nBuffLen < nActualLen)
                return nActualLen;
        strcpy(czInstallTime, stInfo.szInstallTime);
        return 0;
#else

        char	szKeyBuf[_MAX_PATH] = {0}, *pszKeyWorkBuf = szKeyBuf;
        char	szEncBuf[_MAX_PATH] = {0}, *pszEncWorkBuf = szEncBuf;

        strcpy(szKeyBuf, "CLSID\\");
        strcat(szKeyBuf, m_szProductGUID);
        strcat(szKeyBuf, "\\");

        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(czSn));
        if (nLen >= _MAX_PATH)
        {
                pszEncWorkBuf = new char[nLen+1];
                if (!pszEncWorkBuf)	return false;
        }

        base.EncodeBase64((CHAR*)czSn, pszEncWorkBuf);

        int nEncKeyLen = strlen(pszEncWorkBuf) + strlen(pszKeyWorkBuf);
        if (nEncKeyLen < _MAX_PATH)
                strcat(pszKeyWorkBuf, pszEncWorkBuf);
        else
        {
                pszKeyWorkBuf = new char [nEncKeyLen + 1];
                if (!pszKeyWorkBuf)
                {
                        if (nLen >= _MAX_PATH)
                                delete [] pszEncWorkBuf;
                        return false;
                }

                strcpy(pszKeyWorkBuf, szKeyBuf);
                strcat(pszKeyWorkBuf, pszEncWorkBuf);
        }

        if (nLen >= _MAX_PATH)
                delete []pszEncWorkBuf;

        char szDataBuf[_MAX_PATH] = {0};
        char *pszBigDataBuf = szDataBuf;
        int	  nDataLen = _MAX_PATH, nActBufLen = 0;

        int nRetVal = REGReadString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, SETUP_TIME, pszBigDataBuf, nDataLen, nActBufLen) ;
        if (ERROR_SUCCESS != nRetVal)
        {
                if (RAV_CHECKVALID_BUFFER_TOO_SMALL == nRetVal)
                {
                        nDataLen = nActBufLen;
                        pszBigDataBuf = new char[nDataLen];
                        memset(pszBigDataBuf, 0, sizeof(char)*nDataLen);

                        nRetVal = REGReadString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, SETUP_TIME, pszBigDataBuf, nDataLen, nActBufLen) ;
                        if (ERROR_SUCCESS != nRetVal)
                        {
                                if (nDataLen > _MAX_PATH)
                                        delete []pszBigDataBuf;

                                if (nEncKeyLen >= _MAX_PATH)
                                        delete [] pszKeyWorkBuf;

                                return -1;
                        }
                } else
                {
                        if (nEncKeyLen >= _MAX_PATH)
                                delete [] pszKeyWorkBuf;
                        return -1;
                }
        }

        if (nEncKeyLen >= _MAX_PATH)
                delete [] pszKeyWorkBuf;

        if ( nBuffLen < nActBufLen )
        {
                if (nDataLen > _MAX_PATH)
                        delete []pszBigDataBuf;

                return nActBufLen;
        }

        memset(czInstallTime, 0, sizeof(czInstallTime));
        memcpy(czInstallTime, pszBigDataBuf, nActBufLen);

        if (nDataLen > _MAX_PATH)
                delete []pszBigDataBuf;

        return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：long GetLimitType(
//				const char* czSn,
//				char* czLimitType,
//				int nBuffLen )
//	实现功能：查询指定序列号的限时类型
//	0对全局变量的影响：无
//	参数说明：
//		czSn				- 输入，指定的序列号
//		czLimitType			- 输入/输出，限时类型串的首地址
//		nBuffLen			- 输入，czLimitType的buffer长度
//	返回结果说明：实际需要的长度（不包括结尾的'\0'）
//					-1表示没有在注册表中找到限时类型
////////////////////////////////////////////////////////////////////////////////
long CCheckValid::GetLimitType( const char* czSn, char* czLimitType, int nBuffLen )
{
        char szKey[_MAX_PATH] = {0}, *pszKeyWorkBuf = szKey;
        char szDecBuf[_MAX_PATH] = {0}, *pszDecWorkBuf = szDecBuf;

        strcpy(szKey, "CLSID\\");
        strcat(szKey, m_szProductGUID);
        strcat(szKey, "\\");

        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(czSn));
        if (nLen >= _MAX_PATH)
        {
                pszDecWorkBuf = new char[nLen+1];
                if (!pszDecWorkBuf)	return false;
        }

        base.EncodeBase64((char*)czSn, pszDecWorkBuf);

        int nDecKeyLen = strlen(pszKeyWorkBuf) + strlen(pszDecWorkBuf);
        if (nDecKeyLen < _MAX_PATH)
                strcat(pszKeyWorkBuf, pszDecWorkBuf);
        else
        {
                pszKeyWorkBuf = new char[nDecKeyLen + 1];
                if (!pszKeyWorkBuf)
                {
                        if (nLen >= _MAX_PATH)
                                delete [] pszDecWorkBuf;

                        return false;
                }

                strcpy(pszKeyWorkBuf, szKey);
                strcat(pszKeyWorkBuf, pszDecWorkBuf);
        }

        if (nLen >= _MAX_PATH)
                delete []pszDecWorkBuf;

        char szDataBuf[_MAX_PATH] = {0};
        char *pszBigDataBuf = szDataBuf;
        int	  nDataLen = _MAX_PATH, nActBufLen = 0;
        int nRetVal = REGReadString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, SETUP_TYPE, pszBigDataBuf, nDataLen, nActBufLen) ;
        if (ERROR_SUCCESS != nRetVal)
        {
                if (RAV_CHECKVALID_BUFFER_TOO_SMALL == nRetVal)
                {
                        nDataLen = nActBufLen;

                        pszBigDataBuf = new char[nDataLen];
                        memset(pszBigDataBuf, 0, sizeof(char)*nDataLen);

                        nRetVal = REGReadString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, SETUP_TYPE, pszBigDataBuf, nDataLen, nActBufLen) ;
                        if (ERROR_SUCCESS != nRetVal)
                        {
                                if (nDataLen > _MAX_PATH)
                                        delete [] pszBigDataBuf;

                                if (nDecKeyLen >= _MAX_PATH)
                                        delete [] pszKeyWorkBuf;

                                return -1;
                        }
                } else
                {
                        if (nDecKeyLen >= _MAX_PATH)
                                delete [] pszKeyWorkBuf;
                        return -1;
                }
        }

        if (nDecKeyLen >= _MAX_PATH)
                delete [] pszKeyWorkBuf;

        if ( nBuffLen < nActBufLen )
        {
                if (nDataLen > _MAX_PATH)
                        delete []pszBigDataBuf;

                return nActBufLen;
        }

        memset(czLimitType, 0, sizeof(czLimitType));
        memcpy(czLimitType, pszBigDataBuf, nActBufLen);

        if (nDataLen > _MAX_PATH)
                delete []pszBigDataBuf;

        return 0;
}

////////////////////////////////////////////////////////////////////////////////
//	函数名称：long GetEndTime(
//				const char* czSn,
//				char* czEndTime,
//				int nBuffLen )
//	实现功能：查询指定序列号的截止时间
//	对全局变量的影响：无
//	参数说明：
//		czSn				- 输入，指定的序列号
//		czEndTime			- 输入/输出，截止串的首地址
//		nBuffLen			- 输入，czEndTime的buffer长度
//	返回结果说明：实际需要的长度（不包括结尾的'\0'）
//					-1表示没有在注册表中找到截止时间
////////////////////////////////////////////////////////////////////////////////
long CCheckValid::GetEndTime( const char* czSn, char* czEndTime, int nBuffLen )
{
#ifdef UNIXPLATFORM
        STSNINFO_UNIX stInfo = {0};
        if(SearchSn(czSn, &stInfo) < 0)
                return -1;
        long nActualLen = strlen(stInfo.szEndTime);
        if(nBuffLen < nActualLen)
                return nActualLen;
        strcpy(czEndTime, stInfo.szEndTime);
        return 0;
#endif

        char szKey[_MAX_PATH] = {0},	*pszKeyWorkBuf = szKey;
        char szDecBuf[_MAX_PATH] = {0}, *pszDecWorkBuf = szDecBuf;

        strcpy(szKey, "CLSID\\");
        strcat(szKey, m_szProductGUID);
        strcat(szKey, "\\");

        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(czSn));
        if (nLen >= _MAX_PATH)
        {
                pszDecWorkBuf = new char[nLen+1];
                if (!pszDecWorkBuf)	return false;
        }

        base.EncodeBase64((char*)czSn, pszDecWorkBuf);

        int nDecKeyLen = strlen(pszKeyWorkBuf) + strlen(pszDecWorkBuf);
        if (nDecKeyLen < _MAX_PATH)
                strcat(pszKeyWorkBuf, pszDecWorkBuf);
        else
        {
                pszKeyWorkBuf = new char[nDecKeyLen + 1];
                if (!pszKeyWorkBuf)
                {
                        if (nLen >= _MAX_PATH)
                                delete []pszDecWorkBuf;

                        return false;
                }

                strcpy(pszKeyWorkBuf, szKey);
                strcat(pszKeyWorkBuf, pszDecWorkBuf);
        }

        if (nLen >= _MAX_PATH)
                delete []pszDecWorkBuf;

        char szDataBuf[_MAX_PATH] = {0};
        char *pszBigDataBuf = szDataBuf;
        int	  nDataLen = _MAX_PATH, nActBufLen;
        int nRetVal = REGReadString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, END_TIME, pszBigDataBuf, nDataLen, nActBufLen) ;
        if (ERROR_SUCCESS != nRetVal)
        {
                if (RAV_CHECKVALID_BUFFER_TOO_SMALL == nRetVal)
                {
                        nDataLen = nActBufLen;

                        pszBigDataBuf = new char[nDataLen];
                        memset(pszBigDataBuf, 0, sizeof(char)*nDataLen);

                        nRetVal = REGReadString(HKEY_CLASSES_ROOT, pszKeyWorkBuf, END_TIME, pszBigDataBuf, nDataLen, nActBufLen) ;
                        if (ERROR_SUCCESS != nRetVal)
                        {
                                if (nDataLen > _MAX_PATH)
                                        delete []pszBigDataBuf;

                                if (nDecKeyLen >= _MAX_PATH)
                                        delete [] pszKeyWorkBuf;

                                return -1;
                        }
                }
                else
                {
                        if (nDecKeyLen >= _MAX_PATH)
                                delete [] pszKeyWorkBuf;

                        return -1;
                }
        }

        if (nDecKeyLen >= _MAX_PATH)
                delete [] pszKeyWorkBuf;

        if ( nBuffLen < nActBufLen )
        {
                if (nDataLen > _MAX_PATH)
                        delete []pszBigDataBuf;

                return nActBufLen;
        }

        memset(czEndTime, 0, nBuffLen);
        memcpy(czEndTime, pszBigDataBuf, nActBufLen);

        if (nDataLen > _MAX_PATH)
                delete []pszBigDataBuf;

        return 0;
}


////////////////////////////////////////////////////////////////////////////////
//	函数名称：long GetCurDemoStatus( int *pLeftDays )
//	实现功能：查询当前安装序列号的限时状态，
//				函数自己在注册表中找到当前安装的序列号，再根据对应的
//				安装时间，限时类型，截止时间和当前时间进行判断。
//	对全局变量的影响：无
//	参数说明：pLeftDays 返回剩余的天数，小于0表示过期的天数，大于0为剩余的时间
//	返回结果说明：	DEMOSTATUS_ERR = 0, // 读注册表出现错误
//					DEMOSTATUS_NOT = 1, // 不是试用版。
//					DEMOSTATUS_USE = 2, // 在试用期内，而且不在警告期内，可以正常使用。
//					DEMOSTATUS_TIP = 3, // 在最后警告期内，可以使用，但应提示购买。
//					DEMOSTATUS_DIE = 4, // 超过了最后期限，停止使用。
////////////////////////////////////////////////////////////////////////////////
/*
long CCheckValid::GetCurDemoStatus(int *pLeftDays)
{
        char szDataBuf[_MAX_PATH] = {0};
        char *pszBigRegDataBuf = szDataBuf;
        int	 nRegDataLen = _MAX_PATH, nActBufLen = 0;

        //首先获得当前的序列号, 先从产品主键下获得，如没有则从SOFTWARE\\Rising\\产品子键下获得
        int  nRetVal = REGReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Rising\\Rav", CUR_SERIAL, pszBigRegDataBuf, nRegDataLen, nActBufLen) ;
        if (ERROR_SUCCESS != nRetVal)
        {
                if (RAV_CHECKVALID_BUFFER_TOO_SMALL == nRetVal)
                {
                        nRegDataLen = nActBufLen;

                        pszBigRegDataBuf = new char[nRegDataLen];
                        memset(pszBigRegDataBuf, 0, sizeof(char)*nRegDataLen);

                        nRetVal = REGReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Rising\\Rav", CUR_SERIAL, pszBigRegDataBuf, nRegDataLen, nActBufLen) ;
                        if (ERROR_SUCCESS != nRetVal)
                        {
                                if (nRegDataLen > _MAX_PATH)
                                        delete []pszBigRegDataBuf;
                                return DEMOSTATUS_ERR;
                        }
                }
                else
                        return DEMOSTATUS_ERR;
        }

        CRavBase64 base;
        char   szDecodeBuf[_MAX_PATH] = {0};
        char   *pszDecodeBigBuf = szDecodeBuf;

        int nDecodeLen = base.DecodeLen(nRegDataLen);
        if (nDecodeLen > _MAX_PATH)
        {
                pszDecodeBigBuf = new char[nDecodeLen+1];
                memset(pszDecodeBigBuf, 0, nDecodeLen+1);
        }

        base.DecodeBase64(pszBigRegDataBuf, pszDecodeBigBuf);
        if ( nRegDataLen > _MAX_PATH)
                delete [] pszBigRegDataBuf;

        char 		szEndTime[40] = {0};
        if (GetEndTime(pszDecodeBigBuf, szEndTime, 39))
        {
                if ( nDecodeLen > _MAX_PATH)
                        delete [] pszDecodeBigBuf;
                return DEMOSTATUS_ERR;
        }

        if ( nDecodeLen > _MAX_PATH)
                delete [] pszDecodeBigBuf;

        int nEndTime = atol(szEndTime);
        if (!nEndTime )
                return DEMOSTATUS_ERR;

#ifdef UNIXPLATFORM
        time_t tm;
        time(&tm);
#else
        long tm;
        time(&(time_t)tm);
#endif

        if (pLeftDays)
        {
                *pLeftDays = (nEndTime - tm) / (3600 * 24) + 1;
                if (tm > nEndTime)
                {
                        *pLeftDays = (nEndTime - tm) / (3600 * 24) - 1;
                }
        }

        //已经过期
        if (tm > nEndTime)
                return DEMOSTATUS_DIE;

        if (nEndTime - tm <= RAV_CHECKVALID_WARNING_DAYS * 24 * 60 * 60)
                return DEMOSTATUS_TIP;

        return DEMOSTATUS_USE;
}
*/
BOOL CCheckValid::REGWriteString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpData)
{
#ifdef UNIXPLATFORM
        return TRUE;
#else
        if(hKey == NULL || lpSubKey == NULL || lpValueName == NULL || lpData == NULL)
                return FALSE;

        bool bRetVal = TRUE;
        int lResult = 0;
        HKEY hkKey;
        lResult = RegOpenKey(hKey, lpSubKey, &hkKey);
        if(ERROR_SUCCESS != lResult)
        {
                lResult = RegCreateKey(hKey, lpSubKey, &hkKey);
                if(ERROR_SUCCESS != lResult)	// Create Key error
                {
                        return FALSE;
                }
        }

        // 写入字符串
        DWORD dwLength = strlen(lpData) + 1;
        lResult = RegSetValueEx(hkKey, lpValueName , 0, REG_SZ, (LPBYTE)lpData, dwLength);
        if(ERROR_SUCCESS != lResult)
        {
                bRetVal = FALSE;
        }

        lResult = RegCloseKey(hkKey);
        return bRetVal;
#endif
}

int CCheckValid::REGReadString(HKEY hKey, const char* lpSubKey,
                                                                const char* lpValueName,
                                                                char* lpszData,
                                                                int nDataLen, int &nActualLen)
{
#ifdef UNIXPLATFORM
        return ERROR_SUCCESS;
#else

        if(hKey == NULL || lpSubKey == NULL || lpValueName == NULL || !lpszData)
                return RAV_CHECKVALID_INVALID_PARAMETER;

        memset(lpszData, 0, nDataLen * sizeof(char));

        HKEY hkKey;
        LONG lResult = RegOpenKey(hKey, lpSubKey, &hkKey);
        if(ERROR_SUCCESS != lResult)
                return RAV_CHECKVALID_ERR;

        //先去得到要分配的内存大小。
        DWORD dwType = REG_SZ;
        lResult = RegQueryValueEx(hkKey, lpValueName, 0, &dwType, NULL, (unsigned long *)&nActualLen);
        if(ERROR_SUCCESS != lResult)
        {
                RegCloseKey(hkKey);
                return RAV_CHECKVALID_ERR;
        }

        //取得所需要的值
        if (nActualLen > nDataLen)
        {
                RegCloseKey(hkKey);
                return RAV_CHECKVALID_BUFFER_TOO_SMALL;
        }

        lResult = RegQueryValueEx(hkKey, lpValueName , 0, &dwType, (LPBYTE)lpszData, (unsigned long *)&nActualLen);
        if(ERROR_SUCCESS != lResult)
        {
                RegCloseKey(hkKey);
                return RAV_CHECKVALID_ERR;
        }

        RegCloseKey(hkKey);

        return ERROR_SUCCESS;
#endif
}



////////////////////////////////////////////////////////////////////////////////
//	函数名称：GetDemoStatus
//	实现功能：	查询当前要安装的序列号的系统限时状态，
//				函数自己在注册表中找到当前安装的序列号，再根据对应的
//				安装时间，限时类型，截止时间和当前时间进行序列号的限时状态判断，
//				因为有了三个以上的demo版，表示不能再使用demo序列号了
//	对全局变量的影响：无
//	参数说明：lpCurSN 当前将要安装序列号
//	返回结果说明：	DEMOSTATUS_ERR = 0, // 读注册表出现错误
//					DEMOSTATUS_NOT = 1, // 不是限时版，即是正式版的号。（无限时的序列号的返回值）
//					DEMOSTATUS_USE = 2, // 在试用期内，而且不在警告期内，可以正常使用。
//					DEMOSTATUS_TIP = 3, // 在最后警告期内，可以使用，但应提示购买。
//					DEMOSTATUS_DIE = 4, // 超过了最后期限，停止使用。
//  lTrialCount: 此值为这个类型的已经安装的限时序列号的个数
////////////////////////////////////////////////////////////////////////////////
long CCheckValid::GetDemoStatus(LPCSTR lpCurSN, long &lTrialCount)
{
        //此函数遍历所有当前已经安装了的序列号
        lTrialCount = 0;
        char *buf = NULL;

        //枚举出所有序列号
        long nRet = EnumSerialNos(buf,0);
        if(nRet > 0)
        {
                buf = new char[nRet + 1];
                if(NULL == buf)
                        return DEMOSTATUS_ERR;
                memset(buf, 0, (nRet + 1) * sizeof(char));
                nRet = EnumSerialNos(buf,nRet);
                if(nRet < 0)
                {
                        delete []buf;
                        return DEMOSTATUS_ERR;
                }
        }

        //验证输入的序列号是否是合法的序列号
        //-----------------------------------------
        CCheckSn *pCheckSnMain = new CCheckSn;
        if(!pCheckSnMain)
        {
                delete []buf;
                return DEMOSTATUS_DIE;
        }
        DWORD dwSizeMain = strlen(lpCurSN);
        if( ERR_SN_VAL_SUCCESS != pCheckSnMain->CheckSn((char *)lpCurSN, dwSizeMain, 0))
        {
                delete []buf;
                return DEMOSTATUS_DIE;
        }
        LONG lBigTypeMain = pCheckSnMain->GetProductBigType();
        LONG lTypeMain = pCheckSnMain->GetProductType();
        LONG lTrialMain = pCheckSnMain->GetTrialType();
        delete pCheckSnMain;
        //-----------------------------------------

        //根据当前时间来判别序列号是否过期
#ifdef UNIXPLATFORM
        long  nDay;
        time_t tm;
        time(&tm);
#else
        long tm, nDay;
        time(&(time_t)tm);				//当前时间
#endif
        long bFinalRet = DEMOSTATUS_ERR;

        char sztime[64] = {0};
        char seps[]   = ";";
        char *token = NULL;
        if(buf)
                token = strtok( buf, seps );
        for(; token != NULL ; token = strtok( NULL, seps ))
        {
                //----------------------------------------------------
                //不同的序列号限时安装策略不同，要在上层进行限制，所以限制次数要返回给上层
                CCheckSn *pCheckSn = new CCheckSn;
                if(!pCheckSn)
                {
                        bFinalRet = DEMOSTATUS_DIE;
                        break;
                }
                DWORD dwSize = strlen(token);
                if( ERR_SN_VAL_SUCCESS != pCheckSn->CheckSn(token, dwSize, 0))	//验证序列号的合法性
                {
// Modified by LauranceZhang 2004-11-18 11:56:30 <
/*
                        bFinalRet = DEMOSTATUS_DIE;
                        break;
*/
                        //对于有非法序列号的情况要放过
                        continue;
// Modified by LauranceZhang 2004-11-18 11:56:30 >
                }
                LONG lBigType = pCheckSn->GetProductBigType();
                LONG lType = pCheckSn->GetProductType();
                LONG lTrial = pCheckSn->GetTrialType();
                //记录已经安装过的此类型的试用版的次数
                if((lBigType==lBigTypeMain) && (lType==lTypeMain))
                {
                        if( (lTrial!=0) && (lTrialMain!=0) )
                                lTrialCount ++;
                        else if( (lTrial==0) && (lTrialMain==0) )
                                lTrialCount ++;
                }
                delete pCheckSn;

                //----------------------------------------------------
                //取安装时间
//		long lType;
                time_t ltime,llasttime;
                nRet = GetInstallTime(token, sztime, sizeof(sztime));
                if(nRet != 0)	//没有安装时间
                        continue;
                ltime = atol(sztime);

                //----------------------------------------------------
// Deleted by LauranceZhang 2004-11-10 20:17:48 <
/*
                //取类型
                nRet = GetLimitType(token, sztime, sizeof(sztime));
                if(nRet < 0)	//没有序列号类型
                        continue;
                lType=atol(sztime);
*/
// Deleted by LauranceZhang 2004-11-10 20:17:48 >

                //----------------------------------------------------
                //取截止时间
                nRet = GetEndTime(token, sztime, sizeof(sztime));
                if(nRet != 0)
                        continue;//没有结束时间
                llasttime = atol(sztime);

                //----------------------------------------------------
                //判断各种情况，返回不同的值

// Modified by LauranceZhang 2004-11-10 20:15:04 <
/*
                if(lType == 0)		//无限时
                {
                        MyTrace("Formal SN==%s; Type==%d; Install Time==%s; Expired Time==%s", token, lType, ctime(&ltime), ctime(&llasttime));
                        if( 0==lstrcmpA(lpCurSN, token) )
                                bFinalRet = DEMOSTATUS_NOT;//当前序列号为可用的
                        continue;	//曾经安装过正式版
                }
*/
                if(lTrialMain == 0)		//无限时
                {
                        MyTrace("Formal SN==%s; Type==%d; Install Time==%s; Expired Time==%s", token, lTrial, ctime(&ltime), ctime(&llasttime));
                        if( 0==lstrcmpA(lpCurSN, token) )
                                bFinalRet = DEMOSTATUS_NOT;//当前序列号为可用的
                        continue;	//曾经安装过正式版
                }

// Modified by LauranceZhang 2004-11-10 20:15:04 >
                MyTrace("Limited SN==%s; Type==%d; Install Time==%s; Expired Time==%s", token, lTrial, ctime(&ltime), ctime(&llasttime));

                //如果当前时间小于安装时间, 用户一定改系统日期了，直接返回过期
                if (tm < ltime)
                {
                        if(0==lstrcmpA(lpCurSN, token))
                        {
                                bFinalRet = DEMOSTATUS_DIE;
                                break;
                        }
                }

                if(tm < llasttime)		//还没有过期
                {
                        nDay = (llasttime - tm) / (3600 * 24) + 1;
                        //当前序列号为激活的、可用的
                        if(0==lstrcmpA(lpCurSN, token))
                        {
                                //在最后警告期内，但应提示购买。
                                if(llasttime - tm <= RAV_CHECKVALID_WARNING_DAYS * 24 * 60 * 60)
                                        bFinalRet = DEMOSTATUS_TIP;
                                else
                                        bFinalRet = DEMOSTATUS_USE;		//正常使用
                                MyTrace("Left %d days for using!", nDay);
                        }
                }
                else	//已经过期
                {
                        nDay = (tm - llasttime ) / (3600 * 24) + 1;
                        if(0==lstrcmpA(lpCurSN, token))
                        {
                                bFinalRet = DEMOSTATUS_DIE;
                                MyTrace("This SN has expired %d days!", nDay);
                        }
                }
        }
/*
        //如果正在使用这个序列号,并且无超限,返回当前的状态
        if( bFinalRet!=DEMOSTATUS_ERR && nCountSN <= RAV_MAX_DEMO_NUMBER ) return bFinalRet;

        //已经超过了RAV_MAX_DEMO_NUMBER个的限时版序列号
        if((nCountSN >= RAV_MAX_DEMO_NUMBER) && (lTrial != 0))
                return DEMOSTATUS_DIE;
*/
        if(buf)
                delete []buf;
        return bFinalRet;
}

//一个序列号是否过期
BOOL CCheckValid::IsExpireSN(LPCSTR lpszSN)
{
        time_t ltime,llasttime;
        char sztime[128];

        long nRet = GetInstallTime(lpszSN, sztime, sizeof(sztime));
        if(nRet != 0) return FALSE;
        ltime = atol(sztime);

        nRet = GetEndTime(lpszSN, sztime, sizeof(sztime));
        if(nRet != 0) return FALSE;
        llasttime = atol(sztime);

        //根据当前时间来判别序列号是否过期
#ifdef UNIXPLATFORM
        time_t tm;
        time(&tm);
#else
        long tm;
        time(&(time_t)tm);
#endif

        if (tm < llasttime)
        {
                return FALSE;
        }

        return TRUE;
}

//一个序列号是否用过
BOOL CCheckValid::IsExistSN(LPCSTR lpszSN)
{
        char sztime[128];
        long nRet = GetInstallTime(lpszSN, sztime, sizeof(sztime));
        if(nRet != 0) return FALSE;
        return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function: EnumSerialNos
//Purpose: 枚举序列号
//Parameters:
// 	pszSerialNoList: [out] 存储序列号列表的Buffer.(多个序列号之间用";"隔开)
//   nBufLen: 	  [in]  Buffer的长度
//Return:
// 		>0:实际需要的长度（不包括结尾的'\0'）
//		-1表示没有在注册表中找到序列号
//		0: 成功
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CCheckValid::EnumSerialNos(char* pszSerialNoList,int nBuffLen)
{
#ifdef UNIXPLATFORM
        memset(pszSerialNoList,0,nBuffLen);

        FILE *fp = NULL;
        fp = fopen("/etc/.RavCfg", "rb");
        if(!fp)
                return 0;
        STSNINFO_UNIX stSninfo ={0};
        if(1 != fread(&stSninfo, sizeof(STSNINFO_UNIX), 1, fp))
        {
                fclose(fp);
                return 0;
        }

        CRavBase64 base;
        char szDecode[128] = {0};
        DWORD dwLen = 0, dwSnLen = 0;
        while(!feof(fp))
        {
                if(1 != fread(&stSninfo, sizeof(STSNINFO_UNIX), 1, fp))
                {
                        fclose(fp);
                        return ERR_CFG_FILE_READFAILED;
                }
                if((dwSnLen = base.DecodeBase64(stSninfo.szSN,szDecode))>0)
                {
                        if(dwSnLen != 27)
                                continue;
                        dwLen += (dwSnLen + 1);
                        if(dwLen <= (DWORD)nBuffLen)
                        {
                                strcat(pszSerialNoList,szDecode);
                                strcat(pszSerialNoList,";");
                        }
                }

        }
        fclose(fp);
        if(dwLen <= (DWORD)nBuffLen)
                return 0;
        return dwLen;
#else

        ZeroMemory(pszSerialNoList,nBuffLen);

        char	szKey[_MAX_PATH] = {0};
        char	szSn[_MAX_PATH+1] = {0};
        DWORD	dwSnLen = _MAX_PATH ;
        DWORD	dwClassLen = 0;
        DWORD	dwLen = 0;
        FILETIME ftLastWriteTime;

        HKEY	hRavSnKey ;
        DWORD	dwIndex = 0;
        LONG	lRet;
        BOOL	bMoreSn = TRUE;
        char*	pszTemp = pszSerialNoList;

        CRavBase64 base;
        char   szDecodeBuf[_MAX_PATH] = {0};

        wsprintfA(szKey,"CLSID\\%s\\",m_szProductGUID);
        if(ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT,szKey,&hRavSnKey))
        {
                while(bMoreSn)
                {
                        dwSnLen = _MAX_PATH;
                        dwClassLen = 0;
                        //枚举注册表中的序列号
                        lRet = RegEnumKeyEx(hRavSnKey,dwIndex,szSn,&dwSnLen,NULL,NULL,&dwClassLen,&ftLastWriteTime);
                        if((ERROR_SUCCESS == lRet) || (ERROR_MORE_DATA == lRet))
                        {
                                szSn[dwSnLen] = 0;
                                //解码序列号
                                if((dwSnLen = base.DecodeBase64(szSn,szDecodeBuf))>0)
                                {
                                        if(dwSnLen != 27)
                                        {
                                                dwIndex++;
                                                continue;
                                        }
                                        dwLen += (dwSnLen + 1);
                                        if(dwLen <= (DWORD)nBuffLen)
                                        {
                                                strcat(pszTemp,szDecodeBuf);
                                                strcat(pszTemp,";");
                                        }
                                }
                        }
                        else
                        {
                                bMoreSn = FALSE;
                        }
                        dwIndex++;
                }
                RegCloseKey(hRavSnKey);
        }
        if(dwLen <= (DWORD)nBuffLen)
        {
                return 0;
        }
        else
        {
                return dwLen;
        }
        return 0;
#endif
}

bool CCheckValid::SetKeySN(const char* czSn)
{
#ifdef UNIXPLATFORM
        STSNINFO_UNIX stInfo = {0};
        long lOffset = SearchSn(czSn, &stInfo, false);
        if(lOffset < 0)
                strcpy(stInfo.szSN, czSn);
        if(0 != WriteSn(lOffset, &stInfo, true))
                return 0;
        return 1;
#endif

        char	szKey[_MAX_PATH] = {0};

        strcpy(szKey, "CLSID\\");
        strcat(szKey, m_szProductGUID);

        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(czSn));
        char *lpszEncWorkBuf = new char[nLen+1];
        if (!lpszEncWorkBuf) return false;

        base.EncodeBase64((char*)czSn, lpszEncWorkBuf);

        int i = 0;
        for (i = 0; i < WRITE_FAILED_RETRY_TIMES; i++)
        {
                if (REGWriteString (HKEY_CLASSES_ROOT, szKey, KEY_SN, lpszEncWorkBuf))
                        break;
                Sleep(100);
        }

        delete [] lpszEncWorkBuf;

        return i < WRITE_FAILED_RETRY_TIMES ? true : false;
}

bool CCheckValid::GetKeySN(char* czSn)
{
        char	szKey[_MAX_PATH] = {0};

        strcpy(szKey, "CLSID\\");
        strcat(szKey, m_szProductGUID);

        CRavBase64 base;
        char *lpszEncWorkBuf = new char[1024];
        if (!lpszEncWorkBuf) return false;

        int	nDataLen = 1024, nActBufLen = 0;
        int nRetVal = REGReadString(HKEY_CLASSES_ROOT, szKey, KEY_SN, lpszEncWorkBuf, nDataLen, nActBufLen) ;
        if (ERROR_SUCCESS != nRetVal)
        {
                delete [] lpszEncWorkBuf;
                return false;
        }

        char *pszDecodeBuf = new char[1024];
        if (!pszDecodeBuf) return false;
        memset(pszDecodeBuf, 0, 1024);

        //解码序列号
        base.DecodeBase64(lpszEncWorkBuf,pszDecodeBuf);

        strcpy(czSn, pszDecodeBuf);
        delete [] pszDecodeBuf;
        delete [] lpszEncWorkBuf;

        return true;
}
#ifdef UNIXPLATFORM
//在文件中查找一个序列号，找到返回在文件中的偏移，否则为负数
long CCheckValid::SearchSn(const char *pszSn, STSNINFO_UNIX *pstSnInfo, bool bIsKeySn)
{
        FILE *fp = NULL;
        fp = fopen("/etc/.RavCfg", "rb");
        if(!fp)
                return -1;
        STSNINFO_UNIX stSninfo ={0};
        CRavBase64 base;
        if(1 != fread(&stSninfo, sizeof(STSNINFO_UNIX), 1, fp))
        {
                fclose(fp);
                return -1;
        }
        if(bIsKeySn)		//取主序列号
        {
                base.DecodeBase64(stSninfo.szSN, pstSnInfo->szSN);
                base.DecodeBase64(stSninfo.szInstallTime, pstSnInfo->szInstallTime);
                base.DecodeBase64(stSninfo.szEndTime, pstSnInfo->szEndTime);
                fclose(fp);
                return 0;
        }

        int nLen = base.EncodeLen(strlen(pszSn));
        char *pszEncode = new char[nLen+1];
        if(!pszEncode)
        {
                fclose(fp);
                return -1;
        }
        memset(pszEncode, 0, nLen+1);
        base.EncodeBase64((char*)pszSn, pszEncode);

        long lOffset = 0;
        while(!feof(fp))
        {
                lOffset = ftell(fp);
                if(1 != fread(&stSninfo, sizeof(STSNINFO_UNIX), 1, fp))
                {
                        fclose(fp);
                        delete pszEncode;
                        return -1;
                }
                if(strcmp(pszEncode, stSninfo.szSN)==0)
                {
                        base.DecodeBase64(stSninfo.szSN, pstSnInfo->szSN);
                        base.DecodeBase64(stSninfo.szInstallTime, pstSnInfo->szInstallTime);
                        base.DecodeBase64(stSninfo.szEndTime, pstSnInfo->szEndTime);
                        fclose(fp);
                        delete pszEncode;
                        return lOffset;
                }
        }
        delete pszEncode;
        fclose(fp);
        return -1;
}
long CCheckValid::WriteSn(long lOffset, STSNINFO_UNIX *pstSnInfo, bool bIsKeySn)
{
        FILE *fp = NULL;
        fp = fopen("/etc/.RavCfg", "r+b");
        if(!fp)
        {
                fp=fopen("/etc/.RavCfg", "a+b");
                if(!fp)
                        return ERR_CFG_FILE_OPENFAILED;

        }
        STSNINFO_UNIX stSninfo ={0};

        fseek(fp,  0,SEEK_END);
        long lFileLen = ftell(fp);
        if(lFileLen < sizeof(STSNINFO_UNIX))
        {
                if(1 != fwrite(&stSninfo, sizeof(STSNINFO_UNIX), 1, fp))
                {
                        fclose(fp);
                        return ERR_CFG_FILE_WRITEFAILED;
                }
        }

        if(lOffset < 0)
                fseek(fp, 0,SEEK_END);
        else
                fseek(fp,bIsKeySn ? 0:lOffset,SEEK_SET);
        CRavBase64 base;
        int nLen = base.EncodeLen(strlen(pstSnInfo->szSN));
        char szEncode[128] = {0};
        strncpy(szEncode, pstSnInfo->szSN, 128);
        base.EncodeBase64(szEncode, pstSnInfo->szSN);
        memset(szEncode, 0, 128);
        strcpy(szEncode, pstSnInfo->szInstallTime);
        base.EncodeBase64(szEncode, pstSnInfo->szInstallTime);
        memset(szEncode, 0, 128);
        strcpy(szEncode, pstSnInfo->szEndTime);
        base.EncodeBase64(szEncode, pstSnInfo->szEndTime);
        if(1 != fwrite(pstSnInfo, sizeof(STSNINFO_UNIX), 1, fp))
        {
                fclose(fp);
                return ERR_CFG_FILE_WRITEFAILED;
        }
        fflush(fp);
        fclose(fp);
        return 0;
}
#endif
