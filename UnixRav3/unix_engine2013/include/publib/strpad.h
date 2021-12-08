#ifndef __STRING_PAD__
#define __STRING_PAD__

#include <vector>

typedef UINT16 ochar;

template < class CharT >
class StringPadT
{
protected:
	typedef std::vector<CharT*> StringPtrlVector;
	StringPtrlVector	m_StringPad;
	size_t				m_RoomBytes;
	size_t				m_CurrentBytes;
	enum {
		DEFAULT_ROOM = 32 * 1024 * 1024,
	};
public:
	StringPadT() : m_RoomBytes( DEFAULT_ROOM ), m_CurrentBytes(0) {}
	~StringPadT()
	{
		Destroy();
	}
	void SetRoom( size_t bytes )
	{
		m_RoomBytes = bytes;
	}
	const CharT * Pin( const CharT * StringToPin, size_t MaxLength )
	{
		if( !StringToPin) return 0;
		size_t sl = 0;
		for( ; StringToPin[sl]; ++sl );
		if( !sl ) return 0;
		sl = MIN( sl, MaxLength );

		m_CurrentBytes += sl;
		if( m_CurrentBytes > m_RoomBytes )
			return 0;

		CharT * tmp = RSNEW CharT[sl+1];
		if( !tmp ) return 0;
		for( uint32_t i = 0; i < sl; ++ i ) tmp[i] = StringToPin[i];
		tmp[sl] = 0;
		m_StringPad.push_back( tmp );
		return tmp;
	}
//	bool UnPin( const CharT * StringToUnpin )
//	{
//#ifdef _WIN32
//		StringPtrlVector::iterator it( m_StringPad.begin() );
//#else
//		typename StringPtrlVector::iterator it( m_StringPad.begin() );
//#endif
//		for( ; it != m_StringPad.end(); ++ it )
//		{
//			CharT * p = *it;
//			if( p == StringToUnpin )
//			{
//				m_StringPad.erase( it );
//				if( p ) delete [] p;
//				return true;
//			}
//		}
//		return false;
//	}
	void Destroy()
	{
#if CPP_COMPILER==CC_MSVC
		StringPtrlVector::iterator it( m_StringPad.begin() );
#else
	  typename StringPtrlVector::iterator it( m_StringPad.begin() );
#endif
		for( ; it != m_StringPad.end(); ++ it )
		{
			CharT * p = *it;
			if( p ) delete [] p;
		}
		m_StringPad.clear();
		m_CurrentBytes = 0;
	}
};

typedef StringPadT<char>	StringPad;
typedef StringPadT<ochar>	StringPadO;
typedef StringPadT<wchar_t>	StringPadU;

//////////////////////////////////////////////////////////////////////////

template < typename CharT >
static inline void _string_upr( CharT * p )
{
	for( ; *p; ++ p )
		islower( *p ) *p = toupper(*p);
}

template < typename CharT >
static inline void _string_lwr( CharT * p )
{
	for( ; *p; ++ p )
		isupper( *p ) *p = tolower(*p);
}

template < typename CharT >
static inline size_t _string_len( const CharT * p )
{
	const CharT * pe = p;
	for( ; *pe; ++ pe );
	return pe - p;
}

template < typename CharT >
static inline const CharT * _string_cat( CharT * p, const CharT * cut )
{
	CharT * r = p;
	for( ; *p; ++p );
	for( ; *cut; ++ cut ) *p = *cut;
	*p = 0;
	return r;
}

template < typename CharT >
static inline void _string_cpy( CharT * d, const CharT * s )
{
	for( ; *s; ++s, ++ d ) *d = *s;
	*d = 0;
}

#ifndef WIN32

namespace std{		// STRUCT char_traits<unsigned short>
template<>
    struct char_traits<unsigned short>
    {
      typedef unsigned short           char_type;
      typedef unsigned short            int_type;
      typedef streamoff         off_type;
      typedef wstreampos        pos_type;
      typedef mbstate_t         state_type;

      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }

      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }

      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }

      static int
      compare(const char_type* __s1, const char_type* __s2, size_t __n)
      { return memcmp(__s1, __s2, __n*sizeof(char_type)); }

      static size_t
      length(const char_type* __s)
      { return strlen((char*)__s)/sizeof(char_type); }

      static const char_type*
      find(const char_type* __s, size_t __n, const char_type& __a)
      {
	for(int i=0;i<__n;++i)
		if(__s[i]==__a)
			return &__s[i];
	return 0;
      }

      static char_type*
      move(char_type* __s1, const char_type* __s2, int_type __n)
      { return (char_type*)memmove(__s1, __s2, __n*sizeof(char_type)); }
      static char_type*
      copy(char_type* __s1, const char_type* __s2, size_t __n)
      { return (char_type*)memcpy(__s1, __s2, __n*sizeof(char_type)); }

      static char_type*
      assign(char_type* __s, size_t __n, char_type __a)
      { //return memset(__s, __a, __n);
	for(int i=0;i<__n;++i)
		__s[i]=__a;
	return __s;
 }

      static char_type
      to_char_type(const int_type& __c) { return char_type(__c); }

      static int_type
      to_int_type(const char_type& __c) { return int_type(__c); }

      static bool
      eq_int_type(const int_type& __c1, const int_type& __c2)
      { return __c1 == __c2; }

      static int_type
      eof() { return static_cast<int_type>(EOF); }

      static int_type
      not_eof(const int_type& __c)
      { return eq_int_type(__c, eof()) ? 0 : __c; }
  };
};

#endif // #ifndef WIN32

typedef std::basic_string<char>		mstring;
typedef std::basic_string<unsigned short, std::char_traits<unsigned short> >	ostring;
typedef std::basic_string<wchar_t>	ustring;

struct AStr : mstring
{
	AStr( const ochar * poc )
	{
		for( ; *poc; ++ poc )
		{
			if( *poc < 0x100 )
			{
				push_back( (char)(*poc) );
			}
			else
			{
				const char * pc = (const char*)poc;
				push_back( pc[0] );
				push_back( pc[1] );
			}
		}
	}
	AStr( const wchar_t * pwc )
	{
		for( ; *pwc; ++ pwc )
		{
			if( *pwc < 0x100 )
			{
				push_back( (char)(*pwc) );
			}
			else
			{
				const char * pc = (const char*)pwc;
				for( size_t i = 0; i < sizeof(wchar_t); ++ i )
					push_back( pc[i] );
			}
		}
	}
};

struct WStr : ustring
{
	WStr( const char * pc )
	{
		for( ; *pc; ++ pc ) push_back( *pc );
	}
	WStr( const ochar * poc )
	{
		for( ; *poc; ++ poc ) push_back( *poc );
	}
};

struct OStr : ostring
{
	OStr( const char * pc )
	{
		for( ; *pc; ++ pc ) push_back( *pc );
	}
	OStr( const wchar_t * pwc )
	{
		for( ; *pwc; ++ pwc ) push_back( *pwc );
	}
	const ochar * c_str()
	{
		const ochar * ps = ostring::c_str();
		return ps;
	}
};

#endif
