#ifndef __INTERFACE_CLOUD_COM__
#define __INTERFACE_CLOUD_COM__

namespace Cloud
{
	enum { MD5_BYTES = 16 };

	enum TrojCrcState
	{
		/*
			0—不是木马库，1—是木马库，2—白名单,3-未知,4-未知上报, 9-灰
		*/
		TCSNotTrojan,
		TCSIsTrojan,
		TCSIsWhite,
		TCSUnknown,
		TCSUnknownUpload,
		TCSGray = 9,

		_TCS_MAX_,
		//------------------//
		TCSVerified = -1,			// 表明这个crc对应的文件是否已经被验证过了
									// 有这个值，说明就不用再验证了
									// 除非文件的木马校验和变了
	};

	typedef INT16		tcs16_t;
	typedef UINT64		tjsign_t;

	enum 
	{ 
		FILE_CACHE_CAPACIT = 50*10000,
		FILE_CACHE_CAPACIT_V2 = 30*10000,
		FILE_CACHE_CAP_MD5 = 5000,

		NFILE_CACHE_CAPACIT = 100*10000,
		NFILE_CACHE_CAPACIT_V2 = 75*10000,

		LIB_CACHE_CAPACIT = 20*10000,
		LIB_CACHE_CAPACIT_V2_NB = 15*10000,
		LIB_CACHE_CAPACIT_V2_B = 25000,

		LIB_MD5_CACHE_CAP_B = 5000,
		LIB_MD5_CACHE_CAP_NB = 5000,

		QUEUE_CAPACITY = 8000,

		MD5_QUEUE_CAPACITY = 1000,
	};

	struct QueueItem
	{
		tjsign_t trojCRC;
		void *	userdata;
	};

	struct QueueItemEx : QueueItem
	{
		void *	poster;
	};

	struct ICPEventHandler
	{
		virtual void OnQueryCompletion( tjsign_t * crc, tcs16_t * state, void * userdata ) = 0;
		virtual void OnQueueEmpty() = 0;
		virtual void OnWorkerDead() = 0;
	};

	struct ICPEventHandlerEx
	{
		virtual void OnQueryCompletionEx( QueueItemEx * qiex, tcs16_t * state ) = 0;
	};

	enum { VIRNAME_LENGTH = 64 };

#pragma pack(2)
	struct CLOUD_RET_V3
	{
		tcs16_t		State;
		UINT32		VirusId;
		CHAR		VirusName[VIRNAME_LENGTH];
	};
#pragma pack()

	class ICloudEventHandler : public IUnknown
	{
	public:
		// 通过CRC查询返回
		STDMETHOD( OnCloudReturn )( tjsign_t * aTrojCRC, CLOUD_RET_V3 * aInfo, LPVOID aUserData ) = 0;

		// 通过MD5查询返回
		STDMETHOD( OnCloudReturnMD5 )( UINT8 aMD5[MD5_BYTES], CLOUD_RET_V3 * aInfo, LPVOID aUserData ) = 0;

		// 以下两函数优先于上面两个回调 被调用

		// 当CRC返回为Gray时，直接请求对应文件的MD5
		STDMETHOD( OnRequireFileMD5 )( tjsign_t * aTrojCRC, LPVOID aUserData, UINT8 aMD5[MD5_BYTES] ) = 0;

		RE_DECLARE_IID;
	};
	// {38FA119A-78E4-4702-B612-79120C8E3712}
	RE_DEFINE_IID( ICloudEventHandler, "{38FA119A-78E4-4702-B612-79120C8E3712}",
		0x38fa119a, 0x78e4, 0x4702, 0xb6, 0x12, 0x79, 0x12, 0xc, 0x8e, 0x37, 0x12);

	enum  eTFCEvent
	{
		E_TFC_WORK,
		E_TFC_PAUSE,
	};
	class ITFCController : public IUnknown
	{
	public:

		// 给TFC发送控制事件
		STDMETHOD( FireEvent )(eTFCEvent, void *) = 0;
		RE_DECLARE_IID;
	};


	// {8C609669-C0E8-4cf6-89BA-48417BFBBA8F}
	RE_DEFINE_IID(ITFCController, "{8C609669-C0E8-4cf6-89BA-48417BFBBA8F}",
		0x8c609669, 0xc0e8, 0x4cf6, 0x89, 0xba, 0x48, 0x41, 0x7b, 0xfb, 0xba, 0x8f);


