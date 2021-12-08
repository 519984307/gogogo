#ifndef _RX_EXPORT_INTERFACE_H_
#define _RX_EXPORT_INTERFACE_H_

#include "../../include/iunknown.h"
#include "../../include/xvalue.h"

#include "rxclsid.h"
#include "propvariant.h"


//////////////////////////////////////////////////////////////////////////

class IWithOuter : public IUnknown
{
public:
	STDMETHOD(InternalQueryInterface)( REFIID riid, void** ppv ) PURE;
	STDMETHOD(BindOuter)( IUnknown * punk ) PURE;
	RE_DECLARE_IID;
};
// {507C00ED-10AC-4837-B6DF-61B5AD736676}
RE_DEFINE_IID( IWithOuter, "{507C00ED-10AC-4837-B6DF-61B5AD736676}",
			  0x507c00ed, 0x10ac, 0x4837, 0xb6, 0xdf, 0x61, 0xb5, 0xad, 0x73, 0x66, 0x76);

class IDynamicImplement : public IUnknown
{
public:
	STDMETHOD(InternalQueryInterface)( REFIID riid, void** ppv ) PURE;
	STDMETHOD(PushImplement)( IWithOuter* pImpl ) PURE;
	RE_DECLARE_IID;
};

// {A8D29E86-62BC-458b-8A2B-B13840850596}
RE_DEFINE_IID( IDynamicImplement, "{A8D29E86-62BC-458b-8A2B-B13840850596}",
			0xa8d29e86, 0x62bc, 0x458b, 0x8a, 0x2b, 0xb1, 0x38, 0x40, 0x85, 0x5, 0x96);


//////////////////////////////////////////////////////////////////////////
// ROT:

class IRXRunTime;

class IRXClassFactory : public IUnknown
{
public:
	STDMETHOD(CreateInstance)(IUnknown *pco, IUnknown *punkOuter, const IID& riid, void **ppv) = 0;
	STDMETHOD(LockServer)(BOOL fLock) = 0;
public:
	RE_DECLARE_IID;
};
// {3B099D02-B5B7-4ba2-AA9D-8D921A97C703}
RE_DEFINE_IID(IRXClassFactory, "{3B099D02-B5B7-4ba2-AA9D-8D921A97C703}",
			0x3b099d02, 0xb5b7, 0x4ba2, 0xaa, 0x9d, 0x8d, 0x92, 0x1a, 0x97, 0xc7, 0x3);

//////////////////////////////////////////////////////////////////////////
// RunTime:

class IRERunningObjectTable : public IUnknown
{	// !!! revoke to prevent ref cycle !!!
public:
	STDMETHOD(Register)(const GUID& rpid, IUnknown *punk) = 0;
	STDMETHOD(Revoke)(const GUID& rpid) = 0;
	STDMETHOD(IsRunning)(const GUID& rpid) = 0;
	STDMETHOD(GetObject)(const GUID& rpid, IUnknown **ppunk) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IRERunningObjectTable, "{F4447B2E-89C7-4180-A2C6-390A7E539065}",
			  0xf4447b2e, 0x89c7, 0x4180, 0xa2, 0xc6, 0x39, 0xa, 0x7e, 0x53, 0x90, 0x65);
typedef IRERunningObjectTable	IREROT;
// for 2010, 2011, ... 
// simulate COM::CoCreateInstance
class IREObjectLoader : public IUnknown
{
public:
	STDMETHOD(CreateInstance)(IRERunningObjectTable *prot, const CLSID& rclsid, IUnknown *punkOuter, const IID& riid, void **ppv) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREObjectLoader, "{5220C9AC-0D9E-4f27-A660-DD0970C4F800}", 
			  0x5220c9ac, 0xd9e, 0x4f27, 0xa6, 0x60, 0xdd, 0x9, 0x70, 0xc4, 0xf8, 0x0);

class IRXRunTime : public IRERunningObjectTable
{	// !!! revoke to prevent ref cycle !!!
public:
	STDMETHOD(GetObjectEx)( REFGUID rpid, const GUID& iid, void **ppv) = 0;
	// revoke all in-table objects
	STDMETHOD(Dispose)() = 0;
public:
	RE_DECLARE_IID;
};
// {8482CD06-6DFC-479b-949B-848ABCECBE1B}
RE_DEFINE_IID(IRXRunTime, "{8482CD06-6DFC-479b-949B-848ABCECBE1B}",
			0x8482cd06, 0x6dfc, 0x479b, 0x94, 0x9b, 0x84, 0x8a, 0xbc, 0xec, 0xbe, 0x1b);

