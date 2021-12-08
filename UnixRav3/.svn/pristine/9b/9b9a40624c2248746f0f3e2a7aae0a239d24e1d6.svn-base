#ifdef UNIXPLATFORM
    #include <string.h>
#else
    #include "stdafx.h"
#endif

#include "cfgdef2.h"
#include "extern.h"
#include "configimpl.h"
#include "node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNode::CNode()
{
        m_dwNodeEntry = 0;
        m_dwAccess = 0;
        Zerolize();
}

CNode::~CNode()
{
}

void CNode::Zerolize()
{
        m_pSibling = 0;
        m_pChild = 0;
        m_dwId = 0;
        m_dwLevel = 0;
        m_dwStatus = 0;
        m_dwDataType = 0;
        m_dwDataLen = 0;
        m_pData = 0;
        m_dwNameLen = 0;
        m_pName = 0;
        m_dwMemoLen = 0;
        m_pMemo = 0;
}

RS_INT CNode::Compare(CNode* pNode)
{
        if(!pNode)
                return 0;
        RS_DWORD dwID = pNode->GetID();
        if(m_dwId > dwID)
                return 1;
        else if(m_dwId < dwID)
                return -1;
        return 0;
}

RS_INT CNode::Compare(CNode* pNode1, CNode* pNode2)
{
        if(!pNode1 || !pNode2)
                return 0;
        RS_DWORD dwID1 = pNode1->GetID();
        RS_DWORD dwID2 = pNode2->GetID();
        if(dwID1 > dwID2)
                return 1;
        else if(dwID1 < dwID2)
                return -1;
        return 0;
}

bool CNode::IsDeleted()
{
        return (m_dwStatus & CONFIG_STATE_DELETED) ? true : false;
}

RS_DWORD CNode::GetStatus()
{
        return m_dwStatus;
}

RS_DWORD CNode::GetAccess()
{
        return m_dwAccess;
}

