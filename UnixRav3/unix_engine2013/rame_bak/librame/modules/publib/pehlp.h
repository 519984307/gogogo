#ifndef __RC_HELPER__
#define __RC_HELPER__

#include "matching.h"

#if 1
struct RcHelper
{
	STATICINLINE 
	LPPRC GetRootCatelog( I12PEArchive * RcTable )
	{
		LPPRC lpRootPRC = RcTable->GetRcCatelog( RcTable->GetRootRVA() );
		return lpRootPRC;
	}

	STATICINLINE 
	LPPRD FindRcDataByCRC( I12PEArchive * RcTable, uint32_t CRC )
	{
		uint32_t c = 1;
		for( uint32_t i = 0; i < c; ++ i )
		{
			LPPRD lpPRD = 0;
			c = RcTable->EnumRcData( i, &lpPRD );
			if( !c ) break;
			if( CRC == lpPRD->FullPathName.StdCRC || CRC == lpPRD->FullPathName.StdCRCW )
				return lpPRD;
		}
		return 0;
	}

	STATICINLINE
	LPPRD FindRcDataByCRC_R( I12PEArchive * RcTable, uint32_t CRC )
	{
		uint32_t c = 1;
		for( uint32_t i = 0; i < c; ++ i )
		{
			LPPRD lpPRD = 0;
			c = RcTable->EnumRcData( i, &lpPRD );
			if( !c ) break;
			if( CRC == lpPRD->FullPathNameR.StdCRC || CRC == lpPRD->FullPathNameR.StdCRCW )
				return lpPRD;
		}
		return 0;
	}

	STATICINLINE 
	LPPRD FindRcDataByPatten( I12PEArchive * RcTable, const char * Patten, bool usePathR = false )
	{
		uint32_t c = 1;
		for( uint32_t i = 0; i < c; ++ i )
		{
			LPPRD lpPRD = 0;
			c = RcTable->EnumRcData( i, &lpPRD );
			if( !c ) break;
			const char * lpDataFullPath = 0;
			if( usePathR )
				lpDataFullPath = lpPRD->FullPathNameR.NamePtr;
			else
				lpDataFullPath = lpPRD->FullPathName.NamePtr;

			if( ! lpDataFullPath ) continue;	

			const char * LookupEnd = lpDataFullPath + strlen(lpDataFullPath);
			const char * LookinEnd = Patten + strlen(Patten);
			if( iwStmStm( lpDataFullPath, LookupEnd, Patten, LookinEnd ) == LookupEnd )
				continue;
			return lpPRD;
		}
		return 0;
	}

	STATICINLINE
	uint32_t QueryWin32TypedDataCount( I12PEArchive * RcTable, uint32_t Type )
	{
		LPPRC lpRootPRC = GetRootCatelog( RcTable );
		if( !lpRootPRC ) return 0;
		if( ! (lpRootPRC->CountOfSubCatelog) ) return 0;
		typedef std::vector< LPPREI > PREIVector;
		PREIVector TypeCatelog;
		TypeCatelog.resize( lpRootPRC->CountOfSubCatelog );
		uint32_t cnt = RcTable->EnumChildren( lpRootPRC, ENUM_CATELOG, &(TypeCatelog[0]),  TypeCatelog.size() );
		if( !cnt ) return 0;
		uint32_t ret = 0;
		for( size_t i = 0; i < cnt; ++ i )
		{
			LPPRC CatelogPtr = (LPPRC)(TypeCatelog[i]);
			if( CatelogPtr->EntRaw.NameIsString )
				continue;
			uint32_t CatelogID = CatelogPtr->EntRaw.Id;
			if( (Type == -1 && CatelogID < 32 ) || (Type == CatelogPtr->EntRaw.Id) )
				ret += CatelogPtr->CountOfSubCatelog;
		}
		return ret;
	}
};
#endif
//////////////////////////////////////////////////////////////////////////


