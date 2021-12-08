#ifndef __LIGHTALG_H__
#define __LIGHTALG_H__
#include <string>
#include <vector>

namespace alg {;

/////////////////////////////命名空间开始/////////////////////////////

static void base62( std::string & out, uint64_t n ) 
{
	static const char * cs = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	for( ; n ; n = n / 62 )
	{			
		uint8_t x = n % 62;
		out.push_back( cs[n % 62] );
	}
}

static const char * hex_chars = "0123456789ABCDEF";
static inline std::string Bin2Hex( const void * buf, size_t bytes ) 
{
	std::string hex_str;
	hex_str.reserve(bytes*2);
	const uint8_t * bp = (const uint8_t *)buf;
	for( size_t i = 0; i < bytes; ++ i ) {
		uint8_t k = bp[i];
		uint8_t l4 = k & 0xf;
		uint8_t h4 = k >> 4;
		hex_str.push_back( hex_chars[h4] );
		hex_str.push_back( hex_chars[l4] );
	}
	return hex_str;
}

static bool  Hex2Bin(const std::string &strHex , std::vector<uint8_t>& vec)
{
	if (strHex.empty()) return false;

	if (strHex.size() % 2 != 0) return false;

	size_t i = 0;
	uint32_t size = strHex.size()/2;
	if (rs_strnicmp(strHex.c_str() , "0x" , 2) == 0)
	{
		size -= 1;
		i = 2;
	}

	vec.resize(size);

	for (; i < vec.size(); i++)
	{
		uint8_t cTemp = 0;
		for (size_t j = 0; j < 2; j++)
		{
			char cCur = strHex[2 * i + j];
			if (cCur >= '0' && cCur <= '9'){cTemp = (cTemp << 4) + (cCur - '0');}
			else if (cCur >= 'a' && cCur <= 'f'){cTemp = (cTemp << 4) + (cCur - 'a' + 10);}
			else if (cCur >= 'A' && cCur <= 'F'){cTemp = (cTemp << 4) + (cCur - 'A' + 10);}
			else{return false;}
		}

		vec[i] = cTemp;
	}

	return true;
}



static const std::string base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

static std::string base64_encode_s(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];
	}

	return ret;

}


static void base64_decode(const char* data , int size , std::vector<uint8_t>& out) {
	int in_len = size;
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];


	while (in_len-- && ( data[in_] != '=') && is_base64(data[in_])) {
		char_array_4[i++] = data[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				out.push_back(char_array_3[i]);
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) out.push_back(char_array_3[j]);
	}

}

static void base64_decode(std::string const& encoded_string , std::vector<uint8_t>& out) 
{
	base64_decode(encoded_string.c_str() , encoded_string.size() , out);
}
static void base64_decode(std::string const& encoded_string , std::string& out) 
{
	std::vector<uint8_t> data;
	base64_decode(encoded_string.c_str() , encoded_string.size() , data);
	if(data.empty()) return;
	out.assign(&data[0] , &data[0] + data.size());
}

/////////////////////////////命名空间结束/////////////////////////////
}

#endif