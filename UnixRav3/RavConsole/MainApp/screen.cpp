#include "screen.h"
#include "view.h"
#include "window.h"
#include "textview.h"
#include "titleview.h"
#include "logreportview.hpp"
#include "settingview.h"
#include "aboutview.h"
#include "menu.h"
#include "dialog.h"
#include "resources.h"
#include "logdatasupporter.h"
#include "circlelink.h"
#include "appconfig.h"

#include "scanconfig.h"
#include "Common/updateconfig.h"
#include "Entry/logevententry.h"
#include "Entry/scheduleentry.h"
#include "Entry/commonentry.h"

#include <locale.h>
#include <string.h>
#include <iostream>

int MainView::ProcessEvent(int event) {
    switch (event) {
    case 27: {
//        if (mMenuObserver) {
//            mMenuObserver->OnMenuItemClicked(MENU_EXIT_ID, MenuClickObserver::MENU_ENTER, NULL);
//        }
        break;
    }
    default:
        return View::ProcessEvent(event);
    }
    return 0;
}

ScanLogView::ScanLogView(int x, int y, int width, int height):View(x,y,width,height){
    mLogDatas = NULL;
}

ScanLogView::~ScanLogView() {
    if (mLogDatas) {
        delete mLogDatas;
    }
}

void ScanLogView::SetLogs(const string& log) {
    if (NULL == mLogDatas) {
        mLogDatas = CircleLink<string>::NEW(20);
    }
    mLogDatas->Append(log);
}

void ScanLogView::Draw() {
    if (mLogDatas) {
        wattrset(window,A_NORMAL);

        CircleLinkNode<string> *node = mLogDatas->Datas();
        char temp[77] = {' '};
        temp[76] = '\0';

        char valid[77] = {0};
        for (unsigned int line=y+height-1; line>=y; --line) {
            memset(temp, ' ', 77);
            temp[76] = '\0';
            if (node->node.size() > 76) {
                sprintf(valid, "%s...........%s", node->node.substr(0, 30).c_str(), node->node.substr(node->node.length()-35).c_str());
                
            }
            else {
                strcpy(valid, node->node.c_str());
            }

            memcpy(temp, valid, strlen(valid));
            mvwprintw(window,line,x+1,temp);
            node = node->previous;
        }
    }
}

Screen* Screen::NEW() {
    Screen *self = new Screen;
    if (self) {
        int err = self->Construct();
        if (err) {
            delete self;
            self = NULL;
        }
    }
    return self;
}
Screen::Screen() : Application(Application::NORMAL_APP) {
    mVirusShowData = CircleLink<std::string>::NEW(3);

    initscr();
    noecho();
    crmode();

    setlocale(LC_ALL, "");

    mMainWindow = new Window(0,0,80,24);

    mContainer = new MainView(0,0,80,24);
    mContainer->SetBoderMode(true, false, A_BOLD);
    mContainer->SetObserver(this);
    mMainWindow->AddView(mContainer);

    mTitleView = new TitleView(0, 0, 80, 3);
    mContainer->AddView(mTitleView);
    mTitleView->SetMode(TitleView::CENTER_VERTICEL);
    mTitleView->SetText(APP_NAME);

    mMenuView = new MenuBar(1, 3, 78, 1);
    mContainer->AddView(mMenuView);
    InitMenu(mMenuView);

    mMenuView->SetFocus(0);
    mMenuView->SetObserver(this);

    mStatusView = new TextView(1, 10, 60, 1);
    mContainer->AddView(mStatusView);
    mStatusView->SetText("");

    mFootView = new TextView(1, 22, 60, 1);
    mContainer->AddView(mFootView);

    mScanLogView = new ScanLogView(1, 12, 78, 9);
    mContainer->AddView(mScanLogView);

    mVirusLogView = new ScanLogView(1, 5, 78, 4);
    mContainer->AddView(mVirusLogView);

    View *s1 = new View(1, 4, 78, 0);
    s1->SetBoderMode(true, false, A_BOLD);
    mContainer->AddView(s1);

    View *s2 = new View(1, 21, 78, 0);
    s2->SetBoderMode(true, false, A_BOLD);
    mContainer->AddView(s2);

    View *s3 = new View(1, 9, 78, 0);
    s3->SetBoderMode(true, false, A_BOLD);
    mContainer->AddView(s3);

    View *s4 = new View(1, 11, 78, 0);
    s4->SetBoderMode(true, false, A_BOLD);
    mContainer->AddView(s4);    
}

