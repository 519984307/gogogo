#ifndef __BYTE_ENDIAN__
#define __BYTE_ENDIAN__

#pragma pack(1)

namespace bq
{
	template < class T >
	inline T bswap( T v )
	{
		size_t c = sizeof(v);
		unsigned char * bp = (unsigned char *)&v;
		for( size_t i = 0; i < c/2; ++ i )
		{
			unsigned char t = bp[i];
			bp[i] = bp[c-1-i]; bp[c-1-i] = t;
		}
		return v;
	}
	template < class T >
	inline T tole( T v )
	{
#ifdef RS_BIG_ENDIAN
		return v;
#else
		return bswap(v);
#endif
	}
	template < class T >
	inline T tobe( T v )
	{
#ifdef RS_BIG_ENDIAN
		return bswap(v);
#else
		return v;
#endif
	}
	template < class T >
	inline T htob( T v )
	{
#ifdef ARCH_BIG_ENDIAN
		return v;
#else
		return bswap(v);
#endif
	}
	template < class T >
	inline T htol( T v )
	{
#ifdef ARCH_BIG_ENDIAN
		return bswap(v);
#else
		return v;
#endif
	}
	template < class T >
	inline T ltoh( T v )
	{
#ifdef ARCH_BIG_ENDIAN
		return bswap(v);
#else
		return v;
#endif
	}
	template < class T >
	inline T btoh( T v )
	{
#ifdef ARCH_BIG_ENDIAN
		return v;
#else
		return bswap(v);
#endif
	}
};

namespace le
{
	typedef CHAR I8, *PI8;
	typedef BYTE U8, *PU8;

	class U16
	{
		typedef BYTE _U8;
		typedef UINT16 _U16, *_PU16;
	private:
		_U8 _b[2];
	public:
		explicit U16(_U16 u = 0)
		{
			*this = u;
		}
		U16 & operator = (_U16 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PU16(_b) = u;
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  
#endif
			return *this;
		}
		operator _U16 () const
		{
			_U16 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PU16(_b);
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}
		U16 & operator += (_U16 u)
		{
			return *this = _U16(*this) + u;
		}
		U16 & operator -= (_U16 u)
		{
			return *this = _U16(*this) - u;
		}
		U16 & operator ^= (_U16 u)
		{
			return *this = _U16(*this) ^ u;
		}
		U16& operator &=(_U16 u)
		{
			return *this = (_U16)(*this) & u;
		}
	} __attribute__((packed));

	typedef U16 * PU16;

	class I16
	{
		typedef BYTE _U8;
		typedef INT16 _I16, *_PI16;
	private:
		_U8 _b[2];
	public:
		explicit I16(_I16 u = 0)
		{
			*this = u;
		}
		I16 & operator = (_I16 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PI16(_b) = u;
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
#endif
			return *this;
		}
		operator _I16 () const
		{
			_I16 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PI16(_b);
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}

		I16 & operator += (_I16 u)
		{
			return *this = _I16(*this) + u;
		}

		I16 & operator -= (_I16 u)
		{
			return *this = _I16(*this) - u;
		}

		I16 & operator ^= (_I16 u)
		{
			return *this = _I16(*this) ^ u;
		}

		I16 & operator &= (_I16 u)
		{
			return *this = _I16(*this) & u;
		}
	} __attribute__((packed));

	typedef I16 * PI16;


	class U32
	{
		typedef BYTE _U8;
		typedef UINT32 _U32, *_PU32;
	private:
		_U8 _b[4];
	public:
		explicit U32(_U32 u = 0)
		{
			*this = u;
		}
		U32 & operator = (_U32 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PU32(_b) = u;
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  
#endif
			return *this;
		}
		operator _U32 () const
		{
			_U32 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PU32(_b);
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[3]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}

		U32 & operator += (_U32 u)
		{
			return *this = _U32(*this) + u;
		}

		U32 & operator -= (_U32 u)
		{
			return *this = _U32(*this) - u;
		}

		U32 & operator ^= (_U32 u)
		{
			return *this = _U32(*this) ^ u;
		}

		U32 & operator &= (_U32 u)
		{
			return *this = _U32(*this) & u;
		}
	} __attribute__((packed));

