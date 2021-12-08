#ifndef __DRS_H__
#define	__DRS_H__


struct DetectResultV
{			// 
	UINT8	SgUuid[MAX_UUID_SIZE];			// signature tag
	CHAR	MalwName[MAX_MALWNAME_SIZE];		// sig name;
	CHAR	EngName[MAX_ENGNAME_SIZE];		// 
	DetectResultV()
	{
		memset(SgUuid , 0 , MAX_UUID_SIZE);
		memset(MalwName , 0 , MAX_MALWNAME_SIZE);
		memset(EngName , 0 , MAX_ENGNAME_SIZE);
	}
};


class DetectReusltBase : public DETECTR,public IDResult
{

};

//数据与接口分离
//目的：接口稳定，数据结构可以自由的演化
class DetectReusltX : public DetectReusltBase,public IDResultSet
{
public:
	DetectReusltX():Mask(DRM_SIG_ID40)
	{
		Class = nam::kVirus;
		SigId.AreaId = 0;
		SigId.Id32 = 0;

		memset(SgUuid , 0 , MAX_UUID_SIZE);
		memset(MalwName , 0 , MAX_MALWNAME_SIZE);
		strcpy(EngName , "classic");
		Mask |= DRM_SIG_ENGN;
	}


	FORCEINLINE static DetectReusltX* GetDetectReusltX(DETECTR* lp)
	{
		return (DetectReusltX*)lp;
	}

public:
	STDMETHOD_(BOOL,GetInt8)(DResult edr , INT8& value){return FALSE;}
	STDMETHOD_(BOOL,GetInt16)(DResult edr , INT16& value){return FALSE;}
	STDMETHOD_(BOOL,GetInt32)(DResult edr , INT32& value)
	{
		switch (edr)
		{
		case EDR_MASK_I4:
			value = Mask;
			break;
		case EDR_SGID32_I4:
			 if (!(SigId.AreaId == 0 && SigId.Id32 > 0)) return FALSE;
			 value = SigId.Id32;
			 break;
		default:
			return FALSE;
		}

		return TRUE;
	}
	STDMETHOD_(BOOL,GetInt64)(DResult edr , INT32& value){return FALSE;}
	STDMETHOD_(BOOL,GetString)(DResult edr , LPCSTR lp , size_t size)
	{
		RASSERT(lp , FALSE);

		if (edr == EDR_ENGINE_NAME_LPSTR && (Mask&DRM_SIG_ENGN))
		{
			strcpy((LPSTR)lp , EngName);
		}
		else if (edr == EDR_MALWARE_NAME_LPSTR && (Mask & DRM_SIG_NAME))
		{
			strcpy((LPSTR)lp ,MalwName);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	STDMETHOD_(BOOL,GetValue)(DResult edr ,  LPVOID lp , size_t size)
	{
		RASSERT(lp && size > 0 , FALSE);

		if (edr == EDR_SGID40_S && (Mask & DRM_SIG_ID40))
		{
			RASSERT(size == sizeof(SGID40) , FALSE);
			SGID40* lpSID = (SGID40*)lp;
			*lpSID = SigId;
		}
		else if (edr == EDR_DETECT_S)
		{
			RASSERT(size == sizeof(DETECTR) , FALSE);
			DETECTR* dr = (DETECTR*)lp;
			dr->Class = Class;
			dr->SigId = SigId;
		}
		else if (edr == EDR_UUID_16 &&  (Mask & DRM_SIG_UUID))
		{
			RASSERT(size >= MAX_UUID_SIZE , FALSE);
			memcpy(lp , SgUuid , MAX_UUID_SIZE);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	
	STDMETHOD_(BOOL,SetInt8)(DResult edr , INT8 value){return FALSE;}
	STDMETHOD_(BOOL,SetInt16)(DResult edr , INT16 value){return FALSE;}
	STDMETHOD_(BOOL,SetInt32)(DResult edr , INT32 value)
	{
		switch (edr)
		{
		case EDR_MASK_I4:
			Mask = value;
			break;
		case EDR_SGID32_I4:
			SigId.Id32 = value;
			SigId.AreaId = 0;
			Mask |= DRM_SIG_ID40;
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}
	STDMETHOD_(BOOL,SetInt64)(DResult edr , INT32 value){return FALSE;}
	STDMETHOD_(BOOL,SetString)(DResult edr , LPCSTR lp)
	{
		RASSERT(lp , FALSE);
		
		INT32 _len = strlen(lp);

		if (edr == EDR_ENGINE_NAME_LPSTR)
		{
			if (lp[0] == 0) return FALSE;

			RASSERT(_len> 0 && _len < MAX_ENGNAME_SIZE , FALSE);
			strcpy(EngName, lp);
			Mask |= DRM_SIG_ENGN;
		}
		else if (edr == EDR_MALWARE_NAME_LPSTR)
		{
			RASSERT(_len >0 && _len < MAX_MALWNAME_SIZE , FALSE);
			strcpy(MalwName , lp);
			Mask |= DRM_SIG_NAME;
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	STDMETHOD_(BOOL,SetValue)(DResult edr ,  LPVOID lp , size_t size)
	{
		RASSERT(lp && size > 0 , FALSE);
		if (edr == EDR_SGID40_S)
		{
			RASSERT(size == sizeof(SGID40) , FALSE);
			SGID40* lpSGID = (SGID40*)lp;
			SigId = *lpSGID;
			Mask |= DRM_SIG_ID40;
		}
		else if (edr == EDR_DETECT_S)
		{
			RASSERT(size == sizeof(DETECTR) , FALSE);
			DETECTR* dr = (DETECTR*)lp;
			Class = dr->Class;
			SigId = dr->SigId;
		}
		else if (edr == EDR_UUID_16)
		{
			RASSERT(size <= MAX_UUID_SIZE , FALSE);
			memcpy(SgUuid,lp,size);
			Mask |= DRM_SIG_UUID;
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}

private:
	UINT8	SgUuid[MAX_UUID_SIZE];			// signature tag
	CHAR	MalwName[MAX_MALWNAME_SIZE];		// sig name;
	CHAR	EngName[MAX_ENGNAME_SIZE];		// 
public:
	UINT32	Mask;	//为了方便&|操作
};


#endif