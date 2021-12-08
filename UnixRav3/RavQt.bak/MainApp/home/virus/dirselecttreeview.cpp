#include <QMouseEvent>
#include <QStandardItem>
#include "dirselecttreeview.h"

DirSelectTreeView::DirSelectTreeView(QWidget *parent):QTreeView(parent)
{
    setHeaderHidden(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::NoSelection);
    m_cbRefreshItem = NULL;

    setStyleSheet("QTreeView::indicator:enabled:unchecked {image:url(:/resource/unchecked);}"
                  "QTreeView::indicator:enabled:checked {image:url(:/resource/checked);}"
                  "QTreeView::indicator:enabled:indeterminate {image:url(:/resource/indeterminate);}"
                  "QTreeView::item {margin-left:0px;}");
}

void DirSelectTreeView::mousePressEvent(QMouseEvent *e)
{
    QRect rect = visualRect(indexAt(e->pos()));
    QRect expandOrCollape=QRect(rect.left()-20, rect.top(), 20, rect.height());

    if(expandOrCollape.contains(e->pos())){
        QModelIndex index = indexAt(e->pos());
        if(m_cbRefreshItem){
            (*m_cbRefreshItem)((void*)parent(), &index);
        }
    }
    QTreeView::mousePressEvent(e);
}

void DirSelectTreeView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QModelIndex index = indexAt(e->pos());
    QModelIndex indexCurrent = currentIndex();
    if(m_cbRefreshItem && index.row() == indexCurrent.row()){
        (*m_cbRefreshItem)((void*)parent(), &index);
    }
    QTreeView::mouseDoubleClickEvent(e);
}
