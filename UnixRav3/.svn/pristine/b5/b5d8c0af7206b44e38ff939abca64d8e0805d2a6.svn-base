#ifndef __ZIPCFG_H
#define __ZIPCFG_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "zlib.h"
typedef struct tagZipcfgHead
{
	char label[12];
	
	int nSize;
}ZIPCFGHEAD;

int ZipCFG(unsigned char *,unsined long*,unsigned char *,unsigned long);
int UnZipCFG(unsigned char *,unsined long*,unsigned char *,unsigned long);

#endif
