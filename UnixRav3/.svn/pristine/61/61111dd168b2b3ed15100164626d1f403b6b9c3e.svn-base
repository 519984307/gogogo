#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <signal.h>

#include <vector>
#include <string>

#include "setup.h"
#include "base64.h"
#include "baseconfig.h"
#include "appconfig.h"
#include "communicatebundle.h"
#include "rserializer.h"
#include "commonapi.h"
#include "cinifile.h"
#include "rsversion.h"

#ifdef SETUP_SERIAL_CHECK
    #include "regapi.h"
    #include "unixrscfg.h"
#endif

using namespace std;
using namespace Utils::OS;
using namespace Utils::JSON;
using namespace Utils::String;
using namespace Setup;

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

#define SIGNATURE               "RISING-UNIX-VERSION-INSTALL"


const char *RavMachineGUID[] = {"8A8F2A01-07BF-42d3-AA76-2DEF83426170",
                                "479AF07B-BCE0-47d0-8BFA-C44ED660BEB7",
                                "90DDA51B-33AB-4d94-8F04-0B980AD75CDE"};

const char *RavMachineType[] = {"linux:lpsecret", "linux:lgserver", "linux:lcserver"};


SetupApplication::SetupApplication() {
    mInstallPath = "";
    mCenterType = UNKNOWN;
    mComponents = COMPONENT_XAV;
    mSerialNumber = "";
    mHaveSystemd = false;
    mHaveConfigFile = false;
    m_bFileUsbMonitor = false;
    m_bFileMonitor = false;
}

SetupApplication::~SetupApplication() {

}

void SetupApplication::SetFileUsbMonitor(bool bFileUsbMonitor)
{
    m_bFileUsbMonitor = bFileUsbMonitor;
}

void SetupApplication::SetFileMonitor(bool bFileMonitor)
{
    m_bFileMonitor = bFileMonitor;
}

int SetupApplication::CheckSerialNumber() {
    string config_rpk_file = Utils::Path::JoinPath(COMMON_PACK_FOLDER, COMMON_PACK_FILE_NAME, NULL);
    if (Utils::Path::FileAvailed(config_rpk_file)) {
        if (UnpackPackage(config_rpk_file) != 0) {
            return -1;
        }
    }
    string rsconfig = Utils::Path::JoinPath(mInstallPath.c_str(), PLUGINS_PATH, "RsConfig.cfg", NULL);
    if (false == Utils::Path::FileAvailed(rsconfig)) {
        return -1;
    }
#ifdef SETUP_SERIAL_CHECK
    if (!mSerialNumber.empty()) {
        int err = CheckSerial(mSerialNumber, rsconfig);
        OnError(STEP_CHECK_SERIAL, SETUP_CHECK_SERIAL_ERROR, &err);
        if (err <= 0) {
            return -1;
        }
    }
#endif
    return 0;
}

int SetupApplication::StartInstall() {
    SECURITY_CHECK(ShowWelcomeScreen(), 0, SETUP_USER_ABORT);

    OnError(STEP_PREPARE_DATA, 0, NULL);
    SECURITY_CHECK(Prepare(), 0, SETUP_PREPARE_DATA_ERROR);
    SECURITY_CHECK(AgreeLisence(), 0, SETUP_USER_DISAGREE);

    OnError(STEP_CHECK_ENVTIVEMENT, 0, NULL);
    if (CheckInstall()) {
        SECURITY_CHECK(ReInstallPromtp(), 0, SETUP_OVERIDE_ABORT);
        SECURITY_CHECK(CheckResetupInfo(), 0, -1);

        OnError(STEP_STOP_RUNNING, 0, NULL);
        StopAll();
    }

    SECURITY_CHECK(GetInstallPathPrompt(mInstallPath), 0, -1);
    if (!Utils::Path::PathExists(mInstallPath)) {
        SECURITY_CHECK(Utils::Path::mkdirs(mInstallPath), 0, SETUP_CREATE_FOLDER_ERROR);
    }

    OnError(STEP_CREATE_FOLDER, 0, NULL);
    SECURITY_CHECK(MakeRunDirs(), 0, SETUP_CREATE_FOLDER_ERROR);

    if (mCenterType != CENTER_LESS) {
         mCenterType = GetSoftwareType();
        SECURITY_CHECK(GetCenterAddr(mCenterAddr), 0, SETUP_CENTER_ADDR_ERROR);
        if (mCenterType == CENTER_SOC) {
            // 这里不对EID做强制要求
            if (mProductInfo.eid.empty()) {
                GetSOCEID(mProductInfo.eid);
            }
        }
    }

    if (mCenterType == CENTER_LESS) {
        SECURITY_CHECK(GetSerialNumber(mSerialNumber), 0, -1);
        // 在此不做序列号强制校验，不成功也可以安装，里面会有判断
        CheckSerialNumber();
    }

    StopAll();

    GetInstallComponents(mComponents);

    OnError(STEP_UNPACK_COMMON, 0, NULL);
    SECURITY_CHECK(
                UnpackPackage(Utils::Path::JoinPath(COMMON_PACK_FOLDER, COMMON_PACK_FILE_NAME, NULL)),
                0, SETUP_UNPACK_ERROR);

    string config_rpk_file = Utils::Path::JoinPath(COMMON_PACK_FOLDER, CONFIG_PACK_FILE_NAME, NULL);
    if (Utils::Path::FileAvailed(config_rpk_file)) {
        SECURITY_CHECK(UnpackPackage(config_rpk_file), 0, SETUP_UNPACK_ERROR);
    }  
    SECURITY_CHECK(UnpackPackage(Utils::Path::JoinPath(SETUP_PACK_FOLDER, mArch.c_str(), EP_PACK_FILE_NAME, NULL)), 0, SETUP_UNPACK_ERROR);

    if (mComponents & COMPONENT_XAV) {
        OnError(STEP_UNPACK_XAV, 0, NULL);
        SECURITY_CHECK(UnpackPackage(Utils::Path::JoinPath(SETUP_PACK_FOLDER, mArch.c_str(), XAV_PACK_FILE_NAME, NULL)), 0, SETUP_UNPACK_ERROR);
    }
    if (mComponents & COMPONENT_XFW) {
        OnError(STEP_UNPACK_XFW, 0, NULL);
        SECURITY_CHECK(UnpackPackage(Utils::Path::JoinPath(SETUP_PACK_FOLDER, mArch.c_str(), XFW_PACK_FILE_NAME, NULL)), 0, SETUP_UNPACK_ERROR);
    }

    OnError(STEP_CHECK_QT, 0, NULL);
    if (!checkQt5Support()) {
        ProcessQt5Librarys();
    }

    OnError(STEP_RUN_SCRIPT, 0, NULL);
    SetupKernelModule();
    SECURITY_CHECK(MakeExecShell(), 0, SETUP_MAKE_BIN_SHELL_ERROR);
    SECURITY_CHECK(MakeStartShell(START_KO_SHELL), 0, SETUP_MAKE_KO_SHELL_ERROR);
    SECURITY_CHECK(MakeStopShell(STOP_ALL_SHELL), 0, SETUP_MAKE_KO_SHELL_ERROR);

    string rjjhetc_path = Utils::Path::JoinPath(mInstallPath.c_str(), FOLDER_RJJH_ETC, NULL);
    if(!Utils::Path::PathExists(rjjhetc_path)){
        Utils::Path::mkdirs(rjjhetc_path);
    }

    SECURITY_CHECK(GenerateRegisterFile(), 0, SETUP_GENERAL_CONFIG_ERROR);
    SECURITY_CHECK(GenerateInsmodSh(), 0, SETUP_MAKE_INSMOD_ERROR);
    SECURITY_CHECK(GenerateExecScript(), 0, SETUP_MAKE_LOADER_ERROR);
    SECURITY_CHECK(AddRootStartScript(), 0, SETUP_MAKE_SYSTEMD_ERROR);

    AddAutoStart(AUTOSTART_FILE_NAME);
    AddApplicationIcon(APPLICATION_FILE_NAME);

    SECURITY_CHECK(StartApplicationPrompt(), 0, SETUP_USER_ABORT);
    StartServer();
    OnError(STEP_INSTALL_FINISHED, 0, NULL);
    return 0;
}

