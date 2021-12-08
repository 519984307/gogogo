#include "window.h"
#include "view.h"

#include <unistd.h>

Window::Window(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    mExitFlag = false;
    window = NULL;

    initscr();
}

Window::~Window() {
    std::list<View*>::iterator iter;
    for (iter=views.begin(); iter!=views.end(); ++iter) {
        delete (*iter);
    }
    views.clear();
    if (window) {
        delwin(window);
    }
    endwin();
}

void Window::CreateWindow() {
    Measure();
    window = newwin(height,width,y,x);
    std::list<View*>::iterator iter;
    for (iter=views.begin(); iter!=views.end(); ++iter) {
        (*iter)->SetWindow(this);
    }
    Layout();
    Draw();
}

void Window::AddView(View *view) {
    views.push_back(view);
    view->SetWindow(this);
}

void Window::Draw() {
    std::list<View*>::iterator iter;
    for (iter=views.begin(); iter!=views.end(); ++iter) {
        (*iter)->Draw();
    }

    wrefresh(window);
}

void Window::Measure() {

}

void Window::Layout() {

}

void Window::Loop() {
    keypad(window,TRUE);
    ProcessEvent();
}

void::Window::Refresh() {
    touchwin(window);
    wrefresh(window);
}

int Window::ProcessEvent() {
    int event;
    while (!mExitFlag) {
        event = wgetch(window);
        switch (event) {
        default: {
            std::list<View*>::iterator iter;
            for (iter=views.begin(); iter!=views.end(); ++iter) {
                int resp = (*iter)->ProcessEvent(event);
                if (resp > 0) {
                    break;
                }
                else if (resp < 0) {
                    return -1;
                }
            }
        }
        }
        Draw();
    }
    return 0;
}
