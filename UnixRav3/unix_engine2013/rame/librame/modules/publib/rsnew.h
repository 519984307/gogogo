#ifndef _RSNEW_H_
#define _RSNEW_H_

#include "re_string.h"

#ifdef PLATFORM_TYPE_NATIVE
inline __declspec(naked)
DWORD WINAPI GetCurrentThreadId()
{
	__asm
	{
		MOV EAX,DWORD PTR FS:[0x18]
		MOV EAX,DWORD PTR DS:[EAX+0x24]
		RET
	}
}

inline 
LPVOID WINAPI VirtualAlloc(
    IN LPVOID lpAddress,
    IN SIZE_T dwSize,
    IN DWORD flAllocationType,
    IN DWORD flProtect
    )
{
	if NT_SUCCESS(ZwAllocateVirtualMemory((HANDLE)-1, &lpAddress, 0, &dwSize, flAllocationType, flProtect))
	{
		return lpAddress;
	}
	return 0;
}

inline
BOOL WINAPI VirtualFree(
    IN LPVOID lpAddress,
    IN SIZE_T dwSize,
    IN DWORD dwFreeType
    )
{
	BOOL RtlFlushSecureMemoryCache(IN LPVOID lpAddress, IN SIZE_T dwSize);
	DWORD ret;

	if (dwFreeType & MEM_RELEASE)
		if (0!=dwSize) return FALSE;

	if NT_SUCCESS(ret=ZwFreeVirtualMemory((HANDLE)-1, &lpAddress, &dwSize, dwFreeType))
		return TRUE;

//	if (STATUS_INVALID_PAGE_PROTECTION != ret)
//		return FALSE;

//	if (RtlFlushSecureMemoryCache(lpAddress, dwSize))
//		return NT_SUCCESS(ZwFreeVirtualMemory((HANDLE)-1, &lpAddress, &dwSize, dwFreeType));

	return FALSE;
}
#endif


#ifndef LOG_MEMORY_LEAK


#define RSNEW	new

#ifdef PLATFORM_TYPE_NATIVE
#define RSMALLOC(size)		bs_malloc(size)
#define RSREALLOC(p, size)	bs_realloc(p, size)
#define RSCALLOC(num, size)	bs_calloc(num, size)
#define RSFREE(p)			bs_free(p)
#else
#define RSMALLOC(size)		malloc(size)
#define RSREALLOC(p, size)	realloc(p, size)
#define RSCALLOC(num, size)	calloc(num, size)
#define RSFREE(p)			free(p)
#endif


#ifdef PLATFORM_TYPE_WINDOWS
	#define __CDECL __cdecl
#else
	#define __CDECL
#endif


#define RSVIRTUALALLOC(lpAddress, dwSize, flAllocationType, flProtect) \
		::VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect)
#define RSVIRTUALFREE(lpAddress, dwSize, dwFreeType) \
		::VirtualFree(lpAddress, dwSize, dwFreeType)

	#if !defined(NO_OVERLOAD_NEW) && !defined(PLATFORM_TYPE_NATIVE) // not throw exception
		inline
		void* __CDECL operator new (size_t size)
		{
			return RSMALLOC(size);
		}

		inline
		void __CDECL operator delete (void *p)
		{
			RSFREE(p);
		}

		inline
		void* __CDECL operator new [] (size_t size)
		{
			return operator new (size);
		}

		inline
		void __CDECL operator delete [] (void *p)
		{
			operator delete (p);
		}
	#endif


#else // #ifndef LOG_MEMORY_LEAK


#include "stdio.h" // for sprintf
#include "re_stdlib.h" // for rs_malloc, ...
#include "utility.h"


// ... ================================================================
#ifdef RSNEW_CATCH_LEAK_AND_FLOW
//注意！
//必须是内存对齐。 这里取16字节。 要不然某些api会失败。比如CreateMutex的SECURITY_DESCRIPTOR就好象有这个要求
//DECLSPEC_SELECTANY	extern const char REMEMORY_MAGIC[] = "_REMEMORY_MAGIC";
#define REMEMORY_MAGICSIZE 32
bool __flow_handle(void *p);
#endif // #ifdef RSNEW_CATCH_LEAK_AND_FLOW


