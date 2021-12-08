#ifndef __RE_SCANEX_INTERFACE_H_2010__
#define __RE_SCANEX_INTERFACE_H_2010__

/////////////////////////////////////////////////////////////////////////////////////////////////
// extend scan method:
// enum LOCATION_POINTER_TYPE
// {
// 	POINTER_EOF = 0 ,  // -> 截至符
// 	POINTER_FILE ,	 // -> 文件偏移
// 	POINTER_MEM ,	  // -> 内存线性地址
// 	POINTER_VALUE ,	// -> 数值
// 
// 	POINTER_INVALID = 0xFFFFFFFF,  // 无效
// };
// 
// typedef struct tagPELocation
// {
// 	union
// 	{
// 		DWORD linear ; // 内存线性地址
// 		DWORD offset ; // 文件偏移
// 		DWORD data ;   // 数值数据
// 	};
// 	DWORD type ; // LOCATION_POINTER_TYPE
// } PELocation, *PPELocation ;
// 
// 
// // 标准方法之装备箱
// class IPEEngStash : public IUnknown
// {
// public:
// 	// pe文件对象
// 	STDMETHOD(GetPEArc)(IREPEArchive **pppearc) = 0;
// 	// 虚拟机对象
// 	STDMETHOD(GetVMObj)(IREVM **ppvmobj) = 0;
// 	// 标准方法的扩展数据指针和大小
// 	STDMETHOD_(const BYTE*,GetStdExtLibData)(OUT WORD &wSize) = 0 ;  // return NULL if failed
// 	// 回朔引用方法结果
// 	STDMETHOD(GetLocationData)( int index, PELocation &loc ) = 0;
// public:
// 	RE_DECLARE_IID;
// };
// RE_DEFINE_IID(IPEEngStash, "{36EA966F-E05A-419a-A429-54B8D31455DB}",
// 			  0x36ea966f, 0xe05a, 0x419a, 0xa4, 0x29, 0x54, 0xb8, 0xd3, 0x14, 0x55, 0xdb);
// 
// 
// typedef IUnknown IREPEMethodParam;
// 
// // 标准方法
// class IREPEStdMethod : public IUnknown
// {
// public:
// 	STDMETHOD(Invoke)(IUnknown* punk, IREPEMethodParam* /*pstk*/, OUT PELocation *presult) = 0;
// public:
// 	RE_DECLARE_IID;
// };
// RE_DEFINE_IID(IREPEStdMethod, "{9A9B2485-119B-4dca-A3A1-997C09646D25}",
// 			  0x9a9b2485, 0x119b, 0x4dca, 0xa3, 0xa1, 0x99, 0x7c, 0x9, 0x64, 0x6d, 0x25);

// 扩展查毒方法对象
// {E3BEFA13-39DA-44d2-92A4-21B80B211BFA}
DEFINE_GUID(CLSID_PESTDEX_ScanEx, 
0xe3befa13, 0x39da, 0x44d2, 0x92, 0xa4, 0x21, 0xb8, 0xb, 0x21, 0x1b, 0xfa);
#define CLSID_CREPEStdMethodScanEx	CLSID_PESTDEX_ScanEx
/////////////////////////////////////////////////////////////////////////////////////////////////


class IREEngineScanEx : public IUnknown
{
public:
	STDMETHOD_(DWORD, Scan)(IUnknown *punkArchive, void *pvparam) = 0;
	STDMETHOD_(DWORD, Kill)(IUnknown *punkArchive, void *pvparam) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREEngineScanEx, "{132CE9E9-242B-4e76-AA0C-0FEE19CCA959}",
0x132ce9e9, 0x242b, 0x4e76, 0xaa, 0xc, 0xf, 0xee, 0x19, 0xcc, 0xa9, 0x59);


// 库前扩展查/杀毒引擎
// {C96AA3EC-447A-4ecb-9719-9D916A598EAF}
DEFINE_GUID(CLSID_CREEngineScanExPre, 
0xc96aa3ec, 0x447a, 0x4ecb, 0x97, 0x19, 0x9d, 0x91, 0x6a, 0x59, 0x8e, 0xaf);

