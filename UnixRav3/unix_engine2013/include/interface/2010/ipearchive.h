#ifndef __PE_ARCHIVE_2010__
#define __PE_ARCHIVE_2010__

class IREPEArchive : public IRXArchive
{
public:
	STDMETHOD(ReOpen)() = 0 ;
	STDMETHOD(Save)() = 0 ;
public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREPEArchive, "{394BC69E-B336-4de9-B6D4-17C6F555E625}",
0x394bc69e, 0xb336, 0x4de9, 0xb6, 0xd4, 0x17, 0xc6, 0xf5, 0x55, 0xe6, 0x25);


//////////////////////////////////////////////////////////////////////////


class IREPERecordTable : public IUnknown
{
public:
	// PE Table Fields:

	// *** Table - Record - Field *** //
	enum TABLE_ENUM
	{
		TABLE_MZ_HEADER = 0,
		TABLE_NT_HEADER,
		TABLE_FILE_HEADER,
		TABLE_OPTIONAL_HEADER,
		TABLE_DATA_DIRECTORY,
		TABLE_SECTION_HEADER,
		TABLE_ORGSECTION_HEADER,//原始节表
	};

	//IMAGE_DOS_HEADER
	enum TABLE_FIELD_MZHEADER
	{
		MZ_W_Magic = 0,
		MZ_W_LastPageBytes,
		MZ_W_SizeInPages,
		MZ_W_Relocations,
		MZ_W_ParasOfHead,
		MZ_W_MinAlocParas,
		MZ_W_MaxAlocParas,
		MZ_W_InitSSValue,
		MZ_W_InitSPValue,
		MZ_W_Checksum,
		MZ_W_InitIPValue,
		MZ_W_InitCSValue,
		MZ_W_AlocTblOffset,
		MZ_W_OverlayNumber,
		MZ_Wx4_ReservedWords,
		MZ_W_OEMIdentifier,
		MZ_W_OEMInformation,
		MZ_Wx10_ReservedWords,
		MZ_DW_PEHeaderOffset,
	};

	//IMAGE_NT_HEADERS
	enum TABLE_FIELD_NTHEADER
	{
		NH_DW_Signature = 0, 
	};

	//IMAGE_FILE_HEADER
	enum TABLE_FIELD_FILEHEADER
	{
		FH_W_Machine = 0,
		FH_W_NumOfSections,
		FH_DW_TimeDateStamp,
		FH_DW_PtrToSymbolTbl,
		FH_DW_NumOfSymbols,
		FH_W_SizeOfOptHeader,
		FH_W_Characteristic,
	};

	//IMAGE_OPTIONAL_HEADER 
	//For PE32&PE32+:
	enum TABLE_FIELD_OPTIONALHEADER
	{
		OH_W_Magic = 0, 
		OH_B_MajorLinkerVer,
		OH_B_MinorLinkerVer,
		OH_DW_SizeOfCode,
		OH_DW_SizeOfInitData,
		OH_DW_SizeOfUninitData,
		OH_DW_AddressOfEntry,
		OH_DW_BaseOfCode,
		OH_DW_BaseOfData,    // Not In PE32+ !
		OH_DW_QW_ImageBase,  // PE32+ QW
		OH_DW_SectionAlignment,
		OH_DW_FileAlignment,
		OH_W_MajorOSVer,
		OH_W_MinorOSVer,
		OH_W_MajorImageVer,
		OH_W_MinorImageVer,
		OH_W_MajorSubsystemVer,
		OH_W_MinorSubsystemVer,
		OH_DW_Win32VerValue,
		OH_DW_SizeOfImage,
		OH_DW_SizeOfHeaders,
		OH_DW_CheckSum,
		OH_W_Subsystem,
		OH_W_DllCharacteristics,
		OH_DW_QW_SizeOfStackReserve, // PE32+ QW
		OH_DW_QW_SizeOfStackCommit,  // PE32+ QW
		OH_DW_QW_SizeOfHeapReserve,  // PE32+ QW
		OH_DW_QW_SizeOfHeapCommit,   // PE32+ QW
		OH_DW_LoaderFlags,
		OH_DW_NumOfRvaAndSizes,
	};

