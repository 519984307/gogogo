#include "updateappengine.h"
#include "zip/unixzip.h"
#include "commonapi.h"
#include "updateconfig.h"
#include "baseconfig.h"
#include "updateobserver.h"
#include "updateconfig.h"
#include "net/httprequest.h"
#include "libxml/parser.h"
#include "appconfig.h"
#include "communicatebundle.h"
#include "service.h"

#include <fstream>
#include <vector>
#include <string>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

using namespace std;

#define Assert                      assert
#define APP_UPDATE_NAVIGATE_FILE    "RSNetVerForUnix.xml"
#define APP_UPDATE_NAVIGATE_URL     "/ruc/esm/linux/RSNetVerForUnix.xml"


/** define in libravcommon.so */
extern "C" int decompress_7z(const char *zip, const char *outfolder);

UpdateAppEngine* UpdateAppEngine::NEW(const std::string &installdir, const std::string& configpath, int center_type) {
    UpdateAppEngine *self = new UpdateAppEngine;
    if (self) {
        if (self->Contruct(installdir, configpath)) {
            delete self;
            self = NULL;
        }
        self->mCenterType = center_type;
    }

    srand((unsigned int )time(NULL));
    return self;
}

void UpdateAppEngine::SetAppConfig(AppConfig * cfg) {
	mAppConfig = cfg;
}

void UpdateAppEngine::SetUpdateMethod(const string& subguid) {
    if (subguid == COMMON_MACHINE_ID || subguid == CHINA_MECHINE_ID) {
        mUpdateMethod = UPDATE_NORMAL_VERSION;
    }
    else {
        mUpdateMethod = UPDATE_ORIGIN_VERSION;
    }
}

int UpdateAppEngine::UnzipFile(const std::string& in, std::string& out) {
#if 0
    UnZip *unzip = new UnZip;

    char strTempPath[512];
    char strTempFile[512];
    int  nResult;

    char *temp = (char*)in.c_str();
    char *szTmpBuf = strrchr(temp, '/');
    if(!szTmpBuf) {
        return -1;
    }

    nResult = szTmpBuf - temp;
    strncpy(strTempPath, temp, nResult);
    strTempPath[nResult] = '\0';
    strcat(strTempPath,"/");

    nResult = unzip->FindFirst(temp, strTempPath, strTempFile);
    if(nResult==0) {
        delete unzip;
        return -1;
    }

    out = strTempPath;

    delete unzip;
    unlink(in.c_str());
#endif
    char *temp = (char*)in.c_str();
    char *szTmpBuf = strrchr(temp, '.');
    if(!szTmpBuf) {
        return -1;
    }

    char strTempPath[512];

    int nResult = szTmpBuf - temp;
    strncpy(strTempPath, temp, nResult);
    strTempPath[nResult] = '\0';

    CUNIXZIP unzip;
    if (0 == unzip.gz_init(in.c_str(), strTempPath, true, 1)) {
        if (0 == unzip.gz_uncompress()) {
            out = strTempPath;
            return 0;
        }
    }

    return -1;
}

int UpdateAppEngine::GetUpdateFileMd5(const std::string& md5file, std::map<std::string, std::string>& out) {
    std::string line;
    std::ifstream stream(md5file.c_str());
    if (!stream) {
        return -1;
    }

    while(std::getline(stream, line)) {
        std::string::size_type begin = line.find_first_of("=");
        if (begin == std::string::npos) {
            break;
        }

        std::string filename = line.substr(0, begin);
        ++begin;
        std::string hash = line.substr(begin, line.length()-begin);

        out[filename] = hash;
    }
    return 0;
}

std::string UpdateAppEngine::GetNewVersion(const std::string& verlistfile) {
    FILE *file;
    if ((file = fopen(verlistfile.c_str(), "rb")) == NULL) {
        return "";
    }

    UPLISTVERSION   veritem;
    memset(&veritem, 0, sizeof(UPLISTVERSION));

    if (1 == fread(&veritem, sizeof(UPLISTVERSION), 1, file)) {

    }
    fclose(file);
    if (veritem.filenum > 1000) {
        return "";
    }
    return veritem.version;
}

