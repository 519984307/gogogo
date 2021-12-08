#include "updater.h"
#include "commonapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <vector>
#include <iostream>


Updater::Updater()
{
    mHaveTail = false;
}

Updater::~Updater()
{
}

void Updater::SetTailInfo(int type, const string center_addr)
{
    mServerType = type;
    mTailCenterAddr = center_addr;   
    mHaveTail = true;
}

int Updater::InitInfo(bool is_secret_machine)
{
    mSetup.SetLPSecServer(is_secret_machine);
    mSetup.mInstallPath = DEFAULT_INSTALL_PATH;
    mSetup.mSerialNumber = mCVersion.GetSerial();

    if (mCVersion.Exist()) {
        if (mTailCenterAddr.empty()) {
            string addr, type;
            if (mCVersion.GetCenterAddr(type, addr) > 0) {
                mTailCenterAddr = addr;
            }
        }
    }

    if (mTailCenterAddr.empty()) {
        printf("no availd center url\n");
        return -1;
    }

    cout << "Old version serial number is " << mSetup.mSerialNumber << endl;
    return 0;
}

int Updater::BackupData()
{
    string backup_path = TMP_BACKUP_FILES_PATH;
    if ((!mHaveTail) && (!mCVersion.Exist())) {
        return -1;
    }
    if (Utils::Path::PathExists(backup_path)) {
        Utils::Path::DeleteFolder(backup_path.c_str());
    }
    Utils::Path::mkdirs(backup_path);
    //for isloation zone backup
    string old_path = Utils::Path::JoinPath(mCVersion.mOldInstallPath.c_str(), OLD_VERSION_ISOLATION_ZONE_PATH, NULL);
    string new_path = Utils::Path::JoinPath(TMP_BACKUP_FILES_PATH, OLD_VERSION_ISOLATION_ZONE_PATH, NULL);
    CopyDir(old_path.c_str(), new_path.c_str()); //if not exist, do nothing
    //for log
    new_path = Utils::Path::JoinPath(TMP_BACKUP_FILES_PATH, LOG_XML_FILE_NAME, NULL);
    mCVersion.ExportLogs(new_path);
    return 0;
}

//do this after download all other files
int Updater::DownloadCommonAndParse()
{
    string remote_file_name = "product_" + mVersion + ".xml";
    string server_path = Utils::Path::JoinPath(mSetup.mCenterAddr.c_str(), DL_WEB_PATH, mDlParser.GetMachineType().c_str(), "rpk/common", remote_file_name.c_str(), NULL);
    string xml_data = mDownloader.Download(server_path);
    if (xml_data.length() == 0) {
        cout << "DownloadCommonAndParse get server file fail, server_file:" << server_path << endl;
        return -1;
    }
    mVDBVersion = mDlParser.GetVDBVersionFromXml(xml_data);
    if (mVDBVersion.length() == 0) {
        cout << "DownloadCommonAndParse get server virusdb version fail" << endl;
        return -1;
    }
    mSetup.mProductInfo.appver = mVersion;
    mSetup.mProductInfo.vlibver = mVDBVersion;

//    remote_file_name = "common_" + mVersion + ".rpk";
//    server_path = Utils::Path::JoinPath(mSetup.mCenterAddr.c_str(), DL_WEB_PATH, mDlParser.GetMachineType().c_str(), "rpk/common", remote_file_name.c_str(), NULL);
//    string local_file = Utils::Path::JoinPath(TMP_DOWNLOAD_FILES_PATH, "common.rpk", NULL);
//    if (mDownloader.DownloadFile(server_path.c_str(), local_file.c_str()) != 0) {
//        cout << "Download file fail, " << server_path <<  " --->> " << local_file << endl;
//        return -1;
//    }
//    cout << "Download file successful: " << server_path <<  " --->> " << local_file << endl;
    return 0;
}

