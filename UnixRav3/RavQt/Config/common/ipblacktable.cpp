#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QVariant>
#include <QColor>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QEvent>
#include <QMessageBox>
#include <QDebug>

#include "ipblacktable.h"
#include "iptables.h"
#include "common/pubdef.h"

#define COLUMN_NUMBER 2
#define ROW_NUMBER 6
#define DBTABKEIPB "ipblack(ip text)"


//load close icon
IpBlackItemModel::IpBlackItemModel(QObject * parent) 
    :QStandardItemModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

//init table
IpBlackNameTable::IpBlackNameTable(QWidget *parent)
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
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);


    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());

    m_pModel= new IpBlackItemModel;
    m_pModel->setColumnCount(COLUMN_NUMBER);
    m_pModel->setRowCount(ROW_NUMBER);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("ip"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("操作"));
	

    m_pDelegate = new IpBlackIconDelegate;
    m_pDelegate->SetView(this);
	m_pModel->installEventFilter(m_pDelegate);

    setModel(m_pModel);
	setItemDelegate(m_pDelegate);

    setColumnWidth(0, 500);
    setColumnWidth(1, 40);

	//m_Stringdata = new IpTables("ipblack(ip text)");
	m_Stringdata = new IpTables(DBTABKEIPB);

    m_nCurrentRow = -1;
	previouseindex = m_pModel->index(0,0);
	ischanged = false;

	m_Stringdata->ipsqldata->SqlQueryIp("ipblack",2);
	WriteTable();

	connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(PressedHandle(QModelIndex)));
	connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this ,SLOT(ItemChangedHandle(QStandardItem*)));
}


//disable tab key
bool IpBlackIconDelegate::eventFilter(QObject *obj, QEvent *event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		//qDebug() << "Ate key press" << keyEvent->key();
		switch(keyEvent->key())
		{
			case Qt::Key::Key_Tab:
			qDebug() << "tab key press" << keyEvent->key();
			return true;
				break;

			default :
			return QItemDelegate::eventFilter(obj, event);
				break;
		}
	}
	// pass the event on to the parent class
	return QItemDelegate::eventFilter(obj, event);
}

//paint close key
void IpBlackIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
		if(cur_column == COLUMN_NUMBER - 1 && index.row() <= m_pView->m_nCurrentRow)
		{
                QItemDelegate::paint(painter, option, index);

                QPixmap imgDel = *(m_pView->m_pImageDel);
                int x = option.rect.x() + option.rect.width()/2 - imgDel.width()/2;
                int y = option.rect.y() + option.rect.height()/2 - imgDel.height()/2;

                painter->drawPixmap(x,y,imgDel);
                return;
		}
    QItemDelegate::paint(painter, option, index);
}

void IpBlackNameTable::ItemChangedHandle(QStandardItem *item) 
{
	QString str = item->text();
	int row = item->row();
	int size = m_Stringdata->m_IpBlackData.size()-1;
	//qDebug() << item->text() << item->row() << item->column();
	if( !(str == "") && !m_Stringdata->IsIpValid(str))
	{
		QMessageBox::about(NULL, "提示", "ip格式错误,请重新输入。正确格式：ip:192.168.1.1 ip:段192.168.1.1-192.168.1.100");
		return ;
	}

	if(row > size && str == "")
	{
		return;
	}

	//insert ending
	if(row > size && str != "")
	{
		m_Stringdata->m_IpBlackData.push_back(str);
		ischanged = false;
	}

	//change  middle
	if(row < size)
	{
		m_Stringdata->m_IpBlackData.erase(m_Stringdata->m_IpBlackData.begin() + row);
		m_Stringdata->m_IpBlackData.insert(m_Stringdata->m_IpBlackData.begin() + row, str);

		ischanged = false;
	}

	//change ending
	if(row == size)
	{
		m_Stringdata->m_IpBlackData.erase(m_Stringdata->m_IpBlackData.begin() + row);
		m_Stringdata->m_IpBlackData.push_back(str);

		ischanged = false;
	}

	for (int i = 0; i < m_Stringdata->m_IpBlackData.size(); i++) {
		qDebug() << m_Stringdata->m_IpBlackData[i];
	}

}

//write table from db
void IpBlackNameTable::WriteTable()
{
	for (int i = 0; i < m_Stringdata->ipsqldata->m_IpBlackDB.size(); i++) {
		AddRowEdit();
		m_pModel->setItem(m_nCurrentRow,0,new QStandardItem(m_Stringdata->ipsqldata->m_IpBlackDB[i]));
		m_Stringdata->m_IpBlackData.push_back(m_Stringdata->ipsqldata->m_IpBlackDB[i]);
		ischanged = false;
	}
}

//add a new row with editable
void IpBlackNameTable::AddRowEdit()
{
	m_nCurrentRow++;
	if(m_nCurrentRow > ROW_NUMBER-1)
		m_pModel->insertRow(m_nCurrentRow);
	//closePersistentEditor(previouseindex);
	//openPersistentEditor(m_pModel->index(m_nCurrentRow,0));
	//previouseindex = m_pModel->index(m_nCurrentRow,0);
	ischanged = true;
	update(m_pModel->index(m_nCurrentRow, COLUMN_NUMBER-1));
}

void IpBlackNameTable::mouseMoveEvent(QMouseEvent *event)
{
	//if(event->pos().x() < 5 || event->pos().y() >140 || event->pos().x() > 530 || event->pos().y() <5)
	//{
	//qDebug()<< "zobiao" << event->pos();
	//}

}

//when mouse is pressed, we need to do something
void IpBlackNameTable::PressedHandle(QModelIndex index)
{
	if(!firewallflag)
		return;
	m_Index = index;
	if(index.row() <= m_nCurrentRow)
	{
		switch(index.column())
		{
			case COLUMN_NUMBER-1:
				closePersistentEditor(previouseindex);
				m_pModel->removeRow(index.row());

				if( index.row() <= m_Stringdata->m_IpBlackData.size() -1)
					m_Stringdata->m_IpBlackData.erase(m_Stringdata->m_IpBlackData.begin() + index.row());

				if(m_nCurrentRow < ROW_NUMBER)
					m_pModel->insertRow(ROW_NUMBER - 1);

				m_nCurrentRow--;
				ischanged = false;
				return;
				break;

			default :
				break;
		}
		
		//openPersistentEditor(index);
		//if(index != previouseindex)
		//{
		//	closePersistentEditor(previouseindex);
		//	previouseindex = index;
		//}
		
		closePersistentEditor(previouseindex);
		openPersistentEditor(index);

	}
	else
	{
		qDebug() << "no way";
		closePersistentEditor(previouseindex);
	}
		previouseindex = index;
}
