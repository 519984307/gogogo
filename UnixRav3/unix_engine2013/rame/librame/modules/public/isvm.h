#ifndef _ONCE_ISVM_HEADER_
#define _ONCE_ISVM_HEADER_

#include <vector>

typedef bool(*TFNCallback)(void *param);

//内存读写
class IRESuperVMMemory
: public IUnknown
{	
public:
	enum ENUMEMORYINFO
	{
		MEMORYINFO_REMAINSIZE = 1,
		MEMORYINFO_ALLOCBASE,
	};

public:
	//内存读写
	STDMETHOD_(DWORD, ReadMemory)(ADDR linear, IN BYTE* pBuffer, DWORD nBufferSize) PURE;
	STDMETHOD_(BOOL, ReadMemoryByte)(DWORD addr, BYTE *pvalue) PURE;
	STDMETHOD_(BOOL, ReadMemoryWord)(DWORD addr, WORD *pvalue) PURE;
	STDMETHOD_(BOOL, ReadMemoryDword)(DWORD addr, DWORD *pValue) PURE;

	STDMETHOD_(DWORD, WriteMemory)(ADDR addr, const BYTE *pValue, DWORD Size) PURE;
	STDMETHOD_(BOOL, WriteMemoryByte)(DWORD addr, BYTE value) PURE;
	STDMETHOD_(BOOL, WriteMemoryWord)(DWORD addr, WORD value) PURE;
	STDMETHOD_(BOOL, WriteMemoryDword)(DWORD addr, DWORD value) PURE;

	STDMETHOD_(BOOL, ReadMemoryReg)(ENUMCPUREG32 cpureg, BYTE *pValue, DWORD size, DWORD offset = 0) PURE;
	STDMETHOD_(BOOL, ReadMemoryByteReg)(ENUMCPUREG32 cpureg, BYTE *pvalue, DWORD offset = 0) PURE;
	STDMETHOD_(BOOL, ReadMemoryWordReg)(ENUMCPUREG32 cpureg, WORD *pvalue, DWORD offset = 0) PURE;
	STDMETHOD_(BOOL, ReadMemoryDwordReg)(ENUMCPUREG32 cpureg, DWORD *pValue, DWORD offset = 0) PURE;

	STDMETHOD_(BOOL, WriteMemoryReg)(ENUMCPUREG32 cpureg, BYTE *pValue, DWORD size, DWORD offset = 0) PURE;
	STDMETHOD_(BOOL, WriteMemoryByteReg)(ENUMCPUREG32 cpureg, BYTE Value, DWORD offset = 0) PURE;
	STDMETHOD_(BOOL, WriteMemoryWordReg)(ENUMCPUREG32 cpureg, WORD Value, DWORD offset = 0) PURE;
	STDMETHOD_(BOOL, WriteMemoryDwordReg)(ENUMCPUREG32 cpureg, DWORD Value, DWORD offset = 0) PURE;

	//堆栈读写
	STDMETHOD_(BOOL, Push)(DWORD dwValue) PURE;
	STDMETHOD_(BOOL, Pop)(DWORD *pValue) PURE;
	STDMETHOD_(BOOL, Pop)() PURE;

	//////////////////////////////////////////////////////////////////////////
	//内存搜索
	STDMETHOD_(DWORD, SearchMemory)(DWORD addr, DWORD addrsize, const BYTE *mark, DWORD marksize, BOOL bWildcard=FALSE) PURE;
	STDMETHOD_(DWORD, SearchMemoryReg)(ENUMCPUREG32 cpureg, DWORD addrsize, const BYTE *mark, DWORD marksize, BOOL bWildcard = FALSE) PURE;
	//内存信息查询
	STDMETHOD_(BOOL, GetMemoryInfo)(DWORD addr, DWORD *info, ENUMEMORYINFO type) PURE;
	STDMETHOD_(BOOL, GetMemoryRemainSize)(DWORD addr, DWORD *size)  PURE;

	//虚拟内存管理的	
	STDMETHOD_(ADDR, VirtualAlloc)(ADDR linear, DWORD dwSize, DWORD flAllocationType, DWORD flProtect)  PURE;
	STDMETHOD_(BOOL, VirtualProtect)(ADDR linear, DWORD dwSize, DWORD flNewProtect, DWORD *pflOldProtect)  PURE;
	STDMETHOD_(DWORD, VirtualQuery)(ADDR linear, OUT VMMEMORY_BASIC_INFORMATION *pBuffer, DWORD dwLength)  PURE;
	STDMETHOD_(BOOL, VirtualFree)(ADDR linear, DWORD dwSize, DWORD dwFreeType)  PURE;
	//
	//STDMETHOD_(DWORD, ReadMemory)(ADDR linear, IN BYTE* pBuffer, DWORD nBufferSize)  PURE;
	//STDMETHOD_(DWORD, WriteMemory)(ADDR linear, const BYTE* pBuffer, DWORD nBufferSize)  PURE;


public:
	RE_DECLARE_IID;
};


