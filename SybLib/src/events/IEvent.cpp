#include "../../include/events/IEvent.hpp"


namespace syb
{
	IEvent::IEvent(EventType_t type) :
		m_Type(type)
	{ }

	IEvent::~IEvent()
	{ }

	const IEvent::EventType_t& IEvent::Type() const
	{
		return m_Type;
	}
} // namespace syb