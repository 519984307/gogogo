#ifndef _RE_EXPORT_INTERFACE_SCRIPTCI_H_
#define _RE_EXPORT_INTERFACE_SCRIPTCI_H_


class IScriptCompiler : public IUnknown
{
public:
	struct STErrorInfo
	{
		DWORD dwErrorType;
		DWORD dwLine;
	};

public:
	STDMETHOD(Compile)(IN const char* pScriptFile) = 0;
	STDMETHOD(Build)(IN const char* pObjFile) = 0;
	STDMETHOD_(void, GetLastError)(STErrorInfo* pstErr) = 0;
	
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IScriptCompiler, "{85E3EF54-0CD8-4222-87AE-66B2BA3F7349}",
0x85e3ef54, 0xcd8, 0x4222, 0x87, 0xae, 0x66, 0xb2, 0xba, 0x3f, 0x73, 0x49);

//////////////////////////////////////////////////////////////////////////

// {8BFD5EDA-FD82-4a35-B11D-5E7B4D11001C}
DEFINE_GUID(CLSID_CScriptCompiler, 
0x8bfd5eda, 0xfd82, 0x4a35, 0xb1, 0x1d, 0x5e, 0x7b, 0x4d, 0x11, 0x0, 0x1c);



class IREVM;
class IRERoutine;
class IRERoutineParam;
class IREEngineContext;

class IScriptInterpreter : public IUnknown
{
public:
	typedef unsigned long OEP;
	enum { INVALID_OEP = 0 };

	struct STParameterList 
	{
		IREVM *pVM;
		IRERoutine *pRoutine;
		IRERoutineParam *pRoutineParam;
		IREEngineContext *pFileSystem;
		const char *pszPackName;
		IREPEArchive *pPEArchive;
	};

	enum ENScriptRunResultValueType
	{
		SCRIPT_VT_NONE = 0,
		SCRIPT_VT_FILEOFFSET,
		SCRIPT_VT_MEMLINEAR,
		SCRIPT_VT_MEMRVA,
		// add new vt here...
	};

	struct STScriptRunScanResult
	{
		ENScriptRunResultValueType dwValueType;
		DWORD dwValue;
	};

public:
	STDMETHOD(Initialize)(STParameterList* pstParamList) = 0;
	STDMETHOD(Load)(IN const char* pBuf, unsigned short wLen) = 0;

	STDMETHOD_(unsigned long, RunScan)(IN STScriptRunScanResult* pstRunScanResult) = 0;
	STDMETHOD_(unsigned long, RunKill)() = 0;
	STDMETHOD_(unsigned long, RunUnpack)() = 0;

	STDMETHOD_(unsigned long, Step)() = 0;
	STDMETHOD_(OEP, GetOEP)() = 0;

public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IScriptInterpreter, "{69DB1445-E4E4-419a-A597-DB691DEF32D8}",
0x69db1445, 0xe4e4, 0x419a, 0xa5, 0x97, 0xdb, 0x69, 0x1d, 0xef, 0x32, 0xd8);

//////////////////////////////////////////////////////////////////////////

// {638BAC84-B8D5-420f-A355-F0F00911EB0F}
DEFINE_GUID(CLSID_PESTDEX_UnpackScript, 
0x638bac84, 0xb8d5, 0x420f, 0xa3, 0x55, 0xf0, 0xf0, 0x9, 0x11, 0xeb, 0xf);
#define CLSID_CScriptInterpreter	CLSID_PESTDEX_UnpackScript


#endif // #ifndef _RE_EXPORT_INTERFACE_SCRIPTCI_H_