	//IMAGE_DATADIRECTORY:
	enum TABLE_FIELD_DATADIRECTORY
	{
		DD_DW_VirtualAddress = 0 ,
		DD_DW_Size ,
	};

	//IMAGE_SECTIONHEADER:
	enum TABLE_FIELD_SECTIONHEADER
	{
		SH_Bx8_Name = 0 ,
		SH_DW_VirtualSize,
		SH_DW_VirtualAddr,
		SH_DW_SizeOfRawData,
		SH_DW_PtrToRawData,
		SH_DW_PtrToRelocations,
		SH_DW_PtrToLineNums,
		SH_W_NumOfRelocations,
		SH_W_NumOfLineNums,
		SH_DW_Characteristics,
	};

public:
	// PE Header Structure define:

	struct IMAGE_DOS_HEADER
	{
		unsigned short   e_magic;                     // Magic number
		unsigned short   e_cblp;                      // Bytes on last page of file
		unsigned short   e_cp;                        // Pages in file
		unsigned short   e_crlc;                      // Relocations
		unsigned short   e_cparhdr;                   // Size of header in paragraphs
		unsigned short   e_minalloc;                  // Minimum extra paragraphs needed
		unsigned short   e_maxalloc;                  // Maximum extra paragraphs needed
		unsigned short   e_ss;                        // Initial (relative) SS value
		unsigned short   e_sp;                        // Initial SP value
		unsigned short   e_csum;                      // Checksum
		unsigned short   e_ip;                        // Initial IP value
		unsigned short   e_cs;                        // Initial (relative) CS value
		unsigned short   e_lfarlc;                    // File address of relocation table
		unsigned short   e_ovno;                      // Overlay number
		unsigned short   e_res[4];                    // Reserved words
		unsigned short   e_oemid;                     // OEM identifier (for e_oeminfo)
		unsigned short   e_oeminfo;                   // OEM information; e_oemid specific
		unsigned short   e_res2[10];                  // Reserved words
		LONG             e_lfanew;                    // File address of new exe header
	};

	typedef ULONG PE_IMAGE_NT_SIGNATURE ;

	struct IMAGE_FILE_HEADER
	{
		unsigned short    Machine;
		unsigned short    NumberOfSections;
		ULONG     TimeDateStamp;
		ULONG     PointerToSymbolTable;
		ULONG     NumberOfSymbols;
		unsigned short    SizeOfOptionalHeader;
		unsigned short    Characteristics;
	};

	//******************** FH Characteristics *********************//
	enum 
	{ 
		PE_IMAGE_FILE_DLL = 0x2000,
		PE_IMAGE_FILE_EXECUTABLE_IMAGE = 0x0002,
	} ;
	
	//********************* Default Alignment *********************//
	enum { PE_DEFAULT_SECTIONALIGNMENT = 0x1000 } ;
	enum { PE_DEFAULT_FILEALIGNMENT    = 0x200  } ;

	//*********************** Signatures **************************//
	enum { PE_MZ_SIGNATURE = 0x5A4D /*MZ*/ };
	enum { PE_NT_SIGNATURE = 0x00004550 /*PE\0\0*/ };

