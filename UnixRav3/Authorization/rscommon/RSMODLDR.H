#ifndef __RSMODLDR_H__
#define __RSMODLDR_H__

#include "RSMODULE.H"

#ifdef WIN32 // Windows platform
#define rsLoadModule(l)		LoadLibraryA(l)
#define rsGetAddress(h, f)	GetProcAddress(h, f)
#define rsFreeModule(h)		FreeLibrary(h)
#define SYM_RSMODULE		"_GetRSModule@0"
#else // *NIX platform
#include <dlfcn.h>
typedef void* HMODULE;
#define rsLoadModule(l)		dlopen(l, RTLD_LAZY)
#define rsGetAddress(h, f)	dlsym(h, f)
#define rsFreeModule(h)		dlclose(h)
#define SYM_RSMODULE		"GetRSModule"
#endif // WIN32

class CRSModLoader
{
	HMODULE m_hMod;
	fnGetRSModule m_pGetRSModule;

public:
	inline CRSModLoader() : m_hMod(NULL), m_pGetRSModule(NULL)
	{
	}

	inline CRSModLoader(char* pszName) : m_hMod(NULL), m_pGetRSModule(NULL)
	{
		Load(pszName);
	}

	inline ~CRSModLoader()
	{
		Free();
	}

	void Load(char* pszName)
	{
		Free();

		m_pGetRSModule = NULL;

		m_hMod = rsLoadModule(pszName);
		if(m_hMod)
			m_pGetRSModule = (fnGetRSModule)rsGetAddress(m_hMod, SYM_RSMODULE);
	}

	inline const IRSModule* GetRSModule()
	{
		if(m_pGetRSModule)
			return m_pGetRSModule();

		return NULL;
	}

	void Free()
	{
		if(m_hMod)
		{
			rsFreeModule(m_hMod);
			m_hMod = NULL;
			m_pGetRSModule = NULL;
		}
	}
};

#ifdef WIN32
typedef CRSModLoader CRSWin32ModLoader;
#else // WIN32
typedef CRSModLoader CRSXnixModLoader;
#endif // WIN32

#endif // __RSMODLDR_H__