int UpdateAppEngine::ParseVersionList(const std::string& verlistfile,
                                   std::map<std::string, std::string>& filemd5,
                                   std::vector<UPDATALISTINFO>& strUpList,
                                   std::vector<std::string>& updateless) {
    FILE *fpList;
    if ((fpList = fopen(verlistfile.c_str(), "rb")) == NULL) {
        return -1;
    }

    int nTot=0;

    UPLISTVERSION   veritem;
    memset(&veritem, 0, sizeof(UPLISTVERSION));

    while (fread(&veritem, sizeof(UPLISTVERSION), 1, fpList) == 1) {
        UPFILEVERSION * pFileVersion = new UPFILEVERSION[veritem.filenum];

        if (fread(pFileVersion, veritem.filenum*sizeof(UPFILEVERSION), 1, fpList) != 1) {
            fclose(fpList);
            delete [] pFileVersion;
            return -1;
        }

        for(int i=0; i<veritem.filenum; i++) {
            UPDATALISTINFO fileinfo;

            fileinfo.filelist = pFileVersion[i];
            fileinfo.filetype = veritem.vername;

            std::string filename = Utils::Path::JoinPath(mInstallDir.c_str(), fileinfo.filelist.filename, NULL);
            if(Utils::Path::FileAvailed(filename)) {
                std::map<std::string, std::string>::iterator iter = filemd5.find(fileinfo.filelist.filename);
                if(iter != filemd5.end()) {
                    std::string hash = Utils::Hash::GetFileMD5(filename);

                    LOG_GLTX("update %s hash is %s", fileinfo.filelist.filename, iter->second.c_str());
                    LOG_GLTX("file %s hash is %s", filename.c_str(), hash.c_str());

                    if (hash != iter->second) {
                        nTot++;
                        strUpList.push_back(fileinfo);
                    }
                    else {
                        //
                        updateless.push_back(fileinfo.filelist.filename);
                    }
                }
            }
        }

        delete [] pFileVersion;
    }

    fclose(fpList);
    return nTot;
}

int UpdateAppEngine::EmulateEnv(std::vector<UPDATALISTINFO>& strUpList, std::vector<std::string>& unpdateless) {
    return 0;
}

int UpdateAppEngine::BackupOrigin(const std::string &origin) {
    return 0;
}

int UpdateAppEngine::ReplaceFile(const std::string &filepath, const std::string &file) {
    return 0;
}

int UpdateAppEngine::UpdateEntry() {
    LOG_GLTX("<update app engine>2");

    string newversion, folder;
    int err = AppUpdate(newversion, folder);
    //App Update succeed
    if (err == 0) {
        err = UpdateFiles(folder, newversion);
        if (0 == err) {
            if (mObserver) {
                LOG_GLTX("app engine update succeed"); 
                mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                         UpdateObserver::UPDATE_DOWNLOAD_SUCCESS,
                                         &mCurrentVersion);
            }
        }
    }

    if (err < 0) {
        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                     UpdateObserver::UPDATE_ERROR,
                                     &err);
        }
    }
    else if (err > 0) {
        LOG_GLTX("app engine is already the newest %d", err); 
        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                     UpdateObserver::UPDATE_NEWEST,
                                     &mCurrentVersion);
        }
    }

    return 0;
}

