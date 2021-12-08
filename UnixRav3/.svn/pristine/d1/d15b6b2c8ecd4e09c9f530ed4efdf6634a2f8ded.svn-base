#include <QPainter>
#include <QPalette>
#include <QFont>
#include <QScrollBar>
#include <QMessageBox>
#include <QApplication>
#include <string>
#include <vector>

#include "common/pubdef.h"

#include "common/clickedlabel.h"
#include "common/combobox.h"
#include "common/updatesourcetable.h"
#include "common/centerdiscoverytable.h"
#include "weekdayselector.h"
#include "topwidget.h"
#include "leftwidget.h"
#include "righttopwidget.h"
#include "rightmidwidget.h"
#include "rightendwidget.h"
#include "midscrollwidget.h"
#include "origroup.h"
#include "whitelistgroup.h"
#include "backupgroup.h"
#include "kvgroup.h"
#include "timergroup.h"
#include "filegroup.h"
#include "udiskgroup.h"
#include "netblacklistgroup.h"
#include "blacklistiptable.h"
#include "blacklisturltable.h"
#include "dirtable.h"
#include "procnametable.h"
#include "operationpower.h"

#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "application.h"
#include "storagebridge.h"
#include "Entry/commonentry.h"
#include "Entry/urlruleentry.h"
#include "scanconfig.h"
#include "../CommonAPI/communicatebundle.h"
#include "Entry/whitelistentry.h"
#include "appconfig.h"
#include "../Upgrade/Common/updateconfig.h"



using namespace std;

const RavQtUser g_Users[] = {RavQtUser(0x003F), RavQtUser(0x03FF), RavQtUser(0x003B), RavQtUser(0x003F)};

MainWindow::MainWindow(QWidget *parent) : Application(Application::NORMAL_APP),
    QMainWindow(parent), m_pSystemTrayIcon(NULL)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setFixedSize(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
    if (Construct(FOLDER_RJJH_ETC) != 0) {
        qDebug("Error, MainWindow::MainWindow Construct(FOLDER_RJJH_ETC) fail");
        exit(-1);
    }
    //m_nFunction = 0x3FF;
    m_nFunction = mAppConfig->GetFunctions();
    qDebug("MainWindow::MainWindow() m_nFunction=0x%02X", m_nFunction );
    InitWindow();
    m_bDragging = false;
    m_pTimer = new QTimer(this);

    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnSaveTimer()));
}

MainWindow::~MainWindow()
{
    SECURITY_FREE(m_pAvConfig);
    SECURITY_FREE(m_pAvConfig);
    SECURITY_FREE(m_pNetfilterConfig);
}


