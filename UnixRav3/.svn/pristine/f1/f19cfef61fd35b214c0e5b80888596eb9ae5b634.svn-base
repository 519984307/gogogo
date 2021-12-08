#include "StdAfx.h"
#include "RisingAccredit.h"


int  rsunzipm(const char * szZip, std::map<std::string, std::string> & lstMem);
int  RsEsmAccredit(const char * szSerial, const char * szLicPath, AccreidtItem & result)
{
	CRisingAccredit ac;
	MemoryFileList  lmem;

	rsunzipm(szLicPath, lmem);
   	if (lmem.empty()) return -2;

   	ac.SetMemoryFileList(lmem);
	if (ac.ParseAccredit(szSerial, "", 0)) return -3;
	if (ac.m_vResults.size() != 1) return -4;

	result = ac.m_vResults[0];
	return 0;
}
