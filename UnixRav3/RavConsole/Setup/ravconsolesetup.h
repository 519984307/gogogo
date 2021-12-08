#ifndef __RAVCONSOLESETUP__
#define __RAVCONSOLESETUP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>

#include <string>

#include "setup.h"
#include "baseconfig.h"
#include "appconfig.h"
#include "cinifile.h"
#include "commonapi.h"

#define SIGNATURE                       "RISING-UNIX-VERSION-INSTALL"
#define RAVNETWRD_REGISTER_FILE         "/etc/.RavCfg"

using namespace Setup;

class RavConsoleSetup : public SetupApplication {
public:
    RavConsoleSetup();
    ~RavConsoleSetup(){};

    void SetLPSecServer(bool is_sec_machine);
    void SetUsingX11Uninstall(bool UsingX11 = false);

    void SetUnpackSourcePath(std::string &src_path) {
        m_srcSetupPath = src_path;
    }

    int BuildRpkFile(const char *input_folder, const char *out_file);

    int StartInstall();
protected:

    int Prepare();
    int ShowWelcomeScreen(); 

    int AgreeLisence(); 

    int StartApplicationPrompt();
    int ReInstallPromtp(); 

    int GetInstallPath(std::string &install_path); 

    int GetInstallPathPrompt(std::string& installPath); 

    SoftType GetSoftwareType();

    int GetSerialNumber(std::string& serial); 

    int GetCenterAddr(std::string& center); 

    int GetInstallComponents(int &component);
    int GetInstallFunctionsCodePrompt(int &installFunctions);

    int UninstallPrompt(); 

    int ReserveConfig(); 

    int OnError(SetupStep step, int error, void * /*data*/);

private:
    int JudgeSpecialMachine();
    int GetFileType(const char *strFile);
    int PackDirectory(FILE *fpSetup, const char *strFile, SETUPTAIL *pSetupTail, const char *szInstPath);
    int PackFile(FILE *fpSetup, const char *strFile,SETUPTAIL *pSetupTail, const char *strPackPath);
    int PackSetupTail(FILE *fpSetup,SETUPTAIL *pstTail);
    std::string GetFileMD5(const std::string& path);

    int GetProductInfoFromXml(const char *file_name, ProductInfo &produce);
    int GetProductInfoFromCfg(const char *file_name, ProductInfo &produce);
    int GetProductInfo(ProductInfo& produce);
protected:

    std::string  m_srcSetupPath;
    
    bool    m_bUsingX11Uninstall;


private:
    typedef enum {
        INSTALL_MANUAL = 1,
        INSTALL_AUTO
    }InstallType;

        InstallType     mInstallMode;
        //"": prompt  if include "3": auto start process
        std::string mInstalledAction;

        //0: not lock  1: lock path
        int mLockPath;
};

#endif
