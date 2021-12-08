#ifndef _CRYPTOTEST_H_
#define _CRYPTOTEST_H_

#include <string>

using namespace std;

typedef enum {
	GENERAL = 0,
	ECB,
	CBC,
	CFB,
	OFB,
	TRIPLE_ECB,
	TRIPLE_CBC
}CRYPTO_MODE;

string DES_Encrypt(const string& cleartext, const string& key, CRYPTO_MODE mode);
string DES_Decrypt(const string& ciphertext, const string& key, CRYPTO_MODE mode);

string RC4_Encrypt(const string& cleartext, const string& key);
string RC4_Decrypt(const string& ciphertext, const string& key);

#endif //_CRYPTOTEST_H_
