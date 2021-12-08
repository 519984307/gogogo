#ifndef SCANEVENTGROUP_H
#define SCANEVENTGROUP_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>

#include "pubdef.h"


class LogEventEntry;
class CommEventLog;
class MainWindow;

class ScanEventGroup : public QWidget
{
    Q_OBJECT
public:
    ScanEventGroup(QWidget *parent = Q_NULLPTR);

    void InitControls();

    void SetMainWindowPtr(MainWindow* ptr){m_pMain=ptr;}

    void ResetTable();
    void InitData();
    void AddItem(LogEventEntry &info);

protected:
    void resizeEvent(QResizeEvent *event);

public:

    QLabel  *m_plScanEvent;
    QLabel  *m_plLine;
//    QLabel  *m_plTime;
//    QLabel  *m_plSrc;


//    QLabel  *m_plExportLog;
//    QComboBox   *m_pcTime;
//    QComboBox   *m_pcSrc;

    QTableWidget    *m_pTable;

    int m_nMaxIndex;

    MainWindow  *m_pMain;

    bool m_bInitedScreenData;
};

#endif // SCANEVENTGROUP_H
