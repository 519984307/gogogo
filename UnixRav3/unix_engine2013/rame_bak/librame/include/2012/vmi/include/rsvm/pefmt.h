#include "types.h"

#ifndef _RSVM_PE_FORMAT_H_
#define _RSVM_PE_FORMAT_H_

namespace winpe {

#pragma pack(push, 1)

#define WINPE_IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define WINPE_IMAGE_OS2_SIGNATURE                 0x454E      // NE
#define WINPE_IMAGE_OS2_SIGNATURE_LE              0x454C      // LE
#define WINPE_IMAGE_VXD_SIGNATURE                 0x454C      // LE
#define WINPE_IMAGE_NT_SIGNATURE                  0x00004550  // PE00

#define WINPE_IMAGE_SIZEOF_FILE_HEADER             20
#define WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES     16

#define WINPE_IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define WINPE_IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved externel references).
#define WINPE_IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define WINPE_IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define WINPE_IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Agressively trim working set
#define WINPE_IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define WINPE_IMAGE_FILE_WORDS_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define WINPE_IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define WINPE_IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define WINPE_IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define WINPE_IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define WINPE_IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define WINPE_IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define WINPE_IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define WINPE_IMAGE_FILE_WORDS_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define WINPE_IMAGE_FILE_MACHINE_UNKNOWN           0
#define WINPE_IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define WINPE_IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define WINPE_IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define WINPE_IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define WINPE_IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define WINPE_IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define WINPE_IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define WINPE_IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define WINPE_IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define WINPE_IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define WINPE_IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define WINPE_IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define WINPE_IMAGE_FILE_MACHINE_THUMB             0x01c2
#define WINPE_IMAGE_FILE_MACHINE_AM33              0x01d3
#define WINPE_IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define WINPE_IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define WINPE_IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define WINPE_IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define WINPE_IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define WINPE_IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define WINPE_IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define WINPE_IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define WINPE_IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define WINPE_IMAGE_FILE_MACHINE_CEF               0x0CEF
#define WINPE_IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define WINPE_IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define WINPE_IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define WINPE_IMAGE_FILE_MACHINE_CEE               0xC0EE

#define WINPE_IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define WINPE_IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define WINPE_IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define WINPE_IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
	// end_winnt
	// reserved                                4   // Old Windows CE subsystem.
	// begin_winnt
#define WINPE_IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define WINPE_IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define WINPE_IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
#define WINPE_IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
#define WINPE_IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
#define WINPE_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
#define WINPE_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
#define WINPE_IMAGE_SUBSYSTEM_EFI_ROM              13
#define WINPE_IMAGE_SUBSYSTEM_XBOX                 14

#define WINPE_IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define WINPE_IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT             7   // (X86 usage)
#define WINPE_IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define WINPE_IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define WINPE_IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define WINPE_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define WINPE_IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define WINPE_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define WINPE_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor

#define WINPE_IMAGE_SIZEOF_SHORT_NAME              8

#define WINPE_IMAGE_SIZEOF_SECTION_HEADER          40

//      IMAGE_SCN_TYPE_REG                   0x00000000  // Reserved.
//      IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
//      IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
//      IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
#define WINPE_IMAGE_SCN_TYPE_NO_PAD          0x00000008  // Reserved.
//      IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.

#define WINPE_IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define WINPE_IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define WINPE_IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define WINPE_IMAGE_SCN_LNK_OTHER            0x00000100  // Reserved.
#define WINPE_IMAGE_SCN_LNK_INFO             0x00000200  // Section contains comments or some other type of information.
//      IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
#define WINPE_IMAGE_SCN_LNK_REMOVE           0x00000800  // Section contents will not become part of image.
#define WINPE_IMAGE_SCN_LNK_COMDAT           0x00001000  // Section contents comdat.
//                                           0x00002000  // Reserved.
//      IMAGE_SCN_MEM_PROTECTED - Obsolete   0x00004000
#define WINPE_IMAGE_SCN_NO_DEFER_SPEC_EXC    0x00004000  // Reset speculative exceptions handling bits in the TLB entries for this section.
#define WINPE_IMAGE_SCN_GPREL                0x00008000  // Section content can be accessed relative to GP
#define WINPE_IMAGE_SCN_MEM_FARDATA          0x00008000
//      IMAGE_SCN_MEM_SYSHEAP  - Obsolete    0x00010000
#define WINPE_IMAGE_SCN_MEM_PURGEABLE        0x00020000
#define WINPE_IMAGE_SCN_MEM_16BIT            0x00020000
#define WINPE_IMAGE_SCN_MEM_LOCKED           0x00040000
#define WINPE_IMAGE_SCN_MEM_PRELOAD          0x00080000

#define WINPE_IMAGE_SCN_ALIGN_1BYTES         0x00100000  //
#define WINPE_IMAGE_SCN_ALIGN_2BYTES         0x00200000  //
#define WINPE_IMAGE_SCN_ALIGN_4BYTES         0x00300000  //
#define WINPE_IMAGE_SCN_ALIGN_8BYTES         0x00400000  //
#define WINPE_IMAGE_SCN_ALIGN_16BYTES        0x00500000  // Default alignment if no others are specified.
#define WINPE_IMAGE_SCN_ALIGN_32BYTES        0x00600000  //
#define WINPE_IMAGE_SCN_ALIGN_64BYTES        0x00700000  //
#define WINPE_IMAGE_SCN_ALIGN_128BYTES       0x00800000  //
#define WINPE_IMAGE_SCN_ALIGN_256BYTES       0x00900000  //
#define WINPE_IMAGE_SCN_ALIGN_512BYTES       0x00A00000  //
#define WINPE_IMAGE_SCN_ALIGN_1024BYTES      0x00B00000  //
#define WINPE_IMAGE_SCN_ALIGN_2048BYTES      0x00C00000  //
#define WINPE_IMAGE_SCN_ALIGN_4096BYTES      0x00D00000  //
#define WINPE_IMAGE_SCN_ALIGN_8192BYTES      0x00E00000  //
// Unused                                    0x00F00000
#define WINPE_IMAGE_SCN_ALIGN_MASK           0x00F00000

#define WINPE_IMAGE_SCN_LNK_NRELOC_OVFL      0x01000000  // Section contains extended relocations.
#define WINPE_IMAGE_SCN_MEM_DISCARDABLE      0x02000000  // Section can be discarded.
#define WINPE_IMAGE_SCN_MEM_NOT_CACHED       0x04000000  // Section is not cachable.
#define WINPE_IMAGE_SCN_MEM_NOT_PAGED        0x08000000  // Section is not pageable.
#define WINPE_IMAGE_SCN_MEM_SHARED           0x10000000  // Section is shareable.
#define WINPE_IMAGE_SCN_MEM_EXECUTE          0x20000000  // Section is executable.
#define WINPE_IMAGE_SCN_MEM_READ             0x40000000  // Section is readable.
#define WINPE_IMAGE_SCN_MEM_WRITE            0x80000000  // Section is writeable.


#define WINPE_IMAGE_SIZEOF_BASE_RELOCATION   8

#define WINPE_IMAGE_REL_BASED_ABSOLUTE              0
#define WINPE_IMAGE_REL_BASED_HIGH                  1
#define WINPE_IMAGE_REL_BASED_LOW                   2
#define WINPE_IMAGE_REL_BASED_HIGHLOW               3
#define WINPE_IMAGE_REL_BASED_HIGHADJ               4
#define WINPE_IMAGE_REL_BASED_MIPS_JMPADDR          5
#define WINPE_IMAGE_REL_BASED_MIPS_JMPADDR16        9
#define WINPE_IMAGE_REL_BASED_IA64_IMM64            9
#define WINPE_IMAGE_REL_BASED_DIR64                 10

// DllCharacteristics Entries

//      IMAGE_LIBRARY_PROCESS_INIT            0x0001     // Reserved.
//      IMAGE_LIBRARY_PROCESS_TERM            0x0002     // Reserved.
//      IMAGE_LIBRARY_THREAD_INIT             0x0004     // Reserved.
//      IMAGE_LIBRARY_THREAD_TERM             0x0008     // Reserved.
#define WINPE_IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040     // DLL can move.
#define WINPE_IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY    0x0080     // Code Integrity Image
#define WINPE_IMAGE_DLLCHARACTERISTICS_NX_COMPAT    0x0100     // Image is NX compatible
#define WINPE_IMAGE_DLLCHARACTERISTICS_NO_ISOLATION 0x0200     // Image understands isolation and doesn't want it
#define WINPE_IMAGE_DLLCHARACTERISTICS_NO_SEH       0x0400     // Image does not use SEH.  No SE handler may reside in this image
#define WINPE_IMAGE_DLLCHARACTERISTICS_NO_BIND      0x0800     // Do not bind this image.
//                                            0x1000     // Reserved.
#define WINPE_IMAGE_DLLCHARACTERISTICS_WDM_DRIVER   0x2000     // Driver uses WDM model
//                                            0x4000     // Reserved.
#define WINPE_IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE     0x8000

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
	uint16_t e_magic;               // Magic number
	uint16_t e_cblp;                // Bytes on last page of file
	uint16_t e_cp;                  // Pages in file
	uint16_t e_crlc;                // Relocations
	uint16_t e_cparhdr;             // Size of header in paragraphs
	uint16_t e_minalloc;            // Minimum extra paragraphs needed
	uint16_t e_maxalloc;            // Maximum extra paragraphs needed
	uint16_t e_ss;                  // Initial (relative) SS value
	uint16_t e_sp;                  // Initial SP value
	uint16_t e_csum;                // Checksum
	uint16_t e_ip;                  // Initial IP value
	uint16_t e_cs;                  // Initial (relative) CS value
	uint16_t e_lfarlc;              // File address of relocation table
	uint16_t e_ovno;                // Overlay number
	uint16_t e_res[4];              // Reserved words
	uint16_t e_oemid;               // OEM identifier (for e_oeminfo)
	uint16_t e_oeminfo;             // OEM information; e_oemid specific
	uint16_t e_res2[10];            // Reserved words
	int32_t  e_lfanew;              // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	uint16_t  Machine;
	uint16_t  NumberOfSections;
	uint32_t  TimeDateStamp;
	uint32_t  PointerToSymbolTable;
	uint32_t  NumberOfSymbols;
	uint16_t  SizeOfOptionalHeader;
	uint16_t  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
	uint32_t   VirtualAddress;
	uint32_t   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER {
	//
	// Standard fields.
	//

	uint16_t  Magic;
	uint8_t   MajorLinkerVersion;
	uint8_t   MinorLinkerVersion;
	uint32_t  SizeOfCode;
	uint32_t  SizeOfInitializedData;
	uint32_t  SizeOfUninitializedData;
	uint32_t  AddressOfEntryPoint;
	uint32_t  BaseOfCode;
	uint32_t  BaseOfData;

	//
	// NT additional fields.
	//

	uint32_t  ImageBase;
	uint32_t  SectionAlignment;
	uint32_t  FileAlignment;
	uint16_t  MajorOperatingSystemVersion;
	uint16_t  MinorOperatingSystemVersion;
	uint16_t  MajorImageVersion;
	uint16_t  MinorImageVersion;
	uint16_t  MajorSubsystemVersion;
	uint16_t  MinorSubsystemVersion;
	uint32_t  Win32VersionValue;
	uint32_t  SizeOfImage;
	uint32_t  SizeOfHeaders;
	uint32_t  CheckSum;
	uint16_t  Subsystem;
	uint16_t  DllCharacteristics;
	uint32_t  SizeOfStackReserve;
	uint32_t  SizeOfStackCommit;
	uint32_t  SizeOfHeapReserve;
	uint32_t  SizeOfHeapCommit;
	uint32_t  LoaderFlags;
	uint32_t  NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
	uint16_t  Magic;
	uint8_t   MajorLinkerVersion;
	uint8_t   MinorLinkerVersion;
	uint32_t  SizeOfCode;
	uint32_t  SizeOfInitializedData;
	uint32_t  SizeOfUninitializedData;
	uint32_t  AddressOfEntryPoint;
	uint32_t  BaseOfCode;
	uint64_t  ImageBase;
	uint32_t  SectionAlignment;
	uint32_t  FileAlignment;
	uint16_t  MajorOperatingSystemVersion;
	uint16_t  MinorOperatingSystemVersion;
	uint16_t  MajorImageVersion;
	uint16_t  MinorImageVersion;
	uint16_t  MajorSubsystemVersion;
	uint16_t  MinorSubsystemVersion;
	uint32_t  Win32VersionValue;
	uint32_t  SizeOfImage;
	uint32_t  SizeOfHeaders;
	uint32_t  CheckSum;
	uint16_t  Subsystem;
	uint16_t  DllCharacteristics;
	uint64_t  SizeOfStackReserve;
	uint64_t  SizeOfStackCommit;
	uint64_t  SizeOfHeapReserve;
	uint64_t  SizeOfHeapCommit;
	uint32_t  LoaderFlags;
	uint32_t  NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[WINPE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

#ifdef __target_x86_64__
typedef IMAGE_OPTIONAL_HEADER64             IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER64            PIMAGE_OPTIONAL_HEADER;
#else
typedef IMAGE_OPTIONAL_HEADER32             IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER32            PIMAGE_OPTIONAL_HEADER;
#endif

typedef struct _IMAGE_SECTION_HEADER {
	uint8_t   Name[WINPE_IMAGE_SIZEOF_SHORT_NAME];
	union {
		uint32_t   PhysicalAddress;
		uint32_t   VirtualSize;
	} Misc;
	uint32_t  VirtualAddress;
	uint32_t  SizeOfRawData;
	uint32_t  PointerToRawData;
	uint32_t  PointerToRelocations;
	uint32_t  PointerToLinenumbers;
	uint16_t  NumberOfRelocations;
	uint16_t  NumberOfLinenumbers;
	uint32_t  Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_NT_HEADERS {
	uint32_t Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_NT_HEADERS64 {
	uint32_t Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

#ifdef __target_x86_64__
typedef IMAGE_NT_HEADERS64                  IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS64                 PIMAGE_NT_HEADERS;
#else
typedef IMAGE_NT_HEADERS32                  IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS32                 PIMAGE_NT_HEADERS;
#endif

typedef struct _IMAGE_BASE_RELOCATION {
	uint32_t   VirtualAddress;
	uint32_t   SizeOfBlock;
	//  uint16_t    TypeOffset[1];
} IMAGE_BASE_RELOCATION;
typedef IMAGE_BASE_RELOCATION * PIMAGE_BASE_RELOCATION;

typedef struct _IMAGE_EXPORT_DIRECTORY {
	uint32_t   Characteristics;
	uint32_t   TimeDateStamp;
	uint16_t   MajorVersion;
	uint16_t   MinorVersion;
	uint32_t   Name;
	uint32_t   Base;
	uint32_t   NumberOfFunctions;
	uint32_t   NumberOfNames;
	uint32_t   AddressOfFunctions;     // RVA from base of image
	uint32_t   AddressOfNames;         // RVA from base of image
	uint32_t   AddressOfNameOrdinals;  // RVA from base of image
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

typedef struct _IMAGE_IMPORT_BY_NAME {
	uint16_t   Hint;
	uint8_t    Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

#pragma pack(push, 8)

typedef struct _IMAGE_THUNK_DATA64 {
	union {
		uint64_t ForwarderString;  // PBYTE 
		uint64_t Function;         // Puint32_t
		uint64_t Ordinal;
		uint64_t AddressOfData;    // PIMAGE_IMPORT_BY_NAME
	} u1;
} IMAGE_THUNK_DATA64;
typedef IMAGE_THUNK_DATA64 * PIMAGE_THUNK_DATA64;

#pragma pack(pop)

typedef struct _IMAGE_THUNK_DATA32 {
	union {
		uint32_t ForwarderString;      // PBYTE 
		uint32_t Function;             // Puint32_t
		uint32_t Ordinal;
		uint32_t AddressOfData;        // PIMAGE_IMPORT_BY_NAME
	} u1;
} IMAGE_THUNK_DATA32;
typedef IMAGE_THUNK_DATA32 * PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_TLS_DIRECTORY64 {
	uint64_t   StartAddressOfRawData;
	uint64_t   EndAddressOfRawData;
	uint64_t   AddressOfIndex;         // Puint32_t
	uint64_t   AddressOfCallBacks;     // PIMAGE_TLS_CALLBACK *;
	uint32_t   SizeOfZeroFill;
	uint32_t   Characteristics;
} IMAGE_TLS_DIRECTORY64;
typedef IMAGE_TLS_DIRECTORY64 * PIMAGE_TLS_DIRECTORY64;

typedef struct _IMAGE_TLS_DIRECTORY32 {
	uint32_t   StartAddressOfRawData;
	uint32_t   EndAddressOfRawData;
	uint32_t   AddressOfIndex;             // Puint32_t
	uint32_t   AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
	uint32_t   SizeOfZeroFill;
	uint32_t   Characteristics;
} IMAGE_TLS_DIRECTORY32;
typedef IMAGE_TLS_DIRECTORY32 * PIMAGE_TLS_DIRECTORY32;

#ifdef __target_x86_64__
typedef IMAGE_THUNK_DATA64              IMAGE_THUNK_DATA;
typedef PIMAGE_THUNK_DATA64             PIMAGE_THUNK_DATA;
typedef IMAGE_TLS_DIRECTORY64           IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY64          PIMAGE_TLS_DIRECTORY;
#else
typedef IMAGE_THUNK_DATA32              IMAGE_THUNK_DATA;
typedef PIMAGE_THUNK_DATA32             PIMAGE_THUNK_DATA;
typedef IMAGE_TLS_DIRECTORY32           IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY32          PIMAGE_TLS_DIRECTORY;
#endif

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
	union {
		uint32_t   Characteristics;            // 0 for terminating null import descriptor
		uint32_t   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
	};
	uint32_t   TimeDateStamp;                  // 0 if not bound,
	// -1 if bound, and real date\time stamp
	//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
	// O.W. date/time stamp of DLL bound to (Old BIND)

	uint32_t   ForwarderChain;                 // -1 if no forwarders
	uint32_t   Name;
	uint32_t   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR *PIMAGE_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_RESOURCE_DIRECTORY {
        uint32_t   Characteristics;
        uint32_t   TimeDateStamp;
        uint16_t   MajorVersion;
        uint16_t   MinorVersion;
        uint16_t   NumberOfNamedEntries;
        uint16_t   NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
        union {
                struct {
#ifdef RS_BIG_ENDIAN
						uint32_t NameIsString:1;
						uint32_t NameOffset:31;
						
#else
						uint32_t NameOffset:31;
						uint32_t NameIsString:1;
#endif
                };
                uint32_t   Name;
				struct{
#ifdef RS_BIG_ENDIAN
					uint16_t   nouse;
					uint16_t   Id;
#else
					uint16_t   Id;
					uint16_t   nouse;
#endif
				};
        };
        union {
                uint32_t   OffsetToData;
                struct {
#ifdef RS_BIG_ENDIAN
						uint32_t   DataIsDirectory:1;
						uint32_t   OffsetToDirectory:31;
#else
						uint32_t   OffsetToDirectory:31;
						uint32_t   DataIsDirectory:1;
#endif
				};
        };
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
        uint32_t   OffsetToData;
        uint32_t   Size;
        uint32_t   CodePage;
        uint32_t   Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

#define WINPE_IS_INTRESOURCE(_r) (((uint32_t)(_r) >> 16) == 0)


// CLR 2.0 header structure.
typedef struct IMAGE_COR20_HEADER
{
	// Header versioning
	uint32_t					cb;
	int16_t						MajorRuntimeVersion;
	int16_t						MinorRuntimeVersion;

	// Symbol table and startup information
	IMAGE_DATA_DIRECTORY		MetaData;
	uint32_t					Flags;

	// If COMIMAGE_FLAGS_NATIVE_ENTRYPOINT is not set, EntryPointToken represents a managed entrypoint.
	// If COMIMAGE_FLAGS_NATIVE_ENTRYPOINT is set, EntryPointRVA represents an RVA to a native entrypoint.
	union {
		uint32_t				EntryPointToken;
		uint32_t				EntryPointRVA;
	} DUMMYUNIONNAME;

	// Binding information
	IMAGE_DATA_DIRECTORY		Resources;
	IMAGE_DATA_DIRECTORY		StrongNameSignature;

	// Regular fixup and binding information
	IMAGE_DATA_DIRECTORY		CodeManagerTable;
	IMAGE_DATA_DIRECTORY		VTableFixups;
	IMAGE_DATA_DIRECTORY		ExportAddressTableJumps;

	// Precompiled image info (internal use only - set to zero)
	IMAGE_DATA_DIRECTORY		ManagedNativeHeader;

} IMAGE_COR20_HEADER, *PIMAGE_COR20_HEADER;


#pragma pack(pop)

} /* namespace winpe */

#endif /* _RSVM_PE_FORMAT_H_ */
