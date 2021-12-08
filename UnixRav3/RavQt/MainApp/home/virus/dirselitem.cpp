#include "dirselitem.h"

DirSelItem::DirSelItem(int nID, const QString &sAbsPath, const QString &sPath, bool bSelected)
    :m_nID(nID),m_sAbsPath(sAbsPath),m_sPath(sPath),m_bSelected(bSelected)
{

}

DirSelItem::~DirSelItem()
{
    map<int,DirSelItem*>::iterator it = m_mapChild.begin();
    while(it != m_mapChild.end()){
        DirSelItem *pitem = it->second;
        delete pitem;
        pitem = NULL;
        it++;
    }
    m_mapChild.clear();
}


void DirSelItem::AddChild(int nID, const QString &sAbsPath, const QString &sPath, bool bSelected)
{
    DirSelItem *pitem = new DirSelItem(nID,sAbsPath ,sPath, bSelected);
    m_mapChild[nID] = pitem;
}

void DirSelItem::SetSelected()
{
    m_bSelected = true;
    map<int,DirSelItem*>::iterator it = m_mapChild.begin();
    while(it != m_mapChild.end()){
        DirSelItem *pitem = it->second;
        pitem->SetSelected();
        it++;
    }
}

void DirSelItem::SetUnSelect()
{
    m_bSelected = false;
    map<int,DirSelItem*>::iterator it = m_mapChild.begin();
    while(it != m_mapChild.end()){
        DirSelItem *pitem = it->second;
        pitem->SetUnSelect();
        it++;
    }
}

bool DirSelItem::HasItemID(int nID)
{
    if(m_mapChild.count(nID) != 0)
        return true;
    return false;
}

bool DirSelItem::HasChild()
{
    if(m_mapChild.size() > 0)
        return true;
    return false;
}
