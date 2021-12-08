#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>
#include <errno.h>
#include <dlfcn.h>
#include <mntent.h>

#include <list>
#include <string>
#include <map>

#include "defenmodproxy.h"
#include "baseconfig.h"
#include "commonapi.h"
#include "panzer_if.h"
#include "observer.h"


#ifndef FANOTIFY_EXEC
#include <sys/inotify.h>

static const char *paths[] = {"/bin", "/usr/bin", "/usr/lib", "/boot"};


#else
#include <linux/fanotify.h>
//#include <sys/fanotify.h>
typedef int (*glibc_fanotify_init)(unsigned int __flags, unsigned int __event_f_flags);
typedef int (*glibc_fanotify_mark)(int __fanotify_fd, unsigned int __flags, unsigned long long int __mask, int __dfd, const char *__pathname);


#ifndef O_CLOEXEC
	#define O_CLOEXEC        02000000
#endif

#define FANOTIFY_BUFFER_SIZE 8192
static glibc_fanotify_init   fanotify_init = 0;
static glibc_fanotify_mark   fanotify_mark = 0;


/*
static uint64_t event_mask =
    (   FAN_ACCESS |         // File accessed 
        FAN_MODIFY |         // File modified 
        FAN_CLOSE_WRITE |    // Writtable file closed 
        FAN_CLOSE_NOWRITE |  // Unwrittable file closed 
        FAN_OPEN |           // File was opened 
        FAN_ONDIR |          // We want to be reported of events in the directory 
        FAN_EVENT_ON_CHILD
        //FAN_OPEN_PERM
        //| FAN_ACCESS_PERM
     ); 


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
*/

static char *get_file_path_from_fd(int fd, char *buffer, size_t buffer_size) {
    ssize_t len;

    if (fd <= 0)
        return NULL;

    sprintf(buffer, "/proc/self/fd/%d", fd);
    if ((len = readlink(buffer, buffer, buffer_size - 1)) < 0){
		return NULL;
    }

    buffer[len] = '\0';
    return buffer;
}

static void load_fanotify_funcs()
{
	// 采用动态加载的方式. 可能还需要用 syscall
	// linux kernel 2.6.37, glibc_2.13 ??
	if(!fanotify_init)
	{
		void * p = dlopen("libc.so.6", RTLD_NOW);
		if(p)
		{
			fanotify_init = (glibc_fanotify_init)dlsym(p, "fanotify_init");
			fanotify_mark = (glibc_fanotify_mark)dlsym(p, "fanotify_mark");
		}
	}
}

int  GetMountedDirList(std::list<std::string> & lstDir)
{
	const  char   * table = MOUNTED;
	struct mntent * mnt;
	FILE          * pf;

	pf = setmntent(table, "r");
	if(!pf) return -1;

	while((mnt = getmntent(pf)))
	{
		if(mnt->mnt_fsname[0] == '/') 
		{
			lstDir.push_back(mnt->mnt_dir);
		}
	}

	endmntent(pf);
	return 0;
}
#endif



DefenModProxy::DefenModProxy() {
    mStatus = 0;
    mObserver = NULL;
    mDefenceThread = 0;
	miNotifyFd = 0; miAccessFd = 0;
}

DefenModProxy::~DefenModProxy() {
    Stop();
}

char *DefenModProxy::get_proc_path(int pid, char *path, int path_len) {
    int rslt;
    char link[100];

    sprintf(link, "/proc/%d/exe", pid);
    rslt = readlink(link, path, path_len - 1);
    if (rslt < 0 || rslt >= path_len - 1)
        return NULL;
    path[rslt] = '\0';
    return path;
}

int DefenModProxy::Start() {
    pthread_mutex_init(&mMutex, NULL);
    return pthread_create(&mDefenceThread, NULL, DefenceWork, this);
}

int DefenModProxy::Stop() {
    mStatus = 0;

    if (mDefenceThread) {
        //pthread_cancel(mDefenceThread);
        pthread_join(mDefenceThread, NULL);
    }

    pthread_mutex_destroy(&mMutex);

    return 0;
}

void* DefenModProxy::DefenceWork(void *args) {
    DefenModProxy *self = (DefenModProxy*)args;
    self->DefenceWorkFunc();
    return NULL;
}

