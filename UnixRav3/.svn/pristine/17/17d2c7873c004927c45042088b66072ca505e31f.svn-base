#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#include "../BaseController/baseconfig.h"

#define PATH_MAX_LENGTH     128
#define BUFFER_SIZE         1024


int get_install_path(char *path) {
    strcpy(path, INSTALL_PATH);
    if (strlen(path) == 0) {
        FILE *file = fopen(RAV_CONFIG_FILE, "rb");
        if (NULL != file) {
            fseek(file, 128, SEEK_SET);
            fread(path, 1, 128, file);
            fclose(file);
            return 0;
        }
        return -1;
    }
    return 0;
}

int trimblank(char *szStr,int nMode)
{
    int nLen,i,j,nFlag;
    char szTempBuf[512];

    if(szStr==NULL)return -1;
    nLen=strlen(szStr);
    if(nLen==0 || nLen >511)return -1;
    strcpy(szTempBuf,szStr);
    j=0;

    //trim right blank
    if(nMode==1 || nMode==2) {
        for(i=nLen-1;i>=0;i--) {
            if(szTempBuf[i]==' ' || szTempBuf[i]=='\t' || szTempBuf[i]==0x0a || szTempBuf[i]==0x0d) {
                szTempBuf[i]='\0';
            }
            else {
                break;
            }
        }
    }

    nLen=strlen(szTempBuf);
    if (nLen==0) {
        szStr[0]='\0';
        return 0;
    }

    //trim left
    nFlag=0;
    j=0;
    for(i=0;i<=nLen;i++) {
        if((szTempBuf[i]==' ' ||szTempBuf[i]=='\t' || szTempBuf[i]=='\n') && nFlag==0) {
            continue;
        }
        else {
            nFlag=1;
            szStr[j++]=szTempBuf[i];
        }
    }
    return 0;
}

int check_service_state(const char *process) {
    FILE *fpPipe=NULL;
    char szProcess[512];
    pid_t pnPid[10];

    sprintf(szProcess,"ps -ef|grep %s", process);

    if((fpPipe=popen(szProcess,"r"))==NULL)
        return -1;

    int nTot=0;

    char szBuf[512];
    char szGrepFlag[512];
    sprintf(szGrepFlag,"grep %s", process);

    while(fgets(szBuf,sizeof(szBuf)-1,fpPipe)!=NULL)
    {
        trimblank(szBuf,2);
        if( !strstr(szBuf,process) || strstr(szBuf,szGrepFlag) )
            continue;

        // get filed two for pid

        char *szSavePtr;
        char *token;
        char *szStr=szBuf;
        char szPid[256],szTmpCmd[256];
        memset(szPid,0,sizeof(szPid));
        memset(szTmpCmd,0,sizeof(szTmpCmd));

        int nItemsPid=1;
        int nCmdItems=7;
        int i;
        for(i=0;i<=nCmdItems;i++,szStr=NULL)
        {
            token=strtok_r(szStr," \t",&szSavePtr);
            if(!token)
                break;
            if(i==nItemsPid)
            {
                strncpy(szPid,token,sizeof(szPid)-1);
            }
            else if(i==nCmdItems)
                strncpy(szTmpCmd,token,sizeof(szTmpCmd)-1);
        }
        if(szPid[0]=='\0' || szTmpCmd[0]=='\0')
            continue;

        char *endptr;
        long int pid=strtol(szPid,&endptr,10);
        if( (endptr && *endptr!=0)|| pid<=0)
            continue;
        // getcmd
        pnPid[nTot++]=pid;
        if(nTot >= 10)
            break;
    }
    pclose(fpPipe);

    return nTot;
}

void join_path(char *out, const char* path, ...) {
    strcpy(out, path);
    if (out[strlen(out)-1] == '/') {
        out[strlen(out)-1] = '\0';
    }

    va_list args;
    va_start(args, path);
    while(1) {
        const char* one = va_arg(args, const char*);
        if (one == NULL) {
            break;
        }

        if (strlen(one) == 0) {
            continue;
        }
        if (one[0] != '/') {
            strcat(out, "/");
        }
        strcat(out, one);
    }
}

int start_service(const char *process, const char *command) {
    if (check_service_state(process) == 0) {
        return system(command);
    }
    return 999;
}

static int g_status = 0;
void exit_signal(int signal) {
    g_status = 0;
}

void* exit_thread(void *args) {
    char installdir[128];
    get_install_path(installdir);

    char sockpath[256];
    sprintf(sockpath, "%s/%s/%s", installdir, SOCKET_PATH, P2W_SOCKET);
    if (access(sockpath, F_OK) == 0) {
        unlink(sockpath);
    }
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        return NULL;
    }

    struct sockaddr_un srv_addr;
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, sockpath, sizeof(srv_addr.sun_path)-1);

    int err = bind(sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(err < 0) {
        close(sock);
        unlink(sockpath);
        return NULL;
    }

    err = listen(sock, 1);
    if (err < 0){
        close(sock);
        unlink(sockpath);
        return NULL;
    }

    struct sockaddr_un cli_addr;
    socklen_t len = sizeof(cli_addr);

    while (g_status) {
        int cli_fd = accept(sock, (struct sockaddr*)&cli_addr, &len);
        if (cli_fd < 0) {
            close(sock);
            unlink(sockpath);
            return -1;
        }

        int type, length, command;
        read(cli_fd, (unsigned char*)&type, sizeof(int));
        read(cli_fd, (unsigned char*)&length, sizeof(int));

        read(cli_fd, (unsigned char*)&command, sizeof(int));

        char buffer[128] = {0};
        read(cli_fd, (unsigned char*)buffer, length-4);

        if (command == 105) {
            g_status = 0;
        }

        command = 1;
        write(cli_fd, (unsigned char*)&(command), sizeof(int));

        close(cli_fd);
    }

    close(sock);
    return NULL;
}

void* main_function(void *args) {
    const char *install = (char*)args;
    char dog[128] = {0};
    join_path(dog, install, DEPLOYED_BIN_FOLDER, WATCHDOG_PROGUARD_PROCESS, NULL);
    if (check_service_state(dog) > 1) {
        return 0;
    }

    char process_name[128];
    char service_command[128];

    g_status = 1;
    int sleep_time;
    sleep_time = 10;
    while(g_status) {
        join_path(process_name, install, DEPLOYED_BIN_FOLDER, DAEMON_PROCESS_ENTRY, NULL);
        join_path(service_command, install, DEPLOYED_BIN_FOLDER, DAEMON_PROCESS_COMMAND, NULL);

        if (start_service(process_name, service_command) == 999) {
            sleep_time *= 2;
            if (sleep_time > 3600) {
                sleep_time = 3600;
            }
        }
        else {
            sleep_time = 10;
        }

#ifdef DEBUG
        sleep(5);
#else
        sleep(sleep_time);
#endif
    }

    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid;
    if ((pid=fork()) == 0)
    {
        signal(SIGUSR2, exit_signal);

        g_status = 1;
        char install[PATH_MAX_LENGTH];
        memset(install, 0, PATH_MAX_LENGTH);
        if (get_install_path(install)) {
            return -1;
        }

        pthread_t command_thread;
        pthread_create(&command_thread, NULL, exit_thread, install);

        pthread_t main_thread;
        pthread_create(&main_thread, NULL, main_function, install);

        while (g_status > 0) {
            sleep(1);
        }
        exit(0);
        return 0;
    }
    return 0;
}
