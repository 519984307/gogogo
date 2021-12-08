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

#include "pubfunc.h"
#include "blacklistiptable.h"


IpItemModel::IpItemModel(QObject * parent)
    :QStandardItemModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

QVariant IpItemModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
            return QVariant();

    int column =index.column();
    int rows = m_pItems->size();
    int cur_row = index.row();

    //qDebug("ExtItemModel::data() column=%d row=%d", column, cur_row);
    if(role == Qt::BackgroundRole  && cur_row == m_nHoverRow)
    {
        return QColor(247,243,251);
    }
    if(role == Qt::TextAlignmentRole && (column == 0|| column == 1) && cur_row < rows){
        return QVariant(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    if(role==Qt::DisplayRole && cur_row < rows){
        switch(column){
        case 0:
//                ExtDisplayItem* pdata = (*m_pItems)[cur_row];
//                return pdata->m_sName;
                break;
        case 1:
                break;
        case 2:
                return *m_pImageDel;
                break;
        default:
            break;
        }
    }
    return QStandardItemModel::data(index,role);
}

void IpPortIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
    switch(cur_column){
    case 0:
//            QString sname = index.model()->data(index, Qt::DisplayRole).toString();
//            QStyleOptionViewItem myOption = option;
//            myOption.displayAlignment = Qt::AlignLeft| Qt::AlignVCenter;
//            drawDisplay(painter, myOption, myOption.rect, sname);
        break;
    case 1:
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


BlacklistIPTable::BlacklistIPTable(QWidget *parent)
    :QTableView(parent)
{
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(24);
    horizontalScrollBar()->setVisible(false);
    QFont font;
    font.setPixelSize(12);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef QT_NEW_HEADER_54
    font.setWeight(0);
#else
    font.setWeight(25);
#endif
#else //QT_VERSION
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
#endif //QT_VERSION
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

    m_pModel= new IpItemModel;
    m_pModel->setColumnCount(3);
    m_pModel->setRowCount(6);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("IP地址"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("端口"));
    m_pModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("操作"));
    m_pModel->SetData(&m_AllItems);

    m_pDelegate = new IpPortIconDelegate;
    m_pDelegate->SetView(this);

    setModel(m_pModel);
    setItemDelegate(m_pDelegate);

    setColumnWidth(0, 200);
    setColumnWidth(1, 40);
    setColumnWidth(2, 40);

    m_nCurrentRow = -1;

//    QString *pitem = new QString("test dir and child type");
//    m_AllItems.push_back(pitem);
//    QString *pitem1 = new QString("test xxxx");
//    m_AllItems.push_back(pitem1);

//    m_pModel->setItem(0, 0, new QStandardItem("test dir and child type"));
//    m_pModel->setItem(1, 0, new QStandardItem("test xxxx"));

}

void BlacklistIPTable::AddIpNameStart()
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
//    qDebug("BlacklistIPTable::AddExtName event row_count=%d recores=%d", row_count, rows);
    if((row_count-1) >= rows || row_count ==rows){
        m_pModel->setRowCount(row_count + 1);
    }
    QModelIndex index = m_pModel->index(rows, 0);
    if(index.isValid()){
        openPersistentEditor(index);
        setFocus();
        QWidget* editWidgetIP = indexWidget(index);
        //QWidget* editWidgetPort = indexWidget(indexport);
        if(editWidgetIP != NULL /*&& editWidgetPort != NULL*/){
            m_pWidget = editWidgetIP;
            editWidgetIP->setFocus();
            m_Index = index;
            QString *pitem = new QString("");
            m_AllItems.push_back(pitem);

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
            disconnect(editWidgetIP,SIGNAL(editingFinished()), this, SLOT(AddPortStart()));
            connect(editWidgetIP,SIGNAL(editingFinished()), this, SLOT(AddPortStart()));
#else
            disconnect(m_connIPEditEnd);
            m_connIPEditEnd = connect(editWidgetIP,SIGNAL(editingFinished()), this, SLOT(AddPortStart()));
#endif
            //
            //m_connPortEditEnd = connect(editWidgetPort,SIGNAL(editingFinished()), this, SLOT(AddIpPortEnd()));
        }
    }
}

