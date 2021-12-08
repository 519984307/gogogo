#ifndef __CFGMUTEX_H__
#define __CFGMUTEX_H__

#ifndef UNIXPLATFORM
#include "stdafx.h"
#endif
#include "RSTYPE.H"
#include "mutex.h"

class CCfgMutex
{
public:
        RS_RESULT Create(RS_DWORD dwType, RS_CHAR *pProductName);
        RS_VOID Unlock();
        RS_RESULT Lock(RS_DWORD dwTimeWait = 0);
        RS_VOID Destroy();
        RS_RESULT Create(const RS_CHAR  *pName);
        CCfgMutex(const RS_CHAR *pName = RS_NULL);
        virtual ~CCfgMutex();
        RS_DWORD GetStatus();
private:
        Mutex m_Mutex;
};

// Deleted by LauranceZhang 2004-10-26 11:53:37 <
/*
class CMemMutex
{
public:
        CMemMutex(CCfgMutex *pMutex);
        ~CMemMutex();
private:
        CCfgMutex *m_pMutex;
};
*/
// Deleted by LauranceZhang 2004-10-26 11:53:37 >

// Modified by LauranceZhang 2004-11-5 17:16:08 <
/*
class CMemMutex
{
        enum{MUTEX_TYPE_READ =1, MUTEX_TYPE_WRITE=2};
public:
        CMemMutex(RS_DWORD dwMutexType, CCfgMutex *pMutexRead = RS_NULL, CCfgMutex *pMutexWrite = RS_NULL);
        ~CMemMutex();
private:
        CCfgMutex *m_pMutexRead;
        CCfgMutex *m_pMutexWrite;
        RS_DWORD m_dwMutexType;
};
*/
class CMemMutex
{
public:
        CMemMutex(CCfgMutex *pMutex = RS_NULL);
        ~CMemMutex();
private:
        CCfgMutex *m_pMutex;
};
// Modified by LauranceZhang 2004-11-5 17:16:08 >

#endif //__CFGMUTEX_H__
