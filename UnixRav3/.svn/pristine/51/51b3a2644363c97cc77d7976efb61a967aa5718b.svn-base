#ifndef __RSCFGMGR_H__
#define __RSCFGMGR_H__

#include "RSCONFIG.H"

// 配置管理接口
class IRSCfgMgr : public IRSConfig
{
public:
	// 打开或创建一个相对或者绝对路径	参数: 访问 TOKEN
	// 打开时可能自动锁定一个项，其它访问者不可修改或访问
	virtual RS_RESULT CreateNode(const RS_DWORD* pdwIdPath, RS_DWORD& dwAccess, const RS_DWORD* pdwUserId, IRSConfig** p) = 0;
	// 本级可以保存临时配置节点
	virtual RS_RESULT SaveAsNode(RS_DWORD dwOldId, RS_DWORD dwNewId) = 0;
	// 关闭自己
	virtual RS_RESULT Close() = 0;
	// 取当前节点的路径
	virtual RS_RESULT GetPath(RS_DWORD* pdwPath, RS_DWORD& dwPathSize) = 0;
	// 装载配置，从一个配置文件加载配置，用于做映射
	virtual RS_RESULT LoadConfig(const char* pFileName, const RS_DWORD* pdwPath, RS_DWORD dwAccess, const RS_DWORD* pdwUserId) = 0;
	// 装载配置，从一个配置加载配置，用于做映射
	virtual RS_RESULT LoadConfig(IRSConfig* pCfg, const RS_DWORD* pdwPath, RS_DWORD dwAccess, const RS_DWORD* pdwUserId) = 0;
	// 卸载配置
	virtual RS_RESULT UnloadConfig(const RS_DWORD* pdwPath, const RS_DWORD* pdwUserId) = 0;

	// 将配置文件导入到这个配置对象中，如果具有权限，覆盖重复项而不提示
	virtual RS_RESULT Import(RS_CHAR* pszFileName) = 0;
	// 把这个配置对象导出成文件，如果具有权限，覆盖原有文件而不提示
	virtual RS_RESULT Export(RS_CHAR* pszFileName) = 0;
	// 锁定配置项，如果 dwId 等于 INVALID_ITEMID，则表示此对象结点本身
	virtual RS_RESULT Lock(RS_DWORD dwId) = 0;
	// 解锁配置项，如果 dwId 等于 INVALID_ITEMID，则表示此对象结点本身
	virtual RS_RESULT Unlock(RS_DWORD dwId) = 0;
};

#endif // __RSCFGMGR_H__
