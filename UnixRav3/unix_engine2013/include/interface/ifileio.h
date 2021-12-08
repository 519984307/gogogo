#ifndef __RX_FILE_IO__
#define __RX_FILE_IO__

struct XSTRP
{
	BOOL	IsUnicode;
	union 
	{
		LPCSTR	NameA;
		LPCWSTR	NameW;
		LPCVOID	NameV;
	};
};


typedef XSTRP			UnionNameRO;
typedef XSTRP&			REFXSTRP;
typedef const XSTRP&	REFCXSTRP;

typedef DWORD			OPENMODE;

class IRXStream;

class IRXFileSystem : public IUnknown
{
public:
	RE_DECLARE_IID;
	STDMETHOD_(BOOL, IsPhysical)() PURE;
	STDMETHOD(ReOpenFile)( LPHANDLE lpHandle, HANDLE hOriginalFile, OPENMODE dwOpenMode ) PURE;
	STDMETHOD(OpenFileById)( LPHANDLE, UINT64 , DWORD ) PURE;
	STDMETHOD(CreateFile)( REFCXSTRP rFileName, OPENMODE dwOpenMode, OUT LPHANDLE phFile ) PURE;
	STDMETHOD(CloseHandle)( HANDLE hFile ) PURE;
	STDMETHOD(ReadFile)( HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead ) PURE;
	STDMETHOD(WriteFile)( HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten ) PURE;
	STDMETHOD(FlushFileBuffers)(HANDLE hFile) PURE;
	STDMETHOD(SetEndOfFile)(HANDLE hFile) PURE;
	STDMETHOD(SetFilePointerEx)(HANDLE hFile,LARGE_INTEGER liDistanceToMove,PULARGE_INTEGER lpNewFilePointer OPTIONAL, DWORD dwMoveMethod ) PURE;
	STDMETHOD(GetFileSizeEx)(HANDLE hFile,PULARGE_INTEGER lpFileSize) PURE;
	STDMETHOD(DeleteFileH)( HANDLE hFile ) PURE;
	STDMETHOD(DeleteFile)( REFCXSTRP rFileName ) PURE;
	STDMETHOD(EnsureFileExists)( REFCXSTRP rPath, BOOL bPrefixOnly = FALSE ) PURE;
	STDMETHOD(CreateDirectory)( REFCXSTRP rDirName, LPCVOID ps ) PURE;
	STDMETHOD(MoveFileEx)( REFCXSTRP rNameOld, REFCXSTRP rNameNew, DWORD dwFlags = 0 ) PURE;
	STDMETHOD(XCreateStream)( REFCXSTRP rFileName, IRXStream** ppstm, DWORD dwOpenMode = 0 ) PURE;
};

// {9BB5555E-02B0-46ca-A94A-5BF12C565F2D}
RE_DEFINE_IID(IRXFileSystem, "{9BB5555E-02B0-46ca-A94A-5BF12C565F2D}",
			  0x9bb5555e, 0x2b0, 0x46ca, 0xa9, 0x4a, 0x5b, 0xf1, 0x2c, 0x56, 0x5f, 0x2d);

class IRXFileSystemMapping : public IUnknown
{
public:
	RE_DECLARE_IID;
	//
	// file mapping
	//
	STDMETHOD(CreateFileMap)(HANDLE hFile,DWORD flProtect,UINT64 uMaxSize, OUT LPHANDLE phFileMapping ) PURE;
	STDMETHOD(MapViewOfFile)(HANDLE hMap, DWORD dwDesiredAccess, UINT64 uOffset, SIZE_T cbToMap, OUT LPVOID * ppBaseAddress ) PURE;
	STDMETHOD(FlushViewOfFile)(HANDLE hMap, LPCVOID lpBaseAddress, SIZE_T dwNumberOfBytesToFlush ) PURE;
	STDMETHOD(UnmapViewOfFile)(LPCVOID lpBaseAddress, HANDLE hMap = NULL ) PURE;
};

