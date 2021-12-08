#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include <QObject>
#include <QWidget>
#include <QRadioButton>
#include <QPalette>

#include "pubdef.h"

class RadioButton : public QRadioButton
{
    Q_OBJECT
public:
    RadioButton(const QString &text, QWidget *parent = Q_NULLPTR);


};

#endif // RADIOBUTTON_H
