#ifndef RIGHTTOPWIDGET_H
#define RIGHTTOPWIDGET_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>

#include "common/pubdef.h"
#include "topscrollwidget.h"
//#include "endscrollwidget.h"

class QVBoxLayout;
class TopScrollWidget;
//class EndScrollWidget;

class RightTopWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit RightTopWidget(QWidget *parent = nullptr, int nHeight = APP_SCREEN_HEIGHT);

public slots:
    void SwitchTopWidget();
    void SwitchEndWidget();

signals:
  void valueChanged(int newValue);

protected:
    bool eventFilter(QObject *obj,QEvent *evt);

public:
    TopScrollWidget     *m_pTopWidget;
    //EndScrollWidget     *m_pEndWidget;

    bool m_bMoveStart;
    bool m_bScrollStart;
};

#endif // RIGHTTOPWIDGET_H
