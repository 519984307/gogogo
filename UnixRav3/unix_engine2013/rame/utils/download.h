#pragma once


#ifndef RS_DOWNLOAD_BY_CURL
#define RS_DOWNLOAD_BY_CURL
#endif

#ifndef RS_DOWNLOAD_BY_CURL

#include "../lib3rd/libghttp/ghttp.h"
#pragma comment(lib, "libghttp.lib")

#else

#define BUILDING_LIBCURL
#define HTTP_ONLY
#include "../lib3rd/libcurl/curl/curl.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "libcurl.lib")

#endif


#ifndef RS_DOWNLOAD_BY_CURL
class RXDownload
{
private:
	ghttp_request *request;

public:
	RXDownload(): request(NULL){ request = ghttp_request_new(); }
	~RXDownload(){ if(request) ghttp_request_destroy(request); }
	HRESULT get( const char* url, char* filename, int length ) {
		HRESULT hr = E_FAIL;
		FILE * fp = NULL;
		do {
			if( !request ) break;
			if( ghttp_set_uri(request, (char*)url) == ghttp_error ) break;
			if( ghttp_set_type(request, ghttp_type_get) == ghttp_error ) break;

			ghttp_set_header(request, "Content-Type", "application/x-www-form-urlencoded");  
			if( ghttp_prepare(request) == ghttp_error ) break;
			if( ghttp_process(request) != ghttp_done ) break;

			const char* lpTmpfilename = '\0' != filename[0] ? filename : "rxdownload.tmp";
			char* body = ghttp_get_body( request );
			int body_len = ghttp_get_body_len( request );
			if( body && body_len )
			{
				if( 0 == memicmp( body, "error", 5 ) ) break;
				fp = fopen(lpTmpfilename,"wb");
				if( NULL == fp ) break;
				fwrite(body, 1, body_len, fp);
			}
			if( '\0' == filename[0] ) {	strcpy( filename, lpTmpfilename ); }
			hr = S_OK;
		} while (false);
		if(fp) fclose(fp);
		ghttp_clean(request);
		return hr;
	}
};
#else
static size_t write_data( void *ptr, size_t size, size_t nmemb, void *stream )
{
	return fwrite( ptr, size, nmemb, (FILE*)stream );
}

class RXDownload
{
private:
	CURL* curl;

public:
	RXDownload(): curl(NULL){
		curl = curl_easy_init();
	}
	~RXDownload(){
		if(curl) curl_easy_cleanup(curl);
	}
	long get_file_lenth(const char *url){
		long length = 0;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
		if (curl_easy_perform(curl) == CURLE_OK) {
			curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
		} else {
			length = -1;
		}
		return length;
	}
	HRESULT get( const char* url, char* filename, int length ) {
		HRESULT hr = E_FAIL;
		FILE *	fp = NULL;
		do {
			if( !url ) break;
			CURLcode lc = curl_easy_setopt( curl, CURLOPT_URL, url );
			if( CURLE_OK != lc ) break;

			lc = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			if( CURLE_OK != lc ) break;

			lc = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			if( CURLE_OK != lc ) break;

			//lc = curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, 1 );
			//RASSERT( CURLE_OK == lc, E_FAIL );

			const char* lpTmpfilename = '\0' != filename[0] ? filename : "rxdownload.tmp";
			fp = fopen(lpTmpfilename,"wb");
			if( NULL == fp ) break;

			lc = curl_easy_setopt( curl, CURLOPT_WRITEDATA, fp );
			if( CURLE_OK != lc ) break;

			lc = curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
			if( CURLE_OK != lc ) break;

			lc = curl_easy_perform( curl );
			if( CURLE_OK != lc ) break;

			if(fp) fclose(fp); fp = NULL;
			if( '\0' == filename[0] ) {	strcpy( filename, lpTmpfilename ); }
			hr = S_OK;
		} while (false);
		if(fp) fclose(fp);
		return hr;
	}
};
#endif



#ifndef _WIN32

#define DeleteFileA					RS_DeleteFileA
#define RemoveDirectoryA			RS_RemoveDirectoryA
#define CreateDirectoryA			RS_CreateDirectoryA

inline HRESULT RS_DeleteFileA( const char* filename ){
	remove(filename);
	return S_OK;
}

inline HRESULT RS_RemoveDirectoryA( const char* filename ){
	rmdir(filename);
	return S_OK;
}

inline HRESULT RS_CreateDirectoryA( const char* filename, long ){
	mkdir( filename, 0755 );
	return S_OK;
}

#endif

inline HRESULT RS_URLDownloadToCacheFileA( RXDownload* DLFS, LPCSTR url, char* filename, DWORD bufsize, int, int ) {
	if( NULL == DLFS ) return E_FAIL;
	return DLFS->get( url, filename, bufsize );
}

inline HRESULT RS_URLDownloadToFileA( RXDownload* DLFS, LPCSTR url, std::string file, int, int ) {
	if( NULL == DLFS ) return E_FAIL;
	std::vector<char> temp( 1024 );
	memcpy( &temp[0], file.c_str(), file.size() );
	return DLFS->get( url, &temp[0], temp.size() );
};

