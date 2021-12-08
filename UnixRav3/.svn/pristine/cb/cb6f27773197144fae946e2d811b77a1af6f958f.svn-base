#include <QVBoxLayout>
#include <QFileIconProvider>
#include <iostream>

#include "dirselectmidbodywidget.h"
#include "mainwindow.h"


DirSelectMidBodyWidget::DirSelectMidBodyWidget(QWidget *parent, MainWindow *pMainWindow) : QWidget(parent),m_nItemID(0)
{
    setFixedHeight(DIR_SELECT_MID_BODY_WIDGET_HEIGHT);
    m_PathProc.m_pMainWindow = pMainWindow;
    InitTreeView();

    QVBoxLayout *player = new QVBoxLayout;
    player->setContentsMargins(0,0,0,0);
    player->addWidget(m_pTree);
    setLayout(player);

    connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(treeItemChanged(QStandardItem*)));
}

void DirSelectMidBodyWidget::InitTreeView()
{
    m_pTree = new DirSelectTreeView(this);
    m_pModel = new DirSelTreeModel(this);
    m_pTree->setModel(m_pModel);
    m_pTree->SetRefreshItemCallback(DirSelectMidBodyWidget::CB_RefreshItem);

    QStringList list;
    list << "boot" << "dev" << "proc";

    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(QFileIconProvider::Folder);
    m_pRootItem = new QStandardItem(icon, QString("/"));
    m_pRootItem->setCheckable(true);
    //m_pRootItem->setAutoTristate(true);
    m_pModel->setItem(0, 0, m_pRootItem);

    //QString sroot = "/";
    m_PathProc.InitTreeItems(m_nItemID, false, m_mapAll, QString("/"), 2);
    map<int, DirSelItem*>::iterator it = m_mapAll.begin();
    while(it != m_mapAll.end()){
        DirSelItem *pitem = it->second;
        if(list.indexOf(pitem->m_sPath) != -1){
            it++;
            continue;
        }
        QStandardItem *psitem = new QStandardItem(icon, pitem->m_sPath);
        QString stip;
        stip.setNum(pitem->m_nID);
        psitem->setStatusTip(stip);
        psitem->setCheckable(true);
        //psitem->setAutoTristate(true);
        if(pitem->HasChild()){
            InitChildTreeItem(icon, psitem, pitem->m_mapChild);
        }
        m_pRootItem->appendRow(psitem);
        it++;
    }
    m_pTree->expand(m_pRootItem->index());
}

QStandardItem * DirSelectMidBodyWidget::GetChildPathTreeItem(QStandardItem *pItem, QString &sPath)
{
    int rowCount = pItem->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = pItem->child(i);
        if(childItems->text() == sPath){
            return childItems;
        }
    }
    return NULL;
}

//int DirSelectMidBodyWidget::RefreshChildTreeItem(QStandardItem *pItem)
//{

//}

int DirSelectMidBodyWidget::InitChildTreeItem(QIcon &icon, QStandardItem *pItem, map<int,DirSelItem*> &mapItems)
{
    int append_count = 0;
    map<int,DirSelItem*>::iterator it = mapItems.begin();
    while(it != mapItems.end()){
        DirSelItem *pdsi = it->second;
        QStandardItem * childItem = GetChildPathTreeItem(pItem, pdsi->m_sPath);
        if(childItem){
            append_count = InitChildTreeItem(icon, childItem, pdsi->m_mapChild);
            it++;
            continue;
        }
        QStandardItem *psi = new QStandardItem(icon, pdsi->m_sPath);
        QString stip;
        stip.setNum(pdsi->m_nID);
        psi->setStatusTip(stip);
        psi->setCheckable(true);
        //psi->setAutoTristate(true);
        if(pdsi->HasChild()){
            append_count += InitChildTreeItem(icon, psi, pdsi->m_mapChild);
        }
        pItem->appendRow(psi);
        append_count += 1;
        it++;
    }
    return append_count;
}

void DirSelectMidBodyWidget::treeItemChanged (QStandardItem *item)
{
    if(item == nullptr)
        return ;
    if(item->isCheckable())
    {
        Qt::CheckState state = item ->checkState();
        //if(item->isAutoTristate())
        {
            if(state != Qt::PartiallyChecked)
            {
                treeItem_checkAllChild(item, state == Qt::Checked?true:false);
            }
        }
        treeItem_CheckChildChanged(item);

    }
}

