#include <QVBoxLayout>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QBitmap>
#include <QMutexLocker>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>

#include <locale.h>
#include <string.h>
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>
#include <sys/utsname.h>

#include "../ScanEngine/scanconfig.h"
#include "../ScanEngine/multivirusengine.h"
#include "../Storage/Entry/logevententry.h"
#include "../Storage/Entry/scheduleentry.h"
#include "../Storage/Entry/commonentry.h"
#include "../CommonAPI/communicatebundle.h"
#include "../CommonAPI/configfile.h"
#include "../CommonAPI/commonapi.h"
#include "virusmessagebox.h"
#include "common/himagebutton.h"
#include "common/imgtxtlabel.h"
#include "common/scanpausebtn.h"
#include "mainwindow.h"
#include "titlewidget.h"
#include "headerwidget.h"
#include "pushbutton.h"
#include "bodywidget.h"
#include "common.h"
#include "dynmenu.h"
#include "modemenu.h"
#include "cinifile.h"
#include "baseconfig.h"
#include "appconfig.h"
#include "circlelink.h"
#include "common.h"
#include "virus/dirselectdialog.h"
#include "defendcenterdialog.h"
#include "defendstatewidget.h"
#include "defendwidget.h"
#include "virus/scanprogresswidget.h"
#include "checkupdatepushbutton.h"
#include "operationpower.h"
#include "offlineupdateinfodialog.h"


using namespace std;

const int TOTAL_PROGRESS = 321;
const string en_rs_status[] = {u8"默认", u8"忽略", u8"清除", u8"删除", u8"放弃"};
const string en_rs_virus_type[] = {"Suspicious File", "Virus", "Worm", "Trojan", "Rootkit", "Adware", "Backdoor", "Spyware"};
const string chn_rs_virus_type[] = {u8"可疑文件", u8"病毒", u8"蠕虫", u8"木马", u8"Rootkit", u8"广告软件", u8"后门", u8"壳"};

const RavQtUser g_Users[] = {RavQtUser(0x003F), RavQtUser(0x03FF), RavQtUser(0x003B), RavQtUser(0x003F)};


MainWindow::MainWindow(QWidget *parent) : QWidget(parent), Application(NORMAL_APP), m_pSystemTrayIcon(NULL),
    m_bInited(false),m_nScaningPercent(0),m_bIsScaning(false)
{
    m_nScanItemsCount = 0;
    m_nScanedThreaten = 0;
    m_nScanedThreatenProcessed = 0;
    m_nScanReadyState = RAVQT_NO_SCANNING;
    m_pCenterListener = nullptr;
    m_pTimer = new QTimer(this);

    if(Construct() != 0) {
        exit(-1);
    }

    initWindowsParam();

    mHeaderWidget = new HeaderWidget();
//    m_pTabWidget = new QTabWidget;
    mBodyWidget = new BodyWidget(this);
//    m_pTabWidget->addTab(mBodyWidget, u8"病毒查杀");
//    m_pTabWidget->setStyleSheet(
//                "QTabWidget::pane{border: 0px;}"
//                "QTabWidget::tab-bar{left:10px;alignment:left;}"
//                "QTabBar::tab{background:#f0f0f0; color:black; min-width:450px; min-height:40px;font:13pt '宋体';}"
//                "QTabBar::tab:hover{background:rgb(250, 250, 250);color:blue;}"
//                "QTabBar::tab:selected{border-color: white;background:#fefefe;color:blue;}");

//    m_pDefendWidget = new DefendWidget(this);
//    m_pTabWidget->addTab(m_pDefendWidget, u8"上网防护");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    layout->addWidget(mHeaderWidget, 0, Qt::AlignTop);
//    layout->addWidget(m_pTabWidget, 0, Qt::AlignBottom);
    layout->addWidget(mBodyWidget, 0, Qt::AlignTop);
    layout->addStretch();
    this->setLayout(layout);

    //this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QColor(245,245,245));
    this->setPalette(palette);
    this->setWindowState(Qt::WindowMinimized | Qt::WindowActive);
    setFixedSize(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);

    mDynMenu = new DynMenu(IsSecretMachine() || mAppConfig->GetCenterURL().empty());
    bool permit = true;
    UserType type = GetUserType();
//    qDebug("MainWindow::MainWindow UserType %d", type);
    if(IsSecretMachine()){
        permit = g_Users[type].HavePower(OperationPower::OP_POWER_VIEWLOG);
        if (type != SECADM){
            this->mHeaderWidget->mTitleView->mSettingBtn->hide();
        }
    }
    if(!permit){
        mDynMenu->HideNonAdminMenu();
    }

    if (mAppConfig->GetProductType() == 0) {
        mDynMenu->setUpdateTitle(mActived?"检测更新":"激活");
    }
    else if (mAppConfig->GetCenterURL().empty()) {
        mDynMenu->setUpdateTitle("", false);
    }


    mModeMenu = new ModeMenu;
    m_pMsgBox = new VirusMessageDialog(this);
    m_pMsgBox->hide();
    mUpdateInfoDlg = new OfflineUpdateInfoDialog(this);
    mUpdateInfoDlg->hide();

    connect(mDynMenu,SIGNAL(triggered(QAction*)),this,SLOT(trigerMenu(QAction*)));
    //connect(mModeMenu, &ModeMenu::triggered, this, &MainWindow::trigerModeMenu);
    connect(mModeMenu, SIGNAL(triggered(QAction*)), this, SLOT(trigerModeMenu(QAction*)));
    //connect(this->mHeaderWidget->mTitleView->mCloseBtn, &PushButton::clicked, this, &MainWindow::close);
    connect(this->mHeaderWidget->mTitleView->mCloseBtn,SIGNAL(clicked(bool)), this, SLOT(onUIExit()));
    //connect(this->mHeaderWidget->mTitleView->mMiniBtn, &PushButton::clicked, this, &MainWindow::setMinimum);

    connect(this->mHeaderWidget->mTitleView->mMenuBtn, SIGNAL(clicked(bool)), this, SLOT(popMenu()));
    connect(this->mHeaderWidget->mTitleView->mSettingBtn, SIGNAL(clicked(bool)), this, SLOT(trigerSetting()));

    if (!IsSecretMachine()) {
        // 保密机取消模式选择
        connect(this->mBodyWidget->m_pScanTopBody->m_pModeSelector, SIGNAL(clicked(bool)), this, SLOT(popScanningSelectModeMenu()));
    }

    connect(this->mBodyWidget->m_pTopBody->m_pAllKillButton, SIGNAL(clicked(bool)), this, SLOT(ScanAllDisk()));
    connect(this->mBodyWidget->m_pTopBody->m_pFastKillButton, SIGNAL(clicked(bool)), this, SLOT(QuickScanPath()));
    connect(this->mBodyWidget->m_pTopBody->m_pCustomKillButton, SIGNAL(clicked(bool)), this,SLOT(CustomScanPath()));

    connect(this, SIGNAL(sigPauseContinueMessage()), this, SLOT(onSignaledPauseContinue()));
    connect(this->mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn, SIGNAL(clicked(bool)), this, SLOT(onClickPauseContinueWindowScan()));
    connect(this, SIGNAL(sigStopScanMessage()), this, SLOT(onSignaledStopScan()));
    connect(this->mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pStopBtn, SIGNAL(clicked(bool)), this, SLOT(StopWindowsScan()));
    connect(this->mBodyWidget->m_pScanMidBody->m_pBtnQuit, SIGNAL(clicked(bool)), this, SLOT(QuitStopState()));
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnScaningTimer()));
//    connect(mBodyWidget->m_pBotBody->m_pcilAutoProcVirus, &CheckboxImageLabel::clicked, this, &MainWindow::OnClickedAutoProcVirus);
//    connect(mBodyWidget->m_pBotBody->m_pcilShutdownAfterScan, &CheckboxImageLabel::clicked, this, &MainWindow::OnClickedAutoPoweroffAfterScan);
//    connect(m_pTabWidget, &QTabWidget::currentChanged, this, &MainWindow::OnTabWidgetCurrentChanged);
//    connect(m_pDefendWidget, &DefendWidget::valueChanged, this, &MainWindow::OnDefendStateChange);
//    connect(mHeaderWidget->m_pDefendStateWidget->m_pbtnOpenAll, &HImageButton::clicked, this, &MainWindow::OnClickedDefendOpenAll);
    if (IsSecretMachine()) {
        connect(mBodyWidget->m_pBotBody->m_cupbCheckUpdate, SIGNAL(clicked(bool)), this, SLOT(OnClickOfflineUpdateVirusDB()));
    }
    connect(mBodyWidget->m_pMidBody->m_pDefendButton, SIGNAL(clicked(bool)), this, SLOT(OnClickedDefendCenter()));

    m_pSystemTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/resource/logo");
    m_pSystemTrayIcon->setIcon(icon);
    connect(m_pSystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    connect(&m_UiQueue,SIGNAL(sigLogData()), this, SLOT(onUILogData()));
    connect(&m_UiQueue, SIGNAL(sigVirusLogData()), this, SLOT(onUIVirusLogData()));
    connect(&m_UiQueue, SIGNAL(sigScanStatusData()), this, SLOT(onUIScanStatusData()));

    InitVersionTimeEtc();
    m_bInited = true;

    // 检测单机版是否注册
    RefreshRegisted();
}