// {A8050C7E-075C-4409-8649-C1884D2463BC}
RE_DEFINE_IID(IRXFileSystemMapping, "{A8050C7E-075C-4409-8649-C1884D2463BC}",
			  0xa8050c7e, 0x75c, 0x4409, 0x86, 0x49, 0xc1, 0x88, 0x4d, 0x24, 0x63, 0xbc)

//////////////////////////////////////////////////////////////////////////

class IREMemRegion : public IUnknown
{
public:
	STDMETHOD(GetAccess)( LPDWORD pdwAccess ) PURE;
	STDMETHOD_(LPVOID,GetBase)() PURE;
	STDMETHOD_(SIZE_T,GetSize)() PURE;
	RE_DECLARE_IID;
};
// {BB3EC78C-2E69-4ec9-8D07-93194F3AA639}
RE_DEFINE_IID( IREMemRegion, "{BB3EC78C-2E69-4ec9-8D07-93194F3AA639}",
			0xbb3ec78c, 0x2e69, 0x4ec9, 0x8d, 0x7, 0x93, 0x19, 0x4f, 0x3a, 0xa6, 0x39);


////////////////////////////////////////////////////////////////
//SEEK模式
enum SEEKFROM 
{ 
	SPBegin		= 0x0,	//从文件头
	SPCurrent	= 0x1,	//从当前位置
	SPEnd		= 0x2,	//从文件尾
};

//流状态
enum FOSTATUS
{
	FOSFree			= 0,
	FOSOpened		= 1,
	FOSModified		= 2,	FOSChanged = FOSModified,
	FOSDeleted		= 4,
	FOSRenamed		= 8,

	FOS_DelOrChanged = FOSModified|FOSDeleted,


	FSFree			= FOSFree,
	FSOpened		= FOSOpened,
	FSChanged		= FOSChanged,
	FSDeleted		= FOSDeleted,
	FSRenamed		= FOSRenamed
};

//扫描的对象的类型
enum
{
	HOSTTYPE_ANSINAME = 0,	//ansi文件名
	HOSTTYPE_UNICODENAME,	//UNICODE 文件名
	HOSTTYPE_DISK,			//a b 软盘 c 所有硬盘 *所有磁盘
};

struct STFileName
{
	DWORD dwType;  //扫描的对象的类型
	DWORD dwLen;	  //缓存长度按字节
	union
	{
		void* pData;	//HOSTTYPE_DISK 有效
		char* pAName;	 //HOSTTYPE_ANSINAME 有效
		wchar_t* pWName; //HOSTTYPE_UNICODENAME 有效
	};
};

//文件的打开方式
enum OpenMode
{
	OMCreate		= 0x0001,
	OMAlways		= 0x0002,
	OMTrunc			= 0x0004,

	OMRead			= 0x0100,
	OMWrite			= 0x0200,
	OMExecute		= 0x0400,
	OMNOTResize		= 0x0800,

	OMAutoDel		= 0x1000,
	OMNoCache		= 0x2000,
	OMSeqScan		= 0x4000,
	OMTEMP			= 0x8000,

	FOMSHRD			= 0x10000,
	FOMSHWD			= 0x20000,
	FOMSHDD			= 0x40000,

	//r		Opens for reading 
	//		If the file does not exist or cannot be found, the fopen call fails.
	FOMR			=	OMRead,	
	//w		Opens an empty file for writing 
	//		If the given file exists, its contents are destroyed.
	FOMW			=	OMCreate | OMWrite,

	//r+	Opens for both reading and writing 
	//		(The file must exist.)
	FOMRP			=	OMRead | OMWrite,
	FOMRW			=	FOMRP,
	//w+	Opens an empty file for both reading and writing 
	//		If the given file exists, its contents are destroyed.
	FOMWP			=	OMCreate | OMRead | OMWrite,
	//a+	Opens for reading and appending
	//		creates the file first if it doesn’t exist
	FOMAP			=	OMCreate | OMRead | OMNoCache,	
	// 
	FOMRM			=	FOMR | OMNoCache,
	FOMWM			=	FOMW | OMNoCache,
	FOMRPM			=	FOMRP | OMNoCache,
	FOMWPM			=	FOMWP | OMNoCache,
	FOMAPM			=	FOMAP | OMNoCache,

	FOMSHA = 0,		// It's default
};

