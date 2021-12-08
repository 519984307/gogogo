#ifndef __RE_STREAM_TOOL_API__
#define __RE_STREAM_TOOL_API__

#include <wchar.h>
#include "unionname.h"
#include "filepath.h"


namespace ez
{
#if TARGET_OS!=OS_POSIX
	static inline void * mem_mem(const void *buf, 
		size_t buf_len, 
		const void *byte_sequence, 
		size_t byte_sequence_len)
	{
		BYTE *bf = (BYTE *)buf;
		BYTE *bs = (BYTE *)byte_sequence;
		BYTE *p  = bf;

		while (byte_sequence_len <= (buf_len - (p - bf)))
		{
			UINT b = *bs & 0xFF;
			if ((p = (BYTE *) memchr(p, b, buf_len - (p - bf))) != NULL)
			{
				if ((memcmp(p, byte_sequence, byte_sequence_len)) == 0)
					return p;
				else  
					p++;
			}
			else  
			{
				break;
			}
		}
		return NULL;
	}
#endif

	template < class I>
	static FORCEINLINE HRESULT RequireTable( UTIL::com_ptr<I>& oTable, LPCSTR aTableName, IMssLibFile* plib, ULONG gtf = 0, PTN_PRIVATE_DATA_CREATOR pdc = 0 )
	{
		MSTNAME oName;
		rs_strncpy( (LPSTR)oName.NameB, aTableName, sizeof(oName.NameB) );
		UTIL::com_ptr<IMssTable> tmp;
		RFAILED( plib->RequireTable( &oName, gtf, tmp.pp(), pdc ) );
		oTable = tmp;
		RASSERT( oTable, E_NOINTERFACE );
		return S_OK;
	}