MainWindow::~MainWindow()
{
    if (m_pCenterListener) {
        m_pCenterListener->Stop();
    }
    SECURITY_FREE(m_pAvConf);
}

int MainWindow::InitVersionTimeEtc()
{
    string sversion = Application::GetVersion();
    //string sversion = "12.34.56";
    string slastupdatetime = GetUpdateTime();
    mBodyWidget->m_pBotBody->SetVersion(QString::fromStdString(sversion));
    mBodyWidget->m_pBotBody->SetUpdateTime(QString::fromStdString(slastupdatetime));
    mBodyWidget->m_pScanBotBody->SetVersion(QString::fromStdString(sversion));
    mBodyWidget->m_pScanBotBody->SetUpdateTime(QString::fromStdString(slastupdatetime));

    m_pAvConf = new AntiVirusSetting;

    // TODO
//    GetAntiVirusConfig(*m_pAvConf);

    int nopened = 0;
    DefenCenterSetting dconfig;
    GetDefenCenterConfig(dconfig);
    //m_nFunctions = 0x3F;
    //mAppConfig->SetFunctions(0x3F);
    //mAppConfig->StoreConfig();
    m_nFunctions = mAppConfig->GetFunctions();
//    qDebug("MainWindow::InitVersionTimeEtc() m_nFunctions=0x%02X", m_nFunctions);
    mBodyWidget->m_pMidBody->SetFunctions(m_nFunctions);

    if(dconfig.mFileMonitorState.mValue)
        nopened++;
    if(dconfig.mUSBPluginMonitorState.mValue)
        nopened++;

    mBodyWidget->m_pMidBody->m_pDefendButton->SetBtnText("防护中心", 2, nopened);
    string majorTitle, minorTitie;
    GetTitle(majorTitle, minorTitie);
    mHeaderWidget->mTitleView->UpdateTitle(majorTitle.c_str(), minorTitie.c_str());
    m_pSystemTrayIcon->setToolTip(QString::fromStdString(majorTitle));
    QApplication::setApplicationName(majorTitle.c_str());
    if (IsSecretMachine()) {
        setWindowTitle("瑞星ESM防病毒系统(涉密专用计算平台网络版)V3.0");
        //setWindowTitle("瑞星ESM防病毒系统(涉密专用计算机单机版)V3.0");
    }else{
        setWindowTitle(majorTitle.c_str());
    }
    ScanIntelligentParams param;
    bool blocked = false;
//    GetScanIntelligentMode(param, blocked);
//    if(!blocked){
//        qDebug("MainWindow::InitVersionTimeEtc find thread count=%d", param.thread_count);
//        mBodyWidget->UpdateScaningThreadCount(param.thread_count);
//    }
    mBodyWidget->UpdateScaningThreadCount(1);
    return 0;
}

void MainWindow::OnScaningTimer()
{
    if(!m_bInited) {
        return;
    }
    QMutexLocker locker(&m_Mutex);
    if(!m_bIsScaning) {
        m_pTimer->stop();
    }
    QDateTime dt = QDateTime::currentDateTime();
    int nseconds = dt.secsTo(m_qdtStartTime);
    nseconds = nseconds==0?1:nseconds*(-1);
    //int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes
    //qDebug("MainWindow::OnScaningTimer m_nScanItemsCount:%d", m_nScanItemsCount);
    mHeaderWidget->UpdateScaningInfo(m_nScanItemsCount, m_nScanItemsCount/nseconds, m_nScanedThreaten, m_nScanedThreatenProcessed, nseconds);
    mBodyWidget->UpdateScanInfo(1, m_nScanedThreaten, 0, m_sCurrentFile);
    mHeaderWidget->UpdateScaningProgress(m_nScaningPercent);
    //qDebug("MainWindow::OnScaningTimer occured nseconds=%d", nseconds);
    UpdateScanReadyState(m_nScanReadyState);
}

//void MainWindow::OnClickedAutoProcVirus()
//{
//    AntiVirusSetting avconf;
//    GetAntiVirusConfig(avconf);
//    if(mBodyWidget->m_pBotBody->m_pcilAutoProcVirus->IsChecked()){
//        avconf.mCommon.mScanType.mValue = AntiVirusSetting::AntiCommon::AUTO_MODE;
//        UpdateAntiVirusConfig(avconf);
//    }
//    qDebug("MainWindow::OnClickedAutoProcVirus bChecked=%d\n", mBodyWidget->m_pBotBody->m_pcilAutoProcVirus->m_bChecked?1:0);
//}

