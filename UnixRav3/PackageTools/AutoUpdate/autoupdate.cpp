/**
 *
 **/
#include <string>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>

#include "commonapi.h"
#include "appconfig.h"
#include "baseconfig.h"
#include "communicatebundle.h"
#include "crypt/md5.h"
#include "SQLite3/sqlite3.h"

using namespace std;

const char *MAGIC = "RING";
const char *NAME = "autoupdate.bin";

class UpdateInfo {
public:
    UpdateInfo() {
        mFileInfo = NULL;
    }
    ~UpdateInfo() {

    }

private:
    typedef struct {
        char magic[4];
        int  count;
        int  offset;
        int  dataoff;
    }HEADER;

    typedef struct  {
        unsigned int zipsize;
        unsigned int filesize;
        char         filename[128];
        char         filepath[128];
        char         hash[32];
    }EXTRACT_FILE_INFO;

public:
    int Contruct() {
        char path[256];
        getcwd(path, sizeof(path));

        string self = Utils::Path::JoinPath(path, NAME, NULL);
        ifstream ifs(self.c_str(), ifstream::binary|ios::ate);
        if (!ifs) {
            return -1;
        }

        ifs.seekg(-sizeof(HEADER), ios::end);
        ifs.read((char*)&mHeader, sizeof(HEADER));
        if (memcmp(mHeader.magic, MAGIC, 4)) {
            return -1;
        }

        cout << "find " << mHeader.count << " file need to update" << endl;

        ifs.seekg(mHeader.offset, ios::beg);
        for (int i=0; i<mHeader.count; ++i) {
            EXTRACT_FILE_INFO extracted;
            ifs.read((char*)&extracted, sizeof(EXTRACT_FILE_INFO));
            mExtracFiles.push_back(extracted);
        }

        ifs.seekg(mHeader.dataoff, ios::beg);
        vector<EXTRACT_FILE_INFO>::iterator iter;
        for (iter=mExtracFiles.begin(); iter!=mExtracFiles.end(); ++iter) {
            cout << "extract " << iter->filename << endl;
            if (ExtracFile(ifs, *iter)) {
                cout << "extract failed" << endl;
                return -1;
            }
            sleep(1);
        }

        // change the version
        AppConfig *config = AppConfig::NEW("/etc/.RavCfg");
        config->ChangeConfigSection(AppConfig::APP_VERSION, "3.0.0.19");
        delete config;

        return 0;
    }

    std::string GetFileMD5(const std::string& path) {
        unsigned char data[1024];
        FILE *file = fopen(path.c_str(), "rb");
        if (NULL == file) {
            return "";
        }

        MD5 md5;

        while (1) {
            int readed = fread(data, 1, 1024, file);
            if (readed <= 0) {
                fclose(file);
                break;
            }

            md5.update(data, readed);
        }

        md5.finalize();
        return md5.hexdigest();
    }

    int StartUpdate() {
        vector<int> pids;
        Utils::OS::GetPid("/opt/BDFZ/RAV/bin/RAVRJJH0", pids);
        pid_t pid = pids[0];
        string user = Utils::OS::GetUserFromUID(Utils::OS::GetProcessUID(pid));

//        if (pid < 0) {
//            strcmp(pid, "root");
//        }

        cout << "get current user is " << user << endl;

        const char *PROC[] = {"RAVGLTX0", "RAVGJCZ0", "RAVZDFY0", "RAVRJJH0", "RAVRJJH2", "RAVRJJH4", "RAVRJJH6"};
        for (int i=0; i<sizeof(PROC)/sizeof(const char*); ++i) {
            do {
                cout << "kill " << PROC[i] << endl;
                string proc = Utils::Path::JoinPath("/opt/BDFZ/RAV/bin", PROC[i], NULL);
                Utils::OS::KillProc(proc.c_str(), 9);
            }
            while(Utils::OS::CheckProcessState("/opt/BDFZ/RAV/bin/RAVGLTX0")>0);
        }

        vector<EXTRACT_FILE_INFO>::iterator iter;
        for (iter=mExtracFiles.begin(); iter!=mExtracFiles.end(); ++iter) {
            string newfile = Utils::Path::JoinPath("/tmp", iter->filename, NULL);
            string destfile = Utils::Path::JoinPath("/opt/BDFZ/RAV", iter->filepath, NULL);

            if (strcmp(iter->filename, "RAVbdfz.desktop") == 0) {
                Utils::Path::CopyFile(newfile, "/usr/share/applications/RAVbdfz.desktop");
            }
            else {

                cout << "replace the " << destfile << endl;
                if (!Utils::Path::CopyFile(newfile, destfile)) {
                    cout << "replace failed" << endl;
                }
            }


        }

        sleep(1);

        const char *START[] = {"RAVGLTX1", "RAVGJCZ1", "RAVZDFY1"};
        for (int i=0; i<sizeof(START)/sizeof(const char*); ++i) {
            do {
                cout << "start " << PROC[i] << endl;
                string proc = Utils::Path::JoinPath("/opt/BDFZ/RAV/bin", START[i], NULL);
                system(proc.c_str());
            }
            while(0);
        }

        sleep(1);

        cout << "start the RAVRJJH0" << endl;
        char command[128];
        sprintf(command, "/opt/BDFZ/RAV/bin/RAVRJJH1 -s");
        system(command);

//        sleep(10);

//        cout << "send the update command" << endl;
//        Update("/opt/BDFZ/RAV");

        int retry = 0;
        while (true && retry++ < 300) {
            sleep(1);

            cout << "waiting" << endl;
            AppConfig *config = AppConfig::NEW("/etc/.RavCfg");
            if (config) {
                if (config->GetAppVersion() == "3.0.0.19") {
                    cout << "update success" << endl;
                     delete config;
                    return 0;
                }
                delete config;
            }
        }
        sleep(1);
        return -1;
    }

