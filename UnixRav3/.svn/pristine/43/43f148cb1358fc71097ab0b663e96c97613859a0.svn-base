#include "dlparser.h"
#include "httpdownloader.h"
#include "commonapi.h"
#include "ravconsolesetup.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace Utils;

DlParser gDlParser;
HTTPDownloader gDownloader;
RavConsoleSetup gSetup;

int main(int args, char *argv[]) 
{
    if (args != 2) {
        cout << "Invalid parameter!" << endl;
        cout << "Using:" << argv[0] << "  json_file" << endl;
        return -1; 
    }
    if (gDlParser.GetParamsFromJson(argv[1]) != 0) {
        cout << "Parse json file fail." << endl;
        return -1;
    }
    string dl_file = Path::JoinPath(gDlParser.mDlUrl.c_str(), DL_WEB_PATH, DL_WEB_XML, NULL);
    string xml_data = gDownloader.Download(dl_file);
    string version = gDlParser.GetVersionFromXml(xml_data);
    dl_file = Path::JoinPath(gDlParser.mDlUrl.c_str(), DL_WEB_PATH, gDlParser.GetMachineType().c_str(), "rec_", NULL);
    dl_file += version;
    dl_file += ".xml";
    //printf("filexml=%s\n", dl_file.c_str());
    xml_data = gDownloader.Download(dl_file);
    //printf("xml:%s\n", xml_data.c_str());
    if (gDlParser.GetRpkFilesInfoFromXml(xml_data) != 0) {
        return -1;
    }
    string dl_path = DL_DOWNLOADER_TMP_FOLDER;
    if (Path::PathExists(dl_path)) {
        Utils::Path::DeleteFolder(dl_path.c_str());
    }
    Path::mkdirs(dl_path);
    gSetup.mInstallPath = gDlParser.mInstallPath;
    if (!Path::PathExists(gSetup.mInstallPath)) {
        Path::mkdirs(gSetup.mInstallPath);
    }
    int fsize = gDlParser.mDlFiles.size();
    int unpacked = 0;
    for (int i=0; i<fsize; i++) {
        RpkFile *rf = gDlParser.mDlFiles[i];
        string remote_file = Path::JoinPath(gDlParser.mDlUrl.c_str(), DL_WEB_PATH, gDlParser.GetMachineType().c_str(), "rpk",  rf->name.c_str(), rf->name.c_str(), NULL);
        remote_file = remote_file + "_" + version + ".rpk";
        string local_file = Path::JoinPath(DL_DOWNLOADER_TMP_FOLDER, rf->name.c_str(), NULL) + "_" + version + ".rpk";
        if (gDownloader.DownloadFile(remote_file.c_str(), local_file.c_str()) != 0) {
            cout << "Download file fail, " << remote_file <<  " --->> " << local_file << endl;
            continue;
        }
        if (rf->size != OS::GetFileSize(local_file) || rf->md5 != Hash::GetFileMD5(local_file)) {
            cout << "Check file size and md5 fail, " << local_file << endl;
            continue;
        }
        //printf("%s --> %s\n", remote_file.c_str(), local_file.c_str());
        if (gSetup.UnpackPackage(local_file) != 0) {
            cout << "Unpackage " << local_file << " fail!" << endl;
            return -1;
        }
        unpacked++;
    }
    if (unpacked) {


    }
    return 0;
}
