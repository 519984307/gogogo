////////////////////////////////////////////////////////////////////////////////
//	项目名称：配置模块接口
//	文件名称：IConfig.h
//	实现功能：
//		配置的结构是一个树型结构，叶子用来表示配置项，树枝用来表示结点，
//			用户在打开一个结点的情况下，可以方便地对其叶子进行访问。
//		不管是配置项还是配置结点在当前层次中都具有一个唯一的 ID，不同层次中的配置项或配置结点，
//			可以具有相同的 ID，但它们的 ID 全路径在整个配置系统中是唯一的。
//		配置项：  是树型结构的叶子，它用来保存配置数据
//		配置结点：是树型结构的枝，它用来对其子结点进行权限管理，它也具备配置项的全部功能
//
//		模块的使用：此模块为独立的DLL，请利用导出函数NewConfigObject来获得一个配置对象IConfig，
//					利用IConfig对配置进行访问，使用ReleaseConfigObject来释放一个配置对象。
//	作    者：ZhangLingang
//	编写日期：2004-3-11 10:49:07
//
//	修改历史（自下而上填写 内容包括：日期  修改人  修改说明）
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ICONFIG_H__
#define __ICONFIG_H__

#include "RSCFGDEF.H"

struct STCONFIGHEADER
{
	RS_DWORD dwFileFlag;				//文件标志
	RS_DWORD dwVersion;					//数据文件版本	
	RS_DWORD dwHeaderLen;				//文件头的长度
	RS_DWORD dwFileLen;					//文件长度

	RS_DWORD dwReserved1;				//保留1
	RS_DWORD dwReserved2;				//保留2
	RS_DWORD dwReserved3;				//保留3
	RS_DWORD dwReserved4;				//保留4

	RS_DWORD dwProductID;				//产品标志ID
	RS_DWORD dwDataOffset;				//数据的起始偏移	
	RS_DWORD dwCheckSum1;				//文件校验值1
	RS_DWORD dwCheckSum2;				//文件校验值2

	RS_CHAR  szProductName[16];			//产品名字

	RS_CHAR  szCreateTime[20];			//产品名字(4)
	RS_CHAR  szReleaseTime[20];			//发行时间
	RS_DWORD dwSHMLength;				//共享内存区的长度
	RS_DWORD dwPrivate;					//私有

	RS_DWORD dwEncryptMethod;			//加密方法
	RS_DWORD dwEncryptKeyLen;			//密钥长度，不能超过32
	RS_DWORD dwCompressMethod;			//压缩方法
	RS_DWORD dwCompressKeyLen;			//解压区长度，不能超过32

	RS_BYTE  byEncryptKey[32];			//密钥区(2)
	RS_BYTE  byCompressKey[32];			//解压数据区(2)

	RS_CHAR  szFileName[320];			//数据文件名字(20)
	RS_CHAR  szPrivateData[256];		//私有数据区(16)
};

typedef struct STConfigItem
{
	RS_DWORD	m_dwId;				// 结点 ID
	RS_DWORD	m_dwLevel;			// 结点的级别，是配置项时此结点没用
	RS_DWORD	m_dwStatus;			// 结点的状态和类型
	RS_DWORD	m_dwAccess;			// 访问权限
	RS_DWORD	m_dwDataType;		// 存储的数据类型
	RS_DWORD	m_dwDataLen;		// 数据长度
	RS_BYTE*	m_pData;			// 数据指针
	RS_CHAR*	m_pName;			// 名字指针
	RS_CHAR*	m_pMemo;			// 备注指针
	RS_DWORD*	m_pdwUserId;		// 用户ID指针
}*PSTConfigItem;

class IConfig
{
public:
	// 按全路径创建或打开一个结点，一个结点创建成功后自然打开
	// pdwIdPath: 此参数为一个 RS_DWORD 数组，大小限制为 64，用来表示结点 ID 的全路径, 以 0 为路径的结束标志.
	// dwAccess: 打开或创建此结点时想要的权限，创建时需使用 CONFIG_ACCESS_CREATE
	// pdwUserId: 用户的 ID 标识，UserId 的长度由使用者定义（一般为一个 128 位的数据）
	// 成功返回 ERR_SUCCESS，否则返回错误码
	virtual RS_RESULT CreateNode(const RS_DWORD* pdwIdPath, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig) = 0;