typedef IRXRunTime	IRXRunTime;
typedef IRXRunTime	IR4Context;
typedef IRXRunTime	IRXObjectTable;

// common platform components:
DEFINE_GUID(PID_ROT_FileSystem,			0x4e134c8c, 0x3779, 0x4b61, 0xb0, 0x16, 0xf2, 0x99, 0x51, 0x5d, 0xd4, 0x19);
DEFINE_GUID(PID_ROT_EngineConfig,		0xb4a02808, 0x4086, 0x4c3d, 0xb2, 0xfc, 0x6b, 0xc1, 0xc4, 0xef, 0x67, 0x92);
DEFINE_GUID(PID_ROT_LibLoader,			0x613bd9b2, 0x5554, 0x4bc5, 0x89, 0x18, 0x3c, 0xc8, 0xf7, 0x66, 0x9f, 0x53);
DEFINE_GUID(PID_ROT_ObjectLoader,		0x18a4be9f, 0x3c84, 0x4d46, 0xb1, 0xe1, 0xb3, 0x14, 0x79, 0x5c, 0x56, 0xa0);
DEFINE_GUID(PID_ROT_TableIndexMap,		0xc107d06f, 0x7229, 0x4f37, 0xad, 0x3c, 0x3d, 0xff, 0x42, 0xcb, 0x52, 0xa7);
DEFINE_GUID(PID_ROT_ObjectDB,			0x55ceb32b, 0x7e53, 0x4c92, 0xb8, 0xc1, 0x83, 0xdd, 0x31, 0xb, 0xc9, 0xf0);
// common engine objects map(2010)
DEFINE_GUID(PID_ROT_ArchiveMap,			0xc61793c0, 0x82b5, 0x44f3, 0x8e, 0xc0, 0x39, 0x98, 0x3a, 0xa9, 0xbe, 0x00);
DEFINE_GUID(PID_ROT_EngineMap,			0x47c425ff, 0xda93, 0x437e, 0x8a, 0x6e, 0x9c, 0x76, 0xd4, 0x52, 0x42, 0x3f);
// {EC60C650-D57E-4162-A8E2-E5BECAAA1A3D}
DEFINE_GUID(PID_ROT_StreamHooker,		0xec60c650, 0xd57e, 0x4162, 0xa8, 0xe2, 0xe5, 0xbe, 0xca, 0xaa, 0x1a, 0x3d);
// {4325A9D8-C124-49FF-B38C-8E12E785C0C0}
DEFINE_GUID(PID_ROT_ProcessCondition,	0x4325a9d8, 0xc124, 0x49ff, 0xb3, 0x8c, 0x8e, 0x12, 0xe7, 0x85, 0xc0, 0xc0);
// {C12C604A-8598-47F8-A0EA-163CC6FDE2ED}
DEFINE_GUID(PID_ROT_EngineParameter,	0xc12c604a, 0x8598, 0x47f8, 0xa0, 0xea, 0x16, 0x3c, 0xc6, 0xfd, 0xe2, 0xed);
// {7CB5C343-5DA5-4E2C-A883-B32C84E5469B}
DEFINE_GUID(PID_ROT_NameDB,				0x7cb5c343, 0x5da5, 0x4e2c, 0xa8, 0x83, 0xb3, 0x2c, 0x84, 0xe5, 0x46, 0x9b);
// {D9F5D9C4-7EEF-47EB-BF0E-164A67E15801}
DEFINE_GUID(PID_ROT_Thd3NameDB,			0xd9f5d9c4, 0x7eef, 0x47eb, 0xbf, 0xe, 0x16, 0x4a, 0x67, 0xe1, 0x58, 0x1);
// {762661CB-3FE7-4291-A8F0-4018FAF9A347}
DEFINE_GUID(PID_ROT_TsnClient,			0x762661cb, 0x3fe7, 0x4291, 0xa8, 0xf0, 0x40, 0x18, 0xfa, 0xf9, 0xa3, 0x47);

// {26F79991-B61B-4AA3-8F69-46A3AFBE949E}
DEFINE_GUID(PID_ROT_CondFilter,			0x26f79991, 0xb61b, 0x4aa3, 0x8f, 0x69, 0x46, 0xa3, 0xaf, 0xbe, 0x94, 0x9e);