//流程控制
class IRESuperVMDebugger
: public IUnknown
{
public:
	//基本的调试控制
	//基本调试Go1
	STDMETHOD_(BOOL, GoStep)()	PURE;											//F7	-	F11
	STDMETHOD_(BOOL, GoStepOver)() PURE;										//F8	-	F10
	STDMETHOD_(BOOL, Goto)(DWORD addr) PURE;									//F4	-	Run To Cursor
	STDMETHOD_(DWORD,  Go)(ENUMGOMODE mode = GO_DEFAULT, DWORD dwVMNotify = 0) PURE;				//F9	-	F5
	//扩展调试Go2-页跳相关	
	STDMETHOD_(BOOL, GoJump)() PURE;
	STDMETHOD_(BOOL, GoModify)() PURE;
	STDMETHOD_(BOOL, GoPageJump)() PURE;

	//扩展调试Go3
	STDMETHOD_(BOOL, GoStepCount)(DWORD count) PURE;
	STDMETHOD_(BOOL, GotoOffset)(DWORD offset) PURE;
	STDMETHOD_(BOOL, GoReturn)(DWORD maxstep = 0x1000) PURE;						//Alt+F9
	STDMETHOD_(BOOL, GotoInField)(DWORD VaMin, DWORD VaMax) PURE;//[VaMin, VaMax)
	STDMETHOD_(BOOL, GotoOutField)(DWORD VaMin, DWORD VaMax) PURE;//-VaMin), [VaMax-	
	//STDMETHOD_(BOOL, GotoUserCode)();										//不需要

	//Ctrl+F9

	//扩展调试Go4
	STDMETHOD_(BOOL, GotoStopOnExcept)(DWORD addr, BOOL * isExcept = NULL) PURE;
	STDMETHOD_(BOOL, GotoSelect)(DWORD selectcount, DWORD *BreakPoints, DWORD *retvalue) PURE;



	//扩展调试Go5-页跳相关
	STDMETHOD_(BOOL, PageJumpSection)(DWORD section, DWORD jumpcount = 0) PURE; // 注意: section 从0开始	
	STDMETHOD_(BOOL, PageJumpFirstSection)() PURE;//GotoInField(DWORD VaMin, DWORD VaMax);
	STDMETHOD_(BOOL, PageJumpAnotherSection)(DWORD jumpcount = 0) PURE;//GotoOutFidle(DWORD VaMin, DWORD VaMax);//
	STDMETHOD_(BOOL, PageJumpByLastEip)(const BYTE* codes, const DWORD lens, const DWORD offset = 0, const DWORD jumpcount = 0) PURE;
	STDMETHOD_(BOOL, PageJumpStack)(BOOL bInto, DWORD jumpcount = 0) PURE;
	STDMETHOD_(BOOL, PageJumpStackInto)(DWORD jumpcount = 0) PURE;
	STDMETHOD_(BOOL, PageJumpStackOut)(DWORD jumpcount = 0) PURE;


	//基本断点控制
	STDMETHOD_(BPHANDLE, SetBreakpoint)(PST_BPDESC  pstDesc) PURE;
	STDMETHOD_(BOOL, GetBreakpointInfo)(BPHANDLE hBP, PST_BPDESC pstDesc) PURE;
	STDMETHOD_(BOOL, DeleteBreakpoint)(BPHANDLE hBP) PURE;
	STDMETHOD_(BOOL, DisableBreakpoint)(BPHANDLE hBP) PURE;
	STDMETHOD_(BOOL, EnableBreakpoint)(BPHANDLE hBP) PURE;

	STDMETHOD_(BPHANDLE, GetLastBreakpointHandle)() PURE;

	//寄存器修改
	STDMETHOD_(BOOL, GetCPUContext32)(ST_CPUCONTEXT32 *pstCPUContext) PURE;
	STDMETHOD_(BOOL, SetCPUContext32)(ST_CPUCONTEXT32 *pstCPUContext) PURE;
	STDMETHOD_(DWORD, GetCPURegValue32)(ENUMCPUREG32 reg) PURE;
	STDMETHOD_(BOOL, SetCPURegValue32)(ENUMCPUREG32 reg, DWORD dwValue) PURE;
	//高级调试相关

	
   

	// execute handle
	STDMETHOD_(BOOL, RegisterBPXHandle)(DWORD addr, TFNCallback handle, void *param) PURE;
	STDMETHOD_(BOOL, RegisterBPMWHandle)(DWORD addr, TFNCallback handle, void *param) PURE;
	STDMETHOD_(BOOL, RegisterBPMRWHandle)(DWORD addr, TFNCallback handle, void *param) PURE;
	// api handle
	STDMETHOD_(BOOL, RegisterAPIHandle)(const char *apiname, TFNCallback handle, void *param, const char *dllname = "KERNEL32.DLL") PURE;
	// Exception handle
	// 注意，一旦存在异常回掉，那么程序执行中就不可能产生异常
	STDMETHOD_(BOOL, SetUserExceptionCallBack)(TFNCallback handle, void *param) PURE;

public:
	RE_DECLARE_IID;
};



