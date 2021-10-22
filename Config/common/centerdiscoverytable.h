#ifndef CenterDiscoveryTable_H
#define CenterDiscoveryTable_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QVector>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>
#include <string>
#include <vector>

#include "pubdef.h"

using namespace std;


class CenterDiscoveryTableItem;
class CenterDiscoveryItemModel;
class CenterDiscoveryIconDelegate;
class CenterDiscoveryTable;


class CenterDiscoveryTableItem
{
public:
    CenterDiscoveryTableItem(){
    }

    CenterDiscoveryTableItem(QString address, bool official = false){ //official: if true, means that is the server command
        center_address = address;
        is_official = official;
    }

    CenterDiscoveryTableItem(const CenterDiscoveryTableItem &item){
        center_address = item.center_address;
        is_official = item.is_official;
    }

    QString center_address;
    bool    is_official;
};

Q_DECLARE_METATYPE(CenterDiscoveryTableItem)

class CenterDiscoveryItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    CenterDiscoveryItemModel(QObject * parent=0);
    virtual ~ CenterDiscoveryItemModel(){}

    void SetData(QVector<CenterDiscoveryTableItem*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;


public:
    int m_nHoverRow;

    QVector<CenterDiscoveryTableItem*> *m_pItems;
    QPixmap * m_pImageDel;
};

class CenterDiscoveryIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CenterDiscoveryIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(CenterDiscoveryTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    CenterDiscoveryTable    *m_pView;
};


class CenterDiscoveryTable : public QTableView
{
    Q_OBJECT
public:
    CenterDiscoveryTable(QWidget *parent = Q_NULLPTR);

    void AddCenterStart();

    int AddItem(QString sAddress, bool official = false);
    int DelItem(int row);
    void ClearAllItem();

    void UpdateRow(int row);
    int GetAllItem(vector<CenterDiscoveryTableItem> * vData);

public slots:
    void AddCenterEnd();
    void ShowToolTip(int row);

//protected slots:
//    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());

protected:
    bool event(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);
//    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void CellClicked(const QModelIndex &index);


public:
    QVector<CenterDiscoveryTableItem*>  m_AllItems;

    CenterDiscoveryItemModel        *m_pModel;
    CenterDiscoveryIconDelegate     *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_Index;
    QWidget *m_pWidget;
};

#endif // CenterDiscoveryTable_H
