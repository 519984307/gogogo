#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 1024
#define IPSTR "127.0.0.1" 
#define PORT 8686

int remote_filter(char *url) {
   
    // sock address, define in baseconfig.h
    const char *domain = "/var/ipc/ravblock";
    int ret;
    int socket_fd;

    struct sockaddr_un srv_addr;

    socket_fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(socket_fd < 0) {
        return -1;
    }
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path,domain);

    ret = connect(socket_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(ret == -1) {
        close(socket_fd);
        return -1;
    }

    // the send data format
    // declear in CommonAPI, rserializer.h
    int temp = 1; //string
    write(socket_fd, &temp, sizeof(int));

    // write size
    temp = strlen(url) + sizeof(int)*2;
    write(socket_fd, &temp, sizeof(int));

    // write command id
    temp = 104; //CommandBundle::SCAN_FILE;
    write(socket_fd, &temp, sizeof(int));

    temp = 0; //Integer value
    write(socket_fd, &temp, sizeof(int));

    // write data
    write(socket_fd, url, strlen(url));

    // read the result type 
    read(socket_fd, &temp, sizeof(int));
    if (0 == temp) {
        // read the result
        read(socket_fd, &temp, sizeof(int));
    }
    close(socket_fd);

    return temp;
   
   /* 
    int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        char str1[4096], str2[4096], buf[BUFSIZE], *str;
        socklen_t len;
        fd_set   t_set1;
        struct timeval  tv;

         //创建套接字
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                printf("创建网络连接失败,本线程即将终止---socket error!\n");
                return -1;
        };

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
                printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
                return -1;
        };

        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                printf("连接到服务器失败,connect error!\n");
                return -1;
        }
        printf("与远端建立了连接\n");
        memset(str2, 0, 4096);
        strcat(str2, "theDataToPost");
        str=(char *)malloc(128);
        len = strlen(str2);
        sprintf(str, "%d", len);

        memset(str1, 0, 4096);
        strcat(str1, "GET /deny.json?www.baidu.com HTTP/1.1\n");
        strcat(str1, "Host: 127.0.0.1\n");
        strcat(str1, "Content-Type: text/html\n");
        strcat(str1, "Content-Length: ");
        strcat(str1, str);
        strcat(str1, "\n\n");

        strcat(str1, str2);
        strcat(str1, "\r\n\r\n");
        printf("%s\n",str1);

        ret = write(sockfd,str1,strlen(str1));
        if (ret < 0) {
                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
                return -1;
        }else{
                printf("消息发送成功，共发送了%d个字节！\n\n", ret);
        }


        memset(buf, 0, 4096);
        i= read(sockfd, buf, 4095);
     */   /*
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);

        while(1){
                sleep(2);
                tv.tv_sec= 0;
                tv.tv_usec= 0;
                h= 0;
                printf("--------------->1");
                h= select(sockfd +1, &t_set1, NULL, NULL, &tv);
                printf("--------------->2");

                //if (h == 0) continue;
                if (h < 0) {
                        close(sockfd);
                        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
                        return -1;
                };

                if (h > 0){
                        memset(buf, 0, 4096);
                        i= read(sockfd, buf, 4095);
                        if (i==0){
                                close(sockfd);
                                printf("读取数据报文时发现远端关闭，该线程终止！\n");
                                return -1;
                        }

                        printf("%s\n", buf);
                }
        }
        */
        //close(sockfd);
        return 0;
}