int UpdateAppEngine::GetOfficalAddress(const string &url, const string& serial, int product_type, string & real_url, string & report_url) {
    string header, body, conn;
    int err = http_get(url, "", header, body, conn);
    if (err) {
        LOG_GLTX("get the offical navigate error -->%d,%s", err, url.c_str());
        return -31;
    }

    NavigateModel model;
    if (ParseNavigate(body, model)) {
        LOG_GLTX("parse navigate error -->%s", body.c_str());
        return -32;
    }

	// 流程
	// 1 Validate    (ALL)
	// 2 GetOverTime (新单机1)
	// 3 DownCert    (新单机1)
	// 4 Finish      (ALL)
	#if 0
	LOG_GLTX("navigate -->%s", url.c_str());
	LOG_GLTX("---------------------------------------\n%s\n%s\n%s\n%s", 
		model.mOvertime.c_str(), model.mValidate.c_str(), model.mDownload.c_str(), model.mFinish.c_str()
		);
	#endif

	// 这个值来自网络版, 具体原因不明, 复用
	const int UNIX_NET_PRODUCT_ID = 0x20700100;
	string param, encrypt, source_url, redirect_url, idc;

	idc        = Utils::String::itoa(rand());
	param      = string("role=rua&id=&sn=") + serial + "&time_setup=0&time_overtime=0&type=" + Utils::String::itoa(UNIX_NET_PRODUCT_ID) + "&validate=" + idc + "&ver=" + mCurrentVersion + "&guid=" + mAppConfig->GetAppGUID();
	encrypt    = Utils::Hash::RSEntrypt(param);
	source_url = model.mValidate + "?info=" + encrypt + "&version=" + mCurrentVersion;
	report_url = model.mFinish;

	http_redirect(source_url, redirect_url);
	//LOG_GLTX("http_redirect -->%s,%s", source_url.c_str(), redirect_url.c_str());

	// 处理失败的情况 (-41, -42)
	if (redirect_url.empty()) return -33;
	if (product_type == -1)
 	{
		size_t pos = redirect_url.find("?Info=");
		if (string::npos != pos)
		{
			real_url = redirect_url.substr(0, pos-1);
			return 0;
		}
	}

	if (string::npos != redirect_url.find(".asp")) 
	{
		const struct ErrorRedirect_t { const char * desc; int val; } 
		g_rerr[] = 
		{
			{ "overTime.asp", -41 }, { "notregister.asp", -42 }, { "overDown.asp", -43 }, { "deadline.asp", -44 }, { "servicepause.asp", -45 }, { "notmatch.asp",    -46 }, { "error.asp",    -47}
		};
		for(int i = 0; i < (int)(sizeof(g_rerr)/sizeof(g_rerr[0])); i++)
		{
			size_t pos = redirect_url.find(g_rerr[i].desc);
			if(pos != string::npos) return g_rerr[i].val;
		}
		return -34;
	}

	if(product_type == 0)
	{
		int  nRet,  nDays = 0;
		std::string deadline;

		// 出错不管, Validate已验证
		nRet = OfficalNavigateGetOverTime(model.mOvertime, serial, deadline, nDays);
		LOG_GLTX("get overtime(%d) -->%d,%d,%s", product_type, nRet, nDays, deadline.c_str());
		if(!nRet && !model.mDownload.empty())
		{
			if(nDays > 0 && IsCheckChangeOfDeadlineDate(deadline))
			{
				nRet = OfficalNavigateDownCert(model.mDownload, serial);
				LOG_GLTX("down cert -->%d", nRet);
			}
		}
	}

	real_url = redirect_url;
	return 0;
}