	//********************* Data Directory ***********************//
	enum 
	{
		PE_IMAGE_DIRECTORY_ENTRY_EXPORT  = 0 ,     //  0 - Export Directory
		PE_IMAGE_DIRECTORY_ENTRY_IMPORT ,          //  1 - Import Directory
		PE_IMAGE_DIRECTORY_ENTRY_RESOURCE ,        //  2 - Resource Directory
		PE_IMAGE_DIRECTORY_ENTRY_EXCEPTION ,       //  3-  Exception Directory
		PE_IMAGE_DIRECTORY_ENTRY_SECURITY ,        //  4 - Security Directory
		PE_IMAGE_DIRECTORY_ENTRY_BASERELOC ,       //  5 - Base Relocation Table
		PE_IMAGE_DIRECTORY_ENTRY_DEBUG ,           //  6 - Debug Directory
		PE_IMAGE_DIRECTORY_ENTRY_ARCHITECTURE ,    //  7 - Architecture Specific Data
		PE_IMAGE_DIRECTORY_ENTRY_GLOBALPTR ,       //  8 - RVA of GP
		PE_IMAGE_DIRECTORY_ENTRY_TLS ,             //  9 - TLS Directory
		PE_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG ,     // 10 - Load Configuration Directory
		PE_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT ,    // 11 - Bound Import Directory in headers
		PE_IMAGE_DIRECTORY_ENTRY_IAT ,             // 12 - Import Address Table
		PE_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT ,    // 13 - Delay Load Import Descriptors
		PE_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR    // 14 - COM Runtime descriptor
	};

	struct IMAGE_DATA_DIRECTORY
	{
		ULONG   VirtualAddress;
		ULONG   Size;
	};
	enum { PE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16 };

	//****************** Section *********************//
	enum { PE_IMAGE_SIZEOF_SHORT_NAME = 8 };
	struct IMAGE_SECTION_HEADER
	{
		unsigned char Name[PE_IMAGE_SIZEOF_SHORT_NAME];

		ULONG     VirtualSize;
		ULONG     VirtualAddress;
		ULONG     SizeOfRawData;
		ULONG     PointerToRawData;
		ULONG     PointerToRelocations;
		ULONG     PointerToLinenumbers;
		unsigned short    NumberOfRelocations;
		unsigned short    NumberOfLinenumbers;
		ULONG     Characteristics;
	} ;

	// Optional Header for PE32&PE32+:

	// PE32:
	struct IMAGE_OPTIONAL_HEADERS_32 // 32
	{
		//
		// Standard fields.
		//

		unsigned short    Magic;
		unsigned char     MajorLinkerVersion;
		unsigned char     MinorLinkerVersion;
		ULONG     SizeOfCode;
		ULONG     SizeOfInitializedData;
		ULONG     SizeOfUninitializedData;
		ULONG     AddressOfEntryPoint;
		ULONG     BaseOfCode;
		ULONG     BaseOfData;

		//
		// NT additional fields.
		//

		ULONG     ImageBase;
		ULONG     SectionAlignment;
		ULONG     FileAlignment;
		unsigned short    MajorOperatingSystemVersion;
		unsigned short    MinorOperatingSystemVersion;
		unsigned short    MajorImageVersion;
		unsigned short    MinorImageVersion;
		unsigned short    MajorSubsystemVersion;
		unsigned short    MinorSubsystemVersion;
		ULONG     Win32VersionValue;
		ULONG     SizeOfImage;
		ULONG     SizeOfHeaders;
		ULONG     CheckSum;
		unsigned short    Subsystem;
		unsigned short    DllCharacteristics;
		ULONG     SizeOfStackReserve;
		ULONG     SizeOfStackCommit;
		ULONG     SizeOfHeapReserve;
		ULONG     SizeOfHeapCommit;
		ULONG     LoaderFlags;
		ULONG     NumberOfRvaAndSizes;

		//Exclude DataDirectory !
	};

