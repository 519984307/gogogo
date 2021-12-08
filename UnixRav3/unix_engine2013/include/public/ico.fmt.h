#pragma once
#include <assert.h>

/*************************
	+-------------+
	| REC_HEAD	  |
	+-------------+
	| REC_NAME    |
	+-------------+
	| PIC_HASH    |
	+-------------+
	| BYTE[n]     |
	+-------------+
	| HIS_GRAM    |
	+-------------+
	| HIS_ITEM[n] |
	+-------------+
*************************/

typedef enum _REC_FIELD
{
	REC_FIELD_NAME	  = 0x01,	
	REC_FIELD_PICHASH = 0x02,
	REC_FIELD_HISGRAM = 0x04,
	REC_FIELD_OPTAND  = 0x08,	
}REC_FIELD;

#pragma pack(1)
typedef struct _REC_HEAD
{
	LE_UI16	wRcrdLen;
	LE_UI8	btRcrdField; 
	LE_UI8	btRcrdType;
	LE_UI32	dwRcrdId;	
}__attribute__((packed)) REC_HEAD;
#pragma pack()

#pragma pack(1)
typedef struct _REC_NAME
{
	LE_UI8	btNameLen;
//	le::U8  btNameChr[1];
}__attribute__((packed)) REC_NAME;
#pragma pack()

#pragma pack(4)
typedef struct _PIC_HASH
{
	LE_UI16	wPercent;
	LE_UI16	wWidth;
	LE_UI16	wHeight;
//	le::U18	bBitArray[1];
}__attribute__((packed)) PIC_HASH;
#pragma pack()

#pragma pack(4)
typedef struct _HIS_ITEM
{
	LE_UI16	wColor;
	LE_UI16	wCount;
}__attribute__((packed)) HIS_ITEM;
#pragma pack()

#pragma pack(1)
typedef struct _HIS_GRAM
{
	LE_UI16		wPercent;
	LE_UI16		wPairCnt;
//	HIS_ITEM	aHisPair[1];
}__attribute__((packed)) HIS_GRAM;
#pragma pack()

//////////////////////////////////////////////////////////////////////////
typedef struct SRecNamePtr
{
private:
	REC_NAME* lpRecName;

public:
	SRecNamePtr(LPVOID lpBuff = NULL)
	{
		*this = lpBuff;
	}

	SRecNamePtr& operator =(LPVOID lpBuff)
	{
		lpRecName = (REC_NAME*)lpBuff;
		return *this;
	}

	LPCSTR GetString()
	{
		if(!lpRecName)
			return NULL;
		return (LPCSTR)(lpRecName+1);
	}

	DWORD GetCharCount()
	{
		DWORD dwStrLen = GetLength();
		if(dwStrLen <= sizeof(REC_NAME))
			return 0;

		dwStrLen -= sizeof(REC_NAME);
		return dwStrLen;
	}

	DWORD GetLength()
	{
		if(!lpRecName)
			return 0;

		return lpRecName->btNameLen;
	}

	DWORD CopyTo(LPSTR szName, DWORD dwMaxCch)
	{
		if(!lpRecName)
			return 0;

		DWORD dwStrLen = GetCharCount();
		if(dwStrLen == 0)
			return 0;

		if(dwStrLen > dwMaxCch - 1)
			dwStrLen = dwMaxCch - 1;

		LPCSTR szVirName = GetString();
		if(szVirName == NULL)
			return 0;

		strncpy(szName, szVirName, dwStrLen);
		szName[dwStrLen] = '\0'; 
		return dwStrLen;
	}
}TRecNamePtr;

typedef struct SPicHashPtr
{
private:
	PIC_HASH* lpHash;

public:
	SPicHashPtr(LPVOID lpBuff)
	{
		*this = lpBuff;
	}

	SPicHashPtr& operator =(LPVOID lpBuff)
	{
		lpHash = (PIC_HASH*)lpBuff;
		return *this;
	}

	PIC_HASH* GetHeader()
	{
		return lpHash;
	}

	DWORD GetLength()
	{
		PIC_HASH* lpHash = GetHeader();
		if(!lpHash)
			return 0;

		DWORD dwHashByt = lpHash->wWidth*lpHash->wHeight;
		dwHashByt >>= 3;
		dwHashByt += sizeof(PIC_HASH);
		return dwHashByt;
	}

	BYTE GetItem(DWORD dwPxlX, DWORD dwPxlY)
	{
		PIC_HASH* lpHash = GetHeader();
		assert(dwPxlX < lpHash->wWidth);
		assert(dwPxlY < lpHash->wHeight);

		DWORD dwIndex = 0;
		dwIndex = dwPxlY * (lpHash->wWidth>>3);
		dwIndex += dwPxlX>>3;

		BYTE bytVal = ((BYTE*)(lpHash+1))[dwIndex];
		return (bytVal>>(dwPxlX&0x7)) & 1;	
	}

	BYTE GetItem(DWORD dwIndex)
	{
		assert(dwIndex < GetCount());
		BYTE bytVal = ((BYTE*)(lpHash+1))[dwIndex>>3];
		BYTE bytIdx = 0x07-dwIndex&07;
		return (bytVal>>bytIdx)&1;
	}

	DWORD GetCount()
	{
		return lpHash->wWidth*lpHash->wHeight;
	}
}TPicHashPtr;