	template < class I >
	static FORCEINLINE HRESULT PropGetI( UTIL::com_ptr<I>& oI, IREProperty * prop, REFCLSID rPropId )
	{
		RASSERT( prop, E_FAIL );
		const PROPVARIANT* pvar = 0;
		HRESULT hr = prop->GetProperty( rPropId, &pvar );
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(oI=(IUnknown*)(pvar->pszVal)) )
#else
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(oI=pvar->punkVal) )
#endif
		{
			return E_FAIL;
		}
		return S_OK;
	}
	static FORCEINLINE HRESULT PropSetI( IREProperty * prop, REFCLSID rPropId, IUnknown * punk ) 
	{
		prop->DeleteProperty( rPropId );
		if( !punk ) return S_OK;
		PROPVARIANT var; var.vt = VT_UNKNOWN;
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		var.pszVal = (LPSTR)(IUnknown*)punk;
#else
		var.punkVal = (IUnknown*)punk;
#endif
		return prop->SetProperty( rPropId, &var );
	}

    /// 直接从IRXObjectTable中获取pid指明的对象,并且自动转换到接口I
    ///    
    /// @param oInstance 
    /// @param rt 
    /// @param pid 
    /// @return 参考HRESULT
    /// @see 无
    /// @note 无
	template < class I >
	static FORCEINLINE HRESULT RunTimeGet( UTIL::com_ptr<I>& oInstance, IRXObjectTable* rt, REFGUID pid )
	{
		UTIL::com_ptr<I> obj;
		RFAILED( rt->GetObjectEx( pid, re_uuidof(I), (void**)obj.pp() ) );
		oInstance = obj;
		return S_OK;
	}

    /// 直接从IRXObjectTable中获取pid指明的对象,并且自动转换到接口I
    ///    
    /// @param oInstance 
    /// @param rt 
    /// @param pid 
    /// @return 参考HRESULT
    /// @see 无
    /// @note 无
	template < class I >
	static FORCEINLINE HRESULT RunTimeGet( UTIL::com_ptr<I>& oInstance, IREROT* rt, REFGUID pid )
	{
		UTIL::com_ptr<IUnknown> obj;
		RFAILED( rt->GetObject( pid, obj.pp() ) );
		oInstance = obj;
		if( !oInstance ) return E_NOINTERFACE;
		return S_OK;
	}

    /// 直接从IRXObjectTable中获取pid指明的对象,并且自动转换到接口I
    ///    
    /// @param oInstance 
    /// @param rt 
    /// @param pid 
    /// @return 参考HRESULT
    /// @see 无
    /// @note 无
	template < class I >
	static FORCEINLINE HRESULT RunTimeGet( UTIL::com_ptr<I>& oInstance, IUnknown* rt, REFGUID pid )
	{
		UTIL::com_ptr<IRXObjectTable> ot(rt);
		if( ot )
		{
			UTIL::com_ptr<I> obj;
			RFAILED( ot->GetObjectEx( pid, re_uuidof(I), (void**)obj.pp() ) );
			oInstance = obj;
			return S_OK;
		}
		UTIL::com_ptr<IREROT> ot2(rt);
		if( ot2 )
		{
			UTIL::com_ptr<IUnknown> obj;
			RFAILED( ot2->GetObject( pid, obj.pp() ) );
			oInstance = obj;
			if( !oInstance ) return E_NOINTERFACE;
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	template< class I >
	static FORCEINLINE HRESULT CreateInstance10( UTIL::com_ptr<I>& oInstance, IUnknown * prot, REFCLSID rclsid, IUnknown* pOuter = NULL )
	{
		UTIL::com_ptr<IRERunningObjectTable> rot(prot);
		RASSERT( rot, E_INVALIDARG );
		UTIL::com_ptr<IREObjectLoader> objldr;
		RFAILED( ez::RunTimeGet(objldr, rot.m_p, PID_ROT_ObjectLoader ) );
		UTIL::com_ptr<I> op;
		RFAILED( objldr->CreateInstance(rot,rclsid,pOuter,re_uuidof(I), (void**)op.pp() ) );
		oInstance = op;
		return S_OK;
	}

	template< class I >
	static FORCEINLINE HRESULT CreateInstance( UTIL::com_ptr<I>& oInstance, REFCLSID rclsid, IUnknown* pOuter = NULL, IUnknown * prot = NULL )
	{
		UTIL::com_ptr<I> op;
		RFAILED( RECreateInstance( rclsid, pOuter, re_uuidof(I), (void**)op.pp(), prot ) );
		oInstance = op;
		return S_OK;
	}

	template< class I >
	static FORCEINLINE HRESULT CreateInstanceNCT( UTIL::com_ptr<I>& oInstance, LPCSTR progid, IUnknown * prot = NULL, IUnknown* pOuter = NULL )
	{
		UTIL::com_ptr<I> op;
		RFAILED( RoCreateInstanceNCT( progid, pOuter, re_uuidof(I), (void**)op.pp(), prot ) );
		oInstance = op;
		return S_OK;
	}

	template< class I >
	static FORCEINLINE HRESULT CreateInstance( UTIL::com_ptr<I>& oInstance, TCLSIDREF rclsid, IUnknown* pOuter = NULL )
	{
		UTIL::com_ptr<I> op;
		RFAILED( RoCreateInstance( rclsid, pOuter, re_uuidof(I), (void**)op.pp() ) );
		oInstance = op;
		return S_OK;
	}

	template< class I >
	static FORCEINLINE HRESULT CreateInstance( UTIL::com_ptr<I>& oInstance, TCLSIDREF rclsid, IUnknown* pOuter, IUnknown * prot )
	{
		UTIL::com_ptr<I> op;
		RFAILED( RoCreateInstance( rclsid, pOuter, re_uuidof(I), (void**)op.pp(), prot ) );
		oInstance = op;
		return S_OK;
	}

	template< class I >
	static FORCEINLINE HRESULT CreateInstanceEx( UTIL::com_ptr<I>& oInstance, TCLSIDREF catid, DWORD clsid, IUnknown* pOuter = NULL, IUnknown * prot = NULL)
	{
		UTIL::com_ptr<I> op;
		RFAILED( RoCreateInstanceEx( catid, clsid, pOuter, re_uuidof(I), (void**)op.pp(), prot ) );
		oInstance = op;
		return S_OK;
	}

	static FORCEINLINE HRESULT GetMemBlockOfStream( UTIL::com_ptr<IRxMemFileRO>& rBlock, IREStream * lpStream )
	{
		UTIL::com_ptr<IRxMemFileRO> oBlock;
		UTIL::com_ptr<IREProperty> prop(lpStream);
		RSUCCEED( PropGetI( rBlock, prop, PID_File_ContentBlock ) );
		RFAILED( CreateInstance( oBlock, CLSID_MemBlock ) );
		RFAILED( PropSetI( prop, PID_File_ContentBlock, oBlock ) );
		rBlock = oBlock;
		return S_OK;
	}

	static FORCEINLINE HRESULT GetMemBlockOfStreamEx( UTIL::com_ptr<IRxMemFileRO>& oMemRO, IREStream * pstm, SIZE_T cbMAX )
	{
		oMemRO = pstm;
		if( !oMemRO )
		{
			RFAILED( ez::GetMemBlockOfStream(oMemRO, pstm) );
		}
		if( FAILED( oMemRO->Init(cbMAX) ) )
			RFAILED( oMemRO->Init(pstm,cbMAX) );
		return S_OK;
	}

	template < class I>
	static FORCEINLINE HRESULT CreateVirtualFile( UTIL::com_ptr<I>& oFile )
	{
		UTIL::com_ptr<IRXStream> stm;
		RFAILED( ez::CreateInstance( stm, CLSID_VirtualFile, NULL ) );
		oFile = stm;
		RASSERT( oFile, E_NOINTERFACE );
		return S_OK;
	}

	template < class I, class XCHAR >
	static FORCEINLINE HRESULT CreateVirtualFile( UTIL::com_ptr<I>& oFile, const XCHAR* pName )
	{
		UTIL::com_ptr<IRXStream> stm;
		RFAILED( ez::CreateInstance( stm, CLSID_VirtualFile ) );
		if( pName ) RFAILED( stm->SetName( XSTR(pName) ) );
		oFile = stm;
		RASSERT( oFile, E_NOINTERFACE );
		return S_OK;
	}

	template < class I >
	static FORCEINLINE HRESULT CreatePassThoughStream( UTIL::com_ptr<I>& oFile, IREStreamEx* pBack, OPENMODE uOpenMode = FOMR )
	{
		UINT64 ulFileSize = 0;
		RFAILED( pBack->GetSize64( ulFileSize ) );
		UTIL::com_ptr<I> stm;
		RFAILED( ez::CreateInstance( stm, CLSID_VirtualFile ) );
		UTIL::com_ptr<IRXLogicFile> oLogicFile(stm);
		if( !oLogicFile ) return E_NOINTERFACE;
		RFAILED( oLogicFile->Open( IRXLogicFile::AS_PASSTHOUGH_STREAM, pBack, NULL, uOpenMode ) );
		oFile = stm;
		return S_OK;
	}

	template < class I, class XCHAR >
	static FORCEINLINE HRESULT CreateRegionFile( UTIL::com_ptr<I>& oFile, const XCHAR* pName, IREStream* pBack, LPREGION64 lpRegion, OPENMODE uOpenMode = FOMR )
	{
		UTIL::com_ptr<IREStreamEx> oBack(pBack);
		RASSERT( oBack, E_UNEXPECTED );
		UTIL::com_ptr<I> stm;
		RFAILED( ez::CreateInstance( stm, CLSID_VirtualFile ) );
		UTIL::com_ptr<IRXLogicFile> oLogicFile(stm);
		if( !oLogicFile ) return E_NOINTERFACE;
		RFAILED( oLogicFile->Open( IRXLogicFile::AS_VIRTUAL_STREAM, oBack, lpRegion, uOpenMode ) );
		if( pName )
		{
			UTIL::com_ptr<IRXStream> oStream(stm);
			if( oStream ) RFAILED( oStream->SetName( XSTR(pName) ) );
		}
		oFile = stm;
		return S_OK;
	}

	namespace mss
	{
		template< class I >
		static FORCEINLINE HRESULT CreateInstance( UTIL::com_ptr<I>& oInstance, TCLSIDREF rclsid, IUnknown* pOuter = NULL )
		{
			UTIL::com_ptr<I> op;
			RFAILED( MssCreateInstance( rclsid, pOuter, re_uuidof(I), (void**)op.pp(), NULL ) );
			oInstance = op;
			return S_OK;
		}

		template< class I >
		static FORCEINLINE HRESULT CreateInstance( UTIL::com_ptr<I>& oInstance, TCLSIDREF rclsid, IUnknown* pOuter, IUnknown * prot )
		{
			UTIL::com_ptr<I> op;
			RFAILED( MssCreateInstance( rclsid, pOuter, re_uuidof(I), (void**)op.pp(), prot ) );
			oInstance = op;
			return S_OK;
		}

		template< class XCHAR, class I >
		static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, IRERunningObjectTable* rot, const XCHAR* name, DWORD dwMode = FOMR )
		{
			UTIL::com_ptr<IUnknown> unk;
			RFAILED( rot->GetObject(PID_ROT_FileSystem, unk.pp() ) );
			UTIL::com_ptr<IRXFileSystem> fs(unk);
			RASSERT( fs, E_NOINTERFACE );
			return OpenHandleFile( oInstance, fs, name, dwMode );
		}

		template< class XCHAR, class I >
		static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, const XCHAR* name, DWORD dwMode = FOMR )
		{
			UTIL::com_ptr<IRxHandleFile> oFile;
			RFAILED( CreateInstance( oFile, CLSID_HandleFile ) );
			RFAILED( oFile->Open( pfs, UnionNameR(name), dwMode ) );
			oInstance = oFile;
			RASSERT( oInstance, E_NOINTERFACE );
			return S_OK;
		}
		template < class I>
		static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, UINT64 uFileId, DWORD dwMode = FOMR )
		{
			UTIL::com_ptr<IRxHandleFile> oFile;
			RFAILED( CreateInstance( oFile, CIDOF(HandleFile) ) );
			RFAILED( oFile->Open( pfs, uFileId, dwMode ) );
			oInstance = oFile;
			RASSERT( oInstance, E_NOINTERFACE );
			return S_OK;
		}
		template< class XCHAR, class I >
		static FORCEINLINE HRESULT TOpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, const XCHAR* name, DWORD dwMode = FOMR )
		{
			UTIL::com_ptr<IRxHandleFile> oFile;
			RFAILED( CreateInstance( oFile, CLSID_HandleFile ) );
			RFAILED( oFile->Open( pfs, UnionNameR(name), dwMode ) );
			oInstance = oFile;
			RASSERT( oInstance, E_NOINTERFACE );
			return S_OK;
		}
		template< class I >
		static FORCEINLINE HRESULT TOpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, UINT64 uFileId, DWORD dwMode = FOMR )
		{
			UTIL::com_ptr<IRxHandleFile> oFile;
			RFAILED( CreateInstance( oFile, CLSID_HandleFile ) );
			RFAILED( oFile->Open( pfs, uFileId, dwMode ) );
			oInstance = oFile;
			RASSERT( oInstance, E_NOINTERFACE );
			return S_OK;
		}
	};

	template< class I >
	static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, REFCXSTRP xFileName, IRXFileSystem* pfs, DWORD dwMode = FOMR )
	{
		UTIL::com_ptr<IRxHandleFile> oFile;
		RFAILED( CreateInstance( oFile, CLSID_HandleFile ) );
		RFAILED( oFile->Open( pfs, xFileName, dwMode ) );
		oInstance = oFile;
		RASSERT( oInstance, E_NOINTERFACE );
		return S_OK;
	}

	template< class XCHAR, class I >
	static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, const XCHAR* name, DWORD dwMode = FOMR )
	{
		return OpenHandleFile( oInstance, XSTR(name), pfs, dwMode );
	}

	template < class I>
	static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, UINT64 uFileId, DWORD dwMode = FOMR )
	{
		UTIL::com_ptr<IRxHandleFile> oFile;
		RFAILED( CreateInstance( oFile, CLSID_HandleFile ) );
		RFAILED( oFile->Open( pfs, uFileId, dwMode ) );
		oInstance = oFile;
		RASSERT( oInstance, E_NOINTERFACE );
		return S_OK;
	}
	template< class XCHAR, class I >
	static FORCEINLINE HRESULT XCreateStream( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, const XCHAR* name, DWORD dwMode = FOMR )
	{
		UTIL::com_ptr<IRXStream> stm;
		RFAILED( pfs->XCreateStream( UnionNameR(name), stm.pp(), dwMode ) );
		oInstance = stm;
		RASSERT( oInstance, E_NOINTERFACE );
		return S_OK;
	}

	//template< class XCHAR, class I >
	//static FORCEINLINE HRESULT TOpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, const XCHAR* name, DWORD dwMode = FOMR )
	//{
	//	UTIL::com_ptr<IRXStream> stm;
	//	RFAILED( pfs->XCreateStream( UnionNameR(name), stm.pp(), dwMode ) );
	//	oInstance = stm;
	//	RASSERT( oInstance, E_NOINTERFACE );
	//	return S_OK;
	//}
	//template< class I >
	//static FORCEINLINE HRESULT TOpenHandleFile( UTIL::com_ptr<I>& oInstance, IRXFileSystem* pfs, UINT64 uFileId, DWORD dwMode = FOMR )
	//{
	//	UTIL::com_ptr<IRxHandleFile> oFile;
	//	RFAILED( CreateInstance( oFile, CLSID_HandleFile ) );
	//	RFAILED( oFile->Open( pfs, uFileId, dwMode ) );
	//	oInstance = oFile;
	//	RASSERT( oInstance, E_NOINTERFACE );
	//	return S_OK;
	//}

	template< class XCHAR, class I >
	static FORCEINLINE HRESULT OpenHandleFile( UTIL::com_ptr<I>& oInstance, IRERunningObjectTable* rot, const XCHAR* name, DWORD dwMode = FOMR )
	{
		UTIL::com_ptr<IUnknown> unk;
		RFAILED( rot->GetObject(PID_ROT_FileSystem, unk.pp() ) );
		UTIL::com_ptr<IRXFileSystem> fs(unk);
		RASSERT( fs, E_NOINTERFACE );
		return OpenHandleFile( oInstance, (IRXFileSystem*)fs, name, dwMode );
	}


	template< class I >
	static FORCEINLINE HRESULT OpenArchive( comptr<I>& rArchive, FFMTID fmt, IUnknown* punk )
	{
		RASSERT( punk, E_INVALIDARG );
		UTIL::com_ptr<IRXArchive> oArchive;
		RFAILED( CreateInstanceEx( oArchive, CLSID_ARCHIVE_CLASS_CATELOG, fmt ) );
		comptr<I> tmp( oArchive );
		RASSERT( tmp, E_NOINTERFACE );
		RFAILED( oArchive->Open2( punk, fmt ) );
		rArchive = tmp;
		return S_OK;
	}

	template < class I >
	static FORCEINLINE HRESULT GetSig64List( UTIL::com_ptr<I>& oHashList, IREStream* pstm, BOOL bFecthNow, BOOL bCreateIfNotExist )
	{
		UTIL::com_ptr<IREProperty> prop( pstm );
		if( !prop ) return E_NOINTERFACE;

		UTIL::com_ptr<IRXSig64List> tmp;

		const PROPVARIANT* pvar = 0;
		HRESULT hr = prop->GetProperty( PID_File_Sig64List, &pvar );

#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(tmp==(IUnknown*)(pvar->pszVal) ) )
#else
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(tmp=pvar->punkVal) )
#endif
		{
			if( !bCreateIfNotExist )
				return E_FAIL;

			RFAILED( RoCreateInstance( CLSID_Sig64List, NULL, re_uuidof(IRXSig64List), (void**)tmp.pp() ) );

			if( bFecthNow ) 
			{
				RFAILED( sig64FetchS( pstm, tmp ) );
			}

			prop->DeleteProperty(PID_File_Sig64List);

			PROPVARIANT var;
			var.vt = VT_UNKNOWN;
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
			var.pszVal = (LPSTR)tmp.m_p;
#else
			var.punkVal = tmp;
#endif
			RFAILED( prop->SetProperty( PID_File_Sig64List, &var ) );
		}

		oHashList = tmp;
		return S_OK;
	}

	static FORCEINLINE HRESULT GetCurrentFormat( IUnknown* pstm, FFMTID& fmtid )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		RASSERT( prop, E_NOINTERFACE );
		const PROPVARIANT* pvar = NULL;//{ VT_UI4 };
		RFAILED( prop->GetProperty( PID_File_CurrentFormat, &pvar ) );
		fmtid = pvar->ulVal;
		return S_OK;
	}
	static FORCEINLINE HRESULT SetCurrentFormat( IUnknown* pstm, UINT fmtid )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		RASSERT( prop, E_NOINTERFACE );
		PROPVARIANT var; var.vt = VT_UI4;
		var.ulVal = fmtid;
		return prop->SetProperty( PID_File_CurrentFormat, &var );
	}

	static FORCEINLINE HRESULT SetFormatList( IUnknown* pstm, IREFormatList* pfl )
	{
		UTIL::com_ptr<IREProperty> prop( pstm );
		if( !prop ) return E_NOINTERFACE;
		PROPVARIANT var; var.vt = VT_UNKNOWN;
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		var.pszVal = (LPSTR)(IUnknown*)pfl;
#else
		var.punkVal = (IUnknown*)pfl;
#endif
		return prop->SetProperty( PID_File_FormatList, &var );
	}

	template < class I >
	static FORCEINLINE HRESULT GetStreamName( I* p, XSTRP& str )
	{
		UTIL::com_ptr<IRXStream> xstm(p);
		if( !xstm ) 
		{
			UTIL::com_ptr<IREArchive> arch(p);
			if( !arch ) return E_NOINTERFACE;
			UTIL::com_ptr<IREStream> tmp;
			RFAILED( arch->GetStreamObject(tmp.pp()) );
			xstm = tmp;
			RASSERT( xstm, E_NOINTERFACE );
		}
		return xstm->GetName(str);
	}

	static FORCEINLINE HRESULT OpenStoneProp( REFCXSTRP xFileName, UTIL::com_ptr<IRXStoneProp>& rProp, IUnknown* rot)
	{
		UTIL::com_ptr<IRXStoneProp> tmp;
		RFAILED( ez::CreateInstance( tmp, CLSID_StoneProp, 0, rot ) );
		RFAILED( tmp->Open2( xFileName ) );
		rProp = tmp;
		return S_OK;
	}

	static FORCEINLINE HRESULT OpenStoneProp( LPCWSTR wFileName, UTIL::com_ptr<IRXStoneProp>& rProp, IUnknown* rot)
	{
		return OpenStoneProp(XSTR(wFileName), rProp, rot );
	}

	static FORCEINLINE HRESULT RevokeStreamProp( IUnknown* pstm, REFGUID guidProp )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		RASSERT( prop, E_NOINTERFACE );
		prop->DeleteProperty(guidProp);
		return S_OK;
	}

	static FORCEINLINE HRESULT BindStoneProp( IUnknown* pstm, IRXStoneProp* p )
	{
		UTIL::com_ptr<IREProperty> prop( pstm );
		if( !prop ) return E_NOINTERFACE;
		PROPVARIANT var; var.vt = VT_UNKNOWN;
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		var.pszVal = (LPSTR)p;
#else
		var.punkVal = p;
#endif
		return ( prop->SetProperty( PID_File_StoneProp, &var ) );
	}

	template< class I >
	static FORCEINLINE HRESULT GetStoneProp( IUnknown* pstm, UTIL::com_ptr<I>& rProp, BOOL bCreateIfNotExist = FALSE, IUnknown * prot = NULL )
	{
		UTIL::com_ptr<IREProperty> prop( pstm );
		if( !prop ) return E_NOINTERFACE;

		UTIL::com_ptr<IRXStoneProp> tmp;

		const PROPVARIANT* pvar = 0;
		HRESULT hr = prop->GetProperty( PID_File_StoneProp, &pvar );
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(tmp=(IUnknown*)(pvar->pszVal)) )
#else
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(tmp=pvar->punkVal) )
#endif
		{
			if( !bCreateIfNotExist ) return E_FAIL;

			RFAILED( ez::CreateInstance( tmp, CIDOF(StoneProp), 0, prot ) );

			PROPVARIANT var; var.vt = VT_UNKNOWN;
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
			var.pszVal = (LPSTR)tmp.m_p;
#else
			var.punkVal = tmp;
#endif

			RFAILED( prop->SetProperty( PID_File_StoneProp, &var ) );
		}
		rProp = tmp;
		return S_OK;
	}

	static FORCEINLINE HRESULT GetFielName( IUnknown* pstm, CFilePath& rFilePath )
	{
		UTIL::com_ptr<IREStreamEx> exstm(pstm);
		RASSERT( exstm, E_NOINTERFACE );
		XSTRP tmp;
		RFAILED( exstm->GetName(tmp) );
		rFilePath = tmp;
		return S_OK;
	}

	static FORCEINLINE HRESULT MoveProperty( IUnknown* from, IUnknown* to, REFGUID rpropid, BOOL bDelSource = TRUE )
	{
		UTIL::com_ptr<IREProperty> prop1( from );
		if( !prop1 ) return E_NOINTERFACE;

		UTIL::com_ptr<IREProperty> prop2( to );
		if( !prop2 ) return E_NOINTERFACE;

		const PROPVARIANT * pvar = 0;
		RFAILED( prop1->GetProperty( rpropid, &pvar ) );
		RFAILED( prop2->SetProperty( rpropid, pvar ) );
		if( bDelSource ) prop1->DeleteProperty( rpropid );
		return S_OK;
	}

	static FORCEINLINE HRESULT GetAuxDataBox( UTIL::com_ptr<IRXDTDataBox>& rDatBox, IUnknown* pstm )
	{
		UTIL::com_ptr<IREProperty> prop( pstm );
		if( !prop ) return E_NOINTERFACE;
		return PropGetI(rDatBox,prop,PID_File_AuxDataBox);
	}
	static FORCEINLINE HRESULT GetAuxDataBox( UTIL::com_ptr<IRXDTDataBox>& rDatBox, IREProperty* prop )
	{
		return PropGetI(rDatBox,prop,PID_File_AuxDataBox);
	}
	static FORCEINLINE HRESULT GetAuxDataBox2( UTIL::com_ptr<IRXDTDataBox>& rDatBox, IUnknown* x, LPCSTR aClassID = NULL )
	{
		UTIL::com_ptr<IREProperty> prop( x );
		if( !prop ) return E_NOINTERFACE;
		RSUCCEED( PropGetI(rDatBox,prop,PID_File_AuxDataBox) );
		if( !aClassID ) aClassID = "rxcore.DTDataBox";
		RFAILED( CreateInstanceNCT( rDatBox, aClassID ) );
		return PropSetI( prop, PID_File_AuxDataBox, rDatBox);
	}

	static FORCEINLINE HRESULT GetFormatList( IUnknown* pstm, UTIL::com_ptr<IREFormatList>& rList, BOOL bCreateIfNotExist = TRUE )
	{
		UTIL::com_ptr<IREProperty> prop( pstm );
		if( !prop ) return E_NOINTERFACE;

		UTIL::com_ptr<IREFormatList> tmp;

		const PROPVARIANT* pvar = 0;
		HRESULT hr = prop->GetProperty( PID_File_FormatList, &pvar );
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(tmp=(IUnknown*)(pvar->pszVal)) )
#else
		if( FAILED(hr) || (!pvar) || (pvar->vt != VT_UNKNOWN) || !(tmp=pvar->punkVal) )
