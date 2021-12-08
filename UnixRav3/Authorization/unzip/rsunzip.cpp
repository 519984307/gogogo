#include "zlib/unzip.h"
#include <map>
#include <string>

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif


static int  rsmkdir(const char * name)
{
	int ret;

	#ifdef WIN32
	ret = _mkdir(name);
	#else
	ret = mkdir(name, 0775);
	#endif

	return ret;
}

static int  rsunzipf_onefile(unzFile uf, const char * szDir)
{
	int     err, off = 0;
	char    fpath[256] = {0}, *p, *fname;
	FILE  * pfNew = 0;
	unz_file_info64 finfo;

	p = (char*)szDir;
	while((*p) != '\0')
	{
		fpath[off++] = *p++;
	}

	if(fpath[0] != '\0' && fpath[off] != '/') fpath[off++] = '/';
	err = unzGetCurrentFileInfo64(uf, &finfo, fpath+off, sizeof(fpath)-off, 0, 0, 0, 0);
	if(err != UNZ_OK) return err;

	p = fname = fpath;
	while((*p) != '\0')
	{
		if(((*p) == '/') || ((*p) == '\\')) fname = p+1;
		p++;
	}

	if((*fname) == '\0')
	{
		rsmkdir(fpath);
		return 0;
	}

	err   = unzOpenCurrentFilePassword(uf, 0);
	if(err)    return -1;

	pfNew = fopen(fpath, "wb");
	if(!pfNew) return -2;

	while(1)
	{
		char  buf[1024];
		err = unzReadCurrentFile(uf, buf, sizeof(buf));
		if(err == 0) break;
		if(err <  0) { err = -3; break; }

		if(fwrite(buf, err, 1, pfNew) != 1) { err = -4; break; }
	}

	fclose(pfNew);
	return 0;
}

int  rsunzipf(const char * szZip, const char * szDir)
{
	int     err;
	unzFile uf = 0;
	unz_global_info64 gi;

	uf  = unzOpen64(szZip);
	if(!uf) return -1;

	err = unzGetGlobalInfo64(uf, &gi);
	if(err != UNZ_OK)
	{
		unzClose(uf);
		return err;
	}

	for(int i = 0;i < (int)gi.number_entry; i++)
	{
		if (rsunzipf_onefile(uf, szDir) != UNZ_OK)
			break;

		if((i+1) < (int)gi.number_entry)
		{
			err = unzCloseCurrentFile(uf);
			err = unzGoToNextFile(uf);
			if(err != UNZ_OK)
			{
				printf("error %d with zipfile in unzGoToNextFile\n", err);
				break;
			}
		}
	}

	unzClose(uf);
	return 0;
}


static int  rsunzipm_onefile(unzFile uf, std::map<std::string, std::string> & lstMem)
{
	int     err = 0;
	char    fpath[256] = {0}, *p, *fname;
	FILE  * pfNew = 0;
	unz_file_info64 finfo;

	err = unzGetCurrentFileInfo64(uf, &finfo, fpath, sizeof(fpath), 0, 0, 0, 0);
	if(err != UNZ_OK) return err;

	p = fname = fpath;
	while((*p) != '\0')
	{
		if(((*p) == '/') || ((*p) == '\\')) fname = p+1;
		p++;
	}

	if((*fname) == '\0')
	{
		lstMem[fpath] = "";
		return 0;
	}

	err   = unzOpenCurrentFilePassword(uf, 0);
	if(err)    return -1;

	lstMem[fpath] = "";
	std::string & txt = lstMem[fpath];

	while(1)
	{
		char  buf[1024];
		err = unzReadCurrentFile(uf, buf, sizeof(buf));
		if(err == 0) break;
		if(err <  0) { err = -3; break; }

		txt.append(buf, err);
	}
	return 0;
}

int  rsunzipm(const char * szZip, std::map<std::string, std::string> & lstMem)
{
	int     err;
	unzFile uf = 0;
	unz_global_info64 gi;

	uf  = unzOpen64(szZip);
	if(!uf) return -1;

	err = unzGetGlobalInfo64(uf, &gi);
	if(err != UNZ_OK)
	{
		unzClose(uf);
		return err;
	}

	for(int i = 0;i < (int)gi.number_entry; i++)
	{
		if (rsunzipm_onefile(uf, lstMem) != UNZ_OK)
			break;

		if((i+1) < (int)gi.number_entry)
		{
			err = unzCloseCurrentFile(uf);
			err = unzGoToNextFile(uf);
			if(err != UNZ_OK)
			{
				printf("error %d with zipfile in unzGoToNextFile\n", err);
				break;
			}
		}
	}

	unzClose(uf);
	return 0;
}

