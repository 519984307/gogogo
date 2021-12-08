#include <QLabel>
#include <QHBoxLayout>
#include <QStringList>
#include <QHeaderView>

#include <QDebug>

#include <list>
#include <time.h>
#include <string.h>

#include "scaneventgroup.h"
#include "common/pubdef.h"
#include "../Storage/Entry/logevententry.h"
#include "mainwindow.h"

using namespace std;

//#define SCANEVENT_GROUP_WIDTH   620
#define SCANEVENT_GROUP_HEIGHT  280

ScanEventGroup::ScanEventGroup(QWidget *parent):QWidget(parent),m_bInitedScreenData(false)
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    //setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-20);
    //setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36); //size 36 is the height of sysclose_btn.png

    m_plScanEvent = new QLabel(u8"扫描事件", this);
    m_plScanEvent->adjustSize();
    m_plLine = new QLabel(this);
    m_plLine->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(57, 23, 92);");
    m_plLine->setFixedSize(535, 2);

//    m_plTime = new QLabel(u8"时间:", this);
//    m_plTime->adjustSize();
//    m_plSrc = new QLabel(u8"来源:", this);
//    m_plSrc->adjustSize();

//    QPalette pa;
//    pa.setColor(QPalette::WindowText,Qt::blue);
//    m_plExportLog = new QLabel(u8"导出日志", this);
//    m_plExportLog->adjustSize();
//    m_plExportLog->setPalette(pa);

//    m_pcTime = new QComboBox(this);
//    m_pcSrc = new QComboBox(this);
    m_pTable = new QTableWidget(this);
    QStringList lheader;
    lheader << u8"序号" << u8"开始时间" << u8"事件来源" << u8"事件" << u8"事件号" << u8"状态" << u8"文件数" << u8"对象数" << u8"发现威胁" << u8"已处理" << u8"用时";
    m_pTable->setColumnCount(11);
    m_pTable->setHorizontalHeaderLabels(lheader);
    m_pTable->verticalHeader()->setDefaultSectionSize(25);
    m_pTable->verticalHeader()->hide();
    m_pTable->horizontalHeader()->setFixedHeight(25);
    m_pTable->setFrameShape(QFrame::NoFrame);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->horizontalHeader()->resizeSection(0,50);
    m_pTable->horizontalHeader()->resizeSection(1,130);
    m_pTable->horizontalHeader()->resizeSection(2,70);
    m_pTable->horizontalHeader()->resizeSection(3,70);
    m_pTable->horizontalHeader()->resizeSection(4,70);
    m_pTable->horizontalHeader()->resizeSection(5,70);
    m_pTable->horizontalHeader()->resizeSection(6,100);
    m_pTable->horizontalHeader()->resizeSection(7,100);
    m_pTable->horizontalHeader()->resizeSection(8,80);
    m_pTable->horizontalHeader()->resizeSection(9,80);
    m_pTable->horizontalHeader()->resizeSection(10,70);

    m_pTable->setFixedSize(610, 375);
    m_pTable->setRowCount(0);
    m_nMaxIndex = 0;

    InitControls();
}

