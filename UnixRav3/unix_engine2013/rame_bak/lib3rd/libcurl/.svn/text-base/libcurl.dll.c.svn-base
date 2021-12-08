#include <curl/curl.h>

#ifdef _WIN32
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	if(DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		CURLcode r = curl_global_init(CURL_GLOBAL_ALL);
		if( r != CURLE_OK ) return FALSE;
	}
	else if(DLL_PROCESS_DETACH == ul_reason_for_call)
	{
		curl_global_cleanup();
	}
	return TRUE;
}
#endif
