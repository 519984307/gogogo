#pragma once

#include <stdio.h>

#define __DEBUG__  
#ifdef __DEBUG__  
    #define DEBUG(format, args...) fprintf(stdout, format, ##args);fprintf(stdout, "\n");
#else  
    #define DEBUG(format, args...)  
#endif  

#define NIPQUAD(addr) ((unsigned char*)&addr)[0],((unsigned char*)&addr)[1],((unsigned char*)&addr)[2],((unsigned char*)&addr)[3]
