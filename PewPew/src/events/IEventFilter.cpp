#include "../../include/events/IEventFilter.hpp"


namespace syb
{
	IEventFilter::IEventFilter(ListenerSet_t *listeners) :
		m_pListeners(listeners)
	{ }

	IEventFilter::~IEventFilter()
	{ }
} // namespace syb