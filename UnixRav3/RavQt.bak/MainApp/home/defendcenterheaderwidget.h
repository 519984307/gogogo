#ifndef DEFENDCENTERHEADERWIDGET_H
#define DEFENDCENTERHEADERWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include "common.h"


class PushButton;

class DefendCenterHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendCenterHeaderWidget(QWidget *parent = nullptr);

signals:
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

public slots:

public:
    QLabel      *m_plTitle;
    PushButton  *m_pbtnClose;

    QPoint  m_ptPressPoint;
    bool    m_bIsMove;
};

#endif // DEFENDCENTERHEADERWIDGET_H
