#include "usbpluginchecker.h"
#include "commonapi.h"
#include "commonlocker.h"
#include "baseconfig.h"

#ifdef USB_CHECK_BY_UDEV
    #include <libudev.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#if defined(COMMON_LINUX)
#include <sys/mount.h>
#endif

USBPluginChecker* USBPluginChecker::NEW(USBPluginObserver *observer, const string& mountpath, const string& current_user, int arch_bits) {
    USBPluginChecker *self = new USBPluginChecker(observer, mountpath, current_user, arch_bits);
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

USBPluginChecker::USBPluginChecker(USBPluginObserver *observer, const string& mountpath, const string& current_user, int arch_bits)
                : mCurrentUser(current_user), mDefaultPath(mountpath)
{
    mUdev = NULL;
    mMonitor = NULL;

#if defined (USB_CHECK_BY_DYNAMIC)
    mDynamicHandler = NULL;
#endif

    mMountLocker = NULL;
    mMountState = false;
    mStatus = 0;
    mObserver = observer;
    mArchBits = arch_bits;
    if(mArchBits == 0) {
        mArchBits = 64;
    }
}

USBPluginChecker::~USBPluginChecker() {
#if defined(USB_CHECK_BY_UDEV)
    if (mMonitor) {
        udev_monitor_unref(mMonitor);
    }
    if (mUdev) {
        udev_unref(mUdev);
    }
#elif defined (USB_CHECK_BY_DYNAMIC)
    if (mMountLocker) {
        delete mMountLocker;
        mMountLocker = NULL;
    }

    if (mMonitor) {
        typedef struct udev_monitor* (*udev_monitor_unref)(struct udev_monitor *);
        udev_monitor_unref udev_monitor_unref_p = (udev_monitor_unref)dlsym(mDynamicHandler, "udev_monitor_unref");
        udev_monitor_unref_p(mMonitor);
    }
    if (mUdev) {
        typedef  struct udev* (*udev_unref)(struct udev *);
        udev_unref udev_unref_p = (udev_unref)dlsym(mDynamicHandler, "udev_unref");
        udev_unref_p(mUdev);
    }

    if (mDynamicHandler) {
        dlclose(mDynamicHandler);
    }
#endif
}

int USBPluginChecker::Construct() {
    if (getuid() != 0) {
        return -2;
    }

    mMountLocker = MutexLocker::NEW();
    Utils::Path::CheckPath(mDefaultPath);

#if defined (USB_CHECK_BY_UDEV)
    mUdev = udev_new();
    if (mUdev == NULL) {
        return -1;
    }

//    /* set signal handlers */
//    struct sigaction act;
//    memset(&act, 0x00, sizeof(struct sigaction));
//    act.sa_handler = (void (*)(int)) sig_handler;
//    sigemptyset(&act.sa_mask);
//    act.sa_flags = SA_RESTART;
//    sigaction(SIGINT, &act, NULL);
//    sigaction(SIGTERM, &act, NULL);

#elif defined(USB_CHECK_BY_DYNAMIC)
    std::string udevso;
    if (!FindUdevPath(udevso)) {
        return -1;
    }

    LOG_GJCZ("find the udev path %s", udevso.c_str());

    mDynamicHandler = dlopen(udevso.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (NULL == mDynamicHandler) {
        return -1;
    }

    typedef struct udev* (*udev_construct)();
    udev_construct construct;
    construct = (udev_construct)dlsym(mDynamicHandler, "udev_new");
    if (NULL == construct) {
        return -1;
    }

    mUdev = construct();
    if (mUdev == NULL) {
        return -1;
    }
#endif

    return 0;
}


int USBPluginChecker::Start() {
    mStatus = 1;

#if defined (USB_CHECK_BY_UDEV)
    mMonitor = udev_monitor_new_from_netlink(mUdev, "udev");
    if (NULL == mMonitor) {
        return -1;
    }

    udev_monitor_filter_add_match_subsystem_devtype(mMonitor, "block", NULL);
#elif defined (USB_CHECK_BY_DYNAMIC)
    typedef  struct udev_monitor* (*udev_monitor_p)(struct udev*, const char*);
    udev_monitor_p monitor_pointer = (udev_monitor_p)dlsym(mDynamicHandler, "udev_monitor_new_from_netlink");
    if (NULL == monitor_pointer) {
        return -1;
    }
    mMonitor = monitor_pointer(mUdev, "udev");
    if (NULL == mMonitor) {
        return -1;
    }

    typedef int (*udev_filter)(struct udev_monitor *,const char *, const char *);
    udev_filter filter_pointer = (udev_filter)dlsym(mDynamicHandler, "udev_monitor_filter_add_match_subsystem_devtype");
    if (filter_pointer) {
        filter_pointer(mMonitor, "block", NULL);
    }
#endif

    return pthread_create(&mWorkThread, NULL, Work, this);
}

int USBPluginChecker::Stop() {
    mStatus = 0;
#ifdef USB_CHECK_BY_UDEV
    if (mMonitor) {
        udev_monitor_unref(mMonitor);
        mMonitor = NULL;
    }
#elif defined (USB_CHECK_BY_DYNAMIC)
    if (mMonitor) {
        typedef struct udev_monitor* (*udev_monitor_unref)(struct udev_monitor *);
        udev_monitor_unref udev_monitor_unref_p = (udev_monitor_unref)dlsym(mDynamicHandler, "udev_monitor_unref");
        udev_monitor_unref_p(mMonitor);
        mMonitor = NULL;
    }
#endif

    pthread_cancel(mWorkThread);
    return pthread_join(mWorkThread, NULL);
    return 0;
}

void* USBPluginChecker::Work(void *args) {
    USBPluginChecker *checker = (USBPluginChecker*)args;
    checker->Function();
    return NULL;
}

char *USBPluginChecker::GetBlockMapPath(const char *source, char *path) {
    char *last = strrchr((char*)source, '/');
    if (last) {
        sprintf(path, "/dev%s", last);
    }
    return path;
}

string USBPluginChecker::GetMountPath(const char *mapping, const char *mount_path) {
#ifdef SECRET_MACHINE
    const char *RED_DISK[] = {"RedDisk", "reddisk0"};

    sleep(3);
    string uri = "/media/RedDisk";
    if (Utils::Path::PathExists(uri)) {
        return uri;
    }

    for (int i=0; i<sizeof(RED_DISK)/sizeof(const char*); ++i) {
        uri = Utils::Path::JoinPath("/media", mCurrentUser.c_str(), RED_DISK[i], NULL);
        if (Utils::Path::PathExists(uri)) {
            return uri;
        }
    }
    return "";
#else
    // first, find system mount
    sleep(3); //sleep 5 sec, let system mount first
    string path = GetMountChangePath(USBPluginObserver::USB_ADD);

    LOG_GJCZ("get the USB folder %s", path.c_str());

    if (path.empty()) {
        // second, system not mount or mount err, maunal mount
        char fsdesc[32] = {0};
        int fd = open(mapping, O_RDONLY);
        if (fd > 0) {
            FILE_SYS_TYPE ftype = GetFSType(fd);
            close(fd);

            switch (ftype) {
            case FS_EXT2:
                strcpy(fsdesc, "ext2");
                break;
            case FS_NTFS:
                strcpy(fsdesc, "ntfs");
                break;
            case FS_FAT32:
                strcpy(fsdesc, "vfat");
                break;
            default:
                break;
            }
        }

        if (strlen(fsdesc) == 0) {
            return "";
        }
#if defined (COMMON_LINUX)
        if (mount(mapping, mount_path, fsdesc, MS_SYNCHRONOUS, NULL) == 0) {
            mMountState = true;
            return mount_path;
        }
#endif
    }
    return path;
#endif
}

bool USBPluginChecker::GetUDiskPath(vector<string>& paths) {
    FILE *fp = NULL;
    if((fp=popen("mount -v","r"))==NULL) {
        return -1;
    }

    char output[1024];
    do {
        memset(output, 0, 1024);
        if (fgets(output, sizeof(output)-1, fp) == NULL) {
            pclose(fp);
            fp = NULL;
            break;
        }

        char buffer[3][128] = {0};
        sscanf(output, "%s on %s type %s", &buffer[0], &buffer[1], &buffer[2]);
        if (0 == strncmp(buffer[0], "/dev", 4)) {
            paths.push_back(buffer[1]);
        }

    }while(!feof(fp));

    if (fp) {
        pclose(fp);
    }
    return paths.size()>0;
}
string USBPluginChecker::GetMountChangePath(USBPluginObserver::PluginEvent state) {
    const char * UPATH[] = {"/media", "/run/media"};

    string ret;
    vector<string> udisks;
    if (GetUDiskPath(udisks)) {
        vector<string>::iterator iter;
        for (iter=udisks.begin(); iter!=udisks.end(); ++iter) {
            for (int i=0; i<sizeof(UPATH)/sizeof(const char*); ++i) {
                if (Utils::String::StartsWith(*iter, UPATH[i])) {
                    if (state == USBPluginObserver::USB_ADD) {
                        if (mUDiskMountPath.find(*iter) == mUDiskMountPath.end()) {
                            mMountLocker->Lock();
                            {
                                mUDiskMountPath.insert(make_pair(*iter, true));
                            }
                            mMountLocker->Unlock();
                            ret = *iter;
                            break;
                        }
                        else if (mUDiskMountPath.empty()) {
                            mMountLocker->Lock();
                            {
                                mUDiskMountPath.insert(make_pair(*iter, true));
                            }
                            mMountLocker->Unlock();
                            ret = *iter;
                        }
                    }
                    else {
                        mMountLocker->Lock();
                        {
                            mUDiskMountPath.insert(make_pair(*iter, false));
                        }
                        mMountLocker->Unlock();
                    }
                }
            }

        }
    }

    if (state == USBPluginObserver::USB_ADD) {
        if (!ret.empty()) {
            return ret;
        }
    }
    else {
        map<string,bool>::iterator iter;
        for (iter=mUDiskMountPath.begin(); iter!=mUDiskMountPath.end(); ++iter) {
            if (iter->second) {
                ret = iter->first;
                return ret;
            }
        }
    }

    return "";
}

int USBPluginChecker::RefreshUDiskPath() {
    const char * UPATH[] = {"/media", "/run/media"};

    mMountLocker->Lock();
    mUDiskMountPath.clear();

    string ret;
    vector<string> udisks;
    if (GetUDiskPath(udisks)) {
        vector<string>::iterator iter;
        for (iter=udisks.begin(); iter!=udisks.end(); ++iter) {
            for (int i=0; i<sizeof(UPATH)/sizeof(const char*); ++i) {
                if (Utils::String::StartsWith(*iter, UPATH[i])) {
                    mUDiskMountPath.insert(make_pair(*iter, true));
                }
            }
        }
    }

    mMountLocker->Unlock();

    return 0;
}



void USBPluginChecker::Function() {
    fd_set readfds;

    // init the cache
    RefreshUDiskPath();

    FD_ZERO(&readfds);    

#ifdef USB_CHECK_BY_DYNAMIC
    typedef int (*udev_monitor_get_fd)(struct udev_monitor *);
    typedef int (*udev_monitor_enable_receiving)(struct udev_monitor *);
    typedef struct udev_device *(*udev_monitor_receive_device)(struct udev_monitor *);
    typedef const char* (*udev_device_get_subsystem)(struct udev_device *);
    typedef const char* (*udev_device_get_action)(struct udev_device *);
    typedef const char* (*udev_device_get_devpath)(struct udev_device *);
    typedef struct udev_device* (*udev_device_unref)(struct udev_device *);
#endif

#if defined (USB_CHECK_BY_UDEV)
    FD_SET(udev_monitor_get_fd(mMonitor), &readfds);
    udev_monitor_enable_receiving(mMonitor);
#elif defined (USB_CHECK_BY_DYNAMIC)
    udev_monitor_get_fd get_fd_pointer = (udev_monitor_get_fd)dlsym(mDynamicHandler, "udev_monitor_get_fd");
    FD_SET (get_fd_pointer(mMonitor), &readfds);

    udev_monitor_enable_receiving recive_poiner = (udev_monitor_enable_receiving)dlsym(mDynamicHandler, "udev_monitor_enable_receiving");
    recive_poiner(mMonitor);
#endif

    while (mStatus > 0) {
#if defined (USB_CHECK_BY_UDEV)
    switch (select(udev_monitor_get_fd(mMonitor)+1, &readfds, NULL, NULL, NULL)) {
#elif defined (USB_CHECK_BY_DYNAMIC)
    switch (select(get_fd_pointer(mMonitor)+1, &readfds, NULL, NULL, NULL)) {
#endif
        case -1:
        case 0:
            continue;
        default: {
#if defined (USB_CHECK_BY_UDEV)
            FD_ISSET(udev_monitor_get_fd(mMonitor), &readfds);
            struct udev_device *device = udev_monitor_receive_device(mMonitor);
            const char *subsys = udev_device_get_subsystem(device);
            const char *action = udev_device_get_action(device);
            const char *path = udev_device_get_devpath(device);
#elif defined (USB_CHECK_BY_DYNAMIC)
            FD_ISSET(get_fd_pointer(mMonitor), &readfds);

            udev_monitor_receive_device udev_monitor_receive_device_p = (udev_monitor_receive_device)dlsym(mDynamicHandler, "udev_monitor_receive_device");
            struct udev_device *device = udev_monitor_receive_device_p(mMonitor);

            udev_device_get_subsystem udev_device_get_subsystem_p = (udev_device_get_subsystem)dlsym(mDynamicHandler, "udev_device_get_subsystem");
            const char *subsys = udev_device_get_subsystem_p(device);

            udev_device_get_action udev_device_get_action_p = (udev_device_get_action)dlsym(mDynamicHandler, "udev_device_get_action");
            const char *action = udev_device_get_action_p(device);

            udev_device_get_devpath udev_device_get_devpath_p = (udev_device_get_devpath)dlsym(mDynamicHandler, "udev_device_get_devpath");
            const char *path = udev_device_get_devpath_p(device);
#endif
            if (strcmp("block", subsys) == 0) {
                char mapping[128];
                GetBlockMapPath(path, mapping);
                char last = mapping[strlen(mapping) -1];
                if (last >= '0' && last <= '9') {
                    if (mObserver) {
                        if (strcmp(action, "add") == 0) {
                            string mpath = GetMountPath(mapping, mDefaultPath.c_str());
                            if (!mpath.empty()) {
                                mMounted.push_back(mpath);
                                mObserver->OnPluginEvent(USBPluginObserver::USB_ADD, mpath.c_str());
                            }

                        }
                        else if (strcmp(action, "remove") == 0) {
                            mObserver->OnPluginEvent(USBPluginObserver::USB_REMOVED, mapping);
#if defined (COMMON_LINUX)
                            if (mMountState) {
                                umount(mDefaultPath.c_str());
                            }
                            pthread_create(&mCheckThread, NULL, CheckThread, this);
#endif
                        }
                    }
                }
            }

#if defined (USB_CHECK_BY_UDEV)
            udev_device_unref(device);
#elif defined (USB_CHECK_BY_DYNAMIC)
            udev_device_unref udev_device_unref_p = (udev_device_unref)dlsym(mDynamicHandler, "udev_device_unref");
            udev_device_unref_p(device);
#endif
            break;
        }
        }
    }
}

void* USBPluginChecker::CheckThread(void *params) {
    USBPluginChecker *self = (USBPluginChecker*)params;
    self->CheckFunc();
    return NULL;
}

int USBPluginChecker::CheckFunc() {
    int i = 0;
    while(i<5) {
        vector<string>::iterator iter;
        for (iter=mMounted.begin(); iter!=mMounted.end(); ++iter) {
            if (!Utils::Path::PathExists(*iter)) {
                mMounted.erase(iter);
                break;
            }
        }
        if (mMounted.empty()) {
            break;
        }
        sleep(1);
    }
    RefreshUDiskPath();
    return 0;
}

bool USBPluginChecker::FindUdevPath(std::string& path) {
    std::vector<std::string> fs;
    Utils::OS::GetLibraryFolder(fs);

    path = Utils::OS::GetSoPath("libudev.so", mArchBits, fs);
    return !path.empty();
}

int USBPluginChecker::GetUnameInfo(char *info, size_t length) {
    struct utsname	name;

    if (uname(&name) == -1) {
        return -1;
    }

    snprintf(info, length, "%s_%s", name.machine, name.sysname);
    return 0;
}

USBPluginChecker::FILE_SYS_TYPE USBPluginChecker::GetFSType(int devFD)
{
    unsigned char tmpBuffer[0x400];
    unsigned char* pOffset = NULL;
    unsigned int readSize = 0;

    pOffset = &tmpBuffer[0];
    memset(pOffset, 0x00, 0x400);

    //read the tag data for ntfs/fat32
    readSize = read(devFD, pOffset, 0x100);
    if(!readSize) {
        return FS_ERROR_TYPE;
    }

    if(!memcmp((const void*)(pOffset+0x52), (const void*)"FAT32", 5)) {
        //th offset with 0x52 is fat32 tag;
        return FS_FAT32;
    }
    else if(!memcmp((const void*)(pOffset+0x3), (const void*)"NTFS", 4)) {
        //the offset with 0x3 is ntfs tag;
        return FS_NTFS;
    }
    else
    {
        memset(pOffset, 0x00, 0x400);
        lseek(devFD,0x400, SEEK_SET); //seek to superblock1;
        readSize = read(devFD, pOffset, 0x400);//read the superblock1 to buffer;
        if(!readSize) {
            return FS_ERROR_TYPE;
        }

        pOffset = pOffset+0x38; //the offset 0x438 is the tag of ext2:0x53 0xef;
        if((pOffset[0] == 0x53) && (pOffset[1] == 0xef)) {
            return FS_EXT2;
        }
    }
    return FS_ERROR_TYPE;
}
