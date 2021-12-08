#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <iostream>
#include <fstream>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "../../CommonAPI/crypt/md5.h"
#include "../../CommonAPI/zip/unixzip.h"
#include "ravconsolesetup.h"
#include "commonapi.h"


const char *g_szLib[] = {};
using namespace std;


RavConsoleSetup::RavConsoleSetup() {
    mInstallMode = INSTALL_MANUAL;
    m_bUsingX11Uninstall = false;

    mInstalledAction = "";
    mLockPath = 0;
}
int RavConsoleSetup::JudgeSpecialMachine()
{
    struct utsname name;
    if (uname (&name) == -1) {
        return -1;
    }
    int count = RavGuoChanNodeNameCount;
    for(int i=0; i<count; i++){
        if(strstr(name.nodename, RavGuoChanNodeName[i].c_str()) != NULL){
            mMachineType = MACHINE_LCSERVER;
            return 0;
        }
    }
    return -1;
}

void RavConsoleSetup::SetLPSecServer(bool is_sec_machine) {
    if(is_sec_machine) {
        mMachineType = MACHINE_LPSECRET;
    }else{
        if(access("/etc/os-release", F_OK) == 0) {
            FILE *fp = NULL;
            char sline[255];
            if((fp = fopen("/etc/os-release","r")) != NULL) {
                if(fgets(sline, 254, fp) != NULL) {
                    int count = RavGuoChanOSFlagCount; //sizeof(RavGuoChanOsFlag)/sizeof(RavGuoChanOsFlag[0]);
                    for(int i=0; i<count; i++){
                        if(strstr(sline, RavGuoChanOsFlag[i].c_str()) != NULL){
                            mMachineType = MACHINE_LCSERVER;
                            fclose(fp);
                            return;
                        }
                    }    
                }                       
            }
            fclose(fp);
        }else if(access("/etc/issue", F_OK) == 0) {
            FILE *fp = NULL;
            char sline[255];
            if((fp = fopen("/etc/issue","r")) != NULL) {
                if(fgets(sline, 254, fp) != NULL) {
                    int count = RavGuoChanIssueFlagCount; //sizeof(RavGuoChanIssueFlag)/sizeof(RavGuoChanIssueFlag[0]);
                    for(int i=0; i<count; i++){
                        if(strstr(sline, RavGuoChanIssueFlag[i].c_str()) != NULL){
                            mMachineType = MACHINE_LCSERVER;
                            fclose(fp);
                            return;
                        }
                    }    
                }                       
            }
            fclose(fp);
        }
        if ( JudgeSpecialMachine() != 0 ){
            mMachineType = MACHINE_LGSERVER;
        }
    }
}