int Updater::DownloadInstallFiles()
{
    string type, server_addr, server_path, server_file;
    if ((!mHaveTail) && (0 == mCVersion.GetCenterAddr(type, server_addr)) ) {//not esm or getaddr fail
        cout << "DownloadInstallFiles get center addr fail" << endl;
        return -1;
    }else {
        server_addr = mTailCenterAddr;
    }
    mSetup.mCenterAddr = server_addr;
    server_path = server_addr + "/" + DL_WEB_PATH;
    server_file = Utils::Path::JoinPath(server_path.c_str(), DL_WEB_XML, NULL);
    string xml_data = mDownloader.Download(server_file);
    if (xml_data.length() == 0) {
        cout << "DownloadInstallFiles get server version file fail, server file:" <<  server_file << endl;
        return -1;
    }
    mVersion = mDlParser.GetVersionFromXml(xml_data);
    if (mVersion.length() == 0) {
        return -1;
    }
    server_file = Utils::Path::JoinPath(server_path.c_str(), mDlParser.GetMachineType().c_str(), "rec_", NULL) + mVersion + ".xml";
    cout << "Will download server file:" << server_file << endl;
    xml_data = mDownloader.Download(server_file);
    cout << "File data:" << xml_data << endl;
    if (mDlParser.GetRpkFilesInfoFromXml(xml_data, false) != 0) {
        cout << "DownloadInstallFiles get rpk files from xml fail" << endl;
        return -1;
    }
    string dl_path = TMP_DOWNLOAD_FILES_PATH;
    if (Utils::Path::PathExists(dl_path)) {
        Utils::Path::DeleteFolder(dl_path.c_str());
    }       
    Utils::Path::mkdirs(dl_path);
    int fsize = mDlParser.mDlFiles.size();
    int count = 0;
    string remote_file;
    string local_file;
    for (int i=0; i<fsize; i++) {
        RpkFile *rf = mDlParser.mDlFiles[i];
        remote_file = Utils::Path::JoinPath(server_path.c_str(), mDlParser.GetMachineType().c_str(), "rpk",  rf->name.c_str(), rf->name.c_str(), NULL) + "_" + mVersion + ".rpk";
        local_file = Utils::Path::JoinPath(TMP_DOWNLOAD_FILES_PATH, rf->name.c_str(), NULL) + ".rpk";
        if (mDownloader.DownloadFile(remote_file.c_str(), local_file.c_str()) != 0) {
            cout << "Download file fail, " << remote_file <<  " --->> " << local_file << endl;
            continue;
        }
        //printf("get md5=%s\n", Utils::Hash::GetFileMD5(local_file).c_str());
        if (rf->size != Utils::OS::GetFileSize(local_file) || Utils::String::ToLower(rf->md5) != Utils::Hash::GetFileMD5(local_file)) {
            cout << "Check file size and md5 fail, " << local_file << endl;
            continue;
        }
        cout << "Download file successful: " << remote_file <<  " --->> " << local_file << endl;
        count++;
    }
    if (fsize == 0 || count != fsize) {
        return -1;
    }
    return DownloadCommonAndParse();
}

//get SetupApplication::CENTER_ESM or SetupApplication::CENTER_SOC
//do this after successfully downloaded all rpk files
//todo: judge ESM server flag file
int Updater::GetServerType()
{
    string remote_file = mSetup.mCenterAddr + SERVER_SOC_FLAG_FILE;
    string local_file = Utils::Path::JoinPath(TMP_DOWNLOAD_FILES_PATH, "Api.php", NULL);
    if (Utils::Path::FileAvailed(local_file)) {
        remove(local_file.c_str());
    }
    //printf("xxx Updater::GetServerType  %s --> %s\n", remote_file.c_str(), local_file.c_str());
    if (mDownloader.DownloadFile(remote_file.c_str(), local_file.c_str()) == 0) {
        ifstream fin(local_file.c_str(), ios::in);
        if (!fin){
            cout << "Error, Updater::GetServerType open file fail:" << local_file << endl;
        }else {
            char sline[1024];
            fin.getline(sline, 1024);
            fin.close();
            if (strcmp(sline, "error:1") == 0) {
                cout << "Center type is SOC" << endl;
                mSetup.mCenterType = SetupApplication::CENTER_SOC;
                mSetup.mProductInfo.eid = "e00000110"; //using this eid value,server will dispatch new eid to this machine if its type is soc
                return 0;
            }
        }
    }
    cout << "Center type is ESM" << endl;
    mSetup.mCenterType = SetupApplication::CENTER_ESM;
    return 0;
}