enum 
{
	PROP_EXPLICIT_DEL = 1,		// 表明此PROP必须显式的删除(DeleteProperty,Reset),ClearProperty无法将其清除
};

template<class T>
class __tIREProperty : public IUnknown
{
public:
	typedef	T	KeyType;
public:
	STDMETHOD(SetProperty)(const KeyType& rpid, const PROPVARIANT* pv) = 0;
	STDMETHOD(GetProperty)(const KeyType& rpid, const PROPVARIANT** ppv) = 0;
	STDMETHOD(DeleteProperty)(const KeyType& rpid) = 0;
	STDMETHOD(ClearProperty)() = 0;
};

class IREProperty : public	__tIREProperty<GUID>
{
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREProperty, "{DF343B0B-D142-4e3c-BE9C-02BBED29D27E}",
			  0xdf343b0b, 0xd142, 0x4e3c, 0xbe, 0x9c, 0x2, 0xbb, 0xed, 0x29, 0xd2, 0x7e);


class IREProperty2 : public	__tIREProperty<DWORD>
{
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREProperty2, "{203ADD4C-48C7-486d-B62A-F22644ED1E1D}",
			  0x203add4c, 0x48c7, 0x486d, 0xb6, 0x2a, 0xf2, 0x26, 0x44, 0xed, 0x1e, 0x1d);

class IREPropertyEx : public IREProperty
{
public:
	RE_DECLARE_IID;
	STDMETHOD(MarkProperty)( const KeyType& rpid, SIZE_T uMask ) = 0;
	STDMETHOD(Reset)() = 0;
	//STDMETHOD(MoveProperty)( IREPropertyEx * prop, const KeyType & rpid ) = 0;
};
// {2BC46C11-280B-45C0-9F63-C22B7BFAC4DF}
RE_DEFINE_IID( IREPropertyEx, "{2BC46C11-280B-45C0-9F63-C22B7BFAC4DF}",
			0x2bc46c11, 0x280b, 0x45c0, 0x9f, 0x63, 0xc2, 0x2b, 0x7b, 0xfa, 0xc4, 0xdf);

class IRXArchive;

class IREStream : public IUnknown
{
public:
	STDMETHOD(Read)(LPVOID lpBuffer, DWORD cbBytesToRead, OUT LPDWORD lpBytesRead OPTIONAL) PURE;
	STDMETHOD(Write)(LPCVOID lpBuffer, DWORD cbBytesToWrite,OUT LPDWORD lpBytesWritten OPTIONAL) PURE;
	STDMETHOD(Flush)() PURE;
	STDMETHOD(Seek)(LONG lDistanceToMove,PLONG lpDistanceToMoveHigh OPTIONAL,DWORD dwMoveMethod) PURE;
	STDMETHOD(Tell)(OUT LPDWORD lpOffsetLow,OUT LPDWORD lpOffsetHigh OPTIONAL) PURE;
	STDMETHOD(GetSize)(OUT LPDWORD lpSizeLow,OUT LPDWORD lpSizeHigh OPTIONAL) PURE;
	STDMETHOD(SetSize)(DWORD dwSizeLow, LPDWORD lpSizeHigh OPTIONAL) PURE;
	STDMETHOD_(DWORD, GetStatus)() PURE;
	STDMETHOD_(DWORD, ModifyStatus)( DWORD dwAdd, DWORD dwRemove = 0 ) PURE;
	STDMETHOD(SetAccess)( DWORD dwAccess ) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Remove)() PURE;
	STDMETHOD(IsPhysical)( BOOL& rIsPhy ) PURE;
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREStream, "{BD7BCBDB-A6BC-49ab-A467-6B1C4B591A57}",
			  0xbd7bcbdb, 0xa6bc, 0x49ab, 0xa4, 0x67, 0x6b, 0x1c, 0x4b, 0x59, 0x1a, 0x57);

