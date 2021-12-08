#ifndef _RS_ENGINE_STREAM_MATCHING_H_
#define _RS_ENGINE_STREAM_MATCHING_H_

#include <iterator>

#if(defined(_MSC_VER) && _MSC_VER < 1300 || defined(PLATFORM_TYPE_NATIVE))
	#include "vc6tfix.h" // fixup iterator_traits<> for raw pointer
#endif



// type define:
template<class T>
struct __stream_traits_base
{
	typedef T	char_type;
};


// for non wildcard:
template<class T>
struct __stream_traits_non_wildcard
{
	static bool is_asterisk(const T&) { return false; }
	static bool is_question(const T&) { return false; }
	static bool is_transfer(const T&) { return false; }
};


// for wildcard:
template<class T>
struct __stream_traits_wildcard {};

template<>
struct __stream_traits_wildcard<char>
{
	static bool is_asterisk(char ch) { return '*' == ch; }  // 0x2A
	static bool is_question(char ch) { return '?' == ch; }  // 0x3F
	static bool is_transfer(char ch) { return '\\' == ch; } // 0x5C
};

template<>
struct __stream_traits_wildcard<wchar_t>
{
	static bool is_asterisk(wchar_t ch) { return L'*' == ch; }  // 0x2A
	static bool is_question(wchar_t ch) { return L'?' == ch; }  // 0x3F
	static bool is_transfer(wchar_t ch) { return L'\\' == ch; } // 0x5C
};

template<>
struct __stream_traits_wildcard<unsigned char> // BYTE
{
	static bool is_asterisk(unsigned char ch) { return '*' == ch; }  // 0x2A
	static bool is_question(unsigned char ch) { return '?' == ch; }  // 0x3F
	static bool is_transfer(unsigned char ch) { return '\\' == ch; } // 0x5C
};


// for case sensitive:
template<class T>
struct __stream_traits_non_upper
{
	static T toupper(const T& ch) { return ch; }
};


// for case non sensitive:
template<class T>
struct __stream_traits_upper {};

template<>
struct __stream_traits_upper<char>
{
	static char toupper(char ch) { return (char)::toupper(ch); }
};

template<>
struct __stream_traits_upper<wchar_t>
{
	static wchar_t toupper(wchar_t ch) { return (wchar_t)::toupper(ch); }
};

template<>
struct __stream_traits_upper<unsigned char> // BYTE
{
	static unsigned char toupper(unsigned char ch) { return (unsigned char)::toupper(ch); }
};


// composite:
template<class T>
struct __stream_traits_non_wildcard_case_non_sensitive :	__stream_traits_base<T>,
															__stream_traits_non_wildcard<T>,
															__stream_traits_upper<T>
{};

template<class T>
struct __stream_traits_non_wildcard_case_sensitive :	__stream_traits_base<T>,
														__stream_traits_non_wildcard<T>,
														__stream_traits_non_upper<T>
{};

template<class T>
struct __stream_traits_wildcard_case_non_sensitive :	__stream_traits_base<T>,
														__stream_traits_wildcard<T>,
														__stream_traits_upper<T>
{};

template<class T>
struct __stream_traits_wildcard_case_sensitive :	__stream_traits_base<T>,
													__stream_traits_wildcard<T>,
													__stream_traits_non_upper<T>
{};



