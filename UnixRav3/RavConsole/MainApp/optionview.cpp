#include "optionview.h"
#include "textview.h"
#include "window.h"

OptionPopView::OptionPopView(int &value, int x, int y, int width, int height) : mValue(value), View(x,y,width,height) {
    SetBoderMode(true, true, A_BOLD);

    mOptionItems = NULL;
    mFocusItem = 0;
    mSelectedFlag = 0;
}

void OptionPopView::Draw() {
    View::Draw();

    std::vector<std::string>::iterator iter;
    int index = 0;
    if (mOptionItems) {
        for (iter=mOptionItems->begin(),index=0; iter!=mOptionItems->end(); ++iter,++index) {
            if (index == mFocusItem) {
                wattrset(window,A_REVERSE);
            }
            else {
                wattrset(window,A_NORMAL);
            }

            mvwprintw(window, y+index+1, x+1, MakeOptionItem(index, *iter).c_str());
        }
    }
}

int OptionPopView::ProcessEvent(int event) {
    switch (event) {
    case KEY_UP:
        mFocusItem--;
        if (mFocusItem < 0) {
            mFocusItem = 0;
        }

        break;
    case KEY_DOWN:
        mFocusItem++;
        if (mFocusItem >= (int)mOptionItems->size()) {
            mFocusItem = mOptionItems->size()-1;
        }

        break;
    case 27:
        return -1;
    case 13:
    case 10:
    case KEY_ENTER: {
        return OnItemSelected();
        break;
    }
    default:
        break;
    }
    Redraw();
    return 0;
}

OptionView::OptionView(const std::string& title, const std::vector<std::string>& choices, int& value, int x, int y, int width, int height)
    : View(x,y,width,height), mOptionValue(value) {
    int ml = GetChoicesMaxLength(choices);

    mTitleView = new TextView(x, y, title.size()+2, 1);
    mValueView = new TextView(x+title.size()+3, y, ml, 1);

    SetTitle(title);
    SetChoices(choices);

    AddView(mTitleView);
    AddView(mValueView);
}

void OptionView::SetTitle(const std::string& title) {
    mTitle = title;
    mTitleView->SetText(title.c_str());
}

void OptionView::SetChoices(const std::vector<std::string>& choices) {
    mChoices = choices;
}

void OptionView::SetMode(ChoicsMode mode) {
    mChoiceMode = mode;
    if (mChoiceMode == RADIO) {
        mValueView->SetText(mChoices[mOptionValue].c_str());
    }
    else if (mChoiceMode == MULTI) {
        std::string all;
        for (int i=0; i<mChoices.size(); i++) {
            if (mOptionValue>>i & 0x1) {
                char temp[32];
                sprintf(temp, "%d", i+1);
                all += /*mChoices[i]*/ temp;
                all += ";";
            }
        }
        mValueView->SetText(all.c_str());
    }
}

int OptionView::ProcessEvent(int event) {
    switch(event) {
    case 10:
    case 13:
    case KEY_ENTER:
        ShowOption(mOptionValue);
        SetMode(mChoiceMode);
        return 1;
    case 27:
        return -1;
    case KEY_DOWN:
        break;
    case KEY_UP:
        break;
    }
    return 0;
}

void OptionView::SetFocus(bool focus) {
    mTitleView->SetFocus(focus);
}

int OptionView::GetChoicesMaxLength(std::vector<std::string> choices) {
    int maxWidth = 0;
    std::vector<std::string>::iterator iter;
    for (iter=choices.begin();iter!=choices.end(); ++iter) {
        int w = (*iter).size();
        if (w > maxWidth) {
            maxWidth = w;
        }
    }
    return maxWidth+4;
}

void OptionView::ShowOption(int& defaultSelect) {
    int maxWidth = GetChoicesMaxLength(mChoices);
    Window *win = new Window(mWindow->GetX()+mTitle.size()+6, y+3, maxWidth+5, mChoices.size() + 3);

    OptionPopView *view = NULL;
    if (mChoiceMode == RADIO) {
        view = new RadioOptionPopView(defaultSelect, 0, 0, maxWidth+5, mChoices.size()+3);
    }
    else {
        view = new MultiChoiceOptionPopView(defaultSelect, 0, 0, maxWidth+5, mChoices.size()+3);
    }
    view->SetOptions(&mChoices);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    defaultSelect = view->GetValue();

    delete win;

    touchwin(window);
    wrefresh(window);
}

