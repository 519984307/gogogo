#ifndef PRIVATECLOUDTABLE_H
#define PRIVATECLOUDTABLE_H

#include <string>

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPixmap>
#include <QVector>
#include <QItemDelegate>
#include <QAbstractTableModel>
#include <QPainter>
#include <QMouseEvent>
#include <QSpinBox>

using namespace  std;

class PrivateCloudTable;


class CloudParam{
public:
    CloudParam(bool selected, QString &cname, QString &caddress, int cport){
        is_selected = selected;
        name = cname;
        address = caddress;
        port = cport;
    }

    bool is_selected;
    QString name;
    QString address;
    int port;
};

class CloudParamModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    CloudParamModel(QObject * parent=0);
    virtual ~ CloudParamModel();

    void SetData(QVector<CloudParam*> *pData){
        m_pItems = pData;
    }

    void SetHoverRow(int row){
        m_nHoverRow = row;
    }

    int rowCount(const QModelIndex &) const{
        if (m_pItems->size() < 6)
            return 6;
        return m_pItems->size();
    }

    int rowCount() const{
        if (m_pItems->size() < 6)
            return 6;
        return m_pItems->size();
    }

    int columnCount(const QModelIndex &) const{
        return 5;
    }

    void updateData() {
        beginResetModel();
        endResetModel();
    }

    void updateData(int row);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;

public:
    int m_nHoverRow;

    QVector<CloudParam*> *m_pItems;
    QPixmap * m_pImageDel;
};

class CloudParamDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CloudParamDelegate(QObject *parent = NULL): QItemDelegate  (parent) { }
    void SetView(PrivateCloudTable *pView){m_pView=pView;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const;

public:
    PrivateCloudTable    *m_pView;
};

class PrivateCloudTable : public QTableView
{
    Q_OBJECT
public:
    PrivateCloudTable(QWidget *parent = Q_NULLPTR);
    virtual ~PrivateCloudTable();

    void AddCloudStart();

    int AddItem(bool is_selected, QString name, QString address, int port);
    int AddItem(bool is_selected, QString name, QString address, QString port);
    void DelItem(int row);

    void UpdateRow(int row);

    bool HasAddressName(const QString & name);

public slots:
    void CellEditFinished();

signals:
    void leftDoubleClicked(const QModelIndex &index);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void leaveEvent (QEvent * event);

    void CellClicked(const QModelIndex &index);
    void CellDoubleClicked(const QModelIndex index);

public:
    QVector<CloudParam*>    m_AllItems;
    CloudParamModel         m_Model;
    CloudParamDelegate      *m_pDelegate;

    int m_nCurrentRow;
    QModelIndex m_IndexPrevEdited;
};

#endif // PRIVATECLOUDTABLE_H
