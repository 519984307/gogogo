#ifndef DIRTABLE_H
#define DIRTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QMap>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>
#include <string>
#include <vector>

#include "pubdef.h"

using namespace std;

class DDisplayItem;
class DItemModel;
class DIconDelegate;
class DirTable;


class DDisplayItem{
public:
    DDisplayItem(int nType, QString sName)
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

class DItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    DItemModel(QObject * parent=0);
    virtual ~ DItemModel(){}

    void SetData(QMap<int, DDisplayItem*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;

    //QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public:
    int m_nHoverRow;

    QMap<int, DDisplayItem*>  *m_pItems;
    QPixmap * m_pImageDel;
};

class DIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(DirTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    DirTable    *m_pView;
};

class DItemData{
public:
    DItemData(int nType, const string sName){
        m_nType = nType;
        m_sName = sName;
    }

    int m_nType;
    string m_sName;
};

class DirTable : public QTableView
{
    Q_OBJECT
public:
    DirTable(QWidget *parent = Q_NULLPTR);

    int AddItem(int nType, QString sName);
    void DelItem(int row);
    void ClearAllItem();
    int  GetAllItem(vector<DItemData> * vData);


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
    QMap<int, DDisplayItem*>  m_AllItems;

    DItemModel *m_pModel;
    DIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
};

#endif // DIRTABLE_H
