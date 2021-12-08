#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QStandardItem>
#include <QList>

#include "common/pubdef.h"
#include "Entry/virusinfoentry.h"
#include "virusdetailgroup.h"
#include "application.h"
#include "mainwindow.h"

#include <time.h>
#include <string>

using namespace std;



VirusDetailGroup::VirusDetailGroup(QWidget *parent):QWidget(parent),m_pMain(NULL),m_bInitedScreenData(false)
{
    //setTitle(u8"病毒详情");
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    //setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-20);
    //setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36); //size 36 is the height of sysclose_btn.png

    m_plDetail = new QLabel(u8"病毒详情", this);
    m_plDetail->adjustSize();
    m_plLine = new QLabel(this);
    m_plLine->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(57, 23, 92);");
    m_plLine->setFixedSize(535, 2);

//    m_plTime = new QLabel(u8"时间:", this);
//    m_plTime->adjustSize();
//    m_plSrc = new QLabel(u8"来源:", this);
//    m_plSrc->adjustSize();
//    m_plProcType = new QLabel(u8"处理方式:", this);
//    m_plProcType->adjustSize();
//    m_plEventID = new QLabel(u8"事件号:", this);
//    m_plEventID->adjustSize();
//    QPalette pa;
//    pa.setColor(QPalette::WindowText,Qt::blue);
//    m_plExportLog = new QLabel(u8"导出日志", this);
//    m_plExportLog->adjustSize();
//    m_plExportLog->setPalette(pa);

//    m_pcTime = new QComboBox(this);
//    m_pcSrc = new QComboBox(this);
//    m_pcProcType = new QComboBox(this);
//    m_pcEventID = new QComboBox(this);
    m_pTable = new QTableWidget(this);
    QStringList lheader;
    lheader << u8"序号" << u8"处理时间" << u8"文件路径" << u8"病毒名称" << u8"扫描事件" << u8"事件号" << u8"威胁类型" << u8"状态";
    m_pTable->setColumnCount(8);
    m_pTable->setHorizontalHeaderLabels(lheader);
    m_pTable->verticalHeader()->setDefaultSectionSize(25);
    m_pTable->verticalHeader()->hide();
    m_pTable->horizontalHeader()->setFixedHeight(25);
    m_pTable->setFrameShape(QFrame::NoFrame);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->horizontalHeader()->resizeSection(0,50);
    m_pTable->horizontalHeader()->resizeSection(1,130);
    m_pTable->horizontalHeader()->resizeSection(2,190);
    m_pTable->horizontalHeader()->resizeSection(3,80);
    m_pTable->horizontalHeader()->resizeSection(4,70);
    m_pTable->horizontalHeader()->resizeSection(5,70);
    m_pTable->horizontalHeader()->resizeSection(6,58);
    m_pTable->horizontalHeader()->resizeSection(7,62);
    m_pTable->setFixedSize(610, 410);
//    m_nBlankItemFrom = 0;
//    m_nBlankItemEnd = 15;
    m_nMaxIndex = 0;
//    m_pTable->setRowCount(m_nBlankItemEnd+1);

    InitControls();
    //InitData();
}