int SetupApplication::StartServer() {
    // start the watch dog and other
    const char *START_PROCS[][2] = {
        { DAEMON_GJCZ_COMMAND, "RAVGJCZ0"},
        { DAEMON_GLTX_COMMAND, "RAVGLTX0" },
        { DAEMON_ZDFY_COMMAND, "RAVZDFY0" }
    };
    for (int i=0; i<(int)(sizeof(START_PROCS)/sizeof(START_PROCS[0])); ++i) {
        string proc = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, START_PROCS[i][0], NULL);
        if(!Utils::Path::FileAvailed(proc)){
            continue;
        }
        StartService(proc.c_str());

        string daemon = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, START_PROCS[i][1], NULL);
        int retry = 3;
        do {
            sleep(1);
        }while (Utils::OS::CheckProcessState(daemon.c_str()) && (retry-- >= 0));
    }

    string spath = Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVRJJH1", NULL);
    string command = spath + " -s ";
    StartService(command.c_str());
    return 0;
}

int SetupApplication::MakeNcursesRunShell(std::ofstream &ofs, const std::string &exec_path) {
    string ldpath = Utils::Path::JoinPath(mInstallPath.c_str(), "lib", NULL);

    ofs << "TerminfoPaths=(/usr/share/terminfo /usr/lib/terminfo /lib/terminfo /etc/terminfo)" << endl;
    ofs << "HeaderChar=${TERM:0:1}" << endl;
    ofs << "for value in ${TerminfoPaths[@]}" << endl;
    ofs << "do" << endl;
    ofs << "    RealTerminfoPath=${value}/${HeaderChar}/${TERM}" << endl;
    ofs << "    if [ -f $RealTerminfoPath ]; then" << endl;
    ofs << "        TERMINFO=${value}" << endl;
    ofs << "        break" << endl;
    ofs << "    fi" << endl;
    ofs << "done" << endl;
    ofs << "" << endl;
    ofs << "export TERMINFO" << endl;
    ofs << "export TERM=xterm" << endl;
    ofs << "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:" << ldpath << endl;
	ofs << exec_path << endl;
    return 0;
}

int SetupApplication::MakeBinShell(const char *exec) {
    char shell[20] = {0};
    strcpy(shell, exec);

    if(strcmp(exec, "ravuninst.bin") == 0) {
        strcpy(shell, "ravuninst.sh");
    }else if (strcmp(exec, "rav.bin") == 0) {
        strcpy(shell, "rav.sh");
    }else{
        if (strlen(exec) != 8 && strlen(exec) != 9) {
            printf("SetupApplication::MakeBinShell wrong command size, exec=%s\n", exec);
            return -1;
        }
        if (shell[strlen(shell)-1]>='0' && shell[strlen(shell)-1]<='9') {
            shell[strlen(shell)-1] += 1;
        }
    }

    string shellpath = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, shell, NULL);
    ofstream ofs(shellpath.c_str());
    if (!ofs) {
        printf("SetupApplication::MakeBinShell open file fail:%s\n", shellpath.c_str());
        return -1;
    }

    string libpath = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_LIB_PATH, NULL);
    string execpath = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, exec, NULL);

    if (strcmp(exec, "rav.bin") == 0) {
        ofs << "#!/bin/bash" << endl;
        if (MakeNcursesRunShell(ofs, execpath) != 0) {
            printf("SetupApplication::MakeBinShell create ncurses shell fail, file:%s\n", execpath.c_str());
            return -1;
        }
    }else if(strcmp(exec, "RAVRJJH0") == 0){
        ofs << "#!/bin/sh" << endl;
        ofs << "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:" << libpath << endl;

		// ZCG+无论如何后要加上. 
        //char *display = getenv("DISPLAY");
        //if (NULL == display) {
            ofs << "export DISPLAY=:0.0" << endl;
        //}
        ofs << execpath << " $@" << endl;
    }else if (strcmp(exec, "RAVRJJH6") == 0){
        ofs << "#!/bin/sh" << endl;
        ofs << "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:" + libpath + " " + execpath << " $@" << endl;
    }else{
        ofs << "#!/bin/sh" << endl;
        ofs << "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:" + libpath + " " + execpath << endl;
    }
    ofs << "exit" << endl;
    ofs.close();
    umask(0);
    chmod(shellpath.c_str(), 0755);

    return 0;
}

int SetupApplication::MakeStartShell(const char *exec) {
    string sh_path, bin_path, ravdefen, ravfilter;
    sh_path = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, exec, NULL);

    bin_path = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, NULL);
    ravdefen = Utils::Path::JoinPath(mInstallPath.c_str(), PLUGINS_PATH, FILE_DEFEN_MODULE, NULL);
    ravfilter = Utils::Path::JoinPath(mInstallPath.c_str(), PLUGINS_PATH, NET_FILTER_MODULE, NULL);

    ofstream ofs(sh_path.c_str());
    if (!ofs) {
        return -1;
    }

    ofs << "#!/bin/sh" << endl;
    ofs << bin_path << "/insmod.sh -i " << ravdefen << ".ko &" << endl;
    ofs << bin_path <<"/insmod.sh -i " << ravfilter << ".ko &" << endl;

    string gjcz = Utils::Path::JoinPath(bin_path.c_str(), DAEMON_GJCZ_COMMAND, NULL);
    ofs << gjcz << endl;

    ofs << "exit" << endl;
    ofs.close();

    umask(0);
    chmod(sh_path.c_str(), 0755);

    return 0;
}

int SetupApplication::MakeStopShell(const char *exec) {
    string shellpath;
    shellpath = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, exec, NULL);
    ofstream ofs(shellpath.c_str());
    if (!ofs) {
        return -1;
    }

    const char *EXEC[] = {
        MAIN_CONSOLE_ENTRY,
        MAIN_SCREEN_ENTRY,
        MAIN_SETTING_ENTRY,
        MAIN_LOGVIEW_ENTRY,
        NOTIFYY_ENTRY,
        DAEMON_GJCZ_ENTRY,
        DAEMON_GLTX_ENTRY,
        DAEMON_ZDFY_ENTRY,
    };

    ofs << "#!/bin/sh" << endl;
    for (int i=0; i<(int)(sizeof(EXEC)/sizeof(char*)); ++i) {
        ofs << "killall -9 " << EXEC[i] << endl;
    }

    ofs << "exit" << endl;
    ofs.close();

    umask(0);
    chmod(shellpath.c_str(), 0755);
    return 0;
}

int SetupApplication::MakeRunDirs() {
    const char *DIRS[] = {"RJJHGJCZ", "RJJHGLTX", "RJJHZDFY", "RJJHetc"};
    for (int i=0; i<(int)(sizeof(DIRS)/sizeof(char*)); i++) {
        string path = Utils::Path::JoinPath(mInstallPath.c_str(), DIRS[i], NULL);
        if(!Utils::Path::PathExists(path)){
            if (Utils::Path::mkdirs(path, 0777)) {
                return -1;
            }
        }
    }
    return 0;
}

int SetupApplication::MakeExecShell() {
    const char *EXEC[] = {
        MAIN_CONSOLE_ENTRY,
        MAIN_SCREEN_ENTRY,
        MAIN_SETTING_ENTRY,
        MAIN_LOGVIEW_ENTRY,
        NOTIFYY_ENTRY,
        DAEMON_GJCZ_ENTRY,
        DAEMON_GLTX_ENTRY,
        DAEMON_ZDFY_ENTRY,
        "ravuninst.bin"
    };
    
    for (int i=0; i<(int)(sizeof(EXEC)/sizeof(char*)); ++i) {
        string sfile = Utils::Path::JoinPath(mInstallPath.c_str(), "bin", EXEC[i], NULL);
        if(Utils::Path::FileAvailed(sfile)) {
            SECURITY_CHECK(MakeBinShell(EXEC[i]), 0, -1);
        }
    }
    string path_zdfy1 = Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVZDFY1", NULL);
    string path_rjjh9 = Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVRJJH9", NULL);
    ofstream ofs(path_rjjh9.c_str());
    if (!ofs) {
        return -1;
    }
    ofs << "#!/bin/sh" << endl;
    ofs << path_zdfy1 << endl;
    ofs << "exit" << endl;
    ofs.close();
    umask(0);
    chmod(path_rjjh9.c_str(), 0755);
    return 0;
}

bool SetupApplication::CheckInstall() {
    std::string sconfig;
    if(GetInstallPath(sconfig) != 0)
        return false;
    if(Utils::Path::PathExists(sconfig)){
        return true;
    }
    return false;
}

int SetupApplication::GetSOCEID(string &sEID) {
    fprintf(stdout,"Center EID>");
    fflush(stdin);

    char tmp[256];
    if (NULL == fgets(tmp, 256, stdin)) {
        return -1;
    }
    if (tmp[strlen(tmp)-1] == '\n') {
        tmp[strlen(tmp)-1] = 0;
    }
    if(strlen(tmp) < 1) {
        sEID = "e00000110";
    }
    else {
        sEID = tmp;
    }
    return 0;
}

