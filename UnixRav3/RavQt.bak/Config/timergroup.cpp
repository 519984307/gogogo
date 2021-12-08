#include "timergroup.h"

#include "common/pubdef.h"
#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/combobox.h"
#include "weekdayselector.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>



TimerGroup::TimerGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"定时扫描");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30, 242);
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

    m_pclAllDiskScan = new CheckboxLabel(this, u8"启用定时全盘扫描");
    m_pbgAllDiskScan = new QButtonGroup(this);
    m_prbTPowerOn = new RadioButton(u8"开机", this);
    m_prbTEveryDay = new RadioButton(u8"每天", this);
    QRegExp rx("^([0-1][0-9]|2[0-3]):([0-5][0-9])$");
    m_pValidatorTime = new QRegExpValidator(rx, this);
    m_pleTDayTime = new QLineEdit(this);
    m_pleTDayTime->setFixedWidth(60);
    m_pleTDayTime->setValidator(new QRegExpValidator(rx, this));
    m_pleTDayTime->setInputMask("99:99");
    m_pleTDayTime->setText("09:00");

    m_prbTEveryWeek = new RadioButton(u8"每周", this);
    m_pTWeekSel = new WeekDaySelector(this);
    m_pleTWeekTime = new QLineEdit(this);
    m_pleTWeekTime->setValidator(new QRegExpValidator(rx, this));
    m_pleTWeekTime->setInputMask("99:99");
    m_pleTWeekTime->setText("09:00");
    m_pleTWeekTime->setFixedWidth(60);
    m_prbTEveryMonth = new RadioButton(u8"每月", this);
    m_pcbTMonthDay = new ComboBox(this);
    m_pcbTMonthDay->setFixedWidth(80);
    QString stxt;
    int i;
    for(i=1;i<32;i++){
        stxt = QString(u8"%1号").arg(i,0,10);
        m_pcbTMonthDay->addItem(stxt);
    }
    m_pleTMonthTime = new QLineEdit(this);
    m_pleTMonthTime->setFixedWidth(60);
    m_pleTMonthTime->setValidator(new QRegExpValidator(rx, this));
    m_pleTMonthTime->setInputMask("99:99");
    m_pleTMonthTime->setText("09:00");
    m_pbgAllDiskScan->setExclusive(true);
    m_pbgAllDiskScan->addButton(m_prbTPowerOn);
    m_pbgAllDiskScan->addButton(m_prbTEveryDay);
    m_pbgAllDiskScan->addButton(m_prbTEveryWeek);
    m_pbgAllDiskScan->addButton(m_prbTEveryMonth);

    m_pclFastScan = new CheckboxLabel(this, u8"启用定时快速扫描");
    m_pbgFastScan = new QButtonGroup(this);
    m_prbBPowerOn = new RadioButton(u8"开机", this);
    m_prbBEveryDay = new RadioButton(u8"每天", this);
    m_pleBDayTime = new QLineEdit(this);
    m_pleBDayTime->setFixedWidth(60);
    m_pleBDayTime->setValidator(new QRegExpValidator(rx, this));
    m_pleBDayTime->setInputMask("99:99");
    m_pleBDayTime->setText("09:00");
    m_prbBEveryWeek = new RadioButton(u8"每周", this);
    m_pBWeekSel = new WeekDaySelector(this);
    m_pleBWeekTime = new QLineEdit(this);
    m_pleBWeekTime->setFixedWidth(60);
    m_pleBWeekTime->setValidator(new QRegExpValidator(rx, this));
    m_pleBWeekTime->setInputMask("99:99");
    m_pleBWeekTime->setText("09:00");
    m_prbBEveryMonth = new RadioButton(u8"每月", this);
    m_pcbBMonthDay = new ComboBox(this);
    m_pcbBMonthDay->setFixedWidth(80);
    for(i=1;i<32;i++){
        stxt = QString(u8"%1号").arg(i,0,10);
        m_pcbBMonthDay->addItem(stxt);
    }
    m_pleBMonthTime = new QLineEdit(this);
    m_pleBMonthTime->setFixedWidth(60);
    m_pleBMonthTime->setValidator(new QRegExpValidator(rx, this));
    m_pleBMonthTime->setInputMask("99:99");
    m_pleBMonthTime->setText("09:00");
    m_pbgFastScan->setExclusive(true);
    m_pbgFastScan->addButton(m_prbBPowerOn);
    m_pbgFastScan->addButton(m_prbBEveryDay);
    m_pbgFastScan->addButton(m_prbBEveryWeek);
    m_pbgFastScan->addButton(m_prbBEveryMonth);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(20, 15, 0, 0);
    vlayout->setSpacing(0);

    QHBoxLayout *holayout = new QHBoxLayout;
    holayout->setContentsMargins(0, 0, 0, 0);
    holayout->setSpacing(10);
    holayout->addSpacing(50);
    holayout->addWidget(m_prbTPowerOn, 0, Qt::AlignLeft|Qt::AlignVCenter);
    holayout->addWidget(m_prbTEveryDay, 0, Qt::AlignLeft|Qt::AlignVCenter);
    holayout->addWidget(m_pleTDayTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    holayout->addWidget(m_prbTEveryWeek, 0, Qt::AlignLeft|Qt::AlignVCenter);
    holayout->addWidget(m_pTWeekSel, 0, Qt::AlignLeft|Qt::AlignVCenter);
    holayout->addWidget(m_pleTWeekTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    holayout->addStretch();

    vlayout->addWidget(m_pclAllDiskScan, 0, Qt::AlignLeft);
    vlayout->addLayout(holayout);

    QHBoxLayout *htlayout = new QHBoxLayout;
    htlayout->setContentsMargins(0, 0, 0, 0);
    htlayout->setSpacing(10);
    htlayout->addSpacing(50);
    htlayout->addWidget(m_prbTEveryMonth, 0, Qt::AlignLeft|Qt::AlignVCenter);
    htlayout->addWidget(m_pcbTMonthDay, 0, Qt::AlignLeft|Qt::AlignVCenter);
    htlayout->addWidget(m_pleTMonthTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    htlayout->addStretch();

    vlayout->addLayout(htlayout);

    QHBoxLayout *hhlayout = new QHBoxLayout;
    hhlayout->setContentsMargins(0, 0, 0, 0);
    hhlayout->setSpacing(10);
    hhlayout->addSpacing(50);
    hhlayout->addWidget(m_prbBPowerOn, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hhlayout->addWidget(m_prbBEveryDay, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hhlayout->addWidget(m_pleBDayTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hhlayout->addWidget(m_prbBEveryWeek, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hhlayout->addWidget(m_pBWeekSel, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hhlayout->addWidget(m_pleBWeekTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hhlayout->addStretch();

    vlayout->addWidget(m_pclFastScan, 0, Qt::AlignLeft);
    vlayout->addLayout(hhlayout);

    QHBoxLayout *helayout = new QHBoxLayout;
    helayout->setContentsMargins(0, 0, 0, 0);
    helayout->setSpacing(10);
    helayout->addSpacing(50);
    helayout->addWidget(m_prbBEveryMonth, 0, Qt::AlignLeft|Qt::AlignVCenter);
    helayout->addWidget(m_pcbBMonthDay, 0, Qt::AlignLeft|Qt::AlignVCenter);
    helayout->addWidget(m_pleBMonthTime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    helayout->addStretch();

    vlayout->addLayout(helayout);

    setLayout(vlayout);

    SetFocus(m_bFocus);

    connect(m_pclAllDiskScan, SIGNAL(clicked()), this, SLOT(OnClickedSubTitleCheckBox()));
    connect(m_pclFastScan, SIGNAL(clicked()), this, SLOT(OnClickedSubTitleCheckBox()));

    connect(m_pleTDayTime, SIGNAL(textChanged(const QString&)), this, SLOT(OnLineEditTextChanged(QString)));

    connect(m_prbTPowerOn, SIGNAL(toggled(bool)), this, SLOT(onUpdateAllDiskControlState()));
    connect(m_prbTEveryDay, SIGNAL(toggled(bool)), this, SLOT(onUpdateAllDiskControlState()));
    connect(m_prbTEveryWeek, SIGNAL(toggled(bool)), this, SLOT(onUpdateAllDiskControlState()));
    connect(m_prbTEveryMonth, SIGNAL(toggled(bool)), this, SLOT(onUpdateAllDiskControlState()));

    connect(m_prbBPowerOn, SIGNAL(toggled(bool)), this, SLOT(onUpdateQuickDiskControlState()));
    connect(m_prbBEveryDay, SIGNAL(toggled(bool)), this, SLOT(onUpdateQuickDiskControlState()));
    connect(m_prbBEveryWeek, SIGNAL(toggled(bool)), this, SLOT(onUpdateQuickDiskControlState()));
    connect(m_prbBEveryMonth, SIGNAL(toggled(bool)), this, SLOT(onUpdateQuickDiskControlState()));

    UpdateControlStatus();
}

void TimerGroup::SetFocus(bool bFocus)
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

void TimerGroup::onUpdateAllDiskControlState()
{
    if (m_prbTPowerOn->isChecked()){
        m_pleTDayTime->setDisabled(true);
        m_pTWeekSel->setDisabled(true);
        m_pleTWeekTime->setDisabled(true);
        m_pcbTMonthDay->setDisabled(true);
        m_pleTMonthTime->setDisabled(true);
    }else if(m_prbTEveryDay->isChecked()) {
        m_pleTDayTime->setDisabled(false);
        m_pTWeekSel->setDisabled(true);
        m_pleTWeekTime->setDisabled(true);
        m_pcbTMonthDay->setDisabled(true);
        m_pleTMonthTime->setDisabled(true);
    }else if(m_prbTEveryWeek->isChecked()) {
        m_pleTDayTime->setDisabled(true);
        m_pTWeekSel->setDisabled(false);
        m_pleTWeekTime->setDisabled(false);
        m_pcbTMonthDay->setDisabled(true);
        m_pleTMonthTime->setDisabled(true);
    }else if(m_prbTEveryMonth->isChecked()) {
        m_pleTDayTime->setDisabled(true);
        m_pTWeekSel->setDisabled(true);
        m_pleTWeekTime->setDisabled(true);
        m_pcbTMonthDay->setDisabled(false);
        m_pleTMonthTime->setDisabled(false);
    }
}

void TimerGroup::onUpdateQuickDiskControlState()
{
    if (m_prbBPowerOn->isChecked()) {
        m_pleBDayTime->setDisabled(true);
        m_pBWeekSel->setDisabled(true);
        m_pleBWeekTime->setDisabled(true);
        m_pcbBMonthDay->setDisabled(true);
        m_pleBMonthTime->setDisabled(true);
    }else if (m_prbBEveryDay->isChecked()) {
        m_pleBDayTime->setDisabled(false);
        m_pBWeekSel->setDisabled(true);
        m_pleBWeekTime->setDisabled(true);
        m_pcbBMonthDay->setDisabled(true);
        m_pleBMonthTime->setDisabled(true);
    }else if (m_prbBEveryWeek->isChecked()) {
        m_pleBDayTime->setDisabled(true);
        m_pBWeekSel->setDisabled(false);
        m_pleBWeekTime->setDisabled(false);
        m_pcbBMonthDay->setDisabled(true);
        m_pleBMonthTime->setDisabled(true);
    }else if (m_prbBEveryMonth->isChecked()) {
        m_pleBDayTime->setDisabled(true);
        m_pBWeekSel->setDisabled(true);
        m_pleBWeekTime->setDisabled(true);
        m_pcbBMonthDay->setDisabled(false);
        m_pleBMonthTime->setDisabled(false);
    }
}

void TimerGroup::UpdateControlStatus()
{
    if(m_pclAllDiskScan->m_bChecked){
        m_prbTPowerOn->setEnabled(true);
        m_prbTEveryDay->setEnabled(true);
        m_prbTEveryWeek->setEnabled(true);
        m_prbTEveryMonth->setEnabled(true);
        onUpdateAllDiskControlState();
    }else{
        m_prbTPowerOn->setEnabled(false);
        m_prbTEveryDay->setEnabled(false);
        m_pleTDayTime->setEnabled(false);
        m_prbTEveryWeek->setEnabled(false);
        m_pTWeekSel->setEnabled(false);
        m_pleTWeekTime->setEnabled(false);
        m_prbTEveryMonth->setEnabled(false);
        m_pcbTMonthDay->setEnabled(false);
        m_pleTMonthTime->setEnabled(false);
    }
    if(m_pclFastScan->m_bChecked){
        m_prbBPowerOn->setEnabled(true);
        m_prbBEveryDay->setEnabled(true);
        m_prbBEveryWeek->setEnabled(true);
        m_prbBEveryMonth->setEnabled(true);
        onUpdateQuickDiskControlState();
    }else{
        m_prbBPowerOn->setEnabled(false);
        m_prbBEveryDay->setEnabled(false);
        m_pleBDayTime->setEnabled(false);
        m_prbBEveryWeek->setEnabled(false);
        m_pBWeekSel->setEnabled(false);
        m_pleBWeekTime->setEnabled(false);
        m_prbBEveryMonth->setEnabled(false);
        m_pcbBMonthDay->setEnabled(false);
        m_pleBMonthTime->setEnabled(false);
    }
}

void TimerGroup::SetSizeEditValid()
{
//    if(m_pclProcCompressed->m_bChecked == false){
//        m_pleSize->setDisabled(true);
//    }else{
//        m_pleSize->setDisabled(false);
//    }

}

void TimerGroup::OnClickedSubTitleCheckBox()
{
    UpdateControlStatus();
}

void TimerGroup::OnLineEditTextChanged(const QString &text)
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