void VirusDetailGroup::InitControls()
{
//    QStringList tlist;
//    tlist << u8"全部" << u8"今天" << u8"最近三天" << u8"最近一周" << u8"最近一月";
//    m_pcTime->addItems(tlist);
//    m_pcTime->setCurrentIndex(1);
//    QStringList clist;
//    clist << u8"全部" << u8"快速查杀" << u8"全盘查杀" << u8"自定义查杀" << u8"文件监控";
//    m_pcSrc->addItems(clist);
//    QStringList plist;
//    plist << u8"全部" << u8"暂未处理" << u8"忽略" << u8"删除" << u8"清除"<< u8"信任" << u8"备份失败";
//    m_pcProcType->addItems(clist);
//    m_pcEventID->addItem(u8"全部");

    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setMargin(10);
    hflayout->setSpacing(10);
    hflayout->addWidget(m_plDetail, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hflayout->addWidget(m_plLine, 0 , Qt::AlignLeft | Qt::AlignVCenter);

//    QHBoxLayout *hslayout = new QHBoxLayout;
//    hslayout->setMargin(10);
//    //hslayout->setContentsMargins(0,0,0,0);
//    hslayout->addWidget(m_plTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();
//    hslayout->addWidget(m_plSrc, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcSrc, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();
//    hslayout->addWidget(m_plProcType, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcProcType, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();
//    hslayout->addWidget(m_plEventID, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addWidget(m_pcEventID, 0 , Qt::AlignLeft | Qt::AlignVCenter);
//    hslayout->addStretch();
//    hslayout->addWidget(m_plExportLog, 0 , Qt::AlignLeft | Qt::AlignVCenter);

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->setContentsMargins(10,0,10,10);
    vlayout->addLayout(hflayout);
    //vlayout->addLayout(hslayout);
    vlayout->addWidget(m_pTable, 0, Qt::AlignHCenter);

    setLayout(vlayout);
}

void VirusDetailGroup::ResetTable()
{
    m_pTable->clearContents();
    m_pTable->setRowCount(0);
    m_nMaxIndex = 0;
    m_bInitedScreenData = false;
}

void VirusDetailGroup::InitData()
{
    if(m_bInitedScreenData){
        ResetTable();
    }
    m_bInitedScreenData = true;

    list<VirusInfoEntry> lentrys;
    int count = m_pMain->GetVirusLogs(lentrys);
    if(count > 0){
        list<VirusInfoEntry>::iterator it = lentrys.begin();
        while(it != lentrys.end()){
//long    mScanLogID;
//string  mVirusName;
//int     mVirusType;
//string  mOriginPath;
//string  mOriginName;
//int     mDealState;
//long    mTime;
//long    mFileSize;
//string  mQuarantine;
//int     mReported;    
//lheader << u8"序号" << u8"处理时间" << u8"文件路径" << u8"病毒名称" << u8"扫描事件" << u8"事件号" << u8"威胁类型" << u8"状态";

            AddItem(*it);
            it++;
        }        
    }
}

const string en_rs_virus_type[] = {"Suspicious File", "Virus", "Worm", "Trojan", "Rootkit", "Adware", "Backdoor", "Spyware"};
const string chn_rs_virus_type[] = {u8"可疑文件", u8"病毒", u8"蠕虫", u8"木马", u8"Rootkit", u8"广告软件", u8"后门", u8"壳"};

int VirusDetailGroup::FindVirusType(const string& virus, string& stype)
{
    int i;
    string vtype = virus.substr(0, virus.find("."));
    for(i=0; i<8; i++){
        if(strcasecmp(vtype.c_str(), en_rs_virus_type[i].c_str()) == 0){
            stype = chn_rs_virus_type[i];
            return 0;
        }
    }
    return -1;
}

//typedef enum {
//    ALL_DISK = 0,
//    QUICK_DISK,
//    USER_DISK,
//    SPECIAL_FILE,
//    MONITOR_FILE,
//    MEMORY
//}DiskType  全盘扫描 快速扫描 自定义扫描 特殊文件扫描 文件监控 内存监控  -100 主动防御

void VirusDetailGroup::AddItem(VirusInfoEntry &info)
{
    int rowCount = m_pTable->rowCount();
    m_pTable->insertRow(rowCount);
    const char state[][8] = {"处理","忽略","清除","删除"};
    const char scantype[][30] = {"全盘扫描", "快速扫描", "自定义扫描", "右键扫描", "文件监控", "内存监控"};
    char buf[128]= {0};

    QTableWidgetItem  *itemID = new QTableWidgetItem(QString("%1").arg(++m_nMaxIndex,0,10));
    m_pTable->setItem(rowCount, 0, itemID);
    const time_t vtime = info.mTime;
    struct tm *tvtime = localtime(&vtime);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", tvtime);
    QTableWidgetItem  *itemTime = new QTableWidgetItem(buf);
    m_pTable->setItem(rowCount, 1, itemTime);
    string sfile;
    if(info.mOriginPath.rfind('/') != (info.mOriginPath.size()-1 && info.mOriginPath[info.mOriginPath.size()-1] != '/'))
        sfile = info.mOriginPath + "/" + info.mOriginName;
    else
        sfile = info.mOriginPath + info.mOriginName;
    QTableWidgetItem *itemPath = new QTableWidgetItem(sfile.c_str());
    m_pTable->setItem(rowCount, 2, itemPath);
    QTableWidgetItem *itemVName = new QTableWidgetItem(info.mVirusName.c_str());
    m_pTable->setItem(rowCount, 3, itemVName);
    QString evtype = "主动防御";
    if (info.mVirusType != -100) {
        evtype = scantype[info.mVirusType&0xffff];
    }
    QTableWidgetItem *itemEvent = new QTableWidgetItem(evtype);
    m_pTable->setItem(rowCount, 4, itemEvent);
    QTableWidgetItem *itemEID = new QTableWidgetItem(QString("%1").arg(info.mScanLogID,0,10));
    m_pTable->setItem(rowCount, 5, itemEID);
    string vlevel;
    if(FindVirusType(info.mVirusName, vlevel) != 0){
        vlevel = chn_rs_virus_type[0];
    }
    QTableWidgetItem *itemType = new QTableWidgetItem(QString::fromStdString(vlevel));
    m_pTable->setItem(rowCount, 6, itemType);
    QTableWidgetItem *itemState = new QTableWidgetItem(state[info.mDealState]);
    m_pTable->setItem(rowCount, 7, itemState);

}

void VirusDetailGroup::resizeEvent(QResizeEvent *event)
{
    m_plLine->setFixedWidth(event->size().width() - 100);
    m_pTable->setFixedSize(event->size().width()-50, event->size().height()-100);
}