// only change show-name
class IRXStream : public IREStream
{
public:
	STDMETHOD(SetName)( REFCXSTRP rName ) PURE;
	STDMETHOD(GetName)( REFXSTRP rName ) PURE;
	STDMETHOD(SetParent)( IRXArchive* lpArchive ) PURE;
	STDMETHOD(GetParent)( IRXArchive** lppArchive ) PURE;
	STDMETHOD(Seek64)( INT64 iDistance, DWORD dwMoveMethod) PURE;
	STDMETHOD(Tell64)( UINT64& rFilePtr ) PURE;
	STDMETHOD(GetSize64)( UINT64& rFileSize ) PURE;
	STDMETHOD(SetSize64)( UINT64 uFileSize ) PURE;
	STDMETHOD(SetEOF)() PURE;
	STDMETHOD(GetFileSystem)( IRXFileSystem** ppfs ) PURE;
	STDMETHOD(Readp)( UINT64 off, LPVOID buf, DWORD bytse, DWORD * eff ) PURE;
	STDMETHOD(Writep)( UINT64 off, LPCVOID buf, DWORD bytse, DWORD * eff ) PURE;
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IRXStream, "{06C8A097-0950-45AC-B3D6-A41B9EA80101}",
			  0x6c8a097, 0x950, 0x45ac, 0xb3, 0xd6, 0xa4, 0x1b, 0x9e, 0xa8, 0x01, 0x01);

typedef IRXStream	IREStreamEx;


class IRXLogicFilePatcher : public IUnknown
{
public:
	STDMETHOD(MakeDeltaOfInnerStream)(IREStream* delta) PURE;
	STDMETHOD(RecoverFromInnerStream)(IREStream* delta , IREStream* target) PURE;
public:
	RE_DECLARE_IID;
};

// {499391BB-43C7-4AD9-A7C1-B5AA2837A54E}
RE_DEFINE_IID(IRXLogicFilePatcher, "{499391BB-43C7-4AD9-A7C1-B5AA2837A54E}" , 
			0x499391bb, 0x43c7, 0x4ad9, 0xa7, 0xc1, 0xb5, 0xaa, 0x28, 0x37, 0xa5, 0x4e);

//////////////////////////////////////////////////////////////////////////

class IRxRemoveFile : public IUnknown
{
public:
	RE_DECLARE_IID;
public:
	STDMETHOD( PreWrite )( IRXStream * oTarget ) PURE;
	STDMETHOD( Remove )( IRXStream * oTarget ) PURE;
};

// {5C7DF0F2-EC7E-46ca-A537-17BC68E9F358}
RE_DEFINE_IID(IRxRemoveFile, "{5C7DF0F2-EC7E-46ca-A537-17BC68E9F358}",
			0x5c7df0f2, 0xec7e, 0x46ca, 0xa5, 0x37, 0x17, 0xbc, 0x68, 0xe9, 0xf3, 0x58);

//////////////////////////////////////////////////////////////////////////
class IRXMassArchive;
class IRxHandleFile : public IUnknown
{
public:
	STDMETHOD(Open)( IRXFileSystem * opFileSystem, REFCXSTRP rFileName, DWORD dwOpenMode ) PURE;
	STDMETHOD(Open)( IRXFileSystem * opFileSystem, UINT64 uFileId, DWORD dwOpenMode ) PURE;
	STDMETHOD(Open)( HANDLE hFile, DWORD dwOpenMode, REFCXSTRP rFileName OPTIONAL, IRXFileSystem * opFileSystem = NULL OPTIONAL ) PURE;
	STDMETHOD(GetHandle)( LPHANDLE phHandle, IRXFileSystem ** ppFileSystem OPTIONAL ) PURE;
	STDMETHOD(Rename)( REFCXSTRP rName, DWORD dwFlags = 0 ) PURE;
	STDMETHOD(GetFileName)( REFXSTRP rFileName ) PURE;
public:
	RE_DECLARE_IID;
};

// {1557CD17-6FEC-43ca-AB8D-6741CEE8795E}
RE_DEFINE_IID( IRxHandleFile, "{1557CD17-6FEC-43ca-AB8D-6741CEE8795E}",
			0x1557cd17, 0x6fec, 0x43ca, 0xab, 0x8d, 0x67, 0x41, 0xce, 0xe8, 0x79, 0x5e);

