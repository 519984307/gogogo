#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMutexLocker>
#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
//#include <QTabWidget>
#include <QSystemTrayIcon>

#include "aboutdialog.h"
#include "application.h"
#include "centerlistener.h"
#include "uicircularqueue.h"

class HImageButton;
class PushButton;
class HeaderWidget;
class TitleWidget;
class BodyWidget;
class DynMenu;
class ModeMenu;
class AboutDialog;
class Controller;
class DefendCenterDialog;
class DefendStateWiget;
class DefendWidget;
class ScanWidget;
class CenterListener;
class VirusMessageDialog;
class OperationPower;
class OfflineUpdateInfoDialog;

enum RAVQT_SCAN_READY_STATE{
    RAVQT_NO_SCANNING = 0,
    RAVQT_CLICKED_SCAN_WAITING,
    RAVQT_RECEIVED_RESULT,
    RAVQT_CLICKED_STOP_WAITING
};


class MainWindow : public QWidget,public Application//, public CenterCommandObserver
{
    Q_OBJECT
public:
    friend class Controller;
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

    void showWidget();
    int InitVersionTimeEtc();

    void UpdateScanReadyState(int nState);

//    void RefreshScanLogs(int scantype, CircleLink<std::string> *data);
    void RefreshVirusLogs(const string& file, const string& virus, int/** VirusReportData::VirusStatus */ status);
    void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE status, int progress, void *data=NULL);
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int  ScanFinished(int result);
    int  RefreshUpdateStatus(UPDATE_EVENT type, const string& data);

    int  ScanType();
    int  GetScanPath(std::list<pair<std::string,int> >& path);
    int  ScanFinished(){return 0;}


//    void OnClickedAutoProcVirus();
//    void OnClickedAutoPoweroffAfterScan();


//    void OnTabWidgetCurrentChanged(int nIndex);
//    void OnDefendStateChange(int nNormalState);
//    void OnClickedDefendOpenAll();




    void UpdateDefendCenterBtnInfo(int nOpened, int nTotal);

//    int SendToDaemon(int command);

    // from CenterCommandObserver
    //int OnCommandObserver(int event, void *args, void **result);

protected:
    void mousePressEvent(QMouseEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void resizeEvent(QResizeEvent *event);

signals:
    void sigUpdateMessage(int type, string sdata);
    void sigPauseContinueMessage();
    void sigStopScanMessage();

private slots:
    //void setMinimum();
    void popMenu();
    void trigerMenu(QAction *pAction);
    void trigerSetting();

    void OnClickUpdate();
    void OnClickUpdateLocal();
    void OnClickOfflineUpdateVirusDB();
    void OnClickedDefendCenter();
    void OnScaningTimer();
    void OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

    void popScanningSelectModeMenu();
    void trigerModeMenu(QAction *pAction);

    void ScanAllDisk(bool bServerCommand=false);
    void QuickScanPath(bool bServerCommand=false);
    void CustomScanPath(bool bServerCommand=false);
    void onSignaledPauseContinue();
    void onClickPauseContinueWindowScan(bool signaled = false);
    void onSignaledStopScan();
    void QuitStopState();
    void StopWindowsScan(bool signaled=false, bool bServerCommand=false);

public slots:
    void onUILogData();
    void onUIVirusLogData();
    void onUIScanStatusData();

    void onUIExit();

private:
    void initWindowsParam();
    void startAnimation(QWidget *target);

    void createMenu();

    void ShowAbout();
    int  FindVirusType(const string& virus, string& stype);

    void UpdateScanningIntelligentMode();

public:
    // add by pangxl
    int Construct();

private:
    HeaderWidget *mHeaderWidget;
    BodyWidget   *mBodyWidget;
    //DefendWidget    *m_pDefendWidget;
    DynMenu *mDynMenu;
    ModeMenu *mModeMenu;
    OfflineUpdateInfoDialog *mUpdateInfoDlg;
    /** record current scan type, for engine */
    int mCurrentScanType;
    //for screen state change
    int m_nBeforePausePrevScanState;

    int m_nScanItemsCount;
    int m_nScanedThreaten;
    int m_nScanedThreatenProcessed;
    QDateTime m_qdtStartTime;
    QTimer  *m_pTimer;

    QString m_sCurrentFile;
    //QTabWidget  *m_pTabWidget;
    /** virus area list data */
    //CircleLink<std::string> *mVirusShowData;
    CenterListener * m_pCenterListener;

    enum {
        SCAN_NOT_SET,
        SCAN_AUTO_KILL,
        SCAN_AUTO_IGNORE
    };

    VirusMessageDialog *m_pMsgBox;

    bool m_bMoveStart;
    bool m_bScrollStart;
    bool m_bDragging;
    QPoint  m_poMouseStartPos;
    QPoint  m_poFrameStartPos;

    QSystemTrayIcon *m_pSystemTrayIcon;

    int m_nScanReadyState;  //reference to RAVQT_SCAN_READY_STATE
    QMutex m_Mutex;
    bool m_bInited;
    int m_nScaningPercent;
    bool m_bIsScaning;
    UiCircularQueue m_UiQueue;

public:
    AntiVirusSetting *m_pAvConf;
    int m_nFunctions;
};

#endif // MAINWINDOW_H