    int Update(const string& installPath) {
        std::string sock = Utils::Path::JoinPath(installPath.c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL);
        CommandBundle command;
        command.mCommandID = CommandBundle::DAEMON_CENTER_UPDATE;

        int ret = 0;
        RStreamPack<CommandBundle> bundle(RStreamPack<CommandBundle>::STRING, command);
        if (0 == bundle.Externalize(sock.c_str(), ret)) {
            return ret;
        }
        return -1;
    }


    int ExtracFile(ifstream& ifs, const EXTRACT_FILE_INFO& info) {
        string tmp = Utils::Path::JoinPath("/tmp", Utils::String::itoa(Utils::Date::GetTimeStamp()).c_str(), NULL);
        if (0 == ReadFile(ifs, tmp, info.zipsize)) {
            string realfile = Utils::Path::JoinPath("/tmp", info.filename, NULL);
            if (0 == Utils::Compress::UnzipFile(tmp, realfile)) {
                string hash = GetFileMD5(realfile);
                if (0 == memcmp(info.hash, hash.c_str(), 32)) {
                    return 0;
                }
            }
        }
        return -1;
    }

    int ReadFile(ifstream& ifs, const string& file, int size) {
        char buffer[1024];
        int readed = 0;

        ofstream ofs(file.c_str(), ios::binary|ios::out);
        if (!ofs) {
            return -1;
        }

        do {
            ifs.read(buffer, (size-readed)>1024?1024:(size-readed));
            ofs.write(buffer, ifs.gcount());
            readed += ifs.gcount();
        }
        while(readed < size);
        ofs.close();
        return 0;
    }

    int RepairConfig() {
        cout << "check the config" << endl;

        if (Utils::Path::FileAvailed("/etc/.RavCfg")) {
            AppConfig *config = AppConfig::NEW("/etc/.RavCfg");
            if (config) {
                if (config->GetSubProductGUID() == "1AB67467-096C-4bea-B852-2CA73F6E854C"
                        || config->GetSubProductGUID() == "962CF847-3D72-49d0-895A-16C91858A13D") {
                    cout << "change the sub product id to china auth" << endl;
                    config->SetSubProductGUID("90DDA51B-33AB-4d94-8F04-0B980AD75CDE");
                    config->SetAppType("linux:lcserver");

                    cout << "change the component to XAV" << endl;
                    config->SetSubComponents(3);
                    config->SetFunctions(3);

                    // init the defalut config
                    config->SetInstallPath("/opt/BDFZ/RAV");
                    config->SetProductType(2);

                    cout << "change the update time" << endl;
                    config->SetUpdateTime(Utils::Date::GetTimeStamp());                                  
                }
                else {
                    config->SetInstallPath("/opt/BDFZ/RAV");
                    config->SetProductType(2);
                    config->SetSubComponents(3);
                    config->SetEID("e0001");
                    config->SetCenterURL("https://10.41.108.140");
                    config->SetAppVersion("3.0.0.19");
                    config->SetVlibVersion("31.0508.0002");
                    config->SetInstallTime(Utils::Date::GetTimeStamp());
                    config->SetSubProductGUID("90DDA51B-33AB-4d94-8F04-0B980AD75CDE");
                    config->SetAppType("linux:lcserver");
                    config->SetUpdateTime(Utils::Date::GetTimeStamp());

                    config->SetFunctions(3);

                }
                config->StoreConfig();
                delete config;
                return 0;
            }
        }
        else {
            FILE *f = fopen("/etc/.RavCfg", "wb+");
            if (f) {
                fclose(f);
            }

            AppConfig *config = AppConfig::NEW("/etc/.RavCfg");
            if (config) {
                if (config->GetSubProductGUID() == "1AB67467-096C-4bea-B852-2CA73F6E854C"
                        || config->GetSubProductGUID() == "962CF847-3D72-49d0-895A-16C91858A13D") {
                    cout << "change the sub product id to china auth" << endl;
                    config->SetSubProductGUID("90DDA51B-33AB-4d94-8F04-0B980AD75CDE");
                    config->SetAppType("linux:lcserver");

                    cout << "change the component to XAV" << endl;
                    config->SetSubComponents(3);
                    config->SetFunctions(3);

                    // init the defalut config
                    config->SetInstallPath("/opt/BDFZ/RAV");
                    config->SetProductType(2);

                    cout << "change the update time" << endl;
                    config->SetUpdateTime(Utils::Date::GetTimeStamp());
                }
                else {
                    config->SetInstallPath("/opt/BDFZ/RAV");
                    config->SetProductType(2);
                    config->SetSubComponents(3);
                    config->SetEID("e0001");
                    config->SetCenterURL("https://10.41.108.140");
                    config->SetAppVersion("3.0.0.19");
                    config->SetVlibVersion("31.0508.0002");
                    config->SetInstallTime(Utils::Date::GetTimeStamp());
                    config->SetSubProductGUID("90DDA51B-33AB-4d94-8F04-0B980AD75CDE");
                    config->SetAppType("linux:lcserver");
                    config->SetUpdateTime(Utils::Date::GetTimeStamp());

                    config->SetFunctions(3);

                }
                config->StoreConfig();
                delete config;
                return 0;
            }
        }



        return -1;
    }