namespace ITHelper
{
	STATICINLINE
	uint32_t GetTotalSymbol( I12PEArchive * Archive )
	{
		return Archive->EnumSymbol( 0, 0 );
	}
	STATICINLINE
	uint32_t GetTotalLibrary( I12PEArchive * Archive )
	{
		return Archive->EnumLibrary(0,0);
	}
	STATICINLINE
	LPPIL FindLibByCRC( I12PEArchive * Archive, uint32_t CRCOfLibNameI )
	{
		uint32_t c = 1;
		for( uint32_t i = 0; i < c; ++ i )
		{
			LPPIL lpPil = 0;
			c = Archive->EnumLibrary( i, &lpPil );

			if( !c ) 
				break;

			if( CRCOfLibNameI == lpPil->CRCOfNameI )
				return lpPil;
		}
		return NULL;
	}

	STATICINLINE
	LPPIS32 FindSymbolByName( I12PEArchive * Archive,  uint32_t CRCOfName, LPPIL lpLibrary )
	{
		if( ! lpLibrary )
		{
			uint32_t c = 1;
			for( uint32_t i = 0; i < c; ++ i )
			{
				LPPIS32 lpPis = 0;
				c = Archive->EnumSymbol( i, &lpPis );
				if( !c ) break;
				if( CRCOfName == lpPis->CRCOfNameI || CRCOfName == lpPis->CRCOfName )
					return lpPis;
			}
		}
		else
		{
			uint32_t SymbsT = lpLibrary->CountOfSymbolsN + lpLibrary->CountOfSymbolsO;
			if( !SymbsT ) return 0;

			std::vector<LPPIS32> Symbols;
			Symbols.resize( SymbsT );
			
			SymbsT = Archive->EnumSymbolOfLibrary( lpLibrary, ENUM_NAME_SYMBOL|ENUM_ORDINAL_SYMBOL, &(Symbols[0]), SymbsT );
			if( !SymbsT ) return 0;

			for( uint32_t i = 0; i < SymbsT; ++ i )
			{
				LPPIS32 lpPis = Symbols[i];
				if( CRCOfName == lpPis->CRCOfNameI || CRCOfName == lpPis->CRCOfName )
					return lpPis;
			}
		}
		return NULL;
	}
};

//////////////////////////////////////////////////////////////////////////

namespace ETHelper
{
	STATICINLINE LPPESI FindSymbolByName( I12PEArchive * lpPEArchive, uint32_t crc )
	{
		LPPELI lpLibInfo = lpPEArchive->GetLibraryInfo();
		if( !lpLibInfo ) return NULL;
		for( uint32_t i = 0; i < lpLibInfo->ExportedSymbols; ++ i )
		{
			LPPESI lpPESI = lpPEArchive->EnumExportedSymbols( i );
			if( !lpPESI ) return NULL;
			if( crc == lpPESI->CRCOfName || crc == lpPESI->CRCOfNameI )
				return lpPESI;
		}
		return NULL;
	}
	STATICINLINE LPPESI FindSymbolByOrdinal( I12PEArchive * lpPEArchive, uint16_t uiOrdinal )
	{
		LPPELI lpLibInfo = lpPEArchive->GetLibraryInfo();
		if( !lpLibInfo ) return NULL;
		for( uint32_t i = 0; i < lpLibInfo->ExportedSymbols; ++ i )
		{
			LPPESI lpPESI = lpPEArchive->EnumExportedSymbols( i );
			if( !lpPESI ) return NULL;
			if( uiOrdinal == lpPESI->Ordinal || uiOrdinal == lpPESI->Ordinal )
				return lpPESI;
		}
		return NULL;
	}
	STATICINLINE LPPESI FindSymbolBySubString( I12PEArchive * lpPEArchive, const char * lpSubString )
	{
		LPPELI lpLibInfo = lpPEArchive->GetLibraryInfo();
		if( !lpLibInfo ) return NULL;
		for( uint32_t i = 0; i < lpLibInfo->ExportedSymbols; ++ i )
		{
			LPPESI lpPESI = lpPEArchive->EnumExportedSymbols( i );
			if( !lpPESI ) return NULL;
			if( ! lpPESI->NameRVA ) continue;
			if( ! lpPESI->NamePtr ) continue;
			if( strstr( lpPESI->NamePtr, lpSubString ) )
				return lpPESI;
		}
		return NULL;
	}
};

#endif