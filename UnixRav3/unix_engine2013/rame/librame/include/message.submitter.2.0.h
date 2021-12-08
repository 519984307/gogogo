#ifndef __MESSAGE_SUBMITTER_H__
#define __MESSAGE_SUBMITTER_H__

class IUmqPackage :  public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD_(LPCVOID, GetData)() PURE;
	STDMETHOD_(SIZE_T, GetSize)() PURE;
};
// {33CD6BF7-B2D2-4DDA-8C3E-46188CBED999}
RE_DEFINE_IID(IUmqPackage, "{33CD6BF7-B2D2-4DDA-8C3E-46188CBED999}",
			0x33cd6bf7, 0xb2d2, 0x4dda, 0x8c, 0x3e, 0x46, 0x18, 0x8c, 0xbe, 0xd9, 0x99);

struct UMQ_PARAMS_0 {
	DWORD			dwVersion;			// 0
	LPCSTR			sProduct;
	DWORD			nMaxBlockCount;
	DWORD			nMaxSizeOfBlock;
	DWORD			nMaxMessageSize;
	LPCSTR			sUserHeaderJson;
};

union CONST_VALUE {
	LPCSTR			ProductName;
	LPCSTR			PolicyURL;
	LPCSTR			SubmitionURL;
	BOOL			NeedToSubmit;
};


enum UmqMsgFlag
{
	UMQ_MSG_ENCODE   = 0x01,
	UMQ_MSG_COMPRESS = 0x02
};

class IUmqQueue :  public IUnknown
{
public:
	RE_DECLARE_IID;

	STDMETHOD(TestSubmittable)() PURE;

	// 查询内部常量
	STDMETHOD(QueryConst)( LPCSTR lpConstName, CONST_VALUE * lpConstValue ) PURE;

	// 设置公共消息头用户内容
	STDMETHOD(SetUserHeaderJson)( LPCSTR lpMessageHeaderJson ) PURE;

	// 设置 策略 JSON（支持哪些TOPIC,上报率是多少等）
	STDMETHOD(SetPolicy)( LPCSTR lpPolicy ) PURE;

	// 压入消息，可多线程调用
	STDMETHOD(PushMessage_MT)( LPCSTR lpTopic, LPCVOID lpData, SIZE_T sLen, DWORD dwFlags ) PURE;

	//弹出一个打包后的消息，dwMaxFetchSize 指定最多弹出多少字节的消息（多条消息尺寸累加）
	STDMETHOD(PopPackage_ST)( IUmqPackage ** lpPackage, DWORD dwMaxFetchSize = 32 * 1024 ) PURE;
};
// {5D554FC2-A777-4E50-9C7E-47707F88F6E2}
RE_DEFINE_IID(IUmqQueue, "{5D554FC2-A777-4E50-9C7E-47707F88F6E2}" ,
			0x5d554fc2, 0xa777, 0x4e50, 0x9c, 0x7e, 0x47, 0x70, 0x7f, 0x88, 0xf6, 0xe2);


class IUmqQueue2 :  public IUnknown
{
public:
	RE_DECLARE_IID;

	STDMETHOD(SetFilterCap)( LPCSTR lpTopic, SIZE_T nCount = 10000 ) PURE;
	// 压入消息，可多线程调用
	STDMETHOD(PushMessageWithHash)( 
		LPCSTR lpTopic, 
		UINT64 uMessageHash,
		LPCVOID lpData, SIZE_T sLen, 
		DWORD dwFlags ) PURE;
};
// {475DA22E-0CEE-40AF-B410-B8465F0A89B0}
RE_DEFINE_IID(IUmqQueue2, "{475DA22E-0CEE-40AF-B410-B8465F0A89B0}", 
					0x475da22e, 0xcee, 0x40af, 0xb4, 0x10, 0xb8, 0x46, 0x5f, 0xa, 0x89, 0xb0);


enum UmqOpenMode {
	DO_OPEN		= 1,
	DO_CREATE	= 2,
	DO_RESET	= 4,
};

HRESULT WINAPI UmqCreateQueue( 
	LPCSTR lpProduct,
	LPCSTR lpQueueHome,
	SIZE_T nOpenMode,
	CONST UMQ_PARAMS_0 * lpOptions OPTIONAL,
	IUmqQueue ** ppMessageQueue );

HRESULT WINAPI UmqRemoveQueue( LPCSTR lpQueueHome );


//// 初始化UMQ
//void InitUmQueue() {
//	ComPtr<IUmqQueue> queue;
//	{
//		ComPtr<IUmqCreator> creator;
//		creator->Create("/XXX/...", TRUE, &options, &queue);
//	}
//}
//
//// 策略更新线程。最好以计划任务的方式出现，可以几个小时更新一次
//void umcUpdateProductPolicy() {
//	// http http://umc.rising.com.cn/api/1.0/product/rav
//	ComPtr<IUmqQueue> queue;
//	CONST_VALUE PolicyURL;
//	queue->QueryConst("PolicyURL", &PolicyURL );
//	Content = http.get(PolicyURL);
//	queue->SetPolicy( Content.c_str() );
//}
//
//// 日志写入线程。最好以计划任务的方式出现
//void onEvent( Json::Value & jEvent ) {
//	ComPtr<IUmqQueue> queue;
//	if( FAILED(queue->TestSubmittable()) ) return;
//	std::string body = jEvent;
//	queue->PushMessage_MT("behavior.process", body.c_str(), body.size());
//}
//
//// 消息上报线程。
//void submitThread(  ) {
//	ComPtr<IUmqQueue> queue;
//	CONST_VALUE SubmitionURL;
//	queue->QueryConst("SubmitionURL", &SubmitionURL );
//	for(;;) {
//		if( bBreak ) break;
//		ComPtr<IUmqPackage> package;
//		if( S_OK == queue->PopPackage_ST( &package ) ) {
//			// http ....
//			HTTP.post( SubmitionURL, package->GetData(), package->GetSize() );
//		}
//		::Sleep( 10 + rand(10) );
//	}
//}


#endif
