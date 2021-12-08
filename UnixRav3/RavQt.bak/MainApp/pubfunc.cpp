#include <stdlib.h>
#include "common.h"
#include "pubfunc.h"


PubFunc::PubFunc()
{

}

bool PubFunc::IsValidIP(const char *sIP)
{
    if(sIP == nullptr)
        return false;
    if (inet_addr(sIP) == INADDR_NONE) {
       return false;
    }
    return true;
}


bool PubFunc::IsValidIpPort(const char *ipport)
{
    string sdata = ipport;
    string ip, port;
    int nport;
    if(ipport == nullptr)
        return false;
    //printf("xxxxxxxxxxxx PubFunc::IsValidIpPort ipport=%s", ipport);
    if (sdata.find(":") == string::npos) {
        if (inet_addr(sdata.c_str()) == INADDR_NONE) {
           return false;
        }
        return true;
    }
    if(SplitIpString(sdata, ip, port) != 0)
        return false;

    if(ip != "0.0.0.0"){
        if (inet_addr(ip.c_str()) == INADDR_NONE) {
           return false;
        }
    }
    nport = atoi(port.c_str());
    if(nport < 0 || nport > 65535)
        return false;
    return true;
}

int PubFunc::SplitIpString(const string sdata, string &ip, string &port)
{
    size_t len = sdata.length();
    if(len < 7)
        return -1;
    size_t pos1 = sdata.find(':');
    if(pos1 == string::npos) {//ip a.b.c.d
        ip = sdata;
        port = "";
    }else{// ip:port
        size_t pos2 = sdata.find("0.0.0.0");
        if(pos2 == string::npos){//a.b.c.d:port
            ip = sdata.substr(0, pos1);
            port = sdata.substr(pos1+1, len-pos1);
        }else{//0.0.0.0:port
            ip = "0.0.0.0";
            port = sdata.substr(pos1+1, len-pos1);
        }
    }
    return 0;
}
