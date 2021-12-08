#ifndef VIRUSDETAILGROUP_H
#define VIRUSDETAILGROUP_H

#include "common/pubdef.h"
#include "common/clickedlabel.h"

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>

#include <string>

using namespace std;

class LogEventEntry;
class VirusInfoEntry;
class CommEventLog;
class MainWindow;

class VirusDetailGroup : public QWidget
{
    Q_OBJECT
public:
    VirusDetailGroup(QWidget *parent = Q_NULLPTR);
    void InitControls();
    void SetMainWindowPtr(MainWindow* ptr){m_pMain=ptr;}
    
    void ResetTable();
    void InitData();
    int  FindVirusType(const string& virus, string& stype);
    void AddItem(VirusInfoEntry &info);

protected:
    void resizeEvent(QResizeEvent *event);

public:

    QLabel  *m_plDetail;
    QLabel  *m_plLine;
//    QLabel  *m_plTime;
//    QLabel  *m_plSrc;
//    QLabel  *m_plProcType;
//    QLabel  *m_plEventID;

//    QLabel  *m_plExportLog;
//    QComboBox   *m_pcTime;
//    QComboBox   *m_pcSrc;
//    QComboBox   *m_pcProcType;
//    QComboBox   *m_pcEventID;

    QTableWidget    *m_pTable;
    int m_nBlankItemFrom;
    int m_nBlankItemEnd;
    int m_nMaxIndex;

    MainWindow  *m_pMain;

    bool m_bInitedScreenData;
};

#endif // VIRUSDETAILGROUP_H
