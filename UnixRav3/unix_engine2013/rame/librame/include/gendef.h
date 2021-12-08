#ifndef _SYSMOM_GENDEF_H_
#define _SYSMON_GENDEF_H_

#if defined(_MSC_VER)
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long ptr32_t;
typedef unsigned long long ptr64_t;
#else
# ifndef __KERNEL__
#  include <stdint.h>
# elif defined(__linux__)
#  include <linux/types.h>
# endif
#endif

/* boolean */
typedef long bool_t;

/* process id */
typedef uint64_t pid_t;

#define mkptr32(ptr) (ptr32_t)(uintptr_t)(ptr)
#define mkptr64(ptr) (ptr64_t)(uintptr_t)(ptr)
#define mku32(val) (uint32_t)(uintptr_t)(val)
#define mku64(val) (uint64_t)(uintptr_t)(val)
#define mkpid(val) (pid_t)(uintptr_t)(val)

#if !defined(__linux__) || !defined(__KERNEL__)
# include <stddef.h>
# include <sys/types.h>
#endif

#ifndef __cplusplus
# define true 1L
# define false 0L
#endif

/* Size Constants */
#define _1kb	0x00000400
#define _4kb	0x00001000
#define _32kb	0x00008000
#define _64kb	0x00010000
#define _128kb	0x00020000
#define _256kb	0x00040000
#define _512kb	0x00080000
#define _1mb	0x00100000
#define _2mb	0x00200000
#define _4mb	0x00400000
#define _1gb	0x40000000
#define _2gb32	0x80000000U
#define _2gb	0x0000000080000000LL
#define _4gb	0x0000000100000000LL
#define _1tb	0x0000010000000000LL
#define _1pb	0x0004000000000000LL
#define _1eb	0x1000000000000000LL
#define _2eb	0x2000000000000000ULL

/* handle unreferenced variables */
#define __unreferenced(x) x

#ifdef _MSC_VER
# define __inline__ __forceinline
#else
# define __inline__ static inline
#endif

#endif /* _SYSMON_GENDEF_H_ */
