#ifndef IPBLACKTABLE_H
#define IPBLACKTABLE_H

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
#include <QMenu>

#include "pubdef.h"
#include "iptables.h"

using namespace std;


class IpBlackItemModel;
class IpBlackIconDelegate;
class IpBlackNameTable;


class IpBlackItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    IpBlackItemModel(QObject * parent=0);
    virtual ~ IpBlackItemModel(){}

signals:


public:
    int m_nHoverRow;

    QVector<QString*> *m_pItems;
    QPixmap * m_pImageDel;
};

class IpBlackIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
		IpBlackIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) {}
    void SetView(IpBlackNameTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
	bool eventFilter(QObject *obj, QEvent *event);

public:
    IpBlackNameTable    *m_pView;
};

class IpBlackNameTable : public QTableView
{
    Q_OBJECT
public:
    IpBlackNameTable(QWidget *parent = Q_NULLPTR);
	void WriteTable();


public slots:
	void PressedHandle(QModelIndex index);
	void ItemChangedHandle(QStandardItem*);
	void mouseMoveEvent(QMouseEvent *event);
	void AddRowEdit();

signals:

protected:

public:

    int m_nCurrentRow;
    QModelIndex m_Index;
    QModelIndex previouseindex;
    QWidget *m_pWidget;
	IpBlackItemModel *m_pModel;
	IpBlackIconDelegate *m_pDelegate;
    QPixmap *m_pImageDel;
    IpTables *m_Stringdata;
	bool ischanged;
	bool firewallflag;
};

#endif // IPBLACKTABLE_H