//断点设置??

//虚拟机调试/状态信息/Dump?//

//主要用于输出调试信息
class IRESuperVMDumper
:public IUnknown
{
public:
	STDMETHOD_(void,  DumpPE)(char *szFileName) PURE;
	STDMETHOD_(void,  DumpStatus)() PURE;
	STDMETHOD_(void,  DumpEIP)() PURE;
	STDMETHOD_(void,  DumpLastEIP)() PURE;
	STDMETHOD_(void,  DumpRunState)() PURE;//运行状态??
	STDMETHOD_(void,  DumpLastException)() PURE;

	STDMETHOD_(void,  DumpReg)() PURE;
	STDMETHOD_(void,  DumpMemory)(DWORD address, DWORD len, char *szFileName = NULL) PURE;
	STDMETHOD_(void,  DumpMemoryReg)(ENUMCPUREG32 reg, DWORD len, char *szFileName = NULL) PURE;

	STDMETHOD_(void,  DumpAddressName)(DWORD address) PURE;//输出地址的名字, 可以显示API的名字	
	//STDMETHOD_(HRESULT,  DumpAddressInfo(DWORD address);

	STDMETHOD_(void,  DumpSEHChain)() PURE;

	STDMETHOD_(void,  DumpFileName)() PURE;//输出

	STDMETHOD_(void,  DumpModules)() PURE;//输出所加载的Module信息

public:
	RE_DECLARE_IID;

};


class IRESuperVMHelper
:public IUnknown
{
public:
	STDMETHOD_(DWORD, GetInstructionLen)(ADDR linear) PURE;
	STDMETHOD_(ADDR, Load) (IN IREStream *pFileObj) PURE;
	STDMETHOD_(void, Unload) () PURE;
	STDMETHOD_(ADDR, Reload)() PURE;
	STDMETHOD_(HRESULT, GetInFileObj)(IREStream** ppstm) PURE;
	STDMETHOD_(ADDR, GetMainModuleImageBase)() PURE;
	STDMETHOD_(DWORD, GetMainModuleImageSize)() PURE;
	STDMETHOD_(ADDR, GetProcAddress)(const char* pProcName, const char* pModuleName) PURE;
	STDMETHOD_(DWORD, GetProcAddress)(DWORD dwOrder, const char *pModuleName) PURE;
	
	STDMETHOD_(ADDR, GetModuleHandle)(const char* pModuleName) PURE;
	STDMETHOD_(ADDR, GetTEBAddress)() PURE;
	STDMETHOD_(void, GetVMConfig)(ST_VMCONFIG& stVMConfig) PURE;
	STDMETHOD_(void, SetVMConfig)(ST_VMCONFIG stVMConfig) PURE;
	

	STDMETHOD_(DWORD, GetLastException)() PURE;
	STDMETHOD_(DWORD, GetLastEIP)() PURE;
	STDMETHOD_(DWORD, GetCPURunState)() PURE;

	STDMETHOD_(BOOL, DumpPE)(IN IREStream *pDumpFileObj, ADDR dwOEP, ADDR dwImportTableVA = 0, DWORD dwImportTableSize = 0) PURE;	

	STDMETHOD_(DWORD, GetCurrentDecodeJump)() PURE;
	STDMETHOD_(BOOL, GetDecodeJumpRange)(DWORD desc, OUT ADDR *paddrBeg, OUT ADDR *paddrEnd) PURE;
	STDMETHOD_(void, GetLastWriteRegion)(ADDR& linear, DWORD& dwSize) PURE;
	STDMETHOD_(DWORD, GetVMTickCount)() PURE;

	STDMETHOD_(BOOL, RegAdd)(ENUMCPUREG32 cpureg, DWORD value, ENUMCPUREG32 cpureg2 = CPUREG_INVALID) PURE;

	STDMETHOD_(BOOL, GetImageSectionObj)(void ** obj) PURE;
	STDMETHOD_(BOOL, GetMaxSectionSize)(DWORD *size) PURE;
	STDMETHOD_(BOOL, GetFileOverlayPos)(DWORD *pos) PURE;
	
	STDMETHOD_(BOOL, GetLastDecodeRange)(IN OUT PST_DecodeRange pstImage, IN OUT PST_DecodeRange pstStack, IN OUT PST_DecodeRange pstMem) PURE;

public:
	RE_DECLARE_IID;
};

