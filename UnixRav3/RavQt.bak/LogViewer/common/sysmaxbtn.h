#ifndef SYSMAXBTN_H
#define SYSMAXBTN_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "pubdef.h"


class SysMaxBtn : public QPushButton
{
public:
    SysMaxBtn(QWidget *parent = Q_NULLPTR);

    void SetMaxState();
    void SetRegainState();

public:
    bool    m_bMaxState;
};

#endif // SYSMAXBTN_H
