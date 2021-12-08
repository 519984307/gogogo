#include "str_trans_xor.h"


CStringTransXor::CStringTransXor()
{
}


CStringTransXor::CStringTransXor(unsigned char * str, int count)
{
 	Init(str, count);
}
CStringTransXor::CStringTransXor(char * str, int count)
{
	Init((unsigned char *)str, count);
}
CStringTransXor::CStringTransXor(const unsigned char *str, int count)
{
	Init((unsigned char *)str, count);
}
CStringTransXor::CStringTransXor(const char *str, int count)
{
	Init((unsigned char *)str, count);
}

void CStringTransXor::Init(unsigned char * str, int count)
{
	if (count<0) return;
	_str.resize(count+1);
	memcpy(&_str[0], str, count);
	_str[count] = 0;
	Trans();
}

CStringTransXor::~CStringTransXor()
{
	if ( 0 == _str.size() ) return;
	memset(&_str[0], 0, _str.size());
}

int CStringTransXor::GetLength()
{
	return (int)_str.size()-1;
}

void CStringTransXor::Trans()
{
#ifdef _WIN32
	#pragma code_seg(".text")
	__declspec(allocate(".text"))
#endif
	static unsigned char key[16] = {0x55, 0x8B, 0xEC, 0x33, 0xC0, 0x8B, 0xE5, 0x5D, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,}; //  ^_^
	int i=0;
	for (int i=0; i<(int)(_str.size()-1); ++i) _str[i] ^= key[(i+1) % 16]; //最后一个0不要参与运算！ //shit! 因为工具里面先加了1。
}

CStringTransXor::operator unsigned char *()
{
	if (0 == _str.size() ) return 0;
	return &_str[0];
}
CStringTransXor::operator const unsigned char *()
{
	return (const unsigned char *)operator unsigned char *();
}

CStringTransXor::operator char *()
{
	return (char *)operator unsigned char *();
}

CStringTransXor::operator const char *()
{
	return (const char *)operator unsigned char *();
}
CStringTransXor::operator void *()
{
	return (void *)operator unsigned char *();
}