//void MainWindow::OnClickedAutoPoweroffAfterScan()
//{
//    qDebug("MainWindow::OnClickedAutoPoweroffAfterScan bChecked=%d\n", mBodyWidget->m_pBotBody->m_pcilShutdownAfterScan?1:0);
//}

//void MainWindow::OnTabWidgetCurrentChanged(int nIndex)
//{
//    if(nIndex == 0){
//        mHeaderWidget->VScanState();
//    }else if(nIndex == 1){
//        bool bNormal = m_pDefendWidget->m_pTopWidget->m_nItemCount == m_pDefendWidget->m_pTopWidget->m_nOpenedItem?true:false;
//        mHeaderWidget->SetDefendState(bNormal);
//    }
//}

//void MainWindow::OnDefendStateChange(int nNormalState)
//{
//    mHeaderWidget->SetDefendState(nNormalState==1?true:false);
//}

//void MainWindow::OnClickedDefendOpenAll()
//{
//    m_pDefendWidget->OpenAllDefendItem();
//    mHeaderWidget->SetDefendState(true);
//}

void MainWindow::OnClickUpdate()
{
    UpdateOfflineVirlib("/home/miwoo/workspace/rising/v.7z");
//    qDebug("MainWindow::OnClickUpdate() occur!");
}

void MainWindow::OnClickUpdateLocal()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("选择升级文件"), "", QString::fromUtf8("7z file (*.7z)"));
    if (fileName == "") {
        return;
    }
    if (fileName.startsWith("malware")) {
        UpdateOfflineVirlib(fileName.toStdString());
    }
    else {
        Update(true, fileName.toStdString());
    }


//    QMessageBox msg(this);
//    if(ret == 0) {
//        msg.setIcon(QMessageBox::Information);
//        msg.setText(u8"离线升级成功");
//    }else{
//        msg.setIcon(QMessageBox::Warning);
//        msg.setText(u8"离线升级失败")
//    }
//    msg.exec();
}

void MainWindow::OnClickOfflineUpdateVirusDB()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("选择升级文件"), "", QString::fromUtf8("7z file (*.7z)"));
    if (fileName == "") {
        return;
    }

    int ret = Application::Update(true, fileName.toStdString());
//    if (ret == 0){
//        mUpdateInfoDlg->onUpdateWait();
//        mUpdateInfoDlg->show();
//    }else{
//        mUpdateInfoDlg->onUpdateFail();
//        mUpdateInfoDlg->show();
//    }
}

void MainWindow::OnClickedDefendCenter()
{
    //QRect pos = rect();


    DefendCenterDialog *pdlg = new DefendCenterDialog(this);
    //pdlg->setGeometry((pos.width()-pdlg->width())/2, (pos.height()-pdlg->height())/2, pdlg->width(), pdlg->height());
    //pdlg->setModal(true);
    pdlg->setStyleSheet("DefendCenterDialog {background: white;"
                        "border-bottom:1px;"
                       "border-style:solid;"
                       "border-color:black}");
    pdlg->exec();
    delete pdlg;
}

void MainWindow::UpdateDefendCenterBtnInfo(int nOpened, int nTotal)
{
    mBodyWidget->m_pMidBody->SetBtnValue(nTotal, nOpened);
    //qDebug("MainWindow::UpdateDefendCenterBtnInfo total=%d opened=%d\n", nTotal, nOpened);
}

//int MainWindow::OnCommandObserver(int event, void *args, void **result)
//{
//    if (CenterCommandObserver::CLIENT_COMMAND == event) {
//        CommandBundle *bundle = (CommandBundle*)args;
//        if (bundle) {
//            switch (bundle->mCommandID) {
//            case CommandBundle::DAEMON_EXIT: {
//                this->close();
//                break;
//            }
//            default:
//                break;
//            }
//        }
//    }
//    return -1;
//}

//int MainWindow::SendToDaemon(int command)
//{
//    std::string sock = Utils::Path::JoinPath(GetInstallPath().c_str(), FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);

//    char buffer[128];
//    int index = 0;
//    int temp = 1; //string
//    memcpy(buffer, &temp, 4);
//    index += 4;

//    temp = sizeof(int) + 6; //size, 6 is update length
//    memcpy(buffer+index, &temp, 4);
//    index += 4;

//    temp = command;
//    memcpy(buffer+index, &temp, 4);
//    index += 4;

//    memcpy(buffer+index, "update", 6);
//    index += 6;

//    if (0 == Utils::OS::SendLocal(sock.c_str(), (unsigned char*)buffer, index, &temp)) {

//    }
//    return 0;
//}

//void MainWindow::setMinimum()
//{
//    this->hide();
//    m_pSystemTrayIcon->show();
//}

void MainWindow::OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        m_pSystemTrayIcon->showMessage(QString::fromUtf8("瑞星"), QString::fromUtf8("安全云终端"), QSystemTrayIcon::Information, 2000);
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        m_pSystemTrayIcon->hide();
        break;
    default:
        break;
    }
}

void MainWindow::popMenu()
{
    //qDebug("MainWindow::popMenu clicked!");
    mDynMenu->exec(QCursor::pos());
}

void MainWindow::trigerMenu(QAction *pAction)
{
    string installpath = GetInstallPath();
    string bincmd;
    if(pAction == mDynMenu->logAction){
        if (Utils::Path::FileAvailed(Utils::Path::JoinPath(installpath.c_str(), "RAVRJJH7", NULL))) {
            bincmd = Utils::Path::JoinPath(installpath.c_str(), "RAVRJJH7", NULL);
        }
        else if (Utils::Path::FileAvailed(Utils::Path::JoinPath(installpath.c_str(), "bin/RAVRJJH7", NULL))) {
            bincmd = Utils::Path::JoinPath(installpath.c_str(), "bin/RAVRJJH7", NULL);
        }

        system(bincmd.c_str());
//        qDebug("logAction");
    }else if(pAction == mDynMenu->isolateAction){
        if (Utils::Path::FileAvailed(Utils::Path::JoinPath(installpath.c_str(), "RAVRJJH7", NULL))) {
            bincmd = Utils::Path::JoinPath(installpath.c_str(), "RAVRJJH7", NULL);
        }
        else if (Utils::Path::FileAvailed(Utils::Path::JoinPath(installpath.c_str(), "bin/RAVRJJH7", NULL))) {
            bincmd = Utils::Path::JoinPath(installpath.c_str(), "bin/RAVRJJH7", NULL);
        }
        bincmd += " -t 3";
        system(bincmd.c_str());
//        qDebug("isolateAction cmd:%s", bincmd.c_str());
    }else if(pAction == mDynMenu->updateAction){
        if (mAppConfig->GetProductType() == 0) {
            if (mActived) {
//                qDebug("updateAction");
                Update(false, bincmd);
            } else {
                bool isOK;
                QString text = QInputDialog::getText(NULL,
                                                     "激活",
                                                       "请输入序列号",
                                                       QLineEdit::Normal,
                                                       "XXXX-XXXX-XXXX-XXXX",
                                                       &isOK);
                if (isOK) {
                    int err = Register(text.toStdString());
                    QMessageBox::information(NULL, "提示",
                                                   (err==0)?"激活成功":"激活失败",
                                                   QMessageBox::Ok,
                                                   QMessageBox::Ok);
                    RefreshRegisted();
                }
            }
        } else {
            Update(false, bincmd);
        }

    }else if(pAction == mDynMenu->localUpdateAction){
        OnClickUpdateLocal();
    }else if(pAction == mDynMenu->aboutAction){
        ShowAbout();
    }
}

