#ifndef BASECONFIG_H
#define BASECONFIG_H

#include <stdio.h>
#include "../config.h"


typedef enum {
    HEALTHY             = 0,
    CENTER_DISABLED     = 0x1,
    DEFEND_DISABLED     = 0x2,
    NETFILTER_DISABLED  = 0x4,
    BROWSER_DISABLED    = 0x8,
    PUBLIC_INTERFACE_DISABLED = 0x10,
    PLUGIN_DISABLED     = 0x20,
    REMOTE_COMMAND_DISABLED = 0x40,
}HealthStatus;

#ifdef RAV_LOG
#define LOG_SEC(...) \
    do { \
        Utils::LOG::PrintLog(4, __VA_ARGS__); \
    } while(0);

#define LOG_GJCZ(...) \
    do { \
        if (Utils::Path::FileAvailed("/opt/BDFZ/RAV/plugins/secret")) { \
            Utils::LOG::PrintLog(4, __VA_ARGS__); \
        } else { \
            Utils::LOG::PrintLog(0, __VA_ARGS__); \
        } \
    } while(0);

#define LOG_GJCZ_DEBUG(...) \
    do { \
        if (Utils::Path::FileAvailed("/tmp/rsdebug")) { \
            Utils::LOG::PrintLog(0, __VA_ARGS__); \
        } \
    } while(0);

#define LOG_GLTX(...) \
    do { \
        if (Utils::Path::FileAvailed("/opt/BDFZ/RAV/plugins/secret")) { \
            Utils::LOG::PrintLog(4, __VA_ARGS__); \
        } else { \
            Utils::LOG::PrintLog(1, __VA_ARGS__); \
        } \
    } while(0);

#define LOG_ZDFY(...) \
    do { \
        if (Utils::Path::FileAvailed("/opt/BDFZ/RAV/plugins/secret")) { \
            Utils::LOG::PrintLog(4, __VA_ARGS__); \
        } else { \
            Utils::LOG::PrintLog(2, __VA_ARGS__); \
        } \
    } while(0);

#define LOG_RJJH(...) \
    do { \
        if (Utils::Path::FileAvailed("/opt/BDFZ/RAV/plugins/secret")) { \
            Utils::LOG::PrintLog(4, __VA_ARGS__); \
        } else { \
            Utils::LOG::PrintLog(3, __VA_ARGS__); \
        } \
    } while(0);
#else
#define LOG_GJCZ
#define LOG_GLTX
#define LOG_ZDFY
#define LOG_RJJH
#define LOG_SEC
#define LOG_GJCZ_DEBUG
#endif

//#define PRODUCT 104
//#if defined(PRODUCT)
//#if (PRODUCT==100)
//    #define CHILD_PRODUCTID "479AF07B-BCE0-47d0-8BFA-C44ED660BEB7"  /** LGSERVER tongyong*/
//#elif (PRODUCT==101)
//    #define CHILD_PRODUCTID "80F9BF84-85CF-4f6f-A22F-2AFDE6DD36EF"  /** LGDESKTOP */
//#elif (PRODUCT==102)
//    #define CHILD_PRODUCTID "90DDA51B-33AB-4d94-8F04-0B980AD75CDE"  /** LCSERVER guochan*/
//#elif (PRODUCT==103)
//    #define CHILD_PRODUCTID "67EB9C3C-F9E7-49f9-BEC0-71BBAD60DB73"  /** LCDESKTOP */
//#elif (PRODUCT==104)
//    #define CHILD_PRODUCTID "8A8F2A01-07BF-42d3-AA76-2DEF83426170"  /** LPSECRET baomiji*/
//#endif
//#endif


