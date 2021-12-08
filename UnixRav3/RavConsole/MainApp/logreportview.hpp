#ifndef LOGREPORTVIEW_H
#define LOGREPORTVIEW_H

#include "view.h"
#include "listview.h"
#include "titleview.h"
#include "resources.h"

#include <string>
#include <list>

using namespace std;

#include "listview.h"

class TitleView;

class LogReportView : public View
{
public:
    LogReportView(int x, int y, int width, int height) : View(x,y,width,height) {
        SetBoderMode(true, true, A_BOLD);

        mTitleView = new TitleView(0,0,width,3);
        mTitleView->SetMode(TitleView::CENTER_VERTICEL);
        mListView = new ListView(0,1,width, height-1);

        AddView(mTitleView);
        AddView(mListView);
    }

public:
     void SetTitle(const string& title) {
        mTitleView->SetText(title.c_str());
    }

    void SetDataSource(ListViewStringDelegate *delegate) {
        mListView->SetDataSource(delegate);
    }

protected:
    void DrawScrollBar() const {
        // TODO: if you want add scrollbar, the draw it here
        // const int validHeigth = this->height - 5;
    }

public:
    void Draw() {
        mTitleView->Draw();
        mListView->Draw();
    }

    int  ProcessEvent(int event) {
        switch (event) {
        case 27:
            return -1;            
        default:
            return mListView->ProcessEvent(event);
        }
        return 0;
    }

protected:
    TitleView   *mTitleView;
    ListView    *mListView;
};

class ScanLogReportView : public LogReportView {
public:
    ScanLogReportView(int x, int y, int width, int height) : LogReportView(x,y,width,height) {
        mListView->AddSection(STRING_LIST_ID, 10);
        mListView->AddSection(STRING_LIST_TIME,25);
        mListView->AddSection(STRING_LIST_SOURCE, 15);
        mListView->AddSection(STRING_LIST_STATUS, 20);
        mListView->AddSection(STRING_LIST_COUNT, 10);
        mListView->AddSection(STRING_LIST_VIRUS, 10);
        mListView->AddSection(STRING_LIST_DEAL, 10);
    }
};

class VirusLogReportView : public LogReportView {
public:
    VirusLogReportView(int x, int y, int width, int height) : LogReportView(x,y,width,height) {
        mListView->AddSection(STRING_LIST_ID, 7);
        mListView->AddSection(STRING_LIST_TIME,25);
        mListView->AddSection(STRING_LIST_PATH, 25);
        mListView->AddSection(STRING_LIST_VNAME, 25);
        mListView->AddSection(STRING_LIST_VTYPE, 10);
        mListView->AddSection(STRING_LIST_BLONE, 8);
    }
};

class BaseLogReportView : public LogReportView {
public:
    BaseLogReportView(int x, int y, int width, int height) : LogReportView(x,y,width,height) {
        mListView->AddSection(STRING_LIST_ID, 7);
        mListView->AddSection(STRING_LIST_TIME,25);
        mListView->AddSection(STRING_LIST_LOG_TYPE, 13);
        mListView->AddSection(STRING_LIST_LOG_ACTION, 20);
        mListView->AddSection(STRING_LIST_LOG_SOURCE, 20);
        mListView->AddSection(STRING_LIST_LOG_RESULT, 15);
    }
};

class QuarantineReportView : public LogReportView {
public:
    QuarantineReportView(int x, int y, int width, int height) : LogReportView(x,y,width,height) {
        mListView->AddSection(STRING_LIST_ID, 10);
        mListView->AddSection(STRING_LIST_TIME,25);
        mListView->AddSection(STRING_LIST_PATH, 40);
        mListView->AddSection(STRING_LIST_VNAME, 25);
    }
};

class WhitelistView : public LogReportView {
public:
    WhitelistView(int x, int y, int width, int height) : LogReportView(x,y,width,height) {
        mListView->AddSection(STRING_LIST_ID, 15);
        mListView->AddSection(STRING_LIST_PATH,50);
    }
};

class NetAccesslistView : public LogReportView {
public:
    NetAccesslistView(int x, int y, int width, int height) : LogReportView(x,y,width,height) {
        mListView->AddSection(STRING_LIST_ID, 10);
        mListView->AddSection(STRING_LIST_TIME,25);
        mListView->AddSection(STRING_LIST_PATH, 40);
    }
};

#endif // LOGREPORTVIEW_H