int RavConsoleSetup::BuildRpkFile(const char *input_folder, const char *out_file) {
    SETUPTAIL  SetupTail;
    FILE *fp=fopen(out_file, "w");
    if(!fp){
        return -1;
    }
    memset(&SetupTail, 0, sizeof(SETUPTAIL));
    if(PackDirectory(fp, input_folder, &SetupTail, input_folder)) {
        fclose(fp);
        return -1;
    }
    strcpy(SetupTail.strVersion, "0.0.0.0");
    time_t curTime=time(NULL);
    struct tm *stTime=localtime(&curTime);
    sprintf(SetupTail.strPackedtime,"%04d-%02d-%02d %02d:%02d",stTime->tm_year+1900,stTime->tm_mon+1,stTime->tm_mday,
        stTime->tm_hour,stTime->tm_min);

    if(PackSetupTail(fp, &SetupTail)) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

int RavConsoleSetup::GetFileType(const char *strFile)
{
    struct stat stBuf;
    if(stat(strFile,&stBuf)==-1)
        return -1;
    if(S_ISDIR(stBuf.st_mode))
        return 1;
    else if(S_ISREG(stBuf.st_mode))
        return 2;
    else if(S_ISLNK(stBuf.st_mode))
        return 3;
    return -1;
}

int RavConsoleSetup::PackDirectory(FILE *fpSetup, const char *strFile, SETUPTAIL *pSetupTail, const char *szInstPath) {
    DIR *pDir;
    struct dirent *stDirRead;
    char strTempFile[256];

    switch(GetFileType(strFile)) {
        case 1:
            pDir=opendir(strFile);
            while((stDirRead=readdir(pDir))!=NULL) {
                if(  strcmp(stDirRead->d_name,".")==0||strcmp(stDirRead->d_name,"..")==0)
                        continue;
                else
                {
                    sprintf(strTempFile,"%s",strFile);
                    if(strTempFile[strlen(strTempFile)-1]=='/')
                        strTempFile[strlen(strTempFile)-1]='\0';

                    strcat(strTempFile,"/");
                    strcat(strTempFile,stDirRead->d_name);
                    if(PackDirectory(fpSetup,strTempFile,pSetupTail,szInstPath))
                        return -1;
                }
            }
            closedir(pDir);
            break;
        case 2:
            {
                int nNeedPack=1;
                if(strstr(strFile,"/lib/"))
                {
                    int nI=0;
                    while(g_szLib[nI])
                    {
                        if(strstr(strFile,g_szLib[nI]))
                        {
                            nNeedPack=0;
                            break;
                        }
                        nI++;
                    }
                }
                if(nNeedPack)
                {
                    if(PackFile(fpSetup,strFile,pSetupTail,szInstPath))
                        return -1;
                }
            }
            break;
        case 3:
            break;
        default :
            break;
    }
    return 0;
}

int RavConsoleSetup::PackFile(FILE *fpSetup, const char *strFile,SETUPTAIL *pSetupTail, const char *strPackPath)
{
    FILE *fpTemp;
    CUNIXZIP cZipFile;
    char szTmpFile[256];
    SETUPTOC SetupTOC;
    size_t  nReturn;
    struct stat status;
    memset(&SetupTOC,0,sizeof(SetupTOC));

    if(stat(strFile,&status)) {
        return -1;
    }
    strcpy(SetupTOC.szOrigMd5, GetFileMD5(strFile).c_str());
    SetupTOC.mode=status.st_mode;

    char *szOnlyname=(char *)strrchr(strFile,'/');
    if(szOnlyname) {
        szOnlyname++;
        strcpy(szTmpFile,szOnlyname);
    }else {
        strcpy(szTmpFile,strFile);
    }
    strcat(szTmpFile,".GZ");
    cZipFile.gz_init(strFile,szTmpFile,false,0);
    if(cZipFile.gz_compress()) {
        return -1;
    }
    if(stat(szTmpFile,&status)) {
        return -1;
    }
    strcpy(SetupTOC.szZipMd5, GetFileMD5(szTmpFile).c_str());
    SetupTOC.nSize=status.st_size;
    nReturn=strlen(strPackPath);
    if(strPackPath[nReturn-1] != '/') {
        nReturn += 1;
    }
    strcpy(SetupTOC.strFile,strFile+nReturn);
    if((fpTemp=fopen(szTmpFile,"rb")) == NULL) {
        return -1;
    }
    if(fwrite(&SetupTOC,sizeof(SETUPTOC),1,fpSetup)!=1) {
        fclose(fpTemp);
        fclose(fpSetup);
        unlink(szTmpFile);
        return -1;
    }
    int nReadSize=65536;
    char *pBuff=new char[nReadSize];

    memset(pBuff,0,nReadSize);
    nReturn=0;
    while((nReturn=fread(pBuff,1,nReadSize,fpTemp))>0) {
        if(fwrite(pBuff,1,nReturn,fpSetup)!=nReturn) {
            delete []pBuff;
            fclose(fpTemp);
            fclose(fpSetup);
            unlink(szTmpFile);
            return -1;
        }
    }

    fflush(NULL);
    fclose(fpTemp);

    unlink(szTmpFile);
    pSetupTail->nSize+=status.st_size;
    pSetupTail->nFileNum+=1;
    delete [] pBuff;
    return 0;
}

int RavConsoleSetup::PackSetupTail(FILE *fpSetup, SETUPTAIL *pstTail) {
    strcpy(pstTail->strSignature,SIGNATURE);
    strcpy(pstTail->strNetRegFile,RAVNETWRD_REGISTER_FILE);
    strcpy(pstTail->strSingleRegFile,RAVNETWRD_REGISTER_FILE);

    if(fwrite(pstTail,sizeof(SETUPTAIL),1,fpSetup)!=1) {
        fclose(fpSetup);
        return -1;
    }
    return 0;
}

std::string RavConsoleSetup::GetFileMD5(const std::string& path) {
    unsigned char data[1024];
    FILE *file = fopen(path.c_str(), "rb");
    if (NULL == file) {
        return "";
    }

    MD5 md5;
    while (1) {
        int readed = fread(data, 1, 1024, file);
        if (readed <= 0) {
            fclose(file);
            break;
        }
        md5.update(data, readed);
    }
    md5.finalize();
    return md5.hexdigest();
}

int RavConsoleSetup::ShowWelcomeScreen() {
    fprintf(stdout,"************************************************************************\n");
    fprintf(stdout,"*                                                                      *\n");
    fprintf(stdout,"*                                                                      *\n");
    fprintf(stdout,"*                 RISING ANTIVIRUS SETUP SOFTWARE                      *\n");
    fprintf(stdout,"*       COPYRIGHT FOR BEIJING RISING TECHNOLOGY CO.,LTD. 2015-2020     *\n");
    fprintf(stdout,"*                                                                      *\n");
    fprintf(stdout,"************************************************************************\n");

    return 0;
}

const string license_txt_contents = "Software License Agreement\n\
1. This is an agreement between Licensor and Licensee, who is being licensed to use the named Software.\n\
2. Licensee acknowledges that this is only a limited nonexclusive license. Licensor is and remains the owner of all titles, rights, and interests in the Software.\n\
3. This License permits Licensee to install the Software on more than one computer system, as long as the Software will can be used on more than one computer system simultaneously. Licensee will not make copies of the Software or allow copies of the Software to be made by others, unless authorized by this License Agreement. Licensee may make copies of the Software for backup purposes only.\n\
4. This Software is subject to a limited warranty. Licensor warrants to Licensee that the physical medium on which this Software is distributed is free from defects in materials and workmanship under normal use, the Software will perform according to its printed documentation, and to the best of Licensor's knowledge Licensee's use of this Software according to the printed documentation is not an infringement of any third party's intellectual property rights. This limited warranty lasts for a period of 365 days after delivery. To the extent permitted by law, THE ABOVE-STATED LIMITED WARRANTY REPLACES ALL OTHER WARRANTIES, EXPRESS OR IMPLIED, AND LICENSOR DISCLAIMS ALL IMPLIED WARRANTIES INCLUDING ANY IMPLIED WARRANTY OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, OR OF FITNESS FOR A PARTICULAR PURPOSE. No agent of Licensor is authorized to make any other warranties or to modify this limited warranty. Any action for breach of this limited warranty must be commenced within one year of the expiration of the warranty. Because some jurisdictions do not allow any limit on the length of an implied warranty, the above limitation may not apply to this Licensee. If the law does not allow disclaimer of implied warranties, then any implied warranty is limited to 365 days after delivery of the Software to Licensee. Licensee has specific legal rights pursuant to this warranty and, depending on Licensee's jurisdiction, may have additional rights.\n\
5. In case of a breach of the Limited Warranty, Licensee's exclusive remedy is as follows: Licensee will return all copies of the Software to Licensor, at Licensee's cost, along with proof of purchase. (Licensee can obtain a step-by-step explanation of this procedure, including a return authorization code, by contacting Licensor at [address and toll free telephone number].) At Licensor's option, Licensor will either send Licensee a replacement copy of the Software, at Licensor's expense, or issue a full refund.\n\
6. Notwithstanding the foregoing, LICENSOR IS NOT LIABLE TO LICENSEE FOR ANY DAMAGES, INCLUDING COMPENSATORY, SPECIAL, INCIDENTAL, EXEMPLARY, PUNITIVE, OR CONSEQUENTIAL DAMAGES, CONNECTED WITH OR RESULTING FROM THIS LICENSE AGREEMENT OR LICENSEE'S USE OF THIS SOFTWARE. Licensee's jurisdiction may not allow such a limitation of damages, so this limitation may not apply.\n\
7. Licensee agrees to defend and indemnify Licensor and hold Licensor harmless from all claims, losses, damages, complaints, or expenses connected with or resulting from Licensee's business operations.\n\
8. Licensor has the right to terminate this License Agreement and Licensee's right to use this Software upon any material breach by Licensee.\n\
9. Licensee agrees to return to Licensor or to destroy all copies of the Software upon termination of the License.\n\
10. This License Agreement is the entire and exclusive agreement between Licensor and Licensee regarding this Software. This License Agreement replaces and supersedes all prior negotiations, dealings, and agreements between Licensor and Licensee regarding this Software.\n\
11. This License Agreement is governed by the law of  China applicable to China contracts.\n\
12. This License Agreement is valid without Licensor's signature. It becomes effective upon the earlier of Licensee's signature or Licensee's use of the Software.\n\
";

int RavConsoleSetup::AgreeLisence() {
    while(1){
        if (mInstallMode == INSTALL_AUTO) {
            return 0;
        }

        fprintf(stdout, "Do you agree the license of this software?(yes/no/view)[yes]>");
        char stmp[10] = "";
        if (NULL == fgets(stmp, 10, stdin)) {
            return 0;
        }
        if (stmp[strlen(stmp)-1] == '\n') {
            stmp[strlen(stmp)-1] = 0;
        }
        if (0 == strcasecmp(stmp, "Y") || 0 == strcasecmp(stmp, "yes") || 0 == strcmp(stmp, "")){
            return 0;
        }
        if (0 == strcasecmp(stmp, "N") || 0 == strcasecmp(stmp, "no")){
            return -1;
        }
        if (0 == strcasecmp(stmp, "V") || 0 == strcasecmp(stmp, "view")){
            fprintf(stdout,"************************************************************************\n");
            fprintf(stdout, "%s\n", license_txt_contents.c_str());
            fprintf(stdout,"************************************************************************\n");
            //todo show license txt
//                fprintf(stdout, "Do you agree this license?(yes/no)>");
//                if (NULL == fgets(stmp, 10, stdin)) {
//                    return -1;
//                }
//                if (stmp[strlen(stmp)-1] == '\n') {
//                    stmp[strlen(stmp)-1] = 0;
//                }
//                if (0 == strcasecmp(stmp, "Y") || 0 == strcasecmp(stmp, "yes")){
//                    return 0;
//                }
        }
    }
    return -1;
}

int RavConsoleSetup::StartApplicationPrompt(){
    if (mInstallMode == INSTALL_AUTO || mInstalledAction == "3"){
        return 0;
    }

    fprintf(stdout, "This software is installed successfully,do you want to start it?(yes/no)[yes]>");
    char stmp[10];
    if (NULL == fgets(stmp, 10, stdin)) {
        return -1;
    }
    if (stmp[strlen(stmp)-1] == '\n') {
        stmp[strlen(stmp)-1] = 0;
    }
    if (0 == strcasecmp(stmp, "Y") || 0 == strcasecmp(stmp, "yes") || 0 == strcmp(stmp, "")){
        return 0;
    }
    return -1;
}


int RavConsoleSetup::ReInstallPromtp() {
    if (mInstallMode == INSTALL_AUTO || mInstalledAction == "3"){
        return 0;
    }

    fprintf(stdout, "This software is already installed, do you want reinstall it again?(yes/no)>");
    char stmp[10];
    if (NULL == fgets(stmp, 10, stdin)) {
        return -1;
    }
    if (stmp[strlen(stmp)-1] == '\n') {
        stmp[strlen(stmp)-1] = 0;
    }
    if (0 == strcasecmp(stmp, "Y") || 0 == strcasecmp(stmp, "yes")){
        return 0;
    }
    return -1;
}

int RavConsoleSetup::GetInstallPath(std::string &install_path) {
    std::string sconfig = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
    if (!Utils::Path::FileAvailed(sconfig)) {
        sconfig = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, CONFIG_DEF_POLICY, RAV_CONFIG_FILE, NULL);
        if (!Utils::Path::FileAvailed(sconfig)) {
            return -1;
        }
    }

    AppConfig *cfg = AppConfig::NEW(sconfig);
    if(cfg == NULL) {
        return -1;
    }
    install_path = cfg->GetInstallPath();
    delete cfg;
    return 0;
}

