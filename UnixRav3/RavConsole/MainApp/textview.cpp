#include "textview.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

TextView::TextView(int x, int y, int width, int height) : View(x,y,width,height) {
    mTitle = new char[width];
    mBuffer = new char[width];
    mDrawMode = DEFAULT_LAYOUT;

    mMarginLeft = 0;
    mMarginRight = 0;
    mMarginTop = 0;
    mMarginBottom = 0;
}

TextView::~TextView() {
    delete[] mTitle;
    delete[] mBuffer;
}

void TextView::SetText(const char *message) {
    memset(mTitle, ' ', width-1);
    mTitle[width-1] = '\0';

    snprintf(mBuffer, 79, message);

    memcpy(mTitle, mBuffer, strlen(message));
    Redraw();
}

void TextView::Draw() {
    int dx = x, dy = y;
    if (mDrawMode&CENTER_VERTICEL) {
        dy = y + height/2;
    }

    if (mDrawMode&CENTER_HORIZONTAL) {
        dx = x + width/2;
    }

    if (focus) {
        wattrset(window,A_REVERSE);
    }
    else {
        wattrset(window,A_NORMAL);
    }
    mvwprintw(window,dy,dx+mMarginLeft,mTitle);
}


