#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fdtable.h>
#include <linux/file.h>
#include <linux/namei.h>
#include <linux/pid.h>

#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/mount.h>
#include <linux/netdevice.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#include <panzer_if.h>
#include "hook.h"
#include "ukcom.h"
#include "_crc32.h"
#include "obj_tbl.h"
#include "debug.h"

/* compat_sys_call_table
 * copy from include/asm/unistd32.h
*/
#define __NR_write32 4
#define __NR_execve32 11
#define __NR_close32 6

static unsigned long **sct;

#ifdef CONFIG_ARM64
static unsigned long **csct;
#endif
 
static struct ot_header ot_header;
static struct ukcom_channel ukcom;
int modify_sync_onoff = 0;

asmlinkage long
fake_write(unsigned int fd, const char __user *buf, size_t count);

asmlinkage long fake_close(unsigned int fd);

asmlinkage long
fake_ioctl(unsigned int fd, unsigned int cmd,
				unsigned long arg);
asmlinkage long
fake_execve(const char __user *name,  const char __user *const __user *argv,
		const char __user *const __user *envp, struct pt_regs *regs);
asmlinkage long
fake_rename(const char __user *oldname, const char __user *newname);

asmlinkage long
fake_renameat(int olddfd, const char __user * oldname,
		int newdfd, const char __user * newname);

asmlinkage long
fake_renameat2(int olddfd, const char __user * oldname,
		int newdfd, const char __user * newname, unsigned int flags);

asmlinkage long
(*real_write)(unsigned int fd, const char __user *buf, size_t count);

asmlinkage long (*real_close)(unsigned int fd);

asmlinkage long
(*real_ioctl)(unsigned int fd, unsigned int cmd,
				unsigned long arg);
asmlinkage long
(*real_execve)(const char __user *name,  const char __user *const __user *argv,
		const char __user *const __user *envp, struct pt_regs *regs);

asmlinkage long
(*real_rename)(const char __user *oldname, const char __user *newname);

asmlinkage long
(*real_renameat)(int olddfd, const char __user * oldname,
		int newdfd, const char __user * newname);

asmlinkage long
(*real_renameat2)(int olddfd, const char __user * oldname,
		int newdfd, const char __user * newname, unsigned int flags);

void procfile_cleanup_proc(void);
void procfile_init_proc(void);

static char *get_fullpath(unsigned int fd, char *name);

static enum user_verdict get_verdict(enum action action, const char *pathname, const char *pathname2)
{
	struct info_header *msg;
	enum user_verdict verdict = UV_pass;
	char *curdir = NULL;
	int sync = 0;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,38))
	struct pid *pid_st;
#endif
	pid_t pid = -1;

	if (action == ACTION_write && modify_sync_onoff == 0) {
		return UV_pass;
	}

	/* ignore /dev /sys /proc */
	if (pathname[0] != '/'
		|| strncmp(pathname, "/dev", 4) == 0
		|| strncmp(pathname, "/sys", 4) == 0
		|| strncmp(pathname, "/proc", 5) == 0) {
			return UV_pass;
	}

	msg = vmalloc(sizeof(struct info_header));
	if (msg == NULL) {
		return UV_pass;
	}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,38))
	pid_st = get_task_pid(current, PIDTYPE_PID);
	if (pid_st != NULL) {
		pid = pid_nr(pid_st); /* current->pid */
	}
#else
	pid = current->pid;
#endif

	msg->action = action;
	msg->pid = pid;
	msg->verdict = UV_pass;

	curdir = get_fullpath(AT_FDCWD, NULL);
	if (curdir != NULL) {
		strncpy(msg->curdir, curdir, MAX_PATHNAME);
		msg->curdir[MAX_PATHNAME] = '\0';
		kfree(curdir);
	}

	strncpy(msg->pathname, pathname, MAX_PATHNAME);
	msg->pathname[MAX_PATHNAME] = '\0';
	if (pathname2 != NULL) {
		strncpy(msg->rename.newname, pathname2, MAX_PATHNAME);
		msg->rename.newname[MAX_PATHNAME] = '\0';
	}

	if (action == ACTION_execve || action == ACTION_rename) {
		sync = 1;
	}
	__log_info("[%s:%d] %s\r\n",
			s_action(action), pid, pathname);
	__ukcom_trigger(&ukcom, PANZER_USER_PID, msg, sizeof(struct info_header), sync, 30);
	verdict = msg->verdict;
	__log_info("[%s:%d] %s uv=%d\r\n",
			s_action(action), pid, pathname, verdict);

	vfree(msg);

	return verdict;
}

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE <= KERNEL_VERSION(4,19,0))

