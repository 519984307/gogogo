#include "shmem.h"
#include "hash.h"

#ifndef WIN32
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

#ifndef SHM_MODE
#define SHM_MODE ((SHM_R | SHM_W) | ((SHM_R | SHM_W) >> 3) | ((SHM_R | SHM_W) >> 6))
#endif // SHM_MODE
#else // !WIN32
#include "defsa.h"
#endif // !WIN32

#define MAX_FILE_SIZE		(1024 * 256)			// 256KB

#define RS_SM_REGISTRY		"RSREGISTRY"			// SM - Shared Memory
#define RS_SM_GLOBALREG		"Global\\RSREGISTRY"	// SM - Shared Memory

#define MUTEX_ALLOC_FREE	"RSFMAF"
#define MUTEX_READ_WRITE	"RSFMRW"

bool CSharedMem::m_bInitialized = false;
HSHM CSharedMem::m_hSharedMemory = HSHM_INVALID;
void* CSharedMem::m_pFileMapping = NULL;

char CSharedMem::m_szSHMName[64] = {0};

#ifndef WIN32
#define MUTEX_REFCNT		"RSFMRC"
Mutex CSharedMem::m_mtxRefCnt = NULL;

#if defined( _NOUSESHM_) && defined(UNIXPLATFORM)
#define MutexCreate(d,b) ((void *)0xffffffff)
#define MutexLock(a,b)	(true)
#define MutexUnlock(a)
#define MutexDestroy(a)

#endif // _NOUSESHM_
#else // !WIN32

