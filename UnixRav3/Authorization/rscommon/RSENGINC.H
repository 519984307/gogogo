#ifndef __RSENGINC_H__
#define __RSENGINC_H__

typedef enum _VIRUSTYPE
{
	VIRUSTYPE_UNKNOWN=-1,		//未知类型
	VIRUSTYPE_DOSCOM=0,			//dos下的com病毒
	VIRUSTYPE_DOSEXE,			//dos下的exe病毒
	VIRUSTYPE_WINPE,			//windows下的pe病毒
	VIRUSTYPE_WINNE,			//windows下的ne病毒
	VIRUSTYPE_MEM,				//内存病毒
	VIRUSTYPE_MACRO,			//宏病毒
	VIRUSTYPE_SCRIPT,			//脚本病毒
	VIRUSTYPE_BOOT,				//引导区病毒
	VIRUSTYPE_WINLE,			//windows下的le病毒
	VIRUSTYPE_NORMAL,			//普通型病毒
	VIRUSTYPE_ELF,				//unix下的elf文件
	VIRUSTYPE_MAIL,				//邮件病毒
	VIRUSTYPE_BOOT_FLOPY=21,	//软盘引导区
	VIRUSTYPE_BOOT_HDMBR,		//硬盘主引导记录
	VIRUSTYPE_BOOT_HDSYS,		//硬盘系统引导区
};

//查杀毒配置选项
//查压缩文件
#define		RSSCAN_COMPRESS			1
//查邮箱文件		
#define		RSSCAN_MAILBOX			2
//查邮件文件		
#define		RSSCAN_EML				3
//查自解压可执行文件	
#define		RSSCAN_UNPACKER			4
//查DOS可执行文件		
#define		RSSCAN_DOSEXECUTE		5
//查Windows可执行文件	
#define		RSSCAN_WINEXECUTE		6
//查脚本文件		
#define		RSSCAN_SCRIPT			7
//查宏文件		
#define		RSSCAN_MACRO			8
//查普通文件		
#define		RSSCAN_NORMAL			9
//查未知DOS可执行文件	
#define		RSSCAN_UNDOSEXECUTE		10
//查未知Windows文件	
#define		RSSCAN_UNWINEXECUTE		11
//查未知脚本文件		
#define		RSSCAN_UNSCRIPT			12
//查未知邮件文件		
#define		RSSCAN_UNMAIL			13
//查未知宏文件		
#define		RSSCAN_UNMACRO			14
//使用白名单		
#define		RSSCAN_WHITELIST		15
//使用良民库		
#define		RSSCAN_NVLIST			16
//异常文件入良民库	
#define		RSSCAN_EXCEPTTONV		17
//以指定格式进行查毒(0:全部都查)
#define		RSSCAN_SCANTYPE			18
//查已知引导型病毒
#define		RSSCAN_BOOT				19
//查未知引导型病毒
#define		RSSCAN_UNBOOT			20
//使用虚拟解压缩
#define		RSSCAN_VEUNPACK			21
//DUMP中间文件（引擎内部使用）
#define		RSSCAN_DUMPTEMPFILE		22

//查未知邮件文件

//属性ID
//属性分成Target属性，复合文件属性，邮件属性

//文件对象的属性
#define		RSPROP_BASE				0x100
//父对象
#define		RSPROP_OWNER			RSPROP_BASE+0x1
//当前状态
#define		RSPROP_STATUS			RSPROP_BASE+0x2
//逻辑文件名称
#define		RSPROP_NAME				RSPROP_BASE+0x3
//取文件对象属性
#define		RSPROP_ATTRIBUTEEX		RSPROP_BASE+0x4
//设置frag文件最大反复次数
#define     RSPROP_REPEAT			RSPROP_BASE+0X5
//强制解到实际文件
#define		RSPROP_HANDLEFILE		RSPROP_BASE+0X6

