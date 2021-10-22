#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QVariant>
#include <QColor>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QEvent>
#include <QDebug>
#include <QVariant>

#include "centerdiscoverytable.h"
#include "common/pubdef.h"

CenterDiscoveryItemModel::CenterDiscoveryItemModel(QObject * parent)
    :QStandardItemModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

QVariant CenterDiscoveryItemModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
            return QVariant();

    int column=index.column();
    int rows = m_pItems->size();
    int cur_row = index.row();

    //qDebug("CenterDiscoveryItemModel::data() column=%d row=%d", column, cur_row);
    if(role == Qt::BackgroundRole  && cur_row == m_nHoverRow)
    {
        return QColor(247,243,251);
    }
    if(role == Qt::TextAlignmentRole && column == 0 && cur_row < rows){
        return QVariant(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    if(role==Qt::DisplayRole && cur_row < rows){
        CenterDiscoveryTableItem* pdata = (*m_pItems)[cur_row];
        if (pdata == NULL || index.row() > rows)
            return QStandardItemModel::data(index,role);;
        switch(column){
        case 0:{
                QString center_display_to_display = pdata->center_address;
                QVariant vret = QVariant::fromValue(center_display_to_display);
                return vret;
            }
            break;
        case 1:{
                return *m_pImageDel;
                break;
            }
            break;
        default:
            break;
        }
    }
    return QStandardItemModel::data(index,role);
}

void CenterDiscoveryIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
    switch(cur_column){
    case 0:
        break;
    case 1:
        if(index.data().canConvert<QPixmap>()){
            QItemDelegate::paint(painter, option, index);

            QPixmap imgDel = qvariant_cast<QPixmap>(index.data());
            int x = option.rect.x() + option.rect.width()/2 - imgDel.width()/2;
            int y = option.rect.y() + option.rect.height()/2 - imgDel.height()/2;

            painter->drawPixmap(x,y,imgDel);
            return;
        }
        break;
    default:
        break;
    }
    QItemDelegate::paint(painter, option, index);
}


CenterDiscoveryTable::CenterDiscoveryTable(QWidget *parent)
    :QTableView(parent)
{
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(24);
    horizontalScrollBar()->setVisible(false);

    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);
    setStyleSheet("QTableView {\
                  selection-background-color: rgb(236,229,243);\
                  selection-color: black;\
              }");
    setFrameShape(QFrame::NoFrame);
    setSortingEnabled(false);
    setEditTriggers(QAbstractItemView::DoubleClicked);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);

    m_pModel= new CenterDiscoveryItemModel;
    m_pModel->setColumnCount(2);
    m_pModel->setRowCount(6);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("地址"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("操作"));
    m_pModel->SetData(&m_AllItems);

    m_pDelegate = new CenterDiscoveryIconDelegate;
    m_pDelegate->SetView(this);

    setModel(m_pModel);
    setItemDelegate(m_pDelegate);

    setColumnWidth(0, 510);
    setColumnWidth(1, 40);

    m_nCurrentRow = -1;

//    QString *pitem = new QString("test dir and child type");
//    m_AllItems.push_back(pitem);
//    QString *pitem1 = new QString("test xxxx");
//    m_AllItems.push_back(pitem1);

//    m_pModel->setItem(0, 0, new QStandardItem("test dir and child type"));
//    m_pModel->setItem(1, 0, new QStandardItem("test xxxx"));

}

void CenterDiscoveryTable::AddCenterStart()
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
    qDebug("CenterDiscoveryTable::AddCenterStart event row_count=%d recores=%d", row_count, rows);
    if((row_count-1) == rows ){
        m_pModel->setRowCount(row_count + 1);
    }

    QModelIndex index = m_pModel->index(rows, 0);
    if(index.isValid()){
        openPersistentEditor(index);
        setFocus();
        QWidget* editWidget = indexWidget(index);
        if(editWidget != NULL){
            m_pWidget = editWidget;
            editWidget->setFocus();
            m_Index = index;
            CenterDiscoveryTableItem *pitem = new CenterDiscoveryTableItem("");
            m_AllItems.push_back(pitem);
            connect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddCenterEnd()));
        }
    }
}

void CenterDiscoveryTable::AddCenterEnd()
{
    qDebug("CenterDiscoveryTable::AddCenterEnd m_Index=%d,%d", m_Index.row(), m_Index.column());

    closePersistentEditor(m_Index);
    int row = m_Index.row();
    qDebug() << "CenterDiscoveryTable::AddCenterEnd row=" << m_Index.row();
    QString text = m_pModel->itemFromIndex(m_Index)->text();

    QVector<CenterDiscoveryTableItem*>::iterator it = m_AllItems.begin();
    int i = 0;
    while(it != m_AllItems.end()){
        if(i++ == row){
            CenterDiscoveryTableItem *pdata = *it;
            if(text.isEmpty()){
                delete pdata;
                pdata = NULL;
                it = m_AllItems.erase(it);
                continue;
            }else{
                pdata->center_address = text;
            }
        }
        it++;
    }


    QVector<CenterDiscoveryTableItem*>::iterator it1 = m_AllItems.begin();
    int j = 0;
    while(it1 != m_AllItems.end()){
        CenterDiscoveryTableItem *pdata = *it1;
        qDebug() << j++ << "  " << pdata->center_address;
        it1++;
    }
    //update();
}

