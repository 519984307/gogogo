#ifndef __IRX_THREAT_SITUATION_NET__
#define __IRX_THREAT_SITUATION_NET__

namespace tsn
{
	//class IMessageReporter : public IUnknown
	//{
	//public:
	//	// 只提供异步消息接口
	//	STDMETHOD(QueuedMessage)( LPCSTR aTarget, LPCSTR aJsonMessage ) PURE;
	//	RE_DECLARE_IID;
	//};

	//// {6EC238D9-8462-42AF-99CB-82F990CEDC10}
	//RE_DEFINE_IID( IMessageReporter, "{6EC238D9-8462-42AF-99CB-82F990CEDC10}"
	//	0x6ec238d9, 0x8462, 0x42af, 0x99, 0xcb, 0x82, 0xf9, 0x90, 0xce, 0xdc, 0x10);

	class ITsnClient : public IUnknown
	{
	public:
		class ICallback : public IUnknown
		{
		public:
			STDMETHOD(OnEvent)( HRESULT lr, xv::Value & evnt ) PURE;
		};
	public:
		STDMETHOD(ReportMessage)( LPCSTR aTarget, LPCSTR aJsonMessage, DWORD dwSendTimeOut = 2000 ) PURE;
		STDMETHOD(GetConfig)( LPCSTR aField, xv::Value & val ) PURE;
		// 设置回调, 注意防止产生循环引用，必须SetCallback(NULL)来解除
		//STDMETHOD(SetCallback)( ICallback * callback ) PURE;
		// 开始等待排队消息上报完成
		//STDMETHOD(WaitCompletion)( DWORD dwTimeout = 5 * 1000 ) PURE;
		// 清理队列中的消息
		//STDMETHOD(CleanMessages)() PURE;
		RE_DECLARE_IID;
	};

	// {318F599D-1662-4549-AFB7-93FF837E9033}
	RE_DEFINE_IID( ITsnClient, "{318F599D-1662-4549-AFB7-93FF837E9033}",
		0x318f599d, 0x1662, 0x4549, 0xaf, 0xb7, 0x93, 0xff, 0x83, 0x7e, 0x90, 0x33);

};

EXTERN_C HRESULT WINAPI RxGetRemoteConfig( IRXParameters * lpParam, DWORD dwTimeout = 2000, LPCSTR lpUrl = NULL );


#endif