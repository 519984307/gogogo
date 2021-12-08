#ifndef DIRFILETABLE_H
#define DIRFILETABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QMap>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>

#include "pubdef.h"

class DFDisplayItem;
class DirItemModel;
class DFIconDelegate;
class DirFileTable;


class DFDisplayItem{
public:
    DFDisplayItem(int nType, QString sName)
        :m_nType(nType),m_sName(sName)
    {

    }

    void SetIndex(int nIndex)
    {
        m_nIndex=nIndex;
    }

public:
    int m_nIndex;
    int m_nType;
    QString m_sName;
};

class DirItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    DirItemModel(QObject * parent=0);
    virtual ~ DirItemModel(){}

    void SetData(QMap<int, DFDisplayItem*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;

    //QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public:
    int m_nHoverRow;

    QMap<int, DFDisplayItem*>  *m_pItems;
    QPixmap * m_pImageDel;
};

class DFIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DFIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(DirFileTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    DirFileTable    *m_pView;
};


class DirFileTable : public QTableView
{
    Q_OBJECT
public:
    DirFileTable(QWidget *parent = Q_NULLPTR);

    int AddItem(int nType, QString sName);
    void DelItem(int row);

    void UpdateRow(int row);

public slots:
    void ShowToolTip(int row);

protected:
    bool event(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);
//    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void CellClicked(const QModelIndex &index);


public:
    QMap<int, DFDisplayItem*>  m_AllItems;

    DirItemModel *m_pModel;
    DFIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
};

#endif // DIRFILETABLE_H