	class ICloudProvider : public IUnknown
	{
	public:
		//////////////////////////////////////////////////////////////////////////

		// 后台工作线程

		virtual HRESULT WorkerStart( ULONG crcPerItem = 100, ULONG queryInterval = -1 ) = 0;
		virtual HRESULT WorkerOption( ULONG crcPerItem = 100, ULONG queryInterval = -1 ) = 0;
		virtual HRESULT WorkerExit() = 0;

		//////////////////////////////////////////////////////////////////////////

		// 监视事件
		//
		//	必须在ICPEventHandler销毁前调用EventUnWatch注销事件监听者
		//

		// 5.16 逐渐废弃,建议都使用ICloudProvider4中的事件处理注册机制
		virtual HRESULT EventWatch( ICPEventHandler * eHandler ) = 0;
		// 5.16 逐渐废弃,建议都使用ICloudProvider4中的事件处理注册机制
		virtual HRESULT EventUnWatch( ICPEventHandler * eHandler ) = 0;

		//////////////////////////////////////////////////////////////////////////

		// 队列操作

		// 废弃
		virtual INT32 QueueQueued( IN const QueueItem * items, IN INT32 count ) = 0;
		// 废弃
		virtual INT32 QueueDequeued( OUT QueueItem * items, IN OUT INT32 count ) = 0;
		// 废弃
		virtual HRESULT QueueBreakWait() = 0;
		virtual size_t  QueueGetCount() = 0;

		//////////////////////////////////////////////////////////////////////////

		// 本地缓存库操作
		// 通过crc可以取到对应的状态

		virtual HRESULT LibCacheGet( IN tjsign_t * crc, OUT tcs16_t * state ) = 0;
		virtual HRESULT LibCacheSet( IN tjsign_t * crc, IN tcs16_t * state ) = 0;

		//////////////////////////////////////////////////////////////////////////

		// 本地文件校验和缓存操作
		// 通过文件名可以取到对应的木马校验和
		// 取不到请重新算

		virtual HRESULT FileCacheGet( const wchar_t * fullPath, tjsign_t * crc ) = 0;
		virtual HRESULT FileCacheSet( const wchar_t * fullPath, tjsign_t * crc ) = 0;
		//////////////////////////////////////////////////////////////////////////

		// 修正库更新
		virtual HRESULT FixLibUpdate() = 0;
		virtual HRESULT FixLibFlushCache( const wchar_t * fullPath ) = 0;

		//////////////////////////////////////////////////////////////////////////

		// 无论如何都发起一次云查询,并等待其结束
		virtual HRESULT QueryImmAndWaitCompleted( DWORD dwTimeout ) = 0;

		RE_DECLARE_IID;
	};

	// {7DDE9951-61B0-4c93-AF65-29C7AF405AFD}
	RE_DEFINE_IID( ICloudProvider, "{7DDE9951-61B0-4c93-AF65-29C7AF405AFD}",
		0x7dde9951, 0x61b0, 0x4c93, 0xaf, 0x65, 0x29, 0xc7, 0xaf, 0x40, 0x5a, 0xfd);

	class ICloudProvider2 : public IUnknown
	{
	public:
		// 这两个接口可以读取和设置TCSCloudResultMax以上的值
		virtual HRESULT LibCacheGetEx( IN tjsign_t * crc, OUT tcs16_t * state ) = 0;
		virtual HRESULT LibCacheSetEx( IN tjsign_t * crc, IN tcs16_t * state ) = 0;
		RE_DECLARE_IID;
	};

	// {6F7EECAE-CD3D-47bd-9EBE-E0BE25D32AB3}
	RE_DEFINE_IID( ICloudProvider2, "{6F7EECAE-CD3D-47bd-9EBE-E0BE25D32AB3}",
		0x6f7eecae, 0xcd3d, 0x47bd, 0x9e, 0xbe, 0xe0, 0xbe, 0x25, 0xd3, 0x2a, 0xb3);

	class ICloudProvider3 : public IUnknown
	{
	public:
		// 5.16 逐渐废弃,建议都使用ICloudProvider4中的事件处理注册机制
		virtual HRESULT EventWatchEx( ICPEventHandlerEx * eHandler ) = 0;
		virtual HRESULT EventUnWatchEx( ICPEventHandlerEx * eHandler ) = 0;
		// 5.16 逐渐废弃,建议都使用ICloudProvider4中的安全排队
		virtual HRESULT QueueQueuedEx( IN const QueueItemEx * items ) = 0;
		RE_DECLARE_IID;
	};

