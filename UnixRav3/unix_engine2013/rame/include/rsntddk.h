#ifndef _RSNTDDK_H_
#define _RSNTDDK_H_

#if (TARGET_OS==OS_NATIVE)

#ifdef __cplusplus
extern "C"{ ;
#endif
//
// NTDDK
//
// #pragma warning (disable : 4201) // nameless struct/union
#include <ntddk.h>
// #pragma warning (default : 4201)


// Error Handling ================================================================
NTSYSAPI
NTSTATUS 
NTAPI 
NtDisplayString(
	PUNICODE_STRING String 
	);

NTSYSAPI
ULONG 
NTAPI
RtlNtStatusToDosError(
	NTSTATUS Status
	);


// System Information ================================================================
typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION { // Information Class 3
	LARGE_INTEGER BootTime;
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER TimeZoneBias;
	ULONG CurrentTimeZoneId;
} SYSTEM_TIME_OF_DAY_INFORMATION, *PSYSTEM_TIME_OF_DAY_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemTimeOfDayInformation = 3
}SYSTEM_INFORMATION_CLASS;

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

// NT Objects ================================================================
// File ----------------------------------------------------------------
#define DRIVE_UNKNOWN     0
#define DRIVE_NO_ROOT_DIR 1
#define DRIVE_REMOVABLE   2
#define DRIVE_FIXED       3
#define DRIVE_REMOTE      4
#define DRIVE_CDROM       5
#define DRIVE_RAMDISK     6