class IRESuperVMConfig
	:public IUnknown
{
public:
	STDMETHOD_(void, SetVM)(IREVM *pvm) PURE;
	STDMETHOD_(IREVM *,GetVM)() PURE; 
public:
	RE_DECLARE_IID;
};

class IREImageSection
:public IUnknown
{
public:
	typedef std::pair<DWORD, DWORD>  pairDword;
	//STDMETHOD_(void, SetVM)(IREVM *pvm) PURE;
	//STDMETHOD_(IREVM *,GetVM)() PURE; 

	//STDMETHOD_(BOOL, Init)(IRERunningObjectTable *prot, IREStream *file) PURE;

	//使用之前必须SetFileObject
	STDMETHOD_(BOOL, SetFileObject)(IREStream *file) PURE;

	STDMETHOD_(BOOL, GetSectionRange)(DWORD section, DWORD *left, DWORD *right) PURE;
	STDMETHOD_(BOOL, GetSectionByAddr)(DWORD addr, DWORD *section) PURE;
	STDMETHOD_(BOOL, GetSectionNum)(DWORD *sectionnum) PURE;
	STDMETHOD_(BOOL, GetFileOverlayPos)(DWORD *pos) PURE;
	STDMETHOD_(BOOL, GetResource)(DWORD &offset, DWORD & size, RS_WORD idDir, RS_WORD idItem) PURE;
	
	STDMETHOD_(const std::vector< pairDword > &, GetSectionsObj)() PURE;
	STDMETHOD_(const std::vector< pairDword > & , GetFileSectionsObj)() PURE;
	STDMETHOD_(const IREPERecordTable::IMAGE_SECTION_HEADER  *, GetSectionHeaders)() PURE;
		
public:	
	RE_DECLARE_IID;
};


RE_DEFINE_IID(IRESuperVMDebugger, "{EECCD30E-F2AB-490b-A414-35682E5956F8}",
			  0xeeccd30e, 0xf2ab, 0x490b, 0xa4, 0x14, 0x35, 0x68, 0x2e, 0x59, 0x56, 0xf8);


RE_DEFINE_IID(IRESuperVMDumper, "{E6D1F5F2-FA04-42a9-A287-2AD816A9EEB4}",
			  0xe6d1f5f2, 0xfa04, 0x42a9, 0xa2, 0x87, 0x2a, 0xd8, 0x16, 0xa9, 0xee, 0xb4);


RE_DEFINE_IID(IRESuperVMMemory, "{C2712145-B550-4d1a-8270-E1E93D3B0B4E}",
			  0xc2712145, 0xb550, 0x4d1a, 0x82, 0x70, 0xe1, 0xe9, 0x3d, 0x3b, 0xb, 0x4e);



RE_DEFINE_IID(IRESuperVMHelper, "{FEAC98CD-304A-44ee-B94C-1D6573CF61E2}",
			  0xfeac98cd, 0x304a, 0x44ee, 0xb9, 0x4c, 0x1d, 0x65, 0x73, 0xcf, 0x61, 0xe2);


RE_DEFINE_IID(IRESuperVMConfig, "{6A6A10C2-0907-4f3f-B7AB-4226D4EE9E53}",
			  0x6a6a10c2, 0x907, 0x4f3f, 0xb7, 0xab, 0x42, 0x26, 0xd4, 0xee, 0x9e, 0x53);


RE_DEFINE_IID(IREImageSection, "{A9153021-13B0-434e-82A7-38057764B1B0}",
			0xa9153021, 0x13b0, 0x434e, 0x82, 0xa7, 0x38, 0x5, 0x77, 0x64, 0xb1, 0xb0);


#endif