	// {F5847151-DCDB-4241-9328-763D9EE1A10B}
	RE_DEFINE_IID( ICloudProvider3, "{F5847151-DCDB-4241-9328-763D9EE1A10B}",
		0xf5847151, 0xdcdb, 0x4241, 0x93, 0x28, 0x76, 0x3d, 0x9e, 0xe1, 0xa1, 0xb);

#pragma pack(2)

	struct QU_ITEM
	{
		UINT32		SizeOfStruct;
		HANDLE		Handler;
		LPVOID		UserData;
	};

	struct QIT_TROJCRC : QU_ITEM
	{
		tjsign_t	TrojCRC;
	};

	struct QIT_FILEMD5 : QU_ITEM
	{
		UINT8		FileMD5[MD5_BYTES];
	};

#pragma pack()

	enum QUBY_CLASS
	{
		QUBY_TRJCRC,
		QUBY_MD5,
	};

	enum CACHE_CLASS
	{
		CLOUD_CACHE_B,
		CLOUD_CACHE_NB,
		CLOUD_CACHE_MD5_B,
		CLOUD_CACHE_MD5_NB,
		FILE_CACHE_PE,
		FILE_CACHE_NPE,
		FILE_CACHE_MD5,
		CACHE_CLASS_MAX,
		/*
		CLOUD_CACHE_B      云端缓存,CRC64（黑记录）
		CLOUD_CACHE_NB     云端缓存,CRC64（白/灰记录）
		CLOUD_CACHE_MD5_B  云端缓存,MD5(黑记录）
		CLOUD_CACHE_MD5_NB 云端缓存,MD5(白记录）
		FILE_CACHE_PE      PE文件对CRC64缓存（防止重复计算）
		FILE_CACHE_NPE     非PE文件缓存（扫描时直接跳过）
		FILE_CACHE_MD5     PE文件对MD5缓存（防止重复计算）
		*/
	};

	class ICloudProvider4 : public IUnknown 
	{
	public:

		//
		//	设置云端库的本地缓存的有效时间，以天为单位，最大为0xFFFF
		//

		STDMETHOD( SetLibCacheLife )( UINT32 u16Day ) = 0;

		//
		// lib cache
		//

		STDMETHOD( LibCacheGetV3 )( IN tjsign_t * crc, OUT CLOUD_RET_V3* lpInfo, BOOL bAllState = FALSE ) = 0;
		STDMETHOD( LibCacheSetV3 )( IN tjsign_t * crc, IN CLOUD_RET_V3* lpInfo, BOOL bAllState = FALSE ) = 0;
		STDMETHOD( LibCacheGetMD5 )( IN const UINT8 * md5, OUT CLOUD_RET_V3* lpInfo, BOOL bAllState = FALSE ) = 0;
		STDMETHOD( LibCacheSetMD5 )( IN const UINT8 * md5, IN CLOUD_RET_V3* lpInfo, BOOL bAllState = FALSE ) = 0;

		// md5 cache
		STDMETHOD( FileCacheMD5Get )( const wchar_t * fullPath, UINT8* FileMD5 ) = 0;
		STDMETHOD( FileCacheMD5Set )( const wchar_t * fullPath, UINT8* FileMD5 ) = 0;

		// calc File MD5
		STDMETHOD( HelperGetFileMD5 )( const wchar_t * fullPath, UINT8 * lpMD5, BOOL bIgnoreCache = FALSE ) = 0;

		//
		//	callback
		//

		STDMETHOD( EventHandlerRegister )( LPHANDLE lpHandle, ICloudEventHandler * peh ) = 0;
		STDMETHOD( EventHandlerRevoke )( HANDLE lpHandle ) = 0;

		//
		//	排队云扫描（CRC/MD5)
		//		如果不关心该值的返回结果，item.poster设置为
		//

		STDMETHOD( QueueQueuedExWithHandle )( QUBY_CLASS aClass, QU_ITEM * lpItem ) = 0;

		//
		//	显式地提交恶意文件的文件信息，木马校验和必须给
		// 	没有特殊需求，不需要调用此函数，上报流程在云查询流程里自动完成
		//

