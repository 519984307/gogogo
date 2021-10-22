#include "radiobutton.h"

RadioButton::RadioButton(const QString &text, QWidget *parent):QRadioButton(text, parent)
{
    setStyleSheet("QRadioButton{\
                spacing: 2px;\
                margin:2px;\
                color: black;\
            }\
            QRadioButton::disabled{\
            color: gray;\
            }\
            QRadioButton::indicator {\
                width: 18px;\
                height: 18px;\
            }\
            QRadioButton::indicator:unchecked {\
                image: url(:/Images/switchOff);\
            }\
            QRadioButton::indicator:unchecked:hover {\
                image: url(:/Images/switchOffHover);\
            }\
            QRadioButton::indicator:unchecked:pressed {\
                image: url(:/Images/switchOffPressed);\
            }\
            QRadioButton::indicator:checked {\
                image: url(:/Images/switchOn);\
            }\
            QRadioButton::indicator:checked:hover {\
                image: url(:/Images/switchOnHover);\
            }\
            QRadioButton::indicator:checked:pressed {\
                image: url(:/Images/switchOnPressed);\
            }");
    adjustSize();
}