int Updater::UninstallOldFiles()
{
    if (!mCVersion.Exist()) {
        return -1;
    }
    mCVersion.UninstallOldVersion();
    return 0;
}

int Updater::RestoreBackupedData()
{
    //isolation zone
    string old_path = Utils::Path::JoinPath(TMP_BACKUP_FILES_PATH, OLD_VERSION_ISOLATION_ZONE_PATH, NULL);
    string new_path = Utils::Path::JoinPath(mCVersion.mOldInstallPath.c_str(), OLD_VERSION_ISOLATION_ZONE_PATH, NULL);
    CopyDir(old_path.c_str(), new_path.c_str()); //if not exist, do nothing
    //log data
    old_path = Utils::Path::JoinPath(TMP_BACKUP_FILES_PATH, LOG_XML_FILE_NAME, NULL);
    new_path = Utils::Path::JoinPath(DEFAULT_INSTALL_PATH, NEW_VERSION_ISOLATION_ZONE_PATH, NULL);
    CopyDir(old_path.c_str(), new_path.c_str());
    return 0;
}

int Updater::InstallNewFiles()
{
    string files[] = {"common.rpk", "tools.rpk", "ep.rpk", "xav.rpk", "xfw.rpk"};
    mSetup.SetArch(mDlParser.GetMachineType().c_str());
    if (mSetup.mInstallPath == "") {
        mSetup.mInstallPath = DEFAULT_INSTALL_PATH;
    }
    mSetup.mFunctionCode = 3;
#ifndef GUIZHOU_SUPPORT
    mSetup.mFunctionCode = FunctionsCode[mSetup.mFunctionCode];
#else   
    mSetup.mFunctionCode = 1;
#endif
    if (!Utils::Path::PathExists(mSetup.mInstallPath)) {
        Utils::Path::mkdirs(mSetup.mInstallPath);
    }
    mSetup.MakeRunDirs();
    mSetup.StopAll();
    for (int i=0; i<5; i++) {
        string rpk_file = Utils::Path::JoinPath(TMP_DOWNLOAD_FILES_PATH, files[i].c_str(), NULL);
        if (Utils::Path::FileAvailed(rpk_file)) {
            if (mSetup.UnpackPackage(rpk_file) != 0) {
                printf("Unpackage %s fail\n", rpk_file.c_str());
                return -1;
            }
        }
    }
    mSetup.ProcessQt5Librarys();
    mSetup.SetupKernelModule();
    if(mSetup.MakeExecShell() != 0){
        printf("MakeExecShell fail\n");
        return -1;
    }
    mSetup.MakeStartShell(START_KO_SHELL);
    mSetup.MakeStopShell(STOP_ALL_SHELL);
    string rjjhetc_path = Utils::Path::JoinPath(mSetup.mInstallPath.c_str(), FOLDER_RJJH_ETC, NULL);
    if(!Utils::Path::PathExists(rjjhetc_path)){
        Utils::Path::mkdirs(rjjhetc_path);
    }
    mSetup.mProductInfo.update = Utils::Date::FormatTimeStamp(time(NULL));
    if (mSetup.GenerateRegisterFile() != 0) {  //.RavCfg
        return -1;
    }
    if (mSetup.GenerateInsmodSh() != 0) {  //insmod.sh
        return -1;
    }
    printf("make auto start script\n");
    if (mSetup.GenerateExecScript() != 0) {  //ravloader.sh
        return -1;
    }
    if (mSetup.AddRootStartScript() != 0) {
        return -1;
    }
    if (mSetup.AddAutoStart(AUTOSTART_FILE_NAME) != 0) {
        return -1;
    }
    if (mSetup.AddApplicationIcon(APPLICATION_FILE_NAME) != 0) {
        return -1;
    }
    return 0;
}

