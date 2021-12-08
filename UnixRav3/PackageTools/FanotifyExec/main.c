#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <dlfcn.h>

#include <linux/fanotify.h>
//#include <sys/fanotify.h>
typedef int (*glibc_fanotify_init)(unsigned int __flags, unsigned int __event_f_flags);
typedef int (*glibc_fanotify_mark)(int __fanotify_fd, unsigned int __flags, __u64 __mask, int __dfd, const char *__pathname);

#ifndef AT_FDCWD
	#define AT_FDCWD         -100
#endif

#define FANOTIFY_BUFFER_SIZE 8192

static pid_t pid;
static __u64 event_mask =
    (   FAN_ACCESS |         /* File accessed */
        FAN_MODIFY |         /* File modified */
        FAN_CLOSE_WRITE |    /* Writtable file closed */
        FAN_CLOSE_NOWRITE |  /* Unwrittable file closed */
        FAN_OPEN |           /* File was opened */
        FAN_ONDIR |          /* We want to be reported of events in the directory */
        FAN_EVENT_ON_CHILD
        //FAN_OPEN_PERM
        //| FAN_ACCESS_PERM
     ); /* We want to be reported of events in files of the directory */

static glibc_fanotify_init fanotify_init = 0;
static glibc_fanotify_mark fanotify_mark = 0;

static char * monitors[] = { "./", };
static int monitors_count = (int)sizeof(monitors)/sizeof(monitors[0]);


static void load_fanotify_funcs()
{
	void * p = dlopen("libc.so.6", RTLD_NOW);
	if(p)
	{
		fanotify_init = (glibc_fanotify_init)dlsym(p, "fanotify_init");
		fanotify_mark = (glibc_fanotify_mark)dlsym(p, "fanotify_mark");
	}
}

/***************
static char *get_program_cmdline_from_pid(int pid, char *buffer, size_t buffer_size) {
    int i;
    int fd;
    ssize_t len;

    // Try to get program name by PID 
    sprintf(buffer, "/proc/%d/cmdline", pid);
    if ((fd = open(buffer, O_RDONLY)) < 0)
        return NULL;

    // Read file contents into buffer 
    if ((len = read(fd, buffer, buffer_size - 1)) <= 0) {
        close(fd);
        printf("read the pid:%d cmd error\n", pid);
        return NULL;
    }
    close(fd);

    for (i = 0; i < len; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = ' ';
        }
    }

    buffer[len] = '\0';

    return buffer;
}

static char *get_file_path_from_fd(int fd, char *buffer, size_t buffer_size) {
    ssize_t len;

    if (fd <= 0)
        return NULL;

    sprintf(buffer, "/proc/self/fd/%d", fd);
    if ((len = readlink(buffer, buffer, buffer_size - 1)) < 0){
        perror("readlink");
        exit(EXIT_FAILURE);
    }

    buffer[len] = '\0';
    return buffer;
}

static void event_process(int fd, struct fanotify_event_metadata *event) {
    printf("event_process ");

    if(pid == event->pid){
        close(event->fd);
        printf("event_process self end!\n");

        return;
    }

    char   path[PATH_MAX];
    time_t current_time;
    char * c_time_string;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    printf("%s [%d] Event on '%s' ",
           strtok(c_time_string, "\n"),
           event->pid, get_file_path_from_fd(event->fd, path, PATH_MAX) ? path : "unknown");

    printf("event fd:%d: ", event->fd);
    if (event->mask & FAN_OPEN)
        printf("FAN_OPEN ");
    if (event->mask & FAN_ACCESS)
        printf("FAN_ACCESS ");
    if (event->mask & FAN_MODIFY)
        printf("FAN_MODIFY ");
    if (event->mask & FAN_CLOSE_WRITE)
        printf("FAN_CLOSE_WRITE ");
    if (event->mask & FAN_CLOSE_NOWRITE)
        printf("FAN_CLOSE_NOWRITE ");

    printf("\n");
    fflush(stdout);
    close(event->fd);
}*/


static void shutdown_fanotify(int fanotify_fd) {
    int i;

    for (i = 0; i < monitors_count; ++i) {
        fanotify_mark(fanotify_fd,
                      FAN_MARK_REMOVE,
                      event_mask,
                      AT_FDCWD,
                      monitors[i]);
    }
    close(fanotify_fd);
}

static int initialize_fanotify(int argc, const char **argv) {
    int i;
    int fanotify_fd;
    /* Create new fanotify-cmdline device */
    if (!fanotify_init || !fanotify_mark)
    {
    	printf("Couldn't find fanotify_init!!!\n");
    	return -1;
    }

    if ((fanotify_fd = fanotify_init(FAN_CLOEXEC  | FAN_NONBLOCK | FAN_CLASS_NOTIF,
                                     O_RDONLY | O_LARGEFILE)) < 0)
    {
        fprintf(stderr,
                "Couldn't setup new fanotify-cmdline device: %s\n",
                strerror(errno));
        return -1;
    }

    for (i = 0; i < monitors_count; ++i) {
        if (fanotify_mark(fanotify_fd,
                          FAN_MARK_ADD,
                          event_mask,
                          AT_FDCWD,
                          monitors[i]) < 0)
        {
            fprintf(stderr,
                    "Couldn't add monitor in directory (%d)'%s': '%s'\n", fanotify_fd, 
                    monitors[i],
                    strerror(errno));
            return -1;
        }
    }

    return fanotify_fd;
}


int main(int argc, const char **argv)
{
    int fanotify_fd;

	// 
	load_fanotify_funcs();

    pid =getpid();
    if ((fanotify_fd = initialize_fanotify(argc, argv)) < 0) {
        fprintf(stderr, "Couldn't initialize fanotify-cmdline\n");
        exit(EXIT_FAILURE);
    }

    shutdown_fanotify(fanotify_fd);

    printf("Success\n");
    return EXIT_SUCCESS;
}