int Screen::Construct() {
    //
    string config = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(config)) {
        config = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(config)) {
            return -1;
        }
    }

    mAppConfig = AppConfig::NEW(config);
    if(!mAppConfig) {
        return -1;
    }

    int err = Application::Construct(mAppConfig);
    if (0 == err) {
        Application::ResetClientProxy(false);

        InitListener(FOLDER_RJJH_GJCZ, RJJH_MAJOR_SOCKET);
    }

    ResetAppFoot();
    return 0;
}

Screen::~Screen() {
    delete mMainWindow;
    delete mAppConfig;
}

void Screen::Show() {
    mMainWindow->CreateWindow();
    mMainWindow->Loop();
}

void Screen::InitMenu(MenuBar *menuBar) {
    Menu menu1(0, STRING_OPERATE_MENU);
    menu1.AddSubMenu(MenuItem(MENU_QUICK_SCAN_ID, STRING_OPERATE_QUICK_SCAN, ""));
    menu1.AddSubMenu(MenuItem(MENU_ALL_SCAN_ID, STRING_OPERATE_ALL_SCAN, ""));
    menu1.AddSubMenu(MenuItem(MENU_CUSTOM_SCAN_ID, STRING_OPERATE_CUSTOM_SCAN, ""));
    menu1.AddSubMenu(MenuItem(MENU_STOP_SCAN_ID, STRING_OPERATE_STOP_SCAN, ""));
    menu1.AddSubMenu(MenuItem(MENU_EXIT_ID, STRING_OPERATE_EIXT, ""));
    menuBar->AddMenu(&menu1);

    Menu menu2(0, STRING_LOG_MENU);
    menu2.AddSubMenu(MenuItem(MENU_SCAN_LOG_ID, STRING_LOG_SCAN, ""));
    menu2.AddSubMenu(MenuItem(MENU_VIRUS_LOG_ID, STRING_LOG_VIRUS, ""));
    menu2.AddSubMenu(MenuItem(MENU_URL_LOG_ID, STRING_LOG_URL, ""));
    menu2.AddSubMenu(MenuItem(MENU_BASE_LOG_ID, STRING_LOG_BASE, ""));
    menuBar->AddMenu(&menu2);

    Menu menu3(MENU_QUARANTINE_ID, STRING_QUARANTINE);
    menuBar->AddMenu(&menu3);

    Menu menu4(0, STRING_SETTING_MENU);
    menu4.AddSubMenu(MenuItem(MENU_SCAN_SETT_ID, STRING_SETTING_SCAN, ""));
    menu4.AddSubMenu(MenuItem(MENU_VIRUS_SETT_ID, STRING_SETTING_VIRUS, ""));
#ifdef ALL_VERSION
    menu4.AddSubMenu(MenuItem(MENU_PROC_BLOKCK_ID, STRING_SETTING_PROCESSBL, ""));
    menu4.AddSubMenu(MenuItem(MENU_WHITELIST_ID, STRING_SETTING_WHITELIST, ""));
    menu4.AddSubMenu(MenuItem(MENU_BLACKURL_ID, STRING_SETTING_BLACKLIST, ""));
    menu4.AddSubMenu(MenuItem(MENU_FOLDER_MONITOR, STRING_SETTING_FOLDERL, ""));
#endif
    menu4.AddSubMenu(MenuItem(MENU_SCHEDULE_QU_ID, STRING_SCHEDULE_QUICK_SET, ""));
//    menu4.AddSubMenu(MenuItem(MENU_SCHEDULE_FU_ID, STRING_SCHEDULE_FULL_SET, ""));
#ifdef ALL_VERSION
    menu4.AddSubMenu(MenuItem(MENU_DEFENCE_ID, STRING_SETTING_DEFENCE, ""));
    menu4.AddSubMenu(MenuItem(MENU_ADMIN_PWD_ID, STRING_SETTING_ADMIN, ""));
#endif
//    menu4.AddSubMenu(MenuItem(MENU_UPGRADE_SET_ID, STRING_SETTING_UPGRADE, ""));
    menuBar->AddMenu(&menu4);

    Menu menu5(0, STRING_ABOUT_MENU);
    menu5.AddSubMenu(MenuItem(MENU_UPGRAGE_ID, STRING_ABOUT_UPGRADE, ""));
    menu5.AddSubMenu(MenuItem(MENU_UPGRAGE_OFF_ID, STRING_ABOUT_UPGRADE_OFFLINE, ""));
    menu5.AddSubMenu(MenuItem(MENU_ABOUT_ID, STRING_ABOUT_ABOUT, ""));
    menuBar->AddMenu(&menu5);
}

