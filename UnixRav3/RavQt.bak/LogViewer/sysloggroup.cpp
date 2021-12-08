#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <time.h>
#include <string.h>

#include "common/pubdef.h"

#include "sysloggroup.h"
#include "mainwindow.h"
#include "../Storage/Entry/commeventlog.h"



SysLogGroup::SysLogGroup(QWidget *parent):QWidget(parent),m_bInitedScreenData(false)
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    //setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-20);
    //setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36); //size 36 is the height of sysclose_btn.png

    m_plTitle = new QLabel(u8"平台日志", this);
    m_plTitle->adjustSize();
    m_plLine = new QLabel(this);
    m_plLine->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(57, 23, 92);");
    m_plLine->setFixedSize(535, 2);

//    m_plTime = new QLabel(u8"时间:", this);
//    m_plTime->adjustSize();

//    m_pcTime = new QComboBox(this);
    m_pTable = new QTableWidget(this);
    QStringList lheader;
    lheader << u8"序号" << u8"时间" << u8"来源" << u8"描述";
    m_pTable->setColumnCount(4);
    m_pTable->setHorizontalHeaderLabels(lheader);
    m_pTable->verticalHeader()->setDefaultSectionSize(25);
    m_pTable->verticalHeader()->hide();
    m_pTable->horizontalHeader()->setFixedHeight(25);
    m_pTable->setFrameShape(QFrame::NoFrame);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->horizontalHeader()->resizeSection(0,50);
    m_pTable->horizontalHeader()->resizeSection(1,140);
    m_pTable->horizontalHeader()->resizeSection(2,140);
    m_pTable->horizontalHeader()->resizeSection(3,310);

    m_pTable->setFixedSize(610, 375);
    m_pTable->setRowCount(0);

    InitControls();
}

void SysLogGroup::SetMainWindow(QWidget * pMainWindow)
{
    m_pMain = (MainWindow *)pMainWindow;
}

void SysLogGroup::InitControls()
{
//    QStringList tlist;
//    tlist << u8"全部" << u8"今天" << u8"最近三天" << u8"最近一周" << u8"最近一月";
//    m_pcTime->addItems(tlist);
//    m_pcTime->setCurrentIndex(1);

    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setMargin(10);
    hflayout->setSpacing(10);
    //hflayout->setContentsMargins(0,0,0,0);
    hflayout->addWidget(m_plTitle, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hflayout->addWidget(m_plLine, 0 , Qt::AlignLeft | Qt::AlignVCenter);

//    QHBoxLayout *hslayout = new QHBoxLayout;
//    hslayout->setMargin(10);
//    //hslayout->setContentsMargins(0,0,0,0);
//    hslayout->addWidget(m_plTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->setContentsMargins(10,0,10,10);
    vlayout->addLayout(hflayout);
    //vlayout->addLayout(hslayout);
    vlayout->addWidget(m_pTable, 0, Qt::AlignHCenter);

    setLayout(vlayout);
}

void SysLogGroup::ResetTable()
{
    m_pTable->clearContents();
    m_pTable->setRowCount(0);
    m_bInitedScreenData = false;
}

void SysLogGroup::InitData()
{
    if(m_bInitedScreenData){
        ResetTable();
    }
    m_bInitedScreenData = true;

    std::list<CommEventLog> logs;
    CommEventLog seed;
    seed.mType = CommEventLog::COMMAND;

    int count = m_pMain->GetBaseLogs(logs, seed);
    if(count > 0){
        std::list<CommEventLog>::iterator it = logs.begin();
        while(it != logs.end()){

//int  GetBaseLogs(std::list<CommEventLog>& logs, const CommEventLog& see);
//lheader << u8"序号" << u8"时间" << u8"来源" << u8"描述";

            AddItem(*it);
            it++;
        }
    }
}

//int mDate;
//enum CommEvent mType;
//std::string mAction;
//std::string mTarget;
//std::string mExtend;
//std::string mReserved1;
//std::string mReserved2;
//lheader << u8"序号" << u8"时间" << u8"来源" << u8"描述";

void SysLogGroup::AddItem(const CommEventLog& see)
{
    int rowCount = m_pTable->rowCount();
    m_pTable->insertRow(rowCount);
    char buf[128]= {0};
    QTableWidgetItem  *itemID = new QTableWidgetItem(QString("%1").arg(rowCount,0,10));
    m_pTable->setItem(rowCount, 0, itemID);
    const time_t vtime = see.mDate;
    struct tm *tvtime = localtime(&vtime);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", tvtime);
    QTableWidgetItem  *itemTime = new QTableWidgetItem(buf);
    m_pTable->setItem(rowCount, 1, itemTime);
    QTableWidgetItem *itemTarget = new QTableWidgetItem(QString::fromStdString(see.mTarget));
    m_pTable->setItem(rowCount, 2, itemTarget);
    QTableWidgetItem *itemDesc = new QTableWidgetItem(QString::fromStdString(see.mExtend));
    m_pTable->setItem(rowCount, 3, itemDesc);
}

void SysLogGroup::resizeEvent(QResizeEvent *event)
{
    m_plLine->setFixedWidth(event->size().width() - 100);
    m_pTable->setFixedSize(event->size().width()-50, event->size().height()-100);
}
