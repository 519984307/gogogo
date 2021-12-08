#ifndef SETUP_H
#define SETUP_H

#include "setup_global.h"

#include <string>
#include <vector>

#ifndef SETUP_SERIAL_CHECK
    #define SETUP_SERIAL_CHECK
#endif

namespace Setup
{

const int FunctionsCode[] = {0x0, 0x1, 0x2, 0x3}; //for EP XAV XFW ALL

class SetupApplication
{
protected:
    class ProductInfo {
    public:
        std::string majorname;
        std::string minorname;
        std::string osname;
        std::string platform;
        std::string version;
        std::string appver;
        std::string vlibver;
        std::string produce;
        std::string eid;
        std::string update;
    };

public:
    SetupApplication();
    ~SetupApplication();

    int StartInstall();
    int StartUninstall();

    void SetArch(const std::string& arch) {
        mArch = arch;
    }
    void SetLPSecServer(bool is_sec_machine);

    int UnpackFileToPath(const std::string &pck, const std::string &unpack_path);

    void SetFileUsbMonitor(bool bFileUsbMonitor = false);

    void SetFileMonitor(bool bFileMonitor = false);

public:
    typedef enum {
        MACHINE_LPSECRET = 0,
        MACHINE_LGSERVER,
        MACHINE_LCSERVER
    } MACHINE_TYPE;

    typedef enum {
        UNKNOWN = -1,
        CENTER_LESS = 0,
        CENTER_ESM,
        CENTER_SOC,
        // 网络版标识， 如果配置文件是此标识，则提示网络版安装和选择
        CENTER_NETWARE = 999,
    }SoftType;

//    typedef enum {
//        BASE_FUNC   = 0x1,
//        FILE_DEFEN  = 0x2,
//        NET_FILTER  = 0x4,
//        UDISK_MONITOR = 0x8,
//        CONTEXT_MENU= 0x10,
//    }Functions;

    typedef enum {
        NO_ACTION = 0,
        CREATE_DESKTOP_SHORTCUT = 1,
        ADDTO_START_MENU = 2,
        START_APPLICATION = 3
    }FinishedAction;

    typedef enum {
        COMPONENT_EP  = 0x0,
        COMPONENT_XAV = 0x1,
        COMPONENT_XFW = 0x2,
        COMPONENT_ALL = 0x3,
    }Component;

    typedef enum {
        STEP_PREPARE_DATA,
        STEP_CHECK_ENVTIVEMENT,
        STEP_STOP_RUNNING,
        STEP_CREATE_FOLDER,
        STEP_OVERRIDE_EXISTS,
        STEP_UNPACK_COMMON,
        STEP_UNPACK_XAV,
        STEP_UNPACK_XFW,
        STEP_CHECK_QT,
        STEP_RUN_SCRIPT,
        STEP_CHECK_SERIAL,
        STEP_INSTALL_FINISHED,
        SETUP_UNINSTALL_CHECKING,
        SETUP_UNINSTALL_REPORT,
        SETUP_UNINSTALL_REMOVE_CONFIG,
        SETUP_UNINSTALL_FINISHED,
    }SetupStep;

protected:
    virtual int Prepare() = 0;
    virtual int ShowWelcomeScreen() = 0;
    virtual int AgreeLisence() = 0;
    virtual int StartApplicationPrompt() = 0;
    virtual int ReInstallPromtp() = 0;
    virtual int GetInstallPathPrompt(std::string& installPath) = 0;
    virtual int GetInstallPath(std::string& installPath) = 0;
    virtual SoftType GetSoftwareType() = 0;
    virtual int GetSerialNumber(std::string& serial) = 0;
    virtual int GetCenterAddr(std::string& center) = 0;
    virtual int GetInstallComponents(int &) = 0; //for select EP XAV XFW ALL
    virtual int GetInstallFunctionsCodePrompt(int &installFunctions) = 0;
    virtual int UninstallPrompt() = 0;
    virtual int ReserveConfig() = 0;
    virtual int OnError(SetupStep step, int error, void *data) = 0;

    int MakeNcursesRunShell(std::ofstream &ofs, const std::string &exec_path);

public:
    int GetSOCEID(std::string &sEID);
    int UninstallReport();

    bool CheckInstall();
    int  UnpackPackage(const std::string& pck);

    int GetPathSpace(const char *strPath,int *pnSpace);
    int GetRealPath(char *szRealPath,const char *szInstallPath);

    int SetupCopyFile(std::ifstream& pck, const SETUPTAIL& tail);

    int CheckSerialNumber();

    int MakeRunDirs();
    int MakeExecShell();
    int MakeBinShell(const char *exec);
    int MakeStartShell(const char *exec);
    int MakeStopShell(const char *exec);

    int AddPowerOnScript();
    void trimString(char *src);
    int BeforeRootEtcStartScript();
    int AddInitdScript();
    int AddRootStartScript();
    int AddSystemdScript(const char *script_name, std::vector<std::string>& parent, const char* start, const char* stop); 
    int DelSystemdScript(const char* script_name);
    void DelRCDScripts();
    int AddAutoStart(const char *script_name);
    int DelAutoStart(const char *script_name);
    int AddApplicationIcon(const char *script_name);
    int DelApplicationIcon(const char *script_name);

    int CheckFunctions();
    int ProcessQt5Librarys();
    bool checkQt5Support();

    int RemoveDesktopFile();

#ifdef COMM_WITH_DBUS
    int AddDBusConf(const char *script_name);
    int DelDBusConf(const char *script_name);
#endif
    int GenerateRegisterFile(const char *file_folder);
    //int GeneralConfigFile(const char *sub_folder);

    int StartService(const char *script_name);
    int StopAll();



    int CheckResetupInfo();

    int WriteRavConfig();
    void SetupKernelModule();
    int DoAutoInstall();
    int GenerateRegisterFile();
    int GenerateInsmodSh();
    int GenerateExecScript();

    bool UninstallPermit();//the permission of your unistallization in console mode
    bool GuiUninstallPermit(const char* password);//for gui to get permission to uninstall

    int StartServer();
    int KillProc(const char *app);
    bool RecursiveFindFile(const std::string &dir_full_path, const std::string &file_name);

#ifdef SETUP_SERIAL_CHECK
    // check single product serial number
    // return >0版本正确 -1 序列号错误  0序列号产品不匹配
    int CheckSerial(const std::string& serial, const std::string& configfile);


    bool CheckOldVersion(std::string& url, std::string& infofile, std::string& sguid, std::string& guid);
    int  GetOldLogInfo(const std::string& install_path, std::string& infofile);
    int  UninstallOldVersion(const std::string& install_path);
    int  RestoreRclocal();
    int  CleanCrontab(const std::string& install_path);
#endif

public:
    std::string mInstallPath;

//    bool mBuildDeb;

    SoftType        mCenterType;
    int/*Component*/mComponents;
    MACHINE_TYPE    mMachineType;


    int mFunctionCode;

    std::string mCenterAddr;
    std::string mSerialNumber;
    std::string mArch;

    ProductInfo     mProductInfo;
     //describe for selected tiny function, added up, BASE_FUNCTION = 0x0,X11_SUPPORT = 0x1 ... SECRET_VERSION = 0x3D
    bool m_bFileUsbMonitor;
    bool m_bFileMonitor;

    bool mHaveSystemd;
//
    bool mHaveConfigFile;
};

}

#endif // SETUP_H
