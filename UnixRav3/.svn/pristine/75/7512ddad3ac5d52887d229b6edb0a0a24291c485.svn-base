#ifndef VIEW_H
#define VIEW_H

#include <curses.h>
#include <list>

class Window;
class View
{
public:
    View(int x, int y, int width, int height);
    View() : window(NULL), x(0), y(0),width(0),height(0),focus(false) {
    }

    virtual ~View();

public:
    void SetBoderMode(bool boder, bool shadow, int mode) {
        this->boder = boder;
        this->shadow = shadow;
        this->mode = mode;
    }

    void SetWindow(Window *window);

    void SetParent(View *view) {
        mParentView = view;
    }

    virtual void SetFocus(bool focus) {
        this->focus = focus;
        Redraw();
    }

    virtual void Redraw();

    inline unsigned int Width() const {
        return width;
    }
    inline unsigned int Height() const {
        return height;
    }

    void AddView(View *view);

public:
    virtual void Draw();
    virtual int ProcessEvent(int event);

private:
    void DrawBoder() const;

protected:
    Window  *mWindow;
    WINDOW  *window;

    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;

    int mode;

    bool boder;
    bool shadow;
    bool focus;

    std::list<View*>    mChildrenView;
    View                *mParentView;
};

#endif // VIEW_H