typedef struct _FILE_DIRECTORY_INFORMATION { // Information Class 1
	ULONG NextEntryOffset;
	ULONG Unknown;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	WCHAR FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_RENAME_INFORMATION { // Info Classes 10 
	BOOLEAN ReplaceIfExists;
	HANDLE RootDirectory;
	ULONG FileNameLength;
	WCHAR FileName[1];
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;


NTSTATUS 
NTAPI 
ZwDisplayString(PUNICODE_STRING String);


NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFile(
	IN HANDLE FileHandle,
	IN HANDLE Event OPTIONAL,
	IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN PVOID ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG FileInformationLength,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN ReturnSingleEntry,
	IN PUNICODE_STRING FileName OPTIONAL,
	IN BOOLEAN RestartScan
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushBuffersFile(
	IN HANDLE FileHandle,
	OUT PIO_STATUS_BLOCK IoStatusBlock
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryAttributesFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION BasicInformation
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION NetworkOpenInformation
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwCancelIoFile(
	IN HANDLE FileHandle,
	OUT PIO_STATUS_BLOCK IoStatusBlock
	);
NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

// Section ----------------------------------------------------------------
NTSYSAPI
NTSTATUS 
NTAPI
ZwCreateSection(
    OUT PHANDLE  SectionHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER  MaximumSize OPTIONAL,
    IN ULONG  SectionPageProtection,
    IN ULONG  AllocationAttributes,
    IN HANDLE  FileHandle OPTIONAL
    );

NTSYSAPI
NTSTATUS 
NTAPI
ZwMapViewOfSection(
    IN HANDLE  SectionHandle,
    IN HANDLE  ProcessHandle,
    IN OUT PVOID  *BaseAddress,
    IN ULONG_PTR  ZeroBits,
    IN SIZE_T  CommitSize,
    IN OUT PLARGE_INTEGER  SectionOffset  OPTIONAL,
    IN OUT PSIZE_T  ViewSize,
    IN SECTION_INHERIT  InheritDisposition,
    IN ULONG  AllocationType,
    IN ULONG  Win32Protect
    );

// Event ----------------------------------------------------------------
NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN EVENT_TYPE           EventType,
	IN BOOLEAN              InitialState 
	);
NTSYSAPI 
NTSTATUS
NTAPI
ZwResetEvent(
	IN HANDLE               EventHandle,
	OUT PLONG               PreviousState OPTIONAL 
	);

// Key ----------------------------------------------------------------
NTSYSAPI 
NTSTATUS
NTAPI
ZwDeleteValueKey(
	IN HANDLE               KeyHandle,
	IN PUNICODE_STRING      ValueName 
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadKey (
    IN POBJECT_ATTRIBUTES KeyObjectAttributes,
    IN POBJECT_ATTRIBUTES FileObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadKey2 (
    IN POBJECT_ATTRIBUTES KeyObjectAttributes,
    IN POBJECT_ATTRIBUTES FileObjectAttributes,
	IN ULONG Flags
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadKey (
	IN POBJECT_ATTRIBUTES KeyObjectAttributes
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwSaveKey(
	IN HANDLE KeyHandle,
	IN HANDLE FileHandle
	);

// Process ----------------------------------------------------------------
typedef struct {
	ULONG            Unknown[21];     
	UNICODE_STRING   CommandLine;
	UNICODE_STRING   ImageFile;
} ENVIRONMENT_INFORMATION, *PENVIRONMENT_INFORMATION;

typedef struct {
	ULONG                     Unknown[3];
	PENVIRONMENT_INFORMATION  Environment;
} STARTUP_ARGUMENT, *PSTARTUP_ARGUMENT;

NTSYSAPI
NTSTATUS 
NTAPI 
NtTerminateProcess(
	HANDLE ProcessHandle, 
	LONG ExitStatus 
	);

typedef struct _PEB_LDR_DATA {
	ULONG                   Length;
	BOOLEAN                 Initialized;
	PVOID                   SsHandle;
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

//
// Loader Data Table Entry
//
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union
	{
		LIST_ENTRY HashLinks;
		PVOID SectionPointer;
	};
	ULONG CheckSum;
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports;
	};
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_DRIVE_LETTER_CURDIR {
	USHORT                  Flags;
	USHORT                  Length;
	ULONG                   TimeStamp;
	UNICODE_STRING          DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG                   MaximumLength;
	ULONG                   Length;
	ULONG                   Flags;
	ULONG                   DebugFlags;
	PVOID                   ConsoleHandle;
	ULONG                   ConsoleFlags;
	HANDLE                  StdInputHandle;
	HANDLE                  StdOutputHandle;
	HANDLE                  StdErrorHandle;
	UNICODE_STRING          CurrentDirectoryPath;
	HANDLE                  CurrentDirectoryHandle;
	UNICODE_STRING          DllPath;
	UNICODE_STRING          ImagePathName;
	UNICODE_STRING          CommandLine;
	PVOID                   Environment;
	ULONG                   StartingPositionLeft;
	ULONG                   StartingPositionTop;
	ULONG                   Width;
	ULONG                   Height;
	ULONG                   CharWidth;
	ULONG                   CharHeight;
	ULONG                   ConsoleTextAttributes;
	ULONG                   WindowFlags;
	ULONG                   ShowWindowFlags;
	UNICODE_STRING          WindowTitle;
	UNICODE_STRING          DesktopName;
	UNICODE_STRING          ShellInfo;
	UNICODE_STRING          RuntimeData;
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef void (*PPEBLOCKROUTINE)(
								PVOID PebLock
								); 
typedef struct _PEB_FREE_BLOCK {
	_PEB_FREE_BLOCK          *Next;
	ULONG                   Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef PVOID * PPVOID;

typedef struct _PEB {
  BOOLEAN                 InheritedAddressSpace;
  BOOLEAN                 ReadImageFileExecOptions;
  BOOLEAN                 BeingDebugged;
  BOOLEAN                 Spare;
  HANDLE                  Mutant;
  PVOID                   ImageBaseAddress;
  PPEB_LDR_DATA           LoaderData;
  PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
  PVOID                   SubSystemData;
  PVOID                   ProcessHeap;
  PVOID                   FastPebLock;
  PPEBLOCKROUTINE         FastPebLockRoutine;
  PPEBLOCKROUTINE         FastPebUnlockRoutine;
  ULONG                   EnvironmentUpdateCount;
  PVOID                  *KernelCallbackTable;
  PVOID                   EventLogSection;
  PVOID                   EventLog;
  PPEB_FREE_BLOCK         FreeList;
  ULONG                   TlsExpansionCounter;
  PVOID                   TlsBitmap;
  ULONG                   TlsBitmapBits[0x2];
  PVOID                   ReadOnlySharedMemoryBase;
  PVOID                   ReadOnlySharedMemoryHeap;
  PPVOID                  ReadOnlyStaticServerData;
  PVOID                   AnsiCodePageData;
  PVOID                   OemCodePageData;
  PVOID                   UnicodeCaseTableData;
  ULONG                   NumberOfProcessors;
  ULONG                   NtGlobalFlag;
  UCHAR                    Spare2[0x4];
  LARGE_INTEGER           CriticalSectionTimeout;
  ULONG                   HeapSegmentReserve;
  ULONG                   HeapSegmentCommit;
  ULONG                   HeapDeCommitTotalFreeThreshold;
  ULONG                   HeapDeCommitFreeBlockThreshold;
  ULONG                   NumberOfHeaps;
  ULONG                   MaximumNumberOfHeaps;
  PVOID                  *ProcessHeaps;
  PVOID                   GdiSharedHandleTable;
  PVOID                   ProcessStarterHelper;
  PVOID                   GdiDCAttributeList;
  PVOID                   LoaderLock;
  ULONG                   OSMajorVersion;
  ULONG                   OSMinorVersion;
  ULONG                   OSBuildNumber;
  ULONG                   OSPlatformId;
  ULONG                   ImageSubSystem;
  ULONG                   ImageSubSystemMajorVersion;
  ULONG                   ImageSubSystemMinorVersion;
  ULONG                   GdiHandleBuffer[0x22];
  ULONG                   PostProcessInitRoutine;
  ULONG                   TlsExpansionBitmap;
  UCHAR                    TlsExpansionBitmapBits[0x80];
  ULONG                   SessionId;
} PEB, *PPEB;

// Thread ----------------------------------------------------------------
NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
	IN BOOLEAN              Alertable,
	IN PLARGE_INTEGER       DelayInterval 
	);

typedef struct _TEB {
  NT_TIB                  Tib;
  PVOID                   EnvironmentPointer;
  CLIENT_ID               Cid;
  PVOID                   ActiveRpcInfo;
  PVOID                   ThreadLocalStoragePointer;
  PPEB                    Peb;
  ULONG                   LastErrorValue;
  ULONG                   CountOfOwnedCriticalSections;
  PVOID                   CsrClientThread;
  PVOID                   Win32ThreadInfo;
  ULONG                   Win32ClientInfo[0x1F];
  PVOID                   WOW32Reserved;
  ULONG                   CurrentLocale;
  ULONG                   FpSoftwareStatusRegister;
  PVOID                   SystemReserved1[0x36];
  PVOID                   Spare1;
  ULONG                   ExceptionCode;
  ULONG                   SpareBytes1[0x28];
  PVOID                   SystemReserved2[0xA];
  ULONG                   GdiRgn;
  ULONG                   GdiPen;
  ULONG                   GdiBrush;
  CLIENT_ID               RealClientId;
  PVOID                   GdiCachedProcessHandle;
  ULONG                   GdiClientPID;
  ULONG                   GdiClientTID;
  PVOID                   GdiThreadLocaleInfo;
  PVOID                   UserReserved[5];
  PVOID                   GlDispatchTable[0x118];
  ULONG                   GlReserved1[0x1A];
  PVOID                   GlReserved2;
  PVOID                   GlSectionInfo;
  PVOID                   GlSection;
  PVOID                   GlTable;
  PVOID                   GlCurrentRC;
  PVOID                   GlContext;
  NTSTATUS                LastStatusValue;
  UNICODE_STRING          StaticUnicodeString;
  WCHAR                   StaticUnicodeBuffer[0x105];
  PVOID                   DeallocationStack;
  PVOID                   TlsSlots[0x40];
  LIST_ENTRY              TlsLinks;
  PVOID                   Vdm;
  PVOID                   ReservedForNtRpc;
  PVOID                   DbgSsReserved[0x2];
  ULONG                   HardErrorDisabled;
  PVOID                   Instrumentation[0x10];
  PVOID                   WinSockData;
  ULONG                   GdiBatchCount;
  ULONG                   Spare2;
  ULONG                   Spare3;
  ULONG                   Spare4;
  PVOID                   ReservedForOle;
  ULONG                   WaitingOnLoaderLock;
  PVOID                   StackCommit;
  PVOID                   StackCommitMax;
  PVOID                   StackReserved;
} TEB, *PTEB;

NTSYSAPI
PTEB
NTAPI
NtCurrentTeb(
	);

// Synchronization ----------------------------------------------------------------
NTSYSAPI 
NTSTATUS
NTAPI
ZwWaitForSingleObject(
	IN HANDLE  Handle,
	IN BOOLEAN  Alertable,
	IN PLARGE_INTEGER  Timeout OPTIONAL
	);

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    USHORT   Type;
    USHORT   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    ULONG EntryCount;
    ULONG ContentionCount;
    ULONG Spare[ 2 ];
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;

typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;

    //
    //  The following three fields control entering and exiting the critical
    //  section for the resource
    //

    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;        // force size on 64-bit systems when packed
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;


NTSYSAPI
NTSTATUS
NTAPI
RtlInitializeCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);

NTSYSAPI
NTSTATUS
NTAPI
RtlEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);

NTSYSAPI
NTSTATUS
NTAPI
RtlLeaveCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);

NTSYSAPI
NTSTATUS
NTAPI
RtlDeleteCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);

// Token ----------------------------------------------------------------
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcessToken(
	IN HANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE TokenHandle
	);

typedef struct _TOKEN_PRIVILEGES {
    ULONG PrivilegeCount;
    LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;

NTSYSAPI
NTSTATUS
NTAPI
ZwAdjustPrivilegesToken(
	IN HANDLE TokenHandle,
	IN BOOLEAN DisableAllPrivileges,
	IN PTOKEN_PRIVILEGES NewState,
	IN ULONG BufferLength,
	OUT PTOKEN_PRIVILEGES PreviousState OPTIONAL,
	OUT PULONG ReturnLength
	);

// Memory Management ================================================================
// Pool Memory----------------------------------------------------------------
NTKERNELAPI
VOID
NTAPI
ExFreePoolWithTag(
	IN PVOID  P,
	IN ULONG  Tag 
	); 

// Heap Memory----------------------------------------------------------------
#define HEAP_NO_SERIALIZE               0x00000001      
#define HEAP_GROWABLE                   0x00000002      
#define HEAP_GENERATE_EXCEPTIONS        0x00000004      
#define HEAP_ZERO_MEMORY                0x00000008 

typedef struct {
	ULONG    	Length;
	ULONG    	Unknown[11];
} RTL_HEAP_DEFINITION, *PRTL_HEAP_DEFINITION;

NTSYSAPI
HANDLE 
NTAPI 
RtlCreateHeap(
	ULONG Flags, 
	PVOID BaseAddress, 
	ULONG SizeToReserve, 
	ULONG SizeToCommit, 
	BOOLEAN Lock,
	PRTL_HEAP_DEFINITION Definition
	);

NTSYSAPI
NTSTATUS
NTAPI
RtlDestroyHeap(
	HANDLE HeapHandle
	);

NTSYSAPI
PVOID 
NTAPI 
RtlAllocateHeap(
	HANDLE Heap, 
	ULONG Flags, 
	ULONG Size 
	);

NTSYSAPI
PVOID
NTAPI
RtlReAllocateHeap(
  HANDLE hHeap,
  ULONG dwFlags,
  PVOID lpMem,
  SIZE_T dwBytes
);

				
NTSYSAPI
BOOLEAN 
NTAPI 
RtlFreeHeap(
	HANDLE Heap, 
	ULONG Flags, 
	PVOID Address 
	);

// Virtual Memory ----------------------------------------------------------------
NTSYSAPI
NTSTATUS 
NTAPI
ZwAllocateVirtualMemory(
    IN HANDLE  ProcessHandle,
    IN OUT PVOID  *BaseAddress,
    IN ULONG  ZeroBits,
    IN OUT PSIZE_T  RegionSize,
    IN ULONG  AllocationType,
    IN ULONG  Protect
    ); 

NTSYSAPI
NTSTATUS 
NTAPI
ZwFreeVirtualMemory(
    IN HANDLE  ProcessHandle,
    IN OUT PVOID  *BaseAddress,
    IN OUT PULONG  RegionSize,
    IN ULONG  FreeType
    ); 


// Executable Images ================================================================
// Environment ----------------------------------------------------------------
NTSYSAPI 
NTSTATUS
NTAPI
RtlQueryEnvironmentVariable_U(
	IN PVOID                Environment OPTIONAL,
	IN PUNICODE_STRING      VariableName,
	OUT PUNICODE_STRING     VariableValue 
	);

NTSYSAPI 
NTSTATUS
NTAPI
RtlExpandEnvironmentStrings_U(
	IN PWCHAR Environment,
	IN PUNICODE_STRING SourceString,
	OUT PUNICODE_STRING DestString,
	OUT PULONG Length
	);

// Loader ----------------------------------------------------------------
NTSYSAPI 
NTSTATUS
NTAPI
LdrGetDllHandle(
	IN LPCWSTR              Path OPTIONAL,
	IN PVOID                Unused OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             pHModule 
	);

NTSYSAPI 
NTSTATUS
NTAPI
LdrLoadDll(
	IN LPCWSTR				PathToFile OPTIONAL,
	IN ULONG                Flags OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle 
	);

NTSYSAPI 
NTSTATUS
NTAPI
LdrGetProcedureAddress(
	IN HANDLE				ModuleHandle,
	IN PANSI_STRING         FunctionName OPTIONAL,
	IN USHORT				Oridinal OPTIONAL,
	OUT PVOID				*FunctionAddress 
	);

NTSYSAPI 
NTSTATUS
NTAPI
LdrUnloadDll(
	IN HANDLE					ModuleHandle 
	);


// Run Time Library ================================================================
// String ----------------------------------------------------------------
NTSYSAPI
ULONG
NTAPI
RtlxUnicodeStringToAnsiSize( 
							PCUNICODE_STRING  UnicodeString 
							);

NTSYSAPI
NTSTATUS 
NTAPI 
RtlUnicodeToMultiByteN(  
					   PCHAR  MbString,  
					   ULONG  MbSize,  
					   PULONG  ResultSize,  
					   PWCHAR  UnicodeString,  
					   ULONG  UnicodeSize 
					   );  
NTSYSAPI
NTSTATUS 
NTAPI
RtlMultiByteToUnicodeN(
					   LPWSTR  dst,
					   ULONG   dstlen,
					   PULONG reslen,
					   LPCSTR  src,
					   ULONG   srclen
					   );

// Path Name ----------------------------------------------------------------
typedef struct _CURDIR {
	UNICODE_STRING 	DosPath;
	HANDLE 			Handle;
} CURDIR, *PCURDIR;

NTSYSAPI
BOOLEAN 
NTAPI 
RtlDosPathNameToNtPathName_U( 	
	IN PCWSTR 			DosPathName, 
	OUT PUNICODE_STRING	NtPathName, 
	OUT PCWSTR *		NtFileNamePart, 
	OUT CURDIR * 		DirectoryInfo
	); 	
typedef enum
{
	INVALID_PATH = 0,
	UNC_PATH,              /* "//foo" */
	ABSOLUTE_DRIVE_PATH,   /* "c:/foo" */
	RELATIVE_DRIVE_PATH,   /* "c:foo" */
	ABSOLUTE_PATH,         /* "/foo" */
	RELATIVE_PATH,         /* "foo" */
	DEVICE_PATH,           /* "//./foo" */
	UNC_DOT_PATH           /* "//." */
} DOS_PATHNAME_TYPE;

NTSYSAPI
DOS_PATHNAME_TYPE
NTAPI
RtlDetermineDosPathNameType_U(
	PCWSTR
	);

// Exception ----------------------------------------------------------------
NTSYSAPI
VOID
NTAPI
RtlUnwind(
	IN PVOID TargetFrame OPTIONAL, 
	IN PVOID TargetIp OPTIONAL, 
	IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL, 
	IN PVOID ReturnValue 
	);

NTSYSAPI
VOID
NTAPI
RtlRaiseException(
	IN PEXCEPTION_RECORD ExceptionRecord
	);
#ifdef __cplusplus
}
#endif

#endif // PLATFORM_TYPE_NATIVE

#endif //_RSNTDDK_H_
