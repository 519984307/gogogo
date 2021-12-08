#include "RUserNFLink.h"
#include "usrlink.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;


CRUserNFLink::CRUserNFLink()
:m_ptNlMsgHdr(NULL),m_bRunning(false)
{
    
}

CRUserNFLink::~CRUserNFLink()
{
    UnInit();
}

int CRUserNFLink::Init()
{
    if(!InitSocket(USER_NETLINK_CMD)){
        DEBUG("CRUserNFLink::Init(%d) fail", USER_NETLINK_CMD);
        return -1;
    }
    return StartRecvThread();
}

//text_line: included the null terminator, do the same as the len argument  
int CRUserNFLink::SendLine(int cmd_type, const char *text_line, int len)
{
    if (m_bFindError) {
        return -1;
    }
    int ret = -1;
    memset(&m_tSendMsg, 0 ,sizeof(struct msghdr));
    int newlen=len, i = len - 1;
    if (len > 0) {
        while(i >= 0) {
            if (text_line[i] == '\r' || text_line[i] == '\n') {
                newlen--;
            }else{
                break;
            }
            i--;
        }
    }
    if (newlen > 0) {
        memcpy(NLMSG_DATA(m_ptNlMsgHdr), text_line, newlen);
        m_ptNlMsgHdr->nlmsg_len = NLMSG_LENGTH(newlen);
    }else{
        m_ptNlMsgHdr->nlmsg_len = NLMSG_LENGTH(0);
    }
    m_ptNlMsgHdr->nlmsg_type = cmd_type;
    m_ptNlMsgHdr->nlmsg_pid = m_nPID;  /* self pid */
    m_ptNlMsgHdr->nlmsg_flags = 0;
    m_tSIOV.iov_base = (void *)m_ptNlMsgHdr;
    m_tSIOV.iov_len = m_ptNlMsgHdr->nlmsg_len;
    m_tSendMsg.msg_name = (void *)&m_tSSDAddr;
    m_tSendMsg.msg_namelen = sizeof(m_tSSDAddr);
    m_tSendMsg.msg_iov = &m_tSIOV;
    m_tSendMsg.msg_iovlen = 1;
    pthread_mutex_lock(&m_tLock);
    ret = sendmsg(m_nSockNfl, &m_tSendMsg, 0);
    pthread_mutex_unlock(&m_tLock);
    if (ret == -1) {
        //DEBUG("error, sendmsg fail");
    }
    return ret;
}

bool CRUserNFLink::InitSocket(int nNFLUserCmd)
{
    m_ptNlMsgHdr = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_MSG_LEN));
    if(m_ptNlMsgHdr == NULL)
        return false;

    m_nSockNfl = socket(AF_NETLINK, SOCK_RAW, nNFLUserCmd);
    if (m_nSockNfl == -1) {
        //printf("Error, socket =%d errno=%d %s\n", m_nSockNfl, errno, strerror(errno));
        free(m_ptNlMsgHdr);
        m_ptNlMsgHdr = NULL;
        return false;
    }
    memset(&m_tSSAddr, 0, sizeof(m_tSSAddr));
    memset(&m_tSSDAddr, 0, sizeof(m_tSSDAddr));
    m_nPID = getpid();

    m_tSSAddr.nl_family = AF_NETLINK;
    m_tSSAddr.nl_pid = m_nPID;
    m_tSSAddr.nl_groups = 1;
    bind(m_nSockNfl, (struct sockaddr*)&m_tSSAddr, sizeof(m_tSSAddr));
    int flags = fcntl (m_nSockNfl, F_GETFL, 0);
    if (flags == -1) {
        DEBUG("Error, fcntl get fail.\n");
        free(m_ptNlMsgHdr);
        m_ptNlMsgHdr = NULL;
        close(m_nSockNfl);
        return false;
    }
    if(fcntl (m_nSockNfl, F_SETFL, flags | O_NONBLOCK) == -1) {
        DEBUG("Error, fcntl set fail.\n");
        free(m_ptNlMsgHdr);
        m_ptNlMsgHdr = NULL;
        close(m_nSockNfl);
        return false;
    }

    m_tSSDAddr.nl_family = AF_NETLINK;
    m_tSSDAddr.nl_pid = 0; //kernel
    m_tSSDAddr.nl_groups = 1;
    
    pthread_mutex_init(&m_tLock, NULL);
    
    return true;   
}

