#ifndef _VC6_TEMPLATE_FIXUP_H_
#define _VC6_TEMPLATE_FIXUP_H_



_STD_BEGIN



template<class T>
struct __raw_pointer_traits
{
	typedef	random_access_iterator_tag	iterator_category;
	typedef							 T	value_type;
	typedef					 ptrdiff_t	distance_type;
};


template<> struct iterator_traits<char*> : __raw_pointer_traits<char> {};
template<> struct iterator_traits<const char*> : __raw_pointer_traits<char> {};


template<> struct iterator_traits<signed char*> : __raw_pointer_traits<signed char> {};
template<> struct iterator_traits<const signed char*> : __raw_pointer_traits<signed char> {};


template<> struct iterator_traits<unsigned char*> : __raw_pointer_traits<unsigned char> {};
template<> struct iterator_traits<const unsigned char*> : __raw_pointer_traits<unsigned char> {};

template<> struct iterator_traits<wchar_t*> : __raw_pointer_traits<wchar_t> {};
template<> struct iterator_traits<const wchar_t*> : __raw_pointer_traits<wchar_t> {};



_STD_END



#endif // duplicate inclusion protection
