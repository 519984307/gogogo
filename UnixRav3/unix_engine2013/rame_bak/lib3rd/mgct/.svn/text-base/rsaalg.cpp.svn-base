#include "stdafx.h"
#include "rsaalg.h"
#include <vector>
#include "rsa/rsa.h"
#include "md5.h"

#ifdef _WIN32
#include "guid/userguid2.h"
#include "guid/usersguid.h"
#include "guid/userguid.h"
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mmkey/config.h"
#include "mmkey/types.h"
#include "mmkey/util.h"
#include "mmkey/dmidecode.h"
#include "mmkey/dmiopt.h"
#include "mmkey/dmioem.h"
#endif



#define  MAX_RSA_GEN_BIT			0x2000000

typedef unsigned char   uint8_t;
typedef unsigned int    uint32;
typedef  int			int32;
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

bool  Hex2Bin(const std::string &strHex , std::vector<uint8_t>& vec)
{
	if (strHex.size() % 2 != 0) return false;

	vec.resize(strHex.size() / 2);
	for (size_t i = 0; i < vec.size(); i++)
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

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
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

std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
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

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

bool  is_valid_rsa_bit(uint32 bit)
{
	uint32 _bit = 512;
	uint32 _remain_bit_count = sizeof(uint32) - 8;  //512 = 2^8;
	while (_remain_bit_count > 0)
	{
		_bit <<= 1;
		if (_bit == bit) return true;
		_remain_bit_count --;
	}

	return false;
}

bool GenerateKeys(int bits, std::string& pubkeys , std::string& prikeys)
{

	if (bits > MAX_RSA_GEN_BIT || !is_valid_rsa_bit(bits)) return false;

	R_RSA_PUBLIC_KEY  PubKey;  
	R_RSA_PRIVATE_KEY PriKey;  
	R_RANDOM_STRUCT   RandSt;  
	R_RSA_PROTO_KEY   ProKey;  

	//…˙≥…√‹‘ø∂‘  
	R_RandomCreate(&RandSt);  
	ProKey.bits = bits;//512 or 1024 or 2048  
	ProKey.useFermat4 = 1;  
	if(R_GeneratePEMKeys(&PubKey, &PriKey, &ProKey, &RandSt) != 0) return false;

	
	std::string _pubkeys_hexs = Bin2Hex(&PubKey, sizeof(PubKey));
	std::string sPubkey_base64 = base64_encode((unsigned char*)_pubkeys_hexs.c_str(), _pubkeys_hexs.size());
	pubkeys.assign(sPubkey_base64);


	std::string _prikeys_hexs = Bin2Hex(&PriKey, sizeof(PriKey));
	std::string sPrikeys_base64 = base64_encode((unsigned char*)_prikeys_hexs.c_str(), _prikeys_hexs.size());
	prikeys.assign(sPrikeys_base64);
	
	return true;
}

#define  OUTPUT_BUF_SIZE		0x1024
std::string RSA_Public_Encrypt(std::string& sPubkey , std::string& sMessage)
{
	if (sPubkey.size() == 0 || sMessage.size() == 0) return "";

	std::string sbase64 = base64_decode(sPubkey);
	std::vector<uint8_t> vec;
	Hex2Bin(sbase64 , vec);

	if (vec.size() != sizeof(R_RSA_PUBLIC_KEY)) return  "";

	R_RSA_PUBLIC_KEY*  PubKey = (R_RSA_PUBLIC_KEY*)&vec[0]; 

	int32 bits = PubKey->bits;
	if (bits > MAX_RSA_GEN_BIT || !is_valid_rsa_bit(bits)) return "";

	int32 _once_enc_size = bits / 8 - 11;
	int32 _remain_bytes = sMessage.size();
	uint8_t* pdata = (uint8_t*)sMessage.c_str();

	uint32 _out_buf_size = bits;

	std::vector<uint8_t> _output_vec;
	_output_vec.resize(_out_buf_size);

	R_RANDOM_STRUCT RandSt;
	R_RandomCreate(&RandSt); 

	std::string _enc_res;
	while (_remain_bytes > 0)
	{
		int32 _process_bytes = _remain_bytes > _once_enc_size ? _once_enc_size : _remain_bytes;

		if(RSAPublicEncrypt(&_output_vec[0] , &_out_buf_size , pdata , _process_bytes , PubKey , &RandSt) != 0) return "";

		std::string _hexs = Bin2Hex(&_output_vec[0], _out_buf_size);
		_enc_res.append(_hexs);

		_remain_bytes -= _process_bytes;
		pdata += _process_bytes;
	}


	
	std::string sPub = base64_encode((unsigned char*)_enc_res.c_str(), _enc_res.size());

	return sPub;
}

std::string RSA_Private_Encrypt(std::string& sPriKey , std::string& sMessage)
{

	if (sPriKey.size() == 0 || sMessage.size() == 0) return "";

	std::string sbase64 = base64_decode(sPriKey);
	std::vector<uint8_t> vec;
	Hex2Bin(sbase64 , vec);

	if (vec.size() != sizeof(R_RSA_PRIVATE_KEY)) return  "";

	R_RSA_PRIVATE_KEY*  PriKey = (R_RSA_PRIVATE_KEY*)&vec[0];  

	int32 bits = PriKey->bits;
	if (bits > MAX_RSA_GEN_BIT || !is_valid_rsa_bit(bits)) return "";


	int32 _once_enc_size = bits / 8 - 11;
	int32 _remain_bytes = sMessage.size();
	uint8_t* pdata = (uint8_t*)sMessage.c_str();

	uint32 _out_buf_size = bits;

	std::vector<uint8_t> _output_vec;
	_output_vec.resize(_out_buf_size);


	std::string _enc_res;
	while (_remain_bytes > 0)
	{
		int32 _process_bytes = _remain_bytes > _once_enc_size ? _once_enc_size : _remain_bytes;

		if(RSAPrivateEncrypt(&_output_vec[0] , &_out_buf_size , pdata , _process_bytes , PriKey) != 0) return "";

		std::string _hexs = Bin2Hex(&_output_vec[0], _out_buf_size);
		_enc_res.append(_hexs);

		_remain_bytes -= _process_bytes;
		pdata += _process_bytes;
	}



	std::string sPub = base64_encode((unsigned char*)_enc_res.c_str(), _enc_res.size());

	return sPub;
}

std::string RSA_Public_Decrypt(std::string& sPubkey , std::string& sMessage)
{
	
	std::string sPubKey_base64_decode = base64_decode(sPubkey);
	std::vector<uint8_t> vec;
	Hex2Bin(sPubKey_base64_decode , vec);

	if (vec.size() != sizeof(R_RSA_PUBLIC_KEY)) return  "";

	R_RSA_PUBLIC_KEY*  PubKey = (R_RSA_PUBLIC_KEY*)&vec[0];   

	int32 bits = PubKey->bits;
	if (bits > MAX_RSA_GEN_BIT || !is_valid_rsa_bit(bits)) return "";


	std::string sMsg_base64_decode = base64_decode(sMessage);
	std::vector<uint8_t> vec2;
	Hex2Bin(sMsg_base64_decode , vec2);

	int32 _once_enc_size = bits / 8 - 11;
	int32 _max_enc_block_size = bits / 8;


	uint32 _output_buf_size = bits;
	std::vector<uint8_t> _output_vec;
	_output_vec.resize(_output_buf_size);

	uint8_t* pdata = (uint8_t*)&vec2[0];
	int32 _remain_bytes = vec2.size();

	
	std::string sMsg;
	while (_remain_bytes > 0)
	{
		int32 _process_count = _remain_bytes > _max_enc_block_size ? _max_enc_block_size : _remain_bytes;

		
		if(RSAPublicDecrypt(&_output_vec[0] , &_output_buf_size , pdata , _process_count , PubKey) != 0) return "";

		sMsg.append((const char*)&_output_vec[0] , _output_buf_size);

		_remain_bytes -=  _process_count;
		pdata += _process_count;
	}


	return sMsg;

}


std::string RSA_Private_Decrypt(std::string& sPriKey , std::string& sMessage)
{

	
	std::string sPri_base64_decode = base64_decode(sPriKey);
	std::vector<uint8_t> vec;
	Hex2Bin(sPri_base64_decode , vec);

	if (vec.size() != sizeof(R_RSA_PRIVATE_KEY)) return  "";

	R_RSA_PRIVATE_KEY*  PriKey = (R_RSA_PRIVATE_KEY*)&vec[0];   

	int32 bits = PriKey->bits;
	if (bits > MAX_RSA_GEN_BIT || !is_valid_rsa_bit(bits)) return "";


	std::string sMsg_base64_decode = base64_decode(sMessage);
	std::vector<uint8_t> vec2;
	Hex2Bin(sMsg_base64_decode , vec2);
	

	int32 _once_enc_size = bits / 8 - 11;
	int32 _max_enc_block_size = bits / 8;


	uint32 _output_buf_size = bits;
	std::vector<uint8_t> _output_vec;
	_output_vec.resize(_output_buf_size);

	int32 _remain_bytes = vec2.size();

	uint8_t* pdata = (uint8_t*)&vec2[0];
	std::string sMsg;
	while (_remain_bytes > 0)
	{
		int32 _process_count = _remain_bytes > _max_enc_block_size ? _max_enc_block_size : _remain_bytes;

		
		if(RSAPrivateDecrypt(&_output_vec[0] , &_output_buf_size , pdata , _process_count , PriKey) != 0) return "";

		sMsg.append((const char*)&_output_vec[0] , _output_buf_size);

		_remain_bytes -=  _process_count;
		pdata += _process_count;
	}


	return sMsg;

}


bool Generate_Machine_GUID(std::string& sGUID)
{
#ifdef _WIN32
	CUserGUID2 guid2;
	guid2.TryGetUserGUID();
	CHAR szGuid[64] = {0};
	guid2.FormatString(szGuid, _countof(szGuid));

	sGUID.clear();
	sGUID.assign(szGuid);

#else
	#define MAX_KEY_SIZE	8192
	char infos[MAX_KEY_SIZE];
	memset(infos, 0, MAX_KEY_SIZE);
	getHWInfo(infos);     
	if(strlen(infos) > 0)
	{
		char *buf = NULL;
		EncodeString(infos, &buf, F_KEY_WORD);
		if(buf)
		{
			md5_byte_t digest[16];
			md5_state_t _md5;
			md5_init(&_md5);
			md5_append(&_md5 ,(md5_byte_t*)buf , strlen(buf));
			md5_finish(&_md5 , digest);
			
			std::string s = Bin2Hex(digest ,16);
			sGUID.clear();
			sGUID.assign(s);
			free(buf);
		}
	}
#endif
	
	return sGUID.size() > 0 ? true : false;	
}
