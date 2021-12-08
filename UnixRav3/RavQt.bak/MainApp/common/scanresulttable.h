#ifndef DIRFILETABLE_H
#define DIRFILETABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QMap>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>

#include "common.h"

class DFDisplayItem;
class ResultItemModel;
class DFIconDelegate;
class DirFileTable;
class ScanResultTable;

class DFDisplayItem{
public:
    DFDisplayItem(QString sVName, QString sVType, QString sFPath, QString sStatus)
        :m_sVirusName(sVName),m_sVirusType(sVType),m_sFilePath(sFPath),m_sStatus(sStatus)
    {

    }

    void SetIndex(int nIndex)
    {
        m_nIndex=nIndex;
    }

public:
    int m_nIndex;
    QString m_sVirusName;
    QString m_sVirusType;
    QString m_sFilePath;
    QString m_sStatus;
};

class ResultItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    ResultItemModel(QObject * parent=0);
    virtual ~ ResultItemModel(){}

    void SetData(QMap<int, DFDisplayItem*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;

    //QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public:
    int m_nHoverRow;

    QMap<int, DFDisplayItem*>  *m_pItems;
    QPixmap * m_pImageDel;
};

class DFIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DFIconDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(ScanResultTable *pView){m_pView=pView;}

    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
public:
    ScanResultTable    *m_pView;
};


class ScanResultTable : public QTableView
{
    Q_OBJECT
public:
    ScanResultTable(QWidget *parent = Q_NULLPTR);

    int AddItem(QString sVName, QString sVType, QString sFPath, QString sStatus);
    void DelItem(int row);
    void DelAllItems();

    void UpdateRow(int row);

public slots:

    void ShowToolTip(int row);

protected:
    bool event(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *e);
    //void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);
//    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void CellClicked(const QModelIndex &index);


public:
    QMap<int, DFDisplayItem*>  m_AllItems;

    ResultItemModel *m_pModel;
    DFIconDelegate    *m_pDelegate;

    int m_nCurrentRow;
};

#endif // DIRFILETABLE_H
