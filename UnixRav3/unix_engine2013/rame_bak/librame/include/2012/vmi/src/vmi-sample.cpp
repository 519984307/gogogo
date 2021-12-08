#include <stdio.h>
#include <string>

#include <vmi.h>
#include <windows.h>

class real_file : public VmiFile {
	__define_vmi_objfre();
public:
	__method(lresult_t) open(const char *name, 
		bool writeable, bool truncate, bool always) {
		fd = fopen(name, "w+b");
		return fd ? RR_OK : RR_FAIL;
	}
	__method(void) close() {
		fclose(fd);
	}
	__method(size_t) read(void *buf, size_t size) {
		return fread(buf, 1, size, fd);
	}
	__method(size_t) write(void *buf, size_t size) {
		return fwrite(buf, 1, size, fd);
	}
	__method(long) lseek(long delta, int origin) {
		return fseek(fd, delta, origin);
	}
	__method(long) tell() { 
		return ftell(fd);
	}
public:
	real_file() : fd(0) {}
	virtual ~real_file() {
		if (fd) fclose(fd);
	}
private:
	FILE *fd;
};
VmiFile *__rsvm_conv alloc_real_file() { return new real_file; }

void vfs_walk(VmiCore *core, const char *root)
{
	bool dirent;
	const char *filename;

	void *fh = core->vfs_find_first(root, &dirent, &filename);
	if (fh) {
		do {
			printf("[%c] %s\\%s\n", dirent ? 'd' : 'f', root, filename);
			if (dirent) {
				std::string dir = root;
				dir += '\\';
				dir += filename;
				vfs_walk(core, dir.c_str());
			}
		} while (core->vfs_find_next(fh, &dirent, &filename));
		core->vfs_find_close(fh);
	}
}

const char *format_string(const char *str)
{
	static char *fmtstr = 0;
	if (fmtstr)
		delete[] fmtstr;
	fmtstr = new char[(strlen(str) + sizeof(char)) * 2];
	if (!fmtstr)
		return str;
	strcpy(fmtstr, str);

	char *outp = fmtstr;
	for (char *p = (char *)str; *p; ++p) {
		if (*p == '\r') {
			*outp++ = '\\';
			*outp++ = 'r';
		} else if (*p == '\n') {
			*outp++ = '\\';
			*outp++ = 'n';
		} else {
			*outp++ = *p;
		}
	}
	*outp = '\0';
	return fmtstr;
}

const char *format_arg(VmiProcess *process, char fmt, gcptr_t val)
{
	static char *s_buf = 0;
	if (!s_buf) {
		s_buf = new char[4096];
		if (!s_buf)
			return 0;
	}

	switch (fmt) {

	case 'H': case 'l': case ' ':
		{
			wsprintf(s_buf, "0x%08x", val);
			return s_buf;
		}

#define MAX_STR_LEN 512
	case 's':
		{
			char buf[MAX_STR_LEN];
			gcptr_t strptr = val;

			int j = 0;
			for (; j < MAX_STR_LEN - 4; ++strptr, ++j) {
				if (!process->read_virtmm(strptr, &buf[j], sizeof(char))) {
					strcpy(&buf[j], "...");
					break;
				}
				if (!buf[j])
					break;
                                /*
                                if (!isprint(buf[j]))
                                        buf[j] = ' ';
                                */
			}
			if (j == MAX_STR_LEN - 4) {
				strcpy(&buf[MAX_STR_LEN - 4], "...");
			}
			wsprintf(s_buf, "0x%08x \"%s\"", val, format_string(buf));
			return s_buf;
		}
	case 'S':
		{
			int16_t buf[MAX_STR_LEN];
			gcptr_t strptr = val;

			int j = 0;
			for (; j < MAX_STR_LEN - 4; strptr += sizeof(int16_t), ++j) {
				if (!process->read_virtmm(strptr, &buf[j], sizeof(int16_t))) {
					memcpy(&buf[j], ".\0.\0.\0\0\0", 8);
					break;
				}
				if (!buf[j])
					break;
                                /*
                                if ((buf[j] >= 0x80 && buf[j] <= 0xFF) || (buf[j] < 0x80 && !isprint((char)buf[j])))
                                        buf[j]= 0x20;
                                */
                        }
			if (j == MAX_STR_LEN - 4) {
				memcpy((int16_t *)&buf[MAX_STR_LEN - 4], ".\0.\0.\0\0\0", 8);
			}
			wsprintf(s_buf, "0x%08x L\"%S\"", val, buf);

			return s_buf;
		}
#undef MAX_STR_LEN

	default:
		return "<unknown>";
	}
}

#define calc_argc(fmt, n) \
	do { \
		const char *p = fmt; \
		for (; *p && *p != '`'; ++p) \
			; \
		n = (int)(p - fmt); \
	} while (0)

