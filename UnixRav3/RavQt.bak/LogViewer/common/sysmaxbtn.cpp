#include "sysmaxbtn.h"

SysMaxBtn::SysMaxBtn(QWidget *parent):QPushButton(parent)
{

    SetMaxState();
    QPixmap img(":/Images/sysmax_btn_normal");
    setFixedSize(img.width(), img.height());
}

void SysMaxBtn::SetMaxState()
{
    setStyleSheet("QPushButton {\
                  border: 0px transparent;\
                  padding: 0px;\
                  image: url(:/Images/sysmax_btn_normal);\
              }\
              QPushButton:hover {\
                  image: url(:/Images/sysmax_btn_hover);\
              }\
              QPushButton:pressed {\
                  image: url(:/Images/sysmax_btn_pressed);\
              }\
            QPushButton:disabled {\
                image: url(:/Images/sysmax_btn_normal);\
            }\
              QPushButton:flat {\
                  image: url(:/Images/sysmax_btn_normal);\
              }\
              QPushButton:default {\
                  border-color: navy; \
              }");
    m_bMaxState = true;
}

void SysMaxBtn::SetRegainState()
{
    setStyleSheet("QPushButton {\
                  border: 0px transparent;\
                  padding: 0px;\
                  image: url(:/Images/sysregain_btn_normal);\
              }\
              QPushButton:hover {\
                  image: url(:/Images/sysregain_btn_hover);\
              }\
              QPushButton:pressed {\
                  image: url(:/Images/sysregain_btn_pressed);\
              }\
            QPushButton:disabled {\
                image: url(:/Images/sysregain_btn_normal);\
            }\
              QPushButton:flat {\
                  image: url(:/Images/sysregain_btn_normal);\
              }\
              QPushButton:default {\
                  border-color: navy; \
              }");
    m_bMaxState = false;
}
