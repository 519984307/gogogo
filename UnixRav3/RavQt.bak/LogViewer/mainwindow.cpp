#include <QPainter>
#include <QPalette>
#include <QFont>
#include <QScrollBar>
#include <QDesktopWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "mainwindow.h"
#include "common/pubdef.h"
#include "common/globalconf.h"
#include "topwidget.h"
#include "leftwidget.h"
#include "rightwidget.h"
#include "virusdetailgroup.h"
#include "operationpower.h"

#include "baseconfig.h"
#include "appconfig.h"

#include "../Storage/Entry/whitelistentry.h"
#include "../Storage/Entry/quarantinelog.h"
#include "../Storage/Entry/logevententry.h"
#include "../Storage/Entry/virusinfoentry.h"
#include "../Storage/storage.h"
#include "../Storage/cinifile.h"
#include "../Storage/Entry/commonentry.h"
#include "../CommonAPI/communicatebundle.h"
#include "../CommonAPI/configfile.h"

const RavQtUser g_Users[] = {RavQtUser(0x003F), RavQtUser(0x03FF), RavQtUser(0x003B), RavQtUser(0x003F)};

MainWindow::MainWindow(int function_stage, QWidget *parent) : Application(Application::NORMAL_APP),
    QMainWindow(parent), m_pSystemTrayIcon(NULL)
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setWindowOpacity(1);
    //setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
    //setStyleSheet("background-color:rgb(248,248,248)");
    m_nTopSelectedIndex = 0;
    m_nMidSelectedIndex = LOGVIEW_SCREEN_URLACCESS;
    m_nEndSelectedIndex = LOGVIEW_SCREEN_INSTALL;
    m_nFunctionStage = function_stage;
//    if (Construct(FOLDER_GJCZ_ETC) != 0) {
//        qDebug("Error, MainWindow::MainWindow Construct(FOLDER_GJCZ_ETC) fail");
//        exit(-1);
//    }
    m_pCenterListener = nullptr;

    if(Construct() != 0) {
        qDebug("Error, MainWindow::MainWindow Construct() fail\n");
        exit(-1);
    }
//    if(IsSecretMachine()){
//        UserType type = GetUserType();
//        //qDebug("MainWindow::MainWindow UserType %d", type);
//        bool permit = g_Users[type].HavePower(OperationPower::OP_POWER_VIEWLOG);
//        if(!permit) {
//            qDebug("Error, MainWindow::MainWindow no permission to do this");
//            QMessageBox msg(this);
//            msg.setIcon(QMessageBox::Information);
//            msg.setText(u8"当前用户没有权限执行此操作");
//            msg.exec();
//            exit(-1);
//        }
//    }
    InitWindow();
    m_bDragging = false;
    QDesktopWidget *desktop=QApplication::desktop();
    int w=desktop->width();
    int h=desktop->height();

    move((w-this->width())/2,(h-this->height())/2);
}

MainWindow::~MainWindow()
{

}

