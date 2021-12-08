#ifndef HTTPDOWNLOADER_HPP
#define HTTPDOWNLOADER_HPP
#include <string>



class HTTPDownloader 
{
public:
    HTTPDownloader();
    ~HTTPDownloader();
    /**
     * Download a file using HTTP GET and store in in a std::string
     * @param url The URL to download
     * @return The download result
     */
    std::string Download(const std::string& url);

    int DownloadFile(const char* url, const char* outfilename);

};
#endif  /* HTTPDOWNLOADER_HPP */
