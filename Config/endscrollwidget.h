#ifndef ENDSCROLLWIDGET_H
#define ENDSCROLLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "pubdef.h"
//#include "admingroup.h"
#include "traygroup.h"
#include "licensegroup.h"
#include "upgradegroup.h"
#include "centerdiscoverygroup.h"
#include "mainwindow.h"


//class AdminGroup;
class TrayGroup;
class LicenseGroup;
class UpgradeGroup;


class EndScrollWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndScrollWidget(QWidget *parent, MainWindow *pMainWindow);

signals:

public slots:

public:
    MainWindow  *m_pMainWindow;

    QVBoxLayout *m_pLayout;

    //AdminGroup      *m_pAdmin;
    //TrayGroup       *m_pTray;
    //LicenseGroup    *m_pLicense;
    UpgradeGroup            *m_pUpgrade;
    CenterDiscoveryGroup    *m_pCenterDiscovery;
};

#endif // ENDSCROLLWIDGET_H