void MainWindow::trigerSetting()
{
    string installpath = GetInstallPath();
    string bincmd;
    if (Utils::Path::FileAvailed(Utils::Path::JoinPath(installpath.c_str(), "RAVRJJH5", NULL))) {
        bincmd = Utils::Path::JoinPath(installpath.c_str(), "RAVRJJH5", NULL);
    }
    else if (Utils::Path::FileAvailed(Utils::Path::JoinPath(installpath.c_str(), "bin/RAVRJJH5", NULL))) {
        bincmd = Utils::Path::JoinPath(installpath.c_str(), "bin/RAVRJJH5", NULL);
    }
    system(bincmd.c_str());
}

void MainWindow::popScanningSelectModeMenu()
{
    QPoint gpos = mBodyWidget->m_pScanTopBody->mapToGlobal(mBodyWidget->m_pScanTopBody->m_pModeSelector->pos());
    gpos.setY(gpos.y() + mBodyWidget->m_pScanTopBody->m_pModeSelector->height());
    mModeMenu->exec(gpos);
}

void MainWindow::trigerModeMenu(QAction *pAction)
{
    if(pAction == mModeMenu->autoAction){
        SetScanIntelligentMode(AntiVirusSetting::AntiCommon::AUTO_MODE);
    }else if(pAction == mModeMenu->officeAction){
        SetScanIntelligentMode(AntiVirusSetting::AntiCommon::OFFICE_MODE);
    }else if(pAction == mModeMenu->highspeedAction){
        SetScanIntelligentMode(AntiVirusSetting::AntiCommon::HIGH_SPEED_MODE);
    }
    UpdateScanningIntelligentMode();
}

void MainWindow::onUILogData()
{
    int scantype, count;
    string filename;
    while(1) {
        if (0 !=  m_UiQueue.GetLogData(count, scantype, filename)) {
            return;
        }
        UpdateScanReadyState(RAVQT_RECEIVED_RESULT);
        m_sCurrentFile = filename.c_str();

        QDateTime dt = QDateTime::currentDateTime();
        int nseconds = dt.secsTo(m_qdtStartTime);
        nseconds = nseconds==0?1:nseconds*(-1);
        //int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes
        //qDebug("onUILogData before UpdateScaningInfo m_nScanItemsCount=%d count=%d", m_nScanItemsCount, count);
        mHeaderWidget->UpdateScaningInfo(m_nScanItemsCount, m_nScanItemsCount/(nseconds==0?1:nseconds), m_nScanedThreaten, m_nScanedThreatenProcessed, nseconds);
        //qDebug("MainWindow::onUILogData scantype=%d length=%u\n", scantype, data->Length());
        if(count <= 0) {
            break;
        }
    }
}

void MainWindow::onUIVirusLogData()
{
    string file, virus;
    int status, count;
    while(1){
        if (0 != m_UiQueue.GetVirusLogData(count, file, virus, status)) {
            return;
        }
        UpdateScanReadyState(RAVQT_RECEIVED_RESULT);
        if(virus.size() > 0){
            int nstatus = status;
            if(status < 0 || status > 4)
                nstatus = 0;
            string stype="";
            if(FindVirusType(virus, stype) != 0){
                stype = chn_rs_virus_type[0];
            }
            mBodyWidget->AddVirusLog(virus, stype, file, en_rs_status[nstatus]);//en_rs_status[virus.mStatus]);
            m_nScanedThreaten++;
            QDateTime dt = QDateTime::currentDateTime();
            int nseconds = dt.secsTo(m_qdtStartTime);
            nseconds = nseconds==0?1:nseconds*(-1);
            //qDebug("onUIVirusLogData before UpdateScaningInfo m_nScanItemsCount=%d", m_nScanItemsCount);
            //int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes
            mHeaderWidget->UpdateScaningInfo(m_nScanItemsCount, m_nScanItemsCount/(nseconds==0?1:nseconds), m_nScanedThreaten, m_nScanedThreatenProcessed, nseconds);
        }
        //qDebug("MainWindow::onUIVirusLogData:%s  %s\n", virus.c_str(), file.c_str());
        if(count <= 0) {
            break;
        }
    }
}

void MainWindow::onUIScanStatusData()
{
    int dcount, fcount, tag, scantype, status, progress;
    string filename;
    dcount = m_UiQueue.GetScanStatusDataCount();
    UpdateScanReadyState(RAVQT_RECEIVED_RESULT);
    for(int i=0; i<dcount; i++) {
        if(m_UiQueue.GetScanStatusData(scantype, status, progress) == 0){
            //qDebug("MainWindow::onUIScanStatusData() scantype:%d status:%d progress:%d\n", scantype, status, progress);
            if (!m_bIsScaning && status != SCAN_FINISHED) {
                m_bIsScaning = true;
                switch(scantype & 0xFF) {
                case LogEventEntry::ALL_DISK:
                    ScanAllDisk(true);
                    break;
                case LogEventEntry::QUICK_DISK:
                    QuickScanPath(true);
                    break;
                case LogEventEntry::USER_DISK:
                    CustomScanPath(true);
                    break;
                default:
                    break;
                }
            }

            if(status == SCAN_FINISHED){
                //qDebug("onUiScanStatusData: SCAN_FINISHED progress=%d", progress);
                mHeaderWidget->mVirusScanWidget->mScaningWidget->m_nTotal = progress;
                mHeaderWidget->mVirusScanWidget->mScaningWidget->UpdateInfo();
                mHeaderWidget->UpdateScaningProgress(100);
                m_bIsScaning = false;
                StopWindowsScan(false, true);
            }else if(status == TOTAL_PROGRESS && progress >= 0 && progress < 100){
                //OnScaningTimer();
                m_bIsScaning = true;
                m_nScaningPercent = progress;
                //qDebug("MainWindow::onUIScanStatusData TOTLE_PROGRESS progress=%d", progress);
            }
        }else if(m_UiQueue.GetScanStatusThreadData(filename, fcount, tag) == 0){
            //qDebug("onUIScanStatusData after GetScanStatusThreadData  fcount=%d", fcount);
            if (!m_bIsScaning) {
                fcount = mHeaderWidget->mVirusScanWidget->mScaningWidget->m_nTotal;
            }
            mBodyWidget->UpdateThreadInfo(tag, fcount, filename);
        }
    }
}

