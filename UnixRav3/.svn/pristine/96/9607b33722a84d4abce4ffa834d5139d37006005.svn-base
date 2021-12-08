#include "isolationgroup.h"
#include "common/pubdef.h"
#include "common/searchedit.h"
#include "common/pushbutton.h"
#include "common/clickedlabel.h"
#include "confirmrestore.h"
#include "mainwindow.h"
#include "../Storage/Entry/quarantinelog.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPalette>

#include <string.h>
#include <time.h>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


IsolationGroup::IsolationGroup(QWidget *parent):QWidget(parent),m_bInitedScreenData(false)
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    //setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-20);
    //setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36); //size 36 is the height of sysclose_btn.png

    m_plTitle = new QLabel(u8"隔离区", this);
    m_plTitle->adjustSize();
    m_plLine = new QLabel(this);
    m_plLine->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(57, 23, 92);");
    m_plLine->setFixedSize(535, 2);

    m_plComment = new QLabel(u8"文件隔离区中保存了杀毒操作中被删除的文件的备份，您可以将这些文件恢复到指定位置。", this);
    m_plComment->adjustSize();
//    m_plSearch = new QLabel(u8"文件搜索:", this);
//    m_plSearch->adjustSize();

//    m_pseSearchTxt = new SearchEdit(this);
//    m_pseSearchTxt->setFixedSize(464, 24);

//    m_pbtnRefresh = new PushButton(this);
//    m_pbtnRefresh->loadPixmap(":/resource/refresh_btn");
//    m_pbtnRefresh->setCursor(Qt::PointingHandCursor);

    m_pTable = new QTableWidget(this);
    m_pTable->setFrameShape(QFrame::NoFrame);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //m_pTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pTable->setSelectionMode(QAbstractItemView::MultiSelection);
    QStringList lheader;
    lheader << u8"序号" << u8"文件名" << u8"目标文件" << u8"病毒名称" << u8"隔离时间" << u8"大小";
    m_pTable->setColumnCount(6);
    m_pTable->setHorizontalHeaderLabels(lheader);
    m_pTable->verticalHeader()->setDefaultSectionSize(25);
    m_pTable->verticalHeader()->hide();
    m_pTable->horizontalHeader()->setFixedHeight(25);

    m_pTable->horizontalHeader()->resizeSection(0,50);
    m_pTable->horizontalHeader()->resizeSection(1,110);
    m_pTable->horizontalHeader()->resizeSection(2,100);
    m_pTable->horizontalHeader()->resizeSection(3,170);
    m_pTable->horizontalHeader()->resizeSection(4,110);
    m_pTable->horizontalHeader()->resizeSection(5,60);

    m_pTable->setFixedSize(610, 260);
    m_pTable->setRowCount(0);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
//    m_pclAddToWhiteList = new ClickedLabel(u8"加入白名单", this);
//    m_pclAddToWhiteList->adjustSize();
//    m_pclAddToWhiteList->setCursor(Qt::PointingHandCursor);
//    m_pclAddToWhiteList->setPalette(pa);
    m_pbtnRestoreToSource = new PushButton(this);
    m_pbtnRestoreToSource->loadPixmap(":/resource/restore_to_source_btn");
//    m_pbtnRestoreTo = new PushButton(this);
//    m_pbtnRestoreTo->loadPixmap(":/resource/restore_to_btn");
    m_pDeleteSelected = new PushButton(this);
    m_pDeleteSelected->loadPixmap(":/resource/delete_selected_btn");

    InitControls();
    m_nMaxIndex = 0;

    connect(m_pbtnRestoreToSource, SIGNAL(clicked()), this, SLOT(RestoreToSourceQuery()));
    connect(m_pDeleteSelected, SIGNAL(clicked()), this, SLOT(DeleteDest()));
}

IsolationGroup::~IsolationGroup()
{
    vector<QuarantineLog*>::iterator it = m_logs.begin();
    while(it != m_logs.end()) {
        QuarantineLog *ql = *it;
        delete ql;
        ql = NULL;
        it++;
    }
    m_logs.clear();
}

