#include "scanpausebtn.h"

ScanPauseBtn::ScanPauseBtn(QWidget *parent)
    :ImageButton(":/resource/scan_pause_btn", parent)
{
    m_nMode = DISPLAY_PAUSE_MODE;
}


void ScanPauseBtn::SetMode(int mode)
{
    switch(mode){
    case DISPLAY_PAUSE_MODE:
        ResetImage(":/resource/scan_pause_btn");
        break;
    case DISPLAY_CONTINUE_MODE:
        ResetImage(":/resource/scan_continue_btn");
        break;
    default:
        break;
    }
}
