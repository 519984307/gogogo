#ifndef __RSCONFIG_H__
#define __RSCONFIG_H__

#include "RSTYPE.H"
#include "RSIFACE.H"
#include "RSCFGDEF.H"

// 配置接口，配置管理是另外的接口，以防程序对配置系统进行管理
class IRSConfig : public IRSObject
{
public:
	// 生成配置项
	virtual RS_RESULT CreateItem(RS_DWORD dwItemId) = 0;
	// 生成或打开子节点
	virtual RS_RESULT CreateSubNode(RS_DWORD dwId, RS_DWORD& dwAccess, const RS_DWORD* pdwUserId, IRSConfig** p) = 0;
	// 可以删除一个节点，或一项
	virtual RS_RESULT Delete(RS_DWORD dwId) = 0;
	// 取项内容
	virtual RS_RESULT GetItemData(RS_DWORD dwItemId, void* pBuf, RS_DWORD& dwSize) = 0;
	virtual RS_RESULT GetItemBYTE(RS_DWORD dwItemId, RS_BYTE& Item) = 0;
	virtual RS_RESULT GetItemWORD(RS_DWORD dwItemId, RS_WORD& Item) = 0;
	virtual RS_RESULT GetItemDWORD(RS_DWORD dwItemId, RS_DWORD& Item) = 0;
	virtual RS_RESULT GetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize) = 0;
	// 置项内容
	virtual RS_RESULT SetItemData(RS_DWORD dwItemId, const void* pBuf, RS_DWORD dwSize) = 0;
	virtual RS_RESULT SetItemBYTE(RS_DWORD dwItemId, RS_BYTE& Item) = 0;
	virtual RS_RESULT SetItemWORD(RS_DWORD dwItemId, RS_WORD& Item) = 0;
	virtual RS_RESULT SetItemDWORD(RS_DWORD dwItemId, RS_DWORD& Item) = 0;
	virtual RS_RESULT SetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize) = 0;

	// 取信息
	virtual RS_RESULT GetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, void* pBuf, RS_DWORD& dwBufSize) = 0;
	// 置信息
	virtual RS_RESULT SetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, const void* pBuf, RS_DWORD dwBufSize) = 0;
	// 数据同步
	virtual RS_RESULT Synchronize() = 0;

public:
	// 应用程序配置 ID 定义, 产品的过期时间/用户数等是动态信息
	enum
	{
		PRO_CFG_PROINFO         = 1,	// 产品的动态信息,包括过期时间,系统中心数/服务器数/客户端数等,
		PRO_CFG_INSTALLER,				// 安装/升级/卸载信息, 包括语言包信息
		PRO_CFG_RAV,					// 主程序, 包括标题等
		PRO_CFG_INFOCON,				// 产品的运行信息
		PRO_CFG_RAVMOND,				// 实时监控的后台程序
		PRO_CFG_RAVMON,					// 实时监控SHELL程序
		PRO_CFG_CALLCENTER,				// 通讯中心
		PRO_CFG_VSCANNER,				// 漏洞扫描
		PRO_CFG_MAKEDISK,				// 维护工具
		PRO_CFG_LOGMGR,					// 日志管理
		PRO_CFG_PANDORA,				// 病毒隔离系统
		PRO_CFG_HDBACKUP,				// 硬盘备份系统
		PRO_CFG_REGWIZARD,				// 注册向导
		PRO_CFG_SMS,					// 短信通
		PRO_CFG_PERSONALFW,				// 个人防火墙
		PRO_CFG_VPNCLIENT,				// VPN 客户端
	};
};

//=======================错误码定义=======================
#define	INVALID_ITEMID		0x00000000

#endif // __RSCONFIG_H__
