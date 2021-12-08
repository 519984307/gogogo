#ifndef RIGHTWIDGET_H
#define RIGHTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>
#include "pubdef.h"

class VirusDetailGroup;
class ScanEventGroup;
//class SysReinforceGroup;
//class AppReinforceGroup;
class IsolationGroup;
class UrlAccessGroup;
class IPRuleGroup;
class InstallGroup;
class SysLogGroup;

class MainWindow;

enum{
    LOGVIEW_SCREEN_VDETAIL = 0,
    LOGVIEW_SCREEN_SCANEVENT,
//    LOGVIEW_SCREEN_SYSREINFORCE,
//    LOGVIEW_SCREEN_APPREINFORCE,
    LOGVIEW_SCREEN_ISOLATION,
    LOGVIEW_SCREEN_URLACCESS,
    LOGVIEW_SCREEN_IPRULE,
    LOGVIEW_SCREEN_INSTALL,
    LOGVIEW_SCREEN_SYSLOG
};

class RightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightWidget(QWidget *parent = nullptr);

    void ShowScreen(int nIndex);
    void SetMainWindowPtr(MainWindow* ptr);

    void InitAllData();
    void InitVDetailData();
    void InitScanEventData();
    void InitIsolationData();
    void InitUrlAccessData();
    void InitIPRuleData();
    void InitInstallData();
    void InitSysLogData();

    void SetMaxSizeState(bool bMaxSize);

protected:
    void resizeEvent(QResizeEvent *event);

signals:

public slots:

public:
    VirusDetailGroup    *m_pVDetail;
    ScanEventGroup      *m_pScanEvent;
//    SysReinforceGroup   *m_pSysInforce;
//    AppReinforceGroup   *m_pAppInforce;
    IsolationGroup      *m_pIsolation;

    UrlAccessGroup      *m_pUrlAccess;
    IPRuleGroup         *m_pIPRule;

    InstallGroup        *m_pInstall;
    SysLogGroup         *m_pSysLog;

    QList<QWidget*>  m_lScreens;

    MainWindow  *m_pMain;
    bool    m_bMaxSizeState;
};

#endif // RIGHTWIDGET_H
