#ifndef __LIB_UPDATE__
#define __LIB_UPDATE__

namespace mss
{
	enum UPEVENT							// >= 0, Step Report
											// < 0,  Error Report
	{
		STEP_PROCESS_FILE		=  0,

		ERR_UNKNOWN_ANY			= -1,		

		ERR_WANTFILE_OPEN		= -100,		// �����ļ���ʧ�ܣ�wFileNameΪWantFileName
		ERR_WANTFILE_PARSE		= -101,		// �����ļ�����ʧ�ܣ�wFileNameΪWantFileName

		ERR_TMPFILE_CREATE		= -200,		// ������ʱ�ļ�ʧ�ܣ�wFileNameΪ��ʱ�ļ�·��
		ERR_TMPFILE_RENAME		= -201,		// ��ʱ�ļ�����Ϊ���ؿ���ʱʧ�ܣ�wFileNameΪ���ؿ��ļ�����

		ERR_UPDATE_APPLY		= -300,		// ����Ӧ������ʱ������wFileNameΪ��ʱ�ļ�·��
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

	class ILibFileUpTool : public IUnknown
	{
	public:
		STDMETHOD(SetLicence)(LPVOID path) PURE;
		RE_DECLARE_IID;
	};

	// {A5EB0F97-F1D7-43D0-B680-BEFF50909B60}
	RE_DEFINE_IID(ILibFileUpTool, "{A5EB0F97-F1D7-43D0-B680-BEFF50909B60}",
		0xa5eb0f97, 0xf1d7, 0x43d0, 0xb6, 0x80, 0xbe, 0xff, 0x50, 0x90, 0x9b, 0x60);

	class ILibFileUp : public IUnknown
	{
	public:
		
		//
		//	��ʼ�������ݱ����ļ��������ļ�������������������ļ��б�
		//	
		//	S_OK, �ɹ�����Ҫ���� 
		//	S_FALSE, �ɹ�������Ҫ����
		//
		
		STDMETHOD(Init)( LPCSTR wHome, LPCSTR wCfg, ILibFileUpCallback* pcb, LPCSTR aCustomFileName = 0 ) PURE;

		//
		//	�����ļ�������������ɺ�����һ����ʱ�ļ�
		//
		//	S_OK,		�ɹ���������ɣ�������һ����ʱ�ļ�
		//	S_FALSE,	�ɹ�������Ҫ����
		//	< 0,		ͨ��ILibFileUpCallback������ʧ�ܵ����
		//
		//	2013/8/13: ֮ǰ˵�˷��������ظ����ã����ڿ�ʼ����֧�֣�ʹ�����޸��������Ҫ�ͷŴ˶������´���һ��ILibFileUp��������
		//
		
		STDMETHOD(Update)( LPCSTR wWantFileHome, ILibFileUpCallback* pcb ) PURE;

		//
		//	�ļ�������ϣ�����û�з�������ʹ�ô˷��������ڴ��е�ȫ�ְ汾��
		//

		STDMETHOD(NotifyUpdated)() PURE;

		//
		// ���ò������ļ�����·��
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
		//	��ʼ�������ݱ����ļ��������ļ�������������������ļ��б�
		//	
		//	S_OK, �ɹ�����Ҫ����
		//	S_FALSE, �ɹ�������Ҫ����
		//

		STDMETHOD(Init)( LPCWSTR wHome, LPCWSTR wCfg, ILibFileUpCallbackW* pcb, LPCWSTR aCustomFileName = 0 ) PURE;

		//
		//	�����ļ�������������ɺ�����һ����ʱ�ļ�
		//
		//	S_OK,		�ɹ���������ɣ�������һ����ʱ�ļ�
		//	S_FALSE,	�ɹ�������Ҫ����
		//	< 0,		ͨ��ILibFileUpCallback������ʧ�ܵ����
		//
		//	2013/8/13: ֮ǰ˵�˷��������ظ����ã����ڿ�ʼ����֧�֣�ʹ�����޸��������Ҫ�ͷŴ˶������´���һ��ILibFileUp��������
		//
		STDMETHOD(Update)( LPCWSTR wWantFileHome, ILibFileUpCallbackW* pcb ) PURE;


		//
		//	�ļ�������ϣ�����û�з�������ʹ�ô˷��������ڴ��е�ȫ�ְ汾��
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
	//		�������Ŀ¼����Ҫ���ļ��б���
	//
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////

	typedef enum HOME_UP_WANT_FILE_STATUS
	{
		FILE_STATUS_GOOD = 0,
		//////////////////////////////
		FILE_CAN_NOT_OPEN,		// �ļ��򲻿�
		FILE_DAMGED,			// �ļ���֤����
		FILE_MUST_REGET,		// �ļ�������
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