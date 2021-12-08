#ifndef TOPBODYWIDGET_H
#define TOPBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include "animitedbutton.h"
#include "common.h"


class TopBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopBodyWidget(QWidget *parent = nullptr);
    ~TopBodyWidget();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

public:
    AnimitedButton *m_pAllKillButton;
    AnimitedButton *m_pFastKillButton;
    AnimitedButton *m_pCustomKillButton;

};

#endif // TOPBODYWIDGET_H
