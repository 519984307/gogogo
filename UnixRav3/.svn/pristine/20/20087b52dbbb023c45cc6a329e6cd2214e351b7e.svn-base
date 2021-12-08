#ifndef MODEL_DEFINE_H
#define MODEL_DEFINE_H

typedef enum {
    COMMAND_UNKNOWN             =   -1,
    COMMAND_REQUEST_AUTH        =   1001,
    COMMAND_REPORT_CLI_INFO     =   2001,
    COMMAND_REPORT_LOG          =   2002,
    COMMAND_REPORT_UNINSTALL    =   2005,
    COMMAND_POLICY              =   2003,
    COMMAND_REPORT_FILE         =   2006,
    COMMAND_COMMAND             =   3001,
}Command;

typedef enum {
    INVALID             =   -1,
    NOTIFICATION        =   0,
    REPORT_LOCATION     =   1
}CommandType;

typedef enum {
    /** 系统不可用 */
    SYSTEM_IS_UNUSABLE      =   0,
    /** 需要立即被修改的条件 */
    ACTION_TAKEN_IMMEDIATELY=   1,
    /** 阻止某些工具或子系统功能实现的错误条件 */
    CRITICAL_CONDITIONS     =   2,
    /** 阻止某些工具或子系统部分功能实现的错误条件 */
    ERROR_CONDITIONS        =   3,
    /** 预警信息 */
    WARNING_CONDITIONS      =   4,
    /** 具有重要性的普通条件 */
    SIGNIFICANT_CONDITION   =   5,
    /** 提供信息的消息 */
    INFORMATIONAL_MESSAGE   =   6,
    /** 调试消息 */
    DEBUG_MESSAGE           =   7,
}EventLevel;

typedef enum {
    EVENTLOG_SUCCESS        =   0x0000,
    EVENTLOG_AUDIT_FAILURE  =   0x0010,
    EVENTLOG_AUDIT_SUCCESS  =   0x0008,
    EVENTLOG_ERROR_TYPE     =   0x0001,
    EVENTLOG_INFORMATION_TYPE=  0x0004,
    EVENTLOG_WARNING_TYPE   =   0x0002
}EventType;

typedef enum {
    /** 0代表追加型表 */
    APPEND      =   0,
    /** 1代表覆盖型表 */
    OVERRIDE    =   1,
    /** 2代表更新追加型表 */
    UPDATE_APPEND = 2
}LogDealType;

/** log type */
static const char *LogTypeContent[] = {
    "EpState",
    "AND_Phone_SpamMsg",
    "AND_Phone_SpamPhone",
    "AND_ScanEvent",
    "AND_Virus",
    "AND_Loc",
    "XAV_ScanEvent",
    "XAV_Virus",
    "CmdRunStateLog",
    "rua_log"
};

typedef enum {
    /** 基础平台日志 */
    LOG_TYPE_BASIC_PLATFORM,
    /** 举报短信 */
    LOG_TYPE_SPAM_MESSAGE,
    /** 举报电话 */
    LOG_TYPE_SPAM_PHONE,
    /** 一键扫描日志 */
    LOG_TYPE_VIRUS_SCAN,
    /** 病毒日志 */
    LOG_TYPE_VIRUS_RECORD,
    /** 定位上报 */
    LOG_TYPE_LOC_REPORT,
    /** 扫描日志　*/
    LOG_TYPE_SCAN_EVENT,
    /** PC病毒日志　*/
    LOG_PC_VIRUS_EVENT,
    /** 命令上报　*/
    LOG_PC_COMMAND_EVENT,
    /** 布署日志 */
    LOG_RUA_EVENT,
}LogType;

/** logtype end */

/** content type */
static const char *ContentTypeDetail[] = {
    "unknown",
    "computerinfo",
    "moduleinfo",
    "reportcontent",
    "policycontent",
    "cmdcontent",
    "uploadfile"
};

typedef enum {
    UNKNOWN,
    CLIENT,
    MODULE,
    LOG,
    POLICY,
    COMMAND,
    UPLOAD,
}ContentType;

typedef enum {
    RUA_ACTION_UNKNOWN = 0,
    RUA_ACTION_INSTALL = 1,
    RUA_ACTION_UPDATE,
    RUA_ACTION_UNINSTALL,
    RUA_ACTION_REPAIR,
}LogRuaActionType;

typedef enum {
    RUA_SOURCE_UNKNOWN      = 0,
    RUA_SOURCE_MANUAL_START = 1, //手动 : 手动升级；下载器安装；安装包都总结为手动运行
    RUA_SOURCE_TIMER_START,         //定时
    RUA_SOURCE_REMOTE_START,        //远程
    RUA_SOURCE_SYSTEM_START,  //系统

}LogRuaSourceType;

typedef enum {
    RUA_ROLE_REC = 1,  //平台
    RUA_ROLE_PLATFORM = 1,    //平台
    RUA_ROLE_VIRLIB,
}LogRuaRoleType;

/** content end */

/** map the enum value to string */
#define ENUM_VALUE(a, b) (a)[static_cast<int>(b)]


#endif // MODEL_DEFINE_H
