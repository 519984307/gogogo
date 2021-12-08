#ifndef ISOLATIONGROUP_H
#define ISOLATIONGROUP_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <string>
#include <list>
#include <vector>
#include <map>

using namespace std;

class SearchEdit;
class PushButton;
class ClickedLabel;
class MainWindow;
class QuarantineLog;

class IsolationGroup : public QWidget
{
    Q_OBJECT
public:
    IsolationGroup(QWidget *parent = Q_NULLPTR);
    ~IsolationGroup();

    void InitControls();

    void SetMainWindowPtr(MainWindow* ptr){m_pMain=ptr;}

    void ResetTable();
    void InitData();
    void AddItem(QuarantineLog &info);

public slots:
    void RestoreToSourceQuery();
    void DeleteDest();

protected:
    void resizeEvent(QResizeEvent *event);
    void RestoreToSource(bool add_to_whitelist = false);

public:

    QLabel  *m_plTitle;
    QLabel  *m_plLine;
    QLabel  *m_plComment;
//    QLabel  *m_plSearch;

//    SearchEdit   *m_pseSearchTxt;
//    PushButton   *m_pbtnRefresh;

    QTableWidget    *m_pTable;

    //ClickedLabel    *m_pclAddToWhiteList;
    PushButton      *m_pbtnRestoreToSource;
    //PushButton      *m_pbtnRestoreTo;
    PushButton      *m_pDeleteSelected;

    int m_nMaxIndex;

    MainWindow  *m_pMain;
    vector<QuarantineLog*> m_logs;

    bool m_bInitedScreenData;
};

#endif // ISOLATIONGROUP_H