void DirSelectMidBodyWidget::treeItem_checkAllChild(QStandardItem * item, bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

void DirSelectMidBodyWidget::treeItem_checkAllChild_recursion(QStandardItem * item,bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

void DirSelectMidBodyWidget::treeItem_CheckChildChanged(QStandardItem * item)
{
    if(nullptr == item)
        return;
    Qt::CheckState siblingState = checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if(nullptr == parentItem)
        return;
    if(Qt::PartiallyChecked == siblingState)
    {
        if(parentItem->isCheckable()) // && parentItem->isAutoTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if(Qt::Checked == siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItem_CheckChildChanged(parentItem);
}

Qt::CheckState DirSelectMidBodyWidget::checkSibling(QStandardItem * item)
{
    QStandardItem * parent = item->parent();
    if(nullptr == parent)
        return item->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount(0),unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0;i<brotherCount;++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();
        if(Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if(Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0 && unCheckedCount>0)
            return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
        return Qt::Unchecked;
    return Qt::Checked;
}

void DirSelectMidBodyWidget::GetSelectedDir(QStringList *plDirs)
{
    if(m_pRootItem->checkState() == Qt::Unchecked)
        return;

    int rowCount = m_pRootItem->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = m_pRootItem->child(i);
        Qt::CheckState state = childItems->checkState();
        QString spath = m_pRootItem->text() + childItems->text();
        if(state == Qt::PartiallyChecked){
            TraceAllSelectedDir(spath, childItems, plDirs);
        }else if(state == Qt::Checked){
            //qDebug("## %s", spath.toStdString().c_str());
            plDirs->append(spath);
        }
    }
//    vector<QString>::iterator it = vDirs->begin();
//    while(it != vDirs->end()){
//        QString stmp = *it;
//        qDebug("xxx  %s", stmp.toStdString().c_str());
//        it++;
//    }
}

void DirSelectMidBodyWidget::GetFocusDir(QStringList *plDirs) {

}

void DirSelectMidBodyWidget::TraceAllSelectedDir(QString sDir, QStandardItem *pitem, QStringList *plDirs)
{
    int rowCount = pitem->rowCount();
    for(int i=0; i<rowCount; i++){
        QStandardItem* childItems = pitem->child(i);
        QString schildDir = sDir + "/" + childItems->text();
        Qt::CheckState state = childItems->checkState();
        if(state == Qt::PartiallyChecked){
            TraceAllSelectedDir(schildDir, childItems, plDirs);
        }else if(state == Qt::Checked){
            //qDebug("## %s", schildDir.toStdString().c_str());
            plDirs->append(schildDir);
        }
    }
}

DirSelItem * DirSelectMidBodyWidget::GetDirSelItem(int nID, map<int,DirSelItem*> &mapItems)
{
    if(mapItems.size() == 0)
        return NULL;
    map<int, DirSelItem*>::iterator it = mapItems.begin();
    while(it != mapItems.end()){
        DirSelItem *pitem = it->second;
        if(pitem->m_nID == nID){
            return pitem;
        }else{
            DirSelItem *pdsi = GetDirSelItem(nID, pitem->m_mapChild);
            if(pdsi)
                return pdsi;
        }
        it++;
    }
    return NULL;
}

void DirSelectMidBodyWidget::RefreshItem(QModelIndex *index)
{
    QStandardItem *pitem = m_pModel->itemFromIndex(*index);
    if(pitem){
        QFileIconProvider icon_provider;
        QIcon icon = icon_provider.icon(QFileIconProvider::Folder);
        DirSelItem *pdsi = GetDirSelItem(pitem->statusTip().toInt(), m_mapAll);
        if(pdsi == NULL){
            return;
        }
        QString full_path = pdsi->m_sAbsPath;
        if(full_path.right(full_path.length()) != "/")
            full_path += "/";
        full_path += pdsi->m_sPath;

        int added = m_PathProc.InitTreeItems(m_nItemID, pitem->checkState()==Qt::Checked, pdsi->m_mapChild, full_path, 2);
        InitChildTreeItem(icon, pitem, pdsi->m_mapChild);
        if(added > 0){
            m_pModel->dataChanged(*index, *index);
        }
    }
}

void DirSelectMidBodyWidget::CB_RefreshItem(void *pArg, QModelIndex *index)
{
    DirSelectMidBodyWidget *pw = (DirSelectMidBodyWidget*)pArg;
    pw->RefreshItem(index);
}
