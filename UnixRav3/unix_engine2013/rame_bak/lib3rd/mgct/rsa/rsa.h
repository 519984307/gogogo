/*
	RSA.H - header file for RSA.C

    Copyright (c) J.S.A.Kapp 1994 - 1996.

	RSAEURO - RSA Library compatible with RSAREF 2.0.

	All functions prototypes are the Same as for RSAREF.
	To aid compatiblity the source and the files follow the
	same naming comventions that RSAREF uses.  This should aid
        direct importing to your applications.

	This library is legal everywhere outside the US.  And should
	NOT be imported to the US and used there.

	RSA Routines Header File.

	Revision 1.00 - JSAK.
*/

#include "rsaeuro.h"

#ifdef __cplusplus
extern "C" {
#endif

int RSAPublicEncrypt(unsigned char *, unsigned int *, unsigned char *, unsigned int, R_RSA_PUBLIC_KEY *, R_RANDOM_STRUCT *);
int RSAPrivateEncrypt( unsigned char *, unsigned int *, unsigned char *, unsigned int, R_RSA_PRIVATE_KEY *);
int RSAPublicDecrypt( unsigned char *, unsigned int *, unsigned char *, unsigned int, R_RSA_PUBLIC_KEY * );
int RSAPrivateDecrypt( unsigned char *, unsigned int *, unsigned char *, unsigned int, R_RSA_PRIVATE_KEY * );

int R_GeneratePEMKeys(
	R_RSA_PUBLIC_KEY *publicKey,    /* new RSA public key */
	R_RSA_PRIVATE_KEY *privateKey,  /* new RSA private key */
	R_RSA_PROTO_KEY *protoKey,      /* RSA prototype key */
	R_RANDOM_STRUCT *randomStruct );  /* random structure */

void R_RandomCreate(
	R_RANDOM_STRUCT *random);

#ifdef __cplusplus
}
#endif

/*
int RSAPublicEncrypt PROTO_LIST((unsigned char *, unsigned int *, unsigned char *, unsigned int,
	R_RSA_PUBLIC_KEY *, R_RANDOM_STRUCT *));
int RSAPrivateEncrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
   R_RSA_PRIVATE_KEY *));
int RSAPublicDecrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
    R_RSA_PUBLIC_KEY *));
int RSAPrivateDecrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
    R_RSA_PRIVATE_KEY *));
*/