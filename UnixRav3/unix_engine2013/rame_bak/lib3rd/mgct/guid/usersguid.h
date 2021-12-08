
#if !defined(_INCLUDE_USER_SGUID_H_)
#define _INCLUDE_USER_SGUID_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUserSGUID
{
public:
	BOOL Put(LPCSTR pGUID)
	{
		BOOL bRet = FALSE;
		if ( !pGUID || '\0' == pGUID[0] )
		{
			return FALSE;
		}

		HKEY hSGUIDKey = NULL;
		do 
		{
			if ( ERROR_SUCCESS != ::RegCreateKeyExA(
				HKEY_CLASSES_ROOT,
				"CLSID\\{CAA2D3B2-4BB5-4a45-A17A-122773379D99}",
				0, NULL, 0,
#ifdef _WIN64
				KEY_SET_VALUE | KEY_WOW64_32KEY,
#else
				KEY_SET_VALUE,
#endif
				NULL, &hSGUIDKey, NULL) )
			{
				break;
			}

			if ( ERROR_SUCCESS != ::RegSetValueExA(hSGUIDKey, NULL, 0, REG_SZ, (const BYTE*)pGUID, strlen(pGUID) + 1) )
			{
				break;
			}
			bRet = TRUE;
		} while ( false );

		if ( NULL != hSGUIDKey )
		{
			::RegCloseKey(hSGUIDKey);
			hSGUIDKey = NULL;
		}

		return bRet;
	}

	DWORD Get(LPSTR pGUID, DWORD nCount)
	{
		DWORD cbSize = 0;
		HKEY hSGUIDKey = NULL;
		do 
		{
			if ( ERROR_SUCCESS != ::RegOpenKeyExA(
				HKEY_CLASSES_ROOT,
				"CLSID\\{CAA2D3B2-4BB5-4a45-A17A-122773379D99}",
				0,
#ifdef _WIN64
				KEY_QUERY_VALUE | KEY_WOW64_32KEY,
#else
				KEY_QUERY_VALUE,
#endif
				&hSGUIDKey) )
			{
				break;
			}

			DWORD cbNeeded = 0;
			if ( ERROR_SUCCESS != RegQueryValueExA(hSGUIDKey, NULL, NULL, NULL, NULL, &cbNeeded) )
			{
				break;
			}
			cbSize = cbNeeded;
			if ( nCount < cbNeeded )
			{
				break;
			}

			::RegQueryValueExA(hSGUIDKey, NULL, NULL, NULL, (LPBYTE)pGUID, &cbNeeded);

		} while ( false );

		if ( NULL != hSGUIDKey )
		{
			::RegCloseKey(hSGUIDKey);
			hSGUIDKey = NULL;
		}

		return cbSize;
	};
};
#endif//_INCLUDE_USER_SGUID_H_