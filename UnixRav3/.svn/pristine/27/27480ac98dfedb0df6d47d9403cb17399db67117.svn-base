#ifndef __RSSTORE_H__
#define __RSSTORE_H__

#include "RSCMNDEF.H"
#include "RSENGINE.H"
#include "RSIDS.h"

enum POLICY{REPLACE, SPREAD};

class IRSStoreObj : public IRSObject
{
public:
	enum
	{ RSID = RSID_RSStore, RSIID = RSIID_IRSStore,};
	
	////////////////////////////////////////////////////////////////////////////////
	// 函数名称： Store    (引擎调用)
	// 实现功能： 保存染毒内存文件对象
	// 对全局变量的影响： 无
	// 参数说明： 
	//		pFileObj			- 需备份的文件对象
	//		dwVirusID			- 病毒ID
	//      lpszFilePath        - 文件的物理路径（全路径）
	//		lpszStoreName		- 返回的备份文件名
	//		sStoreNameLen		- lpszStoreName缓冲区大小
	// 返回结果说明： 见错误码定义
	////////////////////////////////////////////////////////////////////////////////
	virtual RS_RESULT Store( IRSFileObject* pFileObj, RS_DWORD dwVirusID, RS_LPCSTR lpszFilePath, RS_LPSTR lpszStoreName, RS_SHORT sStoreNameLen) = 0;
	
	////////////////////////////////////////////////////////////////////////////////
	// 函数名称： Restore  (引擎调用)
	// 实现功能： 将染毒文件恢复到pFileObj指定的文件对象
	// 对全局变量的影响： 无
	// 参数说明： 
	//		lpszRecordName		- 需恢复文件名
	//		pFileObj			- 文件对象
	// 返回结果说明： 见错误码定义
	////////////////////////////////////////////////////////////////////////////////
	virtual RS_RESULT Restore( RS_LPCSTR lpszRecordName, IRSFileObject* pFileObj) = 0;

	////////////////////////////////////////////////////////////////////////////////
	// 函数名称： Delete
	// 实现功能： 
	// 对全局变量的影响： 无
	// 参数说明： 
	//		lpszRecordName		- 需删除的文件名
	// 返回结果说明： 见错误码定义
	////////////////////////////////////////////////////////////////////////////////
	virtual RS_RESULT Delete(LPCTSTR lpRecordName) = 0;

	////////////////////////////////////////////////////////////////////////////////
	// 函数名称： Restore(RavStore程序调用)
	// 实现功能： 将染毒文件恢复
	// 对全局变量的影响： 无
	// 参数说明： 
	//		lpszRecordName		- 需恢复文件名
	//		lpszRestoreFileName - 指定位置（NULL则恢复到原位置）
	//      bIsUnicode			- lpszRestoreFileName是否为UNICODE
	// 返回结果说明： 见错误码定义
	////////////////////////////////////////////////////////////////////////////////
	virtual HRESULT Restore( LPCSTR lpszRecordName, LPCSTR lpszRestoreFileName ) = 0;
	
	virtual BOOL IsEnoughSpace( DWORD dwFileSize ) = 0;
	virtual ULONGLONG GetBinFreeSpace( VOID ) = 0;
	virtual ULONGLONG GetBinDiskFreeSpace() = 0;
	virtual POLICY	GetReplacePolicy(void) = 0;
	virtual SHORT	SetReplacePolicy(POLICY Policy) = 0;
	virtual SHORT	SetReservedSpace(ULONGLONG ullSize) = 0;
	//取回收站容量
	virtual ULONGLONG	GetBinSpace(void) = 0;
	virtual SHORT	SetBinSpace(ULONGLONG ullSize) = 0;
	virtual ULONGLONG	GetReservedSpace(void) = 0;
	virtual BOOL	GetBinPath(LPSTR lpBin) = 0;
	
	virtual SHORT	Empty() = 0;
	virtual SHORT	GetFileRecord(LPCTSTR lpRecordName, void* lpRecord/*[out]*/, WORD wRecordSize) = 0;
	virtual SHORT GetFileRecordMarker( LPCSTR lpszRecordName, WORD& wHeaderMarker ) = 0;
};

//错误定义
//#define	STORE_ERROR_SUCCESS			 0
//#define	STORE_ERROR_NOFIXDISK		-1
//#define	STORE_ERROR_NOSOURCEFILE	-2		// 原始染毒文件无法找到
//#define	STORE_ERROR_NOENOUGHBUF		-5		// 所给缓冲区太小，不够容纳
//#define	STORE_ERROR_CREATEFILE		-10		// 创建文件失败
//#define	STORE_ERROR_OPENFILE		-11		// 打开文件失败
//#define	STORE_ERROR_READFILE		-12		// 读文件失败
//#define	STORE_ERROR_WRITEFILE		-13		// 写文件失败
//#define	STORE_ERROR_RELEASEFILE		-14		// 释放文件对象失败（关闭文件失败）
//#define	STORE_ERROR_CHANGEATTRIB	-15		// 改变文件属性失败
//#define	STORE_ERROR_DELFILE			-16		// 删除文件失败
//#define	STORE_ERROR_CREATEBIN		-20		// 创建目录失败（已确认目录不存在，再创建）
//#define	STORE_ERROR_BUILDFILENAME	-50		// 回收站中文件个数大于0xFFFFF
//#define	STORE_ERROR_EMPTYBIN		-51		// 清空回收站错误
//#define	STORE_ERROR_NOENOUGHSPACE	-60		// 回收站没有足够的空间回收文件

// 错误定义
enum StoreErr
{
	STORE_ERROR_SUCCESS			= 0,
	STORE_ERROR_NOFIXDISK		= -1,
	STORE_ERROR_NOSOURCEFILE    = -2,		// 原始染毒文件无法找到
	STORE_ERROR_NOENOUGHBUF		= -5,		// 所给缓冲区太小，不够容纳
	STORE_ERROR_CREATEFILE		= -10,		// 创建文件失败
	STORE_ERROR_OPENFILE		= -11,		// 打开文件失败
	STORE_ERROR_READFILE		= -12,		// 读文件失败
	STORE_ERROR_WRITEFILE		= -13,		// 写文件失败
	STORE_ERROR_RELEASEFILE		= -14,		// 释放文件对象失败（关闭文件失败）
	STORE_ERROR_CHANGEATTRIB	= -15,		// 改变文件属性失败
	STORE_ERROR_DELFILE			= -16,		// 删除文件失败
	STORE_ERROR_CREATEBIN		= -20,		// 创建目录失败（已确认目录不存在，再创建）
	STORE_ERROR_BUILDFILENAME	= -50,		// 回收站中文件个数大于0xFFFFF
	STORE_ERROR_EMPTYBIN		= -51,		// 清空回收站错误
	STORE_ERROR_NOENOUGHSPACE	= -60,		// 回收站没有足够的空间回收文件
	STORE_ERROR_FILEFORMAT  	= -70,		// 文件格式错误
};
#endif  //	__RSSTORE_H__