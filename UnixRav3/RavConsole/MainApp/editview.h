#ifndef EDITVIEW_H
#define EDITVIEW_H

#include "view.h"

#include <string>

class EditView : public View
{
public:
    EditView(const std::string& title, std::string& value, int len, int x, int y, int width, int height);

public:
    void Draw();
    int ProcessEvent(int event);

    void SetEditable(bool edit);
    void SetCursor();

    void RuleDrawString();

public:
    char mValue[1024];
    char mValidString[80];

    std::string& mResultValue;
    std::string mTitle;

private:
    // cursor in the text, the nCurInText max the mValue size
    int nCurInText;
    // curosr in text view
    // the nCurInScrn max pos is mLength
    int nCurInScrn;

    int mLength;

    bool mEditable;


};

#endif // EDITVIEW_H
