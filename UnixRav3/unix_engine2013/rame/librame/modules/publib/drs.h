#ifndef __DRS_H__
#define	__DRS_H__

#include <string>
#include "../../include/imdunit.h"
#include "../../../include/feature.h"

//static inline SIZE_T BITMASK( SIZE_T k ) { return 1 << k; }

struct InnerDetectionResult : IInnertDetectionResult
{
	DETECTR			_V1;
	SGID32			_Vid32;
	std::string		_EngineName;
	std::string		_MalwName;
	std::string		_SignatureTag;

	InnerDetectionResult()  {
		_V1.Class = nam::kTrait;
		_V1.SigId.AreaId = 0;
		_V1.SigId.Id32 = 0;
		_Vid32 = 0;
	}

	VOID Reset() {
		_V1.Class = nam::kTrait;
		_V1.SigId.AreaId = 0;
		_V1.SigId.Id32 = 0;
		_Vid32 = 0;
		_EngineName = "";
		_MalwName = "";
		_SignatureTag = "";
	}

	STDMETHOD_(LPDETECTR, ResultV1Ptr)()
	{
		return &_V1;
	}

	STDMETHOD_(LPCSTR,	EngineName)() {
		return _EngineName.size() ? _EngineName.c_str() : NULL;
	}
	STDMETHOD_(LPCSTR,	MalwareName)() {
		return _MalwName.size() ? _MalwName.c_str() : NULL;
	}
	STDMETHOD_(MMCLS,	MalwareClass)() {
		return _V1.Class;
	}
	STDMETHOD_(UINT32,	MalwareId32)() {
		return _Vid32;
	}
	STDMETHOD_(SGID40,	MalwareId40)() {
		return _V1.SigId;
	}
	STDMETHOD_(LPCSTR,	SignatureTag)( SIZE_T * lpSize ) {
		if( lpSize ) *lpSize = _SignatureTag.size();
		if( _SignatureTag.empty() ) return NULL;
		return _SignatureTag.c_str();
	}
	// return DataSize, <=0 is Failed;
	STDMETHOD_(LONG, GetValue)( XDR_INDEX uIndex, PVOID lpDataRecv ) {
		if( uIndex >= XDR_INDEX_MAX ) return -2;
		SIZE_T cRetSize = 0;
		//if(!( BITMASK(uIndex) & _FieldMask )) return 0;
		switch( uIndex ) {
		case XDR_VID32:
			*(UINT32*)lpDataRecv = _Vid32;
			return sizeof(UINT32);
		case XDR_VID40_SGID40:
			*(SGID40*)lpDataRecv = _V1.SigId;
			return sizeof(SGID40);
		case XDR_MALWCLASS_8:
			*(UINT8*)lpDataRecv = _V1.Class;
			return sizeof(UINT8);
		case XDR_NGINNAME_CPTR:
			*(LPCSTR*)lpDataRecv = EngineName();
			return _EngineName.size();
		case XDR_MALWNAME_CPTR:
			*(LPCSTR*)lpDataRecv = MalwareName();
			return _MalwName.size();
		case XDR_SIGTAG_CPTR:
			*(LPCSTR*)lpDataRecv = SignatureTag(&cRetSize);
			return cRetSize;
		default:
			return -1;
		}
	}
public:
	STDMETHOD( SetMalwBasicInfo )( MMCLS mcls, UINT32 id32, UINT8 area )
	{
		_V1.Class = mcls;
		_V1.SigId.AreaId = area;
		_V1.SigId.Id32 = id32;
		if( !area ) _Vid32 = id32; else _Vid32 = 0;
		return S_OK;
	}
	STDMETHOD( SetMalwBasicInfo )( MMCLS mcls, UINT32 id32 )
	{
		_V1.Class = mcls;
		_V1.SigId.AreaId = 0;
		_V1.SigId.Id32 = id32;
		_Vid32 = id32;
		return S_OK;
	}
	STDMETHOD( SetEngineName )( LPCSTR engineName )
	{
		_EngineName = engineName ? engineName : "";
		return S_OK;
	}
	STDMETHOD( SetMalwareName )( LPCSTR malwareName )
	{
		_MalwName = malwareName ? malwareName : "";
		return S_OK;
	}
	STDMETHOD( SetSignatureTag )( LPCSTR signatureTag, SIZE_T bytes )
	{
		if( !signatureTag ) 
		{ 
			signatureTag = ""; 
			return S_OK;
		}

		if( bytes ) 
			_SignatureTag.assign( signatureTag, bytes );
		else
			_SignatureTag.assign( signatureTag );
		return S_OK;
	}
	
	
	// 规则: 
	//		ID32 不能为0
	//		ID40 两个字段都不能为0
	//		名字 不能为NULL或空
	//		ID32/ID40/名字，必须设置过一个
	//		原则：名字 > ID40 > ID32, ID40 和 ID32 允许没有， ID40 和 ID32 不能同时存在，如果同时存在，则ID40优先
	void MaskVirusName()
	{
		if (_MalwName.empty()) return;

#if 0
		UINT32 mask_size = (UINT32)((float)_MalwName.size() * 0.3);
		if (mask_size == 0) return;

		memset((char*)_MalwName.c_str() + mask_size , '*' , mask_size);
#else
		uint32_t idx = 1;
		while (idx < _MalwName.size())
		{
			_MalwName[idx] = '*';	
			idx+=2;
		}

#endif
		
		
	}
	long CompleteAndCheckFields( IRXNameDB * namedb ) 
	{
		bool has_id32 = 0, has_id40 = 0, has_name = 0, has_kls = 0;
		has_kls = _V1.Class > 0;
		if (_Vid32 == 0 && _V1.SigId.AreaId == 0) _Vid32 =  _V1.SigId.Id32;
		has_id32 = _Vid32 != 0;
		has_id40 = (_V1.SigId.AreaId && _V1.SigId.Id32 );
		has_name = MalwareName() != NULL;


		if( _V1.Class < nam::kMalware ) {
			return S_OK;
		}


		if( !has_kls ) return E_FAIL;
		if( !has_id32 && !has_id40 && !has_name )
			return E_FAIL;
		
		if( has_id40 && has_id32 ) has_id32 = false;
		
		if( has_id40 ) { 
			_Vid32 = 0;
			namedb->Id40To32( _V1.SigId, &_Vid32 );
			has_id32 = _Vid32 != 0;
		} else if( has_id32 ) {
			namedb->Id32To40( _Vid32, &_V1.SigId );
			has_id40 = _V1.SigId.Id32 != 0;
		} 
		
		if( !has_name ) {
			CHAR lMalwareName[256] = {0};
			if( has_id40 ) {
				if( FAILED( namedb->GetNameById40( _V1.SigId, lMalwareName, sizeof(lMalwareName) ) ) || !lMalwareName[0] ) {
					sprintf( lMalwareName, "Malware.UDM!%X.%X", _V1.SigId.AreaId, _V1.SigId.Id32 );
				}
			} else if( has_id32 ) {
				if( FAILED( namedb->GetNameById32( _Vid32, lMalwareName, sizeof(lMalwareName) ) ) || !lMalwareName[0] ) {
					sprintf( lMalwareName, "Malware.UDM!0.%X", _Vid32 );
				}
			}

			if (strnicmp(lMalwareName , "HiddenF" , 7) == 0) return E_FAIL;
			_MalwName.assign(lMalwareName);
			
		}

#if MASK_VIRUS_NAME
		MaskVirusName();
#endif
		return S_OK;
	}
};

#endif