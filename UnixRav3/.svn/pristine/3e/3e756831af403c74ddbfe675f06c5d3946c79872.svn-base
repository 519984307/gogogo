#ifndef UICIRCULARQUEUE_H
#define UICIRCULARQUEUE_H

#include <string>
#include <vector>

#include <QMutexLocker>
#include <QObject>

#include "application.h"

class MainWindow;

using namespace std;

enum UICIRCULAR_REFRESH_TYPE{
    UICIRCULAR_REFRESH_TYPE_STATUS = 0,
    UICIRCULAR_REFRESH_TYPE_LOG,
    UICIRCULAR_REFRESH_TYPE_PROCESS
};
//void RefreshScanLogs(int scantype, CircleLink<std::string> *data);
//void RefreshVirusLogs(const string& file, const string& virus, int/** VirusReportData::VirusStatus */ status);
//void RefreshScanStatus(int scantype, SCAN_STATUS_TYPE status, int progress, void *data=NULL);

class UiCircularQueue:public QObject
{
    friend class MainWindow;
    Q_OBJECT
public:
    UiCircularQueue();
    ~UiCircularQueue();

    void ResetAll();

//    int InitMemory(int bufcount);
    int AddLogData(const int &scantype, const std::string &data);
    int AddVirusLogData(const string &file, const string &virus, const int &status);
    int AddScanStatusData(const int &scantype, const int &status, const int &progress);
    int AddScanStatusThreadData(const int &status, const std::string &file, const int &count, const int &tag);
    int GetLogData(int &count, int &scantype, std::string &data);
    int GetVirusLogData(int &count, std::string &file, std::string &virus, int &status);
    int GetScanStatusDataCount();
    int GetScanStatusData(int &scantype, int &status, int &progress);
    int GetScanStatusThreadData(std::string &file, int &filecount, int &tag);

signals:
    void sigLogData();
    void sigVirusLogData();
    void sigScanStatusData();

public:
    struct T_LogData{
        int scantype;
        std::string file;
    };
    struct T_VirusLogData{
        std::string file;
        std::string virus;
        int status;
    };
    class T_ScanStatusData{
    public:
        int scantype;
        int status;
        int progress;
        std::string file;
        int count;
        int tag;
        //std::vector<struct ReportCache> vcaches;
    };

protected:
    int m_nTotalFileCount;
    vector<T_LogData*> m_vLogDatas;
    vector<T_VirusLogData*> m_vVirusLogDatas;
    vector<T_ScanStatusData*> m_vScanStatusDatas;

private:
    QMutex  m_Mutex;
};

#endif // UICIRCULARQUEUE_H
