#ifndef EDRS_SCHEDULE_ENGINE_H
#define EDRS_SCHEDULE_ENGINE_H

#include <pthread.h>
#include "common/list_op.h"

struct _schedule_context {
  OSList    *log_list;
  pthread_t schedule_thread;
  void(*log_export)(const char *);

  int status;

  pthread_mutex_t list_empty_mutex;
  pthread_cond_t  list_empty_cond;
  pthread_mutex_t list_remove_mutex;
};
typedef struct _schedule_context schedule_context;


void init_schedule_engine(schedule_context *context);
void release_schedule_engine(schedule_context *context);

int add_event_log(schedule_context *context, int event, void *data);

void set_export_log_callback(void(*log_export)(const char *));


#endif // EDRS_SCHEDULE_ENGINE_H
