#ifndef RIGHTMIDWIDGET_H
#define RIGHTMIDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>

#include "common/pubdef.h"


class MidScrollWidget;
//class EndScrollWidget;

class RightMidWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit RightMidWidget(QWidget *parent = nullptr, int nHeight = APP_SCREEN_HEIGHT);

public slots:
    void SwitchTopWidget();
    void SwitchEndWidget();

signals:
  void valueChanged(int newValue);

protected:
    bool eventFilter(QObject *obj,QEvent *evt);

public:
    MidScrollWidget     *m_pMidWidget;
    //EndScrollWidget     *m_pEndWidget;

    bool m_bMoveStart;
    bool m_bScrollStart;
};

#endif // RIGHTMIDWIDGET_H