void MainWindow::showWidget()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
}

void MainWindow::initWindowsParam() {
    // 固定应用程序大小
    setMinimumSize(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
    setMaximumSize(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);

    // 设置应用程序的图标
    setWindowIcon(QIcon(":/resource/logo"));
    //for huawei uos
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(1);
    setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::ShowAbout()
{
    QRect pos = rect();

    QString sVersion = QString::fromStdString(Application::GetVersion());
    QString sUTime = QString::fromStdString(GetUpdateTime());
    QString svVer = QString::fromStdString(GetVirusDbVer());
    QString svUpTime = QString::fromStdString(GetVirusUpdateTime());
    QString serial = "";
    if (mAppConfig->GetProductType() == 0) {
        serial = QString::fromStdString(mAppConfig->GetSerialNumber());
    }
    string majorName, minorName;
    GetTitle(majorName, minorName);
    //majorName = "Rising";
    //minorName = "ESM";
    //QString sVersion = "1.2.3.4";
    //QString sUTime = "2018-11-11 11:11:11";
    //QString svVer = "12.34.56";
    //QString svUpTime = "2018-12-12 12:12:12";

    AboutDialog dialog(this);
    if (IsSecretMachine()) {
        majorName = "瑞星ESM防病毒系统";
        minorName = "";
        if (mAppConfig->GetProductType() == 0) {
            minorName = "涉密专用计算机单机版V3.0";
        }
        else {
            minorName = "涉密专用计算机平台网络版V3.0";
        }

        string cpumodel = Utils::OS::GetCPUName();
        if (cpumodel.find("phytium") != string::npos
                || cpumodel.find("FT") != string::npos) {
            minorName += " 飞腾";
        }
        else if (cpumodel.find("Loongson") != string::npos) {
            minorName += " 龙芯";
        }
        else if (cpumodel.find("Hygon") != string::npos) {
            minorName += " 海光";
        }
        else if (cpumodel.find("ZHAOXIN") != string::npos) {
            minorName += " 兆芯";
        }
        else if (cpumodel.find("Kunpeng") != string::npos) {
        	minorName += " 鲲鹏";
        }
        else if (cpumodel.find("SW") == 0) {
        	minorName += " 申威";
        }

        string system = Utils::OS::GetOSReleaseName();
        if (system.find("NeoKylin") != string::npos) {
                minorName += "中标麒麟版";
        }
        else if (system.find("Kylin") != string::npos) {
            minorName += "银河麒麟版";
        }
        else if (system.find("NFS") != string::npos ||
                 system.find("方德") != string::npos) {
            minorName += "中科方德版";
        }
        //    // 以下代码是为安可项目修改的产品关于标题
        //    majorName = "瑞星ESM 下一代网络版";
        //    struct utsname uts;
        //    if (0 == uname(&uts)) {
        //        if (0 == strcmp(uts.machine, "aarch64")) {
        //            minorName = "飞腾银河麒麟版V3.0";
        //        }
        //        else if (0 == strncmp(uts.machine, "mips", 4)) {
        //            minorName = "龙芯中标麒麟版V3.0";
        //        }
        //        else {
        //            if (Utils::Path::FileAvailed("/etc/.productinfo")) {
        //                minorName = "兆芯中标麒麟版V3.0";
        //            }
        //            else {
        //                minorName = "";
        //            }
        //        }
        //    }
        //    // END
    }
    else if (IsCenterLess()) {
        majorName = "瑞星ESM防病毒系统";
        if (IsGeneralSystem()) {
            minorName = "通用单机版";
        }
        else if (IsChinaSystem()) {
            minorName = "国产单机版";
        }
    }
    dialog.SetProductName(majorName.c_str(), minorName.c_str());
    if (mAppConfig->GetProductType() == 0) {
        // no center, show serial
        dialog.SetVersion(sVersion, sUTime, svVer, svUpTime, serial);
    }
    else {
        dialog.SetVersion(sVersion, sUTime, svVer, svUpTime);
    }
    dialog.setGeometry((pos.width()-dialog.m_nWidth)/2, (pos.height()-dialog.m_nHeight)/2, dialog.m_nWidth, dialog.m_nHeight);
    dialog.exec();
}

void MainWindow::ScanAllDisk(bool bServerCommand)
{
//    UserType type = GetUserType();
//    qDebug("MainWindow::ScanAllDisk UserType %d", type);
//    bool permit;
//    if(IsSecretMachine()){
//        permit = g_Users[type].HavePower(OperationPower::OP_POWER_ALLDISKSCAN);
//        if(!permit) {
//            qDebug("Error, MainWindow::ScanAllDisk no permission to do this");
//            QMessageBox msg(this);
//            msg.setIcon(QMessageBox::Information);
//            msg.setText(u8"当前用户没有权限执行此操作");
//            msg.exec();
//            return;
//        }
//    }
    int nret = 0;
    QMutexLocker locker(&m_Mutex);
    if (!m_bIsScaning) {
        if(!bServerCommand){
            nret = FullScan(true);
            if(nret != 0){
                m_bIsScaning = false;
//                QMessageBox msg(this);
//                msg.setIcon(QMessageBox::Information);
//                msg.setText(u8"出现错误，请稍后再次尝试。");
//                msg.exec();
                QMessageBox::information(this, "Error", u8"扫描失败，有可能是后台正在执行远程查杀或定时查杀，请稍候再试。", QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
        }
        m_bIsScaning = true;
    }
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_SCANING)
        return;
    mBodyWidget->ResetVirusLog();
    UpdateScanningIntelligentMode();
    mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->SetMode(ScanPauseBtn::DISPLAY_PAUSE_MODE);
    mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_SCANING);
    mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_SCANING);
    //qDebug() << "MainWindow::ScanAllDisk clicked\n";
//    std::list<pair<std::string,int> > scan;
//    GetScanPath(scan);

    mCurrentScanType = LogEventEntry::ALL_DISK;
    m_nScanItemsCount = 0;
    m_nScanedThreaten = 0;
    m_nScanedThreatenProcessed = 0;
    m_qdtStartTime = QDateTime::currentDateTime();
    m_pTimer->start(200);

    UpdateScanReadyState(RAVQT_CLICKED_SCAN_WAITING);
}

void MainWindow::QuickScanPath(bool bServerCommand)
{
    QMutexLocker locker(&m_Mutex);
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_QUICK_SCANING) {
        return;
    }

    if (!m_bIsScaning) {
        if(!bServerCommand) {
            int nret = QuickScan(true);
            if(nret != 0){
                m_bIsScaning = false;
                QMessageBox::information(this, "Error", u8"扫描失败，有可能是后台正在执行远程查杀或定时查杀，请稍候再试。", QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
        }
        m_bIsScaning = true;
    }
    //qDebug("%s-%d after StartScan", __FILE__, __LINE__);
    mBodyWidget->ResetVirusLog();

    UpdateScanningIntelligentMode();

    m_nScaningPercent = 0;
    mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->SetMode(ScanPauseBtn::DISPLAY_PAUSE_MODE);
    mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_QUICK_SCANING);
    mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_QUICK_SCANING);
    qDebug() << "MainWindow::QuickScanPath clicked\n";
//    std::list<pair<std::string,int> > scan;
//    GetScanPath(scan);

    mCurrentScanType = LogEventEntry::QUICK_DISK;
    m_nScanItemsCount = 0;
    m_nScanedThreaten = 0;
    m_qdtStartTime = QDateTime::currentDateTime();
    m_pTimer->start(1000);

    UpdateScanReadyState(RAVQT_CLICKED_SCAN_WAITING);
}

