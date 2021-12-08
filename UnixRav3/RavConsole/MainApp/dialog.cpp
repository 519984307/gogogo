#include "dialog.h"
#include "config.h"
#include "window.h"
#include "view.h"
#include "textview.h"
#include "resources.h"

Dialog::Dialog(std::string title, std::string message, std::string positive, std::string negative, View *view) {
    mTitle = title;
    mMessage = message;
    mPositiveText = positive;
    mNegativeText = negative;
    mLayoutView = view;

    mCommandState = NONE;
    if (!mNegativeText.empty()) {
        mCommandState = NO;
    }
    if (!mPositiveText.empty()) {
        mCommandState = YES;
    }

    width = MAX_WIDTH/2;
    height = 6; //default 5, include title edge, message edge and so on

    if (mLayoutView) {
        width = mLayoutView->Width() + 6;
        height = mLayoutView->Height() + 4;
    }

    if (!mTitle.empty()) {
        height += 1;
    }
    if (!mMessage.empty()) {
        height += 1;
    }

    x = (MAX_WIDTH-width)/2;
    y = (MAX_HEIGHT-height)/2;

    window = newwin(height,width,y,x);

    mContainer = new View(0, 0, width, height);
    mContainer->SetBoderMode(true, true, A_BOLD);
    AddView(mContainer);

    Layout();
}

void Dialog::Measure() {

}

void Dialog::Layout() {
    if (!mTitle.empty()) {
        int tx = (width-mTitle.size()-3)/2;
        TextView *titleView = new TextView(tx, 1, mTitle.size()+2,1);
        titleView->SetText(mTitle.c_str());
        AddView(titleView);
    }

    if (!mMessage.empty()) {
        int tx = (width-mMessage.size()-3)/2;
        TextView *messageView = new TextView(tx,3, mMessage.size()+2,1);
        messageView->SetText(mMessage.c_str());
        AddView(messageView);
    }

    if (mLayoutView) {
        AddView(mLayoutView);
    }

    if (mCommandState == NONE) {
        std::string closeTitle = STRING_CLOSE_DLG_PROMPT;
        int tx = (width-closeTitle.size()-2)/2;
        int ty = mLayoutView?mLayoutView->Height()+2:5;
        TextView *closeView = new TextView(tx, ty, closeTitle.size()+2,1);
        closeView->SetText(closeTitle.c_str());
        AddView(closeView);
    }
    else {
        mPositiveButton = new Button(width/2-width/4-mPositiveText.size()/2, height-3, mPositiveText.size()+2,1);
        mPositiveButton->SetText(mPositiveText.c_str());
        AddView(mPositiveButton);
        mPositiveButton->SetFocus(true);

        mNegativeButton = new Button(width/2+width/4-mNegativeText.size()/2,height-3, mNegativeText.size()+2,1);
        mNegativeButton->SetText(mNegativeText.c_str());
        AddView(mNegativeButton);
    }
}

int Dialog::DoModel() {
    Draw();

    keypad(window,TRUE);
    return ProcessEvent();
}

int Dialog::ProcessEvent() {
    int event;
    while (1) {
        event = wgetch(window);
        if (mLayoutView) {
            int err = mLayoutView->ProcessEvent(event);
            if (err > 0) {
                continue;
            }
            else if (err < 0) {
                return NO;
            }
        }

        switch (event) {
        case 27:
            return 0;
        case KEY_LEFT:
        case KEY_RIGHT:
            if (mCommandState != NONE) {
                mCommandState = (enum CommandState)(int)(!bool(mCommandState));
                mPositiveButton->SetFocus(mCommandState);
                mNegativeButton->SetFocus(!mCommandState);
            }
            break;
        case 13:
        case 10:
            return mCommandState;
        default: {
        }
        }
        Draw();
    }

}