int RavConsoleSetup::GetInstallPathPrompt(std::string& installPath) {
    if (mInstallMode == INSTALL_AUTO) {
        if (installPath.empty()) {
            installPath = "/opt/BDFZ/RAV";
        }
        return 0;
    }

    printf("Please input the installing path(must be absolute path),\n");
    printf("You can install RISING ANTIVIRUS SOFTEARE in \n[\"%s\" by Typing Enter]>", mInstallPath.length()==0?"/opt/BDFZ/RAV":mInstallPath.c_str());
    fflush(NULL);

    char tmp[256];
    if (NULL == fgets(tmp, 256, stdin)){
        return -1;
    }
    if (tmp[strlen(tmp)-1] == '\n') {
        tmp[strlen(tmp)-1] = 0;
    }
    if (strlen(tmp) == 0) {
        strcpy(tmp, mInstallPath.length()==0?"/opt/BDFZ/RAV":mInstallPath.c_str());
    }
    installPath = tmp;
    return 0;
}

SetupApplication::SoftType RavConsoleSetup::GetSoftwareType() {
    if (mInstallMode == INSTALL_AUTO) {
        return mCenterType;
    }

    fprintf(stdout, "Please select software type:\n");
    if (mCenterType != CENTER_NETWARE) {
        fprintf(stdout, "0:Single antivirus software, no center server(Default)\n");
    } else {
        if (mCenterType == CENTER_NETWARE) {
            mCenterType = CENTER_SOC;
        }
    }

    mCenterType = SetupApplication::CENTER_LESS;
    fprintf(stdout, "1:ESM antivirus client\n");
    fprintf(stdout, "2:SOC antivirus client\n");
    fprintf(stdout, "Software type [\"%d\" by Typing Enter]>", mCenterType);
    char stmp[20];
    if (NULL == fgets(stmp, 20, stdin)) {
        return mCenterType;
    }
    if (stmp[strlen(stmp)-1] == '\n') {
        stmp[strlen(stmp)-1] = 0;
    }

    int nresult = 1;
    if (strlen(stmp) != 0) {
       nresult = atoi(stmp);
    }else{
        return (SetupApplication::SoftType)(mCenterType); //default
    }

    int type;
    switch(nresult){
        case 0:
            type = SetupApplication::CENTER_LESS;
            break;
        case 1:
            type = SetupApplication::CENTER_ESM;
            break;
        case 2:
            type = SetupApplication::CENTER_SOC;
            break;
        default:
            type = CENTER_LESS;
            break;
    }
    return (SoftType)type;
}

