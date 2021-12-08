#ifndef __RSIFACE_H__
#define __RSIFACE_H__

#include "RSTYPE.H"

//=============基本接口定义==============

class IRSObject;
class IRSEnv;
class IRSConfig;

// 运行时的对象引用管理，只是对象管理器使用
class IRSRefMgr
{
public:
	// 增加引用对象
	virtual void AddRef(IRSObject* pObj) = 0;
	// 释放引用对象
	virtual void RemoveRef(IRSObject* pObj) = 0;
};

// 增加引用计数和释放引用计数没有多大意义，因为对象需要做集中管理。
// 而且，如果用类厂，则会涉及到全局变量，会有多线程问题。
class IRSObject
{
public:
	IRSObject() : m_pMgr(RS_NULL) {}

	// 取类型
	virtual RS_DWORD GetType() = 0;
	// 取接口
	virtual RS_RESULT QueryInterface(RS_DWORD dwIID, void** ppv) = 0;
	// 释放接口
	virtual void Release() = 0;

public:
	// 运行信息, 对象管理器用, 不可它用
	IRSRefMgr* m_pMgr;
};

// 对象管理器, 如果需要支持对象的动态更新, 扩展接口
class IRSObjMgr : public IRSObject
{
public:
	// 创建对象
	virtual RS_RESULT CreateObject(RS_DWORD dwObjId, IRSEnv* pEnv, IRSObject** ppObj) = 0;
	// 释放不用的模块, 为了节约内存
	virtual RS_RESULT ReleaseUnusedModule() = 0;
};

// 运行环境，包括应用程序的运行环境和功能块的运行环境
// 运行环境的功能：
//		提供已生成环境功能对象，提供当前功能的配置，提供子功能的环境，
//		对象管理功能环境功能对象和配置是最可能需要变化的
class IRSEnv : public IRSObjMgr
{
public:
	struct StockObjItem
	{
		RS_DWORD dwObjId;
		IRSObject* pObj;
	};
	typedef StockObjItem* StockObjItems;

	// 取子功能环境, 取出的环境是一个独立的环境，与当前环境没有内存上的关系。
	// pObjItems: 如果 dwObjId 为非空，但 pObj 为空，则取消此对象 ID
	virtual IRSEnv* GetSubEnv(RS_DWORD dwFunId, IRSEnv::StockObjItems pObjItems) = 0;
	// 取配置, 对于应用程序而言，包括序列号、升级 ID、产品配置等
	virtual IRSConfig* GetConfig() = 0;
	// 取环境中的已生成的对象
	virtual RS_RESULT GetStockObject(RS_DWORD dwObjId, void** ppv) = 0;
	// 设置环境中的已生成的对象
	virtual RS_RESULT SetStockObjects(StockObjItems pItems) = 0;
	// 取产品信息, 包括过期时间, 系统中心数/服务器数/客户端数等动态信息
	virtual RS_RESULT GetProductInfo(RS_DWORD dwInfoId, void* pBuf, RS_DWORD dwBufSize) = 0;
};

// 应用程序管理接口
class IRSAppMgr : public IRSObject
{
public:
	virtual RS_RESULT Initialize(void* pvParam) = 0;
	virtual RS_RESULT CreateAppEnv(IRSEnv** ppv, RS_DWORD dwAppId, RS_DWORD dwParam, RS_DWORD* pdwToken) = 0;
};

// 所有模块都使用这个基类
class IRSModule
{
public:
	struct ObjItem
	{
		typedef RS_RESULT (RS_STDCALL* fnCreateObject)(void**, IRSEnv*);

		RS_RESULT m_dwObjId;
		fnCreateObject m_pFun;
	};
	typedef ObjItem* ObjItems;

	// 取模块 ID
	virtual RS_DWORD GetId() const = 0;
	// 取模块版本
	virtual RS_DWORD GetVersion() const = 0;
	// 取模块对象表
	virtual ObjItems GetObjItems() const = 0;
	// 生成对象，包括模块的对外接口对象，这样可以将一个模块拆成几个逻辑模块。
	// 有利于模块自由拆分与组合。
	virtual RS_RESULT CreateObject(RS_DWORD dwObjId, IRSEnv* pEnv, void** ppv) const = 0;
};

class IRSDbgInfoLog : public IRSObject
{
public:
	// dwInfoGrp 低 BYTE 为信息级别, 0 表示调试信息, 数值大表示是严重错误
	virtual void OutputDW(RS_DWORD dwInfoGrp, RS_DWORD dwInfo) = 0;
	virtual void OutputBIN(RS_DWORD dwInfoGrp, void* pInfo, RS_DWORD dwLen) = 0;
	virtual void OutputSTR(RS_DWORD dwInfoGrp, const RS_CHAR* pszFormat, ...) = 0;
};

enum
{
	ERR_QI_TYPEERR			= 1,	// 不支持的接口类型
	ERR_QI_IIDERR,					// 不支持的接口
	ERR_OBJ_NOMEMMGR,

	ERR_OBJMGR_INVALIDOBJID	= 1,
	ERR_OBJMGR_MODLDRERR,
};

#endif // __RSIFACE_H__