//#define PRODUCT 3
//#if defined(PRODUCT)
//#if (PRODUCT==1)
//    #define CHILD_PRODUCTID "30A1265A-7F60-4f4c-99B9-C15353253864" /*GLIBC27*/
//#elif (PRODUCT==6)
//    #define CHILD_PRODUCTID "A40D11F7-63D2-469d-BC9C-E10EB5EF32DB" /*GLIBC25*/
//#elif (PRODUCT==2)
//    #define CHILD_PRODUCTID "87AFB259-7A18-438b-9A74-719E0CFF8395" /* MIPS */
//#elif (PRODUCT==3)
//    #define CHILD_PRODUCTID "27C2A7BA-3602-4297-A937-CACBCAE7A982" /* LINTELX64 */
//#elif (PRODUCT==4)
//    #define CHILD_PRODUCTID "962CF847-3D72-49d0-895A-16C91858A13D" /* MIPS64 */
//#elif (PRODUCT==5)
//    #define CHILD_PRODUCTID "1AB67467-096C-4bea-B852-2CA73F6E854C" /* AARCH */
//#elif (PRODUCT==7)
//    #define CHILD_PRODUCTID "8D0E85E6-90FF-4684-92A1-E7ADB57AA6E0" /* AIX 5.1 */
//#elif (PRODUCT==8)
//    #define CHILD_PRODUCTID "8D0E85E6-90FF-4684-92A1-E7ADB57AA6E0" /* UAIX52 */
//#elif (PRODUCT==9)
//    #define CHILD_PRODUCTID "296949FC-1AC3-47fc-A29C-DE3D3B1E86AC" /* UAIX6 */
//#elif (PRODUCT==10)
//    #define CHILD_PRODUCTID "76880E95-3B96-4ee5-83BB-0951D038BF52" /* UAIX7 */
//#elif (PRODUCT==11)
//    #define CHILD_PRODUCTID "852B7B1D-71F5-4242-9C05-8D161E50DA61" /* HPUXPARISC */
//#elif (PRODUCT==12)
//    #define CHILD_PRODUCTID "3FA07BE3-3DED-4aee-A015-749EDACFD57C" /* LALPHA */
//#elif (PRODUCT==13)
//    #define CHILD_PRODUCTID "713C7B49-1469-4486-8F76-70CBF2561C72" /* USSPARC */
//#elif (PRODUCT==14)
//    #define CHILD_PRODUCTID "2CA029D5-4505-4a0d-8066-95885095A1E0" /* LK24INTEL */
//#elif (PRODUCT==15)
//    #define CHILD_PRODUCTID "3001DB4D-65DC-4085-9467-A1CAC2A3A0EE" /* US10INTEL */
//#elif (PRODUCT==16)
//    #define CHILD_PRODUCTID "553C2A3A-9FFE-49d9-969D-9C8286B1D184"  /* LK26INTEL */
//#endif
//#endif

/////////////////////////////////////////////////////////////////////////////////////
// 特别声明:
// 以下信息分为三部分
// 1 授权信息
// 2 子产品信息
// 3 策略标志信息
/////////////////////////////////////////////////////////////////////////////////////

// 策略标志信息
#define ANTIVIRUS_PRODUCTID "D49170C0-B076-4795-B079-0F97560485AF"
#define LINUX_RAV_PRODUCTID "A40D11F7-63D2-469d-BC9C-E10EB5EF32DB" // use glibc25's child productid
#define UPDATE_PRODUCTID    "40BAC747-7D02-4969-AF79-45EE47365C81"
#define DEPLOYED_PRODUCTID  "50BAC747-7D02-4969-AF79-45EE47365C81" // rising software deployed product id
#define NETCENTER_PRODUCTID "EB8AFFA5-0710-47E6-8F53-55CAE55E1915"

#define SECRET_MACHINE_ID   "8A8F2A01-07BF-42d3-AA76-2DEF83426170"
#define COMMON_MACHINE_ID   "90DDA51B-33AB-4d94-8F04-0B980AD75CDE"
#define CHINA_MECHINE_ID    "479AF07B-BCE0-47d0-8BFA-C44ED660BEB7"
#define RFW_PRODUCTID       "53246C2F-F2EA-4208-9C6C-8954ECF2FA27"

