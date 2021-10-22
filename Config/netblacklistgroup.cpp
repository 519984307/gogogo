#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/pushbutton.h"
#include "blacklistiptable.h"
#include "blacklisturltable.h"

#include "netblacklistgroup.h"


//#define BACKUP_LEFT_LABEL_WIDTH    150
//#define BACKUP_RIGHT_CHKBOX_WIDTH  180

NetBlacklistGroup::NetBlacklistGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"网址黑名单");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(270);
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

    m_pclStartBlacklist = new CheckboxLabel(this, u8"启用黑名单");
    m_pclStartBlacklist->setFont(font);

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


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(20, 15, 10, 10);
    vlayout->setSpacing(0);
    vlayout->addWidget(m_pclStartBlacklist, 0, Qt::AlignLeft);

    QHBoxLayout *h11layout = new QHBoxLayout;
    h11layout->setContentsMargins(0, 0, 0, 0);
    h11layout->addStretch();
    h11layout->addWidget(lblUrlDesc, 0, Qt::AlignRight|Qt::AlignVCenter);
    h11layout->addWidget(m_ppbUrlAdd, 0, Qt::AlignRight|Qt::AlignVCenter);

    QHBoxLayout *h12layout = new QHBoxLayout;
    h12layout->setContentsMargins(0, 0, 0, 0);
    h12layout->addStretch();
    h12layout->addWidget(lblIpDesc, 0, Qt::AlignRight|Qt::AlignVCenter);
    h12layout->addWidget(m_ppbIpAdd, 0, Qt::AlignRight|Qt::AlignVCenter);

    QHBoxLayout *h1layout = new QHBoxLayout;
    h1layout->setContentsMargins(0, 0, 0, 0);
    h1layout->addLayout(h11layout);
    h1layout->addLayout(h12layout);

    vlayout->addLayout(h1layout);

    QHBoxLayout *h2layout = new QHBoxLayout;
    h2layout->setContentsMargins(0, 0, 0, 0);
    h2layout->addWidget(m_pUrlTable, 0, Qt::AlignVCenter);
    h2layout->addWidget(m_pIPTable, 0, Qt::AlignVCenter);

    vlayout->addLayout(h2layout);

    setLayout(vlayout);

    //m_pIPTable

//    connect(m_pclProcCompressed, &CheckboxLabel::clicked, this, &NetBlacklistGroup::SetSizeEditValid);
    connect(m_ppbIpAdd, SIGNAL(clicked()), this, SLOT(OnClickedIpAdd()));
    connect(m_ppbUrlAdd, SIGNAL(clicked()), this, SLOT(OnClickedUrlAdd()));
    SetFocus(m_bFocus);
}

void NetBlacklistGroup::SetFocus(bool bFocus)
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

void NetBlacklistGroup::SetSizeEditValid()
{
//    if(m_pclProcCompressed->m_bChecked == false){
//        m_pleSize->setDisabled(true);
//    }else{
//        m_pleSize->setDisabled(false);
//    }

}

void NetBlacklistGroup::OnClickedIpAdd()
{
    m_pIPTable->AddIpNameStart();
}

void NetBlacklistGroup::OnClickedUrlAdd()
{
    m_pUrlTable->AddUrlNameStart();
}
