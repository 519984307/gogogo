#include <QVBoxLayout>
#include <QHBoxLayout>


#include "common.h"
#include "pubfunc.h"
#include "common/pushbutton.h"
#include "blacklistiptable.h"
#include "blacklisturltable.h"
#include "defendcenteritems.h"
#include "defendcentermidwidget.h"
#include "mainwindow.h"

#include "Entry/commonentry.h"
#include "Entry/urlruleentry.h"
#include "scanconfig.h"




DefendCenterMidWidget::DefendCenterMidWidget(MainWindow *pMainWindow, QWidget *parent) : QWidget(parent)
{
    QPalette pal(palette());

    pal.setColor(QPalette::Background, QColor(250,250,250));
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH-2, DEFENDCENTER_MID_WIDGET_HEIGHT);

    m_pMainWindow = pMainWindow;

    m_plIcon = new QLabel;
    QPixmap picon(":/resource/defendcenter_sub_title");
    m_plIcon->setFixedSize(picon.width(), picon.height()/3);
    m_plIcon->setPixmap(picon.copy(0, picon.height()/3*2, picon.width(), picon.height()/3));

    QLabel *lblUrlDesc = new QLabel(u8"URL地址：", this);
    lblUrlDesc->adjustSize();
    m_ppbUrlAdd = new PushButton(this);
    m_ppbUrlAdd->loadPixmap(":/resource/plus_btn");
    QLabel *lblIpDesc = new QLabel(u8"IP地址：", this);
    lblIpDesc->adjustSize();
    m_ppbIpAdd = new PushButton(this);
    m_ppbIpAdd->loadPixmap(":/resource/plus_btn");

    m_pUrlTable = new BlacklistUrlTable(this);
    m_pIPTable = new BlacklistIPTable(this);

    m_ppbApply = new QPushButton();
    m_ppbApply->setStyleSheet("QPushButton {\
                  border: 0px transparent;\
                  padding: 0px;\
                  image: url(:/resource/apply_btn_normal);\
              }\
              QPushButton:hover {\
                  image: url(:/resource/apply_btn_hovered);\
              }\
              QPushButton:pressed {\
                  image: url(:/resource/apply_btn_down);\
              }\
            QPushButton:disabled {\
                image: url(:/resource/apply_btn_disabled);\
            }\
              QPushButton:flat {\
                  image: url(:/resource/apply_btn_disabled);\
              }\
              QPushButton:default {\
                  border-color: navy; \
              }");
    QPixmap img(":/resource/apply_btn_normal");
    m_ppbApply->setFixedSize(img.width(), img.height());


    //vlayout->setSpacing(0);
    //vlayout->addWidget(m_pclStartBlacklist, 0, Qt::AlignLeft);

    QHBoxLayout *h11layout = new QHBoxLayout;
    h11layout->setContentsMargins(30, 0, 30, 0);
    h11layout->addStretch();
    h11layout->addWidget(lblUrlDesc, 0, Qt::AlignRight|Qt::AlignVCenter);
    h11layout->addWidget(m_ppbUrlAdd, 0, Qt::AlignRight|Qt::AlignVCenter);

    QHBoxLayout *h12layout = new QHBoxLayout;
    h12layout->setContentsMargins(30, 0, 30, 0);
    h12layout->addStretch();
    h12layout->addWidget(lblIpDesc, 0, Qt::AlignRight|Qt::AlignVCenter);
    h12layout->addWidget(m_ppbIpAdd, 0, Qt::AlignRight|Qt::AlignVCenter);

    QHBoxLayout *h1layout = new QHBoxLayout;
    h1layout->setContentsMargins(0, 0, 0, 0);
    h1layout->addLayout(h11layout);
    h1layout->addLayout(h12layout);

    QHBoxLayout *h2layout = new QHBoxLayout;
    h2layout->setContentsMargins(30, 0, 30, 0);
    h2layout->addWidget(m_pUrlTable, 0, Qt::AlignVCenter);
    h2layout->addWidget(m_pIPTable, 0, Qt::AlignVCenter);

    connect(m_ppbIpAdd, SIGNAL(clicked(bool)), this, SLOT(OnClickedIpAdd()));
    connect(m_ppbUrlAdd, SIGNAL(clicked(bool)), this, SLOT(OnClickedUrlAdd()));


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->addWidget(m_plIcon, 0, Qt::AlignLeft);
    vlayout->addLayout(h1layout);
    vlayout->addLayout(h2layout);

    QHBoxLayout * h3layout = new QHBoxLayout;
    h3layout->setContentsMargins(0, 0, 0, 0);
    h3layout->addWidget(m_ppbApply, 0, Qt::AlignRight);
    h3layout->addSpacing(40);
    vlayout->addLayout(h3layout);

    setLayout(vlayout);
}

void DefendCenterMidWidget::InitTable()
{
    std::list<UrlRuleEntry> urllist;
    m_pMainWindow->GetUrlBlackList(urllist);
    std::list<UrlRuleEntry>::iterator itrule = urllist.begin();
    while(itrule != urllist.end()){
        UrlRuleEntry rule = *itrule;
//        if(rule.mCategory != UrlRuleEntry::NETFILTER_ONLY){
//            itrule++;
//            continue;
//        }
        if(rule.mType == UrlRuleEntry::IP_ADDR && rule.mCategory == UrlRuleEntry::BLOCK_ALL){
            //qDebug("DefendCenterMidWidget::InitTable Add IP:%s", rule.mPath.c_str());
            m_pIPTable->AddItem(QString::fromStdString(rule.mPath));
        }else if(rule.mType == UrlRuleEntry::URL_MATCH && rule.mCategory == UrlRuleEntry::BLOCK_ALL){
            //qDebug("DefendCenterMidWidget::InitTable Add URL:%s", rule.mPath.c_str());
            m_pUrlTable->AddItem(QString::fromStdString(rule.mPath));
        }
        itrule++;
    }
}

void DefendCenterMidWidget::SetMainWindow(MainWindow *pMainWindow)
{
    m_pMainWindow = pMainWindow;
}

void DefendCenterMidWidget::OnClickedIpAdd()
{
    m_pIPTable->AddIpNameStart();
}

void DefendCenterMidWidget::OnClickedUrlAdd()
{
    m_pUrlTable->AddUrlNameStart();
}

