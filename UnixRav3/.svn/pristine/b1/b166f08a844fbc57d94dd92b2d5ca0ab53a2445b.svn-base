#ifndef __RSENGCLI_H__
#define __RSENGCLI_H__

#include "RSENGINE.H"	

#pragma pack(push, 8)

struct STHostName
{
	RS_WORD dwType;
	RS_BOOL bIsUnicode;
	RS_DWORD dwLen;
	union
	{
		void* pData;
		char* pAName;
		wchar_t* pWName;
	};
};

enum CLIENTCMD 
{
	CLIENTCMD_CONTINUE	=USERCMD_CONTINUE,		//继续查杀毒流程
	CLIENTCMD_KILL		=USERCMD_KILL,			//杀毒
	CLIENTCMD_DELFILE	=USERCMD_DELFILE,		//删除文件
	CLIENTCMD_RENAME	=USER_RENAME,			//文件重命名
	CLIENTCMD_IGNORE	=USERCMD_IGNORE,		//忽略病毒，继续查杀	
	CLIENTCMD_STOPSCANOBJ=USERCMD_STOPSCANOBJ,	//停止查此对象
	CLIENTCMD_STOPALL	=USERCMD_STOPALL,		//停止查毒直接退出
	CLIENTCMD_PAUSE,							//暂停	
	CLIENTCMD_RETRYBACKUP,						//备份失败时重试
};

enum MAIL_TYPE
{
	MAIL_TYPE_NOTMAIL=0, MAIL_TYPE_OL97, MAIL_TYPE_OL2K, MAIL_TYPE_OE4,
	MAIL_TYPE_OE5, MAIL_TYPE_FOXMAIL, MAIL_TYPE_NETSCAP,
};

enum MAIL_BOX_TYPE
{
	MAIL_BOX_BY_FILENAME=0,		//使用szBoxFileName字符串表示
	MAIL_BOX_DRAFT,				//草稿箱
	MAIL_BOX_IN,				//收件箱
	MAIL_BOX_OUT,				//发件箱
	MAIL_BOX_SENT,				//已发送
	MAIL_BOX_DELETE,			//垃圾箱
};

enum MAIL_ATTACH_TYPE
{
	MAIL_ATTACH_BY_NAME=0,		//使用szAttachName字符串表示
	MAIL_ATTACH_BODY,			//邮件体
	MAIL_ATTACH_HTML,			//HTML正文
};

typedef struct MAIL_INFO
{
	RS_INT		iMailType;			//邮件种类 MAIL_TYPE

	RS_INT		iBoxType;			//邮箱种类MAIL_BOX_TYPE
	RS_LPCSTR	szBoxFileName;		//邮箱文件名，仅在iBoxType==MAIL_BOX_BY_FILENAME时有效

	RS_LPCSTR	szSubject;			//邮件标题
	RS_LPCSTR	szSender;			//发送者
	RS_LPCSTR	szRecever;			//接收者
	
	RS_INT		iAttachType;		//附件类型
	RS_LPCSTR	szAttachName;		//附件名，仅在iAttachType==MAIL_ATTACH_BY_NAME时有效
}*PMAIL_INFO;

typedef struct GENERAL_INFO
{
	RS_DWORD	nType;				//类型 IRSHostTree::HT_FILEDIR,HT_SYSMAIL,HT_SYSMEMORY,HT_BOOTSECT		
	RS_LPCSTR	szDir;				//目录名 c:\program files\rising\rav
	RS_LPCSTR	szFile;				//文件和子文件  aaa.zip>>bbb.exe 
	IRSObject*	pObj;				//附合文件对象
	RS_LPVOID	pSpecialInfo;		//特殊信息，目前邮件为 PMAIL_INFO
} *PGENERAL_INFO;

typedef struct PREDISPLAY_INFO : public GENERAL_INFO
{
	RS_DWORD nScanCount;			//本次查杀当前文件数(用户重新调用Scan后会清零)
} *PPREDISPLAY_INFO;