// 库后扩展查/杀毒引擎
// {3B216068-5205-4a40-804B-C80330716D4A}
DEFINE_GUID(CLSID_CREEngineScanExPost, 
0x3b216068, 0x5205, 0x4a40, 0x80, 0x4b, 0xc8, 0x3, 0x30, 0x71, 0x6d, 0x4a);

// 扩展杀毒引擎
// {E7F0D28C-2451-4ea1-93C5-6EFFB5F36CC5}
DEFINE_GUID(CLSID_CREEngineKillEx, 
0xe7f0d28c, 0x2451, 0x4ea1, 0x93, 0xc5, 0x6e, 0xff, 0xb5, 0xf3, 0x6c, 0xc5);

// 扩展查杀未知家族病毒引擎
// {6838FC2A-60D2-4c89-AF35-5A1B6DD28F38}
DEFINE_GUID(CLSID_CREEngineUnkVirusEx, 
0x6838fc2a, 0x60d2, 0x4c89, 0xaf, 0x35, 0x5a, 0x1b, 0x6d, 0xd2, 0x8f, 0x38);


//////////////////////////////////////////////////////////////////////////


// 扩展处理PE资源病毒绑定器复合文档
// {4C7EF6CF-53F5-4a05-ACDA-48E4C487AB24}
DEFINE_GUID(CLSID_CREArchivePEResDropperEx, 
0x4c7ef6cf, 0x53f5, 0x4a05, 0xac, 0xda, 0x48, 0xe4, 0xc4, 0x87, 0xab, 0x24);


//////////////////////////////////////////////////////////////////////////


// PE文件后置查毒引擎（Interface IREEngine）：
// {20BC865A-BEAA-4249-8751-ED9E1B1FD7F7}
DEFINE_GUID(CLSID_CREPEPostEngine, 
0x20bc865a, 0xbeaa, 0x4249, 0x87, 0x51, 0xed, 0x9e, 0x1b, 0x1f, 0xd7, 0xf7);


//////////////////////////////////////////////////////////////////////////

// PE 可疑文件扫描引擎 ( Interface IREEngine）：
// 2009/3/19
// {F367899A-701A-47f7-A510-AD143CE9009D}
DEFINE_GUID( CLSID_CREScanSuspEx,
0xf367899a, 0x701a, 0x47f7, 0xa5, 0x10, 0xad, 0x14, 0x3c, 0xe9, 0x0, 0x9d);

// 扩展处理PE Overlay复合文档
// {08C11513-B379-4098-959F-1CEE6AE8C82D}
DEFINE_GUID( CLSID_CREPEOverlayCompoundArchive, 
0x8c11513, 0xb379, 0x4098, 0x95, 0x9f, 0x1c, 0xee, 0x6a, 0xe8, 0xc8, 0x2d);


//////////////////////////////////////////////////////////////////////////


// PE Overlay查毒引擎（Interface IREEngine, IREEngineJudgeCompound）：
// {1C481601-9B30-425c-80CE-65882D84B525}
DEFINE_GUID( CLSID_COverlayJudgeEngine, 
0x1c481601, 0x9b30, 0x425c, 0x80, 0xce, 0x65, 0x88, 0x2d, 0x84, 0xb5, 0x25);


//////////////////////////////////////////////////////////////////////////


/*********
* Only defined for ScanEx:
*********/
#ifndef	_E2234C58_D03C_43F3_92A2_D45522D22F21_

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

/*********
* 用于设置异常级别:
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
	DWORD dwValidFieldMask;	 //有效域标志（enum TRACE_RESULT）
	BOOL  bEnableTraceDJ;	   //是否跟踪解密、跳转
	BOOL  bAPIAsException;	  //中断指令或API调用是否算异常
	BOOL  bNullLoopAsException; //空循环是否算异常
	DWORD dwExceptionLimit;	 //允许最大异常数
}STEXCEPTIONLEVEL;

// Where decode reference?
#define EXEC_DEC_AT_IMAGE	(0)
#define EXEC_DEC_AT_STACK	(1<<6)

#endif //#ifndef _E2234C58_D03C_43F3_92A2_D45522D22F21_


/*********
* Environment parameters:
*********/
class IExecExport;
typedef struct tagSTEXECEXP_ENVPARAM
{
	BOOL	bBadFile;
	DWORD	idVirus;
	DWORD	dwParam_1;
	DWORD	dwParam_2;
	IExecExport *pcExecExport;
}STEXECEXP_ENVPARAM;