// {7E8C3C7C-476D-4285-A7CC-04722686C296}
DEFINE_GUID(PID_ROT_DosEngineData,			0x7e8c3c7c, 0x476d, 0x4285, 0xa7, 0xcc, 0x4, 0x72, 0x26, 0x86, 0xc2, 0x96);


// {927963CE-6163-40DC-A523-AD488906CA40}
DEFINE_GUID(PID_ROT_MD5DB, 0x927963ce, 0x6163, 0x40dc, 0xa5, 0x23, 0xad, 0x48, 0x89, 0x6, 0xca, 0x40);

//////////////////////////////////////////////////////////////////////////

DEFINE_GUID( PID_File_MacroRawData, 
			0xfe62727e, 0x1c17, 0x428c, 0x9d, 0x5a, 0x44, 0x11, 0xe2, 0xbc, 0x85, 0xd2);
			
			// {93BB0BF9-660A-426A-B2E1-F571D0FED8BB}
DEFINE_GUID(PID_FILE_InternalFileName, 0x93bb0bf9, 0x660a, 0x426a, 0xb2, 0xe1, 0xf5, 0x71, 0xd0, 0xfe, 0xd8, 0xbb);


//////////////////////////////////////////////////////////////////////////
// Archive:

class IREStream;

class IREArchive : public IUnknown
{
public:
	STDMETHOD(Open)(IUnknown *punk) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(GetStreamObject)(IREStream **ppstm) PURE;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREArchive, "{F0DE6717-1B00-4b9e-8E45-0212E029457B}",
			  0xf0de6717, 0x1b00, 0x4b9e, 0x8e, 0x45, 0x2, 0x12, 0xe0, 0x29, 0x45, 0x7b);

class IRXArchive : public IREArchive
{
public:
	STDMETHOD(Open2)( IUnknown * punk, ULONG uFmtId ) PURE;
	STDMETHOD(GetFormatID)( ULONG * lpFmtId ) PURE;
public:
	RE_DECLARE_IID;
};

// {82CA12EF-1A94-4bb0-9959-416C7051FF26}
RE_DEFINE_IID( IRXArchive, "{82CA12EF-1A94-4bb0-9959-416C7051FF26}",
			0x82ca12ef, 0x1a94, 0x4bb0, 0x99, 0x59, 0x41, 0x6c, 0x70, 0x51, 0xff, 0x26);
/////////////////////////////////////////////////////////////////////////

enum EmbObjType
{
	E_EMBOBJ_UNPACKED,
	E_EMBOBJ_RESOURCE,
	E_EMBOBJ_OVERLAY,
	E_EMBOBJ_DUMPPED,
};

class IRXEmbObjContainer : public IUnknown
{
public:
	STDMETHOD(InsertEmbeded)( IREStream* pstm, EmbObjType uType ) = 0;
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IRXEmbObjContainer, "{4E04723C-8910-407B-B079-B67E0A3B6A43}",
			  0x4e04723c, 0x8910, 0x407b, 0xb0, 0x79, 0xb6, 0x7e, 0xa, 0x3b, 0x6a, 0x43);


//////////////////////////////////////////////////////////////////////////
static const char * const INFO_FIELD_DEFAULT_NAME = "";
class IRXArchiveInfoGather : public IUnknown
{
public:
	//static const char * const DEFAULT_FIELD_NAME = "";
	// if aFiledName == NULL, rInfoMap = info; 
	// if rInfoMap=="", rInfoMap[$default] = info; 
	// otherwise, rInfoMap[aFiledName] = info;
	STDMETHOD(GatherTo)( xv::Value & rInfoMap, LPCSTR aFiledName = NULL ) PURE;	// append a filed to rInfoMap, rInfoMap["xxx"] = {}
	STDMETHOD(AppendOuter)( LPCSTR aName, xv::Value & info) { return E_NOTIMPL; } ;	// append a filed to rInfoMap, rInfoMap["xxx"] = {}
	RE_DECLARE_IID;
};

// {D9DFF22A-4FE9-4E5D-9C45-F5387C58E042}
RE_DEFINE_IID( IRXArchiveInfoGather, "{D9DFF22A-4FE9-4E5D-9C45-F5387C58E042}",
0xd9dff22a, 0x4fe9, 0x4e5d, 0x9c, 0x45, 0xf5, 0x38, 0x7c, 0x58, 0xe0, 0x42);