#define MAX_RELATIVE_CALL_OFFSET (150)

static int hook_sys_execve(unsigned long sys_call_table_addr)
{
	int ret = 0;
	unsigned long orig_stub_execve_addr;
	unsigned long orig_call_addr;

	/* Get stub_execve address */
	orig_stub_execve_addr = ((unsigned long *)(sys_call_table_addr))[__NR_execve];

	/* Patch the relative call instruction.
	 * Replace sys_execve with fake_sys_execve */
	ret = MEM_patch_relative_call(orig_stub_execve_addr,
					MAX_RELATIVE_CALL_OFFSET,
					(unsigned long) fake_execve, &orig_call_addr);
	if (0 != ret) {
		goto cleanup;
	}

	/* Backup the original sys_execve address */
	real_execve = (void * ) orig_call_addr;

cleanup:
	return ret;
}

static void remove_hook_sys_execve(unsigned long sys_call_table_addr)
{
	unsigned long orig_stub_execve_addr;

	/* Get stub_execve address */
	orig_stub_execve_addr = ((unsigned long *)(sys_call_table_addr))[__NR_execve];

	/* Patch the relative call instruction.
	 * Replace fake_sys_execve with the original sys_execve */
	MEM_patch_relative_call(orig_stub_execve_addr,
					MAX_RELATIVE_CALL_OFFSET,
					(unsigned long) real_execve, NULL);
}

#endif

int init_panzer_module(void)
{
#ifdef CONFIG_ARM64
	__log_info("Hello ARM64!\n");
#else
	__log_info("Hello!\n");
#endif

	ot_init(&ot_header, 100);
	__init_ukcom(&ukcom, PANZER_PROTO, 0);

	// procfile_init_proc();

	/* No consideration on failure. */
	sct = get_sys_call_table();
	__log_info("sys_call_table = %p\n", sct);
	if (sct != NULL) {
#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE <= KERNEL_VERSION(4,19,0))
		hook_sys_execve((unsigned long)sct);
#else
		HOOK_SCT(sct, execve);		/* 进程启动 */
#endif
		HOOK_SCT(sct, write);		/* 修改 */
		HOOK_SCT(sct, ioctl);
		HOOK_SCT(sct, close);
#ifndef CONFIG_ARM64
		HOOK_SCT(sct, rename);
#endif
		HOOK_SCT(sct, renameat);
#ifndef CONFIG_X86_64
		HOOK_SCT(sct, renameat2);
#endif
	}

#ifdef CONFIG_ARM64
	csct = get_compat_sys_call_table();
	__log_info("compat_sys_call_table = %p\n", csct);
	if (csct != NULL) {
		HOOK_SCT32(csct, write);		/* 修改 */
		HOOK_SCT32(csct, close);
	}
#endif

	return 0;
}

void exit_panzer_module(void)
{

#ifdef CONFIG_ARM64
	if (csct != NULL) {
		UNHOOK_SCT32(csct, write);
		UNHOOK_SCT32(csct, close);
	}
#endif

	if (sct != NULL) {
#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE < KERNEL_VERSION(4,6,0))
		remove_hook_sys_execve((unsigned long)sct);
#else
		UNHOOK_SCT(sct, execve);
#endif
		UNHOOK_SCT(sct, write);
		UNHOOK_SCT(sct, ioctl);
		UNHOOK_SCT(sct, close);
#ifndef CONFIG_ARM64
		UNHOOK_SCT(sct, rename);
#endif
		UNHOOK_SCT(sct, renameat);
#ifndef CONFIG_X86_64
		UNHOOK_SCT(sct, renameat2);
#endif
	}

	// procfile_cleanup_proc();

	__deinit_ukcom(&ukcom);
	ot_free(&ot_header);

	__log_info("Buy!\n");

	return;
}

