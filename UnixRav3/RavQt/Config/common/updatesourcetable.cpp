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

#include "updatesourcetable.h"
#include "common/pubdef.h"

UpdateSourceItemModel::UpdateSourceItemModel(QObject * parent)
    :QStandardItemModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

QVariant UpdateSourceItemModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
            return QVariant();

    int column=index.column();
    int rows = m_pItems->size();
    int cur_row = index.row();

    //qDebug("UpdateSourceItemModel::data() column=%d row=%d", column, cur_row);
    if(role == Qt::BackgroundRole  && cur_row == m_nHoverRow)
    {
        return QColor(247,243,251);
    }
    if(role == Qt::TextAlignmentRole && column == 0 && cur_row < rows){
        return QVariant(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    if(role==Qt::DisplayRole && cur_row < rows){
        UpdateTableItem* pdata = (*m_pItems)[cur_row];
        if (pdata == NULL || index.row() > rows)
            return QStandardItemModel::data(index,role);;
        switch(column){
        case 0:{
                QString sret = pdata->is_from_cloud_rules?u8"是":u8"否";
                return QVariant::fromValue(sret);
            }
            break;
        case 1:{
                QString source_name_to_display;
                if ( pdata->source_name == APP_RISING_UPDATE_WEB_MAIN) {
                    source_name_to_display = u8"瑞星官网";
                } else {
                    source_name_to_display = pdata->source_name;
                }
                QVariant vret = QVariant::fromValue(source_name_to_display);//(UpdateSourceTable(pdata->source_name, pdata->is_from_cloud_rules));
                return vret;
            }
            break;
        case 2:{
            if (pdata->is_from_cloud_rules) {
                return "";
            } else {
                return *m_pImageDel;
            }
            }
            break;
        default:
            break;
        }
    }
    return QStandardItemModel::data(index,role);
}

void UpdateSourceIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
    switch(cur_column){
    case 0:
        break;
    case 1:{
        //            QString sname = index.model()->data(index, Qt::DisplayRole).toString();
        //            QStyleOptionViewItem myOption = option;
        //            myOption.displayAlignment = Qt::AlignLeft| Qt::AlignVCenter;
        //            drawDisplay(painter, myOption, myOption.rect, sname);
        }
        break;
    case 2:
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


UpdateSourceTable::UpdateSourceTable(QWidget *parent)
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

    m_pModel= new UpdateSourceItemModel;
    m_pModel->setColumnCount(3);
    m_pModel->setRowCount(6);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("云端下发"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("其它升级中心"));
    m_pModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("操作"));
    m_pModel->SetData(&m_AllItems);

    m_pDelegate = new UpdateSourceIconDelegate;
    m_pDelegate->SetView(this);

    setModel(m_pModel);
    setItemDelegate(m_pDelegate);

    setColumnWidth(0, 60);
    setColumnWidth(1, 460);
    setColumnWidth(2, 40);

    m_nCurrentRow = -1;

//    QString *pitem = new QString("test dir and child type");
//    m_AllItems.push_back(pitem);
//    QString *pitem1 = new QString("test xxxx");
//    m_AllItems.push_back(pitem1);

//    m_pModel->setItem(0, 0, new QStandardItem("test dir and child type"));
//    m_pModel->setItem(1, 0, new QStandardItem("test xxxx"));

}

void UpdateSourceTable::AddSourceStart()
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
    qDebug("UpdateSourceTable::AddSourceStart event row_count=%d recores=%d", row_count, rows);
    if((row_count-1) == rows ){
        m_pModel->setRowCount(row_count + 1);
    }

    QModelIndex index = m_pModel->index(rows, 1);
    if(index.isValid()){
        openPersistentEditor(index);
        setFocus();
        QWidget* editWidget = indexWidget(index);
        if(editWidget != NULL){
            m_pWidget = editWidget;
            editWidget->setFocus();
            m_Index = index;
            UpdateTableItem *pitem = new UpdateTableItem("", 0);
            m_AllItems.push_back(pitem);
            connect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddSourceEnd()));
        }
    }
}

void UpdateSourceTable::AddSourceEnd()
{
    qDebug("UpdateSourceTable::AddSourceEnd m_Index=%d,%d", m_Index.row(), m_Index.column());

    //closePersistentEditor(m_Index);
    int row = m_Index.row();
    qDebug() << "UpdateSourceTable::AddSourceEnd row=" << m_Index.row();
    QString text = m_pModel->itemFromIndex(m_Index)->text();

    QVector<UpdateTableItem*>::iterator it = m_AllItems.begin();
    int i = 0;
    while(it != m_AllItems.end()){
        if(i++ == row){
            UpdateTableItem *pdata = *it;
            if(text.isEmpty()){
                delete pdata;
                pdata = NULL;
                it = m_AllItems.erase(it);
                continue;
            }else{
                pdata->source_name = text;
            }
        }
        it++;
    }


//    QVector<UpdateTableItem*>::iterator it1 = m_AllItems.begin();
//    int j = 0;
//    while(it1 != m_AllItems.end()){
//        UpdateTableItem *pdata = *it1;
//        qDebug() << j++ << "  " << pdata->source_name ;
//        it1++;
//    }
    //update();
}

