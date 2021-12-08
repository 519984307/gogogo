#ifndef DEFENMODPROXY_H
#define DEFENMODPROXY_H


#define MAX_FILES_MONITOR 4096


#include <pthread.h>
#include <poll.h>

#include <map>
#include <string>

using namespace std;

class DefenObserver;
class DefenModProxy
{
public:
    DefenModProxy();
    virtual ~DefenModProxy();

    void SetObserver(DefenObserver *observer) {
        mObserver = observer;
    }

    int Start();
    int Stop();

    int Status() const {
        return mStatus;
    }

private:

#ifdef FANOTIFY_EXEC
    __pid_t mPid;
    std::list<std::string> m_lstDir;

    int  InitializeFanotify();

    void WatchFanotifyEvents(int fd) ;
    void ShutdownFanotify();

	int  SetNotifyDirList();

#else
    int WatchInotifyEvents(int fd);

    //Recursion 是否递归处理子目录
    void InitAllWds(const char *pcName, bool bRecursion = false);
    int  InitNotify();

#endif

    static void* DefenceWork(void *args);
    void DefenceWorkFunc();

    inline char *get_proc_path(int pid, char *path, int path_len);


private:
    pthread_t   mDefenceThread;

    volatile int mStatus;
    map<string, long> mWriteMap;
    pthread_mutex_t mMutex;

    char mTmpPath[256];

    struct pollfd mFds[2];
    int miNotifyFd, miAccessFd;
    int miWds[MAX_FILES_MONITOR];
    int miCount;
    std::map<int, std::string> mFileDirMap;
    int miFdsNum;

protected:
    DefenObserver   *mObserver;
};


#endif // DEFENMODPROXY_H
