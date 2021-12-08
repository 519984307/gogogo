#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#include "common.h"

class TitleWidget;
class ScanWidget;
class ScanProgressWidget;
class ScanNormalWidget;
class ScanStoppedlWidget;
class ScaningStateWidget;
class DefendStateWidget;


class HeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeaderWidget(QWidget *parent = 0);
    ~HeaderWidget();

    void SetVScanState(int nState);
    int VScanState();
    void UpdateScaningInfo(int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes);
    void UpdateScaningProgress(int progress);

signals:
    void showMin();
    void closeWidget();
    void changeCenter(int);
    void showConnectMenu();
    void logoClicked();

private:
    void setBackgroud(QString image);

protected:
    /** 处理缩放大小的事件的类 */
    void resizeEvent(QResizeEvent *event);

public:
    TitleWidget *mTitleView; //header,included logo and max/min/set/close button
    ScanWidget      *mVirusScanWidget;

    int m_nVScanState;

};

#endif // HEADERWIDGET_H