int Screen::OnMenuItemClicked(int menuID, int state, void* data) {
    if (state == MENU_ENTER) {
        switch (menuID) {
        case MENU_QUICK_SCAN_ID: {
            mCurrentScanType = LogEventEntry::QUICK_DISK;
            mVirusShowData->Reset("");
            QuickScan(true);
            break;
        }
        case MENU_ALL_SCAN_ID: {
            mCurrentScanType = LogEventEntry::ALL_DISK;
            mVirusShowData->Reset("");
            FullScan(true);
            break;
        }
        case MENU_CUSTOM_SCAN_ID: {
            mCurrentScanType = LogEventEntry::USER_DISK;
            mVirusShowData->Reset("");

            string params;
            SingleInputView *view = new SingleInputView(params, 5,5,40,4);
            Dialog::Builder build;
            Dialog* dialog = build.SetTitle("")
                             .SetLayout(view)
                             .SetPositiveButton(STRING_BUTTON_YES)
                             .SetNegativeButton(STRING_BUTTON_NO)
                             .Build();
            if (dialog->DoModel() == Dialog::DIALOG_OK) {
                if (!view->GetContent().empty()) {
                    std::list<pair<std::string,int> > paths;
                    paths.push_back(make_pair(view->GetContent(), -1));
                    StartScan(paths, true);
                }
            }

            break;
        }
        case MENU_STOP_SCAN_ID: {
            StopScan();
            break;
        }
        case MENU_EXIT_ID:
            Exit();
            break;
        case MENU_SCAN_LOG_ID:
            ShowScanLog();
            break;
        case MENU_VIRUS_LOG_ID:
            ShowVirusLog();
            break;
        case MENU_URL_LOG_ID:
            ShowUrlLog();
            break;
        case MENU_BASE_LOG_ID:
            ShowBaseLog();
            break;
        case MENU_QUARANTINE_ID:
            ShowQuarantineLog();
            break;
        case MENU_SCAN_SETT_ID:
            ShowScanSettings();
            break;
        case MENU_VIRUS_SETT_ID:
            ShowVirusSetting();
            break;
        case MENU_PROC_BLOKCK_ID:
            ShowProcessBlacklist();
            break;
        case MENU_FOLDER_MONITOR:
            ShowFolderBlacklist();
            break;
        case MENU_DEFENCE_ID:
            ShowDefenceCenter();
            break;
        case MENU_WHITELIST_ID:
            ShowWhiteList();
            break;
        case MENU_BLACKURL_ID:
            ShowUrlBlacklist();
            break;
        case MENU_SCHEDULE_QU_ID:
            ShowScheduleQuickSetting();
            break;
        case MENU_SCHEDULE_FU_ID:
            break;
        case MENU_ADMIN_PWD_ID:
            ShowAdminSetting();
            break;
        case MENU_UPGRADE_SET_ID:
            ShowUpdateSetting();
            break;
        case MENU_UPGRAGE_ID: {
        	if (mAppConfig->GetProductType() == 0) {
        		mActived = (Registed() == 0)?1:0;
        		if(!mActived) {
	        		Dialog::Builder    builder;
	        		UpdateActiveView * view = new UpdateActiveView(5,5,40,4);
	        		Dialog           * dialog = 0;

	        		builder.SetTitle("unactivated cannot upgrade ...");
	        		builder.SetLayout(view);
	        		builder.SetPositiveButton(STRING_BUTTON_YES);
	        		builder.SetNegativeButton(STRING_BUTTON_NO);
	        		dialog = builder.Build();
					if (dialog->DoModel() == Dialog::DIALOG_OK && !view->GetSerial().empty()) {
						std::string info = view->GetSerial();
						if(!view->GetLicPath().empty()) info += "|"+view->GetLicPath(); 
						int  nRet = Register(info);
						char szText[128] = {0};
						sprintf(szText, "accredit %s(%d)", nRet == 0? "ok":"failed", nRet);
						mStatusView->SetText(szText);
					}
					break;
				}
        	}
        	std::string cmd;
       		mStatusView->SetText("Start Updating ...");
			Update(false, cmd);
        	break;
       	}
        case MENU_UPGRAGE_OFF_ID: {
            // get the offline
            string params;
            SingleInputView *view = new SingleInputView(params, 5,5,40,4);
            Dialog::Builder build;
            Dialog* dialog = build.SetTitle("offline file path")
                             .SetLayout(view)
                             .SetPositiveButton(STRING_BUTTON_YES)
                             .SetNegativeButton(STRING_BUTTON_NO)
                             .Build();
            if (dialog->DoModel() == Dialog::DIALOG_OK) {
                if (!view->GetContent().empty()) {
                    string path = view->GetContent();
                    Update(true, path);
                }
            }
            break;
        }
        case MENU_ABOUT_ID:
            ShowAbout();
            break;
        default:
            break;
        }
    }

    mMainWindow->Refresh();
    return 0;
}

