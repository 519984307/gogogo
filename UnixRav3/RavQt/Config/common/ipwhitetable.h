#ifndef IPWHITETABLE_H
#define IPWHITETABLE_H

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


class IpWhiteItemModel;
class IpWhiteIconDelegate;
class IpWhiteNameTable;


class IpWhiteItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    IpWhiteItemModel(QObject * parent=0);
    virtual ~ IpWhiteItemModel(){}

signals:


public:
    int m_nHoverRow;

    QVector<QString*> *m_pItems;
    QPixmap * m_pImageDel;
};

class IpWhiteIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
		IpWhiteIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) {}
    void SetView(IpWhiteNameTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
	bool eventFilter(QObject *obj, QEvent *event);

public:
    IpWhiteNameTable    *m_pView;
};

class IpWhiteNameTable : public QTableView
{
    Q_OBJECT
public:
    IpWhiteNameTable(QWidget *parent = Q_NULLPTR);
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
	IpWhiteItemModel *m_pModel;
	IpWhiteIconDelegate *m_pDelegate;
    QPixmap *m_pImageDel;
    IpTables *m_Stringdata;
	bool ischanged;
	bool firewallflag;
};

#endif // IPWHITETABLE_H
