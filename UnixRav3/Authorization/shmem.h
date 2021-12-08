#ifndef __SHAREDMEM_H__
#define __SHAREDMEM_H__

#ifdef WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif // _WINDOWS_
typedef HANDLE HSHM;
#define HSHM_INVALID		((HSHM)NULL)
#else // WIN32
#include <stdlib.h>
#include "cptdef.h" 
typedef int HSHM;
#define HSHM_INVALID		((HSHM)-1)
#endif // WIN32

#include "mutex.h"

typedef DWORD RS_DWORD;
typedef DDWORD RS_DDWORD;

// NOTE: for better performance, do these:
// 1. move the following three macros and CMappingHeader and CBlockHeader to cpp file
// 2. translate all private methods into macro equivalent and move them to cpp file too 

#define MEM_FLAG_ALLOCATED	0x00000001
#define MEM_FLAG_READING	0x00000002
#define MEM_FLAG_WRITING	0x00000004

#define SPEED_UP_ON

#define PRIVATE_DATA

struct CMappingHeader
{
	RS_DWORD dwSize;							// structrut size
	LONG lRefCount;
	RS_DWORD dwTotal;
	RS_DWORD dwUsed;							// DO NOT use it
	void* pBaseline;							// the very first mapping address base
#ifdef SPEED_UP_ON
	RS_DWORD dwBiggestBlockOffset;
#endif // SPEED_UP_ON

#ifdef PRIVATE_DATA
	RS_DDWORD dwPrivateData;
#endif // PRIVATE_DATA
};

struct CBlockHeader
{
	RS_DWORD dwState;							// block state, free or used, reading or writing
	RS_DWORD dwSize;							// without header itself
};

class CSharedMem
{
	static char m_szSHMName[64];
	static bool m_bInitialized;

#ifndef WIN32
	static Mutex m_mtxRefCnt;
#endif // !WIN32
	static Mutex m_mtxAllocFree;

public:
	static bool IsInitialized();
	static bool Initialize(RS_DWORD dwSize, char *pszProductName, bool &bIsCreate);
	static void Finalize();

	static HSHM m_hSharedMemory;
	static void* m_pFileMapping;

	static void* alloc(size_t size);
	static void free(void* mem);
	static bool read(const void* source, void* dest, size_t size);
	static bool write(const void* source, void* dest, size_t size);

	static int GetMappingGap()
	{
		return (int)((PBYTE)m_pFileMapping - (PBYTE)((CMappingHeader*)m_pFileMapping)->pBaseline);
	}

#ifdef PRIVATE_DATA
	static RS_DDWORD GetPrivateData()
	{
		return ((CMappingHeader*)m_pFileMapping)->dwPrivateData;
	}

	static void SetPrivateData(RS_DDWORD dwData)
	{
		((CMappingHeader*)m_pFileMapping)->dwPrivateData = dwData;
	}
#endif // PRIVATE_DATA

protected:
	static
	CBlockHeader* GetFirstBlock()
	{
		return ((CBlockHeader*)(((PBYTE)m_pFileMapping) + sizeof(CMappingHeader)));
	}

	static
	CBlockHeader* GetNextBlock(CBlockHeader* p)
	{
		return ((CBlockHeader*)(((PBYTE)p) + sizeof(CBlockHeader) + p->dwSize));
	}

	static
	bool IsOverRun(CBlockHeader* p)
	{
		return (((PBYTE)p) >= ((PBYTE)(((PBYTE)m_pFileMapping) + ((CMappingHeader*)m_pFileMapping)->dwTotal)));
	}

	static
	bool IsAllocated(CBlockHeader* p)
	{
		return (p->dwState & MEM_FLAG_ALLOCATED);
	}

