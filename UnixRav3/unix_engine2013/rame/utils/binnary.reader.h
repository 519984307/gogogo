#ifndef __BINNARY_READER_H__
#define __BINNARY_READER_H__
#include "../include/_stdint.h"
#include "../include/platform.h"

namespace binnary
{
#define SWAP_VALUE_U16		_BSWAP16
#define SWAP_VALUE_U32 		_BSWAP32
#define SWAP_VALUE_U64 		_BSWAP64

	struct type_base 
	{
		uint32_t offset;
		uint32_t length;
	};

	template<class T>
	struct fix_len_type_define: public type_base
	{
		T value;
		fix_len_type_define()
		{
			offset = 0;
			length = sizeof(T);
		}

	};

	class binary_stream_base;

	template<>
	struct fix_len_type_define<std::string>: private type_base
	{
		std::string value;
		fix_len_type_define()
		{
			offset = 0;
			length = 0;
		}

		uint32_t get_offset(){return offset;}
		uint32_t get_length(){return value.size();}

		friend binary_stream_base;
	};

	template<>
	struct fix_len_type_define<std::wstring>: private type_base
	{
		std::wstring value;
		fix_len_type_define()
		{
			offset = 0;
			length = 0;
		}
		FORCEINLINE uint32_t get_offset(){return offset;}
		FORCEINLINE uint32_t get_length(){return value.size();}

		friend binary_stream_base;
	};

	typedef fix_len_type_define<int8_t> int8_s;
	typedef fix_len_type_define<uint8_t> uint8_s;
	typedef fix_len_type_define<int16_t> int16_s;
	typedef fix_len_type_define<uint16_t> uint16_s;
	typedef fix_len_type_define<int32_t> int32_s;
	typedef fix_len_type_define<uint32_t> uint32_s;
	typedef fix_len_type_define<int64_t> int64_s;
	typedef fix_len_type_define<uint64_t> uint64_s;
	typedef fix_len_type_define<std::string> string_s;
	typedef fix_len_type_define<std::wstring> wstring_s;



	class binary_stream_base
	{
	public:
		binary_stream_base():m_size(0),m_pos(0){}

		//uint8_t
		template<class T>
		FORCEINLINE bool peek_le_x8(T& value) {return peek_value(value);}

		template<class T>
		FORCEINLINE bool peek_le_x8(T& value , uint32_t offset) {return peek_value(value , offset);}

		template<class T>
		FORCEINLINE bool read_le_x8(T& value) {return read_value(value);}

		template<class T>
		FORCEINLINE bool write_le_x8(T& value) {return write_value(value);}

		template<class T>
		FORCEINLINE bool write_le_x8(T& value , uint32_t offset) {return write_value(value , offset);}

		template<class T>
		FORCEINLINE bool peek_be_x8(T& value) {return peek_value(value );}

		template<class T>
		FORCEINLINE bool peek_be_x8(T& value , uint32_t offset) {return peek_value(value , offset);}

		template<class T>
		FORCEINLINE bool read_be_x8(T& value) {return read_value(value);}

		template<class T>
		FORCEINLINE bool write_be_x8(T& value){return write_value(value);}


