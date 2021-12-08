#ifndef __rsvirusinfo_h__2007_5_29__19H52M25S
#define __rsvirusinfo_h__2007_5_29__19H52M25S

struct STRsVirusInfo
{
	DWORD			dwVirusID;			//病毒记录的ID
	BYTE				byVirusType;		//病毒类型
	CHAR				szVirusName[64];	//病毒名
	CHAR				szAliasName[32];	//病毒别名
};














#endif // __rsvirusinfo_h__
