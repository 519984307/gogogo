#ifndef __RSENGCMN_H__
#define __RSENGCMN_H__

#include "RSCMNDEF.H"

class IRSProperty
{
public:
	virtual RS_RESULT GetProperty(RS_DWORD dwPropID, void* pValue, RS_DWORD &dwLen) = 0;
	virtual RS_RESULT SetProperty(RS_DWORD dwPropID, void* pValue, RS_DWORD dwLen) = 0;
};

//扫描的对象的类型
enum
{
	HOSTTYPE_ANSINAME = 0,
	HOSTTYPE_UNICODENAME,
	HOSTTYPE_DISK,
};

struct STVirusHost
{
	RS_DWORD dwType;
	RS_DWORD dwLen;
	union
	{
		void* pData;
		char* pAName;
		wchar_t* pWName;
	};
};

typedef STVirusHost STFileName;

#endif // __RSENGCMN_H__
