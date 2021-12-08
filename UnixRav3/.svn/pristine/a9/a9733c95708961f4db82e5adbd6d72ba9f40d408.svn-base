#ifndef CONSOLESCREEN_H
#define CONSOLESCREEN_H

#include "menu.h"
#include "application.h"
#include "listview.h"

class View;
class TextView;
class TitleView;
class Window;
class MenuBar;
class Controller;

class MainView : public View {
public:
    MainView(int x, int y, int width, int height):View(x,y,width,height){}
    int ProcessEvent(int event);

    void SetObserver(MenuClickObserver *observer) {
        mMenuObserver = observer;
    }

private:
    MenuClickObserver *mMenuObserver;
};

class ScanLogView : public View {
public:
    ScanLogView(int x, int y, int width, int height);
    ~ScanLogView();
    void SetLogs(const std::string& log);
    void SetLogs(CircleLink<std::string>* logs) {
        mLogDatas = logs;
    }

public:
    void Draw();

private:
    CircleLink<std::string> *mLogDatas;
};

class Screen : public Application, public MenuClickObserver
{
public:
    static Screen* NEW();
    Screen();
    ~Screen();

public:
    friend class Controller;

public:
    void Show();

protected:
    int Construct();

    void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE status, int progress, void *data=NULL);
    void RefreshVirusLogs(const string& file, const string& virus, int/** VirusReportData::VirusStatus */ status);
    int  QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus);
    int  RefreshUpdateStatus(UPDATE_EVENT type, const string& data);

    int  ScanType();
    int  GetScanPath(std::list<pair<std::string,int> >& path);

    int LoadScanConfig() {
        return 0;
    }

    int ScanFinished() {
        return 0;
    }

private:
    void InitMenu(MenuBar *menuBar);
    void ResetAppFoot();

    void Exit();
    void ShowScanLog();
    void ShowVirusLog();
    void ShowUrlLog();
    void ShowBaseLog();
    void ShowQuarantineLog();
    void ShowWhiteList();
    void ShowDefenceCenter();
    void ShowUrlBlacklist();
    void ShowProcessBlacklist();
    void ShowFolderBlacklist();
    void ShowScanSettings();
    void ShowVirusSetting();
    void ShowScheduleQuickSetting();
    void ShowAdminSetting();
    void ShowUpdateSetting();
    void ShowAbout();

private:
    int OnMenuItemClicked(int menuID, int state, void* data);


private:
    int ParseScheduleString(const std::string& str, std::vector<int>& days, int& hour, int& minite);

private:
    Window      *mMainWindow;
    MainView    *mContainer;

    TitleView   *mTitleView;
    TextView    *mStatusView;
    TextView    *mFootView;
    MenuBar     *mMenuView;

    ScanLogView *mScanLogView;
    ScanLogView *mVirusLogView;

    /** record current scan type */
    int mCurrentScanType;

    /** virus area list data */
    CircleLink<std::string> *mVirusShowData;
};

#endif // CONSOLESCREEN_H
