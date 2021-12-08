
#ifndef _ENGUTILS_H_
#define _ENGUTILS_H_

#ifdef __cplusplus
#include <time.h>

//#include "re_string.h"
/*
*/
struct RS_CSTR
{
	const char* string;
	size_t size;
	RS_CSTR(const char* str) : string(str), size(strlen(str)) { }
};

struct RS_CWSTR{
	const wchar_t *string;
	size_t size;
	RS_CWSTR(const wchar_t *wstr) :string(wstr), size(wcslen(wstr)) { }
};

template <class _T>
class RS_CARR
{
protected:
	_T *_buffer;
public:
	RS_CARR(size_t size) : _buffer(size ? new _T[size] : NULL) { }
	~RS_CARR() { if (_buffer) { delete [] _buffer; } }
	operator _T * () {  return _buffer; }
};

typedef RS_CARR<BYTE> RS_CBUF;

#endif // __cplusplus


#ifdef _MSC_VER
#define RSSTR2(x) #x
#define RSSTR1(x) RSSTR2(x)
#define RSMSG(desc) message(__FILE__"("RSSTR1(__LINE__)") : " #desc)
#define TODO RSMSG(TODO)
#else
#define TOTO
#endif //_MSC_VER


#if defined(PLATFORM_TYPE_LINUX_INTEL)
#define RS_RDTSC(tsc)  __asm__ __volatile__ ("rdtsc" : "=A" (tsc))

#elif defined(PLATFORM_TYPE_SUN_INTEL)
#define RS_RDTSC(tsc)  __asm__ __volatile__ ("rdtsc" : "=A" (tsc))

#elif defined(PLATFORM_TYPE_SUN_SPARC)
#include <sys/time.h>
#define RS_RDTSC(tsc) tsc.QuadPart = gethrtime();

#elif defined (PLATFORM_TYPE_HP_RISC)
#include <sys/time.h>
#define RS_RDTSC(tsc) { \
	struct timespec t; clock_gettime(CLOCK_REALTIME, &t); \
	tsc.u.HighPart = t.tv_sec; tsc.u.LowPart = t.tv_nsec; \
}

#elif defined(PLATFORM_TYPE_IBM_RISC)
#include <sys/time.h>
#define RS_RDTSC(tsc) { \
	timebasestruct_t t; read_real_time(&t, TIMEBASE_SZ); \
	tsc.u.HighPart = t.tb_high; tsc.u.LowPart = t.tb_low; \
}

#else // defined(PLATFORM_TYPE_WINDOWS)

#if TARGET_BITS == ARCH_32BIT && CPP_COMPILER == CC_MSVC
#define RS_RDTSC(tsc)	__asm rdtsc  __asm mov tsc.LowPart, eax  __asm mov tsc.HighPart, edx
#else
#define RS_RDTSC(tsc)	(tsc.QuadPart = clock())
#endif

#endif


#ifdef __cplusplus
inline void I2A(ULONGLONG num, PCHAR string)
{
	int n = sizeof(ULONGLONG) * 2;
	CHAR ch;
	for(string[n--] = 0; num; num >>= 4)
	{
		ch = (CHAR)(num & 0xF) + '0';
		if (ch > '9') { ch += ('A' - ('9' + 1)); }
		string[n--] = ch;
	}
	for ( ; n >= 0; n--){ string[n] = '0'; }
}

inline void I2W(ULONGLONG num, PWCHAR string)
{
	int n = sizeof(ULONGLONG) * 2;
	WCHAR ch;
	for(string[n--] = 0; num; num >>= 4)
	{
		ch = (WCHAR)(num & 0xF) + L'0';
		if (ch > L'9') { ch += (L'A' - (L'9' + 1)); }
		string[n--] = ch;
	}
	for ( ; n >= 0; n--){ string[n] = L'0'; }
}

#endif // __cplusplus


#endif //_ENGUTILS_H_