int SetupApplication::UnpackFileToPath(const std::string &pck, const std::string &unpack_path) {
    SETUPTAIL tail;
    memset(&tail, 0, sizeof(SETUPTAIL));

    ifstream ifs;
    ifs.open(pck.c_str(), ios::in|ios::binary);
    if (!ifs) {
        printf("Error, UnpackFileToPath ifs.open fail\n");
        return -1;
    }
    ifs.seekg(0, ios::end);
    int endoff = ifs.tellg();
    int tailpos = endoff - sizeof(SETUPTAIL);
    ifs.seekg(tailpos, ios::beg);
    ifs.read((char*)&tail, sizeof(SETUPTAIL));
    if (strcmp(tail.strSignature, SIGNATURE)) {
        ifs.close();
        printf("SetupApplication::UnpackFileToPath find different signature:read:%s  right:%s\n", tail.strSignature, SIGNATURE);
        return -1;
    }
    
    ifs.seekg(0);
    for (int i=0; i<tail.nFileNum; ++i) {
        SETUPTOC toc;
        ifs.read((char*)&toc, sizeof(SETUPTOC));
        //printf("toc.strFile=%s oriMD5=%s zipMD5=%s size=%d\n", toc.strFile, toc.szOrigMd5, toc.szZipMd5, toc.nSize);
        string dest = Utils::Path::JoinPath(unpack_path.c_str(), toc.strFile, NULL);
        //printf("unpack_path=%s  toc.strFile=%s\n", unpack_path.c_str(), toc.strFile);
        string path, name;
        Utils::Path::SplitPath(dest, path, name);
        Utils::Path::mkdirs(path);

        string zipfile = dest + ".UGZ";
        ofstream ofs(zipfile.c_str(), ios::out|ios::binary);
        if (!ofs) {
            printf("SetupApplication::UnpackFileToPath open file fail:%s\n", zipfile.c_str());
            return -1;
        }

        char buffer[1024];
        int destlen = toc.nSize;
        while(destlen > 0) {
            ifs.read(buffer, destlen>1024?1024:destlen);
            int len = ifs.gcount();
            ofs.write(buffer, len);
            destlen -= len;
        }
        ofs.close();

        SECURITY_CHECK(Utils::Hash::GetFileMD5(zipfile), toc.szZipMd5, -1);
        SECURITY_CHECK(Utils::Compress::UnzipFile(zipfile, dest), 0, -2);
        SECURITY_CHECK(Utils::Hash::GetFileMD5(dest), toc.szOrigMd5, -3);
        umask(0);
        chmod(dest.c_str(), toc.mode);
    }
    ifs.close();
    return 0;
}

int SetupApplication::UnpackPackage(const std::string &pck) {
    SETUPTAIL setupTail;
    memset(&setupTail, 0, sizeof(SETUPTAIL));

    ifstream ifs;
    ifs.open(pck.c_str(), ios::in|ios::binary);
    if (!ifs) {
        printf("Error, open package file fail, file:%s", pck.c_str());
        return -1;
    }

    ifs.seekg(0, ios::end);

    int endoff = ifs.tellg();
    int tailpos = endoff - sizeof(SETUPTAIL);
    ifs.seekg(tailpos, ios::beg);

    ifs.read((char*)&setupTail, sizeof(SETUPTAIL));
    if (strcmp(setupTail.strSignature, SIGNATURE)) {
        ifs.close();
        printf("SetupApplication::UnpackPackage find different signature:read:%s  right:%s\n", setupTail.strSignature, SIGNATURE);
        return -1;
    }

    int ret = SetupCopyFile(ifs, setupTail);
    if (ret != 0) {
        printf("Error, Decompress file from package fail.\n");
    }
    ifs.close();
    return 0;
}

int SetupApplication::GetPathSpace(const char *strPath,int *pnSpace)
{

#ifdef PLATFORM_TYPE_FREEBSD
    struct statfs status;
#else
    struct statvfs status;
#endif
    unsigned long ul1;
    unsigned long ul2;

    *pnSpace=-1;


    char szRealPath[256];
#ifdef PLATFORM_TYPE_FREEBSD
    GetRealPath(szRealPath,strPath);
    if(statfs(szRealPath,&status)==-1)
    {
        return -1;
    }
#else
    GetRealPath(szRealPath,strPath);
    if(statvfs(szRealPath,&status)==-1)
    {

        return -1;
    }
#endif
  ul1=status.f_bavail/1024;


#ifdef PLATFORM_TYPE_FREEBSD
    ul2=(status.f_bsize*4)/1024;
#else
    ul2=(status.f_frsize*4)/1024;
#endif

  if(ul1==0)
  {
#ifdef PLATFORM_TYPE_FREEBSD
    *pnSpace=(status.f_bavail*status.f_bsize)/(1024*1024);
#else
    *pnSpace=(status.f_bavail*status.f_frsize)/(1024*1024);
#endif
  }
  else
  {
    *pnSpace=(ul1*ul2)/4;
  }

  return 0;
}

int SetupApplication::GetRealPath(char *szRealPath,const char *szInstallPath)
{
    int nIndex,nLen,nI;
    const char *szTmp;
    char szExistPath[512];
    char szTmpPath[512];

        // cut off  multiple '/'

    nLen=strlen(szInstallPath);
    bool bPrev=false;

    nIndex=0;
    memset(szTmpPath,0,sizeof(szTmpPath));
    for(nI=0;nI<nLen;nI++)
    {
            if(bPrev && szInstallPath[nI]=='/')
                    continue;
            szTmpPath[nIndex++]=szInstallPath[nI];

            if(szInstallPath[nI]=='/')
                    bPrev=true;
            else
                    bPrev=false;
    }

    szTmp=&szTmpPath[1];
    szExistPath[0]='\0';
    szRealPath[0]='/';
    szRealPath[1]='\0';
    while(1)
    {
        szTmp=strchr(szTmp,'/');
        if(!szTmp)
        {
            if('\0'==szExistPath[0])
                strcpy(szRealPath, szTmpPath);
            break;
        }
        nLen=szTmp-szTmpPath;
        strncpy(szExistPath,szTmpPath,nLen);
        szExistPath[nLen]='\0';
        if(access(szExistPath,F_OK))
                break;
        strcpy(szRealPath,szExistPath);
        szTmp++;

    }
        return 0;
}


int SetupApplication::SetupCopyFile(ifstream& pck, const SETUPTAIL& tail) {
    pck.seekg(0);
    for (int i=0; i<tail.nFileNum; ++i) {
        SETUPTOC toc;
        pck.read((char*)&toc, sizeof(SETUPTOC));
        //printf("toc.strFile=%s oriMD5=%s zipMD5=%s size=%d\n", toc.strFile, toc.szOrigMd5, toc.szZipMd5, toc.nSize);
        string dest = Utils::Path::JoinPath(mInstallPath.c_str(), toc.strFile, NULL);
        //printf("mInstallPath=%s  toc.strFile=%s\n", mInstallPath.c_str(), toc.strFile);
        string path, name;
        Utils::Path::SplitPath(dest, path, name);
        Utils::Path::mkdirs(path);

        string zipfile = dest + ".UGZ";
        ofstream ofs(zipfile.c_str(), ios::out|ios::binary);
        if (!ofs) {
            printf("SetupApplication::SetupCopyFile open file fail:%s\n", zipfile.c_str());
            return -1;
        }

        char buffer[1024];
        int destlen = toc.nSize;
        while(destlen > 0) {
            pck.read(buffer, destlen>1024?1024:destlen);
            int len = pck.gcount();
            ofs.write(buffer, len);
            destlen -= len;
        }
        ofs.close();

        if (Utils::Hash::GetFileMD5(zipfile) != toc.szZipMd5) {
            printf("Error,verify MD5 fail, zip file:%s\n", zipfile.c_str());
            return -1;
        }
        if (Utils::Compress::UnzipFile(zipfile, dest) != 0) {
            printf("Error, unzip file fail, zip file:%s\n", zipfile.c_str());
            return -2;
        }
        if (Utils::Hash::GetFileMD5(dest) != toc.szOrigMd5) {
            printf("Error, verify MD5 fail, file:%s\n", dest.c_str());
            return -3;
        }
        umask(0);
        chmod(dest.c_str(), toc.mode);
    }
    return 0;
}

void SetupApplication::trimString(char *src) {
    char *begin = src;
    char *end   = src;

    while (*end++);        
    if ( begin == end  ) return; 

    while ( *begin == ' ' || *begin == '\t' || *begin=='\"' )          
        ++begin;
    while ( (*end) == '\0' || *end == ' ' || *end == '\t' || *end=='\"' ) 
        --end;

    if ( begin > end  ) {
         *src = '\0';  return; 
    } 
    while ( begin != end  ) {
        *src++ = *begin++;
    }
    
    *src++ = *end;
    *src = '\0';
    return;
}

