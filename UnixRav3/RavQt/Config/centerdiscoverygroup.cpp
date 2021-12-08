#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "common/pubdef.h"
#include "common/centerdiscoverytable.h"
#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/combobox.h"
#include "common/weekdayselector.h"
#include "common/pushbutton.h"
#include "common/updatesourcetable.h"
#include "centerdiscoverygroup.h"


CenterDiscoveryGroup::CenterDiscoveryGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"中心发现");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(250);
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

    QRegExp rx("^([0-1][0-9]|2[0-3]):([0-5][0-9])$");
    m_pValidatorTime = new QRegExpValidator(rx, this);
    //QRegExp rx_num("[0-9\*]+$");
    QRegExp rx_num("^\\d+$");

    lTableName = new QLabel(u8"服务器列表", this);
    lTableName->setFont(font);
    lTableName->adjustSize();
    m_pTable = new CenterDiscoveryTable(this);
    m_pbAddBtn = new PushButton(this);
    m_pbAddBtn->loadPixmap(":/resource/plus_btn");

//    lConnectType = new QLabel(u8"网络重连方式：", this);
//    lConnectType->setFont(font);
//    lConnectType->adjustSize();
//    m_pcbConnectType = new ComboBox();
//    m_pcbConnectType->setFont(font);
//    m_pcbConnectType->insertItem(0, u8"实时连接");
//    m_pcbConnectType->insertItem(1, u8"5分钟");
//    m_pcbConnectType->insertItem(2, u8"30分钟");
//    m_pcbConnectType->insertItem(3, u8"1小时");
//    m_pcbConnectType->insertItem(4, u8"2小时");
//    m_pcbConnectType->insertItem(5, u8"4小时");
//    m_pcbConnectType->insertItem(6, u8"8小时");
//    m_pcbConnectType->adjustSize();
//    lUploadBandwidthLimit = new QLabel(u8"上传带宽限制：", this);
//    lUploadBandwidthLimit->setFont(font);
//    lUploadBandwidthLimit->adjustSize();
//    m_pcbUploadBandwidthLimit = new ComboBox();
//    m_pcbUploadBandwidthLimit->setFont(font);
//    m_pcbUploadBandwidthLimit->insertItem(0, u8"不限制");
//    m_pcbUploadBandwidthLimit->insertItem(1, u8"10KB");
//    m_pcbUploadBandwidthLimit->insertItem(2, u8"100KB");
//    m_pcbUploadBandwidthLimit->insertItem(3, u8"200KB");
//    m_pcbUploadBandwidthLimit->insertItem(4, u8"500KB");

//    lValidTime = new QLabel(u8"生效时间：", this);
//    lValidTime->setFont(font);
//    lValidTime->adjustSize();
//    m_pleValidTimeStart = new QLineEdit(this);
//    m_pleValidTimeStart->setFont(font);
//    m_pleValidTimeStart->setFixedSize(60, 20);
//    m_pleValidTimeStart->setValidator(new QRegExpValidator(rx, this));
//    m_pleValidTimeStart->setInputMask("99:99");
//    m_pleValidTimeStart->setText("00:00");
//    lSeparate = new QLabel("-", this);
//    lSeparate->setFont(font);
//    lSeparate->adjustSize();
//    m_pleValidTimeEnd = new QLineEdit(this);
//    m_pleValidTimeEnd->setFont(font);
//    m_pleValidTimeEnd->setFixedSize(60, 20);
//    m_pleValidTimeEnd->setValidator(new QRegExpValidator(rx, this));
//    m_pleValidTimeEnd->setInputMask("99:99");
//    m_pleValidTimeEnd->setText("00:00");

//    m_pclUseProxy = new CheckboxLabel(this, u8"启用中心代理");
//    lUseProxyDesc = new QLabel(u8"（网络内未连接中心终端，通过本代理找到中心。）", this);
//    lUseProxyDesc->setFont(font);
//    lUseProxyDesc->adjustSize();
//    lUseProxyDesc->setStyleSheet("color:#9E9E9E;");
//    m_pclOnlyValidedForTerminal = new CheckboxLabel(this, u8"仅对同域终端生效");

    QHBoxLayout *h_source_layout = new QHBoxLayout;
    h_source_layout->setContentsMargins(20, 0, 0, 10);
    h_source_layout->setSpacing(0);
    h_source_layout->addWidget(lTableName, 0, Qt::AlignLeft|Qt::AlignBottom);
    h_source_layout->addStretch();
    h_source_layout->addWidget(m_pbAddBtn, 0, Qt::AlignRight|Qt::AlignBottom);
    h_source_layout->addSpacing(10);

    QHBoxLayout *h_table_layout = new QHBoxLayout;
    h_table_layout->setContentsMargins(20, 0, 0, 10);
    h_table_layout->setSpacing(0);
    h_table_layout->addWidget(m_pTable, 0, Qt::AlignTop);
    h_table_layout->addSpacing(10);