void MainWindow::CustomScanPath(bool bServerCommand)
{
//    UserType type = GetUserType();
//    qDebug("MainWindow::CustomScanPath UserType %d", type);
//    bool permit;
//    if(IsSecretMachine()){
//        permit = g_Users[type].HavePower(OperationPower::OP_POWER_CUSTOMSCAN);
//        if(!permit) {
//            qDebug("Error, MainWindow::CustomScanPath no permission to do this");
//            QMessageBox msg(this);
//            msg.setIcon(QMessageBox::Information);
//            msg.setText(u8"当前用户没有权限执行此操作");
//            msg.exec();
//            return;
//        }
//    }
//    qDebug("MainWindow::CustomScanPath begin before lock");
    QMutexLocker locker(&m_Mutex);
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_CUSTOM_SCANING)
        return;

    if (!m_bIsScaning && !bServerCommand) {
        DirSelectDialog *pDlg = new DirSelectDialog(this);
        pDlg->setModal(true);
        pDlg->exec();
        if(pDlg->result() == QDialog::Accepted){
            m_nScaningPercent = 0;
            std::list<pair<std::string,int> > scan;
//            qDebug("MainWindow::CustomScanPath() size=%d", pDlg->m_lDirs.size());
            for(int i=0; i<pDlg->m_lDirs.size(); i++){
                QString stmp = pDlg->m_lDirs[i];
                scan.push_back(make_pair(stmp.toStdString(), -1));
//                qDebug("scan path: %s", stmp.toStdString().c_str());
            }
            delete pDlg;
            pDlg = NULL;
            int nret = StartScan(scan, false);
            if(nret != 0){
                m_bIsScaning = false;
//                QMessageBox msg(this);
//                msg.setIcon(QMessageBox::Information);
//                msg.setText(u8"出现错误，请稍后再次尝试。");
//                msg.exec();
                QMessageBox::information(this, "Error", u8"扫描失败，有可能是后台正在执行远程查杀或定时查杀，请稍候再试。", QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
            mBodyWidget->ResetVirusLog();
            UpdateScanningIntelligentMode();
            mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->SetMode(ScanPauseBtn::DISPLAY_PAUSE_MODE);
            mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_CUSTOM_SCANING);
            mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_CUSTOM_SCANING);
            mCurrentScanType = LogEventEntry::USER_DISK;
            m_nScanItemsCount = 0;
            m_nScanedThreaten = 0;
            m_nScanedThreatenProcessed = 0;
            m_qdtStartTime = QDateTime::currentDateTime();
            m_pTimer->start(1000);
            m_bIsScaning = true;
//            qDebug("scan call end");
        }
        delete pDlg;
        pDlg = NULL;
    }else{
        mBodyWidget->ResetVirusLog();
        mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->SetMode(ScanPauseBtn::DISPLAY_PAUSE_MODE);
        mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_CUSTOM_SCANING);
        mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_CUSTOM_SCANING);
        mCurrentScanType = LogEventEntry::USER_DISK;
        m_nScanItemsCount = 0;
        m_nScanedThreaten = 0;
        m_nScanedThreatenProcessed = 0;
        m_qdtStartTime = QDateTime::currentDateTime();
        m_pTimer->start(1000);
        UpdateScanReadyState(RAVQT_CLICKED_SCAN_WAITING);
    }
}

void MainWindow::onSignaledPauseContinue()
{
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_PAUSED) {
        int ret = QMessageBox::information(this, u8"提示", u8"当前正处于扫描暂停状态，确定要继续扫描吗？", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok);
        if (ret == QMessageBox::Ok) {
            onClickPauseContinueWindowScan(true);

        }
    }else{
        int ret = QMessageBox::information(this, u8"提示", u8"当前正处于病毒扫描中，确定要暂停扫描吗？", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok);
        if (ret == QMessageBox::Ok) {
            onClickPauseContinueWindowScan(true);
        }
    }
}

void MainWindow::onClickPauseContinueWindowScan(bool signaled)
{
    qDebug() << "MainWindow::onClickPauseContinueWindowScan clicked\n";
    if (IsSecretMachine()) {
        if(signaled == false){
            emit sigPauseContinueMessage();
            return;
        }
    }
    QMutexLocker locker(&m_Mutex);
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_PAUSED) {
        m_pTimer->start();
        m_bIsScaning = true;

        mBodyWidget->SetVScanState(m_nBeforePausePrevScanState);
        mHeaderWidget->SetVScanState(m_nBeforePausePrevScanState);
        mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->SetMode(ScanPauseBtn::DISPLAY_PAUSE_MODE);

        Continue();
    }else{
        m_nBeforePausePrevScanState = mHeaderWidget->VScanState();

        mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_PAUSED);
        mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_PAUSED);
        mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->SetMode(ScanPauseBtn::DISPLAY_CONTINUE_MODE);

        m_pTimer->stop();
        m_bIsScaning = false;

        PauseScan();
    }
}

void MainWindow::onSignaledStopScan()
{
    int ret = QMessageBox::information(this, u8"提示", u8"当前正处于病毒扫描中，确定要停止扫描吗？", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok);
    if (ret == QMessageBox::Ok) {
        StopWindowsScan(true);
    }
}

