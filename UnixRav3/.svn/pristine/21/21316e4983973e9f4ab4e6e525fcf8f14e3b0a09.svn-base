#include "view.h"
#include "window.h"

View::View(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->mode = A_BOLD;
    this->window = NULL;

    this->boder = false;
    this->focus = false;
}

View::~View() {
    std::list<View*>::iterator iter;
    for (iter=mChildrenView.begin(); iter!=mChildrenView.end(); ++iter) {
        delete (*iter);
    }
    mChildrenView.clear();
}

void View::SetWindow(Window *window) {
    mWindow = window;
    this->window = window->GetWindow();
    std::list<View*>::iterator iter;
    for (iter=mChildrenView.begin(); iter!=mChildrenView.end(); ++iter) {
        (*iter)->SetWindow(window);
    }
}

void View::Draw() {
    if (boder) {
        DrawBoder();
    }

    std::list<View*>::iterator iter;
    for (iter=mChildrenView.begin(); iter!=mChildrenView.end(); ++iter) {
        (*iter)->Draw();
    }
}

int View::ProcessEvent(int event) {
    std::list<View*>::iterator iter;
    for (iter=mChildrenView.begin(); iter!=mChildrenView.end(); ++iter) {
        int respond = (*iter)->ProcessEvent(event);
        if (respond) {
            return respond;
        }
    }
    return 0;
}

void View::DrawBoder() const {
    if (height>0) {
        int dw = width;
        int dh = height;
        if (shadow) {
            dw -= 1;
            dh -= 1;

            wattrset(window,A_REVERSE);
            for(unsigned int i=1;i<height;i++) {
                mvwprintw(window,i,width-1," ");
            }
            for (unsigned int i=1; i<width-1; ++i) {
                mvwprintw(window, height-1, i," ");
            }
        }

        wattrset(window,A_NORMAL);
        mvwaddch(window,y,x,ACS_ULCORNER|mode);
        mvwhline(window,y,x+1,ACS_HLINE|mode,dw-2);
        mvwaddch(window,y,x+dw-1,ACS_URCORNER|mode);
        mvwaddch(window,y+dh-1,x,ACS_LLCORNER|mode);
        mvwhline(window,y+dh-1,x+1,ACS_HLINE|mode,dw-2);
        mvwaddch(window,y+dh-1,x+dw-1,ACS_LRCORNER|mode);
        for(int i=1;i<dh-1;i++) {
            mvwaddch(window,y+i,x,ACS_VLINE|mode);
            mvwaddch(window,y+i,x+dw-1,ACS_VLINE|mode);
        }
    }
    else {
        wattrset(window,A_NORMAL);
        mvwhline(window,y,x,ACS_HLINE|A_BOLD,width);
    }
}

void View::Redraw() {
    Draw();

    wrefresh(window);
}

void View::AddView(View *view) {
    bool found = false;
    std::list<View*>::iterator iter;
    for (iter=mChildrenView.begin();iter!=mChildrenView.end();++iter) {
        if (view == (*iter)) {
            found = true;
            break;
        }
    }
    if (!found) {
        mChildrenView.push_back(view);
        view->SetParent(this);
    }
}
