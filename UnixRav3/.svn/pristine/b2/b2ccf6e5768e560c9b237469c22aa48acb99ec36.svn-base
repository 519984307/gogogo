#ifndef __CONFIG_H__
#define __CONFIG_H__
#ifndef UNIXPLATFORM
    #include "stdafx.h"
#endif

#include "cfgdef2.h"
#include "extern.h"
#include "config.h"
#include "tree.h"
#include "checksn.h"

class CConfig : public IConfig
{
public:
        CConfig(const CHAR* pszFileName = NULL);
        ~CConfig();

        // 读配置项或配置结点(此时dwItemId为0)的信息
        RS_RESULT GetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, RS_BYTE* pbyBuffer, RS_DWORD& dwBufSize);
        // 写配置项或配置结点(此时dwItemId为0)的信息
        RS_RESULT SetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, const RS_BYTE* pbyBuffer, RS_DWORD dwBufSize);

        // 用这个对象本身打开一个配置项的孩子
        RS_RESULT GetChildNode(RS_DWORD dwId, RS_DWORD dwAccess, const RS_DWORD* pdwUserId);
        RS_RESULT GetChildNode(RS_DWORD dwId, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig);
        // 用这个对象本身打开一个配置项的兄弟
        RS_RESULT GetSiblingNode(RS_DWORD dwId, RS_DWORD dwAccess, const RS_DWORD* pdwUserId);
        RS_RESULT GetSiblingNode(RS_DWORD dwId, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig);

        RS_RESULT Create(PSTConfigItem pstItem, IConfig** ppConfig);							//创建一个配置项或一个配置结点
        RS_RESULT Delete(RS_DWORD dwId);														//删除一个配置项或一个配置结点
        RS_RESULT CreateItem(RS_DWORD dwItemId);												//创建一个配置项
        RS_RESULT DeleteItem(RS_DWORD dwItemId);												//删除一个配置项

        RS_RESULT GetItemValue(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD& dwSize);			// 取配置项值
        RS_RESULT GetItemValueBYTE(RS_DWORD dwItemId, RS_BYTE& byData);							// 取 BYTE 类型项值
        RS_RESULT GetItemValueWORD(RS_DWORD dwItemId, RS_WORD& wData);							// 取 WORD 类型项值
        RS_RESULT GetItemValueDWORD(RS_DWORD dwItemId, RS_DWORD& dwData);						// 取 RS_DWORD 类型项值
        RS_RESULT GetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize);	// 取 字符串 类型项值

        RS_RESULT SetItemValue(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD& dwSize);	// 写项值
        RS_RESULT SetItemValueBYTE(RS_DWORD dwItemId, RS_BYTE byData);							// 写 BYTE 类型项值
        RS_RESULT SetItemValueWORD(RS_DWORD dwItemId, RS_WORD wData);							// 写 WORD 类型项值
        RS_RESULT SetItemValueDWORD(RS_DWORD dwItemId, RS_DWORD dwData);						// 写 RS_DWORD 类型项值
        RS_RESULT SetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize);	// 写 字符串 类型项值

        // 按全路径打开或创建一个配置结点,ppConfig为RS_NULL时用此对象本身打开
        RS_RESULT CreateNode(const RS_DWORD* pdwIdPath, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig);
        RS_RESULT CloseNode();												// 关闭配置项
        RS_RESULT DeleteNode();												// 删除配置项
        RS_RESULT GetNodePath(RS_DWORD* pdwPath, RS_DWORD& dwPathLen);		// 取此配置结点的全路径
        RS_RESULT GetNodeAccess(RS_DWORD& dwAccess);						// 取权限
        RS_RESULT SetNodeAccess(RS_DWORD dwAccess);							// 设置权限
        RS_RESULT GetNodeStatus(RS_DWORD& dwStatus);						// 取状态
        RS_RESULT Lock(RS_DWORD dwId);										//锁定一个配置项或一个配置结点
        RS_RESULT Unlock(RS_DWORD dwId);									//解锁一个配置项或一个配置结点

        RS_RESULT LoadConfig(const RS_CHAR* pszFilename, IConfig** ppConfig = RS_NULL);			//从文件装入配置
        RS_RESULT LoadConfig(CTree * pTree=RS_NULL);
        RS_RESULT LoadConfigEx(const RS_CHAR* pszFilename, IConfig** ppConfig = RS_NULL, RS_DWORD dwLoadMode = LOAD_MODE_INIT);			//从文件装入配置
        RS_RESULT AddConfig(IConfig* pConfig);

        RS_RESULT Import(RS_CHAR *pszFileName);					//导入配置文件到此结点下
        RS_RESULT ImportToRoot(RS_CHAR *pszFileName);			//导入配置文件到根
        RS_RESULT Export(RS_CHAR *pszFileName);					//导出此结点到一个配置文件中
        RS_RESULT ExportAll(RS_CHAR *pszFileName);				//导出所有配置
        RS_RESULT Synchronize(RS_VOID *pParam = RS_NULL);

        //********************************************************************************//
        //以下方法供编辑器使用,均按照句柄访问
        HITEM GetRoot();
        HITEM GetSibling(HITEM hParent);
        HITEM GetChild(HITEM hParent);

        RS_RESULT Create(const RS_DWORD* pdwIdPath, PHITEM phItem, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId);
        RS_RESULT Close(PHITEM phItem);

        RS_RESULT GetInfo(HITEM hItem, RS_DWORD& dwId, RS_DWORD& dwLevel, RS_DWORD& dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo);
        RS_RESULT SetInfo(HITEM hItem, RS_DWORD dwId, RS_DWORD dwLevel, RS_DWORD dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo);
        RS_RESULT GetValue(HITEM hItem, RS_DWORD& dwDataType, RS_BYTE* pbyData, RS_DWORD& dwSize);
        RS_RESULT SetValue(HITEM hItem, RS_DWORD dwDataType, RS_BYTE* pbyData, RS_DWORD dwSize);
        RS_RESULT GetMemo(HITEM hItem, RS_CHAR* pszMemo);
        RS_RESULT SetMemo(HITEM hItem, RS_CHAR* pszMemo);
        RS_BOOL Save(HITEM hItem);
        RS_DWORD DeleteNode(HITEM hItem);
        RS_RESULT GetProductID(RS_CHAR *pszSN, RS_LONG &lPrtID);
        //********************************************************************************//
