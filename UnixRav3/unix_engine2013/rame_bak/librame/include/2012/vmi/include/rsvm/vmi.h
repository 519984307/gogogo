#include "types.h"
#include "result.h"
#include "compiler.h"
#include "peinfo.h"

#ifndef _RSVM_VM_INTERFACE_H_
#define _RSVM_VM_INTERFACE_H_

#define __method(rt) virtual RsvmConv(rt)
#define __mtdimpl(rt) RsvmConv(rt)

/* cpu context */
struct VmiContext {

	uint32_t mask;
#define CmCommReg	0x01
#define CmSpecReg	0x02
#define CmSegReg	0x04
#define CmControlReg	0x08
#define CmDebugReg	0x10
#define CmStateReg	0x20 /* NOTE: readonly, cannot be set */
#define CmAll		-1

	/* CmCommReg */
	__align(gcptr_size, gcptr_t) ax;
	__align(gcptr_size, gcptr_t) cx;
	__align(gcptr_size, gcptr_t) dx;
	__align(gcptr_size, gcptr_t) bx;
	__align(gcptr_size, gcptr_t) sp;
	__align(gcptr_size, gcptr_t) bp;
	__align(gcptr_size, gcptr_t) si;
	__align(gcptr_size, gcptr_t) di;

	/* CmSpecReg */
	__align(gcptr_size, gcptr_t) fl;
	__align(gcptr_size, gcptr_t) ip;

	/* CmSegReg */
	__align(gcptr_size, uint16_t) cs;
	__align(gcptr_size, uint16_t) ds;
	__align(gcptr_size, uint16_t) es;
	__align(gcptr_size, uint16_t) ss;
	__align(gcptr_size, uint16_t) fs;
	__align(gcptr_size, uint16_t) gs;

	/* CmControlReg */
	__align(gcptr_size, gcptr_t) cr0;
	__align(gcptr_size, gcptr_t) cr2;
	__align(gcptr_size, gcptr_t) cr3;
	__align(gcptr_size, gcptr_t) cr4;

	/* CmDebugReg */
	__align(gcptr_size, gcptr_t) dr0;
	__align(gcptr_size, gcptr_t) dr1;
	__align(gcptr_size, gcptr_t) dr2;
	__align(gcptr_size, gcptr_t) dr3;
	__align(gcptr_size, gcptr_t) dr6;
	__align(gcptr_size, gcptr_t) dr7;

	/* CmStateReg */
	__align(gcptr_size, gcptr_t) last_ip;

	__align(gcptr_size, gcptr_t) image_wr_start;
	__align(gcptr_size, gcptr_t) image_wr_end;
	__align(gcptr_size, long) image_wr_cnt;

	__align(gcptr_size, gcptr_t) stack_wr_start;
	__align(gcptr_size, gcptr_t) stack_wr_end;
	__align(gcptr_size, long) stack_wr_cnt;

	__align(gcptr_size, gcptr_t) mem_wr_start;
	__align(gcptr_size, gcptr_t) mem_wr_end;
	__align(gcptr_size, long) mem_wr_cnt;
};
#define __comm_reg_array(vmi_context) ((gcptr_t *)((uint8_t *)(vmi_context) + offsetof(struct VmiContext, ax)))

/* api info */
struct VmiApiInfo {
	uint32_t unique_id;
	const char *modname;
	const char *argfmt;
	const char *apiname;
	uint16_t ordinal; /* available when apiname == 0 */
	bool emulated_p;
};

/* exception info */
struct VmiExcepInfo {
	int trapno;
#define TrapDiv0		0x00,
#define TrapDebug		0x01,
#define TrapBreakpt		0x03,
#define TrapOverflow		0x04,
#define TrapBound		0x05,
#define TrapInvlOpcode		0x06,
#define TrapNmi			0x07,
#define TrapDblFault		0x08,
#define TrapCopSegOver		0x09,
#define TrapTss			0x0a,
#define TrapSegNP		0x0b,
#define TrapStackFault		0x0c,
#define TrapGP			0x0d,
#define TrapPageFault		0x0e,
#define TrapFloatErr		0x10,
#define TrapAlign		0x11,
#define TrapMC			0x12,
#define TrapSIMD		0x13,
#define TrapRsvmSyscall		0x2b,
#define TrapNTSyscall		0x2e,
#define TrapIa32Sysenter	0x80,
#define TrapHardInt		0x7ffffffe,
#define TrapVMMAbout		0x7fffffff,
#define TrapInvlParam		-1,
#define TrapInternErr		-2,
	gcptr_t error_code;
	gcptr_t trap_va; /* do not care about trap/fault */
	gcptr_t seh_handler;
};

