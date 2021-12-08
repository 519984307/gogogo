#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

extern int modify_sync_onoff;

static struct proc_dir_entry *root;

static int modify_sync_show(struct seq_file *s, void *p)
{
	seq_printf(s, "%d\n", modify_sync_onoff);
	return 0;
}

static int file_open(struct inode *inode, struct file *file)
{
	return single_open(file, modify_sync_show, NULL);
}

ssize_t file_read(struct file *file, char __user *buffer, size_t count, loff_t *off)
{
	return 0;
}

ssize_t file_write(struct file *file, const char __user *buffer, size_t count, loff_t *off)
{
	char val;

	if (copy_from_user(&val, buffer, 1))
		return -EFAULT;

	modify_sync_onoff = (val == '0') ? 0 : 1;
	return count;
}


#if 0
static ssize_t procfile_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	return 0;
}

static ssize_t procfile_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	return count;
}
#endif

void procfile_cleanup_proc(void)
{
	remove_proc_entry("modify", root);
	remove_proc_entry("ravdefen", NULL);
}

struct file_operations proc_fops = {
	.open = file_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = file_write,
	.release = single_release,
	.owner=THIS_MODULE,
};

void procfile_init_proc(void)
{
	struct proc_dir_entry *entry;
	root = proc_mkdir("ravdefen", NULL);
#if 0
	entry = create_proc_entry("modify", 0664, root);
	entry->read_proc = procfile_read_proc;
	entry->write_proc = procfile_write_proc;
#endif
	entry = proc_create("modify", 0664, root, &proc_fops);
}
