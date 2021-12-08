#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "common/pubdef.h"
#include "dirtable.h"
#include "procnametable.h"
#include "dirselectdialog.h"
#include "filegroup.h"


FileGroup::FileGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"文件监控");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(580);
    setFlat(true);

    setStyleSheet("QGroupBox {\
                  font-size:13px; \
                  border: 2px solid #7B599F;\
                  margin-top: 1ex; \
                  }\
                  QGroupBox::title {\
                  top: 1px;\
                  left: 20px;\
                  subcontrol-origin: margin;\
                  subcontrol-position: top left;}");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(25, 20, 0, 10);

    /*
    QLabel *lFile = new QLabel(u8"文件监控：", this);
    lFile->setFont(font);
    lFile->adjustSize();
    m_pclStartAtPowerOn = new CheckboxLabel(this, u8"开机启用");
    m_pclStartAtPowerOn->setFont(font);
    QHBoxLayout *h1layout = new QHBoxLayout;
    h1layout->setContentsMargins(0, 0, 0, 0);
    h1layout->addWidget(lFile, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addSpacing(13);
    h1layout->addWidget(m_pclStartAtPowerOn, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addStretch();
    vlayout->addLayout(h1layout);
    */

    /*
    QLabel *lSmartBlackList = new QLabel(u8"智能黑名单：", this);
    lSmartBlackList->setFont(font);
    lSmartBlackList->adjustSize();
    m_pclStartSmartBlacklist = new CheckboxLabel(this, u8"开启");
    m_pclStartSmartBlacklist->setFont(font);
    QHBoxLayout *h2layout = new QHBoxLayout;
    h2layout->setContentsMargins(0, 0, 0, 0);
    h2layout->addWidget(lSmartBlackList, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h2layout->addWidget(m_pclStartSmartBlacklist, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h2layout->addStretch();
    vlayout->addLayout(h2layout);
    */