void IsolationGroup::InitControls()
{
    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setMargin(10);
    hflayout->setSpacing(10);
    //hflayout->setContentsMargins(0,0,0,0);
    hflayout->addWidget(m_plTitle, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hflayout->addWidget(m_plLine, 0 , Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout *hslayout = new QHBoxLayout;
    hslayout->setMargin(10);
    //hslayout->setContentsMargins(0,0,0,0);
    hslayout->addWidget(m_plComment, 0 , Qt::AlignLeft | Qt::AlignVCenter);

//    QHBoxLayout *htlayout = new QHBoxLayout;
//    htlayout->setMargin(10);
//    htlayout->addWidget(m_plSearch, 0, Qt::AlignLeft | Qt::AlignVCenter);
//    htlayout->addWidget(m_pseSearchTxt, 0, Qt::AlignLeading | Qt::AlignVCenter);
//    htlayout->addStretch();
//    htlayout->addWidget(m_pbtnRefresh, 0, Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *hdlayout = new QHBoxLayout;
    hdlayout->setMargin(10);
    hdlayout->addStretch();
//    hdlayout->addWidget(m_pclAddToWhiteList, 0, Qt::AlignRight | Qt::AlignVCenter);
    hdlayout->addWidget(m_pbtnRestoreToSource, 0, Qt::AlignRight | Qt::AlignVCenter);
//    hdlayout->addWidget(m_pbtnRestoreTo, 0, Qt::AlignRight | Qt::AlignVCenter);
    hdlayout->addWidget(m_pDeleteSelected, 0, Qt::AlignRight | Qt::AlignVCenter);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(10,0,10,10);
    vlayout->addLayout(hflayout);
    vlayout->addLayout(hslayout);
    //vlayout->addLayout(htlayout);
    vlayout->addWidget(m_pTable, 0, Qt::AlignHCenter);
    vlayout->addLayout(hdlayout);
    setLayout(vlayout);
}

void IsolationGroup::ResetTable()
{
    m_pTable->clearContents();
    m_pTable->setRowCount(0);
    vector<QuarantineLog*>::iterator it = m_logs.begin();
    while(it != m_logs.end()) {
        QuarantineLog *ql = *it;
        delete ql;
        ql = NULL;
        it++;
    }
    m_logs.clear();
    m_nMaxIndex = 0;
    m_bInitedScreenData = false;
}

void IsolationGroup::InitData()
{
    if(m_bInitedScreenData){
        ResetTable();
    }
    m_bInitedScreenData = true;

    list<QuarantineLog>  lentrys;
    int count = m_pMain->GetQuaLogs(lentrys);
    if(count > 0){
        list<QuarantineLog>::iterator it = lentrys.begin();
        while(it != lentrys.end()){
//int mDate;
//std::string mFile;
//std::string mSource;
//std::string mTarget;
//std::string mVirusName;
//long mSize;
//int         mUID;
//int         mGID;
//int         mPermision;
//QuarantineLog() {
//mDate = 0;
//mSize = 0;
//}
//lheader << 序号" << u8"文件名" << u8"目标文件" << u8"病毒名称" << u8"隔离时间" << u8"大小
            printf("IsolationGroup::InitData before AddItem\n");
            QuarantineLog log = *it;
            AddItem(log);
            it++;
        }
    }
}

void IsolationGroup::AddItem(QuarantineLog &info)
{
    if(info.mSize <= 0)
        return;
    int rowCount = m_pTable->rowCount();
    m_pTable->insertRow(rowCount);
    QuarantineLog *ql = new QuarantineLog;
    *ql = info;
    m_logs.push_back(ql); //info;
    char buf[128]= {0};
    QTableWidgetItem  *itemID = new QTableWidgetItem(QString("%1").arg(++m_nMaxIndex,0,10));
    m_pTable->setItem(rowCount, 0, itemID);

    QTableWidgetItem *itemPath = new QTableWidgetItem(info.mTarget.c_str());
    m_pTable->setItem(rowCount, 1, itemPath);

    QTableWidgetItem *itemTarget = new QTableWidgetItem(info.mSource.c_str());
    m_pTable->setItem(rowCount, 2, itemTarget);

    QTableWidgetItem *itemVName = new QTableWidgetItem(info.mVirusName.c_str());
    m_pTable->setItem(rowCount, 3, itemVName);

    const time_t vtime = info.mDate;
    struct tm *tvtime = localtime(&vtime);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", tvtime);
    QTableWidgetItem  *itemTime = new QTableWidgetItem(buf);
    m_pTable->setItem(rowCount, 4, itemTime);

    QTableWidgetItem *itemSize = new QTableWidgetItem(QString("%1").arg(info.mSize,0,10));
    m_pTable->setItem(rowCount, 5, itemSize);

}

void IsolationGroup::RestoreToSourceQuery()
{
    ConfirmRestoreWidget cfrw(this);
    QRect pos = rect();
    QString title = "恢复隔离区文件";
    QString virus = "virus text";
    QString sfile = "file_name";

    cfrw.SetMessage(title, virus, sfile);
    cfrw.setGeometry((pos.width()-cfrw.width())/2, (pos.height()-cfrw.height())/2, cfrw.width(), cfrw.height());
    cfrw.show();
    cfrw.exec();

    if (cfrw.Value() == 1) {
        RestoreToSource(cfrw.IfAddWhiteList());
    }
}

void IsolationGroup::RestoreToSource(bool add_to_whitelist)
{
    vector<int> vselected;
    int prev_row = -1, row = -1;
    QList<QTableWidgetItem*> items = m_pTable->selectedItems();
    QList<QTableWidgetItem*>::iterator it = items.begin();
    while(it != items.end()){
        QTableWidgetItem *ptwi = *it;
        row = ptwi->row();
        if (row != prev_row) {
            vselected.push_back(row);
        }
        prev_row = row;
        it++;
    }
    sort(vselected.begin(), vselected.end());
    int count = vselected.size();
    for (int i = count-1; i>=0; i--){
        QuarantineLog *log = m_logs[vselected[i]];
       if (log != NULL){
           m_pMain->RestoreQuarantine(*log, add_to_whitelist);
           m_pTable->removeRow(vselected[i]);
           delete log;
           log = NULL;
           m_logs.erase(m_logs.begin() + vselected[i]);
       }
    }
}

void IsolationGroup::DeleteDest()
{
    vector<int> vselected;
    int prev_row = -1, row = -1;
    QList<QTableWidgetItem*> items = m_pTable->selectedItems();
    QList<QTableWidgetItem*>::iterator it = items.begin();
    while(it != items.end()){
        QTableWidgetItem *ptwi = *it;
        row = ptwi->row();
        if (row != prev_row) {
            vselected.push_back(row);
        }
        prev_row = row;
        it++;
    }
    sort(vselected.begin(), vselected.end());
    int count = vselected.size();
    for (int i = count-1; i>=0; i--){
        QuarantineLog *log = m_logs[vselected[i]];
       if (log != NULL){
           m_pMain->DeleteQuarantine(*log);
           m_pTable->removeRow(vselected[i]);
           delete log;
           log = NULL;
           m_logs.erase(m_logs.begin() + vselected[i]);
       }
    }
}

void IsolationGroup::resizeEvent(QResizeEvent *event)
{
    m_plLine->setFixedWidth(event->size().width() - 100);
    m_pTable->setFixedSize(event->size().width()-50, event->size().height()-150);
}
