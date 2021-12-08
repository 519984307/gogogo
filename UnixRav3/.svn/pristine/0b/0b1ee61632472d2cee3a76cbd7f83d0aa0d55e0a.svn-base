#include "cross.h"
#include "commonapi.h"
#include "crypt/base64.h"
#include "cinifile.h"
#include "sqlite3.h"
#include "libxml/xpath.h"
#include "libxml/xpathInternals.h"
#include "libxml/xmlmemory.h"

#include <dlfcn.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define CFG_FILE    "/etc/.RavCfg"
#define REG_FILE    "/etc/.RiSingravnetwrd.reg"

/** 定义sqlite3的函数指针 */
typedef int (*sqlite3_prepare_ptr)(
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nBytes,             /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
typedef int (*sqlite3_open_ptr)(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
);
typedef int (*sqlite3_step_ptr)(sqlite3_stmt*);
typedef int (*sqlite3_column_int_ptr)(sqlite3_stmt*, int iCol);
typedef const unsigned char *(*sqlite3_column_text_ptr)(sqlite3_stmt*, int iCol);
typedef int (*sqlite3_finalize_ptr)(sqlite3_stmt *pStmt);
typedef int (*sqlite3_close_ptr)(sqlite3 *);

sqlite3_open_ptr    sqlite3_open_p    = NULL;
sqlite3_prepare_ptr sqlite3_prepare_p = NULL;
sqlite3_step_ptr    sqlite3_step_p    = NULL;
sqlite3_column_int_ptr  sqlite3_column_int_p = NULL;
sqlite3_column_text_ptr sqlite3_column_text_p = NULL;
sqlite3_finalize_ptr sqlite3_finalize_p = NULL;
sqlite3_close_ptr   sqlite3_close_p   = NULL;

/** 定义libxml2的函数指针 */
typedef xmlDocPtr (*xmlNewDoc_ptr)(const xmlChar *version);
typedef xmlNodePtr (*xmlNewNode_ptr)(xmlNsPtr ns, const xmlChar *name);
typedef xmlNodePtr (*xmlDocSetRootElement_ptr)(xmlDocPtr doc, xmlNodePtr root);
typedef xmlAttrPtr (*xmlNewProp_ptr)(xmlNodePtr node, const xmlChar *name, const xmlChar *value);
typedef xmlNodePtr (*xmlAddChild_ptr)(xmlNodePtr parent, xmlNodePtr cur);
typedef xmlNodePtr (*xmlNewTextChild_ptr)(xmlNodePtr parent, xmlNsPtr ns, const xmlChar *name, const xmlChar *content);
typedef int (*xmlSaveFormatFileEnc_ptr)(const char *filename, xmlDocPtr cur, const char *encoding, int format);
typedef void (*xmlFreeDoc_ptr)(xmlDocPtr cur);

xmlNewDoc_ptr   xmlNewDoc_p   = NULL;
xmlNewNode_ptr  xmlNewNode_p  = NULL;
xmlDocSetRootElement_ptr xmlDocSetRootElement_p =NULL;
xmlNewProp_ptr  xmlNewProp_p  = NULL;
xmlAddChild_ptr xmlAddChild_p = NULL;
xmlNewTextChild_ptr xmlNewTextChild_p = NULL;
xmlSaveFormatFileEnc_ptr xmlSaveFormatFileEnc_p = NULL;
xmlFreeDoc_ptr  xmlFreeDoc_p  = NULL;


CrossVersion::CrossVersion() {
    mSqliteHandler = NULL;
    mXmlHandler = NULL;
}

int CrossVersion::InitFuncPtr() {
    string sqlib = Utils::Path::JoinPath(mOldInstallPath.c_str(), "lib", "libravsqlite3.so", NULL);
    if (Utils::Path::FileAvailed(sqlib)) {
        mSqliteHandler = dlopen(sqlib.c_str(), RTLD_NOW | RTLD_GLOBAL);
        if (NULL != mSqliteHandler) {
            sqlite3_open_p = (sqlite3_open_ptr)dlsym(mSqliteHandler, "sqlite3_open");
            sqlite3_prepare_p = (sqlite3_prepare_ptr)dlsym(mSqliteHandler, "sqlite3_prepare");
            sqlite3_step_p = (sqlite3_step_ptr)dlsym(mSqliteHandler, "sqlite3_step");
            sqlite3_column_int_p = (sqlite3_column_int_ptr)dlsym(mSqliteHandler, "sqlite3_column_int");
            sqlite3_column_text_p = (sqlite3_column_text_ptr)dlsym(mSqliteHandler, "sqlite3_column_text");
            sqlite3_finalize_p = (sqlite3_finalize_ptr)dlsym(mSqliteHandler, "sqlite3_finalize");
            sqlite3_close_p = (sqlite3_close_ptr)dlsym(mSqliteHandler, "sqlite3_close");
        }
    }
    string xmlib = Utils::Path::JoinPath(mOldInstallPath.c_str(), "lib", "libravxml2.so", NULL);
    if (Utils::Path::FileAvailed(xmlib)) {
        mXmlHandler = dlopen(xmlib.c_str(), RTLD_NOW | RTLD_GLOBAL);
        char *error = dlerror();
        if (NULL != mXmlHandler) {
            xmlNewDoc_p = (xmlNewDoc_ptr)dlsym(mXmlHandler, "xmlNewDoc");
            xmlNewNode_p = (xmlNewNode_ptr)dlsym(mXmlHandler, "xmlNewNode");
            xmlDocSetRootElement_p = (xmlDocSetRootElement_ptr)dlsym(mXmlHandler, "xmlDocSetRootElement");
            xmlNewProp_p = (xmlNewProp_ptr)dlsym(mXmlHandler, "xmlNewProp");
            xmlAddChild_p = (xmlAddChild_ptr)dlsym(mXmlHandler, "xmlAddChild");
            xmlNewTextChild_p = (xmlNewTextChild_ptr)dlsym(mXmlHandler, "xmlNewTextChild");
            xmlSaveFormatFileEnc_p = (xmlSaveFormatFileEnc_ptr)dlsym(mXmlHandler, "xmlSaveFormatFileEnc");
            xmlFreeDoc_p = (xmlFreeDoc_ptr)dlsym(mXmlHandler, "xmlFreeDoc");
        }
    }
    return 0;
}

string CrossVersion::GetVersion() {
    string verfile = Utils::Path::JoinPath(mOldInstallPath.c_str(), "version", "version.ini", NULL);
    if (Utils::Path::FileAvailed(verfile)) {
        CINIFile ini(verfile, false);
        string version;
        ini.readitemstr("VERSION", "VERSION", version, "");
        return version;
    }
    return "";
}

string CrossVersion::GetSerial() {
    if (!Utils::Path::FileAvailed(CFG_FILE)) {
        return "";
    }

    STSNINFO_UNIX stSninfo = {0};
    FILE *fp = fopen(CFG_FILE, "rb");
    if (NULL == fp) {
        return "";
    }
    while(!feof(fp)) {
        if(1 != fread(&stSninfo, sizeof(STSNINFO_UNIX), 1, fp)) {
            fclose(fp);
            return "";
        }

        string sn = base64_decode(stSninfo.szSN);
        if (!sn.empty()) {
            return sn;
        }
    }
    return "";
}

int CrossVersion::GetSGUID(string& guid, string& sguid) {
    string sguidfile = Utils::Path::JoinPath(mOldInstallPath.c_str(), "bin", "sguid.ini", NULL);
    if (!Utils::Path::FileAvailed(sguidfile)) {
        return -1;
    }

    CINIFile ini(sguidfile, false);
    ini.readitemstr("sguid", "sguid", sguid, "");
    ini.readitemstr("sguid", "guid", guid, "");
    return 0;
}

bool CrossVersion::Exist() {
    if (!Utils::Path::FileAvailed(REG_FILE)) {
        return false;
    }

    FILE *fp = NULL;
    if ((fp = fopen(REG_FILE, "r")) == NULL) {
        return false;
    }

    char install_path[1024] = {0};
    fgets(install_path, 1024, fp);
    fclose(fp);

    if (Utils::Path::PathExists(install_path)) {
        mOldInstallPath = install_path;
        return true;
    }
    return false;
}

int CrossVersion::GetCenterAddr(string& type, string &addr) {
    if (mOldInstallPath.empty()) {
        FILE *f = fopen(REG_FILE, "r");
        if (NULL != f) {
            char install_path[1024] = {0};
            fgets(install_path, 1024, f);
            mOldInstallPath = install_path;
            fclose(f);
        }
    }

    string inifile = Utils::Path::JoinPath(mOldInstallPath.c_str(), "ravnet", "sys", "ravservice.ini2", NULL);
    if (Utils::Path::FileAvailed(inifile)) {
        CINIFile ini(inifile, false);
        ini.readitemstr("AGENTIP", "AGENTIP", addr, "");
        ini.readitemstr("AGENTIP", "MANAGEMENT_CENTER_TYPE", type, "");
        if (type == "ESM") {
            return 1; // SoftType::CENTER_ESM
        }
    }
    return 0;
}

int CrossVersion::ImportLogs(const string &outxml, string& version, string& guid, string& sguid, vector<LOG_INFO> &scanlogs, vector<LOG_INFO> &viruslogs) {
    xmlDocPtr doc = xmlReadFile(outxml.c_str(), NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    for(xmlNodePtr ptr=root->children; ptr; ptr=ptr->next) {
        if(xmlStrcmp(BAD_CAST"VERSION",ptr->name) == 0) {
            xmlChar *data = xmlNodeGetContent(ptr);
            version = (const char*)data;
            xmlFree(data);
        }
        else if(xmlStrcmp(BAD_CAST"GUID",ptr->name) == 0) {
            xmlChar *data = xmlNodeGetContent(ptr);
            guid = (const char*)data;
            xmlFree(data);
        }
        else if(xmlStrcmp(BAD_CAST"SGUID",ptr->name) == 0) {
            xmlChar *data = xmlNodeGetContent(ptr);
            sguid = (const char*)data;
            xmlFree(data);
        }
        else if(xmlStrcmp(BAD_CAST"SCANLOG",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"ITEM",child->name) == 0) {
                    LOG_INFO log;
                    for(xmlNodePtr grand=child->children; grand; grand=grand->next) {
                        if(xmlStrcmp(BAD_CAST"START",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info1 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"END",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info2 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"VIRUS",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info3 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"KILLED",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info4 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"FILES",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info5 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"TYPE",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info6 = (const char*)data;
                            xmlFree(data);
                        }
                    }
                    scanlogs.push_back(log);
                }
            }
        }
        else if(xmlStrcmp(BAD_CAST"VIRUSLOG",ptr->name) == 0) {
            for(xmlNodePtr child=ptr->children; child; child=child->next) {
                if(xmlStrcmp(BAD_CAST"ITEM",child->name) == 0) {
                    LOG_INFO log;
                    for(xmlNodePtr grand=child->children; grand; grand=grand->next) {
                        if(xmlStrcmp(BAD_CAST"VIRUS",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info1 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"STATE",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info2 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"TIME",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info3 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"PATH",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info4 = (const char*)data;
                            xmlFree(data);
                        }
                        else if(xmlStrcmp(BAD_CAST"TYPE",grand->name) == 0) {
                            xmlChar *data = xmlNodeGetContent(grand);
                            log.info5 = (const char*)data;
                            xmlFree(data);
                        }
                    }
                    viruslogs.push_back(log);
                }
            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int CrossVersion::ExportLogs(const string &outxml) {
//    sqlite3* sql_db = NULL;
//    string dbfile = Utils::Path::JoinPath(mOldInstallPath.c_str(), "ravnet", "sys", "logproxy.db", NULL);
//    if (Utils::Path::FileAvailed(dbfile)) {
//        if (SQLITE_OK != sqlite3_open_p(dbfile.c_str(), &sql_db)) {
//            sql_db = NULL;
//        }
//    }

    xmlDocPtr pdoc = xmlNewDoc(BAD_CAST("1.0"));
    xmlNodePtr root = xmlNewNode(NULL, BAD_CAST("ROOT"));
    xmlDocSetRootElement(pdoc, root);

    xmlNewTextChild(root, NULL, BAD_CAST("VERSION"), (xmlChar*)GetVersion().c_str());

    string guid, sguid;
    if (0 == GetSGUID(guid, sguid)) {
        xmlNewTextChild(root, NULL, BAD_CAST("GUID"), (xmlChar*)guid.c_str());
        xmlNewTextChild(root, NULL, BAD_CAST("SGUID"), (xmlChar*)sguid.c_str());
    }

    ExportScanLog(NULL, root);
    ExportVirusLog(NULL, root);
    ExportQuarantine(NULL, root);

    xmlSaveFormatFileEnc(outxml.c_str(), pdoc, "UTF-8", 1);
    xmlFreeDoc(pdoc);

//    sqlite3_close_p(sql_db);
    return 0;
}

int CrossVersion::ExportScanLog(void *sqlptr, void *xmlptr) {
    xmlNodePtr xml = (xmlNodePtr)xmlptr;

    xmlNodePtr scanlog = xmlNewNode(NULL, BAD_CAST"SCANLOG");

    // read from maunal
    string manual = Utils::Path::JoinPath(mOldInstallPath.c_str(), "log", "manuscanlog", NULL);
    if (Utils::Path::FileAvailed(manual)) {
        ifstream ifs(manual.c_str());
        if (ifs) {
            std::string line;
            while(std::getline(ifs, line)) {
                std::vector<std::string> flags;
                Utils::String::Split(line, ";", &flags);

                if (flags.size() == 5) {
                    xmlNodePtr item = xmlNewNode(NULL, BAD_CAST"ITEM");

                    xmlNewTextChild(item, NULL, BAD_CAST("START"), (xmlChar*)flags[0].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("END"), (xmlChar*)flags[1].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("VIRUS"), (xmlChar*)flags[2].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("KILLED"), (xmlChar*)flags[3].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("FILES"), (xmlChar*)flags[4].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("TYPE"), (xmlChar*)"MANUAL");

                    xmlAddChild(scanlog, item);
                }
            }
            ifs.close();
        }
    }

    manual = Utils::Path::JoinPath(mOldInstallPath.c_str(), "log", "remotescanlog", NULL);
    if (Utils::Path::FileAvailed(manual)) {
        ifstream ifs(manual.c_str());
        if (ifs) {
            std::string line;
            while(std::getline(ifs, line)) {
                std::vector<std::string> flags;
                Utils::String::Split(line, ";", &flags);

                if (flags.size() == 5) {
                    xmlNodePtr item = xmlNewNode(NULL, BAD_CAST"ITEM");

                    xmlNewTextChild(item, NULL, BAD_CAST("START"), (xmlChar*)flags[0].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("END"), (xmlChar*)flags[1].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("VIRUS"), (xmlChar*)flags[2].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("KILLED"), (xmlChar*)flags[3].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("FILES"), (xmlChar*)flags[4].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("TYPE"), (xmlChar*)"REMOTE");

                    xmlAddChild(scanlog, item);
                }
            }
            ifs.close();
        }
    }

    // read from sqlite
//    sqlite3 *sql_db = (sqlite3*)sqlptr;
//    if (sql_db == NULL || xml == NULL) {
//        return -1;
//    }

//    const char* tail = 0;
//    sqlite3_stmt* mt = NULL;
//    sqlite3_prepare_p(sql_db, "", -1, &mt, &tail);
//    if (sqlite3_step_p(mt) == SQLITE_ROW) {

//        sqlite3_column_int_p(mt, 0);
//        (const char*)sqlite3_column_text_p(mt, 0);
//    }
//    sqlite3_finalize_p(mt);

    xmlAddChild(xml, scanlog);
    return 0;
}

int CrossVersion::ExportVirusLog(void *sqlptr, void *xmlptr) {
    xmlNodePtr xml = (xmlNodePtr)xmlptr;

    xmlNodePtr scanlog = xmlNewNode(NULL, BAD_CAST"VIRUSLOG");

    // read from maunal
    string manual = Utils::Path::JoinPath(mOldInstallPath.c_str(), "log", "manuvirlog", NULL);
    if (Utils::Path::FileAvailed(manual)) {
        ifstream ifs(manual.c_str());
        if (ifs) {
            std::string line;
            while(std::getline(ifs, line)) {
                std::vector<std::string> flags;
                Utils::String::Split(line, ";", &flags);

                if (flags.size() == 5) {
                    xmlNodePtr item = xmlNewNode(NULL, BAD_CAST"ITEM");

                    xmlNewTextChild(item, NULL, BAD_CAST("VIRUS"), (xmlChar*)flags[0].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("STATE"), (xmlChar*)flags[1].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("TIME"), (xmlChar*)flags[2].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("PATH"), (xmlChar*)flags[3].c_str());
                    xmlNewTextChild(item, NULL, BAD_CAST("TYPE"), (xmlChar*)flags[4].c_str());

                    xmlAddChild(scanlog, item);
                }
            }
            ifs.close();
        }
    }

//    // read from sqlite
//    sqlite3 *sql_db = (sqlite3*)sqlptr;
//    if (sql_db == NULL || xml == NULL) {
//        return -1;
//    }

//    const char* tail = 0;
//    sqlite3_stmt* mt = NULL;
//    sqlite3_prepare_p(sql_db, "", -1, &mt, &tail);
//    if (sqlite3_step_p(mt) == SQLITE_ROW) {

//        sqlite3_column_int_p(mt, 0);
//        (const char*)sqlite3_column_text_p(mt, 0);
//    }
//    sqlite3_finalize_p(mt);

    xmlAddChild(xml, scanlog);
    return 0;
}

int CrossVersion::ExportQuarantine(void *sqlptr, void *xmlptr) {
    return 0;
}

int CrossVersion::CleanCrontab() {
    const char *tempfile = "/tmp/oldcrontab";
    const char *RULES[] = {
        "ravsingle/bin/timingscan",
        "bin/ravcmdscan",
        "ravsingle/bin/2005Update",
        "bin/ravcmdupdate",
        "bin/esmcmdscan",
        "bin/esmupdate.sh"
    };

    system("/usr/bin/crontab -l > /tmp/oldcrontab 2>/dev/null");
    if (Utils::Path::FileAvailed(tempfile)) {
        ifstream ifs(tempfile);
        if (ifs) {
            ofstream ofs("/tmp/newcrontab");
            if (!ofs) {
                return -1;
            }

            std::string line;
            while(std::getline(ifs, line)) {
                for(int i=0; i<sizeof(RULES)/sizeof(const char*); ++i) {
                    string cmd = Utils::Path::JoinPath(mOldInstallPath.c_str(), RULES[i], NULL);
                    if (line.find(cmd) != string::npos) {
                        ofs << line << endl;
                    }
                }
            }
            ofs.close();
        }
        ifs.close();
    }

    system("/usr/bin/crontab /tmp/newcrontab > /dev/null 2>&1");

    unlink(tempfile);
    unlink("/tmp/newcrontab");

    return 0;
}

int CrossVersion::DelSudoScript() {
    const char *shell = "/tmp/delsudo.sh";
    ofstream ofs(shell);
    if (ofs) {
        ofs << "#!/bin/sh" << endl;
        ofs << "sed -i '/\\/bin\\/ravunix/d' /etc/sudoers" << endl;
        ofs.close();
    }

    chmod(shell, 0755);
    system(shell);
    unlink(shell);

    return 0;
}

int CrossVersion::DisableAutostart() {
    const char *services[] = {
        "/etc/rc3.d/S99RISING",
        "/etc/rc2.d/S99RISING",
        "/etc/rc5.d/S99RISING",
        "/etc/rc.d/rc3.d/S99RISING",
        "/etc/rc.d/rc5.d/S99RISING",
        "/etc/rc0.d/K01RISING",
        "/etc/rc6.d/K01RISING",
        "/etc/rc.d/rc0.d/K01RISING",
        "/etc/rc.d/rc6.d/K01RISING"
    };
    for (int i=0; i<sizeof(services)/sizeof(const char*); ++i) {
        if (Utils::Path::FileAvailed(services[i])){
            unlink(services[i]);
        }
    }

    const char *systemd = "/usr/lib/systemd/system/Rising-Antivirus.service";
    if (Utils::Path::FileAvailed(systemd)) {
        system("systemctl disable /usr/lib/systemd/system/Rising-Antivirus.service  >/dev/null 2>&1");
        unlink(systemd);
    }
    return 0;
}

int CrossVersion::DeleteDesktopFile() {
    char chBuf[256];
    DIR * dir = NULL;
    struct dirent *ptr;

    dir = opendir("/home");
    if(NULL == dir) {
        return false;
    }
    while((ptr = readdir(dir)) != NULL) {
        if(0 == strcmp(ptr->d_name, ".")) {
            continue;
        }
        if(0 == strcmp(ptr->d_name, "..")){
            continue;
        }
        snprintf(chBuf, 256, "/home/%s", ptr->d_name);

        struct stat buf;
        stat(chBuf, &buf);
        if(buf.st_mode & S_IFDIR) {
            string desktop = Utils::Path::JoinPath(chBuf, "Desktop", "Rising-RAV.desktop", NULL);
            if (Utils::Path::FileAvailed(desktop)) {
                unlink(desktop.c_str());
            }
            desktop = Utils::Path::JoinPath(chBuf, "桌面", "Rising-RAV.desktop", NULL);
            if (Utils::Path::FileAvailed(desktop)) {
                unlink(desktop.c_str());
            }
        }
    }
    closedir(dir);

    string desktop = Utils::Path::JoinPath("/root", "Desktop", "Rising-RAV.desktop", NULL);
    if (Utils::Path::FileAvailed(desktop)) {
        unlink(desktop.c_str());
    }
    desktop = Utils::Path::JoinPath("/root", "桌面", "Rising-RAV.desktop", NULL);
    if (Utils::Path::FileAvailed(desktop)) {
        unlink(desktop.c_str());
    }

    // start menu
    desktop = Utils::Path::JoinPath("/usr/share/applications/", "Rising-RAV.desktop", NULL);
    if (Utils::Path::FileAvailed(desktop)) {
        unlink(desktop.c_str());
    }
    return 0;
}

int CrossVersion::UninstallOldVersion() {
    // stop the running process
    string process = Utils::Path::JoinPath(mOldInstallPath.c_str(), "bin", "esmagent.bin", NULL);
    Utils::OS::KillProc(process.c_str(), 9);

    // delete sudo script
    DelSudoScript();

    // clean the crontab
    CleanCrontab();

    // delete the autostart service
    DisableAutostart();

    // delete the reg file
    const char *reg_file[] = {
        "/etc/.RavCfg",
        "/etc/.RiSingravnetwrd.reg"
    };
    for (int i=0; i<sizeof(reg_file)/sizeof(const char*); ++i) {
        if (Utils::Path::FileAvailed(reg_file[i])){
            unlink(reg_file[i]);
        }
    }

    // delete all desktop file
    DeleteDesktopFile();

    // delete all file
    Utils::Path::DeleteFolder(mOldInstallPath.c_str());

    return 0;
}
