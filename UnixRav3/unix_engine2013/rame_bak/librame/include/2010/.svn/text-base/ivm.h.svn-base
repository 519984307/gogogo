#ifndef _RE_EXPORT_INTERFACE_VM_H_2010_
#define _RE_EXPORT_INTERFACE_VM_H_2010_

#ifndef NOT_INCLUDE_IREVM_10

#ifndef IS_COMPILER_VC6
	#ifdef WIN32
		#if(_MSC_VER < 1300)
			#define IS_COMPILER_VC6 1
		#else
			#define IS_COMPILER_VC6 0
		#endif
	#else
		#define IS_COMPILER_VC6 0
	#endif
#endif

#if(IS_COMPILER_VC6)
	typedef SIZE_T	ADDR;
#else
	typedef SIZE_T	ADDR;
#endif


typedef UINT32				BPHANDLE;
#define INVALID_BPHANDLE	((BPHANDLE)-1)


//异常号
enum
{
	VM_EXCEPTION_DE = 0,   // Divide Error (fault)
	VM_EXCEPTION_DB = 1,   // Debug (fault/trap)
	VM_EXCEPTION_BP = 3,   // Breakpoint (trap)
	VM_EXCEPTION_OF = 4,   // Overflow (trap)
	VM_EXCEPTION_BR = 5,   // BOUND (fault)
	VM_EXCEPTION_UD = 6,   // Invalid Opcode
	VM_EXCEPTION_NM = 7,   // Device Not Available
	VM_EXCEPTION_DF = 8,   // Double Fault
	VM_EXCEPTION_TS = 10,  // Invalid TSS
	VM_EXCEPTION_NP = 11,  // Segment Not Present
	VM_EXCEPTION_SS = 12,  // Stack-Segment Fault
	VM_EXCEPTION_GP = 13,  // General Protection
	VM_EXCEPTION_PF = 14,  // Page Fault
	VM_EXCEPTION_MF = 16,  // FPU Floating-Point Error
	VM_EXCEPTION_AC = 17,  // Alignment Check
	VM_EXCEPTION_MC = 18,  // Machine Check
	VM_EXCEPTION_XM = 19,  // SIMD Floating-Point Exception

	VM_EXCEPTION_MAX ,
};


//虚拟机通知类型
enum
{
	VM_NOTIFY_EXCEPTION = 0x00000001,  //异常通知
	VM_NOTIFY_API	   = 0x00000002,  //API通知
};


enum ENUMGORESULT
{
	GR_OK = 1,
	GR_API,
	GR_BP,
	GR_EXCEPTION,
	GR_FAILED
};

enum ENUMGOMODE
{
	GO_DEFAULT = 1,
	GO_STEP,
	GO_JUMP,
	GO_DECODE,
	GO_LOOP,
	GO_PAGEJUMP,
	GO_STOPDECODEMEM,
	GO_SCAN,
	GO_STEPOVER,
	GO_UNCACHED_JMP,
	//GO_UNCACHED_,
};

enum ENUMRUNSTATE
{
	RUN_NORMAL = 1,
	RUN_JUMP,
	RUN_PAGEJUMP,
	RUN_LOOP_START,
	RUN_LOOP_END,
	RUN_DECODED,
	RUN_INDECODEMEM
};


enum ENUMCPUREG32
{
	CPUREG_INVALID	=	0,
	//gen
	CPUREG_EAX = 1,
	CPUREG_ECX,
	CPUREG_EDX,
	CPUREG_EBX,

	CPUREG_ESP,
	CPUREG_EBP,
	CPUREG_ESI,
	CPUREG_EDI,
	CPUREG_EIP,

	//DR
	CPUREG_DR0,
	CPUREG_DR1,
	CPUREG_DR2,
	CPUREG_DR3,
	CPUREG_DR6,
	CPUREG_DR7,

	//CR
	CPUREG_CR0,
	CPUREG_CR1,
	CPUREG_CR2,
	CPUREG_CR3,
	CPUREG_CR4,

	// EFL
	CPUREG_EFL,

	CPUREG_CS,
	CPUREG_DS,
	CPUREG_ES,
	CPUREG_FS,
	CPUREG_GS,
	CPUREG_SS,

	CPUREG_MAX,
};