typedef BOOL (*PFNSTEPCALLBACK)(LPCVOID pUserParam);


class IExecExport
{
public:
	enum ERROR_CODE
	{
		EC_SUCCESS = 0,
		EC_VE_NEW_DECODE, //在匹配到此次跳转前，又产生了新的解密
		EC_VE_NORECORD,   //没有这个跳转记录(未发生、或记录太老已被丢弃)
		EC_VE_RUN_API,	//上一条指令是中断指令或一个API调用
		EC_MALLOC,		//内存分配失败
		EC_INIT,		  //未作初始化或初始化失败
		EC_VE_OVERFLOW,   //IP地址记录表溢出
		EC_VE_NULLLOOP,   //达到最大空循环计数
		EC_VM,			//虚拟机异常(程序已退出或其他异常)
		EC_FILE,		  //基本文件操作错误
		EC_NOMATCHING,	//不匹配
		EC_ERROR		  //其他未知错误
	};
	enum FILE_SEEK_POSITION 
	{ 
		SPBegin = 0x0,   //从文件头
		SPCurrent = 0x1, //从当前位置
		SPEnd = 0x2	  //从文件尾
	};

	//取得最后一次的错误标识
	virtual DWORD GetLastError(void) = 0;
	//取得某个查毒方法的结果地址：
	virtual BOOL GetSMResultAddr(DWORD idSM, OUT DWORD *pdwAddr) = 0;

	/*********
	* 虚拟执行:
	*********/
	//复位虚拟机，并清空所有跟踪记录信息
	virtual void ResetVM(void) = 0;
	//设置允许内存页保护：
	virtual bool EnableMemoryWriteProtect(bool bEnable) = 0;
	//设置最大虚拟执行步数
	virtual void SetMaxVESteps(DWORD dwMaxSteps) = 0;
	//设置异常级别
	virtual void SetExceptionLevel(STEXCEPTIONLEVEL *pstEL) = 0;
	//清除异常标志
	virtual void ClearExceptionMark(void) = 0;
	//获得当前异常计数
	virtual DWORD GetExceptionCount(void) = 0;
	//获得第一个异常地址，0 = 失败！
	virtual DWORD GetFirstExceptionLinear(void) = 0;
	//获得下一个异常地址，0 = 失败！
	virtual DWORD GetNextExceptionLinear(void) = 0;
	//获得映象加载基址
	virtual DWORD GetImageBase(void) = 0;
	//单步执行
	virtual BOOL Step(DWORD dwSteps = 1) = 0;
	virtual BOOL ConditionStep(PFNSTEPCALLBACK pfnCallBack, void* pUserParam) = 0;
	//运行到指定的解密跳转次数处
	virtual BOOL GoDecodeJumpTimes(DWORD dwDecodeTimes, DWORD dwJumpTimes) = 0;
	//取得当前解密跳转次数
	virtual DWORD GetDecodeJumpTimes(OUT DWORD *pdwJumpTimes = NULL) = 0;
	//取得指定解密跳转次数的记录
	virtual BOOL GetDecodeJumpRecord(DWORD dwDecodeTimes, DWORD dwJumpTimes,
									 OUT DWORD *pdwLinearBeg, OUT DWORD *pdwLinearEnd,
									 DWORD ufWhere = EXEC_DEC_AT_IMAGE) = 0;
	//虚拟内存读写
	virtual BOOL VirtualMemRead(DWORD dwLinear, OUT void *pvBuffer, DWORD dwLength) = 0;
	virtual BOOL VirtualMemReadDeReloc(DWORD dwLinear, OUT void *pvBuffer, DWORD dwLength) = 0;
	virtual BOOL VirtualMemWrite(DWORD dwLinear, void *pvData, DWORD dwLength) = 0;
	//存取通用寄存器： I_EAX, I_EBX, I_ECX, I_EDX, I_ESP, I_EBP, I_EDI, I_ESI
	virtual DWORD GetGeneralReg(DWORD dwRegIndex) = 0;
	virtual void  SetGeneralReg(DWORD dwRegIndex, DWORD dwValue) = 0;
	//存取指令指针寄存器
	virtual DWORD GetEIPReg(void) = 0;
	virtual void  SetEIPReg(DWORD dwValue) = 0;
	//存取标志寄存器
	virtual DWORD GetEFlagReg(void) = 0;
	virtual void  SetEFlagReg(DWORD dwValue) = 0;
	//存取段寄存器
	virtual WORD GetSegmentReg(DWORD dwSegIndex) = 0;
	virtual void SetSegmentReg(DWORD dwSegIndex, WORD wValue) = 0;