int RavConsoleSetup::GetSerialNumber(std::string& serial) {
    if(!mSerialNumber.empty())
        return 0;

    printf("\nPlease input the serial number of RISING ANTIVIRUS SOFTWARE>");
    fflush(NULL);

    char tmp[32];
    if (NULL == fgets(tmp, 32, stdin)){
        return -1;
    }
    if (tmp[strlen(tmp)-1] == '\n') {
        tmp[strlen(tmp)-1] = 0;
    }
    serial = tmp;
    return 0;
}

int RavConsoleSetup::GetCenterAddr(std::string& center) {
    if(mCenterType == SetupApplication::CENTER_LESS)
        return 0;
    if (mInstallMode == INSTALL_AUTO && !mCenterAddr.empty()) {
        fprintf(stdout, "Center addr is %s\n", mCenterAddr.c_str());
        return 0;
    }

    fprintf(stdout,"********************************************************** \n");
    fprintf(stdout,"*                                                        * \n");
    fprintf(stdout,"*                 Please input Center URL                * \n");
    fprintf(stdout,"*         For Example:                                   * \n");
    fprintf(stdout,"*           http://xx.xx.xx.xx                           * \n");
    fprintf(stdout,"*           https://xx.xx.xx.xx:443                      * \n");
    fprintf(stdout,"*                                                        * \n");
    fprintf(stdout,"********************************************************** \n");
    fprintf(stdout,"Center URL>%s", mCenterAddr.length()>0?mCenterAddr.c_str():"");
    fflush(stdin);

    char tmp[256];
    if (NULL == fgets(tmp, 256, stdin)) {
        return -1;
    }
    if (tmp[strlen(tmp)-1] == '\n') {
        tmp[strlen(tmp)-1] = 0;
    }
    if(strlen(tmp) < 1)
        center = mCenterAddr;
    else
        center = tmp;
    return 0;
}