////////////////////////////////////////////////////////////////////////////////////////
//1.1 Linux_General_Server_XFW(通用服务器版)
//   LGSERVERXFW (这行产品代号)
#define SUB_PRODUCT_LGSERVER_XAV	"479AF07B-BCE0-47d0-8BFA-C44ED660BEB7"
#define SUB_PRODUCT_LGSERVER_XFW	"500E8EF2-2878-4d1c-B045-6874323DD2D0"


////////////////////////////////////////////////////////////////////////////////////////
//1.2 Linux_General_Desktop_XFW 通用桌面版
//   LGDESKTOPXFW
#define SUB_PRODUCT_LGDESKTOP_XAV	"80F9BF84-85CF-4f6f-A22F-2AFDE6DD36EF"
#define SUB_PRODUCT_LGDESKTOP_XFW	"FB0FCC91-8054-4232-A4A9-48F82441115A"


////////////////////////////////////////////////////////////////////////////////////////
//1.3 Linux_China_Server_XFW 国产服务器版
//   LCSERVERXFW
#define SUB_PRODUCT_LCSERVER_XAV	"90DDA51B-33AB-4d94-8F04-0B980AD75CDE"
#define SUB_PRODUCT_LCSERVER_XFW	"41A06057-4AD7-42b8-8977-158D8A0015BD"


////////////////////////////////////////////////////////////////////////////////////////
//1.4 Linux_China_Desktop_XFW 国产桌面版
//   LCDESKTOPXFW
#define SUB_PRODUCT_LCDESKTOP_XAV	"67EB9C3C-F9E7-49f9-BEC0-71BBAD60DB73"
#define SUB_PRODUCT_LCDESKTOP_XFW	"BFFCCB01-958C-43bd-B2C8-DC617BCC0EAF"

////////////////////////////////////////////////////////////////////// //////////////////
//1.5 Linux_Professional_Secret_XFW 保密专用版
//   LPSECRETXFW
#define SUB_PRODUCT_LPSECRET_XAV    "8A8F2A01-07BF-42d3-AA76-2DEF83426170"
#define SUB_PRODUCT_LPSECRET_XFW	"29B2026E-B0CE-427c-A1C2-7D01ECB75F91"

// 授权信息
#define LINUX_GENERAL_SERVER_AUTHORIZATION  "479AF07B-BCE0-47d0-8BFA-C44ED660BEB7"  /** LGSERVER tongyong*/
#define LINUX_GENERAL_DESKTOP_AUTHORIZATION "80F9BF84-85CF-4f6f-A22F-2AFDE6DD36EF"  /** LGDESKTOP */
#define LINUX_CHINA_SERVER_AUTHORIZATION    "90DDA51B-33AB-4d94-8F04-0B980AD75CDE"  /** LCSERVER guochan*/
#define LINUX_CHINA_DESKTOP_AUTHORIZATION   "67EB9C3C-F9E7-49f9-BEC0-71BBAD60DB73"  /** LCDESKTOP */
#define LINUX_SECRET_AUTHORIZATION          "8A8F2A01-07BF-42d3-AA76-2DEF83426170"  /** LPSECRET baomiji*/



#define RAV_CONFIG_FILE     ".RavCfg"
#define RAV_DEFAULT_PATH    "/opt/BDFZ/RAV"
//#define RAV_DEFAULT_PATH    "/home/miwoo/workspace/esm_dev/linux/install"

#define CENTER_SOC_URL      "/Api/Api.php"
#define CENTER_ESM_URL      "/ESM/Client/ClientRequest.aspx"


/** define if start the process defence */
#define ACTIVE_DEFEND_PROCESS   1

/** for security offical, define the product flag */
#define PRODUCT_FLAG                "RAV"

#define NETLINK_ACTIVE_DEFENCE_PROTOCOL     29
#define NETLINK_NETFILTER_DEFENCE_PROTOCOL  30

#define PUBLIC_API_LOCAL_SOCKET     "/var/av/ipc/scanfiles"


