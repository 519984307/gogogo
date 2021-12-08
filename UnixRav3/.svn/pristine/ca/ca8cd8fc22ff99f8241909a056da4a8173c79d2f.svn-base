#ifndef UNIXPLATFORM
    #include "stdafx.h"
#endif

#include "cfgmutex.h"
#include "RSERROR.H"
#include "extern.h"

CCfgMutex::CCfgMutex(const RS_CHAR *pName)
{
        m_Mutex = 0;
        if(pName)
                Create(pName);
}

CCfgMutex::~CCfgMutex()
{
        Destroy();
}

RS_RESULT CCfgMutex::Create(const RS_CHAR *pName)
{
        if(!pName)
                return ERR_CFG_PARAM_INVALID;
        if(m_Mutex)
                return ERR_CFG_MUTEX_EXISITED;
#ifdef UNIXPLATFORM
        m_Mutex = MutexCreate(  MT_INTERPROCESS, pName);
#else
        m_Mutex = MutexCreate( IsTerminalServer() ? MT_GLOBAL : MT_INTERPROCESS, pName);
#endif
        if(!m_Mutex)
                return ERR_CFG_MUTEX_CREATEFAILED;
        return ERR_SUCCESS;
}

RS_VOID CCfgMutex::Destroy()
{
        MutexUnlock(m_Mutex);
        MutexDestroy(m_Mutex);
        m_Mutex = 0;
}

RS_RESULT CCfgMutex::Lock(RS_DWORD dwTimeWait)
{
        if(!MutexLock(m_Mutex, dwTimeWait))
                return ERR_CFG_MUTEX_LOCKFAILED;
        return ERR_SUCCESS;
}

RS_VOID CCfgMutex::Unlock()
{
        MutexUnlock(m_Mutex);
}

RS_RESULT CCfgMutex::Create(RS_DWORD dwType, RS_CHAR *pProductName)
{
        CHAR szMutexName[128] = {0};
        GetMutexName(dwType, szMutexName, pProductName);
        return Create(szMutexName);
}

#include "mutex.h"
RS_DWORD CCfgMutex::GetStatus()
{
#ifdef UNIXPLATFORM
        // TODO
        return RS_FALSE;
#else

        Muteks* pmtx = (Muteks*)m_Mutex;
        if(!pmtx)
                return WAIT_ABANDONED;
        switch(WaitForSingleObject((HANDLE)pmtx->pobj, 10))
        {
        case WAIT_ABANDONED:
                // We now have ownership, but the object is not signalled
                // added by zhanglingang 2004.10.13 <
                //此为程序非法退出时状态，即无信号状态，这时应释放Mutex，获得信号，否则程序再一次启动时就会出现死等
                ReleaseMutex((HANDLE)pmtx->pobj);
                return WAIT_ABANDONED;
                // added by zhanglingang 2004.10.13 >
        case WAIT_OBJECT_0:
        case WAIT_TIMEOUT:
                return WAIT_OBJECT_0;
        }
        return RS_FALSE;
#endif
}


// Deleted by LauranceZhang 2004-10-26 11:53:30 <
/*
CMemMutex::CMemMutex(CCfgMutex *pMutex)
{
        m_pMutex = pMutex;
        if(m_pMutex)
                m_pMutex->Lock();

        if(g_plRefWrite)
        {
                MyTrace("CMutexMemWrite ->:ReadRequestNum=%d, ReadNum=%d, WriteNum=%d", *g_plRefReadRequest, *g_plRefRead, *g_plRefWrite);
                //判断读请求计数
                while( 0!=InterlockedCompareExchange((RS_LONG *)g_plRefReadRequest, *g_plRefReadRequest, 0) )
                {
                        Sleep(10);

                        //如果读计数为0，则表明一定有读进程非正常退出，这时要修正计数
                        if(0 == InterlockedCompareExchange(g_plRefRead, *g_plRefRead, 0))
                                //将读请求计数清0
                                InterlockedCompareExchange(g_plRefReadRequest, 0, *g_plRefReadRequest);
                }
                //判断读计数
                while( 0!=InterlockedCompareExchange(g_plRefRead, *g_plRefRead, 0) )
                {
                        Sleep(10);

                        //如果读请求计数为0，则表明一定有读进程非正常退出，这时要修正计数
                        if(0 == InterlockedCompareExchange(g_plRefReadRequest, *g_plRefReadRequest, 0))
                                //将读请求计数清0
                                InterlockedCompareExchange(g_plRefRead, 0, *g_plRefRead);
                }
                //写计数加1
                InterlockedExchangeAdd(g_plRefWrite, 1);
        }
        MyTrace("CMutexMemWrite --:ReadRequestNum=%d, ReadNum=%d, WriteNum=%d", *g_plRefReadRequest, *g_plRefRead, *g_plRefWrite);
}
CMemMutex::~CMemMutex()
{
        if(m_pMutex)
        {
                m_pMutex->Unlock();
                m_pMutex = 0;
        }

        if(g_plRefWrite)
        {
                //将读计数清0
                InterlockedCompareExchange(g_plRefRead, 0, *g_plRefRead);
                //写计数减1
                InterlockedExchangeAdd(g_plRefWrite, -1);
                MyTrace("CMutexMemWrite <-:ReadRequestNum=%d, ReadNum=%d, WriteNum=%d", *g_plRefReadRequest, *g_plRefRead, *g_plRefWrite);
        }
}
*/
// Deleted by LauranceZhang 2004-10-26 11:53:30 >