	class I32
	{
		typedef BYTE _U8;
		typedef INT32 _I32, *_PI32;
	private:
		_U8 _b[4];
	public:
		explicit I32(_I32 u = 0)
		{
			*this = u;
		}
		I32 & operator = (_I32 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PI32(_b) = u;
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  
#endif
			return *this;
		}
		operator _I32 () const
		{
			_I32 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PI32(_b);
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[3]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}

		I32 & operator += (_I32 u)
		{
			return *this = _I32(*this) + u;
		}

		I32 & operator -= (_I32 u)
		{
			return *this = _I32(*this) - u;
		}

		I32 & operator ^= (_I32 u)
		{
			return *this = _I32(*this) ^ u;
		}

		I32 & operator &= (_I32 u)
		{
			return *this = _I32(*this) & u;
		}
	} __attribute__((packed));

	typedef I32 * PI32;

	class U24
	{
		typedef BYTE	_U8;
		typedef UINT32 _U24, *_PU24;
	private:
		_U8 _b[3];
	public:
		explicit U24(_U24 u = 0)
		{
			*this = u;
		}
		U24 & operator = (_U24 u)
		{
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);
			return *this;
		}
		operator _U24 () const
		{
			_U24 u;
			u = 0;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
			return u;
		}

		U24 & operator += (_U24 u)
		{
			return *this = _U24(*this) + u;
		}

