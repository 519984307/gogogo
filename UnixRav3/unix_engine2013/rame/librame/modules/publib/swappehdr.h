#ifndef __SWAP_PE_HEADER__
#define __SWAP_PE_HEADER__

/*
===========================================================
Swap_Struct_XX (For Swap Struct)
===========================================================
*/
inline void Swap_Struct_DOS_HEADER( winpe::IMAGE_DOS_HEADER * pDosHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pDosHeader->e_magic      = _BSWAP16( pDosHeader->e_magic ) ;
	pDosHeader->e_cblp       = _BSWAP16( pDosHeader->e_cblp ) ;
	pDosHeader->e_cp         = _BSWAP16( pDosHeader->e_cp ) ;
	pDosHeader->e_crlc       = _BSWAP16( pDosHeader->e_crlc ) ;
	pDosHeader->e_cparhdr    = _BSWAP16( pDosHeader->e_cparhdr ) ;
	pDosHeader->e_minalloc   = _BSWAP16( pDosHeader->e_minalloc ) ;
	pDosHeader->e_maxalloc   = _BSWAP16( pDosHeader->e_maxalloc ) ;
	pDosHeader->e_ss         = _BSWAP16( pDosHeader->e_ss ) ;
	pDosHeader->e_sp         = _BSWAP16( pDosHeader->e_sp ) ;
	pDosHeader->e_csum       = _BSWAP16( pDosHeader->e_csum ) ;
	pDosHeader->e_ip         = _BSWAP16( pDosHeader->e_ip ) ;
	pDosHeader->e_cs         = _BSWAP16( pDosHeader->e_cs ) ;
	pDosHeader->e_lfarlc     = _BSWAP16( pDosHeader->e_lfarlc ) ;
	pDosHeader->e_ovno       = _BSWAP16( pDosHeader->e_ovno ) ;
	//pDosHeader->e_res[4]   = _BSWAP16( pDosHeader->e_res[4] ) ;
	pDosHeader->e_oemid      = _BSWAP16( pDosHeader->e_oemid ) ;
	pDosHeader->e_oeminfo    = _BSWAP16( pDosHeader->e_oeminfo ) ;
	//pDosHeader->e_res2[10] = _BSWAP16( pDosHeader->e_res2[10] ) ;
	pDosHeader->e_lfanew     = _BSWAP32( pDosHeader->e_lfanew ) ;	
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_NT_SIGNATURE( uint32_t * pNtSignature )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	*pNtSignature = _BSWAP32( *pNtSignature ) ;
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_FILE_HEADER( winpe::IMAGE_FILE_HEADER * pFileHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pFileHeader->Machine              = _BSWAP16( pFileHeader->Machine ) ;
	pFileHeader->NumberOfSections     = _BSWAP16( pFileHeader->NumberOfSections ) ;
	pFileHeader->TimeDateStamp        = _BSWAP32( pFileHeader->TimeDateStamp ) ;
	pFileHeader->PointerToSymbolTable = _BSWAP32( pFileHeader->PointerToSymbolTable ) ;
	pFileHeader->NumberOfSymbols      = _BSWAP32( pFileHeader->NumberOfSymbols ) ;
	pFileHeader->SizeOfOptionalHeader = _BSWAP16( pFileHeader->SizeOfOptionalHeader ) ;
	pFileHeader->Characteristics      = _BSWAP16( pFileHeader->Characteristics ) ;
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_OPTIONAL_HEADER32( winpe::IMAGE_OPTIONAL_HEADER32 * pOptionalHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pOptionalHeader->Magic                       = _BSWAP16( pOptionalHeader->Magic ) ;
//	pOptionalHeader->MajorLinkerVersion          = _BSWAP16( pOptionalHeader->MajorLinkerVersion ) ;
//	pOptionalHeader->MinorLinkerVersion          = _BSWAP16( pOptionalHeader->MinorLinkerVersion ) ;
	pOptionalHeader->SizeOfCode                  = _BSWAP32( pOptionalHeader->SizeOfCode ) ;
	pOptionalHeader->SizeOfInitializedData       = _BSWAP32( pOptionalHeader->SizeOfInitializedData ) ;
	pOptionalHeader->SizeOfUninitializedData     = _BSWAP32( pOptionalHeader->SizeOfUninitializedData ) ;
	pOptionalHeader->AddressOfEntryPoint         = _BSWAP32( pOptionalHeader->AddressOfEntryPoint ) ;
	pOptionalHeader->BaseOfCode                  = _BSWAP32( pOptionalHeader->BaseOfCode ) ;
	pOptionalHeader->BaseOfData                  = _BSWAP32( pOptionalHeader->BaseOfData ) ;
	pOptionalHeader->ImageBase                   = _BSWAP32( pOptionalHeader->ImageBase ) ;
	pOptionalHeader->SectionAlignment            = _BSWAP32( pOptionalHeader->SectionAlignment ) ;
	pOptionalHeader->FileAlignment               = _BSWAP32( pOptionalHeader->FileAlignment ) ;

	pOptionalHeader->MajorOperatingSystemVersion = _BSWAP16( pOptionalHeader->MajorOperatingSystemVersion ) ;
	pOptionalHeader->MinorOperatingSystemVersion = _BSWAP16( pOptionalHeader->MinorOperatingSystemVersion ) ;
	pOptionalHeader->MajorImageVersion           = _BSWAP16( pOptionalHeader->MajorImageVersion ) ;
	pOptionalHeader->MinorImageVersion           = _BSWAP16( pOptionalHeader->MinorImageVersion ) ;
	pOptionalHeader->MajorSubsystemVersion       = _BSWAP16( pOptionalHeader->MajorSubsystemVersion ) ;
	pOptionalHeader->MinorSubsystemVersion       = _BSWAP16( pOptionalHeader->MinorSubsystemVersion ) ;

	pOptionalHeader->Win32VersionValue           = _BSWAP32( pOptionalHeader->Win32VersionValue ) ;
	pOptionalHeader->SizeOfImage                 = _BSWAP32( pOptionalHeader->SizeOfImage ) ;
	pOptionalHeader->SizeOfHeaders               = _BSWAP32( pOptionalHeader->SizeOfHeaders ) ;
	pOptionalHeader->CheckSum                    = _BSWAP32( pOptionalHeader->CheckSum ) ;

	pOptionalHeader->Subsystem                   = _BSWAP16( pOptionalHeader->Subsystem ) ;
	pOptionalHeader->DllCharacteristics          = _BSWAP16( pOptionalHeader->DllCharacteristics ) ;

	pOptionalHeader->SizeOfStackReserve          = _BSWAP32( pOptionalHeader->SizeOfStackReserve ) ;
	pOptionalHeader->SizeOfStackCommit           = _BSWAP32( pOptionalHeader->SizeOfStackCommit ) ;
	pOptionalHeader->SizeOfHeapReserve           = _BSWAP32( pOptionalHeader->SizeOfHeapReserve ) ;
	pOptionalHeader->SizeOfHeapCommit            = _BSWAP32( pOptionalHeader->SizeOfHeapCommit ) ;
	pOptionalHeader->LoaderFlags                 = _BSWAP32( pOptionalHeader->LoaderFlags ) ;
	pOptionalHeader->NumberOfRvaAndSizes         = _BSWAP32( pOptionalHeader->NumberOfRvaAndSizes ) ;
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_OPTIONAL_HEADER64( winpe::IMAGE_OPTIONAL_HEADER64 * pOptionalHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pOptionalHeader->Magic                       = _BSWAP16( pOptionalHeader->Magic ) ;
	//	pOptionalHeader->MajorLinkerVersion      = _BSWAP16( pOptionalHeader->MajorLinkerVersion ) ;
	//	pOptionalHeader->MinorLinkerVersion      = _BSWAP16( pOptionalHeader->MinorLinkerVersion ) ;
	pOptionalHeader->SizeOfCode                  = _BSWAP32( pOptionalHeader->SizeOfCode ) ;
	pOptionalHeader->SizeOfInitializedData       = _BSWAP32( pOptionalHeader->SizeOfInitializedData ) ;
	pOptionalHeader->SizeOfUninitializedData     = _BSWAP32( pOptionalHeader->SizeOfUninitializedData ) ;
	pOptionalHeader->AddressOfEntryPoint         = _BSWAP32( pOptionalHeader->AddressOfEntryPoint ) ;
	pOptionalHeader->BaseOfCode                  = _BSWAP32( pOptionalHeader->BaseOfCode ) ;

	pOptionalHeader->ImageBase                   = _BSWAP64( pOptionalHeader->ImageBase ) ;

	pOptionalHeader->SectionAlignment            = _BSWAP32( pOptionalHeader->SectionAlignment ) ;
	pOptionalHeader->FileAlignment               = _BSWAP32( pOptionalHeader->FileAlignment ) ;
	pOptionalHeader->MajorOperatingSystemVersion = _BSWAP16( pOptionalHeader->MajorOperatingSystemVersion ) ;
	pOptionalHeader->MinorOperatingSystemVersion = _BSWAP16( pOptionalHeader->MinorOperatingSystemVersion ) ;
	pOptionalHeader->MajorImageVersion           = _BSWAP16( pOptionalHeader->MajorImageVersion ) ;
	pOptionalHeader->MinorImageVersion           = _BSWAP16( pOptionalHeader->MinorImageVersion ) ;
	pOptionalHeader->MajorSubsystemVersion       = _BSWAP16( pOptionalHeader->MajorSubsystemVersion ) ;
	pOptionalHeader->MinorSubsystemVersion       = _BSWAP16( pOptionalHeader->MinorSubsystemVersion ) ;

	pOptionalHeader->Win32VersionValue           = _BSWAP32( pOptionalHeader->Win32VersionValue ) ;
	pOptionalHeader->SizeOfImage                 = _BSWAP32( pOptionalHeader->SizeOfImage ) ;
	pOptionalHeader->SizeOfHeaders               = _BSWAP32( pOptionalHeader->SizeOfHeaders ) ;
	pOptionalHeader->CheckSum                    = _BSWAP32( pOptionalHeader->CheckSum ) ;

	pOptionalHeader->Subsystem                   = _BSWAP16( pOptionalHeader->Subsystem ) ;
	pOptionalHeader->DllCharacteristics          = _BSWAP16( pOptionalHeader->DllCharacteristics ) ;

	pOptionalHeader->SizeOfStackReserve          = _BSWAP64( pOptionalHeader->SizeOfStackReserve ) ;
	pOptionalHeader->SizeOfStackCommit           = _BSWAP64( pOptionalHeader->SizeOfStackCommit ) ;
	pOptionalHeader->SizeOfHeapReserve           = _BSWAP64( pOptionalHeader->SizeOfHeapReserve ) ;
	pOptionalHeader->SizeOfHeapCommit            = _BSWAP64( pOptionalHeader->SizeOfHeapCommit ) ;

	pOptionalHeader->LoaderFlags                 = _BSWAP32( pOptionalHeader->LoaderFlags ) ;
	pOptionalHeader->NumberOfRvaAndSizes         = _BSWAP32( pOptionalHeader->NumberOfRvaAndSizes ) ;
#	endif // RS_BIG_ENDIAN
}

inline void Swap_Struct_DATA_DIRECTORY32( winpe::IMAGE_OPTIONAL_HEADER32 * pOptionalHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	uint32_t NumberOfRvaAndSizes = pOptionalHeader->NumberOfRvaAndSizes > WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES ? 
		WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES : pOptionalHeader->NumberOfRvaAndSizes;
	for( uint32_t i = 0; i < NumberOfRvaAndSizes; ++ i )
	{
		winpe::IMAGE_DATA_DIRECTORY & DataDirEnt = pOptionalHeader->DataDirectory[i];
		DataDirEnt.VirtualAddress = _BSWAP32( DataDirEnt.VirtualAddress ) ;
		DataDirEnt.Size           = _BSWAP32( DataDirEnt.Size ) ;
	}
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_DATA_DIRECTORY64( winpe::IMAGE_OPTIONAL_HEADER64 * pOptionalHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	uint32_t NumberOfRvaAndSizes = pOptionalHeader->NumberOfRvaAndSizes > WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES ? 
		WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES : pOptionalHeader->NumberOfRvaAndSizes;
	for( uint32_t i = 0; i < NumberOfRvaAndSizes; ++ i )
	{
		winpe::IMAGE_DATA_DIRECTORY & DataDirEnt = pOptionalHeader->DataDirectory[i];
		DataDirEnt.VirtualAddress = _BSWAP32( DataDirEnt.VirtualAddress ) ;
		DataDirEnt.Size           = _BSWAP32( DataDirEnt.Size ) ;
	}
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_SECTION_HEADER( winpe::IMAGE_SECTION_HEADER* pSectionHeader )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	//pSectionHeader->Name[8]
	pSectionHeader->Misc.VirtualSize		  = _BSWAP32( pSectionHeader->Misc.VirtualSize ) ;
	pSectionHeader->VirtualAddress        = _BSWAP32( pSectionHeader->VirtualAddress ) ;
	pSectionHeader->SizeOfRawData         = _BSWAP32( pSectionHeader->SizeOfRawData ) ;
	pSectionHeader->PointerToRawData      = _BSWAP32( pSectionHeader->PointerToRawData ) ;
	pSectionHeader->PointerToRelocations  = _BSWAP32( pSectionHeader->PointerToRelocations ) ;
	pSectionHeader->PointerToLinenumbers  = _BSWAP32( pSectionHeader->PointerToLinenumbers ) ;
	pSectionHeader->NumberOfRelocations   = _BSWAP16( pSectionHeader->NumberOfRelocations ) ;
	pSectionHeader->NumberOfLinenumbers   = _BSWAP16( pSectionHeader->NumberOfLinenumbers ) ;
	pSectionHeader->Characteristics       = _BSWAP32( pSectionHeader->Characteristics ) ;
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_IMAGE_IMPORT_DESCRIPTOR( winpe::PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pImportDescriptor->Characteristics = _BSWAP32(pImportDescriptor->Characteristics);
	pImportDescriptor->FirstThunk = _BSWAP32(pImportDescriptor->FirstThunk);
	pImportDescriptor->ForwarderChain = _BSWAP32(pImportDescriptor->ForwarderChain);
	pImportDescriptor->Name = _BSWAP32(pImportDescriptor->Name);
	pImportDescriptor->OriginalFirstThunk = _BSWAP32(pImportDescriptor->OriginalFirstThunk);
	pImportDescriptor->TimeDateStamp = _BSWAP32(pImportDescriptor->TimeDateStamp);
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_IMAGE_EXPORT_DIRECTORY( winpe::IMAGE_EXPORT_DIRECTORY * pExportDirectory )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pExportDirectory->AddressOfFunctions = _BSWAP32(pExportDirectory->AddressOfFunctions);
	pExportDirectory->AddressOfNameOrdinals = _BSWAP32(pExportDirectory->AddressOfNameOrdinals);
	pExportDirectory->Base = _BSWAP32(pExportDirectory->Base);
	pExportDirectory->Characteristics = _BSWAP32(pExportDirectory->Characteristics);
	pExportDirectory->MajorVersion = _BSWAP16(pExportDirectory->MajorVersion);
	pExportDirectory->MinorVersion = _BSWAP16(pExportDirectory->MinorVersion);
	pExportDirectory->Name = _BSWAP32(pExportDirectory->Name);
	pExportDirectory->NumberOfFunctions = _BSWAP32(pExportDirectory->NumberOfFunctions);
	pExportDirectory->NumberOfNames = _BSWAP32(pExportDirectory->NumberOfNames);
	pExportDirectory->AddressOfNames = _BSWAP32(pExportDirectory->AddressOfNames);
	pExportDirectory->TimeDateStamp = _BSWAP32(pExportDirectory->TimeDateStamp);
#	endif // RS_BIG_ENDIAN
}
inline void Swap_Struct_IMAGE_COR20_HEADER( winpe::IMAGE_COR20_HEADER * pCor20Hdr )
{
#if TARGET_ENDIAN==BYTE_ENDIAN_B
	pCor20Hdr->cb = _BSWAP32(pCor20Hdr->cb);
	pCor20Hdr->MajorRuntimeVersion = _BSWAP16(pCor20Hdr->MajorRuntimeVersion);
	pCor20Hdr->MinorRuntimeVersion = _BSWAP16(pCor20Hdr->MinorRuntimeVersion);
	pCor20Hdr->MetaData.VirtualAddress = _BSWAP32(pCor20Hdr->MetaData.VirtualAddress);
	pCor20Hdr->MetaData.Size = _BSWAP32(pCor20Hdr->MetaData.Size);
	pCor20Hdr->Flags = _BSWAP32(pCor20Hdr->Flags);
	pCor20Hdr->DUMMYUNIONNAME.EntryPointRVA = _BSWAP32(pCor20Hdr->DUMMYUNIONNAME.EntryPointRVA);
	pCor20Hdr->Resources.VirtualAddress = _BSWAP32(pCor20Hdr->Resources.VirtualAddress);
	pCor20Hdr->Resources.Size = _BSWAP32(pCor20Hdr->Resources.Size);
	pCor20Hdr->StrongNameSignature.VirtualAddress = _BSWAP32(pCor20Hdr->StrongNameSignature.VirtualAddress);
	pCor20Hdr->StrongNameSignature.Size = _BSWAP32(pCor20Hdr->StrongNameSignature.Size);
	pCor20Hdr->CodeManagerTable.VirtualAddress = _BSWAP32(pCor20Hdr->CodeManagerTable.VirtualAddress);
	pCor20Hdr->CodeManagerTable.Size = _BSWAP32(pCor20Hdr->CodeManagerTable.Size);
	pCor20Hdr->VTableFixups.VirtualAddress = _BSWAP32(pCor20Hdr->VTableFixups.VirtualAddress);
	pCor20Hdr->VTableFixups.Size = _BSWAP32(pCor20Hdr->VTableFixups.Size);
	pCor20Hdr->ExportAddressTableJumps.VirtualAddress = _BSWAP32(pCor20Hdr->ExportAddressTableJumps.VirtualAddress);
	pCor20Hdr->ExportAddressTableJumps.Size = _BSWAP32(pCor20Hdr->ExportAddressTableJumps.Size);
	pCor20Hdr->ManagedNativeHeader.VirtualAddress = _BSWAP32(pCor20Hdr->ManagedNativeHeader.VirtualAddress);
	pCor20Hdr->ManagedNativeHeader.Size = _BSWAP32(pCor20Hdr->ManagedNativeHeader.Size);
#	endif // RS_BIG_ENDIAN
}
#endif