int SetupApplication::BeforeRootEtcStartScript() {
    if (access("/etc/rc.local", F_OK|R_OK|W_OK) == 0) {
        if (rename("/etc/rc.local", "/etc/rc.local.bak")) {
            return -1;
        }
        
        FILE *d = fopen("/etc/rc.local", "w");
        if (!d) {
            rename("/etc/rc.local.bak", "/etc/rc.local");
            return -2;
        }

        FILE *f = fopen("/etc/rc.local.bak", "r");
        if (!f) {
            fclose(d);
            rename("/etc/rc.local.bak", "/etc/rc.local");
            return -1;
        }

        char buffer[256] = {0};
        bool commandWrited = false;
        while (!feof(f)) {
            memset(buffer, 0, 256);
            fgets(buffer, 255, f);
            
            trimString(buffer);
            if (buffer[0] == '#') {
                fputs(buffer, d);
                continue;
            }

            if (strstr(buffer, START_KO_SHELL) || strstr(buffer, DAEMON_GLTX_COMMAND) || strstr(buffer, DAEMON_ZDFY_COMMAND) \
                || strstr(buffer, "exit")) {
                continue;
            }
            fputs(buffer, d);
        }
        fclose(f);
        fclose(d);

        umask(0);
        chmod("/etc/rc.local", 0755);
        unlink("/etc/rc.local.bak");
        return 0;
    }
    return -1;
}

//int SetupApplication::AddInitdScript()
//{
//    string file_name = "/etc/rc.d/init.d/RISING";
//    string link_file[] = {"/etc/rc3.d/S99RISING", "/etc/rc2.d/S99RISING", "/etc/rc5.d/S99RISING", "/etc/rc0.d/K01RISING", "/etc/rc6.d/K01RISING"};
//    if (!Utils::Path::PathExists("/etc/rc.d/init.d")) {
//        if (Utils::Path::PathExists("/etc/init.d") ) {
//            file_name = "/etc/init.d/RISING";
//        }else{
//            printf("Error, create init script fail. directory /etc/rc.d/init.d and /etc/init.d not exists!\n");
//            return -1;
//        }
//    }
//    if (Utils::Path::FileAvailed(file_name.c_str())) {
//        unlink(file_name.c_str());
//    }
//    ofstream ofs(file_name.c_str());
//    if (!ofs) {
//        return -1;
//    }
//    ofs << "#!/bin/sh" << endl;
//
//    ofs << "### BEGIN INIT INFO" << endl;
//    ofs << "# Provides:        rising" << endl;
//    ofs << "# Required-Start:  $network" << endl;
//    ofs << "# Required-Stop:   $network " << endl;
//    ofs << "# Default-Start:   2 3 4 5" << endl;
//    ofs << "# Default-Stop:    1" << endl;
//    ofs << "# Short-Description: Start RISING daemon" << endl;
//    ofs << "### END INIT INFO" << endl;
//
//    ofs << "INSTALLPATH=/opt/BDFZ/RAV" << endl;
//    ofs << "prog=\"RISING\"" << endl;
//    ofs << "" << endl;
//    ofs << "start_ravservice()" << endl;
//    ofs << "{" << endl;
//    ofs << "    echo \"START RISING ANTIVIRUS SOFTWARE\"" << endl;
//    ofs << "    prog_pid=\"`ps -ef |grep RAVGJCZ0 |grep -v \"grep\"`\"" << endl;
//    ofs << "    if [ -z \"$prog_pid\" ];then" << endl;
//    ofs << "        $INSTALLPATH/bin/RAVGJCZ9 >/dev/null 2>&1" << endl;
//    ofs << "        sleep 1" << endl;
//    ofs << "    fi" << endl;
//    ofs << "" << endl;
//    ofs << "    prog_pid=\"`ps -ef |grep RAVGLTX0 |grep -v \"grep\"`\"" << endl;
//    ofs << "    if [ -z \"$prog_pid\" ];then" << endl;
//    ofs << "        $INSTALLPATH/bin/RAVGLTX1 >/dev/null 2>&1" << endl;
//    ofs << "        sleep 1" << endl;
//    ofs << "    fi" << endl;
//    ofs << "" << endl;
//    ofs << "    prog_pid=\"`ps -ef |grep RAVZDFY0 |grep -v \"grep\"`\"" << endl;
//    ofs << "    if [ -z \"$prog_pid\" ];then" << endl;
//    ofs << "        $INSTALLPATH/bin/RAVZDFY1 >/dev/null 2>&1" << endl;
//    ofs << "        sleep 1" << endl;
//    ofs << "    fi" << endl;
//    ofs << "    if [ -n \"$USER\" ];then" << endl;
//    ofs << "        su - $USER -c $INSTALLPATH/bin/RAVRJJH1 -s >/dev/null 2>&1" << endl;
//    ofs << "    else" << endl;
//    ofs << "        $INSTALLPATH/bin/RAVRJJH1 -s >/dev/null 2>&1" << endl;
//    ofs << "    fi   " << endl;
//    ofs << "}" << endl;
//    ofs << "" << endl;
//    ofs << "stop_ravservice()" << endl;
//    ofs << "{" << endl;
//    ofs << "    echo \"STOP RISING ANTIVIRUS SOFTWARE\"" << endl;
//    ofs << "    killall -9 rav.bin >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVRJJH2 >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVRJJH4 >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVRJJH6 >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVRJJH0 >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVGJCZ0 >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVGLTX0 >/dev/null 2>&1" << endl;
//    ofs << "    killall -9 RAVZDFY0 >/dev/null 2>&1" << endl;
//    ofs << "}" << endl;
//    ofs << "" << endl;
//    ofs << "case $1 in" << endl;
//    ofs << "    start)" << endl;
//    ofs << "        start_ravservice" << endl;
//    ofs << "    ;;" << endl;
//    ofs << "    stop)" << endl;
//    ofs << "        stop_ravservice" << endl;
//    ofs << "    ;;" << endl;
//    ofs << "    restart)" << endl;
//    ofs << "        stop_ravservice" << endl;
//    ofs << "        start_ravservice" << endl;
//    ofs << "    ;;" << endl;
//    ofs << "    *)" << endl;
//    ofs << "    echo \"Usage: $prog {start|stop|restart}\" " << endl;
//    ofs << "    exit 2" << endl;
//    ofs << "esac" << endl;
//    ofs << "exit 0" << endl;
//    ofs.close();
//    umask(0);
//    chmod(file_name.c_str(), 0755);
//    for(int i=0; i<5; i++) {
//        if ( Utils::Path::FileAvailed(link_file[i].c_str()) ) {
//            unlink(link_file[i].c_str());
//        }
//        symlink(file_name.c_str(), link_file[i].c_str());
//    }
//    return 0;
//}

