#ifndef __RSENGINE_H__
#define __RSENGINE_H__

#include "RSENGCMN.H"

//病毒信息(2005应用组)
struct STRsVirusInfo
{
	RS_DWORD			dwVirusID;			//病毒记录的ID
	RS_BYTE				byVirusType;		//病毒类型
	RS_CHAR				szVirusName[64];	//病毒名
	RS_CHAR				szAliasName[32];	//病毒别名
};

class IRsVirusInfoTable
{
public:
	//根据病毒ID取病毒信息
	virtual RS_BOOL	GetVirusInfo(
		RS_DWORD dwVirusID,
		STRsVirusInfo* pstRsVirusBaseInfo) = 0 ;

	//根据病毒名和病毒类型取病毒信息
	virtual RS_BOOL	GetVirusInfo(
		RS_LPSTR lpszVirusName,
		RS_BYTE byVirusType,
		STRsVirusInfo* pstRsVirusBaseInfo) = 0 ;

	//根据病毒名取病毒ID，返回的ID填在pdwVirusArray里面，
	//dwIDCount是出入参。
	//当dwIDCount=0时，函数返回成功，dwIDCount为符合病毒名条件的个数
	virtual RS_BOOL	GetVirusID(
		RS_LPSTR lpszVirusName,
		RS_DWORD* pdwVirusArray,
		RS_DWORD& dwIDCount) = 0 ;

	//取最大病毒ID
	virtual RS_DWORD GetMaxVirusID() = 0 ;
};

class ILibLoader;

class ILibTool : public IRSObject
{
public:
	virtual RS_INT	MakePatch( 
		ILibLoader *pcOldLib, 
		ILibLoader *pcNewLib, 
		RS_CHAR *pszPatchFile, 
		RS_DWORD dwOption ) = 0;

	virtual RS_INT	UpdatePatch( 
		ILibLoader *pcOldPatch, 
		ILibLoader *pcNewPatch, 
		RS_CHAR *pszPatchFile, 
		RS_DWORD dwOption ) = 0;

	virtual RS_INT	PatchLib( 
		ILibLoader *pcOldLib, 
		ILibLoader *pcPatchLib, 
		RS_CHAR *pszLibDir, 
		RS_DWORD dwOption ) = 0;
};

//SEEK起始位置	
enum SeekPosition 
{ 
	SPBegin		= 0x0,		//从文件头
	SPCurrent	= 0x1,		//从当前位置
	SPEnd		= 0x2		//从文件尾
};

//文件的当前状态
enum FileStatus
{
	FSFree		= 0,		//空闲
	FSReadOpen	= 0x0001,	//只读打开
	FSWriteOpen = 0x0002,	//写模式打开
	FSClosed	= 0x0004,	//文件被关闭
	FSChanged	= 0x0100,	//文件被修改
	FSDeleted	= 0x0200,	//文件被删除
};

//文件的打开方式
enum OpenMode
{
	OMOpen		= 0x0001,
	OMCreate	= 0x0002,
	OMRead		= 0x0100,
	OMWrite		= 0x0200,
	OMAppend	= 0x0400,
	OMAutoDel	= 0x1000,
	
	//r		Opens for reading 
	//		If the file does not exist or cannot be found, the fopen call fails.
	OMR	= OMOpen | OMRead,	
	//w		Opens an empty file for writing 
	//		If the given file exists, its contents are destroyed.
	OMW	= OMCreate | OMWrite,			
	//a		Opens for writing at the end of the file
	//		creates the file first if it doesn’t exist.
	OMA	= OMOpen | OMCreate | OMAppend,		
	//r+	Opens for both reading and writing 
	//		(The file must exist.)
	OMRP = OMOpen | OMRead | OMWrite,		
	//w+	Opens an empty file for both reading and writing 
	//		If the given file exists, its contents are destroyed.
	OMWP = OMCreate | OMRead | OMWrite,				
	//a+	Opens for reading and appending
	//		creates the file first if it doesn’t exist
	OMAP = OMOpen | OMCreate | OMRead | OMAppend,	
};

class IRSArchive;

//基本文件对象，需要引擎组完善
//ReOpen时要备份，SetAttribute时要修改备份，Close时修改所有备份
class IRSFileObject : public IRSObject, public IRSProperty
{
public:
	//打开文件，文件对象创建时是没有打开文件的。
	virtual RS_RESULT Open(const STFileName &file, RS_DWORD dwFlags)=0;
	//重新打开，文件对象开始是只读的，杀毒时改为可写，返回错误码
	virtual RS_RESULT ReOpen(RS_DWORD dwFlags)=0;
	virtual RS_RESULT Close()=0;

	virtual RS_RESULT Read(void *pBuf, RS_DWORD dwBufSize, RS_DWORD &dwRead)=0;
	virtual RS_RESULT Write(const void *pBuf, RS_DWORD dwBufSize, RS_DWORD &dwWritten)=0;
	virtual RS_RESULT  Flush()=0;
	