class IRxMemFileRO : public IREMemRegion
{
public:
	STDMETHOD(Init)( SIZE_T cbBlockSize ) PURE;
	STDMETHOD(Init)( IREStream* lpStream, SIZE_T cbBlockSize ) PURE;
	RE_DECLARE_IID;
};

// {DE7B412D-C367-4426-A39D-2AA146A23F27}
RE_DEFINE_IID( IRxMemFileRO, "{DE7B412D-C367-4426-A39D-2AA146A23F27}",
			0xde7b412d, 0xc367, 0x4426, 0xa3, 0x9d, 0x2a, 0xa1, 0x46, 0xa2, 0x3f, 0x27);


// 内存块接口
class IRXMemoryBlock : public IUnknown
{
public:
	STDMETHOD(Alloc)( SIZE_T uBytes ) PURE;
	STDMETHOD(_Free)() PURE;
	STDMETHOD_(SIZE_T, Size)() PURE;
	STDMETHOD(Attach)( LPVOID lpBlock, SIZE_T uBytes ) PURE;
	STDMETHOD_(LPVOID, GetBuffer)() PURE;
	RE_DECLARE_IID;
};

// {E4E75407-8E48-4691-94CE-5C7DB82BC480}
RE_DEFINE_IID( IRXMemoryBlock, "{E4E75407-8E48-4691-94CE-5C7DB82BC480}",
		   0xe4e75407, 0x8e48, 0x4691, 0x94, 0xce, 0x5c, 0x7d, 0xb8, 0x2b, 0xc4, 0x80);



typedef struct _REGION64
{
	UINT64	Offset;
	UINT64	Length;
} REGION64, *LPREGION64;

class IRXLogicFile : public IUnknown
{
public:
	enum LogicStyle
	{
		AS_VIRTUAL_STREAM,
		AS_WRITE_COPY_STREAM,
		AS_PASSTHOUGH_STREAM,
		AS_WRITE_COPY_ALL,
	};
	enum COWS_MODE
	{
		COW_AUTO_FLUSH = 1,
		COW_DELIVER_REMOVE = 2,
	};
public:
	STDMETHOD(Open)( LogicStyle aStyle, IREStream* aBackStream, LPREGION64 lpRegion, OPENMODE uOpenMode ) PURE;
	STDMETHOD(GetBackStream)( IREStream** ppstm, LPREGION64 lpRegion ) PURE;
	STDMETHOD(FlushBack)() PURE;
	STDMETHOD(ModifyMode)( SIZE_T uModeAdd, SIZE_T uModeDel ) PURE;
	STDMETHOD(Rollback)() PURE;	// only AS_WRITE_COPY_STREAM
public:
	RE_DECLARE_IID;
};
// {DF0FAA71-1F87-45c3-9256-3AB32A4765DB}
RE_DEFINE_IID( IRXLogicFile, "{DF0FAA71-1F87-45c3-9256-3AB32A4765DB}",
			  0xdf0faa71, 0x1f87, 0x45c3, 0x92, 0x56, 0x3a, 0xb3, 0x2a, 0x47, 0x65, 0xdb);

//////////////////////////////////////////////////////////////////////////

class IRxFileIOCtl : public IUnknown
{
public:
	RE_DECLARE_IID;
public:
	STDMETHOD( SetFlag )( BOOL f ) PURE;
};

// {F5BF0C2F-2289-4ABB-9B34-DCB2720D7E2E}
RE_DEFINE_IID(IRxFileIOCtl, "{F5BF0C2F-2289-4ABB-9B34-DCB2720D7E2E}",
			  0xf5bf0c2f, 0x2289, 0x4abb, 0x9b, 0x34, 0xdc, 0xb2, 0x72, 0xd, 0x7e, 0x2e);

//////////////////////////////////////////////////////////////////////////

