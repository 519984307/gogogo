#include <QPainter>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>

#include "common.h"
#include "common/pushbutton.h"
#include "pubfunc.h"
#include "defendcenteritems.h"
#include "blacklistiptable.h"
#include "blacklisturltable.h"
#include "defendcenterheaderwidget.h"
#include "defendcentertitlewidget.h"
#include "defendcentertopwidget.h"
#include "defendcentermidwidget.h"
#include "defendcenterbotwidget.h"

#include "defendcenterdialog.h"

#include "mainwindow.h"
#include "Entry/commonentry.h"
#include "communicatebundle.h"
#include "appconfig.h"


DefendCenterDialog::DefendCenterDialog(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH, APP_DEFEND_CENTER_DIALOG_HEIGHT);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags|Qt::FramelessWindowHint|Qt::Popup);
    m_pMainWindow = (MainWindow*)parent;
    m_pHeaderWidget = new DefendCenterHeaderWidget(this);
    //m_pTitleWidget  = new DefendCenterTitleWidget;
    m_pTopWidget    = new DefendCenterTopWidget(this, m_pMainWindow->m_nFunctions);
    m_pMidWidget    = new DefendCenterMidWidget(m_pMainWindow, this);
    //m_pBotWidget    = new DefendCenterBotWidget;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);
    layout->addWidget(m_pHeaderWidget);
    //layout->addWidget(m_pTitleWidget);
    if ((0 == (m_pMainWindow->m_nFunctions & AppConfig::FILE_DEFEN)) && (0 == (m_pMainWindow->m_nFunctions & AppConfig::PLUGIN_CHECKER))) {
        m_pTopWidget->hide();
        setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH, APP_DEFEND_CENTER_DIALOG_HEIGHT-210);
    }else{
        layout->addWidget(m_pTopWidget);
    }
    if (0 == (m_pMainWindow->m_nFunctions & AppConfig::NET_FILTER)) {
        m_pMidWidget->hide();
        setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH, APP_DEFEND_CENTER_DIALOG_HEIGHT-270);
    }else{
        layout->addWidget(m_pMidWidget);
    }
   // layout->addWidget(m_pBotWidget);

    setLayout(layout);

    connect(m_pHeaderWidget->m_pbtnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    connect(m_pTopWidget->m_pItemFile, SIGNAL(clicked()), this, SLOT(OnClickedItem()));
//    connect(m_pTopWidget->m_pItemMail, &DefendCenterItem::clicked, this, &DefendCenterDialog::OnClickedItem);
//    connect(m_pTopWidget->m_pItemShare, &DefendCenterItem::clicked, this, &DefendCenterDialog::OnClickedItem);
    connect(m_pTopWidget->m_pItemUDisk, SIGNAL(clicked()), this, SLOT(OnClickedItem()));
//    connect(m_pMidWidget->m_pItemFeiKe, &DefendCenterItem::clicked, this, &DefendCenterDialog::OnClickedItem);
//    connect(m_pMidWidget->m_pItemYuYun, &DefendCenterItem::clicked, this, &DefendCenterDialog::OnClickedItem);
//    connect(m_pMidWidget->m_pItemWeiKe, &DefendCenterItem::clicked, this, &DefendCenterDialog::OnClickedItem);
//    connect(m_pMidWidget->m_pItemDll, &DefendCenterItem::clicked, this, &DefendCenterDialog::OnClickedItem);
    connect(m_pMidWidget->m_ppbApply, SIGNAL(clicked(bool)), this, SLOT(OnClickedApply()));

    DefenCenterSetting dconfig;

    m_pMainWindow->GetDefenCenterConfig(dconfig);

    SetMainWindow(m_pMainWindow);

    m_pTopWidget->m_pItemFile->UpdateInfo(dconfig.mFileMonitorState.mValue);
    m_pTopWidget->m_pItemUDisk->UpdateInfo(dconfig.mUSBPluginMonitorState.mValue);

}

void DefendCenterDialog::SetMainWindow(MainWindow *pMainWindow)
{
    m_pMidWidget->SetMainWindow(pMainWindow);
    m_pMidWidget->InitTable();
}

