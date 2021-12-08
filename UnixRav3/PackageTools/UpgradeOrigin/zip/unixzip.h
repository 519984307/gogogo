#ifndef _UNIXZIP_HEADER_
#define _UNIXZIP_HEADER_

#include <string.h>
#include <stdio.h>
#include "zlib.h"



#define BUFLEN   16384

class CUNIXZIP
{
public:
	CUNIXZIP();
	~CUNIXZIP();

public:
	
	int gz_compress();
	int gz_bufcompress(unsigned char *bBuf,int len);
	
	int gz_uncompress();
	int gz_bufuncompress(unsigned char *bBuf,int len);
	
	int  gz_init(const char *szIputName,const char *szOutName,bool bUnCompress=false,int nDelete=1);

private:
	char *m_pszInName,*m_pszOutName;
	bool m_nUncompress;  // false －压缩文件，true:解压文件
	int m_nUnlink; // 成功后是否删除源文件
	
	FILE *m_pInOut;  //正常文件句柄
	gzFile m_pZipInOut;  // 压缩文件句柄
};
#endif

