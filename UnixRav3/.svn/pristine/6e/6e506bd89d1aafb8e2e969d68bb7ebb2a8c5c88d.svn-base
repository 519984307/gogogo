#include "StdAfx.h"
#include <functional>
#include <locale>


void remove(std::string& s, const char c)
{
	int nPos = s.find(c);
	while(-1 != nPos)
	{
		s.erase(nPos,1);
		nPos = s.find(c);
	};	
}

void toupper_str(std::string& str)
{
   transform(str.begin(), str.end(), str.begin(), (int(*)(int))toupper);
}

CStringA::CStringA(){};
CStringA::CStringA(const char* pstr){mString = pstr;};
CStringA::CStringA(const std::string & s){mString = s;};

   int CStringA::GetLength()
   {
	   int nLen = strlen(GetBuffer());
	   return nLen;
	   return mString.size();
   }
   std::string CStringA::Mid(int iPos, int iLength)
   {
	  if(-1 == iLength)
		  return mString.substr(iPos);
      return mString.substr(iPos, iLength);
   };

	CStringA CStringA::Right(int count)
	{
		int nAllCount = mString.size();
		if(nAllCount <= count)
			return *this;
		std::string s = Mid(nAllCount-count, count);
		return CStringA(s);
	}
	
	CStringA CStringA::Left(int count)
	{
		std::string s = Mid(0, count);
		return CStringA(s);
	}

	CStringA CStringA::operator+(const CStringA& r)
	{
		std::string s;
		s = mString;
		s += r.mString;
		return CStringA(s);
	}

	CStringA& CStringA::operator+=(const CStringA& r)
	{
		mString += r.mString;
		return *this;
	}

	bool CStringA::operator!=(const CStringA& r)
	{
		return (mString != r.mString);
	}

	bool CStringA::operator==(const CStringA& r)
	{
		return (mString == r.mString);
	}

	void CStringA::Replace(LPCSTR lpszOld, LPCSTR lpszNew)
	{
		int iFind = mString.find(lpszOld);
		if(-1 == iFind)return;
		int nCount = strlen(lpszOld);
		mString.replace(iFind, nCount, lpszNew, strlen(lpszNew));
	}


   CStringA & CStringA::operator = (const std::string & s)
   {
	   mString = s;
	   return *this;
   }
   CStringA & CStringA::operator = (const char* s)
   {
	   mString = s;
	   return *this;
   }

   void CStringA::Remove(char c)
   {
		remove(mString, c);
   }
   CStringA& CStringA::MakeUpper()
   {
		toupper_str(mString);
		return *this;
   }
   CStringA& CStringA::Trim()
   {
	   std::string& str = mString;
	   std::string::size_type pos = str.find_last_not_of(' '); 
	   if(pos != std::string::npos) 
	   { 
		   str.erase(pos + 1); 
		   pos = str.find_first_not_of(' '); 
		   if(pos != std::string::npos) str.erase(0, pos); 
	   } 
	   else 
		   str.erase(str.begin(), str.end()); 
		return *this;
   }
	
   char& CStringA::operator[](int i)
   {
	   return mString[i];
   }

   LPCSTR CStringA::GetBuffer()
   {
	   return mString.c_str();
   }

   int CStringA::Find(char c)
   {
	   return mString.find(c);
   }

   char CStringA::GetAt(int i)
   {
	   return mString.at(i);
   }

   bool CStringA::IsEmpty()
   {
	   return mString.empty();
   }


   time_t CTime::GetCurrentTime()
	{
		time_t timer;
		timer = time(NULL);
		return timer;
	}

	CTime::CTime(){}
	CTime::~CTime(){}
	CTime::CTime(DWORD dwTime)
	{
		m_timer = dwTime;	
	}
	CTime::CTime(DWORD year, DWORD mon, DWORD day, DWORD hour, DWORD minute, DWORD second)
	{
		struct tm atm;

		atm.tm_sec = second;
		atm.tm_min = minute;
		atm.tm_hour = hour;
		atm.tm_mday = day;
		atm.tm_mon =  mon - 1;        // tm_mon is 0 based
		atm.tm_year = year - 1900;     // tm_year is 1900 based
		atm.tm_isdst = -1;

		m_timer = mktime(&atm);
	}

#define RtnLocalVal(val)	struct tm *tblock;\
							tblock = localtime(&m_timer);\
							if(tblock)\
								return tblock->val;\
							return 0;\

	int CTime::GetYear()
	{
		RtnLocalVal(tm_year+1900)
	}
	int CTime::GetMonth()
	{
		RtnLocalVal(tm_mon+1)
	}
	int CTime::GetDay()
	{
		RtnLocalVal(tm_mday)
	}
	int CTime::GetHour()
	{
		RtnLocalVal(tm_hour)
	}
	int CTime::GetMinute()
	{
		RtnLocalVal(tm_min)
	}
	int CTime::GetSecond()
	{
		RtnLocalVal(tm_sec)
	}
	
	time_t CTime::GetTime()
	{
		return m_timer;
	}

void Msg(const char* log, int val)
{
/*	FILE* f = fopen("/tmp/snlog.txt", "a+");
	if(NULL == f)return;
	char temp[256]={0};
	sprintf(temp, "%s, %d\n", log, val);
	fputs(temp, f);
	fclose(f);
*/
}