int UpdateAppEngine::AppUpdate(string& newversion, string& folder) {
    folder = MakeTmpFolder(UPDATE_TEMP_FOLDER);

    string navigate_url, navigate_file, serial, report_url;
    string offline_folder = Utils::Path::JoinPath(mInstallDir.c_str(), mOwnerPath.c_str(), "offline", NULL);
    Utils::Path::CheckPath(offline_folder);
    if (!Utils::Path::PathExists(offline_folder)) {
        LOG_GLTX("can not make offline folder: %s, error code -1", offline_folder.c_str());
        return -1;
    }

    string final_url =  mUpdateConfig.mCenterAddr.second;
    if (OFFICAL_HTTP == mUpdateConfig.mCenterAddr.first) {
    	std::string offical_url  = mUpdateConfig.mCenterAddr.second;
    	int         product_type = mAppConfig->GetProductType();

		serial = "H6F1IJ-C97VCK-S2C5RK-EM5308";  // default, 6*4+3=27
    	if(product_type == 0)
    	{
    		serial = mAppConfig->GetSerialNumber();
    		if(serial.length() == 27) product_type = -1;       // old single esm.
    	}

   		if(product_type == 0) offical_url = "http://rsup10.rising.com.cn/rsv16/esmlinuxver.xml";
   		else                  offical_url = Utils::Path::JoinPath(offical_url.c_str(), "rsunixver.xml", NULL);

		final_url.clear();
        int nRet = GetOfficalAddress(offical_url, serial, product_type, final_url, report_url);
        navigate_url = APP_UPDATE_NAVIGATE_URL;
        if(final_url.empty())
        {
        	LOG_GLTX("get offical address failed -->%d,%d,%s", product_type, nRet, final_url.c_str());
        	return nRet;
        }
    }
    else if (SHARE_FOLDER == mUpdateConfig.mCenterAddr.first) {
        final_url = offline_folder;
        navigate_url = APP_UPDATE_NAVIGATE_URL;
        navigate_file = APP_UPDATE_NAVIGATE_FILE;
    }
    else {
        navigate_url = APP_UPDATE_NAVIGATE_URL;
        navigate_file = APP_UPDATE_NAVIGATE_FILE;
    }

    //baseurl = final_url, store the information in navimodel
    NavigateModel navimodel;
    std::string   baseurl = GetDownloadUrl(mUpdateConfig.mCenterAddr.first, final_url, navigate_url, navimodel);
    if (baseurl.empty()) {
        if (SHARE_FOLDER == mUpdateConfig.mCenterAddr.first) {////final_url = "", return 1 means the current is the newest
            return 1;
        }
        else {
            LOG_GLTX("empty base url, error code -2");
            return -2;
        }
    }

    LOG_GLTX("update base url is: %s", baseurl.c_str());
    if (mUpdateMethod == UPDATE_NORMAL_VERSION) {
        LOG_GLTX("update UPDATE_NORMAL_VERSION");
        int err = NormalUpdateEntry(mUpdateConfig.mCenterAddr.first, folder, baseurl, navimodel, newversion);
        if (err) {
            return err;
        }
    }
    else {
        LOG_GLTX("update UPDATE_ORIGIN_VERSION");
        int err = OriginUpdateEntry(folder, baseurl, mCurrentVersion, newversion);
        if (err) {
            return err;
        }
    }

	ReportUpdateResult(report_url, serial, newversion);
    return 0;
}

string UpdateAppEngine::GetDefaultAddress() {
    if (mUpdateMethod == UPDATE_ORIGIN_VERSION) {
        return "/ruc/esm/linux/" + mUpdateConfig.mUpdateProduct;
    }
    else {
        struct utsname uts;
        if (0 == uname(&uts)) {
            if (Utils::String::StartsWith(uts.machine, "mips")) {
                return string("/ruc/esm/linux/mips64");
            }
            return string("/ruc/esm/linux/") + uts.machine;
        }
    }
    return  "";
}

int UpdateAppEngine::OriginUpdateEntry(const string& folder, const string& baseurl, const string& version, std::string& newversion) {
    const std::string versionlist = Utils::Path::JoinPath(folder.c_str(), CLIENT_NEW_LIST_FILE, NULL);
    int err = DownloadFile(Utils::Path::JoinPath(baseurl.c_str(), CLIENT_NEW_LIST_FILE, NULL), versionlist);
    if (err) {
        LOG_GLTX("new list downloading error, DownloadFile returned %d, errno %d, error code -21", err, errno);
        return -21;
    }

    newversion = GetNewVersion(versionlist);
    LOG_GLTX("the new version is %s and the current is %s", newversion.c_str(), version.c_str());
    if (newversion <= version) {
        return 1;
    }

    if (mObserver) {
        mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                 UpdateObserver::UDDATE_GET_NEW_VERSION,
                                 &newversion);
    }

    LOG_GLTX("start to download file");
    const std::string clientzipfile = Utils::Path::JoinPath(folder.c_str(), CLIENT_MD5_LIST_FILE, NULL);
    err = DownloadFile(Utils::Path::JoinPath(baseurl.c_str(), CLIENT_MD5_LIST_FILE, NULL), clientzipfile);
    if (err) {
        LOG_GLTX("client zip file downloading error, DownloadFile returned %d, error code -22", err, errno);
        return -22;
    }

    std::string clientmd5file;
    if (UnzipFile(clientzipfile, clientmd5file)) {
        LOG_GLTX("failed to unzip file, error code -23");
        return -23;
    }


    std::map<std::string, std::string> clientmd5;
    //filemd5: file hash
    if (GetUpdateFileMd5(clientmd5file, clientmd5)) {
        LOG_GLTX("failed to get update file md5, error code -24");
        return -24;
    }

    unlink(clientmd5file.c_str());

    std::vector<UPDATALISTINFO> updates;
    std::vector<std::string> updateless;
    int count = ParseVersionList(versionlist, clientmd5, updates, updateless);
    if (count < 0) {
        LOG_GLTX("failed to parse version list, error code -25");
        return -25;
    }
    unlink(versionlist.c_str());

    LOG_GLTX("the amount of download files: %d", count);

    if (mObserver) {
        mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                 UpdateObserver::UPDATE_GET_FILE_LIST,
                                 &count);
    }

    std::vector<UPDATALISTINFO>::iterator iter;
    for (iter=updates.begin(); iter!=updates.end(); ++iter) {
        if (DownloadUpdateFile(*iter, folder, baseurl)) {
            LOG_GLTX("failed to download update file from %s, error code -26", baseurl.c_str());
            return -26;
        }

        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                     UpdateObserver::UPDATE_DOWNLOAD_FILE,
                                     iter->filelist.filename);
        }
    }
    return 0;
}

