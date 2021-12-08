#ifndef __TREE_H__
#define __TREE_H__

#ifdef UNIXPLATFORM
    #include <stdio.h>
#endif

#include "cfgdef2.h"
#include "extern.h"
#include "node.h"
#include "shmem.h"

//文件头的总长度是16的整数倍,不足的补齐
//配置数据文件的头
/*
struct STCONFIGHEADEROLD
{
        RS_DWORD dwFileFlag;				//文件标志
        RS_DWORD dwVersion;					//数据文件版本
        RS_DWORD dwProduct;					//产品标志
        RS_DWORD dwHeaderLen;				//文件头的长度
        RS_DWORD dwEncryptMethod;			//加密方法
        RS_DWORD dwEncryptKeyLen;			//密钥区长度
        RS_DWORD dwCompressMethod;			//压缩方法
        RS_DWORD dwCompressKeyLen;			//解压区长度
        RS_CHAR  szCreateTime[20];			//创建时间
        RS_CHAR  szReleaseTime[20];			//发行时间
        RS_DWORD dwDataOffset;				//数据的起始偏移
        RS_DWORD dwReserved;				//保留
};
*/
struct STCONFIGHEADEROLD
{
        RS_DWORD dwFileFlag;				//文件标志
        RS_DWORD dwVersion;					//数据文件版本
        RS_DWORD dwHeaderLen;				//文件头的长度
        RS_DWORD dwFileLen;					//文件长度

        RS_DWORD dwProductID;				//产品标志ID
        RS_DWORD dwDataOffset;				//数据的起始偏移
        RS_DWORD dwCheckSum1;				//文件校验值1
        RS_DWORD dwCheckSum2;				//文件校验值2

        RS_CHAR  szProductName[16];			//产品名字

        RS_CHAR  szCreateTime[20];			//产品名字(4)
        RS_CHAR  szReleaseTime[20];			//发行时间
        RS_DWORD dwSHMLength;				//共享内存区的长度
        RS_DWORD dwReserved2;				//保留2

        RS_DWORD dwEncryptMethod;			//加密方法
        RS_DWORD dwEncryptKeyLen;			//密钥长度，不能超过32
        RS_DWORD dwCompressMethod;			//压缩方法
        RS_DWORD dwCompressKeyLen;			//解压区长度，不能超过32

        RS_BYTE  byEncryptKey[32];			//密钥区(2)
        RS_BYTE  byCompressKey[32];			//解压数据区(2)

        RS_CHAR  szFileName[320];			//数据文件名字(20)
        RS_CHAR  szPrivateData[256];		//私有数据区(16)
};
/*
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
*/
class CTree
{
public:
        CNode* m_pControl;						//当前打开结点的控制结点
        CNodePTR m_pRoot;						//根结点
        RS_DWORD m_dwReference;					//引用计数
        RS_DWORD m_dwSortType;					//树的排序方式
        RS_DWORD m_dwLoadMode;					//树的加载模式
        RS_CHAR m_szFileName[RS_MAX_PATH];		//记录配置文件的名字
private:
        RS_DWORD m_dwErrCode;
public:
        CTree();
        ~CTree();

        RS_DWORD InitTree(const RS_CHAR* pszFileName);

        //RS_DWORD SaveTree(CNode* pRoot, FILE* fp, RS_BOOL bSaveSibling = RS_TRUE, RS_BOOL bRemoveInfo = RS_FALSE);
        RS_DWORD SaveTree(CNode* pRoot, FILE* fp, RS_BOOL bSaveSibling = RS_TRUE, RS_BOOL bRemoveInfo = RS_FALSE, CNode* pParent = RS_NULL);
        RS_DWORD SaveToFile(RS_VOID *pHeader=RS_NULL);

        RS_DWORD OpenItem(const RS_DWORD dwItemId, PHITEM phItem, RS_DWORD dwAccess = CONFIG_ACCESS_READ);
        RS_DWORD OpenItem(const RS_DWORD* pdwIdPath, PHITEM phItem, RS_DWORD &dwAccess, const RS_DWORD* pdwUser);
        RS_DWORD CloseItem(PHITEM phItem);

        RS_DWORD SetItemValue(HITEM hItem, RS_DWORD dwDataType, const RS_BYTE* pbyData, RS_DWORD dwSize);
        RS_DWORD SetItemMemo(HITEM hItem, RS_CHAR* pszMemo);
        RS_DWORD SetItemName(HITEM hItem, RS_CHAR* pszName);
        RS_DWORD SetItemValueBYTE(HITEM hItem, RS_BYTE byData);
        RS_DWORD SetItemValueWORD(HITEM hItem, RS_WORD wData);
        RS_DWORD SetItemValueDWORD(HITEM hItem, RS_DWORD dwData);
        RS_DWORD SetItemValueBinary(HITEM hItem, RS_BYTE* pbyData, RS_DWORD dwSize);
        RS_DWORD SetItemValueString(HITEM hItem, RS_UCHAR* pszData, RS_DWORD dwSize);
        RS_DWORD SetItemInfo(HITEM hItem, RS_DWORD dwId, RS_DWORD dwLevel, RS_DWORD dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo);

