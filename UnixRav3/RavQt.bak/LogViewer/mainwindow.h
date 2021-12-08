#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <string>
#include <list>
#include "common/pubdef.h"
#include "common/clickedlabel.h"
#include "circlelink.h"
#include "application.h"

using namespace std;


enum _enter_function_stage {
    ENTER_FUNCTION_STAGE_VIRUS_DETAIL = 1,
    ENTER_FUNCTION_STAGE_SCAN_EVENTS,
    ENTER_FUNCTION_STAGE_ISOLATION,
    ENTER_FUNCTION_STAGE_URL_ACCESS,
    ENTER_FUNCTION_STAGE_IP_RULES,
    ENTER_FUNCTION_STAGE_INSTALLATION,
    ENTER_FUNCTION_STAGE_PLATFORM
};

class VirusReportData;
class Storage;
class RavQuaFile;
class LogEventEntry;
class VirusInfoEntry;
class CommEventLog;
class QuarantineLog;
class ActiveDefenConfig;
class UpdateConfig;
class ScheduleEntry;
class UrlRuleEntry;
class NetAccessEntry;
class ClickedLabel;
class TopWidget;
class LeftWidget;
class RightWidget;
class GlobalConf;
class VirusDetailGroup;

//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow, public Application
{
    Q_OBJECT
public:
    friend class Controller;
    explicit MainWindow(int function_stage = ENTER_FUNCTION_STAGE_VIRUS_DETAIL, QWidget *parent = nullptr);
    ~MainWindow();

    void InitWindow();

     void InitAllScreenData();

    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, void **result);

public slots:
    void OnClickedLeftKill();
    void OnClickedLeftSetup();

    //void InitVDetailData();

    void OnClickedLeftVDetail();


    void OnClickedLeftScanEvent();
    void OnClickedLeftNetDefence();
//    void OnClickedLeftAppReinforce();
    void OnClickedLeftIsolation();
    void OnClickedLeftUrlAccess();
    void OnClickedLeftIPRule();
    void OnClickedLeftInstall();
    void OnClickedLeftSysLog();

    void setMinimum();
    void OnActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void OnMaxState();

protected:
    void paintEvent(QPaintEvent *pPEvent);

    void mousePressEvent(QMouseEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void resizeEvent(QResizeEvent *event);

    int JumpToStage(int function_stage);
    int Construct();

private slots:


private:
    //Ui::MainWindow *ui;
    bool m_bMoveStart;
    bool m_bScrollStart;
    bool m_bDragging;
    QPoint  m_poMouseStartPos;
    QPoint  m_poFrameStartPos;

    CenterListener *m_pCenterListener;

    TopWidget   *m_pTopWidget;
    LeftWidget  *m_pLeftWidget;
    RightWidget *m_pRightWidget;


    GlobalConf  *m_pScreenConf;

    QByteArray m_baGeometry;
    QSize      m_OldSize;//for MainWindow
    QSize      m_qsLeftSize;
    QSize      m_qsRightSize;

    int m_nTopSelectedIndex;
    int m_nMidSelectedIndex;
    int m_nEndSelectedIndex;

    int m_nFunctionStage;

    Storage             *mStorage;
    RavQuaFile          *mQuaArea;

    QSystemTrayIcon     *m_pSystemTrayIcon;

    int m_nFunctions; //if show usb-monitor and active defend, value: 127 or 31
};

#endif // MAINWINDOW_H