#if 0
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32))
static char *ccs_get_dentry_path(struct dentry *dentry, char * const buffer,
				 const int buflen)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
	char *pos = ERR_PTR(-ENOMEM);
	if (buflen >= 256) {
		pos = dentry_path_raw(dentry, buffer, buflen - 1);
		if (!IS_ERR(pos) && *pos == '/' && pos[1] &&
		    d_is_dir(dentry)) {
			buffer[buflen - 2] = '/';
			buffer[buflen - 1] = '\0';
		}
	}
	return pos;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 38)
	char *pos = ERR_PTR(-ENOMEM);
	if (buflen >= 256) {
		/* rename_lock is locked/unlocked by dentry_path_raw(). */
		pos = dentry_path_raw(dentry, buffer, buflen - 1);
		if (!IS_ERR(pos) && *pos == '/' && pos[1]) {
			struct inode *inode = dentry->d_inode;
			if (inode && S_ISDIR(inode->i_mode)) {
				buffer[buflen - 2] = '/';
				buffer[buflen - 1] = '\0';
			}
		}
	}
	return pos;
#else
	char *pos = buffer + buflen - 1;
	if (buflen < 256)
		return ERR_PTR(-ENOMEM);
	*pos = '\0';
	if (dentry->d_inode && S_ISDIR(dentry->d_inode->i_mode))
		*--pos = '/';
	spin_lock(&dcache_lock);
	while (!IS_ROOT(dentry)) {
		struct dentry *parent = dentry->d_parent;
		const char *name = dentry->d_name.name;
		const int len = dentry->d_name.len;
		pos -= len;
		if (pos <= buffer) {
			pos = ERR_PTR(-ENOMEM);
			break;
		}
		memmove(pos, name, len);
		*--pos = '/';
		dentry = parent;
	}
	spin_unlock(&dcache_lock);
	return pos;
#endif
}

static char *get_abs_path(const char *pathname, char *buffer, int buffer_len)
{
	struct path ph;
	char *abspath = NULL;
	int rc;

	rc = user_path(pathname, &ph);
	if (rc < 0) {
		__log_error("user_path %d\n", rc);
		return NULL;
	}

	abspath = ccs_get_dentry_path(ph.dentry, buffer, buffer_len);
	if (IS_ERR(abspath)) {
		__log_error("dentry_path_raw: %p\n", abspath);
		return NULL;
	}
	return abspath;
}
#endif
#endif

static struct inode *get_inode(unsigned int fd)
{
	struct inode *inode = NULL;
	struct file *file = NULL;

	if (((int)fd) < 0) {
		__log_error("fd is %d..\n", (int)fd);
		return NULL;
	}

	file = fget(fd);
	if (file == NULL) {
		__log_error("file is null.. fd=%d\n", fd);
		return NULL;
	}

	inode = file->f_inode;

	fput(file);

	return inode;
}

static char *get_file_path(unsigned int fd, char *file_path)
{
	struct file *file = NULL;

	if (((int)fd) < 0) {
		__log_error("fd is %d..\n", (int)fd);
		return NULL;
	}

	file = fget(fd);
	if (file == NULL) {
		__log_error("file is null.. fd=%d\n", fd);
		return NULL;
	}

	file_path = d_path(&(file->f_path), file_path, PATH_MAX - 1);

	fput(file);

	return IS_ERR(file_path) ? NULL : file_path;
}

int __strlen_from_user(const char __user *userstring)
{
	char ch;
	int len = 0;
	char *str = (char *)userstring;

	while (true) {
		get_user(ch, str);
		str++;
		if (ch == '\0')
			break;
		len++;
	}

	return len;
}

char *string_from_user(const char __user *userstring)
{
	char *path = NULL;
	long rc;
	path = kmalloc(PATH_MAX + 1, 0);
	if (path == NULL)
		return NULL;

	// strncpy(path, userstring, PATH_MAX);
	rc = strncpy_from_user(path, userstring, PATH_MAX);
	// rc = copy_from_user(path, userstring, PATH_MAX);
	
	if (rc <= 0) {
		kfree(path);
		path = NULL;
	}

	return path;
}