int UpdateSourceTable::AddItem(QString sName, bool is_cloud_rule)
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
    qDebug("UpdateSourceTable::AddItem row_count=%d rows=%d content=%s", row_count, rows, sName.toStdString().c_str());
    if((row_count-1) == rows ){
        m_pModel->setRowCount(row_count + 1);
    }
    UpdateTableItem *pitem = new UpdateTableItem(sName, is_cloud_rule);
    m_AllItems.push_back(pitem);
    m_pModel->setItem(rows, 1, new QStandardItem(sName));
    return 0;
}

void UpdateSourceTable::DelItem(int row)
{
    int i = 0;
    int row_count = m_pModel->rowCount();
    QVector<UpdateTableItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        if(i++ == row){
            qDebug("UpdateSourceTable::DelItem i=%d  row=%d row_count=%d", i, row, row_count);
            UpdateTableItem *pitem = *it;
            if (pitem->is_from_cloud_rules) {
                qDebug("%s from cloud rules, can't be deleted!", pitem->source_name.toStdString().c_str());
                return;
            }
            delete pitem;
            pitem = NULL;
            m_AllItems.erase(it);
            m_pModel->removeRow(row);
            if(row_count == 6)
                m_pModel->setRowCount(6);

//            QVector<UpdateTableItem*>::iterator it1 = m_AllItems.begin();
//            int j = 0;
//            while(it1 != m_AllItems.end()){
//                UpdateTableItem *sdata = *it1;
//                qDebug() << j++ << "  " << (*sdata).source_name ;
//                it1++;
//            }
            return;
        }
        it++;
    }
}

void UpdateSourceTable::ClearAllItem()
{
    int i = 0;
    //int row_count = m_pModel->rowCount();
    QVector<UpdateTableItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        UpdateTableItem *pitem = *it;
        delete pitem;
        pitem = NULL;
        m_pModel->removeRow(i++);
        it = m_AllItems.erase(it);
    }
}

void UpdateSourceTable::UpdateRow(int row)
{
    if (row == m_nCurrentRow)
        return;

    m_pModel->SetHoverRow(row);
    //qDebug("UpdateSourceTable::UpdateRow row=%d  m_nCurrentRow=%d", row, m_nCurrentRow);
    for (int i = 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
        update(model()->index(row, i));
    }
    m_nCurrentRow = row;
}


int UpdateSourceTable::GetAllItem(vector<UpdateTableItem> * vData)
{
    int ncount = 0;
    QVector<UpdateTableItem*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        UpdateTableItem *pitem = *it;
        qDebug("UpdateSourceTable::GetAllItem content=%s", pitem->source_name.toStdString().c_str());
        vData->push_back(*pitem);
        ncount++;
        it++;
    }
    return ncount;
}


bool UpdateSourceTable::event(QEvent *pEvent)
{
    if(pEvent->type() == QEvent::ToolTip){
        QPoint pt =  mapFromGlobal(QCursor::pos());
        int rows = m_AllItems.size();
        int row = this->rowAt(pt.y());
        int col = this->columnAt(pt.x());
        //qDebug("UpdateSourceTable::event ToolTip col=%d row=%d rows=%d", col, row, rows);
        if(col == 0 && row <= rows){
            ShowToolTip(row);
        }
    }
    return QTableView::event(pEvent);
}

void UpdateSourceTable::ShowToolTip(int )
{
//    ExtDisplayItem *pdt = m_AllItems[row-1];
//    QString stip = u8"类型：";
//    stip += APP_MENU_WHITELIST_TYPE[pdt->m_nType];
//    stip += "\n";
//    stip += u8"路径：";
//    stip += pdt->m_sName;

//    QToolTip::showText(QCursor::pos(), stip);
}

void UpdateSourceTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    qDebug("UpdateSourceTable::dataChanged topLeft=%d,%d bottonRight=%d,%d", topLeft.row(), topLeft.column(), bottomRight.row(), bottomRight.column());
    int i;
    for( i = 0; i < roles.size(); i++ ){
        qDebug("UpdateSourceTable::dataChanged role=%d", roles.at(i));
    }
}

void UpdateSourceTable::mousePressEvent(QMouseEvent *event)
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
        qDebug("UpdateSourceTable::mousePressEvent col:%d row:%d", index.column(), index.row());
    }
}

void UpdateSourceTable::mouseMoveEvent(QMouseEvent *event)
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

void UpdateSourceTable::leaveEvent (QEvent * )
{
    qDebug("UpdateSourceTable::leaveEvent");
    m_pModel->SetHoverRow(-1);
    int column_count= model()->columnCount();
    for (int i = column_count- 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
    }
    m_nCurrentRow = -1;
}


void UpdateSourceTable::CellClicked(const QModelIndex &index)
{
    qDebug("UpdateSourceTable::cellClicked %d %d", index.column(), index.row());
    int col = index.column();
    int row = index.row();

    if(col != 2 || row < 0)
        return;
    DelItem(row);
}
