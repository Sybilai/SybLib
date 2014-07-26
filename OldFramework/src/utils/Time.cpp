#include "../../include/utils/Time.hpp"

#include <cmath>
#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <time.h>
#else
#include <chrono>
#endif


namespace syb
{
	// --------------------------------------------------------------------
	// ecospace::PlatformSpecific
	// --------------------------------------------------------------------
	namespace ecospace
	{
		// --------------------------------------------------------------------
		// Holds platform specific time points to be used in calculating the time elapsed between two such points at runtime.
		// --------------------------------------------------------------------
		struct PlatformSpecific
		{
			PlatformSpecific() : time_point{}
			{ }
	#ifdef _WIN32
			LARGE_INTEGER time_point;
	#elif defined(__linux__)
			timespec time_point;
	#else
			std::chrono::high_resolution_clock::time_point time_point;
	#endif
		};
	}

	// --------------------------------------------------------------------
	// Time::Clock
	// --------------------------------------------------------------------
	Time::Clock::Clock() //: m_pPs(nullptr)
	{ 
		m_pPs = new ecospace::PlatformSpecific;
	}

	Time::Clock::~Clock()
	{
		m_pPs = nullptr;
	}

	Time::Clock::Clock(const Clock& clock)
	{
		m_pPs = new ecospace::PlatformSpecific;
		*m_pPs = *clock.m_pPs;
	}

	Time::Clock& Time::Clock::operator = (const Clock& clock)
	{
		if (this != &clock)
		{
			delete m_pPs;
			m_pPs = new ecospace::PlatformSpecific;
			*m_pPs = *clock.m_pPs;
		}
		return *this;
	}

	// --------------------------------------------------------------------
	// Time
	// --------------------------------------------------------------------
#ifdef _WIN32
	static LARGE_INTEGER frequency{};
#endif
	//static int precision_table[5][5];
	static const unsigned int precision_precalc[5] { 1 / 60, 1, std::pow(10, 3), std::pow(10, 6), std::pow(10, 9) };

	// --------------------------------------------------------------------
	void Time::Init()
	{
#ifdef _WIN32
		QueryPerformanceFrequency(&frequency);
#endif
		//// from \ to
		////
		////		    min        | sec  | ms      | micros  | ns
		//// min      1          | 60   | 60*10^3 | 60*10^6 | 60*10^9 
		//// sec      1/60       | 1    | 10^3    | 10^6    | 10^9
		//// ms       1/60*10^-3 | 10^3 | 1       | 10^3    | 10^6
		//// micros   1/60*10^-6 | 10^6 | 10^-3   | 1       | 10^3
		//// ns       1/60*10^-9 | 10^9 | 10^-6   | 10^-3   | 1
		//
		//int negpow10[10];
		//neg10[1]

		//precision_table[Precision::MIN] = {1, 60, };
		//precision_table[Precision::SEC] = {};
		//precision_table[Precision::MILISEC] = {};
		//precision_table[Precision::MICROSEC] = {};
		//precision_table[Precision::NANOSEC] = {};
	}

	// --------------------------------------------------------------------
	void Time::Reset()
	{
#ifdef _WIN32
		QueryPerformanceFrequency(&frequency);
#endif
	}

	// --------------------------------------------------------------------
	// Uses functionality provided by WinApi for Windows(QPC() & QPF()),
	// clock_gettime() for Linux,
	// and the standard high_resolution_clock otherwise.
	// --------------------------------------------------------------------
	Time::Clock Time::GetTime()
	{
		Clock clock;
#ifdef _WIN32
		QueryPerformanceCounter(&clock.m_pPs->time_point);
#elif defined(__linux__)
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock.m_pPs->time_point);
#else
		clock.m_pPs->time_point = std::chrono::high_resolution_clock::now();
#endif
		return clock;
	}

	// --------------------------------------------------------------------
	// Reusing this in the previous Now() definition would imply and additional call, in exchange for 5 lines of economy.
	// --------------------------------------------------------------------
	void Time::GetTime(Clock& clock)
	{
#ifdef _WIN32
		QueryPerformanceCounter(&clock.m_pPs->time_point);
#elif defined(__linux__)
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock.m_pPs->time_point);
#else
		clock.m_pPs->time_point = std::chrono::high_resolution_clock::now();
#endif
	}

	// --------------------------------------------------------------------
	double Time::Elapsed(const Clock& start)
	{
		Clock end;
		GetTime(end);

#ifdef _WIN32
		return static_cast<double>(end.m_pPs->time_point.QuadPart - start.m_pPs->time_point.QuadPart) / frequency.QuadPart;
#elif defined(__linux__)
		// TODO: look into sec/nanosec formatting
		return end.m_pPs->time_point.tv_sec - start.m_pPs->time_point.tv_sec;
		//return end.m_pPs->time_point.tv_nsec - start.m_pPs->time_point.tv_nsec;
#else
		return duration_cast<duration<double>>(end.m_pPs->time_point - start.m_pPs->time_point).count();
#endif
	}

	// --------------------------------------------------------------------
	// Same as in GetTime(Clock& clock)
	// --------------------------------------------------------------------
	double Time::Elapsed(const Clock& start, const Clock& end)
	{
#ifdef _WIN32
		return static_cast<double>(end.m_pPs->time_point.QuadPart - start.m_pPs->time_point.QuadPart) / frequency.QuadPart;
#elif defined(__linux__)
		// TODO: look into sec/nanosec formatting
		return end.m_pPs->time_point.tv_sec - start.m_pPs->time_point.tv_sec;
		//return end.m_pPs->time_point.tv_nsec - start.m_pPs->time_point.tv_nsec;
#else
		return duration_cast<duration<double>>(end.m_pPs->time_point - start.m_pPs->time_point).count();
#endif
	}

	// --------------------------------------------------------------------
	//template <typename T>
	//static int Time::ToPrecision(const T& source, const Precision& from, const Precision& to)
	//{
	//}

	//template int Time::ToPrecision/*<const int&, const Time::Precision&, const Time::Precision&>*/(const int&, const Time::Precision&, const Time::Precision&);
	//template int Time::ToPrecision(const float&, const Time::Precision&, const Time::Precision&);
	//template int Time::ToPrecision(const double&, const Time::Precision&, const Time::Precision&);

	// --------------------------------------------------------------------
	template <typename T>
	static unsigned int Time::FromSecTo(const T& source, const Precision& precision)
	{
		return static_cast<unsigned int>(source * precision_precalc[precision]);
	}

	template unsigned int Time::FromSecTo(const int&, const Time::Precision&);
	template unsigned int Time::FromSecTo(const float&, const Time::Precision&);
	template unsigned int Time::FromSecTo(const double&, const Time::Precision&);
} // namespace syb