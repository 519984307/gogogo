#include "common/pubdef.h"
#include "origroup.h"
#include "whitelistgroup.h"
#include "backupgroup.h"
#include "kvgroup.h"
#include "timergroup.h"
#include "filegroup.h"
#include "udiskgroup.h"
#include "systemgroup.h"
#include "appgroup.h"
#include "topscrollwidget.h"
#include "appconfig.h"

TopScrollWidget::TopScrollWidget(QWidget *parent, MainWindow *pMainWindow, int function) : QWidget(parent)
{
    m_pMainWindow = pMainWindow;

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH);

    m_pLayout = new QVBoxLayout;

    m_pOri = new OriGroup(this, m_pMainWindow);
    m_pWhiteList = new WhiteListGroup(this);
    m_pBackup = new BackupGroup(this);
    m_pKV = new KVGroup(this);
    m_pTimer = new TimerGroup(this);
    m_pFile = new FileGroup(this);
    m_pUDisk = new UDiskGroup(this);
//    m_pSystem = new SystemGroup(this);
//    m_pApp = new AppGroup(this);

    m_pLayout->addWidget(m_pOri);
    m_pLayout->addWidget(m_pWhiteList);
    m_pLayout->addWidget(m_pBackup);
    m_pLayout->addWidget(m_pKV);
    m_pLayout->addWidget(m_pTimer);

    if (function & AppConfig::FILE_DEFEN) {
        m_pLayout->addWidget(m_pFile);
    }else{
        m_pFile->hide();
    }
    if (function & AppConfig::PLUGIN_CHECKER) {
        m_pLayout->addWidget(m_pUDisk);
    }else{
        m_pUDisk->hide();
    }
//    if((RAVQT_FUNC_FLAG & 0x40)==0 || (RAVQT_FUNC_FLAG & 0x20)==0){
//        m_pFile->hide();
//        m_pUDisk->hide();
//    }else{
//        m_pLayout->addWidget(m_pFile);
//        m_pLayout->addWidget(m_pUDisk);
//    }

//    m_pLayout->addWidget(m_pSystem);
//    m_pLayout->addWidget(m_pApp);

    setLayout(m_pLayout);
}