// VT_UI4:
// DEFINE_GUID(PID_File_UpdateMark,		0x2ac2ffb4, 0x950a, 0x4c0f, 0x9e, 0xa0, 0xbe, 0x53, 0xf4, 0x88, 0x65, 0x76);
// {844864DD-FD3A-4452-B1E2-EC4360DC7132}
DEFINE_GUID(PID_File_FormatList,		0x844864dd, 0xfd3a, 0x4452, 0xb1, 0xe2, 0xec, 0x43, 0x60, 0xdc, 0x71, 0x32);
// {C0FFE320-F7FD-4318-8FC1-208205E84ECB}
DEFINE_GUID(PID_File_Sig64List,			0xc0ffe320, 0xf7fd, 0x4318, 0x8f, 0xc1, 0x20, 0x82, 0x5, 0xe8, 0x4e, 0xcb);
// {8E823449-76C3-435f-95BA-70E130B9BCD3}
DEFINE_GUID(PID_File_CurrentFormat,		0x8e823449, 0x76c3, 0x435f, 0x95, 0xba, 0x70, 0xe1, 0x30, 0xb9, 0xbc, 0xd3);
// {6E9930EC-7409-4415-A110-5E4C29199CBE}
DEFINE_GUID(PID_File_StoneProp,			0x6e9930ec, 0x7409, 0x4415, 0xa1, 0x10, 0x5e, 0x4c, 0x29, 0x19, 0x9c, 0xbe);
// {8BE7B4FE-99FA-4052-8911-545828664433}
DEFINE_GUID(PID_File_AuxDataBox,		0x8be7b4fe, 0x99fa, 0x4052, 0x89, 0x11, 0x54, 0x58, 0x28, 0x66, 0x44, 0x33);

// 2010
// 有此属性表明文件有壳
DEFINE_GUID(PID_File_PackerName,		0x10e74674, 0x53a2, 0x454e, 0xb7, 0xbd, 0xac, 0x24, 0xbe, 0xd0, 0xac, 0xf8);
// VT_UI4 | VT_VECTOR: (for debug, enumrecord)
DEFINE_GUID(PID_File_VirusID_List,		0xe9864e3f, 0x4bb6, 0x488e, 0x99, 0xbc, 0xdc, 0xf2, 0xf2, 0x39, 0x29, 0xb3);
// VT_UI4:
DEFINE_GUID(PID_File_PECompilerID,		0xc79c5479, 0x6829, 0x4895, 0xb4, 0xcc, 0x99, 0x74, 0x59, 0xff, 0x85, 0x47);
// VT_UI4:
DEFINE_GUID(PID_File_PERvaOfMain,		0xc382d285, 0x4000, 0x4cab, 0x90, 0xe8, 0x99, 0xcb, 0x24, 0xf0, 0x27, 0xb5);
// VT_UI4 | VT_VECTOR:
DEFINE_GUID(PID_API_LIST,				0x3cd4776e, 0xb021, 0x483d, 0xa2, 0xed, 0x5e, 0xfe, 0xbc, 0x19, 0x1c, 0x9a);
// VT_UI4:
DEFINE_GUID(PID_TROJANLIB_WHITELIST,	0x430c7a7e, 0xe85c, 0x454f, 0x9f, 0xa6, 0x6b, 0x8c, 0x7e, 0x41, 0xa9, 0x60);
// {579E5FF6-F058-405f-A6B0-878DE564A142} VT_UI4
DEFINE_GUID(PID_File_ELFScanOrder,		0x579e5ff6, 0xf058, 0x405f, 0xa6, 0xb0, 0x87, 0x8d, 0xe5, 0x64, 0xa1, 0x42);
// {374BFF61-B740-41cc-AF21-D1A9706997F2}
DEFINE_GUID(PID_File_IsUnpacked,		0x374bff61, 0xb740, 0x41cc, 0xaf, 0x21, 0xd1, 0xa9, 0x70, 0x69, 0x97, 0xf2);
// {C49BB713-D5D0-4210-BD6E-85927AF3A7FF}
DEFINE_GUID(PID_File_PEEngine_HasScanned, 0xc49bb713, 0xd5d0, 0x4210, 0xbd, 0x6e, 0x85, 0x92, 0x7a, 0xf3, 0xa7, 0xff);

// {51051F1D-6F91-4577-9570-3750B35A08A7}
// 有此属性表明文件经过了壳判定
DEFINE_GUID(PID_File_PackerJudeged,		0x51051f1d, 0x6f91, 0x4577, 0x95, 0x70, 0x37, 0x50, 0xb3, 0x5a, 0x8, 0xa7);

