#include "../public/pubinc.h"

#include "rsstring.h"
#include "re_stdio.h"
#include "re_string.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

RS_BOOL IString::ReAlloc(RS_DWORD dwMinSize)
{	
	if(m_dwBufferSize < dwMinSize)
	{	
		if(!HelpRealloc(dwMinSize)) { return RS_FALSE; }
	}
	if (RS_NULL == m_pszBuffer)
		return RS_FALSE;

	Empty();

	return RS_TRUE;
}

RS_BOOL IString::SetLength(RS_DWORD dwDataLen)
{	
	if (RS_NULL == m_pszBuffer || dwDataLen >= m_dwBufferSize)
		return RS_FALSE;

	m_dwDataLength = dwDataLen;
	m_pszBuffer[dwDataLen] = 0;

	return RS_TRUE;
}

RS_BOOL IString::Copy(const RS_VOID * pCopy, RS_DWORD dwLength)
{
	if (RS_NULL == pCopy || 0 == dwLength || pCopy == m_pszBuffer)
		return RS_FALSE;

	if(m_dwBufferSize < dwLength + 1)
	{	
		if(!HelpRealloc(dwLength)) { return RS_FALSE; }
	}
	memcpy(m_pszBuffer, pCopy, dwLength);

	SetLength(dwLength);

	return RS_TRUE;
}

BOOL IString::Copy( const RS_VOID *pCopy, DWORD dwLength, char *OutChar,DWORD dwCount )
{
	if (RS_NULL == pCopy || 0 == dwLength || pCopy == m_pszBuffer)
		return RS_FALSE;

	if(m_dwBufferSize < dwLength + 1)
	{	
		if(!HelpRealloc(dwLength)) { return RS_FALSE; }
	}
	if (0==dwCount||NULL==OutChar)
	{
		memcpy(m_pszBuffer, pCopy, dwLength);
	}
	else
	{
		char *dst=m_pszBuffer;
		const RS_VOID *src=pCopy;
		size_t count=dwLength;
		dwLength=0;
		DWORD i=0;
		while (count--) {
			for (i=0;i<dwCount;i++)
			{
				if (OutChar[i]==*(char *)src)
				{
					break;
				}
			}
			if (i==dwCount)
			{
				*(char *)dst = *(char *)src;
				dst = (char *)dst + 1;
				dwLength++;
			}
			src = (char *)src + 1;
		}
	}
	
	
	
	SetLength(dwLength);

	return RS_TRUE;
}

RS_BOOL IString::UnicodeToAnsi(const RS_WCHAR *pwszUnicode)
{
	if (RS_NULL == pwszUnicode || (RS_VOID *)pwszUnicode == (RS_VOID *)m_pszBuffer)
		return RS_FALSE;

	RS_DWORD dwSize = (RS_DWORD)wcslen(pwszUnicode) * sizeof(RS_WCHAR);

	if (RS_NULL == m_pszBuffer)
	{
		if(!HelpMalloc(dwSize)) { return RS_FALSE; }
	}
	else
	{
		if (m_dwBufferSize < dwSize + 1)
		{
			if(!HelpRealloc(dwSize)) { return RS_FALSE; }
		}
	}

	if (RS_NULL == m_pszBuffer)
		return RS_FALSE;

	RS_INT nLen = (RS_INT)wcstombs(m_pszBuffer,  pwszUnicode, m_dwBufferSize - sizeof(RS_WCHAR));
	if (-1 == nLen)
		return RS_FALSE;

	SetLength(nLen);

	return RS_TRUE;
}


RS_BOOL IString::Append(const RS_VOID *pAppend, RS_DWORD dwAppendLen)
{	
	if (RS_NULL == pAppend)
		return RS_FALSE;

	if (0 == dwAppendLen)
		return RS_TRUE;

	if (RS_NULL == m_pszBuffer)
	{
		if(!HelpMalloc(dwAppendLen)) { return RS_FALSE; }
		memcpy(m_pszBuffer, pAppend, dwAppendLen);
		SetLength(dwAppendLen);
		return RS_TRUE;
	}

	RS_DWORD dwLen = m_dwDataLength + dwAppendLen;

	if (m_dwBufferSize > dwLen + 1)
	{
		memcpy(m_pszBuffer + m_dwDataLength, pAppend, dwAppendLen);
	}
	else
	{
		RS_CHAR *pTemp = m_pszBuffer;

		if(!HelpMalloc(dwLen)) { return RS_FALSE; }

		memcpy(m_pszBuffer, pTemp, m_dwDataLength);
		memcpy(m_pszBuffer + m_dwDataLength, pAppend, dwAppendLen);

		delete [] pTemp;			
	}
	SetLength(dwLen);

	return RS_TRUE;
}

RS_BOOL IString::Append(RS_CHAR chAppend)
{
	if (RS_NULL == m_pszBuffer)
	{
		if(!HelpMalloc(1)) { return RS_FALSE; }
		m_pszBuffer[0] = chAppend;
		SetLength(1);

		return RS_TRUE;
	}

	RS_DWORD dwLenNew = m_dwDataLength + 1;

	if (m_dwBufferSize > dwLenNew + 1)
	{
		m_pszBuffer[m_dwDataLength] = chAppend;
	}
	else
	{
		RS_CHAR *pTemp = m_pszBuffer;

		if(!HelpMalloc(dwLenNew)) { return RS_FALSE; }

		memcpy(m_pszBuffer, pTemp, m_dwDataLength);
		m_pszBuffer[m_dwDataLength] = chAppend;

		delete [] pTemp;
	}

	SetLength(dwLenNew);

	return RS_TRUE;
}