int SetupApplication::AddInitdScript()
{
    string file_name = "/etc/rc.d/init.d/RISING";
    string link_file[] = {"/etc/rc3.d/S99RISING", "/etc/rc2.d/S99RISING", "/etc/rc5.d/S99RISING", "/etc/rc0.d/K01RISING", "/etc/rc6.d/K01RISING"};
    if (!Utils::Path::PathExists("/etc/rc.d/init.d")) {
        if (Utils::Path::PathExists("/etc/init.d") ) {
            file_name = "/etc/init.d/RISING";
        }else{
            printf("Error, create init script fail. directory /etc/rc.d/init.d and /etc/init.d not exists!\n");
            return -1;
        }
    }
    if (Utils::Path::FileAvailed(file_name.c_str())) {
        unlink(file_name.c_str());
    }
    ofstream ofs(file_name.c_str());
    if (!ofs) {
        return -1;
    }
    ofs << "#!/bin/sh" << endl;

    ofs << "### BEGIN INIT INFO" << endl;
    ofs << "# Provides:        rising" << endl;
    ofs << "# Required-Start:  $network" << endl;
    ofs << "# Required-Stop:   $network " << endl;
    ofs << "# Default-Start:   2 3 4 5" << endl;
    ofs << "# Default-Stop:    1" << endl;
    ofs << "# Short-Description: Start RISING daemon" << endl;
    ofs << "### END INIT INFO" << endl;

    ofs << "INSTALLPATH=/opt/BDFZ/RAV" << endl;
    ofs << "prog=\"RISING\"" << endl;
    ofs << "" << endl;
    ofs << "RMMOD() {" << endl;
    ofs << "    lsmod|grep $1" << endl;
    ofs << "    if [ $? -eq 0 ];then" << endl;
    ofs << "        rmmod -f $1" << endl;
    ofs << "    fi" << endl;
    ofs << "}" << endl;
    ofs << "" << endl;
    ofs << "start_ravservice()" << endl;
    ofs << "{" << endl;
    ofs << "    echo \"START RISING ANTIVIRUS SOFTWARE\"" << endl;
    ofs << "    prog_pid=\"`ps -ef |grep RAVGJCZ0 |grep -v \"grep\"`\"" << endl;
    ofs << "    if [ -z \"$prog_pid\" ];then" << endl;
    ofs << "        $INSTALLPATH/bin/RAVGJCZ9 >/dev/null 2>&1" << endl;
    ofs << "        sleep 1" << endl;
    ofs << "    fi" << endl;
    ofs << "" << endl;
    ofs << "    prog_pid=\"`ps -ef |grep RAVGLTX0 |grep -v \"grep\"`\"" << endl;
    ofs << "    if [ -z \"$prog_pid\" ];then" << endl;
    ofs << "        $INSTALLPATH/bin/RAVGLTX1 >/dev/null 2>&1" << endl;
    ofs << "        sleep 1" << endl;
    ofs << "    fi" << endl;
    ofs << "" << endl;
    ofs << "    prog_pid=\"`ps -ef |grep RAVZDFY0 |grep -v \"grep\"`\"" << endl;
    ofs << "    if [ -z \"$prog_pid\" ];then" << endl;
    ofs << "        $INSTALLPATH/bin/RAVZDFY1 >/dev/null 2>&1" << endl;
    ofs << "        sleep 1" << endl;
    ofs << "    fi" << endl;
    //ofs << "    if [ -n \"$USER\" ];then" << endl;
    //ofs << "        su - $USER -c $INSTALLPATH/bin/RAVRJJH1 -s >/dev/null 2>&1" << endl;
    //ofs << "    else" << endl;
    ofs << "    $INSTALLPATH/bin/RAVRJJH1 -s >/dev/null 2>&1" << endl;
    //ofs << "    fi   " << endl;
    ofs << "}" << endl;
    ofs << "" << endl;
    ofs << "stop_ravservice()" << endl;
    ofs << "{" << endl;
    ofs << "    echo \"STOP RISING ANTIVIRUS SOFTWARE\"" << endl;
    ofs << "    killall -9 rav.bin >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVRJJH2 >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVRJJH4 >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVRJJH6 >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVRJJH0 >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVGJCZ0 >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVGLTX0 >/dev/null 2>&1" << endl;
    ofs << "    killall -9 RAVZDFY0 >/dev/null 2>&1" << endl;
    ofs << "    ALLMOD=\"ravdefen ravfilter\"" << endl;
    ofs << "    for MOD in $ALLMOD" << endl;
    ofs << "    do" << endl;
    ofs << "        RMMOD $MOD" << endl;
    ofs << "    done" << endl;
    ofs << "}" << endl;
    ofs << "" << endl;
    ofs << "case $1 in" << endl;
    ofs << "    start)" << endl;
    ofs << "        start_ravservice" << endl;
    ofs << "    ;;" << endl;
    ofs << "    stop)" << endl;
    ofs << "        stop_ravservice" << endl;
    ofs << "    ;;" << endl;
    ofs << "    restart)" << endl;
    ofs << "        stop_ravservice" << endl;
    ofs << "        start_ravservice" << endl;
    ofs << "    ;;" << endl;
    ofs << "    *)" << endl;
    ofs << "    echo \"Usage: $prog {start|stop|restart}\" " << endl;
    ofs << "    exit 2" << endl;
    ofs << "esac" << endl;
    ofs << "exit 0" << endl;
    ofs.close();
    umask(0);
    chmod(file_name.c_str(), 0755);
    for(int i=0; i<5; i++) {
        if ( Utils::Path::FileAvailed(link_file[i].c_str()) ) {
            unlink(link_file[i].c_str());
        }
        symlink(file_name.c_str(), link_file[i].c_str());
    }
    return 0;
}

int SetupApplication::AddRootStartScript() {
    if (Utils::Path::PathExists("/etc/systemd") || Utils::Path::PathExists("/usr/lib/systemd")) {
        const char *systemlib = "/usr/lib/systemd";
        if (Utils::Path::PathExists(systemlib)) {
            string spath = Utils::Path::JoinPath(systemlib, "system", NULL);
            Utils::Path::mkdirs(spath);
        }
        mHaveSystemd = true;
        std::vector<std::string> parent;
        AddSystemdScript(SYSTEMD_RAV_SERVICE, parent, START_RAV_SHELL, STOP_RAV_SHELL);

        system("systemctl enable RAVBDFZ.service 2>/dev/null &");
    }
    else {
        // if no systemd, use rc.local
        //BeforeRootEtcStartScript();
        AddInitdScript();
//        ofstream ofs;
//        ofs.open("/etc/rc.local", std::ios_base::in|std::ios_base::app);
//        ofs.seekp(ios::end);
//
//        ofs << Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, START_KO_SHELL, NULL) << endl;
//        ofs << Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, DAEMON_GLTX_COMMAND, NULL) << endl;
//        ofs << Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, DAEMON_ZDFY_COMMAND, NULL) << endl;
//
//        ofs.close();
    }
    return 0;
}

int SetupApplication::AddSystemdScript(const char *name, std::vector<std::string>& parent, const char* start, const char* stop) {
    string spath = Utils::Path::JoinPath("/usr/lib/systemd/system", name, NULL);
    if (!Utils::Path::PathExists("/usr/lib/systemd/system")) {
        Utils::Path::mkdirs("/usr/lib/systemd/system");
    }

    ofstream ofs(spath.c_str());
    if (!ofs) {
        return -1;
    }

    string start_path = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, start, NULL);
    string stop_path = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, stop, NULL);

    ofs << "[Unit]" << endl;
    ofs << "Description=Rising Antivirus For Linux Service" << endl;
    ofs << "After=network.target ";

    std::vector<std::string>::iterator iter;
    for (iter=parent.begin(); iter!=parent.end(); ++iter) {
        ofs << *iter << " ";
    }

    ofs << endl;
    ofs << "[Service]" << endl;
    ofs << "Type=oneshot" << endl;
    ofs << "ExecStart=" + start_path << endl;
    ofs << "ExecStop=" + stop_path << endl;
    ofs << "RemainAfterExit=yes" << endl;
    ofs << "KillMode=none" << endl;
    ofs << "[Install]" << endl;
    ofs << "WantedBy=multi-user.target" << endl;

    ofs.close();
    return 0;
}

int SetupApplication::DelSystemdScript(const char *script_name) {
    string spath = Utils::Path::JoinPath("/usr/lib/systemd/system", script_name, NULL);
    if (Utils::Path::FileAvailed(spath)) {
        unlink(spath.c_str());
    }
    return 0;
}

void SetupApplication::DelRCDScripts()
{
    string file_name = "/etc/rc.d/init.d/RISING";
    string link_file[] = {"/etc/rc3.d/S99RISING", "/etc/rc2.d/S99RISING", "/etc/rc5.d/S99RISING", "/etc/rc0.d/K01RISING", "/etc/rc6.d/K01RISING"};
    for (int i=0; i<5; i++) {
        if (Utils::Path::FileAvailed(link_file[i])) {
            unlink(link_file[i].c_str());
        }
    }
    if (Utils::Path::FileAvailed(file_name)) {
        remove(file_name.c_str());
    }
}

int SetupApplication::RemoveDesktopFile() {
    const char *desktop_file = "RAVbdfz.desktop";
    const char *desktop_name[] = {"Desktop", "桌面"};

    for (int i=0; i<sizeof(desktop_name)/sizeof(char*); ++i) {
        string deskfile = Utils::Path::JoinPath("/root", desktop_name[i], desktop_file, NULL);
        if (Utils::Path::FileAvailed(deskfile)) {
            unlink(deskfile.c_str());
        }
    }

    DIR *dir = opendir("/home");
    if (NULL == dir) {
        return -1;
    }
    struct dirent *tmp = NULL;
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = false;
#if defined (COMMON_LINUX)
        isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type)
#endif
        {
            char tempPath[1024] = {0};
            snprintf(tempPath, 1023, "%s/%s", "/home", tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
        }
        if (isdir) {
            for (int i=0; i<sizeof(desktop_name)/sizeof(char*); ++i) {
                string deskfile = Utils::Path::JoinPath("/home", tmp->d_name, desktop_name[i], desktop_file, NULL);
                if (Utils::Path::FileAvailed(deskfile)) {
                    unlink(deskfile.c_str());
                }
            }
        }
    }
    closedir(dir);
    return 0;
}