void DefenModProxy::DefenceWorkFunc()
{
    int timeout = -1;
#ifdef FANOTIFY_EXEC
    InitializeFanotify();
#else
    InitNotify();
    timeout = 100;
#endif

    while (mStatus > 0) {
        int poll_num = poll(mFds, miFdsNum, timeout);

        if (poll_num == -1) {
            if (errno == EINTR)
                continue;
			break;
        }

        if(poll_num == 0) continue;
		for(int i = 0; i < miFdsNum; i++) {
            if (mFds[i].revents & POLLIN) {
#ifdef FANOTIFY_EXEC
                WatchFanotifyEvents(mFds[i].fd);
#else
                WatchInotifyEvents(mFds[i].fd);
#endif
            }
        }
    }

    LOG_ZDFY("added watch iwds:%d, CountDefenceWorkFunc exit!\n", miCount);
#ifdef FANOTIFY_EXEC
    ShutdownFanotify();
#else
     // 删除inotify的watch对象
     for (int icount = 0; icount < miCount; icount++) {
        if (inotify_rm_watch(miNotifyFd, miWds[icount]) == -1) {
            printf("notify_rm_watch iwd[%d] = %d error!\n", icount, miWds[icount]);
            continue;
        }
     }

     close(miNotifyFd);
     mFileDirMap.clear();
     miCount=0;
     miNotifyFd = -1;
#endif
}


#ifndef FANOTIFY_EXEC
int DefenModProxy::WatchInotifyEvents(int fd)
{
    char event_buf[512];
    int ret;
    int event_pos = 0;
    int event_size = 0;
    struct inotify_event *event;

    ret = read(miNotifyFd, event_buf, sizeof(event_buf));

    if (ret < (int)sizeof(struct inotify_event)) {
        printf("counld not get event!\n");
        return -1;
    }
    while (ret >= (int)sizeof(struct inotify_event)) {
        event = (struct inotify_event*)(event_buf + event_pos);
        if (event->len >0 ) {
            //printf("wd:%d, path:%s, name = %s\n", event->wd, mFileDirMap[event->wd].c_str(), event->name);
        }
        else
            break;

        string pathname=mFileDirMap[event->wd] +string("/") +  string(event->name);

        /*
        printf("mask = ");
        if (event->mask & IN_ACCESS)        printf("IN_ACCESS ");
        if (event->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
        if (event->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
        if (event->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
        if (event->mask & IN_CREATE)        printf("IN_CREATE ");
        if (event->mask & IN_DELETE)        printf("IN_DELETE ");
        if (event->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
        if (event->mask & IN_IGNORED)       printf("IN_IGNORED ");

        if (event->mask & IN_MODIFY)        printf("IN_MODIFY ");
        if (event->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
        if (event->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
        if (event->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
        if (event->mask & IN_OPEN)          printf("IN_OPEN ");
        if (event->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
        if (event->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
        */

        if (event->mask & IN_ISDIR) {
            InitAllWds(pathname.c_str());
        }

        if(event->mask & (IN_MODIFY))
        {
            if (mObserver) {
                mObserver->onDefenEvent(DefenObserver::DEFEN_FILE_WRITE, pathname.c_str());
            }
        }

        event_size = sizeof(struct inotify_event) + event->len;
        ret -= event_size;
        event_pos += event_size;
    }
    return 0;
}

