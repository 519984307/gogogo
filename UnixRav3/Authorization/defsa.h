#ifndef __DEFSA_H__
#define __DEFSA_H__

#ifndef UNIXPLATFORM
#ifndef _WINDOWS_
#include <windows.h>
#endif // !_WINDOWS_
#endif

class CDefaultSecurityAttribtes
{
	SECURITY_ATTRIBUTES sa;

public:
	CDefaultSecurityAttribtes()
	{
		isValid = InitializeSecurityAttributes(&sa);
	}

	operator LPSECURITY_ATTRIBUTES()
	{
		return isValid ? &sa : NULL;
	}

public:
	BOOL isValid;

	static
	BOOL InitializeSecurityAttributes(LPSECURITY_ATTRIBUTES psa)
	{
		if(psa == NULL || IsBadWritePtr(psa, sizeof(SECURITY_ATTRIBUTES)))
			return FALSE;

		static BOOL bInitialized = FALSE;
		static SECURITY_ATTRIBUTES saMold;
		static SECURITY_DESCRIPTOR sdMold;

		typedef BOOL(WINAPI * INITSECUDES)(PSECURITY_DESCRIPTOR, DWORD);
		typedef BOOL(WINAPI * SETSECUDESDACL)(PSECURITY_DESCRIPTOR, BOOL, PACL, BOOL);

		if(!bInitialized)
		{
			INITSECUDES pInitializeSecurityDescriptor = NULL;
			SETSECUDESDACL pSetSecurityDescriptorDacl = NULL;

			HMODULE hApi32 = LoadLibrary("advapi32.dll");
			if(!hApi32)
				return FALSE;

			pInitializeSecurityDescriptor = (INITSECUDES)GetProcAddress(hApi32, "InitializeSecurityDescriptor");
			pSetSecurityDescriptorDacl = (SETSECUDESDACL)GetProcAddress(hApi32, "SetSecurityDescriptorDacl");

			if(pInitializeSecurityDescriptor)
				pInitializeSecurityDescriptor(&sdMold, SECURITY_DESCRIPTOR_REVISION);

			if(pSetSecurityDescriptorDacl)
			{
				pSetSecurityDescriptorDacl(&sdMold, TRUE, NULL, FALSE);
				saMold.lpSecurityDescriptor = &sdMold;
			}
			else // Windows 9x series
			{
				saMold.lpSecurityDescriptor = NULL;
			}

			saMold.nLength = sizeof(SECURITY_ATTRIBUTES);
			saMold.bInheritHandle = TRUE;

			FreeLibrary(hApi32);
			hApi32 = 0;

			bInitialized = TRUE;
		}

		CopyMemory(psa, &saMold, sizeof(SECURITY_ATTRIBUTES));

		return TRUE;
	}
};

#endif // __DEFSA_H__
