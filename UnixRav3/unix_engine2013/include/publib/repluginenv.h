#ifndef __RE_PLUGIN_ENGINE_ENVIROMENT_H__
#define __RE_PLUGIN_ENGINE_ENVIROMENT_H__

#include <assert.h>
#include <string>
#include "utility_ex.h"

#include "dllutil.h"

class CREEnvironment
{
public:
	virtual HRESULT Initialize(LPCSTR pcszREModulePath, const CLSID *pclsid, DWORD num, LPCSTR pcszVDBCfgFileName, LPCSTR pcszProductId);
	virtual void Clear();
public:
	virtual ~CREEnvironment() { CREEnvironment::Clear(); }
public:
	operator IRERunningObjectTable* () { return m_prot; }
	IRERunningObjectTable* operator -> () { return *this; }
	HRESULT CreateInstance(const CLSID& rclsid, IUnknown *punkOuter, const IID& riid, void **ppv)
	{
		return m_pol->CreateInstance(m_prot, rclsid, punkOuter, riid, ppv);
	}
protected:
	UTIL::sentry<HMODULE, UTILEX::hmodule_sentry> m_hEngComp;
	UTIL::com_ptr<IRERunningObjectTable> m_prot;
	UTIL::com_ptr<IREObjectLoader> m_pol;
protected:
	PFNRECreateROT m_pfnRECreateROT;
	PFNREClearROT m_pfnREClearROT;
};


inline
HRESULT CREEnvironment::
Initialize(LPCSTR pcszREModulePath, const CLSID *pclsid, DWORD num, LPCSTR pcszVDBCfgFileName, LPCSTR pcszProductId)
{
#if !defined(PLATFORM_TYPE_POSIX)
	const char sep = '\\';
#else
	const char sep = '/';
#endif //PLATFORM_TYPE_POSIX
	UTIL::sentry<HMODULE, UTILEX::hmodule_sentry> hEngComp;
#ifndef NATIVE_TEST
	RASSERT(hEngComp = CDllUtil::LoadLibraryA((std::string(pcszREModulePath)+sep+COMPONENT_DLL).c_str()), E_FAIL);
#else
	RASSERT(hEngComp = CDllUtil::LoadLibraryA((std::string(pcszREModulePath)+sep+"rebscomp.dll").c_str()), E_FAIL);
#endif
	m_pfnRECreateROT = (PFNRECreateROT)CDllUtil::GetProcAddress(hEngComp, "RECreateROT");
	m_pfnREClearROT = (PFNREClearROT)CDllUtil::GetProcAddress(hEngComp, "REClearROT");
	RASSERT(m_pfnRECreateROT && m_pfnREClearROT, E_FAIL);
	RFAILED(m_pfnRECreateROT(pclsid, num, pcszVDBCfgFileName, pcszProductId, &m_prot.m_p));
	UTIL::com_ptr<IUnknown> punk;
	m_prot->GetObject(PID_ROT_ObjectLoader, &punk.m_p);
	m_pol = punk;
	m_hEngComp = hEngComp.detach();
	return S_OK;
}


inline
void CREEnvironment::Clear()
{
	if(m_prot)
	{
		HRESULT hr = m_pfnREClearROT(m_prot);
		assert(SUCCEEDED(hr));
		m_prot = (IUnknown*)0;
		UTIL::com_ptr<IREOLManager>(m_pol)->FreeUnusedLibraries();
		m_pol = (IUnknown*)0;
	}
	m_hEngComp = 0;
}


#endif // __RE_PLUGIN_ENGINE_ENVIROMENT_H__
