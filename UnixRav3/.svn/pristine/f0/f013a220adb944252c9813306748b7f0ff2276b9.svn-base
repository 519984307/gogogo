#ifndef COMMONAPI_CONFIGFILE_H_
#define COMMONAPI_CONFIGFILE_H_

#include <stdio.h>
#include <string>

class ConfigFile {
 public:
    static ConfigFile* NEW(const char *config);
    static int MakeConfig(const char *config_path, const char *sn, const char *install_path);

    ConfigFile();
    ~ConfigFile();

    int SaveFile(const char *config);

 private:
    ConfigFile(const char *sn, const char *install_path);
    int Construct(const char *config);

 private:
    int LoadFile(const char *file);
    int ReadConfig(FILE *file);
    int WriteConfig(FILE *file);


 public:
    // for old version, the old version config define
//    char    sn[128];
//    char    installtime[32];
//    char    endtim[32];

    char    mSerial[128];
    char    mInstallPath[128];
    int     mInstallTime;
    int     mEndTime;
    int     mProduct;
    int     mFunctions;
};

#endif  // COMMONAPI_CONFIGFILE_H_