void MainWindow::InitWindow()
{
    m_pTopWidget = new TopWidget(this);
    m_pTopWidget->setFixedHeight(36); //for some machine
    m_pLeftWidget = new LeftWidget(this, m_nFunction, &m_ScreenConf, APP_SCREEN_HEIGHT-m_pTopWidget->height());
    m_pRightTopWidget = new RightTopWidget(this, m_nFunction, APP_SCREEN_HEIGHT-m_pTopWidget->height()-APP_SCREEN_RIGHT_BOTTOM_HEIGHT);
  //  m_pRightMidWidget = new RightMidWidget(this, APP_SCREEN_HEIGHT-m_pTopWidget->height()-APP_SCREEN_RIGHT_BOTTOM_HEIGHT);
    m_pRightEndWidget = new RightEndWidget(this, APP_SCREEN_HEIGHT-m_pTopWidget->height()-APP_SCREEN_RIGHT_BOTTOM_HEIGHT);
 //   m_pRightMidWidget->hide();
    m_pRightEndWidget->hide();

    m_pTopWidget->setGeometry(0,0,m_pTopWidget->width(),m_pTopWidget->height());
    m_pLeftWidget->setFixedHeight(APP_SCREEN_HEIGHT-m_pTopWidget->height());
    m_pLeftWidget->setGeometry(0,m_pTopWidget->height(),m_pLeftWidget->width(),APP_SCREEN_HEIGHT-m_pTopWidget->height());
    m_pRightTopWidget->setGeometry(APP_SCREEN_LEFT_WIDTH,m_pTopWidget->height(),m_pRightTopWidget->width(),APP_SCREEN_HEIGHT-m_pTopWidget->height());
  //  m_pRightMidWidget->setGeometry(APP_SCREEN_LEFT_WIDTH,m_pTopWidget->height(),m_pRightMidWidget->width(),APP_SCREEN_HEIGHT-m_pTopWidget->height());

    m_pRightEndWidget->setGeometry(APP_SCREEN_LEFT_WIDTH,m_pTopWidget->height(),m_pRightTopWidget->width(),APP_SCREEN_HEIGHT-m_pTopWidget->height());

    m_pclLoadDefault = new ClickedLabel(u8"使用默认设置", this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(33,175,224));
    m_pclLoadDefault->setPalette(pa);
    QFont font1;
#ifdef QT_NEW_HEADER_54
    font1.setWeight(QFont::Thin);
#else
    font1.setWeight(QFont::Light);
#endif
    font1.setPointSize(10);
    m_pclLoadDefault->setFont(font1);
    m_pclLoadDefault->setCursor(Qt::PointingHandCursor);
    m_ppbApply = new QPushButton(this);
    m_ppbApply->setStyleSheet("QPushButton{\
                  border:0px transparent;\
                  padding:0px;\
                  image:url(:/Images/apply_btn_normal);\
              }\
              QPushButton:hover{\
                  image:url(:/Images/apply_btn_hovered);\
              }\
              QPushButton:pressed{\
                  image:url(:/Images/apply_btn_pressed);\
              }\
            QPushButton:disabled {\
                image: url(:/resource/saving_bt);\
            }\
              QPushButton:flat {\
                  image: url(:/Images/apply_btn_disabled);\
              }\
              QPushButton:default{\
                  border-color:navy;\
              }");
    QPixmap img(":/Images/apply_btn_normal");
    m_ppbApply->setFixedSize(img.width(), img.height());

    m_pclLoadDefault->setGeometry(APP_SCREEN_LEFT_WIDTH+25, APP_SCREEN_HEIGHT-50, m_pclLoadDefault->width(), m_pclLoadDefault->height());
    m_ppbApply->setGeometry(APP_SCREEN_WIDTH-130, APP_SCREEN_HEIGHT-50, m_ppbApply->width(), m_ppbApply->height());


    connect(m_pTopWidget->m_pCloseBtn, SIGNAL(clicked()), this, SLOT(close()));
    //connect(m_pTopWidget->m_pMinBtn, &PushButton::clicked,this,&MainWindow::setMinimum);

    connect(m_pLeftWidget->m_pitlKill, SIGNAL(clicked()), this, SLOT(OnClickedLeftKill()));
    //connect(m_pLeftWidget->m_pitlNetDefend, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftNetDefend);
    if (m_pLeftWidget->HideAdvanceFunction()) {
        connect(m_pLeftWidget->m_pitlSetup, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftSetup);
    }
    connect(m_pLeftWidget->m_pitlOri, SIGNAL(clicked()), this, SLOT(OnClickedLeftOri()));
    connect(m_pLeftWidget->m_pitlWhiteList, SIGNAL(clicked()), this, SLOT(OnClickedLeftWhiteList()));
    connect(m_pLeftWidget->m_pitlBackup, SIGNAL(clicked()), this, SLOT(OnClickedLeftBackup()));
    connect(m_pLeftWidget->m_pitlKV, SIGNAL(clicked()), this, SLOT(OnClickedLeftKV()));
    connect(m_pLeftWidget->m_pitlTimer, SIGNAL(clicked()), this, SLOT(OnClickedLeftTimer()));
    connect(m_pLeftWidget->m_pitlFile, SIGNAL(clicked()), this, SLOT(OnClickedLeftFile()));
    connect(m_pLeftWidget->m_pitlUDisk, SIGNAL(clicked()), this, SLOT(OnClickedLeftUDisk()));
//    connect(m_pLeftWidget->m_pitlSystem, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftSystem);
//    connect(m_pLeftWidget->m_pitlApp, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftApp);

   // connect(m_pLeftWidget->m_pitlNetBlacklist, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftNetBlacklist);

//    connect(m_pLeftWidget->m_pitlAdmin, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftAdmin);
//    connect(m_pLeftWidget->m_pitlTray, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftTray);
//    connect(m_pLeftWidget->m_pitlLicense, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftLicense);
    if (m_pLeftWidget->HideAdvanceFunction()) {
        connect(m_pLeftWidget->m_pitlUpgrade, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftUpgrade);
        //connect(m_pRightEndWidget->m_pEndWidget->m_pAdmin->m_pclSetOrChange, &ClickedLabel::clicked, this, &MainWindow::OnChangeAdminPasswd);
        connect(m_pLeftWidget->m_pitlCenterDiscovery, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftCenterDiscovery);
        connect(m_pRightEndWidget, &RightEndWidget::valueChanged, this, &MainWindow::AutoSetFocus);
    }
 //   connect(m_pRightMidWidget, &RightMidWidget::valueChanged, this, &MainWindow::AutoSetFocus);
    connect(m_pRightTopWidget, SIGNAL(valueChanged(int)), this, SLOT(AutoSetFocus(int)));
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    m_btnConn = connect(m_ppbApply, &QPushButton::clicked, this, &MainWindow::OnClickedSaveSetting);
#else
    connect(m_ppbApply, SIGNAL(clicked()), this, SLOT(OnClickedSaveSetting()));
#endif
    connect(m_pclLoadDefault, SIGNAL(clicked()), this, SLOT(OnClickedLoadDefault()));

    m_pSystemTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/resource/logo");
    QString titleName = QString::fromUtf8("Config");
    m_pSystemTrayIcon->setIcon(icon);
    m_pSystemTrayIcon->setToolTip(titleName);
    setWindowIcon(icon);

    connect(m_pSystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    QApplication::setApplicationName(titleName);
    setWindowTitle(titleName);
    m_pTopWidget->SetTitle(titleName);

    OnClickedLeftKill();
}
//    int  GetProcessBlackList(std::list<WhiteListEntry>& white);
//    int  AddProcessBlackList(const std::string& process);
//    int  DeleteProcessBlackList(const WhiteListEntry& entry);
//    int  GetFileBlackList(std::list<WhiteListEntry>& white);
//    int  AddFileBlackList(const std::string& filepath);
//    int  DeleteFileBlackList(const WhiteListEntry& entry);
void MainWindow::UpdateFileMonitorListFromConfig(std::list<WhiteListEntry> &white)
{
    GetProcessBlackList(white);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pDirTable->ClearAllItem();
    if(white.size() > 0){
        std::list<WhiteListEntry>::iterator wit = white.begin();
        while(wit != white.end()){
            WhiteListEntry wle = *wit;
            //if(wle.mListType == WhiteListEntry::BLACK && wle.mBlockType == WhiteListEntry::PROCESS_BLOCK){
                 m_pRightTopWidget->m_pTopWidget->m_pFile->m_pProcNameTable->AddItem(wle.mPath.c_str());
            //}
            wit++;
        }
    }
    white.clear();
    GetFolderMonitorList(white);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pDirTable->ClearAllItem();
    if(white.size() > 0){
        std::list<WhiteListEntry>::iterator wit = white.begin();
        while(wit != white.end()){
            WhiteListEntry wle = *wit;
            //if(wle.mListType == WhiteListEntry::BLACK && wle.mBlockType == WhiteListEntry::FOLDER_MONITOR){
                 m_pRightTopWidget->m_pTopWidget->m_pFile->m_pDirTable->AddItem(0, wle.mPath.c_str());
            //}
            wit++;
        }
    }
}

string MainWindow::GetWhiteListStr(std::list<WhiteListEntry>  &list)
{
    if(list.size() <= 0)
        return "";
    string sret = "";
    bool bfirst = true;
    std::list<WhiteListEntry>::iterator it = list.begin();
    while(it != list.end()){
        WhiteListEntry wle = *it;
        if(!bfirst)
            sret += ";";
        sret += wle.mPath;
        bfirst = false;
        it++;
    }
    return sret;
}

int MainWindow::SetStrToWhiteList(const char* sText,std::list<WhiteListEntry>  &list)
{
    string s = sText;
    size_t last = 0;
    size_t index=s.find_first_of(";",last);
    while (index!=std::string::npos)
    {
        WhiteListEntry wle;
        wle.mBlockType = WhiteListEntry::FILE_SCAN;
        wle.mListType = WhiteListEntry::BLACK;
        wle.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
        wle.mPath = s.substr(last,index-last);
        list.push_back(wle);
        last=index+1;
        index=s.find_first_of(";",last);
    }
    if (index-last>0)
    {
        WhiteListEntry wle;
        wle.mBlockType = WhiteListEntry::FILE_SCAN;
        wle.mListType = WhiteListEntry::BLACK;
        wle.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
        wle.mPath = s.substr(last,index-last);
        list.push_back(wle);
    }
    return list.size();
}

bool MainWindow::HideAdvanceFunction() {
    return IsSecretMachine() || IsCenterLess();
}

void MainWindow::UpdateFromConfigOri()
{
    //ori
    m_pRightTopWidget->m_pTopWidget->m_pOri->m_pRunEnv->SetValue(m_pAvConfig->mCommon.mRunEnvJudge.mValue);
    if (m_pAvConfig->mCommon.mRunEnvJudge.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pOri->m_pRunEnv->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pOri->m_pVirusTrace->SetValue(m_pAvConfig->mCommon.mVirusTrace.mValue);
    if (m_pAvConfig->mCommon.mVirusTrace.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pOri->m_pVirusTrace->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pOri->m_pVirusLog->SetValue(m_pAvConfig->mCommon.mVirusLoged.mValue);
    if (m_pAvConfig->mCommon.mVirusLoged.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pOri->m_pVirusLog->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pOri->m_pBuffer->SetValue(m_pAvConfig->mCommon.mRescanAccelerate.mValue);
    if (m_pAvConfig->mCommon.mRescanAccelerate.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pOri->m_pBuffer->setDisabled(true);
    }

    if (IsSecretMachine()) {
        if (m_pRightTopWidget->m_pTopWidget->m_pOri->m_CenterAddr) {
            m_pRightTopWidget->m_pTopWidget->m_pOri->m_CenterAddr->setText(QString::fromStdString(mAppConfig->GetCenterURL()));
        }
    }

    // 设置中去掉了公有云或者私有云的设置项， 如果需要把云扫描加上， 则把此处放开
//    if (!HideAdvanceFunction()) {
//        m_pRightTopWidget->m_pTopWidget->m_pOri->m_pCloud->SetValue(m_pAvConfig->mCommon.mUsePublicCloud.mValue);
//        if (m_pAvConfig->mCommon.mUsePublicCloud.mLocked) {
//            m_pRightTopWidget->m_pTopWidget->m_pOri->m_pCloud->setDisabled(true);
//        }
//        list<AntiVirusSetting::AntiCommon::CloudModel> *plist = &m_pAvConfig->mCommon.mCloudSetting;
//        list<AntiVirusSetting::AntiCommon::CloudModel>::iterator it = plist->begin();
//        while(it != plist->end()){
//            AntiVirusSetting::AntiCommon::CloudModel model = *it;
//            m_pRightTopWidget->m_pTopWidget->m_pOri->m_pPrivateTable->AddItem(model.mState, QString::fromStdString(model.mServerName), QString::fromStdString(model.mServerAddr), (int)model.mPort);
//            it++;
//        }
//        if (m_pAvConfig->mCommon.mUsePrivateCloud.mLocked) {
//            m_pRightTopWidget->m_pTopWidget->m_pOri->m_pPrivateTable->setDisabled(true);
//        }
//    }
}

void MainWindow::UpdateFromConfigWhiteList()
{
    //whitelist
    std::list<WhiteListEntry> white;
    GetWhiteList(white);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_pdfDirFile->ClearAllItem();
    std::list<WhiteListEntry>::iterator wit = white.begin();
    while(wit != white.end()){
        WhiteListEntry wle = *wit;
        if(wle.mBlockType == WhiteListEntry::FILE_SCAN){
            int ntype = RTransPathType(wle.mPathType);
            if(ntype >= 0)
                m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_pdfDirFile->AddItem(ntype, wle.mPath.c_str());
        }else if(wle.mPathType == WhiteListEntry::FILE_EXTEND_TYPE){
             m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_penExtName->AddItem(wle.mPath.c_str());
        }
        wit++;
    }
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_penExtName->ClearAllItem();
    list<string>::iterator ite = m_pAvConfig->mWhiteList.mExtTypes.begin();
    while(ite != m_pAvConfig->mWhiteList.mExtTypes.end()){
        m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_penExtName->AddItem((*ite).c_str());
        ite++;
    }
}

void MainWindow::UpdateFromConfigBackup()
{
    //backup
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_pclBackup->SetValue(m_pAvConfig->mVirusBackup.mVirusBackup.mValue);
    if (m_pAvConfig->mVirusBackup.mVirusBackup.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_pclBackup->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbAskMe->setChecked(m_pAvConfig->mVirusBackup.mLargeFile.mValue==LARGE_FILE_QUERY?true:false);
    if (m_pAvConfig->mVirusBackup.mLargeFile.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbAskMe->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbDeleteFile ->setChecked(m_pAvConfig->mVirusBackup.mLargeFile.mValue==LARGE_FILE_DELETE?true:false);
    if (m_pAvConfig->mVirusBackup.mLargeFile.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbDeleteFile->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbDontCare->setChecked(m_pAvConfig->mVirusBackup.mLargeFile.mValue==LARGE_FILE_IGNORE?true:false);
    if (m_pAvConfig->mVirusBackup.mLargeFile.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbDontCare->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbAutoReplace->setChecked(m_pAvConfig->mVirusBackup.mSpaceLess.mValue==OVERRIDE_OLD?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbAutoIncrease->setChecked(m_pAvConfig->mVirusBackup.mSpaceLess.mValue==INCREAM_SPACE?true:false);
    if (m_pAvConfig->mVirusBackup.mSpaceLess.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbAutoReplace->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbAutoIncrease->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbFailAskMe->setChecked(m_pAvConfig->mVirusBackup.mBackupFailed.mValue==BACKUP_FAILED_QUERY?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbFailDeleteFile->setChecked(m_pAvConfig->mVirusBackup.mBackupFailed.mValue==BACKUP_FAILED_DELETE?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbFailDontCare->setChecked(m_pAvConfig->mVirusBackup.mBackupFailed.mValue==BACKUP_FAILED_IGNORE?true:false);
    if (m_pAvConfig->mVirusBackup.mBackupFailed.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbFailAskMe->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbFailDeleteFile->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pBackup->m_prbFailDontCare->setDisabled(true);
    }
}

void MainWindow::UpdateFromConfigKV()
{
    //kvgroup
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAllFiles->setChecked((m_pAvConfig->mVirusScan.mFileType.mValue==ALL_FILE)?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbProgDoc->setChecked((m_pAvConfig->mVirusScan.mFileType.mValue==EXE_DOC)?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbUserDefine->setChecked((m_pAvConfig->mVirusScan.mFileType.mValue==SPECIAL_FILE_TYPE)?true:false);
    if(m_pAvConfig->mVirusScan.mFileType.mValue==SPECIAL_FILE_TYPE){
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleFileTypes->setDisabled(false);
    }else{
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleFileTypes->setDisabled(true);
    }
    if (m_pAvConfig->mVirusScan.mFileType.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAllFiles->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbProgDoc->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbUserDefine->setDisabled(true);
    }

    UserType type = GetUserType();
    qDebug("MainWindow::UpdateFromConfigKV UserType %d", type);
    bool permit = true;
//    if(IsSecretMachine()){
//        permit = g_Users[type].HavePower(OperationPower::OP_POWER_SETFILESCANTYPE);
//    }
    if (m_pAvConfig->mVirusScan.mFileType.mLocked || (!permit)) {
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAllFiles->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbProgDoc->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbUserDefine->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleFileTypes->setDisabled(true);
    }
    //std::list<WhiteListEntry> sftypes;
    //int nsftypes = GetScanFileTypeList(sftypes);
    //if(nsftypes > 0){
        string sret = GetScanFileCustomType();
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleFileTypes->setText(sret.c_str());
    //}
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclOnWildlist->SetValue(m_pAvConfig->mVirusScan.mScanCommonVirus.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclHeurist->SetValue(m_pAvConfig->mVirusScan.mScanIntelligent.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclProcCompressed->SetValue(m_pAvConfig->mVirusScan.mScanCompressFile.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetSizeEditValid();

    m_pRightTopWidget->m_pTopWidget->m_pKV->SetSizeEditValid();

    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclOnWildlist->setDisabled(m_pAvConfig->mVirusScan.mScanCommonVirus.mLocked);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclHeurist->setDisabled(m_pAvConfig->mVirusScan.mScanIntelligent.mLocked);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclProcCompressed->setDisabled(m_pAvConfig->mVirusScan.mScanCompressFile.mLocked);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleSize->setDisabled(m_pAvConfig->mVirusScan.mZipFileSize.mLocked);


    m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleSize->setText(QString("%1").arg(m_pAvConfig->mVirusScan.mZipFileSize.mValue, 0, 10));
    if (m_pAvConfig->mVirusScan.mZipFileSize.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pKV->SetSizeEditValid();
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleSize->setDisabled(true);
    }
    permit = true;
//    if(IsSecretMachine()){
//        permit = g_Users[type].HavePower(OperationPower::OP_POWER_SETCOMPRESSSCAN);
//    }
//    if(!permit) {
//        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclProcCompressed->setDisabled(true);
//        m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleSize->setDisabled(true);
//    }
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAuto->setChecked(m_pAvConfig->mVirusScan.mVirusDeal.mValue==AUTO_DEAL?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbManual->setChecked(m_pAvConfig->mVirusScan.mVirusDeal.mValue==MANUAL_DEAL?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbIgnore->setChecked(m_pAvConfig->mVirusScan.mVirusDeal.mValue==IGNORE_DEAL?true:false);
//    permit = true;
//    if(IsSecretMachine()){
//        permit = g_Users[type].HavePower(OperationPower::OP_POWER_SETVIRUSHANDLETYPE);
//    }
    if (m_pAvConfig->mVirusScan.mVirusDeal.mLocked || (!permit)) {
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAuto->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbManual->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbIgnore->setDisabled(true);
    }

	m_pRightTopWidget->m_pTopWidget->m_pKV->SetCloudEngine(m_pAvConfig->mCloudEngine.mEnable.mValue, m_pAvConfig->mCloudEngine.mUrl.mValue.c_str());
}

void MainWindow::UpdateFromConfigTimer()
{
    //timergroup --fullscan
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pclAllDiskScan->SetValue(m_pAvConfig->mTimeScan.mFullScan.mValue);
    if (m_pAvConfig->mTimeScan.mFullScan.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pclAllDiskScan->setDisabled(true);
    }
    //ScheduleEntry fullConf = m_pAvConfig->mTimeScan.mValue[ScheduleEntry::ALL_SCAN];
    ScheduleEntry fullConf;
    m_pAvConfig->mTimeScan.GetTimeScanEntry(ScheduleEntry::ALL_SCAN, fullConf);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTPowerOn->setChecked(fullConf.mType==ScheduleEntry::RESTART);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryDay->setChecked(fullConf.mType==ScheduleEntry::DAYS);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTDayTime->setText(QString("%1:%2").arg(fullConf.GetCrontabHour(),2,10,QChar('0')).arg(fullConf.GetCrontabMinute(),2,10,QChar('0')));
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryWeek->setChecked(fullConf.mType==ScheduleEntry::WEEKS);
    std::vector<int> fdays;
    fullConf.GetCrontabDayInWeek(fdays);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pTWeekSel->SetSelected(fdays);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTWeekTime->setText(QString("%1:%2").arg(fullConf.GetCrontabHour(),2,10,QChar('0')).arg(fullConf.GetCrontabMinute(),2,10,QChar('0')));
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryMonth->setChecked(fullConf.mType==ScheduleEntry::MONTH);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbTMonthDay->setCurrentIndex(fullConf.GetCrontabDayInMonth()>0?fullConf.GetCrontabDayInMonth()-1:0);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTMonthTime->setText(QString("%1:%2").arg(fullConf.GetCrontabHour(),2,10,QChar('0')).arg(fullConf.GetCrontabMinute(),2,10,QChar('0')));
    if (m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryDay->isChecked()) {
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pTWeekSel->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTWeekTime->setDisabled(true);
//        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryMonth->setDisabled(true);
//        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbTMonthDay->setDisabled(true);
//        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTMonthTime->setDisabled(true);
    }

//    UserType type = GetUserType();
//    qDebug("MainWindow::ScanAllDisk UserType %d", type);
//    bool permit = true;
//    if(IsSecretMachine()){
//        permit = g_Users[type].HavePower(OperationPower::OP_POWER_TIMERSCAN);
//    }
//    if(!permit) {
//        m_pRightTopWidget->m_pTopWidget->m_pTimer->setDisabled(true);
//        return;
//    }
    //    --quickscan
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pclFastScan->SetValue(m_pAvConfig->mTimeScan.mQuickScan.mValue);
    if (m_pAvConfig->mTimeScan.mQuickScan.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pclFastScan->setDisabled(true);
    }
    //ScheduleEntry quickConf = m_pAvConfig->mTimeScan.mValue[ScheduleEntry::QUICK_SCAN];
    ScheduleEntry quickConf;
    m_pAvConfig->mTimeScan.GetTimeScanEntry(ScheduleEntry::QUICK_SCAN, quickConf);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBPowerOn->setChecked(quickConf.mType==ScheduleEntry::RESTART);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryDay->setChecked(quickConf.mType==ScheduleEntry::DAYS);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBDayTime->setText(QString("%1:%2").arg(quickConf.GetCrontabHour(),2,10,QChar('0')).arg(quickConf.GetCrontabMinute(),2,10,QChar('0')));
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryWeek->setChecked(quickConf.mType==ScheduleEntry::WEEKS);

    std::vector<int> qdays;
    quickConf.GetCrontabDayInWeek(qdays);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pBWeekSel->SetSelected(qdays);
    //qDebug("Hour:%d  minute:%d", quickConf.GetCrontabHour(), quickConf.GetCrontabMinute());
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBWeekTime->setText(QString("%1:%2").arg(quickConf.GetCrontabHour(),2,10,QChar('0')).arg(quickConf.GetCrontabMinute(),2,10,QChar('0')));
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryMonth->setChecked(quickConf.mType==ScheduleEntry::MONTH);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbBMonthDay->setCurrentIndex(quickConf.GetCrontabDayInMonth()>0?quickConf.GetCrontabDayInMonth()-1:0);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBMonthTime->setText(QString("%1:%2").arg(quickConf.GetCrontabHour(),2,10,QChar('0')).arg(quickConf.GetCrontabMinute(),2,10,QChar('0')));
    m_pRightTopWidget->m_pTopWidget->m_pTimer->UpdateControlStatus();
    if (fullConf.mLocked  || m_pAvConfig->mTimeScan.mFullScan.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTPowerOn->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryDay->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryWeek->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryMonth->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTDayTime->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pTWeekSel->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTWeekTime->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbTMonthDay->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTMonthTime->setDisabled(true);
    }
    if (quickConf.mLocked || m_pAvConfig->mTimeScan.mQuickScan.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBPowerOn->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryDay->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBDayTime->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryWeek->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pBWeekSel->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBWeekTime->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryMonth->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbBMonthDay->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBMonthTime->setDisabled(true);
    }
}

void MainWindow::UpdateFromConfigFile()
{
    //filegroup

    /*
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartAtPowerOn->SetValue(m_pAvConfig->mFileMonitor.mStartPowerOn.mValue);
    if (m_pAvConfig->mFileMonitor.mStartPowerOn.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartAtPowerOn->setDisabled(true);
    }
    */
    /*
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartSmartBlacklist->SetValue(m_pAvConfig->mFileMonitor.mIntelligentBlacklist.mValue);
    if (m_pAvConfig->mFileMonitor.mIntelligentBlacklist.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartSmartBlacklist->setDisabled(true);
    }
        */
//    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartKernelScan->SetValue(m_pAvConfig->mFileMonitor.mKernelMonitor.mValue);
    //m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFastMode->setChecked(m_pAvConfig->mFileMonitor.mMonitorType.mValue==0);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbStandardMode->setChecked(m_pAvConfig->mFileMonitor.mMonitorType.mValue==1);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbProfessionalMode->setChecked(m_pAvConfig->mFileMonitor.mMonitorType.mValue==2);
    //m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbPlusMode->setChecked(m_pAvConfig->mFileMonitor.mMonitorType.mValue==3);
    if (m_pAvConfig->mFileMonitor.mMonitorType.mLocked) {
        //m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFastMode->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbStandardMode->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbProfessionalMode->setDisabled(true);
        //m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbPlusMode->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbAllFiles->setChecked(m_pAvConfig->mFileMonitor.mFileType.mValue==ALL_FILE?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbProgDoc->setChecked(m_pAvConfig->mFileMonitor.mFileType.mValue==EXE_DOC?true:false);
    if (m_pAvConfig->mFileMonitor.mFileType.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbAllFiles->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbProgDoc->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pTrustProgAnalysis->SetValue(m_pAvConfig->mFileMonitor.mMonitorAccelerate.mValue);
    if (m_pAvConfig->mFileMonitor.mMonitorAccelerate.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pTrustProgAnalysis->setDisabled(true);
    }
//    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclEmbedSacan->SetValue(m_pAvConfig->mFileMonitor.mInnerScan.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclOnWildlist->SetValue(m_pAvConfig->mFileMonitor.mScanCommonVirus.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclHeuristScan->SetValue(m_pAvConfig->mFileMonitor.mScanIntelligent.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclProcCompressed->SetValue(m_pAvConfig->mFileMonitor.mZipScan.mValue);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pleSize->setText(QString("%1").arg(m_pAvConfig->mFileMonitor.mZipFileSize.mValue,0,10));

    m_pRightTopWidget->m_pTopWidget->m_pFile->SetSizeEditValid();
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclOnWildlist->setDisabled(m_pAvConfig->mFileMonitor.mScanCommonVirus.mLocked);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclHeuristScan->setDisabled(m_pAvConfig->mFileMonitor.mScanIntelligent.mLocked);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclProcCompressed->setDisabled(m_pAvConfig->mFileMonitor.mZipScan.mLocked);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pleSize->setDisabled(m_pAvConfig->mFileMonitor.mZipFileSize.mLocked);
        m_pRightTopWidget->m_pTopWidget->m_pFile->lcomment1->setDisabled(m_pAvConfig->mFileMonitor.mZipFileSize.mLocked);
        m_pRightTopWidget->m_pTopWidget->m_pFile->lcomment2->setDisabled(m_pAvConfig->mFileMonitor.mZipFileSize.mLocked);


    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVAuto->setChecked(m_pAvConfig->mFileMonitor.mVirusDeal.mValue==AUTO_DEAL?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVManual->setChecked(m_pAvConfig->mFileMonitor.mVirusDeal.mValue==MANUAL_DEAL?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVIgnore->setChecked(m_pAvConfig->mFileMonitor.mVirusDeal.mValue==IGNORE_DEAL?true:false);
    if (m_pAvConfig->mFileMonitor.mVirusDeal.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVAuto->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVManual->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVIgnore->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFailVDelete->setChecked(m_pAvConfig->mFileMonitor.mCleanFailed.mValue==0?true:false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFailVIgnore->setChecked(m_pAvConfig->mFileMonitor.mCleanFailed.mValue==2?true:false);
    if (m_pAvConfig->mFileMonitor.mCleanFailed.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFailVDelete->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFailVIgnore->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclNotifyMe->SetValue(m_pAvConfig->mFileMonitor.mCleanPrompt.mValue);
    if (m_pAvConfig->mFileMonitor.mCleanPrompt.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclNotifyMe->setDisabled(true);
    }

    std::list<WhiteListEntry> white;
    UpdateFileMonitorListFromConfig(white);
}

void MainWindow::UpdateFromConfigUDisk()
{
    //udiskgroup
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_prbAskMe->setChecked(m_pAvConfig->mPluginMonitor.mPluginDeal.mValue==1);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_prbProcImmediately->setChecked(m_pAvConfig->mPluginMonitor.mPluginDeal.mValue==0);
    if (m_pAvConfig->mPluginMonitor.mPluginDeal.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_prbAskMe->setDisabled(true);
        m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_prbProcImmediately->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_pleSize->setText(QString("%1").arg(m_pAvConfig->mPluginMonitor.mScanDepth.mValue,0,10));
    if (m_pAvConfig->mPluginMonitor.mScanDepth.mLocked) {
        m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_pleSize->setDisabled(true);
    }
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetSizeEditValid();
}

void MainWindow::UpdateFromConfigUpgrade()
{
    ScheduleEntry entry;
    std::vector<int> fdays;
    switch ((int)m_pUpgradeSetting->mUpdateMode.mValue) {
    case UpgradeSetting::UPGRADE_MANUALLY:
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbManual->setChecked(true);
        break;
    case UpgradeSetting::UPGRADE_EVERY_DAY:
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryDay->setChecked(true);
        GetScheduleEntry(ScheduleEntry::UPDATE, entry);
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleDayTime->setText(QString("%1:%2").arg(entry.GetCrontabHour(),2,10,QChar('0')).arg(entry.GetCrontabMinute(),2,10,QChar('0')));
        break;
    case UpgradeSetting::UPGRADE_EVERY_WEEK:
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryWeek->setChecked(true);
        GetScheduleEntry(ScheduleEntry::UPDATE, entry);
        entry.GetCrontabDayInWeek(fdays);
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pwdlWeekDays->SetSelected(fdays);
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleWeekTime->setText(QString("%1:%2").arg(entry.GetCrontabHour(),2,10,QChar('0')).arg(entry.GetCrontabMinute(),2,10,QChar('0')));
        break;
//    case UpgradeSetting::UPGRADE_INTERVAL:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInterval->setChecked(true);
//        GetScheduleEntry(ScheduleEntry::UPDATE, entry);
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleIntervalHours->setText(QString::number(entry.GetCrontabHour()));
//        break;
    default:
        break;
    }

//    switch ((int)m_pUpgradeSetting->mDelayStartAction.mValue) {
//    case UpgradeSetting::DELAY_DISABLE:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbDisabledDelayBoot->setChecked(true);
//        break;
//    case UpgradeSetting::DELAY_AUTOMATIC:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbAutoDalayBoot->setChecked(true);
//        break;
//    case UpgradeSetting::DELAY_INTIME:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInRecentMinutesBoot->setChecked(true);
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleRecentMinutesBoot->setText(Utils::String::itoa(m_pUpgradeSetting->mDelayStartTime.mValue).c_str());
//        break;
//    default:
//        break;
//    }

    switch ((int)m_pUpgradeSetting->mUpdateContent.mValue) {
    case UpgradeSetting::UPDATE_ALL:
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbUpgradeAll->setChecked(true);
        break;
    case UpgradeSetting::UPDATE_VIRUS_DB:
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbUpgradeVirusDB->setChecked(true);
        break;
    default:
        break;
    }

//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclRealtimeCheckVDBVersion->SetValue(m_pUpgradeSetting->mCheckVirusDbRealTime.mValue);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclCleaningSysTemp->SetValue(m_pUpgradeSetting->mClearUpdateCache.mValue);

//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclForceUpgrade->SetValue(m_pUpgradeSetting->mForceUpdate.mValue);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleAfterDaysUpgrade->setText(QString::number(m_pUpgradeSetting->mForceUpdateDelay.mValue));

//    switch ((int)m_pUpgradeSetting->mProxySetting.mValue){
//    case UpgradeSetting::CONNECT_DIRECT:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->setCurrentIndex(0);
//        break;
//    case UpgradeSetting::CONNECT_BROWSER:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->setCurrentIndex(1);
//        break;
//    case UpgradeSetting::CONNECT_PROXY:
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->setCurrentIndex(2);
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleAddress->setText(m_pUpgradeSetting->mProxyAddr.mValue.c_str());
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_plePort->setText(m_pUpgradeSetting->mProxyPort.mValue.c_str());
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleUser->setText(m_pUpgradeSetting->mProxyAccount.mValue.c_str());
//        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_plePassword->setText(m_pUpgradeSetting->mProxyPasswd.mValue.c_str());
//        break;
//    default:
//        break;
//    }

//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->SetValue(m_pUpgradeSetting->mBandLimit.mValue);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleLimitedBandWidth->setText(QString::number(m_pUpgradeSetting->mBandWidth.mValue));
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleBandWidthLimitValidStartTime->setText(m_pUpgradeSetting->mStartTime.mValue.c_str());
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleBandWidthLimitValidEndTime->setText(m_pUpgradeSetting->mEndTime.mValue.c_str());

    std::list<SourceListEntry> entries;
    GetUpdateSource(entries);
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pTable->ClearAllItem();
    std::list<SourceListEntry>::iterator isle = entries.begin();
    while(isle != entries.end()){
        SourceListEntry sle = *isle;
        if(sle.mType == SourceListEntry::CLOUD_UPDATE_SOURCE){
            qDebug("xxxx 200000 from config addr=%s", sle.mAddress.c_str());
            m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pTable->AddItem(sle.mAddress.c_str(), sle.mOffical==1);
        }
        isle++;
    }
    //m_pRightEndWidget->m_pEndWidget->m_pUpgrade-

    UpdateGuiWidgetsStateUpgrade();
}

void MainWindow::UpdateGuiWidgetsStateUpgrade()
{
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbManual->setEnabled(m_pUpgradeSetting->mUpdateMode.mLocked==false);
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryDay->setEnabled(m_pUpgradeSetting->mUpdateMode.mLocked==false);
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryWeek->setEnabled(m_pUpgradeSetting->mUpdateMode.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInterval->setEnabled(m_pUpgradeSetting->mUpdateMode.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleDayTime->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryDay->isEnabled()&&
//                                                                          m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryDay->isChecked());
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pwdlWeekDays->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryWeek->isEnabled()&&
                                                                            m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryWeek->isChecked());
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleWeekTime->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryWeek->isEnabled()&&
                                                                           m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbEveryWeek->isChecked());
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleIntervalHours->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInterval->isEnabled()&&
//                                                                                m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInterval->isChecked());
//    //延时启动
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbDisabledDelayBoot->setEnabled(m_pUpgradeSetting->mDelayStartAction.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbAutoDalayBoot->setEnabled(m_pUpgradeSetting->mDelayStartAction.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInRecentMinutesBoot->setEnabled(m_pUpgradeSetting->mDelayStartAction.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleRecentMinutesBoot->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInRecentMinutesBoot->isEnabled()&&
//                                                                                    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbInRecentMinutesBoot->isChecked());
    //升级内容
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbUpgradeAll->setEnabled(m_pUpgradeSetting->mUpdateContent.mLocked==false);
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_prbUpgradeVirusDB->setEnabled(m_pUpgradeSetting->mUpdateContent.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclRealtimeCheckVDBVersion->setEnabled(m_pUpgradeSetting->mCheckVirusDbRealTime.mLocked==false);
//    //缓存空间
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclCleaningSysTemp->setEnabled(m_pUpgradeSetting->mClearUpdateCache.mLocked==false);
//    //强制更新
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclForceUpgrade->setEnabled(m_pUpgradeSetting->mForceUpdate.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleAfterDaysUpgrade->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclForceUpgrade->isEnabled()&&
//                     m_pUpgradeSetting->mForceUpdateDelay.mLocked==false&&m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclForceUpgrade->IsChecked());
////    //代理设置
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->setEnabled(m_pUpgradeSetting->mProxySetting.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleAddress->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->isEnabled()&&
//                                                                          m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->currentIndex()==2);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_plePort->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->isEnabled()&&
//                                                                       m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->currentIndex()==2);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleUser->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->isEnabled()&&
//                                                                       m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->currentIndex()==2);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_plePassword->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->isEnabled()&&
//                                                                           m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->currentIndex()==2);
////    //带宽限制
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->setEnabled(m_pUpgradeSetting->mBandLimit.mLocked==false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleLimitedBandWidth->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->isEnabled()&&
//                     m_pUpgradeSetting->mBandWidth.mLocked==false&&m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->IsChecked());
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleBandWidthLimitValidStartTime->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->isEnabled()&&
//                     m_pUpgradeSetting->mStartTime.mLocked==false&&m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->IsChecked());
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleBandWidthLimitValidEndTime->setEnabled(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->isEnabled()&&
//                     m_pUpgradeSetting->mEndTime.mLocked==false&&m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->IsChecked());


    //    //升级源
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pTable->setEnabled();
}

void MainWindow::UpdateFromConfigCenterDiscovery()
{
    list<SourceListEntry>::iterator it = m_pBaseSetting->mCenterAddr.mValue.begin();
    while(it != m_pBaseSetting->mCenterAddr.mValue.end()) {
        SourceListEntry sle = *it;
        if (sle.mType == SourceListEntry::CENTER_PROXY_SOUCE) {
            m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pTable->AddItem(QString::fromStdString(sle.mAddress), sle.mOffical==1?true:false);
        }
        it++;
    }
    int i, pos = -1, ary_conn_type[] = {0, 5, 30, 60, 120, 240, 480};
    for(i=0; i<7; i++) {
        if (ary_conn_type[i] == m_pBaseSetting->mCenterRetryInterval.mValue/60) {
            pos = i;
            break;
        }
    }
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pcbConnectType->setCurrentIndex(pos==-1?0:pos);
//    pos = -1;
//    int ary_band_width[] = {0, 10, 100, 200, 500};
//    for(i=0; i<5; i++) {
//        if (ary_band_width[i] == m_pBaseSetting->mUploadBandWidthType.mValue) {
//            pos = i;
//            break;
//        }
//    }

//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pcbUploadBandwidthLimit->setCurrentIndex(pos==-1?0:pos);

//    string start_time = m_pBaseSetting->mBandWidthStartTime;
//    string end_time = m_pBaseSetting->mBandWidthEndTime;

//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pleValidTimeStart->setText(QString::fromStdString(start_time));
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pleValidTimeEnd->setText(QString::fromStdString(end_time));
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pclUseProxy->SetValue(m_pBaseSetting->mCenterProxyState.mValue);
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pclOnlyValidedForTerminal->SetValue(m_pBaseSetting->mCheckEIDState.mValue);
//    //update gui controls status
//    if (m_pBaseSetting->mCenterAddr.mLocked) {
//        m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pbAddBtn->setEnabled(false);
//        m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pTable->setEnabled(false);
//    }
//    if (m_pBaseSetting->mUploadBandWidthType.mLocked) {
//        m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pcbUploadBandwidthLimit->setEnabled(false);
//        m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->lUploadBandwidthLimit->setEnabled(false);
//    }

//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pcbConnectType->setEnabled(!m_pBaseSetting->mCenterRetryInterval.mLocked);
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->lConnectType->setEnabled(!m_pBaseSetting->mCenterRetryInterval.mLocked);

//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pleValidTimeStart->setEnabled(!m_pBaseSetting->mCenterRetryInterval.mLocked);
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pleValidTimeEnd->setEnabled(!m_pBaseSetting->mCenterRetryInterval.mLocked);

//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pclUseProxy->setEnabled(!m_pBaseSetting->mCenterProxyState.mLocked);
//    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pclOnlyValidedForTerminal->setEnabled(!m_pBaseSetting->mCheckEIDState.mLocked);
}

void MainWindow::UpdateFromConfig()
{
    m_pAvConfig = new AntiVirusSetting;
    m_pNetfilterConfig = new NetfilterSetting;
    m_pUpgradeSetting = new UpgradeSetting;
    m_pBaseSetting = new BaseSetting;
    GetAntiVirusConfig(*m_pAvConfig);
    GetNetfilterConfig(*m_pNetfilterConfig);
    GetUpdateConfig(*m_pUpgradeSetting);
    GetUpdateSource(m_lstUpdateSouces);
    GetBaseConfig(*m_pBaseSetting);

    UpdateFromConfigOri();
    UpdateFromConfigWhiteList();
    UpdateFromConfigBackup();
    UpdateFromConfigKV();
    UpdateFromConfigTimer();
    UpdateFromConfigFile();
    UpdateFromConfigUDisk();

    if (!HideAdvanceFunction()) {
        UpdateFromConfigUpgrade();
        UpdateFromConfigCenterDiscovery();
    }
}


//enum{APP_MENU_DIRSEL_TYPE_DIR_CHILD = 0,
//     APP_MENU_DIRSEL_TYPE_DIR,
//     APP_MENU_DIRSEL_TYPE_CHILD,
//     APP_MENU_DIRSEL_TYPE_FILE
//};

int MainWindow::TransPathType(int nTableType)
{
    int ntype = -1;
    switch(nTableType){
    case APP_MENU_DIRSEL_TYPE_DIR_CHILD:
        ntype = WhiteListEntry::FOLDER_RECURSION;
        break;
    case APP_MENU_DIRSEL_TYPE_DIR:
        ntype = WhiteListEntry::FOLDER_ONLY;
        break;
    case APP_MENU_DIRSEL_TYPE_CHILD:
        ntype = WhiteListEntry::FOLDER_CHILDERN;
        break;
    case APP_MENU_DIRSEL_TYPE_FILE:
        ntype = WhiteListEntry::FILE_PATH;
        break;
    }
    return ntype;
}

//using namespace WhiteListEntry;

int MainWindow::RTransPathType(int nStoreType)
{
    int ntype = -1;
    switch(nStoreType){
    case WhiteListEntry::FOLDER_RECURSION:
        ntype = APP_MENU_DIRSEL_TYPE_DIR_CHILD;
        break;
    case WhiteListEntry::FOLDER_ONLY:
        ntype = APP_MENU_DIRSEL_TYPE_DIR;
        break;
    case WhiteListEntry::FOLDER_CHILDERN:
        ntype = APP_MENU_DIRSEL_TYPE_CHILD;
        break;
    case WhiteListEntry::FILE_PATH:
        ntype = APP_MENU_DIRSEL_TYPE_FILE;
        break;
    }
    return ntype;
}

bool MainWindow::IsConfigedWhiteEntry(std::list<WhiteListEntry> &lEntry, WhiteListEntry & qwle)
{
    std::list<WhiteListEntry>::iterator wit = lEntry.begin();
    while(wit != lEntry.end()){
        WhiteListEntry wle = *wit;
        if(wle.mListType == WhiteListEntry::BLACK && wle.mBlockType == WhiteListEntry::FOLDER_MONITOR && qwle.mListType ==
                WhiteListEntry::BLACK && qwle.mBlockType == WhiteListEntry::FOLDER_MONITOR){
            if(wle.mPath == qwle.mPath){
                return true;
            }
        }else if(wle.mListType == WhiteListEntry::BLACK && wle.mBlockType == WhiteListEntry::PROCESS_BLOCK && qwle.mListType ==
                 WhiteListEntry::BLACK && qwle.mBlockType == WhiteListEntry::PROCESS_BLOCK){
             if(wle.mPath == qwle.mPath){
                 return true;
             }
        }else if(wle.mBlockType == WhiteListEntry::FILE_SCAN && qwle.mBlockType == WhiteListEntry::FILE_SCAN){
            if(wle.mPathType == qwle.mPathType && wle.mPath == qwle.mPath){
                return true;
            }
        }else if(wle.mPathType == WhiteListEntry::FILE_EXTEND_TYPE && qwle.mPathType == WhiteListEntry::FILE_EXTEND_TYPE){
            if(wle.mPath == qwle.mPath){
                return true;
            }
        }
        wit++;
    }
    return false;
}

//for update source list
bool MainWindow::IsConfigdCloudUpdateSourceEntry(const std::list<SourceListEntry> &lEntry,const string & source_addr)
{
    std::list<SourceListEntry>::const_iterator it = lEntry.begin();
    while(it != lEntry.end()){
        SourceListEntry entry = *it;
        if(entry.mType == SourceListEntry::CLOUD_UPDATE_SOURCE && source_addr == entry.mAddress) {
            return true;
        }
        it++;
    }
    return false;
}

//for center discovery
bool MainWindow::IsConfigdCenterSourceEntry(const std::list<SourceListEntry> &lEntry, const string &source_addr)
{
    std::list<SourceListEntry>::const_iterator it = lEntry.begin();
    while(it != lEntry.end()){
        SourceListEntry entry = *it;
        if(entry.mType == SourceListEntry::CENTER_PROXY_SOUCE && source_addr == entry.mAddress) {
            return true;
        }
        it++;
    }
    return false;
}

//for kv-filescan-listed file extend name
bool MainWindow::IsConfigedScanEntry(std::list<WhiteListEntry> &lEntry, WhiteListEntry & qwle)
{
    if(qwle.mListType != WhiteListEntry::BLACK || qwle.mBlockType != WhiteListEntry::FILE_SCAN || qwle.mPathType != WhiteListEntry::FILE_EXTEND_TYPE)
        return false;

    std::list<WhiteListEntry>::iterator wit = lEntry.begin();
    while(wit != lEntry.end()){
        WhiteListEntry wle = *wit;
        if(wle.mListType == WhiteListEntry::BLACK && wle.mBlockType == WhiteListEntry::FILE_SCAN && wle.mPathType == WhiteListEntry::FILE_EXTEND_TYPE){
            if(wle.mPath == qwle.mPath){
                return true;
            }
        }
        wit++;
    }
    return false;
}

bool MainWindow::IsConfigedPrivateCloud(std::string name)
{
    list<AntiVirusSetting::AntiCommon::CloudModel>::iterator it = m_pAvConfig->mCommon.mCloudSetting.begin();
    while(it != m_pAvConfig->mCommon.mCloudSetting.end()){
        AntiVirusSetting::AntiCommon::CloudModel model = *it;
        if (model.mServerName == name) {
            return true;
        }
        it++;
    }
    return false;
}

void MainWindow::UpdateOrigPrivateCloudToConfig()
{
    if(m_pRightTopWidget->m_pTopWidget->m_pOri->m_pPrivateTable->m_AllItems.size() > 0){
        QVector<CloudParam*>::iterator it = m_pRightTopWidget->m_pTopWidget->m_pOri->m_pPrivateTable->m_AllItems.begin();
        while(it != m_pRightTopWidget->m_pTopWidget->m_pOri->m_pPrivateTable->m_AllItems.end()) {
            CloudParam * param = *it;
            if (IsConfigedPrivateCloud(param->name.toStdString())) {
                it++;
                continue;
            }
            AntiVirusSetting::AntiCommon::CloudModel model;
            model.mType = AntiVirusSetting::AntiCommon::PRIVATE_CLOUD;
            model.mState = param->is_selected;
            model.mServerName = param->name.toStdString();
            model.mServerAddr = param->address.toStdString();
            model.mPort = (short)param->port;
            m_pAvConfig->mCommon.mCloudSetting.push_back(model);
            it++;
        }
    }
    list<AntiVirusSetting::AntiCommon::CloudModel>::iterator it = m_pAvConfig->mCommon.mCloudSetting.begin();
    while ( it != m_pAvConfig->mCommon.mCloudSetting.end()) {
        AntiVirusSetting::AntiCommon::CloudModel model = *it;
        if (false == m_pRightTopWidget->m_pTopWidget->m_pOri->m_pPrivateTable->HasAddressName(QString::fromStdString(model.mServerName))) {
            it = m_pAvConfig->mCommon.mCloudSetting.erase(it);
            continue;
        }
        it++;
    }
}

//    int  GetProcessBlackList(std::list<WhiteListEntry>& white);
//    int  AddProcessBlackList(const std::string& process);
//    int  DeleteProcessBlackList(const WhiteListEntry& entry);
//    int  GetFileBlackList(std::list<WhiteListEntry>& white);
//    int  AddFileBlackList(const std::string& filepath);
//    int  DeleteFileBlackList(const WhiteListEntry& entry);
void MainWindow::UpdataFileMonitorListToConfig()
{
    std::list<WhiteListEntry> white;
    std::list<WhiteListEntry> tablewhite;

    GetFolderMonitorList(white);

    if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_pDirTable->m_AllItems.size() > 0){
        QMap<int, DDisplayItem*>::iterator iit = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pDirTable->m_AllItems.begin();
        while(iit != m_pRightTopWidget->m_pTopWidget->m_pFile->m_pDirTable->m_AllItems.end()){
            DDisplayItem * pdi = iit.value();
            WhiteListEntry wle;
            wle.mListType = WhiteListEntry::BLACK;
            wle.mBlockType = WhiteListEntry::FOLDER_MONITOR;
            wle.mPath = pdi->m_sName.toStdString();
            if(IsConfigedWhiteEntry(white, wle) == false)
                AddWhiteList(wle);
            tablewhite.push_back(wle);
            iit++;
        }
    }
    std::list<WhiteListEntry>::iterator wit = white.begin();
    while(wit != white.end()){
        WhiteListEntry wle = *wit;
        if(IsConfigedWhiteEntry(tablewhite, wle) == false){
            DeleteWhitelist(wle);
        }
        wit++;
    }
    white.clear();
    GetProcessBlackList(white);
    if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_pProcNameTable->m_AllItems.size() > 0){
        QVector<QString*>::iterator uit = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pProcNameTable->m_AllItems.begin();
        while(uit != m_pRightTopWidget->m_pTopWidget->m_pFile->m_pProcNameTable->m_AllItems.end()){
            QString *psname = *uit;
            WhiteListEntry wle;
            wle.mListType = WhiteListEntry::BLACK;
            wle.mBlockType = WhiteListEntry::PROCESS_BLOCK;
            wle.mPath = psname->toStdString();
            //cout << "xxx 100 process name:" << wle.mPath << endl;
            if(IsConfigedWhiteEntry(white, wle) == false) {
                AddWhiteList(wle);
                //cout << "xxx 101 added proc name whitelist" << endl;
            }
            tablewhite.push_back(wle);
            uit++;
        }
    }
    wit = white.begin();
    while(wit != white.end()){
        WhiteListEntry wle = *wit;
        if(IsConfigedWhiteEntry(tablewhite, wle) == false){
            DeleteWhitelist(wle);
        }
        wit++;
    }
}

void MainWindow::SetUpgradeSettingToConfig()
{
    ScheduleEntry entry;
    std::vector<int> fdays;
    QString text;
    int nid = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pbgUpgradeMode->checkedId();
    switch (nid) {
    case 0:
        m_pUpgradeSetting->mUpdateMode.mValue = UpgradeSetting::UPGRADE_MANUALLY;
        break;
    case 1:
        m_pUpgradeSetting->mUpdateMode.mValue = UpgradeSetting::UPGRADE_EVERY_DAY;
        text = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleDayTime->text();
        entry.mFuncEnum = ScheduleEntry::UPDATE;
        entry.mType = ScheduleEntry::DAYS;
        entry.mUserSet = true;
        if(text.size() == 5){
            entry.SetCrontabHour(text.left(2).toInt());
            entry.SetCrontabMinute(text.right(2).toInt());
        }
        break;
    case 2:
        m_pUpgradeSetting->mUpdateMode.mValue = UpgradeSetting::UPGRADE_EVERY_WEEK;
        entry.mFuncEnum = ScheduleEntry::UPDATE;
        entry.mType = ScheduleEntry::WEEKS;
        m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pwdlWeekDays->GetSelected(fdays);
        entry.SetCrontabDayInWeek(fdays);
        text = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleWeekTime->text();
        if(text.size() == 5){
            entry.SetCrontabHour(text.left(2).toInt());
            entry.SetCrontabMinute(text.right(2).toInt());
        }
        break;
//    case UPGRADE_INTERVAL:
//        text = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleIntervalHours->text();
//        entry.mFuncEnum = ScheduleEntry::UPDATE;
//        entry.mType = ScheduleEntry::ALL;
//        entry.mUserSet = true;
//        if(text.size() > 0){
//            entry.SetCrontabHour(text.toInt());
//        }
//        break;
    default:
        break;
    }
    //delay boot
//    nid = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pbgDelayBoot->checkedId();
//    m_pUpgradeSetting->mDelayStartAction.mValue = (UpgradeSetting::DELAY_START)nid;
//    text = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleRecentMinutesBoot->text();
//    m_pUpgradeSetting->mDelayStartTime.mValue = atoi(text.toStdString().c_str());

    SetScheduleEntry(entry);

//    m_pUpgradeSetting->mDelayStartAction.mValue = (UpgradeSetting::DELAY_START)m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pbgDelayBoot->checkedId();
//    if ( m_pUpgradeSetting->mDelayStartAction.mValue == UpgradeSetting::DELAY_INTIME ) {
//        m_pUpgradeSetting->mDelayStartTime.mValue = atoi(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleRecentMinutesBoot->text().toStdString().c_str());
//    }
//    //content

    int rid = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pbgUpgradeType->checkedId();
    switch (rid) {
    case 0:
        m_pUpgradeSetting->mUpdateContent.mValue = UpgradeSetting::UPDATE_ALL;
        break;
    case 1:
        m_pUpgradeSetting->mUpdateContent.mValue = UpgradeSetting::UPDATE_VIRUS_DB;
        break;
    }

//    m_pUpgradeSetting->mCheckVirusDbRealTime.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclRealtimeCheckVDBVersion->IsChecked();

//    //cache
//    m_pUpgradeSetting->mClearUpdateCache.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclCleaningSysTemp->IsChecked();
//    //force update
//    m_pUpgradeSetting->mForceUpdate.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclForceUpgrade->IsChecked();
//    m_pUpgradeSetting->mForceUpdateDelay.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleAfterDaysUpgrade->text().toInt();
//    //proxy
//    m_pUpgradeSetting->mProxySetting.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->currentIndex()==0?UpgradeSetting::CONNECT_DIRECT:
//                                      (m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pcbProxyType->currentIndex()==1?UpgradeSetting::CONNECT_BROWSER:UpgradeSetting::CONNECT_PROXY);
//    if ( m_pUpgradeSetting->mProxySetting.mValue == UpgradeSetting::CONNECT_PROXY ) {
//        m_pUpgradeSetting->mProxyAddr.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleAddress->text().toStdString();
//        m_pUpgradeSetting->mProxyPort.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_plePort->text().toStdString();
//        m_pUpgradeSetting->mProxyAccount.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleUser->text().toStdString();
//        m_pUpgradeSetting->mProxyPasswd.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_plePassword->text().toStdString();
//    }
//    //band width config
//    m_pUpgradeSetting->mBandLimit.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pclBandWidthLimit->IsChecked();
//    m_pUpgradeSetting->mBandWidth.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleLimitedBandWidth->text().toInt();
//    m_pUpgradeSetting->mStartTime.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleBandWidthLimitValidStartTime->text().toStdString();
//    m_pUpgradeSetting->mEndTime.mValue = m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pleBandWidthLimitValidEndTime->text().toStdString();

    SetUpdateConfig(*m_pUpgradeSetting);

    std::list<SourceListEntry> entries_to_save;
    vector<UpdateTableItem> table_source;
    if(m_pRightEndWidget->m_pEndWidget->m_pUpgrade->m_pTable->GetAllItem(&table_source) > 0){
        vector<UpdateTableItem>::iterator iit = table_source.begin();
        while(iit != table_source.end()){
            UpdateTableItem saddr = *iit;
            if((saddr.is_from_cloud_rules == false) && (IsConfigdCloudUpdateSourceEntry(entries_to_save, saddr.source_name.toStdString()) == false)) {
                SourceListEntry source_entry;
                source_entry.mType = SourceListEntry::CLOUD_UPDATE_SOURCE;
                source_entry.mAddress = saddr.source_name.toStdString();
                source_entry.mOffical = 0; //(saddr.is_from_cloud_rules?1:0);
                //qDebug("xxxx 300000 added saddr=%s", source_entry.mAddress.c_str());
                entries_to_save.push_back(source_entry);                
            }
            iit++;
        }
    }
    //qDebug("xxxx 100001 entries.size=%u", entries_to_save.size());

    m_pBaseSetting->mOtherUpdateCenter.mValue.clear();
    std::list<SourceListEntry>::iterator iit = entries_to_save.begin();
    while (iit != entries_to_save.end()) {
        SourceListEntry sle = *iit;
        m_pBaseSetting->mOtherUpdateCenter.mValue.push_back(sle);
        iit++;
    }
}

void MainWindow::SetCenterDiscoverySettingToConfig()
{
    std::list<SourceListEntry> entries_to_save;
    if(m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pTable->m_AllItems.size() > 0){
        QVector<CenterDiscoveryTableItem*>::iterator it = m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pTable->m_AllItems.begin();
        while(it != m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pTable->m_AllItems.end()){
            CenterDiscoveryTableItem *cp = *it;

            printf("%s\n", cp->center_address.toStdString().c_str());
            if((IsConfigdCenterSourceEntry(entries_to_save, cp->center_address.toStdString()) == false) && (cp->is_official == false)) {
                printf("save it\n");
                SourceListEntry source_entry;
                source_entry.mType = SourceListEntry::CENTER_PROXY_SOUCE;
                source_entry.mAddress = cp->center_address.toStdString();
                source_entry.mOffical = 0;
                entries_to_save.push_back(source_entry);
            }
            it++;
        }
    }
    m_pBaseSetting->mCenterAddr.mValue.clear();
    std::list<SourceListEntry>::iterator iit = entries_to_save.begin();
    while (iit != entries_to_save.end()) {
        SourceListEntry sle = *iit;
        m_pBaseSetting->mCenterAddr.mValue.push_back(sle);
        iit++;
    }

//    int ary_conn_type[] = {0, 5, 30, 60, 120, 240, 480};
//    m_pBaseSetting->mCenterRetryInterval.mValue = ary_conn_type[m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pcbConnectType->currentIndex()];
//    int ary_band_width[] = {0, 10, 100, 200, 500};

//    m_pBaseSetting->mUploadBandWidthType.mValue = ary_band_width[m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pcbUploadBandwidthLimit->currentIndex()];
//    m_pBaseSetting->mBandWidthStartTime = m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pleValidTimeStart->text().toStdString();
//    m_pBaseSetting->mBandWidthEndTime += m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pleValidTimeEnd->text().toStdString();

//    m_pBaseSetting->mCenterProxyState.mValue = m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pclUseProxy->IsChecked();
//    m_pBaseSetting->mCheckEIDState.mValue = m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->m_pclOnlyValidedForTerminal->IsChecked();
}

void MainWindow::UpdateUserDefinedScanFileExt(const char* sText)
{
    string stext = sText;
    std::list<WhiteListEntry> conflist;
    GetScanFileTypeList(conflist);

    std::list<WhiteListEntry> lwle;
    int nret = SetStrToWhiteList(stext.c_str(), lwle);
    if(nret > 0){
       std::list<WhiteListEntry>::iterator it =  lwle.begin();
       while(it != lwle.end()){
           WhiteListEntry wle = *it;
           if(!IsConfigedScanEntry(conflist, wle)){
               AddWhiteList(wle);
           }
           it++;
       }
       it =  conflist.begin();
       while(it != conflist.end()){
           WhiteListEntry wle = *it;
           if(!IsConfigedScanEntry(lwle, wle)){
               DeleteWhitelist(wle);
           }
           it++;
       }
    }else{
        std::list<WhiteListEntry>::iterator it =  conflist.begin();
        while(it != conflist.end()){
            WhiteListEntry wle = *it;
            DeleteWhitelist(wle);
            it++;
        }
    }

}

int MainWindow::UpdateToConfig()
{
    //whitelist
    std::list<WhiteListEntry> white;
    std::list<WhiteListEntry> tablewhite;
    GetWhiteList(white);
    vector<DfItemData> vidata;
    if(m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_pdfDirFile->GetAllItem(&vidata) > 0){
        vector<DfItemData>::iterator iit = vidata.begin();
        while(iit != vidata.end()){
            DfItemData did = *iit;
            WhiteListEntry wle;
            wle.mPathType = (WhiteListEntry::PATH_TYPE)TransPathType(did.m_nType);
            wle.mPath = did.m_sName;
            if(IsConfigedWhiteEntry(white, wle) == false)
                AddWhiteList(wle);
            tablewhite.push_back(wle);
            iit++;
        }
    }
    vector<string> vudata;
    if(m_pRightTopWidget->m_pTopWidget->m_pWhiteList->m_penExtName->GetAllItem(&vudata) > 0){
        vector<string>::iterator uit = vudata.begin();
        while(uit != vudata.end()){
            string sext = *uit;
            WhiteListEntry wle;
            wle.mPathType = WhiteListEntry::FILE_EXTEND_TYPE;
            wle.mPath = sext;
            if(IsConfigedWhiteEntry(white, wle) == false)
                AddWhiteList(wle);
            tablewhite.push_back(wle);
            uit++;
        }
    }
    std::list<WhiteListEntry>::iterator wit = white.begin();
    while(wit != white.end()){
        WhiteListEntry wle = *wit;
        if(IsConfigedWhiteEntry(tablewhite, wle) == false){
            DeleteWhitelist(wle);
        }
        wit++;
    }
    //memset(m_pAvConfig, 0, sizeof(AntiVirusSetting));
    delete m_pAvConfig;
    m_pAvConfig = NULL;
    m_pAvConfig = new AntiVirusSetting;
    GetAntiVirusConfig(*m_pAvConfig);
    //org
    m_pAvConfig->mCommon.mRunEnvJudge.mValue =  m_pRightTopWidget->m_pTopWidget->m_pOri->m_pRunEnv->IsChecked();
    m_pAvConfig->mCommon.mVirusTrace.mValue = m_pRightTopWidget->m_pTopWidget->m_pOri->m_pVirusTrace->IsChecked();
    m_pAvConfig->mCommon.mVirusLoged.mValue = m_pRightTopWidget->m_pTopWidget->m_pOri->m_pVirusLog->IsChecked();
    m_pAvConfig->mCommon.mRescanAccelerate.mValue = m_pRightTopWidget->m_pTopWidget->m_pOri->m_pBuffer->IsChecked();
    if (!HideAdvanceFunction()) {
// 设置中去掉了公有云或者私有云的设置项， 如果需要把云扫描加上， 则把此处放开
//        m_pAvConfig->mCommon.mUsePublicCloud.mValue = m_pRightTopWidget->m_pTopWidget->m_pOri->m_pCloud->IsChecked();
//        UpdateOrigPrivateCloudToConfig();
    }
    //backup
    m_pAvConfig->mVirusBackup.mVirusBackup.mValue = m_pRightTopWidget->m_pTopWidget->m_pBackup->m_pclBackup->m_bChecked;
    switch(m_pRightTopWidget->m_pTopWidget->m_pBackup->m_pbgLongFile->checkedId())
    {
    case APP_ASK_ME:
        m_pAvConfig->mVirusBackup.mLargeFile.mValue  = LARGE_FILE_QUERY;
        break;
    case APP_DELETE_FILE:
        m_pAvConfig->mVirusBackup.mLargeFile.mValue  = LARGE_FILE_DELETE;
        break;
    case APP_DONT_CARE:
        m_pAvConfig->mVirusBackup.mLargeFile.mValue  = LARGE_FILE_IGNORE;
        break;
    }
    switch(m_pRightTopWidget->m_pTopWidget->m_pBackup->m_pbgNoSpace->checkedId()){
    case APP_AUTO_REPLACE:
        m_pAvConfig->mVirusBackup.mSpaceLess.mValue = OVERRIDE_OLD;
        break;
    case APP_AUTO_INCREASE:
        m_pAvConfig->mVirusBackup.mSpaceLess.mValue = INCREAM_SPACE;
        break;
    }
    switch(m_pRightTopWidget->m_pTopWidget->m_pBackup->m_pbgBackupFail->checkedId()){
    case APP_ASK_ME:
        m_pAvConfig->mVirusBackup.mBackupFailed.mValue = BACKUP_FAILED_QUERY;
        break;
    case APP_DELETE_FILE:
        m_pAvConfig->mVirusBackup.mBackupFailed.mValue = BACKUP_FAILED_DELETE;
        break;
    case APP_DONT_CARE:
        m_pAvConfig->mVirusBackup.mBackupFailed.mValue = BACKUP_FAILED_IGNORE;
        break;
    }
    //kvgroup
    if(m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAllFiles->isChecked()){
        m_pAvConfig->mVirusScan.mFileType.mValue = ALL_FILE;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbProgDoc->isChecked()){
        m_pAvConfig->mVirusScan.mFileType.mValue = EXE_DOC;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbUserDefine->isChecked()){
        m_pAvConfig->mVirusScan.mFileType.mValue = SPECIAL_FILE_TYPE;
    }
    m_pAvConfig->mVirusScan.mScanCommonVirus.mValue = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclOnWildlist->IsChecked();
    m_pAvConfig->mVirusScan.mScanIntelligent.mValue = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclHeurist->IsChecked();
    m_pAvConfig->mVirusScan.mScanCompressFile.mValue = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pclProcCompressed->IsChecked();
    m_pAvConfig->mVirusScan.mZipFileSize.mValue = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleSize->text().toInt();
    if(m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbAuto->isChecked()){
        m_pAvConfig->mVirusScan.mVirusDeal.mValue = AUTO_DEAL;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbManual->isChecked()){
        m_pAvConfig->mVirusScan.mVirusDeal.mValue = MANUAL_DEAL;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pKV->m_prbIgnore->isChecked()){
        m_pAvConfig->mVirusScan.mVirusDeal.mValue = IGNORE_DEAL;
    }
    m_pAvConfig->mCloudEngine.mEnable.mValue = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pCloudEnable ? m_pRightTopWidget->m_pTopWidget->m_pKV->m_pCloudEnable->IsChecked():0;
	m_pAvConfig->mCloudEngine.mUrl.mValue = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pCloudUrl ? m_pRightTopWidget->m_pTopWidget->m_pKV->m_pCloudUrl->text().toStdString():"";
    //timergroup --fullscan
    if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pclAllDiskScan->IsChecked()){
        m_pAvConfig->mTimeScan.mFullScan.mValue = true;
        ScheduleEntry fullConf;
        fullConf.mUserSet = true;
        fullConf.mFuncEnum = ScheduleEntry::ALL_SCAN;
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTPowerOn->isChecked()){
            fullConf.mType = ScheduleEntry::RESTART;
        }
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryDay->isChecked()){
            fullConf.mType = ScheduleEntry::DAYS;
            QString strtime = m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTDayTime->text();
            if(strtime.size() == 5){
                fullConf.SetCrontabHour(strtime.left(2).toInt());
                fullConf.SetCrontabMinute(strtime.right(2).toInt());
                //qDebug("xxxxxxxxxxxxx ALL_SCAN timer every day %02d:%02d", strtime.left(2).toInt(), strtime.right(2).toInt());
            }/*else{

                m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTDayTime->setFocus();
                return -1;
            }*/
        }
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryWeek->isChecked()){
            fullConf.mType = ScheduleEntry::WEEKS;
            std::vector<int> days;
            m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pTWeekSel->GetSelected(days);
            fullConf.SetCrontabDayInWeek(days);
            QString strtime = m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTWeekTime->text();
            if(strtime.size() == 5){
                fullConf.SetCrontabHour(strtime.left(2).toInt());
                fullConf.SetCrontabMinute(strtime.right(2).toInt());
            }
        }
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbTEveryMonth->isChecked()){
            fullConf.mType = ScheduleEntry::MONTH;
            fullConf.SetCrontabDayInMonth(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbTMonthDay->currentIndex()+1);
            QString strtime = m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleTMonthTime->text();
            if(strtime.size() == 5){
                fullConf.SetCrontabHour(strtime.left(2).toInt());
                fullConf.SetCrontabMinute(strtime.right(2).toInt());
            }
        }
        //m_pAvConfig->mTimeScan.mValue[ScheduleEntry::ALL_SCAN] = fullConf;
        m_pAvConfig->mTimeScan.SetTimeScanEntry(ScheduleEntry::ALL_SCAN, fullConf);
    }else{
        m_pAvConfig->mTimeScan.mFullScan.mValue = false;
    }
    //    --quickscan
    if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pclFastScan->IsChecked()){
        m_pAvConfig->mTimeScan.mQuickScan.mValue = true;
        ScheduleEntry quickConf;
        quickConf.mUserSet = true;
        quickConf.mFuncEnum = ScheduleEntry::QUICK_SCAN;
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBPowerOn->isChecked()){
           quickConf.mType = ScheduleEntry::RESTART;
        }
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryDay->isChecked()){
           quickConf.mType = ScheduleEntry::DAYS;
           QString strtime = m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBDayTime->text();
           if(strtime.size() == 5){
               quickConf.SetCrontabHour(strtime.left(2).toInt());
               quickConf.SetCrontabMinute(strtime.right(2).toInt());
           }/*else{
               m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBDayTime->setFocus();
               return -1;
           }*/
           //qDebug("xxxxxxxxxxxxx QUICK_SCAN timer every day %02d:%02d", strtime.left(2).toInt(), strtime.right(2).toInt());
        }
        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryWeek->isChecked()){
            quickConf.mType = ScheduleEntry::WEEKS;
            std::vector<int> days;
            m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pBWeekSel->GetSelected(days);
            quickConf.SetCrontabDayInWeek(days);
            QString strtime = m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBWeekTime->text();
            if(strtime.size() == 5){
                quickConf.SetCrontabHour(strtime.left(2).toInt());
                quickConf.SetCrontabMinute(strtime.right(2).toInt());
            }
        }

        if(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_prbBEveryMonth->isChecked()){
            quickConf.mType = ScheduleEntry::MONTH;
            quickConf.SetCrontabDayInMonth(m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pcbBMonthDay->currentIndex()+1);
            QString strtime = m_pRightTopWidget->m_pTopWidget->m_pTimer->m_pleBMonthTime->text();
            if(strtime.size() == 5){
                quickConf.SetCrontabHour(strtime.left(2).toInt());
                quickConf.SetCrontabMinute(strtime.right(2).toInt());
            }
        }
        //m_pAvConfig->mTimeScan.mValue[ScheduleEntry::QUICK_SCAN] = quickConf;
        m_pAvConfig->mTimeScan.SetTimeScanEntry(ScheduleEntry::QUICK_SCAN, quickConf);
    }else{
       m_pAvConfig->mTimeScan.mQuickScan.mValue = false;
    }

    //filegroup
    //m_pAvConfig->mFileMonitor.mStartPowerOn.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartAtPowerOn->IsChecked();
    //m_pAvConfig->mFileMonitor.mIntelligentBlacklist.mValue =  m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartSmartBlacklist->IsChecked();
//    m_pAvConfig->mFileMonitor.mKernelMonitor.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclStartKernelScan->IsChecked();
    /*if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFastMode->isChecked()){
        m_pAvConfig->mFileMonitor.mMonitorType.mValue = 0;
    }else
        */
        if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbStandardMode->isChecked()){
        m_pAvConfig->mFileMonitor.mMonitorType.mValue = 1;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbProfessionalMode->isChecked()){
        m_pAvConfig->mFileMonitor.mMonitorType.mValue = 2;
    }
        /*
        else if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbPlusMode->isChecked()){
        m_pAvConfig->mFileMonitor.mMonitorType.mValue = 3;
    }
    */

    if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbAllFiles->isChecked()){
        m_pAvConfig->mFileMonitor.mFileType.mValue = ALL_FILE;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbProgDoc->isChecked()){
        m_pAvConfig->mFileMonitor.mFileType.mValue = EXE_DOC;
    }
    m_pAvConfig->mFileMonitor.mMonitorAccelerate.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pTrustProgAnalysis->IsChecked();
//    m_pAvConfig->mFileMonitor.mInnerScan.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclEmbedSacan->IsChecked();

    m_pAvConfig->mFileMonitor.mScanCommonVirus.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclOnWildlist->IsChecked();
    m_pAvConfig->mFileMonitor.mScanIntelligent.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclHeuristScan->IsChecked();
    m_pAvConfig->mFileMonitor.mZipScan.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclProcCompressed->IsChecked();
    m_pAvConfig->mFileMonitor.mZipFileSize.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pleSize->text().toInt();

    if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVAuto->isChecked()){
       m_pAvConfig->mFileMonitor.mVirusDeal.mValue = AUTO_DEAL;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVManual->isChecked()){
       m_pAvConfig->mFileMonitor.mVirusDeal.mValue = MANUAL_DEAL;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFoundVIgnore->isChecked()){
        m_pAvConfig->mFileMonitor.mVirusDeal.mValue = IGNORE_DEAL;
    }
    if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFailVDelete->isChecked()){
        m_pAvConfig->mFileMonitor.mCleanFailed.mValue = 0;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pFile->m_prbFailVIgnore->isChecked()){
        m_pAvConfig->mFileMonitor.mCleanFailed.mValue = 2;
    }
    m_pAvConfig->mFileMonitor.mCleanPrompt.mValue = m_pRightTopWidget->m_pTopWidget->m_pFile->m_pclNotifyMe->IsChecked();
    UpdataFileMonitorListToConfig();
    //udiskgroup
    if(m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_prbAskMe->isChecked()){
        m_pAvConfig->mPluginMonitor.mPluginDeal.mValue = 1;
    }else if(m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_prbProcImmediately->isChecked()){
        m_pAvConfig->mPluginMonitor.mPluginDeal.mValue = 0;
    }
    m_pAvConfig->mPluginMonitor.mScanDepth.mValue = m_pRightTopWidget->m_pTopWidget->m_pUDisk->m_pleSize->text().toInt();
//    qDebug("xxxxxxxxxxxxx UpdateScanConfig QUICK_SCAN timer every day %02d:%02d",
//           m_pAvConfig->mTimeScan.mValue[ScheduleEntry::QUICK_SCAN].GetCrontabHour(),
//            m_pAvConfig->mTimeScan.mValue[ScheduleEntry::QUICK_SCAN].GetCrontabMinute());
//    qDebug("xxxxxxxxxxxxx UpdateScanConfig ALL_SCAN timer every day %02d:%02d",
//           m_pAvConfig->mTimeScan.mValue[ScheduleEntry::ALL_SCAN].GetCrontabHour(),
//            m_pAvConfig->mTimeScan.mValue[ScheduleEntry::ALL_SCAN].GetCrontabMinute());
    if (!HideAdvanceFunction()) {
        SetUpgradeSettingToConfig();
        SetCenterDiscoverySettingToConfig();
    }
    else {
        if (m_pRightTopWidget->m_pTopWidget->m_pOri->m_CenterAddr) {
            m_pAvConfig->mCenterAddr = m_pRightTopWidget->m_pTopWidget->m_pOri->m_CenterAddr->text().toStdString();
        }
    }
    // 在这里存储中心发现的设置
    UpdateBaseConfig(*m_pBaseSetting);

    int ret = UpdateScanConfig(*m_pAvConfig);
//    qDebug("xxxxxxxxxxxxxxxxxxxxxx ret=%d", ret);
    string stext = m_pRightTopWidget->m_pTopWidget->m_pKV->m_pleFileTypes->text().toStdString();
    ret = SetScanFileCustomType(stext.c_str());
//    qDebug("xxx SetScanFileCustomType() type=%s ret=%d", stext.c_str(), ret);

    return 0;
}


void MainWindow::OnClickedLeftKill()
{
    m_pLeftWidget->SetTopDisable();
   // m_pRightMidWidget->hide();
    m_pRightEndWidget->hide();
    m_pRightTopWidget->show();
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->TopSelectedLabel());

    if(m_pLeftWidget->TopSelectedLabel() == m_pLeftWidget->m_pitlOri){
        //qDebug("MainWindow::OnClickedLeftKill before call init click");
        OnClickedLeftOri();
    }
}

//void MainWindow::OnClickedLeftNetDefend()
//{
//    m_pLeftWidget->SetMidDisable();
//    m_pRightTopWidget->hide();
 //   m_pRightEndWidget->hide();
//    m_pRightMidWidget->show();
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->MidSelectedLabel());


//    OnClickedLeftNetBlacklist();
//}

void MainWindow::OnClickedLeftSetup()
{
    m_pLeftWidget->SetEndDisable();
    m_pRightEndWidget->show();
    m_pRightTopWidget->hide();
//    m_pRightMidWidget->hide();
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->EndSelectLabel());

//    if(m_pLeftWidget->EndSelectLabel() == m_pLeftWidget->m_pitlTray){
    if(m_pLeftWidget->EndSelectLabel() == m_pLeftWidget->m_pitlUpgrade){
        //qDebug("MainWindow::OnClickedLeftSetup before call init click");
        OnClickedLeftUpgrade();
    }
}

void MainWindow::OnClickedLeftOri()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlOri);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(true);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(0);
}

void MainWindow::OnClickedLeftWhiteList()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlWhiteList);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(true);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(162);
}

void MainWindow::OnClickedLeftBackup()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlBackup);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(true);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(434);
}

void MainWindow::OnClickedLeftKV()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlKV);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(true);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(582);
}

void MainWindow::OnClickedLeftTimer()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlTimer);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(true);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(800);
}

void MainWindow::OnClickedLeftFile()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlFile);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(true);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(1050);
}

void MainWindow::OnClickedLeftUDisk()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlUDisk);
    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pTimer->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(true);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
    m_pRightTopWidget->verticalScrollBar()->setValue(1400);
}

//void MainWindow::OnClickedLeftNetBlacklist()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlNetBlacklist);
//    m_pRightMidWidget->m_pMidWidget->m_pBlacklist->SetFocus(true);
//}

//void MainWindow::OnClickedLeftSystem()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlSystem);
//    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(true);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(false);
//    m_pRightTopWidget->verticalScrollBar()->setValue(1190);
//}

//void MainWindow::OnClickedLeftApp()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlApp);
//    m_pRightTopWidget->m_pTopWidget->m_pOri->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pWhiteList->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pBackup->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pKV->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pFile->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pUDisk->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pSystem->SetFocus(false);
//    m_pRightTopWidget->m_pTopWidget->m_pApp->SetFocus(true);
//    m_pRightTopWidget->verticalScrollBar()->setValue(1190);
//}

//void MainWindow::OnClickedLeftAdmin()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlTray);
//    //m_pRightEndWidget->m_pEndWidget->m_pAdmin->SetFocus(true);
//    m_pRightEndWidget->m_pEndWidget->m_pTray->SetFocus(true);
//    m_pRightEndWidget->m_pEndWidget->m_pLicense->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->SetFocus(false);
//    m_pRightEndWidget->verticalScrollBar()->setValue(0);
//}

//void MainWindow::OnClickedLeftTray()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlTray);
//    //m_pRightEndWidget->m_pEndWidget->m_pAdmin->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pTray->SetFocus(true);
//    m_pRightEndWidget->m_pEndWidget->m_pLicense->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->SetFocus(false);
//    m_pRightEndWidget->verticalScrollBar()->setValue(0);
//}

//void MainWindow::OnClickedLeftLicense()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlLicense);
//   // m_pRightEndWidget->m_pEndWidget->m_pAdmin->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pTray->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pLicense->SetFocus(true);
//    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->SetFocus(false);
//    m_pRightEndWidget->verticalScrollBar()->setValue(40);
//}

void MainWindow::OnClickedLeftUpgrade()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlUpgrade);
    //m_pRightEndWidget->m_pEndWidget->m_pAdmin->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pTray->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pLicense->SetFocus(false);
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->SetFocus(true);
    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->SetFocus(false);
    m_pRightEndWidget->verticalScrollBar()->setValue(5);
}

void MainWindow::OnClickedLeftCenterDiscovery()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlCenterDiscovery);
    //m_pRightEndWidget->m_pEndWidget->m_pAdmin->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pTray->SetFocus(false);