typedef struct _tagCPUContext32 {
	//DR寄存器
	DWORD   Dr0;
	DWORD   Dr1;
	DWORD   Dr2;
	DWORD   Dr3;
	DWORD   Dr6;
	DWORD   Dr7;
	//段寄存器
	DWORD   SegGs;
	DWORD   SegFs;
	DWORD   SegEs;
	DWORD   SegDs;
	DWORD   SegCs;
	DWORD   SegSs;
	//
	DWORD   Ebp;
	DWORD   Eip;
	DWORD   Esp;
	//
	DWORD   EFlags;
	//
	DWORD   Edi;
	DWORD   Esi;
	DWORD   Ebx;
	DWORD   Edx;
	DWORD   Ecx;
	DWORD   Eax;
}ST_CPUCONTEXT32, *PST_CPUCONTEXT32;


enum ENUMBPTYPE
{
	BPTYPE_BPX	= 0,
	BPTYPE_BPX_DJ = 1,
	BPTYPE_BPM_W  = 2,
	BPTYPE_BPM_RW = 3
};


typedef struct _tagBPDesc
{
	ADDR linear;	// EA of Breakpoint, 0 if none!
	ENUMBPTYPE type;	// Breakpoint type.
	DWORD desc;	// Extend Breakpoint Info
	// BPTYPE_BPX 时为1
	// BPTYPE_BPX_DJ 高16位为解密次数 低16位跳转次数
	// BPTYPE_BPM_W  以linear值作为基址的区域
	// BPTYPE_BPM_RW 以linear值作为基址的区域
}ST_BPDESC, *PST_BPDESC;


typedef struct _VMMEMORY_BASIC_INFORMATION
{
	ADDR	BaseAddress;
	ADDR	AllocationBase;
	DWORD   AllocationProtect;
	DWORD   RegionSize;
	DWORD   State;
	DWORD   Protect;
	DWORD   Type;
}VMMEMORY_BASIC_INFORMATION, *PVMMEMORY_BASIC_INFORMATION; 


typedef struct _tagVMConfig{
	DWORD	dwScanLimit;
	DWORD	dwNormalLimit;
	DWORD	dwExceptionMaxTime;
}ST_VMCONFIG, *PST_VMCONFIG;


typedef struct _tagDecodeRange{
	ADDR begin;
	ADDR end;
	DWORD count; // number of bytes write
}ST_DecodeRange, *PST_DecodeRange;


class IREVM : public IUnknown
{
public:
	STDMETHOD_(ADDR, Load)(IN IREStream *pstm) = 0; // return IMAGE_BASE, 0 if failed.
	STDMETHOD_(void, Unload)() = 0;
	STDMETHOD_(ADDR, Reload)() = 0; // return IMAGE_BASE, 0 if failed.
	STDMETHOD(GetInFileObj)(IREStream** ppstm) = 0;
	STDMETHOD_(ADDR, GetMainModuleImageBase)() = 0;
	STDMETHOD_(DWORD, GetMainModuleImageSize)() = 0;

	STDMETHOD_(BOOL, GetCPUContext32)(ST_CPUCONTEXT32 *pstCPUContext) = 0;
	STDMETHOD_(BOOL, SetCPUContext32)(ST_CPUCONTEXT32 *pstCPUContext) = 0;

	STDMETHOD_(DWORD, GetCPURegValue32)(ENUMCPUREG32 reg) = 0;
	STDMETHOD_(BOOL, SetCPURegValue32)(ENUMCPUREG32 reg, DWORD dwValue) = 0;

	//32位的虚拟机内存读写
	//返回读取的大小
	STDMETHOD_(DWORD, ReadMemory)(ADDR linear, IN BYTE* pBuffer, DWORD nBufferSize) = 0;
	STDMETHOD_(DWORD, WriteMemory)(ADDR linear, const BYTE* pBuffer, DWORD nBufferSize) = 0;

	//虚拟内存操作
	STDMETHOD_(ADDR, VirtualAlloc)(ADDR linear, DWORD dwSize, DWORD flAllocationType, DWORD flProtect) = 0;
	STDMETHOD_(BOOL, VirtualProtect)(ADDR linear, DWORD dwSize, DWORD flNewProtect, DWORD *pflOldProtect) = 0;
	STDMETHOD_(DWORD, VirtualQuery)(ADDR linear, OUT VMMEMORY_BASIC_INFORMATION *pBuffer, DWORD dwLength) = 0;
	STDMETHOD_(BOOL, VirtualFree)(ADDR linear, DWORD dwSize, DWORD dwFreeType) = 0;

