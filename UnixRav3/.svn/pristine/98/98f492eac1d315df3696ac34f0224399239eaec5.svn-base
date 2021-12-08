////////////////////////////////////////////////////////////////////////////////
//	项目名	：瑞星zip文件解压
//	文件名	：unzip.h
//	用  途	：解压zip文件
//	修改者	：唐江杰
//	修改内容：改写以提高速度
//	修改日期：2001-4-22 11:13:18
////////////////////////////////////////////////////////////////////////////////


#ifndef __UNZIP_H__
#define __UNZIP_H__
#include <stdlib.h>
#include "zlib.h"

#define TRUE  1
#define FALSE 0

typedef unsigned int	UINT;
typedef unsigned int 	DWORD;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;

//

typedef short int boolEAN;
//

//typedef void *(*alloc_func)(void *opaque, UINT items, UINT size);
//typedef void   (*free_func)(void *opaque, void *address);
//
//struct z_stream{
//  BYTE    *next_in;  /* next input byte */
//    DWORD     avail_in;  /* number of bytes available at next_in */
//    DWORD    total_in;  /* total nb of input bytes read so far */

//    BYTE    *next_out; /* next output byte should be put there */
//    DWORD     avail_out; /* remaining free space at next_out */
//   DWORD    total_out; /* total nb of bytes output so far */
//
//    char     *msg;      /* last error message, NULL if no error */
//    struct internal_state  *state; /* not visible by applications */
//
//    alloc_func zalloc;  /* used to allocate the internal state */
//    free_func  zfree;   /* used to free the internal state */
//    void *     opaque;  /* private data object passed to zalloc and zfree */
//
//    int     data_type;  /* best guess about the data type: ascii or binary */
//    DWORD   adler;      /* adler32 value of the uncompressed data */
//    DWORD   reserved;   /* reserved for future use */
//};

//#pragma pack(push, oldpack)
#pragma pack(1)

struct FileHeader{
        DWORD	dwHeadMark;//     4 bytes  (0x04034b50)
        WORD	wExVer;//version needed to extract       2 bytes
        WORD	wFlag;//general purpose bit flag        2 bytes
        WORD	wCmt;//compression method              2 bytes
        WORD	wTime;//last mod file time              2 bytes
        WORD	wData;//last mod file date              2 bytes
        DWORD	dwCRC;//crc-32                          4 bytes
        DWORD	dwzipSize;//compressed size                 4 bytes
        DWORD	dwFileSize;//uncompressed size               4 bytes
        WORD	wFileNameLen;//filename length                 2 bytes
        WORD	wExFieldLen;//extra field length              2 bytes
        //filename (variable size)
        //extra field (variable size)
} __attribute__((packed));

//实际没有用到的
struct FileEnd{
		DWORD	dwCRC;//    crc-32                          4 bytes
		DWORD	dwzipSize;//compressed size                 4 bytes
        DWORD	dwFileSize;//uncompressed size               4 bytes
} __attribute__((packed));

//Central directory structure:
struct CentHead{
	DWORD	dwCentHeadMark;//central file header signature 4 bytes (0x02014b50) 
	WORD	wVer;//version made by 2 bytes 
	WORD	wVerNeed;//version needed to extract 2 bytes 
	WORD	wFlag;//general purpose bit flag 2 bytes 
	WORD	wMethod;//compression method 2 bytes 
	WORD	wTime;//last mod file time 2 bytes 
	WORD	wData;//last mod file date 2 bytes 
	DWORD	dwCRC;
	DWORD	dwzibFileSize;//compressed size 4 bytes 
	DWORD	dwFileSize;//uncompressed size 4 bytes 
	WORD	wFileNameLen;//filename length 2 bytes 
	WORD	wExLen;//extra field length 2 bytes 
	WORD	wComLen;//file comment length 2 bytes 
	WORD	wDisk;//disk number start 2 bytes 
	WORD	wIFA;//internal file attributes 2 bytes 
	DWORD	wExFA;//External file attributes 4 bytes 
	DWORD	dwLocalOffset;//relative offset of local header 4 bytes 
//	filename (variable size) 
//	extra field (variable size) 
//file comment (variable size) 
} __attribute__((packed));
//End of central dir record:
struct CentEnd{
	DWORD	dwCentEndMark;//end of central dir signature 4 bytes (0x06054b50) 
	WORD	wDiskn;//number of this disk 2 bytes 
	WORD	wDiskn2;//number of the disk with the start of the central directory 2 bytes 
	WORD	wEntries;//total number of entries in the central dir on this disk 2 bytes 
	WORD	wEntries2;//total number of entries in the central dir 2 bytes 
	DWORD	wSizeofCent;//size of the central directory 4 bytes 
	DWORD	dwCenOffset;//offset of start of central directory
						//	with respect to the starting disk number 4 bytes 
	WORD	wCommLen;//.ZIP file comment length 2 bytes 
	//.ZIP file comment (variable size) 
} __attribute__((packed));