int CRUserNFLink::StartRecvThread()
{
    pthread_attr_t attr;
    if(pthread_attr_init(&attr) != 0){
        DEBUG("error, pthread_attr_init fail");
        free(m_ptNlMsgHdr);
        m_ptNlMsgHdr = NULL;
        close(m_nSockNfl);
        return -1;
    }
    if(pthread_create(&m_tRcvThread, &attr, ThreadFunc, (void*)this) != 0){
        DEBUG("error, pthread_create fail");
        free(m_ptNlMsgHdr);
        m_ptNlMsgHdr = NULL;
        close(m_nSockNfl);
        return -1;
    }
    return 0;
}

void CRUserNFLink::UnInitSocket()
{
    close(m_nSockNfl);
    if (m_ptNlMsgHdr) {
        free(m_ptNlMsgHdr);
        m_ptNlMsgHdr = NULL;
    }
}

void CRUserNFLink::UnInit()
{
    while(m_bRunning)
        m_bRunning = false;

    pthread_join(m_tRcvThread, NULL);
    pthread_mutex_destroy(&m_tLock);

    UnInitSocket();   
}

void CRUserNFLink::SocketThread()
{
    int epfd, nfds, nret;
    struct epoll_event ev,events[5];
    struct sockaddr_nl nladdr;
    struct msghdr msg;
    struct iovec iov;
    int counter = 0;
    m_bFindError = false;
    bool first_init_epoll = true;
    struct nlmsghdr *nlhdr = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_MSG_LEN));
    if(nlhdr == NULL){
        DEBUG("error, out of memory");
        return;
    }
    iov.iov_base = (void *)nlhdr;
    iov.iov_len = NLMSG_SPACE(MAX_MSG_LEN);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    
    epfd = epoll_create(1);
    //ev.data.fd = m_nSockNfl;
    ev.events = EPOLLIN|EPOLLET;
    //epoll_ctl(epfd, EPOLL_CTL_ADD, m_nSockNfl, &ev);

    m_bRunning = true;
    while(m_bRunning){
        if (false == m_bFindError) {
            if (first_init_epoll) {
                ev.data.fd = m_nSockNfl;
                epoll_ctl(epfd, EPOLL_CTL_ADD, m_nSockNfl, &ev);
                first_init_epoll = false;
            }
            iov.iov_base = (void *)nlhdr;
            iov.iov_len = NLMSG_SPACE(MAX_MSG_LEN);
            msg.msg_iov = &iov;
            msg.msg_iovlen = 1;
            msg.msg_name = (void*)&nladdr;
            msg.msg_namelen = sizeof(nladdr);

            nfds = epoll_wait(epfd, events, 5, 300);
            for (int i = 0; i < nfds; i++) {
                if (events[i].events & EPOLLIN) {
                    if (events[i].data.fd == m_nSockNfl) {
                        pthread_mutex_lock(&m_tLock);
                        nret = recvmsg(m_nSockNfl, &msg, 0);
                        pthread_mutex_unlock(&m_tLock);
                        if(nret > 0){
                            ReceivedMsg(nlhdr->nlmsg_type, NLMSG_DATA(nlhdr), nlhdr->nlmsg_len);
                        }

                        ev.data.fd = m_nSockNfl;
                        ev.events = EPOLLIN|EPOLLET;
                        epoll_ctl(epfd, EPOLL_CTL_MOD, m_nSockNfl, &ev);
                    }
                }else if (events[i].events & EAGAIN) {
                    break;
                }else if ((events[i].events & EPOLLHUP) || events[i].events & EPOLLERR) {
                    m_bFindError = true;
                }            
            }
        }else {
            if (counter > 10) {
                counter = 0;
                UnInitSocket();
                if (InitSocket(USER_NETLINK_CMD)) {
                    m_bFindError = false;
                    first_init_epoll = true;
                }
            }else{
                counter++;
                sleep(1);
            }
        }
    }
    free(nlhdr);
    nlhdr = NULL;
}

void *CRUserNFLink::ThreadFunc(void *Args)
{
    CRUserNFLink *pthis = (CRUserNFLink *)Args; 
    if (pthis == NULL) {
        return NULL;
    }
    pthis->SocketThread();
    return NULL;
}
