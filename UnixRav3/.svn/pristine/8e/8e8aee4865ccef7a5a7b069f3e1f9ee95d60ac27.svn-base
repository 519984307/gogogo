#ifndef DIALOG_H
#define DIALOG_H

#include <string>

#include "window.h"

class View;
class Button;
class Dialog : public Window
{
protected:
    Dialog(std::string title, std::string message, std::string positive, std::string negative, View *view);

    void Measure();
    void Layout();
    int  ProcessEvent();

public:
    int DoModel();
    enum {
        DIALOG_CANCEL = 0,
        DIALOG_OK
    };

public:
    class Builder {
    public:
        Builder() {
            mLayoutView = NULL;
        }
        Builder& SetTitle(const std::string& title) {
            mTitle = title;
            return *this;
        }
        Builder& SetMessage(const std::string& message) {
            mMessage = message;
            return *this;
        }
        Builder& SetPositiveButton(const std::string& title) {
            mPositiveButton = title;
            return *this;
        }
        Builder& SetNegativeButton(const std::string& title) {
            mNegativeButton = title;
            return *this;
        }
        Builder& SetLayout(View *view) {
            mLayoutView = view;
            return *this;
        }

        Dialog* Build() {
            return new Dialog(mTitle, mMessage, mPositiveButton, mNegativeButton, mLayoutView);
        }

    public:
        std::string mTitle;
        std::string mMessage;
        std::string mPositiveButton;
        std::string mNegativeButton;
        View        *mLayoutView;
    };
protected:
    View    *mContainer;
    Button  *mPositiveButton;
    Button  *mNegativeButton;

private:
    std::string mTitle;
    std::string mMessage;
    std::string mPositiveText;
    std::string mNegativeText;
    View        *mLayoutView;

    enum CommandState{
        NONE = -1,
        YES  = 1,
        NO   = 0
    };
    enum CommandState mCommandState;
};

#endif // DIALOG_H