static char *get_fullpath(unsigned int fd, char __user *name)
{
	char *path = NULL;
	char *abspath = NULL;
	char *fullpath = NULL;

	do {
		long rc;
		bool is_absname = false;
		fullpath = kmalloc(PATH_MAX, 0);
		if (fullpath == NULL)
			break;
		fullpath[0] = '\0';

		if (name != NULL) {
			char _name[2];
			rc = strncpy_from_user(_name, name, 1);
			if (_name[0] == '/') {
				is_absname = true;
			}
		}

		if (!is_absname) {
			path = kmalloc(PATH_MAX, 0);
			if (path == NULL)
				break;

			if (fd == AT_FDCWD) {
				struct path ph;
				struct fs_struct *fs = current->fs;
				ph.dentry = fs->pwd.dentry;
				ph.mnt = fs->pwd.mnt;
				abspath = d_path(&ph, path, PATH_MAX);
				if (IS_ERR(abspath)) {
					__log_error("d_path ret = %p\n", abspath);
					abspath = NULL;
					break;
				}
			} else {
				abspath = get_file_path(fd, path);
			}

			if (abspath == NULL) {
				break;
			}

			strncpy(fullpath, abspath, PATH_MAX);
		}

		if (name != NULL) {
			int len = strlen(fullpath);
			if (!is_absname) {
				strncpy(&fullpath[len], "/", PATH_MAX - len - 1);
				len++;
			}
			rc = strncpy_from_user(&fullpath[len], name, PATH_MAX -  len - 1);
		}

	} while (0);

	if (path != NULL) {
		kfree(path);
	}

	if (abspath == NULL) {
		if (fullpath != NULL) {
			kfree(fullpath);
			fullpath = NULL;
		}
	}

	return fullpath;
}

asmlinkage long
fake_write(unsigned int fd, const char __user *buf, size_t count)
{
	long rc = 0;
	struct inode *inode = NULL;

	inode = get_inode(fd);
	if (inode != NULL) {
		struct ot_entry *ote;

		ote = ot_add_obj(&ot_header, (void *)inode, NULL, sizeof(struct ot_entry), NULL);
		if (ote != NULL) {
			ot_release_obj(&ot_header, ote);
		}
	}

	rc = real_write(fd, buf, count);

	return rc;
}

asmlinkage long
fake_close(unsigned int fd)
{
	long rc = 0;
	char *path = NULL;
	struct inode *inode = NULL;

	if ((path = get_fullpath(fd, NULL)) != NULL) {
		inode = get_inode(fd);
		if (inode != NULL) {
			if (ot_del_obj(&ot_header, (void *)inode) > 0) {
				get_verdict(ACTION_written, path, NULL);
			}
		}

		kfree(path);
	}

	rc = real_close(fd);

	return rc;
}

asmlinkage long
fake_ioctl(unsigned int fd, unsigned int cmd,
				unsigned long arg)
{
	long rc = 0;
	char *path = NULL;

	rc = real_ioctl(fd, cmd, arg);

	#define BTRFS_IOC_CLONE        _IOW(0x94, 9, int)
	if (cmd == BTRFS_IOC_CLONE
		&& (path = get_fullpath(fd, NULL)) != NULL) {
			get_verdict(ACTION_written, path, NULL);
			kfree(path);
	}

	return rc;
}

asmlinkage long
fake_execve(const char __user *name,  const char __user *const __user *argv,
		const char __user *const __user *envp, struct pt_regs *regs)
{
	int err;
	enum user_verdict uv = UV_pass;
	char first_ch = 0;
	char *abspath = NULL;
	size_t exec_line_size = 0;
	char * exec_str = NULL;
	char *p_argv = NULL;
	char **_argv = (char **)argv;

	while (true) {
		int err;
		err = get_user(p_argv, _argv);
		_argv++;
		if (p_argv == NULL || err != 0)
			break;
		exec_line_size += (__strlen_from_user(p_argv) + 1);
	}

	if (exec_line_size != 0) {
		exec_str = kmalloc(exec_line_size + 1, 0);
		if (NULL != exec_str) {
			size_t len = 0;
			size_t left_len = exec_line_size;
			exec_str[0] = '\0';

			_argv = (char **)argv;
			while (true) {
				int rc;
				rc = get_user(p_argv, _argv);
				_argv++;
				if (p_argv == NULL || rc != 0)
					break;

				if (left_len != exec_line_size) {
					strncat(&exec_str[len], " ", left_len);
					len++;
					left_len--;
				}
				rc = strncpy_from_user(&exec_str[len], p_argv, left_len);
				len = strlen(exec_str);
				left_len = exec_line_size - len;
			}
		}
	}

	err = get_user(first_ch, name);
	if (first_ch != 0 && err == 0) {
		if (first_ch != '/') {
			abspath = get_fullpath(AT_FDCWD, (char *)name);
		}
	}

	if (abspath == NULL) {
		abspath = string_from_user(name);
	}

	__log_info("abs_path: %s, exec_str: %s\n", abspath, exec_str);
	uv = get_verdict(ACTION_execve, abspath, exec_str);
	if (abspath != NULL) {
		kfree(abspath);
	}
	if (exec_str != NULL) {
		kfree(exec_str);
	}

	if (uv != UV_pass) {
		return -1;
	}

	return real_execve(name, argv, envp, regs);
}

