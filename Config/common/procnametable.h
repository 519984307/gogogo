#ifndef PROCNAMETABLE_H
#define PROCNAMETABLE_H

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


class ProcItemModel;
class ProcIconDelegate;
class ProcNameTable;


class ProcItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    ProcItemModel(QObject * parent=0);
    virtual ~ ProcItemModel(){}

    void SetData(QVector<QString*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;


public:
    int m_nHoverRow;

    QVector<QString*> *m_pItems;
    QPixmap * m_pImageDel;
};

class ProcIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ProcIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(ProcNameTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    ProcNameTable    *m_pView;
};


class ProcNameTable : public QTableView
{
    Q_OBJECT
public:
    ProcNameTable(QWidget *parent = Q_NULLPTR);

    void AddProcNameStart();

    int AddItem(QString sName);
    void DelItem(int row);
    void ClearAllItem();

    void UpdateRow(int row);
    int GetAllItem(vector<string> * vData);

public slots:
    void AddProcNameEnd();
    void ShowToolTip(int row);

protected:
    bool event(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);
//    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void CellClicked(const QModelIndex &index);


public:
    QVector<QString*>  m_AllItems;

    ProcItemModel *m_pModel;
    ProcIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_Index;
    QWidget *m_pWidget;
};

#endif // PROCNAMETABLE_H
