#ifndef __RSMODULE_H__
#define __RSMODULE_H__

#include "RSCMNDEF.H"

template<IRSModule::ObjItems pObjItems, RS_DWORD dwModId, RS_DWORD dwModVer>
class CRSModuleImpl : public IRSModule
{
public:
	virtual RS_DWORD GetId() const
	{
		return dwModId;
	}

	virtual RS_DWORD GetVersion() const
	{
		return dwModVer;
	}

	virtual ObjItems GetObjItems() const
	{
		return pObjItems;
	}

	virtual RS_RESULT CreateObject(RS_DWORD dwObjId, IRSEnv* pEnv, void** ppv) const
	{
		if(!pObjItems)
			return ERR_QI_TYPEERR;

		ObjItem* pOI = pObjItems;
		while(pOI->m_dwObjId)
		{
			if(pOI->m_dwObjId == dwObjId)
				return (pOI->m_pFun(ppv, pEnv));

			pOI++;
		}

		return ERR_QI_TYPEERR;
	}
};


#define DECLARE_OBJECT_ITEM(a)					{ a::RSID, a::CreateObj }

#ifdef WIN32	// MSC only?
#define RSDLLEXPORT								__declspec(dllexport)
#else
#define RSDLLEXPORT
#endif // WIN32

// NOTE: We MUST use the long version macro because of the silly gcc on SunOS.
/*
#define START_DECLARE_MODULE(dwModId, dwModVer)												\
	extern IRSModule::ObjItem gObjItems##dwModId[];											\
	CRSModuleImpl<gObjItems##dwModId, dwModId, dwModVer>	gModule##dwModId;				\
	extern "C"																				\
	RSDLLEXPORT																				\
	const IRSModule* RS_STDCALL GetRSModule()												\
	{ return &gModule##dwModId; }															\
	IRSModule::ObjItem gObjItems##dwModId[] = {
// */

#define START_DECLARE_MODULE(dwModId, dwModVer)	extern IRSModule::ObjItem gObjItems##dwModId[];	CRSModuleImpl<gObjItems##dwModId, dwModId, dwModVer> gModule##dwModId; extern "C" RSDLLEXPORT const IRSModule* RS_STDCALL GetRSModule()	{ return &gModule##dwModId; } IRSModule::ObjItem gObjItems##dwModId[] = {

#define END_DECLARE_MODULE						{0, NULL}};

typedef const IRSModule* (RS_STDCALL *fnGetRSModule)();

#endif // __RSMODULE_H__