    int SetUpdateStorage() {
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
        typedef int (*sqlite3_exec_ptr)(
          sqlite3*,                                  /* An open database */
          const char *sql,                           /* SQL to be evaluated */
          int (*callback)(void*,int,char**,char**),  /* Callback function */
          void *,                                    /* 1st argument to callback */
          char **errmsg                              /* Error msg written here */
        );

        sqlite3_open_ptr    sqlite3_open_p    = NULL;
        sqlite3_prepare_ptr sqlite3_prepare_p = NULL;
        sqlite3_step_ptr    sqlite3_step_p    = NULL;
        sqlite3_column_int_ptr  sqlite3_column_int_p = NULL;
        sqlite3_column_text_ptr sqlite3_column_text_p = NULL;
        sqlite3_finalize_ptr sqlite3_finalize_p = NULL;
        sqlite3_close_ptr   sqlite3_close_p   = NULL;
        sqlite3_exec_ptr   sqlite3_exec_p = NULL;

        string sqlib = Utils::Path::JoinPath("/opt/BDFZ/RAV", "lib", "libravsqlite3.so", NULL);
        if (Utils::Path::FileAvailed(sqlib)) {
            void *mSqliteHandler = dlopen(sqlib.c_str(), RTLD_NOW | RTLD_GLOBAL);
            if (NULL != mSqliteHandler) {
                sqlite3_open_p = (sqlite3_open_ptr)dlsym(mSqliteHandler, "sqlite3_open");
                sqlite3_prepare_p = (sqlite3_prepare_ptr)dlsym(mSqliteHandler, "sqlite3_prepare");
                sqlite3_step_p = (sqlite3_step_ptr)dlsym(mSqliteHandler, "sqlite3_step");
                sqlite3_column_int_p = (sqlite3_column_int_ptr)dlsym(mSqliteHandler, "sqlite3_column_int");
                sqlite3_column_text_p = (sqlite3_column_text_ptr)dlsym(mSqliteHandler, "sqlite3_column_text");
                sqlite3_finalize_p = (sqlite3_finalize_ptr)dlsym(mSqliteHandler, "sqlite3_finalize");
                sqlite3_close_p = (sqlite3_close_ptr)dlsym(mSqliteHandler, "sqlite3_close");
                sqlite3_exec_p = (sqlite3_exec_ptr)dlsym(mSqliteHandler, "sqlite3_exec");

                sqlite3* sql_db = NULL;
                string dbfile = Utils::Path::JoinPath("/opt/BDFZ/RAV", "RJJHGLTX", "rav.db", NULL);
                if (SQLITE_OK == sqlite3_open_p(dbfile.c_str(), &sql_db)) {
                    const char* tail = 0;
                    sqlite3_stmt* mt = NULL;
                    sqlite3_prepare_p(sql_db, "select * from SettingTable where NAME='UpdateContent'", -1, &mt, &tail);
                    if (sqlite3_step_p(mt) == SQLITE_ROW) {
                        sqlite3_column_int_p(mt, 0);
                        (const char*)sqlite3_column_text_p(mt, 0);
                        sqlite3_finalize_p(mt);

                        char* szError=0;
                        sqlite3_exec_p(sql_db, "update SettingTable set INTV=1,INTU=1,AVAIL=1 where NAME='UpdateContent'", 0, 0, &szError);
                    }
                    else {
                        char* szError=0;
                        sqlite3_exec_p(sql_db, "insert into SettingTable(NAME, INTV, INTU, AVAIL) values ('UpdateContent', 1, 1, 1)", 0, 0, &szError);
                    }

                    sqlite3_close_ptr(sql_db);
                }
                dlclose(mSqliteHandler);
            }
        }

    }


private:
    HEADER              mHeader;
    EXTRACT_FILE_INFO   *mFileInfo;

    vector<EXTRACT_FILE_INFO> mExtracFiles;
};



int main() {
    cout << "start update ..." << endl;
    UpdateInfo update;    

    if (update.RepairConfig()) {
        cout << "repair the config failed" << endl;
        return -1;
    }
    update.SetUpdateStorage();

    if (update.Contruct()) {
        cout << "expact the file failed" << endl;
    }

    update.StartUpdate();

    return 0;
}
