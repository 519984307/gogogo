#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <linux/syscalls.h>
#include <linux/version.h>

#include "debug.h"

typedef void *(*kallsyms_lookup_name_t)(const char *name);

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32))
static kallsyms_lookup_name_t kallsyms_lookup_name_func = NULL;
#else
static kallsyms_lookup_name_t kallsyms_lookup_name_func = (kallsyms_lookup_name_t)kallsyms_lookup_name;
#endif

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32))


static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	return 0;
}

static void _get_kallsyms_lookup_name(void)
{
	int ret = 0;
	struct kprobe kp_exec = {
		.symbol_name = "kallsyms_lookup_name",
		.pre_handler = handler_pre,
	};

#ifndef CONFIG_KPROBES
	return;
#endif

	ret = register_kprobe(&kp_exec);
	if (ret != 0 ) {
		__log_error("cannot find kallsyms_lookup_name by kprobe. ret=%d\n", ret);
		return;
	}

	__log_info("kallsyms_lookup_name at %p\n", (void *)kp_exec.addr);
	unregister_kprobe(&kp_exec);

	kallsyms_lookup_name_func = (kallsyms_lookup_name_t)kp_exec.addr;
}

#endif


void *get_symbol_from_kallsyms(const char *name)
{
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32))
	if (kallsyms_lookup_name_func == NULL) {
		_get_kallsyms_lookup_name();
	}
#endif
	if (kallsyms_lookup_name_func == NULL) {
		return NULL;
	}

	return kallsyms_lookup_name_func(name);
}
