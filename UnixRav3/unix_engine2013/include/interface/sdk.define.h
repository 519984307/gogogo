#ifndef __SDK_DEFINE_H__
#define __SDK_DEFINE_H__
//#pragma message("111111111111111111111111111111")
#include <map>
#include <string>
#include "rxclsid.h"
#include "rxclstab.h"

typedef HRESULT (*pfnGetClassObject)(const IID&, void**);

typedef std::map<TCLSID,pfnGetClassObject> SDKClassObjectMap;
typedef std::map<CLSID,pfnGetClassObject> SDKClassObjectMap10;

#define MAKE_SDK_TCLSOBJ_TABLE(name,desobjs,srcobjs)	\
class  CLSOBJMAKER##T##name\
{ \
public:\
	CLSOBJMAKER##T##name()\
	{\
	    std::string ss(#name);\
		printf("****************************************%s\n",ss.c_str());\
		for(size_t i = 0; i < sizeof(srcobjs)/sizeof(srcobjs[0]); ++i)\
		{\
			desobjs.insert(SDKClassObjectMap::value_type(srcobjs[i].clsid,srcobjs[i].pfnGetClassObject));\
		}\
	}\
};\
static CLSOBJMAKER##T##name  _CLSOBJMAKER##T##name;



#define MAKE_SDK_CLSOBJ_TABLE(name,desobjs,srcobjs)	\
class  CLSOBJMAKER##name\
{ \
public:\
	CLSOBJMAKER##name()\
    {\
	   for(size_t i = 0; i < sizeof(srcobjs)/sizeof(srcobjs[0]); ++i)\
       {\
	      desobjs.insert(SDKClassObjectMap10::value_type(srcobjs[i].clsid,srcobjs[i].pfnGetClassObject));\
       }\
    }\
};\
static CLSOBJMAKER##name  _CLSOBJMAKER##name;


//////////////////////////////////////////////////////////////
//rxruntim2
extern  SDKClassObjectMap	g_13_runtime_clsobjs;
extern  SDKClassObjectMap10  g_10_runtime_clsobjs;

extern  SDKClassObjectMap10  g_13_prxy_clsobjs;


BOOL rxGetCLSIDFromCatelog( TCLSIDREF clsidCatelog, DWORD dwClsID, STRPTR& rclsid );
LPCSTR rxGetClassServer( TCLSIDREF clsid );

LPCSTR REGetClassServer( REFCLSID rclsid );

void sdk_append_rxcoml_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxcoml_clsobjs(SDKClassObjectMap10& objs);
//////////////////////////////////////////////////////////////

void sdk_append_rxcore_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxmss_tclsobjs(SDKClassObjectMap& objs);

void sdk_append_rxfsm_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxmail_tclsobjs(SDKClassObjectMap& objs);

void sdk_append_scanelf_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_scanelf_clsobjs(SDKClassObjectMap10& objs);

void sdk_append_scanpe_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_scanpe_clsobjs(SDKClassObjectMap10& objs);

void sdk_append_rxxdu0_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxmltk_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxvision_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxxman_tclsobjs(SDKClassObjectMap& objs);



void sdk_append_methodex_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_scanex_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_scanexec_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_scanmac_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_scansct_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_scriptic_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_vex86_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_urutils_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_uroutine_clsobjs(SDKClassObjectMap10& objs);

extern  SDKClassObjectMap10  g_10_engs_clsobjs;
//////////////////////////////////////////////////////////////

void sdk_append_rxoffice_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxwinpe_tclsobjs(SDKClassObjectMap& objs);

void sdk_append_rxarch_arch_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxarch_comp_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxarch_cryp_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_rxarch_algo_tclsobjs(SDKClassObjectMap& objs);
void sdk_append_revm_clsobjs(SDKClassObjectMap10& objs);
void sdk_append_rxcav_tclsobjs(SDKClassObjectMap& objs);
//////////////////////////////////////////////////////////////
#endif