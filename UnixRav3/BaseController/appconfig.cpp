#include "appconfig.h"
#include "baseconfig.h"

#include "json/json.h"
#include "crypt/md5.h"
#include "commonapi.h"
#include "Entry/commonentry.h"

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <fnmatch.h>
#include <dirent.h>
#include <sys/stat.h>

const unsigned char CONFIG_MAGIC[] = {'R', 'A', 'V', 'C'};

#ifndef SECURITY_FREE
    #define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#endif
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

AppConfig* AppConfig::NEW(const string &configFile) {
    AppConfig *self = new AppConfig;
    if (self) {
        if (self->Construct(configFile)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

AppConfig::AppConfig() {

}

AppConfig::~AppConfig() {

}

void AppConfig::operator=(const AppConfig& config) {
    mConfigFile = config.mConfigFile;
    mSectionValues = config.mSectionValues;
}

int AppConfig::Construct(const string &configFile) {
    mConfigFile = configFile;

    // load from ini file
    // return LoadFromConfig(configFile);

    Reload();

    // some version forget the function tag
    if (GetFunctions() == 0) {
        SetFunctions(X11_SUPPORT|PROC_GUARD);
    }
    return 0;
}

//int AppConfig::LoadFromConfig(const string &configPath) {
//    CINIFile configFile(configPath, true);
//    configFile.readitemstr("CLIENT", "VERSION", mVersion, "");
//    configFile.readitemstr("CLIENT", "VIRVER", mVirusVersion, "");
//    configFile.readitemstr("CLIENT", "PLATFORM", mSystemType, "");
//    configFile.readitemlong("CLIENT", "PRODUCT", mProductType, 0);
//    configFile.readitemlong("CLIENT", "FUNCTION", (int&)mFunctions, 0);


//    if (mProductType > 0) {
//        configFile.readitemstr("SERVER", "URL", mCenterUrl, "");
//        configFile.readitemstr("SERVER", "EID", mEID, "");

//        if (!mCenterUrl.empty()) {
//            if (1 == mProductType) {
//                // 1 mean ESM, define is setup.h
//                mCenterUrl += string(CENTER_ESM_URL);
//            }
//            else if (2 == mProductType) {
//                // 2 mean SOC
//                mCenterUrl += string(CENTER_SOC_URL);
//            }
//        }

//        mSystemType = "linux:" + mSystemType;
//        std::transform(mSystemType.begin(), mSystemType.end(), mSystemType.begin(), ::tolower);
//    }

//    return 0;
//}

int AppConfig::LoadFromConfig(FILE *conf) {
    char buffer[512];
    SECURITY_CHECK(fread(buffer, 1, 4, conf), 4, -1);
    SECURITY_CHECK(memcmp(buffer, CONFIG_MAGIC, 4), 0, -1);

    // read count
    int count = 0;
    SECURITY_CHECK(fread(&count, 1, 4, conf), 4, -1);
    SECTION section;
    union SECTION_VALUE value;
    for (int i=0; i<count; ++i) {
        memset(&value, 0, sizeof(value));
        SECURITY_CHECK(fread(&section.mSection, 4, 1, conf), 1, -1);
        SECURITY_CHECK(fread(&section.mType, 2, 1, conf), 1, -1);
        SECURITY_CHECK(fread(&section.mLength, 2, 1, conf), 1, -1);
        if (section.mType == INT) {
            SECURITY_CHECK(fread(&value.mIntValue, 1, section.mLength, conf), section.mLength, -1);
        }
        else {
            SECURITY_CHECK(fread(buffer, 1, section.mLength, conf), section.mLength, -1);
            buffer[section.mLength] = '\0';
            CryptString(buffer, value.mStrValue, section.mLength);
            value.mStrValue[section.mLength] = '\0';
        }
        mSectionValues.insert(make_pair((enum SECTION_FLAG)section.mSection, make_pair((enum SECTION_VALUE_TYPE)section.mType, value)));
    }
    return 0;
}

int AppConfig::GetConfigValue(enum SECTION_FLAG flag, int& value) {
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter = mSectionValues.find(flag);
    if (iter != mSectionValues.end()) {
        if (iter->second.first == INT) {
            value = iter->second.second.mIntValue;
        }
    }
    return 0;
}

int AppConfig::GetConfigValue(enum SECTION_FLAG flag, string& value) {
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter = mSectionValues.find(flag);
    if (iter != mSectionValues.end()) {
        if (iter->second.first == STRING) {
            value = iter->second.second.mStrValue;
        }
    }
    return 0;
}

int AppConfig::SetConfigValue(enum SECTION_FLAG flag, int value) {
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter = mSectionValues.find(flag);
    if (iter != mSectionValues.end()) {
        iter->second.first = INT;
        iter->second.second.mIntValue = value;
    }
    else {
        union SECTION_VALUE sv;
        sv.mIntValue = value;
        mSectionValues.insert(make_pair(flag, make_pair(INT, sv)));
    }
    return 0;
}

int AppConfig::SetConfigValue(enum SECTION_FLAG flag, const string& value) {
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter = mSectionValues.find(flag);
    if (iter != mSectionValues.end()) {
        iter->second.first = STRING;
        strcpy(iter->second.second.mStrValue, value.c_str());
    }
    else {
        union SECTION_VALUE sv;
        strcpy(sv.mStrValue, value.c_str());
        mSectionValues.insert(make_pair(flag, make_pair(STRING, sv)));
    }
    return 0;
}

string AppConfig::GetInstallPath() {
    string temp;
    GetConfigValue(FLAG_INSTALL_PATH, temp);
    return temp;
}

int AppConfig::GetProductType() {
    int temp;
    GetConfigValue(PRODUCT_TYPE, temp);
    return temp;
}
string AppConfig::GetAppVersion() {
    string temp;
    GetConfigValue(APP_VERSION, temp);
    return temp;
}
string AppConfig::GetVlibVersion() {
    string temp;
    GetConfigValue(VLIB_VERISON, temp);
    return temp;
}
int AppConfig::GetSubComponents() {
    int temp = 0;
    GetConfigValue(SUB_COMPONENTS, temp);
    return temp;
}
string AppConfig::GetSerialNumber() {
    string temp;
    GetConfigValue(SERIAL_NUMBER, temp);
    return temp;
}
string AppConfig::GetEID(){
    string temp;
    GetConfigValue(FLAG_EID, temp);
    return temp;
}
string AppConfig::GetCenterURL() {
    string temp;
    GetConfigValue(CENTER_URL, temp);
    return temp;
}
time_t AppConfig::GetInstallTime() {
    int temp = 0;
    GetConfigValue(INSTALL_TIME, temp);
    return temp;
}
time_t AppConfig::GetPolicyTime() {
    int temp = 0;
    GetConfigValue(POLICY_CHANGE_TIME, temp);
    return temp;
}
string AppConfig::GetSubProductGUID() {
    string temp;
    GetConfigValue(SUB_PRODUCT_GUID, temp);
    return temp;
}

string AppConfig::GetAppGUID() {
    string temp;
    GetConfigValue(APP_GUID, temp);
    return temp;
}

string AppConfig::GetAppSGUID() {
    string temp;
    GetConfigValue(APP_SGUID, temp);
    return temp;
}

string AppConfig::GetAppType() {
    string temp;
    GetConfigValue(APP_TYPE, temp);
    return temp;
}

time_t AppConfig::GetUpdateTime() {
    int temp = 0;
    GetConfigValue(UPDATE_TIME, temp);
    if (temp == 0) {
        GetConfigValue(INSTALL_TIME, temp);
    }
    if (temp == 0) {
        temp = Utils::Date::GetTimeStamp();
    }
    return temp;
}

int AppConfig::GetFunctions() {
    int temp = 0;
    GetConfigValue(FUNCTIONS, temp);
    return temp;
}

string AppConfig::GetMac() {
	string temp;
	GetConfigValue(APP_MAC, temp);
	return temp;
}

string AppConfig::GetProductGuid() {
	string temp;
	GetConfigValue(APP_PROC_GUID, temp);
	return temp;
}

string AppConfig::GetAppLicenseFile() {
	string temp;
	GetConfigValue(APP_LICENSE, temp);
	return temp;
}

string AppConfig::GetAppDeadline() {
	string temp;
	GetConfigValue(APP_DEADLINE, temp);
	return temp;
}

int AppConfig::SetInstallPath(const string& path) {
    return SetConfigValue(FLAG_INSTALL_PATH, path);
}
int AppConfig::SetProductType(int type) {
    return SetConfigValue(PRODUCT_TYPE, type);
}
int AppConfig::SetAppVersion(const string& version) {
    return SetConfigValue(APP_VERSION, version);
}
int AppConfig::SetVlibVersion(const string& version) {
    return SetConfigValue(VLIB_VERISON, version);
}
int AppConfig::SetSubComponents(int sub) {
    return SetConfigValue(SUB_COMPONENTS, sub);
}
int AppConfig::SetSerialNumber(const string& serial) {
    return SetConfigValue(SERIAL_NUMBER, serial);
}
int AppConfig::SetEID(const string& eid) {
    return SetConfigValue(FLAG_EID, eid);
}
int AppConfig::SetCenterURL(const string& url) {
    return SetConfigValue(CENTER_URL, url);
}
int AppConfig::SetInstallTime(time_t time) {
    return SetConfigValue(INSTALL_TIME, time);
}
int AppConfig::SetPolicyTime(time_t time) {
    return SetConfigValue(POLICY_CHANGE_TIME, time);
}
int AppConfig::SetSubProductGUID(const string& guid) {
    return SetConfigValue(SUB_PRODUCT_GUID, guid);
}
int AppConfig::SetUpdateTime(time_t time) {
    return SetConfigValue(UPDATE_TIME, time);
}

int AppConfig::SetAppGUID(const string& guid) {
    return SetConfigValue(APP_GUID, guid);
}

int AppConfig::SetAppSGUID(const string& guid) {
    return SetConfigValue(APP_SGUID, guid);
}


int AppConfig::SetAppType(const string& guid) {
    return SetConfigValue(APP_TYPE, guid);
}

int AppConfig::SetFunctions(int functions) {
    return SetConfigValue(FUNCTIONS, functions);
}

int AppConfig::SetMac(const string & mac) {
	return SetConfigValue(APP_MAC, mac);
}

int AppConfig::SetProcductGuid(const string & guid) {
	return SetConfigValue(APP_PROC_GUID, guid);
}

int AppConfig::SetAppLicenseFile(const string & file) {
	std::string old_file;
	GetConfigValue(APP_LICENSE, old_file);
	if(!old_file.empty())  unlink(old_file.c_str());

	return SetConfigValue(APP_LICENSE, file);
}

int AppConfig::SetAppDeadline(const string & tm) {
	return SetConfigValue(APP_DEADLINE, tm);
}

int AppConfig::UpdateConfig(const string &json_data) {
    Json::Reader reader;
    Json::Value json;
    if (reader.parse(json_data, json)) {
        // update the section
        SetProductType(json["type"].asInt());
        SetSubComponents(json["components"].asInt());
        SetAppVersion(json["version"].asString());
        SetVlibVersion(json["vlibver"].asString());
        SetInstallPath(json["path"].asString());
        SetCenterURL(json["url"].asString());
        SetEID(json["eid"].asString());
        SetSubProductGUID(json["subguid"].asString());
        SetAppType(json["subapp"].asString());
        SetInstallTime(json["instime"].asUInt());
        SetUpdateTime(json["update"].asUInt());
        SetFunctions(json["function"].asInt());
        SetAppGUID(json["guid"].asString());
        SetSerialNumber(json["serial"].asString());
        SetAppSGUID(json["sguid"].asString());
        mConfigFile = json["file"].asString();

        return true;
    }
    return -1;
}

bool AppConfig::CheckConfig() {
    return !GetInstallPath().empty()
            && !GetAppVersion().empty()
            && !GetCenterURL().empty()
            && !GetSubProductGUID().empty();
}

std::string AppConfig::JsonString() {
    Json::Value json;
    json["type"] = GetProductType();
    json["components"] = GetSubComponents();
    json["version"] = GetAppVersion();
    json["vlibver"] = GetVlibVersion();
    json["path"] = GetInstallPath();
    json["url"] = GetCenterURL();
    json["eid"] = GetEID();
    json["subguid"] = GetSubProductGUID();
    json["subapp"] = GetAppType();
    json["instime"] = (unsigned int)GetInstallTime();
    json["update"] = (unsigned int)GetUpdateTime();
    json["function"] = GetFunctions();
    json["guid"] = GetAppGUID();
    json["serial"] = GetSerialNumber();
    json["sguid"] = GetAppSGUID();
    json["file"] = mConfigFile;

    Json::FastWriter writer;
    return writer.write(json);
}

int AppConfig::StoreConfig() {
    FILE *file = fopen(mConfigFile.c_str(), "wb+");
    ASSERT_ZERO(file, -1);

    int err = StoreToConfig(file);
    if (err) {
        fclose(file);
        return -1;
    }

    unsigned char hash[16] = {0};
    fseek(file, 0, SEEK_SET);
    if (CalcHash(file, hash)) {
        fclose(file);
        return -1;
    }

    fwrite(hash, 16, 1, file);
    fclose(file);
    return 0;
}

int AppConfig::CalcHash(FILE *file, unsigned char *hash) {
    MD5 md5;
    fseek(file, 0, SEEK_SET);
    unsigned char buffer[256];
    while (1) {
        int readed = fread(buffer, 1, 256, file);
        if (readed <= 0) {
            break;
        }

        md5.update(buffer, readed);
    }
    md5.finalize();
    memcpy(hash, md5.getBuffer(), 16);
    return 0;
}

int AppConfig::CryptString(const char *source, char *dest, int length) {
    for (int i=0; i<length; i++) {
        dest[i] = ~source[i];
    }
    return length;
}

int AppConfig::Reload() {
    mSectionValues.clear();
    FILE * file = fopen(mConfigFile.c_str(), "rb");
    ASSERT_ZERO(file, -1);
    int err = LoadFromConfig(file);
    if (err) {
        SECURITY_CLOSE(file);
        return -1;
    }

//    if (0 == GetUpdateTime()) {
//        SetUpdateTime(Utils::Date::GetTimeStamp());
//        StoreConfig();
//    }
    return 0;
}

int AppConfig::StoreToConfig(FILE *file) {
    SECURITY_CHECK(fwrite(CONFIG_MAGIC, 1, 4, file), 4, -1);
    int count = mSectionValues.size();
    SECURITY_CHECK(fwrite(&count, 4, 1, file), 1, -1);

    char temp[128];

    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter;
    for (iter=mSectionValues.begin(); iter!=mSectionValues.end(); ++iter) {
        int type = iter->first;
        SECURITY_CHECK(fwrite(&type, 4, 1, file), 1, -1);
        fwrite(&iter->second.first, 2, 1, file);
        if (iter->second.first == INT) {
            type = 4;
            SECURITY_CHECK(fwrite(&type, 2, 1, file), 1, -1);
            SECURITY_CHECK(fwrite(&iter->second.second.mIntValue, type, 1, file), 1, -1);
        }
        else if (iter->second.first == STRING) {
            type = strlen(iter->second.second.mStrValue);
            SECURITY_CHECK(fwrite(&type, 2, 1, file), 1, -1);
            memset(temp, 0, 128);
            CryptString(iter->second.second.mStrValue, temp, type);
            SECURITY_CHECK(fwrite(temp, 1, type, file), type, -1);
        }

    }
    fflush(file);

    return 0;
}

void AppConfig::ChangeConfigSection(int key, const string& value, bool stored) {
    switch ((enum SECTION_FLAG)key) {
    case FLAG_INSTALL_PATH:
    case SERIAL_NUMBER:
    case FLAG_EID:
    case CENTER_URL:
    case APP_VERSION:
    case VLIB_VERISON:
    case SUB_PRODUCT_GUID:
    case APP_TYPE:
    case APP_MAC:
    case APP_PROC_GUID:
    case APP_LICENSE:
    case APP_DEADLINE:
    case APP_GUID: {
        SetConfigValue((enum SECTION_FLAG)key, value);
        break;
    }
    case PRODUCT_TYPE:
    case SUB_COMPONENTS:
    case INSTALL_TIME:
    case POLICY_CHANGE_TIME:
    case UPDATE_TIME:
    case FUNCTIONS:
        SetConfigValue((enum SECTION_FLAG)key, atoi(value.c_str()));
        break;
    }
    if (stored) {
        StoreConfig();
    }
}

void AppConfig::DeleteSection(int key) {
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter = mSectionValues.find((enum SECTION_FLAG)key);
    if (iter != mSectionValues.end()) {
        mSectionValues.erase(iter);
    }
    StoreConfig();
}

void AppConfig::PrintConfig() {
    printf("config content:\n");
    printf("---------------------------------------------------------------------------------------\n");
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> >::iterator iter;
    for (iter=mSectionValues.begin(); iter!=mSectionValues.end(); ++iter) {
        printf("| %d ", iter->first);
        switch (iter->first) {
        case FLAG_INSTALL_PATH: {
            printf("|\tinstall path: %s\n", iter->second.second.mStrValue);
            break;
        }
        case PRODUCT_TYPE: {
            switch (iter->second.second.mIntValue) {
            case 0: {
                printf("|\tproduct type: SINGLE\n");
                break;
            }
            case 1: {
                printf("|\tproduct type: ESM\n");
                break;
            }
            case 2: {
                printf("|\tproduct type: SOC\n");
                break;
            }
            default: {
                printf("|\tproduct type: ERROR\n");
                break;
            }
            }
            break;
        }
        case SUB_COMPONENTS: {
            printf("|\tcomponents: ");
            if (iter->second.second.mIntValue & XAV) {
                printf("XAV\t");
            }
            if (iter->second.second.mIntValue & XFW) {
                printf("XFW\t");
            }
            printf("\n");
            break;
        }
        case SERIAL_NUMBER: {
            printf("|\tserial number: %s\n", iter->second.second.mStrValue);
            break;
        }
        case FLAG_EID: {
            printf("|\teid: %s\n", iter->second.second.mStrValue);
            break;
        }
        case CENTER_URL: {
            printf("|\tcenter url: %s\n", iter->second.second.mStrValue);
            break;
        }
        case APP_VERSION: {
            printf("|\tapp version: %s\n", iter->second.second.mStrValue);
            break;
        }
        case VLIB_VERISON: {
            printf("|\tvirus version: %s\n", iter->second.second.mStrValue);
            break;
        }
        case INSTALL_TIME: {
            printf("|\tinstall time: %ld\n", iter->second.second.mIntValue);
            break;
        }
        case POLICY_CHANGE_TIME: {
            printf("|\tpolicy time: %ld\n", iter->second.second.mIntValue);
            break;
        }
        case SUB_PRODUCT_GUID: {
            printf("|\tsub product guid: %s\n", iter->second.second.mStrValue);
            break;
        }
        case UPDATE_TIME: {
            printf("|\tupdate time: %ld\n", iter->second.second.mIntValue);
            break;
        }
        case APP_GUID: {
            printf("|\tapp guid: %s\n", iter->second.second.mStrValue);
            break;
        }
        case APP_TYPE: {
            printf("|\tsub product type: %s\n", iter->second.second.mStrValue);
            break;
        }
        case FUNCTIONS: {
            int value = iter->second.second.mIntValue;
            printf("|\tfunctions:");
            if (value&X11_SUPPORT) printf("graph |");
            if (value&PROC_GUARD)  printf("watch dog |");
            if (value&FILE_DEFEN) printf("file defen |");
            if (value&NET_FILTER) printf("net filter |");
            if (value&CONTEXT_MENU) printf("context menu |");
            if (value&PLUGIN_CHECKER) printf("USB monitor |");
            printf("\n");
            break;
        }
        }
    }
    printf("---------------------------------------------------------------------------------------\n");
}

void AppConfig::GeneralConfig(const string& config) {
    FILE *file = fopen(config.c_str(), "wb+");
    if (file) {
        fwrite(CONFIG_MAGIC, 1, 4, file);
        int count = 0;
        fwrite(&count, 4, 1, file);
        fclose(file);
    }
}

AppConfig* AppConfig::MakeGeneralConfig(bool configexists, const string &center, const string& version) {
    if (!configexists) {
        FILE *file = fopen("/etc/.RavCfg", "wb+");
        if (file) {
            fclose(file);
        }
        else {
            return NULL;
        }
    }

    AppConfig *config = AppConfig::NEW("/etc/.RavCfg");
    if (config) {
        config->SetInstallPath("/opt/BDFZ/RAV");
        config->SetProductType(2);
        config->SetSubComponents(3);
        config->SetEID("e0001");
        config->SetCenterURL(center);
        config->SetAppVersion(version);
        config->SetVlibVersion("31.0508.0002");
        config->SetInstallTime(Utils::Date::GetTimeStamp());
        config->SetSubProductGUID("90DDA51B-33AB-4d94-8F04-0B980AD75CDE");
        config->SetAppType("linux:lcserver");
        config->SetUpdateTime(Utils::Date::GetTimeStamp());
        config->SetFunctions(3);

        config->StoreConfig();
    }
    return config;
}

map<string, bool> g_so_state;
const char *FUNC_DEST[] ={
    "libQt5Widgets.so*",
    "libudev.so*",
    "libcaja-extension.so*",
    "libnemo-extension.so*",
};

static int check_file(const char *filename, const char *file) {
    for (int i=0; i<sizeof(FUNC_DEST)/sizeof(char*); ++i) {
        if (0 == fnmatch(FUNC_DEST[i], file, FNM_NOESCAPE)) {
            g_so_state[file] = true;
            break;
        }
    }
    return 0;
}

int AppConfig::InitializeFunctions(int& functions) {
    g_so_state.clear();
    const char *search_dir[] = {
        "/usr",
        "/lib",
        "/lib64"
    };
    if (GetInstallPath().empty()) {
        return -1;
    }

    for (int i=0; i<sizeof(search_dir)/sizeof(char*); ++i) {
        Utils::Path::TravelDictionary(search_dir[i], check_file);
    }

    map<string,bool>::iterator iter;
    for (iter=g_so_state.begin(); iter!=g_so_state.end(); ++iter) {
        if (iter->first.find("libQt5Widgets.so") != string::npos) {
            functions |= X11_SUPPORT;
        }
        else if (iter->first.find("libudev.so") != string::npos) {
            functions |= PLUGIN_CHECKER;
        }
        if (iter->first.find("libcaja-extension.so") != string::npos) {
            functions |= CONTEXT_MENU;
        }
        if (iter->first.find("libnemo-extension.so") != string::npos) {
            functions |= CONTEXT_MENU;
        }
    }

    string kofile = string(FILE_DEFEN_MODULE) + "_" + Utils::OS::GetKernelVersionString() + ".ko";
    if (Utils::Path::FileAvailed(
                Utils::Path::JoinPath(GetInstallPath().c_str(), PLUGINS_PATH, kofile.c_str(), NULL))) {
        functions |= FILE_DEFEN;
    }
    kofile = string(NET_FILTER_MODULE) + "_" + Utils::OS::GetKernelVersionString() + ".ko";
    if (Utils::Path::FileAvailed(
                Utils::Path::JoinPath(GetInstallPath().c_str(), PLUGINS_PATH, kofile.c_str(), NULL))) {
        functions |= NET_FILTER;
    }
    return 0;
}

bool AppConfig::SecretMachine() {
    return GetSubProductGUID() == SECRET_MACHINE_ID;
}

bool AppConfig::CenterLess() {
    return GetProductType() == 0;
}
//////////////////////////////////
int ScanIntelligentParams::Initilize(const string& jsonstr) {
    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(jsonstr, json)) {
        return -1;
    }

    thread_count = json["thread"].asInt();
    cpuload_precent = json["cpu"].asInt();
    return 0;
}

string ScanIntelligentParams::Externalize() {
    Json::Value json;
    json["thread"] = Json::Value(thread_count);
    json["cpu"] = Json::Value(cpuload_precent);

    Json::FastWriter writer;
    return writer.write(json);
}

void ScanIntelligentParams::Detail(int /*AntiVirusSetting::AntiCommon::ScanType*/ type) {
    int cpus = Utils::OS::GetCPUKernelCount();
    switch(type) {
    case AntiVirusSetting::AntiCommon::OFFICE_MODE: {
        thread_count = 1;
        cpuload_precent = 20;
        break;
    }
    case AntiVirusSetting::AntiCommon::AUTO_MODE: {
        thread_count = cpus/2;
        cpuload_precent = 100;
        break;
    }
    case AntiVirusSetting::AntiCommon::HIGH_SPEED_MODE: {
        thread_count = cpus;
        cpuload_precent = 200;
        break;
    }
    default:
        thread_count = 1;
        break;
    }
    if (thread_count == 0) {
        thread_count = 1;
    }
    else if (thread_count > 4) {
        thread_count = 4;
    }
}
