#ifndef IMGBUTTON_H
#define IMGBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "pubdef.h"


class ImgButton : public QPushButton
{
public:
    ImgButton(QWidget *parent = Q_NULLPTR);
};

#endif // IMGBUTTON_H
