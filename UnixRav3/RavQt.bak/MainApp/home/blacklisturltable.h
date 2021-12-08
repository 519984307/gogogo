#ifndef BLACKLISTURLTABLE_H
#define BLACKLISTURLTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QVector>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>

#include "common.h"


class UrlItemModel;
class UrlIconDelegate;
class BlacklistUrlTable;


class UrlItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    UrlItemModel(QObject * parent=0);
    virtual ~ UrlItemModel(){}

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

class UrlIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    UrlIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(BlacklistUrlTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    BlacklistUrlTable    *m_pView;
};


class BlacklistUrlTable : public QTableView
{
    Q_OBJECT
public:
    BlacklistUrlTable(QWidget *parent = Q_NULLPTR);

    void AddUrlNameStart();

    int AddItem(QString sName);
    void DelItem(int row);

    void UpdateRow(int row);

    bool HasUrl(const QString &sUrl);

public slots:
    void AddUrlNameEnd();
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

    UrlItemModel *m_pModel;
    UrlIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_Index;
    QWidget *m_pWidget;
};

#endif // BLACKLISTURLTABLE_H