	// PE32+:
	struct IMAGE_OPTIONAL_HEADERS_64 // 64
	{
		unsigned short        Magic;
		unsigned char         MajorLinkerVersion;
		unsigned char         MinorLinkerVersion;
		ULONG         SizeOfCode;
		ULONG         SizeOfInitializedData;
		ULONG         SizeOfUninitializedData;
		ULONG         AddressOfEntryPoint;
		ULONG         BaseOfCode;
		ULONGLONG             ImageBase;
		ULONG         SectionAlignment;
		ULONG         FileAlignment;
		unsigned short        MajorOperatingSystemVersion;
		unsigned short        MinorOperatingSystemVersion;
		unsigned short        MajorImageVersion;
		unsigned short        MinorImageVersion;
		unsigned short        MajorSubsystemVersion;
		unsigned short        MinorSubsystemVersion;
		ULONG         Win32VersionValue;
		ULONG         SizeOfImage;
		ULONG         SizeOfHeaders;
		ULONG         CheckSum;
		unsigned short        Subsystem;
		unsigned short        DllCharacteristics;
		ULONGLONG             SizeOfStackReserve;
		ULONGLONG             SizeOfStackCommit;
		ULONGLONG             SizeOfHeapReserve;
		ULONGLONG             SizeOfHeapCommit;
		ULONG         LoaderFlags;
		ULONG         NumberOfRvaAndSizes;

		//Exclude DataDirectory !
	};

