#ifndef _FILEPATH_H_
#define _FILEPATH_H_

//#include "re_stdlib.h"
#include "../publib/engutils.h"

#if(IS_COMPILER_VC6)
	#pragma warning (disable : 4996)
#endif

struct _CFilePathBase : STFileName
{
	_CFilePathBase()
	{ _Zero(); }
	_CFilePathBase(const CHAR *name)
	{ dwType = kAnsiType; pAName = (CHAR *)name; dwLen = name ? _Len(name) : 0; }
	_CFilePathBase(const WCHAR *name)
	{ dwType = kUnicodeType; pWName = (WCHAR *)name; dwLen = name ? _Len(name) : 0; }
	_CFilePathBase(const STFileName & name)
	{ dwType = name.dwType; pData = name.pData; dwLen = name.dwLen ? name.dwLen : _Len(name); }
	_CFilePathBase(const PROPVARIANT& var)
	{
		if(VT_LPWSTR == var.vt) { dwType = kUnicodeType; pWName = var.pwszVal; dwLen = pWName ? _Len(pWName) : 0; }
		else if(VT_LPSTR == var.vt) { dwType = kAnsiType; pAName = var.pszVal; dwLen = pAName ? _Len(pAName) : 0; }
		else { dwType = kAnsiType; pAName = 0; dwLen = 0; }
	}
	_CFilePathBase(const UnionNameRO& unn)
	{
		dwType = unn.IsUnicode ? HOSTTYPE_UNICODENAME : HOSTTYPE_ANSINAME;
		pData = (VOID*)unn.NameV;
		dwLen = _Len(unn);
	}
protected:
	enum { kAnsiType = (DWORD)HOSTTYPE_ANSINAME, kUnicodeType = (DWORD)HOSTTYPE_UNICODENAME, kInvalidType = (DWORD)-1 };
#ifdef PLATFORM_TYPE_WINDOWS
	enum { kSlash = '\\' };
#else
	enum { kSlash = '/' };
#endif 
	void _Zero() { dwType = (DWORD)kInvalidType; pData = NULL; dwLen = 0; }
	DWORD _Len(const CHAR *name) { return (DWORD)strlen(name) + 1; }
	DWORD _Len(const WCHAR *name) { return (DWORD)((wcslen(name) + 1) * sizeof(WCHAR)); }
	DWORD _Len(const STFileName &name); 
	DWORD _Len(const UnionNameRO &name); 
};

inline DWORD _CFilePathBase::_Len(const STFileName &name)
{ 
	if (!name.pData) return 0;
	switch (name.dwType) 
	{
	case kAnsiType: return _Len(name.pAName); 
	case kUnicodeType: return _Len(name.pWName); 
	default: return 0;
	} 
}
inline DWORD _CFilePathBase::_Len(const UnionNameRO &name)
{ 
	if (!name.NameV) return 0;
	if (name.IsUnicode) return _Len(name.NameW);
	else return _Len(name.NameA); 
}

struct CConstUnionName : UnionNameRO
{
	CConstUnionName( LPCSTR aName )
	{
		IsUnicode = FALSE;
		NameA = aName;
	}
	CConstUnionName( LPCWSTR wName )
	{
		IsUnicode = TRUE;
		NameW = wName;
	}
};

class CFilePath : public _CFilePathBase
{
	DWORD m_dwAlloc;
public:
	//Constructors
	CFilePath() : m_dwAlloc(0) {}
	template<class CharType> CFilePath(const CharType *name) : _CFilePathBase(name), m_dwAlloc(0) {}
	CFilePath(const STFileName& name) : _CFilePathBase(name), m_dwAlloc(0) { }
	CFilePath(const PROPVARIANT& var) : _CFilePathBase(var), m_dwAlloc(0) { }
	CFilePath(const UnionNameRO& name ) : _CFilePathBase(name), m_dwAlloc(0) { }
	// Destructor
	~CFilePath() { _Free(); };
	BOOL IsUnicode() { return dwType == kUnicodeType; }
	//
	VOID Destroy() { _Free(); }
	BOOL IsValid() const { return NULL != pData; };
	// Operator
	operator WCHAR * () { if (!ToUnicode()) { return NULL; } return pWName; }
	operator CHAR * () { if (!ToAnsi()) { return NULL; } return pAName; }
	BOOL ToUnicode() { if (kAnsiType == dwType) { return _AtoW(); } return kUnicodeType == dwType; }
	BOOL ToAnsi() { if (kUnicodeType == dwType) { return _WtoA(); } return kAnsiType == dwType; }
	// 
	CFilePath & operator = (const CHAR *pszPath);
	CFilePath & operator = (const WCHAR *pwcsPath);
	CFilePath & operator = (const STFileName &path);
	CFilePath & operator = (const CFilePath &path) { return operator = ((STFileName &)path); }
	CFilePath & operator = (const PROPVARIANT& var)
	{
		if(VT_LPWSTR == var.vt) { return *this = var.pwszVal; }
		else if(VT_LPSTR == var.vt) { return *this = var.pszVal; }
		else { return *this = (CHAR*)0; }
	}
	CFilePath & operator = (const XSTRP& var)
	{
		if(var.IsUnicode) return *this = var.NameW;
		else return *this = var.NameA;
	}
	operator XSTRP () 
	{
		XSTRP tmp;
		tmp.IsUnicode = IsUnicode();
		if( tmp.IsUnicode )
			tmp.NameW = pWName;
		else
			tmp.NameA = pAName;
		return tmp;
	}
	//
	BOOL ReplaceExt(const CHAR *pszExt, const WCHAR *pwcsExt);
	DWORD GetExtName(CHAR *pszExtName, DWORD dwBufferSize);
	DWORD GetAnsiName(CHAR *pszAnsiName, DWORD dwBufferSize);

private:
	VOID _Free() 
	{ if (m_dwAlloc) { delete [] (BYTE*)pData; pData = NULL; m_dwAlloc = 0;} }
	VOID _Alloc(DWORD dwSize) 
	{ if (dwSize) { if (NULL != (pData = RSNEW BYTE[dwSize])) { m_dwAlloc = dwSize; } } };
	BOOL _Realloc(DWORD dwSize) 
	{ if (dwSize > m_dwAlloc) { _Free(); _Alloc(dwSize); } return NULL != pData; }
	
