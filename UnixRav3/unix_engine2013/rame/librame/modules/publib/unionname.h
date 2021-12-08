#ifndef _UNION_NAME_
#define _UNION_NAME_

struct XSTR : XSTRP
{
	XSTR(LPCWSTR wStr)
	{
		IsUnicode = TRUE;
		NameW = wStr;
	}
	XSTR(LPCSTR aStr)
	{
		IsUnicode = FALSE;
		NameA = aStr;
	}
	XSTR( const PROPVARIANT* pvar )
	{
		if( !pvar ) return ;
		if( pvar->vt == VT_LPSTR )
		{
			IsUnicode = FALSE;
			NameA = pvar->pszVal;
		}
		else if( pvar->vt == VT_LPWSTR )
		{
			IsUnicode = TRUE;
			NameW = pvar->pwszVal;
		}
	}
};

typedef XSTR UnionNameR;

struct UnionName
{
	BOOL	_IsUnicode;
	union {
		LPSTR	_NameA;
		LPWSTR	_NameW;
		PVOID	_NameV;
	};
	UnionName() : _IsUnicode(0)
	{
		_NameV = 0;
	}
	~UnionName()
	{
		Clear();
	}
	operator XSTRP () const
	{
		XSTRP tmp;
		tmp.IsUnicode = _IsUnicode;
		tmp.NameV = _NameV;
		return tmp;
	}
	HRESULT Set( const UnionNameRO & uro )
	{
		Clear();
		_IsUnicode = uro.IsUnicode;
		if(_IsUnicode)
		{
			if(!uro.NameW) return E_INVALIDARG;
			_NameW = rs_wcsdup(uro.NameW);
			if(!_NameW ) return E_OUTOFMEMORY;
		}
		else
		{
			if(!uro.NameA) return E_INVALIDARG;
			_NameA = rs_strdup(uro.NameA);
			if(!_NameA ) return E_OUTOFMEMORY;
		}
		return S_OK;
	}
	VOID Clear()
	{
		if( _NameV ) free(_NameV);
		_NameV = 0;
		_IsUnicode = FALSE;
	}
	HRESULT FillIt( XSTRP & rNameRO )
	{
		if(!_NameV) return E_FAIL;
		rNameRO.IsUnicode = _IsUnicode;
		rNameRO.NameV = _NameV;
		return S_OK;
	}
};


#endif
