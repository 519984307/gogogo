#ifndef __RX_INTERNAL_FUNCTIONAL__
#define __RX_INTERNAL_FUNCTIONAL__

#include "recfmt.h"
#include "2010/ivm.h"
#include "tc/tcpe.h"

namespace dbg
{
	class ITCEPoorDebugger : public IUnknown
	{
	public:

		enum RecEvent
		{
			OnRecSet,			// 即将开始跑这条记录
			OnRecStep,			// 正常的单步事件
			OnRecOver,			// 记录跑完了
			OnRecBadOp,			// 遇到了不认识的OPCODE
			OnRecError,			// 遇到了指令错误（指令解析）
			OnRecBreak,			// 遇到了指令跳出（指令执行）
			OnRecLimitSteps,	// 跑到了限制步数
		};

	public:

		STDMETHOD_( SIZE_T, GetCustomRecordNum)() PURE;

		STDMETHOD_(tcr::LPTCREC, GetCustomRecord)( SIZE_T uIndex ) PURE;

		STDMETHOD(OnRecEvent)( IUnknown* lpImpl, RecEvent uIdEvent, LPEMUX lpEmuCtx, BOOL& bContinueDebug ) PURE;

		RE_DECLARE_IID;
	};

	// {BA5C487E-06E0-4799-A912-08D4A41C487F}
	RE_DEFINE_IID( ITCEPoorDebugger, "{BA5C487E-06E0-4799-A912-08D4A41C487F}",
		0xba5c487e, 0x6e0, 0x4799, 0xa9, 0x12, 0x8, 0xd4, 0xa4, 0x1c, 0x48, 0x7f);

	// {7D5D250C-069B-4ECF-9B71-C779547F7BEE}
	DEFINE_GUID(PID_ROT_TCE_Poor_Debugger, 0x7d5d250c, 0x69b, 0x4ecf, 0x9b, 0x71, 0xc7, 0x79, 0x54, 0x7f, 0x7b, 0xee);


};

#endif