void BlacklistIPTable::AddPortStart()
{
    int row_count = m_pModel->rowCount();
    closePersistentEditor(m_Index);
    int row = m_Index.row();

    QModelIndex index = m_pModel->index(row, 1);
    if(index.isValid()){
        openPersistentEditor(index);
        setFocus();
        QWidget* editWidget = indexWidget(index);
        if(editWidget != NULL){
            m_pWidget = editWidget;
            editWidget->setFocus();
            m_Index = index;
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
            disconnect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddIpPortEnd()));
            connect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddIpPortEnd()));
#else
            disconnect(m_connPortEditEnd);
            m_connPortEditEnd = connect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddIpPortEnd()));
#endif
        }
    }
}

void BlacklistIPTable::AddIpPortEnd()
{
    closePersistentEditor(m_Index);
    int row = m_Index.row();
    if(m_Index.column() == 2)
        return;
    QModelIndex indexip = m_pModel->index(row, 0);
    QModelIndex indexport = m_pModel->index(row, 1);
    closePersistentEditor(indexip);
    closePersistentEditor(indexport);
    QString textip = m_pModel->itemFromIndex(indexip)->text();
    if (textip.isEmpty()) {
        textip = "0.0.0.0";
    }
    QString textport = m_pModel->itemFromIndex(indexport)->text();
    m_Index = indexport;
    QString text = "";
    if (textport.isEmpty() || textport == "-") {
        text = textip;
    }else {
        text = textip + ":" + textport;
    }
    QVector<QString*>::iterator it = m_AllItems.begin();
    int i = 0;
    //string ip, port;
    while(it != m_AllItems.end()){
        if(i++ == row){
            QString *pdata = *it;
            if(text.isEmpty()){
                delete pdata;
                pdata = NULL;
                it = m_AllItems.erase(it);
                continue;
                //this->repaint();
            }else{
                *pdata = text;
//                qDebug("BlacklistIPTable::AddIpPortEnd() text=%s", text.toStdString().c_str());
            }
        }
        it++;
    }
}

int BlacklistIPTable::AddItem(QString sName)
{
    int row_count = m_pModel->rowCount();
    int rows = m_AllItems.size();
//    qDebug("BlacklistIPTable::AddItem row_count=%d rows=%d name=%s", row_count, rows, sName.toStdString().c_str());
    if(row_count == rows ){
        m_pModel->setRowCount(row_count + 1);
    }
    string ip, port;
    PubFunc::SplitIpString(sName.toStdString(), ip, port);
    if(port.size() == 0) {
        port = "-";
    }
    QString *pitem = new QString(sName);
    m_AllItems.push_back(pitem);
    m_pModel->setItem(rows, 0, new QStandardItem(QString::fromStdString(ip)));
    m_pModel->setItem(rows, 1, new QStandardItem(QString::fromStdString(port)));
    return 0;
}

void BlacklistIPTable::DelItem(int row)
{
    int i = 0;
    int row_count = m_pModel->rowCount();
    QVector<QString*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        if(i++ == row){
            //qDebug("BlacklistIPTable::DelItem i=%d  row=%d row_count=%d", i, row, row_count);
            QString *pitem = *it;
            delete pitem;
            pitem = NULL;
            m_AllItems.erase(it);
            m_pModel->removeRow(row);
            if(row_count == 6)
                m_pModel->setRowCount(6);

//            QVector<QString*>::iterator it1 = m_AllItems.begin();
//            int j = 0;
//            while(it1 != m_AllItems.end()){
//                QString *sdata = *it1;
//                qDebug() << j++ << "  " << *sdata ;
//                it1++;
//            }
            return;
        }
        it++;
    }
}

void BlacklistIPTable::UpdateRow(int row)
{
    if (row == m_nCurrentRow)
        return;

    m_pModel->SetHoverRow(row);
    //qDebug("BlacklistIPTable::UpdateRow row=%d  m_nCurrentRow=%d", row, m_nCurrentRow);
    for (int i = 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
        update(model()->index(row, i));
    }
    m_nCurrentRow = row;
}