int UpdateAppEngine::NormalUpdateEntry(UPDATE_SOURCE sourcetype, const string& folder, const string& baseurl, const NavigateModel& navimodel, string& newversion) {
    string vernavi = "rec_" + navimodel.mVersion + ".xml";
    string versionurl = Utils::Path::JoinPath(baseurl.c_str(), vernavi.c_str(), NULL);
    string header, body;

    LOG_GLTX("normal update entry url: %s", versionurl.c_str());
    if (SHARE_FOLDER == sourcetype) {
        if (!Utils::Path::FileAvailed(versionurl)) {
            return 1;
        }
    }

    GetNavigate(sourcetype, versionurl, body);
    if (IsHtmlErrorPage(body)) {
        LOG_GLTX("html page error, error code -11");
        return -11;
    }
    map<string, string> versions;
    if (GetUpdateVersion(body, newversion, versions)) {//error: -n, success: 0
        LOG_GLTX("failed to get update versions, body: %s, error code -12", body.c_str());
        return -12;
    }

    if (Utils::String::CompareVersion(mCurrentVersion, newversion) < 0) {
        if (mObserver) {
            mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                     UpdateObserver::UDDATE_GET_NEW_VERSION,
                                     &newversion);
        }
    }
    else {
        LOG_GLTX("current version is already the newest");
        return 1;
    }

    map<string, string>::iterator iter;
    for (iter=versions.begin(); iter!=versions.end(); ++iter) {
        // TODO 此处是为了应对3.0.0.19版本无法解析COMMON和TOOLS,故在此做了强制修改，以后必须得把这个改回去
        if (iter->first == "ep" || iter->first == "xav" || iter->first == "xfw") {
            if (UpdateComponent(sourcetype, iter->first, baseurl, folder)) {
                LOG_GLTX("failed to update component %s, error code -13", iter->first.c_str());
                return -13;
            }
        }
    }
    return 0;
}

int UpdateAppEngine::UpdateFiles(const std::string & folder, const std::string& version) {
    LOG_GLTX("download finished");
    mCurrentVersion = version;

    //file downloading finished, replace the update entry
    string update_entry = Utils::Path::JoinPath(folder.c_str(), DEPLOYED_BIN_FOLDER, UPDATE_PROCESS_ENTRY, NULL);
    if (Utils::Path::FileAvailed(update_entry)) {
        LOG_GLTX("found GLTX2, and replace it");

        std::string backupFolder = Utils::Path::JoinPath(mInstallDir.c_str(), FOLDER_RJJH_GLTX, UPDATE_BACKUP_FOLDER, NULL);
        Utils::Path::CheckPath(backupFolder);

        // first, backup the origin file
        if (!Utils::Path::CopyFile(
                    Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, UPDATE_PROCESS_ENTRY, NULL),
                    Utils::Path::JoinPath(backupFolder.c_str(), UPDATE_PROCESS_ENTRY, NULL))) {
            Utils::Path::DeleteFolder(backupFolder.c_str());
            LOG_GLTX("failed to copy file, error code -31");
            return -31;
        }
        // second, copy the new update entry to bin
        if (!Utils::Path::CopyFile(
                    update_entry,
                    Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, UPDATE_PROCESS_ENTRY, NULL))) {
            Utils::Path::CopyFile(
                        Utils::Path::JoinPath(backupFolder.c_str(), UPDATE_PROCESS_ENTRY, NULL),
                        Utils::Path::JoinPath(mInstallDir.c_str(), DEPLOYED_BIN_FOLDER, UPDATE_PROCESS_ENTRY, NULL));
            Utils::Path::DeleteFolder(backupFolder.c_str());
            LOG_GLTX("failed to copy file, error code -32");
            return -32;
        }
        // third, delete the temp folder
        Utils::Path::DeleteFolder(backupFolder.c_str());
        // must delete the update entry
        unlink(update_entry.c_str());
    }
    return 0;
}

