#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "common/pubdef.h"
#include "upgradegroup.h"
#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/combobox.h"
#include "common/weekdayselector.h"
#include "common/pushbutton.h"
#include "common/updatesourcetable.h"


UpgradeGroup::UpgradeGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"软件更新");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(320);
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

    lUpgradeModeTxt = new QLabel(u8"升级模式：", this);
    lUpgradeModeTxt->setFont(font);
    lUpgradeModeTxt->adjustSize();
    m_pbgUpgradeMode = new QButtonGroup(this);
    m_prbManual = new RadioButton(u8"手动", this);
    m_prbManual->setFont(font);
    m_prbEveryDay = new RadioButton(u8"每天", this);
    m_prbEveryDay->setFont(font);
    m_prbEveryWeek = new RadioButton(u8"每周", this);
    m_prbEveryWeek->setFont(font);
//    m_prbInterval = new RadioButton(u8"间隔", this);
//    m_prbInterval->setFont(font);
    m_pbgUpgradeMode->addButton(m_prbManual, APP_UPGRADE_MODE_MANUAL);
    m_pbgUpgradeMode->addButton(m_prbEveryDay, APP_UPGRADE_MODE_EVERYDAY);
    m_pbgUpgradeMode->addButton(m_prbEveryWeek, APP_UPGRADE_MODE_EVERYWEEK);
//    m_pbgUpgradeMode->addButton(m_prbInterval, APP_UPGRADE_MODE_INTERVAL);
    m_pleDayTime = new QLineEdit(this);
    m_pleDayTime->setFont(font);
    m_pleDayTime->setFixedSize(40, 20);
    m_pleDayTime->setValidator(new QRegExpValidator(rx, this));
    m_pleDayTime->setInputMask("99:99");
    m_pleDayTime->setText("00:00");

    m_pwdlWeekDays = new WeekDaySelector(this);
    m_pleWeekTime = new QLineEdit(this);
    m_pleWeekTime->setFont(font);
    m_pleWeekTime->setFixedSize(40, 20);
    m_pleWeekTime->setValidator(new QRegExpValidator(rx, this));
    m_pleWeekTime->setInputMask("99:99");
    m_pleWeekTime->setText("00:00");
