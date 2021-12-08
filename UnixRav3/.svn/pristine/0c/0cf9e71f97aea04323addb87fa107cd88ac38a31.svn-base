#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>

#include "common/pubdef.h"

#include "appreinforcegroup.h"



AppReinforceGroup::AppReinforceGroup(QWidget *parent):QWidget(parent)
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-20);
    //setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36); //size 36 is the height of sysclose_btn.png

    m_plScanEvent = new QLabel(u8"应用加固", this);
    m_plScanEvent->adjustSize();
    m_plLine = new QLabel(this);
    m_plLine->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(57, 23, 92);");
    m_plLine->setFixedSize(535, 2);

    m_plTime = new QLabel(u8"时间:", this);
    m_plTime->adjustSize();
    m_plProtectType = new QLabel(u8"防护类型:", this);
    m_plProtectType->adjustSize();

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    m_plExportLog = new QLabel(u8"导出日志", this);
    m_plExportLog->adjustSize();
    m_plExportLog->setPalette(pa);

    m_pcTime = new QComboBox(this);
    m_pcProtectType = new QComboBox(this);
    m_pTable = new QTableWidget(this);
    QStringList lheader;
    lheader << u8"序号" << u8"拦截时间" << u8"防护类型" << u8"攻击来源" << u8"攻击目标" << u8"攻击动作" << u8"补充信息";
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
    m_pTable->horizontalHeader()->resizeSection(2,60);
    m_pTable->horizontalHeader()->resizeSection(3,110);
    m_pTable->horizontalHeader()->resizeSection(4,100);
    m_pTable->horizontalHeader()->resizeSection(5,100);
    m_pTable->horizontalHeader()->resizeSection(6,100);

    m_pTable->setFixedSize(610, 375);
    m_pTable->setRowCount(16);

    InitControls();
}

void AppReinforceGroup::InitControls()
{
    QStringList tlist;
    tlist << u8"全部" << u8"今天" << u8"最近三天" << u8"最近一周" << u8"最近一月";
    m_pcTime->addItems(tlist);
    m_pcTime->setCurrentIndex(1);
    QStringList clist;
    clist << u8"全部" << u8"浏览器" << u8"办公软件";
    m_pcProtectType->addItems(clist);

    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setMargin(10);
    //hflayout->setContentsMargins(0,0,0,0);
    hflayout->addWidget(m_plScanEvent, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hflayout->addWidget(m_plLine, 0 , Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout *hslayout = new QHBoxLayout;
    hslayout->setMargin(10);
    //hslayout->setContentsMargins(0,0,0,0);
    hslayout->addWidget(m_plTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hslayout->addWidget(m_pcTime, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hslayout->addSpacing(5);
    hslayout->addWidget(m_plProtectType, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hslayout->addWidget(m_pcProtectType, 0 , Qt::AlignLeft | Qt::AlignVCenter);
    hslayout->addStretch();
    hslayout->addWidget(m_plExportLog, 0 , Qt::AlignRight | Qt::AlignVCenter);

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->setContentsMargins(10,0,10,10);
    vlayout->addLayout(hflayout);
    vlayout->addLayout(hslayout);
    vlayout->addWidget(m_pTable, 0, Qt::AlignHCenter);

    setLayout(vlayout);
}