//复合文件对象的属性(GetProperty)
//复合文件是否被加密
#define		RSPROP_ISENCRYPTED		RSPROP_BASE+0x11
//复合文件中的文件个数
#define		RSPROP_FILECOUNT		RSPROP_BASE+0x12
//当前索引
#define		RSPROP_CURRENT_INDEX	RSPROP_BASE+0x13
//可否回写
#define		RSPROP_CANUPDATE		RSPROP_BASE+0x14

//复合文件对象中一项的属性(GetItemProperty)
//复合文件中单个文件的原始大小
#define		RSPROP_FILEOSIZE		RSPROP_BASE+0x21
//复合文件中单个文件的压缩大小
#define		RSPROP_FILEPSIZE		RSPROP_BASE+0x22
//指定文件类型(只查指定格式的病毒)
#define		RSPROP_FILETYPE			RSPROP_BASE+0x23

//邮件的一些属性
//邮件发送者
#define		RSPROP_MAILSENDER		RSPROP_BASE+0x31
//邮件接受者
#define		RSPROP_MAILRECEIVER		RSPROP_BASE+0x32
//邮件CC
#define		RSPROP_MAILCC			RSPROP_BASE+0x33
//邮件BCC
#define		RSPROP_MAILBCC			RSPROP_BASE+0x34
//邮件标题
#define		RSPROP_MAILSUBJECT		RSPROP_BASE+0x35
//邮件附件列表
#define		RSPROP_MAILATTLIST		RSPROP_BASE+0x36
//邮件附件的类型
#define		RSPROP_MAILSUBTYPE		RSPROP_BASE+0x37

//正文 or 附件
enum {MST_TEXT_HTML = 0, MST_ATTACHMENT};
//to be continued...
//

//这堆宏用于判断Archive的类别
#define IsCompoundArchive(fmt)			((fmt > RSID_ENG_COMPRESS) && (fmt <= RSID_ENG_MBOE5))
#define IsMailBoxArchive(fmt)			((fmt >= RSID_ENG_MBFM3) && (fmt <= RSID_ENG_MBOE5))
#define IsEMLArchive(fmt)				(fmt == RSID_ENG_MAIL)
#define IsPackerArchive(fmt)			(fmt >= RSID_ENG_PACKER) && (fmt < RSID_ENG_COMPOUND))
#define IsDosArchive(fmt)				((fmt >= RSID_ENG_COMARCHIVE) && (fmt <= RSID_ENG_MZARCHIVE))
#define IsPEArchive(fmt)				(fmt == RSID_ENG_PEARCHIVE)
#define IsScrtArchive(fmt)				((fmt >= RSID_ENG_VBSARCHIVE) && (fmt <= RSID_ENG_ENCVBSARCHIVE))
#define IsMacrArchive(fmt)				(fmt == RSID_ENG_MACARCHIVE)
#define IsNormArchive(fmt)				(fmt == RSID_ENG_NORMARCHIVE)
#define IsBootArchive(fmt)				(fmt == RSENGINE_BOOTARCHIVE)


//病毒ID定义
//-------------
//|00|00|00|00|
//-------------
//第一个字节是标志位，后三个字节是已知病毒的ID或者是未知病毒的行为特征
//四个字节全FF，表示出错
//第一个字节的位的含义:
/*
|7|6|5|4|3|2|1|0|
 | | | | | | | |-------------------------|
 | | | | | | |---------------------------|
 | | | | | |-----------------------------|--未知病毒类型标识（5个bit）
 | | | | |-------------------------------|---参见_VIRUSTYPE
 | | | |---------------------------------|
 | | |--------------------------------------保留，置为0
 | |----------------------------------------保留，置为0
 |------------------------------------------未知病毒标记
*/

#define VIRUSID_UNKNOWN			0x80000000
#define VIRUSID_ERROR			0xFFFFFFFF
#define SETVIRUSTYPE(x,t)		(((x) & 0xE0FFFFFF)|(t << 24))
#define GETVIRUSTYPE(x)			(((x) & 0x1F000000) >> 24)
#define SETVIRUSUNKOWN(x)		((x) | VIRUSID_UNKNOWN)
#define ISVIURSUNKNOWN(x)		((x) & VIRUSID_UNKNOWN)
#define HAVEVIRUS(x)			((x) & 0xFFFFFF)

