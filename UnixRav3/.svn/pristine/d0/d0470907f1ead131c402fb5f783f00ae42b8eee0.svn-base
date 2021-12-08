///////////////////////////////////////////////////////////////////////////
//  项目名	：瑞星2005版配置树模块组
//	文件名	：regapi.h
//	用  途	：该头文件提供的接口和常量可供unix各类产品使用
//	作  者	：许自龙
//	完成日期  : 2004-11-10
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _REGAPI_H
#define _REGAPI_H

//
// 为了和C兼容，导出的接口为C格式的
#ifdef __cplusplus
extern "C" {
#endif

//
// 相关的几个注册文件
//

#define RAVNETWRD_REGISTER_FILE			"/etc/.RiSingravnetwrd.reg"
#define RAVNET_PRODUCT 0
#define RAVWRD_PRODUCT 1
#define RAVTIMER_PRODUCT 2

// 2005版导出节点类型
#define EXPORTCONFIG 0x01
#define EXPORTRAVSERVICE 0x02



#define UNIX_RS_ERROR_TYPE int
#define UNIX_RS_ERROR_OK 0// 正确
#define UNIX_RS_ERROR_NEWOBJ -1 //调用

#define UNIX_RS_ERROR_LOADCONF -2
#define UNIX_RS_ERROR_CREATENODE -3
#define UNIX_RS_ERROR_GETVALUE -4
#define UNIX_RS_ERROR_IMPORT -5
#define UNIX_RS_ERROR_EXPORT -6
#define UNIX_RS_ERROR_SYNC -7
#define UNIX_RS_ERROR_GETCHILD -8
#define UNIX_RS_ERROR_SETVALUE -9

#define UNIX_RS_ERROR_SYSINFO -10
#define UNIX_RS_ERROR_NEW -11
#define UNIX_RS_ERROR_STRANGELINUX -12

#define UNIX_RS_ERROR_SNNOSUPPORTOS -13
#define UNIX_RS_ERROR_SNNOMATCHOS -14
#define UNIX_RS_ERROR_SNNOMATCHVER -15
#define UNIX_RS_ERROR_SNNOMATCHPLAT -16

#define UNIX_RS_ERROR_INVALIDPRODUCT -17
#define UNIX_RS_ERROR_INVALIDREGISTER -18
#define UNIX_RS_ERROR_OPENREGERROR -19
#define UNIX_RS_ERROR_INVALIDSN -20
#define UNIX_RS_ERROR_WRITEREGFAILED -21

#define UNIX_RS_ERROR_INVALIDPARAM -22

#define UNIX_RS_ERROR_INVALIDINSTALL -23

#define UNIX_RS_ERROR_INVALIDCONVERT -24
#define UNIX_RS_ERROR_NOEXITCFGDLL -25


//
// 	系统注册信息,2005版改变过去的两个配置文件的习惯，使用
//	一个配置文件，数据结构目前是相同的
//


typedef struct tagREGINFO
{
        char szInstallPath[1024]; // 安装路径
        char szSerial[32];		// 序列号
        long Currdate;			// 当前日期
        unsigned long Usedays;			// 使用天数
        unsigned int  proino;			// 节点号
}REGINFO;

//
//unix 客户端读取的配置文件中ravservice 项的数据结构
//
typedef struct _UNIXRsRavParam_
{
    int nKillMode; // 发现病毒的处理方式
    int nKillSuspicMode; // 发现可疑文件的处理方式
    int nKFailedMode ; // 杀毒失败的处理方式
    int nFinishMode; // 杀毒完成的处理方式
    unsigned char szForceType[256]; // 强制查杀类型

    int nFilterType; // 文件过虑类型
    char szAllFilter[256]; //中间层所有文件过虑类型
    char szExeFilter[256]; // 中间层执行文件过虑类型
    char szExtFilter[256]; // 文件后缀过虑
    int BackupType; // 是否备份
    int nScanOnly ; // 是否只查毒，据说是对测试序列号而用

    unsigned char  nKillCompress; // 是否查压缩文件
    unsigned char  nKillMbox; // 是否查邮箱文件
    unsigned char  nKillMail; //是否 查邮件
    unsigned char  nKillSelfExtract; // 是否查自解压可执行文件

    unsigned char  nKillDos; // 是否查杀Dos执行文件
    unsigned char  nKillWin;  // 是否查杀Win执行文件
    unsigned char  nKillScript;  // 是否查杀脚本文件
    unsigned char  nKillMacro;  // 是否查杀宏
    unsigned char  nKillGeneral;  // 是否查杀一般文件
        unsigned char  nKillElf; // 是否查杀elf文件
    unsigned char  nKillNet; // 是否查杀.Net文件

    unsigned char  nKillUnknownDos; // 是否查杀未知Dos执行文件
    unsigned char  nKillUnknownWin; // 是否查杀未知Win执行文件
    unsigned char  nKillUnknownScript; // 是否查杀未知脚本文件
//	unsigned char  nKillUnknownMail; // 是否查杀未知邮件
    unsigned char  nKillUnknownMacro; // 是否查杀未知宏
    unsigned char  nKillUnpack; // 是否进行虚拟脱壳
    unsigned char  nKillBoot; // 是否查已知引导型病毒
    unsigned char  nKillDepth; // 是否深度启发扫描
    unsigned char  nKillSuspiciousFile; // 查杀可疑文件
    unsigned char  nIfScanDays; // 是否查杀最近N天的文件
    unsigned int   nMaxZipSize; // 查压缩包最大大小值
    unsigned int   nMaxSfxSize; // 查自解压包最大大小值
    unsigned int   nMaxMailboxSize; // 查自邮箱最大大小值
    unsigned int   nScanDays; // 扫描最近N天的文件
    unsigned int   nScanDangerType; // 扫描的危险类型



    int nWhiteLib; // 是否使用白名单
    int nGoodLib;  // 是否使用良民库
    int nExceptLib; //是否将异常文件加入良名库



}UNIXRSRAVPARAM;

// 定义用于序列号的操作结构结构
typedef struct _unixsnparam_
{
    char szSn[50]; // 序列号
    int nBigProdId; // 产品大类型
    int nProdId;  // 当前产品类型
    int nChildProdId; // 产品子类型
    int nExpireType; // 产品过期类型
    int nPNo; //批号
    int nStreamNo; //流水号
    int nAreaNo ; //地区号
    int nOEMId; // OEM ID
    int nHardWare; //硬件平台
    int nOsType; // OS 类型
    int nOsVer;  // OS 版本
    int nCenterNum; // 系统中心数
    int nServerNum; // 服务器数量
    int nClientNo;
    int nDecrypt;
    int nPrId;	// 产品ID
    int nExpireFlag;
}UNIXSNPARAM;

// 根据错误码区的错误信息
extern char *unixrscfgerr(UNIX_RS_ERROR_TYPE nErr,char *szBuf,int nLen);

// 初始化配置树
extern UNIX_RS_ERROR_TYPE InitCfgConfig(int nCode,const char *szConfigFile,void **pHandle);
extern UNIX_RS_ERROR_TYPE ExitCfgConfig(int nCode,void **pConfig);

// 检查指定的序列好是否匹配当前系统
extern UNIX_RS_ERROR_TYPE CheckCfgSnValid(void *pConfig,int nCode, const char *szSerial,UNIXSNPARAM *pStSn);

// 读取当前安装的序列号信息
extern UNIX_RS_ERROR_TYPE ReadCfgSnInfo(void *pConfig,int nCode,UNIXSNPARAM *pStSn);

// 写注册文件和配置树中的序列号
extern UNIX_RS_ERROR_TYPE WriteCfgSnInfo(void *pConfig,int nCode,char *szSerial,UNIXSNPARAM *pStSn);


// 读取查杀参数
extern UNIX_RS_ERROR_TYPE ReadCfgScanParam(void *pConfig,int nCode,void *pParm);

// 写查杀参数
extern UNIX_RS_ERROR_TYPE WriteCfgScanParam(void *pConfig,int nCode,void *pParm);

// 将2004的配置文件转换为2005的配置文件
//extern int Convert2004Reg();

// 获取安装路径
UNIX_RS_ERROR_TYPE GetInstallPath(int nCode,char *szInstallPath,int nLen);
UNIX_RS_ERROR_TYPE GetInstallPath_(int nCode,char *szInstallPath,int nLen,REGINFO *pReg);

// 写注册文件和配置树中的序列号
UNIX_RS_ERROR_TYPE WriteRegInfo(void *pConfig,int nCode,char *szInstPath,char *szSerial);

// 读取序列号和产品ID，主要为升级使用
UNIX_RS_ERROR_TYPE ReadSNID(const char *configfile, const char *installpath, const char *serial, UNIXSNPARAM *pStSn);
UNIX_RS_ERROR_TYPE ExportNode(void *pConfig,int nCode,char *szFile);
UNIX_RS_ERROR_TYPE CopyRsCfg(int nCode,const char *szFile);

UNIX_RS_ERROR_TYPE readserial(REGINFO *pRegInfo);

bool checkCfgExpired(const char *sn);
bool resetSN(const char *sn);
bool warningExpired(const char *cn);
bool isTrialVersion();
bool isTrialSnVersion(const char *sn);

#ifdef __cplusplus
}
#endif

#endif //_REGAPI_H
