#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_
#include "../util/utility_ex.h"
#include "../util/engutils.h"
#include "../util/filepath.h"
#include <ctime>
// #ifndef NOTIMPL
// #define NOTIMPL { return E_NOTIMPL; }
// #endif
#ifdef _WIN32
	#define PLATFORM_TYPE_WIN32
#endif
#ifndef ASSERT
#ifndef NDEBUG
	#if defined(PLATFORM_TYPE_WIN32)
		#define ASSERT(x) { if (!(x)) { DebugBreak(); } }
	#elif defined(PLATFORM_TYPE_NATIVE)
		#define ASSERT(x) { if (!(x)) { DbgBreakPoint(); } }
	#elif defined(PLATFORM_TYPE_POSIX)
		#define ASSERT(x)
	#else
		#error "platform not defined"
	#endif
#else
	#define ASSERT(x)
#endif //_DEBUG
#endif //ASSERT

#ifndef ASSERT0
#define ASSERT0 { int i; i = 0; ASSERT(i); }
#endif

#define ASSERTNOTIMPL { ASSERT0; return E_NOTIMPL; }
#ifndef RFAILED_
#define RFAILED_(x, r)		{ if ( FAILED(x) ) { return (r); } }
#endif



#include <vector>
#include <algorithm>

typedef UTIL::com_ptr<IREStream> CStreamPtr;
typedef UTIL::com_ptr<IREFile> CFilePtr;
typedef UTIL::com_ptr<IREProperty> CPropPtr;
typedef UTIL::com_ptr<IREFileSystem> CFileSystemPtr;
typedef UTIL::com_ptr<IREFileSystemEx> CFileSystemExPtr;

enum CTF_TYPE
{
	CTF_PHY,
	CTF_TMP,
	CTF_MEM,
	CTF_FRAG,
#if defined(PLATFORM_TYPE_POSIX)
	CTF_PHY_NTFS, // UNIX NTFS
#endif
};

#include <vector>

namespace hp
{
	template< class I >
	static HRESULT GetFromRuntime( UTIL::co<I>& ret, IRxRunTime * rt, REFGUID pid )
	{
		RASSERT( rt, E_INVALIDARG );
		ret.dispose();
		return rt->GetObjectEx( pid, re_uuidof(I), (void**)&ret.m_p );
	}
	template< class I >
	static HRESULT GetFromRuntime( UTIL::com_ptr<I>& ret, IRxRunTime * rt, REFGUID pid )
	{
		RASSERT( rt, E_INVALIDARG );
		ret.dispose();
		return rt->GetObjectEx( pid, re_uuidof(I), (void**)&ret.m_p );
	}
	template < class I >
	static HRESULT CreateInstance( UTIL::co<I>& ret, REFCLSID rclsid, IRxRunTime * rt = 0, IUnknown * outer = 0 )
	{
		UTIL::co<I> face;
		RFAILED( r4CreateInstance( rclsid, outer, re_uuidof(I), (void**)&face.m_p, rt ) );
		ret = face;
		return S_OK;
	}
	template < class I >
	static HRESULT CreateInstanceEx( UTIL::co<I>& ret, REFCLSID catelog, DWORD dwIndex, IRxRunTime * rt = 0, IUnknown * outer = 0 )
	{
		UTIL::co<I> face;
		RFAILED( r4CreateInstanceEx( catelog, dwIndex, outer, re_uuidof(I), (void**)&face.m_p, rt ) );
		ret = face;
		return S_OK;
	}

	template < class T >
	static HRESULT CreateGlobal( UTIL::co<T>& ret, IRxRunTime * rt, REFCLSID rclsid, REFCLSID rotpid )
	{
		RASSERT( rt, E_INVALIDARG );
		UTIL::co<T> face;
		if( FAILED( rt->GetObjectEx( rotpid, re_uuidof(T), (void**)&face.m_p ) ) )
		{
			RFAILED( rt->CreateInstance( rclsid, 0, re_uuidof(T), (void**)&face.m_p ) );
			RFAILED( rt->Register( rotpid, face ) );
		}
		ret = face;
		return S_OK;
	}

	//template < class I >
	//static HRESULT CreateIoChain( UTIL::co<I> & ret, IRxRunTime * rt, LPCLSID clsids, DWORD cc )
	//{
	//	if( !clsids || !cc ) return E_INVALIDARG;
	//	UTIL::co<IRxIoChainNode> top;
	//	RFAILED( rt->CreateInstance( clsids[0], 0, re_uuidof(IRxIoChainNode), (void**)&top.m_p ) );
	//	UTIL::co<IRxIoChainNode> pre_node = top;
	//	for( DWORD i = 1; i < cc; ++ i )
	//	{
	//		UTIL::co<IRxIoChainNode> node;
	//		RFAILED( rt->CreateInstance( clsids[i], 0, re_uuidof(IRxIoChainNode), (void**)&node.m_p ) );
	//		RFAILED( pre_node->SetNext( node ) );
	//		pre_node = node;
	//	}
	//	ret = top;
	//	if( !ret ) return E_NOINTERFACE;
	//	return S_OK;
	//}

	template< class CharType >
	static HRESULT CraeteFileT( IUnknown* punk, IRxRunTime * rt, BOOL bVirtual, CharType * filename, DWORD dwFlags )
	{
		UTIL::co<IREHandleFile> hfile( punk );
		if( !hfile ) return E_NOINTERFACE;
		UTIL::co<IREFileSystem> pfs;
		REFGUID pid = bVirtual ? PID_ROT_FileSystemVirtual : PID_ROT_FileSystem;
		RFAILED( rt->GetObjectEx( pid, re_uuidof(IREFileSystem), (void**)&pfs.m_p ) );
		return hfile->Open( pfs, CFilePath(filename), dwFlags );
	}

	template< class CharType >
	static HRESULT CraeteCachedFileT( UTIL::co<IREStream>& stm, IRxRunTime * rt, CharType * filename, DWORD dwFlags )
	{
		UTIL::co<IREStream> ostm;
		RFAILED( CreateInstance( ostm, rt, CLSID_CREHandleFileWithCC ) );
		// try open/create
		RFAILED( CraeteFileT( ostm.m_p, rt, FALSE, filename, dwFlags|OMMapping ) );
		stm = ostm;
		return S_OK;
	}


