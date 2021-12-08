/**
 * Copyright
 */
#ifndef RAVDAEMON_EDRS_WATCHENGINE_H_
#define RAVDAEMON_EDRS_WATCHENGINE_H_

#include <stdio.h>

// 初始化监控引擎
// 启动预扫描文件引擎
// 启动文件监控
// 启动日志文件监控
// 动态规划监控目录
enum _FUNCTION {
  MONITOR_LOGIN_STATE       = 0x1,      // 登录信息
  MONITOR_USER_INFO         = 0x2,      // 用户帐户信息
  MONITOR_FILE_CHANGED      = 0x4,      // 文件的改变
  MONITOR_AUTSTART          = 0x8,      // 开机启动项
  MONITOR_CORE_COMMAND      = 0x10,     // 核心文件的修改
  MONITOR_SERVICE           = 0x20,     // 服务的修改
  MONITOR_INSTALLED         = 0x40,     // 安装软件
};
typedef enum _FUNCTION FUNCTION;


int init_watch_engine();

int start_watch_engine(FUNCTION functions);

int add_watch_function(FUNCTION function);

int stop_watch_engine();



#endif  // RAVDAEMON_EDRS_WATCHENGINE_H_
