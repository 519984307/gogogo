#ifndef COMMUNICATEBUNDLE_H
#define COMMUNICATEBUNDLE_H

#include <string>
#include "rserializer.h"

class CommunicateBundle : public RSerializer
{
public:
    CommunicateBundle() {
        mFlag = TOAST;
        mType = NOTIFY_VIRUS;
        mSelected = 0;
    }

    bool Internalize(const char *data, int len);
    int  Externalize(char **data, int *len);
    std::string Externalize();
    bool Externalize(int fd);
    int  CalculateSize();
public:
    enum _CommunicateType {
        CONFIRM = 0,
        TOAST,
        QUERY,
        INFO,
        ISEEYOU = 999,
    };
    typedef enum _CommunicateType CommunicateType;

    typedef enum _NotifyType {
        NOTIFY_VIRUS = 0,
        NOTIFY_SCANING,
        NOTIFY_FIREWALL,
        NOTIFY_USB_PLUGIN,
        NOTIFY_AUTH,
        NOTIFY_MESSAGE,
        NOTIFY_EIXT,
        NOTIFY_SCAN_FINISHED,
        NOTIFY_UPDATE_FINISHED,
    }NotifyType;

    CommunicateType mFlag;
    NotifyType  mType;
    std::string mTitle;
    std::string mContent;
    std::string mButtonTitle1;
    std::string mButtonTitle2;
    // for out
    int mSelected;

public:
    void operator=(const CommunicateBundle& bundle) {
        mFlag = bundle.mFlag;
        mType = bundle.mType;
        mTitle = bundle.mTitle;
        mContent = bundle.mContent;
        mButtonTitle1 = bundle.mButtonTitle1;
        mButtonTitle2 = bundle.mButtonTitle2;
    }

private:
    std::string mTempBuffer;
};


class CommandBundle : public RSerializer {
public:
    enum CommandType {
        QUERY_HEALTH    = 100,
        REPAIR,
        RELOAD_CONFIG,
        CONTEXT_SCAN,   // fixed 103
        BROWSER_BLOCK,  // FIXED 104
        RESET_CONFIG,
        DAEMON_CENTER_UPDATE,
        DAEMON_OFFLINE_UPDATE,
        DAEMON_EXIT,
        QEURY_PATH_INFO,
        CENTERLESS_REGISTER,

        NOTIFY_READY,
        STORAGE_REQ,
        POLICY_CHANGED,
        UNINSTALL_REPORT,
        UNINSTALL_PROTECT,
        UNINSTALL_VERIFY,
        NOTIFY_EVENT,

        // report scan event
        SCAN_EVENT,
        QUARANTINE_EVENT,

        //for gltx
        REMOTE_COMMAND,
        FIND_PROXY_CLIENT, 

		//update state.
		UPDATE_FINISH_NOTIFY, 
    };

    enum CommandExtendType {
        // GJCZ TYPE
        STORE_EVENT_LOG = 1000,
        DELETE_QUARANTINE_LOG,
        GET_USB_PLUGIN_STATE,
        SET_USB_PLUGIN_STATE,
        GET_SCAN_CONFIG,
        GET_SCAN_WHITE_LIST,
        GET_SCAN_FILE_TYPE,
        APPEND_SCAN_LOG,
        APPEND_VIRUS_LOG,
        APPEND_QUARANTINE_LOG,
        RESTORE_QUARANTINE_LOG,
        TRUST_QUARANTINE_LOG,

        QUICK_SCAN,
        FULL_SCAN,
        CUSTOM_SCAN,
        PAUSE_SCAN,
        CONTINUE_SCAN,
        STOP_SCAN,
        SCHEDULE_SCAN,
        INTELLIGENT_SCAN,
        UPGRADE_DOWNLOAD_RP,
        UPGRADE_COMBINE_RP,
        UPGRADE_VLIB_SUCCESS,
        UPGRADE_APP_FILE,
        REFRESH_SCAN_LOGS,
        REFRESH_VIRUS_LOGS,
        PREPARE_SCAN_SUCCESS,
        REFRESH_SCAN_STATUS,
        SCAN_ALL_FINISHED,
        QUERY_VIRUS_RESULT,
        NOTIFY_TRAY_COMPONENT,
        UPDATE_OFFLINE_VIRLIB,
        UPDATE_OFFLINE_VIRLIB_SUCCESS,
        UPDATE_OFFLINE_VIRLIB_ERROR,

        // policy type
        POLICY_LINUX_CHANGED = 2000,
        POLICY_GLOBAL_CHANGED,
        POLICY_DEPLOY_CHANGED,
        POLICY_CENTER_CHANGED,
        POLICY_FIREWALL_CHANGED,
        REMOTE_COMMAND_START,
        REMOTE_COMMAND_FINISHED,
        REMOTE_COMMAND_SCANLOG,
        REMOTE_COMMAND_VIRUSLOG,
        REMOTE_COMMAND_HIDETRAY,
        REMOTE_COMMAND_CLOUD_POST,
        REMOTE_COMMAND_CLOUD_CHECK,

        REMOTE_COMMAND_DEFEN,
        REMOTE_COMMAND_UPDATE,
        REMOTE_UPDATE_VIRUS_DOWNLAOD,
        REMOTE_UPDATE_VIRUS_COMBINE,
        REMOTE_UPDATE_VIRUS_SUCCESS,
        REMOTE_UPDATE_VIRUS_FAILED,
        REMOTE_DEFEN_STATE_REPORT,
        REMOTE_UPDATE_HTTP_DOWNLOAD,

        // file watcher
        DEFEN_FILE_CHANGED = 3000,
        DEFEN_PROC_STARTED,

        // deploy success
        DEPLOY_CROSS_SUCCESS,
        RESET_TRAY_PROCESS,
        RESET_TRAY_PROCESS_SILENCE,
        SHUTDOWN_DAEMON_TRAY,

        // single centerless regiter,
        CHECK_CLIENT_REGISTER = 4000,
        REGISTER_USE_SERIAL,
    };



public:
    int mCommandID;
    int mIntegerValue;
    std::string mStringValue;

    /** 这个值不计入发送的序列，是为转发时，临时变量记录转发的超时时间设置 */
    int mResendOvertime;

public:
    CommandBundle() {
        Reset();
    }

    void Reset() {
        mCommandID = 0;
        mIntegerValue = 0;
        mStringValue.clear();
        mResendOvertime = 0;
    }

public:
    bool Internalize(const char *data, int len);
    int  Externalize(char **data, int *len);
    bool Externalize(int fd);
    int  CalculateSize();
};


#endif // COMMUNICATEBUNDLE_H