int RavConsoleSetup::GetInstallComponents(int &components) {
    if (mInstallMode == INSTALL_AUTO) {
        return 0;
    }

    components = COMPONENT_XAV|COMPONENT_EP;

    // 母盘的版本，暂时不需要选持XAV/XFW
//    int ntmp = 1;
//    do {
//        fprintf(stdout, "Select the funtions of the software:\n");
//        fprintf(stdout, "0:EP.\n");
//        fprintf(stdout, "1:XAV.\n");
////        fprintf(stdout, "2:XFW.\n");
////        fprintf(stdout, "3:ALL.\n");
//        fprintf(stdout, "Please input the functions value\n[\"%d\" by Typing Enter]>",ntmp);

//        char stmp[10];
//        if (NULL == fgets(stmp, 10, stdin)) {
//            return -1;
//        }

//        if (stmp[strlen(stmp)-1] == '\n') {
//            stmp[strlen(stmp)-1] = 0;
//        }

//        if (strlen(stmp) != 0) {
//            components = atoi(stmp);
//        }
//        else {
//            components = COMPONENT_EP;
//        }
//    } while (0);
    return 0;
}

int RavConsoleSetup::StartInstall() {
    if (0 == SetupApplication::StartInstall()) {
        if (mInstallMode == INSTALL_AUTO) {
            FILE *f = fopen(Utils::Path::JoinPath(mInstallPath.c_str(), PLUGINS_PATH, "autoinstall", NULL).c_str(), "w+");
            if (f) {
                fclose(f);
            }
        }
    }
    return 0;
}

