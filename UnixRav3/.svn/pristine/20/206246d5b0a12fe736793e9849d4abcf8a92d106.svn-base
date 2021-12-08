#include "devicemonitor.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

DeviceMonitor::DeviceMonitor()
{

}

int DeviceMonitor::InstallUdevRule(const char *mount_path, const char *checker) {
    char content[256];
    sprintf(content, "ACTION==\"add\",KERNEL==\"sd[a-z][0-9]\",RUN+=\"%s %%k\",RUN+=\"%s\"", mount_path, checker);

    if(0 == access("/etc/udev/rules.d", F_OK|W_OK)) {
        FILE *file = fopen("/etc/udev/rules.d/99-rav-monitor.rules", "w+");
        if (file) {
            fputs(content, file);
            fclose(file);
            return 0;
        }
    }
    return -11;
}

int DeviceMonitor::InstallMountCommand(const char *command_path, const char *mount_dest) {
    char content[256];
    sprintf(content, "mount -t vfat /dev/$1 %s\nsync\n", mount_dest);

    if (0 == access(command_path, F_OK|W_OK)) {
        char command[128];
        sprintf(command, "%s/mount_device.sh", command_path);
        FILE *file = fopen(command, "w+");
        if (file) {
            fputs(content, file);
            fclose(file);

            return chmod(command, 0755);
        }
    }
    return -1;
}

