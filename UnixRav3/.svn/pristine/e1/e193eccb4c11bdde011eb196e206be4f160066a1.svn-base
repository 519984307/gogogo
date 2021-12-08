///////////////////////////////////////////////////////////////////////////
//  项目名	：瑞星2005版配置文件树
//	文件名	：unixrscfg.h
//	用  途	：定义一些宏，声明CUnixRsCfg类
//	作  者	：许自龙
//	完成日期  : 2004-11-10
//
//	历史和原因说明：
//		由于瑞星unix2004eng 和2005的配置文件中有关的数据类型定义存在
//		存在冲突，只好将2005配置文件树的所有资源包括头文件严格封装，所有的
//		对2005配置树的访问只能通过CUnixRsCfg 类完成，该头文件不能包含任何与配置
//		树相关的头文件
/////////////////////////////////////////////////////////////////////////////

#ifndef _PUBLICCFG_H_
#define _PUBLICCFG_H_

#include <regapi.h>
#include <configimpl.h>

enum	RS_RAVMAIN_CONFIG
{
        //主程序发现病毒的处理方式
        RS_RAVMAIN_CFG_METHOD_FINDVIURS = 10,
        /*	类型 = 值
        *	METHOD_FINDVIURS = FINDVIRUS_QUERYUSER
        */

        //主程序杀毒失败后的处理方式
        RS_RAVMAIN_CFG_METHOD_KILLFAILED,
        /*	类型 = 值
        *	METHOD_KILLFAILED = KILLFAILED_QUERYUSER
        */

        //主程序杀毒完成后的处理方式
        RS_RAVMAIN_CFG_METHOD_SCANFINISH,
        /*	类型 = 值
        *	METHOD_SCANFINISH = SCANFINISH_RETURN
        */

        // 发现病毒是否声音报警
        RS_RAVMAIN_CFG_VIRUSALERT		= 0x20,
        //	INT = 0 不 1 Beep报警 2 Wave声音 3 其他

        // 杀毒日志记录设置
        RS_RAVMAIN_CFG_SCANLOGSIZE		= 0x21,
        // INT = 0 不记录日志， -1 不限制大小 XX 日志大小(K)

        // 定时杀毒日志记录设置
        RS_RAVMAIN_CFG_TIMERLOGSIZE		= 0x22,
        // INT = 0 不记录日志， -1 不限制大小 XX 日志大小(K)

        // 病毒日志记录设置
        RS_RAVMAIN_CFG_VIRUSLOGSIZE		= 0x23,
        // INT = 0 不记录日志， -1 不限制大小 XX 日志大小(K)

        // 杀毒线程的线程优先级
        RS_RAVMAIN_CFG_SCANPRIORITY		 = 0x30,
        // INT = 线程优先级数值

        // 杀毒计数线程的线程优先级
        RS_RAVMAIN_CFG_SCANCOUNTPRIORITY,
        // INT = 线程优先级数值

        // 静默杀毒( 禁止通知 MessageBox )
        RS_RAVMAIN_CFG_HIDEUIMSGBOX	,
        // BOOL 不显示通知消息 , 需要选择 Yes or No 的都选择 Yes

        // 打开启动自检
        RS_RAVMAIN_CFG_BOOTTEST	,
        // BOOL 检查自己的完整性，以及依赖文件是否存在

        // 静默启动( 禁止显示用户界面 )
        RS_RAVMAIN_CFG_HIDEUI,

        // 强制查杀类型( 强制指定杀毒范围，无视用户选择 )
        RS_RAVMAIN_CFG_SCANTYPE,
        // INT = 0 默认 1 引导区 2 邮件 4 内存 0x10 全部硬盘 0x20 指定目录

        // 强制查杀目录( 强制指定杀毒范围，无视用户选择 )
        RS_RAVMAIN_CFG_SCANDIR,
        // STRING RS_RAVMAIN_CFG_SCANTYPE 指定查杀目录, 这个保存目录

        // 启动的时候是否加载RavTimer
        RS_RAVMAIN_CFG_LOADTIMER,

        //	启动的时候是否启动RsAgent
        RS_RAVMAIN_CFG_LOADAGENT,

        //	是否使用瑞星主页
        RS_RAVMAIN_CFG_SHOWWEBSITE,
};
//	中间层配置
//IRsScanner对象	配置ID为RS_SCANNER_CFG_SCAN_FILETYPE,	值

