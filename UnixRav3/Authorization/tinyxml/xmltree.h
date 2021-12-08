////////////////////////////////////////////////////////////////////////////////
/**
* @file
* IXMLTree 接口功能实现
* <p>项目名称：RsGUILib
* <br>文件名称：xmltree.h
* <br>实现功能：IXMLTree 接口功能实现
* <br>作    者：Dooyan
* <br>编写日期：2007-1-8 13:35:37
* <br>
* <br>修改历史（自下而上填写 内容包括：日期  修改人  修改说明）
*/
////////////////////////////////////////////////////////////////////////////////

// XMLTree.h: interface for Read/Write XML file as Tree
// Version 1.0 by Dooyan
//////////////////////////////////////////////////////////////////////

#if !defined(XMLTREE_H__DOOYAN)
#define XMLTREE_H__DOOYAN

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "tinyxml.h"
#ifndef LPVOID
typedef void *LPVOID;
#endif
#ifdef INT
typedef int                 INT;
#endif


#define HXMLTREEITEM	             LPVOID
#define TESTIFNULL_RETURN_NULL(X)	 if(!X) return NULL;

#define TESTIFNULL_AS_ROOT(XITEM)	 if(!XITEM) XITEM = GetRoot();

class CXMLTree
{
public:
	CXMLTree()
	{ 
		memset(m_szEncoding, 0, sizeof(m_szEncoding));
	};

	virtual ~CXMLTree(){};

public:
	// 节点处理  [7/10/2006 Dooyan]
	virtual HXMLTREEITEM GetRoot()
	{
		HXMLTREEITEM hRoot = (TiXmlNode*)&m_Doc;
		INT nCount = GetChildCount(hRoot);
		if(!nCount)
		{
			if(*m_szEncoding)
			{
				TiXmlDeclaration tDec("1.0", m_szEncoding, "");
				m_Doc.InsertEndChild(tDec);	
			}
			else
			{
				TiXmlDeclaration tDec("1.0", "", "");
				m_Doc.InsertEndChild(tDec);
			}
		}
		return hRoot;
	}
 
	virtual BOOL SetDocEncoding(LPCSTR lpEncoding)
	{
		strcpy(m_szEncoding, lpEncoding);


		TiXmlDeclaration tDec("1.0", m_szEncoding, "yes");
		TiXmlElement* pElement = m_Doc.RootElement();
		m_Doc.InsertBeforeChild(pElement,tDec);
		
// 		TiXmlDeclaration tDec("1.0", m_szEncoding, "");
// 		TiXmlDocument tNew;
// 		tNew.InsertEndChild(tDec);
// 		TiXmlNode* pNew = m_Doc.FirstChildElement()->Clone();
// 		tNew.LinkEndChild(pNew);
// 		m_Doc = tNew;
		return TRUE;
	}

	virtual INT	GetChildCount(HXMLTREEITEM hItem, LPCSTR lpChildName = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;

		INT nCount = 0;
		if(lpChildName)
		{
			for(TiXmlNode* nodeCount = pNode->FirstChild(lpChildName);
			nodeCount;
			nodeCount = pNode->NextSibling(lpChildName) )
			{
				if(IsTreeItem(nodeCount))
					nCount ++;
			}
		}
		else
		{
			for(TiXmlNode* nodeCount = pNode->FirstChild();
			nodeCount;
			nodeCount = nodeCount->NextSibling() )
			{
				if(IsTreeItem(nodeCount))
					nCount ++;
			}
		}
		return nCount;
	}