void ScanEventGroup::InitControls()
{
//    QStringList tlist;
//    tlist << u8"全部" << u8"今天" << u8"最近三天" << u8"最近一周" << u8"最近一月";
//    m_pcTime->addItems(tlist);
//    m_pcTime->setCurrentIndex(1);
//    QStringList clist;
//    clist << u8"全部" << u8"快速查杀" << u8"全盘查杀" << u8"自定义查杀" << u8"文件监控";
//    m_pcSrc->addItems(clist);

    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setMargin(10);
    hflayout->setSpacing(10);
    //hflayout->setContentsMargins(0,0,0,0);
    hflayout->addWidget(m_plScanEvent, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hflayout->addWidget(m_plLine, 0 , Qt::AlignLeft | Qt::AlignVCenter);

//    QHBoxLayout *hslayout = new QHBoxLayout;
//    hslayout->setMargin(10);
//    //hslayout->setContentsMargins(0,0,0,0);
//    hslayout->addWidget(m_plTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addSpacing(5);
//    hslayout->addWidget(m_plSrc, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcSrc, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();
//    hslayout->addWidget(m_plExportLog, 0 , Qt::AlignRight | Qt::AlignVCenter);

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->setContentsMargins(10,0,10,10);
    vlayout->addLayout(hflayout);
    //vlayout->addLayout(hslayout);
    vlayout->addWidget(m_pTable, 0, Qt::AlignHCenter);

    setLayout(vlayout);
}

void ScanEventGroup::ResetTable()
{
    m_pTable->clearContents();
    m_pTable->setRowCount(0);
    m_nMaxIndex = 0;
    m_bInitedScreenData = false;
}

void ScanEventGroup::InitData()
{
    if(m_bInitedScreenData){
        ResetTable();
    }
    m_bInitedScreenData = true;

    list<LogEventEntry> lentrys;
    int count = m_pMain->GetScanLogs(lentrys);
    if(count > 0){
        list<LogEventEntry>::iterator it = lentrys.begin();
        while(it != lentrys.end()){
//union {
//    int mScanCount;
//    int mReserved1;
//};
//int mVirusCount;
//int mKilledCount;
//int mDeletedCount;
//int mFailedCount;
//int mIgnoreCount;
//int mStartTime;
//int mEndTime;
//string mScanPath;
//int mTotalCount;
//int mReported;
//lheader <<  u8"序号" << u8"开始时间" << u8"事件来源" << u8"事件" << u8"事件号" << u8"状态" << u8"文件数" << u8"对象数" << u8"发现威胁" << u8"已处理" << u8"用时";

            AddItem(*it);
            it++;
        }
    }
}


void ScanEventGroup::AddItem(LogEventEntry &info)
{
    int rowCount = m_pTable->rowCount();
    m_pTable->insertRow(rowCount);
    const char stype[][16] = {"界面发起", "远程命令", "定时器发起", "右键扫描", "自动防护"};
    const char etype[][16] = {"全盘查杀", "快速查杀", "自定义查杀", "指定文件"};
    //const char ltype[][16] = {"其它", "扫描", "升级"};
    const char sstatus[][16] = {"完成", "中断"};
    char buf[128]= {0};
    QTableWidgetItem  *itemID = new QTableWidgetItem(QString("%1").arg(++m_nMaxIndex,0,10));
    m_pTable->setItem(rowCount, 0, itemID);
    const time_t vtime = info.mStartTime;
    struct tm *tvtime = localtime(&vtime);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", tvtime);
    QTableWidgetItem  *itemTime = new QTableWidgetItem(buf);
    m_pTable->setItem(rowCount, 1, itemTime);

    string starter;
    if (info.GetScanType() == 0) { //界面发起
        starter = info.GetLogUser();
        if (starter.length() > 0) {
            starter += "发起";
        }else{
            starter = "界面发起";
        }
    }else{
        starter = stype[info.GetScanType()];
    }
    QTableWidgetItem *itemPath = new QTableWidgetItem(QString::fromStdString(starter));
    m_pTable->setItem(rowCount, 2, itemPath);
    QTableWidgetItem *itemVName = new QTableWidgetItem(etype[info.GetDiskType()]);
    m_pTable->setItem(rowCount, 3, itemVName);

    QTableWidgetItem *itemEvent = new QTableWidgetItem(QString("%1").arg(info.mID,0,10));
    m_pTable->setItem(rowCount, 4, itemEvent);

    QTableWidgetItem *itemEID = new QTableWidgetItem(sstatus[info.mStatus]);
    m_pTable->setItem(rowCount, 5, itemEID);

    QTableWidgetItem *itemType = new QTableWidgetItem(QString("%1").arg(info.mTotalCount,0,10));
    m_pTable->setItem(rowCount, 6, itemType);

    QTableWidgetItem *itemSC = new QTableWidgetItem(QString("%1").arg(info.mScanCount,0,10));
    m_pTable->setItem(rowCount, 7, itemSC);

    QTableWidgetItem *itemICount = new QTableWidgetItem(QString("%1").arg(info.mKilledCount+info.mDeletedCount+info.mFailedCount+info.mIgnoreCount,0,10));
    m_pTable->setItem(rowCount, 8, itemICount);

    QTableWidgetItem *itemPC = new QTableWidgetItem(QString("%1").arg(info.mKilledCount+info.mDeletedCount,0,10));
    m_pTable->setItem(rowCount, 9, itemPC);
    time_t nsec = (time_t)info.mEndTime-info.mStartTime;

    sprintf(buf, "%d:%02d:%02d", ((int)nsec)/3600, ((int)nsec)%3600/60, ((int)nsec)%60);
    QTableWidgetItem *itemUST = new QTableWidgetItem(buf);
    m_pTable->setItem(rowCount, 10, itemUST);
}

void ScanEventGroup::resizeEvent(QResizeEvent *event)
{
    m_plLine->setFixedWidth(event->size().width() - 100);
    m_pTable->setFixedSize(event->size().width()-50, event->size().height()-100);
}
