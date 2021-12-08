#ifndef __IRSVIRUSINFOTABLE_H__
#define __IRSVIRUSINFOTABLE_H__

#include "./irstable.h"
#include "./virusinfo.h"

//----------------------------------------------------------------------------------


class IREVirusInfoLib : public IUnknown
{
public:
	RE_DECLARE_IID;

	//根据病毒ID取病毒信息
	STDMETHOD(GetVirusInfo) (DWORD dwVirusID,	STRsVirusInfo* pstRsVirusBaseInfo) PURE ;

	//根据病毒名和病毒类型取病毒信息
	STDMETHOD(GetVirusInfo) (LPSTR lpszVirusName,BYTE byVirusType,STRsVirusInfo* pstRsVirusBaseInfo) PURE;

	//根据病毒名取病毒ID，返回的ID填在pdwVirusArray里面，
	//dwIDCount是出入参。
	//当dwIDCount=0时，函数返回成功，dwIDCount为符合病毒名条件的个数
	STDMETHOD(GetVirusID) (LPSTR lpszVirusName, DWORD* pdwVirusArray, DWORD& dwIDCount) PURE ;

	//取最大病毒ID
	STDMETHOD_(DWORD, GetMaxVirusID) () PURE ;

	//根据病毒名和病毒类型取病毒信息
	STDMETHOD(GetVirusInfo) (DWORD dwVirusID, IREArchive* pArchive, STRsVirusInfo* pstRsVirusBaseInfo) PURE;
};

RE_DEFINE_IID(IREVirusInfoLib, "{358E7ADD-DC05-48e7-A3D3-D858D6C0B142}",
			0x358e7add, 0xdc05, 0x48e7, 0xa3, 0xd3, 0xd8, 0x58, 0xd6, 0xc0, 0xb1, 0x42);

class IEnumVirusInfo : public IUnknown
{
public:
	RE_DECLARE_IID;
	virtual HRESULT GetCountOfVirusInfoItems( PDWORD pdwItemCount ) = 0;
	virtual HRESULT GetVirusInfoByItemID( DWORD dwItem, STRsVirusInfo* pstRsVirusBaseInfo ) = 0;
};

RE_DEFINE_IID( IEnumVirusInfo, "{BF3DEA53-E81A-435b-A823-724D6935DD33}",
			0xbf3dea53, 0xe81a, 0x435b, 0xa8, 0x23, 0x72, 0x4d, 0x69, 0x35, 0xdd, 0x33);


//====================================================================================
// {77918387-59E1-4f6f-A8EF-FD4B055612A8}
DEFINE_GUID(CLSID_CREVirusInfoLib, 
			0x77918387, 0x59e1, 0x4f6f, 0xa8, 0xef, 0xfd, 0x4b, 0x5, 0x56, 0x12, 0xa8);










#endif // __IVirusInfo_h__



