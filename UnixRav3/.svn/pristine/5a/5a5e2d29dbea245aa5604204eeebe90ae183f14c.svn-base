#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"
#include "systemgroup.h"


SystemGroup::SystemGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"系统加固");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(150);
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

    QLabel *lSystem = new QLabel(u8"发现威胁：", this);
    lSystem->setFont(font);
    lSystem->adjustSize();
    m_pbgFoundThreaten = new QButtonGroup;
    m_prbProcAuto = new RadioButton(u8"自动处理", this);
    m_prbAskMe = new RadioButton(u8"通知我", this);
    m_pbgFoundThreaten->addButton(m_prbProcAuto, APP_PROC_AUTO);
    m_pbgFoundThreaten->addButton(m_prbAskMe, APP_ASK_ME);
    m_prbProcAuto->setFont(font);
    m_prbAskMe->setFont(font);
    lSystem->setGeometry(25, 30, lSystem->width(), lSystem->height());
    m_prbProcAuto->setGeometry(110, 30, m_prbProcAuto->width(), lSystem->height());
    m_prbAskMe->setGeometry(270, 30, m_prbAskMe->width(), lSystem->height());

    QLabel *lMonitorSensibility = new QLabel(u8"监控灵敏度：", this);
    lMonitorSensibility->setFont(font);
    lMonitorSensibility->adjustSize();
    m_pbgMonitorSensibility = new QButtonGroup;
    m_prbLow = new RadioButton(u8"低", this);
    m_prbMiddle = new RadioButton(u8"中", this);
    m_prbHigh = new RadioButton(u8"高", this);
    m_prbLow->setFont(font);
    m_prbMiddle->setFont(font);
    m_prbHigh->setFont(font);
    m_pbgMonitorSensibility->addButton(m_prbLow, APP_LOW);
    m_pbgMonitorSensibility->addButton(m_prbMiddle, APP_MIDDLE);
    m_pbgMonitorSensibility->addButton(m_prbHigh, APP_HIGH);
    lMonitorSensibility->setGeometry(25, 55, lMonitorSensibility->width(), lMonitorSensibility->height());
    m_prbLow->setGeometry(110, 55, m_prbLow->width(), lMonitorSensibility->height());
    m_prbMiddle->setGeometry(270, 55, m_prbMiddle->width(), lMonitorSensibility->height());
    m_prbHigh->setGeometry(430, 55, m_prbHigh->width(), lMonitorSensibility->height());

    QLabel *lAuditMode = new QLabel(u8"审计模式：", this);
    lAuditMode->setFont(font);
    lAuditMode->adjustSize();
    m_pclAuditMode = new CheckboxLabel(this, u8"开启");
    m_pclAuditMode->setFont(font);
    lAuditMode->setGeometry(25, 80, lAuditMode->width(), lAuditMode->height());
    m_pclAuditMode->setGeometry(110, 80, m_pclAuditMode->width(), lAuditMode->height());

    QLabel *lOther = new QLabel(u8"其它：", this);
    lOther->setFont(font);
    lOther->adjustSize();
    m_pclPassDigitalSignature = new CheckboxLabel(this, u8"放过带数字签名的程序");
    m_pclPassDigitalSignature->setFont(font);
    lOther->setGeometry(25, 105, lOther->width(), lOther->height());
    m_pclPassDigitalSignature->setGeometry(110, 105, m_pclPassDigitalSignature->width(), lOther->height());

    SetFocus(m_bFocus);
}

void SystemGroup::SetFocus(bool bFocus)
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

void SystemGroup::SetSizeEditValid()
{

}