namespace rsnew { ;


class CMemTrace
{
public:
	CMemTrace(const char *logfilename = "")
	{
		m_hDbgInfo = 0;
		m_pfnREGetDbgInfo = 0;
		m_pfnREGetDbgMemTraceMgr = 0;
		m_logfilename = logfilename;

		HMODULE h;
		__asm
		{
			call __next
		__next:
			pop eax
			and eax, 0xffff0000
		__loop:
			cmp word ptr[eax], 'ZM'
			jz __done
			sub eax, 0x10000
			jmp __loop
		__done:
			mov h, eax
		}
		char filename[MAX_PATH*4];
		DWORD len = ::GetModuleFileNameA(h, filename, sizeof(filename));
		while(len-- && '\\' != filename[len]);
		strcpy(&filename[++len], "redbginfo.dll");
		RASSERTPV(m_hDbgInfo = ::LoadLibraryA(filename));
		m_pfnREGetDbgInfo = (PFNREGetDbgInfo)::GetProcAddress(m_hDbgInfo, "REGetDbgInfo");
		m_pfnREGetDbgMemTraceMgr = (PFNREGetDbgMemTraceMgr)::GetProcAddress(m_hDbgInfo, "REGetDbgMemTraceMgr");
	}
	~CMemTrace()
	{
	#ifdef RSNEW_CATCH_LEAK_AND_FLOW
		if(IREDbgMemTrace *ptr = GetMemTrace()) for(void *p = 0; SUCCEEDED(ptr->NextItem(p, &p)); __flow_handle(p));
	#endif
		if(m_hDbgInfo) { ::FreeLibrary(m_hDbgInfo); m_hDbgInfo = 0; m_pfnREGetDbgInfo = 0; m_pfnREGetDbgMemTraceMgr = 0; }
	}

public:
	static IREProperty* GetDbgInfo()
	{
		if(!m_pfnREGetDbgInfo) { return 0; }
		return m_pfnREGetDbgInfo();
	}
	static IREDbgMemTrace* GetMemTrace()
	{
		if (!m_pfnREGetDbgMemTraceMgr) { return 0; }
		IREDbgMemTraceManager *pmgr = m_pfnREGetDbgMemTraceMgr();
		if (!pmgr) { return 0; }
		return pmgr->GetMemTrace(m_logfilename);
	}

private:
	static HMODULE m_hDbgInfo;
	static PFNREGetDbgInfo m_pfnREGetDbgInfo;
	static PFNREGetDbgMemTraceMgr m_pfnREGetDbgMemTraceMgr;
	static const char *m_logfilename;
};

DECLSPEC_SELECTANY	HMODULE	CMemTrace::m_hDbgInfo;
DECLSPEC_SELECTANY	PFNREGetDbgInfo	CMemTrace::m_pfnREGetDbgInfo;
DECLSPEC_SELECTANY	PFNREGetDbgMemTraceMgr	CMemTrace::m_pfnREGetDbgMemTraceMgr;
DECLSPEC_SELECTANY	const char*	CMemTrace::m_logfilename;
typedef	CMemTrace	CLogMemoryLeak;


inline int LogNew(void *p, size_t size, char *magic = NULL)
{
	if(p)
	{
		memset(p, 0xcc, size);
		// 如果p已经被log了， 现在没有被释放， 堆居然又产生了同样的p， 则要记录下， 可能是通过其他渠道释放的。 或堆已经乱了。
		IREDbgMemTrace *ptr = CMemTrace::GetMemTrace();
		if(ptr)
		{
			if(ptr->IsItemExist(p))
			{
				__asm int 3 // ASSERT0;
			}
			PROPVARIANT var;
			ptr->SetItem(p, IREDbgMemTrace::PID_AllocSize, &(var.vt=VT_UI4, var.ulVal=(ULONG)size, var));
			if (magic)
				ptr->SetItem(p, IREDbgMemTrace::PID_Magic, &(var.vt=VT_LPSTR, var.pcVal=magic, var));
		}
	}
	return 0;
}


inline void* LogNew(void *p, const char *pszFile, size_t nLine)
{
	IREDbgMemTrace *ptr = CMemTrace::GetMemTrace(); 
	if(p && ptr)
	{
		PROPVARIANT var;
		ptr->SetItem(p, IREDbgMemTrace::PID_SourceFileName,
					 &(var.vt=VT_LPSTR, var.pszVal=(pszFile ? pszFile : ""), var));
		ptr->SetItem(p, IREDbgMemTrace::PID_SourceFileLine, &(var.vt=VT_UI4, var.ulVal=(ULONG)nLine, var));
	}
	return p;
}


inline int LogDelete(void *p, bool bMark = true)
{
	IREDbgMemTrace *ptr = CMemTrace::GetMemTrace(); 
	if(p && ptr)
	{
		if(!ptr->IsItemExist(p))
		{
			__asm int 3 // ASSERT0;
		}
		const PROPVARIANT *pvar;
		if(bMark && SUCCEEDED(ptr->GetItem(p, IREDbgMemTrace::PID_AllocSize, &pvar)))
		{
			memset(p, 0xee, pvar->ulVal);
		}
		ptr->DeleteItem(p); 
	}
	return 0;
}


} // namespace rsnew


