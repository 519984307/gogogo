#pragma once

#if (TARGET_OS == OS_WINDOWS)

#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

#define RS_URLDownloadToCacheFileA(x1,x2,x3,x4,x5,x6)	URLDownloadToCacheFileA(NULL,x2,x3,x4,x5,x6)
#define RS_URLDownloadToFileA(x1,x2,x3,x4,x5)			URLDownloadToFileA(NULL,x2,x3,x4,x5)
#define RS_DeleteFileA									DeleteFileA
#define RS_RemoveDirectoryA								RemoveDirectoryA
#define RS_CreateDirectoryA(x)							CreateDirectoryA( x, NULL )

class RXDownload{};

#else

//#include <winsock2.h>
#define RS_URLDownloadToCacheFileA						POSIX_URLDownloadToCacheFileA
#define RS_URLDownloadToFileA							POSIX_URLDownloadToFileA
#define RS_DeleteFileA									POSIX_DeleteFileA
#define RS_RemoveDirectoryA								POSIX_RemoveDirectoryA
#define RS_CreateDirectoryA								POSIX_CreateDirectoryA

#ifndef RS_DOWNLOAD_BY_CURL
#include "../lib3rd/libghttp/ghttp.h"
#pragma comment(lib, "libghttp.lib")
#else
#include "../lib3rd/libcurl/curl/curl.h"
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
	RXDownload(): curl(NULL){ curl = curl_easy_init(); }
	~RXDownload(){ if(curl) curl_easy_cleanup(curl); }
	HRESULT get( const char* url, char* filename, int length ) {
		HRESULT hr = E_FAIL;
		FILE *	fp = NULL;
		do {
			if( !url ) break;
			if( CURLE_OK != curl_easy_setopt( curl, CURLOPT_URL, url ) ) break;
			//RASSERT( CURLE_OK == curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, 1 ), E_FAIL );

			const char* lpTmpfilename = '\0' != filename[0] ? filename : "rxdownload.tmp";
			fp = fopen(lpTmpfilename,"wb");
			if( NULL == fp ) break;

			if( CURLE_OK != curl_easy_setopt( curl, CURLOPT_WRITEDATA, fp ) ) break;
			if( CURLE_OK != curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data ) ) break;
			if( CURLE_OK != curl_easy_perform( curl ) ) break;
			if(fp) fclose(fp); fp = NULL;
			if( '\0' == filename[0] ) {	strcpy( filename, lpTmpfilename ); }
			hr = S_OK;
		} while (false);
		if(fp) fclose(fp);
		return hr;
	}
};
#endif

//////////////////////////////////////////////////////////////////////////

inline HRESULT POSIX_URLDownloadToCacheFileA( RXDownload* DLFS, LPCSTR url, char* filename, DWORD bufsize, int, int ){
	if( NULL == DLFS ) return E_FAIL;
	return DLFS->get( url, filename, bufsize );
}

inline HRESULT POSIX_URLDownloadToFileA( RXDownload* DLFS, LPCSTR url, std::string file, int, int ) {
	if( NULL == DLFS ) return E_FAIL;
	std::vector<char> temp( 1024 );
#ifdef GCC_COMPATIBILITY_3_X_X
	memcpy( &temp[0], file.c_str(), file.size() );
	return DLFS->get( url, &temp[0], temp.size() );
#else
	memcpy( temp.data(), file.c_str(), file.size() );
	return DLFS->get( url, temp.data(), temp.size() );
#endif
};

inline HRESULT POSIX_DeleteFileA( const char* filename ){
	remove(filename);
	return S_OK;
}

inline HRESULT POSIX_RemoveDirectoryA( const char* filename ){
	rmdir(filename);
	return S_OK;
}

inline HRESULT POSIX_CreateDirectoryA( const char* filename ){
	mkdir( filename, 0755 );
	return S_OK;
}

#endif //(TARGET_OS == OS_WINDOWS)