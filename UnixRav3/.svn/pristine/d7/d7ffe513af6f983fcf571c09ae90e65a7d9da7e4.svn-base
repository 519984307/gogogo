#ifndef BLACKLISTIPTABLE_H
#define BLACKLISTIPTABLE_H

#include <string>

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

using namespace  std;

class IpItemModel;
class IpPortIconDelegate;
class BlacklistIPTable;


class IpItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    IpItemModel(QObject * parent=0);
    virtual ~ IpItemModel(){}

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

class IpPortIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    IpPortIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(BlacklistIPTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    BlacklistIPTable    *m_pView;
};


class BlacklistIPTable : public QTableView
{
    Q_OBJECT
public:
    BlacklistIPTable(QWidget *parent = Q_NULLPTR);

    void AddIpNameStart();

    int AddItem(QString sName);
    void DelItem(int row);

    void UpdateRow(int row);

    bool HasIP(const QString & sIP);

public slots:
    void AddPortStart();
    void AddIpPortEnd();
    void ShowToolTip(int row);

protected:
    bool event(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);
//    void keyPressEvent(QKeyEvent *event);
//    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void CellClicked(const QModelIndex &index);


public:
    QVector<QString*>  m_AllItems;

    IpItemModel *m_pModel;
    IpPortIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_Index;
    QWidget *m_pWidget;
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    QMetaObject::Connection  m_connIPEditEnd;
    QMetaObject::Connection  m_connPortEditEnd;
#endif
};

#endif // BLACKLISTIPTABLE_H