bool BlacklistIPTable::HasIP(const QString &sIP)
{
    QVector<QString*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        QString *pitem = *it;
        if(pitem->compare(sIP) == 0)
            return true;
        it++;
    }
    return false;
}

bool BlacklistIPTable::event(QEvent *pEvent)
{
    if(pEvent->type() == QEvent::ToolTip){
        QPoint pt =  mapFromGlobal(QCursor::pos());
        int rows = m_AllItems.size();
        int row = this->rowAt(pt.y());
        int col = this->columnAt(pt.x());
        //qDebug("BlacklistIPTable::event ToolTip col=%d row=%d rows=%d", col, row, rows);
        if(col == 0 && row <= rows){
            //ShowToolTip(row);
        }
    }/*else if(pEvent->type() == QEvent::KeyPress){
        int row;
        QModelIndex indexport;
        QKeyEvent *pkevent = (QKeyEvent*)pEvent;
        qDebug("bbbbbbbbbbbbbbb BlacklistIPTable::event key=%02X row:%d col:%d", pkevent->key(), m_Index.row(), m_Index.column());
        if(Qt::Key_Tab == pkevent->key() && m_Index.column() == 0){
              closePersistentEditor(m_Index);
            row = m_Index.row();
            indexport = m_pModel->index(row, 1);
            disconnect(m_connIPEditEnd);
            if(indexport.isValid()){
                openPersistentEditor(indexport);
                QWidget* editWidget = indexWidget(indexport);
                if(editWidget != NULL){
                    m_pWidget = editWidget;
                    editWidget->setFocus();
                    m_Index = indexport;
                    m_connPortEditEnd = connect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddIpPortEnd()));
                }
            }
            return true;
        }
        QTableView::keyPressEvent(pkevent);
        return false;
    }*/
    return QTableView::event(pEvent);
}

void BlacklistIPTable::ShowToolTip(int )
{
//    ExtDisplayItem *pdt = m_AllItems[row-1];
//    QString stip = u8"类型：";
//    stip += APP_MENU_WHITELIST_TYPE[pdt->m_nType];
//    stip += "\n";
//    stip += u8"路径：";
//    stip += pdt->m_sName;

//    QToolTip::showText(QCursor::pos(), stip);
}

void BlacklistIPTable::mousePressEvent(QMouseEvent *event)
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
        //qDebug("BlacklistIPTable::mousePressEvent col:%d row:%d", index.column(), index.row());
    }
}

void BlacklistIPTable::mouseMoveEvent(QMouseEvent *event)
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

void BlacklistIPTable::leaveEvent (QEvent * )
{
    m_pModel->SetHoverRow(-1);
    int column_count= model()->columnCount();
    for (int i = column_count- 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
    }
    m_nCurrentRow = -1;
}

//void BlacklistIPTable::keyPressEvent(QKeyEvent *event)
//{
//    int row;
//    QModelIndex indexport;
//    qDebug("BlacklistIPTable::keyPressEvent key=%02X row:%d col:%d", event->key(), m_Index.row(), m_Index.column());
//    if(Qt::Key_Tab == event->key() && m_Index.column() == 0){

//        closePersistentEditor(m_Index);
//        row = m_Index.row();
//        indexport = m_pModel->index(row, 1);
//        disconnect(m_connIPEditEnd);
//        if(indexport.isValid()){
//            openPersistentEditor(indexport);
//            QWidget* editWidget = indexWidget(indexport);
//            if(editWidget != NULL){
//                m_pWidget = editWidget;
//                editWidget->setFocus();
//                m_Index = indexport;
//                m_connPortEditEnd = connect(editWidget,SIGNAL(editingFinished()), this, SLOT(AddIpPortEnd()));
//            }
//        }
//        return;
//    }
//    return QTableView::keyPressEvent(event);
//}

void BlacklistIPTable::CellClicked(const QModelIndex &index)
{
    //qDebug("BlacklistIPTable::cellClicked %d %d", index.column(), index.row());
    int col = index.column();
    int row = index.row();

    if(col != 2 || row < 0)
        return;
    DelItem(row);
}
