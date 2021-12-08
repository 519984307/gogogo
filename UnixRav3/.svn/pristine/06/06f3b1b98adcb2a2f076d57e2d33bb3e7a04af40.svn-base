#include "screenwidget.h"
#include "resource.h"
#include "popscreen.h"
#include "communicatebundle.h"
#include "commonlocker.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "appconfig.h"
#include "notifycontroller.h"

#include <unistd.h>
#include <QToolTip>
#include <stdlib.h>

ScreenWidget::ScreenWidget(AppConfig *config, QWidget *parent)
                        : QWidget(parent), mObserver(NULL), mAppConfig(config) {
    InitTrayIcon();
    InitMenu();

    mPopWidget = new PopScreen(this);
    mCondLocker = CondLocker::NEW();

    connect(this, SIGNAL(tip()), this, SLOT(ShowMessage()));
    connect(mPopWidget, SIGNAL(end(int)), this, SLOT(onPopEnd(int)));
    connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(IconActived(QSystemTrayIcon::ActivationReason)));
}

ScreenWidget::~ScreenWidget() {
    if (mCondLocker) {
        delete mCondLocker;
    }
    if (mPopWidget) {
        delete mPopWidget;
    }
    if (mTrayIcon) {
        delete mTrayIcon;
    }
    if (mMenu) {
        delete mMenu;
    }
}

int ScreenWidget::InitMenu() {
    mMenu = new QMenu;
    QAction *openAction = new QAction(mMenu);
    openAction->setText(MENU_OPEN_APP);

//    QAction *quickAction = new QAction(mMenu);
//    quickAction->setText(MENU_QUICK_SCAN);

    QAction *setAction = new QAction(mMenu);
    setAction->setText(MENU_SETTING);

    QAction *logAction = new QAction(mMenu);
    logAction->setText(MENU_LOGS);


//    QAction *updateAction = new QAction(mMenu);
//    updateAction->setText(MENU_UPGRADE);

//    QAction *exitAction = new QAction(mMenu);
//    exitAction->setText(MENU_EXIT_APP);

    mMenu->addAction(openAction);
//    mMenu->addAction(quickAction);

    if (mAppConfig->GetSubProductGUID() == SECRET_MACHINE_ID) {
        // 保密机中只有管理员帐户才能看日志
        if (Utils::OS::GetUserFromUID(getuid()) == "secadm") {
            mMenu->addAction(logAction);
            mMenu->addAction(setAction);
        }
    }
    else {
        mMenu->addAction(logAction);
        mMenu->addAction(setAction);
    }
//    mMenu->addAction(updateAction);
//    mMenu->addAction(exitAction);

    connect(openAction,SIGNAL(triggered(bool)), this, SLOT(OpenMainScreen()));
    connect(setAction,SIGNAL(triggered(bool)), this, SLOT(OpenSettings()));
    connect(logAction,SIGNAL(triggered(bool)), this, SLOT(OpenLogView()));

//    connect(updateAction,SIGNAL(triggered(bool)), this, SLOT(Update()));
//    connect(exitAction,SIGNAL(triggered(bool)), this, SLOT(ExitAll()));

    return 0;
}

int ScreenWidget::InitTrayIcon() {
    mTrayIcon = new QSystemTrayIcon;
#ifdef SECRET_MACHINE
    mTrayIcon->setToolTip("瑞星ESM防病毒系统V3.0");
#endif
    mTrayIcon->setIcon(QIcon(":/images/rising"));
    return 0;
}

void ScreenWidget::SetTrayIcon(bool state) {
    if (mTrayIcon) {
        if (state) {
            mTrayIcon->setContextMenu(mMenu);
            mTrayIcon->show();
        }
        else {
            mTrayIcon->setContextMenu(NULL);
            mTrayIcon->hide();
        }
    }
}

void ScreenWidget::Start(bool tray) {    
    if (tray) {
        mTrayIcon->setContextMenu(mMenu);
        mTrayIcon->show();
    }
}

void ScreenWidget::ShowTips(CommunicateBundle& bundle) {
//    QToolTip::showText(QPoint(100,100), content);
    LOG_RJJH("qtsupport receive the bunld %s", bundle.mContent.c_str());
    mCommunicateBundle = bundle;
    emit tip();

    mCondLocker->Wait();
}

void ScreenWidget::paintEvent(QPaintEvent *event) {

}

void ScreenWidget::ShowMessage() {
    LOG_RJJH("received the tip signal");
    if (!mCommunicateBundle.mContent.empty()) {
        LOG_RJJH("content is %s", mCommunicateBundle.mContent.c_str());
        mPopWidget->showMessage(
                    mCommunicateBundle.mFlag,
                    mCommunicateBundle.mTitle.c_str(),
                    mCommunicateBundle.mType,
                    mCommunicateBundle.mContent.c_str(),
                    mCommunicateBundle.mButtonTitle2.c_str(),
                    mCommunicateBundle.mButtonTitle1.c_str());
    }
}

void ScreenWidget::onPopEnd(int selected) {
    LOG_RJJH("received the end signal, close it");
    mCommunicateBundle.mSelected = selected;
    mCondLocker->Signal();
}

void ScreenWidget::OpenMainScreen() {
    if (mAppConfig) {
        string command;
        if (Utils::Path::FileAvailed(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),DEPLOYED_BIN_FOLDER,MAIN_SCREEN_COMMAN, NULL))) {
            command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),DEPLOYED_BIN_FOLDER,MAIN_SCREEN_COMMAN, NULL);
        }
        else if (Utils::Path::FileAvailed(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),MAIN_SCREEN_COMMAN, NULL))) {
            command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),MAIN_SCREEN_COMMAN, NULL);
        }
        system(command.c_str());
    }
}

void ScreenWidget::OpenSettings() {
    if (mAppConfig) {
        string command;
        if (Utils::Path::FileAvailed(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),DEPLOYED_BIN_FOLDER,MAIN_SETTING_COMMAND, NULL))) {
            command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),DEPLOYED_BIN_FOLDER,MAIN_SETTING_COMMAND, NULL);
        }
        else if (Utils::Path::FileAvailed(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),MAIN_SETTING_COMMAND, NULL))) {
            command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),MAIN_SETTING_COMMAND, NULL);
        }
        system(command.c_str());
    }
}

void ScreenWidget::OpenLogView() {
    if (mAppConfig) {
        string command;
        if (Utils::Path::FileAvailed(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),DEPLOYED_BIN_FOLDER,MAIN_LOGVIEW_COMMAND, NULL))) {
            command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),DEPLOYED_BIN_FOLDER,MAIN_LOGVIEW_COMMAND, NULL);
        }
        else if (Utils::Path::FileAvailed(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),MAIN_LOGVIEW_COMMAND, NULL))) {
            command = Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(),MAIN_LOGVIEW_COMMAND, NULL);
        }
        system(command.c_str());
    }
}

void ScreenWidget::Update() {

}

void ScreenWidget::ExitAll() {
//    if (mObserver) {
//        mObserver->OnWidgetEvent(WidgetObserver::MENU_EVENT, WidgetObserver::EXIT_ALL, NULL);
//    }
    this->close();
}

void ScreenWidget::IconActived(QSystemTrayIcon::ActivationReason reson) {
    switch(reson) {
    case QSystemTrayIcon::DoubleClick:
        OpenMainScreen();
        break;
    }
}