/* data directory, used by dump functions */
struct VmiDataDir {
	gcptr_t rva;
	uint32_t size;
};

/* process information */
struct VmiProcessInfo {
	uint32_t pid;
	const char *current_path;
	const char *process_name;
	gcptr_t entry_va;
	gcptr_t peb_va;
	gcptr_t image_base;
	uint32_t image_size;
	gcptr_t kernel32_base;
	gcptr_t ntdll_base;
};

/* process state */
struct VmiProcessState {
	long nr_api_called;
	long nr_excp_occured;
	long time_elapsed; /* milliseconds */
};

/* thread information */
struct VmiThreadInfo {
	uint32_t tid;
	gcptr_t entry_va;
	gcptr_t teb_va;
	gcptr_t stack_base;
	uint32_t stack_size;
	uint32_t max_stack_size;
};

/* module information */
struct VmiModuleInfo {
	gcptr_t base;
	uint32_t size;
	gcptr_t entry_va;
	const char *module_name;
	const char *path_name;
	uint32_t type;
#define MtMainImage 0
#define MtKnownModuleImage 1
#define MtNormalImage 2
#define MtDataFileImage 3
#define MtUnknownImage -1
};

/* object information */
struct VmiObjInfo {
	uint32_t type; /* magic values */
	const char *objname; /* may be invalid later... */
};

/* virtmm region information */
struct VmiVMRegion {
	gcptr_t base;
	gcptr_t alloc_base;
	uint32_t size;
	uint32_t attrib; /* combination of PAGE_XXX from windows.h */
	uint32_t alloc_attrib;
	const char *filename; /* filename related to this region, may be nil */
};

/* all object starts here */
class VmiObject {

public:
	__method(void) free() = 0;
};
#define __define_vmi_objfre() \
	public: __method(void) free() { delete this; }

/* file operation */
class VmiFile : public VmiObject {

public:
	/* open/close */
	__method(lresult_t) open(const char *name, 
		bool writeable, bool truncate, bool always) = 0;
	__method(void) close() = 0;

public:
	/* accessors */
	__method(size_t) read(void *buf, size_t size) = 0;
	__method(size_t) write(void *buf, size_t size) = 0;

public:
	/* file pointer operations */
	__method(long) lseek(long delta, int origin) = 0;
	__method(long) tell() = 0;
};

/* classified file allocator */
typedef VmiFile *(__rsvm_conv *VmiFileClass)();

/* process module */
class VmiModule : public VmiObject {

public:
	/* get information */
	__method(void) info(struct VmiModuleInfo *i) = 0;

public:
	/* get symbol address */
	__method(gcptr_t) get_symbol_va(uint16_t ordinal) = 0;
	__method(gcptr_t) get_symbol_va(const char *symname) = 0;
};

/* process */
class VmiProcess : public VmiObject {

public:
	/* get information */
	__method(void) info(struct VmiProcessInfo *i) = 0;

	/* get process state information */
	__method(void) stat(struct VmiProcessState *s) = 0;

public:
	/* select virtual cpu */
	__method(lresult_t) selvcpu(bool natvcpu) = 0;

public:
	/* reset timeout counter */
	__method(void) clear_timeout_counter() = 0;

public:
	/* execution control, w/ auto thread scheduler in process range */
	__method(int) go(int stop_mask, /* refer to VmiThread::go */
		struct VmiApiInfo *api_info, /* only valid when (gr & GrApi) */
		struct VmiExcepInfo *excep_info /* only valid when (gr & GrException) */) = 0;

public:
	/* ignore the exception last occured (means DO NOT CALL SEH HANDLER) */
	__method(void) ignore_last_exception() = 0;

public:
	/* virtual memory (de)allocator */
	__method(gcptr_t) alloc_virtmm(gcptr_t va_wanted, uint32_t length, uint32_t attrib) = 0;
#define PgAttrNoAcc	0x00000001
#define PgAttrRdOnly	0x00000002
#define PgAttrRdWr	0x00000004
#define PgAttrExec	0x00000010
#define PgAttrExecRd	0x00000020
#define PgAttrExecRdWr	0x00000040
#define PgAttrGuard	0x00000100
	__method(lresult_t) free_virtmm(gcptr_t guest_va, uint32_t length) = 0;

