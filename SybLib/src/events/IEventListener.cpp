#include "../../include/events/IEventListener.hpp"
#include "../../include/events/EventManager.hpp"


namespace syb
{

	IEventListener::IEventListener(ListenerType_t type, const std::shared_ptr<EventManager>& event_manager) :
		m_Type(type),
		m_pEventManager(event_manager)
	{
		m_pEventManager->RegisterListener(this);
	}

	IEventListener::~IEventListener()
	{ }

	const IEventListener::ListenerType_t& IEventListener::Type() const
	{ 
		return m_Type;
	}
} // namespace syb