#include "ravlockfile.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>

int ravlockfile(const char *szLockFile,bool bBlock/*=true */)
{
    int nFd;
    if((nFd=open(szLockFile,O_CREAT|O_RDWR,0777))==-1)
    {
        perror("open:");
        return -1;
    }


#if defined(PLATFORM_TYPE_LINUX_INTEL) || defined(PLATFORM_TYPE_IBM_RISC )
    if(bBlock)
        flock(nFd,LOCK_EX);
    else
    {
        if(flock(nFd,LOCK_EX|LOCK_NB)==-1)
        {
            close(nFd);
            return -1;
        }
    }
#else
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;

    if(bBlock)
        fcntl(nFd,F_SETLKW,&lock);
    else
    {
        if(fcntl(nFd,F_SETLK,&lock)==-1)
        {
            printf("%s getlock failed %d\n",szLockFile,getpid());
            close(nFd);
            return -1;
        }
    }

#endif

    return nFd;
}

void ravunlockfile(int nFd)
{
#if defined(PLATFORM_TYPE_LINUX_INTEL) || defined(PLATFORM_TYPE_IBM_RISC )
    flock(nFd,LOCK_UN);
#else
        struct flock lock;
        lock.l_type=F_UNLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=0;
        lock.l_len=0;
        fcntl(nFd,F_SETLK,&lock);
#endif
    close(nFd);
}

#ifdef __TEST_MAIN__
#include <stdio.h>
int main(int argc,char *argv[])
{
    char *szLockfile="./.lockfile";

    bool bBlock=false;
    bool bFork=false;

    char *szValid_options="bf";;

    int nRet;
    while( (nRet=getopt(argc,argv,szValid_options))!=-1)
    {
        switch(nRet)
        {
            case 'b':
                bBlock=true;
                break;
            case 'f':
                bFork=true;
                break;
        }
    }


    int nFd;


    if((nFd=ravlockfile(szLockfile,bBlock))==-1)
    {
        printf("lock failed\n");
        return -1;
    }
    if(!bFork)
    {
        printf(" we test in no fork\n");
        int i=0;
        while(i<30)
        {
            sleep(1);
            i++;
        }
        ravunlockfile(nFd);
    }
    else
    {
        printf(" we test in fork\n");
        pid_t nPid;
        nPid=fork();
        if(nPid==0)
        {
            int i=0;
            while(i<30)
            {
                sleep(1);
                i++;
            }
            ravunlockfile(nFd);
        }
        else
        {
            ravunlockfile(nFd);
        }
    }
    return 0;
}
#endif
