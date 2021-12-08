#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>

#include "common/pubdef.h"
#include "commonapi.h"
#include "Entry/virusinfoentry.h"
#include "installgroup.h"
#include "mainwindow.h"



InstallGroup::InstallGroup(QWidget *parent):QWidget(parent),m_bInitedScreenData(false)
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    //setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-20);
    //setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36); //size 36 is the height of sysclose_btn.png

    m_plTitle = new QLabel(u8"安装部署", this);
    m_plTitle->adjustSize();
    m_plLine = new QLabel(this);
    m_plLine->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(57, 23, 92);");
    m_plLine->setFixedSize(535, 2);

//    m_plTime = new QLabel(u8"时间:", this);
//    m_plTime->adjustSize();
//    m_plAction = new QLabel(u8"动作:", this);
//    m_plAction->adjustSize();
//    m_plEntry = new QLabel(u8"条目:", this);
//    m_plEntry->adjustSize();


//    m_pcTime = new QComboBox(this);
//    m_pcAction = new QComboBox(this);
//    m_pcEntry = new QComboBox(this);
    m_pTable = new QTableWidget(this);
    QStringList lheader;
    lheader << u8"序号" << u8"时间" << u8"动作" << u8"条目" << u8"状态" << u8"旧版本" << u8"新版本";
    m_pTable->setColumnCount(7);
    m_pTable->setHorizontalHeaderLabels(lheader);
    m_pTable->verticalHeader()->setDefaultSectionSize(25);
    m_pTable->verticalHeader()->hide();
    m_pTable->horizontalHeader()->setFixedHeight(25);
    m_pTable->setFrameShape(QFrame::NoFrame);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->horizontalHeader()->resizeSection(0,50);
    m_pTable->horizontalHeader()->resizeSection(1,130);
    m_pTable->horizontalHeader()->resizeSection(2,97);
    m_pTable->horizontalHeader()->resizeSection(3,100);
    m_pTable->horizontalHeader()->resizeSection(4,100);
    m_pTable->horizontalHeader()->resizeSection(5,100);
    m_pTable->horizontalHeader()->resizeSection(6,100);

    m_pTable->setFixedSize(610, 375);
    m_pTable->setRowCount(0);

    InitControls();
    m_nMaxIndex = 0;
}

void InstallGroup::InitControls()
{
//    QStringList tlist;
//    tlist << u8"全部" << u8"今天" << u8"最近三天" << u8"最近一周" << u8"最近一月";
//    m_pcTime->addItems(tlist);
//    m_pcTime->setCurrentIndex(1);
//    QStringList clist;
//    clist << u8"全部" << u8"安装" << u8"升级" << u8"修复" << u8"重启";
//    m_pcAction->addItems(clist);
//    QStringList enlist;
//    enlist << u8"全部" << u8"病毒库" << u8"恶意网址库" << u8"安全云终端";
//    m_pcEntry->addItems(enlist);

    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setMargin(10);
    hflayout->setSpacing(10);
    hflayout->addWidget(m_plTitle, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hflayout->addWidget(m_plLine, 0 , Qt::AlignLeft | Qt::AlignVCenter);

//    QHBoxLayout *hslayout = new QHBoxLayout;
//    hslayout->setMargin(10);
//    //hslayout->setContentsMargins(0,0,0,0);
//    hslayout->addWidget(m_plTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addSpacing(5);
//    hslayout->addWidget(m_plAction, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcAction, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addSpacing(5);
//    hslayout->addWidget(m_plEntry, 0, Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcEntry, 0, Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->setContentsMargins(10,0,10,10);
    vlayout->addLayout(hflayout);
    //vlayout->addLayout(hslayout);
    vlayout->addWidget(m_pTable, 0, Qt::AlignHCenter);

    setLayout(vlayout);
}

void InstallGroup::ResetTable()
{
    m_pTable->clearContents();
    m_pTable->setRowCount(0);
    m_nMaxIndex = 0;
    m_bInitedScreenData = false;
}

void InstallGroup::InitData()
{
    if(m_bInitedScreenData){
        ResetTable();
    }
    m_bInitedScreenData = true;

    std::list<CommEventLog> logs;
    CommEventLog seed;
    seed.mType = CommEventLog::DEPLOY;
    seed.mID = 0;

    int count = m_pMain->GetBaseLogs(logs, seed);
    if(count > 0){
        std::list<CommEventLog>::iterator it = logs.begin();
        while(it != logs.end()){

//int  GetBaseLogs(std::list<CommEventLog>& logs, const CommEventLog& see);
//lheader << u8"序号" << u8"时间" << u8"来源" << u8"描述";
//lheader << u8"序号" << u8"时间" << u8"动作" << u8"条目" << u8"旧版本" << u8"新版本" << u8"重启标志";
            AddItem(*it);
            it++;
        }
    }
}

//int mDate;      Utils::String::Split
//enum CommEvent mType;
//std::string mAction;
//std::string mTarget;
//std::string mExtend;
//std::string mReserved1;
//std::string mReserved2;
//lheader << u8"序号" << u8"时间" << u8"来源" << u8"描述";

void InstallGroup::AddItem(const CommEventLog& see)
{
    char buf[128]= {0};
    int rowCount = m_pTable->rowCount();
    m_pTable->insertRow(rowCount);

    std::vector< std::string> vsdata;
    Utils::String::Split(see.mExtend, ";", &vsdata);

    QTableWidgetItem  *itemID = new QTableWidgetItem(QString("%1").arg(see.mID,0,10));
    m_pTable->setItem(rowCount, 0, itemID);
    const time_t vtime = see.mDate;
    struct tm *tvtime = localtime(&vtime);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", tvtime);
    QTableWidgetItem  *itemTime = new QTableWidgetItem(buf);
    m_pTable->setItem(rowCount, 1, itemTime);

    QString data[4];
    int i = 0;
    std::vector<std::string>::iterator it; //= vsdata.begin();
//    while(it != vsdata.end()) {
//        data[i] = QString::fromStdString(vsdata[i]);
//        if ( ++i == 4) {
//            break;
//        }
//    }

    for (it=vsdata.begin(),i=0;it!=vsdata.end();++it,++i) {
        QTableWidgetItem *itemAction = new QTableWidgetItem(QString::fromStdString(vsdata[i]));
        m_pTable->setItem(rowCount, 2+i, itemAction);
    }

//    QTableWidgetItem *itemAction = new QTableWidgetItem(data[0]);
//    m_pTable->setItem(rowCount, 2, itemAction);
//    QTableWidgetItem *itemTarget = new QTableWidgetItem(data[1]);
//    m_pTable->setItem(rowCount, 3, itemTarget);
//    QTableWidgetItem *itemOld = new QTableWidgetItem(data[2]);
//    m_pTable->setItem(rowCount, 4, itemOld);
//    QTableWidgetItem *itemNew = new QTableWidgetItem(data[3]);
//    m_pTable->setItem(rowCount, 5, itemNew);
}

void InstallGroup::resizeEvent(QResizeEvent *event)
{
    m_plLine->setFixedWidth(event->size().width() - 100);
    m_pTable->setFixedSize(event->size().width()-50, event->size().height()-100);
}