// {46F564B6-82FB-42D3-A025-D809449A4D54}
DEFINE_GUID(PID_File_HasKnownPacker,	0x46f564b6, 0x82fb, 0x42d3, 0xa0, 0x25, 0xd8, 0x9, 0x44, 0x9a, 0x4d, 0x54);

// RXMAIL
DEFINE_GUID(PID_File_MailFile_SubType,	0xb2e6620d, 0x29dc, 0x4114, 0x8b, 0xf5, 0xc6, 0xad, 0xd7, 0x3c, 0x45, 0xae);
//Subfile type
// {8b480fcb-cc41-42f2-85f7-5f2325231d28} 
DEFINE_GUID(PID_File_SubDataFile,		0x8b480fcb, 0xcc41, 0x42f2, 0x85, 0xf7, 0x5f, 0x23, 0x25, 0x23, 0x1d, 0x28);

//////////////////////////////////////////////////////////////////////////

// {2925A0B8-9744-4F5B-B0AC-AED7AD8CBA77}
DEFINE_GUID(PID_File_ContentBlock,		0x2925a0b8, 0x9744, 0x4f5b, 0xb0, 0xac, 0xae, 0xd7, 0xad, 0x8c, 0xba, 0x77);
// {1596AEF1-87A9-4a67-A1BB-1B10A9B9F431}
// VT_UI8
DEFINE_GUID(PID_File_PETrojanCRC,		0x1596aef1, 0x87a9, 0x4a67, 0xa1, 0xbb, 0x1b, 0x10, 0xa9, 0xb9, 0xf4, 0x31);

// {0BFC51E9-952E-4CD1-BFF1-B063347E9A2F}
DEFINE_GUID(PID_File_HasKnownCompiler,	0xbfc51e9, 0x952e, 0x4cd1, 0xbf, 0xf1, 0xb0, 0x63, 0x34, 0x7e, 0x9a, 0x2f);

// {0BFC51E9-952E-4CD1-BFF1-B063347E9A2F}
DEFINE_GUID(PID_File_HasKnownAppType,	0xbfc51e9, 0x952e, 0x4cd1, 0xbf, 0xf1, 0xb0, 0x63, 0x34, 0x7e, 0x9a, 0x2f);

// {B0A8670C-3F9A-4FFE-9353-E9E5FD968E83}
DEFINE_GUID(PID_File_NeedSubmit,		0xb0a8670c, 0x3f9a, 0x4ffe, 0x93, 0x53, 0xe9, 0xe5, 0xfd, 0x96, 0x8e, 0x83);

// {3DBC8E90-078F-4E90-AD1C-A9995EC5A01A}
DEFINE_GUID(PID_File_Conscan_Info2Submit,0x3dbc8e90, 0x78f, 0x4e90, 0xad, 0x1c, 0xa9, 0x99, 0x5e, 0xc5, 0xa0, 0x1a);

// {923EDDDE-D5FF-4381-8F5A-F194ACF3FF3F}
DEFINE_GUID(PID_File_RenameName,	0x923eddde, 0xd5ff, 0x4381, 0x8f, 0x5a, 0xf1, 0x94, 0xac, 0xf3, 0xff, 0x3f);


// {A976F19C-D314-4588-B9FF-1BBF3887C798}
DEFINE_GUID( PID_FILE_PETOOLS, 0xa976f19c, 0xd314, 0x4588, 0xb9, 0xff, 0x1b, 0xbf, 0x38, 0x87, 0xc7, 0x98);
// {25210CD1-E8EC-4F71-B272-98E6FE6FAFF4}
DEFINE_GUID( PID_FILE_PETRUNK, 0x25210cd1, 0xe8ec, 0x4f71, 0xb2, 0x72, 0x98, 0xe6, 0xfe, 0x6f, 0xaf, 0xf4);


// {0BC3677E-A00E-47E0-84F6-9DB426793405}
DEFINE_GUID(PID_FILE_FLAG_ScanByCav, 0xbc3677e, 0xa00e, 0x47e0, 0x84, 0xf6, 0x9d, 0xb4, 0x26, 0x79, 0x34, 0x5);

