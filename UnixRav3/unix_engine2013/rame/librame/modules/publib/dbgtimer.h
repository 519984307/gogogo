#ifndef _RS_ENGINE_DEBUG_TIMER_H_
#define _RS_ENGINE_DEBUG_TIMER_H_


//////////////////////////
// #define DEBUG_TIMER		//
// #define DEBUG_COUNTER	//
//////////////////////////



#ifndef IS_COMPILER_VC6
	#ifdef _MSC_VER
		#if(_MSC_VER < 1300)
			#define IS_COMPILER_VC6 1
		#else
			#define IS_COMPILER_VC6 0
		#endif
	#else
		#define IS_COMPILER_VC6 0
	#endif
#endif



#include <stdio.h>
#include <string>


#ifdef DEBUG_TIMER


	//##### debug_timer ############################################################
	#if defined(PLATFORM_TYPE_WIN32)
		extern "C" WINBASEAPI VOID WINAPI Sleep ( IN DWORD dwMilliseconds );
	#elif defined(PLATFORM_TYPE_NATIVE)
		VOID NTAPI BsSysSleep ( IN ULONG dwMilliseconds );
	#else // linux
		//extern "C" void usleep(unsigned long dwMicroseconds);
		extern "C" int usleep(unsigned int dwMicroseconds);
	#endif


	class debug_timer
	{
	public:
			typedef	ULONGLONG	value_type;

	public:
		debug_timer() : m_count(0) {}
		void Reset(value_type init_val = 0) { m_count = init_val; }

		operator value_type () const { return m_count; }
		value_type GetCount() const { return *this; }

		void Ready() { m_last = rdtsc(); }
		void TickCount() { m_count += rdtsc() -m_last; }

	public:
		static value_type rdtsc();

		static void Sleep(unsigned int dwMilliseconds)
		{
		#if defined(PLATFORM_TYPE_WIN32)
			::Sleep(dwMilliseconds);
		#elif defined(PLATFORM_TYPE_NATIVE)
			::BsSysSleep(dwMilliseconds);
		#else // linux
			::usleep(dwMilliseconds*1000); // turn to microseconds
		#endif
		}

		static value_type GetCPUClockFrequency()
		{
			static value_type frequency = calc_cpu_clock_frequency();
			return frequency;
		}

	private:
		static value_type calc_cpu_clock_frequency()
		{
			Sleep(1000); // first call discarded
			value_type last = rdtsc();
			Sleep(1000);
			return rdtsc() -last;
		}

	private:
		value_type m_count;
		value_type m_last;
	};


	// '__declspec(naked)' can not be inlined
	#if(IS_COMPILER_VC6)
		#pragma warning(disable : 4035) // warning C4035: 'rdtsc' : no return value
	#endif
	inline debug_timer::value_type debug_timer::rdtsc()
	{
	#ifdef WIN32
      #if !defined(PLATFORM_TYPE_NATIVE)
		LARGE_INTEGER liPerformanceCount;
		QueryPerformanceCounter(&liPerformanceCount);
		return liPerformanceCount.QuadPart;
// 		__asm rdtsc ;// multi cpu/core problem
      #else
		return 0;
      #endif
	#else // linux
		__asm__("rdtsc");
	#endif
	}
	#if(IS_COMPILER_VC6)
		#pragma warning(default : 4035)
	#endif
	//############################################################ debug_timer #####


	class CDbgTimer : public debug_timer
	{
		std::string m_fmt;
		value_type m_coef;
	public:
		CDbgTimer(const char *format, value_type coefficient = 0) : m_fmt(format), m_coef(coefficient) {}
		~CDbgTimer()
		{
			if(FILE *fp = fopen("c:\\dbgtimer.log", "a+"))
			{
				char buf[1024];
				value_type coefficient = m_coef ? m_coef : GetCPUClockFrequency()/1000;
				value_type count = (GetCount() +coefficient/2) /coefficient; /* rounded */
				fwrite(buf, 1, sprintf(buf, m_fmt.c_str(), count), fp);
				fclose(fp);
			}
		}
	};


	#define DECLARE_TIMER( timer )		CDbgTimer timer
	#define EXTERN_TIMER( timer )		extern CDbgTimer timer
	#define TIMER_READY( timer )		(timer).Ready()
	#define TIMER_TICKCOUNT( timer )	(timer).TickCount()
	#define TIMER_ADDRESS( timer )		&(timer)