int UpdateAppEngine::UpdateComponent(UPDATE_SOURCE type, const string& name, const string& baseurl, const string& tmpfolder) {
    string navi = name + ".xml";
    string componenturl = Utils::Path::JoinPath(baseurl.c_str(), "xml", name.c_str(), navi.c_str(), NULL);
    string header, content;

    GetNavigate(type, componenturl, content);
//    if (http_get(componenturl, "", header, content)) {
//        return -1;
//    }
    LOG_GLTX("GetNavigate component url:%s, content:%s", componenturl.c_str(), content.c_str());
    vector<UpdateItem> details;
    if (GetComponentDetail(name, content, details)) {
        return -1;
    }

    vector<UpdateItem>::iterator iter;
    for (iter=details.begin(); iter!=details.end(); ++iter) {
        string origin = Utils::Path::JoinPath(mInstallDir.c_str(), iter->dest.c_str(), NULL);
        string hash = Utils::String::ToUpper(Utils::Hash::GetFileMD5(origin));
        if (hash != iter->md5) {
            string zipfile = iter->dest + "_" + iter->md5 + ".rz";
            string downloadurl = Utils::Path::JoinPath(baseurl.c_str(), "rz", name.c_str(), zipfile.c_str(), NULL);
            string downloadfile = Utils::Path::JoinPath(tmpfolder.c_str(), iter->dest.c_str(), NULL);

            string path, file;
            Utils::Path::SplitPath(downloadfile, path, file);
            Utils::Path::CheckPath(path);
            string downzip = Utils::Path::JoinPath(path.c_str(), (iter->name + "_" + iter->md5 + ".rz").c_str(), NULL);

            LOG_GLTX("download UpdateComponent file %s to %s", downloadurl.c_str(), downzip.c_str());
            bool ret = false;
            if (SHARE_FOLDER == type) {
                ret = Utils::Path::CopyFile(downloadurl, downzip)?0:-1;
            }
            else {
                ret = DownloadFile(downloadurl, downzip);//success: 0, empty: -2, curl failure: positive num
            }
            if (ret) {
                LOG_GLTX("file download failed, errno %d", ret);
                unlink(downzip.c_str());
            }

            if (Utils::SevenZip::lzma_uncompress(downzip, downloadfile)) {
                LOG_GLTX("file extraction failed");
                unlink(downloadfile.c_str());
            }
            unlink(downzip.c_str());

            string newhash = Utils::String::ToUpper(Utils::Hash::GetFileMD5(downloadfile));
            if (newhash != iter->md5) {
                LOG_GLTX("md5 mismatched, file download failed, return -1");
                return -1;
            }

            chmod(downloadfile.c_str(), 0755);

            if (mObserver) {
                mObserver->OnUpdateEvent(UpdateObserver::UPDATE_EVENT_APP,
                                         UpdateObserver::UPDATE_DOWNLOAD_FILE,
                                         &iter->name);
            }
        }

    }
    return 0;
}

