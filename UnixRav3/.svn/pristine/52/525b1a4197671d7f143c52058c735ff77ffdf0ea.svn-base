
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <panzer_if.h>
#include "ukcom.h"


static struct ukcom_channel ukcom;
static FILE *fp = NULL;

char *get_proc_path(int pid, char *path, int path_len)
{
	int rslt;
	char link[100];

	sprintf(link, "/proc/%d/exe", pid);
	rslt = readlink(link, path, path_len - 1);
	if (rslt < 0 || rslt >= path_len - 1)
		return NULL;
	path[rslt] = '\0';
	return path;
}

void __init(void)
{
    int rc;
    int retry = 10;

    while (retry > 0) {
        rc = __init_ukcom(&ukcom, PANZER_PROTO, PANZER_USER_PID);
        if (rc >= 0)
            break;
        sleep(1);
        if (fp != NULL) {
            fprintf(fp, "rc = %d, retry %d\n", rc, retry);
            fflush(fp);
        }
        printf("rc = %d, retry %d\n", rc, retry);
        retry--;
    }
}

void main(void)
{
    fp = NULL; //fopen("/opt/BDFZ/RAV/RJJHZDFY/RAVZDFY0.log", "w");

    __init();

    while (1) {
        struct info_header *umsg;

        umsg = ukcom_get_msg(&ukcom, sizeof(*umsg));

        if (umsg != NULL) {
            char path[1024];
            if (fp != NULL) {
                fprintf(fp, "[%s:%s(%d)] %s\r\n",
                    s_action(umsg->action),
                    get_proc_path(umsg->pid, path, sizeof(path)), umsg->pid, umsg->pathname);
                fflush(fp);
            }
            printf("[%s:%s(%d)] %s\r\n",
                s_action(umsg->action),
                get_proc_path(umsg->pid, path, sizeof(path)), umsg->pid, umsg->pathname);
            if (strstr(umsg->pathname, "pstree") != NULL) {
                umsg->verdict = UV_deny;
            }
            ukcom_reply_msg(&ukcom, umsg, sizeof(*umsg));
        }
    }

    __deinit_ukcom(&ukcom);
    fclose(fp);
}