//当是未知病毒时，后三个字节的位的含义（根据病毒类型不同，位含义各有不同）
//未知宏
#define UNMACRO_WRITETEMPLATE		0x1			//改写模板动作
#define UNMACRO_WRITEBINFILE		0x2			//有写大的二进制文件的动作	通常写木马
#define UNMACRO_DELFILES			0x4			//有删除文件动作	*,?
#define UNMACRO_EXEFILES			0x8			//有写可以运行的文件动作	vbs,bat,com,exe,scr,js,reg
#define UNMACRO_RUNDANGERDOS		0x10		//有执行危险dos命令的动作	del,format,deltree
#define UNMACRO_COPYMACRO			0x20		//有拷贝宏脚本的动作
#define UNMACRO_OTHERDANGER			0x40		//有其他危险的病毒常用的宏脚本操作	笼统的概括其他类型
#define UNMACRO_RESERVERFORMACRO	0x80		//为macro未知病毒保留

//未知脚本
#define UNSCRIPT_MAIL				0x1			//发送邮件操作
#define UNSCRIPT_FILESART 			0x2			//写文件，并设置启动项
#define UNSCRIPT_FILEREGENC			0x4			//写文件，写注册表，加解密串
#define UNSCRIPT_FILEWRITEIRC		0x8			//写IRC文件
#define UNSCRIPT_REGKEY				0x10		//修改注册表关键项目
#define UNSCRIPT_FILEWRITEBAT		0x20		//写批处理文件
#define UNSCRIPT_FILEEXECUTE		0x40		//写文件，执行命令execute，解密串 
#define UNSCRIPT_STRINGENC			0x80		//执行一个特长的字符串，并解密（误报比较多正在解决）
#define UNSCRIPT_FILEOPCHR			0x100		//写文件，并有大量的CHR编码操作
#define UNSCRIPT_EXESTART			0x200		//启动程序，发送消息，写注册表
#define UNSCRIPT_OPFLODER			0x400		//遍历目录，删除文件
#define UNSCRIPT_SHELLDOS			0x800		//循环执行一个dos命令
#define UNSCRIPT_SYSCALL			0x1000		//打开系统管理对象，操作系统的东西，例如进程等
#define UNSCRIPT_CALLOUTLOOK		0x2000		//打开outlook程序，发送消息
#define UNSCRIPT_SHELLDANGCMD		0x4000		//执行破坏命令
#define UNSCRIPT_CREATESHORTSTR		0x8000		//创建快捷方式
#define UNSCRIPT_CHGWININI			0x10000		//修改配置文件win.ini
#define UNSCRIP_USEAPPLET			0x20000

//未知邮件：
#define UNMAIL_IFRAMEAUTORUN		0x1			//利用IFARME漏洞

//未知PE
#define UNPE_WIN95LOADEREXPLOIT		0x1			//利用Win95装载器漏洞
#define UNPE_WIN9XRING0EXPLOIT 		0x2			//利用Win9x漏洞得到系统级权限
#define UNPE_WIN9XABNORMAPICALL		0x4			//Win9x型病毒，采用非法手段获得API调用，调用文件相关函数，感染文件
#define UNPE_WIN32ABNORMAPICALL		0x8			//Win32型病毒，采用非法手段获得API调用，调用文件相关函数，感染文件
#define UNPE_WINNTABNORMSYSCALL		0x10		//利用NT Service Call直接调用系统内核代码
#define UNPE_WIN32BINDER			0x20		//释放文件并调用执行

//未知DOS
#define UNDOS_COM					0x1			//感染DOS可执行文件的COM病毒
#define UNDOS_EXE					0x2			//感染DOS可执行文件的EXE病毒
#define UNDOS_MEM					0x4			//DOS内存驻留型病毒

//未知引导病毒
#define UNBOOT_MBR					0x1			//感染硬盘主引导的病毒
#define UNBOOT_HBOOT				0x2			//感染硬盘分区引导的病毒
#define UNBOOT_FLOPPY				0x4			//感染软盘引导区的病毒


#endif // __RSENGINC_H__
