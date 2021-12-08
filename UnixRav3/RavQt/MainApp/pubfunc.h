#ifndef PUBFUNC_H
#define PUBFUNC_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "common.h"

using namespace std;

class PubFunc
{
public:
    PubFunc();

    static bool IsValidIP(const char* sIP);
    static bool IsValidIpPort(const char *data);//ip:port  ip
    static int SplitIpString(const string sdata, string &ip, string &port);
};

#endif // PUBFUNC_H
