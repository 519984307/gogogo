#ifndef __IRXZDR_H__
#define __IRXZDR_H__


class IRXZdrClient : public IUnknown
{
	public:
		RE_DECLARE_IID;
	public:
		STDMETHOD(SetRemote)(const char * server, int port)PURE;
		STDMETHOD(SetRemote)(const char * url)PURE;
		STDMETHOD(Connect)()PURE;
		STDMETHOD_(BOOL,IsConnected)()PURE;
		STDMETHOD(Query)(const xv::Value& xin, xv::Value & xout, size_t timeout) PURE;
};


// {50C8965A-4BD2-4F33-8E01-AE9C3660D1AF}
RE_DEFINE_IID(IRXZdrClient, "{50C8965A-4BD2-4F33-8E01-AE9C3660D1AF}",
0x50c8965a, 0x4bd2, 0x4f33, 0x8e, 0x1, 0xae, 0x9c, 0x36, 0x60, 0xd1, 0xaf);


class IRXZdrClientParam : public IUnknown
{
public:
	RE_DECLARE_IID;
public:
	STDMETHOD(SetParam)(IRXParameters* param)PURE;
};
// {772EF383-4D43-4227-AA50-08BD26F73C01}
RE_DEFINE_IID(IRXZdrClientParam, "{772EF383-4D43-4227-AA50-08BD26F73C01}",
			0x772ef383, 0x4d43, 0x4227, 0xaa, 0x50, 0x8, 0xbd, 0x26, 0xf7, 0x3c, 0x1);


enum CLOUDHASH
{
	E_CRC32 = 0x00000001,
	//E_CRC64 = 0x00000002,
	E_SHA1  = 0x00000008,
	E_SHA2  = 0x00000010,
	E_SHA4  = 0x00000020,
	E_MD5	= 0x00000040,
	E_ALL_NORMAL_HASH = E_CRC32|E_SHA1|E_SHA2|E_SHA4|E_MD5,
	E_TJC64	= 0x00000080,
	E_TFE   = 0x00000100,
	E_PATH	= 0x00000200,
	E_RMDK	= 0x00000400,
	E_RMDV	= 0x00000800,
	E_AI	= 0x00001000,
	E_RDM   = E_RMDK|E_RMDV,
};

class IRXFetchZDRHashes: public IUnknown
{
public:
	RE_DECLARE_IID;
public:
	STDMETHOD(FetchHashes)(IUnknown* , xv::Value*)PURE;
	STDMETHOD(FetchHashes)(IUnknown* , UINT64 mask , xv::Value*)PURE;
};

// {9A687059-DE89-4FB4-902C-5E12642AEDA6}
RE_DEFINE_IID(IRXFetchZDRHashes, "{9A687059-DE89-4FB4-902C-5E12642AEDA6}",
			0x9a687059, 0xde89, 0x4fb4, 0x90, 0x2c, 0x5e, 0x12, 0x64, 0x2a, 0xed, 0xa6);




class IRXZdrBatchClient : public IUnknown
{
public:
	RE_DECLARE_IID;
public:
	STDMETHOD(Query)(const char** fnames , UINT32 count , size_t timeout ,xv::Value* xv)PURE;
	STDMETHOD(Query)(const wchar_t** fnames , UINT32 count , size_t timeout, xv::Value* xv )PURE;
	STDMETHOD(FetchCloudHashs)(const char** fnames , UINT32 count , xv::Value* xv)PURE;
	STDMETHOD(FetchCloudHashs)(const wchar_t** fnames , UINT32 count , xv::Value* xv)PURE;
};
// {F046E3D2-CDD4-4D29-9D4F-3098FD092365}
RE_DEFINE_IID(IRXZdrBatchClient, "{F046E3D2-CDD4-4D29-9D4F-3098FD092365}",
			0xf046e3d2, 0xcdd4, 0x4d29, 0x9d, 0x4f, 0x30, 0x98, 0xfd, 0x9, 0x23, 0x65);


#endif