// ... ================================================================
//////////////////////////////////////////////////////////////////////////
#if defined(RSNEW_CATCH_LEAK) // case: catch memory leak

	inline 
	void* __cdecl __malloc(size_t size)
	{
		if (0 == size || size > 0x40000000)
		{
#			if defined(_DEBUG) && defined(PLATFORM_TYPE_WIN32)
				char buf[1024];
				sprintf(buf, "Malloc Size is %X, maybe a bug??\n", size);
				OutputDebugStringA(buf);
#			endif
			if (0 != size) return 0;
		}

		void *p = rs_malloc(size);
		if (!p) return 0;
		
		rsnew::LogNew(p, size);
		return p;
	}

	inline
	void __cdecl __free(void *p)
	{
		if (!p) return;
		rsnew::LogDelete(p);
		rs_free(p);
	}

//////////////////////////////////////////////////////////////////////////
#elif defined(RSNEW_CATCH_LEAK_AND_FLOW)	// case: catch memory leak & flow

	inline
	void random_general8(char *p, bool reverse)
	{
		char buf[9];
		DWORD _rdtsc;
		__asm rdtsc  __asm mov _rdtsc, eax
		int2hstr(_rdtsc, buf, 9);
		memcpy(p, buf, 8);
		if (reverse)
		{
			for(int i=0; i<4; ++i) { char c=p[7-i]; p[7-i]=p[i]; p[i]=c; }
		}
	}