RS_DWORD CNode::SetStatus(RS_DWORD dwStatus)
{

//	m_dwStatus &= ~dwStatus;
//	m_dwStatus |= dwStatus;
        m_dwStatus = dwStatus;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetAccess(RS_DWORD dwAccess)
{

        m_dwAccess = dwAccess;
        return ERR_SUCCESS;
}

RS_DWORD CNode::GetDataType()
{
        return m_dwDataType;
}

RS_DWORD CNode::GetData(RS_BYTE* pBuffer, RS_DWORD& dwDataLen)
{
        if((!pBuffer) || (dwDataLen < m_dwDataLen))
        {
                dwDataLen = m_dwDataLen;
                return ERR_CFG_BUFFER_OVERFLOW;
        }

        memset(pBuffer, 0, dwDataLen);
        if(m_dwDataLen>0)
                memcpy(pBuffer, m_pData, m_dwDataLen);
        dwDataLen = m_dwDataLen;

        return ERR_SUCCESS;
}

RS_DWORD CNode::GetName(RS_CHAR* pName, RS_DWORD& dwNameLen)
{
        if((!pName) || (dwNameLen < m_dwNameLen))
        {
                dwNameLen = m_dwNameLen;
                return ERR_CFG_BUFFER_OVERFLOW;
        }

        memset(pName, 0, dwNameLen);
        if(m_dwNameLen > 0)
                memcpy(pName, m_pName, m_dwNameLen);
        dwNameLen = m_dwNameLen;

        return ERR_SUCCESS;
}

RS_DWORD CNode::GetMemo(RS_CHAR* pMemo, RS_DWORD& dwMemoLen)
{
        if((!pMemo) || (dwMemoLen < m_dwMemoLen))
        {
                dwMemoLen = m_dwMemoLen;
                return ERR_CFG_BUFFER_OVERFLOW;
        }

        memset(pMemo, 0, dwMemoLen);
        if(m_dwMemoLen > 0)
                memcpy(pMemo, m_pMemo, m_dwMemoLen);
        dwMemoLen = m_dwMemoLen;

        return ERR_SUCCESS;
}

RS_DWORD CNode::GetNodeEntry()
{
        return m_dwNodeEntry;
}

RS_BYTEPTR CNode::GetDataEntry()
{
        return m_pData;
}

RS_CHARPTR CNode::GetNameEntry()
{
        return m_pName;
}

RS_CHARPTR CNode::GetMemoEntry()
{
        return m_pMemo;
}

RS_DWORD CNode::GetID()
{
        return m_dwId;
}

RS_DWORD CNode::GetLevel()
{
        return m_dwLevel;
}

RS_DWORD CNode::GetDataLen()
{
        return m_dwDataLen;
}

RS_DWORD CNode::GetNameLen()
{
        return m_dwNameLen;
}

RS_DWORD CNode::GetMemoLen()
{
        return m_dwMemoLen;
}

RS_DWORD CNode::SetName(RS_CHAR* pszName)
{
        if(pszName == NULL)
        {
                m_dwNameLen = 0;
                m_pName = pszName;
                return ERR_SUCCESS;
        }
        RS_DWORD dwNewLength = strlen(pszName);
        if( dwNewLength != 0)
        {
                if(m_dwNameLen >= dwNewLength)
                {
                        memcpy(m_pName, pszName, dwNewLength);
                        m_dwNameLen = dwNewLength;
                }
                else
                {
                        m_dwNameLen = dwNewLength;
                        m_pName = MYNEW(RS_CHAR, dwNewLength + 1);
                        if(!m_pName)
                        {
                                m_dwNameLen = 0;
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }
                        memcpy(m_pName, pszName, m_dwNameLen);
                        m_pName[m_dwNameLen] = 0;
                }
        }
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetMemo(RS_CHAR* pszMemo)
{
        if(pszMemo == NULL)
        {
                m_dwMemoLen = 0;
                m_pMemo = pszMemo;
                return ERR_SUCCESS;
        }
        RS_DWORD dwNewLength = strlen(pszMemo);
        if( dwNewLength != 0)
        {
                if(m_dwMemoLen >= dwNewLength)
                {
                        memcpy(m_pMemo, pszMemo, dwNewLength);
                        m_dwMemoLen = dwNewLength;
                }
                else
                {
                        m_dwMemoLen = dwNewLength;
                        m_pMemo = MYNEW(RS_CHAR, dwNewLength + 1);
                        if(!m_pMemo)
                        {
                                m_dwMemoLen = 0;
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }
                        memcpy(m_pMemo, pszMemo, m_dwMemoLen);
                        m_pMemo[m_dwMemoLen] = 0;
                }
        }
        return ERR_SUCCESS;
}

RS_DWORD CNode::ResetName(RS_CHAR* pszName)
{
        if(m_pName && m_dwNameLen!=0)
        {
                RS_DDWORD dwAddr = (RS_DDWORD)((RS_CHAR *)m_pName);
                if((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd))
                {
                        MYDELETE(m_pName);
                        m_dwNameLen = 0;
                        m_pName = RS_NULL;
                }
        }

        if(pszName == NULL)//这个参数是把数据设置为空
        {
                m_dwNameLen = 0;
                m_pName = pszName;
                return ERR_SUCCESS;
        }

        RS_DWORD dwOldLen = m_dwNameLen;
        m_dwNameLen = strlen(pszName);
        if(m_dwNameLen)
        {
                if(!(dwOldLen && dwOldLen >= m_dwNameLen))
                {
                        m_pName = MYNEW(RS_CHAR, m_dwNameLen + 1);
                        if(!m_pName)
                        {
                                m_dwNameLen = 0;
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }
                }
                memcpy(m_pName, pszName, m_dwNameLen);
                if(dwOldLen != m_dwNameLen)
                        m_pName[m_dwNameLen] = 0;
        }
        else
        {
                m_pName = RS_NULL;
        }
        return ERR_SUCCESS;
}

RS_DWORD CNode::ResetMemo(RS_CHAR* pszMemo)
{
        if(m_pMemo && m_dwMemoLen!=0)
        {
                RS_DDWORD dwAddr = (RS_DDWORD)((RS_CHAR *)m_pMemo);
                if((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd))
                {
                        MYDELETE(m_pMemo);
                        m_dwMemoLen = 0;
                        m_pMemo = RS_NULL;
                }
        }

        if(pszMemo == NULL)//这个参数是把数据设置为空
        {
                m_dwMemoLen = 0;
                m_pMemo = pszMemo;
                return ERR_SUCCESS;
        }


        RS_DWORD dwOldLen = m_dwMemoLen;
        m_dwMemoLen = strlen(pszMemo);
        if(m_dwMemoLen)
        {
                if(!(dwOldLen && dwOldLen >= m_dwMemoLen))
                {
                        m_pMemo = MYNEW(RS_CHAR, m_dwMemoLen + 1);
                        if(!m_pMemo)
                        {
                                m_dwMemoLen = 0;
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }
                }
                memcpy(m_pMemo, pszMemo, m_dwMemoLen);
                if(dwOldLen != m_dwMemoLen)
                        m_pMemo[m_dwMemoLen] = 0;
        }
        else
        {
                m_pMemo = RS_NULL;
        }
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetData(RS_BYTE* pData, RS_DWORD dwDataLen, RS_DWORD dwDataType)
{
        if((pData == NULL) || (dwDataLen==0))
        {
                m_dwDataLen = 0;
                m_pData = RS_NULL;
                return ERR_SUCCESS;
        }

        if(dwDataLen != 0)
        {
                if(dwDataType == CONFIG_DATA_DWORD)
                {
                        if(dwDataLen > sizeof(RS_DWORD))
                                dwDataLen = sizeof(RS_DWORD);
                }
                else if(dwDataType == CONFIG_DATA_WORD)
                {
                        if(dwDataLen > sizeof(RS_WORD))
                                dwDataLen = sizeof(RS_WORD);
                }
                else if(dwDataType == CONFIG_DATA_BYTE)
                {
                        if(dwDataLen > sizeof(RS_BYTE))
                                dwDataLen = sizeof(RS_BYTE);
                }

                if(m_dwDataLen >= dwDataLen)
                {
                        m_dwDataLen = dwDataLen;
                        memcpy(m_pData, pData, m_dwDataLen);
                }
                else
                {
                        m_dwDataLen = dwDataLen;
                        m_pData = MYNEW(RS_BYTE, m_dwDataLen + 1);
                        if(!m_pData)
                        {
                                m_dwDataLen = 0;
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }

                        memcpy(m_pData, pData, m_dwDataLen);
                        m_pData[m_dwDataLen] = 0;
                }
        }
        m_dwDataType = dwDataType;
        return ERR_SUCCESS;
}

RS_DWORD CNode::ResetData(const RS_BYTE* pData, RS_DWORD dwDataLen, RS_DWORD dwDataType)
{
        if(m_pData && m_dwDataLen!=0)	//先删除原来的数据
        {
                RS_DDWORD dwAddr = (RS_DDWORD)((RS_BYTE *)m_pData);
                if(((dwAddr < g_dwSHMStart) || (dwAddr > g_dwSHMEnd)) && (dwDataLen != m_dwDataLen) )
                {
//			MyTrace("ResetData : delete(0x%08x) Len = %d", dwAddr, m_dwDataLen);
                        MYDELETE(m_pData);
                        m_dwDataLen = 0;
                        m_pData = RS_NULL;
                }
        }

        if((pData == RS_NULL) || (dwDataLen==0))//这个参数是把数据设置为空
        {
                m_dwDataLen = 0;
                m_pData = RS_NULL;
                return ERR_SUCCESS;
        }

        if(dwDataType == CONFIG_DATA_DWORD)
        {
                if(dwDataLen > sizeof(RS_DWORD))
                        dwDataLen = sizeof(RS_DWORD);
        }
        else if(dwDataType == CONFIG_DATA_WORD)
        {
                if(dwDataLen > sizeof(RS_WORD))
                        dwDataLen = sizeof(RS_WORD);
        }
        else if(dwDataType == CONFIG_DATA_BYTE)
        {
                if(dwDataLen > sizeof(RS_BYTE))
                        dwDataLen = sizeof(RS_BYTE);
        }

        RS_DWORD dwOldLen = m_dwDataLen;
        m_dwDataLen = dwDataLen;
        if(m_dwDataLen)
        {
                //原有数据如果不够长则申请新的内存
                if(!(dwOldLen && dwOldLen >= m_dwDataLen))
                {
                        m_pData = MYNEW(RS_BYTE, m_dwDataLen + 1);
                        if(!m_pData)
                        {
//				MyTrace("ResetData : new(0x%08x) Len = %d", (RS_DWORD)((RS_BYTE *)m_pData), m_dwDataLen + 1);
                                m_dwDataLen = 0;
                                return ERR_CFG_MEM_ALLOCFAILED;
                        }
                }

                memcpy(m_pData, pData, m_dwDataLen);
                if(dwOldLen != m_dwDataLen)
                        m_pData[m_dwDataLen] = 0;
        }
        else
        {
                m_pData = RS_NULL;
        }
        m_dwDataType = dwDataType;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetNodeEntry(RS_DWORD dwEntry)
{
        m_dwNodeEntry = dwEntry;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetDataType(RS_DWORD dwType)
{
        m_dwDataType = dwType;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetDataEntry(RS_BYTE* dwEntry)
{
        m_pData = dwEntry;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetNameEntry(RS_CHAR* dwEntry)
{
        m_pName = dwEntry;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetMemoEntry(RS_CHAR* dwEntry)
{
        m_pMemo = dwEntry;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetChild(CNode* dwEntry)
{
        m_pChild = dwEntry;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetSibling(CNode* dwEntry)
{
        m_pSibling = dwEntry;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetID(RS_DWORD dwID)
{
        m_dwId = dwID;
        return ERR_SUCCESS;
}

RS_DWORD CNode::SetLevel(RS_DWORD dwLevel)
{
        m_dwLevel = dwLevel;
        return ERR_SUCCESS;
}

void *CNode::operator new(size_t size)
{
    void *p = CSharedMem::alloc(size);  // Replace this with alternative allocator
    return (p);
}
void CNode::operator delete(void *p)
{
    CSharedMem::free(p);  // Replace this with alternative de-allocator
}
void *CNode::operator new[](size_t size)
{
    void *p = CSharedMem::alloc(size);
    return (p);
}
void CNode::operator delete[](void *p)
{
    CSharedMem::free(p);
}

CNode* CNode::GetChild()
{
        return (CNode*)m_pChild;
}
CNode* CNode::GetSibling()
{
        return (CNode*)m_pSibling;
}

bool CNode::IsAccessMatchRule(RS_DWORD dwRule)
{
        return ((m_dwAccess & dwRule) == dwRule) ? RS_TRUE: RS_FALSE;
}
bool CNode::IsStatusMatchRule(RS_DWORD dwRule)
{
        return ((m_dwStatus & dwRule) == dwRule) ? RS_TRUE: RS_FALSE;
}

//追加状态
void CNode::AppendStatus(RS_DWORD dwRule)
{
        m_dwStatus |= dwRule;
}

void CNode::ExcludeStatus(RS_DWORD dwRule)
{
        m_dwStatus &= ~dwRule;
}

RS_VOID CNode::Swap()
{
        RS_SWAPDWORDEX(m_dwAccess);
        RS_SWAPDWORDEX(m_dwDataLen);
        RS_SWAPDWORDEX(m_dwDataType);
        RS_SWAPDWORDEX(m_dwId);
        RS_SWAPDWORDEX(m_dwLevel);
        RS_SWAPDWORDEX(m_dwMemoLen);
        RS_SWAPDWORDEX(m_dwNameLen);
        RS_SWAPDWORDEX(m_dwNodeEntry);
        RS_SWAPDWORDEX(m_dwStatus);
        m_pChild.Swap();
        m_pSibling.Swap();
        m_pData.Swap();
        m_pMemo.Swap();
        m_pName.Swap();
}
