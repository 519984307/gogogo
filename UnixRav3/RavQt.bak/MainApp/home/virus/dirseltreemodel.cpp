#include "dirselectmidbodywidget.h"
#include "dirseltreemodel.h"

DirSelTreeModel::DirSelTreeModel(QObject *parent):QStandardItemModel(parent)
{
    setColumnCount(1);
}

bool DirSelTreeModel::hasChildren(const QModelIndex &parent) const
{
    QStandardItem *pitem = itemFromIndex(parent);
    if(pitem){
        return pitem->hasChildren();
    }
    return true;
}