//    m_pRightEndWidget->m_pEndWidget->m_pLicense->SetFocus(false);
    m_pRightEndWidget->m_pEndWidget->m_pUpgrade->SetFocus(false);
    m_pRightEndWidget->m_pEndWidget->m_pCenterDiscovery->SetFocus(true);
    m_pRightEndWidget->verticalScrollBar()->setValue(600);
}

//void MainWindow::OnChangeAdminPasswd()
//{

//}

void MainWindow::OnSaveTimer()
{
    m_pTimer->stop();
    QMutexLocker locker(&m_Mutex);
    if (UpdateToConfig() != 0) {
        m_ppbApply->setEnabled(true);
        return;
    }
    m_ppbApply->setEnabled(true);
    close();
}

void MainWindow::OnClickedSaveSetting()
{
    if(!m_ppbApply->isEnabled())
        return;
    m_ppbApply->setEnabled(false);
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    disconnect(m_btnConn);
#else
    disconnect(m_ppbApply, SIGNAL(clicked()), this, SLOT(OnClickedSaveSetting()));
#endif
    QMutexLocker locker(&m_Mutex);
    m_pTimer->start(500);
}

void MainWindow::OnClickedLoadDefault()
{
    //qDebug("MainWindow::OnClickedLoadDefault()");
    if(!m_ppbApply->isEnabled())
        return;
    m_ppbApply->setEnabled(false);
    QMutexLocker locker(&m_Mutex);
    ResetConfig();
    m_ppbApply->setEnabled(true);
    close();
}

