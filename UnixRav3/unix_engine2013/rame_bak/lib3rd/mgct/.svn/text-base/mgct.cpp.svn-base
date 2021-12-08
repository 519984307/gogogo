#include "stdafx.h"
#include "mgct.h"

#include "rsaalg.h"

#define  ERR_FAILD			-1
#define  ERR_OUTOFMEMORY	-2
#define  ERR_SUCCESS		0



int Generate_RAS_Keys(int bits , LPSTR lpPubKey , int* pub_buf_size  , LPSTR lpPriKey , int* pri_buf_size)
{
	if (!(bits == 1024 || bits == 2048 || bits == 4096)) return ERR_FAILD;
	if (lpPubKey == NULL || pub_buf_size == 0 || lpPriKey == 0 || pub_buf_size == 0) return ERR_FAILD;

	std::string sPub;
	std::string sPri;
	GenerateKeys(bits , sPub , sPri);

	if (sPub.size() == 0 || sPri.size() == 0) return ERR_FAILD;

	if ( sPub.size() > *pub_buf_size || sPri.size() > *pri_buf_size)
	{
		*pub_buf_size = sPub.size();
		*pri_buf_size = sPri.size();

		return ERR_OUTOFMEMORY;
	}

	strcpy(lpPubKey , sPub.c_str());
	strcpy(lpPriKey , sPri.c_str());

	return ERR_SUCCESS;
}

int RSA_Public_Encrypt_String(LPSTR lpPubkey, LPSTR lpMsg , LPSTR lpEnc , int* enc_buf_size)
{
	if (lpPubkey == NULL || lpMsg == NULL || lpEnc == NULL || enc_buf_size == 0) return ERR_FAILD;

	std::string sEnc = RSA_Public_Encrypt(std::string(lpPubkey) , std::string(lpMsg));
	if (sEnc.size() == 0) return ERR_FAILD;
	
	if (sEnc.size() > *enc_buf_size)
	{
		*enc_buf_size = sEnc.size();
		return ERR_OUTOFMEMORY;
	} 

	strcpy(lpEnc , sEnc.c_str());

	return ERR_SUCCESS;
}


int RSA_Private_Encrypt_String(LPSTR lpPriKey , LPSTR lpMsg , LPSTR lpEnc ,int* enc_buf_size)
{
	if (lpPriKey == NULL || lpMsg == NULL || lpEnc == NULL || enc_buf_size == 0) return ERR_FAILD;

	std::string sEnc = RSA_Private_Encrypt(std::string(lpPriKey) , std::string(lpMsg));
	if (sEnc.size() == 0) return ERR_FAILD;
	
	if (sEnc.size() > *enc_buf_size) 
	{
		*enc_buf_size = sEnc.size();
		return ERR_OUTOFMEMORY;
	}

	strcpy(lpEnc , sEnc.c_str());

	return ERR_SUCCESS;
}

int RSA_Public_Decrypt_String(LPSTR lpPubkey , LPSTR lpEncMsg , LPSTR lpMsg , int* msg_buf_size)
{
	if (lpPubkey == NULL || lpMsg == NULL || lpMsg == NULL || msg_buf_size == 0) return ERR_FAILD;

	std::string sDec = RSA_Public_Decrypt(std::string(lpPubkey) , std::string(lpEncMsg));
	if (sDec.size() == 0) return ERR_FAILD;
	
	if (sDec.size() > *msg_buf_size)
	{
		*msg_buf_size = sDec.size();
		return ERR_OUTOFMEMORY;
	}

	strcpy(lpMsg , sDec.c_str());

	return ERR_SUCCESS;
}


int RSA_Private_Decrypt_String(LPSTR lpPrikey , LPSTR lpEncMsg , LPSTR lpMsg , int* msg_buf_size)
{

	if (lpPrikey == NULL || lpMsg == NULL || lpMsg == NULL || msg_buf_size == 0) return ERR_FAILD;

	std::string sDec = RSA_Private_Decrypt(std::string(lpPrikey) , std::string(lpEncMsg));
	if (sDec.size() == 0) return ERR_FAILD;

	if (sDec.size() > *msg_buf_size)
	{
		*msg_buf_size = sDec.size();
		return ERR_OUTOFMEMORY;
	}

	strcpy(lpMsg , sDec.c_str());

	return true;
}

bool Create_Machine_GUID(LPSTR lpGuid , int guid_buf_size)
{

	std::string s ;
	Generate_Machine_GUID(s);
	if (s.c_str() == 0 || s.size() > guid_buf_size) return false;

	strcpy( lpGuid , s.c_str());



	return true;
}