void MainWindow::StopWindowsScan(bool signaled, bool bServerCommand)
{
    qDebug("StopWindowsScan signaled=%d", signaled?1:0);
    if (IsSecretMachine()) {
        if(signaled == false && bServerCommand == false){
            emit sigStopScanMessage();
            return;
        }
    }
    //qDebug("MainWindow::StopWindowScan clicked bServerCommand=%s", bServerCommand?"true":"false");
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_READY_TO_STOP && !bServerCommand) {
        m_nScanReadyState = RAVQT_CLICKED_STOP_WAITING;
        return;
    }
    mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->setDisabled(true);
    QMutexLocker locker(&m_Mutex);
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_STOPPED){
        mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->setDisabled(false);
        return;
    }
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_READY_TO_STOP && !bServerCommand){
        mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->setDisabled(false);
        return;
    }
    if(!bServerCommand) {
        m_nScanReadyState = RAV_QT_VSCAN_STATE_READY_TO_STOP;
        mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_READY_TO_STOP);
        StopScan();
        mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->setDisabled(false);
        return;
    }
    m_bIsScaning = false;
    m_pTimer->stop();
    QDateTime dt = QDateTime::currentDateTime();
    int nseconds = dt.secsTo(m_qdtStartTime);
    nseconds = nseconds==0?1:nseconds*(-1);
    mBodyWidget->UpdateStoppedInfo(m_nScanedThreaten, m_nScanItemsCount, nseconds);

    mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_STOPPED);
    mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_STOPPED);
    m_nScaningPercent = 0;
    mHeaderWidget->mVirusScanWidget->mScaningWidget->m_pPauseBtn->setDisabled(false);
}

void MainWindow::QuitStopState()
{
    if(mHeaderWidget->VScanState() != RAV_QT_VSCAN_STATE_STOPPED) {
        return;
    }
    mHeaderWidget->SetVScanState(RAV_QT_VSCAN_STATE_NORMAL);
    mBodyWidget->SetVScanState(RAV_QT_VSCAN_STATE_NORMAL);
    mBodyWidget->m_pScanMidBody->ResetVirusLog();
}

void MainWindow::UpdateScanReadyState(int nState)
{
    m_nScanReadyState = nState;
    if(nState != RAVQT_CLICKED_SCAN_WAITING){
        return;
    }
    //qDebug("UpdateScanReadyState RAVQT_CLICKED_SCAN_WAITING");
    m_sCurrentFile = QString::fromStdString("杀毒引擎准备中...");

    QDateTime dt = QDateTime::currentDateTime();
    int nseconds = dt.secsTo(m_qdtStartTime);
    nseconds = nseconds==0?1:nseconds*(-1);
    //int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes
    mHeaderWidget->UpdateScaningInfo(0, 0, 0, 0, nseconds);
}
//below defined at logevententry.h  high 16 is LogEventEntry::ScanType, low 16 is LogEventEntry::DiskType
//    typedef enum {
//        MANUAL_SCAN = 0,
//        REMOTE_SCAN,
//        TIMER_SCAN,
//        SPECIAL_FOLDER_SCAN,
//        ACTIVE_DEFENCE
//    } ScanType;
//    typedef enum {
//        ALL_DISK = 0,
//        QUICK_DISK,
//        USER_DISK,
//        SPECIAL_FILE,
//        MONITOR_FILE,
//    }DiskType;
//void MainWindow::RefreshScanLogs(int scantype, CircleLink<std::string> *data)
//{
//    if(!m_bInited) {
//        return;
//    }
//    if(data) {
//        CircleLinkNode<string> *node = data->Datas();
//        if(node->node.length() != 0){
//            m_UiQueue.AddLogData(scantype, node->node);
//        }
//    }
//    emit m_UiQueue.sigLogData();
//    return;
//}

//enum VirusStatus{
//    STATUS_DEFAULT,
//    STATUS_IGNORE,
//    STATUS_CLEAN,
//    STATUS_REMOVE,
//    STATUS_ABORT,
//}mStatus;
int MainWindow::FindVirusType(const string& virus, string& stype)
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

void MainWindow::UpdateScanningIntelligentMode()
{
    ScanIntelligentParams params;
    bool blocked;
    AntiVirusSetting::AntiCommon::ScanType type = GetScanIntelligentMode(params, blocked);
    switch(type){
    case AntiVirusSetting::AntiCommon::AUTO_MODE:
        if (mBodyWidget->m_pScanTopBody->m_pModeSelector) {
            mBodyWidget->m_pScanTopBody->m_pModeSelector->SetText(u8"自动");
        }
        break;
    case AntiVirusSetting::AntiCommon::OFFICE_MODE:
        if (mBodyWidget->m_pScanTopBody->m_pModeSelector) {
            mBodyWidget->m_pScanTopBody->m_pModeSelector->SetText(u8"办公");
        }
        break;
    case AntiVirusSetting::AntiCommon::HIGH_SPEED_MODE:
        if (mBodyWidget->m_pScanTopBody->m_pModeSelector) {
            mBodyWidget->m_pScanTopBody->m_pModeSelector->SetText(u8"高速");
        }
        break;
    default:
        break;
    }
    mBodyWidget->UpdateScaningThreadCount(params.thread_count);
}

void MainWindow::RefreshVirusLogs(const string& file, const string& virus, int/** VirusReportData::VirusStatus*/ status  )
{
    if(!m_bInited) {
        return;
    }
    m_nScanReadyState = RAVQT_RECEIVED_RESULT;
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_PAUSED) {
        return;
    }
    if(virus.length() > 0){
        m_UiQueue.AddVirusLogData(file, virus, status);
        emit m_UiQueue.sigVirusLogData();
    }
    return;
}

void MainWindow::RefreshScanStatus(int scantype, SCAN_STATUS_TYPE status, int progress, void *data)
{
    qDebug("type  %d and status is %d\n", scantype, status);
    if(!m_bInited) {
        return;
    }
    m_nScanReadyState = RAVQT_RECEIVED_RESULT;
    if(mHeaderWidget->VScanState() == RAV_QT_VSCAN_STATE_PAUSED && status != SCAN_FINISHED) {
        return;
    }
    qDebug("refrense the scan event");
    if(status == THREAD_STATUS) {
        if (data == NULL) {
            return;
        }

        m_nScanItemsCount = 0;
        m_nScanedThreatenProcessed = 0;
        vector<struct ReportCache> *rcache = (vector<struct ReportCache> *)data;
        int size = rcache->size();
        for(int i = 0; i< 4; i++) {
            if (i < size) {
//                qDebug("MainWindow::RefreshScanStatus thread:%d count:%d file:%s", (*rcache)[i].mTag, (*rcache)[i].mCount, (*rcache)[i].mFile.c_str());
                m_UiQueue.AddScanStatusThreadData(status, (*rcache)[i].mFile, (*rcache)[i].mCount, (*rcache)[i].mTag);

                m_nScanItemsCount += (*rcache)[i].mCount;
                m_nScanedThreatenProcessed += (*rcache)[i].mVirus;
                m_UiQueue.AddLogData(scantype, (*rcache)[i].mFile);
            }
        }

        int stype = TOTAL_PROGRESS;
        m_UiQueue.AddScanStatusData(scantype, stype, progress);
        qDebug("MainWindow::RefreshScanStatus progress=%d", progress);
    }else if(status == SCAN_FINISHED){
        qDebug("RefreshScanStatus SCAN_FINISHED progress=%d", progress);
        m_nScanItemsCount = progress;
        m_UiQueue.AddScanStatusData(scantype, status, progress);
    } else if (status == SCAN_PREPARE) {

    } else if (status == SCAN_ERROR_EXIT) {

    }

    emit m_UiQueue.sigScanStatusData();
    emit m_UiQueue.sigLogData();
    return;
}