void Screen::ResetAppFoot() {
    char temp[128] = {0};
    sprintf(temp, "%s: %s \t %s: %s" ,FOOTER_VERSION, GetVersion().c_str(), FOOTER_UPDATE_TIME, GetUpdateTime().c_str());
    mFootView->SetText(temp);
}

//void Screen::RefreshScanLogs(CircleLink<std::string> *data) {
//    mScanLogView->SetLogs(data);
//    mScanLogView->Redraw();
//}

//void Screen::RefreshScanStatus(SCAN_STATUS_TYPE scantype, int progress, void *data) {
//    if (scantype == TOTLE_PROGRESS) {
//        char temp[80] = {0};
//        sprintf(temp, "%s: %d %%", SCAN_PROGRESS_NAME, progress);
//        mStatusView->SetText(temp);
//    }
//    else if (scantype == THREAD_STATUS) {
//        vector<ReportCache>* cache = (vector<ReportCache>*)data;
//        for (int i=0; i<cache->size(); ++i) {
//            printf("%d thread scan %d file, current is %s\n", (*cache)[i].mTag, (*cache)[i].mCount, (*cache)[i].mFile.c_str());
//        }
//    }
//}

void Screen::RefreshScanStatus(int scantype, SCAN_STATUS_TYPE status, int progress, void *data) {
    if (THREAD_STATUS == status) {
        vector<ReportCache>* cache = (vector<ReportCache>*)data;
        int count = 0;
        for (int i=0; i<(int)cache->size(); ++i) {
            mScanLogView->SetLogs((*cache)[i].mFile);
            count += (*cache)[i].mCount;
        }
        mScanLogView->Redraw();

        char buf[80] = {0};
        snprintf(buf, 79, "%s: %d %% ==> %d thread has scaned %d file", SCAN_PROGRESS_NAME, progress, (int)cache->size(), count);

        mStatusView->SetText(buf);
    }
    else if (SCAN_FINISHED == status) {
        mStatusView->SetText(SCAN_FINISH_PROMPT);
    }
    else if (SCAN_ERROR_EXIT == status) {
        mStatusView->SetText(SCAN_ERROR_PROMPT);
    }
    else if (SCAN_PREPARE == status) {
        mStatusView->SetText(SCAN_PREPARE_PROMPT);
    }
}

void Screen::RefreshVirusLogs(const string& file, const string& virus, int status) {
    std::string strVirus = virus + "   " + file;
    mVirusShowData->Append(strVirus);

    mVirusLogView->SetLogs(mVirusShowData);
    mVirusLogView->Redraw();
}

int  Screen::QueryUserForResult(QUERY_EVENT event, const string& file, const string& virus) {
    return 0;
}

