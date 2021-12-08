#include "types.h"
#include "result.h"
#include "compiler.h"

#include "pefmt.h"

#ifndef _RSVM_VM_PE_INFO_H_
#define _RSVM_VM_PE_INFO_H_

/* imported APIs */
struct import_api_descr {
	char *apiname;
	uint32_t ordinal;
	uint32_t thunk_ptr_rva;
public:
	import_api_descr() : apiname(0) {}
};

/* export functions */
struct export_func_descr {
	uint32_t rva;
	uint32_t ordinal;
	char *funcname;
public:
	export_func_descr() : funcname(0) {}
};

/* import dynamic library descriptor */
struct import_dll_descr {
	char *dynlib_name;
	int nr_apis;
	struct import_api_descr *api_array;
public:
	import_dll_descr() : 
		dynlib_name(0), nr_apis(0), api_array(0) {}
};

struct pe_info_descr {

	/* headers */
	winpe::IMAGE_DOS_HEADER doshdr;
	winpe::IMAGE_NT_HEADERS nthdr;

	/* section header */
	int nr_section;
	winpe::PIMAGE_SECTION_HEADER secthdrs;

	/* tls callbacks */
	uint32_t static_tls_rva;
	uint32_t static_tls_size;
	uint32_t static_tls_zeroed_size;
	gcptr_t tls_cbtable_rva;

	/* export table */
	int nr_exports;
	struct export_func_descr *exp_desc_array;

	/* import table */
	int nr_import_dynlibs;
	struct import_dll_descr *imp_desc_array;

	/* relocation items */
	int nr_reloc_rva;
	uint32_t *reloc_rva_array;

	/* image base */
	gcptr_t image_base;
};

#endif /* _RSVM_VM_PE_INFO_H_ */
