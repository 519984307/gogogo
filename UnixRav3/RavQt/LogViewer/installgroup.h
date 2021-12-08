#ifndef INSTALL_GROUP_H
#define INSTALL_GROUP_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>

#include "../Storage/Entry/commeventlog.h"

class MainWindow;
class VirusInfoEntry;


class InstallGroup : public QWidget
{
public:
    InstallGroup(QWidget *parent = Q_NULLPTR);

    void InitControls();


    void SetMainWindowPtr(MainWindow* ptr){m_pMain=ptr;}

    void ResetTable();
    void InitData();
    void AddItem(const CommEventLog& see);

protected:
    void resizeEvent(QResizeEvent *event);

public:

    QLabel  *m_plTitle;
    QLabel  *m_plLine;
//    QLabel  *m_plTime;
//    QLabel  *m_plAction;
//    QLabel  *m_plEntry;

//    QComboBox   *m_pcTime;
//    QComboBox   *m_pcAction;
//    QComboBox   *m_pcEntry;

    QTableWidget    *m_pTable;

    int m_nMaxIndex;


    MainWindow  *m_pMain;

    bool m_bInitedScreenData;
};


#endif // INSTALL_GROUP_H
