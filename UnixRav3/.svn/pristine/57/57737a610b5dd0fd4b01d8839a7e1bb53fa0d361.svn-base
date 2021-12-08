#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

    
#define GROUP_ADDR  "239.193.168.87"
#define GROUP_PORT  7767
#define MSGBUFSIZE 256


int main() {
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    char msgbuf[MSGBUFSIZE];

    u_int yes=1;
    int fd = -1;
    if ((fd = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        return -1;
    }


    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        return -2;
    }

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(GROUP_PORT);

    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
        return -3;
    }

    mreq.imr_multiaddr.s_addr = inet_addr(GROUP_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        return -4;
    }

    int addrlen = sizeof(addr);
    int nbytes = 0;
    while (1) {
        if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr, (socklen_t *)&addrlen)) < 0) {
            return -4;
        }

        msgbuf[nbytes] = 0;
        printf("%s\n", msgbuf);
    }
    return 0;
}