	//返回虚拟机内存的线性地址
	STDMETHOD_(ADDR, GetProcAddress)(const char* pProcName, const char* pModuleName) = 0;
	//返回虚拟机支持的模块基址
	STDMETHOD_(ADDR, GetModuleHandle)(const char* pModuleName) = 0;

	//返回主线程线程环境块的线性基址
	STDMETHOD_(ADDR, GetTEBAddress)() = 0;

	STDMETHOD_(BPHANDLE, SetBreakpoint)(PST_BPDESC  pstDesc) = 0;
	STDMETHOD_(BOOL, GetBreakpointInfo)(BPHANDLE hBP, PST_BPDESC pstDesc) = 0;
	STDMETHOD_(BOOL, DeleteBreakpoint)(BPHANDLE hBP) = 0;
	STDMETHOD_(BOOL, DisableBreakpoint)(BPHANDLE hBP) = 0;
	STDMETHOD_(BOOL, EnableBreakpoint)(BPHANDLE hBP) = 0;

	STDMETHOD_(DWORD, Go)(ENUMGOMODE mode, DWORD dwVMNotify) = 0;		// return ENUMGORESULT
	STDMETHOD_(DWORD, GetLastException)() = 0;
	STDMETHOD_(BPHANDLE, GetLastBreakpointHandle)() = 0;
	STDMETHOD_(DWORD, GetLastEIP)() = 0;

	STDMETHOD_(DWORD, GetInstructionLen)(ADDR linear) = 0;

	STDMETHOD_(DWORD, GetCPURunState)() = 0;
	STDMETHOD_(DWORD, GetCurrentDecodeJump)() = 0; // return D.J. desc
	STDMETHOD_(BOOL, GetDecodeJumpRange)(DWORD desc, OUT ADDR *paddrBeg, OUT ADDR *paddrEnd) = 0;

	//dwOEP 是线性地址,不是RVA
	STDMETHOD_(BOOL, DumpPE)(IN IREStream *pstmDumpFileObj, ADDR dwOEP, ADDR dwImportTableVA = 0, DWORD dwImportTableSize = 0) = 0;

	//
	STDMETHOD_(void, GetLastWriteRegion)(ADDR& linear, DWORD& dwSize) = 0;
	STDMETHOD_(void, GetVMConfig)(ST_VMCONFIG& stVMConfig) = 0;
	STDMETHOD_(void, SetVMConfig)(ST_VMCONFIG stVMConfig) = 0;
	STDMETHOD_(DWORD, GetVMTickCount)() = 0;

	STDMETHOD_(BOOL, GetLastDecodeRange)(IN OUT PST_DecodeRange pstImage, IN OUT PST_DecodeRange pstStack, IN OUT PST_DecodeRange pstMem) = 0;

	STDMETHOD_(LPCSTR, GetLastApiToCall)( LPCSTR * lpModule ) = 0;
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREVM, "{BEE51A17-A401-492a-A57E-808A9FDDB4E2}",
0xbee51a17, 0xa401, 0x492a, 0xa5, 0x7e, 0x80, 0x8a, 0x9f, 0xdd, 0xb4, 0xe2);

class IREVMApiCrc : public IUnknown {
public:
	STDMETHOD_(BOOL, GetFirstApiCrc)(OUT DWORD* crc) = 0;
	STDMETHOD_(BOOL, GetNextApiCrc)(OUT DWORD* crc) = 0;
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREVMApiCrc, "{27b2aff7-ef06-47ec-a94e-857e8d07c731}",
0x27b2aff7, 0xef06, 0x47ec, 0xa9, 0x4e, 0x85, 0x7e, 0x8d, 0x07, 0xc7, 0x31);

//////////////////////////////////////////////////////////////////////////


// {72F0D890-962C-4a49-9CC9-D0D63AC0DD4F}
DEFINE_GUID(CLSID_CREVM, 
0x72f0d890, 0x962c, 0x4a49, 0x9c, 0xc9, 0xd0, 0xd6, 0x3a, 0xc0, 0xdd, 0x4f);

#endif

#endif	// #ifndef _RE_EXPORT_INTERFACE_VM_H_

