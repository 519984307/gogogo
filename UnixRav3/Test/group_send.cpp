#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


#define GROUP_ADDR  "239.193.168.87"
#define GROUP_PORT  7767


#define MSGBUFSIZE 256

int main() {
    struct sockaddr_in addr;
    int fd;
    const char *message="Hello, World!";

    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        return -1;
    }

    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(GROUP_ADDR);
    addr.sin_port=htons(GROUP_PORT);

    while (1)
    {
        if (sendto(fd,message, strlen(message), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        {
            return -1;
        }
        sleep(1);
    }
    return 0;
}