	// 关闭已经打开的结点
	virtual RS_RESULT CloseNode() = 0;

	// 删除结点
	virtual RS_RESULT DeleteNode() = 0;
	//创建一个配置项或一个配置结点
#ifdef FOR_TEST
	virtual RS_RESULT Create(PSTConfigItem pstItem, IConfig** ppConfig) = 0;
#endif
	virtual RS_RESULT Delete(RS_DWORD dwId) = 0;

	// 取得结点的全路径
	// pdwPath: 全路径的返回结果
	virtual RS_RESULT GetNodePath(RS_DWORD* pdwPath, RS_DWORD& dwPathLen) = 0;

	// 创建一个配置项
	// dwItemId: 配置项的ID
	virtual RS_RESULT CreateItem(RS_DWORD dwItemId) = 0;

	// 删除一个配置项
	// dwItemId: 配置项的ID
	virtual RS_RESULT DeleteItem(RS_DWORD dwItemId) = 0;

	// 获取一个配置项的值
	// dwItemId: 配置项的ID
	// pbyData: 输出的数据
	// dwSize: 数据的长度
	virtual RS_RESULT GetItemValue(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD& dwSize) = 0;

	// 设置一个配置项的值
	// dwItemId: 配置项的ID
	// pbyData: 数据缓冲区
	// dwSize: 数据长度
	virtual RS_RESULT SetItemValue(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD& dwSize) = 0;

	virtual RS_RESULT GetItemValueBYTE(RS_DWORD dwItemId, RS_BYTE& byData) = 0;
	virtual RS_RESULT SetItemValueBYTE(RS_DWORD dwItemId, RS_BYTE byData) = 0;
	virtual RS_RESULT GetItemValueWORD(RS_DWORD dwItemId, RS_WORD& wData) = 0;
	virtual RS_RESULT SetItemValueWORD(RS_DWORD dwItemId, RS_WORD wData) = 0;
	virtual RS_RESULT GetItemValueDWORD(RS_DWORD dwItemId, RS_DWORD& dwData) = 0;
	virtual RS_RESULT SetItemValueDWORD(RS_DWORD dwItemId, RS_DWORD dwData) = 0;
	virtual RS_RESULT GetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize) = 0;
	virtual RS_RESULT SetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize) = 0;

	// 获取配置项或结点的各种信息, dwItemId 为 IVALID_ITEMID 时表示获得结点本身的信息
	// dwItemId: 配置项 ID
	// dwInfoId: 信息的 ID
	// pbyBuffer: 输出的数据缓冲区
	// dwBufSize: 数据缓冲区大小
	virtual RS_RESULT GetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, RS_BYTE* pbyBuffer, RS_DWORD& dwBufSize) = 0;

	// 设置配置项或结点的各种信息, dwItemId 为 IVALID_ITEMID 时表示获得结点本身的信息
	// dwItemId: 配置项 ID
	// dwInfoId: 信息的 ID
	// pbyBuffer: 输入的数据缓冲区
	// dwBufSize: 数据缓冲区大小
	virtual RS_RESULT SetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, const RS_BYTE* pbyBuffer, RS_DWORD dwBufSize) = 0;
	
	// 使用同一个 IConfig 对象获取孩子结点
	// dwId: 结点 ID
	// dwAccess: 打开或创建此结点时想要的权限，创建时需使用 CONFIG_ACCESS_CREATE
	// pdwUserId: 用户的 ID 标识，UserId 的长度由使用者定义（一般为一个 128 位的数据）
	virtual RS_RESULT GetChildNode(RS_DWORD dwId, RS_DWORD dwAccess, const RS_DWORD* pdwUserId) = 0;

	// 打开一个配置项的孩子结点，返回 IConfig 对象指针
	// dwId: 结点 ID
	// ppConfig: IConfig 对象指针
	virtual RS_RESULT GetChildNode(RS_DWORD dwId, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig) = 0;

	// 使用同一个 IConfig 对象获取兄弟结点
	// dwId: 结点 ID
	// dwAccess: 打开或创建此结点时想要的权限，创建时需使用 CONFIG_ACCESS_CREATE
	// pdwUserId: 用户的 ID 标识，UserId 的长度由使用者定义（一般为一个 128 位的数据）
	virtual RS_RESULT GetSiblingNode(RS_DWORD dwId, RS_DWORD dwAccess, const RS_DWORD* pdwUserId) = 0;
