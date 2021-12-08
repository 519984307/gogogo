#ifndef __X_STRING_OP__
#define __X_STRING_OP__

template < class XChar >
size_t xstrlen( const XChar * p )
{
	const XChar* px = p;
	for( ; *px ; ++ px );
	return px - p;
}

template < class XChar >
void xstrlwr( XChar * p )
{
	for( ; *p; ++ p )
		*p = ::tolower(*p);
}

template < class XChar >
void xstrupr( XChar * p )
{
	for( ; *p; ++ p )
		*p = ::toupper(*p);
}

template < class XChar >
int xstrcmp( XChar * p1,  XChar* p2 )
{
	for( ; ++ p1, ++p2 )
	{
		long diff = *p1 - *p2;
		if( diff ) return diff;
		if( !*p1 ) return 0;
	}
}

template < class XChar >
int xstricmp( XChar * p1,  XChar* p2 )
{
	for( ; ++ p1, ++p2 )
	{
		long diff = *p1 - *p2;
		if( diff ) return diff;
		if( !*p1 ) return 0;
	}
}


LPVOID utf16_to_wcs( LPCVOID utf16 )
{

}

wchar_t* mbs_to_wcs( char * mbs, bool freeit = true )
{
	size_t l = xstrlen(mbs);
	int ch = ::mbstowcs( NULL, mbs, l);
	wchar_t * ws = new wchar_t[ch+1];
	if( !ws ) return false;
	::mbstowcs( ws, mbs, l );
	delete mbs;
	return ws;
}

char* wcs_to_mbs( const wchar_t * wcs, bool freeit = true )
{
	size_t l = xstrlen(wcs);
	int ch = ::wcstombs( NULL, wcs, l);
	wchar_t * as = new char[ch+1];
	if( !ws ) return false;
	::wcstombs( as, wcs, l );
	delete wcs;
	return as;
}

#endif