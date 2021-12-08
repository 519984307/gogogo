// RavBase64.h: interface for the CRavBase64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAVBASE64_H__0A4615EF_FAFB_4185_BFF0_6113DB59AB72__INCLUDED_)
#define AFX_RAVBASE64_H__0A4615EF_FAFB_4185_BFF0_6113DB59AB72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//获得加密串的长度
#define ELEN(nLen) ((nLen / 3) * 4 + ((nLen % 3) ? (nLen % 3 + 1) : 0))
//获得解密串的长度
#define DLEN(nLen) ((nLen / 4) * 3 + ((nLen % 4) ? (nLen % 4 - 1) : 0))

class CRavBase64
{
public:
        int DecodeLen(int nLen);
        int EncodeLen(int nLen);
        int DecodeBase64(char * chInBuf, char *chOutBuf);
        int EncodeBase64(char * chInBuf, char *chOutBuf , int nInLen = 0);
        CRavBase64();
        virtual ~CRavBase64();

private:
        char chBaseTable[64];
        void InitialBaseTable(void);
};

#endif // !defined(AFX_RAVBASE64_H__0A4615EF_FAFB_4185_BFF0_6113DB59AB72__INCLUDED_)