CMemMutex::CMemMutex(CCfgMutex *pMutex)
{
        m_pMutex = pMutex;
        if(m_pMutex)
                m_pMutex->Lock();
}
CMemMutex::~CMemMutex()
{
        if(m_pMutex)
                m_pMutex->Unlock();
}
// Deleted by LauranceZhang 2004-10-28 18:57:39 <
/*
CMemMutex::CMemMutex(RS_DWORD dwMutexType, CCfgMutex *pMutexRead, CCfgMutex *pMutexWrite)
{
        m_dwMutexType = dwMutexType;
//	if(g_plRefWrite)
//		MyTrace("->Type=%d: ReadRequestNum=%d, ReadNum=%d, WriteNum=%d", m_dwMutexType, *g_plRefReadRequest, *g_plRefRead, *g_plRefWrite);
        if(m_dwMutexType==MUTEX_TYPE_READ)
        {
                m_pMutexWrite = 0;
                m_pMutexRead = pMutexRead;
                if(m_pMutexRead  && g_bIsMutexWriteCreated)
                        m_pMutexRead->Lock();
                if(g_plRefWrite)
                {
                        //写请求计数加1
                        InterlockedExchangeAdd(g_plRefReadRequest, 1);
                        //如果读计数和写计数都不为0，则表明一定有读进程或写进程非正常退出，这是要修正计数
                        if( (0 != InterlockedCompareExchange(g_plRefRead, *g_plRefRead, 0)) &&
                                (0 != InterlockedCompareExchange(g_plRefWrite, *g_plRefWrite, 0)) )
                        {
                                if(pMutexWrite)
                                {
                                        //正在写
                                        if(WAIT_OBJECT_0 == pMutexWrite->GetStatus())
                                                InterlockedCompareExchange(g_plRefRead, 0, *g_plRefRead);	//将读计数清0
                                        else
                                                InterlockedCompareExchange(g_plRefWrite, 0, *g_plRefWrite);	//将写计数清0
                                }
                        }
                        //如果读计数和写计数都为0，这时要修正读请求计数
                        if( (0 == InterlockedCompareExchange(g_plRefRead, *g_plRefRead, 0)) &&
                                (0 == InterlockedCompareExchange(g_plRefWrite, *g_plRefWrite, 0)) &&
                                (1 != InterlockedCompareExchange(g_plRefReadRequest, *g_plRefReadRequest, 1)) )
                                InterlockedCompareExchange(g_plRefReadRequest, 1, *g_plRefReadRequest);	//将读计数置1

                        //判断写计数
                        while( 0!=InterlockedCompareExchange(g_plRefWrite, *g_plRefWrite, 0) )
                                Sleep(10);

                        //写计数加1
                        InterlockedExchangeAdd(g_plRefRead, 1);
                }
        }
        else
        {
                m_pMutexRead = 0;
                m_pMutexWrite = pMutexWrite;
                if(m_pMutexWrite && g_bIsMutexWriteCreated)
                        m_pMutexWrite->Lock();

                if(g_plRefWrite)
                {
                        //判断读请求计数
                        while( 0!=InterlockedCompareExchange((RS_LONG *)g_plRefReadRequest, *g_plRefReadRequest, 0) )
                        {
                                Sleep(10);

                                //如果读计数为0，则表明一定有读进程非正常退出，这时要修正计数
                                if(0 == InterlockedCompareExchange(g_plRefRead, *g_plRefRead, 0))
                                        //将读请求计数清0
                                        InterlockedCompareExchange(g_plRefReadRequest, 0, *g_plRefReadRequest);
                        }
                        //判断读计数
                        while( 0!=InterlockedCompareExchange(g_plRefRead, *g_plRefRead, 0) )
                        {
                                Sleep(10);

                                //如果读请求计数为0，则表明一定有读进程非正常退出，这时要修正计数
                                if(0 == InterlockedCompareExchange(g_plRefReadRequest, *g_plRefReadRequest, 0))
                                        //将读请求计数清0
                                        InterlockedCompareExchange(g_plRefRead, 0, *g_plRefRead);
                        }
                        //写计数加1
                        InterlockedExchangeAdd(g_plRefWrite, 1);
                }
        }
//	if(g_plRefWrite)
//		MyTrace("--Type=%d: ReadRequestNum=%d, ReadNum=%d, WriteNum=%d", m_dwMutexType, *g_plRefReadRequest, *g_plRefRead, *g_plRefWrite);
}
*/
// Deleted by LauranceZhang 2004-10-28 18:57:39 >
// Deleted by LauranceZhang 2004-10-28 18:57:59 <
/*
CMemMutex::~CMemMutex()
{
        if(m_dwMutexType==MUTEX_TYPE_READ)
        {
                if(m_pMutexRead  && g_bIsMutexWriteCreated)
                        m_pMutexRead->Unlock();

                if(g_plRefWrite)
                {
                        //将写计数清0
                        InterlockedCompareExchange(g_plRefWrite, 0, *g_plRefWrite);
                        //写计数减1
                        InterlockedExchangeAdd(g_plRefRead, -1);
                        InterlockedExchangeAdd(g_plRefReadRequest, -1);
                }
        }
        else
        {
                if(m_pMutexWrite  && g_bIsMutexWriteCreated)
                        m_pMutexWrite->Unlock();

                if(g_plRefWrite)
                {
                        //将读计数清0
                        InterlockedCompareExchange(g_plRefRead, 0, *g_plRefRead);
                        //写计数减1
                        InterlockedExchangeAdd(g_plRefWrite, -1);
                }
        }

//	if(g_plRefWrite)
//		MyTrace("<-Type=%d: ReadRequestNum=%d, ReadNum=%d, WriteNum=%d", m_dwMutexType, *g_plRefReadRequest, *g_plRefRead, *g_plRefWrite);
}
*/
// Deleted by LauranceZhang 2004-10-28 18:57:59 >
