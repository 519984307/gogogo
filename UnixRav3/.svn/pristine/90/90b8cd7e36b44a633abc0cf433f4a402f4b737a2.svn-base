#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "communicator_global.h"
#include "models/log/logdatascanevent.h"
#include "net/apiservice.h"

class APIService;

class CommunicatorObserver {
public:
    typedef enum {
        COMM_ERROR = -1,
        COMM_INIT,
        COMM_AUTH,
        COMM_REMOTE
    }COMM_EVENT;
    typedef enum {
        NOT_CONNECT,
        INFO_ERROR,
        HEAD_ERROR,
        GENERAL_SGUID,
        TESPAN_CHANGED,
        DATE_CHANGED,
        EID_CHANGED,
        NET_RECONNECT,
        INFO_REPORTED,
        AUTHORIZED,
        UNAUTHORIZE,
        POLICY,
        COMMAND
    }COMM_TYPE;
    virtual void onCommunicateCallback(COMM_EVENT event, COMM_TYPE type, void *object) = 0;

    typedef enum {
        CLIENT_NAME = 0x0,
        CLIENT_ARCH,
        CLIENT_IP,
        CLIENT_ISVM,
        CLIENT_MAC,
        CLIENT_OS,
        CLIENT_ROOT,
        CLIENT_MEM,
        CLIENT_DISK,
        CLIENT_USER,
        CLIENT_VER,
        CLIENT_SYSTEM,
        CLIENT_VLIB_VERSION,
        CLIENT_PRODUCT_TYPE,
        CLIENT_SUB_COMPONENTS,
    }ClientInfoType;
    virtual int requestClientInfo(ClientInfoType type, string& data) = 0;
};

class Communicator : public APICallback
{
public:
    static Communicator* NEW(const string& url,
                             const string& eid,
                             const string& guid,
                             const string& sguid,
                             const string& version,
                             const string& systype);
    virtual ~Communicator();

    void setObserver(CommunicatorObserver *observer) {
        mObserver = observer;
    }

    int Start();
    int Stop();

    string GUID() const {
        return mGUID;
    }

    int ReportVlibVersion(const string& version);
    int ReportDefenState(const string& name, bool state);

private:
    Communicator();
    int Construct(const string& url,
                  const string& eid,
                  const string& guid,
                  const string& sguid,
                  const string& version,
                  const string& systype);



private:
    int onRequestCallback(CALLBACK_EVENT event, CALLBACK_TYPE TYPE, const string& data);

public:
    /**
     * @brief ReportScanLog
     * @param logtype : LogDataScanEvent::LogScanType
     * @param starttime
     * @param runtime
     * @param scancount
     * @param viruscount
     * @param treatedcount
     * @param totalfilecount
     * @param scanfilecount
     * @return
     */
    int ReportScanLog(LogDataScanEvent::LogScanType logtype,
                      LogDataScanEvent::CommandSender sender,
                      string starttime,
                      long taskid,
                      int runtime,
                      int scancount,
                      int viruscount,
                      int treatedcount,
                      int totalfilecount,
                      int scanfilecount,
                      LogDataScanEvent::LogScanState state);

    /**
     * @brief ReportVirusLog
     * @param logtype : LogDataScanEvent::LogScanType
     * @param foundtime
     * @param filepath
     * @param virusid
     * @param virusname
     * @param deal
     * @param state
     * @return
     */
    int ReportVirusLog(LogDataScanEvent::LogScanType logtype,
                       LogDataScanEvent::CommandSender sender,
                       long foundtime,
                       const std::string& filepath,
                       int virusid,
                       const std::string& virusname,
                       int deal,
                       int state,
                       const std::string& hash);

    int ReportCommandLog(const string& productid, const std::string& action, const std::string& cmdid, int type, int result);

    int ReportClientInfo();

    int ReportRUALog(LogRuaSourceType source,
                     LogRuaActionType action,
                     LogRuaRoleType role,
                     const std::string& oldver,
                     const std::string& newver,
                     bool needreboot);

    int ReportFile(const string& file);

private:
    int  HeartBeat();
    int  UploadClientInfo(vector<pair<string,string> >& products);
    int  ReportEPState(const string& state, const string& value,  LogDealType dealtype = APPEND);
    int  RequestAuthorization();

    inline unsigned int HeartBeatInterval() const;

public:
    int  ReportUninstall();

private:
    static void* WorkThread(void*);
    static void* Authrization(void*);

public:
    int Work();
    int Auth();

private:
    APIService              *mAPIService;

    CommunicatorObserver    *mObserver;

    string mUrl;
    string mEid;
    string mGUID;
    string mSGUID;
    string mVersion;
    string mSystem;
    string mProductGUID;

    // save the auth day
    int mAuthDay;

    pthread_mutex_t mRequestMutex;

    pthread_t   mAuthThread;
    pthread_t   mWorkThread;

    unsigned int mTespan;
    int mAuthTespan;
    bool mAuthState;

    int mState;

    /** connect status */
    int mConnectStatus;

    /** connect error, hight 16 is error code, low 16 is retry count */
    int mConnectError;
};

#endif // COMMUNICATOR_H
