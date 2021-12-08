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
