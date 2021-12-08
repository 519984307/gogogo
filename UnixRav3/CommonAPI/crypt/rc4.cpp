#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "openssl/rc4.h"
#include "rc4.h"

using namespace std;

string RC4_Encrypt(const string& cleartext, const string& key)
{
    string str;
#ifndef WITHOUT_CRYPTO
	RC4_KEY rc4key;
	unsigned char* tmp = new unsigned char[cleartext.length() + 1];
	memset(tmp, 0, cleartext.length() + 1);

	RC4_set_key(&rc4key, key.length(), (const unsigned char*)key.c_str());
	RC4(&rc4key, cleartext.length(), (const unsigned char*)cleartext.c_str(), tmp);

	str.assign((char*)tmp,((char*)tmp) + cleartext.length());

	delete [] tmp;
#endif

	return str;
}

string RC4_Decrypt(const string& ciphertext, const string& key)
{
    string str;
#ifndef WITHOUT_CRYPTO
	RC4_KEY rc4key;
    unsigned char* tmp = new unsigned char[ciphertext.size() + 1];
    memset(tmp, 0, ciphertext.size() + 1);

	RC4_set_key(&rc4key, key.length(), (const unsigned char*)key.c_str());
    RC4(&rc4key, ciphertext.size(), (const unsigned char*)ciphertext.c_str(), tmp);

    str.assign((char*)tmp,((char*)tmp) + ciphertext.size());
	delete [] tmp;
#endif

	return str;
}
