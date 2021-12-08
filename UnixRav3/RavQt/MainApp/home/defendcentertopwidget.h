#ifndef DEFENDCENTERTOPWIDGET_H
#define DEFENDCENTERTOPWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include "common.h"

#define DEFENDCENTER_TOP_WIDGET_HEIGHT     208

class DefendCenterItem;

class DefendCenterTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendCenterTopWidget(QWidget *parent = nullptr, int functions = 0x3F);

signals:

public slots:

public:
    QLabel  *m_plIcon;

    DefendCenterItem    *m_pItemFile;
//    DefendCenterItem    *m_pItemMail;
//    DefendCenterItem    *m_pItemShare;
    DefendCenterItem    *m_pItemUDisk;


};

#endif // DEFENDCENTERTOPWIDGET_H