void MainWindow::AutoSetFocus(int nScrollValue)
{
    if(m_pRightTopWidget->m_bMoveStart && m_pRightTopWidget->m_bScrollStart){
        //qDebug("MainWindow::AutoSetFocus RightTopWidget value=%d", nScrollValue);
        if(nScrollValue < 44){
            OnClickedLeftOri();
        }else if(nScrollValue>=44 && nScrollValue <=204){
            OnClickedLeftWhiteList();
        }else if(nScrollValue>204 && nScrollValue<=470){
            OnClickedLeftBackup();
        }else if(nScrollValue>470 && nScrollValue<570){
            OnClickedLeftKV();
        }else if(nScrollValue>753 && nScrollValue<986){
            OnClickedLeftTimer();
        }else if(nScrollValue>=986 && nScrollValue<=1211){
            OnClickedLeftFile();
        }else if(nScrollValue>1211){
            OnClickedLeftUDisk();
        }/*else if(nScrollValue>1000 && nScrollValue<1190){
            //OnClickedLeftSystem();
        }else if(nScrollValue == 1190){
            //OnClickedLeftApp();
        }*/
    }
    if(m_pRightEndWidget->m_bMoveStart && m_pRightEndWidget->m_bScrollStart){
        //qDebug("MainWindow::AutoSetFocus RightEndWidget value=%d", nScrollValue);
        //if(nScrollValue == 0){
//            OnClickedLeftTray();
//        }else if(nScrollValue>=64 && nScrollValue<122){
//            OnClickedLeftLicense();
//        }else if(nScrollValue >= 122){
        if (nScrollValue <= 122) {
            OnClickedLeftUpgrade();
        }else {
            OnClickedLeftCenterDiscovery();
        }
    }
}