int Updater::BackupConfigFile()
{
    string config;
    string path_prefix = "/";
    string backup_path = Utils::Path::JoinPath(TMP_BACKUP_FILES_PATH, RAV_CONFIG_FILE, NULL);

    if (mSetup.mMachineType == SetupApplication::MACHINE_LPSECRET) {
        config = Utils::Path::JoinPath(path_prefix.c_str(), "opt/BDFZ/RAV", FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
    }else{
        config = Utils::Path::JoinPath(path_prefix.c_str(), "etc", RAV_CONFIG_FILE, NULL);
    }
    if(Utils::Path::FileAvailed(config.c_str())) {
        if(!Utils::Path::CopyFile(config.c_str(), backup_path.c_str())){
            return -1;
        }
    }else{
        return -1;
    }
    return 0;
}

int Updater::RestoreConfigFile()
{
    string config;
    string path_prefix = "/";
    string backup_path = Utils::Path::JoinPath(TMP_BACKUP_FILES_PATH, RAV_CONFIG_FILE, NULL);

    if (mSetup.mMachineType == SetupApplication::MACHINE_LPSECRET) {
        config = Utils::Path::JoinPath(path_prefix.c_str(), "opt/BDFZ/RAV", FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
    }else{
        config = Utils::Path::JoinPath(path_prefix.c_str(), "etc", RAV_CONFIG_FILE, NULL);
    }
    if(Utils::Path::FileAvailed(backup_path.c_str())) {
        if(!Utils::Path::CopyFile(backup_path.c_str(), config.c_str())){
            return -1;
        }
    }else{
        return -1;
    }
    return 0;
}

int Updater::StartDaemons()
{
    return mSetup.StartServer();
}

int Updater::CopyDir(const char* srcDir, const char* dstDir)
{
    struct dirent *tmp = NULL;
    struct stat buf;
    char tempSrcPath[1024] = {0};
    string tempDstPath;

    DIR *dir = opendir(srcDir);
    if (NULL == dir) {
        return -1;
    }
    if(stat(srcDir, &buf) ==  0) {
        Utils::Path::CheckPath(dstDir, buf.st_mode);
    }
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type) {
            snprintf(tempSrcPath, 1023, "%s/%s", srcDir, tmp->d_name);
            stat(tempSrcPath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
        }

        bool isfile = DT_REG==tmp->d_type;
        if (!isfile && DT_UNKNOWN==tmp->d_type) {
            snprintf(tempSrcPath, 1023, "%s/%s", srcDir, tmp->d_name);
            stat(tempSrcPath, &buf);
            isfile = bool(buf.st_mode & S_IFREG);
        }

        if (isfile) {
            string srcFile = Utils::Path::JoinPath(srcDir, tmp->d_name, NULL);
            string dstFile = Utils::Path::JoinPath(dstDir, tmp->d_name, NULL);
            if (!Utils::Path::CopyFile(srcFile.c_str(), dstFile.c_str())) {
                return -1;
            }
        } else if (isdir) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }

            sprintf(tempSrcPath, "%s/%s", srcDir, tmp->d_name);
            tempDstPath = Utils::Path::JoinPath(dstDir, tmp->d_name, NULL);
            Utils::Path::CheckPath(tempDstPath, buf.st_mode);
            if (CopyDir(tempSrcPath, tempDstPath.c_str()) < 0) {
                return -1;
            }
        }
    }
    closedir(dir);
    return 0;
}

