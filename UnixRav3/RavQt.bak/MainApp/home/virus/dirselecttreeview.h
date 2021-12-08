#ifndef DIRSELECTTREEVIEW_H
#define DIRSELECTTREEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QApplication>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QItemSelectionModel>

#include <QDirModel>
#include <QTreeView>
#include <QListView>
#include <QTableView>
#include <QSplitter>

#include "common.h"

typedef void (*CB_RefreshItem)(void *pArg, QModelIndex *index);

class DirSelectTreeView : public QTreeView
{
    Q_OBJECT
public:
    DirSelectTreeView(QWidget *parent = Q_NULLPTR);

    void SetRefreshItemCallback(CB_RefreshItem pFunc){
        m_cbRefreshItem=pFunc;
    }

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);


    CB_RefreshItem  m_cbRefreshItem;
};

#endif // DIRSELECTTREEVIEW_H