void DefenModProxy::InitAllWds(const char *pcName, bool bRecursion)
{
    int iWd;
    struct stat tStat;
    DIR *pDir;
    struct dirent *ptDirent;
    char caNametmp[4096]; // 存储目录名字

    //printf("add watch:%s, %d\n", pcName, miCount);
    if (-1 == lstat(pcName, &tStat)) {
        LOG_ZDFY("InitAllWds: stat %s error\n", pcName);
        return;
    }
    if (!S_ISDIR(tStat.st_mode))
        return;
    if(S_ISLNK(tStat.st_mode))
    {
        LOG_ZDFY("InitAllWds: pcName is symbolic link!\n");
        return;
    }

    if(miCount >= (MAX_FILES_MONITOR-1)){
        LOG_ZDFY("InitAllWds: inotify_add_watch num:%d , add dir path:%s error!\n", miCount, pcName);
        return;
    }

    //printf("inotify_add_watch :%s !\n", pcName);
    iWd = inotify_add_watch(miNotifyFd, pcName, IN_ALL_EVENTS);
    miWds[miCount] = iWd;
    mFileDirMap[iWd] = pcName;
    miCount++;

    if(bRecursion)
        return;
    // 处理子目录
    pDir = opendir(pcName);
    if (NULL == pDir) {
        printf("opendir %s error\n", pcName);
        return;
    }

    // 循环读目录下面的子项
    while (NULL != (ptDirent = readdir(pDir))) {
        if ((0 == strcmp(ptDirent->d_name, ".")) || (0 == strcmp(ptDirent->d_name, "..")))
            continue; // 跳过当前目录和上一级父目录

        if(strlen(pcName) + strlen(ptDirent->d_name) > 4096)
            break;
        sprintf(caNametmp, "%s/%s", pcName, ptDirent->d_name); //获取子目录或文件名字
        if (-1 == stat(caNametmp, &tStat)) {
            printf("stat error:%s\n", caNametmp); // 获取统计数据
            return;
        }
        if (!S_ISDIR(tStat.st_mode)) //看是否是子目录，原则只处理目录
            continue;

        InitAllWds(caNametmp); //处理子目录
    }

    // 关闭
    closedir(pDir);
}

int DefenModProxy::InitNotify(){
    miNotifyFd = inotify_init1(IN_NONBLOCK);
    if (miNotifyFd == -1) {
        printf("inotify_init error!\n");
        return -1;
    }

    list<string> &fl = mObserver->GetFolderMonitorList();

    for(list<string>::iterator it=fl.begin(); it!=fl.end(); it++)
    {
        string path, name;
        Utils::Path::SplitPath(*it, path, name);

        LOG_ZDFY("add InitNotify %s", path.c_str());
        InitAllWds(path.c_str(), true);
    }


    size_t lens = sizeof(paths)/ sizeof(char*);
    for(size_t i=0; i< lens; i++){
        LOG_ZDFY("add paths:%s add child dir", paths[i]);
        InitAllWds(paths[i]);
    }

    miFdsNum = 1;
    mStatus = 1;

    mFds[0].fd = miNotifyFd;
    mFds[0].events = POLLIN;

    return 0;
}


#else
void DefenModProxy::WatchFanotifyEvents(int fd) {
    char buffer[FANOTIFY_BUFFER_SIZE];
    ssize_t length;

    if ((length = read(fd, buffer, FANOTIFY_BUFFER_SIZE)) > 0) {
        struct fanotify_event_metadata *event;
        event = (struct fanotify_event_metadata *) buffer;

        while (FAN_EVENT_OK (event, length)) {
			char  path[PATH_MAX] = {0}, *buf_path = 0;

        /*
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
        */
        if((event->mask & FAN_MODIFY))
        {
			if(mPid == event->pid)
			{
				close(event->fd);
				event = FAN_EVENT_NEXT (event, length);
				continue;
			}

            if (mObserver) 
			{
				buf_path = get_file_path_from_fd(event->fd, path, PATH_MAX);
				if(buf_path)
				{
					if(!strcmp(buf_path, MOUNTED)) SetNotifyDirList();
					mObserver->onDefenEvent(DefenObserver::DEFEN_FILE_WRITE, path, (void*)(long)event->pid);
				}
            }
        }
		if((event->mask & FAN_OPEN_PERM))
		{
			int    nDeny = 0;
			struct fanotify_response res = {0};

			if (mObserver && mPid != event->pid)
			{
				buf_path = get_file_path_from_fd(event->fd, path, PATH_MAX);
				if(buf_path) nDeny = mObserver->onDefenEvent(DefenObserver::DEFEN_FILE_ACCESS, path, (void*)(long)event->pid);
			}
			res.fd = event->fd;
			res.response = nDeny ? FAN_DENY:FAN_ALLOW;
			write(fd, &res, sizeof(res)); 
		}

        close(event->fd);
        event = FAN_EVENT_NEXT (event, length);
    }
    }else{
        LOG_ZDFY("error read(fds[FD_POLL_FANOTIFY].fd");
        return;
    }
}

void DefenModProxy::ShutdownFanotify() {
	if(miNotifyFd) close(miNotifyFd); miNotifyFd = 0;
	if(miAccessFd) close(miAccessFd); miAccessFd = 0;
	m_lstDir.clear();
}