	/* query virtual memory region */
	__method(lresult_t) query_virtmm(gcptr_t guest_va, struct VmiVMRegion *info) = 0;

	/* change page attribute */
	__method(lresult_t) protect_virtmm(gcptr_t guest_va, uint32_t length, 
		uint32_t attrib, uint32_t *old_attrib) = 0;

public:
	/* virtual memory control */
	__method(int) read_virtmm(gcptr_t va, void *buf, int size) = 0;
	__method(int) write_virtmm(gcptr_t va, void *buf, int size) = 0;

public:
	/* load module object */
	__method(lresult_t) load_module(const char *modname, VmiModule **module) = 0;

	/* get module object */
	__method(lresult_t) get_module(gcptr_t va, VmiModule **module) = 0;
	__method(lresult_t) get_module(const char *modname, VmiModule **module) = 0;

public:
	/* query object information by handle */
	__method(lresult_t) query_obj_info(gcptr_t obj_handle, struct VmiObjInfo *i) = 0;

public:
	/* get origin image data directories from file */
	/* @return: real number of data directories got */
	__method(int) get_origin_data_dirs(int nr_data_dirs, struct VmiDataDir *out_data_dirs) = 0;

public:
	/* dump image */
	/* NOTE: file binded w/ _file_ops_ should be already open w/ write access */
	__method(lresult_t) dump_image(VmiFile *file_ops, gcptr_t entrypt,
		int nr_data_dirs, struct VmiDataDir *data_dirs, int mode) = 0;
#define DmNormal	0
#define DmExtraHeader	1

/**
 * breakpoint manager 
 */
public:
	/* insert a breakpoint */
	__method(lresult_t) ins_breakpt(int type, 
		gcptr_t guest_va, uint32_t length) = 0;
#define BpExecute	0
#define BpHwVMRead	1
#define BpHwVMWrite	2
#define BpHwExecute	3
#define BpTrace		4

	/* remove a breakpoint */
	__method(lresult_t) del_breakpt(int type, gcptr_t guest_va) = 0;

	/* disable/enable breakpoint */
	__method(lresult_t) enable_breakpt(int type, gcptr_t guest_va, bool enable = true) = 0;

	/**
	 * get breakpoint status 
	 *   < 0 : not exists
	 *   = 0 : disabled
	 *   > 0 : enabled
	 */
	__method(int) stat_breakpoint(int type, gcptr_t guest_va) = 0;

	/* is address breakpointed */
	/* < 0: no breakpoint found at given address
	   >= 0: one of BpXxx breakpoint */
	__method(int /* type */) typeof_breakpoint(gcptr_t guest_va) = 0;

	/* clear all breakpoints */
	__method(void) clear_all_bps() = 0;

	//modified :true for yes
	__method(lresult_t) is_modify_code(bool& modified) = 0;
};