// {303033F7-3246-4EC9-8E0C-4F6784480566}
DEFINE_GUID(PID_FILE_FLAG_SFX,		0x303033f7, 0x3246, 0x4ec9, 0x8e, 0xc, 0x4f, 0x67, 0x84, 0x48, 0x5, 0x66);

// {BF4BEF76-28BB-4311-BFD9-D813A552B8E3}
DEFINE_GUID(PID_FILE_FLAG_CLEAN,	0xbf4bef76, 0x28bb, 0x4311, 0xbf, 0xd9, 0xd8, 0x13, 0xa5, 0x52, 0xb8, 0xe3);

// {11267947-9B56-47CB-AECE-FEE17A8C8C90}
DEFINE_GUID(PID_FILE_FLAG_HASSCAN,	0x11267947, 0x9b56, 0x47cb, 0xae, 0xce, 0xfe, 0xe1, 0x7a, 0x8c, 0x8c, 0x90);


// {6B58F2D7-F006-4F6C-9AE0-E29A14ADB41D}
DEFINE_GUID(PID_FILE_WORDXML03_FLAG,		0x6b58f2d7, 0xf006, 0x4f6c, 0x9a, 0xe0, 0xe2, 0x9a, 0x14, 0xad, 0xb4, 0x1d);

// {C87DFFAD-C51F-4948-830D-26023401CAE5}
DEFINE_GUID(PID_DEL_FILE_NAMES,			0xc87dffad, 0xc51f, 0x4948, 0x83, 0xd, 0x26, 0x2, 0x34, 0x1, 0xca, 0xe5);

// {A84D6B4B-3178-4542-9893-DCBA331C8132}
DEFINE_GUID(PID_PHYSICAL_FILE_SIZE,		0xa84d6b4b, 0x3178, 0x4542, 0x98, 0x93, 0xdc, 0xba, 0x33, 0x1c, 0x81, 0x32);

// {0440A480-02DC-4DA5-9B6E-C6C05F082E0F}
DEFINE_GUID(PID_EMAIL_CHILD_FILE,		0x440a480, 0x2dc, 0x4da5, 0x9b, 0x6e, 0xc6, 0xc0, 0x5f, 0x8, 0x2e, 0xf);

//////////////////////////////////////////////////////////////////////////
//
//	STONE PROP
//

namespace ston
{
	static LPCSTR	kScope			= ":E";
	// dInt
	static LPCSTR	kVerVDB			= "!lv";
	// dInt
	static LPCSTR	kFFR			= "!ffr";	//	version of libffr

	static LPCSTR	kMark			= "!m";

	static LPCSTR	kLastScanTime	= "!t";		// UINT64

	static LPCSTR	kSig64			= "!s64";


	////////////////////////////////////////////////////
	// PE  Hash
	/*
		HASH_ARCHITECH = 0x01,
		HASH_ENTRY_INS8,
		HASH_ENTRY_INS16,
		HASH_ENTRY_INS32,
		HASH_FUN_IMPORTTABLE,
		HASH_FUN_EXPORTTABLE,
		HASH_DIR_IMPORTTABLE,
		HASH_DIR_EXPORTTABLE,
		HASH_DIR_SIGN,
		HASH_DIR_RESOURCE,
		HASH_OVERLAY_DATA,
	*/

	static LPCSTR  HashArchitect	="!Hash_Arch";  
	static LPCSTR  HashEntryIns8	="!HashEntryIns8"; 
	static LPCSTR  HashEntryIns16	="!HashEntryIns16"; 
	static LPCSTR  HashEntryIns32	="!HashEntryIns32"; 
	static LPCSTR  HashFunImport	="!HashFunImport"; 
	static LPCSTR  HashFunExport	="!HashFunExport"; 
	static LPCSTR  HashDirImport	="!HashDirImport"; 
	static LPCSTR  HashDirExport	="!HashDirExport"; 
	static LPCSTR  HashDirSign		="!HashDirSign"; 
	static LPCSTR  HashDirResource	="!HashDirResource"; 	
	static LPCSTR  HashOverlayData	="!HashOverlayData"; 	
	////////////////////////////////////////////////////
};
#endif
