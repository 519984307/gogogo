/**
 * Copyright
 */
#include "watchengine.h"

#include <pthread.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <stdlib.h>

#include "coreengine.h"
#include "common/list_op.h"
#include "entries/system.h"

/** 引擎的状态 */
static int engine_statue;
/** 扫描文件的线程, 此线程用以扫描普通文件的状态 */
static pthread_t pre_scan_thread;
/** 监控模块的context */
static engine_context *g_context;
/** 文件扫描的列表 */
static OSList *g_file_list;


// 首先将所有文件的信息记录在hash_file里, 在应用程序启动前将
// 文件信息上传
static int prescan_files(const char *hash_file) {

}

static void handle_system_info(system_info *info) {
  printf("%s -- %s -- %ld\n", info->username, info->remote, info->time);

  
}

// 解析登录日志的主函数
static void filter_auth_log(int event, int type, void *data) {
  if (type & IN_MODIFY) {
    // 如果文件被修改, 解析文件d
    parse_auth_log((const char*)data, 0, handle_system_info);
  }
}

// 解析开关机信息及登录信息
static void filter_system_log(int event, int type, void *data) {
  if (type & IN_MODIFY) {
    // 如果文件被修改, 解析文件
    get_system_reboot_info(0, handle_system_info);
  }
}

static void filter_user_info_log(int event, int type, void *data) {

}

static void filter_core_command(int event, int type, void *data) {

}

int add_watch_function(FUNCTION function) {
  if (function & MONITOR_LOGIN_STATE) {
    struct stat stat_;
    if (0 == stat("/var/log/auth.log", &stat_)) {
      add_watch_folder(g_context, "/var/log/auth.log", filter_auth_log);
    } else if (0 == stat("/var/log/secure", &stat_)) {
      add_watch_folder(g_context, "/var/log/secure", filter_auth_log);
    }
    if (0 == stat("/var/log/wtmp", &stat_)) {
      add_watch_folder(g_context, "/var/log/secure", filter_system_log);
    }
  }
  if (function & MONITOR_USER_INFO) {
    // struct stat stat_;
    // if (0 == lstat("/etc/passwd", &stat_)) {
    //   add_watch_folder(g_context, "/etc/passwd", filter_user_info_log);
    // }
  }
  if (function & MONITOR_FILE_CHANGED) {

  }
  if (function & MONITOR_AUTSTART) {

  }
  if (function & MONITOR_CORE_COMMAND) {
    // struct stat stat_;
    // if (0 == lstat("/bin", &stat_)) {
    //   add_watch_folder(g_context, "/bin", filter_core_command);
    // }
    // if (0 == lstat("/usr/bin", &stat_)) {
    //   add_watch_folder(g_context, "/usr/bin", filter_core_command);
    // }
  }
  if (function & MONITOR_SERVICE) {

  }
  if (function & MONITOR_INSTALLED) {

  }
}

int init_watch_engine() {
  engine_statue = 0;
  g_context = NULL;
  pre_scan_thread = 0;
  g_file_list = NULL;

  return 0;
}

int start_watch_engine(FUNCTION functions) {
  engine_statue = 1;
  g_file_list = OSList_Create();

  // if (pthread_create(&pre_scan_thread, NULL, prescan_files, NULL)) {
  //   return -1;
  // }

  g_context = (engine_context*)malloc(sizeof(engine_context));
  if (init_core_engine(g_context)) {
    return -1;
  }

  if (functions) {
    add_watch_function(functions);
  }

  start_core_engine(g_context);
  return 0;
}

int stop_watch_engine() {
  if (NULL != g_context) {
    free(g_context);
    g_context = NULL;
  }

  release_core_engine(g_context);

  return 0;
}