enum	RS_SCANNER_CONFIG
{

        //说明	中间层查杀文件类型
        RS_SCANNER_CFG_SCAN_FILETYPE = 10,
        /*	类型 = 值
         *	RS_SCANNER_CONFIG_SCAN_FILE_TYPE = FINDVIRUS_QUERYUSER
         */

        //说明	中间层所有文件过滤字符串
        RS_SCANNER_CFG_ALL_FILTER,
        /*	类型 = 值
        *	LPSTR = "*"
        */

        //说明	中间层可执行文件过滤字符串
        RS_SCANNER_CFG_EXECUTE_FILTER,
        /*	类型 = 值
         *	LPSTR = "EXE|COM|DLL"
         */

        //	中间层文件后缀名过滤字符串
        RS_SCANNER_CFG_FILENAME_FILTER,
        /*	类型 = 值
         *	LPSTR	包括主程序，监控等配置中 = "TEST|SSS|CCC"
         */

        //	中间层是否自动备份文件
        RS_SCANNER_CFG_STORE_FILE,
        /*	类型 = 值
         *	BOOL = TRUE
         */

        //	中间层是否遇到病毒只查不杀（查毒版本专用）
        RS_SCANNER_CFG_ONLY_SCAN,
        /*	类型 = 值
         *	BOOL = TRUE
         */
};

enum RS_FILETYPE_CONFIG {
    RS_FILETYPE_COMPRESSED = 0x01,
    RS_FILETYPE_MAILBOX,
    RS_FILETYPE_MAIL,
    RS_FILETYPE_UNPACK,
    RS_FILETYPE_DOSEXECUTE,
    RS_FILETYPE_WINEXECUTE,
    RS_FILETYPE_SCRIPT,
    RS_FILETYPE_MACRO,
    RS_FILETYPE_NORMAL,
    RS_FILETYPE_UNKNOWNDOS,
    RS_FILETYPE_UNKNOWNWIN,
    RS_FILETYPE_UNKNOWNSCRIPT,
    RS_FILETYPE_UNKNOWNMACRO = 0x0e,
    RS_FILETYPE_ELF = 0x18,
    RS_FILETYPE_SELFEXTRACT=0x19
};

#define RAV_SERVICE  0x6000



//
// 与序列好验证相关的几个全局变量

#define   CURRENT_OS_NUMBER            16 // 操作系统数
#define   MAX_VERSION_NUMBER           16 // 操作系统最大版本数
#define   MAX_PLATFORM_NUMBER          8
#define   DEFAULT_STRING               "RISING"




//
// 验证系统匹配的数据结构
//
typedef struct tagOsSnMap
{
  char strOsName[64];   //操作系统名称
  char strOsVersion[MAX_VERSION_NUMBER][32];//小版本名称
  char strPlatName[MAX_PLATFORM_NUMBER][32];//平台名称
}OSSNMAP;



class CUnixRsCfg
{
public:
        CUnixRsCfg(const char *szFilename);
        ~CUnixRsCfg();
        UNIX_RS_ERROR_TYPE Init();
        UNIX_RS_ERROR_TYPE ReadRavService(UNIXRSRAVPARAM &uParam);
        UNIX_RS_ERROR_TYPE ImportRsCfg(unsigned long  nCode,char *szImportFile);

        UNIX_RS_ERROR_TYPE ExportRsCfg(unsigned long int nCode,char *szExportFile);

        UNIX_RS_ERROR_TYPE ReadSnInfo(UNIXSNPARAM &stSerino);
        UNIX_RS_ERROR_TYPE WriteSnInfo(char *szSerialNo);
        UNIX_RS_ERROR_TYPE CheckSerino(char *szSerialNo,UNIXSNPARAM &stSerino);

//
// 用于检验
//
        UNIX_RS_ERROR_TYPE CheckOsMatch(UNIXSNPARAM stSerino);
        UNIX_RS_ERROR_TYPE CheckTimeOut(UNIXSNPARAM stSerino);
        UNIX_RS_ERROR_TYPE GetSystemInfo();
        UNIX_RS_ERROR_TYPE InitOsSnMap();

private:
        char m_szRsConfig[256];
        IConfig *m_pConfig;
//
// 当前系统的信息
//
        char szSelfPlatName[128];   //平台名称
        char szSelfOsVersion[128];  //操作系统版本
        char szSelfOsName[128];  //操作系统名称
        OSSNMAP *pstOsType;


};

#endif