int SetupApplication::AddAutoStart(const char *script_name) {
    string path_prefix = "/";
    string start = Utils::Path::JoinPath(mInstallPath.c_str(), "bin", NOTIFY_RJJH_COMMAND, NULL);

    string spath = Utils::Path::JoinPath(path_prefix.c_str(), "etc/xdg/autostart", script_name, NULL);
    ofstream ofs(spath.c_str());
    if (!ofs) {
        return -1;
    }

    ofs << "[Desktop Entry]" << endl;
    ofs << "Name=RAV Notify" << endl;
    ofs << "Name[zh_CN]=瑞星ESM" << endl;
    ofs << "Exec=" + start << endl;
    ofs << "Terminal=false" << endl;
    ofs << "Type=Application" << endl;
    ofs << "NoDisplay=true" << endl;
    ofs << "Categories=System;Utility;" << endl;
    ofs << "StartupNotify=false" << endl;
    ofs << "X-GNOME-Autostart-Phase=Applications" << endl;
    ofs << "X-GNOME-Autostart-Notify=false" << endl;
    ofs << "X-GNOME-Autostart-enabled=true" << endl;
    ofs << "X-GNOME-Autostart-Delay=15" << endl;
    ofs << "X-KDE-autostart-phase=1" << endl;
    ofs << "X-KDE-autostart-after=panel" << endl;
    ofs << "X-MATE-Autostart-Phase=Applications" << endl;
    ofs << "X-MATE-Autostart-Delay=15" << endl;
    ofs << "X-MATE-Autostart-Notify=false" << endl;

    ofs.close();
    return 0;
}

int SetupApplication::DelAutoStart(const char *script_name) {
    string spath = Utils::Path::JoinPath("/etc/xdg/autostart", script_name, NULL);
    if (Utils::Path::FileAvailed(spath)) {
        unlink(spath.c_str());
    }
    return 0;
}

int SetupApplication::AddPowerOnScript() {
    SECURITY_CHECK(AddRootStartScript(), 0, -1);

    const char *autostart = "/etc/xdg/autostart";
    if (Utils::Path::PathExists(autostart)) {
        SECURITY_CHECK(AddAutoStart(AUTOSTART_FILE_NAME), 0, -1);
    }
    return 0;
}

int SetupApplication::AddApplicationIcon(const char *script_name) {
    string path_prefix = "/";
    string start = Utils::Path::JoinPath(mInstallPath.c_str(), "bin", MAIN_SCREEN_COMMAN, NULL);
    string icon = Utils::Path::JoinPath(mInstallPath.c_str(), "LOGO", "rising.png", NULL);
    string spath = Utils::Path::JoinPath("/usr/share/applications", script_name, NULL);

    if (!Utils::Path::PathExists("/usr/share/applications")) {
        return -1;
    }

    ofstream ofs(spath.c_str());
    if (!ofs) {
        return -1;
    }
    string newName = (mProductInfo.majorname.size()!=0)?mProductInfo.majorname:"瑞星ESM";
//    if (mProductInfo.minorname.size() != 0) {
//        newName += "-";
//        newName += mProductInfo.minorname;
//    }else{
//        newName += u8"-下一代网络版";
//    }

    ofs << "[Desktop Entry]" << endl;
    ofs << "Version=1.0" << endl;
    ofs << "Encoding=UTF-8" << endl;
    ofs << "Name=" << newName <<endl;
    ofs << "GenericName=瑞星ESM" << endl;
    ofs << "Comment=rav" << endl;
    ofs << "Exec=" + start << endl;
    ofs << "Icon=" + icon << endl;
    ofs << "Terminal=false" << endl;
    ofs << "Type=Application" << endl;
    ofs << "StartupNotify=true" << endl;
    ofs << "X-Desktop-File-Install-Version=0.15" << endl;
    ofs << "Name[zh_CN]=" << newName <<endl;
    ofs << "Categories=System;GTK;Utility;" << endl;

    ofs.close();
    return 0;
}

int SetupApplication::DelApplicationIcon(const char *script_name) {
    string spath = Utils::Path::JoinPath("/usr/share/applications", script_name, NULL);
    if (Utils::Path::FileAvailed(spath)) {
        unlink(spath.c_str());
    }
    return 0;
}

int SetupApplication::StartService(const char *script_name) {
    char cmd[512];
    sprintf(cmd, "%s 2>/dev/null &", script_name);
    system(cmd);
    return 0;
}

int SetupApplication::StopAll() {
#ifndef MANUAL_PACK_
    const char *PROC[] = {
        DAEMON_GJCZ_ENTRY,
        UPDATE_PROCESS_ENTRY,
        DAEMON_GLTX_ENTRY,
        DAEMON_ZDFY_ENTRY
    };

    const char *USER_PROC[] = {
        NOTIFYY_ENTRY,
        MAIN_CONSOLE_ENTRY,
        MAIN_SCREEN_ENTRY,
        MAIN_SETTING_ENTRY,
        MAIN_LOGVIEW_ENTRY,
    };

    for (int i=0; i<(int)(sizeof(PROC)/sizeof(char*)); ++i) {
        string path = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, PROC[i], NULL);
        Utils::OS::KillProc(path.c_str(), SIGUSR2);
        KillProc(PROC[i]);
    }

    for (int i=0; i<(int)(sizeof(USER_PROC)/sizeof(char*)); ++i) {
        string path = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, USER_PROC[i], NULL);
        Utils::OS::KillProc(path.c_str(), SIGKILL);
        KillProc(USER_PROC[i]);
    }

    string ko_shell = Utils::Path::JoinPath(mInstallPath.c_str(), DEPLOYED_BIN_FOLDER, "insmod.sh", NULL);
    if (Utils::Path::FileAvailed(ko_shell)) {
        string ko_cmd = ko_shell + " -r " + FILE_DEFEN_MODULE + " > /dev/null 2>&1";
        system(ko_cmd.c_str());
        ko_cmd = ko_shell + " -r " + NET_FILTER_MODULE + " > /dev/null 2>&1";
        system(ko_cmd.c_str());
    }
#endif
    return 0;
}


int SetupApplication::UninstallReport() {
    int ret = 0;
    string sockpath = Utils::Path::JoinPath(mInstallPath.c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::UNINSTALL_REPORT;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundle.Externalize(sockpath.c_str(), ret)) {
        return 0;
    }
    return -1;
}

bool SetupApplication::UninstallPermit() {
    int ret = 0;
    string sockpath = Utils::Path::JoinPath(mInstallPath.c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::UNINSTALL_PROTECT;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 != bundle.Externalize(sockpath.c_str(), ret)) { //if one can't connect to server, permit its uninstallation
        fprintf(stdout, "Get uninstall permission fail,can not connect to server\n");
        return true;
    }
    if (ret == 0) { //have no password
        return true;
    }
    int retry = 0;
    while (retry++ < 3) {
        fprintf(stdout, "Please input uninstall password:>");
        char password[100];
        if (NULL == fgets(password, 100, stdin)) {
            return false;
        }
        if (password[strlen(password)-1] == '\n') {
            password[strlen(password)-1] = 0;
        }
        if (strlen(password) == 0) {
            return false;
        }
        command.mCommandID = CommandBundle::REMOTE_COMMAND;
        command.mIntegerValue = CommandBundle::UNINSTALL_VERIFY;
        command.mStringValue = password;
        RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
        if (0 == bundle.Externalize(sockpath.c_str(), ret)) {
            if (ret==1) { //verification passed
                return true;
            }
        }
    }
    return false;
}

bool SetupApplication::GuiUninstallPermit(const char* password) {
    int ret = 0;
    string sockpath = Utils::Path::JoinPath(mInstallPath.c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);

    CommandBundle command;
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::UNINSTALL_PROTECT;
    RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
    if (0 != bundle.Externalize(sockpath.c_str(), ret)) { //if can't connect to server,permit to uninstall
        //fprintf(stdout, "Get uninstall permission fail,can not connect to server\n");
        return true;
    }
    if (ret == 0) { //have no password
        return true;
    }
    command.mCommandID = CommandBundle::REMOTE_COMMAND;
    command.mIntegerValue = CommandBundle::UNINSTALL_VERIFY;
    command.mStringValue = password;
    RStreamPack<CommandBundle> bundles(RStreamPack<CommandBundle>::STRING, command);
    if (0 == bundles.Externalize(sockpath.c_str(), ret)) {
        if (ret==1) { //verify passed
            return true;
        }
    }
    return false;
}

