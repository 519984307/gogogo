#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>

using namespace std;

extern "C" {
    int http_get(const string& str_url, const string& str_head, string &resp_head, string &resp_body, string &conn);
    int http_redirect(const string& url, string &redirect);
    int http_post(const string& str_url, const string& str_head, const string& str_body, string &resp_head, string &resp_body, string &conn);
}

#endif // HTTPREQUEST_H
