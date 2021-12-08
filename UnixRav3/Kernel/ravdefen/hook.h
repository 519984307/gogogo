
#ifndef __HOOK_H__
#define __HOOK_H__

int MEM_make_rw(unsigned long addr);

#define HOOK_SCT(sct, name)                     	\
	do {                                        	\
		MEM_make_rw((unsigned long)&sct[__NR_##name]);		\
		real_##name = (void *)sct[__NR_##name]; \
		sct[__NR_##name] = (void *)fake_##name; \
	} while (0)

# define UNHOOK_SCT(sct, name)                  	\
	sct[__NR_##name] = (void *)real_##name

#define HOOK_SCT32(_sct, name)                     	\
	do {                                       	\
		MEM_make_rw((unsigned long)&_sct[__NR_##name##32]);	\
		real_##name = (void *)_sct[__NR_##name##32]; \
		_sct[__NR_##name##32] = (void *)fake_##name; \
	} while (0)

# define UNHOOK_SCT32(_sct, name)                  \
	_sct[__NR_##name##32] = (void *)real_##name

unsigned long **get_sys_call_table(void);

#ifdef CONFIG_ARM64
unsigned long **get_compat_sys_call_table(void);
#endif

int MEM_patch_relative_call(unsigned long mem_addr, size_t block_size, unsigned long new_call_addr, unsigned long *orig_call_addr);

#endif /* __HOOK_H__ */