		STDMETHOD( UploadMalwareFileInfo )( IN LPCWSTR 	lpcFilePath, 
											IN UINT32	uiVirusId,
											IN LPCSTR	lpcVirusName,
											IN UINT64 * u64TrojCRC, 
											IN UINT8 * 	plbMD5 ) = 0;

		STDMETHOD( QueryImmAndWaitCompletedByClass )( IN QUBY_CLASS quClass, DWORD dwTimeout ) = 0;

		STDMETHOD( GetCacheCount )( CACHE_CLASS cls, PULONG pulCount ) = 0;

		RE_DECLARE_IID;
	};
	
	// {8244F30D-881D-466d-BCF3-07858C6C656A}
	RE_DEFINE_IID( ICloudProvider4, "{8244F30D-881D-466d-BCF3-07858C6C656A}",
		0x8244f30d, 0x881d, 0x466d, 0xbc, 0xf3, 0x7, 0x85, 0x8c, 0x6c, 0x65, 0x6a);

	//////////////////////////////////////////////////////////////////////////
	class ICloudProvider5 : public IUnknown 
	{
	public:

		RE_DECLARE_IID;

		typedef HRESULT ( __stdcall * PFN_Calc_MD5 )( tjsign_t * aTrojCRC, LPVOID aUserData, UINT8 aMD5[MD5_BYTES] );

		STDMETHOD( QuerySyncByClass )( 
			CLOUD_RET_V3 * pRetInfo, 
			QUBY_CLASS aClass, 
			LPVOID pvCheckSum /*CRC or MD5*/, 
			PFN_Calc_MD5 pfnMd5Callback, 
			LPVOID aUserData,
			DWORD aTimeout = 0 ) = 0;
	};

	// {6468DF51-3906-40a1-BE9F-58BA0E580404}
	RE_DEFINE_IID(ICloudProvider5, "{6468DF51-3906-40a1-BE9F-58BA0E580404}",
		0x6468df51, 0x3906, 0x40a1, 0xbe, 0x9f, 0x58, 0xba, 0xe, 0x58, 0x4, 0x4);

	//
	// 2013 Engine
	// 

	////////////////////
	//
	// Submit Types
	//

	static LPCSTR	SUBMIT_SUSP_FILE	=	"susp";

	//

	class ICloudProvider2013 : public IUnknown 
	{
	public:
		//
		//	信息上报，不去重，少量数据
		//

		STDMETHOD(SubmitInfo)( LPCSTR aInfoMsg ) PURE;

		//
		//	文件上报，去重，支持大量数据
		//

		typedef struct _FILE_TO_SUBMIT
		{
			LPCSTR		FileMD5A;		// 用于去重的MD5，一般为当前文件的MD5,字符串
			UINT64		FileSize64;		// 要上传的文件大小
			BOOL		VirtualFile;	// 虚拟文件，为真时，脱离当前环境后将无法获得文件内容;
			// 文件内容可以通过FileObject获得;
			// 非虚拟可以通过FileNameA|FileNameW找到文件进行延迟处理
			LPCSTR		FileNameA;		// 文件对象全路径名(MBS),可能没有
		} F2SINFO, *LPF2SINFO;

		//
		//	lpContent == NULL，则表明是个【文件去重查询】
		//		S_OK	:	云端还没有
		//		S_FALSE	:	云端已经存在
		//		< 0		:	调用失败
		//	
		//	lpContent != NULL，则表明是个【文件上报请求】
		//		S_OK	:	云端还没有,上报成功
		//		S_FALSE	:	云端已经存在,没有上报
		//		< 0		:	上报成功
		//
		//	lpContent 一般为 IREStream
		//	

		STDMETHOD(SubmitFile)( LPCSTR aSubmitType, LPF2SINFO lpInfo, IUnknown* lpContent = NULL ) PURE;

		//
		//	获得文件上报的一些参数
		//		MaxFileSize32	32位最大的文件大小
		//

		typedef struct FILE_SUBMIT_OPTIONS
		{
			DWORD	cbStruct;			// == sizeof(FILE_SUBMIT_OPTIONS)
			DWORD	MaxFileSize32;
		} UPOPTS, *LPUPOPTS;

		STDMETHOD(GetSubmitFileOptions)( LPUPOPTS lpUpOpts ) PURE;

		STDMETHOD(TestCloudConnectivity)() PURE;

