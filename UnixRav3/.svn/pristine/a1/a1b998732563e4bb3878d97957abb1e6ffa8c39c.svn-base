#ifndef IPRULEGROUP_H
#define IPRULEGROUP_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include "Entry/netaccessentry.h"
#include "mainwindow.h"

#include <map>

using namespace std;

class MainWindow;

class IPRuleGroup : public QWidget
{
public:
    IPRuleGroup(QWidget *parent = Q_NULLPTR);

    void InitControls();

    void SetMainWindowPtr(MainWindow* ptr){m_pMain=ptr;}

    void ResetTable();
    void InitData();
    void AddItem(NetAccessEntry &info);

protected:
    void resizeEvent(QResizeEvent *event);

public:

    QLabel  *m_plTitle;
    QLabel  *m_plLine;
    //QLabel  *m_plTime;
    //QLabel  *m_plProtectType;


    //QLabel  *m_plExportLog;
    //QComboBox   *m_pcTime;
    //QComboBox   *m_pcProtectType;

    QTableWidget    *m_pTable;

    int m_nMaxIndex;

    //map<int, NetAccessEntry> m_logs;

    MainWindow  *m_pMain;
    bool m_bInitedScreenData;

};


#endif // IPRULEGROUP_H
