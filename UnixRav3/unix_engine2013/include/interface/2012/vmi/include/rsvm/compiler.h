#if !defined(_MSC_VER) && !defined(__GNUC__)
# error "Compiler not supported!"
#endif

#ifndef _HYPER_VM_COMPILER_DEP_H_
#define _HYPER_VM_COMPILER_DEP_H_

#ifdef __x86_64__
# define __hvm_conv
# define __asm_conv
#else
# if defined(_MSC_VER)
#  ifdef __KERNEL__
#   define __hvm_conv __stdcall
#  else
#   define __hvm_conv __cdecl
#  endif
#  define __asm_conv __cdecl
# else
#  ifdef __KERNEL__
#   define __hvm_conv __attribute__((stdcall))
#  else
#   define __hvm_conv __attribute__((cdecl))
#  endif
#  define __asm_conv __attribute__((regparm(0)))
# endif
#endif

#ifdef _MSC_VER
# define HvmConv(_RetType) _RetType __hvm_conv
# define AsmConv(_RetType) _RetType __asm_conv
#else
# define HvmConv(_RetType) __hvm_conv _RetType
# define AsmConv(_RetType) __asm_conv _RetType
#endif

/* alignment specifications */
#ifdef _MSC_VER
# define __align(g, type) __declspec(align(g)) type
#else
# define __align(g, type) __attribute__((aligned(g))) type
#endif

#ifdef _MSC_VER
# define __hvm_inline __forceinline
#else
# define __hvm_inline inline
#endif

#define __rsvm_conv	__hvm_conv
#define RsvmConv	HvmConv
#define __rsvm_inline	__hvm_inline

/* variable definations */
#ifdef __x86_64__
# define var(type) __align(8, type)
#else
# define var(type) __align(4, type)
#endif

/* printf-style type field definations */
#ifdef _MSC_VER
# define FMT_LL		"%I64"
#else
# define FMT_LL		"%ll"
#endif

/* util functions */
#if defined(__linux__) && !defined(__KERNEL__)

/* string cmpare */
# define stricmp strcasecmp
# define strnicmp strncasecmp

/* string lower/upper */
__rsvm_inline char *strlwr(char *string)
{
	char *p;
	for (p = string; *p; ++p) {
		if ('A' <= *p && *p <= 'Z')
			*p += 'a' - 'A';
	}
	return string;
}
__rsvm_inline char *strupr(char *string)
{
	char *p;
	for (p = string; *p; ++p) {
		if ('a' <= *p && *p <= 'z')
			*p -= 'a' - 'A';
	}
	return string;
}

#endif

#endif /* _HYPER_VM_COMPILER_DEP_H_ */
