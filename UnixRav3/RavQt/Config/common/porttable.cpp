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

#include "porttable.h"
#include "iptables.h"
#include "common/pubdef.h"

#define COLUMN_NUMBER 4
#define ROW_NUMBER 6
#define DBTABLEPB "portblack(port text, protocol text, direction text)"

//load close icon
PortItemModel::PortItemModel(QObject * parent)
    :QStandardItemModel(parent)
{
    QPixmap oripixmap = QPixmap(":/resource/delete_btn");
    m_pImageDel = new QPixmap;
    *m_pImageDel = oripixmap.copy(0,0,oripixmap.width()/3,oripixmap.height());
}

//init menu
Menu::Menu(QWidget *parent)
	:QMenu(parent)
{
	tcp = new QAction("tcp",this);
	udp = new QAction("udp", this);
	tcp_udp = new QAction("tcp_udp", this);

	in = new QAction("in", this);
	out = new QAction("out", this);
	in_out = new QAction("in_out", this);

}


//init table
PortNameTable::PortNameTable(QWidget *parent)
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

    m_pModel= new PortItemModel;
    m_pModel->setColumnCount(COLUMN_NUMBER);
    m_pModel->setRowCount(ROW_NUMBER);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("端口"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("协议"));
    m_pModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("方向"));
    m_pModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("操作"));

    m_pDelegate = new PortIconDelegate;
    m_pDelegate->SetView(this);
	m_pModel->installEventFilter(m_pDelegate);

	m_protocol = new Menu(this);
	m_direction = new Menu(this);

	m_protocol->addAction(m_protocol->tcp);
	m_protocol->addAction(m_protocol->udp);
	m_protocol->addAction(m_protocol->tcp_udp);

	m_direction->addAction(m_direction->in);
	m_direction->addAction(m_direction->out);
	m_direction->addAction(m_direction->in_out);

    setModel(m_pModel);
	setItemDelegate(m_pDelegate);

    setColumnWidth(0, 270);
    setColumnWidth(1, 120);
    setColumnWidth(2, 120);
    setColumnWidth(3, 40);

	m_Stringdata = new IpTables(DBTABLEPB);

    m_nCurrentRow = -1;
	previouseindex = m_pModel->index(0,0);
	ischanged = false;
	
	m_Stringdata->ipsqldata->SqlQueryIp("portblack",3);
	WriteTable();

	connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(PressedHandle(QModelIndex)));
	connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this ,SLOT(ItemChangedHandle(QStandardItem*)));
	connect(m_protocol, SIGNAL(triggered(QAction*)), this, SLOT(ProtocolTriggeredHandle(QAction*)));
	connect(m_direction, SIGNAL(triggered(QAction*)), this, SLOT(DirectionTriggeredHandle(QAction*)));
}


