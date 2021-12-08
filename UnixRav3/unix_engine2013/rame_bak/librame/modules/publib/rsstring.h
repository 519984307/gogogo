#ifndef __ISTRING_H__
#define __ISTRING_H__

#include "engutils.h"

class IString 
{
public:
	// Constructions / Destruction
	IString()
		:m_pszBuffer(NULL), m_dwBufferSize(0), m_dwDataLength(0)
	{}

	IString(DWORD dwSize)
		:m_pszBuffer(NULL), m_dwBufferSize(0), m_dwDataLength(0)
	{	
		HelpMalloc(dwSize);
	};
	
	IString(const RS_VOID* pBuffer, 
		DWORD dwLength)
		:m_pszBuffer(NULL), m_dwBufferSize(0), m_dwDataLength(0)
	{
		Copy(pBuffer, dwLength);
	}

	IString(const IString& buffer)
		:m_pszBuffer(NULL), m_dwBufferSize(0), m_dwDataLength(0)
	{
		Copy(buffer.m_pszBuffer, buffer.m_dwDataLength);
	}

	IString(const CHAR* pszBuffer)
		:m_pszBuffer(NULL), m_dwBufferSize(0), m_dwDataLength(0)
	{
		if (pszBuffer)
		{
			Copy(pszBuffer, (DWORD)strlen(pszBuffer));
		}
	}
	
	virtual ~IString()
	{
		HelpFree();
	};

	//As an array
	DWORD GetLength() const
	{
		return m_dwDataLength;
	}
	RS_VOID SetLength()
	{
		m_dwDataLength = (m_pszBuffer) ? (DWORD)strlen(m_pszBuffer) : 0;
	}
	
	BOOL SetLength(DWORD dwDataLen);

	BOOL IsEmpty() const
	{
		return (NULL == m_pszBuffer || 0 == m_dwDataLength);
	}

	RS_VOID Empty()
	{
		SetLength(0);
	}

	RS_VOID Zero()
	{
		if (m_pszBuffer) 
		{
			memset(m_pszBuffer, 0, m_dwBufferSize);
		}
	}

	operator CHAR* () const
	{
		return m_pszBuffer;
	}

	//Assignment / Concatenation
	
	const IString & operator = (const CHAR *pszCopy)
	{
		if (pszCopy)
		{
			Copy(pszCopy, (DWORD)strlen(pszCopy));
		}
		return *this;
	}
	const IString & operator = (const IString &str)
	{
		Copy(str.m_pszBuffer, str.m_dwDataLength);
		return *this;
	}
	const IString & operator = (const RS_CSTR &cstrCopy)
	{
		Copy(cstrCopy.string, (DWORD)cstrCopy.size);
		return *this;
	}
	BOOL Copy(const RS_VOID *pCopy, 
		DWORD dwLength);
	BOOL Copy(const RS_VOID *pCopy, 
		DWORD dwLength,
		char *OutChar,
		DWORD dwCount);
	const IString & operator = (const RS_WCHAR *pwszUnicode)
	{
		UnicodeToAnsi(pwszUnicode);
		return *this;
	}
	BOOL UnicodeToAnsi(const RS_WCHAR *pwszUnicode);
	
	IString & operator += (const CHAR *pszAppend)
	{
		if (pszAppend)
		{
			Append(pszAppend, (DWORD)strlen(pszAppend));
		}
		return *this;
	}
	IString & operator += (const RS_CSTR &cstrAppend)
	{
		Append(cstrAppend.string, (DWORD)cstrAppend.size);
		return *this;
	}
	IString & operator += (const IString &strAppend)
	{
		Append(strAppend.m_pszBuffer, strAppend.m_dwDataLength);
		return *this;
	}
	BOOL Append(const RS_VOID *pAppend, 
		DWORD dwAppendLen);

