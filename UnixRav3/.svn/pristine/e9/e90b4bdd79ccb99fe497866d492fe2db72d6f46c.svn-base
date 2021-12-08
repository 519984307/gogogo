#ifndef _stdafx_
#define _stdafx_


#include <string>
#include <algorithm>
#include <string.h>
#include <time.h>


void remove(std::string& s, const char c);
void toupper_str(std::string& str);

typedef unsigned char BYTE;
typedef long BOOL;
typedef long LONG;
typedef LONG HRESULT;
typedef char CHAR;
typedef const CHAR * LPCSTR;
typedef CHAR *NPSTR, *LPSTR, *PSTR;
#undef DWORD
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char UCHAR;
typedef void *LPVOID;
typedef int                 INT;
typedef unsigned int        UINT;

#define FALSE 0
#define TRUE  1
//#define NULL  0
#define SUCCEEDED(hr)    (((HRESULT)(hr))>=0)
#define FAILED(hr)       (((HRESULT)(hr))<0)
#define IN
#define OUT
#define RASSERT(x, _h_r_) { if(!(x)) return _h_r_; }
#define RFAILED(x) { HRESULT _h_r_ = (x); if(FAILED(_h_r_)) return _h_r_; }
#define S_OK                                   ((HRESULT)0L)
#define S_FALSE                                ((HRESULT)1L)
#define E_FAIL                                 ((HRESULT)-1L)

//inline void min(a,b)		a<b?a:b

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define linux_mode

class CStringA
{
public:
	std::string mString;
public:
	CStringA();
	CStringA(const char* pstr);
	CStringA(const std::string & s);

	int GetLength();
	std::string Mid(int iPos, int iLength=-1);

	CStringA Right(int count);
	
	CStringA Left(int count);

	CStringA operator+(const CStringA& r);

	CStringA& operator+=(const CStringA& r);
	bool operator!=(const CStringA& r);

	bool operator==(const CStringA& r);

	void Replace(LPCSTR lpszOld, LPCSTR lpszNew);


   CStringA & operator = (const std::string & s);
   CStringA & operator = (const char* s);

   void Remove(char c);
   CStringA& MakeUpper();
   CStringA& Trim();
	
   char& operator[](int i);

   LPCSTR GetBuffer();

   int  Find(char c);
   char GetAt(int i);
   bool IsEmpty();
};

class CTime
{
public:
	static time_t GetCurrentTime();

	CTime();
	~CTime();
	CTime(DWORD dwTime);
	CTime(DWORD year, DWORD mon, DWORD day, DWORD hour, DWORD minute, DWORD second);


	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	
	time_t GetTime();
public:
	time_t m_timer;
};

extern void Msg(const char* log, int val);
extern void Log(const char * fmt, ...);
#define LOG printf

#endif