//disable tab key
bool PortIconDelegate::eventFilter(QObject *obj, QEvent *event) {
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

//paint close key where add button is clicked
void PortIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    int cur_column = index.column();
		if(cur_column == 3 && index.row() <= m_pView->m_nCurrentRow)
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

//when user input something, it will be called
void PortNameTable::ItemChangedHandle(QStandardItem *item) 
{
	port_Text text;
	int row = item->row();
	int column = item->column();
	QString str = model()->index(row,0).data().toString();
	int size = m_Stringdata->m_PortData.size()-1;
	//qDebug() << item->text() << item->row() << item->column();
	if( !(str == "") && !m_Stringdata->IsPortValid(str))
	{
		QMessageBox::about(NULL, "提示", "端口错误,请重新输入。端口范围：0-65535,端口格式90或90-100或-90或100-");
		return ;
	}
	if(row > size && str == "")
	{
		return;
	}
	if(row > size && str != "")
	{
		text.str = model()->index(row,0).data().toString();
		text.protocol = model()->index(row,1).data().toString();
		text.direction = model()->index(row,2).data().toString();
		m_Stringdata->m_PortData.push_back(text);
		ischanged = false;
	}
	if(row < size)
	{
		m_Stringdata->m_PortData.erase(m_Stringdata->m_PortData.begin() + row);

		text.str = model()->index(row,0).data().toString();
		text.protocol = model()->index(row,1).data().toString();
		text.direction = model()->index(row,2).data().toString();
		m_Stringdata->m_PortData.insert(m_Stringdata->m_PortData.begin() + row, text);
		ischanged = false;
	}
	if(row == size)
	{
		m_Stringdata->m_PortData.erase(m_Stringdata->m_PortData.begin() + row);
		text.str = model()->index(row,0).data().toString();
		text.protocol = model()->index(row,1).data().toString();
		text.direction = model()->index(row,2).data().toString();
		m_Stringdata->m_PortData.push_back(text);
		ischanged = false;
	}

	for (int i = 0; i < m_Stringdata->m_PortData.size(); i++) {
		qDebug() << m_Stringdata->m_PortData[i].str << m_Stringdata->m_PortData[i].protocol << m_Stringdata->m_PortData[i].direction;
	}

}

//write table from db
void PortNameTable::WriteTable()
{
	port_Text text;
	for(int i = 0; i < m_Stringdata->ipsqldata->m_PortDB.size(); i++)
	{
		AddRowEdit();
		m_pModel->setItem(m_nCurrentRow,0,new QStandardItem(m_Stringdata->ipsqldata->m_PortDB[i].str));
		m_pModel->setItem(m_nCurrentRow,1,new QStandardItem(m_Stringdata->ipsqldata->m_PortDB[i].protocol));
		m_pModel->setItem(m_nCurrentRow,2,new QStandardItem(m_Stringdata->ipsqldata->m_PortDB[i].direction));
		text.str = m_Stringdata->ipsqldata->m_PortDB[i].str;
		text.protocol = m_Stringdata->ipsqldata->m_PortDB[i].protocol;
		text.direction = m_Stringdata->ipsqldata->m_PortDB[i].direction;
		m_Stringdata->m_PortData.push_back(text);
		ischanged = false;
	}
}


//add a new row for getting data
void PortNameTable::AddRowEdit()
{
	m_nCurrentRow++;
	if(m_nCurrentRow > 5)
		m_pModel->insertRow(m_nCurrentRow);
	update(m_pModel->index(m_nCurrentRow, 3));
	m_pModel->setItem(m_nCurrentRow,1,new QStandardItem("tcp"));
	m_pModel->setItem(m_nCurrentRow,2,new QStandardItem("in"));
	ischanged = true;
}

void PortNameTable::mouseMoveEvent(QMouseEvent *event)
{
	//qDebug()<< "zobiao" << event->pos();

}

//where menu action is clicked , it will be called
void PortNameTable::ProtocolTriggeredHandle(QAction *action)
{
	if(action == m_protocol->tcp)
	{
		m_pModel->setItem(m_Index.row(), m_Index.column(), new QStandardItem("tcp"));
	}
	else if(action == m_protocol->udp)
	{

		m_pModel->setItem(m_Index.row(), m_Index.column(), new QStandardItem("udp"));
	}
	else if(action == m_protocol->tcp_udp)
	{

		m_pModel->setItem(m_Index.row(), m_Index.column(), new QStandardItem("tcp_udp"));
	}
		

}

//where menu action is clicked , it will be called
void PortNameTable::DirectionTriggeredHandle(QAction *action)
{
	if(action == m_direction->in)
	{
		m_pModel->setItem(m_Index.row(), m_Index.column(), new QStandardItem("in"));
	}
	else if(action == m_direction->out)
	{

		m_pModel->setItem(m_Index.row(), m_Index.column(), new QStandardItem("out"));
	}
	else if(action == m_direction->in_out)
	{

		m_pModel->setItem(m_Index.row(), m_Index.column(), new QStandardItem("in_out"));
	}
		
}

//where table cell is clicked , it will be called
void PortNameTable::PressedHandle(QModelIndex index)
{
	if(!firewallflag)
		return;
	m_Index = index;
	if(index.row() <= m_nCurrentRow)
	{
		switch(index.column())
		{
			case 1:
				m_protocol->popup(mapToGlobal(visualRect(index).topLeft()));
				return;
				break;

			case 2:
				m_direction->popup(mapToGlobal(visualRect(index).topLeft()));
				return;
				break;

			case 3:
				closePersistentEditor(previouseindex);
				m_pModel->removeRow(index.row());
				if(index.row() <= m_Stringdata->m_PortData.size()-1)
					m_Stringdata->m_PortData.erase(m_Stringdata->m_PortData.begin() + index.row());

				if(m_nCurrentRow < 6)
					m_pModel->insertRow(ROW_NUMBER - 1);

				m_nCurrentRow--;
				ischanged = false;
				
				return;
				break;

			default :
				break;
		}
		openPersistentEditor(index);
		if(index != previouseindex)
		{
			closePersistentEditor(previouseindex);
			previouseindex = index;
		}

	}
	else
		qDebug() << "no way";
}