typedef struct REPORTVIRUS_INFO : public GENERAL_INFO
{
	STVIRUSINFO* pVirusInfo;		//病毒信息
} *PREPORTVIRUS_INFO;

#define KILLRESULT_SCAN_ONLY ((WORD)(-1)) //查毒版pVirusInfo->wKillResult值
typedef struct KILLRESULT_INFO : public GENERAL_INFO
{
	STVIRUSINFO* pVirusInfo;		//病毒信息
} *PKILLRESULT_INFO;

typedef struct BACKUP_INFO : public GENERAL_INFO
{
	STVIRUSINFO* pVirusInfo;		//病毒信息
	RS_BOOL		bSuccess;			//备份结果
} *PBACKUP_INFO;



class IRSClientCallBack
{
public:

	//预显示回调
	virtual RS_DWORD PreDisplay(PPREDISPLAY_INFO pInfo)=0; 

	//发现病毒回调函数
	virtual RS_DWORD ReportVirus(PREPORTVIRUS_INFO pInfo)=0;

	//杀毒结果显示回调函数
	virtual RS_DWORD ReportKillResult(PKILLRESULT_INFO pInfo)=0;

	//文件备份显示回调函数
	virtual RS_DWORD ReportBackup(PBACKUP_INFO pInfo)=0;

	//查询最后一个客户端命令
	virtual RS_DWORD QueryLastClientCmd()=0;
};


class IRSHostTree : public IRSProperty
{
public:
	//取下一个子节点
	virtual IRSHostTree* GetNextSubItem(BOOL bDirOnly)=0;
	//开始遍历
	virtual BOOL BeginEnum()=0;
	//结束遍历
	virtual BOOL EndEnum()=0;


	//取当前节点的名字
	virtual STHostName* GetName()=0;
	
	enum
	{
		//宿主类型
		HT_ROOT=0,				//根节点
		HT_FILEDIR,				//本地文件系统
		HT_DRIVER,				//包括软盘，硬盘，光驱等，具体由使用者区分。
		HT_NETNEIGHBOR,			//网上邻居
		HT_SYSMEMORY,			//系统内存
		HT_BOOTSECT,			//引导区
		HT_SYSMAIL,				//系统邮件
		HT_SYSVULNERA,			//系统漏洞

		//文件属性
		FA_FOLDER=1,			//是否为目录
	};

	//取节点类型：见宿主类型定义
	virtual RS_DWORD GetType()=0;
	
	//是否为目录
	virtual RS_DWORD GetAttribute()=0;
	
	//释放实例
	virtual RS_RESULT Release()=0;
};



class IRSScanner: public IRSObject
{
public:
	enum
	{ RSID = RSID_RSScanner, RSIID = RSIID_IRSScanner,	};

	enum
	{
		//SCANTREE标志
		SF_SCAN=1,	//扫描文件
		SF_COUNT,	//只做文件计数(如果配置中指定扩展名过滤，则只统计匹配的文件)
	};


	//查杀前加载引擎
	virtual BOOL PreScan(IRSEnv *pEnv)=0;
	//查杀后释放引擎
	virtual BOOL PostScan()=0;

	//设置回调对象
	virtual BOOL ClientSetCallBack(IRSClientCallBack* pCallBack)=0;

	//扫描枚举出的对象, 可以不做扫描而只做文件计数，文件计数的策略与扫描策略相同
	virtual RS_RESULT ScanTree(IRSHostTree* pTree, RS_DWORD dwFlags)=0;
	
	//按名字扫描
	virtual RS_RESULT ScanHost(STHostName* pHostName, RS_DWORD dwFlags = IRSScanner::SF_SCAN)=0;
	
	//按对象扫描
	virtual RS_RESULT ScanHostObject(IRSObject *pObject)=0;

	//取扫描，枚举对象通过IRSHostTree的接口完成，如果pHost为空，则表示取指根
	virtual IRSHostTree* GetHostRoot(STHostName *pHost)=0;

	//停止所有处理
	virtual RS_RESULT StopAll()=0;
};

#pragma pack(pop)

#endif	//	__RSENGCLI_H__