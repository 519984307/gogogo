#include "mainwindow.h"
#include "uicircularqueue.h"

UiCircularQueue::UiCircularQueue():m_nTotalFileCount(0)
{

}

UiCircularQueue::~UiCircularQueue()
{

}

void UiCircularQueue::ResetAll()
{
    QMutexLocker locker(&m_Mutex);
    m_nTotalFileCount = 0;
    int i, size = m_vLogDatas.size();
    for(i = 0; i < size; i++){
        T_LogData *data = m_vLogDatas.at(i);
        delete data;
    }
    m_vLogDatas.clear();
    size = m_vVirusLogDatas.size();
    for(i = 0; i < size; i++){
        T_VirusLogData *data = m_vVirusLogDatas.at(i);
        delete data;
    }
    m_vVirusLogDatas.clear();
    size = m_vScanStatusDatas.size();
    for(i = 0; i < size; i++){
        T_ScanStatusData *data = m_vScanStatusDatas.at(i);
        delete data;
    }
    m_vScanStatusDatas.clear();
}

//int UiCircularQueue::InitMemory(int bufcount)
//{
//    return 0;
//}

int UiCircularQueue::AddLogData(const int &scantype, const std::string &data)
{
    QMutexLocker locker(&m_Mutex);
    struct T_LogData *pdata = new T_LogData;
    pdata->scantype = scantype;
    pdata->file = data;
    m_vLogDatas.push_back(pdata);
    return 0;
}

int UiCircularQueue::AddVirusLogData(const string &file, const string &virus, const int &status)
{
    QMutexLocker locker(&m_Mutex);
    struct T_VirusLogData *pdata = new T_VirusLogData;
    pdata->file = file;
    pdata->virus = virus;
    pdata->status = status;
    m_vVirusLogDatas.push_back(pdata);
    return 0;
}

int UiCircularQueue::AddScanStatusData(const int &scantype, const int &status, const int &progress)
{
    QMutexLocker locker(&m_Mutex);
    struct T_ScanStatusData *pdata = new T_ScanStatusData;
    pdata->scantype = scantype;
    pdata->status = status;
    pdata->progress = progress;
    m_vScanStatusDatas.push_back(pdata);
    return 0;
}

int UiCircularQueue::AddScanStatusThreadData(const int &status, const std::string &file, const int &count, const int &tag)
{
    QMutexLocker locker(&m_Mutex);
    struct T_ScanStatusData *pdata = new T_ScanStatusData;
    pdata->status = status;
    pdata->file = file;
    pdata->count = count;
    pdata->tag = tag;
    m_vScanStatusDatas.push_back(pdata);
    return 0;
}

int UiCircularQueue::GetLogData(int &count, int &scantype, std::string &data)
{
    QMutexLocker locker(&m_Mutex);
    count = 0;
    if(m_vLogDatas.size() == 0)
        return -1;
    struct T_LogData *pdata = m_vLogDatas.front();
    scantype = pdata->scantype;
    data = pdata->file;
    m_vLogDatas.erase(m_vLogDatas.begin());
    delete pdata;
    count = m_vLogDatas.size();
    return 0;
}

int UiCircularQueue::GetVirusLogData(int &count, std::string &file, std::string &virus, int &status)
{
    QMutexLocker locker(&m_Mutex);
    count = 0;
    if(m_vVirusLogDatas.size() == 0)
        return -1;
    struct T_VirusLogData *pdata = m_vVirusLogDatas.front();
    file = pdata->file;
    virus = pdata->virus;
    status = pdata->status;
    m_vVirusLogDatas.erase(m_vVirusLogDatas.begin());
    delete pdata;
    count = m_vVirusLogDatas.size();
    return 0;
}

int UiCircularQueue::GetScanStatusDataCount()
{
    QMutexLocker locker(&m_Mutex);
    return m_vScanStatusDatas.size();
}

int UiCircularQueue::GetScanStatusData(int &scantype, int &status, int &progress)
{
    QMutexLocker locker(&m_Mutex);
    if(m_vScanStatusDatas.size() == 0)
        return -1;
    struct T_ScanStatusData *pdata = m_vScanStatusDatas.front();
    if(pdata->status == Application::THREAD_STATUS)
        return -1;
    scantype = pdata->scantype;
    status = pdata->status;
    progress = pdata->progress;
    m_vScanStatusDatas.erase(m_vScanStatusDatas.begin());
    delete pdata;
    return 0;
}

int UiCircularQueue::GetScanStatusThreadData(std::string &file, int &filecount, int &tag)
{
    QMutexLocker locker(&m_Mutex);
    if(m_vScanStatusDatas.size() == 0)
        return -1;
    struct T_ScanStatusData *pdata = m_vScanStatusDatas.front();
    if(pdata->status != Application::THREAD_STATUS)
        return -1;
    file = pdata->file;
    filecount = pdata->count;
    tag = pdata->tag;
    m_vScanStatusDatas.erase(m_vScanStatusDatas.begin());
    delete pdata;
    return 0;
}