void Screen::Exit() {
    Dialog::Builder build;
    Dialog* dialog = build.SetTitle(STRING_PROMPT)
                     .SetMessage(STRING_EXIT_PROM)
                     .SetPositiveButton(STRING_BUTTON_YES)
                     .SetNegativeButton(STRING_BUTTON_NO)
                     .Build();
    if (dialog->DoModel() == Dialog::DIALOG_OK) {
        mMainWindow->Exit();
    }
    delete dialog;
}

int Screen::RefreshUpdateStatus(UPDATE_EVENT type, const string& data) {
    if (type == UPDATE_VLIB_SUCCESS) {
        string sversion = Application::GetVirusDbVer();
        string result = string("Update vlib finished, version is " + sversion);
        mStatusView->SetText(result.c_str());
    }else if (type == UPDATE_VLIB__FAILED) {
        mStatusView->SetText("Update vlib error, please check it");
    }
    else if (type == UPDATE_DOWNLOAD_RP) {
        string result = string("Download rp file " + data);
        mStatusView->SetText(result.c_str());
    }
    else if (type  == UPDATE_COMBINE_RP) {
        string result = string("Combine rp file " + data);
        mStatusView->SetText(result.c_str());
    }
    else if (type == UPDATE_APP_DOWNLOAD) {
        mStatusView->SetText("Download app file");
    }
    else if (type == UPDATE_APP_SUCCESS) {
        mStatusView->SetText("Update app finished, restart waiting...");
    }
    else if (type == UPDATE_APP_FAILED) {
        mStatusView->SetText("Update app failed");
    }
    else if (type == UPDATE_FINISHED) {
        mStatusView->SetText(data.c_str());
    }
    return 0;
}

void Screen::ShowScanLog() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    ScanLogDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    ScanLogReportView *view = new ScanLogReportView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_LOG_SCAN);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowVirusLog() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;


    VirusLogDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    VirusLogReportView *view = new VirusLogReportView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_LOG_VIRUS);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowUrlLog() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;


    UrlLogDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    NetAccesslistView *view = new NetAccesslistView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_LOG_URL);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowBaseLog() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    BaseLogDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    BaseLogReportView *view = new BaseLogReportView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_LOG_BASE);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowQuarantineLog() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    QuarantineDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    QuarantineReportView *view = new QuarantineReportView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_QUARANTINE);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowWhiteList() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    WhiteListDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    WhitelistView *view = new WhitelistView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_SETTING_WHITELIST);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowUrlBlacklist() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    UrlBlackListDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    WhitelistView *view = new WhitelistView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_SETTING_BLACKLIST);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    bridge.StoreToFile();

    mMainWindow->Refresh();
}

void Screen::ShowProcessBlacklist() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    ProcessBlackListDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    WhitelistView *view = new WhitelistView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_SETTING_PROCESSBL);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowFolderBlacklist() {
    int logWidth = mContainer->Width() - 5;
    int logHeight = mContainer->Height() - 2;

    FolderBlackListDataSupporter bridge(this);

    Window *win = new Window(3, 1, logWidth, logHeight);

    WhitelistView *view = new WhitelistView(0, 0, logWidth, logHeight);
    view->SetDataSource((ListViewStringDelegate*)&bridge);
    view->SetTitle(STRING_SETTING_FOLDERL);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    mMainWindow->Refresh();
}

void Screen::ShowScanSettings() {
    AntiVirusSetting config;
    if (0 == GetAntiVirusConfig(config, AntiVirusSetting::VIRUS_SCAN)) {
        ScanSettingView *view = new ScanSettingView(config, 5,5,40,4+3);

        bool updated = false;
        Dialog::Builder build;
        Dialog* dialog = build.SetTitle(STRING_PROMPT)
                         .SetLayout(view)
                         .SetPositiveButton(STRING_BUTTON_YES)
                         .SetNegativeButton(STRING_BUTTON_NO)
                         .Build();
        if (dialog->DoModel() == Dialog::DIALOG_OK) {
            updated = true;
        }
        delete dialog;

        if (updated) {
            UpdateAntiVirusConfig(config);
        }
    }
}

