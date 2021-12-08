#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <linux/netlink.h>

#include "usrlink.h"

class CRUserNFLink{
public:
    CRUserNFLink();
    ~CRUserNFLink();
    
    //on success, return 0
    int Init();

    int SendLine(int cmd_type, const char *text_line, int len);
    virtual void ReceivedMsg(int nCmdType, void *Data, int nLen) = 0;

    bool InitSocket(int nNFLUserCmd);
    void UnInitSocket();
    void UnInit();
    int StartRecvThread();

protected:
    void SocketThread();
    static void * ThreadFunc(void *Args);

private:
    int m_nSockNfl;
    int m_nPID;
    struct nlmsghdr *m_ptNlMsgHdr;
    struct msghdr m_tSendMsg;
    struct sockaddr_nl m_tSSAddr; //发送源地址
    struct sockaddr_nl m_tSSDAddr;//发送目标地址
    struct iovec m_tSIOV;   //发送iov
    pthread_t m_tRcvThread;
    bool    m_bRunning;
    pthread_mutex_t   m_tLock;
    bool    m_bSockInited;
    bool    m_bFindError;
};
