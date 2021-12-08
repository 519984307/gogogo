    #include <sys/mount.h>  
      
    int main(void)  
        {  
               int ret = 0;  
               ret = mount("/dev/sdd4","/mnt/sdcard","vfat",MS_SYNCHRONOUS,NULL);  
               if(ret == 0)  
               printf("mount success!\n");  
               else  
               printf("mount failed!\n");  
               return 0;  
            }  
