#include "unixzip.h"
#include "zlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __MAIN_TEST__
int main(int argc,char *argv[])
{
	int uncompress;

	if(argc!=3 && argc!=4)
	{
		printf("Usage : %s [-d] inputfile outputfile\n");
		exit(0);
	}
	
	uncompress=0;
	if(argc==4)
	{
		if(strcmp(argv[1],"-d"))
		{
			printf("Usage : %s [-d] inputfile outputfile\n");
			exit(0);
		}
		uncompress=1;
		
	}
	
	CUNIXZIP cZip;
	if(uncompress)
	{
		if(cZip.gz_init(argv[2],argv[3],true,0))
		{
			printf("Error init gzip\n");
			return -1;
		}
		
		cZip.gz_uncompress();	
		return 0;
	}


	if(cZip.gz_init(argv[1],argv[2],false,0))
	{
		printf("Error init gzip\n");
		return -1;
	}
	
	cZip.gz_compress();	
	return 0;

}
#endif

CUNIXZIP::CUNIXZIP()
{
	m_pszInName=NULL;
	m_pszOutName=NULL;
	m_pInOut=NULL;
	m_pZipInOut=NULL;
	
}

CUNIXZIP::~CUNIXZIP()
{
	if(m_pszInName)
		delete m_pszInName;

	if(	m_pszOutName)
		delete m_pszOutName;

}


int  CUNIXZIP::gz_init(const char *szIputName,const char *szOutName,bool bUnCompress,int nDelete)
{
	m_nUnlink=nDelete;
	m_nUncompress=bUnCompress;
	
	int nLen;

// 可以不指定文件
	if(szIputName)
	{
		nLen=strlen(szIputName)+1;
		m_pszInName=new char [nLen];
		if(!m_pszInName)
			return -1;
		strcpy(m_pszInName,szIputName);
	}

// 也可以不指定输出文件
	if(!szOutName)
		return -1;
		
	nLen=strlen(szOutName)+1;
	m_pszOutName=new char [nLen];
	if(!m_pszOutName)
		return -1;
	strcpy(m_pszOutName,szOutName);
	
	return 0;

}

int CUNIXZIP::gz_compress()
{
    char buf[BUFLEN];
    int len;

// 必须指定输入文件
	if(!m_pszInName || !m_pszOutName)
		return -1;


    m_pInOut = fopen(m_pszInName, "rb");
    if (m_pInOut == NULL) 
    	return -1;
    	
    m_pZipInOut = gzopen(m_pszOutName, "wb6");
    if (m_pZipInOut == NULL) 
    	return -1;
    

    for (;;)
    {
        len = fread(buf, 1, sizeof(buf), m_pInOut);
        if (len == 0) break;

        if (gzwrite(m_pZipInOut, buf, (unsigned)len) != len) 
        	return -1;
    }
    
    fclose(m_pInOut);
    
    if (gzclose(m_pZipInOut) != Z_OK) 
    	return -1;

	if(m_nUnlink)
    	unlink(m_pszInName);
	return 0;
}

int CUNIXZIP::gz_bufcompress(unsigned char *bBuf,int len)
{
	return 0;
}


int CUNIXZIP::gz_uncompress()
{

	char buf[BUFLEN];
    int len;
    //int err;

	m_pZipInOut = gzopen(m_pszInName, "rb");
	if (m_pZipInOut == NULL)
		return -1;
		
    m_pInOut = fopen(m_pszOutName, "wb");
	if (m_pInOut == NULL)
		return -1;
		

	for (;;) 
	{
		len = gzread(m_pZipInOut, buf, sizeof(buf));
		if (len < 0) return -1;
		if (len == 0) break;
	
	    if ((int)fwrite(buf, 1, (unsigned)len, m_pInOut) != len) 
	    	return -1;
	}
	if (fclose(m_pInOut)) 
		return -1;

	if (gzclose(m_pZipInOut) != Z_OK)
		return -1;
    	
	if(m_nUnlink)
    	unlink(m_pszInName);
	return 0;

}

int CUNIXZIP::gz_bufuncompress(unsigned char *bBuf,int len)
{
	return 0;
}

