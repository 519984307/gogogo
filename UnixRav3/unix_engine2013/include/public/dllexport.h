#ifndef dllexport_h__2007_06_27_
#define dllexport_h__2007_06_27_


#if	defined(PLATFORM_TYPE_WIN32) //win32

#define		COMPONENT_DLL		"recomp.dll"
#define		FILESYSTEM_DLL		"refs.dll"
#define		FFR_DLL				"ffr.dll"
#define		PEARCHIVE_DLL		"pearc.dll"
#define		LIBLOADER_DLL		"relibldr.dll"
#define		VIRUSLIB_DLL		"viruslib.dll"
#define		SCANEXEC_DLL		"scanexec.dll"
#define		POSTTRT_DLL			"posttrt.dll"
#define		UNEXE_DLL			"unexe.dll"

#define		EXTFILE_DLL			"extfile.dll"
#define		EXTMAIL_DLL			"extmail.dll"
#define		EXTOLE_DLL			"extole.dll"
#define		SCANMAC_DLL			"scanmac.dll"
#define		SCANSCT_DLL			"scansct.dll"
#define		SCANELF_DLL			"scanelf.dll"
#define		REVM_DLL			"revm.dll"
#define		SCRIPTCI_DLL		"scriptci.dll"
#define		SCANPACK_DLL		"scanpack.dll"
#define		UROUTINE_DLL		"uroutine.dll"
#define		SCANEX_DLL			"scanex.dll"
#define		NVFILE_DLL			"nvfile.dll"
#define		MVENGINE_DLL		"mvengine.dll"

#elif defined (PLATFORM_TYPE_NATIVE) //native

#define		COMPONENT_DLL		"rebscomp.dll"
#define		FILESYSTEM_DLL		"rebsfs.dll"
#define		FFR_DLL				"rebsffr.dll"
#define		PEARCHIVE_DLL		"rebspe.dll"
#define		LIBLOADER_DLL		"rebslib.dll"
#define		VIRUSLIB_DLL		"rebsvir.dll"
#define		SCANEXEC_DLL		"rebsscn.dll"
#define		POSTTRT_DLL			"rebspst.dll"
#define		UNEXE_DLL			"rebsvm.dll"

#define		EXTFILE_DLL			"extfile.dll"
#define		EXTMAIL_DLL			"extmail.dll"
#define		EXTOLE_DLL			"extole.dll"
#define		SCANMAC_DLL			"scanmac.dll"
#define		SCANSCT_DLL			"scansct.dll"
#define		SCANELF_DLL			"scanelf.dll"
#define		REVM_DLL			"revm.dll"
#define		SCRIPTCI_DLL		"scriptci.dll"
#define		SCANPACK_DLL		"scanpack.dll"
#define		UROUTINE_DLL		"uroutine.dll"
#define		SCANEX_DLL			"scanex.dll"
#define		NVFILE_DLL			"nvfile.dll"
#define		MVENGINE_DLL		"mvengine.dll"

#elif defined (PLATFORM_TYPE_POSIX) //posix

#define		COMPONENT_DLL		"recomp.so"
#define		FILESYSTEM_DLL		"refs.so"


#endif




#endif // dllexport_h__

