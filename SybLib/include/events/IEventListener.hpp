#ifndef SYB_IEVENT_LISTENER_HPP
#define SYB_IEVENT_LISTENER_HPP

#include "IEvent.hpp"
#include <memory>
#include <string>


namespace syb
{
	class EventManager;

	class IEventListener
	{
	public:
		//using ListenerType_t = unsigned int;
		// Slower lookups, but should avoid too many clashes.
		using ListenerType_t = std::string;

		IEventListener(ListenerType_t, const std::shared_ptr<EventManager>&);
		virtual ~IEventListener();
		const ListenerType_t& Type() const;

		virtual void ReceiveEvent(IEvent*) = 0;

	protected:
		// Different types of listeners should handle receiving in their own spcial, unqiue way, like a snowflake. 
		//std::queue<IEvent*> m_Events;
		ListenerType_t m_Type;
		std::shared_ptr<EventManager> m_pEventManager;
	};
} // namespace syb

#endif // SYB_IEVENT_LISTENER_HPP