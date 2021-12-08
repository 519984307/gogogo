#ifndef CLOUDNETPROXY_H
#define CLOUDNETPROXY_H

#include <string>
#include <map>

using namespace std;

class CloudNetProxy {
public:
    virtual int HttpGet(const char *url, const char *params, string& out) = 0;
    virtual int HttpPost(const char *url, const char *params, string& out) = 0;
};

#endif // CLOUDNETPROXY_H
