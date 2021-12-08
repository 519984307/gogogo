#ifndef _RSVM_TYPES_H_
#define _RSVM_TYPES_H_

#if defined(__target_x86_64__) && !defined(__x86_64__)
# error "I don't think this is a good idea :("
#endif

typedef long lresult_t;

#if defined(_MSC_VER)
	#if !defined(_STDINT)
		typedef __int8					int8_t;
		typedef __int16					int16_t;
		typedef __int32					int32_t;
		typedef __int64					int64_t;
		typedef unsigned __int8			uint8_t;
		typedef unsigned __int16		uint16_t;
		typedef unsigned __int32		uint32_t;
		typedef unsigned __int64		uint64_t;
	#endif
#else
# ifndef __KERNEL__
#  include <stdint.h>
# elif defined(__linux__)
#  include <linux/types.h>
# endif
#endif

#ifdef __target_x86_64__
typedef uint64_t gcptr_t;
# define gcptr_size 8
#else
typedef uint32_t gcptr_t;
# define gcptr_size 4
#endif

#ifdef _MSC_VER
# ifdef __x86_64__
typedef __int64 iptr_t;
typedef unsigned __int64 uiptr_t;
# else
typedef long iptr_t;
typedef unsigned long uiptr_t;
# endif
#else
typedef long iptr_t;
typedef unsigned long uiptr_t;
#endif

/* boolean */
#ifndef COMPATIBILITY_NON_MAINSTREAM_OS
typedef uint32_t boolean_t;
#endif

#if !defined(__linux__) || !defined(__KERNEL__)
# include <stddef.h>
# include <sys/types.h>
#endif

#ifndef __cplusplus
# define true 1
# define false 0
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

/* min and max */
#define __rsvm_min(a, b) (((a) < (b)) ? (a) : (b))
#define __rsvm_max(a, b) (((a) > (b)) ? (a) : (b))

#endif /* _RSVM_TYPES_H_ */