void MainWindow::InitWindow()
{
    //m_nFunctions = 0x37;
    //mAppConfig->SetFunctions(0x63);
    m_nFunctions = mAppConfig->GetFunctions();
    qDebug("MainWindow::InitWindow() Functions:0x%02X", m_nFunctions);

    m_pScreenConf = new GlobalConf;
    m_pTopWidget = new TopWidget(this); //m_pTopWidget height is 36
    m_pLeftWidget = new LeftWidget(this, m_pScreenConf, m_nFunctions, IsCenterLess());  //m_pLeftWidget height is 484
    m_pRightWidget = new RightWidget(this);

    m_pTopWidget->setGeometry(0,0,width(),m_pTopWidget->height());
    m_pLeftWidget->setFixedHeight(APP_SCREEN_HEIGHT-m_pTopWidget->height());
    m_pLeftWidget->setGeometry(0,m_pTopWidget->height(),m_pLeftWidget->width(),APP_SCREEN_HEIGHT-m_pTopWidget->height());
    m_pRightWidget->setFixedSize(APP_SCREEN_WIDTH - APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT - m_pTopWidget->height());
    m_pRightWidget->setGeometry(APP_SCREEN_LEFT_WIDTH, m_pTopWidget->height(), m_pRightWidget->width(), m_pRightWidget->height());

//    QVBoxLayout *vlayout = new QVBoxLayout;
//    vlayout->setContentsMargins(0,0,0,0);
//    vlayout->addWidget(m_pTopWidget);
//    QHBoxLayout *hlayout = new QHBoxLayout;
//    hlayout->setContentsMargins(0,0,0,0);
//    hlayout->addWidget(m_pLeftWidget);
//    hlayout->addWidget(m_pRightWidget);
//    vlayout->addLayout(hlayout);
//    setLayout(vlayout);
    m_pRightWidget->m_pVDetail->SetMainWindowPtr(this);
    //qDebug("rightwidget w=%d h=%d ", m_pRightWidget->width(), m_pRightWidget->height());

//    QPalette pa;
//    pa.setColor(QPalette::WindowText,QColor(33,175,224));
//    QFont font1("Arial", 10, QFont::Thin, false);



    connect(m_pTopWidget->m_pCloseBtn, SIGNAL(clicked()), this, SLOT(close()));
    //connect(m_pTopWidget->m_pMinBtn, &PushButton::clicked,this,&MainWindow::setMinimum);
    connect(m_pTopWidget->m_pMaxBtn, SIGNAL(clicked()), this, SLOT(OnMaxState()));

    connect(m_pLeftWidget->m_pitlKill, SIGNAL(clicked()), this, SLOT(OnClickedLeftKill()));
    connect(m_pLeftWidget->m_pitlNetDefence, SIGNAL(clicked()), this, SLOT(OnClickedLeftNetDefence()));
    connect(m_pLeftWidget->m_pitlBasicLog, SIGNAL(clicked()), this, SLOT(OnClickedLeftSetup()));

    connect(m_pLeftWidget->m_pitlVirusDetail, SIGNAL(clicked()), this, SLOT(OnClickedLeftVDetail()));
    connect(m_pLeftWidget->m_pitlScanEvent, SIGNAL(clicked()), this, SLOT(OnClickedLeftScanEvent()));
//    connect(m_pLeftWidget->m_pitlAppReinforce, &ImgTxtLabel::clicked, this, &MainWindow::OnClickedLeftAppReinforce);
    connect(m_pLeftWidget->m_pitlIsolation, SIGNAL(clicked()), this, SLOT(OnClickedLeftIsolation()));
    connect(m_pLeftWidget->m_pitlUrlAccess, SIGNAL(clicked()), this, SLOT(OnClickedLeftUrlAccess()));
    connect(m_pLeftWidget->m_pitlIPRule, SIGNAL(clicked()), this, SLOT(OnClickedLeftIPRule()));
    connect(m_pLeftWidget->m_pitlInstallation, SIGNAL(clicked()), this, SLOT(OnClickedLeftInstall()));
    //connect(m_pLeftWidget->m_pitlPlatformLog, SIGNAL(clicked()), this, SLOT(OnClickedLeftSysLog()));

    m_pSystemTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/resource/logo");
    m_pSystemTrayIcon->setIcon(icon);
    QString titleName = QString::fromUtf8("Log");
    m_pSystemTrayIcon->setToolTip(titleName);
    connect(m_pSystemTrayIcon, SIGNAL(activated()), this, SLOT(OnActivatedSysTrayIcon()));
    setWindowIcon(icon);

    QApplication::setApplicationName(titleName);
    setWindowTitle(titleName);
    m_pTopWidget->SetTitle(titleName);

//    connect(m_pRightEndWidget->m_pEndWidget->m_pAdmin->m_pclSetOrChange, &ClickedLabel::clicked, this, &MainWindow::OnChangeAdminPasswd);

//    connect(m_pRightEndWidget, &RightEndWidget::valueChanged, this, &MainWindow::AutoSetFocus);
//    connect(m_pRightTopWidget, &RightTopWidget::valueChanged, this, &MainWindow::AutoSetFocus);
    OnClickedLeftKill();
}

void MainWindow::InitAllScreenData()
{
    m_pRightWidget->SetMainWindowPtr(this);
    m_pRightWidget->InitAllData();
    JumpToStage(m_nFunctionStage);
}

int MainWindow::OnCommandObserver(int event, void *args, void **result)
{
    if (CenterCommandObserver::CLIENT_COMMAND == event) {
        CommandBundle *bundle = (CommandBundle*)args;
        if (bundle) {
            switch (bundle->mCommandID) {
            case CommandBundle::DAEMON_EXIT: {
                this->close();
                break;
            }
            default:
                break;
            }
        }
    }
    return -1;
}

void MainWindow::OnClickedLeftKill()
{
    m_pLeftWidget->SetTopDisable();
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->TopSelectedLabel());
    m_pRightWidget->ShowScreen(m_nTopSelectedIndex);
}

void MainWindow::OnClickedLeftSetup()
{
    m_pLeftWidget->SetEndDisable();
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->EndSelectLabel());
    m_pRightWidget->ShowScreen(m_nEndSelectedIndex);
}

void MainWindow::OnClickedLeftVDetail()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlVirusDetail);
    m_pRightWidget->InitVDetailData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_VDETAIL);
    m_nTopSelectedIndex = LOGVIEW_SCREEN_VDETAIL;
}

void MainWindow::OnClickedLeftScanEvent()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlScanEvent);
    m_pRightWidget->InitScanEventData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_SCANEVENT);
    m_nTopSelectedIndex = LOGVIEW_SCREEN_SCANEVENT;
}

//void MainWindow::OnClickedLeftAppReinforce()
//{
//    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlAppReinforce);
//    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_APPREINFORCE);
//    m_nTopSelectedIndex = LOGVIEW_SCREEN_APPREINFORCE;
//}

void MainWindow::OnClickedLeftIsolation()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlIsolation);
    m_pRightWidget->InitIsolationData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_ISOLATION);
    m_nTopSelectedIndex = LOGVIEW_SCREEN_ISOLATION;
}

