#include "httpdownloader.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

using namespace std;

static size_t write_datas(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    string data((const char*) ptr, (size_t) size * nmemb);
    *((stringstream*) stream) << data << endl;
    return size * nmemb;
}

static size_t write_dataf(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


HTTPDownloader::HTTPDownloader() 
{
    CURLcode ret;
    ret = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != ret) {
        printf("init libcurl fail.");
        curl_global_cleanup();
        _exit(-1);
    }
}

HTTPDownloader::~HTTPDownloader() 
{
    curl_global_cleanup();
}


string HTTPDownloader::Download(const std::string& url) 
{
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); 
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
    std::stringstream out;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_datas);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    /* Perform the request, res will get the return code */
    CURLcode ret = curl_easy_perform(curl);
    /* Check for errors */
    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    }
    curl_easy_cleanup(curl);
    return out.str();
}

int HTTPDownloader::DownloadFile(const char* url, const char* outfilename)
{
    CURL *curl = NULL;
    FILE *fp = NULL;
    curl = curl_easy_init();    
    if (curl) {
        fp = fopen(outfilename,"wb"); 
        if (fp == NULL) {
            curl_easy_cleanup(curl);
            return -1;
        }
        CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, url);                  
        if (res != CURLE_OK)
        {   
            fclose(fp);
            curl_easy_cleanup(curl);  
            return -1;
        }
        res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        if (res != CURLE_OK)
        {   
            fclose(fp);
            curl_easy_cleanup(curl);  
            return -1;
        }
        res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        if (res != CURLE_OK)
        {   
            fclose(fp);
            curl_easy_cleanup(curl);  
            return -1;
        }
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_dataf);  
        if (res != CURLE_OK)
        {   
            fclose(fp);
            curl_easy_cleanup(curl);  
            return -1;
        }
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if (res != CURLE_OK)
        {   
            fclose(fp); 
            curl_easy_cleanup(curl);  
            return -1;
        }
 
        res = curl_easy_perform(curl);                               
        fclose(fp); 
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            curl_easy_cleanup(curl);  
            return -1;
        }
            
        curl_easy_cleanup(curl);                                     
    }
    return 0;
}