typedef struct SHisGramPtr
{
private:
	HIS_GRAM* lpGram;

public:
	SHisGramPtr(LPVOID lpBuff)
	{
		*this = lpBuff;
	}

	SHisGramPtr& operator =(LPVOID lpBuff)
	{
		lpGram = (HIS_GRAM*)lpBuff;
		return *this;
	}

	HIS_GRAM* GetHeader()
	{
		return lpGram;
	}

	DWORD GetLength()
	{
		HIS_GRAM* lpGram = GetHeader();
		if(!lpGram)
			return 0;

		DWORD dwHashByt = sizeof(HIS_ITEM)*lpGram->wPairCnt;
		dwHashByt += sizeof(HIS_GRAM);		
		return dwHashByt;
	}

	HIS_ITEM* GetItem(DWORD dwIndex)
	{
		HIS_GRAM* lpGram = GetHeader();
		assert(dwIndex < lpGram->wPairCnt);
		HIS_ITEM* lpItem = &((HIS_ITEM*)(lpGram+1))[dwIndex];
		return lpItem;
	}
	
	HIS_ITEM* GetItemByColor(WORD wColor)
	{
		LONG lBegIdx = 0;
		LONG lEndIdx = (LONG)GetCount() - 1;
		while(lEndIdx >= lBegIdx)
		{
			LONG lCurIdx = (lEndIdx + lBegIdx) >> 1;
			HIS_ITEM* lpItm = GetItem((LONG)lCurIdx);
			if(wColor == lpItm->wColor)
			{
				return lpItm;
			}
			else
			{
				if(wColor < lpItm->wColor)
				{
					lEndIdx = lCurIdx - 1;
				}
				else
				{
					lBegIdx = lCurIdx + 1;
				}
			}
		};
		return NULL;
	}

	DWORD GetCount()
	{
		HIS_GRAM* lpGram = GetHeader();
		return lpGram->wPairCnt;
	}
}THisGramPtr;

typedef struct SIconSignPtr
{
private:
	REC_HEAD*	lpRcrdHdr;

public:
	TRecNamePtr	RcrdName;
	TPicHashPtr PicHash;
	THisGramPtr HisGram;

public:
	SIconSignPtr(LPVOID lpBuff)
		: lpRcrdHdr(NULL)
		, RcrdName(NULL)
		, PicHash(NULL)
		, HisGram(NULL)
	{
		LPBYTE lpBytBuf = (LPBYTE)lpBuff;
		if(lpBytBuf != NULL)
		{
			lpRcrdHdr	= (REC_HEAD*)lpBytBuf;	lpBytBuf += sizeof(REC_HEAD);

			WORD wRcrdField = lpRcrdHdr->btRcrdField;
			if(HasField(REC_FIELD_NAME)){
				RcrdName	= (LPVOID)lpBytBuf;		
				lpBytBuf += RcrdName.GetLength();
			}

			if(HasField(REC_FIELD_PICHASH)){
				PicHash   = (LPVOID)lpBytBuf;	
				lpBytBuf += PicHash.GetLength();
			}

			if(HasField(REC_FIELD_HISGRAM)){
				HisGram	= (LPVOID)lpBytBuf;	
				lpBytBuf += HisGram.GetLength();
			}			
		}		
	}

	BOOL HasField(REC_FIELD lpField)
	{
		if(!lpField)
			return FALSE;

		WORD wRcrdField = lpRcrdHdr->btRcrdField;
		return (wRcrdField & lpField) > 0? TRUE: FALSE;
	}

	REC_HEAD* GetHeader()
	{
		return lpRcrdHdr;
	}

	DWORD GetLength()
	{
		if(!lpRcrdHdr)
			return 0;

		DWORD dwTltLen = sizeof(REC_HEAD);
		dwTltLen += RcrdName.GetLength();
		dwTltLen += PicHash.GetLength();
		dwTltLen += HisGram.GetLength();
		return dwTltLen;
	}	

	
}TIconSignPtr;

