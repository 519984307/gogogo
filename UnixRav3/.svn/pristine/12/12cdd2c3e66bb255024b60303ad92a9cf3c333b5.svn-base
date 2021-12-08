#ifndef USBPLUGINCHECKER_H
#define USBPLUGINCHECKER_H

#include <pthread.h>
#include <string>
#include <map>
#include <vector>

#ifdef USB_CHECK_BY_QT
    #include <QObject>
#endif

class USBPluginObserver {
public:
    enum PluginEvent {
        USB_ADD = 0,
        USB_REMOVED
    };


public:
    virtual int OnPluginEvent(enum PluginEvent event, const char *mapping) = 0;
};

using namespace std;

class MutexLocker;
class USBPluginChecker
#if defined(USB_CHECK_BY_QT)
                    : public QObject
#endif
{
#ifdef USB_CHECK_BY_QT
    Q_OBJECT
#endif
public:
    static USBPluginChecker* NEW(USBPluginObserver *observer, const string& mountpath, const string& current_user, int arch_bits=0);
    ~USBPluginChecker();

    int Start();
    int Stop();

    bool GetUDiskPath(vector<string>& path);
protected:
#ifdef USB_CHECK_BY_QT

#endif

private:
    USBPluginChecker(USBPluginObserver *observer, const string& mountpath, const string& current_user, int arch_bits);
    int Construct();

    static void* Work(void *);
    void Function();

    char *GetBlockMapPath(const char *source, char *path);
    string GetMountPath(const char *mapping, const char *mount_path);

    bool FindUdevPath(std::string& path);
    int GetUnameInfo(char *uname, size_t length);
    int RefreshUDiskPath();

    string GetMountChangePath(USBPluginObserver::PluginEvent state);


    typedef enum {
        FS_NTFS,
        FS_FAT32,
        FS_EXT2,
        FS_ERROR_TYPE,
    }FILE_SYS_TYPE;
    FILE_SYS_TYPE GetFSType(int devFD);
private:
    struct udev_monitor *mMonitor;
    struct udev *mUdev;

    pthread_t mWorkThread;

    int mStatus;
    int mArchBits;

    USBPluginObserver   *mObserver;

    map<string, bool> mUDiskMountPath;

    const string mDefaultPath;
    const string mCurrentUser;

    vector<string>  mMounted;

    bool   mMountState;

    pthread_t   mCheckThread;
    static void* CheckThread(void *);
    int CheckFunc();

    MutexLocker *mMountLocker;


#ifdef USB_CHECK_BY_DYNAMIC
    void *mDynamicHandler;
#endif
};

#endif // USBPLUGINCHECKER_H
