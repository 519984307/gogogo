#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"
#include "traygroup.h"



TrayGroup::TrayGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"托盘设置");
    QFont font("Arial", 10, QFont::Thin, false);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(65);
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

    m_pclHideTaskbarTray = new CheckboxLabel(this, u8"隐藏任务栏托盘图标");
    m_pclHideTaskbarTray->setFont(font);
    m_pclHideTaskbarTray->setGeometry(25, 30, m_pclHideTaskbarTray->width(), m_pclHideTaskbarTray->height());

    SetFocus(m_bFocus);
}

void TrayGroup::SetFocus(bool bFocus)
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

void TrayGroup::SetSizeEditValid()
{

}