void Screen::ShowVirusSetting() {
    AntiVirusSetting config;
    GetAntiVirusConfig(config, AntiVirusSetting::VIRUS_BACKUP);

    VirusSettingView *view = new VirusSettingView(config, 5,5,40,4+3);

    bool updated = false;
    Dialog::Builder build;
    Dialog* dialog = build.SetTitle(STRING_PROMPT)
                     .SetLayout(view)
                     .SetPositiveButton(STRING_BUTTON_YES)
                     .SetNegativeButton(STRING_BUTTON_NO)
                     .Build();
    if (dialog->DoModel() == Dialog::DIALOG_OK) {
        updated = true;
    }
    delete dialog;

    if (updated) {
        UpdateAntiVirusConfig(config);
    }
}

void Screen::ShowDefenceCenter() {
    ActiveDefenConfig config;
    GetDefenConfig(config);

    int status = GetDaemonStatus();


    DefenceSettingView *view = new DefenceSettingView(config, 5,5,40,4+3);

    Dialog::Builder build;
    Dialog* dialog = build.SetTitle(STRING_PROMPT)
                     .SetLayout(view)
                     .SetPositiveButton(STRING_BUTTON_YES)
                     .SetNegativeButton(STRING_BUTTON_NO)
                     .Build();
    if (dialog->DoModel() == Dialog::DIALOG_OK) {
        SaveDefenConfig(config);


    }
    delete dialog;
}

void Screen::ShowScheduleQuickSetting() {
    int scanPos = 0;
    int scanType = 0;

    ScheduleEntry entry;
    GetScheduleEntry((int)ScheduleEntry::QUICK_SCAN, entry);

    // map the value
    scanType = entry.mType;

    ScheduleSetView *view = new ScheduleSetView(scanType, scanPos, 5,5,40,4+3);

    Dialog::Builder build;
    Dialog* dialog = build.SetTitle(STRING_PROMPT)
                     .SetLayout(view)
                     .SetPositiveButton(STRING_BUTTON_YES)
                     .SetNegativeButton(STRING_BUTTON_NO)
                     .Build();
    if (dialog->DoModel() == Dialog::DIALOG_OK) {
        std::string paramsTitle = INPUT_SCHEDULE_PARAMS;
        if (scanType == ScheduleEntry::DAYS) {
            paramsTitle += "({9:00})";
        }
        else if (scanType == ScheduleEntry::WEEKS) {
            paramsTitle += "({1,2|9:00})";
        }
        else if (scanType == ScheduleEntry::MONTH) {
            paramsTitle += "({1,2|9:00})";
        }
        else {
            return;
        }

        if (scanPos == 1) {
            entry.mFuncEnum = ScheduleEntry::ALL_SCAN;
        }

        std::string params;
        if (scanType == entry.mType) {
            if (scanType == ScheduleEntry::DAYS) {
                std::string hour, minute;
                Utils::String::itoa(entry.GetCrontabHour(), hour);
                Utils::String::itoa(entry.GetCrontabMinute(), minute);
                params = "{" + hour + ":" + minute + "}";
            }
            else if (scanType == ScheduleEntry::WEEKS) {
                std::vector<int> days;
                std::string temp;
                if (entry.GetCrontabDayInWeek(days) > 0) {
                    std::vector<int>::iterator iter;
                    for (iter=days.begin(); iter!=days.end(); ++iter) {
                        temp += Utils::String::itoa(*iter);
                        temp += ",";
                    }
                    temp = temp.substr(0, temp.size()-1);
                }

                std::string hour, minute;
                Utils::String::itoa(entry.GetCrontabHour(), hour);
                Utils::String::itoa(entry.GetCrontabMinute(), minute);
                params = "{" + temp + "|" + hour + ":" + minute + "}";
            }
            else if (scanType == ScheduleEntry::MONTH) {
                std::string day, hour, minute;
                Utils::String::itoa(entry.GetCrontabDayInMonth(), day);
                Utils::String::itoa(entry.GetCrontabHour(), hour);
                Utils::String::itoa(entry.GetCrontabMinute(), minute);
                params = "{" + day + "|" + hour + ":" + minute + "}";
            }
        }

        SingleInputView *view = new SingleInputView(params, 5,5,40,4);
        Dialog::Builder build;
        Dialog* dialog = build.SetTitle(paramsTitle)
                         .SetLayout(view)
                         .SetPositiveButton(STRING_BUTTON_YES)
                         .SetNegativeButton(STRING_BUTTON_NO)
                         .Build();
        if (dialog->DoModel() == Dialog::DIALOG_OK) {
            if (view->GetContent().empty()) {
                return;
            }

            std::vector<int> days;
            int hour = 0, minuite = 0;
            if (Utils::String::StartsWith(params, "{") && Utils::String::EndsWith(params, "}")) {
                ParseScheduleString(params.substr(1, params.size() -2), days, hour, minuite);

                entry.mCrontab = 0;
                entry.SetCrontabMinute(minuite);
                entry.SetCrontabHour(hour);
                entry.mType = (ScheduleEntry::ScheduleType)scanType;
                if (scanType == ScheduleEntry::WEEKS) {
                    entry.SetCrontabDayInWeek(days);
                }
                else if (scanType == ScheduleEntry::MONTH) {
                    entry.SetCrontabDayInMonth(days[0]);
                }
                SetScheduleEntry(entry, true);
            }
        }
        delete dialog;
    }
    delete dialog;
}

