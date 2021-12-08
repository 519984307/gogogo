#ifndef UPDATEAPPENGINE_H
#define UPDATEAPPENGINE_H

#include "upgrade.h"
#include "libxml/parser.h"

#include <string>
#include <map>

using namespace std;


class AppConfig;
class UpdateAppEngine : public UpdateEngine
{
public:
    static UpdateAppEngine* NEW(const std::string &installdir, const std::string& configpath, int center_type);

	void SetAppConfig(AppConfig * cfg);
    void SetUpdateMethod(const std::string& subproductguid);

private:
    // from UpdateEngine
    int UpdateEntry();
    std::string GetDefaultAddress();

private:
    UpdateAppEngine() : mAppConfig(0) {}
public:
    int AppUpdate(string& newversion, string& folder);
    int UpdateFiles(const std::string & folder, const std::string& version);

    int OriginUpdateEntry(const string& folder, const string& baseurl, const string& version, std::string& newversion);
    int NormalUpdateEntry(UPDATE_SOURCE sourcetype,
                          const string& folder,
                          const string& baseurl,
                          const NavigateModel& navimodel,
                          std::string& newversion);

    int GetUpdateFiles();

    int UnzipFile(const std::string& in, std::string& out);

    int GetUpdateFileMd5(const std::string& md5file, std::map<std::string, std::string>& out);

    int ParseVersionList(const std::string& verlistfile,
                         std::map<std::string, std::string>& filemd5,
                         std::vector<UPDATALISTINFO>& strUpList,
                         std::vector<std::string>& updateless);

    std::string GetNewVersion(const std::string& verlistfile);

    int EmulateEnv(std::vector<UPDATALISTINFO>& strUpList,
                   std::vector<std::string>& unpdateless);

    int DownloadUpdateFile(const UPDATALISTINFO& info, const std::string& folder, const std::string& baseurl);

    int BackupOrigin(const std::string& origin);
    int ReplaceFile(const std::string& filepath, const std::string& file);
public:
    int GetOfficalAddress(const string& url, const string& serial, int product_type, string & real_url, string & report_url);

private:
    enum {
        UPDATE_ORIGIN_VERSION,
        UPDATE_NORMAL_VERSION,
    }mUpdateMethod;

    class UpdateItem {
    public:
        string name;
        string dest;
        string md5;
        string version;
        enum {
            FILE,
            KERNEL
        }type;
    };
    int GetUpdateVersion(const string& content, string& version, map<string, string>& components);
    int GetComponentDetail(const string& name, const string& content, vector<UpdateItem>& details);
    int UpdateComponent(UPDATE_SOURCE type, const string& name, const string& baseurl, const string& tmpfolder);
    int ParseComponent(const char *name, xmlNodePtr ptr, vector<UpdateItem>& details);
    int ParseUpdateItem(xmlNodePtr ptr,UpdateItem& item);

	// new offical update validate. 
	int  OfficalNavigateGetOverTime(const std::string & strUrl, const std::string & strSerial, std::string & deadline, int & nDays);
	bool IsCheckChangeOfDeadlineDate(const std::string & strDate);
	int  OfficalNavigateDownCert(const std::string & strUrl, const std::string & strSerial);
	int  ReportUpdateResult(const std::string & strUrl, const std::string & strSerial, const std::string & strVersion);

	AppConfig * mAppConfig;
};

#endif // UPDATEAPPENGINE_H