	static
	void SetAllocated(CBlockHeader* p, RS_DWORD s)
	{
		p->dwState |= MEM_FLAG_ALLOCATED;

		DWORD dwSize = p->dwSize - s;
		if(dwSize > sizeof(CBlockHeader))
		{
			p->dwSize = s;
			CBlockHeader* pBlock = (CBlockHeader*)(((PBYTE)p) + sizeof(CBlockHeader) + s);
			pBlock->dwState = 0;
			pBlock->dwSize = dwSize - sizeof(CBlockHeader);
#ifdef SPEED_UP_ON
			CBlockHeader* pBiggestBlock = GetBiggestBlock();
			if(pBiggestBlock == p || pBiggestBlock->dwSize < pBlock->dwSize)
				SetBiggestBlock(pBlock);
#endif // SPEED_UP_ON
		}
	}

	static
	void SetFree(CBlockHeader* p)
	{
		(p->dwState &= (~MEM_FLAG_ALLOCATED));
	}

	static
	void* GetBlockMemory(CBlockHeader* p)
	{
		return ((void*)(((PBYTE)p) + sizeof(CBlockHeader)));
	}
#ifdef SPEED_UP_ON
	static
	CBlockHeader* GetBiggestBlock()
	{
		CMappingHeader* pMappingHeader = (CMappingHeader*)m_pFileMapping;
		return (CBlockHeader*)((PBYTE)m_pFileMapping + (pMappingHeader->dwBiggestBlockOffset));
	}

	static
	void SetBiggestBlock(CBlockHeader* pBlock)
	{
		CMappingHeader* pMappingHeader = (CMappingHeader*)m_pFileMapping;
		pMappingHeader->dwBiggestBlockOffset = (PBYTE)pBlock - (PBYTE)m_pFileMapping;
	}

	static
	void TrySetBiggestBlock(CBlockHeader* pBlock)
	{
		CBlockHeader* pBiggestBlock = GetBiggestBlock();
		if(pBiggestBlock != pBlock)
		{
			if(pBlock->dwSize > pBiggestBlock->dwSize)
				SetBiggestBlock(pBlock);
		}
	}

	static
	void MergeFreeBlocks(CBlockHeader* pBlock = NULL, BOOL bToEnd = TRUE)
	{
		// we do not need care metux here, because this function is called by free()
		if(!pBlock)
			pBlock = GetFirstBlock();

		for(; !IsOverRun(pBlock); )
		{
			if(IsAllocated(pBlock))
			{
				pBlock = GetNextBlock(pBlock);
				continue;
			}

			CBlockHeader* pNext = GetNextBlock(pBlock);
			for(;;)
			{
				if(IsOverRun(pNext))
					return;

				if(IsAllocated(pNext))
				{
					if(!bToEnd)
						return;

					pBlock = GetNextBlock(pNext);
					break;
				}
				else
				{
					CBlockHeader* pFree = pNext;
					pNext = GetNextBlock(pNext);

					pBlock->dwSize += sizeof(CBlockHeader) + pFree->dwSize;
					// pFree->dwState = 0;
					pFree->dwSize = 0;

					// update the biggest block offset
					TrySetBiggestBlock(pBlock);
				}
			}
		}
	}
#else // SPEED_UP_ON
	static
	void MergeFreeBlocks()
	{
		// we do not need care metux here, because this function is called by free()
		CBlockHeader* pBlock = GetFirstBlock();

		for(; !IsOverRun(pBlock); )
		{
			if(IsAllocated(pBlock))
			{
				pBlock = GetNextBlock(pBlock);
				continue;
			}

			CBlockHeader* pNext = GetNextBlock(pBlock);
			for(;;)
			{
				if(IsOverRun(pNext))
				{
					pBlock = pNext; // for breaking the outer for-loop
					break;
				}

				if(IsAllocated(pNext))
				{
					pBlock = GetNextBlock(pNext);
					break;
				}
				else
				{
					CBlockHeader* pFree = pNext;
					pNext = GetNextBlock(pNext);

					pBlock->dwSize += sizeof(CBlockHeader) + pFree->dwSize;
					// pFree->dwState = 0;
					pFree->dwSize = 0;
				}
			}
		}
	}
#endif // SPEED_UP_ON
};

#endif // __SHAREDMEM_H__