#define GLTX_RJJH_SOCKET                "gltx2rjjh.sock"
#define GLTX_GGFW_SOCKET                "gltx2ggfw.sock"
#define GGFW_GJCZ_SOCKET                "ggfw2gjcz.sock"
#define GGFW_GLTX_SOCKET                "ggfw2gltx.sock"
#define GGFW_ZDFY_SOCKET                "ggfw2zdfy.sock"
#define GGFW_RJJH_SOCKET                "ggfw2rjjh.sock"
#define GJCZ_RJJH_SOCKET                "gjcz2rjjh.sock"
#define GJCZ_GGFW_SOCKET                "gjcz2ggfw.sock"
#define ZDFY_RJJH_SOCKET                "zdfy2rjjh.sock"
#define ZDFY_GGFW_SOCKET                "zdfy2ggfw.sock"
#define RJJH_GGFW_SOCKET                "rjjh2ggfw.sock"
#define RJJH_MAJOR_SOCKET               "rjjh%d_major.sock"
#define RJJH_NOTIFY_SOCKET              "rjjh%d_notify.sock"
#define RJJH_SETTING_SOCKET             "rjjh%d_setting.sock"
#define RJJH_LOGVIEW_SOCKET             "rjjh%d_logview.sock"
#define RJJH_CONSOLE_SOCKET             "rjjh%d_console.sock"




#define NOTIFY_DBUS_NAME    "com.RAV.BDFZ0"
#define CONTEXT_MENU_DBUS   "com.RAV.BDFZ1"
#define DAEMON_DBUS_NAME    "com.RAV.BDFZ2"
#define MAJOR_SCREEN_NAME   "com.RAV.BDFZ3"
#define SETTING_SCREEN_NAME "com.RAV.BDFZ4"
#define LOG_SCREEN_NAME     "com.RAV.BDFZ5"


#define QUARANTINE_PATH    ".quarzone"
#define PLUGINS_PATH       "plugins"

/** define the virus engine library path */
#define SCAN_ENGINE_LIB_PATH        "lib"
#define SCAN_CACHE_PATH             "cache"
#define VIRUS_DB_FILE               "malware.rmd"

/** define the storage db path */
#define STORAGE_DB_PATH     "RJJHGJCZ"
#define STORAGE_GLTX_PATH   "GLTXetc"
#define FOLDER_RJJH_GLTX    "RJJHGLTX"
#define FOLDER_GGFW_GJCZ    "GGFWGJCZ"
#define FOLDER_GGFW_RJJH    "GGFWRJJH"
#define FOLDER_GGFW_GLTX    "GGFWGLTX"
#define FOLDER_GGFW_ZDFY    "GGFWZDFY"
#define FOLDER_RJJH_GJCZ    "RJJHGJCZ"
#define FOLDER_RJJH_ZDFY    "RJJHZDFY"
#define FOLDER_GJCZ_ETC     "GJCZetc"
#define FOLDER_RJJH_ETC     "RJJHetc"
#define FOLDER_GGFW_ETC     "GGFW"

//#define LOCK_GJCZ_FILE      "GJCZ.lock"
//#define LOCK_RJJH_NOTIFY    "RJJH0_%d.lock"
//#define LOCK_RJJH_MAIN      "RJJH2_%d.lock"
//#define LOCK_RJJH_SETTING   "RJJH4_%d.lock"
//#define LOCK_RJJH_LOGS      "RJJH6_%d.lock"

#define STORAGE_DB_FILE     "rav.db"

#define BROWSER_WEB_DOMAIN  "RJJHZDFY"

#define BROWSER_IPC_PORT    8686
#define BROWSER_IPC_CONF_PATH   "block"

#define NOTIFY_AVAILABLE

#define DEPLOYED_BIN_FOLDER     "bin"
#define DEPLOYED_LIB_PATH       "lib"
#define DEPLOYED_LOG_PATH       "RJJHGJCZ"
#define DEPLOYED_MODULE_SHELL   "insmod.sh"


#define DAEMON_GJCZ_COMMAND      "RAVGJCZ1"
#define DAEMON_GLTX_COMMAND      "RAVGLTX1"
#define DAEMON_ZDFY_COMMAND      "RAVZDFY1"
#define NOTIFY_RJJH_COMMAND      "RAVRJJH1"

