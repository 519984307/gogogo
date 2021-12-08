#include "titleview.h"
#include "config.h"

#include <string.h>

TitleView::TitleView(int x, int y, int width, int height) : TextView(x,y,width,height) {
    mMarginLeft = 1;
    SetFocus(true);
}

void TitleView::Draw() {
    wborder(window,ACS_VLINE|A_BOLD,ACS_VLINE|A_BOLD,ACS_HLINE|A_BOLD,ACS_HLINE|A_BOLD,ACS_ULCORNER|A_BOLD,ACS_URCORNER|A_BOLD,ACS_LLCORNER|A_BOLD,ACS_LRCORNER|A_BOLD);
    wattrset(window,A_REVERSE);
    mvwaddch(window,y,x,ACS_ULCORNER|A_BOLD);
    mvwhline(window,y,x+1,ACS_HLINE|A_BOLD,width-2);
    mvwaddch(window,y,x+width-1,ACS_URCORNER|A_BOLD);
    mvwaddch(window,y+1,x,ACS_VLINE|A_BOLD);
    mvwaddch(window,y+1,x+width-1,ACS_VLINE|A_BOLD);
    mvwaddch(window,y+2,x,ACS_VLINE|A_BOLD);
    mvwaddch(window,y+2,x+width-1,ACS_VLINE|A_BOLD);
    mvwhline(window,y+2,x+1,ACS_HLINE,width-2);

    TextView::Draw();
}

void TitleView::SetText(const char *message) {
    int length = STRING_LENGTH(message);
    if (length < width) {
        memset(mTitle, ' ', width);
        mTitle[width-2] = '\0';

        if (mTitleDrawMode|CENTER_HORIZONTAL) {
            memcpy(mTitle+(width-2-length)/2, message, length);
        }
        else {
            memcpy(mTitle, message, length);
        }
    }

    Redraw();
}

void TitleView::SetMode(int mode) {
    mTitleDrawMode = mode;

    if (mode&CENTER_HORIZONTAL) {
        mode  &= ~CENTER_HORIZONTAL;
    }
    TextView::SetMode(mode);
}

