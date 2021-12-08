#include "listview.h"

ListView::ListView(int x, int y, int width, int height) : View(x,y,width,height) {
    SetBoderMode(true, true, A_BOLD);
    mDelegate = NULL;

    mStartIndex = 0;
    mEndIndex = 0;
    mDataCount = 0;
    mCurrentIndex = 0;
}

void ListView::AddSection(const std::string& section, int percent) {
    mSections.push_back(ListItemSection(section, percent));
}


void ListView::SetTitle(const std::string& title) {
    mTitle = title;
}

void ListView::SetDataSource(ListViewStringDelegate *data) {
    if (data) {
        mDelegate = data;
        mDataCount = data->Count();

        if (mSections.size() > 0) {
            mListAvalidHeight = height-5;
        }
        else {
            mListAvalidHeight = height-3;
        }

        mStartIndex = 0;
        mEndIndex = mStartIndex + mListAvalidHeight -1;
        if (mEndIndex >= mDataCount) {
            mEndIndex = mDataCount-1;
        }
    }
}


void ListView::Draw() {
    DrawTitle();
    DrawSection();

    if (mSections.size() > 0) {
        mvwhline(window,y+3,x+1,ACS_HLINE|A_BOLD,width-2);
    }

    DrawLine();
}
int ListView::ProcessEvent(int event) {
    switch (event) {
    case KEY_UP:
        mCurrentIndex--;
        if (mCurrentIndex < 0) {
            mCurrentIndex = 0;
        }
        if (mCurrentIndex < mStartIndex) {
            mStartIndex = mCurrentIndex;
            mEndIndex = mStartIndex+mListAvalidHeight-1;
        }
        break;
    case KEY_DOWN:
        mCurrentIndex++;
        if (mCurrentIndex >= mDataCount) {
            mCurrentIndex = mDataCount-1;
        }
        if (mCurrentIndex > mEndIndex) {
            mEndIndex = mCurrentIndex;
            mStartIndex = mEndIndex-mListAvalidHeight+1;
        }
        break;
    case KEY_ENTER:
    case 10:
    case 13: {
        if (NULL != mDelegate) {
            mDelegate->SelectData(mCurrentIndex);
        }
        Redraw();
        break;
    case KEY_DC:
    case KEY_BACKSPACE: {
        if (NULL != mDelegate) {
            mDelegate->DeleteData(mCurrentIndex);
        }
        Redraw();
        break;
    }
    case 27:
        break;
    }
    default:
        break;
    }
    Redraw();
    return 1;
}

void ListView::DrawSection() {
    wattrset(window,A_NORMAL);

    const int vw = this->width - 3;
    int pos = 2;
    std::list<ListItemSection>::iterator iter;
    for (iter=mSections.begin(); iter!=mSections.end(); ++iter) {
        int one_width = vw * (*iter).percent / 100;
        mvwprintw(window,3,pos,(*iter).name.c_str());

        pos += one_width;
    }
}

void ListView::DrawTitle() const {
    mvwprintw(window, 1, (this->width-mTitle.size())/2,mTitle.c_str());
}

void ListView::DrawLine() {
    if (NULL == mDelegate) {
        return;
    }

    std::string data;
    for (int i=mStartIndex; i<=mEndIndex; i++) {
        if (i == mCurrentIndex) {
            wattrset(window,A_REVERSE);
        }
        else {
            wattrset(window,A_NORMAL);
        }

        mDelegate->Content(i, data);
        mvwprintw(window, i-mStartIndex+5, 2, data.c_str());
    }
}

void ListView::Redraw() {
    mDataCount = mDelegate->Count();

    mEndIndex = mStartIndex + mListAvalidHeight -1;
    if (mEndIndex >= mDataCount) {
        mEndIndex = mDataCount-1;
    }

    View::Redraw();
}