#define FANOTIFYEXEC_BIN      "FanotifyExec.bin"

#define START_KO_SHELL          "RAVGJCZ9"
#define STOP_ALL_SHELL          "RAVGJCZ8"
#define START_RAV_SHELL          "ravloader.sh start"
#define STOP_RAV_SHELL           "ravloader.sh stop"

#define MAIN_SCREEN_COMMAN          "RAVRJJH3"
#define MAIN_SETTING_COMMAND        "RAVRJJH5"
#define MAIN_LOGVIEW_COMMAND        "RAVRJJH7"
#define NOTIFYY_COMMAND             "RAVRJJH9"

#define CONFIG_DEF_FILE             "default.ini"
#define CONFIG_DEF_POLICY           "policy"

#define SDCARD_MOUNT_PATH           "sdcard"

#define UPDATE_TEMP_FOLDER      "tmpcenter"
#define UPDATE_BACKUP_FOLDER    "backup"
#define UPDATE_VIRUS_FOLDER     "tmpvirus"
#define CLIENT_NEW_LIST_FILE    "versionnew.list"
#define CLIENT_MD5_LIST_FILE    "clientmd5.list.zip"

#define NOTIFY_CONFIRM_SELECT   2
#define NOTIFY_IGNORE_SELECT    1

#define AUTOSTART_FILE_NAME     "ravbdfz.desktop"
#define APPLICATION_FILE_NAME   "RAVbdfz.desktop"
#define DBUS_CONFIG_FILE_NAME   "com.RAV.BDFZ0.conf"

#define SYSTEMD_RAV_SERVICE     "RAVBDFZ.service"
#define SYSTEMD_SERVICE_0       "RAVBDFZ0.service"
#define SYSTEMD_SERVICE_1       "RAVBDFZ1.service"
#define SYSTEMD_SERVICE_2       "RAVBDFZ2.service"

#define SETUP_PACK_FOLDER       "setup"
#define COMMON_PACK_FOLDER      "common"
#define CONFIG_PACK_FILE_NAME   "config.rpk"
#define COMMON_PACK_FILE_NAME   "common.rpk"
#define EP_PACK_FILE_NAME       "ep.rpk"
#define XAV_PACK_FILE_NAME      "xav.rpk"
#define XFW_PACK_FILE_NAME      "xfw.rpk"
#define SETUP_PRODUCE_XML       "product.xml"
#define SETUP_CONFIG_FILE       "antivirus.cfg"

#define FILE_DEFEN_MODULE   "ravdefen"
#define NET_FILTER_MODULE   "ravfilter"

#define QT_WIDGET_SUPPORT_LIB   "libravqtsupport.so"

/** process heartbeat interval */
#define COMMUNICATE_HEART_INTERVAL  5

#define RJJH_BROAD_ADDR  "127.0.0.1"
#define RJJH_BROAD_PORT  8848

#define SOCKET_BUFFER_MAX   2048

#define CROSS_VERSION_LOG_FILE  "cross.xml"


typedef enum {
    SETUP_PREPARE_DATA_ERROR    = -10001,
    SETUP_USER_ABORT,
    SETUP_USER_DISAGREE,
    SETUP_OVERIDE_ABORT,
    SETUP_CREATE_FOLDER_ERROR,
    SETUP_CENTER_ADDR_ERROR,
    SETUP_UNPACK_ERROR,
    SETUP_MAKE_KO_SHELL_ERROR,
    SETUP_MAKE_BIN_SHELL_ERROR,
    SETUP_GENERAL_CONFIG_ERROR,
    SETUP_MAKE_INSMOD_ERROR,
    SETUP_MAKE_LOADER_ERROR,
    SETUP_MAKE_SYSTEMD_ERROR,
    SETUP_MAKE_AUTOSTART_ERROR,
    SETUP_MAKE_DESKTOP_ERROR,
    SETUP_CHECK_SERIAL_ERROR,
    SETUP_OVERRIDE_VERSION_TOO_LOW,

}ERROR_CODE;

#endif // BASECONFIG_H
