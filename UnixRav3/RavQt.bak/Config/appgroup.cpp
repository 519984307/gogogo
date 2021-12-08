#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"
#include "appgroup.h"


AppGroup::AppGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"应用加固");
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

    QLabel *lApp = new QLabel(u8"发现威胁：", this);
    lApp->setFont(font);
    lApp->adjustSize();
    m_pbgFoundThreaten = new QButtonGroup;
    m_prbEnableRun = new RadioButton(u8"允许运行", this);
    m_prbDisableRun = new RadioButton(u8"禁止运行", this);
    m_prbEnableRun->setFont(font);
    m_prbDisableRun->setFont(font);
    m_pbgFoundThreaten->addButton(m_prbEnableRun, APP_ENABLE_RUN);
    m_pbgFoundThreaten->addButton(m_prbDisableRun, APP_DISABLE_RUN);
    lApp->setGeometry(25, 30, lApp->width(), lApp->height());
    m_prbEnableRun->setGeometry(110, 30, m_prbEnableRun->width(), lApp->height());
    m_prbDisableRun->setGeometry(270, 30, m_prbDisableRun->width(), lApp->height());

    QLabel *lProcType = new QLabel(u8"处理方式：", this);
    lProcType->setFont(font);
    lProcType->adjustSize();
    m_pbgProcType = new QButtonGroup;
    m_prbAuto = new RadioButton(u8"自动处理", this);
    m_prbAskMe = new RadioButton(u8"通知我", this);
    m_prbAuto->setFont(font);
    m_prbAskMe->setFont(font);
    m_pbgProcType->addButton(m_prbAuto, APP_PROC_AUTO);
    m_pbgProcType->addButton(m_prbAskMe, APP_ASK_ME);
    lProcType->setGeometry(25, 55, lProcType->width(), lProcType->height());
    m_prbAuto->setGeometry(110, 55, m_prbAuto->width(), lProcType->height());
    m_prbAskMe->setGeometry(270, 55, m_prbAskMe->width(), lProcType->height());

    QLabel *lLogIntercept = new QLabel(u8"拦截日志：", this);
    lLogIntercept->setFont(font);
    lLogIntercept->adjustSize();
    m_pclLogIntercept = new CheckboxLabel(this, u8"记录拦截日志");
    m_pclLogIntercept->setFont(font);
    lLogIntercept->setGeometry(25, 80, lLogIntercept->width(), lLogIntercept->height());
    m_pclLogIntercept->setGeometry(110, 80, m_pclLogIntercept->width(), lLogIntercept->height());


    QLabel *lPopup = new QLabel(u8"启动弹框：", this);
    lPopup->setFont(font);
    lPopup->adjustSize();
    m_pclPopup = new CheckboxLabel(this, u8"启动时弹出软件保护框");
    m_pclPopup->setFont(font);
    lPopup->setGeometry(25, 105, lPopup->width(), lPopup->height());
    m_pclPopup->setGeometry(110, 105, m_pclPopup->width(), lPopup->height());

    SetFocus(m_bFocus);
}

void AppGroup::SetFocus(bool bFocus)
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

void AppGroup::SetSizeEditValid()
{

}
