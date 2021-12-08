#ifndef RIGHTADDWIDGET_H
#define RIGHTADDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QPoint>

#include "common/pubdef.h"
#include "addscrollwidget.h"
#include "mainwindow.h"

class EndScrollWidget;

class RightAddWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit RightAddWidget(QWidget *parent = nullptr, int nHeight = APP_SCREEN_HEIGHT);

signals:
  void valueChanged(int newValue);

protected:
    bool eventFilter(QObject *obj,QEvent *evt);

public slots:
    void SwitchTopWidget();
    void SwitchEndWidget();

public:
    MainWindow  *m_pMainWindow;

    AddScrollWidget     *m_pAddWidget;

    bool m_bMoveStart;
    bool m_bScrollStart;

};

#endif // RIGHTADDWIDGET_H
