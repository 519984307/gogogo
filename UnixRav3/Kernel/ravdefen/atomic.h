
#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#ifdef _MSC_VER
#ifdef __KERNEL__
#include <ntifs.h>
#else
#include <windows.h>
#endif

#define atomic_set(v,i)	(InterlockedExchange(&(v), i))
#define atomic_inc(v) (InterlockedIncrement(&(v)))
#define atomic_dec_and_test(v) (InterlockedDecrement(&(v)) == 0)

#else

/* Linux */
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm-generic/atomic-long.h>

#endif

#endif /* _ATOMIC_H_ */
