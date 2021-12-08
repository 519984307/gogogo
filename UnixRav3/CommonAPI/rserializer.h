#ifndef RSERIALIZER_H
#define RSERIALIZER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include "commonapi.h"

class RSerializer {
public:
    virtual ~RSerializer() {}
    virtual bool Internalize(const char */*data*/, int /*len*/) {
        return true;
    }

    virtual int Externalize(char **/*data*/, int */*len*/) {
        return 0;
    }

    virtual bool Internalize(int /*fd*/) {
        return true;
    }

    virtual bool Externalize(int /*fd*/) {
        return 0;
    }
};


template<class T>
class RStreamPack
{
public:
    enum PackType{
        NO_EXT = 0,
        STRING,
        MEMORY,
        FILE
    };

public:
    RStreamPack<T>(enum PackType stype, T& target, int ot) : type(stype), data(target), overtime(ot) {
        payload_len = target.CalculateSize();
    }

    RStreamPack<T>(enum PackType stype, T& target) : type(stype), data(target) {
        payload_len = target.CalculateSize();
        overtime = 1000*1500;
    }

    RStreamPack<T>(T& target) : data(target) {
        type = STRING;
        payload_len = target.CalculateSize();
        overtime = 1000*1500;
    }

    inline enum PackType GetType() const {
        return type;
    }

    bool Internalize(int fd) {
        if (sizeof(int) != __read(fd, sizeof(int), (char*)&type)) {
            return false;
        }

        if (sizeof(int) != __read(fd, sizeof(int), (char*)&payload_len)) {
            return false;
        }

        if (sizeof(int) != __read(fd, sizeof(int), (char*)&overtime)) {
            return false;
        }

        char *buffer = new char[payload_len + 1];
        if (payload_len != __read(fd, payload_len, buffer)) {
            return false;
        }
        buffer[payload_len] = '\0';

        bool ret = data.Internalize(buffer, payload_len);
        delete[] buffer;
        return ret;
    }

    bool Externalize(int fd) {
        if (sizeof(enum PackType) != __write(fd, sizeof(enum PackType), (char*)&type)) {
            return false;
        }

        if (sizeof(int) != __write(fd, sizeof(int), (char*)&payload_len)) {
            return false;
        }

        if (sizeof(int) != __write(fd, sizeof(int), (char*)&overtime)) {
            return false;
        }
        return data.Externalize(fd);
    }


    int Externalize(const char *domain, int& ret, void **result, int *outlen) {
        int socket_fd;

        struct sockaddr_un srv_addr;
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(socket_fd <= 0) {
            printf("socket\n");
            return -1;
        }
        srv_addr.sun_family = AF_UNIX;

        strncpy(srv_addr.sun_path, domain, sizeof(srv_addr.sun_path) - 1);

        int ok = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok));

        if (overtime > 0) {         
            struct timeval timeout = {0, overtime};
            setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
            setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

            struct linger lger;
            lger.l_onoff = 1;
            lger.l_linger = overtime;
            setsockopt(socket_fd, SOL_SOCKET, SO_LINGER, &lger, sizeof(lger));
        }

        ret = connect(socket_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
        if(ret == -1) {
            printf("connect addr domain:%s failue errno:%d!\n", domain, errno);
            close(socket_fd);
            return -2;
        }

        bool err = Externalize(socket_fd);
        if (err) {
            int type = -1;

            if (Utils::OS::Read(socket_fd, 4, (unsigned char*)&type) != 4) {
                printf("Read\n");
                close(socket_fd);
                return -3;
            }
            if (type == 0) {
                // int
                Utils::OS::Read(socket_fd, 4, (unsigned char*)&ret);
            }
            else if (type == 1) {
                // memory
                int len = 0;
                Utils::OS::Read(socket_fd, 4, (unsigned char*)&len);
                if (len > 0) {
                    *outlen = len;
                    *result = new unsigned char[len];
                    int readed = Utils::OS::Read(socket_fd, len, (unsigned char*)(*result));
                    if (readed != len) {
                        printf("readed\n");
                        close(socket_fd);
                        return -4;
                    }
                }
            }
        }
        close(socket_fd);
        return err?0:-1;
    }

    int Externalize(const char *domain, int& ret) {
        char *data = NULL;
        int len = 0;
        int err = Externalize(domain, ret, (void**)&data, &len);
        if (data) {
            delete [] data;
        }
        return err;
    }

    int Externalize(const char *domain, std::string& ret) {
        char *data = NULL;
        int len = 0;
        int temp = 0;
        int err = Externalize(domain, temp, (void**)&data, &len);
        if (err) {
            return err;
        }
        if (data) {
            ret = std::string(data, len);
            delete [] data;
        }
        return temp;
    }

    void Internalize(const char *data, int len) {
        memcpy((char*)&type, data, 4);
        memcpy((char*)&payload_len, data+4, 4);
        this->data.Internalize(data+8, payload_len);
    }

    void Externalize(unsigned char **data, int *len) {

    }

    inline int GetOverTime() const {
        return overtime;
    }

private:
    int __read(int fd, int len, char* data) {
        int readed = 0;
        do {
            int r = read(fd, data+readed, len-readed);
            if (r <= 0) {
                printf("r=%d, __read\n", r);
                break;
            }

            readed += r;
        }while(readed<len);
        return readed;
    }

    int __write(int fd, int len, char *data) {
        int writed = 0;
        do {
            int w = write(fd, data+writed, len-writed);
            if (w < 0 ){
                printf("w < 0, write\n");
                break;
            }
            writed += w;
        }while(writed<len);
        return writed;
    }

private:
     PackType type;
     int payload_len;
     T& data;

     int overtime;
};

#endif // RSERIALIZER_H