#define ZLIB_VERSION "1.2.3"
#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1 // will be removed, use Z_SYNC_FLUSH instead 
#define Z_SYNC_FLUSH    2
#define Z_FULL_FLUSH    3
#define Z_FINISH        4
#define Z_DEFLATED   8
// Allowed flush values; see deflate() below for details 

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)
#define Z_DEFAULT_STRATEGY    0
#define DEF_MEM_LEVEL 8


#define rs_deflateInit2(strm) \
        deflateInit2_((strm),(-1),(Z_DEFLATED),(-MAX_WBITS),(DEF_MEM_LEVEL),\
                      (Z_DEFAULT_STRATEGY),           ZLIB_VERSION, sizeof(z_stream))

#define MAX_WBITS   15 // 32K LZ77 window 

#define rs_inflateInit2(strm, windowBits) \
        inflateInit2_((strm), (windowBits), ZLIB_VERSION, sizeof(z_stream))

#define ZIP_MAX_BUF	(40*1024)
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void* myalloc(void *opaque, UINT items, UINT size);
void myfree(void *opaque, void *address);


#ifdef __USE_LIB
extern "C"{
DWORD crc32(DWORD crc,BYTE *buf,DWORD len);
int inflate(z_stream *,int);
int inflateInit2_(z_stream *,
						int w,
						const char *,
						int);
int inflateEnd(z_stream *);
	
int deflateInit2_(z_stream *,int,int,int,int,int,const char* ,int);
int deflate(z_stream *,int);
int deflateEnd(z_stream *);
}
#endif // __USE_LIB


class UnZip  
{
	void InitMembers();

	int SetFileEnd(char *szFileName,DWORD dwLen);
	void FMove(FILE *f,DWORD srcOff,DWORD desOff);
	
    bool GetFile(FILE* fpZip,DWORD dwPos,DWORD dwLen,char * szFileName);
    bool DelFileInZip(char *szFileName);
	int GetCHs(char *szZipFile);
	int GetCE(char *szZipFile,CentEnd *lpCE);
	
public:
	DWORD GetCurFileIndex();
	DWORD GetTotalFiles();
    bool SetupZip(char *szZipFileName, char *szSetupPath);
	int DelFile(char *szZipFileName,char *szFileName);
	int AddFileInZip(char *szZipFileName,char *szFileName,char *szFilePath=NULL);
	
    int CreateNewZip(const char *szZipFileName, const char *szFistFileName);
    bool AddFile(const char *szFileName, const char *szFilePath=NULL);
    bool DelCurFile();
    int FindNext(char *szOutFile,char *szFileNameInZip,bool bMakePath=FALSE);
    int FindFirst(char *szZipFile, char *szOutFile,char *szFileNameInZip,bool bMakePath=FALSE);
    bool WriteBack(char *szFileName);

    bool GetFileIsZipType() { return TRUE; }
	char *GetCurZip() { return m_szZipFile; }
	char *GetCurFile() { return m_szCurFileName; }

	UnZip();
	virtual ~UnZip();


private:
    bool GetCurFileName(char *szBase, char *szChilds);
    bool MakePath(char *szPath);
	DWORD dwCurFileIndex;
	DWORD dwTotalFiles;
	char m_szZipFile[300];
	char m_szCurFileName[300];
	FileHeader	m_fh;
	CentHead	m_ch;
	CentEnd		m_ce;
	DWORD dwCEOffset;
	DWORD dwCurFHInCH;
	DWORD dwCurFHInCHSav;
//	DWORD dwCHOffset;
	char *lpCHBuf;
} __attribute__((packed));

//#pragma pack(pop, oldpack)
#pragma pack(8)

#endif //__UNZIP_H__

