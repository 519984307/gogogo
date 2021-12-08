#ifndef MENU_H
#define MENU_H

#include "view.h"

#include <vector>
#include <string>

using namespace std;

class MenuClickObserver {
public:
    enum {
        MENU_FOCUS,
        MENU_ENTER
    };
public:
    virtual int OnMenuItemClicked(int menuID, int state, void* data) = 0;
};

class MenuItem {
public:
    MenuItem(int id, string name, string prompt) {
        mMenuID = id;
        mName = name;
        mPrompt = prompt;
    }

public:
    int  mMenuID;
    string mName;
    string mPrompt;
};

class PopMenu : public View {
public:
    PopMenu(int x, int y, int width, int height);
    void SetMenus(vector<MenuItem> *menus) {
        mMenuItems = menus;
    }

    void SetObserver(MenuClickObserver *observer) {
        mObserver = observer;
    }

    void Draw();
    int ProcessEvent(int event);

private:
    int mFocusItem;
    vector<MenuItem> *mMenuItems;

    MenuClickObserver   *mObserver;
};

class Menu
{
public:
    Menu(int id, string title);
    void AddSubMenu(const MenuItem& item) {
        mChildren.push_back(item);
    }

public:
    int mMenuID;
    string mTitle;
    vector<MenuItem> mChildren;

    bool mFocus;
};

class MenuBar : public View {
public:
    MenuBar(int x, int y, int width, int height);

    void AddMenu(Menu *menu);

    void SetFocus(int i);

    void Draw();

    int ProcessEvent(int event);

    void ShowMenu(Menu& menu);

    void SetObserver(MenuClickObserver *observer) {
        mObserver = observer;
    }

public:
    vector<Menu> mMenuItems;

private:
    int     mFocusItem;
    bool    mMenuShowed;

private:
    MenuClickObserver   *mObserver;
};

#endif // MENU_H