	static HRESULT CreateTempFile( IUnknown * punk, IRxRunTime * rt, BOOL bVirtual, DWORD dwFlags = OMWP|OMAutoDel )
	{
		return E_NOTIMPL;
//		UTIL::co<IREHandleFile> hfile( punk );
//		if( !hfile ) return E_NOINTERFACE;
//		UTIL::co<IREFileSystem> pfs;
//		REFGUID pid = bVirtual ? PID_ROT_FileSystemVirtual : PID_ROT_FileSystem;
//		RFAILED( rt->GetObjectEx( pid, re_uuidof(IREFileSystem), (void**)&pfs.m_p ) );
//#if defined(_WIN32)
//		enum { g_sep = L'\\' }; 
//#else
//		enum { g_sep = L'/' };
//#endif
//		UTIL::com_ptr<IREFileSystemEx> pfsex(pfs);
//		RASSERT(pfsex, E_INVALIDARG);
//		DWORD cch = 0;
//		// get temp path:
//		RFAILED(pfsex->GetSpecialFolderW(IREFileSystemEx::GSF_TEMP, NULL, &cch));
//		cch += 1 + sizeof(ULONGLONG)*2 + 1; // temp name
//		UTIL::sentry<WCHAR*, UTIL::default_array_sentry> lpszPath(RSNEW WCHAR[cch]) ;
//		RASSERT(lpszPath, E_OUTOFMEMORY);
//		RFAILED(pfsex->GetSpecialFolderW(IREFileSystemEx::GSF_TEMP, lpszPath, &cch));
//		lpszPath[cch] = 0;
//		HRESULT hr = pfsex->FolderExistsW(lpszPath);
//		if(FAILED(hr)) { hr = pfs->CreateDirectoryW(lpszPath, 0); }
//		if(FAILED(hr))
//		{
//			const char* tmpdir = "tmp";
//			WCHAR wtmpdir[MAX_PATH*2+1] = { 0 };
//			mbstowcs(wtmpdir,tmpdir,strlen(tmpdir));
//#ifdef PLATFORM_TYPE_WINDOWS
//			WCHAR modulename[MAX_PATH*2+1];
//			DWORD len = ::GetModuleFileNameW(NULL,modulename,sizeof(modulename));
//			while(len-- && '\\' != modulename[len]);
//
//			cch = len+1+sizeof(ULONGLONG)*2+1+1+wcslen(wtmpdir)*sizeof(WCHAR);
//			lpszPath = RSNEW WCHAR[cch];
//			RASSERT(lpszPath,E_OUTOFMEMORY);
//			memset(lpszPath,0,sizeof(WCHAR)*cch);
//			memcpy(lpszPath.m_p,modulename,len*sizeof(WCHAR));
//			if(g_sep != lpszPath[len-1]) lpszPath[len++] = g_sep;
//			memcpy(lpszPath.m_p+len,wtmpdir,wcslen(wtmpdir)*sizeof(WCHAR));
//			cch = len+wcslen(wtmpdir);
//#else
//			cch = (DWORD)(1+wcslen(wtmpdir)+1+sizeof(ULONGLONG)*2+1);
//			lpszPath = RSNEW WCHAR[cch];
//			RASSERT(lpszPath,E_OUTOFMEMORY);
//			memset(lpszPath.m_p,0,sizeof(WCHAR)*cch);
//			lpszPath[0] = g_sep;
//			memcpy(lpszPath.m_p+1,wtmpdir,wcslen(wtmpdir)*sizeof(WCHAR));
//			cch = 1+(DWORD)(wcslen(wtmpdir));
//#endif
//			hr = pfsex->FolderExistsW(lpszPath);
//			if(FAILED(hr)) hr = pfs->CreateDirectoryW(lpszPath,0);
//		}
//		RASSERT(SUCCEEDED(hr),hr);
//		if(g_sep !=  lpszPath[cch -1]) { lpszPath[cch++] = g_sep; }
//		// append random filename:
//		LARGE_INTEGER tsc;
//		RS_RDTSC(tsc);
//		I2W(tsc.QuadPart, &lpszPath[cch]);
//		// create object
//
//		return hfile->Open( pfs, CConstUnionName(lpszPath.m_p), dwFlags );
	}

};


class CFileAccessor
{
	IREStream *m_pstm;
public:
	CFileAccessor(IREStream *p) : m_pstm(p) {}
	operator IREStream* () const { return m_pstm; }
	IREStream* operator -> ()  { return m_pstm; }

	//HRESULT RefFormats( UTIL::co<IR4FormatList> & fmt, IRERunningObjectTable * rot = 0 )
	//{
	//	UTIL::co<IREProperty> prop( m_pstm );
	//	if( !prop ) return E_NOINTERFACE;

	//	const PROPVARIANT * pvar = 0;
	//	if( SUCCEEDED( prop->GetProperty( PID_FMT_FORMATS, &pvar ) ) && 
	//		pvar->vt == VT_UNKNOWN && pvar->punkVal )
	//	{
	//		fmt = (pvar->punkVal);
	//	}
	//	if( fmt ) return S_OK;
	//	if( !rot ) return E_FAIL;
	//	return InitFormats(fmt, rot);
	//}
	//HRESULT InitFormats( UTIL::co<IR4FormatList> & fmt, IRERunningObjectTable * rot )
	//{
	//	if( !rot ) return E_INVALIDARG;
	//	UTIL::co<IREProperty> prop( m_pstm );
	//	if( !prop ) return E_NOINTERFACE;
	//	UTIL::co<IR4FormatList> fmts;
	//	RFAILED( hp::CreateInstance( fmts, rot, CLSID_CRxFormats ) );
	//	PROPVARIANT var; var.vt = VT_UNKNOWN; var.punkVal = fmts.m_p;
	//	RFAILED( prop->SetProperty( PID_FMT_FORMATS, &var ) );
	//	fmt = fmts;
	//	return S_OK;
	//}
	//HRESULT RefStoneProps( UTIL::co<IStonePropOfStream> & stone, IRERunningObjectTable * rot = 0 )
	//{
	//	UTIL::co<IREProperty> prop( m_pstm );
	//	if( !prop ) return E_NOINTERFACE;

	//	const PROPVARIANT * pvar = 0;
	//	if( SUCCEEDED( prop->GetProperty( PID_File_EngineStoneProps, &pvar ) ) && 
	//		pvar->vt == VT_UNKNOWN && pvar->punkVal )
	//	{
	//		stone = (pvar->punkVal);
	//	}
	//	if( stone ) return S_OK;
	//	if( !rot ) return E_NOINTERFACE;
	//	return InitStoneProps(stone, rot);
	//}
	//HRESULT InitStoneProps( UTIL::co<IStonePropOfStream> & stone, IRERunningObjectTable * rot )
	//{
	//	if( !rot ) return E_INVALIDARG;
	//	UTIL::co<IREProperty> prop( m_pstm );
	//	if( !prop ) return E_NOINTERFACE;
	//	// create
	//	UTIL::co<IStonePropOfStream> sprop;
	//	RFAILED( hp::CreateInstance( sprop, rot, CLSID_CRxStonePropOfStream ) );
	//	// set property
	//	PROPVARIANT var; var.vt = VT_UNKNOWN; var.punkVal = sprop.m_p;
	//	RFAILED( prop->SetProperty( PID_File_EngineStoneProps, &var ) );
	//	// try open
	//	RFAILED( sprop->Open( m_pstm, AID_ENGINE_STONE_PROPS, FLG_AUTO_COMMIT_ON_CLOSE ) );
	//	// success
	//	stone = sprop;
	//	return S_OK;
	//}