	virtual RS_RESULT Seek(RS_LONG lOff, RS_UINT nFrom = SPBegin)=0;
	virtual RS_RESULT Tell(RS_DWORD &dwOff)=0;
//	virtual RS_RESULT IsEOF()=0;

	virtual RS_RESULT  Remove()=0;

	//我们只关心我们定义的属性，其它不管
	virtual RS_RESULT GetAttributes(RS_DWORD &dwAttr) const =0;
	virtual RS_RESULT SetAttributes(RS_DWORD dwAttr) = 0;
	
	//逻辑路径全路径由使用者自行构造
	//GetName只取本级逻辑名称，对于物理文件，它就是文件全路径
	virtual const STFileName&  GetName() const =0;
	virtual RS_RESULT  Rename(const STFileName&)=0;
	
	virtual RS_RESULT GetSize(RS_DWORD &fileSize) =0;
	virtual RS_RESULT SetSize(RS_DWORD fileSize)=0;

	virtual RS_DWORD GetStatus() const =0;

	//强制要求实现，不放在属性里面！
	//保持维护链式关系，用于上层程序的文件名显示
	//GetOwner返回文件对象的属主
	virtual IRSArchive* GetOwner() = 0;
	//SetOwner设置文件对象的属主
	virtual RS_RESULT SetOwner(IRSArchive* pArchive) = 0;

	//TODO:暂不支持64位操作，返回值需要改动
	virtual RS_RESULT ReadAt64(RS_INT64 off32, void *pBuf, RS_DWORD &RdLen) { return 0; };
	virtual RS_RESULT Seek64(RS_INT64 dwOff, RS_DWORD nFrom) { return 0; };
	virtual RS_RESULT Tell64(RS_INT64 &dwOff) { return 0; };
	virtual RS_RESULT WriteAt64(RS_INT64 off32, void *pBuf, RS_DWORD &RdLen) { return 0; };
	virtual RS_RESULT GetFileSize64(RS_INT64 &fileSize) { return 0; };
	virtual RS_RESULT SetFileSize64(RS_INT64 &fileSize) { return 0; };

public:
	//引擎组2004代码中用的一些接口
	virtual RS_DWORD Read(RS_VOID *pBuf, 
		RS_DWORD dwBufSize)
	{
		RS_DWORD dwRead;
		return ((Read(pBuf, dwBufSize, dwRead) == RS_OK) ? dwRead : 0);		//TODO:
	}
	virtual RS_DWORD Write(const void *pBuf, RS_DWORD dwBufSize)
	{
		RS_DWORD dwWritten;
		return ((Write(pBuf, dwBufSize, dwWritten) == RS_OK) ? dwWritten : 0);		//TODO:
	}
	virtual RS_DWORD ReadPos(void *pBuf, RS_LONG lOff, RS_DWORD dwLen)
	{
		if(Seek(lOff) != RS_OK)
			return 0;
		return Read(pBuf, dwLen);
	}
	virtual RS_DWORD WritePos(void *pBuf, RS_LONG lOff, RS_DWORD dwLen)
	{
		if(Seek(lOff) != RS_OK)
			return 0;
		return Write(pBuf, dwLen);
	}
	virtual RS_DWORD GetSize()
	{ 
		RS_DWORD dwFileSize; 
		return (GetSize(dwFileSize) == RS_OK) ? dwFileSize : (RS_DWORD)-1 ; 
	}
	virtual RS_BOOL IsEOF()
	{
		RS_DWORD dwOff;
		if(Tell(dwOff) == RS_OK)
			if(GetSize() == dwOff)
				return RS_TRUE;
		return RS_FALSE;
	}
};

///档案文件对象，是指有格式的文件，包括引擎文件对象和复合文件对象
class IRSArchive : public IRSObject, public IRSProperty
{
public:
	//传递系统环境对象以进行初始化
	//引擎组内部调试版本使用，正式版本强制在构造函数中传递，不用此函数！
	//这个接口改成公用接口
	virtual RS_RESULT Initialize(IRSEnv* pEnv) = 0;

	//对象创建时可能是空的，因此需要打开操作
	virtual RS_RESULT Open(IRSFileObject *pTarget) = 0;  
	virtual RS_RESULT Open(IRSArchive *pArchive) = 0;  
	virtual RS_RESULT Close() = 0;
	virtual RS_DWORD GetArchiveType() = 0;
	virtual RS_RESULT GetFileObject(IRSObject **pTarget) = 0;
};

//引擎文件对象
class IRSEngFileObj : public IRSArchive
{
public:
	//重置引擎文件对象，多个引擎要查毒，需要重置引擎文件对象
	virtual RS_RESULT Reset()=0;
	enum{
		//定义属性
	};
};

