#ifndef __TC_PE_ENG__
#define __TC_PE_ENG__

#include "tcbas.h"

//
//	数据类型
//

enum 
{
	avtINT = VNUM,
	avtRAW ,				// 文件偏移
	avtRVA,					// RVA	
	avtVA,					// 线性地址	
	// 以上是可互转的地址
	avtMapable,
	avtREC = avtMapable,	// 记录内偏移
	avtBUF,					// 公共缓冲区偏移
	// 以上是位置类型
	avtPosType,
	avtSetable = avtPosType,
	avtNativePtr,			
	avtMax,
};

//
//	虚拟机寄存器编号
//		顺序必须同VmiContext结构
//

enum 
{
	//
	_eax = CPUREG_EAX, _ecx = CPUREG_ECX, _edx = CPUREG_EDX, _ebx = CPUREG_EBX,
	_esp = CPUREG_ESP, _ebp = CPUREG_EBP, _esi = CPUREG_ESI, _edi = CPUREG_EDI,

	_efl = CPUREG_EFL, _eip = CPUREG_EIP,

	__cs = CPUREG_CS, __ds = CPUREG_DS, __es = CPUREG_ES, 
	__fs = CPUREG_FS, __gs = CPUREG_GS, __ss = CPUREG_SS,

	// 
	_cr0 = CPUREG_CR0, _cr2 = CPUREG_CR1, _cr3 = CPUREG_CR2, _cr4 = CPUREG_CR4,

	//
	_dr0 = CPUREG_DR0, _dr1 = CPUREG_DR1, _dr2 = CPUREG_DR2, _dr3 = CPUREG_DR3, _dr6 = CPUREG_DR6, _dr7 = CPUREG_DR7,

	// 
	x86r_max = CPUREG_MAX,
};

enum 
{
	DecodeInImage,
	DecodeInStack,
	DecodeInMemory,
	DecodeTypeMax,
};

//////////////////////////////////////////////////////////////////////////
//
//	only for pe-engine
//

// old engine stage, DO NOT CHANGE! 
//enum /* Engine Stage */
//{
//	ES_STATIC_SCAN = 0,						// after static scan, before dynmaic scan
//	ES_VM_LOADED,							// after vm load , before dynmaic scan, for patching vm
//	ES_DYNAMIC_SCAN,						// after dynamic scan, virt-machine is still alive, u can re-use vm
//	ES_AFTER_THIS_ONE,						// after lib scan, 
//	ES_AFTER_LAST_ONE,						// the last file in scan-chain scan completed
//	ES_VM_RESET_SCAN,						// full-control vm
//	ES_MAX_SCAN_STAGE,
//	ES_TREATING = ES_MAX_SCAN_STAGE,		// kill virus
//	ES_DEEP_SCAN_STAGE,						// DeepScan
//	ES_MAX_STAGE,
//};

enum ENG_STAGE
{	
	ES_SCAN						= 0,			
	ES2_TREAT					= 1,				// kill virus
	ES_MAX_STAGE
};

enum 
{
	ES_ACCESS_WAIT_VMEXIT		= 1,				// can wait vmexit event
	ES_ACCESS_WRITE_FILE		= 2,				// can write file
	ES_ACCESS_WRITE_VMM			= 4,				// can write virtual-process memory
	ES_ACCESS_CONTROL_VMM		= 8,				// can control virtual memory( alloc/free )
	ES_ACCESS_CONTROL_VMC		= 16,				// can control virtual machine( reload/ ...)
	ES_ACCESS_CHANGE_SOURCE		= 32,				// can change source target
	ES_ACCESS_CHANGE_DEST		= 64,				// can change dest target
	ES_ACCESS_CONTROL_VMR		= 128,				// control vm regs

	ES_ACCESS_WRITE_STREAM	= ES_ACCESS_WRITE_FILE|ES_ACCESS_WRITE_VMM,

	ES_ACCESS_SCAN			= ES_ACCESS_WAIT_VMEXIT|ES_ACCESS_CHANGE_SOURCE|ES_ACCESS_CONTROL_VMR,
	ES_ACCESS_TREAT			= ES_ACCESS_WRITE_STREAM|ES_ACCESS_CONTROL_VMC|ES_ACCESS_CONTROL_VMM|ES_ACCESS_CHANGE_SOURCE|ES_ACCESS_CONTROL_VMR|ES_ACCESS_CHANGE_DEST,
};

enum
{
	ORIGINAL_FILE,
	UNPACKED_FILE,
	SCANNING_FILE = -1,
};

static SIZE_T STAGE_PRIVILEGE[ES_MAX_STAGE] = 
{
	ES_ACCESS_SCAN,
	ES_ACCESS_TREAT,
};



#endif