#endif
		{
			if( !bCreateIfNotExist ) return E_FAIL;

			RFAILED( ez::CreateInstance( tmp, CIDOF(FormatList) ) );

			PROPVARIANT var; var.vt = VT_UNKNOWN;
#if (CPP_COMPILER==CC_GCC) && (TARGET_OS==OS_WINDOWS)
			var.pszVal = (LPSTR)tmp.m_p;
#else
			var.punkVal = tmp;
#endif

			RFAILED( prop->SetProperty( PID_File_FormatList, &var ) );
		}
		rList = tmp;
		return S_OK;
	}

	static FORCEINLINE HRESULT AppendFormat( IUnknown * pstm, FFMTID fmtid )
	{
		UTIL::com_ptr<IREFormatList> fmtlist;
		RFAILED( GetFormatList(pstm,fmtlist) );
		return fmtlist->Append(fmtid);
	}


	template<class XCHAR> static FORCEINLINE
		HRESULT RealCreateHandleFile(IRERunningObjectTable *prot, const XCHAR * filename, DWORD flags, IREStream **ppstm )
	{
		UTIL::com_ptr<IREStream> tmp;
		RFAILED( ez::OpenHandleFile<XCHAR>( tmp, prot, filename, flags ) );
		*ppstm = tmp.detach();
		return S_OK;
		//UTIL::com_ptr<IUnknown> punk;
		//RFAILED(prot->GetObject(PID_ROT_FileSystem, (IUnknown**)&punk));
		//UTIL::com_ptr<IRXFileSystem> pfs(punk);
		//RASSERT(pfs, E_INVALIDARG);
		//return CreateHandleFile(prot, pfs, filename, flags, ppstm, type);
	}

	struct File
	{
		enum
		{
			FAA_BUFFER_SIZE = 0x1000,
		};
		enum
		{
			FERR_EOF = -1,
			FERR_BOF = -2,
			FERR_SEEK = -10,
			FERR_IO	= -11,
		};

		UTIL::com_ptr<IREStream>	sop;
		UTIL::com_ptr<IRXStream>	sop2;

		File( )
		{
		}
		struct BRX
		{
			UINT8	Prev;
			UINT8	Rest;
		};
#define bget( U, f, s ) ( (U)<<(f)>>(8-(s)) )
		HRESULT ReadBits( ULONG& ret, BRX & brx, size_t bits )
		{
			ret = 0;
			for( ; bits >= brx.Rest; bits -= brx.Rest, brx.Rest = 0 )
			{
				if( !brx.Rest )
				{
					RFAILED( ReadBC(&brx.Prev,1) );
					brx.Rest = 8;
				}
				ret |= bget(brx.Prev, (8-brx.Rest), brx.Rest );
				ret <<= 8;
			}
			if( bits < brx.Rest )
			{
				ret |= bget( brx.Prev, 0, bits );
				brx.Rest -= (UINT8)bits;
			}
			return S_OK;
		}

		INT GetB()
		{
			UINT8 zByte = 0;
			DWORD cbEff = 0;
			RFAILED_( sop->Read( &zByte, 1, &cbEff ), FERR_EOF );
			if( !cbEff ) return FERR_EOF;
			return zByte;
		}
		INT PutB( UINT8 ub )
		{
			RFAILED_( WriteBC( &ub, 1 ), FERR_IO );
			return ub;
		}

		//File( IRXStream * p ) : sop(p)
		//{
		//}
		File( IUnknown * p ) : sop(p), sop2(p)
		{
		}
		File( IREStream * p ) : sop(p), sop2(p)
		{
		}
		File( IRXStream * p ) : sop(p), sop2(p)
		{
		}

		operator IREStream* ( )
		{
			return sop;
		}
		IREStream* operator -> ( )
		{
			return sop;
		}

		template< class I >
		static FORCEINLINE HRESULT GetParentStream( UTIL::com_ptr<I>& parent, IREStream* p )
		{
			UTIL::com_ptr<IREArchive> arch;
			RFAILED( GetParent( p, arch.pp() ) );
			UTIL::com_ptr<IREStream> stm;
			RFAILED( arch->GetStreamObject( stm.pp() ) );
			parent = stm;
			RASSERT( parent, E_NOINTERFACE );
			return S_OK;
		}

		//static FORCEINLINE BOOL IsUnpacked( IREStream * p )
		//{
		//	UTIL::com_ptr<IREProperty> prop(p);
		//	if( !prop ) return FALSE;
		//	prop->GetProperty(PID_File_IsUnpacked);
		//}

		template< class I >
		HRESULT GetParentStream( UTIL::com_ptr<I>& parent )
		{
			return GetParentStream(parent, sop );
		}
		static FORCEINLINE HRESULT GetParent( IUnknown* punk, IRXArchive** pparc )
		{
			UTIL::com_ptr<IREStreamEx> stmex( punk );
			RASSERT( stmex, E_INVALIDARG );
			UTIL::com_ptr<IRXArchive> prt;
			RFAILED( stmex->GetParent( prt.pp() ) );
			*pparc = prt.detach();
			return S_OK;
		}
		static FORCEINLINE HRESULT GetParent( IUnknown* punk, IREArchive** pparc )
		{
			UTIL::com_ptr<IREStreamEx> stmex( punk );
			RASSERT( stmex, E_INVALIDARG );
			UTIL::com_ptr<IRXArchive> prt;
			RFAILED( stmex->GetParent( prt.pp() ) );
			*pparc = prt.detach();
			return S_OK;
		}
		static FORCEINLINE HRESULT GetParent( IREArchive* parc, IRXArchive** pparc )
		{
			UTIL::com_ptr<IREStream> stml;
			RFAILED( parc->GetStreamObject( stml.pp() ) );
			return GetParent( stml, pparc );
		}
		static FORCEINLINE HRESULT GetParent( IREArchive* parc, IREArchive** pparc )
		{
			UTIL::com_ptr<IREStream> stml;
			RFAILED( parc->GetStreamObject( stml.pp() ) );
			return GetParent( stml, pparc );
		}
		HRESULT GetParent( IREArchive** pparc )
		{
			return GetParent( sop, pparc );
		}
		HRESULT GetParentEx( IRXArchive** pparc )
		{
			return GetParent( sop, pparc );
		}
		HRESULT GetFileName( REFXSTRP rstrp )
		{
			RASSERT( sop2, E_FAIL );
			return sop2->GetName(rstrp);
		}
		static FORCEINLINE HRESULT GetFileName( IUnknown* p, REFXSTRP rstrp )
		{
			UTIL::com_ptr<IRXStream> stm(p);
			RASSERT( stm, E_NOINTERFACE );
			return GetFileName(stm,rstrp);
		}
		static FORCEINLINE HRESULT GetFileName( IRXStream* p, REFXSTRP rstrp )
		{
			return p->GetName(rstrp);
		}
		static FORCEINLINE HRESULT GetFileName( IREArchive* p, REFXSTRP rstrp )
		{
			UTIL::com_ptr<IREStream> stm;
			RFAILED( p->GetStreamObject(stm.pp()) );
			return GetFileName(stm,rstrp);
		}
		DWORD GetCurrentType()
		{
			return FFR::FMT_UNKNOWN;
		}
		static FORCEINLINE DWORD GetCurrentType( IREArchive* lpArchive )
		{
			UTIL::com_ptr<IRXArchive> arch(lpArchive);
			if( !arch ) return FFR::FMT_UNKNOWN;
			ULONG fmtid = 0;
			RFAILED_( arch->GetFormatID(&fmtid), FFR::FMT_UNKNOWN );
			return fmtid;
		}

		HRESULT Close()
		{
			UTIL::com_ptr<IREStreamEx> exstm(sop);
			if( exstm ) return exstm->Close();
			return E_NOTIMPL;
		}

		HRESULT ReOpen( DWORD dwAccess )
		{
			return SetAccess(dwAccess);
		}

		HRESULT SetAccess( DWORD dwAccess )
		{
			UTIL::com_ptr<IREStreamEx> stmex( sop );
			if( !stmex ) return S_FALSE;
			return stmex->SetAccess( dwAccess );
		}

		// OK: > 0
		static FORCEINLINE LONG ReadLEVI( IREStream* pstm, UINT64& rVal )
		{
			LONG i = 0;
			UINT8 b = 0;
			for( ; (i<5) && (0==(b&0x80)); ++ i )
			{
				RFAILED( pstm->Read( &b, 1, NULL ) );
				UINT32 v = b & 0x7F;
				rVal |= (v << (i*7));
			}
			return i;
		}

		static FORCEINLINE LONG WriteLEVI( IREStream* pstm, UINT32 rVal )
		{
			LONG i = 0;
			do {
				UINT8 b = rVal & 0x7F;
				rVal >>= 7;
				if( !rVal ) b |= 0x80;
				RFAILED( pstm->Write( &b, 1, NULL ) );
				++ i;
			} while( rVal );
			return i;
		};

		static FORCEINLINE HRESULT ReadBC( IREStream* sop, LPVOID pBuffer, DWORD cbToRead )
		{
			DWORD cbEff = 0;
			RFAILED( sop->Read( pBuffer, cbToRead, &cbEff ) );
			RASSERT( cbEff == cbToRead, E_FAIL );
			return S_OK;
		}
		static FORCEINLINE HRESULT WriteBC( IREStream* sop, LPCVOID pBuffer, DWORD cbToWrite )
		{
			DWORD cbEff = 0;
			RFAILED( sop->Write( pBuffer, cbToWrite, &cbEff ) );
			RASSERT( cbEff == cbToWrite, E_FAIL );
			return S_OK;
		}
		FORCEINLINE LONG ReadLEVI( UINT64& rVal )
		{
			return ReadLEVI( sop, rVal );
		}
		FORCEINLINE LONG WriteLEVI( UINT32 rVal )
		{
			return WriteLEVI( sop, rVal );
		};
		FORCEINLINE HRESULT ReadBC( LPVOID pBuffer, UINT64 uPos, DWORD cbToRead )
		{
			RFAILED( Seek64(uPos) );
			return ReadBC(sop,pBuffer,cbToRead);
		}
		FORCEINLINE HRESULT ReadBC( LPVOID pBuffer, DWORD cbToRead )
		{
			return ReadBC(sop,pBuffer,cbToRead);
		}
		FORCEINLINE HRESULT WriteBC( LPCVOID pBuffer, DWORD cbToWrite )
		{
			return WriteBC(sop,pBuffer,cbToWrite);
		}
		FORCEINLINE LONG Read( LPVOID pBuffer, LONG cbToRead )
		{
			RASSERT( cbToRead > 0, 0 );
			DWORD cbEff = 0;
			RFAILEDP( sop->Read( pBuffer, cbToRead, &cbEff ), -1 );
			return cbEff;
		}
		FORCEINLINE DWORD Write(LPCVOID lpBuffer, LONG dwToWrite )
		{
			RASSERT( dwToWrite > 0, 0 );
			DWORD cbEff = 0;
			RFAILEDP( sop->Write( lpBuffer, dwToWrite, &cbEff ), 0 );
			return cbEff;
		}
		FORCEINLINE HRESULT Seek(LONG lOffset, DWORD dwMethod = FILE_BEGIN)
		{
			return Seek64(lOffset, dwMethod);
		}
		FORCEINLINE HRESULT Seek64(INT64 lOffset, DWORD dwMethod = FILE_BEGIN)
		{
			if( sop2 ) return sop2->Seek64(lOffset, dwMethod );
			LARGE_INTEGER liOffset;
			liOffset.QuadPart = lOffset;
			return sop->Seek( liOffset.u.LowPart, &liOffset.u.HighPart, dwMethod );
		}
		FORCEINLINE HRESULT Tell(DWORD &dwOffset)
		{
			return sop->Tell(&dwOffset, NULL);
		}
		FORCEINLINE HRESULT Tell64(UINT64 &dwOffset)
		{
			if( sop2 ) return sop2->Tell64(dwOffset);
			ULARGE_INTEGER uliOffset;
			RFAILED( sop->Tell( &uliOffset.u.LowPart, &uliOffset.u.HighPart ) );
			dwOffset = uliOffset.QuadPart;
			return S_OK;
		}
		FORCEINLINE HRESULT GetSize(DWORD & dwSize )
		{
			return sop->GetSize(&dwSize, NULL);
		}
		FORCEINLINE HRESULT GetSize64( UINT64 & dwSize )
		{
			if( sop2 ) return sop2->GetSize64( dwSize );
			ULARGE_INTEGER uliSize;
			RFAILED( sop->GetSize( &uliSize.u.LowPart, &uliSize.u.HighPart ) );
			dwSize = uliSize.QuadPart;
			return S_OK;
		}
		FORCEINLINE HRESULT SetSize(DWORD dwSize)
		{
			return (sop->SetSize(dwSize, NULL));
		}
		FORCEINLINE HRESULT SetSize64( UINT64 dwSize)
		{
			if( sop2 ) return sop2->SetSize64( dwSize );
			ULARGE_INTEGER uliSize;
			uliSize.QuadPart = dwSize;
			return ( sop->SetSize( uliSize.u.LowPart, &uliSize.u.HighPart ) );
		}
		FORCEINLINE DWORD ReadPos(LPVOID lpBuffer, LONG sOffset, DWORD cbToRead )
		{
			if( !cbToRead ) return 0;
			RFAILED_( Seek( sOffset ), 0 );
			DWORD dwRead = 0;
			RFAILED_(sop->Read(lpBuffer, cbToRead, &dwRead), 0 );
			return dwRead;
		}
		FORCEINLINE DWORD ReadPos64(LPVOID lpBuffer, INT64 sOffset, DWORD cbToRead )
		{
			if( !cbToRead ) return 0;
			RFAILED_( Seek64( sOffset ), 0 );
			DWORD dwRead = 0;
			RFAILED_(sop->Read(lpBuffer, cbToRead, &dwRead), 0 );
			return dwRead;
		}
		FORCEINLINE HRESULT ReadPos2(LPVOID lpBuffer, INT64 sOffset, DWORD& cbToRead )
		{
			if( !cbToRead ) return S_OK;
			RFAILED( Seek64( sOffset ) );
			DWORD dwRead = 0;
			RFAILED(sop->Read(lpBuffer, cbToRead, &dwRead) );
			cbToRead = dwRead;
			return S_OK;
		}
		FORCEINLINE DWORD ReadPosHoldPointer64( LPVOID lpBuffer, INT64 lOffset, DWORD dwToRead)
		{
			RASSERT( dwToRead > 0, 0 );
			UINT64 dwPos;
			RFAILED_(Tell64(dwPos), 0 );
			DWORD dwRead = ReadPos64(lpBuffer, dwPos, dwToRead );
			RFAILED_(Seek64(dwPos), 0);
			return dwRead;
		}
		FORCEINLINE DWORD ReadPosHoldPointer(LPVOID lpBuffer, LONG lOffset, DWORD dwToRead)
		{
			RASSERT( dwToRead > 0, 0 );
			DWORD dwPos;
			RFAILED_(Tell(dwPos), 0);
			DWORD dwRead = ReadPos(lpBuffer, lOffset, dwToRead);
			RFAILED_(Seek(dwPos), 0);
			return dwRead;
		}

		FORCEINLINE DWORD WritePos(LPCVOID lpBuffer, LONG lOffset, DWORD dwToWrite)
		{
			RASSERT( dwToWrite > 0, 0 );
			RFAILED_(sop->Seek(lOffset, NULL, FILE_BEGIN), 0);
			DWORD dwWritten;
			RFAILED_(sop->Write(lpBuffer, dwToWrite, &dwWritten), 0);
			return dwWritten;
		}
		FORCEINLINE DWORD WritePos64(LPCVOID lpBuffer, INT64 lOffset, DWORD dwToWrite)
		{
			RASSERT( dwToWrite > 0, 0 );
			RFAILED_(Seek64( lOffset ), 0 );
			DWORD dwWritten;
			RFAILED_(sop->Write(lpBuffer, dwToWrite, &dwWritten), 0);
			return dwWritten;
		}
		FORCEINLINE DWORD WritePosHoldPointer(LPCVOID lpBuffer, LONG lOffset, DWORD dwToWrite)
		{
			RASSERT( dwToWrite > 0, 0 );
			DWORD dwPos;
			RFAILED_(Tell( dwPos ), 0);
			DWORD dwWritten = WritePos64(lpBuffer, lOffset, dwToWrite);
			RFAILED_(Seek( dwPos ), 0);
			return dwWritten;
		}
		FORCEINLINE DWORD WritePosHoldPointer64(LPCVOID lpBuffer, INT64 lOffset, DWORD dwToWrite)
		{
			RASSERT( dwToWrite > 0, 0 );
			UINT64 dwPos;
			RFAILED_(Tell64( dwPos ), 0);
			DWORD dwWritten = WritePos64(lpBuffer, lOffset, dwToWrite);
			RFAILED_(Seek64( dwPos ), 0);
			return dwWritten;
		}
		FORCEINLINE UINT64 GetSize()
		{
			UINT64 uFileSize = 0;
			RFAILED_( GetSize64(uFileSize), (UINT64)-1 );
			return uFileSize;
		}

#define ALERT(x)		//MessageBoxA( 0, #x, "alert", 0 );

		FORCEINLINE HRESULT FillDataBlock( UINT64 dwOffsetDest, CONST BYTE val, DWORD count )
		{
			ALERT(FillDataBlock);
			enum { FILL_COUNT_PER_TIME = 0x100 };
			RFAILED_( Seek64( dwOffsetDest, FILE_BEGIN ), E_FAIL );
			UTIL::sentry<UINT8*,UTIL::_DAS> fill( new UINT8[FILL_COUNT_PER_TIME] );
			if( !fill ) return E_OUTOFMEMORY;
			memset( fill, val, FILL_COUNT_PER_TIME );
			for( ; count > FILL_COUNT_PER_TIME; count -= FILL_COUNT_PER_TIME )
			{
				RFAILED( WriteBC( fill, FILL_COUNT_PER_TIME ) );
			}
			return WriteBC( fill, count );
		}
		FORCEINLINE HRESULT CopyFileBlock64( UINT64 dwOffsetDest, UINT64 dwOffsetSrc, UINT64 dwLength )
		{
			ALERT(CopyFileBlock);
			if( dwOffsetDest == dwOffsetSrc ) return S_OK ;
			LONG lDelta ;
			BYTE buf[FAA_BUFFER_SIZE] ;
			if( dwOffsetDest <= dwOffsetSrc || dwOffsetDest >= dwOffsetSrc + dwLength)
			{
				lDelta = FAA_BUFFER_SIZE;
			}
			else
			{
				dwOffsetSrc += dwLength - FAA_BUFFER_SIZE;
				dwOffsetDest += dwLength - FAA_BUFFER_SIZE;
				lDelta = -FAA_BUFFER_SIZE;
			}
			while( (INT64)(dwLength -= FAA_BUFFER_SIZE) >= 0 )
			{
				//Src:
				if(ReadPos64( buf, dwOffsetSrc, FAA_BUFFER_SIZE ) != FAA_BUFFER_SIZE )
				{
					return E_UNEXPECTED ;
				}
				//Dest:
				if(WritePos64(buf, dwOffsetDest, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE)
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
				if( ReadPos64(buf, dwOffsetSrc, dwLength) != dwLength )
				{
					return E_UNEXPECTED ;
				}
				//Dest:
				if( WritePos64(buf, dwOffsetDest, dwLength) != dwLength )
				{
					return E_UNEXPECTED ;
				}
			}
			return S_OK ;
		}
		FORCEINLINE HRESULT CopyFileBlock( UINT64 dwOffsetDest, UINT64 dwOffsetSrc, SIZE_T dwLength )
		{
			return CopyFileBlock64( dwOffsetDest, dwOffsetSrc, dwLength );
		}
		FORCEINLINE HRESULT DeleteFileBlock( UINT64 uOffset, SIZE_T cbDelete )
		{
			ALERT(DeleteFileBlock);
			if( !cbDelete ) return S_FALSE;
			UINT64 uFileSize = 0;
			RFAILED( GetSize64(uFileSize) );
			if( uOffset >= uFileSize ) return S_FALSE;
			UINT64 uCutEnd = uOffset + cbDelete;
			if( uCutEnd < uOffset ) uCutEnd = uFileSize;
			UINT64 cbToMove = uFileSize - uCutEnd;
			UINT64 uChunk64 = MIN( 0x1000, cbToMove );
			UINT32 uChunk = uChunk64;
			UTIL::sentry<UINT8*,UTIL::_DAS> buffer( new UINT8[uChunk] );
			if( !buffer ) return E_OUTOFMEMORY;
			UINT64 uReadPos = uCutEnd;
			UINT64 uWritePos = uOffset;
			while( cbToMove )
			{
				DWORD dwToRead = MIN(cbToMove, uChunk);
				RFAILED( Seek64( uReadPos ) );
				DWORD cbRead = 0;
				RFAILED( sop->Read( buffer.m_p, dwToRead, &cbRead ) );
				if( !cbRead ) break;
				RFAILED( Seek64( uWritePos ) );
				RFAILED( WriteBC( buffer.m_p, cbRead ) );
				uReadPos += cbRead;
				uWritePos += cbRead;
				cbToMove -= cbRead;
			}
			return SetSize64(uWritePos);
		}

		FORCEINLINE HRESULT Remove()
		{
			return sop->Remove();
		}

		static FORCEINLINE HRESULT Remove( IUnknown * p )
		{
			UTIL::com_ptr<IREStream> sop(p);
			RASSERT( sop, E_INVALIDARG );
			return sop->Remove();
		}

		FORCEINLINE HRESULT InsertDataBlock( UINT64 uOffsetDest, SIZE_T dwLength, LPCVOID pvBuffer )
		{
			ALERT(InsertDataBlock);
			if( !dwLength ) return S_OK ;

			UINT64 dwFileSize = GetSize() ;
			if( uOffsetDest > dwFileSize ) 
				return E_UNEXPECTED ;

			RFAILED( SetSize64( dwFileSize + dwLength ) );

			if( uOffsetDest < dwFileSize )
			{
				RFAILED( CopyFileBlock64( uOffsetDest + dwLength, uOffsetDest, dwFileSize-uOffsetDest ) );
			}
			if( pvBuffer )
			{
				return WritePos64( pvBuffer, uOffsetDest, dwLength ) ;
			}
			return S_OK ;
		}

		FORCEINLINE HRESULT SwapFileBlock( UINT64 dwOffsetDest, UINT64 dwOffsetSrc, SIZE_T dwLength )
		{
			ALERT(SwapFileBlock);
			if( 0 == dwLength ) return E_UNEXPECTED ;
			if( ((dwOffsetDest>=dwOffsetSrc) && (dwOffsetDest<(dwOffsetSrc+dwLength))) ||
				((dwOffsetSrc>=dwOffsetDest) && (dwOffsetSrc<(dwOffsetDest+dwLength))) )
			{ return E_UNEXPECTED ; }
			BYTE buf_src[FAA_BUFFER_SIZE], buf_dest[FAA_BUFFER_SIZE] ;
			while( (int64_t)(dwLength -= FAA_BUFFER_SIZE) >= 0 )
			{
				//Src Read:
				if( ReadPos64(buf_src, dwOffsetSrc, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
				{
					return E_UNEXPECTED ;
				}
				//Dest Read:
				if( ReadPos64(buf_dest, dwOffsetDest, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
				{
					return E_UNEXPECTED ;
				}
				//Src Write:
				if( WritePos64(buf_dest, dwOffsetSrc, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
				{
					return E_UNEXPECTED ;
				}
				//Dest Write:
				if( WritePos64(buf_src, dwOffsetDest, FAA_BUFFER_SIZE) != FAA_BUFFER_SIZE )
				{
					return E_UNEXPECTED ;
				}
				dwOffsetSrc  += FAA_BUFFER_SIZE ;
				dwOffsetDest += FAA_BUFFER_SIZE ;
			}
			if( dwLength += FAA_BUFFER_SIZE )
			{
				//Src Read:
				if( ReadPos64(buf_src, dwOffsetSrc, dwLength) != dwLength )
				{
					return E_UNEXPECTED ;
				}
				//Dest Read:
				if( ReadPos64(buf_dest, dwOffsetDest, dwLength) != dwLength )
				{
					return E_UNEXPECTED ;
				}
				//Src Write:
				if( WritePos64(buf_dest, dwOffsetSrc, dwLength) != dwLength )
				{
					return E_UNEXPECTED ;
				}
				//Dest Write:
				if( WritePos64(buf_src, dwOffsetDest, dwLength) != dwLength )
				{
					return E_UNEXPECTED ;
				}
			}
			return S_OK ;
		}
	};

	template < class T >
	static FORCEINLINE void HexDump( std::string& str, const T * tp, size_t cb )
	{
		const void * p = (const void * )tp;
		char ch[6] = {};
		for( size_t i = 0; i < cb; ++ i )
		{
			sprintf( ch, "%02X", ((const UINT8*)p)[i] );
			str.append( ch );
		}
	}
	template < class T >
	static FORCEINLINE void HexDump( std::wstring& str, const T * tp, size_t cb )
	{
		const void * p = (const void * )tp;
		wchar_t ch[6] = {};
		for( size_t i = 0; i < cb; ++ i )
		{
			swprintf( ch, sizeof(ch), L"%02X", ((const UINT8*)p)[i] );
			str.append( ch );
		}
	}
	template < class XCHAR >
	static FORCEINLINE bool hex2u4( XCHAR aHex, UINT8& v )
	{
		if( aHex >= '0' && aHex <= '9' )
			v = (UINT8)(aHex - '0');
		else if( aHex >= 'a' && aHex <= 'f' )
			v = (UINT8)(aHex - 'a' + 10);
		else if( aHex >= 'A' && aHex <= 'F' )
			v = (UINT8)(aHex - 'A' + 10);
		else
			return false;
		return true;
	}
	template < class XCHAR >
	static FORCEINLINE bool hex2u8( const XCHAR* aHex, UINT8& v )
	{
		UINT8 h, l;
		if( !hex2u4( aHex[0], h ) || !hex2u4( aHex[1], l ) )
			return false;
		v = (h << 4) + l;
		return true;
	}

	template < class XCHAR, class Itor >
	static FORCEINLINE bool hex2bin_( const XCHAR* pstr, size_t len, Itor itb )
	{
		if( !len ) len = tcslen(pstr);
		if( len % 1 ) return false;
		for( size_t i = 0; i < len; i += 2, ++ itb )
		{
			UINT8 v8 = 0;
			if( !hex2u8( pstr + i, v8 ) ) return false;
			*itb = v8;
		}
		return TRUE;
	}

	template < class XCHAR >
	static FORCEINLINE bool hex2bin( const XCHAR* pstr, size_t len, std::vector<UINT8>& bbin )
	{
		if( !len ) len = tcslen(pstr);
		if( len % 1 ) return false;
		for( size_t i = 0; i < len; i += 2 )
		{
			UINT8 v8 = 0;
			if( !hex2u8( pstr + i, v8 ) ) return false;
			bbin.push_back( v8 );
		}
		return TRUE;
	}

	static FORCEINLINE bool hex2bin( std::string& str, std::vector<UINT8>& bbin )
	{
		return hex2bin( str.c_str(), str.size(), bbin );
	}
	static FORCEINLINE bool hex2bin( std::wstring& str, std::vector<UINT8>& bbin )
	{
		return hex2bin( str.c_str(), str.size(), bbin );
	}


	static FORCEINLINE BOOL IsUpackedFile( IREStream* pstm )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		RASSERT( prop, E_NOINTERFACE );
		const PROPVARIANT * pvar = 0 ;//
		return SUCCEEDED(prop->GetProperty(PID_File_IsUnpacked, &pvar) );
	}

	static FORCEINLINE HRESULT MarkUnpacked( IREStream* pstm )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		RASSERT( prop, E_NOINTERFACE );
		PROPVARIANT var;
		var.vt = VT_UI4;
		return prop->SetProperty( PID_File_IsUnpacked, &var );
	}

	template < SIZE_T FRAME_BUFF_SIZE >
	static FORCEINLINE HRESULT FileToFile(IREStream *deststm, IREStream *srcstm, UINT64 dwOffsetDest, UINT64 dwOffsetSrc, UINT64 uSize)
	{
		UINT8* _p_ = (new UINT8[FRAME_BUFF_SIZE]);
		UTIL::sentry<UINT8*,UTIL::_DAS> kBuffer( _p_ );
		if( !kBuffer ) return E_OUTOFMEMORY;

		LARGE_INTEGER liDistDest, liDistSrc;

		liDistDest.QuadPart = dwOffsetDest;
		liDistSrc.QuadPart = dwOffsetSrc;

		RFAILED(deststm->Seek( liDistDest.u.LowPart, &liDistDest.u.HighPart, FILE_BEGIN));
		RFAILED(srcstm->Seek( liDistSrc.u.LowPart, &liDistSrc.u.HighPart, FILE_BEGIN));

		while( uSize > 0 )
		{
			DWORD dwToRead = (uSize > FRAME_BUFF_SIZE) ? FRAME_BUFF_SIZE : (DWORD)uSize;
			DWORD cbRead = 0;
			RFAILED(srcstm->Read(kBuffer.m_p, dwToRead, &cbRead));
			if (0 == cbRead) return S_FALSE;
			RFAILED(deststm->Write(kBuffer.m_p, cbRead, NULL));
			uSize -= cbRead;
		}
		return S_OK;
	}

	template < SIZE_T FRAME_BUFF_SIZE >
	static FORCEINLINE HRESULT CopyStream(IREStream *deststm, IREStream *srcstm )
	{
		ez::File from(srcstm);
		UINT64 uFileSize = 0;
		RFAILED( from.GetSize64(uFileSize) );
		return FileToFile<FRAME_BUFF_SIZE>(deststm,srcstm,0,0,uFileSize);
	}

	template<class XCHAR>
	static FORCEINLINE HRESULT DumpStreamToFile(IREStream* pstm , const XCHAR* name)
	{
		RASSERT(pstm && name , E_INVALID_LICENSE);

		UTIL::com_ptr<IRXFileSystem> fs;
		RFAILED(ez::CreateInstance(fs , CLSID_LocalFileSystem));

		UTIL::com_ptr<IREStream>  target;
		RFAILED(ez::XCreateStream(target , fs , name , OMAlways|FOMRW));

		RFAILED(ez::CopyStream<0x1000>(target , pstm));

		return S_OK;
	}

	template < SIZE_T FRAME_BUFF_SIZE >
	static FORCEINLINE HRESULT FileToFile(IREStream *deststm, IREStream *srcstm, UINT64 dwOffsetSrc, UINT64 uSize)
	{
		UTIL::sentry<UINT8*,UTIL::_DAS> kBuffer; kBuffer = RSNEW UINT8[FRAME_BUFF_SIZE];
		if( !kBuffer ) return E_OUTOFMEMORY;

		LARGE_INTEGER liDistSrc;
		liDistSrc.QuadPart = dwOffsetSrc;
		RFAILED(srcstm->Seek( liDistSrc.u.LowPart, &liDistSrc.u.HighPart, FILE_BEGIN));

		while( uSize > 0 )
		{
			DWORD dwToRead = (uSize > FRAME_BUFF_SIZE) ? FRAME_BUFF_SIZE : (DWORD)uSize;
			DWORD cbRead = 0;
			RFAILED(srcstm->Read(kBuffer.m_p, dwToRead, &cbRead));
			if (0 == cbRead) return E_FAIL;
			DWORD cbWritten = 0;
			RFAILED(deststm->Write(kBuffer.m_p, cbRead, &cbWritten));
			RASSERT( cbWritten == cbRead, E_FAIL );
			uSize -= cbRead;
		}
		return S_OK;
	}

	template < SIZE_T FRAME_BUFF_SIZE >
	static FORCEINLINE HRESULT CompareStream(IREStream *left, IREStream *right, INT64& diff )
	{
		diff = 0;
		UTIL::sentry<UINT8*,UTIL::_DAS> kBuffer; kBuffer = RSNEW UINT8[FRAME_BUFF_SIZE*2];
		if( !kBuffer ) return E_OUTOFMEMORY;

		File file1(left), file2(right);

		UINT64 fs1 = 0, fs2 = 0;
		RFAILED( file1.GetSize64(fs1) );
		RFAILED( file2.GetSize64(fs2) );

		diff = (fs1 - fs2);
		if( diff ) return S_OK;

		RFAILED( file1.Seek(0) );
		RFAILED( file2.Seek(0) );

		while( fs1 > 0 )
		{
			DWORD cbComp = (fs1 > FRAME_BUFF_SIZE) ? FRAME_BUFF_SIZE : (DWORD)fs1;
			RFAILED( file1.ReadBC( &kBuffer[0], cbComp ) );
			RFAILED( file2.ReadBC( &kBuffer[FRAME_BUFF_SIZE], cbComp ) );

			diff = memcmp( &kBuffer[0], &kBuffer[FRAME_BUFF_SIZE], cbComp );
			if(diff) return S_OK;

			fs1 -= cbComp;
		}
		return S_OK;
	}


	template < SIZE_T FRAME_BUFF_SIZE, class ChunkPU >
	static FORCEINLINE HRESULT FileChunksTo(IREStream *srcstm, UINT64 dwOffsetSrc, UINT64 uSize, ChunkPU& pu )
	{
		UTIL::sentry<UINT8*,UTIL::_DAS> kBuffer; kBuffer = RSNEW UINT8[FRAME_BUFF_SIZE];
		if( !kBuffer ) return E_OUTOFMEMORY;

		LARGE_INTEGER liDistSrc;
		liDistSrc.QuadPart = dwOffsetSrc;
		RFAILED(srcstm->Seek( liDistSrc.u.LowPart, &liDistSrc.u.HighPart, FILE_BEGIN));

		while( uSize > 0 )
		{
			DWORD dwToRead = (uSize > FRAME_BUFF_SIZE) ? FRAME_BUFF_SIZE : (DWORD)uSize;
			DWORD cbRead = 0;
			RFAILED(srcstm->Read(kBuffer.m_p, dwToRead, &cbRead));
			if (0 == cbRead) break;
			pu( kBuffer.m_p, cbRead );
			uSize -= cbRead;
		}
		return S_OK;
	}

	// HRESULT CheckPU()
	template < SIZE_T FRAME_BUFF_SIZE, class ChunkPU >
	static FORCEINLINE HRESULT FileChunksTo2(IREStream *srcstm, UINT64 dwOffsetSrc, UINT64 uSize, ChunkPU& pu )
	{
		UTIL::sentry<UINT8*,UTIL::_DAS> kBuffer; kBuffer = RSNEW UINT8[FRAME_BUFF_SIZE];
		if( !kBuffer ) return E_OUTOFMEMORY;

		ULARGE_INTEGER uFileSize;
		RFAILED(srcstm->GetSize( &uFileSize.u.LowPart, &uFileSize.u.HighPart ));

		if( dwOffsetSrc >= uFileSize.QuadPart ) return S_FALSE;
		LARGE_INTEGER liDistSrc;
		liDistSrc.QuadPart = dwOffsetSrc;
		RFAILED(srcstm->Seek( liDistSrc.u.LowPart, &liDistSrc.u.HighPart, FILE_BEGIN));
		UINT64 uRest = uFileSize.QuadPart - dwOffsetSrc;
		uSize = MIN( uRest, uSize );

		while( uSize > 0 )
		{
			DWORD dwToRead = (uSize > FRAME_BUFF_SIZE) ? FRAME_BUFF_SIZE : (DWORD)uSize;
			DWORD cbRead = 0;
			RFAILED(srcstm->Read(kBuffer.m_p, dwToRead, &cbRead));
			if (0 == cbRead) break;
			RFAILED( pu( kBuffer.m_p, cbRead ) );
			uSize -= cbRead;
		}
		return S_OK;
	}

	template< class I >
	static FORCEINLINE HRESULT OpenRegionFileRO( UTIL::com_ptr<I>& oFile, IREStream * lpstm, UINT64 uOffset, SIZE_T uSize )
	{
		REGION64 oRegion = { uOffset, uSize };
		return CreateRegionFile(oFile,(LPCSTR)NULL,lpstm,&oRegion);
		//UTIL::com_ptr<IREStreamEx> vf;
		//RFAILED( ez::CreateInstance( vf, CLSID_VirtualFile ) );
		//RFAILED( ez::FileToFile<0x8000>( vf, lpstm, uOffset, uSize ) );
		//oFile = vf;
		//RASSERT( oFile, E_NOINTERFACE );
		//vf->SetAccess( FOMR );
		//return S_OK;
	}
	template< class I >
	static FORCEINLINE HRESULT CopyToVirtualFile( UTIL::com_ptr<I>& oFile, IREStream * lpstm, UINT64 uOffset, SIZE_T uSize )
	{
		UTIL::com_ptr<IREStreamEx> vf;
		RFAILED( ez::CreateInstance( vf, CLSID_VirtualFile ) );
		RFAILED( ez::FileToFile<0x8000>( vf, lpstm, uOffset, uSize ) );
		oFile = vf;
		RASSERT( oFile, E_NOINTERFACE );
		return S_OK;
	}
	template < class T >
	HRESULT ParameterGet( IRXParameters* op, LPCSTR path, T& val )
	{
		xv::xvalue_t tmp;
		RFAILED( op->GetParam( path, tmp ) );
		return tmp.get(val) ? S_OK : E_FAIL;
	}
	template < class T >
	HRESULT ParameterSet( IRXParameters* op, LPCSTR path, T val )
	{
		xv::xvalue_t v(val);
		return op->SetParam(path,v);
	}

	template < class T >
	static inline bool hex2int( LPCSTR lpHex, LPCSTR lpHexEnd, T & uValue )
	{
		for( ; *lpHex && lpHex < lpHexEnd; ++ lpHex, uValue <<= 4 )
		{
			CHAR ch = *lpHex;
			if( ch >= '0' && ch <= '9' )
				uValue += (ch - '0');
			else if( ch >= 'a' && ch <= 'f' )
				uValue += ((ch - 'a') + 10);
			else if( ch >= 'A' && ch <= 'F' )
				uValue += ((ch - 'A') + 10);
			else
				return false;
		}
		return true;
	}
	template < class T >
	static inline bool hex2int( LPCSTR lpHex, T & uValue )
	{
		for( ; *lpHex; ++ lpHex, uValue <<= 4 )
		{
			CHAR ch = *lpHex;
			if( ch >= '0' && ch <= '9' )
				uValue += (ch - '0');
			else if( ch >= 'a' && ch <= 'f' )
				uValue += ((ch - 'a') + 10);
			else if( ch >= 'A' && ch <= 'F' )
				uValue += ((ch - 'A') + 10);
			else
				return false;
		}
		return true;
	}
	template < class T >
	static inline bool dec2int( LPCSTR lpDec, LPCSTR lpDecEnd, T & uValue )
	{
		if( lpDec >= lpDecEnd ) return false;
		for(;;)
		{
			CHAR ch = *lpDec;
			if( ch >= '0' && ch <= '9' )
				uValue += (ch - '0');
			else
				return false;
			++ lpDec;

			if( !(*lpDec) ) break;

			if( !(lpDec < lpDecEnd) ) break;

			uValue*=10;
		}
		return true;
	}
	template < class T >
	static inline bool dec2int( LPCSTR lpDec, T & uValue )
	{
		for(;;)
		{
			CHAR ch = *lpDec;
			if( ch >= '0' && ch <= '9' )
				uValue += (ch - '0');
			else
				return false;

			++ lpDec;

			if( !(*lpDec) ) break;

			uValue*=10;
		}
		return true;
	}

	static FORCEINLINE HRESULT MarkWithProperty( IREPropertyEx * prop, REFCLSID rpid, SIZE_T uMask = 0 )
	{
		RFAILED( prop->SetProperty( rpid, 0 ) );
		if(!uMask) return S_OK;
		return( prop->MarkProperty( PID_File_HasKnownPacker, PROP_EXPLICIT_DEL ) );
	}
	static FORCEINLINE BOOL PropertyMarked( IREPropertyEx * prop, REFCLSID rpid )
	{
		return SUCCEEDED(prop->GetProperty(rpid, 0 ));
	}
	static FORCEINLINE HRESULT MarkPackedFile( IREPropertyEx * prop )
	{
		RFAILED( prop->SetProperty( PID_File_HasKnownPacker, 0 ) );
		return ( prop->MarkProperty( PID_File_HasKnownPacker, PROP_EXPLICIT_DEL ) );
	}

	static FORCEINLINE HRESULT MarkPackerJudged( IREPropertyEx * prop )
	{
		RFAILED( prop->SetProperty( PID_File_PackerJudeged, 0 ) );
		return ( prop->MarkProperty( PID_File_PackerJudeged, PROP_EXPLICIT_DEL ) );
	}

	// <0 fail
	// 0  NOT Packed
	// 1  Packed
	static FORCEINLINE LONG IsPackedFile( IREPropertyEx * prop )
	{
		RASSERT( prop, E_INVALIDARG );
		RFAILED( prop->GetProperty( PID_File_PackerJudeged, 0 ) );
		return SUCCEEDED( prop->GetProperty( PID_File_HasKnownPacker, 0 ) ) ? 1 : 0;
	}
	static FORCEINLINE LONG IsPackedFile( IREStream * pstm )
	{
		UTIL::com_ptr<IREPropertyEx> prop(pstm);
		return IsPackedFile( prop );
	}
	static FORCEINLINE BOOL IsUnpackedFile( IREPropertyEx * prop )
	{
		return SUCCEEDED(prop->GetProperty( PID_File_IsUnpacked, 0 ));
	}
	static FORCEINLINE BOOL IsUnpackedFile( IREStream * pstm )
	{
		UTIL::com_ptr<IREPropertyEx> prop(pstm);
		return IsUnpackedFile( prop );
	}

	static FORCEINLINE HRESULT GetNameDB( UTIL::com_ptr<IRXNameDB>& oPtr, IUnknown * lpRotOrNameDB )
	{
		if( !lpRotOrNameDB ) return E_FAIL;
		oPtr = lpRotOrNameDB;
		if( oPtr ) return S_OK;
		return ez::RunTimeGet( oPtr, lpRotOrNameDB, PID_ROT_NameDB );
	}

	static FORCEINLINE HRESULT MarkStreamNeedSubmit( IREProperty* prop, BOOL fSubmit = TRUE )
	{
		if(!prop) return E_FAIL;
		prop->DeleteProperty( PID_File_NeedSubmit );
		if( !fSubmit ) return S_OK;
		return prop->SetProperty( PID_File_NeedSubmit, 0 );
	}

	static FORCEINLINE HRESULT MarkStreamNeedSubmit( IREStream* pstm, BOOL fSubmit = TRUE )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		return MarkStreamNeedSubmit( prop, fSubmit );
	}

	static BOOL StreamNeedToSubmit( IREProperty* prop )
	{
		if( !prop ) return FALSE;
		return SUCCEEDED( prop->GetProperty( PID_File_NeedSubmit, NULL ) );
	}
	static BOOL StreamNeedToSubmit( IREStream* pstm )
	{
		UTIL::com_ptr<IREProperty> prop(pstm);
		return StreamNeedToSubmit( prop );
	}

	static FORCEINLINE HRESULT CommonHandleNotMalwareRecord( 
		IREPropertyEx * lpprop, 
		UINT8 uRecType, 
		SGID40 uId40,
		IUnknown * lpNamDB_Rot,
		IRXDTDataBox * lpBox = 0 )
	{
		if( nam::IsMalwareClass(uRecType) ) return S_FALSE;
		LPCSTR aMain = MainClassName(uRecType);
		if( !aMain ) return E_FAIL;

		UTIL::com_ptr<IRXDTDataBox> oBox(lpBox);
		if( !oBox && lpprop ) ez::GetAuxDataBox( oBox, lpprop );

		if( lpprop )
		{
			if( uRecType == nam::kTrusted ) 
			{
				PROPVARIANT var; var.vt = VT_BOOL; var.boolVal = VARIANT_TRUE;
				lpprop->SetProperty( PID_TROJANLIB_WHITELIST, &var );
			}
			else if( uRecType == nam::kPacker )
			{
				MarkPackerJudged(lpprop);
				MarkPackedFile(lpprop);
			}
			else if( uRecType == nam::kComplier )
			{
				MarkWithProperty( lpprop, PID_File_HasKnownCompiler );
			}
			else if( uRecType == nam::kAppType )
			{
				MarkWithProperty( lpprop, PID_File_HasKnownAppType );
			}
		}
		if( uId40.Id32 && oBox )
		{
			CHAR szTempName[512];
			if( nam::IsHiddenClass( uRecType ) )
			{
				oBox->PushHiddenAlarm( uId40 );
				if( nam::IsHiddenClassNeedFile(uRecType) )
					MarkStreamNeedSubmit(lpprop);
			}
			UTIL::com_ptr<IRXNameDB> oNameDB;
			if( SUCCEEDED( GetNameDB( oNameDB, lpNamDB_Rot ) ) &&
				SUCCEEDED( oNameDB->GetNameById40( uId40, szTempName, sizeof(szTempName) ) ) )
			{
				oBox->DTFCPush( szTempName, rs_strlen(szTempName) );
			}
			else
			{
				std::string tmp = aMain;
				tmp.append( ".[Invalid]" );
				oBox->DTFCPush( tmp.c_str(), tmp.size() );
			}
		}
		return S_OK;
	}
	static FORCEINLINE HRESULT CommonHandleNotMalwareRecord( 
		IREStream * lpstm, 
		UINT8 uRecType, 
		SGID40 uId40,
		IUnknown * lpNamDB_Rot,
		IRXDTDataBox * lpBox = 0)
	{
		UTIL::com_ptr<IREPropertyEx> prop(lpstm);
		RASSERT( prop, E_FAIL );
		return CommonHandleNotMalwareRecord( prop, uRecType, uId40, lpNamDB_Rot, lpBox );
	}
	static FORCEINLINE HRESULT CommonHandleNotMalwareRecord( 
		IRXArchive * arch, 
		UINT8 uRecType, 
		SGID40 uId40,
		IUnknown * lpNamDB_Rot,
		IRXDTDataBox * lpBox = 0)
	{
		UTIL::com_ptr<IREStream> ostm;
		RFAILED( arch->GetStreamObject( ostm.pp() ) );
		return CommonHandleNotMalwareRecord(ostm, uRecType, uId40, lpNamDB_Rot, lpBox );
	}
	static FORCEINLINE HRESULT RemoveByArchiveOrStream( IUnknown * p )
	{
		UTIL::com_ptr<IREStream> tmp( p );
		if( !tmp ) 
		{
			UTIL::com_ptr<IRXArchive> arch( p );
			if( !arch ) return E_NOINTERFACE;
			RFAILED( arch->GetStreamObject( tmp.pp() ) );
		}
		return tmp->Remove();
	}

	static FORCEINLINE HRESULT GetFullNameW( std::wstring& rFullName, IRXStream * lpStream )
	{
		std::vector< std::wstring > names;
		UTIL::com_ptr<IRXStream> oCurrent = lpStream;
		while( oCurrent )
		{
			XSTRP strp;
			RFAILED( oCurrent->GetName( strp ) );
			if( strp.IsUnicode ) 
			{
				names.push_back( strp.NameW );
			}
			else
			{
				tcs::xstr_t xstr;
				if( !xstr.set(strp) ) return E_FAIL;
				names.push_back( xstr.StrWCH );
			}
			UTIL::com_ptr<IREStream> oParentStream;
			UTIL::com_ptr<IRXArchive> oParent;
			if( FAILED( oCurrent->GetParent( oParent.pp() ) ) ) break;
			RFAILED( oParent->GetStreamObject( oParentStream.pp() ) );
			oCurrent = oParentStream;
		}
		size_t k = names.size();
		for( size_t i = 0; i < k; ++ i )
		{
			rFullName.append( names[k-1-i] );
			if( k - i > 1 ) rFullName.append( L">>" );
		}
		return S_OK;
	}
	static FORCEINLINE HRESULT GetFullNameA( std::string& rFullName, IRXStream * lpStream )
	{
		std::vector< std::string > names;
		UTIL::com_ptr<IRXStream> oCurrent = lpStream;
		while( oCurrent )
		{
			XSTRP strp;
			RFAILED( oCurrent->GetName( strp ) );
			if( !strp.IsUnicode ) 
			{
				names.push_back( strp.NameA );
			}
			else
			{
				tcs::xstr_t xstr;
				if( !xstr.set(strp) ) return E_FAIL;
				names.push_back( xstr.StrMBS );
			}
			UTIL::com_ptr<IREStream> oParentStream;
			UTIL::com_ptr<IRXArchive> oParent;
			if( FAILED( oCurrent->GetParent( oParent.pp() ) ) ) break;
			RFAILED( oParent->GetStreamObject( oParentStream.pp() ) );
			oCurrent = oParentStream;
		}
		size_t k = names.size();
		for( size_t i = 0; i < k; ++ i )
		{
			rFullName.append( names[k-1-i] );
			if( k - i > 1 ) rFullName.append( ">>" );
		}
		return S_OK;
	}

	template < class I >
	static FORCEINLINE HRESULT GetActualStream( UTIL::com_ptr<I>& ip, IUnknown * lpLogicFile )
	{
		UTIL::com_ptr<IREStream> bs;
		UTIL::com_ptr<IRXLogicFile> oLogicFile( lpLogicFile );
		if( !oLogicFile ) 
		{
			ip = lpLogicFile;
		}
		else if( SUCCEEDED( oLogicFile->GetBackStream( bs.pp(), NULL ) ) )
		{
			ip = bs;
		}
		RASSERT( ip, E_FAIL );
		return S_OK;
	}

	static HRESULT GetImportantMetaInfo( IUnknown * pstm, xv::xvalue_t & obj, BOOL bForce )
	{
		LPCSTR KEY_NAME = "STM_IMPORTANT_META";
		UTIL::com_ptr<IRXDTDataBox> box;
		if( bForce ) { 
			RFAILED( ez::GetAuxDataBox2(box, pstm) );
		} else {
			RFAILED( ez::GetAuxDataBox(box, pstm) );
		}
		xv::xvalue_t important;
		if( FAILED( box->DTXVGet(KEY_NAME, important) ) || !important.isObject() )
		{
			if( !bForce ) return E_FAIL;
			if( !important.asObject(true) ) return E_OUTOFMEMORY;
			RFAILED( box->DTXVSet(KEY_NAME, important) );
		}
		obj = important;
		return S_OK;
	}
	
};

typedef ez::File	CFileAccessor;

#endif

