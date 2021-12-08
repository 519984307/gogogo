#ifndef _RS_CTYPE_H_
#define _RS_CTYPE_H_


#include <ctype.h>
#include <wchar.h>

#if defined(PLATFORM_TYPE_WINDOWS)
//
#define rs_tolower		tolower
#define rs_towlower		towlower
#define rs_toolower		towlower
#define rs_toupper		toupper
#define rs_towupper		towupper
#define rs_tooupper		towupper

#else
//
inline int rs_tolower(int c)
{ 
	return ('A' <= (char)c && (char)c <= 'Z') ? c + ('a' - 'A') : c;
}

inline int rs_toupper(int c)
{
	return ('a' <= (char)c && (char)c <= 'z') ? c + ('A' - 'a') : c;
}

inline int rs_towlower(int c)
{ 
	return (L'A' <= (WCHAR)c && (WCHAR)c <= L'Z') ? c + (L'a' - L'A') : c;
}

inline int rs_towupper(int c)
{
	return (L'a' <= (WCHAR)c && (WCHAR)c <= L'z') ? c + (L'A' - L'a') : c;
}

//
inline int rs_toolower(int c)
{ 
	return (OLECH('A') <= (OLECHAR)c && (OLECHAR)c <= OLECH('Z')) ? c + (OLECH('a') - OLECH('A')) : c;
}

inline int rs_tooupper(int c)
{
	return (OLECH('a') <= (OLECHAR)c && (OLECHAR)c <= OLECH('z')) ? c + (OLECH('A') - OLECH('a')) : c;
}

#endif



#endif // _RS_CTYPE_H_