asmlinkage long
fake_rename(const char __user *oldname, const char __user *newname)
{
	int err = 0;
	char first_ch = 0;
	enum user_verdict uv = UV_pass;
	char *abs_oldpath = NULL;
	char *abs_newpath = NULL;

	err = get_user(first_ch, oldname);
	if (first_ch != 0 && err == 0) {
		if (first_ch != '/') {
			abs_oldpath = get_fullpath(AT_FDCWD, (char *)oldname);
		}
	}

	if (abs_oldpath == NULL) {
		abs_oldpath = string_from_user(oldname);
	}

	err = get_user(first_ch, newname);
	if (first_ch != 0 && err == 0) {
		if (first_ch != '/') {
			abs_newpath = get_fullpath(AT_FDCWD, (char *)newname);
		}
	}

	if (abs_newpath == NULL) {
		abs_newpath = string_from_user(newname);
	}
	__log_info("rename: %s -> %s\n", abs_oldpath, abs_newpath);

	uv = get_verdict(ACTION_rename, abs_oldpath, abs_newpath);

	if (abs_oldpath != NULL) {
		kfree(abs_oldpath);
	}

	if (abs_newpath != NULL) {
		kfree(abs_newpath);
	}

	if (uv != UV_pass) {
		return -1;
	}

	return real_rename(oldname, newname);
}

asmlinkage long
fake_renameat(int olddfd, const char __user * oldname,
		int newdfd, const char __user * newname)
{
	enum user_verdict uv = UV_pass;
	char *oldpath = NULL;
	char *newpath = NULL;
	__log_info("renameat: [%d]%s -> [%d]%s\n", olddfd, oldname, newdfd, newname);

	oldpath = get_fullpath(olddfd, (char *)oldname);
	newpath = get_fullpath(newdfd, (char *)newname);

	if (oldpath == NULL)
		oldpath = string_from_user(oldname);
	if (newpath == NULL)
		newpath = string_from_user(newname);

	__log_info("renameat: %s -> %s\n", oldpath, newpath);

	uv = get_verdict(ACTION_rename, oldpath, newpath);

	if (oldpath != NULL) {
		kfree(oldpath);
	}

	if (newpath != NULL) {
		kfree(newpath);
	}

	if (uv != UV_pass) {
		return -1;
	}

	return real_renameat(olddfd, oldname, newdfd, newname);
}

asmlinkage long
fake_renameat2(int olddfd, const char __user * oldname,
		int newdfd, const char __user * newname, unsigned int flags)
{
	enum user_verdict uv = UV_pass;
	char *oldpath = NULL;
	char *newpath = NULL;
	__log_info("renameat2: [%d]%s -> [%d]%s\n", olddfd, oldname, newdfd, newname);

	oldpath = get_fullpath(olddfd, (char *)oldname);
	newpath = get_fullpath(newdfd, (char *)newname);

	if (oldpath == NULL)
		oldpath = string_from_user(oldname);
	if (newpath == NULL)
		newpath = string_from_user(newname);

	__log_info("renameat2: %s -> %s\n", oldpath, newpath);

	uv = get_verdict(ACTION_rename, oldpath, newpath);

	if (oldpath != NULL) {
		kfree(oldpath);
	}

	if (newpath != NULL) {
		kfree(newpath);
	}

	if (uv != UV_pass) {
		return -1;
	}

	return real_renameat2(olddfd, oldname, newdfd, newname, flags);
}

MODULE_LICENSE("GPL v2");
module_init(init_panzer_module);
module_exit(exit_panzer_module);
