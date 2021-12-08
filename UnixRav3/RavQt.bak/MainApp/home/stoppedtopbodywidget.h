#ifndef STOPPEDTOPBODYWIDGET_H
#define STOPPEDTOPBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include "common.h"

class StoppedTopBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StoppedTopBodyWidget(QWidget *parent = nullptr);
    ~StoppedTopBodyWidget();

    void UpdateStoppedInfo(int nThreatens, int nCount, int nUsingTime);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

    void UpdateInfo();

public:
    QLabel  *lbl1;
    QLabel  *lbl2;
    QLabel  *lbl3;
    QLabel  *lbl4;

    QLabel  *m_pLThreatens;
    QLabel  *m_pLItemsCount;
    QLabel  *m_pLUsingTime;

    int m_nThreatens;
    int m_nItemCount;
    int m_nUsingTimes; //seconds
};

#endif // STOPPEDTOPBODYWIDGET_H
