#include "clientinfomodel.h"


#define MODEL_GUID_TAG        "guid"
#define MODEL_VERSION_TAG     "version"

ClientInfoModel::ClientInfoModel():mIsvm(0) {

}

Json::Value ClientInfoModel::toJSON() {
    Json::Value json;
    json["computername"] = Json::Value(mComputername);
    json["cpuarchitecture"] = Json::Value(mCpuarchitecture);
    json["ip"] = Json::Value(mIp);
    json["isvm"] = Json::Value(mIsvm);
    json["mac"] = Json::Value(mMac);
    json["os"] = Json::Value(mOs);
    json["systemdirectory"] = Json::Value(mSystemdirectory);
    json["totalmemory"] = Json::Value(mTotalmemory);
    json["username"] = Json::Value(mUsername);
    json["version"] = Json::Value(mVersion);
    json["totalphys"] = Json::Value(mDiskSize);
    return json;
}

std::string ClientInfoModel::getModelType() {
    return "computerinfo";
}

bool ClientInfoModel::validate() {
    return true;
}

std::string ClientInfoModel::generalGUID() {
    return "";
}


ModuleInfoModel::ModuleInfoModel(std::string guid, std::string version) {
    this->guid = guid;
    this->version = version;
}

Json::Value ModuleInfoModel::toJSON() {
    Json::Value value;
    value[MODEL_GUID_TAG] = Json::Value(guid);
    value[MODEL_VERSION_TAG] = Json::Value(version);

    return value;
}

bool ModuleInfoModel::validate() {
    return true;
}


std::string ModuleInfoModel::getModelType() {
    return "moduleinfo";
}

ReportClientModel::ReportClientModel(ClientInfoModel* client, std::list<ModuleInfoModel*> modules) {
    mClientModel = client;
    mModuleModel.merge(modules);
}
