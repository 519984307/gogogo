#include "menu.h"
#include "window.h"
#include "config.h"


PopMenu::PopMenu(int x, int y, int width, int height) : View(x,y,width,height) {
    SetBoderMode(true, true, A_BOLD);

    mFocusItem = 0;
    mMenuItems = NULL;
    mObserver = NULL;
}

void PopMenu::Draw() {
    View::Draw();

    vector<MenuItem>::iterator iter;
    int index = 0;
    if (mMenuItems) {
        for (iter=mMenuItems->begin(),index=0; iter!=mMenuItems->end(); ++iter,++index) {
            if (index == mFocusItem) {
                wattrset(window,A_REVERSE);
            }
            else {
                wattrset(window,A_NORMAL);
            }

            mvwprintw(window, y+index+1, x+1, (*iter).mName.c_str());
        }
    }
}

int PopMenu::ProcessEvent(int event) {
    switch (event) {
    case KEY_UP:
        mFocusItem--;
        if (mFocusItem < 0) {
            mFocusItem = 0;
        }
        if (mObserver) {
            mObserver->OnMenuItemClicked(mMenuItems->at(mFocusItem).mMenuID, MenuClickObserver::MENU_FOCUS, &mMenuItems->at(mFocusItem));
        }
        break;
    case KEY_DOWN:
        mFocusItem++;
        if (mFocusItem >= (int)mMenuItems->size()) {
            mFocusItem = mMenuItems->size()-1;
        }
        if (mObserver) {
            mObserver->OnMenuItemClicked(mMenuItems->at(mFocusItem).mMenuID, MenuClickObserver::MENU_FOCUS, &mMenuItems->at(mFocusItem));
        }
        break;
    case 13:
    case 10:
    case KEY_ENTER: {
        if (mObserver) {
            mObserver->OnMenuItemClicked(mMenuItems->at(mFocusItem).mMenuID, MenuClickObserver::MENU_ENTER, &mMenuItems->at(mFocusItem));
        }
        return -1;
    case 27:
        return -1;
    }
    default:
        break;
    }
    Redraw();
    return 0;
}

Menu::Menu(int id, string title) {
    mMenuID  = id;
    mTitle = title;
    mFocus = false;
}

MenuBar::MenuBar(int x, int y, int width, int height) : View(x,y,width,height) {
    mFocusItem = 0;
    mMenuShowed = false;
    mObserver = NULL;
}

void MenuBar::AddMenu(Menu *menu) {
    mMenuItems.push_back(*menu);
}

void MenuBar::Draw() {
    int menuWidth = (mParentView->Width()-10)/mMenuItems.size();
    vector<Menu>::iterator iter;
    int index = 0;
    for (iter=mMenuItems.begin(); iter!=mMenuItems.end(); ++iter, ++index) {
        if (index == mFocusItem) {
            wattrset(window,A_REVERSE);
        }
        else {
            wattrset(window,A_NORMAL);
        }

        mvwprintw(window, y, x+5+index*menuWidth,(*iter).mTitle.c_str());
    }
}

void MenuBar::SetFocus(int i) {
    vector<Menu>::iterator iter;
    int index = 0;
    for (iter=mMenuItems.begin(),index=0; iter!=mMenuItems.end(); ++iter, index++) {
        (*iter).mFocus = i==index;
    }
}

int MenuBar::ProcessEvent(int event) {
    switch (event) {
    case KEY_LEFT:
        mFocusItem--;
        if (mFocusItem < 0) {
            mFocusItem = 0;
        }
        SetFocus(mFocusItem);

        if (mObserver) {
            mObserver->OnMenuItemClicked(mMenuItems[mFocusItem].mMenuID, MenuClickObserver::MENU_FOCUS, &mMenuItems[mFocusItem]);
        }
        break;
    case KEY_RIGHT:
        mFocusItem++;
        if (mFocusItem >= (int)mMenuItems.size()) {
            mFocusItem = mMenuItems.size()-1;
        }
        SetFocus(mFocusItem);
        if (mObserver) {
            mObserver->OnMenuItemClicked(mMenuItems[mFocusItem].mMenuID, MenuClickObserver::MENU_FOCUS, &mMenuItems[mFocusItem]);
        }
        break;
    case 13:
    case 10:
    case KEY_ENTER: {
        if (!mMenuShowed) {
            Menu& menu = mMenuItems[mFocusItem];
            if (menu.mMenuID > 0) {
                mObserver->OnMenuItemClicked(menu.mMenuID, MenuClickObserver::MENU_ENTER, &menu);
            }
            else {
                ShowMenu(menu);
            }
        }
        break;
    }
    default:
        break;
    }
    Redraw();
    return 0;
}

void MenuBar::ShowMenu(Menu& menu) {
    int menuWidth = (mParentView->Width()-10)/mMenuItems.size();
    Window *win = new Window(x+5+mFocusItem*menuWidth-1, y+2, 20, menu.mChildren.size()+3);

    PopMenu *view = new PopMenu(0, 0, 20, menu.mChildren.size()+3);
    view->SetMenus(&menu.mChildren);
    view->SetObserver(mObserver);
    win->AddView(view);

    win->CreateWindow();
    win->Loop();

    delete win;

    touchwin(window);
    wrefresh(window);
}
