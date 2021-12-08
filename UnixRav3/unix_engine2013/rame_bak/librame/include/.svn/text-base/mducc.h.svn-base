#ifndef __MDU_CC__
#define __MDU_CC__

typedef enum PE10_NOTIFY_CODE
{
	NC_PE10_NONE = 0,
	NC_PE10_REVM_EXIT = 1,
	NC_PE10_STAGE_SCAN_1 = 2,
} NCC_PE10;

typedef DWORD PARAM_NC_PE10_REVM_EXIT;

struct PARAM_NC_PE10_STAGE_SCAN_1
{
	IUnknown*	lpArchive;
	DWORD		nScanOptionMask;
	BOOL *		pbNeedVMRun;
};

//////////////////////////////////////////////////////////////////////////

typedef enum TC11_CTRL_CODE
{
	CC_TC11_NONE = 0,
	CC_TC11_ADD_CUSTOM_REC,
	CC_TC11_DEL_CUSTOM_REC,
} CC_TC11;

typedef struct _PARAM_CC_TC11_ADD_CUSTOM_REC
{
	tcr::LPTCREC	lpRecord;
	SIZE_T			cbRecord;
} PARAM_CC_TC11_ADD_CUSTOM_REC;

typedef struct _PARAM_CC_TC11_DEL_CUSTOM_REC
{
	tcr::LPTCREC	lpRecord;
} PARAM_CC_TC11_DEL_CUSTOM_REC;



//////////////////////////////////////////////////////////////////////////

#endif