int SetupApplication::StartUninstall() {
    SECURITY_CHECK(Prepare(), 0, -1);


    if(UninstallPermit() == false){
        cout << "Refused to uninstall this software, please contact to our customer service." << endl; 
        return -1;
    }
    OnError(SETUP_UNINSTALL_CHECKING, 0, NULL);

    if (UninstallPrompt() < 0) {
        return -1;
    }
    if (mCenterType != CENTER_LESS) {
        OnError(SETUP_UNINSTALL_REPORT, 0, NULL);
        UninstallReport();
    }

    OnError(STEP_STOP_RUNNING, 0, NULL);
    StopAll();

    sleep(1);

    int nret = 0;//don't reserve config
//    ReserveConfig();
    //printf("xxx ReserveConfig ret=%d\n", nret);
//    if (nret == 0) { //reserve config file
//         const char *folders[] = {
//            DEPLOYED_BIN_FOLDER,
//            DEPLOYED_LIB_PATH,
//            DEPLOYED_LOG_PATH,
//            FOLDER_RJJH_GJCZ,
//            FOLDER_RJJH_GLTX,
//            FOLDER_RJJH_ZDFY,
//            FOLDER_GJCZ_ETC,
//            FOLDER_RJJH_ETC,
//            "plugins",
//            "LOGO",
//        };
//        for (int i=0; i<(int)(sizeof(folders)/sizeof(char*)); ++i) {
//            string folder = Utils::Path::JoinPath(mInstallPath.c_str(), folders[i], NULL);
//            if (Utils::Path::PathExists(folder)) {
//                Utils::Path::DeleteFolder(folder.c_str());
//            }
//        }
//    }
//    else {
//        Utils::Path::DeleteFolder(mInstallPath.c_str());
//    }
    Utils::Path::DeleteFolder(mInstallPath.c_str());

    OnError(SETUP_UNINSTALL_REMOVE_CONFIG, 0, NULL);

    SECURITY_CHECK(DelSystemdScript(SYSTEMD_RAV_SERVICE), 0, -1);
    SECURITY_CHECK(DelSystemdScript(SYSTEMD_SERVICE_0), 0, -1);
    SECURITY_CHECK(DelSystemdScript(SYSTEMD_SERVICE_1), 0, -1);
    SECURITY_CHECK(DelSystemdScript(SYSTEMD_SERVICE_2), 0, -1);
    SECURITY_CHECK(DelAutoStart(AUTOSTART_FILE_NAME), 0, -1);
    SECURITY_CHECK(DelApplicationIcon(APPLICATION_FILE_NAME), 0, -1);
    DelRCDScripts();
    RemoveDesktopFile();

    string conf = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (Utils::Path::FileAvailed(conf)) {
        unlink(conf.c_str());
    }

    OnError(SETUP_UNINSTALL_FINISHED, 0, NULL);
    return 0;
}

int SetupApplication::CheckResetupInfo() {
    string sconfig = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(sconfig)) {
        sconfig = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, CONFIG_DEF_POLICY, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(sconfig)) {
            return 0;
        }
    }

    AppConfig *cfg = AppConfig::NEW(sconfig);
    if(cfg == NULL) {
        return 0;
    }
    string version = cfg->GetAppVersion();
    if (version >= mProductInfo.appver) {
        OnError(STEP_OVERRIDE_EXISTS, SETUP_OVERRIDE_VERSION_TOO_LOW, NULL);
        delete cfg;
        return -1;
    }
    delete cfg;
    return 0;
}

int SetupApplication::WriteRavConfig() {
    return -1; //ConfigFile::MakeConfig(RAV_CONFIG_FILE, mSerialNumber.c_str(), mInstallPath.c_str());
}

