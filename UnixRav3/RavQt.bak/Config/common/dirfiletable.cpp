#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QVariant>
#include <QColor>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QEvent>


#include "dirfiletable.h"
#include "common/pubdef.h"


DirItemModel::DirItemModel(QObject * parent)
    :QStandardItemModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

QVariant DirItemModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
            return QVariant();

    int column=index.column();
    int rows = m_pItems->size();
    int cur_row = index.row();

    //qDebug("DirItemModel::data() column=%d row=%d", column, cur_row);
    if(role == Qt::BackgroundRole  && cur_row == m_nHoverRow)
    {
        return QColor(247,243,251);
    }
    if(role == Qt::ToolTipRole && column == 0 && cur_row < rows){
        //QToolTip::showText(QCursor::pos(), file_name);
    }
    if(role==Qt::DisplayRole && cur_row < rows){
        switch(column){
        case 0:{
//                DFDisplayItem* pdata = (*m_pItems)[cur_row];
//                return pdata->m_sName;
                break;
            }
        case 1:{
                return *m_pImageDel;
                break;
            }
        default:
            break;
        }
    }
    return QStandardItemModel::data(index,role);
}

void DFIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
    switch(cur_column){
    case 0:{
//            QString sname = index.model()->data(index, Qt::DisplayRole).toString();
//            QStyleOptionViewItem myOption = option;
//            myOption.displayAlignment = Qt::AlignLeft| Qt::AlignVCenter;
//            drawDisplay(painter, myOption, myOption.rect, sname);
        }
        break;
    case 1:{
            if(index.data().canConvert<QPixmap>()){
                QItemDelegate::paint(painter, option, index);

                QPixmap imgDel = qvariant_cast<QPixmap>(index.data());
                int x = option.rect.x() + option.rect.width()/2 - imgDel.width()/2;
                int y = option.rect.y() + option.rect.height()/2 - imgDel.height()/2;

                painter->drawPixmap(x,y,imgDel);
                return;
            }
        }
        break;
    default:
        break;
    }
    QItemDelegate::paint(painter, option, index);
}


DirFileTable::DirFileTable(QWidget *parent)
    :QTableView(parent)
{
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(24);
    horizontalScrollBar()->setVisible(false);

#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    setStyleSheet("QTableView {\
                  selection-background-color: rgb(236,229,243);\
                  selection-color: black;\
              }");
    setFrameShape(QFrame::NoFrame);
    setSortingEnabled(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);

    m_pModel= new DirItemModel;
    m_pModel->setColumnCount(2);
    m_pModel->setRowCount(6);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("文件/目录"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("操作"));
    m_pModel->SetData(&m_AllItems);

    m_pDelegate = new DFIconDelegate;
    m_pDelegate->SetView(this);

    setModel(m_pModel);
    setItemDelegate(m_pDelegate);

    setColumnWidth(0, 240);
    setColumnWidth(1, 40);

    m_nCurrentRow = -1;

//    DFDisplayItem *pitem = new DFDisplayItem(APP_MENU_DIRSEL_TYPE_DIR_CHILD, "test dir and child type");
//    m_AllItems[0] = pitem;
//    DFDisplayItem *pitem1 = new DFDisplayItem(APP_MENU_DIRSEL_TYPE_DIR_CHILD, "test xxxx");
//    m_AllItems[1] = pitem1;

//    m_pModel->setItem(0, 0, new QStandardItem(pitem->m_sName));
//    m_pModel->setItem(1, 0, new QStandardItem(pitem1->m_sName));

}

int DirFileTable::AddItem(int nType, QString sName)
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
    //qDebug("DirFileTable::AddItem row_count=%d rows=%d", row_count, rows);
    if((row_count-1) == rows ){
        m_pModel->setRowCount(row_count + 1);
    }
    DFDisplayItem *pitem = new DFDisplayItem(nType, sName);
    m_AllItems[rows] = pitem;
    m_pModel->setItem(rows, 0, new QStandardItem(sName));
    m_pModel->item(rows, 0)->setForeground(QBrush(QColor(0, 0, 0)));
    return 0;
}

void DirFileTable::DelItem(int row)
{
    int i = 0;
    int row_count = m_pModel->rowCount();
    QMap<int,DFDisplayItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        if(i++ == row){
            //qDebug("DirFileTable::DelItem i=%d  row=%d row_count=%d", i, row, row_count);
            DFDisplayItem *pitem = *it;
            delete pitem;
            pitem = NULL;
            m_AllItems.erase(it);
            m_pModel->removeRow(row);
            if(row_count == 6)
                m_pModel->setRowCount(6);
            return;
        }
        it++;
    }
}

void DirFileTable::ClearAllItem()
{
    int i = 0;
    //int row_count = m_pModel->rowCount();
    QMap<int,DFDisplayItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        //qDebug("DirFileTable::DelItem i=%d  row=%d row_count=%d", i, row, row_count);
        DFDisplayItem *pitem = *it;
        delete pitem;
        pitem = NULL;
        m_pModel->removeRow(i++);

        it = m_AllItems.erase(it);
    }
}

int DirFileTable::GetAllItem(vector<DfItemData> * vData)
{
    int ncount = 0;
    QMap<int,DFDisplayItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        DFDisplayItem *pitem = *it;
        DfItemData did(pitem->m_nType, pitem->m_sName.toStdString().c_str());
        vData->push_back(did);
        ncount++;
        it++;
    }
    return ncount;
}

void DirFileTable::UpdateRow(int row)
{
    if (row == m_nCurrentRow)
        return;

    m_pModel->SetHoverRow(row);
    //qDebug("DirFileTable::UpdateRow row=%d  m_nCurrentRow=%d", row, m_nCurrentRow);
    for (int i = 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
        update(model()->index(row, i));
    }
    m_nCurrentRow = row;
}

bool DirFileTable::event(QEvent *pEvent)
{
    if(pEvent->type() == QEvent::ToolTip){
        QPoint pt =  mapFromGlobal(QCursor::pos());
        int rows = m_AllItems.size();
        int row = this->rowAt(pt.y());
        int col = this->columnAt(pt.x());
        //qDebug("DirFileTable::event ToolTip col=%d row=%d rows=%d", col, row, rows);
        if(col == 0 && row <= rows){
            ShowToolTip(row);
        }
    }
    return QTableView::event(pEvent);
}

void DirFileTable::ShowToolTip(int row)
{
    if(row < 0)
        return;
    DFDisplayItem *pdt = m_AllItems[row-1];
    QString stip = u8"类型：";
    stip += APP_MENU_WHITELIST_TYPE[pdt->m_nType];
    stip += "\n";
    stip += u8"路径：";
    stip += pdt->m_sName;

    QToolTip::showText(QCursor::pos(), stip);
}

void DirFileTable::mousePressEvent(QMouseEvent *event)
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
        //qDebug("DirFileTable::mousePressEvent col:%d row:%d", index.column(), index.row());
    }
}

void DirFileTable::mouseMoveEvent(QMouseEvent *event)
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

void DirFileTable::leaveEvent (QEvent * )
{
    m_pModel->SetHoverRow(-1);
    int column_count= model()->columnCount();
    for (int i = column_count- 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
    }
    m_nCurrentRow = -1;
}


void DirFileTable::CellClicked(const QModelIndex &index)
{
    //qDebug("DirFileTable::cellClicked %d %d", index.column(), index.row());
    int col = index.column();
    int row = index.row();

    if(col != 1 || row < 0)
        return;
    DelItem(row);
}
