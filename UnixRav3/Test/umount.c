#include <sys/mount.h>  
  
int main(void)  
{  
    int ret = 0;  
    ret = umount("/mnt/sdcard");  
    if(ret == 0)  
    printf("umount success!\n");  
    else  
    printf("umount failed!\n");  
    return 0;  
} 
