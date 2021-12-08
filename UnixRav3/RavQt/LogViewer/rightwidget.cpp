#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>

#include "rightwidget.h"
#include "virusdetailgroup.h"
#include "scaneventgroup.h"
//#include "sysreinforcegroup.h"
//#include "appreinforcegroup.h"
#include "isolationgroup.h"
#include "urlaccessgroup.h"
#include "iprulegroup.h"
#include "installgroup.h"
#include "sysloggroup.h"
#include "mainwindow.h"

#include "common/pubdef.h"


RightWidget::RightWidget(QWidget *parent) : QWidget(parent),m_bMaxSizeState(false)
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(248,248,248));
    setPalette(palette);
    setAutoFillBackground(true);

    m_pVDetail = new VirusDetailGroup(this);
    m_pVDetail->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pScanEvent = new ScanEventGroup(this);
    m_pScanEvent->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pScanEvent->hide();
//    m_pSysInforce = new SysReinforceGroup(this);
//    m_pSysInforce->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
//    m_pSysInforce->hide();
//    m_pAppInforce = new AppReinforceGroup(this);
//    m_pAppInforce->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
//    m_pAppInforce->hide();
    m_pIsolation = new IsolationGroup(this);
    m_pIsolation->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pIsolation->hide();

    m_pUrlAccess = new UrlAccessGroup(this);
    m_pUrlAccess->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pUrlAccess->hide();
    m_pIPRule = new IPRuleGroup(this);
    m_pIPRule->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pIPRule->hide();

    m_pInstall = new InstallGroup(this);
    m_pInstall->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pInstall->hide();
    m_pSysLog = new SysLogGroup(this);
    m_pSysLog->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
    m_pSysLog->hide();

    m_lScreens.append(m_pVDetail);
    m_lScreens.append(m_pScanEvent);
    m_lScreens.append(m_pIsolation);
    m_lScreens.append(m_pUrlAccess);
    m_lScreens.append(m_pIPRule);
    m_lScreens.append(m_pInstall);
    m_lScreens.append(m_pSysLog);

    //m_pSysLog->SetMainWindow(parent);
}

void RightWidget::ShowScreen(int nIndex)
{
    for(int i=0; i<m_lScreens.size(); i++){
        QWidget * pwidget = m_lScreens.at(i);
        if(i == nIndex){
            pwidget->show();
        }else{
            pwidget->hide();
        }
    }
}

void RightWidget::SetMainWindowPtr(MainWindow* ptr)
{
    m_pMain = ptr;
    m_pVDetail->SetMainWindowPtr(ptr);
    m_pScanEvent->SetMainWindowPtr(ptr);
    m_pIsolation->SetMainWindowPtr(ptr);

    m_pUrlAccess->SetMainWindowPtr(ptr);
    m_pIPRule->SetMainWindowPtr(ptr);

    m_pInstall->SetMainWindowPtr(ptr);
    m_pSysLog->SetMainWindow(ptr);
}

void RightWidget::InitAllData()
{
    m_pVDetail->InitData();
    m_pInstall->InitData();
}

void RightWidget::InitVDetailData()
{
    m_pVDetail->InitData();
}

void RightWidget::InitScanEventData()
{
    m_pScanEvent->InitData();
}

void RightWidget::InitIsolationData()
{
    m_pIsolation->InitData();
}

void RightWidget::InitUrlAccessData()
{
    m_pUrlAccess->InitData();
}

void RightWidget::InitIPRuleData()
{
    m_pIPRule->InitData();
}

void RightWidget::InitInstallData()
{
    m_pInstall->InitData();
}

void RightWidget::InitSysLogData()
{
    m_pSysLog->InitData();
}

void RightWidget::SetMaxSizeState(bool bMaxSize)
{
   m_bMaxSizeState = bMaxSize;
}

void RightWidget::resizeEvent(QResizeEvent *event)
{
    QSize main_size = QApplication::desktop()->size();
    for(int i=0; i<m_lScreens.size(); i++){
        QWidget * pwidget = m_lScreens.at(i);
        if(m_bMaxSizeState){
            pwidget->setGeometry(0,0,main_size.width()-APP_SCREEN_LEFT_WIDTH, main_size.height()-36);
        }else{
            pwidget->setGeometry(0,0,APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, APP_SCREEN_HEIGHT-36);
        }
    }
}