void MainWindow::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
//    QPainter painter(this);
//    painter.setBrush(QColor(57,23,92));
//    QPen pen = painter.pen();
//    pen.setStyle(Qt::SolidLine);
//    painter.setPen(pen);
//    QRect pos = rect();
//    qDebug("MainWindow::paintEvent x,y,w,h:%d  %d  %d  %d",pos.x(), pos.y(), pos.width(), pos.height());
//    //painter.drawRect(pos.adjusted(0,0,-pen.width(),-pen.width()));
//    painter.fillRect(pos.adjusted(0,0,-pos.width(),-pos.height()),Qt::red);
}

void MainWindow::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        QPoint pt = QCursor::pos();
        //qDebug("MainWindow::mousePressEvent pt x,y:%d %d", pt.x(), pt.y());
        if(m_pTopWidget->rect().contains(mapFromGlobal(pt))){
            m_bDragging = true;
            m_poMouseStartPos = pEvent->globalPos();
            m_poFrameStartPos = frameGeometry().topLeft();
        }

    }
    QMainWindow::mousePressEvent(pEvent);
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
    QMainWindow::mouseMoveEvent(pEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton){
        m_bDragging = false;
    }
    QMainWindow::mouseReleaseEvent(pEvent);
}


void MainWindow::setMinimum()
{
//    this->showMinimized();
    this->hide();
    m_pSystemTrayIcon->show();
}

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

