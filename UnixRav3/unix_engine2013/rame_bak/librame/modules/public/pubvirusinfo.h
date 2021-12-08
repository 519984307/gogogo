#ifndef pubvirusinfo_h__
#define pubvirusinfo_h__

 

//////////////////////////////////////////////////////////////////////////
#pragma pack(1)
//本地病毒基本信息(后跟中文病毒名)
struct STDefVirusInfo
{
	DWORD	dwVirusID;
	BYTE	byVirusType;		//病毒类型
	CHAR	szVirusName[1];		//病毒名(第一字节为病毒名前缀)
	void endian_adjust()
	{
		dwVirusID		= bq::ltoh(dwVirusID);
	}
} RS_PACK_ONEBYTE;

struct STInfoRecordIndex
{
	SIZE_T	pbyAddress;		//记录的起始地址
	//	DWORD	dwSize;			//记录的大小
} RS_PACK_ONEBYTE;

struct STVarRecordTableHeader
{
	BYTE	bySize;				//结构体的实际大小
	BYTE	byUnknown[254];		//未知数据区（派生类自已定义）
}RS_PACK_ONEBYTE;

#pragma pack()



//////////////////////////////////////////////////////////////////////////
#if(0)
病毒信息库结构：

header
{
	STVarRecordTableHeader;
}[1];

body
{
	STDefVirusInfo;
	AliasName;
}[];

#endif
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

inline BYTE  get_unknown_virus_type(DWORD id) { return (BYTE)((id&~0xC03FFFFF)>>22); }
inline DWORD strip_unknown_virus_type(DWORD id) { return id&0xC03FFFFF; }
inline DWORD merge_unknown_virus_type(DWORD id, BYTE type) { return strip_unknown_virus_type(id) | ((DWORD)type<<22); }
inline DWORD make_unknown_virus_id(DWORD id, BYTE type) { return merge_unknown_virus_type(id, type) | (VID_MASK_UNKNOWN); }

//////////////////////////////////////////////////////////////////////////



#endif // pubvirusinfo_h__
