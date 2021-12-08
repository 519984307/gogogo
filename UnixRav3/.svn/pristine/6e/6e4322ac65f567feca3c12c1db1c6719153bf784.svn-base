#ifndef __RSMEMORY_H__
#define __RSMEMORY_H__

#include "RSIFACE.H"
#include "RSIDs.h"

class IRSMemory : public IRSObject
{
public:
	enum { RSID = RSID_RSMemory, RSIID = RSIID_IRSMemory };
	virtual void* alloc(size_t n) = 0;
	virtual void free(void *p) = 0;
};

// NOTE: We MUST use the long version macro because of the silly gcc on SunOS.
/*
#define CLASSADDMEMMGR(TTHISCLASS)							\
private:													\
	IRSMemory* m_pMMgr;										\
public:														\
	inline void* operator new(size_t siz, IRSMemory* p)		\
	{														\
		TTHISCLASS *pI = (TTHISCLASS*)p->alloc(siz);		\
		pI->m_pMMgr = p;									\
		return pI;											\
	}														\
	inline void operator delete(void* p, IRSMemory* p2)		\
	{														\
		p2->free(p);										\
	}														\
	inline void operator delete(void* p)					\
	{														\
		if(p)												\
			((TTHISCLASS*)p)->m_pMMgr->free(p);				\
	}														\
private:
// */

#define CLASSADDMEMMGR(TTHISCLASS) private: IRSMemory* m_pMMgr; public:	inline void* operator new(size_t siz, IRSMemory* p) { TTHISCLASS *pI = (TTHISCLASS*)p->alloc(siz); pI->m_pMMgr = p; return pI; } inline void operator delete(void* p, IRSMemory* p2) { p2->free(p); } inline void operator delete(void* p) { if(p) ((TTHISCLASS*)p)->m_pMMgr->free(p); } private:

#endif // __RSMEMORY_H__
