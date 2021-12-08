/**
 * Copyright
 */
#include "coreengine.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/inotify.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENTS  1024
#define MAX_POLL_EVENT  20

int init_core_engine(engine_context *context) {
  context->epoll_fd = epoll_create(MAX_EVENTS);
  if (context->epoll_fd < 0) {
    return -1;
  }

  context->notify_fd = inotify_init();
  if (context->notify_fd < 0) {
    return -1;
  }

  ht_init(&context->fds_table, HT_KEY_CONST|HT_NONE, 0.05);
  return 0;
}

int release_core_engine(engine_context *context) {
  if (context->epoll_fd < 0) {
    close(context->epoll_fd);
    context->epoll_fd = -1;
  }
  if (context->notify_fd < 0) {
    close(context->notify_fd);
    context->notify_fd = -1;
  }

  ht_clear(&context->fds_table);
  ht_destroy(&context->fds_table);

  pthread_join(context->work_thread, NULL);
  return 0;
}

int add_watch_folder(engine_context *context,
                      const char *filepath,
                      void(*callback)(int,int,void*)) {
  int wd = inotify_add_watch(context->notify_fd, filepath, IN_ALL_EVENTS);
  if (wd < 0) {
    return -1;
  }

  size_t vsize = 0;
  void *value = ht_get(&context->fds_table, &wd, sizeof(wd), &vsize);

  if (NULL == value) {
    file_monitor_entry *entry = malloc(sizeof(file_monitor_entry));
    strncpy(entry->filename, filepath, 255);
    entry->file_monitor_callback = callback;
    entry->watch_fd = wd;

    ht_insert(&context->fds_table,
              &entry->watch_fd,
              sizeof(entry->watch_fd),
              (void*)&entry,
              sizeof(void*));
  }
  return 0;
}

int remove_watch_folder(engine_context *context, const char *filepath) {
  size_t vsize;
  uint32_t num_keys;
  int **keys;
  keys = (int**)ht_keys(&context->fds_table, &num_keys);
  for (int i=0; i<num_keys; ++i) {
    int tfd = *keys[i];
    void *value = ht_get(&context->fds_table, &tfd, sizeof(tfd), &vsize);
    if (value) {
      file_monitor_entry *entry = (file_monitor_entry*)value;
      if (0 == strcmp(filepath, entry->filename)) {
        inotify_rm_watch(context->notify_fd, tfd);
        ht_remove(&context->fds_table, &tfd, sizeof(tfd));
        free(entry);
        break;
      }
    }

  }
  return 0;
}

int get_notify_file(engine_context *context) {
  int len;
  int i;
  int nread;
  char buf[1024];
  char *offset;
  struct inotify_event *event;

  while ((len = read(context->notify_fd, buf, 1024)) > 0) {
    nread = 0;
    offset = buf;
    event = (struct inotify_event*)buf;
    while (((char*)event-buf) < len) {
      for (i = 0; i < 8; i++) {
        if ((event->mask >> i) & 1) {
          size_t vsize;
          void *value = ht_get(&context->fds_table, &event->wd, sizeof(event->wd), &vsize);
          if (value) {
            file_monitor_entry *entry = (file_monitor_entry*)*(long*)value;
            if (entry->file_monitor_callback) {
              entry->file_monitor_callback(event->mask & IN_ISDIR, event->mask, entry->filename);
            }
          }
        }
      }

      nread = sizeof(struct inotify_event) + event->len;
      event = (struct inotify_event *)(offset + nread);
      offset += nread;
    }
  }
}

static void* _engine_main_function(void *params) {
  int nfds;
  int i;
  struct epoll_event events[MAX_POLL_EVENT];
  engine_context *context = (engine_context*)params;

  while (1) {
    nfds = epoll_wait(context->epoll_fd, events, MAX_POLL_EVENT, 500);
    for (i = 0; i < nfds; ++i) {
      if (events[i].events & EPOLLIN &&
          events[i].data.fd == context->notify_fd) {
        get_notify_file(context);
      }
    }
  }
}

int start_core_engine(engine_context *context) {
  struct epoll_event ev;
  ev.data.fd = context->notify_fd;
  ev.events = EPOLLIN|EPOLLET;
  if (epoll_ctl(context->epoll_fd, EPOLL_CTL_ADD, context->notify_fd, &ev) == 0) {
    return pthread_create(
              &context->work_thread,
              NULL,
              _engine_main_function,
              context);
  }
  return -1;
}

long get_max_user_watches() {
  int max_watch = 0;
  const char *CONFIG_FILE = "/proc/sys/fs/inotify/max_user_watches";
  FILE *f = fopen(CONFIG_FILE, "r");
  if (f) {
    char temp[32] = {0};
    char *ret = fgets(temp, 30, f);
    if (ret) {
      max_watch = atoi(temp);
    }
    fclose(f);
  }
  return max_watch;
}
