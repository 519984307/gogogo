#ifndef __RSENGINE_EXCEPTION_LEVEL_INTERFACE_H__
#define __RSENGINE_EXCEPTION_LEVEL_INTERFACE_H__


/*********
*  用于设置异常级别：
*********/
enum EXCEPTION_LEVEL_FIELD
{
	ELF_bEnableTraceDJ = 1,
	ELF_bAPIAsException = 2,
	ELF_bNullLoopAsException = 4,
	ELF_dwExceptionLimit = 8
};
typedef struct tagSTEXCEPTIONLEVEL
{
	DWORD dwValidFieldMask;     //有效域标志（enum TRACE_RESULT）
	BOOL  bEnableTraceDJ;       //是否跟踪解密、跳转
	BOOL  bAPIAsException;      //中断指令或API调用是否算异常
	BOOL  bNullLoopAsException; //空循环是否算异常
	DWORD dwExceptionLimit;     //允许最大异常数
}STEXCEPTIONLEVEL;


#endif // #ifndef __RSENGINE_EXCEPTION_LEVEL_INTERFACE_H__
