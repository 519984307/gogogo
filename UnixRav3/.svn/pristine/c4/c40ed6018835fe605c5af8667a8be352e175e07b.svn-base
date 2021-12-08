#ifndef __CHECKSN_H__
#define __CHECKSN_H__

#include "RSTYPE.H"
#include "ICheckSn.h"

//批号划分线
#define		CONFIG_SNBATCH_DIVIDELINE1	0x0000001b		//一二批划分线(27)
#define		CONFIG_SNBATCH_DIVIDELINE2	0x00000023		//二三批划分线(35)
#define		CONFIG_SNBATCH_DIVIDELINE3	0x00000025		//三四批划分线(37)
#define		CONFIG_SNBATCH_DIVIDELINE4	0x0000002D		//四五批划分线(45)

typedef struct STSNINFO
{
        RS_LONG		lSCNVerID;				//区分是用那一版本解密  [4/12/2002]
        RS_LONG		lNetOrPc;				//区分网络还是单机版 [4/11/2002]  1 单机版 2 网络版
        RS_LONG		VerType;				//版本类型
        RS_LONG		SysCnt;					//系统中心数量
        RS_LONG		Svr;					//服务器数量
        RS_LONG		Clnt;					//客户端数量
        RS_LONG		Btch;					//批号
        RS_LONG		SNo;					//流水号
        RS_LONG		lAreaID;				// 地区号 中国大陆(0);中国香港(1)
        RS_LONG		lOEMID;					// OEM厂家ID
        RS_LONG		lLimitTime;				// 限时时间 无限制(0);1个月(1);半年(2);一年(3)
        RS_LONG		lEditionType;			// 版本类型 标准版(0);专用版(1)
        RS_LONG		lHardWareID;			// 硬件平台 SPARC(1);X86(2);ALPHA(3)
        RS_LONG		lSystemID;				// OS系统类型
        RS_LONG		lSystemVer;				// OS系统版本
        RS_LONG		lActiveXType;			// 插件类型
}*PSTSNINFO;


class CCheckSn : public ICheckSn
{
public:
        CCheckSn();
        ~CCheckSn();

        RS_LONG	CheckSn(RS_CHAR *pstSn, RS_INT nSnSize, RS_DWORD dwSnKey);	//检测序列号
        RS_LONG	GetProductID();					//取产品唯一号
        RS_LONG	GetTranslatedProductID();		//取升级后的产品号
        RS_LONG	GetProductBigType();			//版本大类型，区分网络还是单机版1 单机版 2 网络版
        RS_LONG	GetProductType();				//版本类型
        RS_LONG	GetProductSubtype();			//版本类型 标准版(0);专用版(1)
        RS_LONG	GetNumCenters();				//系统中心数量
        RS_LONG	GetNumServers();				//服务器数量
        RS_LONG	GetNumClients();				//客户端数量
        RS_LONG	GetBatchNo();					//批号
        RS_LONG	GetFlowNo();					//流水号
        RS_LONG	GetRegionNo();					//地区号 中国大陆(0);中国香港(1)
        RS_LONG	GetOEMID();						//OEM厂家ID
        RS_LONG	GetTrialType();					//限时时间无限制(0);1个月(1);半年(2);一年(3)
        RS_LONG	GetTrialDays();					//获得限时时间的天数
        RS_LONG	GetHardwareType();				//硬件平台 SPARC(1);X86(2);ALPHA(3)
        RS_LONG	GetOSType();					//OS系统类型
        RS_LONG	GetOSVersion();					//OS系统版本
        RS_LONG	GetDecodeVer();					//版本解密
        RS_LONG	GetFirstError();				//取第一个错误号
        RS_LONG	GetLastError();					//取最后一个错误号

private:
        void	InitParam();					//初始化变量

        void	SetErrCode(RS_DWORD dwErrCode);	//置错误号

        RS_BOOL GetSnInfo(RS_CHAR *Source);		//取返回的结果

        // 解码解出相对应的信息
        RS_BOOL DecSNO_0(RS_CHAR *pszno,PSTSNINFO pNetSN,RS_LONG lBatchNo);
        RS_BOOL DecSNO_1(RS_CHAR *szSerialNo,PSTSNINFO pNetSN);
        RS_UINT DecSNO_2(RS_CHAR *psno,PSTSNINFO pNetSN,RS_ULONG SNoKey);
        RS_BOOL DecSNO_3(RS_CHAR *aszSerialNo, RS_INT nSizeSCN, RS_CHAR *abSource, RS_INT nSizeSource, RS_CHAR *ErrDescription,	RS_INT nSizeErr);
        RS_BOOL DecSNO_4(RS_CHAR *aszSerialNo, RS_INT nSizeSCN, RS_CHAR *abSource, RS_INT nSizeSource, RS_CHAR *ErrDescription, RS_INT nSizeErr);
        RS_BOOL DecSNO_5(RS_CHAR *aszSerialNo, RS_INT nSizeSCN, RS_CHAR *abSource, RS_INT nSizeSource, RS_CHAR *ErrDescription, RS_INT nSizeErr);
        RS_BOOL SetInfo(RS_UCHAR *SCNInfo );
        RS_SHORT Instr(RS_UCHAR *tar, RS_UCHAR src );
        RS_CHAR* CheckBatch0(RS_CHAR *OrgSer);
private://私有数据
        RS_DWORD	m_dwFirstErrCode;			//第一个错误号
        RS_DWORD	m_dwLastErrCode;			//最后一个错误号
        RS_BOOL		m_bIsChecked;				//是否已检测标志

        STSNINFO	m_stSnInfo;					//返回的信息的结构
        RS_UCHAR	m_szSnInfo[16];				//序列号信息缓冲区

        RS_CHAR		m_aszSn[40];
        RS_INT		m_nSnSize;
        RS_DWORD	m_dwSnKey;
};

#endif // __CHECKSN_H__


