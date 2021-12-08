#ifndef REG_UTILS_H_
#define REG_UTILS_H_


#ifndef ASSERT
#define ASSERT(x) { if (!(x)) { DebugBreak(); } }
#endif

class CRegAccessor
{
	IRERegistry *m_preg;
public:
	CRegAccessor(IRERegistry * p) : m_preg(p) {}
	operator IRERegistry * () const { return m_preg; }
	IRERegistry * operator -> () { return m_preg; }

	//////////////////////////////////////////////////////////////////////////
	// IRERegistry Extend:
	HRESULT RegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phResult)
	{
		return m_preg->RegCreateKeyExA(hKey, lpSubKey, 0, NULL, 0, MAXIMUM_ALLOWED, NULL, 
			phResult, NULL);
	}
	HRESULT RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
	{
		return m_preg->RegOpenKeyExA(hKey, lpSubKey, 0, MAXIMUM_ALLOWED, 
			phkResult);
	}
	HRESULT RegCreateKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phResult)
	{
		return m_preg->RegCreateKeyExW(hKey, lpSubKey, 0, NULL, 0, MAXIMUM_ALLOWED, NULL, 
			phResult, NULL);
	}
	HRESULT RegOpenKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
	{
		return m_preg->RegOpenKeyExW(hKey, lpSubKey, 0, MAXIMUM_ALLOWED, 
			phkResult);
	}

};

#endif