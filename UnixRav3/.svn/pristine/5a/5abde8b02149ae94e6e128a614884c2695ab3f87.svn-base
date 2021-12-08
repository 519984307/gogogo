#include "configfile.h"

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SECURITY_CHECK(x, y, z) if(x != y) return z;

ConfigFile::ConfigFile() {
    memset(mSerial, 0, 128);
    memset(mInstallPath, 0, 128);
    mInstallTime = 0;
    mEndTime = 0;
    mFunctions = 31;
}

ConfigFile::ConfigFile(const char *sn, const char *install_path) {
    memset(mSerial, 0, 128);
    memset(mInstallPath, 0, 128);

    strcpy(mSerial, sn);
    strcpy(mInstallPath, install_path);

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    mInstallTime = tv.tv_sec;
    mEndTime = 0;
    mFunctions = 31;
}

ConfigFile::~ConfigFile() {

}

ConfigFile* ConfigFile::NEW(const char *config) {
    ConfigFile *self = new ConfigFile;
    if (self) {
        if (0 != self->Construct(config)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int ConfigFile::Construct(const char *config) {
    return LoadFile(config);
}

int ConfigFile::LoadFile(const char *config) {
    FILE *file = fopen(config, "rb");
    if (NULL != file) {
        int err = ReadConfig(file);
        fclose(file);
        return err;
    }
    return -1;
}

int ConfigFile::SaveFile(const char *config) {
    FILE *file = fopen(config, "wb");
    if (NULL != file) {
        WriteConfig(file);
        fclose(file);
    }
    return 0;
}

int ConfigFile::ReadConfig(FILE *file) {
    SECURITY_CHECK(fread(mSerial, 1, 128, file), 128, -1);
    SECURITY_CHECK(fread(mInstallPath, 1, 128, file), 128, -1);
    SECURITY_CHECK(fread(&mInstallTime, 4, 1, file), 1, -1);
    SECURITY_CHECK(fread(&mEndTime, 4, 1, file), 1, -1);
    SECURITY_CHECK(fread(&mFunctions, 4, 1, file), 1, -1);
    return 0;
}

int ConfigFile::WriteConfig(FILE *file) {
    SECURITY_CHECK(fwrite(mSerial, 1, 128, file), 128, -1);
    SECURITY_CHECK(fwrite(mInstallPath, 1, 128, file), 128, -1);
    SECURITY_CHECK(fwrite(&mInstallTime, 4, 1, file), 1, -1);
    SECURITY_CHECK(fwrite(&mEndTime, 4, 1, file), 1, -1);
    SECURITY_CHECK(fwrite(&mFunctions, 4, 1, file), 1, -1);
    return 0;
}

int ConfigFile::MakeConfig(const char *config_path, const char *sn, const char *install_path) {
    FILE *file = fopen(config_path, "wb");
    if (NULL != file) {
        ConfigFile config(sn, install_path);
        config.WriteConfig(file);
        fclose(file);
        return 0;
    }
    return -1;
}