bool log_api_info(VmiProcess *process, VmiThread *thread, struct VmiApiInfo *ai)
{
	int argc;
	calc_argc(ai->argfmt, argc);
	if (argc < 1)
		return true;

	struct VmiContext ctx;
	ctx.mask = CmCommReg;
	thread->get_context(&ctx);

	gcptr_t *callstack = new gcptr_t[argc];
	if (callstack) {
		if (!process->read_virtmm(ctx.sp, callstack, sizeof(gcptr_t) * argc)) {
			delete[] callstack;
			callstack = 0;
		}
	}

	struct VmiProcessInfo pi;
	process->info(&pi);

	struct VmiThreadInfo ti;
	thread->info(&ti);

	if (ai->emulated_p) {
		printf("[ ] ");
	} else {
		printf("[X] ");
	}

	{
		printf("<%d \"%s\" : %d> ", pi.pid, pi.process_name, ti.tid);
	}

	/* return address */
	{
		if (!callstack)
			printf("retaddr=<unknown>");
		else {
			printf("retaddr=<0x%08x> ", callstack[0]);
		}
	}
	/* api name */
	{
		if (ai->apiname) {
			printf("%s!%s(", ai->modname, ai->apiname);
		} else {
			printf("%s!ord_%04x(", ai->modname, ai->ordinal);
		}
	}
	/* arguments */
	do {
		if (argc < 2 || !callstack)
			break;

		std::string str;
		for (int i = 1; i < argc; ++i) {
			str += format_arg(process, ai->argfmt[i], callstack[i]);
			if (i != argc - 1)
				str += ", ";
		}

		printf("%s", str.c_str());
	} while (0);
	printf(") ");

	/* return value */
	{
		printf("=> %s\n", format_arg(process, ai->argfmt[0], ctx.ax));
	}

	delete[] callstack;

	return true;
}

class vmi_cb_t : public VmiCallback {

public:
	/* called on api called */
	/* NOTE: return _false_ in callback routine to STOP execution */
	__method(bool) on_api_called(struct VmiApiInfo *info, void *opaque) {
		if (!thread)
			return true;
		printf("[CB] ");
		log_api_info(process, thread, info);
		return true;
	}

	/* called on messages need to be logged */
	__method(void) on_log_debug(int level, const char *message, void *opaque) {}

public:
	VmiProcess *process;
	VmiThread *thread;
};

vmi_cb_t core_cb;

int main(int argc, char *argv[])
{
	lresult_t rc;

	VmiCore *core;
	rc = vmi_create(&core);
	if (RR_FAILED(rc))
		return 1;

	/* timeout in 1 seconds */
	core->set_timeout(1000);

	/* register callback */
	core->register_callback(&core_cb);

	core->set_switches(VmiUseDummyDynlib | VmiUseDummyApiAddr);

	/* enum virtual files */
	//vfs_walk(core, "c:");

	//core->set_target_fop_class(alloc_real_file);

	VmiProcess *process;
	VmiThread *thread;

	/* run it several times */
	for (int i = 0; i < 1; ++i) {

		rc = core->vfs_map_file("c:\\runme.exe", "c:\\1.ex_");
		if (RR_FAILED(rc))
			return 2;

		rc = core->load_target("c:\\runme.exe", &process, &thread);
		if (RR_FAILED(rc))
			return 3;

		core_cb.process = process;
		core_cb.thread = thread;

		VmiModule *k32;
		if (RR_FAILED(process->get_module("kernel32", &k32)))
			return 4;
		printf("kernel32!LoadLibraryA = %08x\n", k32->get_symbol_va("LoadLibraryA"));
		k32->free();

		VmiContext ctx;
		ctx.mask = CmSegReg;
		thread->get_context(&ctx);

		struct VmiApiInfo api_info;
		struct VmiExcepInfo excep_info;

		int gr;
		do {
			gr = process->go(MskApi | MskException, &api_info, &excep_info);			
			if (gr == GrApi) {
				log_api_info(process, thread, &api_info);
			} else if (gr == GrException) {
				printf("Exception: %d, SEH handler = %08x\n", excep_info.trapno, excep_info.seh_handler);
			} else if (gr == GrExceedLimit) {
				printf("Out of limit, stopped running!\n");
			} else {
				/* ... */
			}

		} while (gr == GrApi || gr == GrException);

		real_file *rf = 0;
		do {
			char dmpname[256];
			wsprintfA(dmpname, "c:\\dump_%d.bin", i);

			rf = new real_file;
			if (!rf || rf->open(dmpname, true, true, true))
				break;

			process->dump_image(rf, ctx.ip, 0, 0, DmNormal);
		} while (0);

		delete rf;
		thread->free();
		process->free();
	}

	core->free();

	return 0;
}
