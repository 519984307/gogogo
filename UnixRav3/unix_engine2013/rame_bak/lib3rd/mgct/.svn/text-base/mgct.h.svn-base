#ifndef __MGCT_H__
#define __MGCT_H__


#ifdef __cplusplus
extern "C" {
#endif

	int Generate_RAS_Keys(int bits , OUT LPSTR lpPubKey , int* pub_buf_size  , OUT LPSTR lpPriKey ,  int* pri_buf_size);

	int RSA_Public_Encrypt_String(LPSTR lpPubkey, LPSTR lpMsg , OUT LPSTR lpEnc , int* enc_buf_size);
	int RSA_Private_Encrypt_String(LPSTR lpPriKey , LPSTR lpMsg , OUT LPSTR lpEnc ,int* enc_buf_size);

	int RSA_Public_Decrypt_String(LPSTR lpPubkey , LPSTR lpEncMsg , OUT LPSTR lpMsg , int* msg_buf_size);
	int RSA_Private_Decrypt_String(LPSTR lpPubkey , LPSTR lpEncMsg , OUT LPSTR lpMsg , int* msg_buf_size);

	bool Create_Machine_GUID(OUT LPSTR lpGuid , int guid_buf_size);

#ifdef __cplusplus
}
#endif



#endif
