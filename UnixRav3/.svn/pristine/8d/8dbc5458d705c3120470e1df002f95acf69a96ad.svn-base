#include "privatecloudtable.h"
//#include "pubfunc.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QLineEdit>
#include <QVariant>
#include <QColor>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QEvent>
#include <QCheckBox>
#include <QDebug>


CloudParamModel::CloudParamModel(QObject * parent)
    :QAbstractTableModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

CloudParamModel::~ CloudParamModel()
{
    delete m_pImageDel;
    m_pImageDel = NULL;
}

QVariant CloudParamModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter| Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return QString::fromLocal8Bit("启用");
                break;
            case 1:
                return QString::fromLocal8Bit("名称");
                break;
            case 2:
                return QString::fromLocal8Bit("地址");
                break;
            case 3:
                return QString::fromLocal8Bit("端口");
                break;
            case 4:
                return QString::fromLocal8Bit("操作");
                break;
            default:
                break;
            }
        }
    }
    default:
        return QVariant();
    }
    return QVariant();
}

bool CloudParamModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if(!index.isValid())
        return false;
    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        if (value == Qt::Checked)
        {
            if((*m_pItems).size() > index.row()) {
                (*m_pItems)[index.row()]->is_selected = true;
            }
        }
        else
        {
            if((*m_pItems).size() > index.row()) {
                (*m_pItems)[index.row()]->is_selected = false;
            }
        }
    }
    return true;
}

Qt::ItemFlags CloudParamModel::flags( const QModelIndex &index ) const
{
    if(!index.isValid())
        return 0;

    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant CloudParamModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
            return QVariant();

    int column =index.column();
    int rows = m_pItems->size();
    int cur_row = index.row();

    if(role == Qt::BackgroundRole  && cur_row == m_nHoverRow)
    {
        return QColor(247,243,251);
    }
    if(role == Qt::TextAlignmentRole && (column == 1||column == 2||column == 3) && cur_row < rows){
        return QVariant(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    if(role==Qt::DisplayRole && cur_row < rows)
    {
        CloudParam *pcp = (*m_pItems)[cur_row];
        if ( pcp != NULL ) {
            if (column == 1 ) {
                return pcp->name;
            }
            if(column == 2 ){
                return pcp->address;
            }
            if ( column == 3 ){
                return pcp->port;
            }
        }
        if(column == 4){
            return *m_pImageDel;
        }
    }
    if (role == Qt::CheckStateRole)
    {
        if (index.column() == 0)
        {
            if (((*m_pItems).size() > index.row()) && (index.row() > -1))
                return (*m_pItems)[index.row()]->is_selected ? Qt::Checked : Qt::Unchecked;
            return Qt::Unchecked;
        }
    }
    return QVariant();
}

void CloudParamModel::updateData(int row)
{
    if (row < 0)
        return;
    QModelIndex t1 = index(row, 0);
    QModelIndex t2 = index(row, 4);
    emit dataChanged(t1, t2);
}

void CloudParamDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
    switch(cur_column){
//    case 0:{
//            bool data = index.model()->data(index, Qt::UserRole).toBool();

//            QStyleOptionButton checkBoxStyle;
//            checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
//            checkBoxStyle.state |= QStyle::State_Enabled;
//            checkBoxStyle.iconSize = QSize(20, 20);
//            checkBoxStyle.rect = option.rect;

//            QCheckBox checkBox;
//            checkBoxStyle.iconSize = QSize(20, 20);
//            checkBoxStyle.rect = option.rect;
//            QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
//        }
//        break;
//    case 1:
//        break;
    case 4:
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


PrivateCloudTable::PrivateCloudTable(QWidget *parent)
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
    setEditTriggers(QAbstractItemView::SelectedClicked);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);

    m_Model.SetData(&m_AllItems);

    m_pDelegate = new CloudParamDelegate;
    m_pDelegate->SetView(this);

    setModel(&m_Model);
    setItemDelegate(m_pDelegate);

    setColumnWidth(0, 40);
    setColumnWidth(1, 130);
    setColumnWidth(2, 210);
    setColumnWidth(3, 130);
    setColumnWidth(4, 40);
    horizontalHeader()->setHighlightSections(false);

    m_nCurrentRow = -1;
}

PrivateCloudTable::~PrivateCloudTable()
{
    QVector<CloudParam*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        CloudParam *pitem = *it;
        qDebug("Delete CloudParam item: %s", pitem->name.toStdString().c_str());
        delete pitem;
        it = m_AllItems.erase(it);
    }
}

