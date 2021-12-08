/*
 * Hacker Disassembler Engine 32
 * Copyright (c) 2006-2009, Vyacheslav Patkov.
 * All rights reserved.
 *
 * hde32.h: C/C++ header file
 *
 */

#ifndef _HDE32_H_
#define _HDE32_H_

/* stdint.h - C99 standard header
 * http://en.wikipedia.org/wiki/stdint.h
 *
 * if your compiler doesn't contain "stdint.h" header (for
 * example, Microsoft Visual C++), you can download file:
 *   http://www.azillionmonkeys.com/qed/pstdint.h
 * and change next line to:
 *   #include "pstdint.h"
 */

#define F_MODRM         0x00000001
#define F_SIB           0x00000002
#define F_IMM8          0x00000004
#define F_IMM16         0x00000008
#define F_IMM32         0x00000010
#define F_DISP8         0x00000020
#define F_DISP16        0x00000040
#define F_DISP32        0x00000080
#define F_RELATIVE      0x00000100
#define F_2IMM16        0x00000800
#define F_ERROR         0x00001000
#define F_ERROR_OPCODE  0x00002000
#define F_ERROR_LENGTH  0x00004000
#define F_ERROR_LOCK    0x00008000
#define F_ERROR_OPERAND 0x00010000
#define F_PREFIX_REPNZ  0x01000000
#define F_PREFIX_REPX   0x02000000
#define F_PREFIX_REP    0x03000000
#define F_PREFIX_66     0x04000000
#define F_PREFIX_67     0x08000000
#define F_PREFIX_LOCK   0x10000000
#define F_PREFIX_SEG    0x20000000
#define F_PREFIX_ANY    0x3f000000
#define F_GROUP			0x40000000

#define PREFIX_SEGMENT_CS   0x2e
#define PREFIX_SEGMENT_SS   0x36
#define PREFIX_SEGMENT_DS   0x3e
#define PREFIX_SEGMENT_ES   0x26
#define PREFIX_SEGMENT_FS   0x64
#define PREFIX_SEGMENT_GS   0x65
#define PREFIX_LOCK         0xf0
#define PREFIX_REPNZ        0xf2
#define PREFIX_REPX         0xf3
#define PREFIX_OPERAND_SIZE 0x66
#define PREFIX_ADDRESS_SIZE 0x67

#pragma pack(push,1)

typedef struct {
    uint8_t len;
	uint8_t len2;
    uint8_t p_rep;
    uint8_t p_lock;
    uint8_t p_seg;
    uint8_t p_66;
    uint8_t p_67;
    uint8_t opcode;
    uint8_t opcode2;
    uint8_t modrm;
    uint8_t modrm_mod;
    uint8_t modrm_reg;
    uint8_t modrm_rm;
    uint8_t sib;
    uint8_t sib_scale;
    uint8_t sib_index;
    uint8_t sib_base;
#ifdef RS_BIG_ENDIAN
#if (CPP_COMPILER==CC_MSVC)
#pragma message("include-2012-hde32 BIG_ENDIAN")
#elif (CPP_COMPILER==CC_GCC)
#warning ("include-2012-hde32 BIG_ENDIAN")
#endif

	union {
		struct{
			uint8_t nouse1;
			uint8_t nouse2;
			uint8_t nouse3;
			uint8_t imm8;
		};
		struct{
			uint16_t nouse;
			uint16_t imm16;
		};
		uint32_t imm32;
	} imm;
	union {
		struct{
			uint8_t nouse1;
			uint8_t nouse2;
			uint8_t nouse3;
			uint8_t disp8;
		};
		struct{
			uint16_t nouse;
			uint16_t disp16;
		};
		uint32_t disp32;
	} disp;
#else
#if (CPP_COMPILER==CC_MSVC)
#pragma message("include-2012-hde32 LITTLE_ENDIAN")
#elif (CPP_COMPILER==CC_GCC)
#warning ("include-2012-hde32 LITTLE_ENDIAN")
#endif

    union {
        uint8_t imm8;
        uint16_t imm16;
        uint32_t imm32;
    } imm;
    union {
        uint8_t disp8;
        uint16_t disp16;
        uint32_t disp32;
    } disp;
#endif
    uint32_t flags;
} hde32s;

#pragma pack(pop)

struct bptr_t
{
	const unsigned char * _head;
	const unsigned char * _bptr;
	const unsigned char * _tail;

	inline bptr_t( const unsigned char * h, const unsigned char * t ) 
		: _head(h), _tail(t), _bptr(h)
	{}

	inline unsigned char * ptr()
	{
		return (unsigned char *)_bptr;
	}
	inline size_t offset( )
	{
		return _bptr - _head;
	}

	inline bool seek( long ofs )
	{
		if( _bptr + ofs > _tail ) return false;
		if( _bptr + ofs < _head ) return false;
		_bptr += ofs;
		return true;
	}
	inline bool setofs( size_t ofs )
	{
		if( _head + ofs > _tail ) return false;
		if( _head + ofs < _head ) return false;
		_bptr = _head + ofs;
		return true;
	}

	inline bool next( unsigned char & rc )
	{
		return fetch(rc);
	}
	template < class T >
	inline bool fetch( T & rc )
	{
		if( !peek(rc) ) return false;
		_bptr += sizeof(T);
		return true;
	}
	template < class T >
	inline bool peek( T & rc )
	{
		if( _bptr + sizeof(rc) >= _tail ) return false;
		rc = GETHX<T,NEEDSWAP>(_bptr);
		return true;
	}
	inline bool fetch( unsigned char * dest, size_t cb )
	{
		if( !peek( dest, cb ) ) return false;
		_bptr += cb;
		return true;
	}
	inline bool peek( unsigned char * dest, size_t cb )
	{
		if( _bptr + cb > _tail ) return false;
		for( size_t b = 0; b < cb; ++ b )
			dest[b] = _bptr[b];
		return true;
	}
};


#ifdef __cplusplus
extern "C" {
#endif

/* __cdecl */
unsigned int hde32_disasm(bptr_t& code, hde32s *hs = 0);

#ifdef __cplusplus
}
#endif

#endif /* _HDE32_H_ */