int RavConsoleSetup::Prepare() {
    return GetProductInfo(mProductInfo);
}

//defined from appconfig.h
//BASE_FUNCTION   = 0x0,
//X11_SUPPORT     = 0x1,
//PROC_GUARD      = 0x2,
//FILE_DEFEN      = 0x4,
//NET_FILTER      = 0x8,
//CONTEXT_MENU    = 0x10,
//PLUGIN_CHECKER  = 0x20,
//SECRET_VERSION  = X11_SUPPORT|FILE_DEFEN|NET_FILTER|CONTEXT_MENU|PLUGIN_CHECKER,
int RavConsoleSetup::GetInstallFunctionsCodePrompt(int &installFunctions) {
    if (mInstallMode == INSTALL_AUTO) {
        installFunctions = 3;
        return 0;
    }

    installFunctions = 3;

//    while(1){
//        printf("Please input the install function code,it depends on your selected module of install.\n");
//        printf("\tBASE_FUNCTION   = 0x0\n");
//        printf("\tX11_SUPPORT     = 0x1\n");
//        printf("\tPROC_GUARD      = 0x2\n");
//        printf("\tFILE_DEFEN      = 0x4\n");
//        printf("\tNET_FILTER      = 0x8\n");
//        printf("\tCONTEXT_MENU    = 0x10\n");
//        printf("\tPLUGIN_CHECKER  = 0x20\n");
//        printf("\tSECRET_VERSION  = 0x3D (X11_SUPPORT|FILE_DEFEN|NET_FILTER|CONTEXT_MENU|PLUGIN_CHECKER)\n");

//        printf("You can input the function code as \n[\"0x%02X\" by Typing Enter]>", FunctionsCode[mFunctionCode]);
//        fflush(NULL);

//        char tmp[256];
//        if (NULL == fgets(tmp, 256, stdin)){
//            return -1;
//        }
//        if (tmp[strlen(tmp)-1] == '\n') {
//            tmp[strlen(tmp)-1] = 0;
//        }
//        if (strlen(tmp) == 0) {
//            installFunctions = FunctionsCode[mFunctionCode];
//        }else{
//            if (strstr(tmp, "0x") == NULL) {
//                installFunctions = atoi(tmp);
//            }else{
//                installFunctions = strtol(tmp, NULL, 16);
//            }
//        }
//        if (installFunctions >= 0 and installFunctions <= 0x3F)
//            break;
//    }
    return 0;
}

