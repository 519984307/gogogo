#ifndef PORTTABLE_H
#define PORTTABLE_H

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

class PortItemModel;
class PortIconDelegate;
class PortNameTable;
class Menu;


class PortItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    PortItemModel(QObject * parent=0);
    virtual ~ PortItemModel(){}


public:
    int m_nHoverRow;

    QVector<QString*> *m_pItems;
    QPixmap * m_pImageDel;
};

class PortIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
		PortIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) {}
    void SetView(PortNameTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
	bool eventFilter(QObject *obj, QEvent *event);
public:
    PortNameTable    *m_pView;
};

class PortNameTable : public QTableView
{
    Q_OBJECT
public:
    PortNameTable(QWidget *parent = Q_NULLPTR);
	void WriteTable();


public slots:
	void PressedHandle(QModelIndex index);
	void ItemChangedHandle(QStandardItem*);
	void ProtocolTriggeredHandle(QAction *aciton);
	void DirectionTriggeredHandle(QAction *action);
	void mouseMoveEvent(QMouseEvent *event);
	void AddRowEdit();

protected:

public:

    int m_nCurrentRow;
    QModelIndex m_Index;
    QModelIndex previouseindex;
    QWidget *m_pWidget;
	PortItemModel *m_pModel;
	PortIconDelegate *m_pDelegate;
    QPixmap *m_pImageDel;
    IpTables *m_Stringdata;
	bool ischanged;
	Menu *m_protocol;
	Menu *m_direction;
};

class Menu : public QMenu
{
    Q_OBJECT
public:
    Menu(QWidget *parent = NULL);


public slots:

public:
	QAction *tcp;
	QAction *udp;
	QAction *tcp_udp;
	QAction *in;
	QAction *out;
	QAction *in_out;
};


#endif // PORTTABLE_H
