/*
 * Common "util" functions
 * This file is part of the dmidecode project.
 *
 *   (C) 2002-2005 Jean Delvare <khali@linux-fr>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *   For the avoidance of doubt the "preferred form" of this code is one which
 *   is in an open unpatent encumbered format. Where cryptographic key signing
 *   forms part of the process of creating an executable the information 
 *   including keys needed to generate an equivalently functional executable
 *   are deemed to be part of the source code.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"

#ifdef USE_MMAP
#include <sys/mman.h>
#ifndef MAP_FAILED
#define MAP_FAILED ((void *) -1)
#endif /* !MAP_FAILED */
#endif /* USE MMAP */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "types.h"
#include "util.h"

#ifndef USE_MMAP
static int myread(int fd, u8 *buf, size_t count, const char *prefix)
{
	ssize_t r=1;
	size_t r2=0;
	
	while(r2!=count && r!=0)
	{
		r=read(fd, buf+r2, count-r2);
		if(r==-1)
		{
			if(errno!=EINTR)
			{
				close(fd);
				perror(prefix);
				return -1;
			}
		}
		else
			r2+=r;
	}
	
	if(r2!=count)
	{
		close(fd);
		fprintf(stderr, "%s: Unexpected end of file\n", prefix);
		return -1;
	}
	
	return 0;
}
#endif

int checksum(const u8 *buf, size_t len)
{
	u8 sum=0;
	size_t a;
	
	for(a=0; a<len; a++)
		sum+=buf[a];
	return (sum==0);
}

/*
 * Copy a physical memory chunk into a memory buffer.
 * This function allocates memory.
 */
void *mem_chunk(size_t base, size_t len, const char *devmem)
{
	void *p;
	int fd;
#ifdef USE_MMAP
	size_t mmoffset;
	void *mmp;
#endif
	
	if((fd=open(devmem, O_RDONLY))==-1)
	{
		perror(devmem);
		return NULL;
	}
	
	if((p=malloc(len))==NULL)
	{
		perror("malloc");
		return NULL;
	}
	
#ifdef USE_MMAP
#ifdef _SC_PAGESIZE
	mmoffset=base%sysconf(_SC_PAGESIZE);
#else
	mmoffset=base%getpagesize();
#endif /* _SC_PAGESIZE */
	/*
	 * Please note that we don't use mmap() for performance reasons here,
	 * but to workaround problems many people encountered when trying
	 * to read from /dev/mem using regular read() calls.
	 */
	mmp=mmap(0, mmoffset+len, PROT_READ, MAP_SHARED, fd, base-mmoffset);
	if(mmp==MAP_FAILED)
	{
		fprintf(stderr, "%s: ", devmem);
		perror("mmap");
		free(p);
		return NULL;
	}
	
	memcpy(p, (u8 *)mmp+mmoffset, len);
	
	if(munmap(mmp, mmoffset+len)==-1)
	{
		fprintf(stderr, "%s: ", devmem);
		perror("munmap");
	}
#else /* USE_MMAP */
	if(lseek(fd, base, SEEK_SET)==-1)
	{
		fprintf(stderr, "%s: ", devmem);
		perror("lseek");
		free(p);
		return NULL;
	}
	
	if(myread(fd, p, len, devmem)==-1)
	{
		free(p);
		return NULL;
	}
#endif /* USE_MMAP */
	
	if(close(fd)==-1)
		perror(devmem);
	
	return p;
}

void EncodeString(char * lpszText, char**lpszReturn,char *lpszKey)
{
    int nTextLen = 0;
    char *cPos = NULL;
    char *pDest = NULL;
    if(lpszReturn)    // 加密
    {
        nTextLen = strlen(lpszText);
	pDest = (char *) malloc(sizeof(char) *(nTextLen + 3));
    }
    else    // 解密
    {
        // 查找自定的中止标记
        cPos = lpszText;
        while(1)    // 从这里可以看到，除非搜索到我们自定的中止标记，否则会一直搜索下去
        {
            if(*cPos == '=')
                if(cPos[1] == '=')
                    if(cPos[2] == '\0')
                        break;
            cPos++;
        }
        if(!cPos)    // 没有找到结束标记，也不是加密
            return;
        nTextLen = cPos - lpszText;
        pDest = lpszText;
    }

    int nKeyLen = strlen(lpszKey);
    int i = 0;
    int k = 0;
    int t = nTextLen;
    int cn = 0;
    int a;
    for(a = 0; a < nKeyLen; a++)    // 将密钥所有值加起来
        cn += lpszKey[a];

    for(; i < nTextLen; i++)
    {
        if(lpszReturn)    // 加密
        {
            pDest[i] = lpszText[i] + t;
            pDest[i] = pDest[i] ^ lpszKey[k];
            pDest[i] = pDest[i] ^ cn;
        }
        else    // 解密，顺序与加密时相反
        {
            pDest[i] = lpszText[i] ^ cn;
            pDest[i] = pDest[i] ^ lpszKey[k];
            pDest[i] = pDest[i] - t;
        }
        k++;
        t--;
        if(k >= nKeyLen)
            k = 0;
        if(t <= 0)
            t = nTextLen;
    }

    if(!cPos)
    {
        memcpy(pDest + nTextLen, "==\0", 3 * sizeof(char));
        *lpszReturn = pDest;
    }
    else
        memset(pDest  + nTextLen, '\0', sizeof(char));
}
