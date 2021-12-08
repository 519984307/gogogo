#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include <string>
#include <map>

using namespace std;

class AppConfig {
public:
    static AppConfig* NEW(const string& configFile);
    AppConfig();    
    ~AppConfig();

    int UpdateConfig(const string& json);
    std::string JsonString();

    int Reload();

    int StoreConfig();
    void operator=(const AppConfig& config);

    bool CheckConfig();

    void PrintConfig();
    void ChangeConfigSection(int key, const string& value, bool stored=true);
    void DeleteSection(int key);

    static void GeneralConfig(const string& config);

    static AppConfig* MakeGeneralConfig(bool configexists, const string& center, const string& version);

    bool SecretMachine();
    bool CenterLess();

public:
    enum SECTION_FLAG{
        /** app install path */
        FLAG_INSTALL_PATH    = 1000,
        /** product type, like SOC or ESM, define in Setup.h */
        PRODUCT_TYPE,
        /** sub components, like XAV, XFW, define in SUB_CHILD_PRODUCT_FLAG */
        SUB_COMPONENTS,
        /** serial number, just for single version */
        SERIAL_NUMBER,
        /** SOC eid, only valid in SOC */
        FLAG_EID,
        /** center url, valid in ESM and SOC, like https://193.168.14.15 */
        CENTER_URL,
        /** app version, like 3.0.0.12 */
        APP_VERSION,
        /** virus lib version like 30.1128.0001 */
        VLIB_VERISON,
        /** install timestamp */
        INSTALL_TIME,
        /** policy change time, when the policy change, recode the time */
        POLICY_CHANGE_TIME,
        /** sub product guid */
        SUB_PRODUCT_GUID,
        /** app update time */
        UPDATE_TIME,
        /** app guid, when center lock the GUID, the record the GUID, from the machine character */
        APP_GUID,
        /** app type, line linux:lintelx64, laarch64 so on */
        APP_TYPE,
        /** functions, like USB plngin monitor, file monitor */
        FUNCTIONS,
        /** app sguid, from cener */
        APP_SGUID,
        /** mac, check mac for sguid**/
        APP_MAC, 
        // license
        APP_PROC_GUID, 
        APP_LICENSE, 
        APP_DEADLINE, 
    };
private:
    int LoadFromConfig(FILE *conf);
    int StoreToConfig(FILE *conf);
    int CalcHash(FILE *file, unsigned char *hash);

    int Construct(const string& configFile);

    int CryptString(const char *, char *, int);

private:
    typedef struct {
        unsigned char   mHeader[4];
        int             mCount;
    }HEADER;
    union SECTION_VALUE {
        char   mStrValue[128];
        unsigned int mIntValue;
    };
    typedef struct {
        int     mSection;
        short   mType;
        short   mLength;
    }SECTION;

    enum SECTION_VALUE_TYPE {
        INT,
        STRING
    };

public:
    enum SUB_CHILD_PRODUCT_FLAG {
        EP  = 0,
        XAV = 1,
        XFW = 2
    };

public:
    string GetInstallPath();
    /** product type, define in Setup.h SoftType*/
    int    GetProductType();
    string GetAppVersion();
    string GetVlibVersion();
    int    GetSubComponents();
    string GetSerialNumber();
    string GetEID();
    string GetCenterURL();
    time_t GetInstallTime();
    time_t GetPolicyTime();
    string GetSubProductGUID();
    time_t GetUpdateTime();
    string GetAppGUID();
    string GetAppSGUID();
    string GetAppType();
    int    GetFunctions();
    string GetMac();
    string GetProductGuid();
    string GetAppLicenseFile();
    string GetAppDeadline();

    int SetInstallPath(const string& path);
    int SetProductType(int type);
    int SetAppVersion(const string& version);
    int SetVlibVersion(const string& version);
    int SetSubComponents(int sub);
    int SetSerialNumber(const string& serial);
    int SetEID(const string& eid);
    int SetCenterURL(const string& url);
    int SetInstallTime(time_t time);
    int SetPolicyTime(time_t time);
    int SetSubProductGUID(const string& guid);
    int SetUpdateTime(time_t time);
    int SetAppGUID(const string& guid);
    int SetAppSGUID(const string& guid);
    int SetAppType(const string& type);
    int SetFunctions(int type);
    int SetMac(const string & mac);
    int SetProcductGuid(const string & guid);
    int SetAppLicenseFile(const string & file);
    int SetAppDeadline(const string & time);


private:
    int GetConfigValue(enum SECTION_FLAG flag, int& value);
    int GetConfigValue(enum SECTION_FLAG flag, string& value);
    int SetConfigValue(enum SECTION_FLAG flag, int value);
    int SetConfigValue(enum SECTION_FLAG flag, const string& value);

public:
//    /** product type, define in Setup.h SoftType*/
//    int mProductType;
//    int mSubProducts;
//    time_t mInstallTime;
//    time_t mPolicyUpdate;
//    string mSerialNumber;
//    string mVersion;
//    string mVirusVersion;
//    string mInstallDir;
//    string mCenterUrl;
//    string mEID;
//    string mSystemType;


    enum FunctionTag {
        BASE_FUNCTION   = 0x0,
        X11_SUPPORT     = 0x1,
        PROC_GUARD      = 0x2,
        FILE_DEFEN      = 0x4,
        NET_FILTER      = 0x8,
        CONTEXT_MENU    = 0x10,
        PLUGIN_CHECKER  = 0x20,
        SECRET_VERSION  = X11_SUPPORT|FILE_DEFEN|NET_FILTER|CONTEXT_MENU|PLUGIN_CHECKER,
    };

    inline bool GraphScreen() { return GetFunctions()&X11_SUPPORT; }
    inline bool WatchGuard()  { return GetFunctions()&PROC_GUARD;  }
    inline bool FileDefen()   { return GetFunctions()&FILE_DEFEN;  }
    inline bool NetFilter()   { return GetFunctions()&NET_FILTER;  }
    inline bool ContextMenu() { return GetFunctions()&CONTEXT_MENU; }
    inline bool USBMonitor()  { return GetFunctions()&PLUGIN_CHECKER; }

    int InitializeFunctions(int& functions);

public:
    map<enum SECTION_FLAG, pair<enum SECTION_VALUE_TYPE, union SECTION_VALUE> > mSectionValues;
    string mConfigFile;
};


class ScanIntelligentParams {
public:
    int thread_count;
    int cpuload_precent;

public:
    ScanIntelligentParams() {
        thread_count = 1;
        cpuload_precent = 100;
    }

    int Initilize(const string& json);
    string Externalize();
    void Detail(int /*AntiVirusSetting::AntiCommon::ScanType*/ type);
};

#endif //APP_CONFIG_H_
