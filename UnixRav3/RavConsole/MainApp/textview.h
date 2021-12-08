#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "view.h"

class TextView : public View
{
public:
    TextView(int x, int y, int width, int height);
    ~TextView();

public:
    void SetText(const char *message);

    void Draw();

    enum {
        DEFAULT_LAYOUT = 0,
        CENTER_VERTICEL = 1,
        CENTER_HORIZONTAL = 2,
        CENTER = CENTER_VERTICEL|CENTER_HORIZONTAL,
    };

    void SetMode(int mode) {
        mDrawMode = mode;
    }

    void SetMargin(int left, int right, int top, int bottom) {
        mMarginLeft = left;
        mMarginRight = right;
        mMarginTop = top;
        mMarginBottom = bottom;
    }

protected:
    char *mTitle;
    char *mBuffer;

    int mDrawMode;

    int mMarginLeft;
    int mMarginRight;
    int mMarginTop;
    int mMarginBottom;
};

class Button : public TextView {
public:
    Button(int x, int y, int width, int height):TextView(x,y,width,height){}
};

#endif // TEXTVIEW_H