void MainWindow::OnClickedLeftNetDefence()
{
    m_pLeftWidget->SetMidDisable();
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->MidSelectedLabel());
    m_pRightWidget->ShowScreen(m_nMidSelectedIndex);
    if(m_nMidSelectedIndex == LOGVIEW_SCREEN_URLACCESS)
        m_pRightWidget->InitUrlAccessData();
}

void MainWindow::OnClickedLeftUrlAccess()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlUrlAccess);
    m_pRightWidget->InitUrlAccessData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_URLACCESS);
    m_nMidSelectedIndex = LOGVIEW_SCREEN_URLACCESS;
}

void MainWindow::OnClickedLeftIPRule()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlIPRule);
    m_pRightWidget->InitIPRuleData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_IPRULE);
    m_nMidSelectedIndex = LOGVIEW_SCREEN_IPRULE;
}

void MainWindow::OnClickedLeftInstall()
{
    m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlInstallation);
    m_pRightWidget->InitInstallData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_INSTALL);
    m_nEndSelectedIndex = LOGVIEW_SCREEN_INSTALL;
}

void MainWindow::OnClickedLeftSysLog()
{
    //m_pLeftWidget->OnClickLabel(m_pLeftWidget->m_pitlPlatformLog);
    m_pRightWidget->InitSysLogData();
    m_pRightWidget->ShowScreen(LOGVIEW_SCREEN_SYSLOG);
    m_nEndSelectedIndex = LOGVIEW_SCREEN_SYSLOG;
}

void MainWindow::setMinimum()
{
    this->hide();
    m_pSystemTrayIcon->show();
}

void MainWindow::OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        m_pSystemTrayIcon->showMessage(QString::fromUtf8("瑞星"), QString::fromUtf8("安全云终端日志查看程序"), QSystemTrayIcon::Information, 2000);
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        m_pSystemTrayIcon->hide();
        break;
    default:
        break;
    }
}

void MainWindow::OnMaxState()
{
    if(m_pTopWidget->m_pMaxBtn->m_bMaxState){
        m_baGeometry = saveGeometry();
        m_OldSize = this->size();
        const QSize MAIN_SIZE_MAX = QApplication::desktop()->size();
        setMaximumSize(MAIN_SIZE_MAX);
        setGeometry(0,0,MAIN_SIZE_MAX.width(), MAIN_SIZE_MAX.height());
        setFixedSize(MAIN_SIZE_MAX);
        m_qsLeftSize = m_pLeftWidget->size();
        m_qsRightSize = m_pRightWidget->size();
        m_pLeftWidget->setFixedHeight(MAIN_SIZE_MAX.height()-36);
        m_pRightWidget->SetMaxSizeState(true);
        m_pRightWidget->setFixedSize(MAIN_SIZE_MAX.width()-m_pLeftWidget->width(), MAIN_SIZE_MAX.height()-36);
        m_pTopWidget->m_pMaxBtn->SetRegainState();
        m_pTopWidget->update();
    }else{
        restoreGeometry(m_baGeometry);
        setFixedSize(m_OldSize);
        m_pLeftWidget->setFixedSize(m_qsLeftSize);
        m_pRightWidget->SetMaxSizeState(false);
        m_pRightWidget->setFixedSize(m_qsRightSize);
        m_pTopWidget->m_pMaxBtn->SetMaxState();
        m_pTopWidget->update();
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_pTopWidget->resize(event->size().width(), 36);
    m_pLeftWidget->resize(m_pLeftWidget->width(), event->size().height()-36);
    m_pRightWidget->resize(event->size().width()-m_pLeftWidget->width(), event->size().height()-36);
    m_pTopWidget->update();
}

int MainWindow::JumpToStage(int function_stage)
{
    switch(function_stage){
    case ENTER_FUNCTION_STAGE_VIRUS_DETAIL:
        OnClickedLeftKill();
        OnClickedLeftVDetail();
        break;
    case ENTER_FUNCTION_STAGE_SCAN_EVENTS:
        OnClickedLeftKill();
        OnClickedLeftScanEvent();
        break;
    case ENTER_FUNCTION_STAGE_ISOLATION:
        OnClickedLeftKill();
        OnClickedLeftIsolation();
        break;
    case ENTER_FUNCTION_STAGE_URL_ACCESS:
        OnClickedLeftNetDefence();
        OnClickedLeftUrlAccess();
        break;
    case ENTER_FUNCTION_STAGE_IP_RULES:
        OnClickedLeftNetDefence();
        OnClickedLeftIPRule();
        break;
    case ENTER_FUNCTION_STAGE_INSTALLATION:
        OnClickedLeftSetup();
        OnClickedLeftInstall();
        break;
    case ENTER_FUNCTION_STAGE_PLATFORM:
        OnClickedLeftSetup();
        OnClickedLeftSysLog();
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

int MainWindow::Construct() {
    return Application::Construct(FOLDER_RJJH_GJCZ, true, RJJH_LOGVIEW_SOCKET);
}