	IString & operator += (CHAR chAppend)
	{
		Append(chAppend);
		return *this;
	}
	BOOL Append(CHAR chAppend);

	
	//Comparison
	BOOL operator == (const CHAR *pszCompare) const
	{
		if (NULL == pszCompare || NULL == m_pszBuffer)
			return FALSE;

		return (0 == strcmp(m_pszBuffer, pszCompare));
	}
	BOOL operator == (const RS_CSTR cstrCompare) const
	{
		return Compare(cstrCompare.string, (DWORD)cstrCompare.size);
	}
	BOOL operator == (const IString &strCompare) const
	{
		return Compare(strCompare.m_pszBuffer, strCompare.m_dwDataLength);
	}
	BOOL Compare(const RS_VOID *pCompare, 
		DWORD dwLength) const
	{
		if (NULL == pCompare || NULL == m_pszBuffer)
		{
			return FALSE;
		}
		
		return (0 == memcmp(m_pszBuffer, pCompare, dwLength));	
	}

	BOOL CompareNoCase(const CHAR *pszCompare) const
	{
		if (NULL == m_pszBuffer || NULL == pszCompare)
		{
			return FALSE;
		}
		
		return (0 == rs_stricmp(m_pszBuffer, pszCompare));
	}

	//Other Conversions
	BOOL Lower();

	BOOL Replace(CHAR chReplaced, 
		CHAR chReplacing);
	BOOL Replace(CHAR chReplaced[], 
		DWORD dwCount,
		CHAR chReplacing);
	INT Format(const CHAR *pszFormat, ...);

	BOOL TrimLeft();
	BOOL TrimRight();

	BOOL Trim()
	{
		if (!TrimRight())
			return FALSE;
		if (!TrimLeft())
			return FALSE;
		return RS_TRUE;
	}
	

	//Searching
	BOOL ReverseChar(CHAR chFind, 
		DWORD &dwPos) const;

	BOOL FindChar(DWORD dwStart, 
		DWORD dwEnd, 
		CHAR cFind, 
		DWORD &dwPos) const;

	//Buffer
	CHAR* GetBuffer(DWORD dwStart) const
	{
		return ((dwStart >= m_dwBufferSize) ? NULL : (m_pszBuffer + dwStart));
	}
	RS_BYTE* GetBuffer() const
	{
		return (RS_BYTE *)m_pszBuffer;
	}
		
	BOOL ReAlloc(DWORD dwMinSize);

	//////////////////////////////////////////////////////////////////////////
	//Additional function
	//By: Yang Le
	//Date: 2008-8-4
	//Convert a char * directly to IString
	static void CreateIStringDirectlyFromArray(CHAR *pszBuffer,IString &strValue)
	{
		if (pszBuffer)
		{
			DWORD Len=(DWORD)strlen(pszBuffer);
			delete [] strValue.m_pszBuffer;
			strValue.m_pszBuffer=pszBuffer;
			strValue.m_dwBufferSize=DWORD(Len+1);
			strValue.m_dwDataLength=DWORD(Len);
		}
	}

	void ConvertFromArrayDirectely(CHAR * pszBuffer)
	{
		if (pszBuffer)
		{
			DWORD Len=(DWORD)strlen(pszBuffer);
			delete [] m_pszBuffer;
			this->m_pszBuffer=pszBuffer;
			this->m_dwBufferSize=DWORD(Len+1);
			this->m_dwDataLength=DWORD(Len);
		}
	}
	//End of Addition
	//////////////////////////////////////////////////////////////////////////
protected:
	CHAR* m_pszBuffer;			//缓冲区
	DWORD m_dwBufferSize;	//缓冲区长度
	DWORD m_dwDataLength;

	enum _CONST
	{
		REDUNDANCE = 0x40,
	};

	RS_VOID HelpFree()
	{
		delete [] m_pszBuffer;
		m_pszBuffer = NULL;
	}

	bool HelpMalloc(DWORD dwBufferSize)
	{
		m_dwBufferSize = dwBufferSize + REDUNDANCE;
		if((m_pszBuffer = RSNEW CHAR[m_dwBufferSize])) { return true; }
		m_dwBufferSize = 0;
		return false;
	}

	bool HelpRealloc(DWORD dwBufferSize)
	{
		delete [] m_pszBuffer;
		return HelpMalloc(dwBufferSize);
	}
};


#endif //__ISTRING_H__