template<class Traits, class ItLookin, class ItFindWhat>
ItLookin
__stmstm_t(ItLookin lookin_begin, ItLookin lookin_end, ItFindWhat findwhat_begin, ItFindWhat findwhat_end,
		   OUT size_t *pmatching_size = 0, OUT size_t *pmin_matching_size = 0, OUT bool *pis_asterisk_remain = 0)
{
	using namespace std;
	// adjust findwhat stream:
	for(; findwhat_begin != findwhat_end && Traits::is_asterisk(*findwhat_begin); ++findwhat_begin);
	if( findwhat_begin == findwhat_end ||
		(Traits::is_transfer(*findwhat_begin) && std::distance(findwhat_begin, findwhat_end) == 1) )
	{
		if(pmatching_size) { *pmatching_size = 0; }
		if(pmin_matching_size) { *pmin_matching_size = 0; }
		if(pis_asterisk_remain) { *pis_asterisk_remain = true; }
		return lookin_begin;
	}
	ItFindWhat findwhat = findwhat_begin;
	// matching first:
	if(!Traits::is_question(*findwhat))
	{
		if(Traits::is_transfer(*findwhat)) { ++findwhat; }
		typename Traits::char_type ch = Traits::toupper(*findwhat);
		for(; lookin_begin != lookin_end && Traits::toupper(*lookin_begin) != ch; ++lookin_begin);
		if(lookin_begin == lookin_end) { return lookin_end; }
	}
	ItLookin lookin = lookin_begin;
	// matching last and counting:
	size_t min_matching_size = 0;
	while(++lookin, ++findwhat, ++min_matching_size,
		  lookin != lookin_end && findwhat != findwhat_end && !Traits::is_asterisk(*findwhat))
	{
		if(Traits::is_question(*findwhat)) { continue; }
		if(Traits::is_transfer(*findwhat)) { if(++findwhat == findwhat_end) { break; } }
		typename Traits::char_type ch = Traits::toupper(*findwhat);
		for(; lookin != lookin_end && Traits::toupper(*lookin) != ch; ++lookin);
		if(lookin == lookin_end) { return lookin_end; }
	}
	if(lookin == lookin_end)
	{
		if(findwhat != findwhat_end)
		{
			if(Traits::is_transfer(*findwhat))
			{
				if(std::distance(findwhat, findwhat_end) != 1) { return lookin_end; }
			}
			else if(!Traits::is_asterisk(*findwhat))
			{
				return lookin_end;
			}
		}
// 		if((size_t)std::distance(lookin_begin, lookin) != min_matching_size) { return lookin_end; }
	}
	ItFindWhat findwhat_mid = findwhat;
	// matching loop:
	while((size_t)std::distance(lookin_begin, lookin) != min_matching_size)
	{
		// adjust streams:
		std::advance(lookin, 0 -min_matching_size);
		findwhat = findwhat_begin;
		// lookin stream matching first:
		if(!Traits::is_question(*findwhat))
		{
			if(Traits::is_transfer(*findwhat)) { ++findwhat; }
			typename Traits::char_type ch = Traits::toupper(*findwhat);
			for(; lookin != lookin_end && Traits::toupper(*lookin) != ch; ++lookin);
			if(lookin == lookin_end) { return lookin_end; }
		}
		lookin_begin = lookin;
		// lookin stream matching last:
		while(++lookin, ++findwhat != findwhat_mid)
		{
			if(lookin == lookin_end) { return lookin_end; }
			if(Traits::is_question(*findwhat)) { continue; }
			if(Traits::is_transfer(*findwhat)) { if(++findwhat == findwhat_mid) { break; } }
			typename Traits::char_type ch = Traits::toupper(*findwhat);
			for(; lookin != lookin_end && Traits::toupper(*lookin) != ch; ++lookin);
			if(lookin == lookin_end) { return lookin_end; }
		}
	}
	//
	if(lookin != lookin_end && findwhat != findwhat_end && Traits::is_asterisk(*findwhat))
	{
		if(lookin_end == (lookin = __stmstm_t<Traits>(lookin, lookin_end, findwhat, findwhat_end,
													  pmatching_size, pmin_matching_size, pis_asterisk_remain)))
		{
			return lookin_end;
		}
		if(pmatching_size) { *pmatching_size += lookin -lookin_begin; }
		if(pmin_matching_size) { *pmin_matching_size += min_matching_size; }
	}
	else
	{
		if(pmatching_size) { *pmatching_size = lookin -lookin_begin; }
		if(pmin_matching_size) { *pmin_matching_size = min_matching_size; }
		if(pis_asterisk_remain) { *pis_asterisk_remain = findwhat != findwhat_end && Traits::is_asterisk(*findwhat); }
	}
	return lookin_begin;
}


// shortcut:
template<class ItLookin, class ItFindWhat>
FORCEINLINE  ItLookin
StmStm(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end,
	   OUT size_t *pmatching_size = 0, OUT size_t *pmin_matching_size = 0, OUT bool *pis_asterisk_remain = 0)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type		value_type;
	typedef	__stream_traits_non_wildcard_case_sensitive<value_type>	traits;
	return	__stmstm_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end,
							   pmatching_size, pmin_matching_size, pis_asterisk_remain);
}

template<class ItLookin, class ItFindWhat>
FORCEINLINE  ItLookin
iStmStm(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end,
		OUT size_t *pmatching_size = 0, OUT size_t *pmin_matching_size = 0, OUT bool *pis_asterisk_remain = 0)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type			value_type;
	typedef	__stream_traits_non_wildcard_case_non_sensitive<value_type>	traits;
	return 	__stmstm_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end,
							   pmatching_size, pmin_matching_size, pis_asterisk_remain);
}

template<class ItLookin, class ItFindWhat>
FORCEINLINE  ItLookin
wStmStm(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end,
		OUT size_t *pmatching_size = 0, OUT size_t *pmin_matching_size = 0, OUT bool *pis_asterisk_remain = 0)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type	value_type;
	typedef	__stream_traits_wildcard_case_sensitive<value_type>	traits;
	return	__stmstm_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end,
							   pmatching_size, pmin_matching_size, pis_asterisk_remain);
}

template<class ItLookin, class ItFindWhat>
FORCEINLINE  ItLookin
iwStmStm(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end,
		 OUT size_t *pmatching_size = 0, OUT size_t *pmin_matching_size = 0, OUT bool *pis_asterisk_remain = 0)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type		value_type;
	typedef	__stream_traits_wildcard_case_non_sensitive<value_type>	traits;
	return	__stmstm_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end,
							   pmatching_size, pmin_matching_size, pis_asterisk_remain);
}