void Screen::ShowAdminSetting() {
    AdminPasswdView *view = new AdminPasswdView(5,5,40,4+1);

    Dialog::Builder build;
    Dialog* dialog = build.SetTitle(STRING_PROMPT)
                     .SetLayout(view)
                     .SetPositiveButton(STRING_BUTTON_YES)
                     .SetNegativeButton(STRING_BUTTON_NO)
                     .Build();
    if (dialog->DoModel() == Dialog::DIALOG_OK) {

    }
    delete dialog;
}

void Screen::ShowUpdateSetting() {
    UpdateConfig config;
    if (0 == GetUpdateConfig(config)) {
        UpdateSetView *view = new UpdateSetView(config, 5,5,40,4+3);
        Dialog::Builder build;
        Dialog* dialog = build.SetTitle(STRING_PROMPT)
                         .SetLayout(view)
                         .SetPositiveButton(STRING_BUTTON_YES)
                         .SetNegativeButton(STRING_BUTTON_NO)
                         .Build();
        if (dialog->DoModel() == Dialog::DIALOG_OK) {
            // TODO
            // SaveUpdateConfig(config);
        }
        delete dialog;
    }
}

void Screen::ShowAbout() {
    AboutView *view = new AboutView(
                          GetVersion(),
                          GetUpdateTime(),
                          GetVirusDbVer(),
                          GetVirusUpdateTime(),
                          5,5,40,4+5);
    Dialog::Builder build;
    Dialog* dialog = build.SetTitle(STRING_APP_TITLE).SetLayout(view).Build();
    if (dialog->DoModel() == Dialog::DIALOG_OK) {

    }
    delete dialog;
}

int Screen::ScanType() {
    return LogEventEntry::SetScanType(LogEventEntry::MANUAL_SCAN, (LogEventEntry::DiskType)mCurrentScanType);
}

int Screen::GetScanPath(std::list<pair<std::string,int> >& path) {
    if (mCurrentScanType == LogEventEntry::QUICK_DISK) {
        path.push_back(make_pair("/usr", 1));
    }
    else if (LogEventEntry::ALL_DISK == mCurrentScanType) {
        path.push_back(make_pair("/usr", 1));
    }
    return 0;
}

int Screen::ParseScheduleString(const std::string& str, std::vector<int>& days, int& hour, int& minite) {
    std::vector<std::string> array;
    Utils::String::Split(str, "|", &array);

    std::string time = array[0];
    if (array.size() > 1) {
        time = array[1];
        //
        std::vector<std::string> child;
        Utils::String::Split(array[0], ",", &child);
        std::vector<std::string>::iterator iter;
        for (iter=child.begin(); iter!=child.end(); ++iter) {
            int value =atoi(iter->c_str());
            if (value > 0 && value < 31) {
                days.push_back(value);
            }
        }
    }

    {
        std::vector<std::string> child;
        Utils::String::Split(time, ":", &child);
        if (child.size() == 2) {
            hour = atoi(child[0].c_str());
            minite = atoi(child[1].c_str());
        }
    }
    return 0;
}
