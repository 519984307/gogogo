#ifndef _H_GUI_UNINSTALLER_H_
#define _H_GUI_UNINSTALLER_H_


#include "setup.h"


class GuiUninstaller : public SetupApplication
{
public:
    GuiUninstaller();
    ~GuiUninstaller();

    int GetInstallInfo();

    int QuietUninstall();

protected:
    int ShowWelcomeScreen(){return 0;}
    int AgreeLisence(){return 0;}
    int StartApplicationPrompt(){return 0;}
    int ReInstallPromtp(){return 0;}
    int GetInstallPathPrompt(std::string& installPath){return 0;}
    int GetInstallPath(std::string& installPath){return 0;}
    SoftType GetSoftwareType(){return CENTER_LESS;}
    int GetSerialNumber(std::string& serial){return 0;}
    int GetCenterAddr(std::string& center){return 0;}
    int GetInstallFunction(int &){return 0;}
    int UninstallPrompt(){return 0;}
    int ReserveConfig(){return 0;}
    int OnError(SetupError error, void *data){return 0;}











};


#endif
