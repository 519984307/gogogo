#include "unzip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>


#define RS_SWAPDWORD

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

void RS_GetFileHeader(FileHeader *pHeader);
void RS_GetCentHeader(CentHead *pHeader);
void RS_GetCentEnd(CentEnd *pEnd);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UnZip::UnZip()
{
	InitMembers();
}

void UnZip::InitMembers()
{
	ZeroMemory(&m_fh,sizeof(m_fh));
	ZeroMemory(&m_ce,sizeof(m_ce));
	ZeroMemory(&m_ch,sizeof(m_ch));
	lpCHBuf = NULL;
	dwCurFileIndex = 0;
	dwTotalFiles = 0;
	dwCEOffset = 0;
	dwCurFHInCH = 0;
	dwCurFHInCHSav = 0;
	m_szZipFile[0] = 0;
	m_szCurFileName[0] = 0;
}

UnZip::~UnZip()
{
	
	if(lpCHBuf){
		delete lpCHBuf;
		lpCHBuf=NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	函数名：int UnZip::FindFirst(char *szZipFile, char *szOutFile, int nOutFileLen)
//	用  途：解压第一个文件
//	参  数：szOutFile:解压路径 //OUT 为文件名
//			nOutFileLen:解压路径缓冲长度
//			szZipFile :ZIP文件名
//	返回值：int 1 成功,0 失败
////////////////////////////////////////////////////////////////////////////////


int UnZip::FindFirst(char *szZipFile, char *szOutFile, char *szFileNameInZip,bool bMakePath)
{
	//reset all member var
	InitMembers();

	sprintf(m_szZipFile,"%s",szZipFile);
	if(this->GetCE(m_szZipFile,&m_ce))
		return 0;
	dwTotalFiles=m_ce.wEntries;
	if(GetCHs(m_szZipFile)<0)
		return 0;
	return FindNext(szOutFile,szFileNameInZip,bMakePath);
}

////////////////////////////////////////////////////////////////////////////////
//	函数名：int UnZip::FindNext(char *szOutFile, int nOutFileLen)
//	用  途：解压下一个文件
//	参  数：szOutFile: //OUT 为文件名
//			nOutFileLen:szOutFile缓冲长度
//	返回值：int 1 成功,0 失败 或者ZIP文件结束
////////////////////////////////////////////////////////////////////////////////

int UnZip::FindNext(char *szOutFile, char *szFileNameInZip,bool bMakePath)
{
	if(dwCurFileIndex>=dwTotalFiles)//end of all files
		return 0;
	if(lpCHBuf==NULL)
		return 0;
	//if get next fh err;
	FILE *ZipFile=fopen(m_szZipFile,"rb");
	if(ZipFile==NULL)//err
		return 0;

	CentHead *lpch;
	CentHead chCur;
	dwCurFHInCHSav=dwCurFHInCH;
	//find a real file
	while(1){	
		lpch=(CentHead *)(lpCHBuf+dwCurFHInCH);

		memcpy(&chCur, lpch, sizeof(CentHead)); ///

	// Add by Jim 2001-12-4 16:44:23 < 
		RS_GetCentHeader(&chCur);
	// Add by Jim 2001-12-4 16:44:23 > 

		if(chCur.dwCentHeadMark!=0x02014b50){
			fclose(ZipFile);
			return 0;
		}
		if(chCur.wFileNameLen>300){
			fclose(ZipFile);
			return 0;
		}
		//get FileNameInZip
		char *ptr=(char *)lpch;
		strncpy(szFileNameInZip,ptr+sizeof(CentHead),chCur.wFileNameLen);
		szFileNameInZip[chCur.wFileNameLen]=0;
#ifdef _WINDOWS
		if(szFileNameInZip[chCur.wFileNameLen-1]=='/'||szFileNameInZip[chCur.wFileNameLen-1]=='\\'){
#else
		if(szFileNameInZip[chCur.wFileNameLen-1]=='/'||szFileNameInZip[chCur.wFileNameLen-1]=='\\'){
#endif
			dwCurFHInCH+=sizeof(CentHead);
			dwCurFHInCH+=chCur.wExLen;
			dwCurFHInCH+=chCur.wFileNameLen;
			dwCurFHInCH+=chCur.wComLen;
			dwCurFileIndex++;
			if(dwCurFileIndex>=dwTotalFiles){//end of all files
				fclose(ZipFile);
				return 0;
			}
			dwCurFHInCHSav=dwCurFHInCH;
			continue;
		}
		break;
	}

	//read fh
	fseek(ZipFile,chCur.dwLocalOffset,SEEK_SET);
	fread(&m_fh	,sizeof(m_fh),1,ZipFile);
	
	// Add by Jim 2001-12-4 16:24:34 < 
	RS_GetFileHeader(&m_fh);
	// Add by Jim 2001-12-4 16:24:34 > 

	if(m_fh.dwHeadMark!=0x04034b50){
		fclose(ZipFile);
		return 0;
	}

	//read file;
	DWORD dwFileOffset;
	dwFileOffset=chCur.dwLocalOffset;
	dwFileOffset+=sizeof(m_fh);
	dwFileOffset+=m_fh.wFileNameLen;
	dwFileOffset+=m_fh.wExFieldLen;
	
	if(bMakePath){
		char *p;
		p=strrchr(szFileNameInZip,'/');
#ifdef _WINDOWS
		while(p){
			p[0]='\\';
			p=strrchr(szFileNameInZip,'/');
		}
#endif
		GetCurFileName(szOutFile,szFileNameInZip);
        strcpy(szOutFile,m_szCurFileName);
	}
	else{
		sprintf(m_szCurFileName,"%s",szFileNameInZip);
		char *szP=strrchr(szFileNameInZip,'/');
		if(szP==NULL){
			szP=szFileNameInZip;
		}
//	i wonder why do this??
		char *szTempa =strrchr(szP,'/');
		if(szTempa)
			strncat(szOutFile,szTempa+1,1024);
		else
			strncat(szOutFile,szP,1024);
//		strncat(szOutFile,szP,1024);
	}
		
		

	if(m_fh.wCmt==0){// not compress wei just save file
		fseek(ZipFile,dwFileOffset,SEEK_SET);
		FILE *OutFile=fopen(szOutFile,"wb");
		if(OutFile==NULL){
			fclose(ZipFile);
			return 0;
		}
		DWORD dwReaded=0;
		DWORD dwRead=0;
		BYTE *lpZipBuf = NULL;
		lpZipBuf=new BYTE[ZIP_MAX_BUF];
		if( NULL == lpZipBuf )
		{
			fclose(OutFile);
			fclose(ZipFile);
			return -1;
		}
	
		while(dwReaded<m_fh.dwzipSize){
			dwRead=fread(lpZipBuf,1,
					m_fh.dwzipSize-dwReaded>ZIP_MAX_BUF?ZIP_MAX_BUF:m_fh.dwzipSize-dwReaded,
					ZipFile);
	
			if(dwRead==0)
				break;
			dwReaded+=dwRead;
			fwrite(lpZipBuf,dwRead,1,OutFile);
		}
		fclose(OutFile);
		fclose(ZipFile);
		delete lpZipBuf;
		lpZipBuf=NULL;

	
		//move to next CH
		dwCurFHInCH+=sizeof(CentHead);
		dwCurFHInCH+=chCur.wExLen;
		dwCurFHInCH+=chCur.wFileNameLen;
		dwCurFHInCH+=chCur.wComLen;

		dwCurFileIndex++;
		return 1;
	}
	// get compressed file
	GetFile(ZipFile,dwFileOffset,m_fh.dwzipSize,szOutFile);
	fclose(ZipFile);

	//move to next CH
	dwCurFHInCH+=sizeof(CentHead);
	dwCurFHInCH+=chCur.wExLen;
	dwCurFHInCH+=chCur.wFileNameLen;
	dwCurFHInCH+=chCur.wComLen;
	
	dwCurFileIndex++;
	return 1;

}

/////////////////////////////////////////////////////////////////////////////
//函 数 名: GetCE
//功能描述：取得szZipFile所指ZIP文件的CENTEND，
//影响变量：
//参数说明:szZipFile文件名，lpCE，CentEnd指针
//返 回 值：int 0 ok, <0 err
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

int UnZip::GetCE(char *szZipFile,CentEnd *lpCE)
{
	ZeroMemory(lpCE,sizeof(CentEnd));
	FILE *ZipFile=fopen(szZipFile,"rb");
	if(ZipFile==NULL)//err
		return -1;
	fseek(ZipFile,0,SEEK_END);
	long lFileLen=ftell(ZipFile);

	fseek(ZipFile,lFileLen-sizeof(CentEnd),SEEK_SET);
	fread(lpCE,sizeof(CentEnd),1,ZipFile);

	// Add by Jim 2001-12-4 16:25:05 < 
	RS_GetCentEnd(lpCE);
	// Add by Jim 2001-12-4 16:25:05 > 

	if(lpCE->wSizeofCent > lFileLen )
	{
		fclose(ZipFile);
		return 1;
	}

	if(lpCE->dwCentEndMark==0x06054b50){//ok
		fclose(ZipFile);
		return 0;
	}

	long lCheckLen;
	lFileLen>65535+sizeof(CentEnd)?lCheckLen=65535+sizeof(CentEnd):lCheckLen=lFileLen;
	char *buf = NULL;
	buf=new char[lCheckLen];
	if( NULL == buf )
	{
		fclose(ZipFile);
		return -1;
	}

	fseek(ZipFile,-lCheckLen,SEEK_END);
	fread(buf,lCheckLen,1,ZipFile);
	char *ptr=buf;
	while(ptr<=buf+lCheckLen-sizeof(CentEnd)){
		*lpCE=*(CentEnd*)ptr;

	// Add by Jim 2001-12-4 16:25:56 < 
		RS_GetCentEnd(lpCE);
	// Add by Jim 2001-12-4 16:25:56 > 

		if((ptr[0]==0x50)&&(ptr[1]==0x4b)&&(ptr[2]==0x05)&&(ptr[3]==0x06)){//found
			*lpCE=*(CentEnd*)ptr;
			delete buf;
			buf=NULL;
			fclose(ZipFile);
			return 0;
		}
		ptr++;
	}
	delete buf;
	buf=NULL;
	fclose(ZipFile);
	//success
	//*lpCE=somevar
	return -1;
}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: GetCHs
//功能描述：取得szZipFile所指ZIP文件的所有CentHead
//影响变量：lpCHBuf
//参数说明:szZipFile文件名，
//返 回 值：int 0 ok, <0 err
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

int UnZip::GetCHs(char *szZipFile)
{
	FILE *ZipFile=fopen(szZipFile,"rb");
	if(ZipFile==NULL)//err
		return -1;
	fseek(ZipFile,m_ce.dwCenOffset,SEEK_SET);
	if(lpCHBuf){
		delete lpCHBuf;
		lpCHBuf=NULL;
	}
	if(m_ce.wSizeofCent<=0){
		fclose(ZipFile);
		return -1;
	}
	lpCHBuf=new char[m_ce.wSizeofCent];
	if(lpCHBuf==NULL){
		fclose(ZipFile);
		return -1;
	}
	fread(lpCHBuf,m_ce.wSizeofCent,1,ZipFile);
	fclose(ZipFile);
	CentHead *lpCH;
	lpCH=(CentHead*)lpCHBuf;

	// Add by Jim 2001-12-4 16:26:53 < 
//	RS_GetCentHeader(lpCH);
	// Add by Jim 2001-12-4 16:26:53 > 

#ifdef RS_BIG_ENDIAN
	if(lpCH->dwCentHeadMark==0x504B0102)
#else
	if(lpCH->dwCentHeadMark==0x02014b50)
#endif
		return 0;
	delete lpCHBuf;
	lpCHBuf=NULL;
	return -1;
}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: DelFileInZip
//功能描述：把m_szZipFile所指的ZIP包里名称为szFileName文件删除
//影响变量：
//参数说明: szFileName,要删除的文件
//返 回 值：bool
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

bool UnZip::DelFileInZip(char *szFileName)
{
	//find the current file to del
	//lpCHBuf
	CentHead *pch, chCur;
	char *ptr=lpCHBuf;
	DWORD CURSEEK=0;
	if(m_ce.wSizeofCent== 0)
		return false;
	while(CURSEEK<m_ce.wSizeofCent){
		ptr=lpCHBuf+CURSEEK;

	// Add by Jim 2001-12-4 16:56:18 < 
		//	ch=(CentHead*)ptr;
		memcpy(&chCur, ptr, sizeof(CentHead));
		RS_GetCentHeader(&chCur);
	// Add by Jim 2001-12-4 16:56:18 > 

		if(chCur.dwCentHeadMark!=0x02014b50){//can't find 
			return false;
		}
		//get file name and compare
		if(strncmp(szFileName,ptr+sizeof(CentHead),strlen(szFileName))==0){
			break;
		}
		CURSEEK+=sizeof(CentHead)+chCur.wComLen+chCur.wFileNameLen+chCur.wExLen;
	}
	if(CURSEEK>=m_ce.wSizeofCent)
		return false;

	if(chCur.dwCentHeadMark!=0x02014b50)
		return false;
	
	//save CentHead offset
	DWORD dwCentOffset=CURSEEK;
	
	//read zipfile
	FILE *ZipFile;
	ZipFile=fopen(m_szZipFile,"rb+");
	if(ZipFile==NULL)
		return false;
	fseek(ZipFile,0,SEEK_END);
	DWORD dwZipFileSize=ftell(ZipFile);
	
	if(dwZipFileSize<sizeof(CentHead)+sizeof(CentEnd)+sizeof(FileHeader)){
		fclose(ZipFile);
		return false;
	}
	
	// modify
//	fseek(ZipFile,0,SEEK_SET);

	
	// del 4-23
//	char *szFileBuf=new char[dwZipFileSize];
//	fread(szFileBuf,dwZipFileSize,1,ZipFile);
//	fclose(ZipFile);
    // del 4-23

	//save local file head
	FileHeader savfh;
	fseek(ZipFile,chCur.dwLocalOffset,SEEK_SET);
	fread(&savfh,sizeof(savfh),1,ZipFile);

	// Add by Jim 2001-12-4 16:29:28 < 
	RS_GetFileHeader(&savfh);
	// Add by Jim 2001-12-4 16:29:28 > 

	//	savfh=*(FileHeader*)(szFileBuf+ch->dwLocalOffset);
	
	//sav centhead of current file 
	CentHead savch;
//	savch=*ch;

	// Add by Jim 2001-12-7 12:02:23 < 
	// savch = chCur;	// ok too!
	memcpy(&savch,&chCur, sizeof(CentHead));
	// Add by Jim 2001-12-7 12:02:23 > 

	DWORD DelFileSize=sizeof(savfh)+savfh.dwzipSize+savfh.wExFieldLen+savfh.wFileNameLen;
	DWORD DelCentSize=sizeof(CentHead)+savch.wComLen+savch.wFileNameLen+savch.wExLen;
	//del fh and file data
	FMove(ZipFile,savch.dwLocalOffset+DelFileSize,savch.dwLocalOffset);
	//
	
	//fwrite 0->ch->dwLocalOffset
//	ZipFile=fopen(m_szZipFile,"wb");

//	ptr=szFileBuf;
//	if(savch.dwLocalOffset)
//		fwrite(ptr,savch.dwLocalOffset,1,ZipFile);
	//seek +DelFileSize

//	ptr+=DelFileSize;
//	ptr+=savch.dwLocalOffset;
	//fwrite .->old_m_ce.dwCenOffset
//	fwrite(ptr,m_ce.dwCenOffset-DelFileSize-savch.dwLocalOffset,1,ZipFile);
//	delete szFileBuf;
	//write fhs over
	//fwrite chs
	//modify  CentHead		
	CURSEEK=0;
	while(CURSEEK<m_ce.wSizeofCent){
		ptr=lpCHBuf+CURSEEK;
		pch=(CentHead*)ptr;

	// Add by Jim 2001-12-11 15:47:22 < 
		memcpy(&chCur, pch, sizeof(CentHead));
		RS_GetCentHeader(&chCur);
		if(chCur.dwLocalOffset>savch.dwLocalOffset){
			chCur.dwLocalOffset-=DelFileSize;
			pch->dwLocalOffset = RS_SWAPDWORD(chCur.dwLocalOffset);
		}
	// Add by Jim 2001-12-11 15:47:22 > 
		//goto next CentHead or End;
		CURSEEK+=sizeof(CentHead)+chCur.wComLen+chCur.wFileNameLen+chCur.wExLen;
	}
	
	//write CentHead
	fseek(ZipFile,m_ce.dwCenOffset-DelFileSize,SEEK_SET);
	if(dwCentOffset)
		fwrite(lpCHBuf,dwCentOffset,1,ZipFile);

	ptr=lpCHBuf+dwCentOffset+DelCentSize;
	fwrite(ptr,m_ce.wSizeofCent-dwCentOffset-DelCentSize,1,ZipFile);

	//modify all of the CentHead and CentEnd
	m_ce.wEntries--;
	m_ce.wEntries2--;
	m_ce.wSizeofCent-=DelCentSize;
	m_ce.wCommLen=0;
	m_ce.dwCenOffset-=DelFileSize;

	// Add by Jim 2001-12-5 17:58:03 < 
	CentEnd ceCur;
	memcpy(&ceCur, &m_ce, sizeof(CentEnd));
	RS_GetCentEnd(&ceCur);
	fwrite(&ceCur, sizeof(ceCur), 1, ZipFile);
	// Add by Jim 2001-12-5 17:58:03 > 

	//fwrite ce;
	//fwrite(&m_ce,sizeof(m_ce),1,ZipFile);
	
	dwZipFileSize=ftell(ZipFile);
	fclose(ZipFile);
	SetFileEnd(m_szZipFile,dwZipFileSize);
	GetCHs(m_szZipFile);
	dwCurFHInCH=dwCurFHInCHSav;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: GetFile
//功能描述：取得fpZip所指ZIP文件中的一个文件
//影响变量：
//参数说明:fpZip文件指针，
//		dwPos：压缩数据偏移
//		dwLen：生成文件名
//返 回 值：bool
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

bool UnZip::GetFile(FILE *fpZip, DWORD dwPos, DWORD dwLen, char *szFileName)
{
//	__asm int 3
	fseek(fpZip,dwPos,SEEK_SET);
	BYTE zipBuf[ZIP_MAX_BUF];
	BYTE FileBuf[ZIP_MAX_BUF];
	int nReaded;
	
	FILE *OutFile=fopen(szFileName,"wb");
	if(OutFile==NULL)
		return false;
	
	z_stream zs;
	memset(&zs,0,sizeof(zs));
	zs.zfree=myfree;
	zs.zalloc=myalloc;
	
    int ferr = rs_inflateInit2(&zs,-MAX_WBITS);
    if (rs_inflateInit2(&zs,-MAX_WBITS) != Z_OK) {
		return -1;
	}

	while(dwLen>0){
		if(dwLen>ZIP_MAX_BUF)
			nReaded=fread(zipBuf,1,ZIP_MAX_BUF,fpZip);
		else
			nReaded=fread(zipBuf,1,dwLen,fpZip);
		if( 0 == nReaded )
			break;

		dwLen-=nReaded;

		zs.next_in=zipBuf;
		zs.next_out=FileBuf;
		zs.avail_in=nReaded;
		zs.avail_out=ZIP_MAX_BUF;
		zs.adler=0;
		zs.total_out=0;
		while(zs.avail_in>0){
			if(zs.avail_out==0){
				fwrite(FileBuf,1,ZIP_MAX_BUF,OutFile);
				zs.avail_out=ZIP_MAX_BUF;
				zs.next_out=FileBuf;
			}
			int nRet=inflate(&zs,Z_NO_FLUSH);

			//if(nRet!=0){
			if(nRet < 0){
				inflateEnd(&zs);
				fclose(OutFile);
#ifdef _WINDOWS	
				DeleteFile(szFileName);
#else
				unlink(szFileName);
#endif
				return false;
			}
			if( nRet == 1 )
				break;
		}
		if(ZIP_MAX_BUF-zs.avail_out>0)
			fwrite(FileBuf,ZIP_MAX_BUF-zs.avail_out,1,OutFile);
	}
	zs.avail_out=ZIP_MAX_BUF;
	zs.next_out=FileBuf;
	int err=0;
	while(err==0){
		if (zs.avail_out == 0){
            fwrite(FileBuf,1,ZIP_MAX_BUF,OutFile);
			zs.avail_out=ZIP_MAX_BUF;
			zs.next_out=FileBuf;
        }
		err=inflate(&zs,Z_FINISH);
	}
	if(ZIP_MAX_BUF-zs.avail_out)
		fwrite(FileBuf,ZIP_MAX_BUF-zs.avail_out,1,OutFile);

	inflateEnd(&zs);
	fclose(OutFile);
	return true;
}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: myalloc
//功能描述：提供给压缩程序的内存分配函数
//影响变量：
//参数说明:  void *opaque内存链表指针
//			DWORD items	 需要分配结构的大小（按字节计算）
//			DWORD size	需要分配的结构的数目
//返 回 值：void*
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

void* myalloc(void *opaque, UINT items, UINT size)
{
//	nNew++;
	return malloc(size * items);
}

/////////////////////////////////////////////////////////////////////////////
//函 数 名: myfree
//功能描述：提供给压缩程序的内存释放函数
//影响变量：
//参数说明: void *opaque内存链表指针
//			void *address	 需要释放的内存地址
//返 回 值：void
//相关函数： 
/////////////////////////////////////////////////////////////////////////////
#include <ctype.h>


void myfree(void *opaque, void *address)
{
//	nFree++;
	free(address);
//	delete[] address;
}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: AddFile
//功能描述：在m_szZipFile所指的ZIP包里添加文件
//影响变量：
//参数说明: szFileName,要添加的文件

//返 回 值：bool
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

bool UnZip::AddFile(const char *szFileName, const char *szFilePath)
{
	FILE *ZipFile=fopen(m_szZipFile,"rb+");
	if(ZipFile==NULL)//err
		return false;

	FILE *tempFile=fopen(szFileName,"rb");
	if(tempFile==NULL){
		fclose(ZipFile);
		return false;
	}
	fseek(ZipFile,m_ce.dwCenOffset+sizeof(m_fh)+strlen(szFilePath?szFilePath:m_szCurFileName),SEEK_SET);

	BYTE InBuf[ZIP_MAX_BUF];
	BYTE OutBuf[ZIP_MAX_BUF];
	
	DWORD dwCRC=0;
	z_stream zz;
	memset(&zz,0,sizeof(zz));
	
	zz.zfree=myfree;
	zz.zalloc=myalloc;
	if( NULL == zz.zalloc )
	{
		fclose(tempFile);
		fclose(ZipFile);
		return false;
	}
	if (rs_deflateInit2(&zz) != Z_OK) {
		return -1;
	}
	
	int nReaded;
	while(!feof(tempFile)){
		nReaded=fread(InBuf,1,ZIP_MAX_BUF,tempFile);
		if(nReaded==0)
			break;
		dwCRC=crc32(dwCRC,InBuf,nReaded);
		
		zz.next_in=InBuf;
		zz.avail_in=nReaded;
		zz.next_out=OutBuf;
		zz.avail_out=ZIP_MAX_BUF;

		while(zz.avail_in>0){
			if(zz.avail_out==0){
				fwrite(OutBuf,ZIP_MAX_BUF,1,ZipFile);
				zz.avail_out =ZIP_MAX_BUF;
				zz.next_out =OutBuf;
			}
            if (deflate(&zz,Z_NO_FLUSH) != Z_OK) {
                return -1;
            }
		}
		if(ZIP_MAX_BUF-zz.avail_out>0)
			fwrite(OutBuf,ZIP_MAX_BUF-zz.avail_out,1,ZipFile);
	}

	fclose(tempFile);

	zz.next_out=OutBuf;
	zz.avail_out=ZIP_MAX_BUF;
	int err=0;
	while(err==0){
		if (zz.avail_out == 0){
            fwrite(OutBuf,ZIP_MAX_BUF,1,ZipFile);
            zz.avail_out =ZIP_MAX_BUF;
			zz.next_out =OutBuf;
        }
		err=deflate(&zz,Z_FINISH);
	}

	if(ZIP_MAX_BUF-zz.avail_out>0)
		fwrite(OutBuf,ZIP_MAX_BUF-zz.avail_out,1,ZipFile);
	
	DWORD dwCentOffset=ftell(ZipFile);

	memset(&m_fh,0,sizeof(m_fh));
	m_fh.wFlag=0;//6?
	m_fh.dwCRC=dwCRC;
	m_fh.dwHeadMark=0x04034b50;
	m_fh.dwFileSize=zz.total_in;
	m_fh.dwzipSize=zz.total_out;
	if(szFilePath)
		m_fh.wFileNameLen=strlen(szFilePath);
	else
		m_fh.wFileNameLen=strlen(m_szCurFileName);
	m_fh.wCmt=Z_DEFLATED;
	m_fh.wExVer=20;
	
	fseek(ZipFile,m_ce.dwCenOffset,SEEK_SET);

	// Add by Jim 2001-12-5 17:58:03 < 
	FileHeader fhCur;
	memcpy(&fhCur, &m_fh, sizeof(FileHeader));
	RS_GetFileHeader(&fhCur);
	fwrite(&fhCur, sizeof(fhCur), 1, ZipFile);
	// Add by Jim 2001-12-5 17:58:03 > 

	//write fh
//	fwrite(&m_fh,sizeof(m_fh),1,ZipFile);

	//write file name
	if(szFilePath)
		fwrite(szFilePath,strlen(szFilePath),1,ZipFile);
	else
		fwrite(&m_szCurFileName,strlen(m_szCurFileName),1,ZipFile);

	
	//write chs
	fseek(ZipFile,dwCentOffset,SEEK_SET);
	fwrite(lpCHBuf,m_ce.wSizeofCent,1,ZipFile);
	delete lpCHBuf;
	lpCHBuf=NULL;
	//create new ch;
	memset(&m_ch,0,sizeof(m_ch));
	m_ch.dwCentHeadMark=0x02014b50;
	m_ch.wVer=20;
	m_ch.wVerNeed=20;
	m_ch.wMethod=m_fh.wCmt;
	m_ch.dwCRC=m_fh.dwCRC;
	
	m_ch.dwzibFileSize=m_fh.dwzipSize;
	m_ch.dwFileSize=m_fh.dwFileSize;
	if(szFilePath)
		m_ch.wFileNameLen=strlen(szFilePath);
	else
		m_ch.wFileNameLen=strlen(m_szCurFileName);
	
	m_ch.dwLocalOffset=m_ce.dwCenOffset;

	// Add by Jim 2001-12-5 17:58:03 < 
	CentHead chCur;
	memcpy(&chCur, &m_ch, sizeof(CentHead));
	RS_GetCentHeader(&chCur);
	fwrite(&chCur, sizeof(chCur), 1, ZipFile);
	// Add by Jim 2001-12-5 17:58:03 > 

//	fwrite(&m_ch,sizeof(m_ch),1,ZipFile);

	if(szFilePath)
		fwrite(szFilePath,strlen(szFilePath),1,ZipFile);
	else
		fwrite(&m_szCurFileName,strlen(m_szCurFileName),1,ZipFile);
	//write ce
	m_ce.dwCenOffset=dwCentOffset;
	m_ce.wEntries++;
	m_ce.wEntries2++;
	m_ce.wSizeofCent+=sizeof(m_ch);
	if(szFilePath)
		m_ce.wSizeofCent+=strlen(szFilePath);
	else
		m_ce.wSizeofCent+=strlen(m_szCurFileName);
	m_ce.wCommLen=0;

	// Add by Jim 2001-12-5 17:58:03 < 
	CentEnd ceCur;
	memcpy(&ceCur, &m_ce, sizeof(CentEnd));
	RS_GetCentEnd(&ceCur);
	fwrite(&ceCur, sizeof(ceCur), 1, ZipFile);
	// Add by Jim 2001-12-5 17:58:03 > 

//	fwrite(&m_ce,sizeof(m_ce),1,ZipFile);
	deflateEnd(&zz);
	fclose(ZipFile);
	this->GetCHs(m_szZipFile);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
//	函数名：int UnZip::WriteBack()
//	用  途：把上一次解压的文件写回ZIP.
//	返回值：int 1 成功,0 失败
////////////////////////////////////////////////////////////////////////////////

bool UnZip::WriteBack(char *szFileName)
{
#ifdef _WINDOWS
	DWORD dwSavAtt=GetFileAttributes(this->m_szZipFile);
	SetFileAttributes(m_szZipFile,FILE_ATTRIBUTE_NORMAL);
#endif

	if(DelFileInZip(this->m_szCurFileName)==false)
		return false;
//	printf("TempName=%s\nName In zip=%s\n",szFileName,m_szCurFileName);
//	return true;
    bool bResult=AddFile(szFileName);
#ifdef _WINDOWS
	::SetFileAttributes(m_szZipFile,dwSavAtt);
#endif
	return bResult;
	//copy new 
	//delete  file in new
	//add file in new
	//delete old file
	//rename new file
}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: FMove
//功能描述：把f所指的文件，从srcOff开始，到文件结束。移动到desOff所指的地方
//影响变量：
//参数说明:f文件指针，srcOff源偏移，目标偏移
//返 回 值：void
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

void UnZip::FMove(FILE *f, DWORD srcOff, DWORD desOff)
{
	DWORD dwSrcOff,dwDesOff,dwCurOff;
	dwCurOff=ftell(f);
	BYTE szFileBuf[ZIP_MAX_BUF];
	
	dwDesOff=desOff;
	dwSrcOff=srcOff;

	DWORD dwFileLen;
	DWORD dwReaded=srcOff;
	
	fseek(f,0,SEEK_END);
	dwFileLen=ftell(f);
	int nRead;
	fseek(f,dwSrcOff,SEEK_SET);
	while(dwReaded<dwFileLen){
		nRead=fread(szFileBuf,1,ZIP_MAX_BUF,f);
		if(nRead==0)
			break;
		dwReaded+=nRead;
		dwSrcOff=ftell(f);
		fseek(f,dwDesOff,SEEK_SET);
		fwrite(szFileBuf,nRead,1,f);
		dwDesOff=ftell(f);
		fseek(f,dwSrcOff,SEEK_SET);
	}

}
/////////////////////////////////////////////////////////////////////////////
//函 数 名: SetFileEnd
//功能描述：设置szFileName文件长度
//影响变量：
//参数说明:szFileName文件名，dwLen长度
//返 回 值：int =0 OK <0 ERR
//相关函数： 
/////////////////////////////////////////////////////////////////////////////

int UnZip::SetFileEnd(char *szFileName, DWORD dwLen)
{
	int fh;
#ifdef _WINDOWS	
	fh=_open(szFileName,_O_RDWR|_O_CREAT,_S_IREAD|_S_IWRITE);
#else
    fh=open(szFileName,O_RDWR|O_CREAT, 0755);
#endif
	if(fh<0)
		return -1;
#ifdef _WINDOWS	
	_chsize(fh,dwLen);
#else
	ftruncate(fh,dwLen);
#endif
#ifdef _WINDOWS	
	_close(fh);
#else
	close(fh);
#endif
	return 0;
}

bool UnZip::DelCurFile()
{
	return DelFileInZip(m_szCurFileName);
}

int UnZip::CreateNewZip(const char *szZipFileName, const char *szFistFileName)
{

	FILE *fOut=fopen(szZipFileName,"wb");
	if(fOut==NULL){
		return -1;
	}
	memset(&m_ce,0,sizeof(m_ce));
	m_ce.dwCentEndMark=0x06054b50;

	// Add by Jim 2001-12-5 17:58:03 < 
	CentEnd ceCur;
	memcpy(&ceCur, &m_ce, sizeof(CentEnd));
	RS_GetCentEnd(&ceCur);
	fwrite(&ceCur, sizeof(ceCur), 1, fOut);
	// Add by Jim 2001-12-5 17:58:03 > 
//	fwrite(&m_ce,sizeof(m_ce),1,fOut);
	fclose(fOut);

	if(szFistFileName==NULL)
		return 0 ;
	
	sprintf(m_szZipFile,"%s",szZipFileName);
/*	
	FILE *fIn=fopen(szFistFileName,"rb");
	if(fIn==NULL)
		return -1;
*/	
	sprintf(m_szCurFileName,"%s",szFistFileName);
	return AddFile(szFistFileName)?0:-1;
}

int UnZip::AddFileInZip(char *szZipFileName,char *szFileName,char *szFilePath)
{
	sprintf(m_szCurFileName,"%s",szFileName);
	sprintf(m_szZipFile,"%s",szZipFileName);
	return AddFile(szFileName,szFilePath)?0:-1;
}

int UnZip::DelFile(char *szZipFileName, char *szFileName)
{
	sprintf(m_szZipFile,"%s",szZipFileName);
	GetCE(m_szZipFile,&m_ce);
	GetCHs(m_szZipFile);
	return  DelFileInZip(szFileName)?0:-1;

}

bool UnZip::MakePath(char *szPath)
{
	char szTemp[300];
	    strcpy(szTemp,szPath);
	
	char *p=szTemp;
	char *szMake=p;
	struct stat buf;

#ifdef _WINDOWS
	char *ptr=strchr(p,'\\');
#else
	char *ptr=strchr(p,'/');
#endif
	while(ptr&&ptr<szTemp+300){
		ptr[0]=0;
#ifdef _WINDOWS
		if(::GetFileAttributes(szMake)==0xffffffff)
			if(::CreateDirectory(szMake,NULL)==FALSE)
				return FALSE;
#else
		if(stat(szMake,&buf)==-1)
			if(mkdir(szMake,0755) == -1)
				return FALSE;
#endif
		p=ptr+1;
#ifdef _WINDOWS
		ptr[0]='\\';
		ptr=strchr(p,'\\');
#else
		ptr[0]='/';
		ptr=strchr(p,'/');
#endif
	}
	return TRUE;
}

bool UnZip::GetCurFileName(char *szBase, char *szChilds)
{
#ifdef _WINDOWS
	sprintf(m_szCurFileName,"%s\\%s",szBase,szChilds);
#else
	sprintf(m_szCurFileName,"%s/%s",szBase,szChilds);
#endif
	return MakePath(m_szCurFileName);
}

bool UnZip::SetupZip(char *szZipFileName, char *szSetupPath)
{
	char szTemp2[300];
	char szTempSetPath[300];
    	strcpy(szTempSetPath,szSetupPath);
	FindFirst(szZipFileName,szTempSetPath,szTemp2,TRUE);
    	strcpy(szTempSetPath,szSetupPath);
	while(FindNext(szTempSetPath,szTemp2,TRUE))
        strcpy(szTempSetPath,szSetupPath);
	return TRUE;
}

DWORD UnZip::GetTotalFiles()
{
	return dwTotalFiles;
}

DWORD UnZip::GetCurFileIndex()
{
	return dwCurFileIndex;
}

void RS_GetFileHeader(FileHeader *pHeader)
{
#ifdef RS_BIG_ENDIAN
	pHeader->dwCRC			= RS_SWAPDWORD(pHeader->dwCRC);
	pHeader->dwFileSize		= RS_SWAPDWORD(pHeader->dwFileSize);
	pHeader->dwHeadMark		= RS_SWAPDWORD(pHeader->dwHeadMark);
	pHeader->dwzipSize		= RS_SWAPDWORD(pHeader->dwzipSize);
	pHeader->wCmt			= RS_SWAPWORD(pHeader->wCmt);
	pHeader->wData			= RS_SWAPWORD(pHeader->wData);
	pHeader->wExFieldLen	= RS_SWAPWORD(pHeader->wExFieldLen);
	pHeader->wExVer			= RS_SWAPWORD(pHeader->wExVer);
	pHeader->wFileNameLen	= RS_SWAPWORD(pHeader->wFileNameLen);
	pHeader->wFlag			= RS_SWAPWORD(pHeader->wFlag);
	pHeader->wTime			= RS_SWAPWORD(pHeader->wTime);
#endif
}

void RS_GetCentHeader(CentHead *pHeader)
{
#ifdef RS_BIG_ENDIAN
	pHeader->dwCentHeadMark	= RS_SWAPDWORD(pHeader->dwCentHeadMark);
	pHeader->dwCRC			= RS_SWAPDWORD(pHeader->dwCRC);
	pHeader->dwFileSize		= RS_SWAPDWORD(pHeader->dwFileSize);
	pHeader->dwLocalOffset	= RS_SWAPDWORD(pHeader->dwLocalOffset);
	pHeader->dwzibFileSize	= RS_SWAPDWORD(pHeader->dwzibFileSize);
	pHeader->wComLen		= RS_SWAPWORD(pHeader->wComLen);
	pHeader->wData			= RS_SWAPWORD(pHeader->wData);
	pHeader->wDisk			= RS_SWAPWORD(pHeader->wDisk);
	// a bug of definition, it is a DWORD!
	pHeader->wExFA			= RS_SWAPDWORD(pHeader->wExFA);
	pHeader->wExLen			= RS_SWAPWORD(pHeader->wExLen);
	pHeader->wFileNameLen	= RS_SWAPWORD(pHeader->wFileNameLen);
	pHeader->wFlag			= RS_SWAPWORD(pHeader->wFlag);
	pHeader->wIFA			= RS_SWAPWORD(pHeader->wIFA);
	pHeader->wMethod		= RS_SWAPWORD(pHeader->wMethod);
	pHeader->wTime			= RS_SWAPWORD(pHeader->wTime);
	pHeader->wVer			= RS_SWAPWORD(pHeader->wVer);
	pHeader->wVerNeed		= RS_SWAPWORD(pHeader->wVerNeed);
#endif
}

void RS_GetCentEnd(CentEnd *pEnd)
{
#ifdef RS_BIG_ENDIAN
	pEnd->dwCenOffset	= RS_SWAPDWORD(pEnd->dwCenOffset);
	pEnd->dwCentEndMark = RS_SWAPDWORD(pEnd->dwCentEndMark);
	pEnd->wCommLen		= RS_SWAPWORD(pEnd->wCommLen);
	pEnd->wDiskn		= RS_SWAPWORD(pEnd->wDiskn);
	pEnd->wDiskn2		= RS_SWAPWORD(pEnd->wDiskn2);
	pEnd->wEntries		= RS_SWAPWORD(pEnd->wEntries);
	pEnd->wEntries2		= RS_SWAPWORD(pEnd->wEntries2);
	// a bug of definition, it is a DWORD!
	pEnd->wSizeofCent	= RS_SWAPDWORD(pEnd->wSizeofCent);
#endif
}


