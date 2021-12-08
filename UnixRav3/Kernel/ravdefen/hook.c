#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/syscalls.h>
#include <linux/fs.h>

#include "hook.h"
#include "debug.h"
#include "udis86.h"

void *get_symbol_from_kallsyms(const char *name);


#define RELATIVE_CALL_SIZE (5)
#define CHECK_NUMBER 20

static unsigned long **__search_sys_call_table(void *text_start, int nr_close, void *__sys_close)
{
	// FIXME: 此方式极其不稳定，不适合在企业级产品中使用，故暂时去掉
#if 1
	unsigned long **offset = (unsigned long **)text_start;
	unsigned long nr_linux = 0;

#ifdef __NR_Linux
	nr_linux = __NR_Linux;
#endif
	nr_close -= nr_linux;
	__log_info("find sys_call_table from 0x%p\n", offset);
	__log_info("__NR_close=%d, sys_close=%p\n", nr_close, __sys_close);

	for (; (unsigned long)offset < ULONG_MAX; offset += 1) {
		if (offset[nr_close] == (unsigned long *)__sys_close) {
			int i;
			for (i = 0; i < CHECK_NUMBER; i++) {
				__log_info("[%d] %p\n", i, offset[i]);
				if (offset[i] < (unsigned long *)PAGE_OFFSET) {
					break;
				}
			}
			if (i < CHECK_NUMBER)
				continue;

			__log_info("sys_call_table=%p\n", offset);
			return offset - nr_linux;
		}
	}

#endif
	return NULL;
}

unsigned long **get_sys_call_table(void)
{
	unsigned long **sct = NULL;

	/* 首先尝试从 kallsyms 中获取地址，如果不成功则进行搜索 */
#if !defined(CONFIG_MIPS)
	sct = get_symbol_from_kallsyms("sys_call_table");
#endif
	__log_info("get_symbol_from_kallsyms sys_call_table=%p\n", sct);

	if (sct == NULL) {
		/* 尝试从 _text 开始搜索 */
		void *_text = get_symbol_from_kallsyms("_text");
		_text = (_text == NULL) ? (void *)PAGE_OFFSET : 
			(void *)max((unsigned char *)PAGE_OFFSET, (unsigned char *)_text);
		sct = __search_sys_call_table(_text, __NR_close, sys_close);
	}
	return sct;
}

#ifdef CONFIG_ARM64

unsigned long **get_compat_sys_call_table(void)
{
	unsigned long **csct = NULL;

	/* 首先尝试从 kallsyms 中获取地址，如果不成功则进行搜索 */
	csct = get_symbol_from_kallsyms("compat_sys_call_table");
	__log_info("get_symbol_from_kallsyms compat_sys_call_table=%p\n", csct);

	if (csct == NULL) {
		/* 尝试从 _text 开始搜索 */
		void *_text = get_symbol_from_kallsyms("_text");
		_text = (_text == NULL) ? (void *)PAGE_OFFSET : 
			(void *)max((unsigned char *)PAGE_OFFSET, (unsigned char *)_text);
		csct = __search_sys_call_table(_text, 6, sys_close);
	}
	return csct;
}

#endif


int MEM_make_rw(unsigned long addr)
{
#if (defined(CONFIG_X86) || defined(CONFIG_X86_64))
	pte_t *pte;
	unsigned int level;

	pte = lookup_address(addr, &level);
	if (NULL == pte) {
		return -1;
	}

	if (0 == (pte->pte & _PAGE_RW)) {
		pte->pte |= _PAGE_RW;
	}
#endif

	return 0;
}



#ifdef CONFIG_X86_64

/*******************************************************************
* Name:		MEM_find_insn
* Description:	This function gets a memory address, block
*		size, asm instruction and its size. It disassembles
*		the memory region until it finds the given
*		instruction, or until it reaches the max block size.
*		Finally it returns the offset of the instruction
*		from the given address, or -1 otherwise.
*******************************************************************/
static long MEM_find_insn_off(unsigned long mem_addr,
				size_t block_size,
				int insn_type,
				size_t insn_size)
{
	ud_t ud;

	ud_init(&ud);
	ud_set_input_buffer(&ud, (char *)mem_addr, block_size);
	ud_set_mode(&ud, 64);

	/* Run over the memory region */
	while (ud_disassemble(&ud)) {

		if ((insn_type == ud.mnemonic) && 
				(insn_size == ud_insn_len(&ud)) ) {
			return ud_insn_off(&ud);		
		}

	}
	
	return -1;
}

/*******************************************************************
* Name:		MEM_patch_relative_call
* Description:	This function searches for a relative call in a
*		given memory region. In case a relative call is
*		found, it will patch it to call `new_call_addr`.
*		It saves the original called address in 
*		`orig_call_addr`.
*******************************************************************/
int MEM_patch_relative_call(unsigned long mem_addr,
				size_t block_size,
				unsigned long new_call_addr,
				unsigned long * orig_call_addr)
{
	int ret = 0;
	long call_insn_offset; 
	unsigned long call_insn_addr;
	unsigned long call_relative_val;
	unsigned long new_call_relative_val;


	/* Find the relative call instruction (E8) offset */
	call_insn_offset = MEM_find_insn_off(mem_addr,
						block_size, UD_Icall, 
						RELATIVE_CALL_SIZE);	
	if (-1 == call_insn_offset) {
		__log_error("Error patching the relative call address");
		ret = -1;
		goto cleanup;
	}

	/* Calculate the call instruction address */
	call_insn_addr = (mem_addr + call_insn_offset);

	MEM_make_rw(call_insn_addr);

	call_relative_val = (*((int *) (call_insn_addr + 1)));

	/* Calculate the relative value for calling the new_sys_execve */
	new_call_relative_val = ((unsigned long) new_call_addr - call_insn_addr - RELATIVE_CALL_SIZE);

	/* Save the address of the original sys_execve */
	if (NULL != orig_call_addr) {
		*orig_call_addr = call_insn_addr + RELATIVE_CALL_SIZE + call_relative_val;
	}

	/* Patch */
	(*((int*)(call_insn_addr + 1))) = (int) new_call_relative_val;

cleanup:
	return ret;
}

#endif
