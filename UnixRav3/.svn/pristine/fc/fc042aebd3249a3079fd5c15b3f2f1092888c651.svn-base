#ifndef RS_UPGRADE_CODE_H
#define RS_UPGRADE_CODE_H

#define LONG	long
#define LPTSTR	char*
#define LPCTSTR const char*
#define DWORD	unsigned long
#define WORD	short
#define BOOL	bool
#define FALSE	0
#define CHAR	char
#define INVALID_HANDLE_VALUE	0
#define BYTE	unsigned char
#define TCHAR	char
#define _T		


LONG RsUpgradeEncrypt(LPTSTR pOut, LONG lOutBufSize, LPCTSTR pUserID, LPCTSTR pSn, DWORD dwTimeSetup, DWORD dwTimeOver, LONG lType, LONG lValidate);
LONG RsUpgradeEncrypt(LPTSTR pOut, LONG lOutBufSize, LPCTSTR pIn);

LONG RsUpgradeDecrypt(LPTSTR pOut, LONG lOutBufSize, LPCTSTR pIn);

#endif // RS_UPGRADE_CODE_H