	typedef struct _IMAGE_NT_HEADERS64 {
		DWORD Signature;
		IMAGE_FILE_HEADER FileHeader;
		IMAGE_OPTIONAL_HEADERS_64 OptionalHeader;
		IMAGE_DATA_DIRECTORY DataDirectory[PE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

	typedef struct _IMAGE_NT_HEADERS {
		DWORD Signature;
		IMAGE_FILE_HEADER FileHeader;
		IMAGE_OPTIONAL_HEADERS_32 OptionalHeader;
		IMAGE_DATA_DIRECTORY DataDirectory[PE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

	//************************ OH Subsystem *************************//
	enum PESubsystem
	{
		PE_IMAGE_SUBSYSTEM_NATIVE      = 1 ,
		PE_IMAGE_SUBSYSTEM_WINDOWS_GUI = 2 ,
		PE_IMAGE_SUBSYSTEM_WINDOWS_CUI = 3 ,
	} ;


public:
	// structures for resource:

	// IMAGE_RESOURCE_DIRECTORY
	struct IMAGE_RESOURCE_DIRECTORY
	{
		ULONG   Characteristics;
		ULONG   TimeDateStamp;
		unsigned short  MajorVersion;
		unsigned short  MinorVersion;
		unsigned short  NumberOfNamedEntries;
		unsigned short  NumberOfIdEntries;
	};
	
	// IMAGE_RESOURCE_DIRECTORY_ENTRY
	struct IMAGE_RESOURCE_DIRECTORY_ENTRY
	{
		union
		{
			struct
			{
			#if defined (RS_BIG_ENDIAN)
				ULONG  NameIsString:1;
				ULONG  NameOffset:31;
			#else
				ULONG  NameOffset:31;
				ULONG  NameIsString:1;
			#endif
			} stname;
			ULONG Name;
			struct  
			{
			#if defined (RS_BIG_ENDIAN)
				unsigned short pad;
				unsigned short Id;
			#else
				unsigned short Id;
				unsigned short pad;
			#endif
			} stid;
		} unname;
		union
		{
			ULONG    OffsetToData;
			struct
			{
			#if defined (RS_BIG_ENDIAN)
				ULONG    DataIsDirectory:1;
				ULONG    OffsetToDirectory:31;
			#else
				ULONG    OffsetToDirectory:31;
				ULONG    DataIsDirectory:1;
			#endif
			} stoffset;
		} unoffset;
	};

	// IMAGE_RESOURCE_DATA_ENTRY
	struct IMAGE_RESOURCE_DATA_ENTRY
	{
		ULONG  OffsetToData;
		ULONG  Size;
		ULONG  CodePage;
		ULONG  Reserved;
	};

	enum { PE_IMAGE_RESOURCE_NAME_IS_STRING = 0x80000000 } ;
	enum { PE_IMAGE_RESOURCE_DATA_IS_DIRECTORY = 0x80000000 } ;
	enum { PE_MAX_RESNAME_LEN = 64 } ;
	
public:
	// Import Directory:

	struct PE_IMAGE_IMPORT_DESCRIPTOR
	{
		union 
		{
			DWORD   Characteristics; 
			DWORD   OriginalFirstThunk;
		};
		DWORD   TimeDateStamp;
		DWORD   ForwarderChain;
		DWORD   Name;
		DWORD   FirstThunk;
	} RS_PACK_ONEBYTE;
	typedef PE_IMAGE_IMPORT_DESCRIPTOR * PE_PIMAGE_IMPORT_DESCRIPTOR ;

	struct PE_IMAGE_IMPORT_BY_NAME
	{
		WORD    Hint;
		BYTE    Name[1];
	};

#	define PE_IMAGE_ORDINAL_FLAG32 0x80000000
#	define PE_IMAGE_ORDINAL_FLAG64 0x8000000000000000LL 

public:
	// Export Directory:

	struct PE_IMAGE_EXPORT_DIRECTORY
	{
		DWORD   Characteristics;
		DWORD   TimeDateStamp;
		WORD    MajorVersion;
		WORD    MinorVersion;
		DWORD   Name;
		DWORD   Base;
		DWORD   NumberOfFunctions;
		DWORD   NumberOfNames;
		DWORD   AddressOfFunctions;
		DWORD   AddressOfNames;
		DWORD   AddressOfNameOrdinals;
	};

public:
	// other:

	struct PE_IMAGE_BASE_RELOCATION
	{
		ULONG   VirtualAddress;
		ULONG   SizeOfBlock;
		// unsigned short    TypeOffset[1];
	};

	struct PE_TLS_DIRECTORY32 // PE32
	{
		DWORD RawDataStartVA;
		DWORD RawDataEndVA;
		DWORD AddressOfIndex;
		DWORD AddressOfCallBack;
		DWORD SizeOfZeroFill;
		DWORD Characteristics;
	};

	struct PE_TLS_DIRECTORY32P // PE32+
	{
		ULONGLONG RawDataStartVA;
		ULONGLONG RawDataEndVA;
		ULONGLONG AddressOfIndex;
		ULONGLONG AddressOfCallBack;
		DWORD SizeOfZeroFill;
		DWORD Characteristics;		
	};
	
	//----------------------------------------------------------//

public:
	// Functions:
	STDMETHOD_(bool, AddTable)(ULONG tblID, 
							   const void* pfi, 
							   ULONG number_of_field, 
							   ULONG recordsize, 
							   ULONG recordhint = 0) = 0;
	STDMETHOD_(ULONG, GetTableSize)(ULONG tblID) = 0;
	STDMETHOD_(void*, GetTable)(ULONG tblID) = 0;
	STDMETHOD_(bool, SetTable)(ULONG tblID, 
							   const void * pvbuf,
							   ULONG size_of_buf) = 0;
	STDMETHOD_(ULONG, GetRecordCount)(ULONG tblID) = 0;
	STDMETHOD_(bool, AddRecord)(ULONG tblID, ULONG idx_insert_before,
								const void * pvbuf, ULONG size_of_buf) = 0;
	STDMETHOD_(bool, DeleteRecord)(ULONG tblID, ULONG recordID) = 0;
	STDMETHOD_(ULONG, GetRecordSize)(ULONG tblID) = 0;
	STDMETHOD_(void*, GetRecord)(ULONG tblID, ULONG recordID) = 0;
	STDMETHOD_(bool,  SetRecord)(ULONG tblID, 
								 ULONG recordID , 
								 const void * pvbuf, 
								 ULONG size_of_buf) = 0;
	STDMETHOD_(ULONG, GetFieldCount)(ULONG tblID) = 0;
	STDMETHOD_(ULONG, GetFieldSize)(ULONG tblID, ULONG fieldID) = 0;
	STDMETHOD_(void*, GetField)(ULONG tblID, 
								ULONG fieldID, 
								ULONG recordID =0) = 0;
	STDMETHOD_(bool, SetField)(ULONG tblID, 
							   ULONG fieldID, 
							   ULONG recordID , 
							   const void * pvbuf, 
							   ULONG size_of_buf) = 0;

public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREPERecordTable, "{CBFCB0AE-F6D1-45c8-8A1C-22ACFF4249F3}",
0xcbfcb0ae, 0xf6d1, 0x45c8, 0x8a, 0x1c, 0x22, 0xac, 0xff, 0x42, 0x49, 0xf3);


//////////////////////////////////////////////////////////////////////////


class IREPESectionTable : public IUnknown
{
public:

	enum
	{
		PE_IMAGE_SCN_LNK_NRELOC_OVFL      =      0x01000000,  // Section contains extended relocations.
		PE_IMAGE_SCN_MEM_DISCARDABLE      =      0x02000000,  // Section can be discarded.
		PE_IMAGE_SCN_MEM_NOT_CACHED       =      0x04000000,  // Section is not cachable.
		PE_IMAGE_SCN_MEM_NOT_PAGED        =      0x08000000,  // Section is not pageable.
		PE_IMAGE_SCN_MEM_SHARED           =      0x10000000,  // Section is shareable.
		PE_IMAGE_SCN_MEM_EXECUTE          =      0x20000000,  // Section is executable.
		PE_IMAGE_SCN_MEM_READ             =      0x40000000,  // Section is readable.
		PE_IMAGE_SCN_MEM_WRITE            =      0x80000000  // Section is writeable.
	};


public:
	STDMETHOD(AddSection)(void * pSectionHeader,
						  ULONG buf_size,
						  bool bNewSecHdrTable = false) = 0;
	STDMETHOD(DeleteSection)(int nIndex) = 0;
	STDMETHOD(RVAToOffset)(ULONG dwRva,
						   OUT ULONG *pdwOffset) = 0;
	STDMETHOD(RVAToSectionId)(ULONG dwRva,
							  OUT ULONG *pdwSectionId) = 0;
	STDMETHOD(OffsetToRVAFirst)(ULONG dwOffset,
								OUT ULONG *pdwRva) = 0;
	STDMETHOD(OffsetToRVANext)(ULONG dwOffset,
							   OUT ULONG *pdwRva) = 0;
	STDMETHOD(OffsetToSectionIdFirst)(ULONG dwOffset,
									  OUT ULONG *pdwSectionId) = 0;
	STDMETHOD(OffsetToSectionIdNext)(ULONG dwOffset,
									 OUT ULONG *pdwSectionId) = 0;

public:
	RE_DECLARE_IID;
};

RE_DEFINE_IID(IREPESectionTable, "{F27C6E8F-298A-48c1-8E14-124EDF1E292D}",
0xf27c6e8f, 0x298a, 0x48c1, 0x8e, 0x14, 0x12, 0x4e, 0xdf, 0x1e, 0x29, 0x2d);

//
////////////////////////////////////////////////////////////////////////////
//
//#ifndef IREPERESOURCETABLE
//#define IREPERESOURCETABLE
//class IREPEResourceTable : public IUnknown
//{
//public:
//	// 指定资源类型id,资源项id:
//	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
//								   WORD idRcItem, WORD idRcDir,
//								   WORD wLanguage = (WORD)(-1)) = 0;
//	// 指定资源类型id,资源项名称:
//	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
//								   const CHAR* pszRcItemName, WORD idRcDir,
//								   WORD wLanguage = (WORD)(-1)) = 0;
//	// 指定资源类型名称,资源项id:
//	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
//								   WORD idRcItem, const CHAR* pszRcDirName,
//								   WORD wLanguage = (WORD)(-1)) = 0;
//	// 指定资源类型名称,资源项名称:
//	STDMETHOD_(bool, FindResource)(OUT DWORD& dwPos, OUT DWORD& dwSize,
//								   const CHAR* pszRcItemName, const CHAR* pszRcDirName,
//								   WORD wLanguage = (WORD)(-1)) = 0;
//public:
//	RE_DECLARE_IID;
//};
//
//RE_DEFINE_IID(IREPEResourceTable, "{3308ADE0-BC60-4d48-B033-188D34248415}",
//0x3308ade0, 0xbc60, 0x4d48, 0xb0, 0x33, 0x18, 0x8d, 0x34, 0x24, 0x84, 0x15);
//
//#endif
//
//
////////////////////////////////////////////////////////////////////////////
//
//#ifndef IREX86ASSEMBLER
//#define IREX86ASSEMBLER
//class IREX86Assembler : public IUnknown
//{
//public:
//	// 反汇编函数:
//	STDMETHOD_(int,Disassembler) (BYTE *incode, char *outbuff = 0, DWORD eip = 0) = 0 ;
//	// 正汇编函数:
//	STDMETHOD_(int,Assembler) (char *pasm, BYTE *outcode, DWORD eip) = 0 ;
//
//public:
//	RE_DECLARE_IID;
//};
//
//RE_DEFINE_IID(IREX86Assembler, "{F9BB3482-6A35-404c-B6E8-C83E9FCC8DFB}",
//0xf9bb3482, 0x6a35, 0x404c, 0xb6, 0xe8, 0xc8, 0x3e, 0x9f, 0xcc, 0x8d, 0xfb);
//#endif
//
////////////////////////////////////////////////////////////////////////////
//#ifndef IRECOMPILERSEARCHER
//#define IRECOMPILERSEARCHER
//
//// 分析函数的opcode列表:
//// struct OPCode
//// {
//// 	DWORD rva;
//// 	DWORD len;
//// };
//// <PROPVARIANT>.vt = VT_UI4 | VT_VECTOR
//// <PROPVARIANT>.caul.cElems = <struct OPCode总数x2>
//// <PROPVARIANT>.caul.pElems = <struct OPCode列表>
//DEFINE_GUID(PID_PE_AnalysisProcess_OPCodeList,
//			0x18894d11, 0x9e76, 0x4695, 0x9f, 0xbb, 0x6a, 0x28, 0x32, 0x6f, 0xca, 0xb1);
//
//class IRECompilerSearcher : public IUnknown
//{
//public:
//	enum MAJOR_COMPILER_TYPE
//	{
//		cmpl_vcplusplus = 0x01, //VC++
//		cmpl_delphi     = 0x02, //Delphi
//		cmpl_bcplusplus = 0x03, //BC++
//		cmpl_other      = 0x04  //other
//	};
//	enum MINOR_COMPILER_VC_TYPE
//	{
//		cmpl_vc_unkcmpl = 0x00,//Microsoft Visual C++
//		cmpl_vc_4x,			   //Microsoft Visual C++ 4.x
//		cmpl_vc_50,			   //Microsoft Visual C++ 5.0
//		cmpl_vc_60,			   //Microsoft Visual C++ 6.0
//		cmpl_vc_70,			   //Microsoft Visual C++ 7.0
//		cmpl_vc_v71,		   //Microsoft Visual C++ v7.1
//		cmpl_vc_80,			   //Microsoft Visual C++ 8.0
//		cmpl_vc_cab			   //Microsoft CAB
//	};
//	enum MINOR_COMPILER_DELPHI_TYPE
//	{
//		cmpl_delphi_unkcmpl = 0x00,//Borland Delphi
//		cmpl_delphi_20,			   //Borland Delphi 2.0
//		cmpl_delphi_30,			   //Borland Delphi 3.0
//		cmpl_delphi_40,			   //Borland Delphi 4.0
//		cmpl_delphi_50,			   //Borland Delphi 5.0
//		cmpl_delphi_60,			   //Borland Delphi 6.0
//		cmpl_delphi_70,			   //Borland Delphi 7.0
//		cmpl_delphi_80			   //Borland Delphi 8.0
//	};
//	enum MINOR_COMPILER_BC_TYPE
//	{
//		cmpl_bc_unkcmpl = 0x00,//Borland C++
//		cmpl_bc_1999,		   //Borland C++ 1999
//		cmpl_bc_component_cn,   //Borland 组件
//		cmpl_bc_component	   //Borland Component
//	};
//	enum MINOR_COMPILER_OTHER_TYPE
//	{
//		cmpl_other_unkcmpl = 0x00,     //unknown
//		cmpl_other_winzip_unkcmpl,     //winzip
//		cmpl_other_winrar_unkcmpl,     //winrar
//		cmpl_other_wise_ins_unkcmpl,   //wise
//		cmpl_other_ins_shelld_unkcmpl, //InstallShelld
//		cmpl_other_nullsoft_PiMP,      //Nullsoft PiMP
//		cmpl_other_inno_setup,         //Inno Setup
//		cmpl_other_watcom_c_cpp,       //Watcom C/C++
//	};
//	static BYTE major_ver(WORD ver) { return (BYTE)(ver>>8); }
//	static BYTE minor_ver(WORD ver) { return (BYTE)(ver); }
//
//public:
//	STDMETHOD(FindCompiler)(PDWORD pdwRvaMain = NULL, PWORD pwCompilerID = NULL) = 0 ;
//	STDMETHOD(GetOpcode16)(BYTE* pbyOpCode) = 0 ;
//	STDMETHOD(AnalysisProcess)(DWORD entry_rva, DWORD code_len, OUT IREProperty *presult) = 0 ;
//
//public:
//	RE_DECLARE_IID;
//};
//
//RE_DEFINE_IID(IRECompilerSearcher, "{7D158BFE-0E39-4d3a-AAFD-47FA3B1CAC77}",
//			  0x7d158bfe, 0xe39, 0x4d3a, 0xaa, 0xfd, 0x47, 0xfa, 0x3b, 0x1c, 0xac, 0x77);
//#endif
//
////////////////////////////////////////////////////////////////////////////
//#ifndef IREPESIGNATURECOMPUTER 
//#define IREPESIGNATURECOMPUTER 
//class IREPESignatureComputer : public IUnknown
//{
//public:
//	STDMETHOD( CalcSignature2010 )( ULARGE_INTEGER * liSignature ) PURE;
//	STDMETHOD( CalcSignature2011 )( ULARGE_INTEGER * liSignature ) PURE;
//	RE_DECLARE_IID;
//};
//
//// {D9E471B2-41D7-4cfe-9545-DB55DF4CCF8C}
//RE_DEFINE_IID( IREPESignatureComputer, "{D9E471B2-41D7-4cfe-9545-DB55DF4CCF8C}",
//			  0xd9e471b2, 0x41d7, 0x4cfe, 0x95, 0x45, 0xdb, 0x55, 0xdf, 0x4c, 0xcf, 0x8c);
//#endif
////////////////////////////////////////////////////////////////////////////

// {AE68FC69-B8D6-4132-9BE7-9E8648258D52}
DEFINE_GUID(CLSID_CREPEArchive, 
0xae68fc69, 0xb8d6, 0x4132, 0x9b, 0xe7, 0x9e, 0x86, 0x48, 0x25, 0x8d, 0x52);


// {8CD4BC83-0CB5-4863-8E03-AE5FFE922296}
DEFINE_GUID(CLSID_CREPE32PArchive, 
0x8cd4bc83, 0xcb5, 0x4863, 0x8e, 0x3, 0xae, 0x5f, 0xfe, 0x92, 0x22, 0x96);


//////////////////////////////////////////////////////////////////////////


#endif // #ifndef _RE_EXPORT_INTERFACE_PE_ARCHIVE_H_

