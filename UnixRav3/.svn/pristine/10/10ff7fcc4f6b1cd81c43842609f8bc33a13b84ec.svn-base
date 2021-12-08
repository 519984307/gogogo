/**
 * Copyright
 */
#ifndef RAVDAEMON_EDRS_COREENGINE_H_
#define RAVDAEMON_EDRS_COREENGINE_H_

#include <pthread.h>
#include <stdio.h>

#include "hashtable/hashtable.h"


struct _file_monitor {
  FILE  *fd;
  int   watch_fd;
  char  filename[255];
  void (*file_monitor_callback)(int event, int type, void* data);
};
typedef struct _file_monitor file_monitor_entry;

struct _engine_context{
  int       epoll_fd;
  int       notify_fd;
  pthread_t work_thread;
  hash_table fds_table;
};
typedef struct _engine_context engine_context;

int init_core_engine(engine_context *context);

int start_core_engine(engine_context *context);

int release_core_engine(engine_context *context);

int add_watch_folder(
                      engine_context *context,
                      const char *filepath,
                      void(*callback)(int, int, void*)
);
int remove_watch_folder(engine_context *context, const char *filepath);

long get_max_user_watches();


#endif  // RAVDAEMON_EDRS_COREENGINE_H_
