#ifndef STORAGE_GLOBAL_H
#define STORAGE_GLOBAL_H

#define LOG_PAGE_COUNT  50

#define SGUID_TAG   "sguid"
#define GUID_TAG    "guid"

#define THREAD_SCAN_COMMAND "ThreadScanCommand"
#define KILL_VIRUS_STRONGER "StrongerKill"
#define ENV_INTELLIGENT     "EnvJudge"
#define VIRUS_TRACE         "VirusTrace"
#define VIRUS_LOG_FIELD     "VirusLoged"
#define RESCAN_ACCELERATE   "RescanAccelerate"
#define FILE_ACCELERATE     "FileAccelerate"
#define PUBLIC_CLOUD_STATE  "PublicCloud"
#define PRIVATE_CLOUD_STATE "PrivateCloud"
#define SCAN_COMMON_TYPE    "ScanCommonType"
#define SCAN_CPU_RADIO      "ScanCPURadio"

#define CPU_RADIO_ITEM      "CPURadio"
#define CONNECT_RATE_ITEM   "ConnectRate"
#define OFFICAL_UPDATE_STATE_ITEM   "OfficalUpdateState"
#define OFFICAL_UPDATE_ADDR_ITEM    "OfficalUpdateAddr"
#define OFFICAL_UPDATE_PORT_ITEM    "OfficalUpdatePort"
#define OFFICAL_UPDATE_MODE_ITEM    "OfficalUpdateMode"
#define OFFICAL_UPDATE_COUNT_ITEM   "OfficalUpdateCount"
#define OFFICAL_UPDATE_NAME_ITEM    "OfficalSourceName"


#define SCAN_VIRUS_ACTION   "VirusAction"
#define CLEAN_FAIL_ACTION   "CleanFailAction"
#define BACKUP_FAIL_ACTION  "BackupFailAction"
#define BACKUP_VIRUS_STATE  "BackupVirusState"
#define LARGE_VIRUS_ACTION  "LargeVirusAction"
#define SCAN_FILE_TYPE      "ScanFileType"
#define SCAN_COMMON_VIRUS   "ScanCommonVirus"
#define SCAN_INTELLIGENT    "ScanIntelligent"
#define SCAN_COMPRESS_STATE "ScanCompressFile"
#define BACKUP_SPACE_LESS   "SpaceLessAction"
#define COMPRESS_FILE_SIZE  "CompressSize"
#define COMPRESS_FILE_DEPTH "CompressDepth"

#define CLOUD_ENGINE_ENABLE  "CloudEngineEnable"
#define CLOUD_ENGINE_URL     "CloudEngineUrl"

#define TIME_QUICK_SCAN_FLAG "TimeScanQuick"
#define TIME_FULL_SCAN_FLAG  "TimeScanFull"
#define TIME_LINUX_SCAN_FLAG "TimeScanLinux"

#define EFFICIENT_ITEM      "Eficient"
#define ADMIN_OPERATOR_LOCK "AdminOperateLocked"


#define FILE_MONITOR_POWERON    "FileMonitorPowerOn"
#define FILE_INTELLIGENT_BL     "IntelligentBlacklist"
#define MONITOR_KERNEL_STATE    "KernelWatch"
#define MONITOR_WATCH_MODE      "FileMonitorMode"
#define MONITOR_DEF_TYPE        "MonitorDefType"
#define MONITOR_DEF_EXTS        "MonitorDefExts"
#define FILE_MONITOR_FILE_TYPE  "FileMonitorType"
#define FILE_MONITOR_FILE_EXTS  "FileMonitorExts"
#define FILE_MONITOR_ACCELERATE "FileAccelerate"
#define FILE_EMBED_SCAN         "FileEmbedScan"
#define FILE_MONITOR_COMMON_VIRUS  "FileMonitorCommonVirus"
#define FILE_MONITOR_SCAN_INTELLIGENT  "FileMonitorScanIntelligent"
#define FILE_MONITOR_SCAN_ZIP   "FileMonitorZip"
#define FILE_MONITOR_ZIP_SIZE   "FileMonitorZipSize"
#define FILE_MONITOR_ZIP_DEPTH  "FileMonitorZipDepth"
#define FILE_MONITOR_VIRUS      "FileMonitorVirusDeal"
#define FILE_MONITOR_CLEAN_FAIL "FileMonitorCleanFailed"
#define FILE_MONITOR_KILL_PROMPT "FileMonitorPrompt"
#define FILE_MONITOR_USE_CLOUD  "FileMonitorUseCloud"
#define FILE_MONITOR_DOC_FOLDER "FileMonitorDocFolder"
#define FILE_MONITOR_CLEAR_CACHE "FileMonitorClearCache"

#define U_PLUGIN_MONITOR        "UPluginState"
#define U_PLUGIN_INSERT         "UPluginInsert"
#define U_PLUGIN_SCAN_DEPTH     "UPluginScanDepth"

#define MODULE_STOP_TO_REMOVE   "ModuleStopToRemove"

//define netfilter setting



