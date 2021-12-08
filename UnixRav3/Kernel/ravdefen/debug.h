#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <linux/module.h>

#define __log_info(fmt, ...) \
	printk(KERN_INFO "%s.%s: " fmt, \
			THIS_MODULE->name, __func__, ##__VA_ARGS__)

#define __log_error(fmt, ...) \
	printk(KERN_ERR "%s.%s: " fmt, \
			THIS_MODULE->name, __func__, ##__VA_ARGS__)

#endif /* __DEBUG_H__ */
