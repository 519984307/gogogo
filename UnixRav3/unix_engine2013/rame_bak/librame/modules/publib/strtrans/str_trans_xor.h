#ifndef __RSLIB_STRINGS_H__
#define __RSLIB_STRINGS_H__
#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif



/********************************************************************
str_trans_xor.cpp
********************************************************************/
#include <vector>
class CStringTransXor
{
public:
	CStringTransXor();
	CStringTransXor(unsigned char * str, int count);
	CStringTransXor(char * str, int count);
	CStringTransXor(const unsigned char * str, int count);
	CStringTransXor(const char * str, int count);

	void Init(unsigned char * str, int count);
	virtual ~CStringTransXor();
	operator unsigned char *();
	operator const unsigned char *();
	operator char *();
	operator const char *();
	operator void *();
	int GetLength();
private:
	void Trans();
	std::vector<unsigned char> _str;
};


#endif //__RSLIB_STRINGS_H__
