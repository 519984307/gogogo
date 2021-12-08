#ifndef DIRSELECTMIDBODYWIDGET_H
#define DIRSELECTMIDBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStringList>
#include <map>


#include "common.h"
#include "dirselecttreeview.h"
#include "dirseltreemodel.h"
#include "dirselpathproc.h"
#include "dirselitem.h"

using namespace std;

#define DIR_SELECT_MID_BODY_WIDGET_HEIGHT   294

class DirSelectTreeView;
class DirSelTreeModel;
class DirSelPathProc;
class DirSelItem;
class MainWindow;



class DirSelectMidBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirSelectMidBodyWidget(QWidget *parent, MainWindow *pMainWindow);

    void InitTreeView();
    int InitChildTreeItem(QIcon &icon, QStandardItem *pItem, map<int,DirSelItem*> &mapItems);

    void treeItem_checkAllChild(QStandardItem * item, bool check);
    void treeItem_checkAllChild_recursion(QStandardItem * item,bool check);
    void treeItem_CheckChildChanged(QStandardItem * item);
    Qt::CheckState checkSibling(QStandardItem * item);

    void GetFocusDir(QStringList *plDirs);
    void GetSelectedDir(QStringList *plDirs);
    void TraceAllSelectedDir(QString sDir, QStandardItem *pitem, QStringList *plDirs);

    void RefreshItem(QModelIndex *index);
    static void CB_RefreshItem(void *pArg, QModelIndex *index);
    DirSelItem * GetDirSelItem(int nID, map<int,DirSelItem*> &mapItems);
    QStandardItem * GetChildPathTreeItem(QStandardItem *pItem, QString &sPath);

signals:

public slots:
    void treeItemChanged (QStandardItem *item);

public:
    DirSelectTreeView   *m_pTree;
    DirSelTreeModel     *m_pModel;
    QStandardItem       *m_pRootItem;

    map<int, DirSelItem*>  m_mapAll;
    DirSelPathProc   m_PathProc;

    int m_nItemID;

    MainWindow * m_pMainWindow;
};

#endif // DIRSELECTMIDBODYWIDGET_H
