#ifndef SYB_TIME_HPP
#define SYB_TIME_HPP


namespace syb
{
	// Keep syb:: nice and clean
	namespace ecospace
	{
		struct PlatformSpecific;	
	}

	/**
	* A cross-platform wrapper for a high-resolution timer.
	*
	* Alternatives:
	*		1. std::clock() is not really hi-res: https://i.imgur.com/zwRIhWK.jpg
	*		2. std::chrono::high_resolution_clock: Microsoft's implementation is faulty as of VS2013.2. It does
	*											approach the resolution it's supposed to, but it's not there yet.	
	*
	* Currently, the available functions are not thread-safe, so in the unlikely situation this thing will be threaded like mad and time 
	* becomes super-important for whatever reason, like: 
	*		1. someone with this mad 24-core setup controls like 10 bots and gives each one its own separate thread that runs like mad and needs
	*		to check the time because Terminators are oh so posh bastards
	*								OR
	*		2. performance measurements are sprinkled here and there, in systems that run on their own thread(e.g. networking/IO and the main loop)
	* [in either case] call and pray for the best. *Late note: Not sure if this is valid or not, but solely reading data concurrently might not
	* be a source of problems.
	*
	* Example usage:
	*		1. syb::Time::Init();
	*		   syb::Time::Clock t1 = syb::Time::GetTime();
	*		   // do lots of cool stuff ...
	*		   double deltaTime = syb::Time::Elapsed(t1); // this is how much the cool stuff took
	*
	*		2. syb::Time::Init();
	*		   syb::Time::Clock t1 = syb::Time::GetTime(); 
	*		   // do lots of cool stuff ...
	*		   syb::Time::Clock t2 = syb::Time::GetTime();
	*		   double deltaTime = syb::Time::Elapsed(t1, t2); // this is how much the cool stuff took
	*
	*		3. using namespace syb;
	*		   Time::Init();
	*		   auto t1 = Time::GetTime();
	*		   // do lots of cool stuff ...
	*		   double deltaTime = Time::Elapsed(t1);
	*/
	class Time final
	{
	public:
		class Clock final
		{
			friend class Time;
			ecospace::PlatformSpecific *m_pPs;

		public:
			Clock();
			~Clock();
			Clock(const Clock& clock);
			Clock& operator = (const Clock& clock);
		};

		/// Call before using GetTime(), otherwise said function will yield undefined results on Windows(most likely division by 0).  
		/// Should only be called from the main thread(where the game loop resides) to avoid multicore issues. 
		static void Init();

		/// Used to avoid any errors whenever the processor frequency changes(like with Intel's TurboBoost thingamagig or power-saving mode on a laptop)
		/// Should only be called from the main thread(where the game loop resides) to avoid multicore issues. 
		/// TODO: Figure out how to figure out when that actually happens and call this automatically-ish. Otherwise, call this on every frame in the main loop.
		static void Reset();

		/// Returns the time that has elapsed since startup in a Clock structure that can be later used with Elapsed(). 
		static Clock GetTime();

		/// Returns the time that has elapsed since startup in a Clock structure provided by @clock that can be later used with Elapsed(). 
		static void GetTime(Clock& clock);
		
		enum Precision
		{
			MIN,
			SEC,
			MILISEC,
			MICROSEC,
			NANOSEC
		};

		/// Returns the time that has elapsed since @start was retrieved with GetTime() in seconds[64-bit double precision float(double)]. 
		/// Consider casting to 32-bit single precision(float) 
		static double Elapsed(const Clock& start/*, const Precision&*/);

		/// Returns the time that has elapsed between the moments @start and @end were retrieved in seconds[64-bit double precision float(double)].
		/// Consider casting to 32-bit single precision(float)
		static double Elapsed(const Clock& start, const Clock& end/*, const Precision&*/);

		///	Takes time @source in precision @from and returns its corresponding integer number in precision @to
		//template <typename T> 
		//static int ToPrecision(const T& source, const Precision& from, const Precision& to);

		/// Takes time @source in seconds and returns its corresponding integer number in the specified Time::Precision
		/// Getting the time elapsed between two points in integer format becomes something like:
		/// auto start = Time::GetTime();
		/// ...stuff...
		/// int elapsed_ms = Time::FromSecTo(Time::Elapsed(start), Time::Precision::MILISEC);
		template <typename T>
		static unsigned int FromSecTo(const T& source, const Precision&);

		// TODO: Consider upgrading commenting skill
		// Consider possiblity of considering the usage of a skill point for said skill on next level-up

	private:
		Time();
	};
} // namespace syb

#endif //SYB_TIME_HPP