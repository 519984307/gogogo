#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QComboBox>

#include "pubdef.h"


class ComboBox : public QComboBox
{
public:
    ComboBox(QWidget *parent = Q_NULLPTR);
};

#endif // COMBOBOX_H
