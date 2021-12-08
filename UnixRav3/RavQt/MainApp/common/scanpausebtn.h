#ifndef SCANPAUSEBTN_H
#define SCANPAUSEBTN_H

#include "imagebutton.h"


class ScanPauseBtn : public ImageButton
{
    Q_OBJECT
public:
    ScanPauseBtn(QWidget *parent = Q_NULLPTR);
    ~ScanPauseBtn(){}

    void SetMode(int mode);
    int GetMode() {
        return m_nMode;
    }

public:
    enum{
        DISPLAY_PAUSE_MODE,
        DISPLAY_CONTINUE_MODE
    };

private:
    int m_nMode;
};

#endif // SCANPAUSEBTN_H