#ifdef FOR_TEST
	virtual RS_RESULT GetSiblingNode(RS_DWORD dwId, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig) = 0;
#endif
	//装入一个配置文件,返回一个配置对象, ppConfig==RS_NULL时,用对象本身装入
	virtual RS_RESULT LoadConfig(const RS_CHAR* pszFilename, IConfig** ppConfig = RS_NULL) = 0;
	//将1个配置对象叠加到这个配置对象上
	virtual RS_RESULT AddConfig(IConfig* pConfig) = 0;
	
	//将配置文件导入到这个配置对象中
	//在具有权限的情况下,如果具有重复的项,则进行覆盖而不进行提示
	virtual RS_RESULT Import(RS_CHAR *pszFileName) = 0;
	virtual	RS_RESULT ImportToRoot(RS_CHAR *pszFileName) = 0;
	//把这个配置对象导出成为一个配置文件
	//在具有权限的情况下,如果此文件已经存在,则覆盖此文件而不进行提示
	virtual RS_RESULT Export(RS_CHAR *pszFileName) = 0;
	virtual RS_RESULT ExportAll(RS_CHAR *pszFileName) = 0;
	//锁定一个配置项
	//如果dwId==INVALID_ITEMID,则表示此对象结点本身
	virtual RS_RESULT Lock(RS_DWORD dwId) = 0;
	//解锁一个配置项
	//如果dwId==INVALID_ITEMID,则表示此对象结点本身
	virtual RS_RESULT Unlock(RS_DWORD dwId) = 0;
	//数据同步
	virtual RS_RESULT Synchronize(RS_VOID *pParam = RS_NULL) = 0;


	//===========================以下方法供编辑器使用===========================
	virtual HITEM GetRoot() = 0;
	virtual HITEM GetSibling(HITEM hParent) = 0;
	virtual HITEM GetChild(HITEM hParent) = 0;
	
	virtual RS_RESULT Create(const RS_DWORD* pdwIdPath, PHITEM phItem, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId) = 0;
	virtual RS_RESULT Close(PHITEM phItem) = 0;
	
	virtual RS_RESULT GetInfo(HITEM hItem, RS_DWORD& dwId, RS_DWORD& dwLevel, RS_DWORD& dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo) = 0;
	virtual RS_RESULT SetInfo(HITEM hItem, RS_DWORD dwId, RS_DWORD dwLevel, RS_DWORD dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo) = 0;
	virtual RS_RESULT GetValue(HITEM hItem, RS_DWORD& dwDataType, RS_BYTE* pbyData, RS_DWORD& dwSize) = 0;
	virtual RS_RESULT SetValue(HITEM hItem, RS_DWORD dwDataType, RS_BYTE* pbyData, RS_DWORD dwSize) = 0;		
	virtual RS_RESULT GetMemo(HITEM hItem, RS_CHAR* pszMemo) = 0;
	virtual RS_RESULT SetMemo(HITEM hItem, RS_CHAR* pszMemo) = 0;
	virtual RS_BOOL Save(HITEM hItem) = 0;
	virtual RS_DWORD DeleteNode(HITEM hItem) = 0;
	//根据序列号取得产品ID，输入lPrtID为-1，则取得相应序列号升级以后的产品ID，否则是原始产品ID。
	virtual RS_RESULT GetProductID(RS_CHAR *pszSN, RS_LONG &lPrtID) = 0;
	virtual RS_RESULT GetConfigHeader(RS_DDWORD &dwHeaderAddr) = 0;
	virtual RS_RESULT QueryUser(RS_DWORD *pUserID) = 0;
};

#endif // __ICONFIG_H__