#define ADMIN_PASSWORD      "AdminPasswd"
#define TRAY_ICON_STATE     "TrayIconState"
#define SHOW_AUTH_STATE     "ShowAuthState"
#define CLIENT_TITLE        "ClientTitle"
#define CLIENT_SUB_TITLE    "ClientSubTitle"
#define CENTER_PROXY_STATE  "CenterProxyState"
#define CLIENT_LOCK_GUID    "ClientLockGUID"
#define CLIENT_CENTER_LIST  "ClientCeneterList"
#define CHECK_CENTER_EID    "CheckClientEID"
#define RECONNECT_INTERVAL  "ReconnectInterval"
#define UPLOAD_BANDWIDTH_LIMIT  "LimitUpdateBandWidth"
#define BW_LIMIT_START_TIME "BandWidthLimteStart"
#define BW_LIMIT_END_TIME   "BandWidthLimteEnd"

//#define SETTING_UPLOAD_BANDWIDTH_TYPE       "UploadBandWidthType"
// 上传带宽时间 00:00-00:00
//#define SETTING_UPLOAD_BANDWIDTH_LIMIT_TIME "UploadBandWidthDuration"


#define UPDATE_MODE         "UpdateMode"
#define UPDATE_HOUR_MINUTE  "UpdateTime"
#define UPDATE_WEEKS        "UpdateWeeks"
#define UPDATE_DELAY_START  "UpdateDelayAction"
#define UPDATE_DELAY_TIME   "UpdateDelayTime"
#define UPDATE_CONTENT_ACTION "UpdateContent"
#define VIRUS_LIB_UPDATE_INTIME "VirusLibIntime"
#define URL_LIB_VALID_ATONCE "UrlLibValidAtonce"
#define UPDATE_PROXY_ACTION "ProxyAction"
#define UPDATE_PROXY_ADDR   "ProxyAddr"
#define UPDATE_PROXY_PORT   "ProxyPort"
#define UPDATE_PROXY_ACCOUNT "ProxyAccount"
#define UPDATE_PROXY_PASSWD "ProxyPasswd"
#define UPDATE_PROXY_AUTH   "ProxyAuth"
#define OFFICE_UPDATE_SOURCE "OfficalSource"
#define UPDATE_NET_BAND_LIMIT "UpdateNetLimit"
#define UPDATE_BAND_WIDTH   "UpdateBandWidth"
#define UPDATE_NET_LIMIT_START "UpdateNetLimitStart"
#define UPDATE_NET_LIMIT_END "UpdateNetLimitEnd"
#define UPDATE_FORCE_DELAY_DAY "UpdateForceDelayDay"
#define UPDATE_FORCE_SWITCH     "UpdateForceFlag"
#define UPDATE_CLEAR_SYS_CACHE "UpdateClearSysCache"

#define UPDATE_SOURCE_ACTION "UpdateSoureAction"
#define UPDATE_SOURCE_EXTEND "UpdateSourceAddr"
#define UPDATE_EXTEND_DATA   "UpdateExtend"

#define DEPLOY_START_MENU   "DeployStartMenu"
#define DEPLOY_DESKTOP_ICON "DeployDesktopIcon"

#define UPDATE_MAX_SPEED_KB "UpdateMaxSpeed"
#define UPDATE_DOWN_THREAD  "UpdateThreadCount"



#define ACTIVE_DEFENCE_SWITCH   "ActiveDefenceState"
#define FILE_DEFEN_SWITCH       "FileDefenceState"
#define MAIL_DEFEN_SWITCH       "MailDefenceState"
#define NETFILTER_SWITCH        "NetFilterState"

#define SHARED_DEFEN_SWITCH     "SharedDefenState"
#define BROWSER_BLOCK_SWITCH    "BrowserBlockState"
#define SPECIAL_FAKE_WORM       "FakeWormState"
#define SPECIAL_VIRUS_RAIN      "RainCloudState"
#define SPECIAL_WEIKE_WORM      "WeikeWormState"
#define SPECIAL_DLL_INJECTLESS  "DLLInjectLess"

#define EP_SHOW_AUTH_TIP        "ShowAuthTipState"
#define EP_TITLE                "ClientTitle"
#define EP_SUB_TITLE            "ClientSubTitle"
#define EP_LOCK_GUID            "ClientGUIDLocked"
#define EP_CENTER_LIST          "OtherCenterAddr"

// 网络重连的间隔
//#define SETTING_CENTER_RETRY_INTERVEL       "CenterRetryInterval"
// 上传带宽限制类型

//
//#define SETTING_CENTER_FIND_SAME_DOMAIN "CheckProxyCenterEID"




#define SCHEDULE_SCAN_TYPE      "SheduleScanType"

#define USB_PROMPT_STATE_SWITCH "USBPromptState"
//#define USE_SCAN_DEPTH_LEVEL    "USBScanDepthLevel"



#define REMOTE_SCAN_PATH        "RemoteScanPath"


#define VLIB_UPDATE_TIME        "VlibUpdateTime"
#define VLIB_UPDATE_VERSION     "VlibUpdateVersion"

#define APP_VERSION_ITEM     "AppVersion"
#define CENTER_URL_ITEM     "CenterUrl"


#endif // STORAGE_GLOBAL_H
