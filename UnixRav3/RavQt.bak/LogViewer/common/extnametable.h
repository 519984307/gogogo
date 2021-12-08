#ifndef EXTNAMETABLE_H
#define EXTNAMETABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QVector>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>

#include "pubdef.h"


class ExtItemModel;
class ExtIconDelegate;
class ExtNameTable;


class ExtItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    ExtItemModel(QObject * parent=0);
    virtual ~ ExtItemModel(){}

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

class ExtIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ExtIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(ExtNameTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    ExtNameTable    *m_pView;
};


class ExtNameTable : public QTableView
{
    Q_OBJECT
public:
    ExtNameTable(QWidget *parent = Q_NULLPTR);

    void AddExtNameStart();

    int AddItem(QString sName);
    void DelItem(int row);

    void UpdateRow(int row);

public slots:
    void AddExtNameEnd();
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

    ExtItemModel *m_pModel;
    ExtIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_Index;
    QWidget *m_pWidget;
};

#endif // EXTNAMETABLE_H
