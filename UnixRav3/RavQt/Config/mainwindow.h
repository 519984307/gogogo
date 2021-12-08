#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMutexLocker>
#include <QTimer>

#include "common/globalconf.h"
#include "application.h"
#include "circlelink.h"
#include "centerlistener.h"
#include "../Storage/Entry/sourcelistentry.h"
#include "../Storage/Entry/commonentry.h"

#include <string>
using namespace std;

class ClickedLabel;
class GlobalConf;
class TopWidget;
class LeftWidget;
class RightTopWidget;
class RightMidWidget;
class RightEndWidget;
class RightAddWidget;

class OriGroup;
class WhiteListGroup;
class BackupGroup;
class KVGroup;
class TimerGroup;
class FileGroup;
class UDiskGroup;

class AntiVirusSetting;
class ScanConfig;


//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow ,public Application
{
    Q_OBJECT
public:
    friend class Controller;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitWindow();
    void UpdateFromConfig();
    int UpdateToConfig();

    bool HideAdvanceFunction();
public slots:
    void OnClickedLeftKill();
//    void OnClickedLeftNetDefend();
    void OnClickedLeftSetup();
	//2021.9.18 by zhangdong
    void OnClickedLeftIp();

    void OnClickedLeftOri();
    void OnClickedLeftWhiteList();
    void OnClickedLeftBackup();
    void OnClickedLeftKV();
    void OnClickedLeftTimer();
    void OnClickedLeftFile();
    void OnClickedLeftUDisk();
//    void OnClickedLeftSystem();
//    void OnClickedLeftApp();

//    void OnClickedLeftNetBlacklist();

    //void OnClickedLeftAdmin();
//    void OnClickedLeftTray();
//    void OnClickedLeftLicense();
    void OnClickedLeftUpgrade();
    void OnClickedLeftCenterDiscovery();

	//2021.9.18 by zhangdong
    void OnClickedLeftIpWhiteList();
    void OnClickedLeftIpBlackList();
    void OnClickedLeftIpPort();

 //   void OnChangeAdminPasswd();
    void OnClickedSaveSetting();
    void OnClickedLoadDefault();

    void AutoSetFocus(int nScrollValue);

protected:
    void paintEvent(QPaintEvent *pPEvent);

    void mousePressEvent(QMouseEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);


    int TransPathType(int nTableType);
    int RTransPathType(int nStoreType);

    void UpdateFromConfigOri();
    void UpdateFromConfigWhiteList();
    void UpdateFromConfigBackup();
    void UpdateFromConfigKV();
    void UpdateFromConfigTimer();
    void UpdateFromConfigFile();
    void UpdateFromConfigUDisk();
    void UpdateFromConfigUpgrade();
    void UpdateFromConfigCenterDiscovery();

    void UpdateGuiWidgetsStateUpgrade();

    bool IsConfigedPrivateCloud(std::string name);
    void UpdateOrigPrivateCloudToConfig();

    bool IsConfigedWhiteEntry(std::list<WhiteListEntry> &lEntry, WhiteListEntry & qwle);
    void UpdateFileMonitorListFromConfig(std::list<WhiteListEntry> &white);
    void UpdataFileMonitorListToConfig();

    string GetWhiteListStr(std::list<WhiteListEntry>  &list);
    int SetStrToWhiteList(const char* sText,std::list<WhiteListEntry>  &list);
    bool IsConfigedScanEntry(std::list<WhiteListEntry> &lEntry, WhiteListEntry & qwle);
    void UpdateUserDefinedScanFileExt(const char* sText);

    void SetUpgradeSettingToConfig();
    bool IsConfigdCloudUpdateSourceEntry(const std::list<SourceListEntry> &lEntry, const string & source_addr);
    void SetCenterDiscoverySettingToConfig();
    bool IsConfigdCenterSourceEntry(const std::list<SourceListEntry> &lEntry, const string & source_addr);

private slots:
    void setMinimum();
    void OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void OnSaveTimer();

signals:
    void changeSaveBtnStatus();

private:
    //Ui::MainWindow *ui;
    bool m_bMoveStart;
    bool m_bScrollStart;
    bool m_bDragging;
    QPoint  m_poMouseStartPos;
    QPoint  m_poFrameStartPos;


    TopWidget   *m_pTopWidget;
    LeftWidget  *m_pLeftWidget;

    RightTopWidget *m_pRightTopWidget;
 //   RightMidWidget  *m_pRightMidWidget;
    RightEndWidget *m_pRightEndWidget;
    RightAddWidget *m_pRightAddWidget;

    QSystemTrayIcon *m_pSystemTrayIcon;

    ClickedLabel    *m_pclLoadDefault;
    QPushButton     *m_ppbApply;

    GlobalConf  m_ScreenConf;

    AntiVirusSetting    *m_pAvConfig;

    NetfilterSetting *m_pNetfilterConfig;

    UpgradeSetting    *m_pUpgradeSetting;
    std::list<SourceListEntry>  m_lstUpdateSouces;

    BaseSetting     *m_pBaseSetting;


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    QMetaObject::Connection m_btnConn;
#endif
    QMutex m_Mutex;
    QTimer  *m_pTimer;
    int     m_nFunction;
};

#endif // MAINWINDOW_H
