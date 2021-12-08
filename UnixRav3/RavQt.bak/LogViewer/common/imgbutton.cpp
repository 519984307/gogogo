#include "imgbutton.h"

ImgButton::ImgButton(QWidget *parent):QPushButton(parent)
{
    setStyleSheet("QPushButton {\
                  border: 0px transparent;\
                  padding: 0px;\
                  image: url(:/Images/license_active_normal);\
              }\
              QPushButton:hover {\
                  image: url(:/Images/license_active_hovered);\
              }\
              QPushButton:pressed {\
                  image: url(:/Images/license_active_pressed);\
              }\
            QPushButton:disabled {\
                image: url(:/Images/license_active_disabled);\
            }\
              QPushButton:flat {\
                  image: url(:/Images/license_active_disabled);\
              }\
              QPushButton:default {\
                  border-color: navy; \
              }");
    QPixmap img(":/Images/license_active_normal");
    setFixedSize(img.width(), img.height());

}
