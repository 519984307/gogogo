#ifndef SYSLOG_GROUP_H
#define SYSLOG_GROUP_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>

class CommEventLog;
class MainWindow;


class SysLogGroup : public QWidget
{
    Q_OBJECT
public:
    SysLogGroup(QWidget *parent = Q_NULLPTR);

    void SetMainWindow(QWidget * pMainWindow);

    void InitControls();

    void ResetTable();
    void InitData();
    void AddItem(const CommEventLog& see);

protected:
    void resizeEvent(QResizeEvent *event);
public:

    QLabel  *m_plTitle;
    QLabel  *m_plLine;
//    QLabel  *m_plTime;

//    QComboBox   *m_pcTime;

    QTableWidget    *m_pTable;

    MainWindow  *m_pMain;

    bool m_bInitedScreenData;

};


#endif // SYSLOG_GROUP_H