//////////////////////////////////////////////////////////////////////////
//
// Component:
//
#include "ifileio.h"
#include "filefmt.h"
#include "icomparch.h"
#include "imdunit.h"
#include "imsslib.h"
//////////////////////////////////////////////////////////////////////////
#include "virlib/ilibloader.h"
#include "virlib/irsvirusinfotable.h"
#include "2012/ipearchive.h"
#include "2010/iengine.h"
#include "2010/iffr.h"
#include "2010/defclsid.h"
#include "2012/ipearchive.h"
#include "2012/ivm.h"
#include "mpe.h"
#include "irsodb.h"
#include "irxfront.h"
#include "ssigen.h"
#include "ilibup.h"
#include "iextmail.h"
#include "recfmt.h"
#include "rxicci.h"
#include "irxzdr.h"
#include "icrypto.h"
//#endif

class IRXPropXV : public IUnknown
{
public:
	STDMETHOD(Set)( LPCSTR pcName, xv::Value& val ) PURE;
	STDMETHOD(Get)( LPCSTR pcName, xv::Value& val ) PURE;
	STDMETHOD(Get)( LPCSTR pcName, xv::Value& val, xv::ValueType wantType ) PURE;
	STDMETHOD(Erase)( LPCSTR pcName ) PURE;
	STDMETHOD(Clear)() PURE;
	RE_DECLARE_IID;
};

// {12B35A21-D43A-4847-9513-1E826C49E74C}
RE_DEFINE_IID( IRXPropXV, "{12B35A21-D43A-4847-9513-1E826C49E74C}",
			0x12b35a21, 0xd43a, 0x4847, 0x95, 0x13, 0x1e, 0x82, 0x6c, 0x49, 0xe7, 0x4c);

//////////////////////////////////////////////////////////////////////////
//
//	IUnknown* can't be save
//

class IRXStoneProp : public IRXPropXV
{
public:
	//
	// open, 
	// 
	STDMETHOD(Open)( IREStream* lpstm ) PURE;
	STDMETHOD(Open2)( REFCXSTRP xFileName ) PURE;
	STDMETHOD(Flush)() PURE;
	STDMETHOD(EnableStone)( BOOL bStone ) PURE;
	RE_DECLARE_IID;
};
// {B5743E87-DBEB-45b5-B0C4-EB8458C7E0A4}
RE_DEFINE_IID(IRXStoneProp, "{B5743E87-DBEB-45b5-B0C4-EB8458C7E0A4}",
			  0xb5743e87, 0xdbeb, 0x45b5, 0xb0, 0xc4, 0xeb, 0x84, 0x58, 0xc7, 0xe0, 0xa4);

class IRXStoneObject : public IUnknown
{
public:
	STDMETHOD( Save )( IRXStoneProp* lpProp ) PURE;
	STDMETHOD( Load )( IRXStoneProp* lpProp ) PURE;
	RE_DECLARE_IID;
};

// {BDF1D412-4DF9-42cb-95B9-DD95DCCE1162}
RE_DEFINE_IID( IRXStoneObject, "{BDF1D412-4DF9-42cb-95B9-DD95DCCE1162}",
			  0xbdf1d412, 0x4df9, 0x42cb, 0x95, 0xb9, 0xdd, 0x95, 0xdc, 0xce, 0x11, 0x62);

class IRXProcessCondition : public IUnknown
{
public:
	STDMETHOD( Build )( IRXParameters* param ) PURE;
	STDMETHOD( Build )( SIZE_T argc, LPCSTR* argv ) PURE;
	STDMETHOD_( BOOL, IsMeet )( IREStream* stm ) PURE;
	RE_DECLARE_IID;
};

// {1DC55402-954B-46D5-B80C-0B105E5D31D0}
RE_DEFINE_IID( IRXProcessCondition, "{1DC55402-954B-46D5-B80C-0B105E5D31D0}",
			0x1dc55402, 0x954b, 0x46d5, 0xb8, 0xc, 0xb, 0x10, 0x5e, 0x5d, 0x31, 0xd0);

class IRXProcessFilter : public IUnknown
{
public:
	STDMETHOD( Build )() PURE;
	STDMETHOD( Build )(FFMTID fmtid) PURE;
	STDMETHOD_( BOOL, IsMeet )( IREStream* stm , IREFormatList* fmtlst) PURE;
	STDMETHOD_( BOOL, IsMeet )( IREStream* stm , FFMTID fmt) PURE;
	RE_DECLARE_IID;
};
// {64D41A77-19DD-4458-BFB7-8B84033067A9}
RE_DEFINE_IID(IRXProcessFilter, "{64D41A77-19DD-4458-BFB7-8B84033067A9}",
			0x64d41a77, 0x19dd, 0x4458, 0xbf, 0xb7, 0x8b, 0x84, 0x3, 0x30, 0x67, 0xa9);