//the memorys order is (app size), (magic), appsize, (magic)

	inline
	void* __cdecl __malloc(size_t size)
	{
		if (0 == size || size > 0x40000000)
		{
#			if defined(_DEBUG) && defined(PLATFORM_TYPE_WIN32)
				char buf[1024];
				sprintf(buf, "Malloc Size is %X, maybe a bug??\n", size);
				OutputDebugStringA(buf);
#			endif
			if (0 != size) return 0;
		}

		char memory_magic[REMEMORY_MAGICSIZE] = "11111111_REMEMORY_MAGIC_1111111";
		random_general8(memory_magic, true); //reverse 保证memory_magic第一字节和最后字节有变化.
		random_general8(memory_magic+24, false);
		void *p = rs_malloc(size+sizeof(size_t)+2*REMEMORY_MAGICSIZE);
		if(!p) { return 0; }

		*(size_t *)p = size;
		memcpy((char *)p+sizeof(size_t), memory_magic, REMEMORY_MAGICSIZE);
		memcpy((char *)p+sizeof(size_t)+size+REMEMORY_MAGICSIZE, memory_magic, REMEMORY_MAGICSIZE);

		p = (char *)p+sizeof(size_t)+sizeof(memory_magic);
		rsnew::LogNew(p, size, memory_magic);
		return p;
	}

	inline
	void __cdecl __free(void *p)
	{
		if (!p) return;
		if(!__flow_handle(p)) return;
		rsnew::LogDelete(p);
		rs_free((char *)p-REMEMORY_MAGICSIZE-sizeof(size_t));
	}

	inline
	bool __flow_handle(void *p)
	{
		// point must be in MemTrace.
		IREDbgMemTrace *ptr = rsnew::CMemTrace::GetMemTrace(); 
		RASSERT(ptr, true);

		if(p && !ptr->IsItemExist(p))
		{
			__asm int 3 // ASSERT0;
		}

		char memory_magic[REMEMORY_MAGICSIZE] = "11111111_REMEMORY_MAGIC_1111111";

		{	// get magic:
			const PROPVARIANT *var;
			RASSERT(SUCCEEDED(ptr->GetItem(p, IREDbgMemTrace::PID_Magic, &var)), false); // unexpected!!!
			memcpy(memory_magic, var->pcVal, REMEMORY_MAGICSIZE);
		}

		char *c = (char *)p;
		if ( 0 != memcmp(c-REMEMORY_MAGICSIZE, memory_magic, REMEMORY_MAGICSIZE) )
		{	// test underflow:
			PROPVARIANT var;
			var.vt = VT_UI1|VT_VECTOR;
			var.caub.cElems = (ULONG)REMEMORY_MAGICSIZE;
			var.caub.pElems = (BYTE*)(c-REMEMORY_MAGICSIZE);
			ptr->SetItem(p, IREDbgMemTrace::PID_CookieErrorData, &var);
			ptr->SetItem(p, IREDbgMemTrace::PID_CookieErrorCode,
						 &(var.vt=VT_UI4, var.ulVal=IREDbgMemTrace::CookieError_Underflow, var));
		}
		else
		{	// test overflow:
			size_t size = *(size_t *)(c-REMEMORY_MAGICSIZE-sizeof(size_t));
			if ( 0 != memcmp(c+size, memory_magic, REMEMORY_MAGICSIZE) )
			{
				PROPVARIANT var;
				var.vt = VT_UI1|VT_VECTOR;
				var.caub.cElems = (ULONG)REMEMORY_MAGICSIZE;
				var.caub.pElems = (BYTE*)(c+size);
				ptr->SetItem(p, IREDbgMemTrace::PID_CookieErrorData, &var);
				ptr->SetItem(p, IREDbgMemTrace::PID_CookieErrorCode,
							 &(var.vt=VT_UI4, var.ulVal=IREDbgMemTrace::CookieError_Overflow, var));
			}
			else
			{
				return true;
			}
		}

		return false;
	}

//////////////////////////////////////////////////////////////////////////
#elif defined(RSNEW_CATCH_LEAK_AND_UNDERFLOW) // case: catch memory leak & underflow

	inline 
	void* __cdecl __malloc(size_t size)
	{
		if (0 == size || size > 0x40000000)
		{
#			if defined(_DEBUG) && defined(PLATFORM_TYPE_WIN32)
				char buf[1024];
				sprintf(buf, "Malloc Size is %X, maybe a bug??\n", size);
				OutputDebugStringA(buf);
#			endif
			if (0 != size) return 0;
		}


		char *p = (char*)VirtualAlloc(0, size+0x2000, MEM_COMMIT, PAGE_READWRITE);
		if(!p) { return 0; }
		p += 0x1000;
		VirtualFree(p-0x1000, 1, MEM_DECOMMIT);
		VirtualFree(p+size+0xFFF, 1, MEM_DECOMMIT);

		rsnew::LogNew(p, size);
		return p;
	}

	inline
	void __cdecl __free(void *p)
	{
		if (!p) return;
		rsnew::LogDelete(p);
		VirtualFree((char*)p-0x1000, 0, MEM_RELEASE);
	}

