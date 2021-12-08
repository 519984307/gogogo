#ifndef DEFENDSTATEWIDGET_H
#define DEFENDSTATEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QLabel>

#include "common.h"


class HImageButton;
class ShowButton;


class DefendStateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendStateWidget(QWidget *parent = nullptr);

    void SetNormalState(bool bEnable);

signals:

public slots:


public:
    //QPixmap     *m_ppIcon;
    ShowButton      *m_plIcon;
    QLabel      *m_plDesc;

    HImageButton    *m_pbtnOpenAll;
};

#endif // DEFENDSTATEWIDGET_H
