////////////////////////////////////////////////////////////////////////////////
//	项目名称：瑞星网络版
//	文件名称：CheckValid.h
//	实现功能：
//	作    者：曾杰
//	编写日期：2002-7-24 17:54:23
//
//	修改历史（自下而上填写 内容包括：日期  修改人  修改说明）
//  2004-8-19	张林刚	修改此类为各个产品通用的类，增加了一些函数
//  增加的函数有：SetKeySN, GetDemoStatus, EnumSerialNos, IsExistSN, IsExpireSN
////////////////////////////////////////////////////////////////////////////////

#ifndef __CHECKVALID_H__
#define __CHECKVALID_H__

#include "RSTYPE.H"
#include "cfgdef2.h"

// 试用版过期检查的主键
//"{F1CF8F61-AB1D-11d4-ABBD-0050BACEC828}" 单机和网络
//"{5E655840-114E-4bbb-A252-3A139E7E6DFD}" Domino
//"{B7B4C60A-E930-48c1-8EF5-C710BF326BE7}" Exchange

#define SETUP_TIME		"ProcInfo"		//安装时间
#define SETUP_TYPE		"ProcKind"		//过期类型
#define END_TIME		"ProcDll"		//过期时间

#define	CUR_SERIAL		"ProgramKind"	//以前记录主序列号的地方
#define KEY_SN			"ProcKey"		//现在记录主序列号的地方

#define RAV_CHECKVALID_WARNING_DAYS			10		//警告期的天数，试用期的最后几天为警告期，需要提示购买
//#define	RAV_MAX_DEMO_NUMBER					3		//允许使用的试用序列号数目
#define WRITE_FAILED_RETRY_TIMES			3		//写注册表失败后重试的次数

#define RAV_CHECKVALID_BUFFER_TOO_SMALL		-101
#define RAV_CHECKVALID_INVALID_PARAMETER	-102
#define	RAV_CHECKVALID_ERR					-103


struct STPRODUCTGUID
{
        DWORD dwProductID;
        CHAR szProductGUID[40];
};

#ifdef UNIXPLATFORM
struct STSNINFO_UNIX
{
        char szSN[128];
        char szInstallTime[32];
        char szEndTime[32];
};
#endif

class CCheckValid
{
private:
        CHAR m_szProductGUID[40];
        LONG m_lProductID;
public:
        CCheckValid();
        ~CCheckValid();

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
        bool SetInstallTime( const char* czSn, const char* czInstallTime );

        ////////////////////////////////////////////////////////////////////////////////
        //	函数名称：bool SetLimitType(
        //				const char* czSn,
        //				const char* czLimitType )
        //	实现功能：设置指定序列号的限时类型。
        //	对全局变量的影响：无
        //	参数说明：
        //		czSn			- 输入，指定的序列号
        //		czLimitType		- 输入，限时类型（十进制的字符串）
        //	返回结果说明：成功true,失败false
        ////////////////////////////////////////////////////////////////////////////////
        bool SetLimitType( const char* czSn, const char* czLimitType );

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
        bool SetEndTime( const char* czSn, const char* czEndTime );

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
        long GetInstallTime( const char* czSn, char* czInstallTime, int nBuffLen );

        ////////////////////////////////////////////////////////////////////////////////
        //	函数名称：long GetLimitType(
        //				const char* czSn,
        //				char* czLimitType,
        //				int nBuffLen )
        //	实现功能：查询指定序列号的限时类型
        //	对全局变量的影响：无
        //	参数说明：
        //		czSn				- 输入，指定的序列号
        //		czLimitType			- 输入/输出，限时类型串的首地址
        //		nBuffLen			- 输入，czLimitType的buffer长度
        //	返回结果说明：实际需要的长度（不包括结尾的'\0'）
        //					-1表示没有在注册表中找到限时类型
        ////////////////////////////////////////////////////////////////////////////////
        long GetLimitType( const char* czSn, char* czLimitType, int nBuffLen );

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
        long GetEndTime( const char* czSn, char* czEndTime, int nBuffLen );

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
//	long GetCurDemoStatus(int *pLeftDays = NULL);

        //写注册表
        BOOL REGWriteString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpData);

        //读注册表
        int REGReadString( HKEY hKey, const char* lpSubKey,
                                           const char* lpValueName,
                                           char* lpszData,
                                           int nDataLen, int &nActualLen);

        //不同的产品使用不同的GUID，所以在使用此类之前要初始化一个GUID
        BOOL SetGUID(LONG lProductID);

        ////////////////////////////////////////////////////////////////////////////////
        //	函数名称：GetDemoStatus
        //	实现功能：查询当前要安装demo版序列号时的系统限时状态，
        //				函数自己在注册表中找到当前安装的序列号，再根据对应的
        //				安装时间，限时类型，截止时间和当前时间进行序列号的限时状态判断，
        //				因为有了三个以上的demo版，表示不能再使用demo序列号了
        //	对全局变量的影响：无
        //	参数说明：lpCurSN 当前将要安装序列号,一定是demo版的!!!
        //	返回结果说明：	DEMOSTATUS_ERR = 0, // 读注册表出现错误
        //					DEMOSTATUS_NOT = 1, // 不是试用版。
        //					DEMOSTATUS_USE = 2, // 在试用期内，而且不在警告期内，可以正常使用。
        //					DEMOSTATUS_TIP = 3, // 在最后警告期内，可以使用，但应提示购买。
        //					DEMOSTATUS_DIE = 4, // 超过了最后期限，停止使用。
        ////////////////////////////////////////////////////////////////////////////////
        long GetDemoStatus(LPCSTR lpCurSN, long &lTrialCount);
        //long GetDemoStatus(LPCSTR lpCurSN);

        //一个序列号是否过期
        BOOL IsExpireSN(LPCSTR lpszSN);

        //一个序列号是否用过
        BOOL IsExistSN(LPCSTR lpszSN);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Function: EnumSerialNos
        //Purpose: 枚举序列号
        //Parameters:
        // 	pszSerialNoList: [out] 存储序列号列表的Buffer.(多个序列号之间用";"隔开)
        //   nBufLen: 	  [in]  Buffer的长度
        //Return:
        // 		>0:实际需要的长度（不包括结尾的'\0'）
        //		-1:表示没有在注册表中找到序列号
        //		0: 成功
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int EnumSerialNos(char* pszSerialNoList,int nBuffLen) ;
        bool SetKeySN(const char* czSn);
        bool GetKeySN(char* czSn);
#ifdef UNIXPLATFORM
        long SearchSn(const char *pszSn, STSNINFO_UNIX *pstSnInfo, bool bIsKeySn = false);
        long WriteSn(long lOffset, STSNINFO_UNIX *pstSnInfo, bool bIsKeySn = false);
#endif
};


#endif //__CHECKVALID_H__
