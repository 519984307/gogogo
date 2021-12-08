#ifndef __CPTDEF_H__
#define __CPTDEF_H__

// Cross-platform type definition

typedef long LONG;

typedef unsigned char BYTE;
typedef BYTE* LPBYTE;
typedef LPBYTE PBYTE;
typedef unsigned int DWORD;
typedef unsigned long DDWORD;
typedef DWORD* LPWORD;
typedef LPWORD PDWORD;

typedef int BOOL;

#ifndef TRUE
#define TRUE (1)
#endif // TRUE

#ifndef FALSE
#define FALSE (0)
#endif // FALSE

#endif // __CPTDEF_H__