int RavConsoleSetup::UninstallPrompt() {     
    fprintf(stdout, "This software is already installed, do you want to continue?(yes/no)\n[\"yes\" by Typing Enter]>");
    char stmp[10];
    if (NULL == fgets(stmp, 10, stdin)) {
        return -1;
    }
    if (stmp[strlen(stmp)-1] == '\n') {
        stmp[strlen(stmp)-1] = 0;
    }
    if (strlen(stmp) == 0) {
        return 0;
    }
    if (strcasecmp(stmp, "Y") == 0 || strcasecmp(stmp, "yes") == 0){
        return 0;
    }
    return -1; 
}

int RavConsoleSetup::ReserveConfig() {
    fprintf(stdout, "Do you want to reserve the config file?(yes/no)\n[\"yes\" by Typing Enter]>");
    char stmp[10];
    if (NULL == fgets(stmp, 10, stdin)) {
        return -1;
    }
    if (stmp[strlen(stmp)-1] == '\n') {
        stmp[strlen(stmp)-1] = 0;
    } 
    if (strlen(stmp) == 0) {
        return 0;
    }
    if (strcasecmp(stmp, "Y") == 0 || strcasecmp(stmp, "yes") == 0){
        return 0;
    }
    return -1; 
}

int RavConsoleSetup::OnError(SetupStep step, int error, void * data) {
    switch (step) {
    case STEP_PREPARE_DATA: {
        fprintf(stdout, "Prepare install data\n");
        break;
    }
    case STEP_CHECK_ENVTIVEMENT: {
        fprintf(stdout, "Check the environment\n");
        break;
    }
    case STEP_STOP_RUNNING: {
        fprintf(stdout, "Stop the running process\n");
        break;
    }
    case STEP_CREATE_FOLDER: {
        fprintf(stdout, "Create the install folder\n");
        break;
    }
    case STEP_OVERRIDE_EXISTS: {
        fprintf(stdout, "The software has exists, override?\n");
        break;
    }
    case STEP_UNPACK_COMMON: {
        fprintf(stdout, "Unpack common component...\n");
        break;
    }
    case STEP_UNPACK_XAV: {
        fprintf(stdout, "Unpack XAV....\n");
        break;
    }
    case STEP_UNPACK_XFW: {
        fprintf(stdout, "Unpack XFW...\n");
        break;
    }
    case STEP_CHECK_QT: {
        fprintf(stdout, "Check QT environment\n");
        break;
    }
    case STEP_RUN_SCRIPT: {
        fprintf(stdout, "Create the running scrpit\n");
        break;
    }
    case STEP_CHECK_SERIAL: {
        fprintf(stdout, "Check the serial\n");
        break;
    }
    case STEP_INSTALL_FINISHED: {
        fprintf(stdout, "Install Finished, enjoy it\n");
        break;
    }
//    case NO_ERROR:
//        fprintf(stdout, "Install finished, enjoy it\n");
//        break;
//    case VERSION_TOO_LOW:
//        fprintf(stdout, "The version is lower than installed\n");
//        break;
//    case PRODUCT_NOT_ASSISTANT:
//        fprintf(stdout, "Product not assistant\n");
//        break;
//    case SERIAL_CHECK: {
//        int *err = (int*)data;
//        switch (*err) {
//        case 12:
//            printf("国产桌面机授权\n");
//            break;
//        case 13:
//            printf("国产服务器授权\n");
//            break;
//        case 14:
//            printf("通用桌面机授权\n");
//            break;
//        case 15:
//            printf("通用服务器授权\n");
//            break;
//        default:
//            printf("序列号不正确");
//            break;
//        }
//    }
    default:
        break;
    }
    return 0;
}