BOOL IsTerminalServer()
{
	struct NC_OSVERSIONINFOEX
	{
		DWORD  dwOSVersionInfoSize;
		DWORD  dwMajorVersion;
		DWORD  dwMinorVersion;
		DWORD  dwBuildNumber;
		DWORD  dwPlatformId;
		CHAR   szCSDVersion[ 128 ];  
		WORD   wServicePackMajor;
		WORD   wServicePackMinor;
		WORD   wSuiteMask;
		BYTE   wProductType;
		BYTE   wReserved;
	};
	
#ifndef VER_SUITE_TERMINAL
#define VER_SUITE_TERMINAL                  0x00000010
#endif
	
	NC_OSVERSIONINFOEX		osvi;
	BOOL bOsVersionInfoEx = FALSE;
	
	// 首先使用OSVERSIONINFOEX结构来尝试使用GetVersionEx
	ZeroMemory( &osvi, sizeof(NC_OSVERSIONINFOEX) );
	osvi.dwOSVersionInfoSize = sizeof(NC_OSVERSIONINFOEX);
	if( !( bOsVersionInfoEx = GetVersionEx ( (OSVERSIONINFO *)&osvi ) ) )
	{
		// 使用OSVERSIONINFOEX结构失败，则使用OSVERSIONINFO结构
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if ( !GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	
	// 确定操作系统类型 - NT系列
	if( osvi.dwPlatformId != VER_PLATFORM_WIN32_NT )
		return FALSE;
	
	// 检查产品版本
	if( bOsVersionInfoEx ) // after NT SP6
	{
		if( osvi.wSuiteMask & VER_SUITE_TERMINAL )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		HKEY	hKey;
		TCHAR	szProductType[80] = { 0 };
		DWORD	dwBufLen = 80;
		
		RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey );
		RegQueryValueEx( hKey, "ProductSuite", NULL, NULL, (LPBYTE) szProductType, &dwBufLen );
		RegCloseKey( hKey );
		
		// 变为小写
		strlwr( szProductType );
		
		// Compare
		if( strstr( szProductType ,  "terminal server" ) != NULL )
			return TRUE;
		else
			return FALSE;
	}
	
	return FALSE; 
	
}

BOOL IsGlobalNeeded()
{
	//return (LOWORD(GetVersion()) > 4);
	return IsTerminalServer();
}
#endif // !WIN32

Mutex CSharedMem::m_mtxAllocFree = NULL;

bool CSharedMem::IsInitialized()
{
	return m_bInitialized;
}

/// NOTE:
// if the shared memory is exists already, then the dwSize is ignored.
// otherwise, we will return false if the dwSize is zero.
// Modified by LauranceZhang 2004-11-10 11:35:39 < 
/*
bool CSharedMem::Initialize(RS_DWORD dwSize, char *pszProductName)
*/
//判断共享内存是否是第一次创建
bool CSharedMem::Initialize(RS_DWORD dwSize, char *pszProductName, bool &bIsCreate)
// Modified by LauranceZhang 2004-11-10 11:35:39 > 
{
	if(m_bInitialized)
		return true;

	bool bRet = false;
	bool bCreated = false;

	// try to open an existing file mapping object
// xuzilong 2004-12-07 mod <
#ifdef UNIXPLATFORM
	strcpy(m_szSHMName,  RS_SM_REGISTRY);
#else
	strcpy(m_szSHMName, IsGlobalNeeded() ? RS_SM_GLOBALREG : RS_SM_REGISTRY);
#endif
// xuzilong 2004-12-07 mod >


	if(pszProductName)
	{
		if( (strlen(m_szSHMName) + strlen(pszProductName)) >= 64 )
			return false;
		strcat(m_szSHMName, pszProductName);
	}
#ifdef WIN32
	HSHM hSharedMemory = OpenFileMappingA(FILE_MAP_WRITE, FALSE,
		m_szSHMName);
#else // WIN32
	// create a key from RS_SM_REGISTRY

#if defined(UNIXPLATFORM) && defined(_NOUSESHM_) // 在UNIX下不用共享内存的情况
	HSHM hSharedMemory=HSHM_INVALID;
#else											// 在UNIX下使用共享内存的情况

	key_t key = (key_t)DJBHash(m_szSHMName);
	int flag = SHM_MODE;

	HSHM hSharedMemory = shmget(key, 0, flag);
#endif

#endif // WIN32

	if(hSharedMemory == HSHM_INVALID)
	{
		if(dwSize == 0)
			return false;

		// Additional MAX_FILE_SIZE is for header or new data
		dwSize += MAX_FILE_SIZE;

		// create shared memory
#ifdef WIN32
		hSharedMemory = CreateFileMappingA(INVALID_HANDLE_VALUE,
			CDefaultSecurityAttribtes(),
			PAGE_READWRITE, 0,
			dwSize, m_szSHMName);
#else // WIN32

#if defined(UNIXPLATFORM) && defined(_NOUSESHM_)
		hSharedMemory=0;
#else
		hSharedMemory = shmget(key, dwSize, flag | IPC_CREAT);
#endif

#endif // WIN32

		bCreated = (hSharedMemory != HSHM_INVALID);
// Added by LauranceZhang 2004-11-10 11:35:27 < 
		bIsCreate = bCreated;
// Added by LauranceZhang 2004-11-10 11:35:27 > 		
	}

	if(hSharedMemory != HSHM_INVALID)
	{
#ifdef WIN32
		PBYTE pDest = (PBYTE)MapViewOfFile(hSharedMemory, FILE_MAP_WRITE, 0, 0, 0);
#else // WIN32


#if defined(UNIXPLATFORM) && defined(_NOUSESHM_)
		PBYTE pDest=new BYTE [dwSize];
#else
		PBYTE pDest = (PBYTE)shmat(hSharedMemory, NULL, 0);
#endif

#endif // WIN32

		if(pDest == NULL)
		{
#ifdef WIN32
			CloseHandle(hSharedMemory);
#else // WIN32
#if !defined(UNIXPLATFORM) && !defined(_NOUSESHM_)
			if(bCreated)
				shmctl(hSharedMemory, IPC_RMID, NULL);
#endif

#endif // WIN32
		}
		else
		{
			m_pFileMapping = pDest;

			CMappingHeader* pHeader = (CMappingHeader*)pDest;
			if(bCreated)
			{
				pHeader->dwSize = sizeof(CMappingHeader);
				pHeader->lRefCount = 0;
				pHeader->dwTotal = dwSize;
				pHeader->dwUsed = pHeader->dwSize;
				pHeader->pBaseline = pDest;
#ifdef PRIVATE_DATA
				pHeader->dwPrivateData = 0;
#endif // PRIVATE_DATA
			}

#ifdef WIN32
			InterlockedIncrement(&pHeader->lRefCount);
#else // WIN32
			// create/open an inter-process mutex here for Inc/Dec the lRefCount
			//m_mtxRefCnt = MutexCreate(
#ifdef WIN32
			m_mtxRefCnt = MutexCreate(
				IsGlobalNeeded() ? MT_GLOBAL : MT_INTERPROCESS, MUTEX_REFCNT);
#else
			m_mtxRefCnt = MutexCreate(MT_INTERPROCESS, MUTEX_REFCNT);
#endif // WIN32
				//MT_INTERPROCESS, MUTEX_REFCNT);

			// increment the count
			MutexLock(m_mtxRefCnt, 0);
			pHeader->lRefCount++;
			MutexUnlock(m_mtxRefCnt);
#endif // WIN32m

			//m_mtxAllocFree = MutexCreate(
#ifdef WIN32
				m_mtxAllocFree = MutexCreate(
				IsGlobalNeeded() ? MT_GLOBAL :MT_INTERPROCESS, MUTEX_ALLOC_FREE);
#else
 			m_mtxAllocFree = MutexCreate(MT_INTERPROCESS, MUTEX_ALLOC_FREE);
#endif // WIN32
//				MT_INTERPROCESS, MUTEX_ALLOC_FREE);

			if(bCreated)
			{
				// fill the very first free block header here
				CBlockHeader* pBlock = GetFirstBlock();
				pBlock->dwState = 0;
				pBlock->dwSize = pHeader->dwTotal - pHeader->dwUsed - sizeof(CBlockHeader);
#ifdef SPEED_UP_ON
				pHeader->dwBiggestBlockOffset = (PBYTE)pBlock - (PBYTE)pHeader;
#endif // SPEED_UP_ON
			}

			m_hSharedMemory = hSharedMemory;
			bRet = true;
		}
	}

	m_bInitialized = bRet;

	return bRet;
}

void CSharedMem::Finalize()
{
	if(!m_bInitialized)
		return;

	CMappingHeader* pHeader = (CMappingHeader*)m_pFileMapping;

#ifdef WIN32
	InterlockedDecrement(&pHeader->lRefCount);
// Added by LauranceZhang 2004-11-5 16:54:03 < 
//解决内存泄漏问题
	if(pHeader->lRefCount == 0)
	{
		MutexDestroy(m_mtxAllocFree);
		m_mtxAllocFree = NULL;
	}
// Added by LauranceZhang 2004-11-5 16:54:03 > 
#else // WIN32
	// decrement the count, if the count is zero, delete the mutex
	bool bRemoveNeeded = false;
	MutexLock(m_mtxRefCnt, 0);
	pHeader->lRefCount--;
	MutexUnlock(m_mtxRefCnt);

	if(pHeader->lRefCount == 0)
	{
		MutexDestroy(m_mtxRefCnt);
		m_mtxRefCnt = NULL;

		MutexDestroy(m_mtxAllocFree);
		m_mtxAllocFree = NULL;
		bRemoveNeeded = true;
	}
#endif // WIN32

#ifdef WIN32
	UnmapViewOfFile(m_pFileMapping);
	CloseHandle(m_hSharedMemory);
#else // WIN32

#if defined(UNIXPLATFORM) && defined(_NOUSESHM_) // 在UNIX下不用共享内存的情况
	if(bRemoveNeeded)
		delete m_pFileMapping;
#else
#ifdef PLATFORM_TYPE_SUN_SPARC
	shmdt((char *)m_pFileMapping);
#else
	shmdt(m_pFileMapping);
#endif
	if(bRemoveNeeded)
		shmctl(m_hSharedMemory, IPC_RMID, NULL);
#endif

#endif // WIN32

	m_bInitialized = false;
}

#ifdef SPEED_UP_ON
void* CSharedMem::alloc(size_t size)
{
	if(!m_bInitialized)
		return NULL;

	if(size == 0)
		return NULL;

	// DWORD aligned
	size = ((size >> 2) << 2) + ((!!(size % 4)) << 2);

	void* mem = NULL;
	for(int i=0; i<3; i++)
	{
		if(!MutexLock(m_mtxAllocFree, 5000))
			continue;

		bool failed = false;
		CBlockHeader* pBlock = GetBiggestBlock();
		if(pBlock->dwSize < size)
		{
			MergeFreeBlocks();
			pBlock = GetBiggestBlock();
		}

		if(pBlock->dwSize < size)
			failed = true;
		else
		{
			SetAllocated(pBlock, size);
			mem = GetBlockMemory(pBlock);
		}

		MutexUnlock(m_mtxAllocFree);

		if(mem || failed)
			break;
	}

	return mem;
}

void CSharedMem::free(void* mem)
{
	if(!m_bInitialized)
		return;

	if(mem == NULL)
		return;

	for(int i=0; i<3; i++)
	{
		if(!MutexLock(m_mtxAllocFree, 5000))
			continue;

		// NOTE: if you input an invalid pointer, the result is unpredictable
		CBlockHeader* pBlock = (CBlockHeader*)((PBYTE)mem - sizeof(CBlockHeader));
		SetFree(pBlock);
		MergeFreeBlocks(pBlock, FALSE);

		MutexUnlock(m_mtxAllocFree);

		break;
	}
}
#else // SPEED_UP_ON
void* CSharedMem::alloc(size_t size)
{
	if(!m_bInitialized)
		return NULL;

	if(size == 0)
		return NULL;

	// DWORD aligned
	size = ((size >> 2) << 2) + ((!!(size % 4)) << 2);

	void* mem = NULL;
	for(int i=0; i<3; i++)
	{
		if(!MutexLock(m_mtxAllocFree, 5000))
			continue;

		CBlockHeader* pBlock = GetFirstBlock();
		for(;;)
		{
			if(IsOverRun(pBlock))
				break;

			if(!IsAllocated(pBlock) && pBlock->dwSize >= size)
			{
				SetAllocated(pBlock, size);
				mem = GetBlockMemory(pBlock);
				break;
			}
			else
			{
				pBlock = GetNextBlock(pBlock);
				continue;
			}
		}

		MutexUnlock(m_mtxAllocFree);

		if(mem || IsOverRun(pBlock))
			break;
	}

	return mem;
}

void CSharedMem::free(void* mem)
{
	if(!m_bInitialized)
		return;

	if(mem == NULL)
		return;

	for(int i=0; i<3; i++)
	{
		if(!MutexLock(m_mtxAllocFree, 5000))
			continue;

		CBlockHeader* pBlock = GetFirstBlock();
		for(;;)
		{
			if(IsOverRun(pBlock))
				break;

			if(IsAllocated(pBlock) && mem == GetBlockMemory(pBlock))
			{
				SetFree(pBlock);
				MergeFreeBlocks();
				mem = NULL;
				break;
			}
			else
			{
				pBlock = GetNextBlock(pBlock);
				continue;
			}
		}

		MutexUnlock(m_mtxAllocFree);

		if(mem == NULL || IsOverRun(pBlock))
			break;
	}
}
#endif // SPEED_UP_ON

bool CSharedMem::read(const void* source, void* dest, size_t size)
{
	/// TODO:
	return true;
}

bool CSharedMem::write(const void* source, void* dest, size_t size)
{
	/// TODO:
	return true;
}
