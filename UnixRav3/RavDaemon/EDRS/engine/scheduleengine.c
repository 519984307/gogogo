#include "scheduleengine.h"

// 设置最大上传结点的数量, 为了防止上传的数据量太大
#define MAX_POST_NODE_COUNT   500


static schedule_context *g_shedule_context;


static void waiting_for_data() {
  pthread_mutex_lock(&g_shedule_context->list_empty_mutex);
  pthread_cond_wait(&g_shedule_context->list_empty_cond, &g_shedule_context->list_empty_mutex);
  pthread_mutex_unlock(&g_shedule_context->list_empty_mutex);
}

static int make_log_upload_data(OSList *list, int count, char **data) {
  
  return 0;
}

static void* schedule_log_function(void *params) {
  while(g_shedule_context->status) {
    waiting_for_data();

    char *log = NULL;
    int size = g_shedule_context->log_list->currently_size>MAX_POST_NODE_COUNT
                  ? MAX_POST_NODE_COUNT : g_shedule_context->log_list->currently_size;
    if (0 == make_log_upload_data(g_shedule_context->log_list, 500, &log)) {
      if (g_shedule_context->log_export) {
        g_shedule_context->log_export(log);
      }
      if (log) {
        free(log);
      }

      pthread_mutex_lock(&g_shedule_context->list_remove_mutex);
      {
        OSListNode *node = OSList_GetFirstNode(g_shedule_context->log_list);
        free(node->data);
        OSList_DeleteThisNode(g_shedule_context, node);
      }
      pthread_mutex_unlock(&g_shedule_context->list_remove_mutex);
    }
  }
}


void init_schedule_engine(schedule_context *context) {
  g_shedule_context = (schedule_context*)malloc(sizeof(schedule_context));
  memset(g_shedule_context, 0, sizeof(schedule_context));

  g_shedule_context->log_list = OSList_Create();

  pthread_mutex_init(&g_shedule_context->list_remove_mutex, NULL);
  pthread_mutex_init(&g_shedule_context->list_empty_mutex, NULL);
  pthread_cond_init(&g_shedule_context->list_empty_cond, NULL);

  pthread_create(&g_shedule_context->schedule_thread, NULL, schedule_log_function, g_shedule_context);
}

void release_schedule_engine(schedule_context *context) {
  g_shedule_context->status = 0;
  pthread_cond_signal(&g_shedule_context->list_empty_cond);
  pthread_join(g_shedule_context->schedule_thread, NULL);

  pthread_cond_destroy(&g_shedule_context->list_empty_cond);
  pthread_cond_destroy(&g_shedule_context->list_remove_mutex);
  pthread_cond_destroy(&g_shedule_context->list_empty_mutex);

  while (g_shedule_context->log_list->currently_size) {
    OSListNode *node = OSList_GetCurrentlyNode(g_shedule_context->log_list);
    if (node) {
      free(node->data);
    }
    OSList_DeleteCurrentlyNode(g_shedule_context->log_list);
  }
  free(g_shedule_context->log_list);
}

int add_event_log(schedule_context *context, int event, void *data) {
  pthread_mutex_lock(&g_shedule_context->list_remove_mutex);
  {
    OSList_AddData(g_shedule_context->log_list, data);
  }

  pthread_cond_signal(&g_shedule_context->list_empty_cond);
  return 0;
}

void set_export_log_callback(void(*log_export)(const char *)) {

}