int  DefenModProxy::SetNotifyDirList()
{
	std::list<std::string> lstDir, lstDel, lstAdd;

	if(miNotifyFd < 0 && miAccessFd < 0) return -1;

	GetMountedDirList(lstDir);
	if(lstDir.empty()) lstDir.push_back("/");

	pthread_mutex_lock(&mMutex);
	for(std::list<std::string>::iterator it1 = m_lstDir.begin(); it1 != m_lstDir.end(); it1++)
	{
		bool bFind = false;
		for(std::list<std::string>::iterator it2 = lstDir.begin(); it2 != lstDir.end(); it2++)
		{
			if(!strcmp(it1->c_str(), it2->c_str())) { bFind = true; break; }
		}

		if(!bFind) lstAdd.push_back(*it1);
	}
	
	for(std::list<std::string>::iterator it1 = lstDir.begin(); it1 != lstDir.end(); it1++)
	{
		bool bFind = false;
		for(std::list<std::string>::iterator it2 = m_lstDir.begin(); it2 != m_lstDir.end(); it2++)
		{
			if(!strcmp(it1->c_str(), it2->c_str())) { bFind = true; break; }
		}

		if(!bFind) lstAdd.push_back(*it1);
	}

	if(!lstDel.empty() || !lstAdd.empty()) m_lstDir = lstDir;
	pthread_mutex_unlock(&mMutex);

	for(std::list<std::string>::iterator it = lstDel.begin(); it != lstDel.end(); it++)
	{
		if(miNotifyFd > 0) fanotify_mark(miNotifyFd, FAN_MARK_REMOVE | FAN_MARK_DONT_FOLLOW | FAN_MARK_MOUNT, FAN_EVENT_ON_CHILD | FAN_MODIFY, -1, it->c_str());
		if(miAccessFd > 0) fanotify_mark(miAccessFd, FAN_MARK_REMOVE | FAN_MARK_DONT_FOLLOW | FAN_MARK_MOUNT, FAN_EVENT_ON_CHILD | FAN_MODIFY, -1, it->c_str());
	}

	for(std::list<std::string>::iterator it = lstAdd.begin(); miNotifyFd > 0 && it != lstAdd.end(); it++)
	{
		if(miNotifyFd > 0) fanotify_mark(miNotifyFd, FAN_MARK_ADD | FAN_MARK_DONT_FOLLOW | FAN_MARK_MOUNT, FAN_EVENT_ON_CHILD | FAN_MODIFY, -1, it->c_str());
		if(miAccessFd > 0) fanotify_mark(miAccessFd, FAN_MARK_ADD | FAN_MARK_DONT_FOLLOW | FAN_MARK_MOUNT, FAN_EVENT_ON_CHILD | FAN_MODIFY, -1, it->c_str());
	}

	return 0;
}

int  DefenModProxy::InitializeFanotify() {
	struct stat s;

	mPid = getpid();
	load_fanotify_funcs();
	if (!fanotify_init)
	{
		LOG_ZDFY("error to find fanotify lib!!!");
		return -1;
	}

	if ((miNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_NONBLOCK | FAN_UNLIMITED_MARKS | FAN_CLASS_NOTIF, O_RDONLY | O_CLOEXEC | O_LARGEFILE)) < 0)
	{
		LOG_ZDFY("error Couldn't setup new InitializeFanotify device: %s", strerror(errno));
		return -1;
	}

    miFdsNum = 1; mStatus = 1;
    mFds[0].fd = miNotifyFd;
    mFds[0].events = POLLIN;

	if(stat("/opt/BDFZ/RAV/plugins/filemon.access.1", &s) != -1)
	{
		miAccessFd = fanotify_init(FAN_CLOEXEC | FAN_NONBLOCK | FAN_CLASS_CONTENT, O_RDONLY | O_CLOEXEC | O_LARGEFILE);
		if(miAccessFd >= 0)
		{
			mFds[1].fd = miAccessFd;
			mFds[1].events = POLLIN;
			miFdsNum = 2;
		}
	}

	// notify dirs
	SetNotifyDirList();
	LOG_ZDFY(">> init fanotify ok --->%d,%d", miFdsNum, m_lstDir.size());
    return miNotifyFd;
}


#endif