//    QHBoxLayout *h_conn_type_layout = new QHBoxLayout;
//    h_conn_type_layout->setContentsMargins(20, 0, 0, 10);
//    h_conn_type_layout->setSpacing(0);
//    h_conn_type_layout->addWidget(lConnectType, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_conn_type_layout->addWidget(m_pcbConnectType, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_conn_type_layout->addStretch();

//    QHBoxLayout *h_upbandwidth_layout = new QHBoxLayout;
//    h_upbandwidth_layout->setContentsMargins(20, 0, 0, 10);
//    h_upbandwidth_layout->setSpacing(0);
//    h_upbandwidth_layout->addWidget(lUploadBandwidthLimit, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_upbandwidth_layout->addWidget(m_pcbUploadBandwidthLimit, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_upbandwidth_layout->addSpacing(20);
//    h_upbandwidth_layout->addWidget(lValidTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_upbandwidth_layout->addWidget(m_pleValidTimeStart, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_upbandwidth_layout->addSpacing(5);
//    h_upbandwidth_layout->addWidget(lSeparate, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_upbandwidth_layout->addSpacing(5);
//    h_upbandwidth_layout->addWidget(m_pleValidTimeEnd, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_upbandwidth_layout->addStretch();

//    QHBoxLayout *h_proxy_layout = new QHBoxLayout;
//    h_proxy_layout->setContentsMargins(20, 0, 0, 10);
//    h_proxy_layout->setSpacing(0);
//    h_proxy_layout->addWidget(m_pclUseProxy, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_layout->addWidget(lUseProxyDesc);
//    h_proxy_layout->addStretch();

//    QHBoxLayout *h_validforterminal_layout = new QHBoxLayout;
//    h_validforterminal_layout->setContentsMargins(20, 0, 0, 10);
//    h_validforterminal_layout->addSpacing(30);
//    h_validforterminal_layout->addWidget(m_pclOnlyValidedForTerminal, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_validforterminal_layout->addStretch();

    QVBoxLayout *v_total_layout = new QVBoxLayout;
    v_total_layout->setContentsMargins(0, 0, 0, 0);
    v_total_layout->setSpacing(0);
    v_total_layout->addSpacing(10);
    v_total_layout->addLayout(h_source_layout);
    v_total_layout->addLayout(h_table_layout);
//    v_total_layout->addLayout(h_conn_type_layout);
//    v_total_layout->addLayout(h_upbandwidth_layout);
//    v_total_layout->addLayout(h_proxy_layout);
//    v_total_layout->addLayout(h_validforterminal_layout);

    setLayout(v_total_layout);

//    connect(m_pleValidTimeStart, SIGNAL(textChanged(const QString&)), this, SLOT(OnLineEditTextChanged(QString)));
//    connect(m_pleValidTimeEnd, SIGNAL(textChanged(const QString&)), this, SLOT(OnLineEditTextChanged(QString)));
    connect(m_pbAddBtn, SIGNAL(clicked()), this, SLOT(AddCenterClicked()));

    SetFocus(m_bFocus);
}

void CenterDiscoveryGroup::AddCenterClicked()
{
    m_pTable->AddCenterStart();
}

void CenterDiscoveryGroup::SetFocus(bool bFocus)
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

void CenterDiscoveryGroup::OnLineEditTextChanged(const QString &text)
{
    int pos;
    QString stext = text;
    if(text.size() >= 5) {
        if (m_pValidatorTime->validate(stext, pos) != QValidator::Acceptable) {
            //QMessageBox::information(this, "Error", u8"无效的时间字串。", QMessageBox::Ok, QMessageBox::Ok);
            qDebug() << "CenterDiscoveryGroup::OnLineEditTextChanged() text=" << text << endl;
        }
    }
}
