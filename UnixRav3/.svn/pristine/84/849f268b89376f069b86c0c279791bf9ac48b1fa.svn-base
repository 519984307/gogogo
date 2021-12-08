// NodeData.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NODE_H__
#define __NODE_H__

#include "smartptr.h"

// 此结构为直接存储在文件中的
class CNode;
typedef ClassPtr<CNode,	&g_pBasePtr>		CNodePTR;
typedef BasePtr<RS_BYTE, &g_pBasePtr>		RS_BYTEPTR;
typedef BasePtr<RS_CHAR, &g_pBasePtr>		RS_CHARPTR;

class CNode
{
private:
        CNodePTR	m_pSibling;		// 兄弟
        CNodePTR	m_pChild;			// 孩子
        RS_DWORD	m_dwId;				// 结点 ID
        RS_DWORD	m_dwLevel;			// 结点的级别，是配置项时此结点没用
        RS_DWORD	m_dwStatus;			// 结点的状态和类型
        RS_DWORD	m_dwDataType;		// 存储的数据类型
        RS_DWORD	m_dwDataLen;		// 数据长度
        RS_BYTEPTR	m_pData;		// 数据在文件中的入口
        RS_DWORD	m_dwNameLen;		// 名字长度
        RS_CHARPTR	m_pName;		// 名字在文件中的入口
        RS_DWORD	m_dwMemoLen;		// 备注长度
        RS_CHARPTR	m_pMemo;

        RS_DWORD	m_dwNodeEntry;		// 结点在文件中的偏移
        RS_DWORD	m_dwAccess;			// 打开的访问属性

public:
        RS_VOID Swap();
        CNode();
        ~CNode();

        RS_DWORD GetData(RS_BYTE* pBuffer, RS_DWORD& dwDataLen);
        RS_DWORD GetName(RS_CHAR* pName, RS_DWORD& dwNameLen);
        RS_DWORD GetMemo(RS_CHAR* pMemo, RS_DWORD& dwMemoLen);

        RS_DWORD GetNodeEntry();
        RS_BYTEPTR GetDataEntry();
        RS_CHARPTR GetNameEntry();
        RS_CHARPTR GetMemoEntry();

        RS_DWORD GetID();
        RS_DWORD GetLevel();
        RS_DWORD GetStatus();
        RS_DWORD GetDataType();
        RS_DWORD GetAccess();
        RS_DWORD GetDataLen();
        RS_DWORD GetNameLen();
        RS_DWORD GetMemoLen();

        RS_DWORD SetID(RS_DWORD dwID);
        RS_DWORD SetLevel(RS_DWORD dwLevel);
        RS_DWORD SetStatus(RS_DWORD dwStatus);
        RS_DWORD SetDataType(RS_DWORD dwType);
        RS_DWORD SetAccess(RS_DWORD dwAccess);
        RS_DWORD SetData(RS_BYTE* pData, RS_DWORD dwDataLen, RS_DWORD dwDataType = CONFIG_DATA_STRING);
        RS_DWORD SetName(RS_CHAR* pszName);
        RS_DWORD SetMemo(RS_CHAR* pszMemo);

        RS_DWORD ResetData(const RS_BYTE* pData, RS_DWORD dwDataLen, RS_DWORD dwDataType = CONFIG_DATA_STRING);
        RS_DWORD ResetName(RS_CHAR* pszName);
        RS_DWORD ResetMemo(RS_CHAR* pszMemo);

        RS_DWORD SetNodeEntry(RS_DWORD dwEntry);
        RS_DWORD SetDataEntry(RS_BYTE* dwEntry);
        RS_DWORD SetNameEntry(RS_CHAR* dwEntry);
        RS_DWORD SetMemoEntry(RS_CHAR* dwEntry);

        RS_DWORD SetChild(CNode* dwEntry);
        RS_DWORD SetSibling(CNode* dwEntry);

        bool IsDeleted();
        bool HaveValue();
        bool IsAccessMatchRule(RS_DWORD dwRule);
        bool IsStatusMatchRule(RS_DWORD dwRule);
        void AppendStatus(RS_DWORD dwRule);
        void ExcludeStatus(RS_DWORD dwRule);

        RS_INT Compare(CNode* pNode1, CNode* pNode2);
        RS_INT Compare(CNode* pNode);

        CNode* GetChild();
        CNode* GetSibling();

    void *operator new(size_t size);
    void operator delete(void *p);
    void *operator new[](size_t size);
    void operator delete[](void *p);
private:
        void Zerolize();
};


#endif // __NODE_H__
