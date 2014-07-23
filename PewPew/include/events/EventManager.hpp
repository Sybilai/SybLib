#ifndef SYB_EVENT_MANAGER_HPP
#define SYB_EVENT_MANAGER_HPP

#include "IEventListener.hpp"
#include "IEventFilter.hpp"

#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <map>


namespace syb
{
	// Should it be a singleton? Should it not be? Stay tuned to find out in the next TODO:	
	class EventManager
	{
	public:
		EventManager();
		virtual ~EventManager();

		/// Returns a pointer to some object of some class that inherits from EventManager. Singleton-ish style. 
		// Actually, this is retarded. Instead, a game will register its own event "filter" that will handle
		// incoming/outgoing events
		// sensible sidenote: OH GOD I FEEL SO SMART FOR THIS
		void Init();

		/// Plugs in a game-specific event filter(-er?)
		void RegisterFilter(IEventFilter*);

		/// Called by an event listener's constructor to make itself available for receiving events.
		void RegisterListener(IEventListener*);

		void SendEvent(IEvent*);

		// This assumes that relevant systems will not forget to call this when needed. Otherwise, the mutex could 
		// be locked on demand, whenever a system requests some functionality => sad cpu cycles
		/// Gives the calling thread access 
		void Lock();
		/// Makes it available
		void Unlock();

	protected:
		IEventFilter::ListenerSet_t m_Listeners;
		IEventFilter* m_pGameFilter;
		std::mutex m_Mutex;
	};
} // namespace syb

#endif // SYB_EVENT_MANAGER_HPP