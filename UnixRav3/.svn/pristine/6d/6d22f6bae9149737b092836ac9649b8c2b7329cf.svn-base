#include "editview.h"

#include <string.h>

EditView::EditView(const std::string& title, std::string& value, int len, int x, int y, int width, int height)
    : View(x,y,width,height), mResultValue(value) {
    memset(mValue, 0, 1024);

    memset(mValidString, ' ', len);
    mValidString[len] = '\0';

    strcpy(mValue, value.c_str());

    mTitle = title;
    mLength = len;

    nCurInScrn = value.size()>mLength?mLength:value.size();
    nCurInText = value.size();

    RuleDrawString();
}

void EditView::RuleDrawString() {
    memset(mValidString, ' ', mLength);
    mValidString[mLength] = '\0';

    int length = strlen(mValue);
    if (length > mLength) {
        memcpy(mValidString, mValue+(length-mLength), mLength);
    }
    else {
        memcpy(mValidString, mValue, length);
    }
}

void EditView::Draw() {
    if (focus) {
        wattrset(window,A_REVERSE);
    }
    else {
        wattrset(window,A_NORMAL);
    }
    mvwprintw(window, y, x+2, mTitle.c_str());

    wattrset(window,A_NORMAL|A_UNDERLINE);
    mvwprintw(window, y, x+5+mTitle.size(), mValidString);
}

int EditView::ProcessEvent(int nKey) {
    switch(nKey){
    case KEY_LEFT:
        if( nCurInText ){
            nCurInText--;
            if( nCurInScrn )
                nCurInScrn--;         
        }
        SetCursor();
        break;
    case KEY_RIGHT: {
        int textLen = strlen(mValue);
        if( nCurInText < textLen){
            nCurInText++;
            if( nCurInScrn<mLength)
                nCurInScrn++;
        }
        SetCursor();
        break;
    }
    case 8: 		// ctrl_h key
    case KEY_BACKSPACE:	// backspace key
        if( nCurInText ) {
            int i = nCurInText;
            int textLen = strlen(mValue);
            while( i < textLen ) {
                mValue[i-1] = mValue[i];
                i++;
            }
            mValue[i-1] = '\0';
            if(nCurInText-nCurInScrn==0) {
                if( nCurInScrn ) {
                    nCurInScrn--;
                }
            }
            nCurInText--;

            RuleDrawString();

            Redraw();
            SetCursor();
        }
        break;

    case 24: 		// ctrl_x key
    case 127: 		// ctrl_x key
    case KEY_DC: {
        // delete key
        int textLen = strlen(mValue);
        if( textLen > nCurInText ){
            int i = nCurInText;
            while( i < textLen ){
                mValue[i] = mValue[i+1];
                i++;
            }
        }
        RuleDrawString();
        Redraw();
        SetCursor();
        break;
    }
    case '':
    case  27: return 2;
    case  13:
    case  10:
    case KEY_UP :
    case KEY_DOWN:
        mResultValue = mValue;
        return 0;
    default : {
        int textLen = strlen(mValue);
        if( textLen<1023 && nKey>=' ' && nKey< 127 ){
            int i = textLen;
            while( i >= nCurInText ){
                mValue[i+1] = mValue[i];
                i--;
            }
            mValue[textLen+1] = '\0';
            mValue[nCurInText] = (char)nKey;
            nCurInText++;
            if( nCurInScrn<mLength ){
                nCurInScrn++;
            }
            RuleDrawString();

            Redraw();
            SetCursor();
        }
        break;
    }
    }

    return 1;
}

void EditView::SetEditable(bool edit) {
    focus = edit;
}

void EditView::SetCursor() {
    wmove(window,y,x+5+mTitle.size()+nCurInScrn);
}


