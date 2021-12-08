#ifndef OPTIONVIEW_H
#define OPTIONVIEW_H

#include "view.h"

#include <string>
#include <vector>

class EditView;
class TextView;

class OptionPopView : public View {
public:
    OptionPopView(int &value, int x, int y, int width, int height);
    void SetOptions(std::vector<std::string> *options) {
        mOptionItems = options;
    }

    virtual void Draw();
    int  ProcessEvent(int event);

protected:
    virtual std::string MakeOptionItem(int pos, const std::string& data) {
        if (mSelectedFlag>>pos&0x1) {
            return "[*] " + data;
        }
        else {
            return "[ ] " + data;
        }
    }
    virtual int OnItemSelected() = 0;

public:
    virtual int GetValue() = 0;

protected:
    int mFocusItem;
    std::vector<std::string> *mOptionItems;

    // select flag
    int mSelectedFlag;

    int& mValue;
};

class RadioOptionPopView : public OptionPopView {
public:
    RadioOptionPopView(int &value, int x, int y, int width, int height):OptionPopView(value, x,y,width,height) {
        mSelectedFlag = 0;
        mSelectedFlag |= 0x1<<mValue;
    }

protected:
    int OnItemSelected() {
        mSelectedFlag = 0;
        mSelectedFlag |= 1<<mFocusItem;
        return -1;
    }
public:
    int GetValue() {
        int value = 0;
        while (mSelectedFlag>>value++);
        return value-2;
    }
};

class MultiChoiceOptionPopView : public OptionPopView {
public:
    MultiChoiceOptionPopView(int& value, int x, int y, int width, int height):OptionPopView(value, x,y,width,height) {
        mSelectedFlag = mValue;
    }
protected:
    int OnItemSelected() {
        if (mSelectedFlag>>mFocusItem&0x1) {
            mSelectedFlag &= ~(1<<mFocusItem);
        }
        else {
            mSelectedFlag |= 1<<mFocusItem;
        }
        return 1;
    }
public:
    int GetValue() {
        return mSelectedFlag;
    }
};

class TextView;
class OptionView : public View
{
public:
    OptionView(const std::string& title, const std::vector<std::string>& chioces, int& value, int x, int y, int width, int height);

    void SetTitle(const std::string& title);
    void SetChoices(const std::vector<std::string>& chioces);
    enum ChoicsMode {
        RADIO,
        MULTI,
        INPUT
    };

    void SetMode(enum ChoicsMode mode);

    void SetFocus(bool focus);

public:
    int ProcessEvent(int event);

protected:
    virtual void ShowOption(int& defaultSelect);

    int GetChoicesMaxLength(std::vector<std::string> chioces);

private:
    TextView    *mTitleView;
    TextView    *mValueView;

private:
    std::vector<std::string> mChoices;
    std::string mTitle;

    enum ChoicsMode mChoiceMode;

    int& mOptionValue;
};


#endif // OPTIONVIEW_H