		/////////////////////////////////////////////////////////////////
		//uint16_t
		template<class T>
		FORCEINLINE bool peek_le_x16(T& value)
		{
			if (!peek_value(value)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool peek_le_x16(T& value , uint32_t offset)
		{
			if (!peek_value(value , offset)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool read_le_x16(T& value)
		{
			if (!read_value(value)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_le_x16(T& value)
		{
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value);
#endif 
			return write_value(value);
		}

		//////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x16(T& value)
		{
			if (!peek_value(value)) return false;
#ifndef ARCH_BIG_ENDIAN
			value = SWAP_VALUE_U16(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool peek_be_x16(T& value , uint32_t offset)
		{
			if (!peek_value(value , offset)) return false;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool read_be_x16(T& value)
		{
			if (!peek_be_x16(value)) return false;
			m_pos += sizeof(uint16_t);
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_be_x16(T& value)
		{
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value);
#endif 
			return write_value(value);
		}

		/////////////////////////////////////////////////////////////////
		template<class T>
		FORCEINLINE bool peek_le_x24(T& value)
		{
			uint8_t bytes[4] = {0};
			if (!peek_bytes(bytes , 3)) return false;

			value = *(uint32_t*)bytes;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool peek_le_x24(T& value , uint32_t offset)
		{
			uint8_t bytes[4] = {0};
			if (!peek_bytes(bytes , 3 , offset)) return false;

			value = *(uint32_t*)bytes;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool read_le_x24(T& value)
		{
			if (!peek_le_x24(value)) return false;
			m_pos += 3;
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_le_x24(T& value)
		{

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return write_bytes(&value , 3);
		}

		template<class T>
		FORCEINLINE bool write_le_x24(T& value , uint32_t offset)
		{

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return write_bytes(&value , 3 , offset);
		}

		/////////////////////////
		template<class T>
		FORCEINLINE bool peek_be_x24(T& value)
		{
			uint8_t bytes[4];
			if (!peek_bytes(bytes , 3)) return false;

			value = *(uint32_t*)bytes;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool peek_be_x24(T& value , uint32_t offset)
		{
			uint8_t bytes[4];
			if (!peek_bytes(bytes , 3 , offset)) return false;

			value = *(uint32_t*)bytes;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return true;	
		}

		template<class T>
		FORCEINLINE bool read_be_x24(T& value)
		{
			if (!peek_le_x24(value)) return false;
			m_pos += 3;
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_be_x24(T& value)
		{

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif 
			return write_bytes(&value , 3);
		}


		/////////////////////////////////////////////////////////////////

		//uint32_t
		template<class T>
		FORCEINLINE bool peek_le_x32(T& value )
		{
			if (!peek_value(value)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif
			return true;	
		}

		template<class T>
		FORCEINLINE bool peek_le_x32(T& value , uint32_t offset)
		{
			if (!peek_value(value , offset)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif
			return true;	
		}

		template<class T>
		FORCEINLINE bool read_le_x32(T& value)
		{
			if (!peek_le_x32(value)) return false;
			m_pos += sizeof(uint32_t);
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_le_x32(T& value)
		{
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif
			return write_value(value);
		}

		///////////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x32(T& value)
		{
			if (!peek_value(value)) return false;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif
			return true;	
		}

		template<class T>
		FORCEINLINE bool peek_be_x32(T& value , uint32_t offset)
		{
			if (!peek_value(value , offset)) return false;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif
			return true;	
		}

		template<class T>
		FORCEINLINE bool read_be_x32(T& value)
		{
			if (!peek_le_x32(value)) return false;
			m_pos += sizeof(uint32_t);
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_be_x32(T& value)
		{
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value);
#endif
			return write_value(value);
		}

		/////////////////////////////////////////////////////////////////
		//uint64_t
		template<class T>
		FORCEINLINE bool peek_le_x64(T& value)
		{
			if (!peek_value(value)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return true;

		}

		template<class T>
		FORCEINLINE bool peek_le_x64(T& value , uint32_t offset)
		{
			if (!peek_value(value , offset)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return true;

		}

		template<class T>
		FORCEINLINE bool read_le_x64(T& value)
		{
			if (!read_value(value)) return false;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_le_x64(T& value)
		{
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return write_value(value);
		}

		///////////////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x64(T& value)
		{
			if (!peek_value(value)) return false;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return true;

		}

		template<class T>
		FORCEINLINE bool peek_be_x64(T& value , uint32_t offset)
		{
			if (!peek_value(value , offset)) return false;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return true;

		}

		template<class T>
		FORCEINLINE bool read_be_x64(T& value)
		{
			if (!peek_le_x64(value)) return false;
			m_pos += sizeof(uint64_t);
			return true;	
		}

		template<class T>
		FORCEINLINE bool write_be_x64(T& value)
		{
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value);
#endif
			return write_value(value);
		}

		/////////////////////////////////////////////////////////////////

		FORCEINLINE bool peek_string(std::string& v , uint32_t size)
		{
			if (m_pos + size > m_size) return false;
			v.resize(size);
			return xx_read_bytes((void*)v.c_str() , size , m_pos);
		}

		FORCEINLINE bool peek_string(std::string& v , uint32_t size ,uint32_t offset)
		{
			if (offset + size > m_size) return false;
			v.resize(size);
			return xx_read_bytes((void*)v.c_str() , size, offset );
		}
		FORCEINLINE bool read_string(std::string& v , uint32_t size)
		{
			if (!peek_string(v , size)) return false;
			m_pos += size;
			return true;
		}
		FORCEINLINE bool write_string(std::string& v , uint32_t offset)
		{
			if (offset + v.size() > m_size) return false;

			return xx_write_bytes((void*)v.c_str() , v.size() , offset);
		}
		/////////////////////////////////
		FORCEINLINE bool peek_wstring(std::wstring& v , uint32_t size)
		{
			uint32_t bytes_count = size * 2;
			if (m_pos + bytes_count > m_size) return false;

			v.resize(size);

			return xx_read_bytes((void*)v.c_str() , bytes_count , m_pos);
		}
		FORCEINLINE bool peek_wstring(std::wstring& v , uint32_t size ,uint32_t offset)
		{
			uint32_t bytes_count = size * 2;
			if (offset + bytes_count > m_size) return false;

			v.resize(size);
			return xx_read_bytes((void*)v.c_str() , bytes_count  , offset);
		}
		FORCEINLINE bool read_wstring(std::wstring& v , uint32_t size)
		{
			if (!peek_wstring(v , size)) return false;
			m_pos += size * 2;
			return true;
		}
		FORCEINLINE bool write_wstring(std::wstring& v , uint32_t offset)
		{
			if (offset + v.size() * 2 > m_size) return false;
			return xx_write_bytes((void*)v.c_str() ,  v.size() * 2 , offset);
		}

		/////////////////////////////////////////////////////////////////

		FORCEINLINE bool peek_bytes(void* p , uint32_t size)
		{
			if (!p || !size || m_pos + size > m_size) return false;
			return xx_read_bytes(p , size , m_pos);
		}

		FORCEINLINE bool peek_bytes(void* p , uint32_t size , uint32_t offset)
		{
			if (!p || !size || offset + size > m_size) return false;
			return xx_read_bytes(p , size , offset);
		}

		FORCEINLINE bool read_bytes(void* p , uint32_t size)
		{
			if (!peek_bytes(p, size)) return false;
			m_pos += size;
			return true;
		}

		FORCEINLINE bool write_bytes(void* p , uint32_t size)
		{
			if (!p || !size || m_pos + size > m_size) return false;
			if (!xx_write_bytes(p , size , m_pos)) return false;
			m_pos += size;
			return true;
		}

		FORCEINLINE bool write_bytes(void* p , uint32_t size , uint32_t offset)
		{
			if (!p || !size || offset + size > m_size) return false;
			return xx_write_bytes(p , size , offset);
		}

		/////////////////////////////////read data structure//////////////////////////////////////

		template<class T>
		FORCEINLINE bool peek_le_x8_s(T& value ){return peek_value_s<T,uint8_t>(value );}

		template<class T>
		FORCEINLINE bool peek_le_x8_s(T& value , uint32_t offset ){return peek_value_s<T,uint8_t>(value , offset);}

		template<class T>
		FORCEINLINE bool read_le_x8_s(T& value ){return read_value_s<T,uint8_t>(value);}

		template<class T>
		FORCEINLINE bool write_le_x8_s(T& value ){return write_value_s<T,uint8_t>(value);}

		/////////////////////////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x8_s(T& value ) {return peek_value_s<T,uint8_t>(value);}

		template<class T>
		FORCEINLINE bool peek_be_x8_s(T& value , uint32_t offset) {return peek_value_s<T,uint8_t>(value , offset);}

		template<class T>
		FORCEINLINE bool read_be_x8_s(T& value ){return read_value_s<T,uint8_t>(value);}

		template<class T>
		FORCEINLINE bool write_be_x8_s(T& value){return write_value_s<T,uint8_t>(value);}

		/////////////////////////////////////////////////////////////////
		//uint16_s
		template<class T>
		FORCEINLINE bool peek_le_x16_s(T& value)
		{
			if (!peek_value_s<T , uint16_t>(value)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool peek_le_x16_s(T& value , uint32_t offset)
		{
			if (!peek_value_s<T , uint16_t>(value , offset)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool read_le_x16_s(T& value)
		{
			if (!read_value_s<T , uint16_t>(value)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool write_le_x16_s(T& value)
		{
			T vl = value;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(vl.value);
#endif
			if(!write_value_s(vl)) return false;
			return true;
		}

		/////////////////////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x16_s(T& value)
		{
			if (!peek_value_s<T , uint16_t>(value)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool peek_be_x16_s(T& value , uint32_t offset)
		{
			if (!peek_value_s<T , uint16_t>(value , offset)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool read_be_x16_s(T& value)
		{
			if (!read_value_s<T , uint16_t>(value)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool write_be_x16_s(T& value)
		{
			T vl = value;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U16(vl.value);
#endif
			if(!write_value_s(vl)) return false;
			return true;
		}

		/////////////////////////////////////////////////////////////////

		template<class T>
		FORCEINLINE bool peek_le_x24_s(T& value)
		{

			return false;
		}


		/////////////////////////////////////////////////////////////////

		//uint32_s
		template<class T>
		FORCEINLINE bool peek_le_x32_s(T& value)
		{
			if (!peek_value_s<T , uint32_t>(value)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool peek_le_x32_s(T& value , uint32_t offset)
		{
			if (!peek_value_s<T , uint32_t>(value , offset)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool read_le_x32_s(T& value )
		{
			if (!read_value_s<T , uint32_t>(value)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool write_le_x32_s(T& value )
		{
			T vl = value;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(vl.value);
#endif
			if(!write_value_s(vl)) return false;
			return true;
		}

		///////////////////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x32_s(T& value)
		{
			if (!peek_value_s<T , uint32_t>(value)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool peek_be_x32_s(T& value , uint32_t offset)
		{
			if (!peek_value_s<T , uint32_t>(value , offset)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool read_be_x32_s(T& value)
		{
			if (!read_value_s<T , uint32_t>(value)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(value.value);
#endif
		}

		template<class T>
		FORCEINLINE bool write_be_x32_s(T& value )
		{
			T vl = value;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U32(vl.value);
#endif
			if(!write_value_s(vl)) return false;
			return true;
		}

		/////////////////////////////////////////////////////////////////
		//uint64_s
		template<class T>
		FORCEINLINE bool peek_le_x64_s(T& value)
		{
			if (!peek_value_s<T , uint64_t>(value)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool peek_le_x64_s(T& value , uint32_t offset)
		{
			if (!peek_value_s<T , uint64_t>(value , offset)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool read_le_x64_s(T& value)
		{
			if (!read_value_s<T , uint64_t>(value)) return false;

#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool write_le_x64_s(T& value)
		{
			T vl = value;
#ifdef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(vl.value);
#endif
			if(!write_value_s(vl)) return false;
			return true;
		}

		////////////////////////////////////////

		template<class T>
		FORCEINLINE bool peek_be_x64_s(T& value)
		{
			if (!peek_value_s<T , uint64_t>(value)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool peek_be_x64_s(T& value , uint32_t offset)
		{
			if (!peek_value_s<T , uint64_t>(value , offset)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool read_be_x64_s(T& value )
		{
			if (!read_value_s<T , uint64_t>(value)) return false;

#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(value.value);
#endif
			return true;
		}

		template<class T>
		FORCEINLINE bool write_be_x64_s(T& value )
		{
			T vl = value;
#ifndef ARCH_BIG_ENDIAN
			SWAP_VALUE_U64(vl.value);
#endif
			if(!write_value_s(vl)) return false;
			return true;
		}

		/////////////////////////////////////////////////////////////////

		FORCEINLINE bool peek_string_s(string_s& v , uint32_t size)
		{
			if (m_pos  + size > m_size) return false;
			v.value.resize(size);

			if (!xx_read_bytes((void*)v.value.c_str() , size , m_pos)) return false;

			v.offset = m_pos ;
			v.length = size;

			return true;
		}

		FORCEINLINE bool peek_string_s(string_s& v , uint32_t size ,uint32_t offset )
		{
			if (offset + size > m_size) return false;
			v.value.resize(size);

			if (!xx_read_bytes((void*)v.value.c_str() , size , offset)) return false;

			v.offset = offset;
			v.length = size;

			return true;
		}
		FORCEINLINE bool read_string_s(string_s& v , uint32_t size)
		{
			if (!peek_string_s(v , size)) return false;
			m_pos += size;
			return true;
		}
		FORCEINLINE bool write_string_s(string_s& v)
		{
			if (m_pos + v.offset + v.length > m_size) return false;

			return xx_write_bytes((void*)v.value.c_str(), v.length , v.offset);
		}

		//////////////////////////////////////////////////

		FORCEINLINE bool peek_wstring_s(wstring_s& v , uint32_t size )
		{
			uint32_t bytes_count = size * 2;
			if (m_pos + bytes_count > m_size) return false;

			v.value.resize(size);
			if (!xx_read_bytes((void*)v.value.c_str() , bytes_count , m_pos)) return false;

			v.offset = m_pos;
			v.length = size;

			return true;
		}
		FORCEINLINE bool peek_wstring_s(wstring_s& v , uint32_t size ,uint32_t offset)
		{
			uint32_t bytes_count = size * 2;
			if (m_pos + offset + bytes_count > m_size) return false;

			v.value.resize(size);
			if (!xx_read_bytes((void*)v.value.c_str() , bytes_count , offset)) return false;

			v.offset = m_pos + offset;
			v.length = size;

			return true;
		}
		FORCEINLINE bool read_wstring_s(wstring_s& v , uint32_t size)
		{
			if (!peek_wstring_s(v , size)) return false;
			m_pos += size * 2;
			return true;
		}
		FORCEINLINE bool write_wstring_s(wstring_s& v)
		{
			if (m_pos + v.offset + v.length * 2 > m_size) return false;
			return xx_write_bytes((void*)v.value.c_str(), v.length * 2 ,v.offset);
		}

		/////////////////////////////////////////////////////////////////

		FORCEINLINE bool has_data_to_read(uint32_t bytes = 1)
		{
			if (m_pos + bytes > m_size ) return false;
			return true;
		}

		FORCEINLINE bool skip(uint32_t bytes)
		{
			if (m_pos + bytes > m_size) return false;
			m_pos += bytes;
			return true;
		}
		FORCEINLINE bool seek(uint32_t offset)
		{
			if (offset > m_size) return false;
			m_pos = offset;
			return true;
		}
		FORCEINLINE bool seek_end()
		{
			m_pos = m_size;
			return true;
		}
		FORCEINLINE uint32_t get_position()
		{
			return m_pos;
		}
		FORCEINLINE uint32_t get_stream_size()
		{
			return m_size;
		}
		FORCEINLINE uint32_t get_remain_bytes()
		{
			return m_size - m_pos;
		}

		template<class T>
		bool peek_value(T& value)
		{
			if (m_pos + sizeof(T) > m_size) return false;
			return xx_read_bytes((void*)&value  , sizeof(T), m_pos);
		}

		template<class T>
		bool peek_value(T& value , uint32_t offset)
		{
			if (offset + sizeof(T) > m_size) return false;
			return xx_read_bytes((void*)&value  , sizeof(T), offset);
		}

		template<class T>
		bool read_value(T& value)
		{
			if (m_pos + sizeof(T) > m_size) return false;
			if (!xx_read_bytes((void*)&value , sizeof(T) , m_pos)) return false;
			m_pos += sizeof(T);
			return true;
		}

		template <class T>
		bool write_value(T& value )
		{
			if (m_pos + sizeof(T) > m_size) return false;
			if (!xx_write_bytes((void*)&value , sizeof(T) , m_pos)) return false;
			m_pos += sizeof(T);
			return true;
		}

		template <class T>
		bool write_value(T& value , uint32_t offset)
		{
			if (offset + sizeof(T) > m_size) return false;
			if (!xx_write_bytes((void*)&value , sizeof(T) , offset)) return false;

			return true;
		}
	private:
		virtual bool xx_read_bytes(void* data , uint32_t size, uint32_t abs_offset) = 0;
		virtual bool xx_write_bytes(void* data , uint32_t size, uint32_t abs_offset) = 0;
	private:

		template<class T , class T2>
		bool peek_value_s(T& value )
		{
			if (m_pos  + sizeof(T2) > m_size) return false;
			if (!xx_read_bytes((void*)&value.value , sizeof(T2) , m_pos)) return false;
			value.offset = m_pos;
			return true;
		}

		template<class T , class T2>
		bool peek_value_s(T& value , uint32_t offset)
		{
			if (m_pos + offset + sizeof(T2) > m_size) return false;
			if (!xx_read_bytes((void*)&value.value , sizeof(T2) , offset)) return false;
			value.offset = m_pos + offset;
			return true;
		}

		template<class T , class T2>
		bool read_value_s(T& value)
		{
			if (m_pos  + sizeof(T2) > m_size) return false;
			if (!xx_read_bytes((void*)&value.value , sizeof(T2) , m_pos)) return false;
			value.offset = m_pos ;
			m_pos += sizeof(T2);
			return true;
		}

		template <class T , class T2>
		bool write_value_s(T& value)
		{
			if (value.offset + sizeof(T2) > m_size) return false;
			return xx_write_bytes((void*)&value.value  , sizeof(T2), value.offset);
		}

	protected:
		uint32_t m_size;
		uint32_t m_pos;
	};

	class BinnaryReader : public binary_stream_base
	{
	public:
		BinnaryReader(uint8_t* data , uint32_t size):m_data(data)
		{
			m_size = size;
		}
	private:
		virtual bool xx_read_bytes(void* data ,uint32_t size , uint32_t offset)
		{
			if (!data || offset + size > m_size) return false;
			memcpy(data , m_data + offset , size);
			return true;
		}
		virtual bool xx_write_bytes(void* data , uint32_t size , uint32_t offset)
		{
			if (!data || offset + size > m_size) return false;
			memcpy(m_data + offset , data , size);
			return true;
		}
	private:
		uint8_t* m_data;
	};

	class StreamReader : public binary_stream_base
	{
	public:
		StreamReader(IREStream* pstm):m_pstm(pstm)
		{
			RASSERTV(pstm);
			m_size = -1;
			DWORD size = 0;
			pstm->GetSize(&size , 0);
			m_size = size;

			pstm->Seek(0 , 0 , FILE_BEGIN);
		}
	private:
		virtual bool xx_read_bytes(void* data ,uint32_t size , uint32_t offset)
		{
			if (!m_pstm || offset + size > m_size) return false;

			long l = offset;
			if (m_pstm->Seek(0 , &l , FILE_BEGIN  ) < 0) return false;

			DWORD dwRead = 0;
			if (m_pstm->Read(data , size , &dwRead) < 0) return false;


			return dwRead == size ? true : false;
		}
		virtual bool xx_write_bytes(void* data , uint32_t size , uint32_t offset)
		{
			if (!m_pstm || offset + size > m_size) return false;

			long l = offset;
			if (m_pstm->Seek(0 , &l , FILE_BEGIN  ) < 0) return false;

			DWORD dwWrite = 0;
			if (m_pstm->Write(data , size ,&dwWrite ) < 0)  return false;

			return dwWrite == size ? true : false;
		}
	private:
		UTIL::com_ptr<IREStream> m_pstm;
	};
};

#endif