void DefendCenterDialog::OnClickedItem()
{
    //qDebug("DefendCenterDialog::OnClickedItem occur");
    int opened = 0;
    const QObjectList olt = m_pTopWidget->children();
    int nnum = olt.size();
    for(int i=0; i<nnum; i++){
        QObject *po = olt.at(i);
        if(po->inherits("DefendCenterItem")){
            DefendCenterItem* pdci = (DefendCenterItem*)po;
            if(pdci->m_bEnabled)
                opened++;
            //qDebug("DefendCenterDialog::OnClickedItem: %s", po->objectName().toStdString().c_str());
        }
    }

    DefenCenterSetting dconfig;
    MainWindow *pmain = (MainWindow*)parent();
    pmain->GetDefenCenterConfig(dconfig);


    dconfig.mFileMonitorState.mValue = m_pTopWidget->m_pItemFile->m_bEnabled;
    dconfig.mUSBPluginMonitorState.mValue = m_pTopWidget->m_pItemUDisk->m_bEnabled;


    pmain->UpdateDefenCenterConfig(dconfig);
    pmain->UpdateDefendCenterBtnInfo(opened, 2);
    //m_pMainWindow->ReloadAntiVirusConfig();

//    const QObjectList olm = m_pMidWidget->children();
//    nnum = olm.size();
//    for(int i=0; i<nnum; i++){
//        QObject *po = olm.at(i);
//        if(po->inherits("DefendCenterItem")){
//            DefendCenterItem* pdci = (DefendCenterItem*)po;
//            if(pdci->m_bEnabled)
//                opened++;
//            //qDebug("DefendCenterDialog::OnClickedItem: %s", po->objectName().toStdString().c_str());
//        }
//    }
   // m_pTitleWidget->UpdateInfo(8, opened);
}

bool DefendCenterDialog::InUrlList(list<UrlRuleEntry> &ulist, UrlRuleEntry & uentry)
{
    std::list<UrlRuleEntry>::iterator itrule = ulist.begin();
    while(itrule != ulist.end()){
        UrlRuleEntry rule = *itrule;
        if(uentry.mType == UrlRuleEntry::IP_ADDR && rule.mCategory == UrlRuleEntry::BLOCK_ALL){
            if(rule.mPath == uentry.mPath){
                return true;
            }
        }else if(uentry.mType == UrlRuleEntry::URL_MATCH && rule.mCategory == UrlRuleEntry::BLOCK_ALL){
            if(rule.mPath == uentry.mPath){
                return true;
            }
        }
        itrule++;
    }
    return false;
}

//update blacklist config, file and udisk control to see DefendCenterDialog::OnClickedItem()
void DefendCenterDialog::OnClickedApply()
{
    std::list<UrlRuleEntry> urllist;
    m_pMainWindow->GetUrlBlackList(urllist);
    std::list<UrlRuleEntry>::iterator itrule = urllist.begin();
    while(itrule != urllist.end()){
        UrlRuleEntry rule = *itrule;
        if(rule.mType == UrlRuleEntry::IP_ADDR && rule.mCategory == UrlRuleEntry::BLOCK_ALL){
            if(m_pMidWidget->m_pIPTable->HasIP(QString::fromStdString(rule.mPath)) == false){
                m_pMainWindow->DeleteUrlBlacklist(rule);
            }
        }else if(rule.mType == UrlRuleEntry::URL_MATCH && rule.mCategory == UrlRuleEntry::BLOCK_ALL){
            if(m_pMidWidget->m_pUrlTable->HasUrl(QString::fromStdString(rule.mPath)) == false){
                m_pMainWindow->DeleteUrlBlacklist(rule);
            }
        }
        itrule++;
    }

    std::list<UrlRuleEntry> newlist;
    m_pMainWindow->GetUrlBlackList(newlist);
    QVector<QString*>::iterator itbip = m_pMidWidget->m_pIPTable->m_AllItems.begin();
    while(itbip != m_pMidWidget->m_pIPTable->m_AllItems.end()){
        QString *psip = *itbip;
        if(PubFunc::IsValidIpPort(psip->toStdString().c_str()) == false){
            qDebug("DefendCenterDialog::OnClickedApply find invalid IP address:%s", psip->toStdString().c_str());
            itbip++;
            continue;
        }
        UrlRuleEntry uentry;
        uentry.mType = UrlRuleEntry::IP_ADDR;
        uentry.mPath = psip->toStdString();
        //qDebug("DefendCenterMidWidget::OnClickedApply ip:%s", psip->toStdString().c_str());
        if(InUrlList(newlist, uentry) == false){
//            qDebug("xxxxxx 100 DefendCenterMidWidget::OnClickedApply ip:%s", psip->toStdString().c_str());
            m_pMainWindow->AddUrlBlackList(uentry);
        }
        itbip++;
    }

    itbip = m_pMidWidget->m_pUrlTable->m_AllItems.begin();
    while(itbip != m_pMidWidget->m_pUrlTable->m_AllItems.end()){
        QString *psurl = *itbip;
        UrlRuleEntry uentry;
        uentry.mType = UrlRuleEntry::URL_MATCH;
        uentry.mPath = psurl->toStdString();
//        qDebug("DefendCenterMidWidget::OnClickedApply url:%s", psurl->toStdString().c_str());
        if(InUrlList(newlist, uentry) == false){
            m_pMainWindow->AddUrlBlackList(uentry);
        }
        itbip++;
    }
    m_pMainWindow->ReloadNetfilterConfig();
    close();
}


void DefendCenterDialog::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    painter.drawRect(0, 0, APP_DEFEND_CENTER_DIALOG_WIDTH-1, APP_DEFEND_CENTER_DIALOG_HEIGHT-1);

}