//////////////////////////////////////////////////////////////////////////
#else // default: catch memory leak & overflow

	inline 
	void* __cdecl __malloc(size_t size)
	{
		if (0 == size || size > 0x40000000)
		{
#			if defined(_DEBUG) && defined(PLATFORM_TYPE_WIN32)
				char buf[1024];
				sprintf(buf, "Malloc Size is %X, maybe a bug??\n", size);
				OutputDebugStringA(buf);
#			endif
			if (0 != size) return 0;
		}


		char *p = (char*)VirtualAlloc(0, size+0x2000, MEM_COMMIT, PAGE_READWRITE);
		if(!p) { return 0; }
		p += 0x1000 +0x1000 -((size-1)&0xFFF) -1;
		VirtualFree(p-0x1000, 1, MEM_DECOMMIT);
		VirtualFree(p+size, 1, MEM_DECOMMIT);

		rsnew::LogNew(p, size);
		return p;
	}

	inline
	void __cdecl __free(void *p)
	{
		if (!p) return;
		rsnew::LogDelete(p);
		VirtualFree((void*)(((size_t)p&~0xFFF)-0x1000), 0, MEM_RELEASE);
	}

#endif // end case catch memory

inline
void* __cdecl __realloc(void *p_old, size_t size_new)
{
	if(!size_new) { __free(p_old); return 0; }
	if(!p_old) { return __malloc(size_new); }
	size_t size_old;
	{
		IREDbgMemTrace *ptr;
		const PROPVARIANT *pvar;
		if(!(ptr = rsnew::CMemTrace::GetMemTrace()) ||
			FAILED(ptr->GetItem(p_old, IREDbgMemTrace::PID_AllocSize, &pvar)))
		{
			return 0;
		}
		size_old = pvar->ulVal;
	}
	void *p_new = __malloc(size_new);
	if(!p_new) { return 0; }
	memcpy(p_new, p_old, min(size_old, size_new));
	__free(p_old);
	return p_new;
}

inline
void* __cdecl __calloc(size_t num, size_t size)
{
	void *p = __malloc(size*=num);
	if(p) { memset(p, 0, size); }
	return p;
}

// ... ================================================================
inline
void* __cdecl operator new (size_t size)
{
	void *p = __malloc(size);
	return p;
}

inline
void* __cdecl operator new (size_t size, const char *file, size_t line)
{
	void *p = __malloc(size);
	rsnew::LogNew(p, file, line);
	return p;
}

inline
void __cdecl operator delete (void *p)
{
	__free(p);
}
inline
void __cdecl operator delete (void *p, const char*, size_t)
{
	__free(p);
}




inline
void* __cdecl operator new [] (size_t size)
{
	return operator new (size);
}

inline
void* __cdecl operator new [] (size_t size, const char *file, size_t line)
{
	return operator new (size, file, line);
}

inline
void __cdecl operator delete [] (void *p)
{
	operator delete (p);
}
inline
void __cdecl operator delete [] (void *p, const char*, size_t)
{
	operator delete (p);
}

// ... ================================================================
inline
void* __stdcall __VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
	void *p = VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect);
	rsnew::LogNew(p, dwSize);
	return p;
}

inline
BOOL __stdcall __VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
	if (!lpAddress) return FALSE;
	rsnew::LogDelete(lpAddress, false);
	if(!VirtualFree(lpAddress, dwSize, dwFreeType)) { return FALSE; }
	return TRUE;
}


// ... ================================================================
#define RSNEW new(__FILE__, __LINE__)


#define RSMALLOC(size)		(rsnew::LogNew(__malloc(size), __FILE__, __LINE__))
#define RSREALLOC(p, size)	(rsnew::LogNew(__realloc(p, size), __FILE__, __LINE__))
#define RSCALLOC(num, size)	(rsnew::LogNew(__calloc(num, size), __FILE__, __LINE__))
#define RSFREE(p)			__free(p)

#define RSVIRTUALALLOC(lpAddress, dwSize, flAllocationType, flProtect) \
		(rsnew::LogNew(__VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect), __FILE__, __LINE__))
#define RSVIRTUALFREE(lpAddress, dwSize, dwFreeType) \
		__VirtualFree(lpAddress, dwSize, dwFreeType)


#endif  // #ifndef LOG_MEMORY_LEAK


#endif // #ifndef _RSNEW_H_
