#ifndef CLIENTINFOMODEL_H
#define CLIENTINFOMODEL_H

#include <string>
#include <list>

#include "json/json.h"

class ClientInfoModel
{
public:
    ClientInfoModel();

    Json::Value toJSON();
    std::string getModelType();

    bool validate();
    std::string generalGUID();

public:
    std::string   mComputername;
    std::string   mCpuarchitecture;
    std::string   mIp;
    int           mIsvm;
    std::string   mMac;
    std::string   mOs;
    std::string   mSystemdirectory;
    std::string   mTotalmemory;
    std::string   mDiskSize;
    std::string   mUsername;
    std::string   mVersion;
};

class ModuleInfoModel
{
public:
    ModuleInfoModel(std::string guid, std::string version);

    std::string getModelType();
    Json::Value toJSON();
    bool validate();

public:
    std::string guid;
    std::string version;
};

class ReportClientModel {
public:
    ReportClientModel(ClientInfoModel* client, std::list<ModuleInfoModel*> modules);

public:
    ClientInfoModel             *mClientModel;
    std::list<ModuleInfoModel*>  mModuleModel;
};

#endif // CLIENTINFOMODEL_H