void PrivateCloudTable::AddCloudStart()
{
    qDebug("AddCloudStart !");
    AddItem(true, "", "", "");
    int rows = m_AllItems.size();
    QModelIndex index = model()->index(rows-1, 1);
    if (index.isValid()) {
        CellClicked(index);
    }
}

int PrivateCloudTable::AddItem(bool is_selected, QString name, QString address, int port)
{
    int row_count = m_Model.rowCount();
    int rows = m_AllItems.size();
    qDebug("PrivateCloudTable::AddItem row_count=%d rows=%d", row_count, rows);
    CloudParam *pitem = new CloudParam(is_selected, name, address, port);
    m_AllItems.push_back(pitem);
    m_Model.updateData();
    return 0;
}

int PrivateCloudTable::AddItem(bool is_selected, QString name, QString address, QString port)
{
    return AddItem(is_selected, name, address, port.toInt());
}

void PrivateCloudTable::DelItem(int row)
{
    int i = 0;
    int row_count = m_Model.rowCount();
    if (row >= row_count )
        return;
    QVector<CloudParam*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        if(i++ == row){
            //qDebug("PrivateCloudTable::DelItem i=%d  row=%d row_count=%d", i, row, row_count);
            CloudParam *pitem = *it;
            delete pitem;
            pitem = NULL;
            m_AllItems.erase(it);
            m_Model.removeRow(row);
            m_Model.updateData();
            return;
        }
        it++;
    }
}

void PrivateCloudTable::UpdateRow(int row)
{
    if (row == m_nCurrentRow)
        return;

    m_Model.SetHoverRow(row);
    for (int i = 4; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
        update(model()->index(row, i));
    }
    m_nCurrentRow = row;
}

bool PrivateCloudTable::HasAddressName(const QString & name)
{
    QVector<CloudParam*>::iterator it = m_AllItems.begin();
    while(it != m_AllItems.end()){
        CloudParam *pitem = *it;
        if(pitem->name == name)
            return true;
        it++;
    }
    return false;
}

void PrivateCloudTable::mousePressEvent(QMouseEvent *event)
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
    }
}

void PrivateCloudTable::mouseMoveEvent(QMouseEvent *event)
{
    int rows = m_AllItems.size();
    int column=this->columnAt(event->x());
    int row=this->rowAt(event->y());

    if(column == 4 && row < rows){
        this->setCursor(Qt::PointingHandCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    if(row < rows)
        UpdateRow(row);
}

void PrivateCloudTable::leaveEvent (QEvent * )
{
    m_Model.SetHoverRow(-1);
    int column_count= model()->columnCount();
    for (int i = column_count- 1; i >= 0; i--)
    {
        update(model()->index(m_nCurrentRow, i));
    }
    m_nCurrentRow = -1;
}

void PrivateCloudTable::CellClicked(const QModelIndex &index)
{
    int col = index.column();
    int row = index.row();

    if (row < 0)
        return;

    if (col == 1 || col == 2 || col == 3) {
        QString text = m_Model.data(index).toString();
        openPersistentEditor(index);
        QLineEdit* editWidget = static_cast<QLineEdit* >(indexWidget(index));
        if(editWidget != NULL){
            qDebug("editWidget != NULL");
            editWidget->setFocus();
            editWidget->setText(text);
            editWidget->setSelection(0, text.size());
            m_IndexPrevEdited = index;
            connect(editWidget, SIGNAL(editingFinished()), this, SLOT(CellEditFinished()));
        }
        return;
    }

    if( col != 4 )
        return;
    DelItem(row);
}

void PrivateCloudTable::CellEditFinished()
{
    QLineEdit* editWidget = static_cast<QLineEdit* >(indexWidget(m_IndexPrevEdited));
    QString text = editWidget->text();
    int row = m_IndexPrevEdited.row();
    int col = m_IndexPrevEdited.column();
    CloudParam * pitem = m_AllItems[row];
    switch (col) {
    case 1:
        pitem->name = text;
        break;
    case 2:
        pitem->address = text;
        break;
    case 3:
        pitem->port = text.toInt();
        break;
    default:
        break;
    }
    closePersistentEditor(m_IndexPrevEdited);
}