template<class Traits, class ItLookin, class ItFindWhat>
int
__stmcmp_t(ItLookin lookin, ItLookin lookin_end, ItFindWhat findwhat, ItFindWhat findwhat_end)
{
	for(; ; ++lookin, ++findwhat)
	{
		if(findwhat == findwhat_end || (Traits::is_transfer(*findwhat) && std::distance(findwhat, findwhat_end) == 1))
		{
			return lookin == lookin_end ? 0 : 1;
		}

		if(Traits::is_asterisk(*findwhat))
		{
		#if !defined(PLATFORM_TYPE_NATIVE)
			std::reverse_iterator<ItLookin> reverse_lookin(lookin_end), reverse_lookin_end(lookin);
			std::reverse_iterator<ItFindWhat> reverse_findwhat(findwhat_end), reverse_findwhat_end(++findwhat);
		#else
			typedef	typename std::iterator_traits<ItLookin>::value_type		value_type;
			std::reverse_iterator<ItLookin, value_type> reverse_lookin(lookin_end), reverse_lookin_end(lookin);
			std::reverse_iterator<ItFindWhat, value_type> reverse_findwhat(findwhat_end), reverse_findwhat_end(++findwhat);
		#endif
			for(; ; ++reverse_lookin, ++reverse_findwhat)
			{
				if(Traits::is_transfer(*reverse_findwhat)) { ++reverse_findwhat; }
				if(reverse_findwhat == reverse_findwhat_end) { return 0; }
				if(Traits::is_asterisk(*reverse_findwhat))
				{
					if(reverse_findwhat +1 == reverse_findwhat_end) { return 0; }
					if(!Traits::is_transfer(*(reverse_findwhat+1))) { ++reverse_findwhat; break; }
				}
				if(reverse_lookin == reverse_lookin_end) { return -1; }
				if(Traits::is_question(*reverse_findwhat))
				{
					if(reverse_findwhat +1 == reverse_findwhat_end) { return 0; }
					if(!Traits::is_transfer(*(reverse_findwhat+1))) { continue; }
				}
				if(Traits::toupper(*reverse_lookin) != Traits::toupper(*reverse_findwhat))
				{
					return (int)((unsigned int)*reverse_lookin -(unsigned int)*reverse_findwhat);
				}
			}
			return __stmstm_t<Traits>(lookin, reverse_lookin.base(), findwhat, reverse_findwhat.base()) != reverse_lookin.base();
		}

		if(lookin == lookin_end) { return -1; }

		if(Traits::is_question(*findwhat)) { continue; }

		if(Traits::is_transfer(*findwhat) && ++findwhat == findwhat_end) { return 1; }

		if(Traits::toupper(*lookin) != Traits::toupper(*findwhat))
		{
			return (int)((unsigned int)*lookin -(unsigned int)*findwhat);
		}
	}
}


// shortcut:
template<class ItLookin, class ItFindWhat>
FORCEINLINE  int
StmCmp(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type		value_type;
	typedef	__stream_traits_non_wildcard_case_sensitive<value_type>	traits;
	return	__stmcmp_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end);
}

template<class ItLookin, class ItFindWhat>
FORCEINLINE  int
iStmCmp(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type			value_type;
	typedef	__stream_traits_non_wildcard_case_non_sensitive<value_type>	traits;
	return	__stmcmp_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end);
}

template<class ItLookin, class ItFindWhat>
FORCEINLINE  int
wStmCmp(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type	value_type;
	typedef	__stream_traits_wildcard_case_sensitive<value_type>	traits;
	return	__stmcmp_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end);
}

template<class ItLookin, class ItFindWhat>
FORCEINLINE  int
iwStmCmp(ItLookin plookin_begin, ItLookin plookin_end, ItFindWhat pfindwhat_begin, ItFindWhat pfindwhat_end)
{
	typedef	typename std::iterator_traits<ItLookin>::value_type		value_type;
	typedef	__stream_traits_wildcard_case_non_sensitive<value_type>	traits;
	return	__stmcmp_t<traits>(plookin_begin, plookin_end, pfindwhat_begin, pfindwhat_end);
}



#if(0)
#include <stdio.h>
void main()
{
	char lookin[] = "ababbaaaccaaaaaac";
	char findwhat[] = "a*?b**B*a*\\a*c*A*c\\";
	printf("%s\n", iwStmStm(lookin, &lookin[sizeof(lookin)-1], findwhat, &findwhat[sizeof(findwhat)-1]));
	printf("%i\n", iwStmCmp(lookin, &lookin[sizeof(lookin)-1], findwhat, &findwhat[sizeof(findwhat)-1]));
}
#endif



#endif //  duplicate inclusion protection