int CenterDiscoveryTable::AddItem(QString sAddress, bool official)
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
    qDebug("CenterDiscoveryTable::AddItem row_count=%d rows=%d content=%s", row_count, rows, sAddress.toStdString().c_str());
    if((row_count-1) == rows ){
        m_pModel->setRowCount(row_count + 1);
    }
    CenterDiscoveryTableItem *pitem = new CenterDiscoveryTableItem(sAddress, official);
    m_AllItems.push_back(pitem);
    m_pModel->setItem(rows, 0, new QStandardItem(sAddress));
    return 0;
}

int CenterDiscoveryTable::DelItem(int row)
{
    int i = 0;
    int row_count = m_pModel->rowCount();
    QVector<CenterDiscoveryTableItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        if(i++ == row){
            qDebug("CenterDiscoveryTable::DelItem i=%d  row=%d row_count=%d", i, row, row_count);
            CenterDiscoveryTableItem *pitem = *it;
            if (pitem->is_official) {
                return -1; //it's the command from server's config
            }
            delete pitem;
            pitem = NULL;
            m_AllItems.erase(it);
            m_pModel->removeRow(row);
            if(row_count == 6) {
                m_pModel->setRowCount(6);
            }
//            QVector<CenterDiscoveryTableItem*>::iterator it1 = m_AllItems.begin();
//            int j = 0;
//            while(it1 != m_AllItems.end()){
//                CenterDiscoveryTableItem *sdata = *it1;
//                qDebug() << j++ << "  " << (*sdata).source_name ;
//                it1++;
//            }
            return 0;
        }
        it++;
    }
    return -1;
}

void CenterDiscoveryTable::ClearAllItem()
{
    int i = 0;
    //int row_count = m_pModel->rowCount();
    QVector<CenterDiscoveryTableItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        CenterDiscoveryTableItem *pitem = *it;
        delete pitem;
        pitem = NULL;
        m_pModel->removeRow(i++);
        it = m_AllItems.erase(it);
    }
}

void CenterDiscoveryTable::UpdateRow(int row)
{
    if (row == m_nCurrentRow)
        return;

    m_pModel->SetHoverRow(row);
    //qDebug("CenterDiscoveryTable::UpdateRow row=%d  m_nCurrentRow=%d", row, m_nCurrentRow);
    for (int i = 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
        update(model()->index(row, i));
    }
    QStandardItem *item = m_pModel->item(row, 0);
    if (item) {
        item->setForeground(QBrush(QColor(0, 0, 0)));
        m_nCurrentRow = row;
    }
}


int CenterDiscoveryTable::GetAllItem(vector<CenterDiscoveryTableItem> * vData)
{
    int ncount = 0;
    QVector<CenterDiscoveryTableItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        CenterDiscoveryTableItem *pitem = *it;
        qDebug("CenterDiscoveryTable::GetAllItem content=%s", pitem->center_address.toStdString().c_str());
        vData->push_back(*pitem);
        ncount++;
        it++;
    }
    return ncount;
}


bool CenterDiscoveryTable::event(QEvent *pEvent)
{
    if(pEvent->type() == QEvent::ToolTip){
//        QPoint pt =  mapFromGlobal(QCursor::pos());
//        int rows = m_AllItems.size();
//        int row = this->rowAt(pt.y());
//        int col = this->columnAt(pt.x());
//        //qDebug("CenterDiscoveryTable::event ToolTip col=%d row=%d rows=%d", col, row, rows);
//        if(col == 0 && row <= rows){
//            ShowToolTip(row);
//        }
    }
    return QTableView::event(pEvent);
}

void CenterDiscoveryTable::ShowToolTip(int )
{
//    ExtDisplayItem *pdt = m_AllItems[row-1];
//    QString stip = u8"类型：";
//    stip += APP_MENU_WHITELIST_TYPE[pdt->m_nType];
//    stip += "\n";
//    stip += u8"路径：";
//    stip += pdt->m_sName;

//    QToolTip::showText(QCursor::pos(), stip);
}

//void CenterDiscoveryTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
//{
//    qDebug("CenterDiscoveryTable::dataChanged topLeft=%d,%d bottonRight=%d,%d", topLeft.row(), topLeft.column(), bottomRight.row(), bottomRight.column());
//    int i;
//    for( i = 0; i < roles.size(); i++ ){
//        qDebug("CenterDiscoveryTable::dataChanged role=%d", roles.at(i));
//    }
//}

void CenterDiscoveryTable::mousePressEvent(QMouseEvent *event)
{
    int rows = m_AllItems.size();
    int row = this->rowAt(event->y());
    if(row >= rows){
        return;
    }else{
        setCurrentIndex(QModelIndex());
        QTableView::mousePressEvent(event);
        QModelIndex index = currentIndex();
        if(event->button() == Qt::LeftButton){
            CellClicked(index);
        }
        qDebug("CenterDiscoveryTable::mousePressEvent col:%d row:%d", index.column(), index.row());
    }
}

void CenterDiscoveryTable::mouseMoveEvent(QMouseEvent *event)
{
    int rows = m_AllItems.size();
    int column=this->columnAt(event->x());
    int row=this->rowAt(event->y());

    if(column == 1 && row < rows){
        this->setCursor(Qt::PointingHandCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    if(row < rows)
        UpdateRow(row);
}

void CenterDiscoveryTable::leaveEvent (QEvent * )
{
    qDebug("CenterDiscoveryTable::leaveEvent");
    m_pModel->SetHoverRow(-1);
    int column_count= model()->columnCount();
    for (int i = column_count- 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
    }
    m_nCurrentRow = -1;
}


void CenterDiscoveryTable::CellClicked(const QModelIndex &index)
{
    qDebug("CenterDiscoveryTable::cellClicked %d %d", index.column(), index.row());
    int col = index.column();
    int row = index.row();

    if(col != 1 || row < 0)
        return;
    DelItem(row);
}