int  MainWindow::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus)
{
//    if (event == Application::QUERY_FIND_VIRUS) {
//        if (AUTO_DEAL == m_pAvConf->mVirusScan.mVirusDeal.mValue) {
//            return VirusReportData::STATUS_CLEAN;
//        }
//        else if (IGNORE_DEAL == m_pAvConf->mVirusScan.mVirusDeal.mValue) {
//            return VirusReportData::STATUS_IGNORE;
//        }
        
    QRect pos = rect();
        QString svirus = QString("病毒名称: %1").arg(virus.c_str());
        QString sfile = QString("文件路径: %2").arg(file.c_str());
        QString stitle = "瑞星ESM_文件扫描";

//        if(m_pMsgBox->SameAction() == false)

            m_pMsgBox->SetMessage(QString::fromUtf8(stitle.toUtf8()), QString::fromUtf8(svirus.toUtf8()), QString::fromUtf8(sfile.toUtf8()));
            m_pMsgBox->setGeometry((pos.width()-m_pMsgBox->width())/2, (pos.height()-m_pMsgBox->height())/2, m_pMsgBox->width(), m_pMsgBox->height());
            m_pMsgBox->show();
            int err = m_pMsgBox->exec();
            m_pMsgBox->hide();


        int sel = m_pMsgBox->SameAction()?1:0;
        if (err == QDialog::Accepted) {//1:kill
            return sel<<16|NOTIFY_CONFIRM_SELECT;
        }
        else {  //0:ignore
            return sel<<16|NOTIFY_IGNORE_SELECT;
        }
//    }
//    return NOTIFY_IGNORE_SELECT;
}

int MainWindow::ScanFinished(int)
{
    m_bIsScaning = false;
    return 0;
}
//switch (type) {
//case CommandBundle::UPGRADE_DOWNLOAD_RP: {
//    printf("update %s\n", data.c_str());

//    RefreshUpdateStatus(UPDATE_DOWNLOAD_RP, data);
//    break;
//}
//case CommandBundle::UPGRADE_COMBINE_RP: {
//    RefreshUpdateStatus(UPDATE_COMBINE_RP, data);
//    break;
//}
//case CommandBundle::UPGRADE_APP_FILE: {
//    RefreshUpdateStatus(UPDATE_APP_DOWNLOAD, data);
//    break;
//}
//default:
//    RefreshUpdateStatus((UPDATE_EVENT)type, data);
int MainWindow::RefreshUpdateStatus(UPDATE_EVENT type, const string& data)
{
    if (type == UPDATE_VLIB_SUCCESS) {
//        mUpdateInfoDlg->onUpdateOK();
//        mUpdateInfoDlg->show();
        mBodyWidget->m_pBotBody->SetUpdateTime(QString::fromStdString(Utils::Date::FormatTimeStamp(mAppConfig->GetUpdateTime())));
        mBodyWidget->m_pScanBotBody->SetUpdateTime(QString::fromStdString(Utils::Date::FormatTimeStamp(mAppConfig->GetUpdateTime())));
        string sversion = Application::GetVersion();
        mBodyWidget->m_pBotBody->SetVersion(QString::fromStdString(sversion));
        mBodyWidget->m_pScanBotBody->SetVersion(QString::fromStdString(sversion));
    }else if (type == UPDATE_VLIB__FAILED) {
//        mUpdateInfoDlg->onUpdateFail();
//        mUpdateInfoDlg->show();
    }
    else if (type == RESPOND_REGISTER_STATE) {
        string major, minor;
        GetTitle(major, minor);
        if (!data.empty()) {
            minor = minor + "(" + data + ")";
        }
        mHeaderWidget->mTitleView->UpdateTitle(major.c_str(), minor.c_str());

        if (mAppConfig->GetProductType() == 0) {
            mDynMenu->setUpdateTitle(mActived?"检测更新":"激活");
        }
        else if (mAppConfig->GetCenterURL().empty()) {
            mDynMenu->setUpdateTitle("", false);
        }
    }
    //emit sigUpdateMessage(type, data);
    return 0;
}

int  MainWindow::ScanType()
{
    return LogEventEntry::SetScanType(LogEventEntry::MANUAL_SCAN, (LogEventEntry::DiskType)mCurrentScanType);
}

int  MainWindow::GetScanPath(std::list<pair<std::string,int> >& path)
{
    path.push_back(make_pair("/usr", 1));
    path.push_back(make_pair("/opt", 1));

    return 0;
}

//int  MainWindow::GetFullScanPath(std::list<pair<std::string,int> >& path)
//{
//    path.push_back(make_pair("/usr", 2));
//    path.push_back(make_pair("/opt", 2));
//    path.push_back(make_pair("/tmp", 2));
//    path.push_back(make_pair("/home", 4));
//    path.push_back(make_pair("/var", 2));
//    path.push_back(make_pair("/etc", 1));
//    path.push_back(make_pair("/mnt", 1));
//    return 0;
//}

void MainWindow::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        QPoint pt = QCursor::pos();
        //qDebug("MainWindow::mousePressEvent pt x,y:%d %d", pt.x(), pt.y());
        if(mHeaderWidget->rect().contains(mapFromGlobal(pt))){
            m_bDragging = true;
            m_poMouseStartPos = pEvent->globalPos();
            m_poFrameStartPos = frameGeometry().topLeft();
        }

    }
    QWidget::mousePressEvent(pEvent);
}

void MainWindow::mouseMoveEvent(QMouseEvent *pEvent)
{
    if(pEvent->buttons() == Qt::LeftButton)
    {
        if(m_bDragging){
            QPoint m_mousenow = pEvent->globalPos();
            move(m_poFrameStartPos + m_mousenow - m_poMouseStartPos);
        }
    }
    QWidget::mouseMoveEvent(pEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton){
        m_bDragging = false;
    }
    QWidget::mouseReleaseEvent(pEvent);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    QBitmap bitMap(this->width(), this->height());
    QPainter bitMapPainter(&bitMap);
    bitMapPainter.setPen(Qt::color0);
    bitMapPainter.drawRect(0, 0, width( ), height( ));
}

int MainWindow::Construct() {
    return Application::Construct(FOLDER_RJJH_GJCZ, true, RJJH_MAJOR_SOCKET);
}

void MainWindow::onUIExit() {
    StopScan();
    sleep(1);
    close();
}
