#ifndef _I_COMPILER_LIB_H_
#define _I_COMPILER_LIB_H_

// 识别指令序列长度
#define CODE_SEQUENCE_LEN 0x10

// 识别库
class ICodeRecordLib : public IUnknown
{
public:
	virtual HRESULT AddCodeRecord (
		IN  CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		IN  BOOL   bEnabled,  // 识别指令序列是否有效
		OUT 	PDWORD  pdwIndex = NULL  // 识别记录索引
		) PURE;
	virtual HRESULT IsCodeRecordExists (
		IN  	CONST  PCHAR   pccCodeSequence  // 识别指令序列
		) PURE;
	virtual HRESULT IsCodeRecordExists (
		IN  	DWORD   dwIndex  // 记录的位置索引值
		) PURE;
	virtual HRESULT GetCodeRecordInsertIndex (
		IN  	CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		OUT 	PDWORD  pdwIndex  // 识别记录索引
		) PURE;
	virtual HRESULT  AddCodeRecordItem(
		IN  CONST  PCHAR  pccCodeSequence,  // 识别指令序列
		IN  CONST  PCHAR  pccResult,  // 识别项的识别结果。
		IN  DWORD  dwIndex = -1  // 识别记录项索引
		) PURE;
	virtual HRESULT IsCodeRecordItemExists (
		IN  CONST  PCHAR  pccCodeSequence,  // 识别指令序列
		IN  CONST  PCHAR  pccResult  // 识别项的识别结果。
		) PURE;
	virtual HRESULT IsCodeRecordItemExists (
		IN  DWORD	dwIndex,  // 记录的位置索引值
		IN  CONST  PCHAR  pccResult  // 识别项的识别结果。
		) PURE;
	virtual HRESULT  AddMethod(
		IN  CONST  PCHAR  pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwItemIndex, 	 	// 识别记录项索引
		IN  CONST  PCHAR   pccMethod,		// 方法描述；
		IN  DWORD  dwIndex = -1		// 方法的索引位置
		) PURE;
	virtual HRESULT IsMethodExists (
		IN  CONST  PCHAR  pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwItemIndex, 	 	// 识别记录项索引
		IN  CONST  PCHAR   pccMethod		// 方法描述；
		) PURE;
	virtual HRESULT IsMethodExists (
		IN  DWORD	dwIndex,  // 记录的位置索引值
		IN  DWORD  dwItemIndex, 	 	// 识别记录项索引
		IN  CONST  PCHAR   pccMethod		// 方法描述；
		) PURE;
	virtual HRESULT EnableCodeRecord (
		IN  CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		IN  BOOL   bEnabled  // 识别指令序列是否有效
		) PURE;
	virtual HRESULT EnableCodeRecord (
		IN  DWORD  dwIndex,  // 记录索引
		IN  BOOL    bEnabled  // 识别指令序列是否有效
		) PURE;
	virtual HRESULT SetCodeRecord (
		IN  CONST  PCHAR  pccSourCodeSequence,  // 源识别指令序列
		IN  CONST  PCHAR  pccDestCodeSequence  // 目的识别指令序列
		) PURE;
	virtual HRESULT SetCodeRecord (
		IN  DWORD  dwIndex,  // 记录索引
		IN  CONST  PCHAR  pccDestCodeSequence  // 识别指令序列
		) PURE;
	virtual HRESULT  CodeRecordItemCount (
		IN  CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		OUT  PDWORD  pdwCodeRecordItemCount  // 识别项数量
		) PURE;
	virtual HRESULT  MethodCount (
		IN  CONST  PCHAR  pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwItemIndex, 	 	// 识别记录项索引
		OUT  PDWORD  pdwMethodCount  // 识别方法数量
		) PURE;
	virtual HRESULT  GetCodeRecord (
		IN  DWORD  dwIndex,  // 识别记录的索引值
		OUT  PCHAR  pcCodeSequence,  // 识别指令序列
		OUT  PBOOL  pbEnabled = NULL // 识别指令序列是否有效
		) PURE;
	virtual HRESULT  GetCodeRecord (
		IN  	CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		OUT 	PDWORD  pdwIndex,  // 识别记录索引
		OUT  PBOOL   pbEnabled = NULL // 识别指令序列是否有效
		) PURE;
	virtual HRESULT  GetCodeRecordItem (
		IN  CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwIndex,  // 识别记录项索引
		OUT  PCHAR  pcResult,   // 识别记录项的识别结果
		IN OUT  PDWORD  pdwBufSize	 // 识别结果字节数
		) PURE;
	virtual HRESULT  GetMethod (
		IN  CONST  PCHAR  pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwItemIndex, 	 	// 识别记录项索引
		IN   DWORD  dwIndex, 	// 识别方法的索引值
		OUT  PCHAR   pcMethod,  // 识别方法；
		IN OUT  PDWORD  pdwBufSize	 // 数据字节数；
		) PURE;
	virtual HRESULT  RemoveCodeRecordItem(
		IN  CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwIndex  // 识别项索引
		) PURE;
	virtual HRESULT  RemoveMethod(
		IN  CONST  PCHAR   pccCodeSequence,  // 识别指令序列
		IN  DWORD  dwItemIndex, 	 	// 识别记录项索引
		IN   DWORD  dwIndex 	// 识别方法的索引值
		) PURE;
	virtual HRESULT  CodeRecordCount (
		IN  PDWORD  pdwCodeRecordCount  // 识别记录数量
		) PURE;
	virtual HRESULT  RemoveCodeRecord (
		IN  CONST  PCHAR  pccCodeSequence  // 识别指令序列
		) PURE;
	virtual HRESULT  RemoveCodeRecord (
		IN  DWORD  dwIndex  // 识别记录的索引值
		) PURE;
	virtual HRESULT  ClearCodeRecord (void) PURE;
	virtual HRESULT  ToAscii (
		IN  CONST  PBYTE  pcbyCodeSequence,  // 识别指令序列
		OUT  PCHAR  pAsciiBuf  // 接收识别指令序列Ascii码形式
		) PURE;
	virtual HRESULT  ToBinary (
		IN  CONST  PCHAR  pccAsciiBuf,  // 识别指令序列Ascii码形式
		OUT  PBYTE  pbyCodeSequence  // 识别指令序列
		) PURE;
	virtual HRESULT  MakeLibrary (
		IN  CONST  PCHAR  pcFilePath,  // 文件路径
		IN  BOOL  bFailIfExists	// 是否需要在文件存在时使用操作失败；
		) PURE;
	virtual HRESULT  GetSequenceFromPE (
		IN  CONST  CHAR  *pcFilePath,  // PE文件路径
		OUT  PCHAR  pcCodeSequence  // 识别指令序列
		) PURE;
	virtual HRESULT  ImportDescription(
		IN  CONST  CHAR*  pcFilePath,  // 描述文件路径
		OUT  DWORD*  pdwLine,  // 错误行
		OUT  CHAR*   pcContent,  // 错误内容
		IN  DWORD  dwContentSize,  // 内容字节数
		OUT  CHAR*   pcErrorMsg,  // 错误信息
		IN  DWORD  dwErrorMsgSize  //错误信息节数
		) PURE;
public:
	RE_DECLARE_IID;
};
typedef ICodeRecordLib *PICodeRecordLib;

/////////////////////////////////////////////////////////////////////////////////////////

// {15873747-3E73-4081-997C-F1D8E60793C4}
RE_DEFINE_IID(ICodeRecordLib, "{15873747-3E73-4081-997C-F1D8E60793C4}",
			0x15873747, 0x3e73, 0x4081, 0x99, 0x7c, 0xf1, 0xd8, 0xe6, 0x7, 0x93, 0xc4);

// {B71C3948-2EF5-4348-80DF-9035758FDEF5}
DEFINE_GUID(CLSID_CodeRecordLib, 
			0xb71c3948, 0x2ef5, 0x4348, 0x80, 0xdf, 0x90, 0x35, 0x75, 0x8f, 0xde, 0xf5);

/////////////////////////////////////////////////////////////////////////////////////////

#endif //_I_COMPILER_LIB_H_


