#ifndef __iextmail_h__2007_5_16__18H04M13S
#define __iextmail_h__2007_5_16__18H04M13S

//------------------------------------------------------------------------------------
//邮件接口

//用于子邮件类型，
//此属性位于邮件解出的子IREStream中，PID为 PID_MAILFILE_SUBTYPE
enum MAILFILE_SUBTYPE
{
	MAILFILE_ST_TEXT_HTML = 0, 
	MAILFILE_ST_TEXT_PLAIN,
	MAILFILE_ST_ATTACHMENT,
};

//标题和周边
//此属性位于IREExtmail的SetProperty\GetProperty
enum MAILFILE_PROPERTY
{
	//读写
	MAILFILE_SUBJECTMARK,		//主题附加串				
	MAILFILE_HTMLMARK,			//超文本附加串			
	MAILFILE_PLAINMARK,			//文本附加串	
	MAILFILE_APPCHASET,			//应用程序字符集，用作改写邮件的编码。包括：主题编码和附件名编码

	MAILFILE_SUBJECT,			//主题					
	MAILFILE_SENDER, 			//发信人				
	MAILFILE_RECEIVER,			//收信人			
	MAILFILE_CC, 				//cc					
	MAILFILE_BCC,				//bcc		

	//只读．固定的属性
	MAILFILE_PLAINCHARSET,	 	//正文字符集		    
	MAILFILE_HTMLCHARSET, 		//超文本正文字符集 
	MAILFILE_ATTCOUNT, 			//附件数目			　　
	MAILFILE_SUBITEMCOUNT,		//子文档的数目			

	//特殊
	MAILFILE_BUILDALLSIZE,		//邮件回写后的估算大小	 

};


//正文 和 附件
struct REAttInfo
{
	CHAR pszAttName[MAX_PATH*2];
	DWORD dwAttSize;
};

class IREMailFile : public IUnknown
{
public:
	RE_DECLARE_IID;

	//对于邮件参数和固定的一些变量的存取接口
	STDMETHOD(SetProperty)(const MAILFILE_PROPERTY rpid, const PROPVARIANT *pv) PURE;
	STDMETHOD(GetProperty)(const MAILFILE_PROPERTY rpid, const PROPVARIANT **ppv) PURE;

	//附件
	STDMETHOD( GetAttInfo )			(INT iIndex,	OUT REAttInfo *pInfo) PURE;
	STDMETHOD( GetAttData )			(INT iIndex,	OUT const IREStream **ppStream) PURE;
	STDMETHOD( AddAtt)				(IREStream* lpszAttName) PURE;

	//正文
	STDMETHOD( GetHTML )			(OUT IREStream** ppStream) PURE;
	STDMETHOD( GetPlain	)			(OUT IREStream** ppStream) PURE;
};

RE_DEFINE_IID(IREMailFile, "{4ACF628E-8F96-4bfb-BF67-A16CEB84FC99}",
			0x4acf628e, 0x8f96, 0x4bfb, 0xbf, 0x67, 0xa1, 0x6c, 0xeb, 0x84, 0xfc, 0x99);


//------------------------------------------------------------------------------------
//邮件
//treat as compound file OR mail file


//------------------------------------------------------------------------------------
//邮箱
//treat as compound file





//------------------------------------------------------------------------------------
//特殊编码文件
//treat as compound file









#endif // __iextmail_h__
