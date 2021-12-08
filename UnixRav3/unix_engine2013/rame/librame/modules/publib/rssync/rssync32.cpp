#include "stdafx.h"
#if defined(PLATFORM_TYPE_WINDOWS) && !defined(PLATFORM_TYPE_NATIVE)


#include "./rssync32.h"




CSecurityACL CSecurityACL::sm_Instance;

CSecurityACL::CSecurityACL()
{
	InitSecurityDACL();
}

CSecurityACL* CSecurityACL::Instance()
{
	return &sm_Instance;
}

// to share kernel object in NT and above
// i must init security privileges
// snatch code from RSEngine
// Jim Cai @ 08/11/2003
BOOL CSecurityACL::InitSecurityDACL()
{
	typedef BOOL(_stdcall * INITSECUDES)(PSECURITY_DESCRIPTOR, DWORD );
	typedef BOOL(_stdcall * SETSECUDESDACL)
		(PSECURITY_DESCRIPTOR,	BOOL, PACL,BOOL );

	INITSECUDES pInitializeSecurityDescriptor = NULL;
	SETSECUDESDACL pSetSecurityDescriptorDacl = NULL;

	HMODULE hApi32 = LoadLibrary("advapi32.dll");
	if (hApi32)
	{
		pInitializeSecurityDescriptor =(INITSECUDES)GetProcAddress(hApi32, "InitializeSecurityDescriptor");
		pSetSecurityDescriptorDacl =(SETSECUDESDACL)GetProcAddress(hApi32, "SetSecurityDescriptorDacl");
	}

	if (pInitializeSecurityDescriptor)
		pInitializeSecurityDescriptor(&m_stSecurityDesc, SECURITY_DESCRIPTOR_REVISION);

	if (pSetSecurityDescriptorDacl)
		pSetSecurityDescriptorDacl(&m_stSecurityDesc, TRUE,(PACL)NULL, FALSE);

	if (pSetSecurityDescriptorDacl)
		m_stSecurityAttr.lpSecurityDescriptor =&m_stSecurityDesc;
	else
		m_stSecurityAttr.lpSecurityDescriptor = NULL;

	m_stSecurityAttr.nLength = sizeof(m_stSecurityAttr);
	m_stSecurityAttr.bInheritHandle = TRUE;
	FreeLibrary(hApi32);

	return TRUE;
}


#endif