private:
        // 按全路径打开一个配置项
        RS_RESULT Create(const RS_DWORD* pdwIdPath, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId);
        RS_VOID ZeroData();
        //判断是否具有权限
        RS_RESULT HaveRights(RS_DWORD dwRights, RS_BOOL bLockCheck = RS_FALSE);
        //序列号验证
        RS_RESULT HookCheck(RS_DWORD dwItemId, RS_DWORD &dwDataType, RS_BYTE* pbyData, RS_DWORD &dwSize, RS_BOOL bIsWrite);
        RS_RESULT OPCheckSn(RS_DWORD dwItemId, RS_DWORD &dwDataType, RS_BYTE* pbyData, RS_DWORD &dwSize, RS_BOOL bIsWrite, RS_BOOL bIsInstall);
        //写序列号的详细信息
        RS_RESULT WriteSnInfo(RS_BOOL bIsInsall, CCheckSn *pCheckSn);
        //写安装相关的信息(第一次安装时间和第一次使用时间)
        RS_RESULT WriteRelatedInfo();
        //检验序列号的在配置中的限制
        RS_RESULT CheckSNDetail(RS_DWORD dwItemID, RS_CHAR *pszSrc, RS_DWORD dwValue);
        //检验序列号是否过期
        RS_BOOL IsSnExpired(RS_CHAR *pchSn, CCheckSn *pCheckSn=RS_NULL);
        //带有数据类型的写配置项值
        RS_RESULT SetItemValueEx(RS_DWORD dwItemId, RS_DWORD dwDataType,  RS_BYTE* pbyData, RS_DWORD& dwSize, RS_BOOL bHookCheck = RS_FALSE);
        RS_RESULT WriteInstallInfo(RS_LONG lProductID, RS_LONG lTranslatedPrtID, RS_LONG lEndTime);

        RS_RESULT DealwithHookExpired(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD &dwSize);


        RS_RESULT OPGetSNTime(RS_BYTE *pbyData, RS_BOOL bIsWrite);
        RS_RESULT OPExpireTime(RS_DWORD *pdwValue, RS_BOOL bIsWrite);
        RS_RESULT OPLeftTime(RS_DWORD *pdwValue, RS_BOOL bIsWrite);
        RS_RESULT OPAddTime(RS_DWORD dwValue, RS_BOOL bIsWrite);	//增加续费时间段
        RS_RESULT OPExpireFlag(RS_DWORD *pdwFlag, RS_BOOL bIsWrite);
        RS_RESULT OPInstallFlag(RS_DWORD &dwInstallFlag, RS_BOOL bIsWrite);	//检查产品是否已经安装
        RS_RESULT WriteProductItemDWORD(RS_DWORD dwItemID, RS_DWORD dwValue);
        RS_RESULT ReadProductItemDWORD(RS_DWORD dwItemID, RS_DWORD &dwValue);
        RS_RESULT ReadProductItemSTRING(RS_DWORD dwItemID, RS_CHAR *pszData, RS_DWORD &dwSize);
        RS_RESULT OPGetInstallPath(RS_CHAR *pszData, RS_DWORD &dwSize);
        RS_RESULT SetParentEnv(CConfig *pParent);

        //以下2个函数供内部使用
        RS_RESULT SuperGetValue(const RS_DWORD* pdwIdPath, RS_DWORD &dwDataType,  RS_BYTE* pbyData, RS_DWORD& dwSize);
        RS_RESULT SuperSetValue(const RS_DWORD* pdwIdPath, RS_DWORD dwDataType,  const RS_BYTE* pbyData, RS_DWORD& dwSize);
        RS_RESULT Query(RS_DWORD dwItemId);
        //********************************************************************************//
        //以下方法供编辑器使用,均按照句柄访问
        RS_RESULT OpenItem(RS_DWORD dwItemId, HITEM &hItem, RS_DWORD dwAccess);
        RS_RESULT CloseItem(PHITEM phItem);
        RS_RESULT GetConfigHeader(RS_DDWORD &dwHeaderAddr);
        RS_RESULT QueryUser(RS_DWORD *pUserID);
        //********************************************************************************//
private:
        CTree* m_tree;
        HITEM m_hItem;											//结点句柄
        RS_DWORD m_adwUser[RS_USERID_LEN];						//用户ID
        RS_DWORD m_dwAccess;									//打开的权限

        RS_DWORD m_adwPath[RS_MAX_CFGPATH_LEN];					//配置结点全路径
        RS_DWORD m_nPosition;									//最后一个元素的位置

        RS_BOOL m_bTreeLoaded;									//树是否装入了
        CConfig *m_pParent;										//父结点对象
};

#endif // __CONFIG_H__