#else // #ifdef DEBUG_TIMER


	class CDbgTimer
	{
	public:
		typedef	ULONGLONG	value_type;

	public:
		CDbgTimer(const char*, value_type = 0) {}
		void Reset(value_type = 0) {}

		void Ready() {}
		void TickCount() {}

		operator value_type () const { return 0; }
		value_type GetCount() const { return 0; }

	public:
		static value_type rdtsc() { return 0; }
		static void Sleep(unsigned int) {}
		static value_type GetCPUClockFrequency() { return 0; }
	};


	#define DECLARE_TIMER(timer) 
	#define EXTERN_TIMER( timer )
	#define TIMER_READY(timer) 
	#define TIMER_TICKCOUNT(timer) 


#endif // #ifdef DEBUG_TIMER



class CDbgScopeTime
{
	CDbgTimer& m_tmr;
public:
	CDbgScopeTime(CDbgTimer& tmr) : m_tmr(tmr) { m_tmr.Ready(); }
	~CDbgScopeTime() { m_tmr.TickCount(); }
private:
	CDbgScopeTime & operator = (const CDbgScopeTime &);
};

#ifdef DEBUG_TIMER
	#define SCOPE_TIMER( timer )	CDbgScopeTime s(timer)
#else // #ifdef DEBUG_TIMER
	#define SCOPE_TIMER( timer )
#endif // #ifdef DEBUG_TIMER



#ifdef DEBUG_COUNTER


	//##### debug_counter ##########################################################
	class debug_counter
	{
	public:
			typedef	ULONGLONG	value_type;

	public:
		debug_counter() : m_count(0) {}
		void Reset(value_type init_val = 0) { m_count = init_val; }

		operator value_type () const { return m_count; }
		value_type GetCount() const { return *this; }

	public:
		debug_counter& operator ++ () { ++m_count; return *this; }
		debug_counter& operator -- () { --m_count; return *this; }
		debug_counter  operator ++ (int) { debug_counter tmp(*this); ++*this; return tmp; }
		debug_counter  operator -- (int) { debug_counter tmp(*this); --*this; return tmp; }
		debug_counter& operator += (value_type delta) { m_count += delta; return *this; }
		debug_counter& operator -= (value_type delta) { m_count -= delta; return *this; }

	private:
		value_type m_count;
	};
	//########################################################## debug_counter #####


	class CDbgCounter : public debug_counter
	{
		std::string m_fmt;
		value_type m_coef;
	public:
		CDbgCounter(const char *format, value_type coefficient = 1) : m_fmt(format), m_coef(coefficient) {}
		~CDbgCounter()
		{
			if(FILE *fp = fopen("c:\\dbgtimer.log", "a+"))
			{
				char buf[1024];
				value_type coefficient = m_coef ? m_coef : 1;
				value_type count = (GetCount() +coefficient/2) /coefficient; /* rounded */
				fwrite(buf, 1, sprintf(buf, m_fmt.c_str(), count), fp);
				fclose(fp);
			}
		}
	};


	#define DECLARE_COUNTER( counter )			CDbgCounter counter
	#define EXTERN_COUNTER( counter )			extern CDbgCounter counter
	#define COUNTER_INCREASE( counter )			(counter)++
	#define COUNTER_INCREASE_( counter, size )	(counter) += (size)


#else // #ifdef DEBUG_COUNTER


	class CDbgCounter
	{
	public:
		typedef	ULONGLONG	value_type;

	public:
		CDbgCounter(const char*, value_type = 1) {}
		void Reset(value_type = 0) {}

		operator value_type () const { return 0; }
		value_type GetCount() const { return 0; }

	public:
		CDbgCounter& operator ++ () { return *this; }
		CDbgCounter& operator -- () { return *this; }
		CDbgCounter  operator ++ (int) { return *this; }
		CDbgCounter  operator -- (int) { return *this; }
		CDbgCounter& operator += (value_type) { return *this; }
		CDbgCounter& operator -= (value_type) { return *this; }
	};


	#define DECLARE_COUNTER( counter ) 
	#define EXTERN_COUNTER( counter )
	#define COUNTER_INCREASE( counter ) 
	#define COUNTER_INCREASE_( counter, size )


#endif // #ifdef DEBUG_COUNTER



#endif // #ifndef _RS_ENGINE_DEBUG_TIMER_H_
