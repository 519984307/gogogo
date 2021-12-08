#include "webserver.h"
#include "threadpool.h"
#include "commonapi.h"
#include "baseconfig.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#if defined (COMMON_LINUX)
#include <sys/sendfile.h>
#endif
#include <sys/wait.h>

const int BUFFER_SIZE = 4096;


WebService::Server* WebService::Server::NEW(const char *path, unsigned short port, WebObserver* observer) {
    WebService::Server *self = new WebService::Server(observer);
    if (self) {
        if (self->Construct(path, port)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

WebService::Server::Server(WebObserver* observer) {
    mThreadPool = NULL;
    mObserver = observer;
    mServerFd = 0;
    mStatus = 0;
}

WebService::Server::~Server() {
    if (mThreadPool) {
        delete mThreadPool;
        mThreadPool = NULL;
    }

    if (mServerFd > 0) {
        close(mServerFd);
        mServerFd = 0;
    }
}

int WebService::Server::Construct(const char *path, unsigned short port) {
    strcpy(mDomainPath, path);
    mPort = port;

    if (access(path, F_OK)) {
        return -1;
    }

    //创建线程池
    mThreadPool = ThreadPool<Receiver>::NEW(1);

    char blockfile[128];
    sprintf(blockfile, "%s/%s", path, BROWSER_IPC_CONF_PATH);
    if (access(blockfile, F_OK)) {
        FILE *file = fopen(blockfile, "w+");
        fclose(file);
    }

    return 0;
}

void* WebService::Server::ThreadWork(void *args) {
    WebService::Server *server = (WebService::Server*)args;
    server->Work();
    return NULL;
}

int WebService::Server::Work() {
    struct sockaddr_in servaddr, client;

    //设置服务端的sockaddr_in
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(mPort);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建socket
    mServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if(mServerFd < 0) {
        return -1;
    }

    //绑定
    int ret = bind(mServerFd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        return -1;
    }

    //监听
    ret = listen(mServerFd, 10);
    if (ret < 0) {
        return -1;
    }

    //线程池开始运行
    mThreadPool->Start();

    while(mStatus > 0) {
        socklen_t len = sizeof(client);

        //接受连接
        int connfd = accept(mServerFd, (struct sockaddr *)&client, &len);
        Receiver *child = new Receiver(connfd, mDomainPath);
        child->SetObserver(mObserver);

        //向线程池添加任务
        mThreadPool->AppendTask(child);
    }
}

int WebService::Server::Start() {
    mStatus = true;
    return pthread_create(&mThread, NULL, ThreadWork, this);
}

int WebService::Server::Stop() {
    mStatus = false;
    if (mServerFd > 0) {
        close(mServerFd);
        mServerFd = 0;
    }
    return 0;
}



//
WebService::Receiver::Receiver(char *domain_path) {
    strcpy(mDomainPath, domain_path);
    mObserver = NULL;
}

WebService::Receiver::Receiver(int fd, char *domain_path) : mConnectFd(fd) {
    strcpy(mDomainPath, domain_path);
    mObserver = NULL;
}

void WebService::Receiver::Response(char *message, int status) {
    char buf[512];
    sprintf(buf, "HTTP/1.1 %d OK\r\nConnection: Close\r\n"
    "content-length:%d\r\n\r\n", status, (int)strlen(message));

    sprintf(buf, "%s%s", buf, message);
    write(mConnectFd, buf, strlen(buf));
}

void WebService::Receiver::FileResponse(int size, int status) {
    char buf[128];
    sprintf(buf, "HTTP/1.1 %d OK\r\nConnection: Close\r\n"
    "content-length:%d\r\n\r\n", status, size);
    write(mConnectFd, buf, strlen(buf));
}

void WebService::Receiver::Work()
{
    char buffer[BUFFER_SIZE];
    int  size;

read:
    size = read(mConnectFd, buffer, BUFFER_SIZE - 1);
    if(size > 0) {
        char method[5];
        char filename[50];
        int i, j;
        i = j = 0;

        while(buffer[j] != ' ' && buffer[j] != '\0') {
            method[i++] = buffer[j++];
        }
        ++j;
        method[i] = '\0';
        i = 0;

        while(buffer[j] != ' ' && buffer[j] != '\0') {
            filename[i++] = buffer[j++];
        }
        filename[i] = '\0';

        if(strcasecmp(method, "GET") == 0) {
            //get method
            GetResponse(filename);
        }
        else if(strcasecmp(method, "POST") == 0) {
            //post method
            char argvs[100];
            memset(argvs, 0, sizeof(argvs));
            int k = 0;
            char *ch = NULL;
            ++j;

            while((ch = strstr(argvs, "Content-Length")) == NULL) {
                k = 0;
                memset(argvs, 0, sizeof(argvs));
                while(buffer[j] != '\r' && buffer[j] != '\0') {
                    argvs[k++] = buffer[j++];
                }
                ++j;
            }

            int length;
            char *str = strchr(argvs, ':');
            ++str;
            sscanf(str, "%d", &length);
            j = strlen(buffer) - length;
            k = 0;
            memset(argvs, 0, sizeof(argvs));
            while(buffer[j] != '\r' && buffer[j] != '\0') {
                argvs[k++] = buffer[j++];
            }

            argvs[k] = '\0';
            PostResponse(filename, argvs);
        }
        else {
            char message[512];
            sprintf(message, "<html><title>Tinyhttpd Error</title>");
            sprintf(message, "%s<body>\r\n", message);
            sprintf(message, "%s 501\r\n", message);
            sprintf(message, "%s <p>%s: Httpd does not implement this method",
                message, method);
            sprintf(message, "%s<hr><h3>The Tiny Web Server<h3></body>", message);
            Response(message, 501);
        }
    }
    else if(size < 0) {
        goto read;
    }

    sleep(3);
    //wait for client close, avoid TIME_WAIT
    close(mConnectFd);
}

void WebService::Receiver::GetResponse(char *filename) {
    char file[256];
    strcpy(file, mDomainPath);

    int i = 0;
    bool is_dynamic = false;
    char argv[128];

    //查找是否有？号
    while(filename[i] != '?' && filename[i] != '\0') {
        ++i;
    }

    if(filename[i] == '?') {
        //有?号，则是动态请求
        int j = i;
        ++i;
        int k = 0;
        while(filename[i] != '\0') {
            //分离参数和文件名
            argv[k++] = filename[i++];
        }

        argv[k] = '\0';
        filename[j] = '\0';
        is_dynamic = true;
    }

    if(strcmp(filename, "/") == 0) {
        strcat(file, "/index.html");
    }
    else {
        strcat(file, filename);
    }

    struct stat filestat;
    int ret = stat(file, &filestat);

    if(ret < 0 || S_ISDIR(filestat.st_mode)) {
        //file doesn't exits
        char message[512];
        sprintf(message, "<html><title>Tinyhttpd Error</title>");
        sprintf(message, "%s<body>\r\n", message);
        sprintf(message, "%s 404\r\n", message);
        sprintf(message, "%s <p>GET: Can't find the file", message);
        sprintf(message, "%s<hr><h3>The Tiny Web Server<h3></body>",
            message);
        Response(message, 404);
        return;
    }

    if(is_dynamic) {
//        if(fork() == 0) {
//            // 创建子进程执行对应的子程序，多线程中，创建子进程，
//            // 只有当前线程会被复制，其他线程就“不见了”，这正符合我们的要求，
//            // 而且fork后执行execl，程序被进程被重新加载
//            dup2(mConnectFd, STDOUT_FILENO);
//            //将标准输出重定向到sockfd,将子程序输出内容写到客户端去。
//            execl(file, argv); //执行子程序
//        }
//        wait(NULL);
        int ret = 0;
        if (mObserver) {
            ret = mObserver->OnWebRequestObserver(WebObserver::GET, argv, NULL);
        }

        char message[512];
        sprintf(message, "%d", ret);
        Response(message, 200);
        return;

    }
    else {
        int filefd = open(file, O_RDONLY);
        FileResponse(filestat.st_size, 200);
        //使用“零拷贝”发送文件
#if defined (COMMON_LINUX)
        sendfile(mConnectFd, filefd, 0, filestat.st_size);
#endif
    }
}

void WebService::Receiver::PostResponse(char *filename, char *argvs) {
    char file[512];
    strcpy(file, mDomainPath);

    strcat(file, filename);

    struct stat filestat;
    int ret = stat(file, &filestat);
    if(ret < 0 || S_ISDIR(filestat.st_mode)) {
        //file doesn't exits
        char message[512];
        sprintf(message, "<html><title>Tinyhttpd Error</title>");
        sprintf(message, "%s<body>\r\n", message);
        sprintf(message, "%s 404\r\n", message);
        sprintf(message, "%s <p>GET: Can't find the file", message);
        sprintf(message, "%s<hr><h3>The Tiny Web Server<h3></body>",
            message);
        Response(message, 404);
        return;
    }

    char argv[20];
    int a, b;
    ret = sscanf(argvs, "a=%d&b=%d", &a, &b);
    if(ret < 0 || ret != 2) {
        char message[512];
        sprintf(message, "<html><title>Tinyhttpd Error</title>");
        sprintf(message, "%s<body>\r\n", message);
        sprintf(message, "%s 404\r\n", message);
        sprintf(message, "%s <p>GET: Parameter error", message);
        sprintf(message, "%s<hr><h3>The Tiny Web Server<h3></body>",
            message);
        Response(message, 404);
        return;
    }

    sprintf(argv, "%d&%d", a, b);
    if(fork() == 0) {
        // 创建子进程执行对应的子程序，多线程中，创建子进程，
        // 只有当前线程会被复制，其他线程就“不见了”，这正符合我们的要求，
        // 而且fork后执行execl，程序被进程被重新加载

        dup2(mConnectFd, STDOUT_FILENO);
        //将标准输出重定向到sockfd,将子程序输出内容写到客户端去。
        execl(file, argv); //执行子程序
    }
    wait(NULL);
}

