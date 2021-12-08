#ifndef CROSS_H
#define CROSS_H

#include <string>
#include <vector>

using namespace std;
#undef LOG_INFO

class CrossVersion {
public:
    CrossVersion();

    typedef struct LOG_INFO {
        string info1;
        string info2;
        string info3;
        string info4;
        string info5;
        string info6;
    } LOG_INFO;

public:
    int InitFuncPtr();
    bool Exist();
    string  GetSerial();
    string GetVersion();
    int GetSGUID(string& guid, string& sguid);

    /**
     * @brief GetCenterAddr
     * @param addr out params
     * @return SoftType, define in setup.h
     */
    int  GetCenterAddr(string& type, string &addr);

    int ImportLogs(const string& outxml, string& version, string& guid, string& sguid, vector<LOG_INFO>& scanlogs, vector<LOG_INFO>& viruslogs);
    int ExportLogs(const string& outxml);
    int UninstallOldVersion();

private:
    int ExportScanLog(void *sqlptr, void *xmlptr);
    int ExportVirusLog(void *sqlptr, void *xmlptr);
    int ExportQuarantine(void *sqlptr, void *xmlptr);

    int Uninstall();

    int CleanCrontab();
    int DelSudoScript();
    int DisableAutostart();
    int DeleteDesktopFile();

public:
    string  mOldInstallPath;
	
protected:
    void *mSqliteHandler;
    void *mXmlHandler;

protected:
    struct STSNINFO_UNIX {
        char szSN[128];
        char szInstallTime[32];
        char szEndTime[32];
    };
};

#endif // CROSS_H