		STDMETHOD(StandaloneInit)( IUnknown * rot ) PURE;

		RE_DECLARE_IID;
	};

	// {DA06D590-CDDE-4BB3-BA2D-EB91686C9B6F}
	RE_DEFINE_IID( ICloudProvider2013, "{DA06D590-CDDE-4BB3-BA2D-EB91686C9B6F}",
		0xda06d590, 0xcdde, 0x4bb3, 0xba, 0x2d, 0xeb, 0x91, 0x68, 0x6c, 0x9b, 0x6f);

};

//////////////////////////////////////////////////////////////////////////

/*
	Key -> | MemCache <-> DB |
	
		key -> object{ 

*/

HRESULT WINAPI FileNameToUniqID( LPCWSTR wFileName, PUINT64 lpUniqId64 );
HRESULT WINAPI GetMarkOfFile( LPCWSTR wFileName );

#ifndef __I_RS_ODB__
#define __I_RS_ODB__

typedef struct DB_OPTION
{
	UINT	Capacity;
	UINT	CacheSize;
	UINT	Compress;
	UINT	FlushTime;
} DB_OPTION, *LPDB_OPTION;

class IODBCreate : public IUnknown
{
public:
	STDMETHOD(Create)( LPCSTR aFileName, SIZE_T cbInMem, LPDB_OPTION lpOption ) PURE;
	STDMETHOD(Open)( LPCSTR aFileName, SIZE_T cbInMem, LPDB_OPTION lpOption ) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Remove)() PURE;
	RE_DECLARE_IID;
};

// {FA752082-A90C-4513-A8FE-FC58281CFB14}
RE_DEFINE_IID( IODBCreate, "{FA752082-A90C-4513-A8FE-FC58281CFB14}",
			0xfa752082, 0xa90c, 0x4513, 0xa8, 0xfe, 0xfc, 0x58, 0x28, 0x1c, 0xfb, 0x14);

//////////////////////////////////////////////////////////////////////////

typedef const RX_BLOB&	REFCBLOB;
typedef RX_BLOB&		REFBLOB;

class IODBAccess : public IUnknown
{
public:
	STDMETHOD(SetProp)( REFCBLOB rKey, LPCSTR aAttr, REFCBLOB rValue ) PURE;
	STDMETHOD(GetProp)( REFCBLOB rKey, LPCSTR aAttr, REFBLOB rValue ) PURE;
	STDMETHOD(DelProp)( REFCBLOB rKey, LPCSTR aAttr ) PURE;
	STDMETHOD(DelPropEx)( REFCBLOB rKey ) PURE;
	STDMETHOD(FlushProp)() PURE;
	RE_DECLARE_IID;
};

// {AD8A4D78-04C0-44dc-8409-B909BEA6EA3F}
RE_DEFINE_IID( IODBAccess, "{AD8A4D78-04C0-44dc-8409-B909BEA6EA3F}",
			0xad8a4d78, 0x4c0, 0x44dc, 0x84, 0x9, 0xb9, 0x9, 0xbe, 0xa6, 0xea, 0x3f);

#endif

// {BFC97F06-1CCC-4a5c-B993-327C032424D5}
DEFINE_GUID( CLSID_CProcessCloudProvider, 			/*使用单独的内存队列,关闭后丢失数据*/
			0xbfc97f06, 0x1ccc, 0x4a5c, 0xb9, 0x93, 0x32, 0x7c, 0x3, 0x24, 0x24, 0xd5);

// {2C8FD015-5CD5-47bc-971D-EB797E54B8DE}
DEFINE_GUID( CLSID_CRODB, 
			0x2c8fd015, 0x5cd5, 0x47bc, 0x97, 0x1d, 0xeb, 0x79, 0x7e, 0x54, 0xb8, 0xde);

// {7E77645A-FEB5-4406-8847-E772C9D37B73}
DEFINE_GUID(CLSID_CcUnkn, 
			   0x7e77645a, 0xfeb5, 0x4406, 0x88, 0x47, 0xe7, 0x72, 0xc9, 0xd3, 0x7b, 0x73);

// {D52DDB6D-8201-49fa-B41B-FDB09DBC5C02}
DEFINE_GUID(CLSID_PackageCount, 
			0xd52ddb6d, 0x8201, 0x49fa, 0xb4, 0x1b, 0xfd, 0xb0, 0x9d, 0xbc, 0x5c, 0x2);

#endif