	BOOL _AtoW();
	BOOL _WtoA();
	CHAR *_ExtPosA();
	WCHAR *_ExtPosW();
};


inline BOOL CFilePath::_AtoW()
{
	RS_CARR<CHAR> buffer(dwLen);
	if (!buffer) return FALSE;
	memcpy(buffer, pAName, dwLen);
	if (!_Realloc(dwLen * sizeof(WCHAR))) return FALSE;
	int cc = mbstowcs(pWName, buffer, dwLen);
	if( cc <= 0 ) return FALSE;
	dwLen = cc * sizeof(WCHAR);
	dwType = kUnicodeType;	
	return TRUE;
}

inline BOOL CFilePath::_WtoA()
{
	RS_CARR<WCHAR> buffer(dwLen / sizeof(WCHAR));
	if (!buffer) return FALSE;
	memcpy(buffer, pWName, dwLen);
	if (!_Realloc(dwLen)) return FALSE;
	int cc = wcstombs(pAName, buffer, dwLen);
	if( cc <= 0 ) return FALSE;
	dwLen = (DWORD)cc;
	dwType = kAnsiType;
	return TRUE;
}

inline CFilePath & CFilePath::operator = (const CHAR *pszPath)
{
	if (pszPath && pszPath != pAName)
	{
		dwLen = _Len(pszPath);
		_Realloc(dwLen);
		dwType = kAnsiType;
		memcpy(pAName, pszPath, dwLen);
	}
	return *this;
}
inline CFilePath & CFilePath::operator = (const WCHAR *pwcsPath)
{
	if (pwcsPath && pwcsPath != pWName)
	{
		dwLen = _Len(pwcsPath);
		_Realloc(dwLen);
		dwType = kUnicodeType;
		memcpy(pWName, pwcsPath, dwLen);
	}
	return *this;
}
inline CFilePath & CFilePath::operator = (const STFileName &path)
{
	if (path.pData && path.pData != pData)
	{
		dwLen = _Len(path);
		_Realloc(dwLen);
		dwType = path.dwType;
		memcpy(pData, path.pData, dwLen);
	}
	return *this;
}

inline BOOL CFilePath::ReplaceExt(const CHAR *pszExt, const WCHAR *pwcsExt)
{
	if (kAnsiType == dwType) 
	{
		CHAR *psz = _ExtPosA();
		if (!psz) return FALSE;
		// ASSERT(strlen(pszExt) == strlen(psz));
		rs_strcpy(psz, pszExt);
	}
	else if (kUnicodeType == dwType) 
	{
		WCHAR *pwcs = _ExtPosW();
		if (!pwcs) return FALSE;
		// ASSERT(wcslen(pwcsExt) == wcslen(pwcs));
		rs_wcscpy(pwcs, pwcsExt);
	}
	return TRUE;
}

inline DWORD CFilePath::GetExtName(CHAR *pszExtName, DWORD dwBufferSize)
{
	size_t len = 0;
	if (kAnsiType == dwType)
	{
		CHAR *pszExt = _ExtPosA();
		if (!pszExt) return 0;
		len = _Len(pszExt);
		if (len <= dwBufferSize) { memcpy(pszExtName, pszExt, len); }
	}
	else if (kUnicodeType == dwType)
	{
		WCHAR *pwcsExt = _ExtPosW();
		if (!pwcsExt) return 0;
		len = _Len(pwcsExt);
		if (len <= dwBufferSize) { len = wcstombs(pszExtName, pwcsExt, len); }
	}
	return (DWORD)len;
}

inline DWORD CFilePath::GetAnsiName(CHAR *pszAnsiName, DWORD dwBufferSize)
{
	size_t len = 0;
	if (kAnsiType == dwType)
	{
		CHAR *psz = strrchr(pAName, kSlash);
		if (!psz) psz = pAName;
		else psz++;
		len = _Len(psz);
		if (len <= dwBufferSize) { memcpy(pszAnsiName, psz, len); }
	}
	else if (kUnicodeType == dwType)
	{
		WCHAR *pwcs = wcsrchr(pWName, kSlash);
		if (!pwcs) pwcs = pWName;
		else pwcs++;
		len = _Len(pwcs);
		if (len <= dwBufferSize) { len = wcstombs(pszAnsiName, pwcs, len); }
	}
	return (DWORD)len;
}
inline CHAR * CFilePath::_ExtPosA()
{
	CHAR *pd = strrchr(pAName, '.');
	if (!pd) return NULL;
	CHAR *ps = strrchr(pAName, kSlash);
	if (ps && pd < ps) return NULL;
	return pd + 1;
}
inline WCHAR * CFilePath::_ExtPosW()
{
	WCHAR *pd = wcsrchr(pWName, L'.');
	if (!pd) return NULL;
	WCHAR *ps = wcsrchr(pWName, kSlash);
	if (ps && pd < ps) return NULL;
	return pd + 1;
}



#endif	//#ifndef _FILEPATH_H_
