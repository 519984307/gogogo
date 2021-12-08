/*
 * =====================================================================================
 *
 *       Filename:  ICloudQuery.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/18/2014 06:10:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  chenggl (), chenggl@rising.com.cn
 *        Company:  RISING
 *
 * =====================================================================================
 */
#ifndef __RS_CQ_ICloudQuery__
#define __RS_CQ_ICloudQuery__
#include <cstddef>
#include <iostream>
#include <string>
using namespace std;
#include <unistd.h>

typedef unsigned int		UINT32;
//typedef unsigned long long	UINT64;
typedef char				CHAR;
typedef unsigned char		BYTE;
//typedef unsigned int		SIZE_T;
typedef const char*			LPCSTR;
//typedef bool				BOOL;
typedef unsigned int		DWORD;

class Iobject
{
	public:
		virtual ~Iobject(){};
};

enum IKFaceScannerType
{
	CloudScanner = 1,
	BackstageScanner = 2,
};
enum 
{ 
	MD5_BYTES = 16,
	TJCRC_BYTES = 8,
	VNAME_LEN = 64,
};

enum KFaceScanVirusType
{
	/*
	 *		0—不是木马库，1—是木马库，2—白名单,3-未知,4-未知上报
	 *				请和Cloud::TCSxxx同步定义!
	 *					*/
	kTCSNotTrojan,
	kTCSIsTrojan,
	kTCSIsWhite,
	kTCSUnknown,
	kTCSUnknownUpload,
	kTCSGray = 9,
};

struct KFaceCloudRequest
{
	unsigned long Id; /*  ID是唯一的 */
	unsigned char CheckSum1[8];
	unsigned char CheckSum2[8];
};

struct KFaceCloudResponse
{
	unsigned long Id; /*  ID是唯一的，应该和KFaceCloudRequest的Id一致 */
	long	      Success;/*  0 = failed, else = success */
	KFaceScanVirusType	Type1;
	KFaceScanVirusType	Type2;
};

struct KFaceCloudVersionInfo
{
	unsigned long	Version; 
};

class IKFaceScanResult:public Iobject{};

class IKFaceScanServiceProvider:public Iobject
{
	public:
		virtual long MayIcontinue(){};
		virtual long CloudQuery(const KFaceCloudRequest* pRequest, KFaceCloudResponse* pResults, int nElements, int* pnElementsRet){};
		virtual long UpdateWLLib(KFaceCloudVersionInfo* pCurrentLib, KFaceCloudVersionInfo* pNewLib, const wchar_t* pLibPath) = 0;
		virtual long DownloadWhiteLib(const wchar_t* pLibPath) = 0;
		virtual void OnScanFinish(const wchar_t* strPath, IKFaceScanResult* pResult){};
};


struct CLOUD_RESPONSE_V3
{
	KFaceScanVirusType	State;
	UINT32				VirusId;
	CHAR				VirusName[VNAME_LEN];
};
union FILE_TJCRC
{
	BYTE	Bytes[TJCRC_BYTES];
	UINT64	Value64;
};
struct FILE_MD5
{
	BYTE	Bytes[MD5_BYTES];
};

class IRsCloudQueryV3:public Iobject
{
	public:
		virtual long GetLifeTimeOfLibCacheFromCloud( SIZE_T* lpDay )=0;
		virtual long CloudQueryByTJCRC(const FILE_TJCRC* lpTrojCRCs, UINT32 uiCount, CLOUD_RESPONSE_V3* pResults) = 0;
		virtual long CloudQueryByMD5(const FILE_MD5* lpFileMD5s, UINT32 uiCount, CLOUD_RESPONSE_V3* pResults ) = 0;
		virtual long IsCloudCenterConnected()=0;
};


class IRsCloudQueryCacheLifeTime:public Iobject
{
	public:
		virtual long GetLifeTimeOfKnownLibCache( SIZE_T * lpHours )=0;
		virtual long GetLifeTimeOfUnknownLibCache( SIZE_T * lpHours )=0;
};

typedef struct _FILE_TO_SUBMIT
{
	LPCSTR		FileMD5A;		// 用于去重的MD5，一般为当前文件的MD5,字符串
	UINT64		FileSize64;		// 要上传的文件大小
	BOOL		VirtualFile;	// 虚拟文件，为真时，脱离当前环境后将无法获得文件内容;
	// 文件内容可以通过FileObject获得;
	// 非虚拟可以通过FileNameA|FileNameW找到文件进行延迟处理
	LPCSTR		FileNameA;		// 文件对象全路径名(MBS),可能没有
} F2SINFO, *LPF2SINFO;

typedef struct FILE_SUBMIT_OPTIONS
{
	DWORD	cbStruct;			// == sizeof(FILE_SUBMIT_OPTIONS)
	DWORD	MaxFileSize32;
} UPOPTS, *LPUPOPTS;

class IRsDataSubmitter:public Iobject
{
	public:
		virtual long SubmitInfo( LPCSTR aInfoMsg )=0;
		virtual long SubmitFile( LPCSTR aSubmitType, LPF2SINFO lpInfo, Iobject* lpContent) =0;
		virtual long GetSubmitFileOptions( LPUPOPTS lpUpOpts )=0;
};

//------------------------------------------------------

enum CLOUDQUERY_INSTANCE_ID
{ 
	IKFaceScanServiceProvider_INSTANCE = 1,
	IRsCloudQueryV3_INSTANCE = 2,
	IRsCloudQueryCacheLifeTime_INSTANCE = 4,
	IRsDataSubmitter_INSTANCE = 8,
};

class IFactory:public Iobject
{
	public:
		virtual Iobject* GetObject(CLOUDQUERY_INSTANCE_ID iid) =0;
};

#endif

