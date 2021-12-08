#ifndef UPDATESOURCETABLE_H
#define UPDATESOURCETABLE_H

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


class UpdateTableItem;
class UpdateSourceItemModel;
class UpdateSourceIconDelegate;
class UpdateSourceTable;


class UpdateTableItem
{
public:
    UpdateTableItem(){
    }

    UpdateTableItem(QString source, bool is_cloud_rule){
        source_name = source;
        is_from_cloud_rules = is_cloud_rule;
    }

    UpdateTableItem(const UpdateTableItem &item){
        source_name = item.source_name;
        is_from_cloud_rules = item.is_from_cloud_rules;
    }

    QString source_name;
    bool    is_from_cloud_rules;
};

Q_DECLARE_METATYPE(UpdateTableItem)

class UpdateSourceItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    UpdateSourceItemModel(QObject * parent=0);
    virtual ~ UpdateSourceItemModel(){}

    void SetData(QVector<UpdateTableItem*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;


public:
    int m_nHoverRow;

    QVector<UpdateTableItem*> *m_pItems;
    QPixmap * m_pImageDel;
};

class UpdateSourceIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    UpdateSourceIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(UpdateSourceTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    UpdateSourceTable    *m_pView;
};


class UpdateSourceTable : public QTableView
{
    Q_OBJECT
public:
    UpdateSourceTable(QWidget *parent = Q_NULLPTR);

    void AddSourceStart();

    int AddItem(QString sName, bool is_cloud_rule);
    void DelItem(int row);
    void ClearAllItem();

    void UpdateRow(int row);
    int GetAllItem(vector<UpdateTableItem> * vData);

public slots:
    void AddSourceEnd();
    void ShowToolTip(int row);

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());

protected:
    bool event(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);
//    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void CellClicked(const QModelIndex &index);


public:
    QVector<UpdateTableItem*>  m_AllItems;

    UpdateSourceItemModel *m_pModel;
    UpdateSourceIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_Index;
    QWidget *m_pWidget;
};

#endif // UpdateSourceTable_H