	/*********
	* 基本文件操作:
	*********/
	//读文件，返回读写字节数
	virtual DWORD Read(void *pvBuffer, DWORD dwLen) = 0;
	//指定位置读，返回读写字节数
	virtual DWORD ReadPos(void *pvBuffer, DWORD dwPos, DWORD dwLen) = 0;
	//写文件，返回读写字节数
	virtual DWORD Write(void *pvBuffer, DWORD dwLen) = 0;
	//指定位置写，返回读写字节数
	virtual DWORD WritePos(void *pvBuffer, DWORD dwPos, DWORD dwLen) = 0;
	//移动文件指针
	virtual HRESULT Seek(DWORD dwPos, WORD wFrom = SPBegin) = 0;
	//取当前文件指针，0xFFFFFFFF：出错
	virtual DWORD Tell(void) = 0;
	//取文件大小，0xFFFFFFFF：出错
	virtual DWORD GetLength(void) = 0;
	//置文件大小
	virtual BOOL SetLength(DWORD dwNewLen) = 0;
	//删除文件
	virtual BOOL Remove(void) = 0;
	//拷贝数据块(覆盖)
	virtual BOOL CopyDataBlock(DWORD dwFileOffsetDest,DWORD dwLength,
							   DWORD dwAddrSrc, BOOL bSrcIsMem = TRUE) = 0;
	//移动数据块(插入)
	virtual BOOL MoveDataBlock(DWORD dwFileOffsetDest,DWORD dwLength,
							   DWORD dwAddrSrc, BOOL bSrcIsMem = TRUE) = 0;
	//插入数据块(插入)
	virtual BOOL InsertDataBlock(DWORD dwFileOffsetDest, void *pvData,
								 DWORD dwLength) = 0;
	//修改数据块(覆盖)
	virtual BOOL ModifyDataBlock(DWORD dwFileOffsetDest, void *pvData, DWORD dwLength) = 0;
	//填充数据块(覆盖)
	virtual BOOL FillDataBlock(DWORD dwFileOffsetDest, BYTE byValue, DWORD dwLength) = 0;
	//删除数据块
	virtual BOOL DeleteDataBlock(DWORD dwFileOffsetDest, DWORD dwLength) = 0;
	//将虚拟内存中的文件映象提交到磁盘文件
	virtual BOOL VirtualMemCommitFile(void) = 0;
	//浮动搜索文件、虚拟内存中的二进制匹配串，不管文件的字节序
	virtual BOOL FloatingSearch(DWORD dwAddrBeg, DWORD dwRange, BYTE *pbyFind,
								DWORD dwFindLen, OUT DWORD *pdwAddrMatch,
								BOOL bSearchInMem = TRUE) = 0;

	/*********
	* 寻址转换:
	*********/
	//将内存中的线性地址转换为文件中相对于文件头的文件偏移
	virtual BOOL ConvertLinearToFileOffset(DWORD dwLinear,  OUT DWORD *pdwFileOffset) = 0;
	//将文件中相对于文件头的文件偏移转换为内存中的线性地址
	virtual BOOL ConvertFileOffsetToLinear(OUT DWORD *pdwLinear, DWORD dwFileOffset) = 0;

	/*********
	* Misc:
	*********/
	virtual BOOL CalculateCheckSum(void) = 0;
};


#endif // #ifndef __RE_SCANEX_INTERFACE_H__