//    m_pleIntervalHours = new QLineEdit(this);
//    m_pleIntervalHours->setFont(font);
//    m_pleIntervalHours->setFixedSize(60, 20);
//    m_pleIntervalHours->setValidator(new QRegExpValidator(rx_num, this));
//    lHourTxt = new QLabel(u8"小时", this);
//    lHourTxt->setFont(font);
//    lHourTxt->adjustSize();
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(25, 20, 0, 20);
    vlayout->setSpacing(5);
    QHBoxLayout *h1layout = new QHBoxLayout;
    h1layout->setContentsMargins(0, 0, 0, 0);
    h1layout->setSpacing(5);
    h1layout->addWidget(lUpgradeModeTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addWidget(m_prbManual, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addSpacing(30);
    h1layout->addWidget(m_prbEveryDay, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addWidget(m_pleDayTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addSpacing(30);
    h1layout->addWidget(m_prbEveryWeek, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addWidget(m_pwdlWeekDays, 0, Qt::AlignVCenter|Qt::AlignVCenter);
    h1layout->addWidget(m_pleWeekTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h1layout->addStretch();
    QHBoxLayout *h2layout = new QHBoxLayout;
    h2layout->setContentsMargins(0, 0, 0, 0);
    h2layout->setSpacing(5);
    h2layout->addSpacing(70);
//    h2layout->addWidget(m_prbInterval, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h2layout->addWidget(m_pleIntervalHours, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h2layout->addWidget(lHourTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
    h2layout->addStretch();
    vlayout->addLayout(h1layout);
    vlayout->addLayout(h2layout);
    vlayout->addSpacing(5);

//    lDelayBootTxt = new QLabel(u8"延时启动：", this);
//    lDelayBootTxt->setFont(font);
//    lDelayBootTxt->adjustSize();
//    m_pbgDelayBoot = new QButtonGroup(this);
//    m_prbDisabledDelayBoot = new RadioButton(u8"禁用", this);
//    m_prbDisabledDelayBoot->setFont(font);
//    m_prbAutoDalayBoot = new RadioButton(u8"自动", this);
//    m_prbAutoDalayBoot->setFont(font);
//    m_prbInRecentMinutesBoot = new RadioButton(u8"在", this);
//    m_prbInRecentMinutesBoot->setFont(font);
//    m_pbgDelayBoot->addButton(m_prbDisabledDelayBoot, APP_DELAY_BOOT_DISABLED);
//    m_pbgDelayBoot->addButton(m_prbAutoDalayBoot, APP_DELAY_BOOT_AUTO);
//    m_pbgDelayBoot->addButton(m_prbInRecentMinutesBoot, APP_DELAY_BOOT_MINUTES_LATER);
//    m_pleRecentMinutesBoot = new QLineEdit(this);
//    m_pleRecentMinutesBoot->setFixedSize(60, 20);
//    m_pleRecentMinutesBoot->setValidator(new QRegExpValidator(rx_num, this));
//    lMinutesLaterTxt = new QLabel(u8"分钟内启动", this);
//    lMinutesLaterTxt->setFont(font);
//    QHBoxLayout *h3layout = new QHBoxLayout;
//    h3layout->setContentsMargins(0, 0, 0, 0);
//    h3layout->setSpacing(5);
//    h3layout->addWidget(lDelayBootTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addWidget(m_prbDisabledDelayBoot, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addSpacing(40);
//    h3layout->addWidget(m_prbAutoDalayBoot, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addSpacing(40);
//    h3layout->addWidget(m_prbInRecentMinutesBoot, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addWidget(m_pleRecentMinutesBoot, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addWidget(lMinutesLaterTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h3layout->addStretch();
//    vlayout->addLayout(h3layout);
//    vlayout->addSpacing(5);

    lUpgradeType = new QLabel(u8"升级内容：", this);
    lUpgradeType->setFont(font);
    lUpgradeType->adjustSize();
    m_pbgUpgradeType = new QButtonGroup(this);
    m_prbUpgradeAll = new RadioButton(u8"升级所有组件", this);
    m_prbUpgradeVirusDB = new RadioButton(u8"仅升级病毒库", this);
    m_pbgUpgradeType->addButton(m_prbUpgradeAll, APP_UPGRADE_ALL);
    m_pbgUpgradeType->addButton(m_prbUpgradeVirusDB, APP_UPGRADE_VIRUSDB);
    m_prbUpgradeAll->setFont(font);
    m_prbUpgradeVirusDB->setFont(font);

//    m_pclRealtimeCheckVDBVersion = new CheckboxLabel(this, u8"允许实时检查病毒库新版本");

    QHBoxLayout *hcontentlayout = new QHBoxLayout;
    hcontentlayout->setContentsMargins(0, 0, 0, 0);
    hcontentlayout->setSpacing(5);
    hcontentlayout->addWidget(lUpgradeType, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hcontentlayout->addWidget(m_prbUpgradeAll, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hcontentlayout->addWidget(m_prbUpgradeVirusDB, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hcontentlayout->addStretch();

//    QHBoxLayout *h_realtime_layout = new QHBoxLayout;
//    h_realtime_layout->setContentsMargins(0, 0, 0, 0);
//    h_realtime_layout->setSpacing(5);
//    h_realtime_layout->addSpacing(70);
//    h_realtime_layout->addWidget(m_pclRealtimeCheckVDBVersion, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_realtime_layout->addStretch();
    vlayout->addLayout(hcontentlayout);
//    vlayout->addLayout(h_realtime_layout);
    vlayout->addSpacing(5);

//    lBufferReserve = new QLabel(u8"缓存空间：",this);
//    lBufferReserve->adjustSize();
//    m_pclCleaningSysTemp = new CheckboxLabel(this, u8"升级时，自动尝试清理系统临时目录");
//    QHBoxLayout *h_buffer_space_layout = new QHBoxLayout;
//    h_buffer_space_layout->setContentsMargins(0, 0, 0, 0);
//    h_buffer_space_layout->setSpacing(10);
//    h_buffer_space_layout->addWidget(lBufferReserve, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_buffer_space_layout->addWidget(m_pclCleaningSysTemp, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_buffer_space_layout->addStretch();
//    vlayout->addLayout(h_buffer_space_layout);
//    vlayout->addSpacing(5);

//    lForceUpdateTxt = new QLabel(u8"强制更新：", this);
//    lForceUpdateTxt->setFont(font);
//    lForceUpdateTxt->adjustSize();
//    m_pclForceUpgrade = new CheckboxLabel(this, u8"在");
//    m_pleAfterDaysUpgrade = new QLineEdit(this);
//    m_pleAfterDaysUpgrade->setFixedSize(60, 20);
//    m_pleAfterDaysUpgrade->setValidator(new QRegExpValidator(rx_num, this));
//    lDaysLaterTxt = new QLabel(u8"天后强制升级",this);
//    lDaysLaterTxt->setFont(font);
//    lDaysLaterTxt->adjustSize();
//    QHBoxLayout *h_forceupgrade_layout = new QHBoxLayout;
//    h_forceupgrade_layout->setContentsMargins(0, 0, 0, 0);
//    h_forceupgrade_layout->setSpacing(5);
//    h_forceupgrade_layout->addWidget(lForceUpdateTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_forceupgrade_layout->addWidget(m_pclForceUpgrade, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_forceupgrade_layout->addWidget(m_pleAfterDaysUpgrade, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_forceupgrade_layout->addWidget(lDaysLaterTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_forceupgrade_layout->addStretch();
//    vlayout->addLayout(h_forceupgrade_layout);
//    vlayout->addSpacing(5);

//    lProxySetTxt = new QLabel(u8"代理设置：", this);
//    lProxySetTxt->setFont(font);
//    lProxySetTxt->adjustSize();
//    m_pcbProxyType = new ComboBox(this);
//    m_pcbProxyType->setFont(font);
//    m_pcbProxyType->insertItem(0, u8"直接连接");
//    m_pcbProxyType->insertItem(1, u8"使用浏览器设置");
//    m_pcbProxyType->insertItem(2, u8"使用代理设置");
//    m_pcbProxyType->adjustSize();
//    lAddressTxt = new QLabel(u8"地址：", this);
//    lAddressTxt->adjustSize();
//    m_pleAddress = new QLineEdit(this);
//    m_pleAddress->setFont(font);
//    m_pleAddress->setFixedSize(96, 20);
//    lPortTxt = new QLabel(u8"端口：", this);
//    lPortTxt->adjustSize();
//    m_plePort = new QLineEdit(this);
//    m_plePort->setFont(font);
//    m_plePort->setFixedSize(96, 20);
//    m_plePort->setValidator(new QRegExpValidator(rx_num, this));
//    lUserTxt = new QLabel(u8"账号：", this);
//    lUserTxt->adjustSize();
//    m_pleUser = new QLineEdit(this);
//    m_pleUser->setFont(font);
//    m_pleUser->setFixedSize(96, 20);
//    lPasswdTxt = new QLabel(u8"密码：", this);
//    lPasswdTxt->adjustSize();
//    m_plePassword = new QLineEdit(this);
//    m_plePassword->setFont(font);
//    m_plePassword->setFixedSize(96, 20);
//    QHBoxLayout *h_proxy_layout = new QHBoxLayout;
//    h_proxy_layout->setContentsMargins(0, 0, 0, 0);
//    h_proxy_layout->setSpacing(5);
//    h_proxy_layout->addWidget(lProxySetTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_layout->addWidget(m_pcbProxyType, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_layout->addStretch();
//    QHBoxLayout *h_proxy_addr_layout = new QHBoxLayout;
//    h_proxy_addr_layout->setContentsMargins(0, 0, 0, 0);
//    h_proxy_addr_layout->setSpacing(5);
//    h_proxy_addr_layout->addSpacing(70);
//    h_proxy_addr_layout->addWidget(lAddressTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_addr_layout->addWidget(m_pleAddress, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_addr_layout->addWidget(lPortTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_addr_layout->addWidget(m_plePort, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_addr_layout->addStretch();
//    QHBoxLayout *h_proxy_user_layout = new QHBoxLayout;
//    h_proxy_user_layout->setContentsMargins(0, 0, 0, 0);
//    h_proxy_user_layout->setSpacing(5);
//    h_proxy_user_layout->addSpacing(70);
//    h_proxy_user_layout->addWidget(lUserTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_user_layout->addWidget(m_pleUser, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_user_layout->addWidget(lPasswdTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_user_layout->addWidget(m_plePassword, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_proxy_user_layout->addStretch();
//    vlayout->addLayout(h_proxy_layout);
//    vlayout->addLayout(h_proxy_addr_layout);
//    vlayout->addLayout(h_proxy_user_layout);
//    vlayout->addSpacing(5);

//    lBandWidthLimitTxt = new QLabel(u8"带宽限制：", this);
//    lBandWidthLimitTxt->adjustSize();
//    m_pclBandWidthLimit = new CheckboxLabel(this, u8"开启");
//    m_pleLimitedBandWidth = new QLineEdit(this);
//    m_pleLimitedBandWidth->setFixedSize(80, 20);
//    m_pleLimitedBandWidth->setValidator(new QRegExpValidator(rx_num, this));
//    m_pleBandWidthLimitValidStartTime = new QLineEdit(this);
//    m_pleBandWidthLimitValidStartTime->setFixedSize(40, 20);
//    m_pleBandWidthLimitValidStartTime->setValidator(new QRegExpValidator(rx, this));
//    m_pleBandWidthLimitValidStartTime->setInputMask("99:99");
//    m_pleBandWidthLimitValidStartTime->setText("00:00");
//    m_pleBandWidthLimitValidEndTime = new QLineEdit(this);
//    m_pleBandWidthLimitValidEndTime->setFixedSize(40, 20);
//    m_pleBandWidthLimitValidEndTime->setValidator(new QRegExpValidator(rx, this));
//    m_pleBandWidthLimitValidEndTime->setInputMask("99:99");
//    m_pleBandWidthLimitValidEndTime->setText("00:00");
//    lBandWidthLimitTxt1 = new QLabel(u8"带宽限制：", this);
//    lBandWidthUnitTxt = new QLabel(u8"kb/s", this);
//    lValidTimeTxt = new QLabel(u8"生效时间：", this);
//    lSpanSymbleTxt = new QLabel(u8"--", this);
//    QHBoxLayout *h_bandwidth_layout = new QHBoxLayout;
//    h_bandwidth_layout->setContentsMargins(0, 0, 0, 0);
//    h_bandwidth_layout->setSpacing(5);
//    h_bandwidth_layout->addWidget(lBandWidthLimitTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_layout->addWidget(m_pclBandWidthLimit, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_layout->addStretch();
//    QHBoxLayout *h_bandwidth_limit_layout = new QHBoxLayout;
//    h_bandwidth_layout->setContentsMargins(0, 0, 0, 0);
//    h_bandwidth_limit_layout->setSpacing(5);
//    h_bandwidth_limit_layout->addSpacing(70);
//    h_bandwidth_limit_layout->addWidget(lBandWidthLimitTxt1, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_limit_layout->addWidget(m_pleLimitedBandWidth, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_limit_layout->addWidget(lBandWidthUnitTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_limit_layout->addStretch();
//    QHBoxLayout *h_bandwidth_time_layout = new QHBoxLayout;
//    h_bandwidth_time_layout->setContentsMargins(0, 0, 0, 0);
//    h_bandwidth_time_layout->setSpacing(5);
//    h_bandwidth_time_layout->addWidget(lValidTimeTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_time_layout->addWidget(m_pleBandWidthLimitValidStartTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_time_layout->addWidget(lSpanSymbleTxt, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_time_layout->addWidget(m_pleBandWidthLimitValidEndTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    h_bandwidth_time_layout->addStretch();
//    vlayout->addLayout(h_bandwidth_layout);
//    vlayout->addLayout(h_bandwidth_limit_layout);
//    vlayout->addLayout(h_bandwidth_time_layout);
//    vlayout->addSpacing(5);

    m_pTable = new UpdateSourceTable(this);
    m_pbAddBtn = new PushButton(this);
    m_pbAddBtn->loadPixmap(":/resource/plus_btn");
    lUpdateSourceTxt = new QLabel(u8"升级源：", this);
    lUpdateSourceTxt->setFont(font);
    lAddOtherServer = new QLabel(u8"增加其它升级中心", this);
    lAddOtherServer->setFont(font);
    QHBoxLayout *h_source_layout = new QHBoxLayout;
    h_source_layout->setContentsMargins(0, 0, 0, 10);
    h_source_layout->setSpacing(0);
    h_source_layout->addWidget(lUpdateSourceTxt, 0, Qt::AlignLeft|Qt::AlignBottom);
    h_source_layout->addStretch();
    h_source_layout->addWidget(lAddOtherServer, 0, Qt::AlignRight|Qt::AlignVCenter);
    h_source_layout->addSpacing(3);
    h_source_layout->addWidget(m_pbAddBtn, 0, Qt::AlignRight|Qt::AlignBottom);
    h_source_layout->addSpacing(10);
    QHBoxLayout *h_table_layout = new QHBoxLayout;
    h_table_layout->setContentsMargins(0, 0, 0, 0);
    h_table_layout->setSpacing(0);
    h_table_layout->addWidget(m_pTable, 0, Qt::AlignTop);
    h_table_layout->addSpacing(10);
    QVBoxLayout *v_source_layout = new QVBoxLayout;
    v_source_layout->setContentsMargins(0, 0, 0, 0);
    v_source_layout->setSpacing(0);
    v_source_layout->addLayout(h_source_layout);
    v_source_layout->addLayout(h_table_layout);
    vlayout->addLayout(v_source_layout);
    setLayout(vlayout);

    m_pMenu = new QMenu;
    m_pMenu->setStyleSheet(" QMenu::item {\
                           background-color: white;\
                           padding:2px 2px;\
                           margin:2px 2px;\
                           }\
                            QMenu::item:selected {\
                            background-color: rgb(228,221,235);\
                            }");
    m_paAddRising = new QAction(u8"瑞星官网", m_pMenu);
    m_paAddOthers = new QAction(u8"其它升级源", m_pMenu);
    m_pMenu->addAction(m_paAddRising);
    m_pMenu->addAction(m_paAddOthers);

    connect(m_pleDayTime, SIGNAL(textChanged(const QString&)), this, SLOT(OnLineEditTextChanged(QString)));
    connect(m_pbAddBtn, SIGNAL(clicked()), this, SLOT(PopAddMenu()));
    connect(m_pMenu, SIGNAL(triggered(QAction *)), this, SLOT(TrigerAddSourceMenu(QAction *)));
    connect(m_pbgUpgradeMode, SIGNAL(buttonClicked(int)), this, SLOT(OnClickedUpgradeMode(int)));

//    connect(m_pbgDelayBoot, SIGNAL(buttonClicked(int)), this, SLOT(OnClickedDelayBootType(int)));
//    connect(m_pcbProxyType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangedProxyType(int)));
//    connect(m_pclForceUpgrade, SIGNAL(clicked()), this, SLOT(OnClickedForceUpgrade()));
//    connect(m_pclBandWidthLimit, SIGNAL(clicked()), this, SLOT(OnClickedBandWidth()));

    SetFocus(m_bFocus);
}

void UpgradeGroup::PopAddMenu()
{
    m_pMenu->exec(QCursor::pos());
}

void UpgradeGroup::MenuAddRisingClicked()
{
    m_pTable->AddItem(APP_RISING_UPDATE_WEB_MAIN, false); //瑞星官网
}

void UpgradeGroup::MenuAddOthersClicked()
{
    m_pTable->AddSourceStart();
}

void UpgradeGroup::TrigerAddSourceMenu(QAction *pAction)
{
    if(pAction == m_paAddRising){
        MenuAddRisingClicked();
    }else if(pAction == m_paAddOthers){
        MenuAddOthersClicked();
    }
}

void UpgradeGroup::OnClickedUpgradeMode(int index)
{
    m_pleDayTime->setEnabled(index==1?true:false);
    m_pwdlWeekDays->setEnabled(index==2?true:false);
    m_pleWeekTime->setEnabled(index==2?true:false);
//    m_pleIntervalHours->setEnabled(index==3?true:false);
}

void UpgradeGroup::OnClickedDelayBootType(int index)
{
//    m_pleRecentMinutesBoot->setEnabled(index==2?true:false);
}

void UpgradeGroup::OnClickedForceUpgrade()
{
//    m_pleAfterDaysUpgrade->setEnabled(m_pclForceUpgrade->IsChecked());
}

void UpgradeGroup::OnClickedBandWidth()
{
//    m_pleLimitedBandWidth->setEnabled(m_pclBandWidthLimit->IsChecked());
//    m_pleBandWidthLimitValidStartTime->setEnabled(m_pclBandWidthLimit->IsChecked());
//    m_pleBandWidthLimitValidEndTime->setEnabled(m_pclBandWidthLimit->IsChecked());
}

void UpgradeGroup::OnChangedProxyType(int index)
{
//    m_pleAddress->setEnabled(index==2?true:false);
//    m_plePort->setEnabled(index==2?true:false);
//    m_pleUser->setEnabled(index==2?true:false);
//    m_plePassword->setEnabled(index==2?true:false);
}

void UpgradeGroup::SetFocus(bool bFocus)
{
    m_bFocus = bFocus;
//    if(m_bFocus){
//        setStyleSheet("QGroupBox {\
//              font-size:13px; \
//              border: 2px solid #7B599F;\
//              margin-top: 1ex; \
//              }\
//              QGroupBox::title {\
//              top: 1px;\
//              left: 20px;\
//              subcontrol-origin: margin;\
//              subcontrol-position: top left;}");

//    }else{
//        setStyleSheet("QGroupBox {\
//              font-size:13px; \
//              border: 2px solid #EFEBE7;\
//              margin-top: 1ex; \
//              }\
//              QGroupBox::title {\
//              top: 1px;\
//              left: 20px;\
//              subcontrol-origin: margin;\
//              subcontrol-position: top left;}");
//    }
    update();
}

void UpgradeGroup::SetSizeEditValid()
{
//    if(m_pclProcCompressed->m_bChecked == false){
//        m_pleSize->setDisabled(true);
//    }else{
//        m_pleSize->setDisabled(false);
//    }

}

void UpgradeGroup::OnLineEditTextChanged(const QString &text)
{
    int pos;
    QString stext = text;
    if(text.size() >= 5) {
        if (m_pValidatorTime->validate(stext, pos) != QValidator::Acceptable) {
            //QMessageBox::information(this, "Error", u8"无效的时间字串。", QMessageBox::Ok, QMessageBox::Ok);
            qDebug() << "TimerGroup::OnLineEditTextChanged() text=" << text << endl;
        }
    }
}