int UpdateAppEngine::GetComponentDetail(const string& name, const string& content, vector<UpdateItem>& details) {
    xmlDocPtr doc = xmlReadMemory(content.c_str(), content.size(), NULL, "UTF-8", 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    string version = (const char*)xmlGetProp(root, BAD_CAST"version");
    for(xmlNodePtr ptr = root->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if (xmlStrcmp(BAD_CAST "componet",ptr->name) == 0) {
            ParseComponent(name.c_str(), ptr, details);
        }
    }

    return 0;
}

int UpdateAppEngine::GetUpdateVersion(const string& content, string& version, map<string, string>& components) {
    xmlDocPtr doc = xmlReadMemory(content.c_str(), content.size(), NULL, "UTF-8", 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }
    xmlChar *data = xmlGetProp(root, BAD_CAST"version");
    version = (const char*)data;
    xmlFree(data);

    for(xmlNodePtr ptr = root->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "item",ptr->name) == 0) {
            data = xmlGetProp(ptr, BAD_CAST"name");
            string name = (const char*)data;
            xmlFree(data);
            data = xmlGetProp(ptr, BAD_CAST"version");
            string ver = (const char*)data;
            xmlFree(data);
            if (name != "common") {
                components.insert(make_pair(name, ver));
            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int UpdateAppEngine::ParseComponent(const char *name, xmlNodePtr node, vector<UpdateItem>& details) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if (xmlStrcmp(BAD_CAST "kernel", ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(XML_ELEMENT_NODE != child->type) {
                    continue;
                }
                else if (xmlStrcmp(BAD_CAST "file", child->name) == 0) {
                    UpdateItem item;
                    ParseUpdateItem(child, item);
                    if (!item.name.empty()) {
                        // if the kernel file match the system kernel version
                        int start = item.name.find("_");
                        int end = item.name.rfind(".ko");
                        if (start!=(int)string::npos && end!=(int)string::npos) {
                            string env = item.name.substr(start+1, end-start-1);
                            if (env == Utils::OS::GetKernelVersionString()) {
                                item.type = UpdateItem::KERNEL;
                                details.push_back(item);
                            }
                        }
                    }
                }
            }
        }
        else if (xmlStrcmp(BAD_CAST name, ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(XML_ELEMENT_NODE != child->type) {
                    continue;
                }
                else if (xmlStrcmp(BAD_CAST "file", child->name) == 0) {
                    UpdateItem item;
                    ParseUpdateItem(child, item);
                    item.type = UpdateItem::FILE;
                    details.push_back(item);
                }
            }
        }
    }
    return 0;
}

int UpdateAppEngine::ParseUpdateItem(xmlNodePtr child, UpdateItem& item) {
    xmlChar *data = xmlGetProp(child, BAD_CAST"name");
    item.name = (const char*)data;
    xmlFree(data);
    data = xmlGetProp(child, BAD_CAST"dstpath");
    item.dest = (const char*)data;
    xmlFree(data);
    data = xmlGetProp(child, BAD_CAST"md5");
    item.md5 = (const char*)data;
    xmlFree(data);
    data = xmlGetProp(child, BAD_CAST"ver");
    item.version = (const char*)data;
    xmlFree(data);
    return 0;
}

int UpdateAppEngine::DownloadUpdateFile(const UPDATALISTINFO& info, const std::string& folder, const std::string& baseurl) {
    std::string dest = Utils::Path::JoinPath(folder.c_str(), info.filelist.filename, NULL);
    std::string path, file;
    if (Utils::Path::SplitPath(dest, path, file)) {
        Utils::Path::mkdirs(path);

        std::string url = Utils::Path::JoinPath(baseurl.c_str(), info.filelist.downname, NULL);
        std::string protocol, remotefile;
        if (!Utils::Path::SplitPath(url, protocol, remotefile)) {
            return -1;
        }

        std::string downloadfile = Utils::Path::JoinPath(path.c_str(), remotefile.c_str(), NULL);
        LOG_GLTX("download file %s to %s", url.c_str(), downloadfile.c_str());
        int err = DownloadFile(url, downloadfile);
        if (err) {
            LOG_GLTX("file downloading error, DownloadFile returned %d, errno %d", err, errno);
            return -2;
        }

        if (Utils::Path::FileAvailed(downloadfile)) {
            std::string outfile;
            if (UnzipFile(downloadfile, outfile)) {
                return -1;
            }

            int pos = outfile.find(info.filetype);
            if (pos != -1) {
                std::string real = outfile.substr(0, pos-1);
                return rename(outfile.c_str(), real.c_str());
            }
        }
    }
    return -1;
}

int UpdateAppEngine::OfficalNavigateGetOverTime(const std::string & strUrl, const std::string & strSerial, std::string & strDeadline, int & nDays)
{
	#define PARSE_OFFICAL_REDIRECT_VALUE(_reply_info, _key, _key_size, _value) { \
		const char * __p1, * __p2; \
		__p1 = strstr(_reply_info, _key); \
		if(__p1) { \
			__p2 = __p1+_key_size; \
			__p1 = strchr(__p2, '&'); \
			if(__p1) _value.assign(__p2, __p1-__p2); \
			else     _value = __p2; \
		} \
	}

	std::string  strQueryUrl, strRedirectUrl, strTmp;
	const char * p;

	Assert(!strSerial.empty());
	strQueryUrl  = strUrl + "?info=";
	strTmp       = "sn="+strSerial;
	strQueryUrl += Utils::Hash::RSEntrypt(strTmp);

	http_redirect(strQueryUrl, strRedirectUrl);
	if(strRedirectUrl.empty()) return -1;

	p = strstr(strRedirectUrl.c_str(), "?info=");
	if(p)
	{
		std::string strOverInfo;

		strTmp = p+6;
		strOverInfo = Utils::Hash::RSDecrypt(strTmp);
		strTmp.clear();

		PARSE_OFFICAL_REDIRECT_VALUE(strOverInfo.c_str(), "stopdate=", 9, strDeadline);
		PARSE_OFFICAL_REDIRECT_VALUE(strOverInfo.c_str(), "interval=", 9, strTmp);
		if(!strTmp.empty())  nDays = atoi(strTmp.c_str());
		return 0;
	}

	return -2;
}