/* thread */
class VmiThread : public VmiObject {

public:
	/* get information */
	__method(void) info(struct VmiThreadInfo *i) = 0;

public:
	/* execution control */
	__method(int) go(
		int type /* one of TypGoXxx */, 
		int stop_mask /* combo of MskXxx or just 0 */,
		struct VmiApiInfo *api_info, /* only valid when (gr & GrApi) */
		struct VmiExcepInfo *excep_info /* only valid when (gr & GrException) */) = 0;
	/* go type */
#define TypGoooo		0
#define TypGoStep		1
#define TypGoJump		2
#define TypGoDecode		3
#define TypGoLoopBegin		4
#define	TypGoLoopEnd		5
#define TypGoModifiedCode	6
#define TypGoCrossPage		7
#define TypGoCrossMMReg		8
#define TypGoCrossImgReg	9
#define TypGoScan		10
	/* stop mask */
#define MskFail			0
#define MskApi			1
#define MskException		2
	/* go result */
#define GrDone			0
#define GrBreakpt		1
#define GrApi			2
#define GrException		3
#define GrExceedLimit		4
#define GrScanRcBase		10
#define GrJump			(GrScanRcBase + TypGoJump)
#define GrDecode		(GrScanRcBase + TypGoDecode)
#define GrLoopBegin		(GrScanRcBase + TypGoLoopBegin)
#define GrLoopEnd		(GrScanRcBase + TypGoLoopEnd)
#define GrModifiedCode		(GrScanRcBase + TypGoModifiedCode)
#define GrCrossPage		(GrScanRcBase + TypGoCrossPage)
#define GrCrossMMReg		(GrScanRcBase + TypGoCrossMMReg)
#define GrCrossImgReg		(GrScanRcBase + TypGoCrossImgReg)
#define GrUnsupported		-1
#define GrInvlParam		-2
#define GrIntrnErr		-3
#define GrStopped		-4
#define GrExecFailed(gr)	((gr) < 0)

public:
	/* thread context manipulation */
	__method(lresult_t) get_context(struct VmiContext *ctx) = 0;
	__method(lresult_t) set_context(struct VmiContext *ctx) = 0;

public:
	/* get process object */
	__method(lresult_t) get_process(VmiProcess **process) = 0;
};

/* target (PE) file information */
struct VmiPeInfo {

	/* file object */
	VmiFile *fileobj;

	/* informations */
	pe_info_descr info;
};

/* vm level callbacks */
class VmiCallback {

public:
	/* called on api called */
	/* NOTE: return _false_ in callback routine to STOP execution */
	__method(bool) on_api_called(struct VmiApiInfo *info, void *opaque) = 0;

	/* called on messages need to be logged */
	__method(void) on_log_debug(int level, const char *message, void *opaque) = 0;
};

/* vm core */
class VmiCore : public VmiObject {

/**
 * configurations
 */
public:
	/* set timeout, in milliseconds */
	__method(void) set_timeout(uint32_t timeout) = 0;

	/* set switches */
	__method(void) set_switches(uint32_t switch_mask) = 0;
#define VmiUseDummyDynlib	1
#define VmiUseDummyApiAddr	2
#define VmiAutoLoadDynlib	4
#define VmiDefaultSwitch	0

/**
 * callback control
 */
public:
	/* register/deregister callback */
	__method(void) register_callback(VmiCallback *cb, void *opaque = 0) = 0;
	__method(void) deregister_callback(VmiCallback *cb) = 0;

/**
 * process/thread control 
 */
public:
	/* create process */
	/* NOTE: i named these two methods different names
	   'cause FUCKING MSVC swaps vftable when same function name used */
	__method(lresult_t) load_target(const char *vfs_name, 
		VmiProcess **proc, VmiThread **thread) = 0;
	__method(lresult_t) load_parsed_target(const char *disp_name, const VmiPeInfo *peinfo,
		VmiProcess **proc, VmiThread **thread) = 0;

	/* get current thread */
	/* call ->free() when not using acquired objects any more */
	__method(lresult_t) get_cur_thread(VmiThread **thread) = 0;

	/* execution control, w/ auto thread scheduler in system range */
	__method(int) go(int stop_mask, /* refer to VmiThread::go */
		struct VmiApiInfo *api_info, /* only valid when (gr == GrApi) */
		struct VmiExcepInfo *excep_info /* only valid when (gr == GrException) */) = 0;

/**
 * file system control
 */
public:
	/* set file operation classes */
	__method(lresult_t) set_target_fop_class(VmiFileClass cls) = 0;
	__method(lresult_t) set_lib_fop_class(VmiFileClass cls) = 0;
	__method(lresult_t) set_drop_fop_class(VmiFileClass cls) = 0;

public:
	/* map file to vfs */
	__method(lresult_t) vfs_map_file(const char *vfs_filename, 
		const char *entity_filename, bool library = false) = 0;

	/* vfs enumeration */
	__method(void *) vfs_find_first(const char *dir, bool *dirent, const char **filename) = 0;
	__method(bool) vfs_find_next(void *find_handle, bool *dirent, const char **filename) = 0;
	__method(void) vfs_find_close(void *find_handle) = 0;
};

/* create vm instance */
RsvmConv(lresult_t) vmi_create(VmiCore **vmicore);

#endif /* _RSVM_VM_INTERFACE_H_ */