int RavConsoleSetup::GetProductInfoFromXml(const char *file_name, ProductInfo &produce) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile(file_name);
    if (doc == NULL)
        return -1;
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        xmlFreeDoc(doc);
        return -1;
    }
    if (xmlStrcmp(cur->name, BAD_CAST"rpklist")) {
        xmlFreeDoc(doc);
        return -1;
    }
    xmlChar *version = xmlGetProp(cur, BAD_CAST"version");
    xmlChar *update = xmlGetProp(cur, BAD_CAST"updatetime");
    xmlChar* vlibver = xmlGetProp(cur, BAD_CAST"virver");
    produce.appver = reinterpret_cast<char*>(version);
    produce.update = reinterpret_cast<char*>(update);
    produce.vlibver = reinterpret_cast<char*>(vlibver);
    xmlFreeDoc(doc);

    return 0;
}

int RavConsoleSetup::GetProductInfoFromCfg(const char *file_name, ProductInfo &produce) {
    FILE *fp = fopen(file_name, "rb");
    if(fp == NULL) {
       return -1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    char *buf = new char[size+1];
//    unsigned char *sdecoded = new unsigned char[size*2];
    fseek(fp, 0, SEEK_SET);
    size_t readed = 0;
    while(readed < size){
        readed += fread(buf+readed, 1, size*2-readed, fp);
    }
    fclose(fp);
    buf[readed] = 0;

    char *valid_str = buf;
    const unsigned char BOM[] = {0xef, 0xbb, 0xbf};
    if (memcmp(buf, BOM, 3) == 0) {
        valid_str = buf+3;
    }

    int ret = 0;
    string decoded = Utils::Hash::Base64Decode(valid_str);

    string sbuf;
    Json::Value json;
    Json::Reader reader;
    if(reader.parse(decoded, json)){
        mCenterAddr = Utils::JSON::GetJsonString(json, "maurl");
        produce.eid = Utils::JSON::GetJsonString(json, "eid");

        // maybe the flag is center_type
        mCenterType = (SoftType)Utils::JSON::GetJsonInt(json, "center_type");

        produce.appver = Utils::JSON::GetJsonString(json, "version");
        produce.vlibver = Utils::JSON::GetJsonString(json, "virus");

        mInstallMode = (InstallType)Utils::JSON::GetJsonInt(json, "install_mode"); //1: common install  2: auto install
        mInstalledAction = Utils::JSON::GetJsonString(json, "install_finish"); //1: application icon 3: auto start  other: prompt and chose

        mInstallPath = Utils::JSON::GetJsonString(json, "install_path");
        mSerialNumber =  Utils::JSON::GetJsonString(json, "serial");
        //fprintf(stderr, "GetJsonString mSerialNumber:%s\n", mSerialNumber.c_str());


        mLockPath = atoi(Utils::JSON::GetJsonString(json, "lock_path").c_str());

        string rpks = Utils::JSON::GetJsonString(json, "dl_rpk");
        if (rpks.find("xav") != string::npos) {
            mComponents |= COMPONENT_XAV;
        }
        if (rpks.find("xfw") != string::npos) {
            mComponents |= COMPONENT_XFW;
        }

        mHaveConfigFile = true;
    }else{
        ret = -1;
    }
    delete []buf;
//    delete []sdecoded;

    if (produce.eid.empty()) {
        produce.eid = "e00000110";
    }
    return ret;
}

int RavConsoleSetup::GetProductInfo(SetupApplication::ProductInfo &produce) {
    if (GetProductInfoFromXml(SETUP_PRODUCE_XML, produce)) {
        return -1;
    }
    GetProductInfoFromCfg(SETUP_CONFIG_FILE, produce);
    return 0;
}