int  UpdateAppEngine::OfficalNavigateDownCert(const std::string & strUrl, const std::string & strSerial)
{
	int          nRet;
	std::string  strQueryUrl, strHeader, strBody, strConn, strTmp;

	Assert(!strSerial.empty());
	strQueryUrl  = strUrl + "?info=";
	strTmp       = "sn="+strSerial;
	strQueryUrl += Utils::Hash::RSEntrypt(strTmp);

    nRet = http_get(strQueryUrl, "", strHeader, strBody, strConn);
    if(nRet || strBody.empty())
	{
        LOG_GLTX("down the offical cert failed -->%d,%d,%s", nRet, strBody.length(), strQueryUrl.c_str());
        return nRet;
    }

	// 保存证书
	char szName[32] = {0};
	sprintf(szName, "%u.lic", (int)time(0));
	strTmp = Utils::Path::JoinPath(mInstallDir.c_str(), PLUGINS_PATH, szName, NULL);
	FILE * pf = fopen(strTmp.c_str(), "wb");
	if(!pf) return -1;

	fwrite(strBody.c_str(), 1, strBody.length(), pf);
	fclose(pf);

	mAppConfig->SetAppLicenseFile(strTmp);
	mAppConfig->StoreConfig();

	return 0;
}

bool UpdateAppEngine::IsCheckChangeOfDeadlineDate(const std::string & strDate)
{
	std::string strLocalDate = mAppConfig->GetAppDeadline();
	std::string strNewDate;

	Assert(!strLocalDate.empty());
	Assert(!strDate.empty());

	for(int i = 2; i < (int)strDate.length(); i++)
	{
		if(strDate[i] != '-') strNewDate += strDate[i];
	}

	return strstr(strLocalDate.c_str(), strNewDate.c_str()) != strLocalDate.c_str();
}

int  UpdateAppEngine::ReportUpdateResult(const std::string & strUrl, const std::string & strSerial, const std::string & strVersion)
{
	int  ret;
	std::string param, encrypt, url;
	std::string head, body, conn;

	if(strUrl.empty()) return 0;
	param      = "role=rua&sn=" + strSerial + "&guid=" + mAppConfig->GetAppGUID() + "&ver=" + strVersion;
	encrypt    = Utils::Hash::RSEntrypt(param);
	url        = strUrl + "?info=" + encrypt;

	ret = http_get(url, "", head, body, conn);
	if(ret) LOG_GLTX("report update result -->%d", ret);
	return ret;
}