//复合文件对象，由引擎组负责定义
class IRSCompFileObj : public IRSArchive
{
public:
	virtual RS_RESULT ExtractItem(
					const RS_UINT index, 
					IRSFileObject **stream) = 0;

	virtual RS_RESULT ExtractItem(
					  const RS_UINT index, 
					  const RS_UINT from, 
					  OUT RS_BYTE *buf,
					  const RS_UINT len, 
					  OUT RS_UINT *retlen
					  ) = 0;

	virtual RS_RESULT DeleteItem(const RS_UINT index) = 0;

	virtual RS_RESULT InsertItem(IRSFileObject *stream) = 0;

	virtual RS_RESULT UpdateItem(const RS_UINT index, IRSFileObject *stream) = 0;

	//  virtual BOOL GetNumberOfProperties(RS_UINT *numProperties);  
	//  virtual GetProperty(UINT32 index, RSPROPID propID, PROPVARIANT *value);
	virtual RS_RESULT GetItemProperty(
					  RS_UINT index, 
					  RS_DWORD propID, 
					  OUT RS_VOID *pValue, 
					  INOUT RS_DWORD *pdwLen) = 0;

	virtual RS_RESULT SetItemProperty(
					  RS_UINT index, 
					  RS_DWORD propID, 
					  RS_VOID *pValue, 
					  RS_DWORD dwLen) = 0;

	virtual RS_RESULT BuildAll() = 0;

//	virtual RS_RESULT GetFileObject(IRSFileObject**ppv)=0;
//	virtual RS_RESULT ExtractItem(const RS_UINT32 index, OUT IRSFileObject **stream)=0;
//	virtual RS_RESULT DeleteItem(const RS_UINT32 index)=0;
//	virtual RS_RESULT InsertItem(IRSFileObject *stream)=0;
//	virtual RS_RESULT UpdateItem(const RS_UINT32 index, IRSFileObject *stream)=0;
	enum{
		//错误代码
	};
	enum{
		//属性定义
	};
};


//这是内存中的结构，并不代表病毒库中的结构，病毒中的结构应是确定字节序的，而且字段大小可能小于内存中的。
struct STVIRUSINFO
{
	RS_DWORD	dwVirusID;		//病毒ID, 未知病毒的ID为一确定值，比如为-1
	RS_WORD		wType;			//病毒类型
	RS_WORD		wKillResult;	//
};

//漏洞信息
struct STVulnInfo
{
	//环境类型(操作系统，应用程序，语言)
	RS_DWORD	dwEnv;
	//漏洞标识
	RS_WORD		wVulID;
	RS_WORD		wResult;
};

enum USERCMD 
{
	USERCMD_CONTINUE=0,					//继续查杀毒流程
	USERCMD_KILL,						//杀毒
	USERCMD_DELFILE,					//删除文件
	USER_RENAME,						//文件重命名
	USERCMD_IGNORE,						//忽略病毒，继续查杀	
	USERCMD_STOPSCANOBJ=16,				//停止查此对象
	USERCMD_STOPALL,					//停止查毒直接退出
};

//dwScanResult：杀毒结果值定义
enum KILLRESULT
{
		KILLRESULT_NOTFOUND=0,			//未发现病毒
		KILLRESULT_FOUND=1,				//发现病毒
		KILLRESULT_IGNORE,				//用户忽略
		KILLRESULT_KILLED,				//杀毒成功	
		KILLRESULT_KILL_FAIL,			//杀毒失败	
		KILLRESULT_DELETED,				//文件被删除
		KILLRESULT_DELETE_FAIL,			//删除文件失败
		KILLRESULT_RENAMED,				//文件改名成功
		KILLRESULT_RENAME_FAIL,			//文件改名失败
		KILLRESULT_ZIP_ENCRYPT,			//压缩文件被加密
		KILLRESULT_DOC_ENCRYPT,			//Office文件被加密
		KILLRESULT_AFTER_UNZIP,			//解压后才能杀毒
		KILLRESULT_DEAD_LOOP			//杀毒死循环
};

class IRSScanCallBack : public IRSObject
{
public:
	enum{RSID=1,RSIID=1};
	//显示文件名函数
	virtual RS_DWORD PreDisplay(IRSObject *pObject)=0; 
	//发现病毒回调函数
	virtual RS_DWORD ReportVirus(IRSObject *pObject, STVIRUSINFO *pVirusInfo)=0;
	//杀毒结果显示回调函数
	virtual RS_DWORD ReportKillResult(IRSObject* pObject, STVIRUSINFO *pVirusInfo)=0;
};

//基本引擎，引擎对外接供的接口
class IRSEngine : public IRSObject
{
public:
	virtual RS_RESULT Initialize(IRSEnv* pEnv)=0;
	virtual RS_RESULT Scan(IRSObject *pHostObject)=0;
//	virtual RS_RESULT Kill(IRSObject *pHostObject)=0;
	virtual RS_RESULT Reset()=0;
};

#endif // __RSENGINE_H__