	virtual HXMLTREEITEM GetParent(HXMLTREEITEM hItem)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		return pNode->Parent();
	}

	virtual HXMLTREEITEM GetChild(HXMLTREEITEM hItem, LPCSTR lpChildName = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;

		if(lpChildName)
			pNode = pNode->FirstChild(lpChildName);
		else
			pNode = pNode->FirstChild();
		
		if(!pNode)
			return NULL;
		else if(IsTreeItem(pNode))
			return pNode;
		else 
			return GetNextItem(pNode, lpChildName);
	}

	virtual HXMLTREEITEM GetPrevItem(HXMLTREEITEM hItem, LPCSTR lpChildName = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;

		do 
		{
			if(lpChildName)
				pNode = pNode->PreviousSibling(lpChildName);
			else
				pNode = pNode->PreviousSibling();
		} while(pNode && !IsTreeItem(pNode));
		
		return pNode;
	}

	virtual HXMLTREEITEM GetNextItem(HXMLTREEITEM hItem, LPCSTR lpChildName = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;

		do 
		{
			if(lpChildName)
				pNode = pNode->NextSibling(lpChildName);
			else
				pNode = pNode->NextSibling();
		} while(pNode && !IsTreeItem(pNode));
		
		return pNode;
	}

	virtual HXMLTREEITEM LocalItem(HXMLTREEITEM hItem, LPCSTR lpPathName, BOOL bCreate = FALSE)
	{
		TESTIFNULL_AS_ROOT(hItem);
		if(!hItem)
			return NULL;

        std::string sPathName = lpPathName;
		if(sPathName.size() && *(sPathName.end() - 1) != '\\')
			sPathName += "\\";

#if 1
		INT nPath = 0;
		HXMLTREEITEM hItemSave = NULL;
		while ((nPath = (INT)sPathName.find('\\')) != -1)
		{
			sPathName[nPath] = 0;

			if(bCreate)
				hItemSave = hItem;

			hItem = GetChild(hItem, sPathName.c_str());
		
			if(!hItem)
			{
				if(bCreate)
					hItem = InsertChild(hItemSave, sPathName.c_str());
				else
					break;
			}

			sPathName = &sPathName[nPath + 1];
		}
#else
		INT nPath = 0;
		INT nNode = 0;
		HXMLTREEITEM hItemSave = NULL;
		while ((nCount = sPathName.find('\\')) != -1)
		{
			sPathName[nPath] = 0;

			if(bCreate)
				hItemSave = hItem;

			nNode = atoi(sPathName.c_str());
			if(nNode)
				sPathName = &sPathName[1];

			hItem = GetChild(hItem, sPathName.c_str());
			while(nNode > 0)
			{
				hItem = GetNextItem(hItem, sPathName.c_str());
				nNode 
			}

			if(bCreate)
				hItem = InsertChild(hItemSave, sPathName.c_str());

			if(!hItem)
			{
				break;
			}

			sPathName = &sPathName[nPath + 1];
		}
#endif
		return (sPathName.size() > 0 ? NULL : hItem);
	}

	virtual HXMLTREEITEM InsertAfterItem(HXMLTREEITEM hItem, LPCSTR lpName)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement xmNewNode(lpName);
		TiXmlNode* pNodeParent = (TiXmlNode*)GetParent(hItem);
		if(!pNodeParent)
			pNodeParent = pNode;
		return pNodeParent->InsertAfterChild(pNode, xmNewNode);
	}

	virtual HXMLTREEITEM InsertAfterItem(HXMLTREEITEM hItem, HXMLTREEITEM hNewItem)
	{
 		TESTIFNULL_AS_ROOT(hItem);
 		TiXmlNode* pNode = (TiXmlNode*)hItem;
 		TiXmlNode* pNewNode = (TiXmlNode*)hNewItem;
 		TiXmlNode* pElem = pNewNode->Clone();
 		TiXmlNode* pNodeParent = (TiXmlNode*)GetParent(hItem);
 		if(!pNodeParent)
 			pNodeParent = pNode;
 		TiXmlNode * pReturnNode =  pNodeParent->InsertAfterChild(pNode, *pElem);
 		delete pElem;
 		return pReturnNode;
	}

	virtual HXMLTREEITEM InsertBeforeItem(HXMLTREEITEM hItem, LPCSTR lpName)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement xmNewNode(lpName);
		TiXmlNode* pNodeParent = (TiXmlNode*)GetParent(hItem);
		if(!pNodeParent)
			pNodeParent = pNode;
		return pNodeParent->InsertBeforeChild(pNode, xmNewNode);
	}

	virtual HXMLTREEITEM InsertBeforeItem(HXMLTREEITEM hItem, HXMLTREEITEM hNewItem)
	{
		TESTIFNULL_AS_ROOT(hItem);
		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlNode* pNewNode = (TiXmlNode*)hNewItem;
		TiXmlNode* pElem = pNewNode->Clone();
		TiXmlNode* pNodeParent = (TiXmlNode*)GetParent(hItem);
		if(!pNodeParent)
			pNodeParent = pNode;
		TiXmlNode * pReturnNode =  pNodeParent->InsertBeforeChild(pNode, *pElem);
		delete pElem;
		return pReturnNode;
	}

	virtual HXMLTREEITEM InsertChild(HXMLTREEITEM hItem, LPCSTR lpName)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement xmNewNode(lpName);
		return pNode->InsertEndChild(xmNewNode);
	}

	virtual HXMLTREEITEM InsertChild(HXMLTREEITEM hItem, HXMLTREEITEM hNewItem)
	{
		TESTIFNULL_AS_ROOT(hItem);
		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlNode* pNewNode = (TiXmlNode*)hNewItem;
		TiXmlNode* pElem = pNewNode->Clone();
		TiXmlNode * pReturnNode =  pNode->InsertEndChild(*pElem);
		delete pElem;
		return pReturnNode;
	}

	virtual BOOL DeleteItem(HXMLTREEITEM hItem)
	{
		TESTIFNULL_RETURN_NULL(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		pNode->Clear();
		TiXmlNode* pParent = pNode->Parent();
		if(!pParent)
			m_Doc.Clear();
		else
			pParent->RemoveChild(pNode);
		return TRUE;
	}

	// 节点属性处理 [7/10/2006 Dooyan]
	virtual LPCSTR GetText(HXMLTREEITEM hItem)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		switch(pNode->Type())
		{
		case TiXmlNode::ELEMENT:
			for(pNode = pNode->FirstChild();
				pNode;
				pNode = pNode->NextSibling())
			{
				if(pNode->Type() == TiXmlNode::TEXT)
					return pNode->Value();
			}
			break;
		}
		return NULL;
	}

	virtual LPCSTR GetName(HXMLTREEITEM hItem)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		return pNode->Value();
	}

	virtual LPCSTR GetAttribute(HXMLTREEITEM hItem, LPCSTR lpAttName = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement* pElement = pNode->ToElement();
		LPCSTR pRet = NULL;
		if(pElement)
		{
			if(lpAttName)
				pRet = pElement->Attribute(lpAttName);
			else
			{
				TiXmlAttribute* pAttrib = pElement->FirstAttribute();
				if(pAttrib)
					pRet = pAttrib->Value();
			}
		}
		return pRet;
	}

	virtual INT GetAttributeInt(HXMLTREEITEM hItem, LPCSTR lpAttName = NULL)
	{
		LPCSTR lpRet = GetAttribute(hItem, lpAttName);
		if(lpRet)
			return atoi(lpRet);
		return 0;		
	}

	virtual INT GetAttributeCount(HXMLTREEITEM hItem)
	{
		TESTIFNULL_AS_ROOT(hItem);

		INT nCount = 0;
		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement)
		{
			TiXmlAttribute* pAttrib = pElement->FirstAttribute();
			while(pAttrib)
			{
				nCount ++;
				pAttrib = pAttrib->Next();
			}
		}
		return nCount;
	}

	virtual LPCSTR GetAttribute(HXMLTREEITEM hItem, INT nIndex, LPSTR pName, INT nLen)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement)
		{
			TiXmlAttribute* pAttrib = pElement->FirstAttribute();
			while(pAttrib)
			{
				if(nIndex == 0)
				{
					if(pName)
						strncpy(pName, pAttrib->Name(), nLen);
					return pAttrib->Value();
				}
				nIndex --;
				pAttrib = pAttrib->Next();
			}
		}
		return NULL;
	}

	virtual LPCSTR GetComment(HXMLTREEITEM hItem)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		pNode = pNode->PreviousSibling();
		TiXmlComment* pComment = pNode->ToComment();
		if(pComment)
			return pComment->Value();
		return NULL;
	}

	virtual BOOL SetName(HXMLTREEITEM hItem, LPCSTR lpName)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement)
		{
			pElement->SetValue(lpName);
			return TRUE;
		}
		return FALSE;
	}

	virtual BOOL SetText(HXMLTREEITEM hItem, LPCSTR lpText)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		switch(pNode->Type())
		{
		case TiXmlNode::ELEMENT:
			for(pNode = pNode->FirstChild();
				pNode;
				pNode = pNode->NextSibling())
			{
				if(pNode->Type() == TiXmlNode::TEXT)
				{
					pNode->SetValue(lpText);
					return TRUE;
				}
			}
			if(!pNode)
			{
				TiXmlText xmlText(lpText);
				pNode = (TiXmlNode*)hItem;
				pNode->InsertEndChild(xmlText);
			}
			return TRUE;
			break;
		}
		return FALSE;
	}

	virtual BOOL SetAttribute(HXMLTREEITEM hItem, LPCSTR lpAttName, LPCSTR lpAttValue = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);

		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlElement* pElement = NULL;
		if(pNode)
			pElement = pNode->ToElement();
		if(pElement)
		{
			if(lpAttValue)
				pElement->SetAttribute(lpAttName, lpAttValue);
			else
				pElement->RemoveAttribute(lpAttName);
			return TRUE;
		}
		return FALSE;
	}

	virtual BOOL SetAttributeInt(HXMLTREEITEM hItem, LPCSTR lpAttName, INT nValue = 0)
	{
		CHAR szTemp[100] = {0};
		//_itoa_s(nValue, szTemp, 100, 10);
		sprintf(szTemp, "%d", nValue);
		return SetAttribute(hItem, lpAttName, szTemp);
	}

	virtual HXMLTREEITEM SetComment(HXMLTREEITEM hItem, LPCSTR lpszComment = NULL)
	{
		TESTIFNULL_AS_ROOT(hItem);
		
		TiXmlNode* pNode = (TiXmlNode*)hItem;
		TiXmlNode* pNodePre = pNode->PreviousSibling();
		TiXmlComment* pComment = NULL;
		if(pNodePre)
			pComment = pNodePre->ToComment();
		if(pComment)
		{// 修改或者删除
			if(lpszComment)
			{
				pComment->SetValue(lpszComment);
				return pNodePre;
			}
			else
			{
				DeleteItem(pNodePre);
			}
			return NULL;
		}

		if(lpszComment)
		{
			TiXmlComment xmlComment;
			xmlComment.SetValue(lpszComment);
			TiXmlNode* pParent = pNode->Parent();
			if(!pParent)
				pParent = &m_Doc;
			return pParent->InsertBeforeChild(pNode, xmlComment);
		}
		return NULL;
	}
	
	// 文件读写处理 [7/10/2006 Dooyan]
	virtual BOOL Load(LPCSTR lpFileName)
	{
		m_Doc.Clear();
		return m_Doc.LoadFile(lpFileName);
	}

	virtual BOOL LoadBuff(LPCSTR lpFileBuff)
	{
		m_Doc.Clear();
		return (m_Doc.Parse(lpFileBuff) == 0);
	}

	virtual BOOL Save(LPCSTR lpFileName)
	{
		return m_Doc.SaveFile(lpFileName);
	}

	// 接口处理 [7/10/2006 Dooyan] 
	virtual BOOL Release()
	{
		delete this;
		return TRUE;
	}

private:
	BOOL IsTreeItem(HXMLTREEITEM hItem)
	{
		TiXmlNode* pNode = (TiXmlNode*)hItem;
		switch(pNode->Type())
		{
		case TiXmlNode::DOCUMENT:
		case TiXmlNode::ELEMENT:
			return TRUE;
		case TiXmlNode::COMMENT:
		case TiXmlNode::UNKNOWN:
		case TiXmlNode::TEXT:
		case TiXmlNode::DECLARATION:
		default:
			return FALSE;
		}
		return FALSE;
	}

public:
	TiXmlDocument m_Doc;
	CHAR m_szEncoding[260];
};

#endif // !defined(XMLTREE_H__DOOYAN)
