#include "httprequest.h"

#include <curl/curl.h>

#include <iostream>

static int using_tls_version = -1;


static inline void get_curl_conninfo(CURL * curl, std::string & conn)
{
	char * ip = 0;
	curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &ip);
	if(ip) conn = ip;
}

size_t write_data_body(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )  {
        return -1;
    }

    string data;
    data.assign((char*)buffer,((char*)buffer) + size*nmemb);

    *str +=data;
    return nmemb;
}

size_t write_data_header( void *ptr, size_t size, size_t nmemb, void *stream )
{
    int len = size * nmemb;

    string tmp;
    std::string* res_head = dynamic_cast<std::string*>((std::string *)stream);
    if ((((char*)ptr)[0] == 'i') && (((char*)ptr)[1] == ':')) {
        (*res_head)= (char*)ptr + 3;
    }

    int written = len;
    return written;
}

int http_get(const string& str_url, const string& str_head, string &resp_head, string &resp_body, string &conn) {
    CURLcode res;

    //printf("http_get:url=%s, head:%s\n", str_url.c_str(), str_head.c_str());

    curl_slist *plist = NULL;
    plist = curl_slist_append(plist, str_head.c_str());

    CURL* curl = curl_easy_init();
    if(NULL == curl)  {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
    curl_easy_setopt( curl, CURLOPT_HEADERFUNCTION, write_data_header );
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&resp_head);
    curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());

    if (str_url.find("https") == 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp_body);

    int err = curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, "ALL");

    if (using_tls_version >= 0) {
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, using_tls_version);
        res = curl_easy_perform(curl);
    }
    else {
//        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_0);
        res = curl_easy_perform(curl);
//        if (res == CURLE_SSL_CONNECT_ERROR) {
//            for (int version=CURL_SSLVERSION_DEFAULT; version<CURL_SSLVERSION_LAST; version++) {
//                curl_easy_setopt(curl, CURLOPT_SSLVERSION, version);
//                res = curl_easy_perform(curl);
//                if (res == 0) {
//                    using_tls_version = version;
//                }
//            }
//        }
    }

    if(res == 0) get_curl_conninfo(curl, conn);
    curl_slist_free_all(plist);
    curl_easy_cleanup(curl);
    return res;
}

int http_redirect(const string& url, string &redirect) {
    CURLcode res;

    CURL* curl = curl_easy_init();
    if(NULL == curl)  {
        return CURLE_FAILED_INIT;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (url.find("https") == 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    res = curl_easy_perform(curl);

    char *ret = NULL;
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &ret);
    if (ret) {
        redirect = string(ret);
    }
    curl_easy_cleanup(curl);
    return res;
}

int http_post(const string& str_url, const string& str_head, const string& str_body, string &resp_head, string &resp_body, string &conn)
{
    CURLcode res;
    curl_slist *plist = NULL;
    plist = curl_slist_append(plist, str_head.c_str());

    CURL* curl = curl_easy_init();
    if(NULL == curl)  {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_data_header );
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&resp_head);
    curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());

    curl_easy_setopt(curl, CURLOPT_POST, 1);
    if (str_url.find("https") == 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, str_body.size() );
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp_body);

    if (using_tls_version >= 0) {
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, using_tls_version);
        res = curl_easy_perform(curl);
    }
    else {
        res = curl_easy_perform(curl);
        if (res == CURLE_SSL_CONNECT_ERROR) {
            for (int version=CURL_SSLVERSION_DEFAULT; version<CURL_SSLVERSION_LAST; version++) {
                curl_easy_setopt(curl, CURLOPT_SSLVERSION, version);
                res = curl_easy_perform(curl);
                if (res == 0) {
                    using_tls_version = version;
                }
            }
        }
    }

    if(res == 0) get_curl_conninfo(curl, conn);
    curl_slist_free_all(plist);
    curl_easy_cleanup(curl);
    return res;
}
