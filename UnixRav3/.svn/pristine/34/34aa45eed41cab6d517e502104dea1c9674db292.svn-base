#ifndef RIGHTENDWIDGET_H
#define RIGHTENDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QPoint>

#include "common/pubdef.h"
//#include "topscrollwidget.h"
#include "endscrollwidget.h"

//class TopScrollWidget;
class EndScrollWidget;

class RightEndWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit RightEndWidget(QWidget *parent = nullptr, int nHeight = APP_SCREEN_HEIGHT);

signals:
  void valueChanged(int newValue);

protected:
    bool eventFilter(QObject *obj,QEvent *evt);

public slots:
    void SwitchTopWidget();
    void SwitchEndWidget();

public:
    //TopScrollWidget     *m_pTopWidget;
    EndScrollWidget     *m_pEndWidget;

    bool m_bMoveStart;
    bool m_bScrollStart;

};

#endif // RIGHTENDWIDGET_H