		U24 & operator -= (_U24 u)
		{
			return *this = _U24(*this) - u;
		}
	} __attribute__((packed));

	typedef U24 * PU24;

	class U64
	{
		typedef BYTE _U8;
		typedef UINT64 _U64, *_PU64;
	private:
		_U8 _b[8];
	public:
		explicit U64(_U64 u = 0)
		{
			*this = u;
		}
		U64 & operator = (_U64 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PU64(_b) = u;
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  u >>= 8;
			_b[4] = _U8(u);  u >>= 8;
			_b[5] = _U8(u);  u >>= 8;
			_b[6] = _U8(u);  u >>= 8;
			_b[7] = _U8(u);  
#endif
			return *this;
		}
		operator _U64 () const
		{
			_U64 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PU64(_b);
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[7]; u <<= 8;
			u |= _b[6]; u <<= 8;
			u |= _b[5]; u <<= 8;
			u |= _b[4]; u <<= 8;
			u |= _b[3]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}
	} __attribute__((packed));

	typedef U64 * PU64;

	class I64
	{
		typedef BYTE _U8;
		typedef INT64 _I64, *_PI64;
	private:
		_U8 _b[8];
	public:
		explicit I64(_I64 u = 0)
		{
			*this = u;
		}
		I64 & operator = (_I64 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PI64(_b) = u;
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  u >>= 8;
			_b[4] = _U8(u);  u >>= 8;
			_b[5] = _U8(u);  u >>= 8;
			_b[6] = _U8(u);  u >>= 8;
			_b[7] = _U8(u);  
#endif
			return *this;
		}
		operator _I64 () const
		{
			_I64 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PI64(_b);
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[7]; u <<= 8;
			u |= _b[6]; u <<= 8;
			u |= _b[5]; u <<= 8;
			u |= _b[4]; u <<= 8;
			u |= _b[3]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}
	} __attribute__((packed));
	typedef I64 * PI64;
	class F64
	{
		typedef BYTE _U8;
		typedef double _F64, *_PF64;
		typedef UINT64 _U64, *_PU64;
	private:
		_U8 _b[8];
	public:
		explicit F64(_F64 u = 0)
		{
			*this = u;
		}
		F64 & operator = (_F64 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			*_PF64(_b) = u;
#else
			_U64 ui = *(_PU64)(&u);
			_b[0] = _U8(ui);  ui >>= 8;
			_b[1] = _U8(ui);  ui >>= 8;
			_b[2] = _U8(ui);  ui >>= 8;
			_b[3] = _U8(ui);  ui >>= 8;
			_b[4] = _U8(ui);  ui >>= 8;
			_b[5] = _U8(ui);  ui >>= 8;
			_b[6] = _U8(ui);  ui >>= 8;
			_b[7] = _U8(ui);  
#endif
			return *this;
		}
		operator _F64 () const
		{
			_F64 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = *_PF64(_b);
#else // RS_BIG_ENDIAN
			_U64 ui = 0;
			ui |= _b[7]; ui <<= 8;
			ui |= _b[6]; ui <<= 8;
			ui |= _b[5]; ui <<= 8;
			ui |= _b[4]; ui <<= 8;
			ui |= _b[3]; ui <<= 8;
			ui |= _b[2]; ui <<= 8;
			ui |= _b[1]; ui <<= 8;
			ui |= _b[0];
			u = *(_PF64)(&ui);
#endif 
			return u;
		}

		F64 & operator += (_F64 u)
		{
			return *this = _F64(*this) + u;
		}

		F64 & operator -= (_F64 u)
		{
			return *this = _F64(*this) - u;
		}

		F64 & operator *= (_F64 u)
		{
			return *this = _F64(*this) * u;
		}

		F64 & operator /= (_F64 u)
		{
			return *this = _F64(*this) / u;
		}

	} __attribute__((packed));

	typedef F64 * PF64;

	//////////////////////////////////////////////////////////////////////////
	class N32
	{
		typedef BYTE _U8;
		typedef UINT32 _N32, *_PN32;
	private:
		_U8 _b[4];
	public:
		explicit N32(_N32 u = 0)
		{
			*this = u;
		}
		N32 & operator = (_N32 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			_b[3] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[0] = _U8(u);  
#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  
#endif
			return *this;
		}
		operator _N32 () const
		{
			_N32 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = 0;
			u |= _b[0]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[3];
#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[3]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}
		N32 & operator += (_N32 u)
		{
			return *this = _N32(*this) + u;
		}
		N32 & operator -= (_N32 u)
		{
			return *this = _N32(*this) - u;
		}
	} __attribute__((packed));

	typedef N32 * PN32;

	class N64
	{
		typedef BYTE _U8;
		typedef UINT64 _N64, *_PN64;
	private:
		_U8 _b[8];
	public:
		explicit N64(_N64 u = 0)
		{
			*this = u;
		}
		N64 & operator = (_N64 u)
		{
#ifdef ARCH_LITTLE_ENDIAN
			_b[7] = _U8(u);  u >>= 8;
			_b[6] = _U8(u);  u >>= 8;
			_b[5] = _U8(u);  u >>= 8;
			_b[4] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[0] = _U8(u);  

#else
			_b[0] = _U8(u);  u >>= 8;
			_b[1] = _U8(u);  u >>= 8;
			_b[2] = _U8(u);  u >>= 8;
			_b[3] = _U8(u);  u >>= 8;
			_b[4] = _U8(u);  u >>= 8;
			_b[5] = _U8(u);  u >>= 8;
			_b[6] = _U8(u);  u >>= 8;
			_b[7] = _U8(u);  
#endif
			return *this;
		}
		operator _N64 () const
		{
			_N64 u;
#ifdef ARCH_LITTLE_ENDIAN
			u = 0;
			u |= _b[0]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[3]; u <<= 8;
			u |= _b[4]; u <<= 8;
			u |= _b[5]; u <<= 8;
			u |= _b[6]; u <<= 8;
			u |= _b[7];

#else // RS_BIG_ENDIAN
			u = 0;
			u |= _b[7]; u <<= 8;
			u |= _b[6]; u <<= 8;
			u |= _b[5]; u <<= 8;
			u |= _b[4]; u <<= 8;
			u |= _b[3]; u <<= 8;
			u |= _b[2]; u <<= 8;
			u |= _b[1]; u <<= 8;
			u |= _b[0];
#endif 
			return u;
		}
	} __attribute__((packed));

	typedef N64 * PN64;

	//////////////////////////////////////////////////////////////////////////
};


#ifdef RS_BIG_ENDIAN
	typedef le::U8	LE_UI8;
	typedef le::I16	LE_I16;
	typedef le::U16	LE_UI16;
	typedef le::I32 LE_I32;
	typedef le::U32	LE_UI32;
	typedef le::F64	LE_F64;
	typedef le::I64	LE_I64;
	typedef le::U64	LE_UI64;
#else
	typedef UINT8	LE_UI8;
	typedef INT16	LE_I16;
	typedef UINT16	LE_UI16;
	typedef INT32	LE_I32;
	typedef UINT32	LE_UI32;
	typedef double	LE_F64;
	typedef INT64	LE_I64;
	typedef UINT64	LE_UI64;
#endif


#pragma pack()

#endif