        RS_DWORD GetItemValue(HITEM hItem, RS_DWORD& dwDataType, RS_BYTE* pbyData, RS_DWORD& dwSize);
        RS_DWORD GetItemMemo(HITEM hItem, RS_CHAR* pszMemo);
        RS_DWORD GetItemValueBYTE(HITEM hItem, RS_BYTE& byData);
        RS_DWORD GetItemValueWORD(HITEM hItem, RS_WORD& wData);
        RS_DWORD GetItemValueDWORD(HITEM hItem, RS_DWORD& dwData);
        RS_DWORD GetItemValueBinary(HITEM hItem, RS_BYTE* pbyData, RS_DWORD& dwSize);
        RS_DWORD GetItemValueString(HITEM hItem, UCHAR* pszData, RS_DWORD& dwSize);
        RS_DWORD GetItemInfo(HITEM hItem, RS_DWORD& dwId, RS_DWORD& dwLevel, RS_DWORD& dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo);

//	RS_DWORD GetItemPath(HITEM hItem, RS_DWORD* pdwPath, RS_DWORD& dwPathLen);

        HITEM SearchItemById(RS_DWORD dwId, HITEM hParent);
        HITEM SearchItemByName(RS_CHAR* pszName, HITEM hParent);

        RS_DWORD DeleteItem(HITEM hItem);

        RS_DWORD GetAccess(HITEM hItem, RS_DWORD& dwAccess);
        RS_DWORD SetAccess(HITEM hItem, RS_DWORD dwAccess);
        RS_DWORD GetInfo(HITEM hItem, RS_DWORD dwInfoId, RS_BYTE* pbyBuffer, RS_DWORD& dwBufSize);
        RS_DWORD SetInfo(HITEM hItem, RS_DWORD dwInfoId, const RS_BYTE* pbyBuffer, RS_DWORD dwBufSize);

        RS_DWORD Import(HITEM hItemParent, HITEM hItemChild);
        RS_DWORD Export(HITEM hItem, RS_CHAR *pszFileName);
        RS_DWORD ExportAll(HITEM hItem, RS_CHAR *pszFileName);

        CNode *Clone(CNode *pHead, CTree *pTree);
        CNode *CloneAll(CNode *pHead, CTree *pTree, CNode *pParent = RS_NULL);
        CNode *CloneNode(CNode* pcDataSrc, CNode* pcDataDes = RS_NULL);
        RS_DWORD CloneTree(HITEM hItem, CTree **ppTree);
        RS_RESULT GetUserAccess(const RS_DWORD* pdwUser, RS_DWORD dwItemLevel, RS_DWORD& dwAccess);
        RS_BOOL DisposeFile(RS_CHAR *pszSrcFileName, RS_CHAR *pszDestFileName, RS_BOOL bIsCompress, RS_BOOL bDelSrcFile);
// Deleted by LauranceZhang 2004-8-4 13:13:54 <
/*
    void * operator new(size_t size);
    void operator delete(void *p);
    void *operator new[](size_t size);
    void operator delete[](void *p);
*/
// Deleted by LauranceZhang 2004-8-4 13:13:54 >

private:
        CNode* SearchChildByName(RS_CHAR* pszName, CNode* pParent);
        CNode* SearchSiblingByName(RS_CHAR* pszName, CNode* pSibling);
        CNode* SearchChildById(RS_DWORD dwId, CNode* pParent);
        CNode* SearchSiblingById(RS_DWORD dwId, CNode* pSibling);
        CNode* SearchParent(CNode* pRoot, CNode* pNode);

        CNode* InsertChildById(RS_DWORD dwId, CNode* pParent);
        CNode* InsertSiblingById(RS_DWORD dwId, CNode* pSubHead);

        CNode* InsertChild(CNode& item, CNode* pParent);
        CNode* InsertChild(CNode& item);
        CNode* InsertSibling(CNode& item, CNode* pSubHead, CNode* pParent=RS_NULL);
        CNode* InsertSibling(CNode* pItem, CNode* pSubHead, CNode* pParent=RS_NULL);

        RS_DWORD WriteFileHeader(FILE *fp, STCONFIGHEADER *pHeader=RS_NULL);
        RS_DWORD LoadTreeFromFile(const RS_CHAR* pszFileName);
        RS_DWORD LoadTreeFromFileOLD(RS_CHAR* pszFileName);
        RS_VOID AdjustOffset(CNode*pRoot, RS_DWORD dwAdjust);
        RS_VOID ExcludeOpenStatus(CNode*pRoot, RS_DWORD dwStatus);

        RS_VOID SwapHeader(STCONFIGHEADER *pstHeader);
        RS_RESULT EncodeFile(FILE *fp);
        RS_RESULT DecodeData(RS_BYTE *pData);
        RS_RESULT DecodeDataOLD(RS_BYTE *pData);
        RS_DWORD GetProductName(FILE *fp, RS_CHAR *pszPrtname);
        RS_DWORD GetProductNameFromFile(RS_CHAR *pszFileName, RS_CHAR *pszPrtname);

        RS_RESULT SetControlNode(CNode *pParent, CNode *pBase, CNode *pNode);
};

#endif // __TREE_H__
