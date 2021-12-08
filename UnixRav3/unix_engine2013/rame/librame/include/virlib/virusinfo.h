#ifndef __rsvirusinfo_h__2007_5_29__19H52M25S
#define __rsvirusinfo_h__2007_5_29__19H52M25S

//病毒行为类型定义			
#define ACTION_VIRUS		1	//病毒：病毒库中主类型为 virus 的记录
#define ACTION_WORM			2	//蠕虫：病毒库中主类型为 worm 的记录
#define ACTION_ROOTKIT		3	//rookit工具：病毒库中主类型为 rookit的记录
#define ACTION_ADWARE		4	//广告程序：病毒库中主类型为 adware 的记录
#define ACTION_TROJAN		5	//木马：病毒库中主类型为 backdoor trojan dropper binder hack constructor harm pack jock junk    的记录
#define ACTION_BACKDOOR		6	//后门：病毒库中主类型为 backdoor  的记录
#define ACTION_SUSPICOUS	7	//可疑：

typedef union _UNION_ACTION
{
	struct  
	{
#ifdef RS_BIG_ENDIAN
		BYTE Reserved:1;		//保留位未使用
		BYTE IsHide:1;			//是否隐藏，0显示，1隐藏。
		BYTE SusLevel:2;		//是否启发扫描判定出的病毒，0非启发记录，1低级启发记录，2中级启发记录，3高级启发记录
		BYTE VirusActType:4;	//病毒行为类型
#else
		BYTE VirusActType:4;	//病毒行为类型
		BYTE SusLevel:2;		//是否启发扫描判定出的病毒，0非启发记录，1低级启发记录，2中级启发记录，3高级启发记录
		BYTE IsHide:1;			//是否隐藏，0显示，1隐藏。
		BYTE Reserved:1;		//保留位未使用
#endif
	};
	BYTE bValue;
	
}UNION_ACTION;
struct STRsVirusInfo
{
	DWORD			dwVirusID;			//病毒记录的ID
	BYTE			byVirusType;		//病毒类型
	UNION_ACTION	uiVirusActType;		//病毒行为类型
	CHAR			szVirusName[64];	//病毒名
	CHAR			szAliasName[32];	//病毒别名
};

#endif // __rsvirusinfo_h__
