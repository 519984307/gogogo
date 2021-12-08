#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "view.h"
#include "listdatadelegate.h"

#include <string>

class ListViewStringDelegate : public ListViewDataDelegate<std::string> {
public:
    virtual int Count() = 0;
    virtual void Content(int pos, std::string& data) = 0;
    virtual void AddData(const std::string& data) = 0;
    virtual void DeleteData(int pos) = 0;
};

class ListView : public View
{
public:
    ListView(int x, int y, int width, int height);

    void AddSection(const std::string& section, int percent=0);
    void SetTitle(const std::string& title);

    void SetDataSource(ListViewStringDelegate *data);
    void Redraw();

public:
    void Draw();
    int ProcessEvent(int event);

protected:
    void DrawSection();
    void DrawTitle() const;
    void DrawLine();

private:
    unsigned int mDataCount;
    int mCurrentIndex;

    class ListItemSection {
    public:
        std::string name;
        int percent;

        ListItemSection(const std::string& name, int percent) {
            this->name = name;
            this->percent = percent;
        }
    };
    std::list<ListItemSection> mSections;

    std::string mTitle;

    ListViewStringDelegate    *mDelegate;

    int mStartIndex;
    int mEndIndex;
    int mListAvalidHeight;
};

#endif // LISTVIEW_H
