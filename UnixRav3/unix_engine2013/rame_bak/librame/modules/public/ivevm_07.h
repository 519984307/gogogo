#ifndef _VEVM_H_
#define _VEVM_H_

#ifndef NOT_INCLUDE_IREVM_07

#define I_EAX	0
#define I_ECX	1
#define I_EDX	2
#define I_EBX	3
#define I_ESP	4
#define I_EBP	5
#define I_ESI	6
#define I_EDI	7
#define I_ES	0
#define I_CS	1
#define I_SS	2
#define I_DS	3
#define I_FS	4
#define I_GS	5

//enum
//{
//	I_EAX	= 0,
//	I_ECX	= 1,
//	I_EDX	= 2,
//	I_EBX	= 3,
//	I_ESP	= 4,
//	I_EBP	= 5,
//	I_ESI	= 6,
//	I_EDI	= 7,
//	I_ES	= 0,
//	I_CS	= 1,
//	I_SS	= 2,
//	I_DS	= 3,
//	I_FS	= 4,
//	I_GS	= 5,
//};

struct VEVM_INSTR_INFO
{
	int		LastInstrLen;
	bool	bIsAPI;
};

#ifdef _VEX86_
class CDebugInterface;
#endif

class IVEVM_07 : public IUnknown
{
public:
#ifdef _VEX86_
	virtual	bool	Init(CDebugInterface*pDebugInterface = NULL){return true;}
#else
	virtual	bool	Init(void*pDebugInterface = NULL){return true;}
#endif
	//释放虚拟机
	//virtual	void	Release(){}
	virtual	bool	Open(const char*FileName){return true;}
	virtual	bool	OpenByRsFileObj(void*FileObj){return true;}
	//关闭执行中的PE文件
	virtual	bool	Close(){return true;}
	//虚拟执行(单线程)
	//Step: 执行指令条数
	virtual	bool	Step(){return false;}

	virtual bool	Reset(){return true;}
	virtual	void	SetExecuteAddres(DWORD LineAddr){}
	virtual	DWORD	GetExecuteAddres(VEVM_INSTR_INFO*pInstrInfo=NULL){return 0;}
	virtual DWORD	GetMainModuleBase(){return 0x400000;}

	virtual	DWORD	GetEFlagReg(){return 0;}
	virtual void	SetEFlagReg(DWORD Value){}

	virtual DWORD	GetGeneReg(int Index){return 0;}
	virtual void	SetGeneReg(int Index,DWORD Value){}

	virtual DWORD	GetEIPReg(){return 0;}
	virtual void	SetEIPReg(DWORD Value){}

	virtual WORD	GetSegReg(int Index){return 0;}
	virtual DWORD	GetSegBase(int Index){return 0;}

	virtual bool	SetSegReg(int Index,WORD value){return false;}
	virtual void 	SetSegBase(int Index,DWORD value){}

	virtual DWORD	GetLastReadMemAddr(){return 0;}
	virtual DWORD	GetLastReadMemSize(){return 0;}
	virtual DWORD	GetLastWriteMemAddr(){return 0;}
	virtual DWORD	GetLastWriteMemSize(){return 0;}

	virtual int		ReadVirtualMemory(DWORD Address,void*Buffer,int Size){return 0;}
	virtual int		ReadVirtualMemoryDeReloc(DWORD Address,void*Buffer,int Size){return 0;}
	virtual int		WriteVirtualMemory(DWORD Address,void*Buffer,int Size){return 0;}
	virtual DWORD	GetErrorMessage(char*szMsg){*szMsg=0;return 0;}
	virtual bool	EnableMemoryWriteProtect(bool bEnable){return true;}
	virtual bool	IsStackAddress(DWORD Address){return false;}

public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IVEVM_07, "{7D75CF20-3A88-4ecf-B6D0-266A3BE8DF64}",
0x7d75cf20, 0x3a88, 0x4ecf, 0xb6, 0xd0, 0x26, 0x6a, 0x3b, 0xe8, 0xdf, 0x64);


//////////////////////////////////////////////////////////////////////////


// Normal 虚拟机：
// {8EB96514-44D5-43fb-A03F-5D85DFDD553E}
DEFINE_GUID(CLSID_CRENormalVM_07, 
0x8eb96514, 0x44d5, 0x43fb, 0xa0, 0x3f, 0x5d, 0x85, 0xdf, 0xdd, 0x55, 0x3e);

// Win32 虚拟机：
// {1D860499-9AF4-444e-94A9-872595BF722C}
DEFINE_GUID(CLSID_CREWin32VM_07, 
0x1d860499, 0x9af4, 0x444e, 0x94, 0xa9, 0x87, 0x25, 0x95, 0xbf, 0x72, 0x2c);

// DOS 虚拟机：
// {31CD2CE8-281A-4a06-A414-647C76E81867}
DEFINE_GUID(CLSID_CREDOSVM_07, 
0x31cd2ce8, 0x281a, 0x4a06, 0xa4, 0x14, 0x64, 0x7c, 0x76, 0xe8, 0x18, 0x67);

// Boot 虚拟机：
// {1474AEB9-C761-40cb-9EF5-04AC6C030476}
DEFINE_GUID(CLSID_CREBootVM_07, 
0x1474aeb9, 0xc761, 0x40cb, 0x9e, 0xf5, 0x4, 0xac, 0x6c, 0x3, 0x4, 0x76);

#endif

#endif //_VEVM_H_