	//HRESULT SetCandidate( IRxIoChainNode * cand )
	//{
	//	RASSERT( cand, E_INVALIDARG );
	//	UTIL::co<IREProperty> prop( m_pstm );
	//	if( !prop ) return E_FAIL;
	//	PROPVARIANT var; var.vt = VT_UNKNOWN; var.punkVal = cand;
	//	return prop->SetProperty( PID_FILE_STREAM_CANDIDATE, &var );
	//}

	//////////////////////////////////////////////////////////////////////////
	// IREStream wrapper:
	HRESULT Read(LPVOID pBuffer, DWORD dwToRead, DWORD & dwRead)
	{
		ASSERT(m_pstm);
		RFAILED(m_pstm->Read(pBuffer, dwToRead, &dwRead));
		return S_OK;
	}
	HRESULT Write(LPCVOID lpBuffer, DWORD dwToWrite, DWORD & dwWritten)
	{
		ASSERT(m_pstm);
		RFAILED(m_pstm->Write(lpBuffer, dwToWrite, &dwWritten));
		return S_OK;
	}
	HRESULT Seek(LONG lOffset, DWORD dwMethod = FILE_BEGIN)
	{
		ASSERT(m_pstm);
		RFAILED(m_pstm->Seek(lOffset, NULL, dwMethod));
		return S_OK;
	}
	HRESULT Tell(DWORD &dwOffset)
	{
		ASSERT(m_pstm);
		RFAILED(m_pstm->Tell(&dwOffset, NULL));
		return S_OK;
	}
	HRESULT GetSize(DWORD & dwSize)
	{
		ASSERT(m_pstm);
		RFAILED(m_pstm->GetSize(&dwSize, NULL));
		return S_OK;
	}
	HRESULT SetSize(DWORD dwSize)
	{
		ASSERT(m_pstm);
		RFAILED(m_pstm->SetSize(dwSize, NULL));
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// IREFile
	HRESULT Close()
	{
		CFilePtr file = m_pstm;
		ASSERT(file);
		RFAILED(file->Close());
		return S_OK;
	}
	HRESULT Remove()
	{
		CFilePtr file = m_pstm;
		ASSERT(file);
		RFAILED(file->Remove());
		return S_OK;
	}
	HRESULT ReOpen(DWORD flags)
	{
		CFilePtr file = m_pstm;
		ASSERT(file);
		RFAILED(file->ReOpen(flags));
		return S_OK;
	}
	HRESULT Rename( LPCSTR aName )
	{
		CFilePtr file = m_pstm;
		RASSERT(file,E_NOINTERFACE);
		return file->Rename(CConstUnionName(aName));
	}
	HRESULT Rename(LPCWSTR wName )
	{
		CFilePtr file = m_pstm;
		RASSERT(file,E_NOINTERFACE);
		return file->Rename(CConstUnionName(wName));
	}
	HRESULT Rename( const UnionNameRO & name)
	{
		CFilePtr file = m_pstm;
		RASSERT(file,E_NOINTERFACE);
		RFAILED(file->Rename(name));
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// 2004
	DWORD Read(LPVOID lpBuffer, DWORD dwToRead)
	{
		ASSERT(m_pstm);
		DWORD dwRead = 0;
		RFAILED_(m_pstm->Read(lpBuffer, dwToRead, &dwRead), 0);
		return dwRead;
	}
	DWORD Write(LPCVOID lpBuffer, DWORD dwToWrite)
	{
		ASSERT(m_pstm);
		DWORD dwWritten = 0;
		RFAILED_(m_pstm->Write(lpBuffer, dwToWrite, &dwWritten), 0);
		return dwWritten;
	}
	DWORD ReadPos(LPVOID lpBuffer, LONG lOffset, DWORD dwToRead)
	{
		ASSERT(m_pstm);
		RFAILED_(m_pstm->Seek(lOffset, NULL, FILE_BEGIN), 0);
		DWORD dwRead;
		RFAILED_(m_pstm->Read(lpBuffer, dwToRead, &dwRead), 0);
		return dwRead;
	}
	DWORD ReadPosHoldPointer(LPVOID lpBuffer, LONG lOffset, DWORD dwToRead)
	{
		DWORD dwPos;
		RFAILED_(m_pstm->Tell(&dwPos, NULL), 0);
		DWORD dwRead = ReadPos(lpBuffer, lOffset, dwToRead);
		RFAILED_(m_pstm->Seek((LONG)dwPos, NULL, FILE_BEGIN), 0);
		return dwRead;
	}
	DWORD WritePos(LPCVOID lpBuffer, LONG lOffset, DWORD dwToWrite)
	{
		ASSERT(m_pstm);
		RFAILED_(m_pstm->Seek(lOffset, NULL, FILE_BEGIN), 0);
		DWORD dwWritten;
		RFAILED_(m_pstm->Write(lpBuffer, dwToWrite, &dwWritten), 0);
		return dwWritten;
	}
	DWORD WritePosHoldPointer(LPCVOID lpBuffer, LONG lOffset, DWORD dwToWrite)
	{
		DWORD dwPos;
		RFAILED_(m_pstm->Tell(&dwPos, NULL), 0);
		DWORD dwWritten = WritePos(lpBuffer, lOffset, dwToWrite);
		RFAILED_(m_pstm->Seek((LONG)dwPos, NULL, FILE_BEGIN), 0);
		return dwWritten;
	}

	DWORD GetSize()
	{
		ASSERT(m_pstm);
		DWORD dwSizeLow;
		RFAILED_(m_pstm->GetSize(&dwSizeLow, NULL), INVALID_FILE_SIZE);
		return dwSizeLow;
	}
	BOOL IsEOF()
	{
		ASSERT(m_pstm);
		DWORD dwSizeLow, dwSizeHigh;
		RFAILED_(m_pstm->GetSize(&dwSizeLow, &dwSizeHigh), FALSE);
		DWORD dwOffsetLow, dwOffsetHigh;
		RFAILED_(m_pstm->Tell(&dwOffsetLow, &dwOffsetHigh), FALSE);
		return ((dwSizeHigh == dwOffsetHigh) && (dwSizeLow == dwOffsetLow));
	}
	//////////////////////////////////////////////////////////////////////////
	// assistance:
	enum
	{
		FAA_BUFFER_SIZE = 0x1000,
	};
	// 拷贝文件块，覆盖目标区域
	HRESULT CopyFileBlock( DWORD dwOffsetDest, DWORD dwOffsetSrc, DWORD dwLength )
	{
		if( dwOffsetDest == dwOffsetSrc ) return S_OK ;
		LONG lDelta ;
		BYTE buf[FAA_BUFFER_SIZE] ;
		if(dwOffsetDest <= dwOffsetSrc || dwOffsetDest >= dwOffsetSrc +dwLength)
		{
			lDelta = FAA_BUFFER_SIZE;
		}
		else
		{
			dwOffsetSrc += dwLength -FAA_BUFFER_SIZE;
			dwOffsetDest += dwLength -FAA_BUFFER_SIZE;
			lDelta = -FAA_BUFFER_SIZE;
		}
		while( (int)(dwLength -= FAA_BUFFER_SIZE) >= 0 )
		{
			//Src:
			if(ReadPos(buf, dwOffsetSrc, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE)
			{
				return E_UNEXPECTED ;
			}
			//Dest:
			if(WritePos(buf, dwOffsetDest, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE)
			{
				return E_UNEXPECTED ;
			}
			dwOffsetSrc  += lDelta;
			dwOffsetDest += lDelta;
		}
		if( dwLength += FAA_BUFFER_SIZE )
		{
			if( lDelta < 0 )
			{
				dwOffsetSrc  += FAA_BUFFER_SIZE -dwLength ;
				dwOffsetDest += FAA_BUFFER_SIZE -dwLength ;
			}
			//Src:
			if( ReadPos(buf, dwOffsetSrc, dwLength) != dwLength )
			{
				return E_UNEXPECTED ;
			}
			//Dest:
			if( WritePos(buf, dwOffsetDest, dwLength) != dwLength )
			{
				return E_UNEXPECTED ;
			}
		}
		return S_OK ;
	}
	// 交换文件块，源、目的文件区域不能重叠
	HRESULT SwapFileBlock( DWORD dwOffsetDest, DWORD dwOffsetSrc, DWORD dwLength )
	{
		if( 0 == dwLength ) return E_UNEXPECTED ;
		if( ((dwOffsetDest>=dwOffsetSrc) && (dwOffsetDest<(dwOffsetSrc+dwLength))) ||
			((dwOffsetSrc>=dwOffsetDest) && (dwOffsetSrc<(dwOffsetDest+dwLength))) )
		{ return E_UNEXPECTED ; }
		BYTE buf_src[FAA_BUFFER_SIZE], buf_dest[FAA_BUFFER_SIZE] ;
		while( (int)(dwLength -= FAA_BUFFER_SIZE) >= 0 )
		{
			//Src Read:
			if( ReadPos(buf_src, dwOffsetSrc, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
			{
				return E_UNEXPECTED ;
			}
			//Dest Read:
			if( ReadPos(buf_dest, dwOffsetDest, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
			{
				return E_UNEXPECTED ;
			}
			//Src Write:
			if( WritePos(buf_dest, dwOffsetSrc, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
			{
				return E_UNEXPECTED ;
			}
			//Dest Write:
			if( WritePos(buf_src, dwOffsetDest, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
			{
				return E_UNEXPECTED ;
			}
			dwOffsetSrc  += FAA_BUFFER_SIZE ;
			dwOffsetDest += FAA_BUFFER_SIZE ;
		}
		if( dwLength += FAA_BUFFER_SIZE )
		{
			//Src Read:
			if( ReadPos(buf_src, dwOffsetSrc, dwLength) != dwLength )
			{
				return E_UNEXPECTED ;
			}
			//Dest Read:
			if( ReadPos(buf_dest, dwOffsetDest, dwLength) != dwLength )
			{
				return E_UNEXPECTED ;
			}
			//Src Write:
			if( WritePos(buf_dest, dwOffsetSrc, dwLength) != dwLength )
			{
				return E_UNEXPECTED ;
			}
			//Dest Write:
			if( WritePos(buf_src, dwOffsetDest, dwLength) != dwLength )
			{
				return E_UNEXPECTED ;
			}
		}
		return S_OK ;
	}
	// 插入文件块
	HRESULT InsertFileBlock( DWORD dwOffsetDest, DWORD dwOffsetSrc, DWORD dwLength )
	{
		if( 0 == dwLength ) return E_UNEXPECTED ;
		DWORD dwFileSize ;
		if( 0xFFFFFFFF == (dwFileSize = GetSize()) ) return E_UNEXPECTED ;
		if( S_OK != m_pstm->SetSize( dwFileSize+dwLength, 0 ) ) return E_UNEXPECTED ;
		DWORD dwSrcCutLen = 0 ;
		if( (dwOffsetDest>dwOffsetSrc) && (dwOffsetDest<(dwOffsetSrc+dwLength)) )
		{// Src Block been cutted!
			dwSrcCutLen = dwOffsetDest-dwOffsetSrc ;
		}
		// Move File Block:
		if( S_OK != CopyFileBlock( (dwOffsetDest+dwLength), dwOffsetDest, (dwFileSize-dwOffsetDest) ) )
			return E_UNEXPECTED ;
		if( dwSrcCutLen > 0 )
		{
			if( S_OK != CopyFileBlock( dwOffsetDest, dwOffsetSrc, dwSrcCutLen ) )
				return E_UNEXPECTED ;
		}
		return CopyFileBlock( (dwOffsetDest+dwSrcCutLen), 
			(dwOffsetSrc+dwLength+dwSrcCutLen), (dwLength-dwSrcCutLen) ) ;
	}
	// 删除文件块
	HRESULT DeleteFileBlock( DWORD dwOffsetDest, DWORD dwLength )
	{
		DWORD dwFileSize = GetSize() ;
		if( dwOffsetDest>dwFileSize ) { return E_UNEXPECTED ; }
		if( (dwOffsetDest+dwLength)>dwFileSize )
		{
			dwLength = (dwFileSize-dwOffsetDest) ;
		}
		if( (dwOffsetDest+dwLength)<dwFileSize )
		{
			if( S_OK != CopyFileBlock( dwOffsetDest, (dwOffsetDest+dwLength), (dwFileSize-dwOffsetDest-dwLength) ) )
				return E_UNEXPECTED ;
		}
		return m_pstm->SetSize( dwFileSize-dwLength, 0 ) ;
	}
	// 插入数据块到文件
	HRESULT InsertDataBlock( DWORD dwOffsetDest, DWORD dwLength, void *pvBuffer /*=NULL*/ )
	{
		if( !dwLength ) return S_OK ;
		DWORD dwFileSize = GetSize() ;
		if( dwOffsetDest > dwFileSize ) return E_UNEXPECTED ;
		if( S_OK != m_pstm->SetSize( dwFileSize+dwLength, 0 ) ) return E_UNEXPECTED ;
		if( dwOffsetDest<dwFileSize )
		{
			if( S_OK != CopyFileBlock((dwOffsetDest+dwLength),dwOffsetDest,(dwFileSize-dwOffsetDest)) )
				return E_UNEXPECTED ;
		}
		if( pvBuffer )
		{
			return WritePos( pvBuffer, dwOffsetDest, dwLength ) ;
		}
		return S_OK ;
	}
	// 填入字节块
	template<class T>
	HRESULT FillDataBlock( DWORD dwOffsetDest, const T& val, DWORD count )
	{
		DWORD delta = FAA_BUFFER_SIZE > sizeof(T) ? FAA_BUFFER_SIZE/sizeof(T) : 1;
		std::vector<T> buf(delta, val);
		while( (int)(count -= delta) >= 0 )
		{
			if( sizeof(T)*delta != WritePos( &buf[0], dwOffsetDest, sizeof(T)*delta ) )
				return E_UNEXPECTED ;
			dwOffsetDest += sizeof(T)*delta;
		}
		if( count += delta )
		{
			if( sizeof(T)*count != WritePos( &buf[0], dwOffsetDest, sizeof(T)*count ) )
				return E_UNEXPECTED ;
		}
		return S_OK ;
	}
	// 从另一个文件更新
	static 
	HRESULT FileToFile(IREStream *deststm, IREStream *srcstm, DWORD dwOffsetDest, DWORD dwOffsetSrc, DWORD dwSize)
	{
		RFAILED(deststm->Seek(dwOffsetDest, NULL, FILE_BEGIN));
		RFAILED(srcstm->Seek(dwOffsetSrc, NULL, FILE_BEGIN));
		BYTE buf[FAA_BUFFER_SIZE];
		while(dwSize>0)
		{
			DWORD dwRead=dwSize>FAA_BUFFER_SIZE?FAA_BUFFER_SIZE:dwSize, dwReaded;
			RFAILED(srcstm->Read(buf, dwRead, &dwReaded));
			if (0 == dwReaded) return E_FAIL;
			RFAILED(deststm->Write(buf, dwReaded, NULL));
			dwSize -= dwReaded;
		}
		return S_OK;
	}
	HRESULT FillPosSizeFromFile(DWORD dwOffsetDest, DWORD dwSize, IREStream *srcstm)
	{
		return FileToFile(m_pstm, srcstm, dwOffsetDest, 0, dwSize);
	}
	//////////////////////////////////////////////////////////////////////////
	//file stream
	static 
	HRESULT CopyFile(IN IREStream* pstmSrc, IN OUT IREStream* pstmDest)
	{
		DWORD dwSize;
		RFAILED(pstmSrc->GetSize(&dwSize, 0));
		RFAILED(pstmDest->SetSize(dwSize, 0));
		RFAILED(CFileAccessor::FileToFile(pstmDest, pstmSrc, 0, 0, dwSize));
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// property shortcut:

	//static
	//	WORD GetCurrentType(IREArchive *parc)
	//{
	//	UTIL::com_ptr<IREStream> pstm;
	//	RASSERT(parc && SUCCEEDED(parc->GetStreamObject((IREStream**)&pstm)), REFFR::FMT_UNDETECT);
	//	return CFileAccessor(pstm).GetCurrentType();
	//}
	//WORD GetCurrentType()
	//{
	//	UTIL::com_ptr<IREProperty> pprop(m_pstm);
	//	RASSERT(pprop, REFFR::FMT_UNDETECT);
	//	return GetCurrentType(pprop);
	//}
	//---
	static
		void MarkUpdate(IREProperty *ppropfile, bool bUpdate)
	{
		//PROPVARIANT var;
		//ppropfile->SetProperty(PID_File_UpdateMark, &(var.vt=VT_UI4, var.ulVal=bUpdate, var));
	}
	static
		void MarkUpdate(IREArchive *parc, bool bUpdate)
	{
		RASSERTV(parc);
		UTIL::com_ptr<IREStream> pstm;
		RFAILEDV(parc->GetStreamObject((IREStream**)&pstm));
		CFileAccessor(pstm).MarkUpdate(bUpdate);
	}
	void MarkUpdate(bool bUpdate)
	{
		UTIL::com_ptr<IREProperty> pprop(m_pstm);
		RASSERTV(pprop);
		MarkUpdate(pprop, bUpdate);
	}
	//---
	static
		HRESULT NotifyParentUpdate(IREProperty *ppropfile)
	{
		const PROPVARIANT *pvar;
		RASSERT(SUCCEEDED(ppropfile->GetProperty(PID_File_ParentArchive, &pvar)) && pvar->punkVal, S_FALSE);
		UTIL::com_ptr<IREArchive> parc(pvar->punkVal);
		RASSERT(parc, E_UNEXPECTED);
		UTIL::com_ptr<IREStream> pstm;
		RFAILED(parc->GetStreamObject((IREStream**)&pstm));
		UTIL::com_ptr<IREProperty> pprop(pstm);
		RASSERT(pprop, E_UNEXPECTED);
		PROPVARIANT var;
		var.ulVal = SUCCEEDED(pprop->GetProperty(PID_File_SubItemChanged, &pvar)) ? pvar->ulVal+1 : 1;
		return pprop->SetProperty(PID_File_SubItemChanged, &(var.vt=VT_UI4, var));
	}
	static
		HRESULT NotifyParentUpdate(IREArchive *parc)
	{
		RASSERT(parc, E_INVALIDARG);
		UTIL::com_ptr<IREStream> pstm;
		RFAILED(parc->GetStreamObject((IREStream**)&pstm));
		return CFileAccessor(pstm).NotifyParentUpdate();
	}
	HRESULT NotifyParentUpdate()
	{
		UTIL::com_ptr<IREProperty> pprop(m_pstm);
		RASSERT(pprop, E_UNEXPECTED);
		return NotifyParentUpdate(pprop);
	}
	//---
	static
		bool IsSubItemChanged(IREProperty *ppropfile)
	{
		const PROPVARIANT *pvar;
		return SUCCEEDED(ppropfile->GetProperty(PID_File_SubItemChanged, &pvar)) && (int)pvar->ulVal > 0;
	}
	static
		bool IsSubItemChanged(IREArchive *parc)
	{
		RASSERT(parc, false);
		UTIL::com_ptr<IREStream> pstm;
		RASSERT(SUCCEEDED(parc->GetStreamObject((IREStream**)&pstm)), false);
		return CFileAccessor(pstm).IsSubItemChanged();
	}
	bool IsSubItemChanged()
	{
		UTIL::com_ptr<IREProperty> pprop(m_pstm);
		RASSERT(pprop, false);
		return IsSubItemChanged(pprop);
	}
	//---
	static
	HRESULT GetParent(IREProperty *ppropfile, IREArchive **pparc)
	{
		const PROPVARIANT *pvar;
		RFAILED(ppropfile->GetProperty(PID_File_ParentArchive, &pvar));
		UTIL::com_ptr<IREArchive> parc(pvar->punkVal);
		RASSERT(parc, E_UNEXPECTED);
		*pparc = parc.detach();
		return S_OK;
	}

	static
	HRESULT SetParent( IUnknown *punk, IREArchive *parc)
	{
		RASSERT( punk, E_INVALIDARG );
		UTIL::co<IREProperty> prop( punk );
		RASSERT( prop, E_NOINTERFACE );
		if( parc )
		{
			PROPVARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = parc;
			return prop->SetProperty(PID_File_ParentArchive, &var);
		}
		return prop->DeleteProperty(PID_File_ParentArchive );
	}

	HRESULT SetParent( IREArchive *parc)
	{
		return SetParent(m_pstm, parc );
	}

	static
	HRESULT GetParent(IREArchive *parc, IREArchive **pparc)
	{
		UTIL::com_ptr<IREStream> pstm;
		RFAILED(parc->GetStreamObject((IREStream**)&pstm));
		return CFileAccessor(pstm).GetParent(pparc);
	}

	HRESULT GetParent(IREArchive **pparc)
	{
		UTIL::com_ptr<IREProperty> pprop(m_pstm);
		RASSERT(pprop, E_UNEXPECTED);
		return GetParent(pprop, pparc);
	}

	//---
	static
	HRESULT GetParentStream(IREProperty *ppropfile, IREStream **ppstm)
	{
		UTIL::com_ptr<IREArchive> parc;
		RFAILED(GetParent(ppropfile, &parc.m_p));
		return parc->GetStreamObject(ppstm);
	}
	static
	HRESULT GetParentStream(IREArchive *parc, IREStream **ppstm)
	{
		UTIL::com_ptr<IREStream> pstm;
		RFAILED(parc->GetStreamObject((IREStream**)&pstm));
		return CFileAccessor(pstm).GetParentStream(ppstm);
	}
	HRESULT GetParentStream(IREStream **ppstm)
	{
		UTIL::com_ptr<IREProperty> pprop(m_pstm);
		RASSERT(pprop, E_UNEXPECTED);
		return GetParentStream(pprop, ppstm);
	}
	static long GetRootStream( IREStream * pstm, IREStream ** ppstm )
	{
		if( !pstm || !ppstm ) return -1;
		long lev = 0;
		UTIL::com_ptr<IREStream> othis = pstm;
		for( ;; )
		{
			UTIL::com_ptr<IREStream> oparent;
			if( FAILED( CFileAccessor(othis).GetParentStream( &oparent.m_p ) ) )
				break;
			lev ++;
			othis = oparent;
		}
		ppstm[0] = othis.detach();
		return lev;
	}
	static long GetRootStream( IREArchive * parc, IREStream ** ppstm )
	{
		if( !parc || !ppstm ) return -1;
		UTIL::com_ptr<IREStream> file;
		RFAILED_( parc->GetStreamObject( &file.m_p ), -1 );
		return GetRootStream( file, ppstm );
	}

	//////////////////////////////////////////////////////////////////////////
	// creation:

	template<class CharType> static
		HRESULT CreateHandleFile( IRxRunTime *runtime, IREFileSystem *pfs, const CharType *filename,  DWORD flags, IREStream **ppstm )
	{
		using namespace UTIL;
		com_ptr<IREHandleFile> pfile;
		RFAILED(runtime->CreateInstance(CLSID_CREHandleFile, 0, re_uuidof(IREHandleFile), (void**)&pfile));
		RFAILED(pfile->Open(pfs, CConstUnionName(filename), flags));
		return pfile->QueryInterface(re_uuidof(IREStream), (void**)ppstm);
	}
	//template<class CharType> static
	//	HRESULT CreateHandleFile( IRxRunTime *runtime, IREFileSystem *pfs,const CharType *filename, DWORD flags, IREStream **ppstm, CTF_TYPE type = CTF_PHY)
	//{
	//	return CreateHandleFile<CharType>( runtime, pfs, filename, flags, ppstm, type );
	//}
	template<class CharType> static
		HRESULT RealCreateHandleFile(IRxRunTime *runtime, const CharType *filename, DWORD flags,
		IREStream **ppstm )
	{
		UTIL::com_ptr<IUnknown> punk;
		RFAILED(runtime->GetObject(PID_ROT_FileSystem, (IUnknown**)&punk));
		UTIL::com_ptr<IREFileSystem> pfs(punk);
		RASSERT(pfs, E_INVALIDARG);
		return CreateHandleFile( runtime, pfs, filename, flags, ppstm );
	}
	template<class CharType> static
		HRESULT VirtualCreateHandleFile(IRxRunTime *runtime, const CharType *filename, DWORD flags,
		IREStream **ppstm )
	{
		UTIL::com_ptr<IUnknown> punk;
		RFAILED(runtime->GetObject(PID_ROT_FileSystemVirtual, &punk.m_p));
		UTIL::com_ptr<IREFileSystem> pfs(punk);
		RASSERT(pfs, E_INVALIDARG);
		return CreateHandleFile(runtime, pfs, filename, flags, ppstm );
	}
	static
		HRESULT CreateTempFile(IRxRunTime *runtime, IREFileSystem *pfs, IREStream **ppstm )
	{
		return E_NOTIMPL;
//#if defined(_WIN32)
//		enum { g_sep = L'\\' }; 
//#else
//		enum { g_sep = L'/' };
//#endif
//		UTIL::com_ptr<IREFileSystemEx> pfsex(pfs);
//		RASSERT(pfsex, E_INVALIDARG);
//		DWORD cch = 0;
//		// get temp path:
//		RFAILED(pfsex->GetSpecialFolderW(IREFileSystemEx::GSF_TEMP, NULL, &cch));
//		cch += 1 + sizeof(ULONGLONG)*2 + 1; // temp name
//		UTIL::sentry<WCHAR*, UTIL::default_array_sentry> lpszPath(RSNEW WCHAR[cch]) ;
//		RASSERT(lpszPath, E_OUTOFMEMORY);
//		RFAILED(pfsex->GetSpecialFolderW(IREFileSystemEx::GSF_TEMP, lpszPath, &cch));
//		lpszPath[cch] = 0;
//		HRESULT hr = pfsex->FolderExistsW(lpszPath);
//		if(FAILED(hr)) { hr = pfs->CreateDirectoryW(lpszPath, 0); }
//		if(FAILED(hr))
//		{
//			const char* tmpdir = "tmp";
//			WCHAR wtmpdir[MAX_PATH*2+1] = { 0 };
//			mbstowcs(wtmpdir,tmpdir,strlen(tmpdir));
//#ifdef PLATFORM_TYPE_WINDOWS
//			WCHAR modulename[MAX_PATH*2+1];
//			DWORD len = ::GetModuleFileNameW(NULL,modulename,sizeof(modulename));
//			while(len-- && '\\' != modulename[len]);
//			
//			cch = len+1+sizeof(ULONGLONG)*2+1+1+wcslen(wtmpdir)*sizeof(WCHAR);
//			lpszPath = RSNEW WCHAR[cch];
//			RASSERT(lpszPath,E_OUTOFMEMORY);
//			memset(lpszPath,0,sizeof(WCHAR)*cch);
//			memcpy(lpszPath.m_p,modulename,len*sizeof(WCHAR));
//			if(g_sep != lpszPath[len-1]) lpszPath[len++] = g_sep;
//			memcpy(lpszPath.m_p+len,wtmpdir,wcslen(wtmpdir)*sizeof(WCHAR));
//			cch = len+wcslen(wtmpdir);
//#else
//			cch = (DWORD)(1+wcslen(wtmpdir)+1+sizeof(ULONGLONG)*2+1);
//			lpszPath = RSNEW WCHAR[cch];
//			RASSERT(lpszPath,E_OUTOFMEMORY);
//			memset(lpszPath.m_p,0,sizeof(WCHAR)*cch);
//			lpszPath[0] = g_sep;
//			memcpy(lpszPath.m_p+1,wtmpdir,wcslen(wtmpdir)*sizeof(WCHAR));
//			cch = 1+(DWORD)(wcslen(wtmpdir));
//#endif
//			hr = pfsex->FolderExistsW(lpszPath);
//			if(FAILED(hr)) hr = pfs->CreateDirectoryW(lpszPath,0);
//		}
//		RASSERT(SUCCEEDED(hr),hr);
//		if(g_sep !=  lpszPath[cch -1]) { lpszPath[cch++] = g_sep; }
//		// append random filename:
//		LARGE_INTEGER tsc;
//		RS_RDTSC(tsc);
//		I2W(tsc.QuadPart, &lpszPath[cch]);
//		// create object
//		return CreateHandleFile(runtime, pfs, (WCHAR*)lpszPath, OMWP|OMAutoDel, ppstm );
	}

	static
		HRESULT RealCreateTempFile(IRxRunTime *runtime, IREStream **ppstm )
	{
		UTIL::com_ptr<IUnknown> punk;
		RFAILED(runtime->GetObject(PID_ROT_FileSystem, (IUnknown**)&punk));
		UTIL::com_ptr<IREFileSystem> pfs(punk);
		RASSERT(pfs, E_INVALIDARG);
		return CreateTempFile(runtime, pfs, ppstm );
	}
	static
		HRESULT VirtualCreateTempFile(IRxRunTime *runtime, IREStream **ppstm )
	{
		UTIL::com_ptr<IUnknown> punk;
		RFAILED(runtime->GetObject(PID_ROT_FileSystemVirtual, (IUnknown**)&punk));
		UTIL::com_ptr<IREFileSystem> pfs(punk);
		RASSERT(pfs, E_INVALIDARG);
		return CreateTempFile(runtime, pfs, ppstm );
	}
public:
	static HRESULT CreateTempFileSystem(IRxRunTime *prot, IREStream *pstrm, IREFileSystem **ppfs)
	{
		UTIL::com_ptr<IREFileLockBytes> pflb;
		RFAILED( r4CreateInstance(CLSID_CREFileLockBytes, NULL, re_uuidof(IREFileLockBytes), (void **)&pflb), prot ); 
		pflb->Init(pstrm);
		UTIL::com_ptr<IRESSFileSystem> pfs;
		RFAILED(r4CreateInstance( CLSID_CRESSFileSystem, NULL, re_uuidof(IRESSFileSystem), (void **)&pfs, prot) );
		RFAILED(pfs->Create(UTIL::com_ptr<IRELockBytes>(pflb), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ));
		return (*ppfs = UTIL::com_ptr<IREFileSystem>(pfs).detach()) ? S_OK : E_UNEXPECTED;
	}
	//static HRESULT CreateTempFileSystem( IRxRunTime *runtime, IREFileSystem **ppfs, DWORD dwPageSize, DWORD dwPageNum, BOOL bRegToROT = TRUE )
	//{
	//	CLSID clsids[] = { CLSID_CRECacheFileLRU, CLSID_CREHandleFile };
	//	UTIL::co<IRECacheFile> hfile;
	//	RFAILED( hp::CreateIoChain( hfile, runtime, clsids, 2 ) );
	//	UTIL::co<IREStream> stm(hfile);
	//	RASSERT( stm, E_FAIL );
	//	RFAILED( hfile->Init( dwPageSize, dwPageNum ) );
	//	RFAILED( hp::CreateTempFile( hfile, runtime, FALSE ) );
	//	UTIL::com_ptr<IREFileSystem> pfs;
	//	RFAILED(CreateTempFileSystem( runtime, stm, &pfs.m_p));
	//	if( bRegToROT ) RFAILED( runtime->Register( PID_ROT_FileSystemVirtual, pfs ) );
	//	*ppfs = pfs.detach();
	//	return S_OK;
	//}
	// file name
	static 
	HRESULT GetFileName(IN IREProperty *pprop, OUT CFilePath &filepath)
	{
		const PROPVARIANT *pv = NULL;
		if (FAILED(pprop->GetProperty(PID_File_DisplayName, &pv)) || !pv->pszVal)
		{
			RASSERT(SUCCEEDED(pprop->GetProperty(PID_File_FileName, &pv)) && pv->pszVal, E_UNEXPECTED);
		}
		filepath = *pv;
		return S_OK;
	}
	static
	HRESULT GetFileName(IN IREStream *stm, OUT CFilePath &filepath)
	{
		UTIL::com_ptr<IREProperty> pprop(stm);
		RASSERT(pprop, E_UNEXPECTED);
		return GetFileName(pprop, filepath);
	}
	HRESULT GetFileName(OUT CFilePath &filepath)
	{
		return GetFileName(m_pstm, filepath);
	}
	static HRESULT GetFileFullName(IN IREProperty* pprop,OUT CFilePath& fullpath)
	{
		UTIL::com_ptr<IREProperty> prop = pprop;
		CFilePath filepath;
		RFAILED(GetFileName(prop,filepath));
		RASSERT(filepath.ToAnsi(),E_UNEXPECTED);
		std::string fullname[2];
		size_t prev = 0,curr = 1;
		fullname[prev] = filepath.pAName;
		UTIL::com_ptr<IREStream> parent;
		while(SUCCEEDED(GetParentStream(prop,&parent.m_p)))
		{
			RASSERT(prop = parent,E_UNEXPECTED);
			RFAILED(GetFileName(prop,filepath));
			RASSERT(filepath.ToAnsi(),E_UNEXPECTED);
			fullname[curr] = filepath.pAName;
			fullname[curr] += ">>";
			fullname[curr] += fullname[prev];
			prev ^= curr;curr ^= prev;prev ^= curr;
			parent = (IUnknown*)(0);
		}
		fullpath = fullname[prev].c_str();
		return S_OK;
	}
	static HRESULT GetFileFullName(IN IREStream* stm,OUT CFilePath& filepath)
	{
		UTIL::com_ptr<IREProperty> pprop(stm);
		RASSERT(pprop, E_UNEXPECTED);
		return GetFileFullName(pprop,filepath);
	}
	HRESULT GetFileFullName(OUT CFilePath& filepath)
	{
		return GetFileFullName(m_pstm,filepath);
	}

	static HRESULT GetFileInternalName(IN IREProperty* pprop,OUT CFilePath& filepath)
	{
		CFilePath fullpath;
		RFAILED(GetFileFullName(pprop,fullpath));
		RASSERT(fullpath.ToAnsi(),E_UNEXPECTED);
		size_t len = fullpath.dwLen;
		const char* p = fullpath.pAName;filepath = p;
		for(size_t i = 0;i+1 < len;++i)
		{
			if(p[i] != '>' || p[i+1] != '>') continue;
			filepath = p + i + 2;break;
		}
		return S_OK;
	}
	static HRESULT GetFileInternalName(IN IREStream* stm,OUT CFilePath& filepath)
	{
		UTIL::com_ptr<IREProperty> pprop(stm);
		RASSERT(pprop, E_UNEXPECTED);
		return GetFileInternalName(pprop,filepath);
	}
	HRESULT GetFileInternalName(OUT CFilePath& filepath)
	{
		return GetFileInternalName(m_pstm,filepath);
	}


	//// file attributes
	//static
	//HRESULT GetFileAttributesExA(IN IREProperty *pprop, OUT WIN32_FILE_ATTRIBUTE_DATA *pfad)
	//{
	//	const PROPVARIANT *pvar;
	//	RFAILED(pprop->GetProperty(PID_File_FileSystem, &pvar));
	//	UTIL::com_ptr<IREFileSystem> pfs(pvar->punkVal);
	//	RASSERT(pfs, E_UNEXPECTED);
	//	RFAILED(pprop->GetProperty(PID_File_FileName, &pvar));
	//	return pfs->GetFileAttributesExA(CFilePath(*pvar), GetFileExInfoStandard, pfad);
	//}
	// file time
	static
	HRESULT FileTimeToTimeT(FILETIME &ft, time_t *tt)
	{
		ULONGLONG llft = ((ULONGLONG)ft.dwHighDateTime<<32) | ft.dwLowDateTime;
		*tt = (time_t)(llft/10000000 - 11644473600LL);	//second - datediff("s", "1601-1-1", "1970-1-1"); //filetime是1601-1-1, time_t是1970-1-1
		return S_OK;
	}
	static
	HRESULT TimeTToDosDateTime(time_t tt, WORD *pdate, WORD *ptime)
	{
		tm *ptm = gmtime(&tt);
		if (NULL == ptm) return E_UNEXPECTED;
		if (ptm->tm_sec == 59)			//59秒拥有特殊性. 因为后面的4舍5入就成60秒了. 系统就乱了
		{
			++tt;
			ptm = gmtime(&tt);
			if (NULL == ptm) return E_UNEXPECTED;
		}
		*ptime = (WORD)( ((ptm->tm_sec+1) / 2) |	//4舍5入
				(ptm->tm_min << 5) |
				(ptm->tm_hour << 11) );
		*pdate = (WORD)( (ptm->tm_mday) |
				((ptm->tm_mon+1)<<5) |		//msdos mon start from 1, tm start from 0
				((ptm->tm_year-80)<<9));		//msdos year start from 1980, tm start from 1900
		return S_OK;
	}
	static
	HRESULT TimeTToLocalTimeT(time_t tt, time_t *ltt)
	{
		time_t tt_tmp = tt;
		tm *ptm = gmtime(&tt_tmp);
		if (NULL == ptm) return E_UNEXPECTED;
		tm tmp_tm = *ptm;
		*ltt = mktime(&tmp_tm);
		*ltt = *ltt + 2*(tt_tmp - *ltt);
		return S_OK;
	}
	static
	HRESULT FileTimeToDosDateTime(FILETIME *ft, WORD *pdate, WORD *ptime)
	{
		time_t tt;
		RFAILED(FileTimeToTimeT(*ft, &tt));
		return TimeTToDosDateTime(tt, pdate, ptime);
	}
	static
	HRESULT FileTimeToLocalDosDateTime(FILETIME *ft, WORD *pdate, WORD *ptime)
	{
		time_t tt;
		RFAILED(FileTimeToTimeT(*ft, &tt));
		RFAILED(TimeTToLocalTimeT(tt, &tt));
		return TimeTToDosDateTime(tt, pdate, ptime);
	}
protected:
	//static
	//	HRESULT _GetObjectLoader(IRxRunTime *runtime, IREObjectLoader **ppol)
	//{
	//	UTIL::com_ptr<IUnknown> punk;
	//	RFAILED(runtime->GetObject(PID_ROT_ObjectLoader, (IUnknown**)&punk));
	//	UTIL::com_ptr<IREObjectLoader> pol(punk);
	//	RASSERT(pol, E_INVALIDARG);
	//	*ppol = pol.detach();
	//	return S_OK;
	//}
};

#endif
