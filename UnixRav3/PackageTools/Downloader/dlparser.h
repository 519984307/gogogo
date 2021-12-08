#ifndef __DLPARSER_H__
#define __DLPARSER_H__

#include "rpkfile.h"
#include <string>
#include <vector>

using namespace std;

//for server
#define DL_WEB_PATH "ruc/esm/linux"
#define DL_WEB_XML  "RSNetVerForUnix.xml"
//for local
#define DL_DOWNLOADER_TMP_FOLDER "/tmp/dltmp"

class DlParser
{
public:
    DlParser();
    ~DlParser();
    
    void UnInit();

    int GetParamsFromJson(const char *file);
    string GetVersionFromXml(string &data);
    string GetVDBVersionFromXml(string &data);
    int GetRpkFilesInfoFromXml(string &data, bool using_json_file = true);

    string GetMachineType();
    
public:
    string mDlUrl;
    string mDlRpk;
    string mInstallPath;
    string mInstallFinish;
    string mUpdateTime;

    vector<RpkFile*> mDlFiles;

};

#endif
