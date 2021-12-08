#ifndef __LIB_UPDATE__
#define __LIB_UPDATE__

namespace mss
{
	enum UPEVENT							// >= 0, Step Report
											// < 0,  Error Report
	{
		STEP_PROCESS_FILE		=  0,

		ERR_UNKNOWN_ANY			= -1,		

		ERR_WANTFILE_OPEN		= -100,		// 升级文件打开失败，wFileName为WantFileName
		ERR_WANTFILE_PARSE		= -101,		// 升级文件解析失败，wFileName为WantFileName

		ERR_TMPFILE_CREATE		= -200,		// 创建临时文件失败，wFileName为临时文件路径
		ERR_TMPFILE_RENAME		= -201,		// 临时文件改名为本地库名时失败，wFileName为本地库文件名，

		ERR_UPDATE_APPLY		= -300,		// 真正应用升级时出错，wFileName为临时文件路径
	};

	class ILibFileUpCallback
	{
	public:
		STDMETHOD(PushWantFile)( LPCSTR wFileName ) PURE;
		STDMETHOD(PushWorkEvent)( UPEVENT idErr, LPCSTR wFileName ) PURE;
		STDMETHOD_(LPCSTR,GetObNamePrefix)() PURE;
	};


	class ILibFileUpCallbackW
	{
	public:
		STDMETHOD(PushWantFile)( LPCWSTR wFileName ) PURE;
		STDMETHOD(PushWorkEvent)( UPEVENT idErr, LPCWSTR wFileName ) PURE;
		STDMETHOD_(LPCSTR,GetObNamePrefix)() PURE;
	};

	class ILibFileUp : public IUnknown
	{
	public:
		
		//
		//	初始化，根据本地文件、配置文件计算升级所需的增量文件列表
		//	
		//	S_OK, 成功，需要升级 
		//	S_FALSE, 成功，不需要升级
		//
		
		STDMETHOD(Init)( LPCSTR wHome, LPCSTR wCfg, ILibFileUpCallback* pcb, LPCSTR aCustomFileName = 0 ) PURE;

		//
		//	进行文件升级，升级完成后，生成一个临时文件
		//
		//	S_OK,		成功，升级完成，产生了一个临时文件
		//	S_FALSE,	成功，不需要升级
		//	< 0,		通过ILibFileUpCallback返回了失败的情况
		//
		//	2013/8/13: 之前说此方法可以重复调用，现在开始不再支持；使用者修复问题后，需要释放此对象重新创建一个ILibFileUp进行升级
		//
		
		STDMETHOD(Update)( LPCSTR wWantFileHome, ILibFileUpCallback* pcb ) PURE;

		//
		//	文件升级完毕，并且没有发生错误，使用此方法更新内存中的全局版本号
		//

		STDMETHOD(NotifyUpdated)() PURE;

		//
		// 设置病毒库文件缓存路径
		//

		STDMETHOD(SetCachePath)( LPCSTR lpCachePath ) PURE;

		RE_DECLARE_IID;
	};

	// {89EF00DB-B948-4DA3-B586-1B42DA645E53}
	RE_DEFINE_IID( ILibFileUp, "{89EF00DB-B948-4DA3-B586-1B42DA645E53}",
		0x89ef00db, 0xb948, 0x4da3, 0xb5, 0x86, 0x1b, 0x42, 0xda, 0x64, 0x5e, 0x53);



	class ILibFileUpW : public IUnknown
	{
	public:
		//
		//	初始化，根据本地文件、配置文件计算升级所需的增量文件列表
		//	
		//	S_OK, 成功，需要升级
		//	S_FALSE, 成功，不需要升级
		//

		STDMETHOD(Init)( LPCWSTR wHome, LPCWSTR wCfg, ILibFileUpCallbackW* pcb, LPCWSTR aCustomFileName = 0 ) PURE;

		//
		//	进行文件升级，升级完成后，生成一个临时文件
		//
		//	S_OK,		成功，升级完成，产生了一个临时文件
		//	S_FALSE,	成功，不需要升级
		//	< 0,		通过ILibFileUpCallback返回了失败的情况
		//
		//	2013/8/13: 之前说此方法可以重复调用，现在开始不再支持；使用者修复问题后，需要释放此对象重新创建一个ILibFileUp进行升级
		//
		STDMETHOD(Update)( LPCWSTR wWantFileHome, ILibFileUpCallbackW* pcb ) PURE;


		//
		//	文件升级完毕，并且没有发生错误，使用此方法更新内存中的全局版本号
		//

		STDMETHOD(NotifyUpdated)() PURE;
		RE_DECLARE_IID;
	};
	// {6756125B-3294-4F21-B195-1AD88A147DAD}
	RE_DEFINE_IID(ILibFileUpW, "{6756125B-3294-4F21-B195-1AD88A147DAD}",
		0x6756125b, 0x3294, 0x4f21, 0xb1, 0x95, 0x1a, 0xd8, 0x8a, 0x14, 0x7d, 0xad);


	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//
	//		获得升级目录必须要的文件列表的
	//
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////

	typedef enum HOME_UP_WANT_FILE_STATUS
	{
		FILE_STATUS_GOOD = 0,
		//////////////////////////////
		FILE_CAN_NOT_OPEN,		// 文件打不开
		FILE_DAMGED,			// 文件验证不过
		FILE_MUST_REGET,		// 文件必须下
	}  HUWFS;

	struct IHomeFileEnumCallback
	{
		// S_OK, CONTINUE,
		// FAILED, BREAK 
		STDMETHOD(PushFile)( LPCSTR aWanntFile, HUWFS status ) PURE;
	};

	class ILibHomeList : public IUnknown
	{
	public:
		STDMETHOD(EnumFiles)( LPCSTR wHome, LPCSTR wCfg, IHomeFileEnumCallback* pcb ) PURE;
		RE_DECLARE_IID;
	};

	// {FE06A18C-59C6-4F2D-866C-6DD36639F888}
	RE_DEFINE_IID(ILibHomeList, "{FE06A18C-59C6-4F2D-866C-6DD36639F888}",
		0xfe06a18c, 0x59c6, 0x4f2d, 0x86, 0x6c, 0x6d, 0xd3, 0x66, 0x39, 0xf8, 0x88);

};


#endif