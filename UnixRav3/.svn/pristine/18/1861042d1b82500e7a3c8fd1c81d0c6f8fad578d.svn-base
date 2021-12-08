#include "utils.h"

#include <string.h>

 bool ScanUtils::IsSpecialDir(const char *path) {
     if(strcmp(path,"/proc") == 0
             ||strncmp(path,"/proc/",6) == 0) {
         return true;
     }
     else if(strcmp(path,"/dev") == 0
             || strncmp(path,"/dev/",5) == 0) {
         return true;
     }
     else if(strcmp(path, "/sys") == 0
             || strncmp(path, "/sys/", 5) == 0) {
         return true;
     }

     return false;
 }