RS_BOOL IString::FindChar(RS_DWORD dwStart, 
						  RS_DWORD dwEnd, 
						  RS_CHAR cFind, 
						  RS_DWORD &dwPos) const
{
	if (dwStart >= m_dwDataLength)
		return RS_FALSE;

	RS_CHAR * pDest = (RS_CHAR *)memchr(m_pszBuffer + dwStart, cFind, dwEnd - dwStart);

	if (RS_NULL == pDest)
		return RS_FALSE;

	dwPos = (RS_DWORD)(pDest - m_pszBuffer);

	return RS_TRUE;
}

RS_BOOL IString::ReverseChar(RS_CHAR chFind,  
							 RS_DWORD &dwPos) const
{
	if (RS_NULL == m_pszBuffer)
		return RS_FALSE;

	RS_CHAR *p = strrchr(m_pszBuffer, chFind);
	if (RS_NULL == p)
		return RS_FALSE;

	dwPos = (RS_DWORD)(p - m_pszBuffer);

	return RS_TRUE;
}

RS_INT IString::Format(const RS_CHAR *pszFormat, ...)
{
	if (RS_NULL == pszFormat || RS_NULL == m_pszBuffer)
		return -1;

	va_list vaArgList;

	va_start(vaArgList, pszFormat);

	RS_INT nPrinted = _vsnprintf((RS_CHAR *)m_pszBuffer, m_dwBufferSize, pszFormat, vaArgList);

	va_end(vaArgList);

	if (-1 == nPrinted)
	{
		SetLength(0);
		return -1;
	}

	SetLength(nPrinted);

	return nPrinted;
}

////////////////////////////////////////////////////////////////////////////////
//			  函数名：	RS_BOOL CMPString::Trim()
//				用途：	清除字符串前后的空格和制表符
//	        参数说明：	无
//		返回结果说明：	如果成功返回 RS_TRUE，否则返回 RS_FALSE
//	对全局变量的影响：	无
////////////////////////////////////////////////////////////////////////////////
RS_BOOL IString::TrimLeft()
{
	if (RS_NULL == m_pszBuffer || 0 == m_dwDataLength)
		return RS_TRUE;

	RS_CHAR *pStart = m_pszBuffer, *pEnd = m_pszBuffer + m_dwDataLength;
	RS_CHAR ch;

	while (pStart < pEnd)
	{
		ch = *pStart;
		if (' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch)
		{
			pStart++;
		}
		else
		{
			break;
		}
	}
	if (pStart == m_pszBuffer)
	{
		return RS_TRUE;
	}
	RS_DWORD dwLen = (RS_DWORD)(pEnd - pStart);
	if (dwLen > 0)
	{
		memmove(m_pszBuffer, pStart, dwLen);
	}

	SetLength(dwLen);
	return RS_TRUE;
}
RS_BOOL IString::TrimRight()
{
	if (RS_NULL == m_pszBuffer || 0 == m_dwDataLength)
		return RS_TRUE;

	RS_DWORD dwLen = m_dwDataLength;
	RS_CHAR ch;

	while (dwLen)
	{
		ch = m_pszBuffer[dwLen - 1];
		if (' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch)
		{
			dwLen--;
		}
		else
		{
			break;
		}
	}

	SetLength(dwLen);

	return RS_TRUE;
}

RS_BOOL IString::Replace(RS_CHAR chReplaced, RS_CHAR chReplacing)
{
	if (RS_NULL == m_pszBuffer) return RS_FALSE;

	for(RS_DWORD i = 0; i < m_dwDataLength; i++)
	{
		if (m_pszBuffer[i] == chReplaced) 
			m_pszBuffer[i] = chReplacing;
	}
	return RS_TRUE;
}

BOOL IString::Replace( CHAR chReplaced[], 
					  DWORD dwCount,
					  CHAR chReplacing )
{
	if (RS_NULL == m_pszBuffer) return RS_FALSE;

	for(RS_DWORD i = 0; i < m_dwDataLength; i++)
	{
		for (int j=0;j<dwCount;j++)
		{
			if (m_pszBuffer[i] == chReplaced[j]) 
				m_pszBuffer[i] = chReplacing;
		}
	}
	return RS_TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//			  函数名：	RS_BOOL CMPString::Lower()
//				用途：	将字符串中的字符转为小写
//	        参数说明：	无
//		返回结果说明：	如果成功返回 RS_TRUE，否则返回 RS_FALSE
//	对全局变量的影响：	无
////////////////////////////////////////////////////////////////////////////////

RS_BOOL IString::Lower()
{
	if (RS_NULL == m_pszBuffer || 0 == m_dwBufferSize)
		return RS_TRUE;

	RS_CHAR cSub = 'a' - 'A';

	for (RS_DWORD i = 0; i < m_dwDataLength; i++)
	{
		if ('A' <= m_pszBuffer[i] && m_pszBuffer[i]<= 'Z')
			m_pszBuffer[i] += cSub;
	}

	return RS_TRUE;
};