int SetupApplication::GenerateRegisterFile() {
    string config;
    string path_prefix = "/";

    if (mMachineType == MACHINE_LPSECRET) {
        config = Utils::Path::JoinPath(path_prefix.c_str(), "opt/BDFZ/RAV", FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
    }
    else{
        config = Utils::Path::JoinPath(path_prefix.c_str(), "etc", RAV_CONFIG_FILE, NULL);
    }

    AppConfig::GeneralConfig(config);
    AppConfig *cfg = AppConfig::NEW(config);
    if (cfg == NULL) {
        return -1;
    }

    cfg->SetInstallPath(mInstallPath);
    cfg->SetSubComponents(mComponents);
    cfg->SetAppType(RavMachineType[mMachineType]);
    cfg->SetSubProductGUID(RavMachineGUID[mMachineType]);

    cfg->SetProductType(mCenterType); //0 ESM  SOC
    cfg->SetSerialNumber(mSerialNumber);
    if (CENTER_ESM == mCenterType) {
        mProductInfo.eid = "e00000110";
    }
    cfg->SetEID(mProductInfo.eid);
    cfg->SetCenterURL(mCenterAddr);
    cfg->SetAppVersion(mProductInfo.appver);
    cfg->SetVlibVersion(mProductInfo.vlibver);

    time_t tinstall = time(NULL);
    struct tm *p = localtime(&tinstall);
    tinstall = mktime(p);
    cfg->SetUpdateTime(tinstall);
    cfg->SetInstallTime(tinstall);

    //initialize, 3 mean BASE_FUNC|WATCH_DOG
    int iflag= AppConfig::X11_SUPPORT | AppConfig::PROC_GUARD;
    if(m_bFileUsbMonitor)
    {
        //文件监控
        iflag |= AppConfig::FILE_DEFEN;
        //U盘监控
        iflag |= AppConfig::PLUGIN_CHECKER;
    }

    cfg->SetFunctions(iflag);

    if(cfg->StoreConfig() != 0) {
        delete cfg;
        return -1;
    }
    delete cfg;
    return 0;
}

int SetupApplication::GenerateInsmodSh() {
    string path_prefix, insmod_file;
    insmod_file = Utils::Path::JoinPath(mInstallPath.c_str(), "bin/insmod.sh", NULL);
    ofstream ofs(insmod_file.c_str());
    if (!ofs) {
        return -1;
    }
    ofs << "#!/bin/bash" << endl;
    ofs << "function set_env()" << endl;
    ofs << "{" << endl;
    ofs << "    insmod_path=`find /sbin /bin /usr/bin /usr/sbin -iname \"insmod\"`" << endl;
    ofs << "    rmmod_path=`find /sbin /bin /usr/bin /usr/sbin  -iname \"rmmod\"`" << endl;
    ofs << "    insmod_env=`dirname $insmod_path`" << endl;
    ofs << "    rmmod_env=`dirname $insmod_path`" << endl;
    ofs << "    if [ \"$insmod_env\"x = \"$rmmod_env\"x ]; then " << endl;
    ofs << "    path=$insmod_env" << endl;
    ofs << "    else" << endl;
    ofs << "    path=$insmod_env:$rmmod_env" << endl;
    ofs << "    fi" << endl;
    ofs << "    export PATH=$PATH:$path" << endl;
    ofs << "}" << endl;
    ofs << "set_env" << endl;
    ofs << "function usage() {" << endl;
    ofs << "cat << EOF" << endl;
    ofs << "usage: $0 [-i|-r|-h] ko" << endl;
    ofs << "EOF" << endl;
    ofs << "}" << endl;
    ofs << "if [ $# -eq 0 ];then" << endl;
    ofs << "    usage" << endl;
    ofs << "    exit 1" << endl;
    ofs << "elif [ $# -gt 3 ];then" << endl;
    ofs << "    usage" << endl;
    ofs << "    exit 1" << endl;
    ofs << "fi" << endl;
    ofs << "while getopts \":hi:r:\" OPTION; do" << endl;
    ofs << "    case $OPTION in" << endl;
    ofs << "    i)" << endl;
    ofs << "    insmod $OPTARG  " << endl;
    ofs << "    exit" << endl;
    ofs << "    ;;" << endl;
    ofs << "    r)" << endl;
    ofs << "    rmmod -f $OPTARG" << endl;
    ofs << "    exit " << endl;
    ofs << "    ;;" << endl;
    ofs << "    h)" << endl;
    ofs << "    usage" << endl;
    ofs << "    exit" << endl;
    ofs << "    ;;" << endl;
    ofs << "    esac" << endl;
    ofs << "done" << endl;
    ofs << "" << endl;
    ofs.close();
    umask(0);
    chmod(insmod_file.c_str(), 0755);
    return 0;
}

int SetupApplication::GenerateExecScript() {
    string path_prefix, insmod_file;
    insmod_file = Utils::Path::JoinPath(mInstallPath.c_str(), "bin/ravloader.sh", NULL);
    ofstream ofs(insmod_file.c_str());
    if (!ofs) {
        return -1;
    }
	ofs << "#!/bin/sh" << endl;
	ofs << "if [ $# -ne 1 ];then" << endl;
	ofs << "	echo \"Invalid params\"" << endl;
	ofs << "fi" << endl;
	ofs << "" << endl;
	ofs << "KILL() {" << endl;
	ofs << "	ps -ef|grep $1|grep -v grep" << endl;
	ofs << "	if [ $? -eq 0 ]; then" << endl;
	ofs << "		killall -9 $1" << endl;
	ofs << "	fi" << endl;
	ofs << "}" << endl;
	ofs << "" << endl;
	ofs << "RMMOD() {" << endl;
	ofs << "	lsmod|grep $1" << endl;
	ofs << "	if [ $? -eq 0 ];then" << endl;
	ofs << "		rmmod -f $1 >/dev/null 2>&1" << endl;
	ofs << "	fi" << endl;
	ofs << "}" << endl;
	ofs << "" << endl;
	ofs << "ACTION=$1" << endl;
	ofs << "case $ACTION in" << endl;
	ofs << "	start)" << endl;
	ofs << "		" << Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVGJCZ1", NULL) << " >/dev/null 2>&1" << endl;
	ofs << "		" << Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVGLTX1", NULL) << " >/dev/null 2>&1" << endl;
	ofs << "		" << Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVZDFY1", NULL) << " >/dev/null 2>&1" << endl;
	ofs << "		" << Utils::Path::JoinPath(mInstallPath.c_str(), "bin/RAVRJJH1", NULL) << " -s >/dev/null 2>&1" << endl;
	ofs << "		;;" << endl;
	ofs << "	stop)" << endl;
	ofs << "		ALLPROC=\"RAVGJCZ0 RAVGLTX0 RAVZDFY0 RAVRJJH0 RAVRJJH2 RAVRJJH4 RAVRJJH6\"" << endl;
	ofs << "		for ONE in $ALLPROC" << endl;
	ofs << "		do" << endl;
	ofs << "			KILL $ONE >/dev/null 2>&1" << endl;
	ofs << "		done" << endl;
	ofs << "		ALLMOD=\"ravdefen ravfilter\"" << endl;
	ofs << "		for MOD in $ALLMOD" << endl;
	ofs << "		do" << endl;
	ofs << "			RMMOD $MOD" << endl;
	ofs << "		done" << endl;
	ofs << "		;;" << endl;
	ofs << "	*)" << endl;
	ofs << "		;;" << endl;
	ofs << "esac" << endl;
	ofs << "" << endl;
    ofs.close();
    umask(0);
    chmod(insmod_file.c_str(), 0755);
    return 0;
}


int SetupApplication::CheckFunctions() {
    return AppConfig::BASE_FUNCTION|AppConfig::X11_SUPPORT|AppConfig::PROC_GUARD;
}

int SetupApplication::ProcessQt5Librarys() {
    string platforms, srclib, linkname;
    string binpath, libpath;
    string path_prefix = mInstallPath;

    string libname[4] = {"libQt5Core.so", "libQt5Widgets.so", "libQt5Gui.so", "libQt5XcbQpa.so"};

    binpath = Utils::Path::JoinPath(mInstallPath.c_str(), "bin", NULL);
    libpath = Utils::Path::JoinPath(mInstallPath.c_str(), "lib", NULL);

    linkname = Utils::Path::JoinPath(libpath.c_str(), "libQt5Core.so.5", NULL);
    if (Utils::Path::FileAvailed(linkname)) {
        return 0;
    }

    platforms = Utils::Path::JoinPath(binpath.c_str(), "platforms", NULL);
    if (Utils::Path::PathExists(platforms) == false) {
        Utils::Path::mkdirs(platforms);
    }
    srclib = Utils::Path::JoinPath(libpath.c_str(), "libqxcb.so", NULL);
    linkname = Utils::Path::JoinPath(platforms.c_str(), "libqxcb.so", NULL);
    if (Utils::Path::FileAvailed(linkname) == false) {
        symlink(srclib.c_str(), linkname.c_str());
    }
    for(int i=0; i<4; i++) {
        srclib = Utils::Path::JoinPath(libpath.c_str(), libname[i].c_str(), NULL);
        linkname = srclib + ".5";
        if (Utils::Path::FileAvailed(linkname)) {
            continue;
        }
        symlink(srclib.c_str(), linkname.c_str());
    }
    return 0;
}

bool SetupApplication::checkQt5Support() {
    int i, j;
    bool bfind = false;
    string files_qt[] = {"libQt5Core.so.5", "libQt5Widgets.so.5", "libQt5Gui.so.5", "libQt5XcbQpa.so.5"};
    vector<string> folders;

    folders.push_back("/usr/lib64");
    folders.push_back("/lib64");
    folders.push_back("/usr/lib");
    folders.push_back("/usr/local/lib");
    folders.push_back("/usr/local/lib64");
    
    vector<string>::iterator it = folders.begin();
    while(it != folders.end()) {
        string path = *it;
        j = 0;
        for( i=0; i<4; i++ ) {
            if(Utils::Path::PathExists(path) &&  RecursiveFindFile(path, files_qt[i])) {
                j++;
            }
        }
        if (j == 4) {
            bfind = true;
            break;
        }
        it++;
    }
    return bfind;
}

int SetupApplication::DoAutoInstall() {


    return 0;
}

void SetupApplication::SetupKernelModule() {
#ifdef MANUAL_PACK_
    std::string kernel_version = "";
#else
    std::string kernel_version = Utils::OS::GetKernelVersionString();
#endif

    if (kernel_version == "") {
        return;
    }
    string path = Utils::Path::JoinPath(mInstallPath.c_str(), PLUGINS_PATH, NULL);
    DIR *p_dir = NULL;
    struct dirent *ent = NULL;
    struct stat st;

    if((p_dir = opendir(path.c_str())) == NULL){
        return;
    }
    chdir(path.c_str());
    while(NULL != (ent = readdir(p_dir))) {
        lstat(ent->d_name, &st);

        if(S_IFREG & st.st_mode) {
            if (strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
                continue;
            string fname = ent->d_name;
            if(fname.find(".ko") == std::string::npos || fname.find("_") == std::string::npos)
                continue;
            string fullpath = Utils::Path::JoinPath(path.c_str(), ent->d_name, NULL);
            if(fname.find(kernel_version) == std::string::npos) {
                //remove file
                if(remove(fullpath.c_str()) != 0) {
                    printf("Error,SetupApplication::SetupKernelModule() remove %s fail\n", fullpath.c_str());
                }
           // }else{
           //     //rename file
           //     vector<string> secs;
           //     Utils::String::Split(fname, "_", &secs);
           //     string kofile = secs[0] + ".ko";
           //     Utils::Path::CopyFile(fullpath, Utils::Path::JoinPath(path.c_str(), kofile.c_str(), NULL));
           //     if(remove(fullpath.c_str()) != 0){
           //         printf("Error,SetupApplication::SetupKernelModule() remove %s fail\n", fullpath.c_str());
           //     }
            }
            //printf("file:%s\n", ent->d_name);
        }
    }
    chdir("..");
    closedir(p_dir);
}

#ifdef SETUP_SERIAL_CHECK
int SetupApplication::CheckSerial(const std::string& serial, const std::string& configfile) {
    UNIXSNPARAM params;
    memset(&params, 0, sizeof(params));

    int ret = -1;
    void *handle = NULL;
    int err = InitCfgConfig(0, configfile.c_str(), &handle);
    if (UNIX_RS_ERROR_OK == err) {
        if (UNIX_RS_ERROR_OK == CheckCfgSnValid(handle, 0, serial.c_str(), &params)) {
            switch (params.nOsVer) {
            case 12:
            case 13:
            case 14:
            case 15:
                return params.nOsVer;
            default:
                ret = 0;
                break;
            }
        }
    }
    else {
        ret = -1;
    }
    ExitCfgConfig(0, &handle);
    return ret;
}
#endif


int SetupApplication::KillProc(const char *app) {
    int status = -1;
    string pid;
    string cmd = "pidof ";
    cmd += app;
    pid = Utils::OS::GetCmdResult(cmd.c_str());
    if (atoi(pid.c_str()) > 0) {
        status = kill(atoi(pid.c_str()), SIGKILL);
    }
    return status;
}

bool SetupApplication::RecursiveFindFile(const std::string &dir_full_path, const std::string &file_name)
{    
    std::string full_name = Utils::Path::JoinPath(dir_full_path.c_str(), file_name.c_str(), NULL);
    if (Utils::Path::FileAvailed(full_name)) {
        return true;
    }
    DIR* dirp = opendir(dir_full_path.c_str());    
    if (!dirp) {
        return -1;
    }
    struct dirent *dir;
    struct stat st;
    while((dir = readdir(dirp)) != NULL)
    {
        if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) {
            continue;
        }    
        std::string sub_path = Utils::Path::JoinPath(dir_full_path.c_str(), dir->d_name, NULL);
        if (lstat(sub_path.c_str(),&st) == -1) {
            continue;
        }    
        if (S_ISDIR(st.st_mode)) {
            if (RecursiveFindFile(sub_path, file_name)) {
                return true;
            }
        }
    }
    closedir(dirp);
    return false;
}

