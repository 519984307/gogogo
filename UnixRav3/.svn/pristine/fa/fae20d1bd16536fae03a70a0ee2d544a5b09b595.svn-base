#ifndef WINDOW_H
#define WINDOW_H

#include "curses.h"

#include <list>

class View;
class Window
{
public:
    Window(int x, int y, int width, int height);
    Window(){
        x = 0;
        y = 0;
        width = 0;
        height = 0;
        mExitFlag = false;
        window = NULL;
    }

    virtual ~Window();

public:
    void AddView(View *view);
    void AddSeprator(int x, int y, int width);

    void CreateWindow();
    void Loop();
    void Refresh();

    void Exit() {
        mExitFlag = true;
    }

    WINDOW* GetWindow() const {
        return this->window;
    }

    int GetX() const {
        return x;
    }

protected:
    virtual void Draw();
    virtual int  ProcessEvent();
    virtual void Measure();
    virtual void Layout();

private:


protected:
    WINDOW  *window;

    int x;
    int y;
    int width;
    int height;

    std::list<View*> views;

    bool mExitFlag;
};

#endif // WINDOW_H