//
//	Detecion Time Data Box
//

class IRXDTDataBox : public IUnknown
{
public:
	enum 
	{
		AUXID_USER = 0x80000000,
	};
	
	RE_DECLARE_IID;

	// AUX Stream 
	STDMETHOD(AuxStreamGet)( UINT32 uAuxId, IRXStream** ppstm ) PURE;
	STDMETHOD(AuxStreamSet)( UINT32 uAuxId, IRXStream* pstm ) PURE;

	/////////////////////////////////////////////////////////
	//
	//	Detecion Time Feature Collector APIs
	//
	//	|FeatureString||FeatureString||FeatureString|
	//

	// string cat, 
	STDMETHOD(DTFCPush)( LPCVOID aText, SIZE_T cbText ) PURE;
	// use sunday algo to search
	STDMETHOD(DTFCFind)( LPCVOID aText, SIZE_T cbText ) PURE;
	// get a memory block
	STDMETHOD(DTFCGetBlock)( RX_BLOB& rBlob ) PURE;

	/////////////////////////////////////////////////////////
	//
	//	Detecion Time X-Value Dict  APIs
	//

	STDMETHOD(DTXVGet)( LPCSTR aName, xv::Value& rValue ) PURE;
	STDMETHOD(DTXVSet)( LPCSTR aName, const xv::Value& rValue ) PURE;

	/////////////////////////////////////////////////////////
	//
	//	Hidden Report
	//		aInfoProgId means how to fetch extend-info
	//

	STDMETHOD(PushHiddenAlarm)( SGID40 uId ) PURE;
	STDMETHOD(GetHiddenAlarm)( SIZE_T uIndex, LPSGID40 lpID40 ) PURE;
	STDMETHOD_(SIZE_T, HiddenAlarmCount )() PURE;

#if 0// FOREVER DISABLE !!!! YECHAO // def GATHER_INFO_IMPL

	typedef enum GATHER_MODE
	{
		GATHER_GATHERED,			// Type=LPCSTR
		GATHER_BY_PROGID,			// Type=LPCSTR
		GATHER_BY_OBJECT,			// Type=IUnknown*, IRXInfoGather
		GATHER_BY_OBJECT_WP,		// Type=Object, ["o"] = IRXInfoGather, ["param"] = xxx

		GATHER_MAX,			
	} GATHERMODE ;

	typedef struct GATHER_INFO
	{
		LPCSTR		Tag;
		GATHERMODE	Mode;
	} GII, LPGII;

	STDMETHOD(GatherInfo)( LPCSTR aInfoTag, GATHERMODE uMode, xv::Value& xValue ) PURE;
	STDMETHOD(GetInfoToGather)( SIZE_T uIndex, GII& rGather, xv::Value& xValue ) PURE;
	STDMETHOD_(SIZE_T, InfoToGatherCount )() PURE;
#endif

	STDMETHOD(PushHiddenAlarm2)( LPCSTR strDescText ) PURE;
	STDMETHOD(GetHiddenAlarmEx)( SIZE_T uIndex, LPSGID40 lpID40, LPCSTR * lpDescName ) PURE;
};

// {192094FC-1FBD-4296-B4CE-AC9BFBE075DD}
RE_DEFINE_IID( IRXDTDataBox, "{192094FC-1FBD-4296-B4CE-AC9BFBE075DD}",
			0x192094fc, 0x1fbd, 0x4296, 0xb4, 0xce, 0xac, 0x9b, 0xfb, 0xe0, 0x75, 0xdd);

#ifdef GATHER_INFO_IMPL

class IRXInfoGather : public IUnknown
{
public:
	STDMETHOD( Gather )( IUnknown* punk, xv::Value& param, RX_BLOB& bInfoString ) PURE;
	RE_DECLARE_IID;
};

// {C77D29E8-5AF2-4922-8FAE-56EB72888C32}
RE_DEFINE_IID( IRXInfoGather, "{C77D29E8-5AF2-4922-8FAE-56EB72888C32}",
			0xc77d29e8, 0x5af2, 0x4922, 0x8f, 0xae, 0x56, 0xeb, 0x72, 0x88, 0x8c, 0x32);

#endif

#endif // duplicate inclusion protection