//    QLabel *lMonitorSet = new QLabel(u8"监控设置：", this);
//    lMonitorSet->setFont(font);
//    lMonitorSet->adjustSize();
//    m_pclStartKernelScan = new CheckboxLabel(this, u8"开启内核监控");
//    m_pclStartKernelScan->setFont(font);
//    QHBoxLayout *h3layout = new QHBoxLayout;
//    h3layout->setContentsMargins(0, 0, 0, 0);
//    h3layout->addWidget(lMonitorSet, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addSpacing(13);
//    h3layout->addWidget(m_pclStartKernelScan, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addStretch();
//    vlayout->addLayout(h3layout);

    QLabel *lMonitorMode = new QLabel(u8"监控模式：", this);
    lMonitorMode->setFont(font);
    lMonitorMode->adjustSize();
    m_pbgMode = new QButtonGroup;
    //m_prbFastMode = new RadioButton(u8"极速", this);
    m_prbStandardMode = new RadioButton(u8"标准", this);
    m_prbProfessionalMode = new RadioButton(u8"专业", this);
    //m_prbPlusMode = new RadioButton(u8"增强", this);
    //m_pbgMode->addButton(m_prbFastMode, APP_MODE_FAST);
    m_pbgMode->addButton(m_prbStandardMode, APP_MODE_STANDARD);
    m_pbgMode->addButton(m_prbProfessionalMode, APP_MODE_PROFESSIONAL);
    //m_pbgMode->addButton(m_prbPlusMode, APP_MODE_PLUS);
    //m_prbFastMode->setFont(font);
    m_prbStandardMode->setFont(font);
    m_prbProfessionalMode->setFont(font);
    //m_prbPlusMode->setFont(font);
    QHBoxLayout *h4layout = new QHBoxLayout;
    h4layout->setContentsMargins(0, 0, 0, 0);
    h4layout->addWidget(lMonitorMode, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h4layout->addSpacing(13);

    h4layout->addWidget(m_prbStandardMode, 0, Qt::AlignLeft|Qt::AlignVCenter);
    //h4layout->addWidget(m_prbFastMode, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h4layout->addSpacing(65);

    h4layout->addWidget(m_prbProfessionalMode, 0, Qt::AlignLeft|Qt::AlignVCenter);

    h4layout->addStretch();

    //QHBoxLayout *h5layout = new QHBoxLayout;
    //h5layout->setContentsMargins(0, 0, 0, 0);
    //h5layout->addSpacing(83);
    //h5layout->addWidget(m_prbProfessionalMode, 0, Qt::AlignLeft|Qt::AlignVCenter);
    //h5layout->addSpacing(65);
    //h5layout->addWidget(m_prbPlusMode, 0, Qt::AlignLeft|Qt::AlignVCenter);
    //h5layout->addStretch();
    vlayout->addLayout(h4layout);
    //vlayout->addLayout(h5layout);

    QLabel *lFileType = new QLabel(u8"文件类型：", this);
    lFileType->setFont(font);
    lFileType->adjustSize();
    m_pbgMode = new QButtonGroup;
    m_prbAllFiles = new RadioButton(u8"所有文件", this);
    m_prbProgDoc = new RadioButton(u8"程序和文档", this);
    m_pbgMode->addButton(m_prbAllFiles, APP_FILE_ALL);
    m_pbgMode->addButton(m_prbProgDoc, APP_FILE_PROGDOC);
    m_prbAllFiles->setFont(font);
    m_prbProgDoc->setFont(font);
    QHBoxLayout *h6layout = new QHBoxLayout;
    h6layout->setContentsMargins(0, 0, 0, 0);
    h6layout->addWidget(lFileType, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h6layout->addSpacing(13);
    h6layout->addWidget(m_prbAllFiles, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h6layout->addSpacing(38);
    h6layout->addWidget(m_prbProgDoc, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h6layout->addStretch();
    vlayout->addLayout(h6layout);

    QLabel *lMonitorAccelerate = new QLabel(u8"监控加速：", this);
    lMonitorAccelerate->setFont(font);
    lMonitorAccelerate->adjustSize();
    m_pTrustProgAnalysis = new CheckboxLabel(this, u8"信任程序分析");
    m_pTrustProgAnalysis->setFont(font);
    QHBoxLayout *h7layout = new QHBoxLayout;
    h7layout->setContentsMargins(0, 0, 0, 0);
    h7layout->addWidget(lMonitorAccelerate, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h7layout->addSpacing(13);
    h7layout->addWidget(m_pTrustProgAnalysis, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h7layout->addStretch();
    vlayout->addLayout(h7layout);

//    QLabel *lEmbedScan = new QLabel(u8"嵌入查杀：", this);
//    lEmbedScan->setFont(font);
//    lEmbedScan->adjustSize();
//    m_pclEmbedSacan = new CheckboxLabel(this, u8"启用嵌入式查杀");
//    m_pclEmbedSacan->setFont(font);
//    QHBoxLayout *h8layout = new QHBoxLayout;
//    h8layout->setContentsMargins(0, 0, 0, 0);
//    h8layout->addWidget(lEmbedScan, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h8layout->addSpacing(13);
//    h8layout->addWidget(m_pclEmbedSacan, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h8layout->addStretch();
//    vlayout->addLayout(h8layout);

    QLabel  *lKVEngine = new QLabel(u8"查杀引擎：", this);
    lKVEngine->adjustSize();
    lKVEngine->setFont(font);
    m_pclOnWildlist = new CheckboxLabel(this, u8"仅查杀流行病毒");
    m_pclHeuristScan = new CheckboxLabel(this, u8"启发式查杀");
    m_pclProcCompressed = new CheckboxLabel(this, u8"启动压缩包查杀");
    lcomment1 = new QLabel(u8"查杀不大于", this);
    lcomment2 = new QLabel(u8"M的压缩包", this);
    QLabel *lcomment3 = new QLabel(u8"（重点查杀活跃病毒）", this);
    QLabel *lcomment4 = new QLabel(u8"（可有效发现可疑病毒）", this);
    QLabel *lcomment5 = new QLabel(u8"（查杀压缩包内的文件）", this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::gray);
    lcomment3->setPalette(pa);
    lcomment4->setPalette(pa);
    lcomment5->setPalette(pa);
    lcomment1->adjustSize();
    lcomment2->adjustSize();
    lcomment3->adjustSize();
    lcomment4->adjustSize();
    lcomment5->adjustSize();
    m_pleSize = new QLineEdit("100", this);
    m_pclOnWildlist->setFont(font);
    m_pclHeuristScan->setFont(font);
    m_pclProcCompressed->setFont(font);
    lcomment1->setFont(font);
    lcomment2->setFont(font);
    QPalette pd;
    pd.setColor(QPalette ::Disabled,QPalette::Text,Qt::gray);
    lcomment1->setPalette(pd);
    lcomment2->setPalette(pd);
    lcomment3->setFont(font);
    lcomment4->setFont(font);
    lcomment5->setFont(font);
    m_pleSize->setFont(font);
    m_pleSize->setFixedWidth(30);
    QHBoxLayout *h9layout = new QHBoxLayout;
    h9layout->setContentsMargins(0, 0, 0, 0);
    h9layout->addWidget(lKVEngine, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h9layout->addSpacing(13);
    h9layout->addWidget(m_pclOnWildlist, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h9layout->addWidget(lcomment3, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h9layout->addStretch();
    vlayout->addLayout(h9layout);
    QHBoxLayout *halayout = new QHBoxLayout;
    halayout->setContentsMargins(0, 0, 0, 0);
    halayout->addSpacing(83);
    halayout->addWidget(m_pclHeuristScan, 0, Qt::AlignLeft|Qt::AlignVCenter);
    halayout->addWidget(lcomment4, 0, Qt::AlignLeft|Qt::AlignVCenter);
    halayout->addStretch();
    vlayout->addLayout(halayout);
    QHBoxLayout *hblayout = new QHBoxLayout;
    hblayout->setContentsMargins(0, 0, 0, 0);
    hblayout->addSpacing(83);
    hblayout->addWidget(m_pclProcCompressed, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hblayout->addWidget(lcomment5, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hblayout->addStretch();
    vlayout->addLayout(hblayout);
    QHBoxLayout *hclayout = new QHBoxLayout;
    hclayout->setContentsMargins(0, 0, 0, 0);
    hclayout->addSpacing(110);
    hclayout->addWidget(lcomment1, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hclayout->addWidget(m_pleSize, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hclayout->addWidget(lcomment2, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hclayout->addStretch();
    vlayout->addLayout(hclayout);

    QLabel *lFoundVirus = new QLabel(u8"发现病毒：", this);
    lFoundVirus->setFont(font);
    lFoundVirus->adjustSize();
    m_pbgFoundVirus = new QButtonGroup;
    m_prbFoundVAuto = new RadioButton(u8"自动处理", this);
    m_prbFoundVManual = new RadioButton(u8"手动处理", this);
    m_prbFoundVIgnore = new RadioButton(u8"不处理", this);
    m_pbgFoundVirus->addButton(m_prbFoundVAuto, APP_FOUNDV_AUTO);
    m_pbgFoundVirus->addButton(m_prbFoundVManual, APP_FOUNDV_MANUAL);
    m_pbgFoundVirus->addButton(m_prbFoundVIgnore, APP_FOUNDV_IGNORE);
    m_prbFoundVAuto->setFont(font);
    m_prbFoundVManual->setFont(font);
    m_prbFoundVIgnore->setFont(font);
    QHBoxLayout *hdlayout = new QHBoxLayout;
    hdlayout->setContentsMargins(0, 0, 0, 0);
    hdlayout->addWidget(lFoundVirus, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hdlayout->addSpacing(13);
    hdlayout->addWidget(m_prbFoundVAuto, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hdlayout->addSpacing(40);
    hdlayout->addWidget(m_prbFoundVManual, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hdlayout->addSpacing(40);
    hdlayout->addWidget(m_prbFoundVIgnore, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hdlayout->addStretch();
    vlayout->addLayout(hdlayout);

    QLabel *lFailV = new QLabel(u8"清除失败：", this);
    lFailV->setFont(font);
    lFailV->adjustSize();
    m_pbgFailV = new QButtonGroup;
    m_prbFailVDelete = new RadioButton(u8"直接删除", this);
    m_prbFailVIgnore = new RadioButton(u8"不处理", this);
    m_pbgFailV->addButton(m_prbFailVDelete);
    m_pbgFailV->addButton(m_prbFailVIgnore);
    m_prbFailVDelete->setFont(font);
    m_prbFailVIgnore->setFont(font);
    QHBoxLayout *helayout = new QHBoxLayout;
    helayout->setContentsMargins(0, 0, 0, 0);
    helayout->addWidget(lFailV, 0, Qt::AlignLeft|Qt::AlignVCenter);
    helayout->addSpacing(13);
    helayout->addWidget(m_prbFailVDelete, 0, Qt::AlignLeft|Qt::AlignVCenter);
    helayout->addSpacing(40);
    helayout->addWidget(m_prbFailVIgnore, 0, Qt::AlignLeft|Qt::AlignVCenter);
    helayout->addStretch();
    vlayout->addLayout(helayout);

    QLabel * lReport = new QLabel(u8"报告结果：", this);
    lReport->setFont(font);
    lReport->adjustSize();
    m_pclNotifyMe = new CheckboxLabel(this, u8"病毒清除成功后通知我");
    m_pclNotifyMe->setFont(font);
    QHBoxLayout *hflayout = new QHBoxLayout;
    hflayout->setContentsMargins(0, 0, 0, 0);
    hflayout->addWidget(lReport, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hflayout->addSpacing(13);
    hflayout->addWidget(m_pclNotifyMe, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hflayout->addStretch();
    vlayout->addLayout(hflayout);

#ifdef QT_NEW_HEADER_54
    QFont tfont("Arial", 10, QFont::Thin, false);
#else
    QFont tfont("Arial", 10, QFont::Light, false);
#endif

    QGridLayout *glayout = new QGridLayout;

    QHBoxLayout *fdlayout = new QHBoxLayout;
    QLabel *pDLabel = new QLabel(u8"监控目录：", this);
    pDLabel->setFont(tfont);
    m_ppbDir = new PushButton(this);
    m_ppbDir->loadPixmap(":/resource/plus_btn");
    fdlayout->addWidget(pDLabel, 0, Qt::AlignRight);
    fdlayout->addWidget(m_ppbDir, 0, Qt::AlignRight);
    glayout->addLayout(fdlayout, 0, 0, Qt::AlignRight);

//    QVBoxLayout *all_layout = new QVBoxLayout;
    QHBoxLayout *extlayout = new QHBoxLayout;
    QLabel *pProcLabel = new QLabel(u8"监控进程：", this);
    pProcLabel->setFont(tfont);
    m_ppbProcName = new PushButton(this);
    m_ppbProcName->loadPixmap(":/resource/plus_btn");
    extlayout->addWidget(pProcLabel, 0, Qt::AlignRight);
    extlayout->addWidget(m_ppbProcName, 0, Qt::AlignRight);
    glayout->addLayout(extlayout, 0, 1, Qt::AlignRight);

    m_pDirTable = new DirTable(this);
    glayout->addWidget(m_pDirTable, 1, 0);

    m_pProcNameTable = new ProcNameTable(this);
    glayout->addWidget(m_pProcNameTable, 1, 1);

    vlayout->addSpacing(10);
    vlayout->addLayout(glayout);
    setLayout(vlayout);

    connect(m_pclProcCompressed, SIGNAL(clicked()), this, SLOT(SetSizeEditValid()));
    connect(m_ppbDir, SIGNAL(clicked()), this, SLOT(AddDirClicked()));
    connect(m_ppbProcName, SIGNAL(clicked()), this, SLOT(AddProcNameClicked()));

    SetFocus(m_bFocus);
}

void FileGroup::SetFocus(bool bFocus)
{
    m_bFocus = bFocus;
    if(m_bFocus){
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #7B599F;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");

    }else{
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #EFEBE7;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");
    }
    update();
}

void FileGroup::SetSizeEditValid()
{
    if(m_pclProcCompressed->m_bChecked == false){
        m_pleSize->setDisabled(true);
        lcomment1->setDisabled(true);
        lcomment2->setDisabled(true);
    }else{
        m_pleSize->setDisabled(false);
        lcomment1->setDisabled(false);
        lcomment2->setDisabled(false);
    }
    update();
}

void FileGroup::AddDirClicked()
{
    DirSelectDialog dlg(this);
    dlg.m_nType = APP_MENU_DIRSEL_TYPE_DIR;
    if(dlg.exec() == QDialog::Accepted){
        m_pDirTable->AddItem(dlg.m_nType, dlg.m_sName);
    }
}

void FileGroup::AddProcNameClicked()
{
    m_pProcNameTable->AddProcNameStart();
}
